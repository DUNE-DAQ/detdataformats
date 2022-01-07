/**
 * @file wib2.cpp Python bindings for the WIB2Frame format
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "detdataformats/wib2/WIB2Frame.hpp"
#include "WIB2Unpacker.cc"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace dunedaq {
namespace detdataformats {
namespace wib2 {
namespace python {

void
register_wib2(py::module& m)
{

  py::class_<WIB2Frame>(m, "WIB2Frame", py::buffer_protocol())
    .def(py::init())
    .def(py::init([](py::capsule capsule) {
        auto wfp = *static_cast<WIB2Frame*>(capsule.get_pointer());
        return wfp;
    } ))
    .def("get_adc", static_cast<uint16_t (WIB2Frame::*)(const int) const>(&WIB2Frame::get_adc))
    .def("get_u", static_cast<uint16_t (WIB2Frame::*)(const int, const int) const>(&WIB2Frame::get_u))
    .def("get_v", static_cast<uint16_t (WIB2Frame::*)(const int, const int) const>(&WIB2Frame::get_v))
    .def("get_x", static_cast<uint16_t (WIB2Frame::*)(const int, const int) const>(&WIB2Frame::get_x))
    .def("get_timestamp", &WIB2Frame::get_timestamp)
    .def_static("sizeof", [](){ return sizeof(WIB2Frame); })
  ;

  py::class_<WIB2Frame::Header>(m, "WIB2Header")
    .def_property_readonly("start_frame", [](WIB2Frame::Header& self) -> uint32_t {return self.start_frame;})
    .def_property_readonly("crate", [](WIB2Frame::Header& self) -> uint32_t {return self.crate;})
    .def_property_readonly("frame_version", [](WIB2Frame::Header& self) -> uint32_t {return self.frame_version;})
    .def_property_readonly("slot", [](WIB2Frame::Header& self) -> uint32_t {return self.slot;})
    .def_property_readonly("fiber", [](WIB2Frame::Header& self) -> uint32_t {return self.fiber;})
    .def_property_readonly("femb_valid", [](WIB2Frame::Header& self) -> uint32_t {return self.femb_valid;})
    .def_property_readonly("wib_code_1", [](WIB2Frame::Header& self) -> uint32_t {return self.wib_code_1;})
    .def_property_readonly("wib_code_2", [](WIB2Frame::Header& self) -> uint32_t {return self.wib_code_2;})
    .def_property_readonly("timestamp_1", [](WIB2Frame::Header& self) -> uint32_t {return self.timestamp_1;})
    .def_property_readonly("timestamp_2", [](WIB2Frame::Header& self) -> uint32_t {return self.timestamp_2;})

  ;
  py::class_<WIB2Frame::Trailer>(m, "WIB2Trailer")
    .def_property_readonly("crc20", [](WIB2Frame::Trailer& self) -> uint32_t {return self.crc20;})
    .def_property_readonly("flex_word_12", [](WIB2Frame::Trailer& self) -> uint32_t {return self.flex_word_12;})
    .def_property_readonly("eof", [](WIB2Frame::Trailer& self) -> uint32_t {return self.eof;})
    .def_property_readonly("flex_word_24", [](WIB2Frame::Trailer& self) -> uint32_t {return self.flex_word_24;})
  ;

  m.def("np_array_adc", &np_array_adc);
  m.def("np_array_timestamp", &np_array_timestamp);

}

} // namespace python
} // namespace wib2
} // namespace detdataformats
} // namespace dunedaq
