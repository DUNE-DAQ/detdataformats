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
    .def("get_OSheader", [](MPDFrame& self) -> const MPDFrame::MPDOSHeader& {return self.OSheader;})
    .def("get_event_header", [](MPDFrame& self) -> const MPDFrame::MPDEventHeader& {return self.event_header;})
    .def("get_device_header", [](MPDFrame& self) -> const MPDFrame::MPDDeviceHeader& {return self.device_header;})
    .def("get_trigger_header", [](MPDFrame& self) -> const MPDFrame::MPDTriggerHeader& {return self.trigger_header;})
    .def("get_trigger_data_header", [](MPDFrame& self) -> const MPDFrame::MPDTriggerDataHeader& {return self.trigger_data_header;})
    .def("get_data", &MPDFrame::get_data)
    .def("get_frame_size", &MPDFrame::get_frame_size)
    .def("get_nchannels", &MPDFrame::get_nchannels)
    .def("get_nsamples", &MPDFrame::get_nsamples)
  ;

  py::class_<MPDFrame::MPDOSHeader>(m, "MPDOSHeader")
    .def_property_readonly("timestamp_sync", [](MPDFrame::MPDOSHeader& self) -> uint32_t {return self.timestamp_sync;})
    .def_property_readonly("timestamp_length", [](MPDFrame::MPDOSHeader& self) -> uint32_t {return self.timestamp_length;})
    .def_property_readonly("timestamp_OS", [](MPDFrame::MPDOSHeader& self) -> uint64_t {return self.timestamp_OS;});

  py::class_<MPDFrame::MPDEventHeader>(m, "MPDEventHeader")
    .def_property_readonly("SyncMagic", [](MPDFrame::MPDEventHeader& self) -> uint32_t {return self.SyncMagic;})
    .def_property_readonly("length", [](MPDFrame::MPDEventHeader& self) -> uint32_t {return self.length;})
    .def_property_readonly("event_num", [](MPDFrame::MPDEventHeader& self) -> uint32_t {return self.event_num;});

  py::class_<MPDFrame::MPDDeviceHeader>(m, "MPDDeviceHeader")
    .def_property_readonly("device_serial_num", [](MPDFrame::MPDDeviceHeader& self) -> uint32_t {return self.device_serial_num;})
    .def_property_readonly("device_length", [](MPDFrame::MPDDeviceHeader& self) -> uint32_t {return self.device_length;})
    .def_property_readonly("device_model_id", [](MPDFrame::MPDDeviceHeader& self) -> uint32_t {return self.device_model_id;});

  py::class_<MPDFrame::MPDTriggerHeader>(m, "MPDTriggerHeader")
    .def_property_readonly("trigger_type", [](MPDFrame::MPDTriggerHeader& self) -> uint32_t {return self.trigger_type;})
    .def_property_readonly("trigger_length", [](MPDFrame::MPDTriggerHeader& self) -> uint32_t {return self.trigger_length;})
    .def_property_readonly("trigger_channel_number", [](MPDFrame::MPDTriggerHeader& self) -> uint32_t {return self.trigger_channel_number;});

  py::class_<MPDFrame::MPDTriggerDataHeader>(m, "MPDTriggerDataHeader")
    .def_property_readonly("event_timestamp_1", [](MPDFrame::MPDTriggerDataHeader& self) -> uint32_t {return self.event_timestamp_1;})
    .def_property_readonly("flags", [](MPDFrame::MPDTriggerDataHeader& self) -> uint32_t {return self.flags;})
    .def_property_readonly("event_timestamp_2", [](MPDFrame::MPDTriggerDataHeader& self) -> uint32_t {return self.event_timestamp_2;})
    .def_property_readonly("channel_bit_mask", [](MPDFrame::MPDTriggerDataHeader& self) -> uint32_t {return self.channel_bit_mask;});
}

} // namespace python
} // namespace mpd
} // namespace detdataformats
} // namespace dunedaq
