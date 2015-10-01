#!/usr/bin/python

MIN_VERSION = 2

import serial, sys, argparse, time, math

class Image:
    
    def __init__(self, image_file_name):
        self.image_file = open(image_file_name, "rb")
        self.read_elf_header()

    def dump(self):
        print("EHDR {")
        print("\tverified   \t\t" + str(self.verify_elf_header()))
        print("\te_ident    \t\t" + str(self.e_ident))
        print("\te_type     \t\t" + str(self.e_type))
        print("\te_machine  \t\t" + str(self.e_machine))
        print("\te_version  \t\t" + str(self.e_version))
        print("\te_entry    \t\t" + str(self.e_entry))
        print("\te_phoff    \t\t" + str(self.e_phoff))
        print("\te_shoff    \t\t" + str(self.e_shoff))
        print("\te_flags    \t\t" + str(self.e_flags))
        print("\te_ehsize   \t\t" + str(self.e_ehsize))
        print("\te_phentsize\t\t" + str(self.e_phentsize))
        print("\te_phnum    \t\t" + str(self.e_phnum))
        print("\te_shentsize\t\t" + str(self.e_shentsize))
        print("\te_shnum    \t\t" + str(self.e_shnum))
        print("\te_shstridx \t\t" + str(self.e_shstridx))
        print("}")

        for n in range(self.e_phnum):
            self.read_prog_header(n)
            print("PHDR " + str(n) + " {")
            print("\tp_type  \t\t" + str(self.p_type))
            print("\tp_offset\t\t" + str(self.p_offset))
            print("\tp_vaddr \t\t" + str(self.p_vaddr))
            print("\tp_paddr \t\t" + str(self.p_paddr))
            print("\tp_filesz\t\t" + str(self.p_filesz))
            print("\tp_memsz \t\t" + str(self.p_memsz))
            print("\tp_flags \t\t" + str(self.p_flags))
            print("\tp_align \t\t" + str(self.p_align))
            print("}")

    def read_elf_header(self):
        self.seek(0)
        self.e_ident = self.read_bytes(16)
        self.e_type = self.read_u16()
        self.e_machine = self.read_u16()
        self.e_version = self.read_u32()
        self.e_entry = self.read_u32()
        self.e_phoff = self.read_u32()
        self.e_shoff = self.read_u32()
        self.e_flags = self.read_u32()
        self.e_ehsize = self.read_u16()
        self.e_phentsize = self.read_u16()
        self.e_phnum = self.read_u16()
        self.e_shentsize = self.read_u16()
        self.e_shnum = self.read_u16()
        self.e_shstridx = self.read_u16()

    def verify_elf_header(self):
        if self.e_ident[0:4] != b"\x7FELF":
            return False
        if self.e_ident[4:7] != b"\x01\x01\x01":
            return False
        if self.e_type != 2:
            return False
        if self.e_machine != 8:
            return False
        if self.e_version != 1:
            return False
        #if self.e_flags != 0:
        #    return False
        return True

    def read_prog_header(self, n):
        if n >= self.e_phnum:
            return False
        self.seek(self.e_phoff + (n * self.e_phentsize))
        self.p_type = self.read_u32()
        self.p_offset = self.read_u32()
        self.p_vaddr = self.read_u32()
        self.p_paddr = self.read_u32()
        self.p_filesz = self.read_u32()
        self.p_memsz = self.read_u32()
        self.p_flags = self.read_u32()
        self.p_align = self.read_u32()

    def seek(self, n):
        self.image_file.seek(n)

    def read_bytes(self, n):
        return self.image_file.read(n)

    def read_u16(self):
        return int.from_bytes(self.image_file.read(2), byteorder=sys.byteorder, signed=False)

    def read_s16(self):
        return int.from_bytes(self.image_file.read(2), byteorder=sys.byteorder, signed=True)

    def read_u32(self):
        return int.from_bytes(self.image_file.read(4), byteorder=sys.byteorder, signed=False)

    def read_s32(self):
        return int.from_bytes(self.image_file.read(4), byteorder=sys.byteorder, signed=True)


