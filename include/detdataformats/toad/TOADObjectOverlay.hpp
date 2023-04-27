#ifndef DETDATAFORMATS_INCLUDE_DATAFORMATS_TOADOBJECTOVERLAY_HPP_
#define DETDATAFORMATS_INCLUDE_DATAFORMATS_TOADOBJECTOVERLAY_HPP_

#include "TOADFrame.hpp"
#include "TOADFrameOverlay.hpp"

namespace dunedaq {
namespace detdataformats {
namespace toad {

class TOADObjectOverlay
{
  public:
  TOADFrameOverlay* overlay;
  void write_toad_overlay(TOADFrame& object, void* buffer, size_t nbytes){
    overlay = reinterpret_cast<TOADFrameOverlay*>(buffer);
    overlay->fec = object.fec;
    overlay->tstmp = object.tstmp;
    overlay->hdr_par_check = object.hdr_par_check;
    overlay->n_samples = object.toadsamples.size();
    overlay->n_bytes = nbytes;
    size_t sample_num = object.toadsamples.size();
    for (size_t i = 0; i < sample_num; i++) {
      overlay->toadsamples[i] = object.toadsamples[i];
      printf("sample: %d\n", (int)(overlay->toadsamples[i])); 
    }
    
  }
  size_t get_toad_overlay_nbytes(TOADFrame& object) {
    return sizeof(TOADFrameOverlay) + object.toadsamples.size() * sizeof(unsigned short);
  }
    
};

} // namespace toad
} // namespace detdataformats
} // namespace dunedaq
#endif
