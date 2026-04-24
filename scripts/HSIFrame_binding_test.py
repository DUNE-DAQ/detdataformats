#!/usr/bin/env python3

import sys
from detdataformats import HSIFrame


def main() -> int:

    frame_size = HSIFrame.sizeof()
    if frame_size <= 0:
        print("HSIFrame sizeof test failed")
        return 1
    else:
        print("HSIFrame sizeof test passed")

    frame = HSIFrame()
    print("HSIFrame default constructor test passed")

    # Default-constructed frame has max values for timestamp components
    default_timestamp = frame.get_timestamp()
    expected_default_timestamp = 0xFFFFFFFFFFFFFFFF
    if default_timestamp != expected_default_timestamp:
        print(f"HSIFrame default get_timestamp() test failed: got {hex(default_timestamp)}, expected {hex(expected_default_timestamp)}")
        return 1
    else:
        print("HSIFrame default get_timestamp() test passed")

    frame_from_bytes = HSIFrame(bytes(frame_size))
    print("HSIFrame created from raw bytes")

    if frame_from_bytes.version != 0:
        print("HSIFrame version read from raw bytes test failed")
        return 1

    if frame_from_bytes.detector_id != 0:
        print("HSIFrame detector_id read from raw bytes test failed")
        return 1

    if frame_from_bytes.crate != 0:
        print("HSIFrame crate read from raw bytes test failed")
        return 1

    if frame_from_bytes.slot != 0:
        print("HSIFrame slot read from raw bytes test failed")
        return 1

    if frame_from_bytes.link != 0:
        print("HSIFrame link read from raw bytes test failed")
        return 1

    if frame_from_bytes.input_low != 0:
        print("HSIFrame input_low read from raw bytes test failed")
        return 1

    if frame_from_bytes.input_high != 0:
        print("HSIFrame input_high read from raw bytes test failed")
        return 1

    if frame_from_bytes.trigger != 0:
        print("HSIFrame trigger read from raw bytes test failed")
        return 1

    if frame_from_bytes.sequence != 0:
        print("HSIFrame sequence read from raw bytes test failed")
        return 1

    print("HSIFrame read-only property tests passed")

    try:
        frame_from_bytes.version = 1
        print("HSIFrame version write should have raised an exception")
        return 1
    except Exception:
        print("HSIFrame version attempted write correctly raised an exception")

    timestamp = frame_from_bytes.get_timestamp()
    expected_timestamp = 0x0
    if timestamp != expected_timestamp:
        print(f"HSIFrame get_timestamp() test failed: got {timestamp}, expected {expected_timestamp}")
        return 1
    else:
        print("HSIFrame zeroed-bytes get_timestamp() test passed")

    print("\nAll HSIFrame Python binding tests passed")
    return 0


if __name__ == "__main__":
    sys.exit(main())