class Loader:

    def __init__(self, image, port_name, preamble_baud, verbose=False):
        self.image = image

        if verbose:
            print("Parsing ELF image")
        if not image.verify_elf_header():
            print("Error parsing image", file=sys.stderr)
            exit(1)

        if verbose:
            print("Opening port")
        self.port_name = port_name
        try:
            self.port = serial.Serial(port_name, baudrate=preamble_baud, timeout=1)
        except Exception:
            print("Cannot open serial port", file=sys.stderr)
            exit(1)


    def preamble(self, new_baud, min_version, verbose=False):
        port = self.port
        if verbose:
            print("Beginning preamble")
        # opening message
        port.write(b"BSA PREAMBLE")
        response = port.read(2) # should get OK back
        if response != b"OK":
            print("Error in preamble (0)", file=sys.stderr)
            exit(1)
        
        if verbose:
            print("Version check...")
        # version check
        port.write(b"VER?")
        response = int.from_bytes(port.read(4), byteorder='big', signed=False)
        if response < min_version:
            print("Incompatible bootloader version (" + str(response) + " < " + str(min_version), file=sys.stderr)
            exit(1)

        port.write(b"OK")
        if verbose:
            print("passed")

        if verbose:
            print("Communicating load baud")

        # set post-preamble baud
        port.write(b"BAUD")
        port.write(int.to_bytes(new_baud, 4, byteorder='little', signed=False))
        response = port.read(2) # should get OK back
        if response != b"OK":
            print("Error in preamble (1)", file=sys.stderr)
            exit(1)


        if verbose:
            print("Requesting preferred block size...")

        # inquire for block size
        port.write(b"BSZ?")
        self.block_size = int.from_bytes(port.read(4), byteorder='big', signed=False)
        port.write(b"OK")
        
        if verbose:
            print(self.block_size)

        # end preamble
        port.write(b"DONE")
        response = port.read(2) # should get OK back
        if response != b"OK":
            print("Error in preamble (2)", file=sys.stderr)
            exit(1)

        if verbose:
            print("Ended preamble, reopening port")

        # re-open port
#        port.close()
        self.port = serial.Serial(self.port_name, baudrate=new_baud, timeout=30)
        port = self.port

        # wait for ready
        response = port.read(16)
        if verbose:
            print(response)
        if response != b"BOOTLOADER READY":
            print("Error in preamble (3)", file=sys.stderr)
            exit(1)
        port.write(b"OK")

        if verbose:
            print("Preamble complete")

    def load(self, verbose=False):
        port = self.port
        if verbose:
            print("Beginning load")

        # opening message
        port.write(b"LOAD")
        response = port.read(2) # should get OK back
        if response != b"OK":
            print("Error in load (4)", file=sys.stderr)
            exit(1)

        if verbose:
            print("Sending entry point")

#        print(port.read(8))
    
        # send entry
        port.write(b"ENTR")
        response = port.read(2) # should get OK back
        if response != b"OK":
            print(response)
            print("Error in load (5a)", file=sys.stderr)
            exit(1)
        port.write(int.to_bytes(self.image.e_entry, 4, byteorder=sys.byteorder, signed=False))
        response = port.read(2) # should get OK back
        if response != b"OK":
            print(response)
            print("Error in load (5b)", file=sys.stderr)
            exit(1)

        if verbose:
            print("Sending pre-data info")

        # send pre-data information
        # --> allows client to allocate space for initialized memory
        for n in range(self.image.e_phnum):
            self.image.read_prog_header(n)

            if self.image.p_type != 1: # only LOAD is important
                continue

            if self.image.p_memsz == 0:
                continue

            port.write(b"PHDR")
            port.write(int.to_bytes(self.image.p_vaddr, 4, byteorder=sys.byteorder, signed=False))
            port.write(int.to_bytes(self.image.p_memsz, 4, byteorder=sys.byteorder, signed=False))
            port.write(int.to_bytes(self.image.p_filesz, 4, byteorder=sys.byteorder, signed=False))
            response = port.read(2) # should get OK back
            if response != b"OK":
                print("Error in load (6)", file=sys.stderr)
                exit(1)

            if verbose:
                print("Sent header " + str(n+1))
        port.write(b"\x00")

        if verbose:
            print("Confirming that image can be loaded")
        response = port.read(2) # should get OK back
        if response != b"OK":
            print(response)
            print("Error in load (7)", file=sys.stderr)
            exit(1)
