/**
 * @file TriggerCandidate.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DETDATAFORMATS_INCLUDE_DETDATAFORMATS_TRIGGER_TRIGGERCANDIDATE_HPP_
#define DETDATAFORMATS_INCLUDE_DETDATAFORMATS_TRIGGER_TRIGGERCANDIDATE_HPP_

#include "detdataformats/trigger/TriggerActivityData.hpp"
#include "detdataformats/trigger/TriggerCandidateData.hpp"

#include <vector>

namespace dunedaq {
namespace detdataformats {
namespace trigger {

struct TriggerCandidate : public TriggerCandidateData
{
  std::vector<TriggerActivityData> inputs;
};

} // namespace trigger
} // namespace detdataformats
} // namespace dunedaq

#endif // DETDATAFORMATS_INCLUDE_DETDATAFORMATS_TRIGGER_TRIGGERCANDIDATE_HPP_
