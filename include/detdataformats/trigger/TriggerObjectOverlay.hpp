/**
 * @file TriggerObjectOverlay.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DETDATAFORMATS_INCLUDE_DETDATAFORMATS_TRIGGER_TRIGGEROBJECTOVERLAY_HPP_
#define DETDATAFORMATS_INCLUDE_DETDATAFORMATS_TRIGGER_TRIGGEROBJECTOVERLAY_HPP_

#include "detdataformats/trigger/TriggerActivityData.hpp"
#include "detdataformats/trigger/TriggerCandidateData.hpp"
#include "detdataformats/trigger/TriggerPrimitive.hpp"

namespace dunedaq {
namespace detdataformats {
namespace trigger {

template<class DataType, class InputType>
struct TriggerObjectOverlay
{
  using data_t = DataType;
  using input_t = InputType;
  data_t data;
  uint64_t n_inputs;
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpedantic"
  input_t inputs[]; // Non-standard flexible array member, but alternatives are worse
  #pragma GCC diagnostic pop
};

using TriggerActivity = TriggerObjectOverlay<TriggerActivityData, TriggerPrimitive>;

using TriggerCandidate = TriggerObjectOverlay<TriggerCandidateData, TriggerActivityData>;

} // namespace trigger
} // namespace detdataformats
} // namespace dunedaq

#endif // DETDATAFORMATS_INCLUDE_DETDATAFORMATS_TRIGGER_TRIGGERCANDIDATEOVERLAY_HPP_
