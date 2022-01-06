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
  candidate.data.time_start = 5;
  candidate.data.time_end = 6;
  candidate.data.time_candidate = 7;
  candidate.data.detid = 8;
  candidate.data.type = TriggerCandidateData::Type::kSupernova;
  candidate.data.algorithm = TriggerCandidateData::Algorithm::kPrescale;
  candidate.data.version = 1;

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

  TriggerCandidate candidate_read = read_overlay_from_buffer<TriggerCandidate>(buffer);

  BOOST_CHECK_EQUAL(candidate.data.time_start,     candidate_read.data.time_start);
  BOOST_CHECK_EQUAL(candidate.data.time_end,       candidate_read.data.time_end);
  BOOST_CHECK_EQUAL(candidate.data.time_candidate, candidate_read.data.time_candidate);
  BOOST_CHECK_EQUAL(candidate.data.detid,          candidate_read.data.detid);
  BOOST_CHECK_EQUAL(candidate.data.type,           candidate_read.data.type);
  BOOST_CHECK_EQUAL(candidate.data.algorithm,      candidate_read.data.algorithm);
  BOOST_CHECK_EQUAL(candidate.data.version,        candidate_read.data.version);

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
  activity.data.time_start = 1;
  activity.data.time_end = 2;
  activity.data.time_peak = 3;
  
  activity.data.channel_start = 4;
  activity.data.channel_end = 5;
  activity.data.channel_peak = 6;
  activity.data.adc_integral = 7;
  activity.data.adc_peak = 8;
  activity.data.detid = 9;
  activity.data.type = TriggerActivityData::Type::kTPC;
  activity.data.algorithm = TriggerActivityData::Algorithm::kSupernova;
  activity.data.version = 1;

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

  BOOST_CHECK_EQUAL(activity.data.time_start,     activity_read.data.time_start);
  BOOST_CHECK_EQUAL(activity.data.time_end,       activity_read.data.time_end);
  BOOST_CHECK_EQUAL(activity.data.time_peak,      activity_read.data.time_peak);
  BOOST_CHECK_EQUAL(activity.data.time_activity,  activity_read.data.time_activity);
  BOOST_CHECK_EQUAL(activity.data.channel_start,  activity_read.data.channel_start);
  BOOST_CHECK_EQUAL(activity.data.channel_end,    activity_read.data.channel_end);
  BOOST_CHECK_EQUAL(activity.data.channel_peak,   activity_read.data.channel_peak);
  BOOST_CHECK_EQUAL(activity.data.adc_integral,   activity_read.data.adc_integral);
  BOOST_CHECK_EQUAL(activity.data.adc_peak,       activity_read.data.adc_peak);
  BOOST_CHECK_EQUAL(activity.data.detid,          activity_read.data.detid);
  BOOST_CHECK_EQUAL(activity.data.type,           activity_read.data.type);
  BOOST_CHECK_EQUAL(activity.data.algorithm,      activity_read.data.algorithm);
  BOOST_CHECK_EQUAL(activity.data.version,        activity_read.data.version);

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
