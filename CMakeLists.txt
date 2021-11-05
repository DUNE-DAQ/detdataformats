cmake_minimum_required(VERSION 3.12)
project(detdataformats VERSION 3.2.0)

find_package(daq-cmake REQUIRED)
find_package(Boost 1.70.0 COMPONENTS unit_test_framework REQUIRED)

daq_setup_environment()


##############################################################################
# Main library

daq_add_library(*.cpp LINK_LIBRARIES)


##############################################################################
daq_add_python_bindings(*.cpp LINK_LIBRARIES ${PROJECT_NAME})

##############################################################################
# Plugins

##############################################################################
# Integration tests

##############################################################################
# Unit Tests
daq_add_unit_test(WIBFrame_test  LINK_LIBRARIES detdataformats)
daq_add_unit_test(WIB2Frame_test LINK_LIBRARIES detdataformats)

##############################################################################

daq_install()
