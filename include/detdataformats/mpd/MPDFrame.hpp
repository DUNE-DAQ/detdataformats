/**
 * @file MPDFrame.hpp MPD 
 *
 * Contains declaration of MPDFrame. This class access raw MPD frames as produced by the MPD boards 
 *
 * Dataformat: https://docs.google.com/spreadsheets/d/1b9ohCndIZcYHEPiRDz4TMbaf6CJNlXrEuMnt3XS2BZ8/edit#gid=1694585828
 * 
 * This is part of the DUNE DAQ , copyright 2022.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DETDATAFORMATS_INCLUDE_DATAFORMATS_MPDFRAME_HPP_
#define DETDATAFORMATS_INCLUDE_DATAFORMATS_MPDFRAME_HPP_

#include <bitset>
#include <iostream>
#include <stdint.h>
#include <vector> 

namespace dunedaq {
namespace detdataformats {
namespace mpd {

/**
 * @brief MPD frame
 */
class MPDFrame
{
  public:

  typedef uint32_t word_t ; // NOLINT(build/unsigned)
  //  static constexpr unsigned int num_samples = 2048 ;
  
  struct MPDOSHeader {
    word_t timestamp_sync ; // 0x3f60b8a8 ;
    word_t timestamp_length ; // 0x00000008 ;
    uint64_t timestamp_OS ; // timestamp taken from the operating system
  };

  struct MPDEventHeader {  
    word_t SyncMagic ; // 0x2A502A50 ;
    word_t length ;
    word_t event_num ; 
  };

  struct MPDDeviceHeader {
    word_t device_serial_num ;
    word_t device_length : 24, device_model_id : 8;  
  };

  struct MPDTriggerHeader {
    //MStreamTriggerHeader
    word_t trigger_type: 2, trigger_length: 22, trigger_channel_number: 8 ; 
  };

  struct MPDTriggerDataHeader {
    //Trigger Data
    word_t event_timestamp_1: 32 ; 
    word_t flags: 2, event_timestamp_2: 30 ; 
    uint64_t channel_bit_mask ; 
  };

  // =======================================================
  // Data members 
  // =======================================================
  MPDOSHeader OSheader ; 
  MPDEventHeader event_header ; 
  MPDDeviceHeader device_header ; 
  MPDTriggerHeader trigger_header ; 
  MPDTriggerDataHeader trigger_data_header ; 

  //MStreamBlock
  // It contains a Header with the data_type, data_lenth, channel number
  // followed by two emtpy words and an array of data
  // This is repeated for every enabled channel
  // All this information is stored altogether in an array of MStreamBlocks
  word_t MStreamBlock[] ; 

  /** @brief Get the 64-bit timestamp of the frame
   */
  uint64_t get_timestamp() const {
    // in nanoseconds
    uint64_t s_to_ns = 1E9 ; 
    uint64_t timestamp = trigger_data_header.event_timestamp_1 * s_to_ns + trigger_data_header.event_timestamp_2 ; 
    timestamp *= 6.25E-2 ; 
    return timestamp ; 
  }

  word_t get_data(unsigned int i) { return MStreamBlock[i] ; }

};

} // namespace mpd
} // namespace detdataformats
} // namespace dunedaq

#endif // DETDATAFORMATS_INCLUDE_DATAFORMATS_MPDFRAME_HPP_
