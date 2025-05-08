// generated from rosidl_typesupport_cpp/resource/idl__type_support.cpp.em
// with input from navigation_interfaces:srv/NavigateToPose.idl
// generated code does not contain a copyright notice

#include "cstddef"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "navigation_interfaces/srv/detail/navigate_to_pose__struct.hpp"
#include "rosidl_typesupport_cpp/identifier.hpp"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_c/type_support_map.h"
#include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
#include "rosidl_typesupport_cpp/visibility_control.h"
#include "rosidl_typesupport_interface/macros.h"

namespace navigation_interfaces
{

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _NavigateToPose_Request_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _NavigateToPose_Request_type_support_ids_t;

static const _NavigateToPose_Request_type_support_ids_t _NavigateToPose_Request_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _NavigateToPose_Request_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _NavigateToPose_Request_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _NavigateToPose_Request_type_support_symbol_names_t _NavigateToPose_Request_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, navigation_interfaces, srv, NavigateToPose_Request)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, navigation_interfaces, srv, NavigateToPose_Request)),
  }
};

typedef struct _NavigateToPose_Request_type_support_data_t
{
  void * data[2];
} _NavigateToPose_Request_type_support_data_t;

static _NavigateToPose_Request_type_support_data_t _NavigateToPose_Request_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _NavigateToPose_Request_message_typesupport_map = {
  2,
  "navigation_interfaces",
  &_NavigateToPose_Request_message_typesupport_ids.typesupport_identifier[0],
  &_NavigateToPose_Request_message_typesupport_symbol_names.symbol_name[0],
  &_NavigateToPose_Request_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t NavigateToPose_Request_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_NavigateToPose_Request_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace navigation_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<navigation_interfaces::srv::NavigateToPose_Request>()
{
  return &::navigation_interfaces::srv::rosidl_typesupport_cpp::NavigateToPose_Request_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, navigation_interfaces, srv, NavigateToPose_Request)() {
  return get_message_type_support_handle<navigation_interfaces::srv::NavigateToPose_Request>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "navigation_interfaces/srv/detail/navigate_to_pose__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace navigation_interfaces
{

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _NavigateToPose_Response_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _NavigateToPose_Response_type_support_ids_t;

static const _NavigateToPose_Response_type_support_ids_t _NavigateToPose_Response_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _NavigateToPose_Response_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _NavigateToPose_Response_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _NavigateToPose_Response_type_support_symbol_names_t _NavigateToPose_Response_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, navigation_interfaces, srv, NavigateToPose_Response)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, navigation_interfaces, srv, NavigateToPose_Response)),
  }
};

typedef struct _NavigateToPose_Response_type_support_data_t
{
  void * data[2];
} _NavigateToPose_Response_type_support_data_t;

static _NavigateToPose_Response_type_support_data_t _NavigateToPose_Response_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _NavigateToPose_Response_message_typesupport_map = {
  2,
  "navigation_interfaces",
  &_NavigateToPose_Response_message_typesupport_ids.typesupport_identifier[0],
  &_NavigateToPose_Response_message_typesupport_symbol_names.symbol_name[0],
  &_NavigateToPose_Response_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t NavigateToPose_Response_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_NavigateToPose_Response_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace navigation_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<navigation_interfaces::srv::NavigateToPose_Response>()
{
  return &::navigation_interfaces::srv::rosidl_typesupport_cpp::NavigateToPose_Response_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, navigation_interfaces, srv, NavigateToPose_Response)() {
  return get_message_type_support_handle<navigation_interfaces::srv::NavigateToPose_Response>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "navigation_interfaces/srv/detail/navigate_to_pose__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
#include "rosidl_typesupport_cpp/service_type_support.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
#include "rosidl_typesupport_cpp/service_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace navigation_interfaces
{

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _NavigateToPose_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _NavigateToPose_type_support_ids_t;

static const _NavigateToPose_type_support_ids_t _NavigateToPose_service_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _NavigateToPose_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _NavigateToPose_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _NavigateToPose_type_support_symbol_names_t _NavigateToPose_service_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, navigation_interfaces, srv, NavigateToPose)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, navigation_interfaces, srv, NavigateToPose)),
  }
};

typedef struct _NavigateToPose_type_support_data_t
{
  void * data[2];
} _NavigateToPose_type_support_data_t;

static _NavigateToPose_type_support_data_t _NavigateToPose_service_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _NavigateToPose_service_typesupport_map = {
  2,
  "navigation_interfaces",
  &_NavigateToPose_service_typesupport_ids.typesupport_identifier[0],
  &_NavigateToPose_service_typesupport_symbol_names.symbol_name[0],
  &_NavigateToPose_service_typesupport_data.data[0],
};

static const rosidl_service_type_support_t NavigateToPose_service_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_NavigateToPose_service_typesupport_map),
  ::rosidl_typesupport_cpp::get_service_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace navigation_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_service_type_support_t *
get_service_type_support_handle<navigation_interfaces::srv::NavigateToPose>()
{
  return &::navigation_interfaces::srv::rosidl_typesupport_cpp::NavigateToPose_service_type_support_handle;
}

}  // namespace rosidl_typesupport_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_cpp, navigation_interfaces, srv, NavigateToPose)() {
  return ::rosidl_typesupport_cpp::get_service_type_support_handle<navigation_interfaces::srv::NavigateToPose>();
}

#ifdef __cplusplus
}
#endif
