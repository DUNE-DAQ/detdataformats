/**
 * @file mpd.cpp Python bindings for the MPDFrame format
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "detdataformats/mpd/MPDFrame.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace dunedaq {
namespace detdataformats {
namespace mpd {
namespace python {

void
register_mpd(py::module& m)
{

  py::class_<MPDFrame>(m, "MPDFrame", py::buffer_protocol())
    .def(py::init())
    .def(py::init([](py::capsule capsule) {
        auto wfp = *static_cast<MPDFrame*>(capsule.get_pointer());
        return wfp;
    } ))
    .def("get_timestamp", &MPDFrame::get_timestamp)
    .def("get_header", [](MPDFrame& self) -> const MPDFrame::MPDHeader& {return self.header;})
    .def_static("sizeof", [](){ return sizeof(MPDFrame); })
  ;

  py::class_<MPDFrame::MPDHeader>(m, "MPDHeader")
    .def_property_readonly("timestamp_sync", [](MPDFrame::MPDHeader& self) -> uint32_t {return self.timestamp_sync;})
    .def_property_readonly("timestamp_length", [](MPDFrame::MPDHeader& self) -> uint32_t {return self.timestamp_length;})
    .def_property_readonly("timestamp_OS", [](MPDFrame::MPDHeader& self) -> uint64_t {return self.timestamp_OS;})
    .def_property_readonly("SyncMagic", [](MPDFrame::MPDHeader& self) -> uint32_t {return self.SyncMagic;})
    .def_property_readonly("timestamp_sync", [](MPDFrame::MPDHeader& self) -> uint32_t {return self.timestamp_sync;})
    .def_property_readonly("length", [](MPDFrame::MPDHeader& self) -> uint32_t {return self.length;})
    .def_property_readonly("event_num", [](MPDFrame::MPDHeader& self) -> uint32_t {return self.event_num;})
    .def_property_readonly("device_serial_num", [](MPDFrame::MPDHeader& self) -> uint32_t {return self.device_serial_num;})
    .def_property_readonly("device_length", [](MPDFrame::MPDHeader& self) -> uint32_t {return self.device_length;})
    .def_property_readonly("device_model_id", [](MPDFrame::MPDHeader& self) -> uint32_t {return self.device_model_id;})
    .def_property_readonly("trigger_type", [](MPDFrame::MPDHeader& self) -> uint32_t {return self.trigger_type;})
    .def_property_readonly("trigger_length", [](MPDFrame::MPDHeader& self) -> uint32_t {return self.trigger_length;})
    .def_property_readonly("trigger_channel_number", [](MPDFrame::MPDHeader& self) -> uint32_t {return self.trigger_channel_number;})
    .def_property_readonly("event_timestamp_1", [](MPDFrame::MPDHeader& self) -> uint32_t {return self.event_timestamp_1;})
    .def_property_readonly("flags", [](MPDFrame::MPDHeader& self) -> uint32_t {return self.flags;})
    .def_property_readonly("event_timestamp_2", [](MPDFrame::MPDHeader& self) -> uint32_t {return self.event_timestamp_2;})
    .def_property_readonly("channel_bit_mask", [](MPDFrame::MPDHeader& self) -> uint32_t {return self.channel_bit_mask;})
    .def_property_readonly("data_type", [](MPDFrame::MPDHeader& self) -> uint32_t {return self.data_type;})
    .def_property_readonly("data_length", [](MPDFrame::MPDHeader& self) -> uint32_t {return self.data_length;})
    .def_property_readonly("channel_number", [](MPDFrame::MPDHeader& self) -> uint32_t {return self.channel_number;});
}

} // namespace python
} // namespace mpd
} // namespace detdataformats
} // namespace dunedaq
