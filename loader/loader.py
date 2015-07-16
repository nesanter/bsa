#!/usr/bin/python

import serial, sys

TRANSFER_SIZE = 1024
BAUD_RATE = 9600
PORT = "/dev/ttyUSB0"

args = []

if len(sys.argv) > 1 and sys.argv[1] == "-v":
    verbose = True
    args = sys.argv[2:]
else:
    verbose = False
    args = sys.argv[1:]

if len(args) == 0:
    print("Error: no input file", file=sys.stderr)
    exit(1)
else:
    try:
        image = open(args[0], "rb")
    except Exception:
        print("Error: cannot open input file", file=sys.stderr)
        exit(1)

try:
    ser = serial.Serial(PORT, baudrate=BAUD_RATE, timeout=1)
except Exception:
    print("Error: cannot open serial port", file=sys.stderr)
    exit(1)

ser.open()


while True:
    ser.write(b">")
    response = ser.read(1)
    if response == b">":
        # bootloader is ready to accept commands
        break
    elif response == b"m":
        # bootloader has a message to print
        print("[INFO] ",ser.readline())
    else:
        # error
        print("Error: unexpected response from bootloader")
        exit(1)

ser.write(b"L")
response = ser.read(2)

done = False
while True:
    if response == b"OK":
        if done:
            break
        data = image.read(TRANSFER_SIZE)
        ser.write(bytes([data.length // 256, data.length % 256]))
        ser.write(data)
        response = ser.read(2)
        if len(data) < TRANSFER_SIZE:
            done = True
    elif response == b"mi":
        # bootloader has a message to print
        print("[INFO] ",ser.readline())
    elif response == b"me":
        # bootloader has an error message to print
        print("[ERROR] ",ser.readline(), file=sys.stderr)
    elif response == b"NO":
        print("Loading failed", file=sys.stderr)
    else:
        # error
        print("Error: unexpected response from bootloader", file=sys.stderr)
        exit(1)

response = ser.read(3)
if response == b"CRC":
    response = ser.read(4)
    # theoretically, we would check the CRC here
    print("[CRC] ",response)
elif response == b"ERR":
    print("[FAIL] ", ser.readline(), file=sys.stderr)
    print("Loading failed", file=sys.stderr)
    exit(1)
else:
    print("Error: unexpected response from bootloader")
    exit(1)

ser.write(b"R")
response = ser.read(1)

if response == b"R":
    print("Loading complete")
else:
    print("Error: unexpected response from bootloader")
    exit(1)
