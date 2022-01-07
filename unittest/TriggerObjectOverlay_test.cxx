/**
 * @file TriggerObjectoverlay_test.cxx TriggerObjectoverlay class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "detdataformats/trigger/TriggerActivityData.hpp"
#include "detdataformats/trigger/TriggerCandidate.hpp"
#include "detdataformats/trigger/TriggerPrimitive.hpp"
#include "detdataformats/trigger/TriggerObjectOverlay.hpp"

/**
 * @brief Name of this test module
 */
#define BOOST_TEST_MODULE TriggerObjectOverlay_test // NOLINT

#include "boost/test/data/test_case.hpp"
#include "boost/test/unit_test.hpp"

using namespace dunedaq::detdataformats::trigger;

BOOST_TEST_DONT_PRINT_LOG_VALUE(TriggerCandidate)
BOOST_TEST_DONT_PRINT_LOG_VALUE(TriggerCandidateData)
BOOST_TEST_DONT_PRINT_LOG_VALUE(TriggerActivityData)
BOOST_TEST_DONT_PRINT_LOG_VALUE(TriggerPrimitive::Algorithm)
BOOST_TEST_DONT_PRINT_LOG_VALUE(TriggerActivityData::Algorithm)
BOOST_TEST_DONT_PRINT_LOG_VALUE(TriggerCandidateData::Algorithm)
BOOST_TEST_DONT_PRINT_LOG_VALUE(TriggerPrimitive::Type)
BOOST_TEST_DONT_PRINT_LOG_VALUE(TriggerActivityData::Type)
BOOST_TEST_DONT_PRINT_LOG_VALUE(TriggerCandidateData::Type)

BOOST_AUTO_TEST_SUITE(BOOST_TEST_MODULE)

BOOST_AUTO_TEST_CASE(TriggerCandidateOverlay_in_out) {
  TriggerCandidate candidate;
  candidate.time_start = 5;
  candidate.time_end = 6;
  candidate.time_candidate = 7;
  candidate.detid = 8;
  candidate.type = TriggerCandidateData::Type::kSupernova;
  candidate.algorithm = TriggerCandidateData::Algorithm::kPrescale;
  candidate.version = 1;

  const size_t n_activity = 5;
  
  for(size_t i=0; i<n_activity; ++i){
    TriggerActivityData activity;
    activity.time_start = i;
    activity.time_end = i+1;
    activity.time_peak = i+2;

    activity.channel_start = i;
    activity.channel_end = 2*i;
    activity.channel_peak = 3*i;
    activity.adc_integral =  i;
    activity.adc_peak = 2*i;
    activity.detid = i;
    activity.type = TriggerActivityData::Type::kTPC;
    activity.algorithm = TriggerActivityData::Algorithm::kSupernova;
    activity.version = 1;

    candidate.inputs.push_back(activity);
  }

  size_t nbytes = get_overlay_nbytes(candidate);

  char* buffer = new char[nbytes];

  write_overlay(candidate, buffer);

  const TriggerCandidateOverlay& candidate_overlay = *reinterpret_cast<const TriggerCandidateOverlay*>(buffer);
  BOOST_CHECK_EQUAL(candidate.time_start,     candidate_overlay.data.time_start);
  
  TriggerCandidate candidate_read = read_overlay_from_buffer<TriggerCandidate>(buffer);

  BOOST_CHECK_EQUAL(candidate.time_start,     candidate_read.time_start);
  BOOST_CHECK_EQUAL(candidate.time_end,       candidate_read.time_end);
  BOOST_CHECK_EQUAL(candidate.time_candidate, candidate_read.time_candidate);
  BOOST_CHECK_EQUAL(candidate.detid,          candidate_read.detid);
  BOOST_CHECK_EQUAL(candidate.type,           candidate_read.type);
  BOOST_CHECK_EQUAL(candidate.algorithm,      candidate_read.algorithm);
  BOOST_CHECK_EQUAL(candidate.version,        candidate_read.version);

  for(size_t i=0; i<n_activity; ++i){
    BOOST_CHECK_EQUAL(candidate.inputs[i].time_start,    candidate_read.inputs[i].time_start);
    BOOST_CHECK_EQUAL(candidate.inputs[i].time_end,      candidate_read.inputs[i].time_end);
    BOOST_CHECK_EQUAL(candidate.inputs[i].time_peak,     candidate_read.inputs[i].time_peak);
    BOOST_CHECK_EQUAL(candidate.inputs[i].channel_start, candidate_read.inputs[i].channel_start);
    BOOST_CHECK_EQUAL(candidate.inputs[i].channel_end,   candidate_read.inputs[i].channel_end);
    BOOST_CHECK_EQUAL(candidate.inputs[i].channel_peak,  candidate_read.inputs[i].channel_peak);
    BOOST_CHECK_EQUAL(candidate.inputs[i].adc_integral,  candidate_read.inputs[i].adc_integral);
    BOOST_CHECK_EQUAL(candidate.inputs[i].adc_peak,      candidate_read.inputs[i].adc_peak);
    BOOST_CHECK_EQUAL(candidate.inputs[i].detid,         candidate_read.inputs[i].detid);
    BOOST_CHECK_EQUAL(candidate.inputs[i].type,          candidate_read.inputs[i].type);
    BOOST_CHECK_EQUAL(candidate.inputs[i].algorithm,     candidate_read.inputs[i].algorithm);
    BOOST_CHECK_EQUAL(candidate.inputs[i].version,       candidate_read.inputs[i].version);
  }

  delete[] buffer;
}

