cmake_minimum_required(VERSION 3.12)
project(detdataformats VERSION 3.7.1)

find_package(daq-cmake REQUIRED)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

daq_setup_environment()


##############################################################################
# Main library

daq_add_library(LINK_LIBRARIES)


##############################################################################

message(WARNING "Skipping Python bindings for now")
#daq_add_python_bindings(*.cpp LINK_LIBRARIES ${PROJECT_NAME})

##############################################################################
# Plugins

##############################################################################
# Integration tests

##############################################################################
# Unit Tests
daq_add_unit_test(DetID_test                LINK_LIBRARIES detdataformats)
##############################################################################

daq_install()
