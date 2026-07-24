import re
import serial
import time

PORT = "/dev/cu.usbmodem1101"  # deinen Port anpassen
BAUD = 115200
MAX_ATTEMPTS = 10
MAX_FILE_ATTEMPTS = 3
STALL_TIMEOUT = 10  # seconds with no bytes arriving before we give up on a file

EOF_RE = re.compile(rb"EOF:(\d+)")


class DownloadError(Exception):
    pass


def download_file(ser, filename):
    ser.write(f"get:{filename}\n".encode())

    size_line = ser.readline().decode(errors="replace").strip()
    if not size_line.startswith("SIZE:"):
        raise DownloadError(f"expected SIZE:, got {size_line!r}")
    expected = int(size_line.replace("SIZE:", ""))
    print(f"Downloading {filename} ({expected} bytes)...")

    start = time.time()
    last_progress = start
    data = bytearray()
    while len(data) < expected:
        chunk = ser.read(min(512, expected - len(data)))
        if not chunk:
            if time.time() - last_progress > STALL_TIMEOUT:
                raise DownloadError(
                    f"stalled after {len(data)}/{expected} bytes (no data for {STALL_TIMEOUT}s)"
                )
            continue
        data.extend(chunk)
        last_progress = time.time()
        elapsed = time.time() - start
        rate = len(data) / elapsed if elapsed > 0 else 0
        print(f"\r  {len(data)}/{expected} bytes ({rate:.0f} B/s)", end="", flush=True)
    print()

    # trailing "\nEOF:<sent>\r\n" marker sent after the file content
    trailer = ser.read_until(b"\r\n")
    match = EOF_RE.search(trailer)
    if not match:
        raise DownloadError(f"missing/garbled EOF marker (got {trailer!r})")
    sent = int(match.group(1))
    if sent != expected:
        raise DownloadError(f"device only sent {sent}/{expected} bytes")

    with open(filename, "wb") as f:
        f.write(data)

    elapsed = time.time() - start
    rate = len(data) / elapsed if elapsed > 0 else 0
    print(f"Saved {filename} — {len(data)} bytes in {elapsed:.2f}s ({rate:.0f} B/s)")


def download_all():
    with serial.Serial(PORT, BAUD, timeout=2) as ser:
        time.sleep(2)  # Arduino reset abwarten

        # Dateiliste
        ser.write(b"list\n")
        files = []
        list_start = time.time()
        while True:
            line = ser.readline().decode(errors="replace").strip()
            if line == "EOF":
                break
            if line:
                files.append(line)
                list_start = time.time()
            elif time.time() - list_start > STALL_TIMEOUT:
                raise DownloadError(
                    f"device never sent EOF for 'list' (got {len(files)} file(s) so far)"
                )
        print(f"Files: {files}")

        # jede Datei herunterladen
        for filename in files:
            for file_attempt in range(1, MAX_FILE_ATTEMPTS + 1):
                try:
                    download_file(ser, filename)
                    break
                except DownloadError as e:
                    print(f"\n  {filename}: {e} (attempt {file_attempt}/{MAX_FILE_ATTEMPTS})")
                    ser.reset_input_buffer()
            else:
                print(f"  Giving up on {filename} — leaving it on the SD card.")
                continue

            ser.write(f"delete:{filename}\n".encode())
            reply = ser.readline().decode().strip()
            if reply == "DELETED":
                print(f"Deleted {filename} from SD card")
            else:
                print(f"WARNING: could not delete {filename} from SD card ({reply!r})")


# The device only deletes a file after it has been fully saved locally, so a
# dropped connection (or a failed/verified-bad transfer) can't lose data —
# re-running download_all() from a fresh port just re-lists the remaining
# files and picks up where it left off.
for attempt in range(1, MAX_ATTEMPTS + 1):
    try:
        download_all()
        break
    except serial.SerialException as e:
        print(f"\nSerial connection dropped ({e}); reconnecting (attempt {attempt}/{MAX_ATTEMPTS})...")
        time.sleep(2)
    except DownloadError as e:
        print(f"\nProtocol error ({e}); reconnecting (attempt {attempt}/{MAX_ATTEMPTS})...")
        time.sleep(2)
else:
    print(f"Giving up after {MAX_ATTEMPTS} attempts.")
