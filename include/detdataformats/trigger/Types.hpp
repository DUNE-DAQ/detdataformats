/**
 * @file Types.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DETDATAFORMATS_INCLUDE_DETDATAFORMATS_TRIGGER_TYPES_HPP_
#define DETDATAFORMATS_INCLUDE_DETDATAFORMATS_TRIGGER_TYPES_HPP_

#include <cstdint>
#include <limits>

namespace dunedaq {
namespace detdataformats {
namespace trigger {

// A data timestamp in timing system clock ticks (50 MHz for
// ProtoDUNE-I, 62.5 MHz for ProtoDUNE-II and DUNE FD). If/when we
// depend on daqdataformats, we can get these from there
using timestamp_t = uint64_t; // NOLINT

constexpr timestamp_t INVALID_TIMESTAMP = std::numeric_limits<timestamp_t>::max();

using timestamp_diff_t = int64_t;

// A logical region of the detector from which DS objects may be
// formed, eg an APA or a module
//
// TODO P. Rodrigues 2021-06-01: it would be nice to have this be just
// daqdataformats::GeoID, if/when we can depend on the daqdataformats
// package
using detid_t = uint16_t;

constexpr detid_t INVALID_DETID = std::numeric_limits<detid_t>::max();

// A detid representing a request to read out the whole detector
constexpr detid_t WHOLE_DETECTOR = INVALID_DETID - 1;

// A trigger number
// TODO P. Rodrigues 2021-06-14: it would be nice to have this be just
// daqdataformats::trigger_number_t, if/when we can depend on the daqdataformats
// package
using trigger_number_t = uint64_t;

constexpr trigger_number_t INVALID_TRIGGER_NUMBER = std::numeric_limits<trigger_number_t>::max();

// A channel number
using channel_t = int32_t;

constexpr channel_t INVALID_CHANNEL = std::numeric_limits<channel_t>::max();

// A version number of an object
using version_t = uint16_t;

constexpr version_t INVALID_VERSION = std::numeric_limits<version_t>::max();

} // namespace trigger
} // namespace detdataformats
} // namespace dunedaq

#endif // DETDATAFORMATS_INCLUDE_DETDATAFORMATS_TRIGGER_TYPES_HPP_
