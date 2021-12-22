/**
 * @file WIBDecoder.hpp Fast C++ -> numpy WIB format decoder
 *
 * This is part of the DUNE DAQ , copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "detdataformats/wib/WIBFrame.hpp"

#include <cstdint>
#include <pybind11/numpy.h>

namespace py = pybind11;
namespace dunedaq::detdataformats::wib {

py::array_t<uint16_t> numpy_decoder(void* data, int nframes){
  py::array_t<uint16_t> ret(256 * nframes);
  auto ptr = static_cast<uint16_t*>(ret.request().ptr);
  for (size_t i=0; i<(size_t)nframes; ++i) {
    auto fr = reinterpret_cast<WIBFrame*>(static_cast<char*>(data) + i * sizeof(WIBFrame));
    for (size_t j=0; j<256; ++j)
      ptr[256 * i + j] = fr->get_channel(j);
  }
  ret.resize({nframes, 256});

  return ret;
}

} // namespace dunedaq::detdataformats::wib // NOLINT
