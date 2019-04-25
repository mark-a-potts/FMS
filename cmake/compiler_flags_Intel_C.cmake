# (C) Copyright 2018 UCAR.
#
# This software is licensed under the terms of the Apache Licence Version 2.0
# which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.

####################################################################
# FLAGS COMMON TO ALL BUILD TYPES
####################################################################

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -sox -traceback" )

####################################################################
# RELEASE FLAGS
####################################################################

set( CMAKE_C_FLAGS_RELEASE     "-O2 -debug minimal" )

####################################################################
# DEBUG FLAGS
####################################################################

set( CMAKE_C_FLAGS_DEBUG       "-O0 -g -ftrapuv" )

####################################################################
# BIT REPRODUCIBLE FLAGS
####################################################################

set( CMAKE_C_FLAGS_BIT         "-O2 -debug minimal" )

####################################################################
# LINK FLAGS
####################################################################

set( CMAKE_C_LINK_FLAGS        "" )

####################################################################

# Meaning of flags
# ----------------
# todo

