import serial
import time

RECORD_DURATION_S   = 10
SERIAL_PORT         = "/dev/cu.usbmodem2101"
BAUD_RATE           = 115200

s = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
time.sleep(2)

data = []
start = time.time()
print("Start record...")

while time.time() - start < RECORD_DURATION_S:
    row = s.readline().decode("utf-8").strip()
    if row:
        data.append(row)

s.close()

with open("record.csv", 'w') as f:
    f.write("\n".join(data))
    
print("Done reading data from stream")
print("Wrote data tp record.csv")