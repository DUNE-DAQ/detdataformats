/**
 * @file wib2.cpp Python bindings for the WIB2Frame format
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "detdataformats/wib2/WIB2Frame.hpp"

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
    .def_property_readonly("version", [](WIB2Frame::Header& self) -> uint32_t {return self.version;})
    .def_property_readonly("detector_id", [](WIB2Frame::Header& self) -> uint32_t {return self.detector_id;})
    .def_property_readonly("crate", [](WIB2Frame::Header& self) -> uint32_t {return self.crate;})
    .def_property_readonly("slot", [](WIB2Frame::Header& self) -> uint32_t {return self.slot;})
    .def_property_readonly("link", [](WIB2Frame::Header& self) -> uint32_t {return self.link;})
    .def_property_readonly("timestamp_1", [](WIB2Frame::Header& self) -> uint32_t {return self.timestamp_1;})
    .def_property_readonly("timestamp_2", [](WIB2Frame::Header& self) -> uint32_t {return self.timestamp_2;})
    .def_property_readonly("colddata_timestamp_id", [](WIB2Frame::Header& self) -> uint32_t {return self.colddata_timestamp_id;})
    .def_property_readonly("femb_valid", [](WIB2Frame::Header& self) -> uint32_t {return self.femb_valid;})
    .def_property_readonly("link_mask", [](WIB2Frame::Header& self) -> uint32_t {return self.link_mask;})
    .def_property_readonly("lock_output_status", [](WIB2Frame::Header& self) -> uint32_t {return self.lock_output_status;})
    .def_property_readonly("femb_pulser_frame_bits", [](WIB2Frame::Header& self) -> uint32_t {return self.femb_pulser_frame_bits;})
    .def_property_readonly("femb_sync_flags", [](WIB2Frame::Header& self) -> uint32_t {return self.femb_sync_flags;})
    .def_property_readonly("colddata_timestamp", [](WIB2Frame::Header& self) -> uint32_t {return self.colddata_timestamp;})

  ;
  py::class_<WIB2Frame::Trailer>(m, "WIB2Trailer")
    .def_property_readonly("flex_bits", [](WIB2Frame::Trailer& self) -> uint32_t {return self.flex_bits;})
    .def_property_readonly("ws", [](WIB2Frame::Trailer& self) -> uint32_t {return self.ws;})
    .def_property_readonly("psr_cal", [](WIB2Frame::Trailer& self) -> uint32_t {return self.psr_cal;})
    .def_property_readonly("ready", [](WIB2Frame::Trailer& self) -> uint32_t {return self.ready;})
    .def_property_readonly("context_code", [](WIB2Frame::Trailer& self) -> uint32_t {return self.context_code;})
  ;

}

} // namespace python
} // namespace wib2
} // namespace detdataformats
} // namespace dunedaq
