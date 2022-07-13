/**
 * @file DetID.hpp Identification for the detector in the raw data
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DETDATAFORMATS_INCLUDE_DETDATAFORMATS_DETID_HPP_
#define DETDATAFORMATS_INCLUDE_DETDATAFORMATS_DETID_HPP_

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <istream>
#include <limits>
#include <ostream>
#include <sstream>
#include <string>
#include <tuple>

namespace dunedaq::detdataformats {

/**
 * @brief DetID is a versioned structure containing the 6 bits field of the unique identifier for a subdetector in the raw data.
 * For convenience this field is expanded to 16 bits.
 */
struct DetID
{

  using Version_t = uint16_t;   // NOLINT(build/unsigned)
  using Subdetector_t = uinti16_t; // NOLINT(build/unsigned)

  /**
   * @brief The Subdetector enum describes the kind of source we're dealing with
   */

  enum class Subdetector : Subdetector_t
  {
    kUNDEFINED = 0,
    kHDPDS = 1,
    kHDTPC = 2,
    kHDCRT = 3,
    kVDCPDS = 8,
    kVDMPDS = 9,
    kVDBTPC = 10,
    kVDTTPC = 11,
    kNDLAR = 32,
    kNDGAR = 33
  };

  /**
   * @brief The version of this DetID struct.
   */
  static constexpr Version_t s_det_id_version = 1;

  /**
   * @brief Version number of the DetID
   */
  Version_t version{ s_det_id_version };
  /**
   * @brief The general subdetector of the source of the data
   */
  Subdetector subdetector{ Subdetector::kUNDEFINED };

  DetID() = default;

  DetID(const Subdetector& subdetector_arg)
    : subdetector(subdetector_arg)
  {}

  std::string to_string() const
  {
    std::ostringstream ostr;
    ostr << subdetector_to_string(subdetector);
    return ostr.str();
  }

  bool is_in_valid_state() const noexcept { return subdetector != Subdetector::kUNDEFINED; }

  inline static std::string subdetector_to_string(const Subdetector& type);
  inline static Subdetector string_to_subdetector(const std::string& typestring);
};

} // namespace dunedaq::detdataformats

#include "detail/DetID.hxx"

#endif // DETDATAFORMATS_INCLUDE_DETDATAFORMATS_DETID_HPP_
