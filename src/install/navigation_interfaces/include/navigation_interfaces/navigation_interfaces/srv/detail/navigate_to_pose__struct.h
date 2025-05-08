// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from navigation_interfaces:srv/NavigateToPose.idl
// generated code does not contain a copyright notice

#ifndef NAVIGATION_INTERFACES__SRV__DETAIL__NAVIGATE_TO_POSE__STRUCT_H_
#define NAVIGATION_INTERFACES__SRV__DETAIL__NAVIGATE_TO_POSE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/NavigateToPose in the package navigation_interfaces.
typedef struct navigation_interfaces__srv__NavigateToPose_Request
{
  float x;
  float y;
  float z;
  float w;
} navigation_interfaces__srv__NavigateToPose_Request;

// Struct for a sequence of navigation_interfaces__srv__NavigateToPose_Request.
typedef struct navigation_interfaces__srv__NavigateToPose_Request__Sequence
{
  navigation_interfaces__srv__NavigateToPose_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} navigation_interfaces__srv__NavigateToPose_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'message'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/NavigateToPose in the package navigation_interfaces.
typedef struct navigation_interfaces__srv__NavigateToPose_Response
{
  bool success;
  rosidl_runtime_c__String message;
} navigation_interfaces__srv__NavigateToPose_Response;

// Struct for a sequence of navigation_interfaces__srv__NavigateToPose_Response.
typedef struct navigation_interfaces__srv__NavigateToPose_Response__Sequence
{
  navigation_interfaces__srv__NavigateToPose_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} navigation_interfaces__srv__NavigateToPose_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // NAVIGATION_INTERFACES__SRV__DETAIL__NAVIGATE_TO_POSE__STRUCT_H_
