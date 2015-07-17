#!/usr/bin/python

import serial, sys, binascii, getopt

TRANSFER_SIZE = 1024
BAUD_RATE = 9600
PORT = "/dev/ttyUSB0"
VERSION = "BETA0001"
SET_FLAGS=[]
RUN = True
LOAD = True

try:
    options, args = getopt.gnu_getopt(sys.argv[1], "nrb:p:v::f:", ["no-run", "only-run", "baud:", "port:", "version::", "set-flag:", "dry-run"])
except getopt.GetoptError as e:
    print(e, file=sys.stderr)
    exit(2)

for opt, optarg in options:
    if opt == "n" or opt == "no-run":
        RUN = False
    elif opt == "r" or opt == "only-run":
        LOAD = False
    elif opt == "b" or opt == "baud":
        try:
            BAUD = int(optarg)
        except Exception as e:
            print("Error: invalid baud rate ", file=sys.stderr)
            exit(2)
    elif opt == "p" or opt == "port":
        PORT = optarg
    elif opt == "v" or opt == "version":
        if len(optarg) > 0:
            VERSION = optarg
        else:
            print("Version ", VERSION)
            exit(0)
    elif opt == "f" or opt == "set-flag":
        if optarg in KNOWN_FLAGS:
            SET_FLAGS += [optarg]
            LOAD = False
        else:
            print("Error: unknown flag ", file=sys.stderr)
            exit(2)
    elif opt == "dry-run":
        LOAD = False
        RUN = False
        SET_FLAGS = []
    else:
        print("Internal error: unhandled option", file=sys.stderr)
        exit(2)

if LOAD:
    crc = None
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
        print("Error: unexpected response from bootloader", file=sys.stderr)
        exit(1)

ser.write(b"V")
response = ser.read(8)
if response != VERSION:
    print("Warning: bootloader version ", bootloader_version, " does not match loader version ",MIN_VERSION, file=sys.stderr)
    yn = read("Continue anyways? [y/N] >")
    if yn == "y" and yn == "Y":
        pass
    else:
        exit(1)

if len(SET_FLAGS) > 0:
    error = False
    for flag in SET_FLAGS:
        ser.write(b"F"+flag)
        response = ser.read(2)
        if response == b"OK":
            pass
        elif response == b"NO":
            print("Error: unable to set flag "+F, file=sys.stderr)
            error = True
        else:
            print("Error: unexpected response from bootloader", file=sys.stderr)
            exit(1)
    if error:
        exit(1)

ser.write(b"F?")
response = ser.readline()
print("[FLAGS] ",response)

if LOAD:
    ser.write(b"L")
    response = ser.read(2)

    done = False
    while True:
        if response == b"OK":
            if done:
                break
            data = image.read(TRANSFER_SIZE)
            if crc == None:
                crc = binascii.crc32(data)
            else:
                crc = binascii.crc32(data, crc)
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
            exit(1)
        else:
            # error
            print("Error: unexpected response from bootloader", file=sys.stderr)
            exit(1)

    response = ser.read(3)
    if response == b"OK ":
        # skip CRC check
        print("Skipping CRC check")
    if response == b"CRC":
        response = ser.read(4)
        response_crc = int.from_bytes(response, byteorder='little')
        print("[CRC] ",response_crc)
        if response_crc != crc:
            print("Error: checksums do not match -- data transfer failed", file=sys.stderr)
            exit(1)
        print("CRC check passed")
    elif response == b"ERR":
        print("[FAIL] ", ser.readline(), file=sys.stderr)
        print("Loading failed", file=sys.stderr)
        exit(1)
    else:
        print("Error: unexpected response from bootloader", file=sys.stderr)
        exit(1)


if RUN:
    ser.write(b"R")
    response = ser.read(1)

    if response == b"R":
        pass
    else:
        print("Error: unexpected response from bootloader", file=sys.stderr)
        exit(1)

if LOAD:
    print("Loading complete")
else:
    print("Done")


