#!/usr/bin/env python3

import sys
from detdataformats import DetID


def main() -> int:

    detid_default = DetID()
    if detid_default.subdetector != DetID.Subdetector.kUnknown:
        print("DetID default subdetector value test failed")
        return 1
    else:
        print("DetID default subdetector value test passed")

    detid = DetID(DetID.Subdetector.kVD_BernCRT)

    if detid.subdetector != DetID.Subdetector.kVD_BernCRT:
        print("DetID constructor/readwrite field test failed")
        return 1
    else:
        print("DetID constructor/readwrite field test passed")

    subdetector_name = DetID.subdetector_to_string(detid.subdetector)
    if subdetector_name != "VD_BernCRT":
        print(f"subdetector_to_string test failed, got: {subdetector_name}")
        return 1
    else:
        print(f"subdetector_to_string test passed, got: {subdetector_name}")

    parsed = DetID.string_to_subdetector("VD_GrenobleCRT")
    if parsed != DetID.Subdetector.kVD_GrenobleCRT:
        print("string_to_subdetector test failed")
        return 1
    else:
        print("string_to_subdetector test passed")

    print("\nAll DetID Python binding tests passed")
    return 0


if __name__ == "__main__":
    sys.exit(main())
