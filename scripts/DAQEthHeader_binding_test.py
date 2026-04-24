#!/usr/bin/env python3

import sys
from detdataformats import DAQEthHeader


def main() -> int:

    header_size = DAQEthHeader.sizeof()
    if header_size <= 0:
        print("DAQEthHeader sizeof test failed")
        return 1
    else:
        print("DAQEthHeader sizeof test passed")

    header = DAQEthHeader(bytes(header_size))
    print("DAQEthHeader created from raw bytes")

    header.version = 5
    if header.version != 5:
        print("DAQEthHeader version property test failed")
        return 1

    header.det_id = 3
    if header.det_id != 3:
        print("DAQEthHeader det_id property test failed")
        return 1

    header.crate_id = 100
    if header.crate_id != 100:
        print("DAQEthHeader crate_id property test failed")
        return 1

    header.slot_id = 7
    if header.slot_id != 7:
        print("DAQEthHeader slot_id property test failed")
        return 1

    header.stream_id = 42
    if header.stream_id != 42:
        print("DAQEthHeader stream_id property test failed")
        return 1

    header.reserved = 12
    if header.reserved != 12:
        print("DAQEthHeader reserved property test failed")
        return 1

    header.seq_id = 4095
    if header.seq_id != 4095:
        print("DAQEthHeader seq_id property test failed")
        return 1

    header.block_length = 256
    if header.block_length != 256:
        print("DAQEthHeader block_length property test failed")
        return 1

    header.timestamp = 0x1234567890ABCDEF
    if header.timestamp != 0x1234567890ABCDEF:
        print("DAQEthHeader timestamp property test failed")
        return 1

    print("DAQEthHeader basic read/write property tests passed")

    if header.get_timestamp() != 0x1234567890ABCDEF:
        print("DAQEthHeader get_timestamp() test failed")
        return 1

    print("DAQEthHeader get_timestamp() test passed")    

    print("\nAll DAQEthHeader Python binding tests passed")
    return 0

if __name__ == "__main__":
    sys.exit(main())
