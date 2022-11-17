#ifndef DETDATAFORMATS_INCLUDE_DATAFORMATS_TOADFRAMEOVERLAY_HPP_
#define DETDATAFORMATS_INCLUDE_DATAFORMATS_TOADFRAMEOVERLAY_HPP_

namespace dunedaq {
namespace detdataformats {
namespace toad {

struct TOADFrameOverlay {
  unsigned fec: 10;
  unsigned channel: 5;
  unsigned tstmp: 32;
  bool hdr_par_check: 1;
  bool data_par_check: 1;
  size_t n_samples;
  unsigned short toadsamples[];

  uint64_t get_timestamp() const {
    return (uint64_t)tstmp;
  }
  
  void set_timestamp(uint32_t new_tstmp){
    tstmp = new_tstmp;
  }
 
  std::size_t get_size() {
    return (3*sizeof(unsigned) + 2*sizeof(bool) + sizeof(size_t)+n_samples*sizeof(unsigned short));
  }
};
 
} // namespace toad
} // namespace detdataformats
} // namespace dunedaq
#endif                           

