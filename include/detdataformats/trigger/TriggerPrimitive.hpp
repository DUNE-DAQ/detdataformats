/**
 * @file TriggerPrimitive.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DETDATAFORMATS_INCLUDE_DETDATAFORMATS_TRIGGER_TRIGGERPRIMITIVE_HPP_
#define DETDATAFORMATS_INCLUDE_DETDATAFORMATS_TRIGGER_TRIGGERPRIMITIVE_HPP_

#include "detdataformats/trigger/Types.hpp"

#include <bitset>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <type_traits>

namespace dunedaq {
namespace detdataformats {
namespace trigger {

/**
 * @brief A single energy deposition on a TPC or PDS channel
 */
struct TriggerPrimitive
{
  /**
   * @brief The type of a TriggerPrimitive
   */
  enum class Type
  {
    kUnknown = 0,
    kTPC = 1,
    kPDS = 2,
  };

  /**
   * @brief The algorithm used to form a TriggerPrimitive
   */
  enum class Algorithm
  {
    kUnknown = 0,
    kTPCDefault = 1
  };

  /**
   * @brief A bitmask of flags from PrimitiveFlagsBits OR'ed together
   *
   * (a std::bitset<> would be nicer, but we don't have code to serialize it yet)
   */
  using Flags = uint16_t;

  // Update this version number if there are any changes to the in-memory representation of this class!
  static constexpr version_t s_trigger_primitive_version = 1; // NOLINT(build/unsigned)
  
  version_t version = s_trigger_primitive_version; // NOLINT(build/unsigned)
  timestamp_t time_start = INVALID_TIMESTAMP;
  timestamp_t time_peak = INVALID_TIMESTAMP;
  timestamp_t time_over_threshold = INVALID_TIMESTAMP;
  channel_t channel = INVALID_CHANNEL;
  uint32_t adc_integral = { 0 }; // NOLINT(build/unsigned)
  uint16_t adc_peak = { 0 };     // NOLINT(build/unsigned)
  detid_t detid = INVALID_DETID;
  Type type = Type::kUnknown;
  Algorithm algorithm = Algorithm::kUnknown;

  Flags flag = 0;
};

/**
 * Names for each of the bits in the TriggerPrimitive Flags
 */
namespace FlagBits {
using Flags = TriggerPrimitive::Flags;

constexpr Flags kSomehowBad = static_cast<Flags>(1) << 0;
constexpr Flags kUnassigned1 = static_cast<Flags>(1) << 1;
constexpr Flags kUnassigned2 = static_cast<Flags>(1) << 2;
constexpr Flags kUnassigned3 = static_cast<Flags>(1) << 3;
constexpr Flags kUnassigned4 = static_cast<Flags>(1) << 4;
constexpr Flags kUnassigned5 = static_cast<Flags>(1) << 5;
constexpr Flags kUnassigned6 = static_cast<Flags>(1) << 6;
constexpr Flags kUnassigned7 = static_cast<Flags>(1) << 7;
constexpr Flags kUnassigned8 = static_cast<Flags>(1) << 8;
constexpr Flags kUnassigned9 = static_cast<Flags>(1) << 9;
constexpr Flags kUnassigned10 = static_cast<Flags>(1) << 10;
constexpr Flags kUnassigned11 = static_cast<Flags>(1) << 11;
constexpr Flags kUnassigned12 = static_cast<Flags>(1) << 12;
constexpr Flags kUnassigned13 = static_cast<Flags>(1) << 13;
constexpr Flags kUnassigned14 = static_cast<Flags>(1) << 14;
constexpr Flags kUnassigned15 = static_cast<Flags>(1) << 15;

}

inline std::istream&
operator>>(std::istream& is, TriggerPrimitive::Type& t)
{
  std::underlying_type<TriggerPrimitive::Type>::type tmp;
  is >> tmp;
  t = static_cast<TriggerPrimitive::Type>(tmp);
  return is;
}

inline std::ostream&
operator<<(std::ostream& os, const TriggerPrimitive::Type& t)
{
  using underlying_t = std::underlying_type<TriggerPrimitive::Type>::type;
  return os << static_cast<underlying_t>(t);
}

} // namespace trigger
} // namespace detdataformats
} // namespace dunedaq

#endif // DETDATAFORMATS_INCLUDE_DETDATAFORMATS_TRIGGER_TRIGGERPRIMITIVE_HPP_
