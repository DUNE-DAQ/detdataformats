/**
 * @file TriggerObjectOverlay.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DETDATAFORMATS_INCLUDE_DETDATAFORMATS_TRIGGER_TRIGGEROBJECTOVERLAY_HPP_
#define DETDATAFORMATS_INCLUDE_DETDATAFORMATS_TRIGGER_TRIGGEROBJECTOVERLAY_HPP_

#include "detdataformats/trigger/TriggerActivity.hpp"
#include "detdataformats/trigger/TriggerActivityData.hpp"
#include "detdataformats/trigger/TriggerCandidate.hpp"
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
  input_t inputs[]; // Non-standard flexible array member, but alternatives are worse
};

using TriggerActivityOverlay = TriggerObjectOverlay<TriggerActivityData, TriggerPrimitive>;

using TriggerCandidateOverlay = TriggerObjectOverlay<TriggerCandidateData, TriggerActivityData>;

// TODO: Is this sensible? TriggerPrimitive only has simple data
// members, so it can function as its own overlay. Is it useful to
// have this typedef to make TriggerPrimitive "look like" TA and TC?
using TriggerPrimitiveOverlay = TriggerPrimitive;

// TypeToOverlayType acts as a "map" from a type to its overlay type
// (eg from TriggerCandidate to TriggerCandidateOverlay). The point of
// having this is so that we can call the various overlay conversion
// functions below without having to explicitly specify the template
// arguments. An alternative way to do this would be to have
// TriggerActivity and TriggerCandidate each contain a using declaration
// like `using overlay_t = Trigger(Activity|Candidate)Overlay;`, but I
// couldn't figure out the right forward declaration for the overlay
// types in the TriggerCandidate/TriggerActivity header files
template<class T>
struct TypeToOverlayType;

template<>
struct TypeToOverlayType<TriggerActivity>
{
  using overlay_t = TriggerActivityOverlay;
  using data_t = TriggerActivityData;
};

template<>
struct TypeToOverlayType<TriggerCandidate>
{
  using overlay_t = TriggerCandidateOverlay;
  using data_t = TriggerCandidateData;
};

// Populate a TriggerObjectOverlay in `buffer`, created from
// `object`. The necessary size for the buffer can be found with
// `get_overlay_nbytes()`
template<class Object, class Overlay = typename TypeToOverlayType<Object>::overlay_t,
         class Data = typename TypeToOverlayType<Object>::data_t>
void
write_overlay(const Object& object, void* buffer)
{
  Overlay* overlay = reinterpret_cast<Overlay*>(buffer);
  overlay->data = static_cast<Data>(object);
  overlay->n_inputs = object.inputs.size();
  for (size_t i = 0; i < object.inputs.size(); ++i) {
    overlay->inputs[i] = object.inputs[i];
  }
}

// Calculate the size of buffer (in bytes) required to store an
// overlay created from `object`
template<class Object, class Overlay = typename TypeToOverlayType<Object>::overlay_t>
size_t
get_overlay_nbytes(const Object& object)
{
  // Need to check that this is actually right: what does sizeof
  // return when the class contains a flexible array member? Seems to
  // work in unit tests, so probably this is right
  return sizeof(Overlay) + object.inputs.size() * sizeof(typename Overlay::input_t);
}

// Given an overlay object (TriggerActivityOverlay or
// TriggerCandidateOverlay), create a corresponding non-overlay object
// (TriggerActivity or TriggerCandidate) with the same contents, and return it
template<class Object, class Overlay = typename TypeToOverlayType<Object>::overlay_t,
         class Data = typename TypeToOverlayType<Object>::data_t>
Object
read_overlay(const Overlay& overlay)
{
  Object ret;
  // overlay.data is a Trigger(Activity|Candidate)Data, which is the
  // base class of Trigger(Activity|Candidate). So we want to set the
  // base-class part of `ret` to `overlay.data`, which is what the
  // static_cast is for. We have to do the cast on the pointer to get
  // reference semantics (without the &, we get a _copy_ of ret, which
  // is not what we want)
  *static_cast<Data*>(&ret) = overlay.data;
  for (uint64_t i = 0; i < overlay.n_inputs; ++i) {
    ret.inputs.push_back(overlay.inputs[i]);
  }
  return ret;
}

// Given a buffer containing an overlay object (TriggerActivityOverlay
// or TriggerCandidateOverlay), create a corresponding non-overlay
// object (TriggerActivity or TriggerCandidate) with the same
// contents, and return it. This function requires the return type to
// be explicitly specified as a template argument, unlike the other
// functions in this file. Eg
//
//   TriggerActivity activity_read = read_overlay_from_buffer<TriggerActivity>(buffer);
template<class Object, class Overlay = typename TypeToOverlayType<Object>::overlay_t>
Object
read_overlay_from_buffer(const void* buffer)
{
  return read_overlay<Object, Overlay>(*reinterpret_cast<const Overlay*>(buffer));
}

} // namespace trigger
} // namespace detdataformats
} // namespace dunedaq

#endif // DETDATAFORMATS_INCLUDE_DETDATAFORMATS_TRIGGER_TRIGGERCANDIDATEOVERLAY_HPP_
