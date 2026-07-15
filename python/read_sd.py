import serial
import time

PORT = "/dev/cu.usbmodem1101"  # deinen Port anpassen
BAUD = 115200

with serial.Serial(PORT, BAUD, timeout=2) as ser:
    time.sleep(2)  # Arduino reset abwarten

    # Dateiliste
    ser.write(b"list\n")
    files = []
    while True:
        line = ser.readline().decode().strip()
        if line == "EOF":
            break
        if line:
            files.append(line)
    print(f"Files: {files}")

    # jede Datei herunterladen
    for filename in files:
        ser.write(f"get:{filename}\n".encode())

        size_line = ser.readline().decode().strip()
        expected = int(size_line.replace("SIZE:", ""))
        print(f"Downloading {filename} ({expected} bytes)...")

        start = time.time()
        data = bytearray()
        while len(data) < expected:
            chunk = ser.read(min(512, expected - len(data)))
            if not chunk:
                print("  ...no data received in last 2s, still waiting")
                continue
            data.extend(chunk)
            elapsed = time.time() - start
            rate = len(data) / elapsed if elapsed > 0 else 0
            print(f"\r  {len(data)}/{expected} bytes ({rate:.0f} B/s)", end="", flush=True)
        print()

        # trailing "\nEOF\r\n" marker sent after the file content
        ser.read_until(b"\nEOF\r\n")

        with open(filename, "wb") as f:
            f.write(data)

        elapsed = time.time() - start
        rate = len(data) / elapsed if elapsed > 0 else 0
        print(f"Saved {filename} — {len(data)} bytes in {elapsed:.2f}s ({rate:.0f} B/s)")

        ser.write(f"delete:{filename}\n".encode())
        reply = ser.readline().decode().strip()
        if reply == "DELETED":
            print(f"Deleted {filename} from SD card")
        else:
            print(f"WARNING: could not delete {filename} from SD card ({reply!r})")
