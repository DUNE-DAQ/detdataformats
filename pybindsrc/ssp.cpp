/**
 * @file ssp.cpp
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "detdataformats/ssp/SSPTypes.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include <iomanip>

namespace py = pybind11;

namespace dunedaq {
namespace detdataformats {
namespace python {

void
register_ssp(py::module& m)
{ 
    py::enum_<ssp::Comm_t>(m, "Comm_t")
    .value("kUSB", ssp::kUSB)
    .value("kEthernet", ssp::kEthernet)
    .value("kEmulated", ssp::kEmulated)
    .export_values()
  ;

  m.attr("max_control_data") = ssp::max_control_data;
  
  py::enum_<commandConstants>(m, "commandConstants")
    .value("cmdNone", cmdNone)
    .value("cmdRead", cmdRead)
    .value("cmdReadMask", cmdReadMask)
    .value("cmdWrite", cmdWrite)
    .value("cmdWriteMask", cmdWriteMask)
    .value("cmdArrayRead", cmdArrayRead)
    .value("cmdArrayWrite", cmdArrayWrite)
    .value("cmdFifoRead", cmdFifoRead)
    .value("cmdFifoWrite", cmdFifoWrite)
    .value("numCommands", numCommands)
    .export_values() 
  ;

  py::enum_<statusConstants>(m, "statusConstants")
    .value("statusNoError", statusNoError)
    .value("statusSendError", statusSendError)
    .value("statusReceiveError", statusReceiveError)
    .value("statusTimeoutError", statusTimeoutError)
    .value("statusAddressError", statusAddressError)
    .value("statusAlignError", statusAlignError)
    .value("statusCommandError", statusCommandError)
    .value("statusSizeError", statusSizeError)
    .value("statusWriteError", statusWriteError)
    .export_values() 
  ;

  py::class_<MillisliceHeader> (m, "MillisliceHeader")
    .def(py::init<>())
    .def_readwrite("startTime", &MillisliceHeader::startTime)
    .def_readwrite("endTime", &MillisliceHeader::endTime)
    .def_readwrite("length", &MillisliceHeader::length)
    .def_readwrite("nTriggers", &MillisliceHeader::nTriggers)
    .def_readwrite("triggerType", &MillisliceHeader::triggerType)
    .def_readonly_static("sizeInUInts", &MillisliceHeader::sizeInUInts)
  ;

  py::class_<EventHeader> (m, "EventHeader", pybind11::buffer_protocol())
    .def(py::init<>())
    .def_readwrite("header", &EventHeader::header)
    .def_readwrite("length", &EventHeader::length)
    .def_readwrite("group1", &EventHeader::group1)
    .def_readwrite("triggerID", &EventHeader::triggerID)
    .def_readwrite("group2", &EventHeader::group2)
    .def_property("timestamp",
      [](EventHeader &self) -> pybind11::array {
            auto dtype = py::dtype(py::format_descriptor<unsigned short>::format());
            auto base = py::array(dtype, {4}, {sizeof(unsigned short)});
            return pybind11::array(dtype, {4}, {sizeof(unsigned short)}, self.timestamp, base);
        },
      [](EventHeader& self,const std::vector<unsigned short>& timestamp){std::copy(&timestamp[0], &timestamp[0]+4, self.timestamp);}, 
      py::return_value_policy::reference_internal)
    .def_readwrite("peakSumLow", &EventHeader::peakSumLow)
    .def_readwrite("group3", &EventHeader::group3)
    .def_readwrite("preriseLow", &EventHeader::preriseLow)
    .def_readwrite("group4", &EventHeader::group4)
    .def_readwrite("intSumHigh", &EventHeader::intSumHigh)
    .def_readwrite("baseline", &EventHeader::baseline)
    .def_property("cfdPoint",
      [](EventHeader &self) -> pybind11::array {
            auto dtype = py::dtype(py::format_descriptor<unsigned short>::format());
            auto base = py::array(dtype, {4}, {sizeof(unsigned short)});
            return pybind11::array(dtype, {4}, {sizeof(unsigned short)}, self.cfdPoint, base);
        },
      [](EventHeader& self,const std::vector<unsigned short>& cfdPoint){std::copy(&cfdPoint[0], &cfdPoint[0]+4, self.cfdPoint);}, 
      py::return_value_policy::reference_internal)
    .def_property("intTimestamp",
      [](EventHeader &self) -> pybind11::array {
            auto dtype = py::dtype(py::format_descriptor<unsigned short>::format());
            auto base = py::array(dtype, {4}, {sizeof(unsigned short)});
            return pybind11::array(dtype, {4}, {sizeof(unsigned short)}, self.intTimestamp, base);
        },
      [](EventHeader& self,const std::vector<unsigned short>& intTimestamp){std::copy(&intTimestamp[0], &intTimestamp[0]+4, self.intTimestamp);},
      py::return_value_policy::reference_internal)
  ;
  py::class_<CtrlHeader> (m, "CtrlHeader", pybind11::buffer_protocol())
    .def(py::init<>())
    .def_readwrite("length", &CtrlHeader::length)
    .def_readwrite("address", &CtrlHeader::address)
    .def_readwrite("command", &CtrlHeader::command)
    .def_readwrite("size", &CtrlHeader::size)
    .def_readwrite("status", &CtrlHeader::status)
  ;
  py::class_<CtrlPacket> (m, "CtrlPacket")
    .def(py::init<>())
    .def_readwrite("header", &CtrlPacket::header)
    .def_property("data",
      [](CtrlPacket &self) -> pybind11::array {
        auto dtype = py::dtype(py::format_descriptor<unsigned int>::format());
        auto base = py::array(dtype, {ssp::max_control_data}, {sizeof(unsigned int)});
        return pybind11::array(dtype, {ssp::max_control_data}, {sizeof(unsigned int)}, self.data, base);
      },
      [](CtrlPacket& self,const std::vector<unsigned int>& data){std::copy(&data[0], &data[0]+ssp::max_control_data, self.data);}, 
      py::return_value_policy::reference_internal)
  ;
}
} // namespace python
} // namespace detdataformats
} // namespace dunedaq