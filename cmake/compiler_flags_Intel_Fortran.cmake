# (C) Copyright 2018-2020 UCAR.
#
# This software is licensed under the terms of the Apache Licence Version 2.0
# which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.

# Redefine the ecbuild defaults but without -heap-arrays 32
set( CMAKE_Fortran_FLAGS_RELEASE        "-O3 -DNDEBUG -unroll -inline " )
set( CMAKE_Fortran_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG "              )
set( CMAKE_Fortran_FLAGS_BIT            "-O2 -DNDEBUG -unroll -inline " )
set( CMAKE_Fortran_FLAGS_DEBUG          "-O0 -g -traceback -check all " )
set( CMAKE_Fortran_FLAGS_PRODUCTION     "-O3 -g "                       )

