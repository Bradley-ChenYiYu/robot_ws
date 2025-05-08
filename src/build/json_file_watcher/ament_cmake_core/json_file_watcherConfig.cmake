# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_json_file_watcher_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED json_file_watcher_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(json_file_watcher_FOUND FALSE)
  elseif(NOT json_file_watcher_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(json_file_watcher_FOUND FALSE)
  endif()
  return()
endif()
set(_json_file_watcher_CONFIG_INCLUDED TRUE)

# output package information
if(NOT json_file_watcher_FIND_QUIETLY)
  message(STATUS "Found json_file_watcher: 0.0.0 (${json_file_watcher_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'json_file_watcher' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${json_file_watcher_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(json_file_watcher_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${json_file_watcher_DIR}/${_extra}")
endforeach()
