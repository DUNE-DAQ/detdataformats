/**
 * @file GeoID.hpp Geographic identification of a detector data stream
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DETDATAFORMATS_INCLUDE_DETDATAFORMATS_GEOID_HPP_
#define DETDATAFORMATS_INCLUDE_DETDATAFORMATS_GEOID_HPP_

#include "detdataformats/DetID.hpp"

#include <compare>
#include <cstdint>
#include <ostream>
#include <string>

namespace dunedaq::detdataformats {

/**
 * @brief GeoID locates a detector data stream in detector-geographic coordinates
 * (subdetector, crate, slot, stream), mirroring the confmodel GeoId configuration class.
 *
 * It has a canonical packed 64-bit form, which is what is stored in raw data files:
 * bits 0-15 detector_id, 16-31 crate_id, 32-47 slot_id, 48-63 stream_id.
 */
struct GeoID
{
  using encoded_t = uint64_t; // NOLINT(build/unsigned)

  uint16_t detector_id{ 0 }; // NOLINT(build/unsigned) a DetID::Subdetector value
  uint16_t crate_id{ 0 };    // NOLINT(build/unsigned)
  uint16_t slot_id{ 0 };     // NOLINT(build/unsigned)
  uint16_t stream_id{ 0 };   // NOLINT(build/unsigned)

  constexpr GeoID() = default;
  constexpr GeoID(DetID::Subdetector subdet,
                  uint16_t crate,                // NOLINT(build/unsigned)
                  uint16_t slot,                 // NOLINT(build/unsigned)
                  uint16_t stream)               // NOLINT(build/unsigned)
    : detector_id(static_cast<uint16_t>(subdet)) // NOLINT(build/unsigned)
    , crate_id(crate)
    , slot_id(slot)
    , stream_id(stream)
  {
  }

  constexpr DetID::Subdetector subdetector() const noexcept { return static_cast<DetID::Subdetector>(detector_id); }

  constexpr encoded_t encode() const noexcept
  {
    return (static_cast<encoded_t>(stream_id) << 48) | (static_cast<encoded_t>(slot_id) << 32) |
           (static_cast<encoded_t>(crate_id) << 16) | static_cast<encoded_t>(detector_id);
  }

  static constexpr GeoID decode(encoded_t value) noexcept
  {
    GeoID geo_id;
    geo_id.detector_id = static_cast<uint16_t>(value & 0xffff);       // NOLINT(build/unsigned)
    geo_id.crate_id = static_cast<uint16_t>((value >> 16) & 0xffff);  // NOLINT(build/unsigned)
    geo_id.slot_id = static_cast<uint16_t>((value >> 32) & 0xffff);   // NOLINT(build/unsigned)
    geo_id.stream_id = static_cast<uint16_t>((value >> 48) & 0xffff); // NOLINT(build/unsigned)
    return geo_id;
  }

  // Ordering follows the encoded value, so sorted GeoIDs and sorted encoded values agree
  constexpr auto operator<=>(const GeoID& other) const noexcept { return encode() <=> other.encode(); }
  constexpr bool operator==(const GeoID& other) const noexcept { return encode() == other.encode(); }

  std::string to_string() const;
};

std::ostream&
operator<<(std::ostream& o, GeoID const& geo_id);

} // namespace dunedaq::detdataformats

#include "detail/GeoID.hxx"

#endif // DETDATAFORMATS_INCLUDE_DETDATAFORMATS_GEOID_HPP_
