/**
 * @file TriggerCandidate.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DETDATAFORMATS_INCLUDE_DETDATAFORMATS_TRIGGER_TRIGGERCANDIDATEDATA_HPP_
#define DETDATAFORMATS_INCLUDE_DETDATAFORMATS_TRIGGER_TRIGGERCANDIDATEDATA_HPP_

#include "detdataformats/trigger/Types.hpp"

#include <cstdint>

namespace dunedaq {
namespace detdataformats {
namespace trigger {

struct TriggerCandidateData
{
  enum class Type
  {
    kUnknown = 0,
    kTiming = 1,
    kTPCLowE = 2,
    kSupernova = 3,
    kRandom = 4,
    kPrescale = 5,
    kADCSimpleWindow = 6,
    kHorizontalMuon = 7,
    kMichelElectron = 8,
    kPlaneCoincidence = 9,
  };

  enum class Algorithm
  {
    kUnknown = 0,
    kSupernova = 1,
    kHSIEventToTriggerCandidate = 2,
    kPrescale = 3,
    kADCSimpleWindow = 4,
    kHorizontalMuon = 5,
    kMichelElectron = 6, 
    kPlaneCoincidence = 7,    
    kCustom = 8, 
  };

  // Update this version number if there are any changes to the in-memory representation of this class!
  static constexpr version_t s_trigger_candidate_version = 1; // NOLINT(build/unsigned)

  version_t version = s_trigger_candidate_version;       // NOLINT(build/unsigned)
  timestamp_t time_start = INVALID_TIMESTAMP;
  timestamp_t time_end = INVALID_TIMESTAMP;
  timestamp_t time_candidate = INVALID_TIMESTAMP;
  // TODO P. Rodrigues 2021-01-06: This was originally a
  // std::vector<detid_t> but that messes up the overlay scheme, so
  // I've changed it for now to be just a detid_t. Need to work out
  // what to do longer term
  detid_t detid; // NOLINT(build/unsigned)
  Type type = Type::kUnknown;
  Algorithm algorithm = Algorithm::kUnknown; // NOLINT(build/unsigned)
};

} // namespace trigger
} // namespace detdataformats
} // namespace dunedaq

#endif // DETDATAFORMATS_INCLUDE_DETDATAFORMATS_TRIGGER_TRIGGERCANDIDATEDATA_HPP_
