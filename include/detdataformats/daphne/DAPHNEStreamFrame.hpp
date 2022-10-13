/**
 * @file DAPHNEStreamFrame.hpp
 *
 *  Contains declaration of DAPHNEStreamFrame, a class for accessing 
 *  raw DAPHNE streaming version frames, as produced by the DAPHNE boards
 *
 *  The canonical definition of the PDS DAPHNE format is given in EDMS document 2088726:
 *  https://edms.cern.ch/document/2088726/3
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DETDATAFORMATS_INCLUDE_DATAFORMATS_DAPHNE_DAPHNESTREAMFRAME_HPP_
#define DETDATAFORMATS_INCLUDE_DATAFORMATS_DAPHNE_DAPHNESTREAMFRAME_HPP_

#include "detdataformats/DAQHeader.hpp" // For unified DAQ header

#include <algorithm> // For std::min
#include <cassert>   // For assert()
#include <cstdio>
#include <cstdlib>
#include <stdexcept> // For std::out_of_range
#include <cstdint>  // For uint32_t etc

namespace dunedaq {
namespace detdataformats {
namespace daphne {

class DAPHNEStreamFrame
{
public:
  // ===============================================================
  // Preliminaries
  // ===============================================================

  // The definition of the format is in terms of 32-bit words
  typedef uint32_t word_t; // NOLINT

  static constexpr int s_bits_per_adc = 14;
  static constexpr int s_bits_per_word = 8 * sizeof(word_t);
  static constexpr int s_channels_per_frame = 4;
  static constexpr int s_adcs_per_channel = 64;
  static constexpr int s_daphnes_per_frame = 1;
  static constexpr int s_num_adc_words = s_channels_per_frame * s_adcs_per_channel * s_bits_per_adc / s_bits_per_word;

  struct Header
  {
    word_t channel_0 : 6, channel_1 : 6, channel_2 : 6, channel_3 : 6, tbd_0 : 8;
    word_t tbd_1 : 32;
  };

  struct Trailer
  {
    word_t tbd : 32;
  };

  // ===============================================================
  // Data members
  // ===============================================================
  detdataformats::DAQHeader daq_header;
  Header header;
  word_t adc_words[s_num_adc_words]; // NOLINT
  Trailer trailer; 

  // ===============================================================
  // Accessors
  // ===============================================================

  /**
   * @brief Get the @p i ADC value of @p chn in the frame
   */
  uint16_t get_adc(int i, int chn) const // NOLINT
  {
    // RS FIXME: TBA
  }

  /**
   * @brief Set the @p i ADC value of @p chn in the frame to @p val
   */
  void set_adc(int i, int chn, uint16_t val) // NOLINT
  {
    // RS FIXME: TBA
  }
};

} // namespace daphne
} // namespace detdataformats
} // namespace dunedaq

#endif // DETDATAFORMATS_INCLUDE_DATAFORMATS_DAPHNE_DAPHNESTREAMFRAME_HPP_
