/**
 *  * @file toad.cpp Python bindings for the TOADFrame format
 *   *
 *    * This is part of the DUNE DAQ Software Suite, copyright 2020.
 *     * Licensing/copyright details are in the COPYING file that you should have
 *      * received with this code.
 *       */

#include "detdataformats/toad/TOADFrameOverlay.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <cstdint>
#include <cstddef>
#include <pybind11/numpy.h>
#include <vector>

namespace py = pybind11;

namespace dunedaq {
namespace detdataformats {

namespace toad {
namespace python {
/*
py::array_t<unsigned short> readsamples(unsigned short* toadsamples, uint32_t n_samples){
  printf("here");
  auto samples = py::array_t<unsigned short>(n_samples);
  printf("here");
  std::vector<unsigned short> csamples;
  for(int index = 0; index < n_samples; index++){
    csamples.push_back(*(toadsamples+index));
  }
  printf("here");
  samples = py::cast(csamples);
  printf("here");
  return samples;
}
py::array_t<uint16_t> np_array_adc_data(void* data, int nsamples, int nbytes){
  py::array_t<uint16_t> ret(nsamples);
  auto ptr = static_cast<uint16_t*>(ret.request().ptr);
//  auto fr = reinterpret_cast<detdataformats::toad::TOADFrameOverlay*>(static_cast<char*>(data));
    for (size_t j=0; j<nsamples; ++j){
      ptr[j] = data->get_samples(j);
    }
  return ret;
}
*/
void
register_toad(py::module& m)
{
  py::class_<TOADFrameOverlay>(m, "TOADFrameOverlay", py::buffer_protocol())
    .def(py::init())
    .def(py::init([](py::capsule capsule) {
        auto wfp = *static_cast<TOADFrameOverlay*>(capsule.get_pointer());
        return wfp;
    } ))
    .def_property_readonly("tstmp", [](TOADFrameOverlay& self) -> uint64_t {return self.tstmp;})
    .def_property_readonly("fec", [](TOADFrameOverlay& self) -> uint32_t {return self.fec;})
    .def_property_readonly("hdr_par_check", [](TOADFrameOverlay& self) -> uint32_t {return self.hdr_par_check;})
    .def_property_readonly("n_samples", [](TOADFrameOverlay& self) -> size_t {return self.n_samples;})
    .def_property_readonly("n_bytes", [](TOADFrameOverlay& self) -> size_t {return self.n_bytes;})
    .def("get_n_samples", &TOADFrameOverlay::get_n_samples, py::return_value_policy::reference)
    .def("get_first_sample", &TOADFrameOverlay::get_first_sample, py::return_value_policy::reference)
    .def("get_samples", &TOADFrameOverlay::get_samples, py::return_value_policy::reference)
    .def_static("sizeof", [](){ return sizeof(TOADFrameOverlay); });
//    .def("np_array_adc", &np_array_adc)
//    .def("np_array_adc_data", &np_array_adc_data);
    //.def("readsamples", &readsamples, py::return_value_policy::reference)
    //.def("toadsamples", [](TOADFrameOverlay& self)-> int {return self.toadsamples[0];});
}

} // namespace python
} // namespace toad
} // namespace detdataformats
} // namespace dunedaq
