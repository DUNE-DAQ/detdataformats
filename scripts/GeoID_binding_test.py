#!/usr/bin/env python3

import sys
from detdataformats import DetID, GeoID


def main() -> int:

    geo_id = GeoID(DetID.Subdetector.kHD_TPC, 1, 2, 3)
    if (geo_id.detector_id, geo_id.crate_id, geo_id.slot_id, geo_id.stream_id) != (3, 1, 2, 3):
        print("GeoID constructor/field test failed")
        return 1
    else:
        print("GeoID constructor/field test passed")

    if GeoID.decode(geo_id.encode()) != geo_id:
        print("GeoID encode/decode round trip test failed")
        return 1
    else:
        print("GeoID encode/decode round trip test passed")

    # equal GeoIDs hash equally, so they work as dict keys and set members
    same = GeoID(DetID.Subdetector.kHD_TPC, 1, 2, 3)
    if hash(same) != hash(geo_id) or len({geo_id, same}) != 1 or {geo_id: "x"}.get(same) != "x":
        print("GeoID hash/equality test failed")
        return 1
    else:
        print("GeoID hash/equality test passed")

    # the fields are read-only, so a stored key can't change its hash
    for field in ("detector_id", "crate_id", "slot_id", "stream_id"):
        try:
            setattr(geo_id, field, 9)
        except AttributeError:
            continue
        print(f"GeoID read-only field test failed: {field} could be assigned")
        return 1
    print("GeoID read-only field test passed")

    print("\nAll GeoID Python binding tests passed")
    return 0


if __name__ == "__main__":
    sys.exit(main())