BOOST_AUTO_TEST_CASE(TriggerActivityOverlay_in_out) {
  TriggerActivity activity;
  activity.time_start = 1;
  activity.time_end = 2;
  activity.time_peak = 3;
  
  activity.channel_start = 4;
  activity.channel_end = 5;
  activity.channel_peak = 6;
  activity.adc_integral = 7;
  activity.adc_peak = 8;
  activity.detid = 9;
  activity.type = TriggerActivityData::Type::kTPC;
  activity.algorithm = TriggerActivityData::Algorithm::kSupernova;
  activity.version = 1;

  const size_t n_primitive = 5;
  
  for(size_t i=0; i<n_primitive; ++i){
    TriggerPrimitive primitive;

    primitive.time_start = i;
    primitive.time_peak = i+1;
    primitive.time_over_threshold = i+2;
    primitive.channel = i+3;
    primitive.adc_integral = i+4; // NOLINT(build/unsigned)
    primitive.adc_peak = i+5;     // NOLINT(build/unsigned)
    primitive.detid = i+65;
    primitive.type = TriggerPrimitive::Type::kPDS;
    primitive.algorithm = TriggerPrimitive::Algorithm::kTPCDefault;
    primitive.version = 1;      // NOLINT(build/unsigned)
    primitive.flag = 1;

    activity.inputs.push_back(primitive);
  }

  size_t nbytes = get_overlay_nbytes(activity);

  char* buffer = new char[nbytes];

  write_overlay(activity, buffer);

  TriggerActivity activity_read = read_overlay_from_buffer<TriggerActivity>(buffer);

  BOOST_CHECK_EQUAL(activity.time_start,     activity_read.time_start);
  BOOST_CHECK_EQUAL(activity.time_end,       activity_read.time_end);
  BOOST_CHECK_EQUAL(activity.time_peak,      activity_read.time_peak);
  BOOST_CHECK_EQUAL(activity.time_activity,  activity_read.time_activity);
  BOOST_CHECK_EQUAL(activity.channel_start,  activity_read.channel_start);
  BOOST_CHECK_EQUAL(activity.channel_end,    activity_read.channel_end);
  BOOST_CHECK_EQUAL(activity.channel_peak,   activity_read.channel_peak);
  BOOST_CHECK_EQUAL(activity.adc_integral,   activity_read.adc_integral);
  BOOST_CHECK_EQUAL(activity.adc_peak,       activity_read.adc_peak);
  BOOST_CHECK_EQUAL(activity.detid,          activity_read.detid);
  BOOST_CHECK_EQUAL(activity.type,           activity_read.type);
  BOOST_CHECK_EQUAL(activity.algorithm,      activity_read.algorithm);
  BOOST_CHECK_EQUAL(activity.version,        activity_read.version);

  for(size_t i=0; i<n_primitive; ++i){
    BOOST_CHECK_EQUAL(activity.inputs[i].time_start,          activity_read.inputs[i].time_start);
    BOOST_CHECK_EQUAL(activity.inputs[i].time_over_threshold, activity_read.inputs[i].time_over_threshold);
    BOOST_CHECK_EQUAL(activity.inputs[i].time_peak,           activity_read.inputs[i].time_peak);
    BOOST_CHECK_EQUAL(activity.inputs[i].channel,             activity_read.inputs[i].channel);
    BOOST_CHECK_EQUAL(activity.inputs[i].adc_integral,        activity_read.inputs[i].adc_integral);
    BOOST_CHECK_EQUAL(activity.inputs[i].adc_peak,            activity_read.inputs[i].adc_peak);
    BOOST_CHECK_EQUAL(activity.inputs[i].detid,               activity_read.inputs[i].detid);
    BOOST_CHECK_EQUAL(activity.inputs[i].type,                activity_read.inputs[i].type);
    BOOST_CHECK_EQUAL(activity.inputs[i].algorithm,           activity_read.inputs[i].algorithm);
    BOOST_CHECK_EQUAL(activity.inputs[i].version,             activity_read.inputs[i].version);
    BOOST_CHECK_EQUAL(activity.inputs[i].flag,                activity_read.inputs[i].flag);
  }

  delete[] buffer;
}

BOOST_AUTO_TEST_SUITE_END()
