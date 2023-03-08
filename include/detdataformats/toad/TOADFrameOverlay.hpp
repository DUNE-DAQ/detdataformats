#ifndef DETDATAFORMATS_INCLUDE_DATAFORMATS_TOADFRAMEOVERLAY_HPP_
#define DETDATAFORMATS_INCLUDE_DATAFORMATS_TOADFRAMEOVERLAY_HPP_

#include <cstdint>
#include <cstddef>

namespace dunedaq {
namespace detdataformats {
namespace toad {

struct TOADFrameOverlay {
  unsigned fec: 16;
  uint64_t tstmp;
  bool hdr_par_check: 1;
  size_t n_samples;
  size_t n_bytes;
  unsigned short toadsamples[];

  uint64_t get_timestamp() const {
    return tstmp;
  }
  
  void set_timestamp(uint64_t new_tstmp){
    tstmp = new_tstmp;
  }
 
  std::size_t get_size() {
    return n_bytes;
  }   
};
 
} // namespace toad
} // namespace detdataformats
} // namespace dunedaq
#endif                           

