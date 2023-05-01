#ifndef DETDATAFORMATS_INCLUDE_DATAFORMATS_TOADFRAMEOVERLAY_HPP_
#define DETDATAFORMATS_INCLUDE_DATAFORMATS_TOADFRAMEOVERLAY_HPP_

#include <cstdint>
#include <cstddef>
#include <cstdio>

namespace dunedaq {
namespace detdataformats {
namespace toad {

class TOADFrameOverlay {
  public:
  uint64_t tstmp;
  size_t n_samples;
  size_t n_bytes; 
  unsigned fec;
  bool hdr_par_check;
  //char toadsample_test[200];
  unsigned short toadsamples[];

  uint64_t get_timestamp() const {
    return tstmp;
  }
  
  void set_timestamp(uint64_t new_tstmp){
    tstmp = new_tstmp;
  }
 
  size_t get_size() {
    return n_bytes;
  }

  int get_n_samples(){
    return n_samples;
  }

  int get_first_sample() {
    printf("%p, %p, %p\n", &n_bytes, &toadsamples[0], &toadsamples[n_samples]); 
    return toadsamples[0];
  }

  unsigned short get_samples(int j) {
    return toadsamples[j];
  } 

  unsigned int bare_size()
  {
    return sizeof(tstmp)+sizeof(n_samples)+sizeof(n_bytes)+sizeof(fec)+sizeof(hdr_par_check);
  }   
};
 
} // namespace toad
} // namespace detdataformats
} // namespace dunedaq
#endif                           

