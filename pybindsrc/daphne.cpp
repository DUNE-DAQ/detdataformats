/**
 * @file daphne.cpp Python bindings for the DAPHNEFrame format
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "detdataformats/daphne/DAPHNEFrame.hpp"
#include "DAPHNEUnpacker.cc"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace dunedaq {
namespace detdataformats {
namespace daphne {
namespace python {

void
register_daphne(py::module& m)
{

  py::class_<DAPHNEFrame>(m, "DAPHNEFrame", py::buffer_protocol())
    .def(py::init())
    .def(py::init([](py::capsule capsule) {
        auto wfp = *static_cast<DAPHNEFrame*>(capsule.get_pointer());
        return wfp;
    } ))
    .def("get_adc", static_cast<uint16_t (DAPHNEFrame::*)(const int) const>(&DAPHNEFrame::get_adc))
    .def("get_t", static_cast<uint16_t (DAPHNEFrame::*)(const int) const>(&DAPHNEFrame::get_t))
    .def("get_timestamp", &DAPHNEFrame::get_timestamp)
    .def_static("sizeof", [](){ return sizeof(DAPHNEFrame); })
  ;

  py::class_<DAPHNEFrame::Header>(m, "DAPHNEHeader")
    .def_property_readonly("start_frame", [](DAPHNEFrame::Header& self) -> uint32_t {return self.start_frame;})
    .def_property_readonly("data_version", [](DAPHNEFrame::Header& self) -> uint32_t {return self.data_version;})
    .def_property_readonly("daphne", [](DAPHNEFrame::Header& self) -> uint32_t {return self.daphne;})
    .def_property_readonly("channel", [](DAPHNEFrame::Header& self) -> uint32_t {return self.channel;})
    .def_property_readonly("trigger_peak_height", [](DAPHNEFrame::Header& self) -> uint32_t {return self.trigger_peak_height;})
    .def_property_readonly("wf_length_in_words", [](DAPHNEFrame::Header& self) -> uint32_t {return self.wf_length_in_words;})
    .def_property_readonly("packet_counter", [](DAPHNEFrame::Header& self) -> uint32_t {return self.packet_counter;})
    .def_property_readonly("timestamp_wf_1", [](DAPHNEFrame::Header& self) -> uint32_t {return self.timestamp_wf_1;})
    .def_property_readonly("timestamp_wf_2", [](DAPHNEFrame::Header& self) -> uint32_t {return self.timestamp_wf_2;})

  ;
  py::class_<DAPHNEFrame::Trailer>(m, "DAPHNETrailer")
    .def_property_readonly("crc20", [](DAPHNEFrame::Trailer& self) -> uint32_t {return self.crc20;})
    .def_property_readonly("flex_word_12", [](DAPHNEFrame::Trailer& self) -> uint32_t {return self.flex_word_12;})
    .def_property_readonly("eof", [](DAPHNEFrame::Trailer& self) -> uint32_t {return self.eof;})
    .def_property_readonly("flex_word_24", [](DAPHNEFrame::Trailer& self) -> uint32_t {return self.flex_word_24;})
  ;

  m.def("np_array_adc_data", &np_array_adc_data);
  m.def("np_array_timestamp_data", &np_array_timestamp_data);
  m.def("np_array_adc", &np_array_adc);
  m.def("np_array_timestamp", &np_array_timestamp);

}

} // namespace python
} // namespace daphne
} // namespace detdataformats
} // namespace dunedaq
