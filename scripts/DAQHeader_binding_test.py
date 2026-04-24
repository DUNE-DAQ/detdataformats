#!/usr/bin/env python3

import sys
from detdataformats import DAQHeader


def main() -> int:

    header = DAQHeader()

    header.version = 5
    if header.version != 5:
        print("DAQHeader version property test failed")
        return 1

    header.det_id = 3
    if header.det_id != 3:
        print("DAQHeader det_id property test failed")
        return 1

    header.crate_id = 100
    if header.crate_id != 100:
        print("DAQHeader crate_id property test failed")
        return 1

    header.slot_id = 7
    if header.slot_id != 7:
        print("DAQHeader slot_id property test failed")
        return 1

    header.link_id = 15
    if header.link_id != 15:
        print("DAQHeader link_id property test failed")
        return 1

    print("DAQHeader basic read/write property tests passed")

    timestamp_1 = 0x3E8
    header.timestamp_1 = timestamp_1
    
    timestamp_2 = 0x7D0
    header.timestamp_2 = timestamp_2

    try:
        _ = header.timestamp_1
        print("DAQHeader timestamp_1 read should have raised an exception")
        return 1
    except Exception:
        print("DAQHeader timestamp_1 attempted read correctly raised an exception")

    try:
        _ = header.timestamp_2
        print("DAQHeader timestamp_2 read should have raised an exception")
        return 1
    except Exception:
        print("DAQHeader timestamp_2 attempted read correctly raised an exception")

    # Test get_timestamp method returns combined timestamp
    timestamp = header.get_timestamp()
    expected_timestamp = 0x000007D0000003E8
    if timestamp != expected_timestamp:
        print(f"DAQHeader get_timestamp() test failed: got {timestamp}, expected {expected_timestamp}")
        return 1
    else:
        print("DAQHeader get_timestamp() test passed")

    print("\nAll DAQHeader Python binding tests passed")
    return 0


if __name__ == "__main__":
    sys.exit(main())
