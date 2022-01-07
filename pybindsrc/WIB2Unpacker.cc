/**
 * @file WIB2Unpacker.cc Fast C++ -> numpy WIB2 format decoder
 *
 * This is part of the DUNE DAQ , copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "detdataformats/wib2/WIB2Frame.hpp"

#include <cstdint>
#include <pybind11/numpy.h>

namespace py = pybind11;
namespace dunedaq::detdataformats::wib2 {

/**
 * @brief Unpacks data containing WIB2Frames into a numpy array with the ADC
 * values and dimension (number of WIB2Frames, 256)
 * Warning: It doesn't check that nframes is a sensible value (can read out of bounds)
 */
py::array_t<uint16_t> np_array_adc(void* data, int nframes){
  py::array_t<uint16_t> ret(256 * nframes);
  auto ptr = static_cast<uint16_t*>(ret.request().ptr);
  for (size_t i=0; i<(size_t)nframes; ++i) {
    auto fr = reinterpret_cast<WIB2Frame*>(static_cast<char*>(data) + i * sizeof(WIB2Frame));
    for (size_t j=0; j<256; ++j)
      ptr[256 * i + j] = fr->get_adc(j);
  }
  ret.resize({nframes, 256});

  return ret;
}

/**
 * @brief Unpacks data containing WIB2Frames into a numpy array with the
 * timestamps with dimension (number of WIB2Frames)
 * Warning: It doesn't check that nframes is a sensible value (can read out of bounds)
 */
py::array_t<uint64_t> np_array_timestamp(void* data, int nframes){
  py::array_t<uint64_t> ret(nframes);
  auto ptr = static_cast<uint64_t*>(ret.request().ptr);
  for (size_t i=0; i<(size_t)nframes; ++i) {
    auto fr = reinterpret_cast<WIB2Frame*>(static_cast<char*>(data) + i * sizeof(WIB2Frame));
    ptr[i] = fr->get_timestamp();
  }

  return ret;
}

} // namespace dunedaq::detdataformats::wib // NOLINT
