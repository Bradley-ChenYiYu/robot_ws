# generated from
# rosidl_cmake/cmake/template/rosidl_cmake_export_typesupport_targets.cmake.in

set(_exported_typesupport_targets
  "__rosidl_generator_c:grpr2f85_ifaces__rosidl_generator_c;__rosidl_typesupport_fastrtps_c:grpr2f85_ifaces__rosidl_typesupport_fastrtps_c;__rosidl_generator_cpp:grpr2f85_ifaces__rosidl_generator_cpp;__rosidl_typesupport_fastrtps_cpp:grpr2f85_ifaces__rosidl_typesupport_fastrtps_cpp;__rosidl_typesupport_introspection_c:grpr2f85_ifaces__rosidl_typesupport_introspection_c;__rosidl_typesupport_c:grpr2f85_ifaces__rosidl_typesupport_c;__rosidl_typesupport_introspection_cpp:grpr2f85_ifaces__rosidl_typesupport_introspection_cpp;__rosidl_typesupport_cpp:grpr2f85_ifaces__rosidl_typesupport_cpp;__rosidl_generator_py:grpr2f85_ifaces__rosidl_generator_py")

# populate grpr2f85_ifaces_TARGETS_<suffix>
if(NOT _exported_typesupport_targets STREQUAL "")
  # loop over typesupport targets
  foreach(_tuple ${_exported_typesupport_targets})
    string(REPLACE ":" ";" _tuple "${_tuple}")
    list(GET _tuple 0 _suffix)
    list(GET _tuple 1 _target)

    set(_target "grpr2f85_ifaces::${_target}")
    if(NOT TARGET "${_target}")
      # the exported target must exist
      message(WARNING "Package 'grpr2f85_ifaces' exports the typesupport target '${_target}' which doesn't exist")
    else()
      list(APPEND grpr2f85_ifaces_TARGETS${_suffix} "${_target}")
    endif()
  endforeach()
endif()
