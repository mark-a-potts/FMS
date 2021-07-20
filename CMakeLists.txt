# (C) Copyright 2017-2020 UCAR.
#
# This software is licensed under the terms of the Apache Licence Version 2.0
# which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.

################################################################################
# FMS
################################################################################

cmake_minimum_required( VERSION 3.12 )
project( fms VERSION 2020.4.0 LANGUAGES C Fortran )

## Ecbuild integration
find_package( ecbuild 3.3.2 REQUIRED )
include( ecbuild_system NO_POLICY_SCOPE )
ecbuild_declare_project()
list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake )
set( CMAKE_DIRECTORY_LABELS ${PROJECT_NAME} )

include( fms_compiler_flags )

################################################################################
# Dependencies
################################################################################

find_package( jedicmake QUIET)
find_package( MPI REQUIRED COMPONENTS C Fortran )
find_package( OpenMP COMPONENTS C Fortran )
find_package( NetCDF REQUIRED COMPONENTS Fortran )

################################################################################
# Sources
################################################################################

list( APPEND fms_hdr_files
include/file_version.h
include/fms_platform.h
)

list( APPEND fms_src_files
  affinity/affinity.c
  affinity/fms_affinity.F90
  amip_interp/amip_interp.F90
  amip_interp/amip_interp.rey_oi.txt
  astronomy/astronomy.F90
  astronomy/astronomy.tech.ps
  axis_utils/axis_utils2.F90
  axis_utils/axis_utils.F90
  block_control/block_control.F90
  column_diagnostics/column_diagnostics.F90
  constants/constants.F90
  constants/fmsconstants.F90
  coupler/atmos_ocean_fluxes.F90
  coupler/coupler_types.F90
  coupler/ensemble_manager.F90
  data_override/data_override.F90
  data_override/get_grid_version_fms2io.F90
  data_override/get_grid_version_mpp.F90
  diag_integral/diag_integral.F90
  diag_manager/diag_axis.F90
  diag_manager/diag_data.F90
  diag_manager/diag_grid.F90
  diag_manager/diag_manager.F90
  diag_manager/diag_output.F90
  diag_manager/diag_table.F90
  diag_manager/diag_util.F90
  drifters/cloud_interpolator.F90
  drifters/drifters_combine
  drifters/drifters_comm.F90
  drifters/drifters_compute_k.h
  drifters/drifters_core.F90
  drifters/drifters.F90
  drifters/drifters_input.F90
  drifters/drifters_io.F90
  drifters/drifters_push.h
  drifters/drifters_set_field.h
  drifters/fms_switches.h
  drifters/quicksort.F90
  exchange/stock_constants.F90
  exchange/xgrid.F90
  field_manager/field_manager.F90
  field_manager/fm_util.F90
  field_manager/parse.inc
  fms2_io/blackboxio.F90
  fms2_io/fms2_io.F90
  fms2_io/fms_io_utils.F90
  fms2_io/fms_netcdf_domain_io.F90
  fms2_io/fms_netcdf_unstructured_domain_io.F90
  fms2_io/netcdf_io.F90
  fms/fms.F90
  fms/fms_io.F90
  fms/fms_io_unstructured_field_exist.inc
  fms/fms_io_unstructured_file_unit.inc
  fms/fms_io_unstructured_get_field_size.inc
  fms/fms_io_unstructured_get_file_name.inc
  fms/fms_io_unstructured_get_file_unit.inc
  fms/fms_io_unstructured_read.inc
  fms/fms_io_unstructured_register_restart_axis.inc
  fms/fms_io_unstructured_register_restart_field.inc
  fms/fms_io_unstructured_save_restart.inc
  fms/fms_io_unstructured_setup_one_field.inc
  fms/read_data_2d.inc
  fms/read_data_3d.inc
  fms/read_data_4d.inc
  fms/write_data.inc
  horiz_interp/horiz_interp_bicubic.F90
  horiz_interp/horiz_interp_bilinear.F90
  horiz_interp/horiz_interp_conserve.F90
  horiz_interp/horiz_interp.F90
  horiz_interp/horiz_interp_spherical.F90
  horiz_interp/horiz_interp_type.F90
  include/file_version.h
  include/fms_platform.h
  interpolator/interpolator.F90
  memutils/memutils.F90
  monin_obukhov/monin_obukhov.F90
  monin_obukhov/monin_obukhov_inter.F90
  mosaic2/grid2.F90
  mosaic2/mosaic2.F90
  mosaic/constant.h
  mosaic/create_xgrid.c
  mosaic/create_xgrid.h
  mosaic/gradient_c2l.c
  mosaic/gradient_c2l.h
  mosaic/gradient.F90
  mosaic/grid.F90
  mosaic/interp.c
  mosaic/interp.h
  mosaic/mosaic.F90
  mosaic/mosaic_util.c
  mosaic/mosaic_util.h
  mosaic/read_mosaic.c
  mosaic/read_mosaic.h
  mpp/include/group_update_pack.inc
  mpp/include/group_update_unpack.inc
  mpp/include/mpp_alltoall_mpi.h
  mpp/include/mpp_alltoall_nocomm.h
  mpp/include/mpp_chksum.h
  mpp/include/mpp_chksum_int.h
  mpp/include/mpp_chksum_scalar.h
  mpp/include/mpp_comm.inc
  mpp/include/mpp_comm_mpi.inc
  mpp/include/mpp_comm_nocomm.inc
  mpp/include/mpp_data_mpi.inc
  mpp/include/mpp_data_nocomm.inc
  mpp/include/mpp_define_nest_domains.inc
  mpp/include/mpp_do_check.h
  mpp/include/mpp_do_checkV.h
  mpp/include/mpp_do_get_boundary_ad.h
  mpp/include/mpp_do_get_boundary.h
  mpp/include/mpp_do_global_field_ad.h
  mpp/include/mpp_do_global_field.h
  mpp/include/mpp_domains_comm.inc
  mpp/include/mpp_domains_define.inc
  mpp/include/mpp_domains_misc.inc
  mpp/include/mpp_domains_reduce.inc
  mpp/include/mpp_domains_util.inc
  mpp/include/mpp_do_redistribute.h
  mpp/include/mpp_do_update_ad.h
  mpp/include/mpp_do_update.h
  mpp/include/mpp_do_update_nest.h
  mpp/include/mpp_do_update_nonblock.h
  mpp/include/mpp_do_updateV_ad.h
  mpp/include/mpp_do_updateV.h
  mpp/include/mpp_do_updateV_nonblock.h
  mpp/include/mpp_error_a_a.h
  mpp/include/mpp_error_a_s.h
  mpp/include/mpp_error_s_a.h
  mpp/include/mpp_error_s_s.h
  mpp/include/mpp_gather.h
  mpp/include/mpp_get_boundary_ad.h
  mpp/include/mpp_get_boundary.h
  mpp/include/mpp_global_field_ad.h
  mpp/include/mpp_global_field.h
  mpp/include/mpp_global_field_ug.h
  mpp/include/mpp_global_reduce.h
  mpp/include/mpp_global_sum_ad.h
  mpp/include/mpp_global_sum.h
  mpp/include/mpp_global_sum_tl.h
  mpp/include/mpp_group_update.h
  mpp/include/mpp_io_connect.inc
  mpp/include/mpp_io_misc.inc
  mpp/include/mpp_io_read.inc
  mpp/include/mpp_io_unstructured_read.inc
  mpp/include/mpp_io_unstructured_write.inc
  mpp/include/mpp_io_util.inc
  mpp/include/mpp_io_write.inc
  mpp/include/mpp_read_2Ddecomp.h
  mpp/include/mpp_read_compressed.h
  mpp/include/mpp_read_distributed_ascii.h
  mpp/include/mpp_read_distributed_ascii.inc
  mpp/include/mpp_reduce_mpi.h
  mpp/include/mpp_reduce_nocomm.h
  mpp/include/mpp_scatter.h
  mpp/include/mpp_sum_ad.inc
  mpp/include/mpp_sum.inc
  mpp/include/mpp_sum_mpi_ad.h
  mpp/include/mpp_sum_mpi.h
  mpp/include/mpp_sum_nocomm_ad.h
  mpp/include/mpp_sum_nocomm.h
  mpp/include/mpp_transmit.inc
  mpp/include/mpp_transmit_mpi.h
  mpp/include/mpp_transmit_nocomm.h
  mpp/include/mpp_type_mpi.h
  mpp/include/mpp_type_nocomm.h
  mpp/include/mpp_unstruct_domain.inc
  mpp/include/mpp_unstruct_pass_data.h
  mpp/include/mpp_update_domains2D_ad.h
  mpp/include/mpp_update_domains2D.h
  mpp/include/mpp_update_domains2D_nonblock.h
  mpp/include/mpp_update_nest_domains.h
  mpp/include/mpp_util.inc
  mpp/include/mpp_util_mpi.inc
  mpp/include/mpp_util_nocomm.inc
  mpp/include/mpp_write_2Ddecomp.h
  mpp/include/mpp_write_compressed.h
  mpp/include/mpp_write.h
  mpp/include/mpp_write_unlimited_axis.h
  mpp/include/system_clock.h
  mpp/mpp_data.F90
  mpp/mpp_domains.F90
  mpp/mpp_efp.F90
  mpp/mpp.F90
  mpp/mpp_io.F90
  mpp/mpp_memuse.c
  mpp/mpp_memutils.F90
  mpp/mpp_parameter.F90
  mpp/mpp_pset.F90
  mpp/mpp_utilities.F90
  platform/platform.F90
  random_numbers/mersennetwister.F90
  random_numbers/random_numbers.F90
  sat_vapor_pres/sat_vapor_pres.F90
  sat_vapor_pres/sat_vapor_pres_k.F90
  time_interp/time_interp_external2.F90
  time_interp/time_interp_external.F90
  time_interp/time_interp.F90
  time_manager/get_cal_time.F90
  time_manager/time_manager.F90
  topography/gaussian_topog.F90
  topography/topography.F90
  tracer_manager/tracer_manager.F90
  tridiagonal/tridiagonal.F90
)
################################################################################