#
#    
#
#        # count initialized memory
#        initialized_data_size = 0
#        for n in range(image.e_phnum):
#            image.read_program_header(n)
#
#            if image.p_type != 1: # only LOAD is important
#                continue
#
#            # check if data based on address
#            if DATA_ADDR_START <= image.p_vaddr < DATA_ADDR_END:
#                initialized_data_size += math.ceil(image.p_filesz / DATA_INIT_BLOCK_SIZE) * DATA_INIT_BLOCK_SIZE
#
#        # send count
#        port.write(b"INIT")
#        port.write(int.to_bytes(initialized_data_size, 4, byteorder=sys.byteorder, signed=False))
#        response = port.read(2)
#        if response != b"OK":
#            print("Error in load (6)", file=sys.stderr)
#            exit(1)

        if verbose:
            print("Sending data")

        # send actual data
        for n in range(self.image.e_phnum):
            self.image.read_prog_header(n)

            # check header type
            if self.image.p_type != 1: # only LOAD is important
                continue

            if self.image.p_memsz == 0:
                continue

            # transfer in blocks
            port.write(b"DATA")
            nblocks = math.ceil(self.image.p_filesz / self.block_size)
            port.write(int.to_bytes(nblocks, 4, byteorder=sys.byteorder, signed=False)) # number of blocks

            if verbose:
                print("Sending " + str(nblocks) + " blocks...")

            self.image.seek(self.image.p_offset)
            fsz = self.image.p_filesz;
            for i in range(0, nblocks):
                if verbose:
                    print(i)
                if fsz > self.block_size:
                    data = self.image.read_bytes(self.block_size)
                else:
                    data = self.image.read_bytes(fsz)

                port.write(data)
                if len(data) < self.block_size:
                    if verbose:
                        print("padding block")
                    for j in range(self.block_size - len(data)):
                        port.write(b"\x00")
                response = port.read(2)
                if response != b"OK":
                    print(response)
#                    response = port.read(2)
                    print("Error in load (8)", file=sys.stderr)
                    exit(1)
                fsz -= self.block_size

            response = port.read(4)
            if response != b"DONE":
                print("Error in load (9)", file=sys.stderr)
                exit(1)

            if verbose:
                print("sent")

        port.write(b"\x00")

        # all data sent
        port.write(b"ALLCLEAR")
        response = port.read(2)
        if response != b"OK":
            print(response)
            print("Error in load (10)", file=sys.stderr)
            exit(1)

        if verbose:
            print("Load done")



parser = argparse.ArgumentParser(description="BoMu Image Loader")
parser.add_argument("image")
parser.add_argument("port", nargs="?", default="/dev/ttyUSB0")
parser.add_argument("-b", "--baud", "--load-baud", type=int, dest="postbaud", default=9600)
parser.add_argument("-B", "--preamble-baud", type=int, dest="prebaud", default=9600)
parser.add_argument("-v", "--verbose", action="store_true", dest="verbose")
parser.add_argument("-d", "--dump", action="store_true", dest="dump")
parser.add_argument("-M", "--min-version", type=int, dest="min_version", default=MIN_VERSION)

args = parser.parse_args()

if args.dump:
    image = Image(args.image)
    image.dump()
    exit(0)

ldr = Loader(Image(args.image), args.port, args.prebaud, verbose=args.verbose)
ldr.preamble(args.postbaud, args.min_version, verbose=args.verbose)
ldr.load(verbose=args.verbose)
