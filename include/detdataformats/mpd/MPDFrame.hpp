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
  static constexpr unsigned int num_samples = 2048 ;
  
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

  struct MPDDataHeader {
    //Data header
    word_t data_type: 2, data_length: 22, channel_number: 8 ;
  };
    

  // =======================================================
  // Data members 
  // =======================================================
  MPDOSHeader OSheader ; 
  MPDEventHeader event_header ; 
  MPDDeviceHeader device_header ; 
  MPDTriggerHeader trigger_header ; 
  MPDTriggerDataHeader trigger_data_header ; 
  MPDDataHeader data_header ; 

  //MStream Data
  uint64_t _null; // unused words
  uint16_t data[num_samples];

  uint16_t get_sample( const unsigned i ) const {
    if ( i >= num_samples   ) throw std::out_of_range("Index out of range");
    return data[i];
  }

  void set_value( const unsigned int i, uint16_t val ) {
    if ( i >= num_samples   ) throw std::out_of_range("Index out of range");
    data[i] = val ; 
    return; 
  }

  /** @brief Get the 64-bit timestamp of the frame
   */
  uint64_t get_timestamp() const {
    // in nanoseconds
    return (uint64_t)trigger_data_header.event_timestamp_1 | ((uint64_t)trigger_data_header.event_timestamp_2 << 32 ) ; 
  }

};

} // namespace mpd
} // namespace detdataformats
} // namespace dunedaq

#endif // DETDATAFORMATS_INCLUDE_DATAFORMATS_MPDFRAME_HPP_