################################################################################
ecbuild_add_library( TARGET   fms
                     SOURCES  ${fms_hdr_files} ${fms_src_files}
                     INSTALL_HEADERS_LIST ${fms_hdr_files}
                     LINKER_LANGUAGE ${FMS_LINKER_LANGUAGE}
                    )

# Link dependencies
target_link_libraries( fms PUBLIC NetCDF::NetCDF_Fortran )
target_link_libraries( fms PUBLIC MPI::MPI_Fortran )
target_link_libraries( fms PUBLIC OpenMP::OpenMP_C OpenMP::OpenMP_Fortran )

################################################################################

# Disable compiler checks for GFortran >= 10.x
if(CMAKE_Fortran_COMPILER_ID MATCHES GNU AND CMAKE_Fortran_COMPILER_VERSION VERSION_GREATER_EQUAL 10)
    target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:Fortran>:-fallow-invalid-boz>
                                                   $<$<COMPILE_LANGUAGE:Fortran>:-fallow-argument-mismatch>)
endif()

if(UNIX AND NOT APPLE)
    #FIX for linking error with glibc>=2.30 which includes gettid in unistd.h if macro _GNU_SOURCE is set
    include(CheckSymbolExists)
    include(CMakePushCheckState)
    cmake_push_check_state(RESET)
    set(CMAKE_REQUIRED_DEFINITIONS "-D_GNU_SOURCE=1")
    check_symbol_exists("gettid" "unistd.h" HAVE_GETTID)
    unset(CMAKE_REQUIRED_DEFINITIONS)
    cmake_pop_check_state() # Restore any modified state
    if(HAVE_GETTID)
        target_compile_definitions(fms PRIVATE HAVE_GETTID)
    endif()
endif()


# Fortran module output directory for build and install interfaces
set(MODULE_DIR ${PROJECT_NAME}/module)
set_target_properties(${PROJECT_NAME} PROPERTIES Fortran_MODULE_DIRECTORY ${CMAKE_BINARY_DIR}/${MODULE_DIR})
install(DIRECTORY ${CMAKE_BINARY_DIR}/${MODULE_DIR}/ DESTINATION ${CMAKE_INSTALL_LIBDIR}/${MODULE_DIR})
target_include_directories(${PROJECT_NAME} INTERFACE
                            $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/${MODULE_DIR}>
                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_LIBDIR}/${MODULE_DIR}>)

# Private .inc include locations
target_include_directories( fms PRIVATE
                    ${CMAKE_CURRENT_SOURCE_DIR}/mpp/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/fms2_io/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/fms)

# Installed .h include locations (fms_hdr_files)
target_include_directories( fms PUBLIC
                    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> )

# Export package config
ecbuild_install_project( NAME fms )
ecbuild_print_summary()
