// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from grpr2f85_ifaces:srv/SetGripperState.idl
// generated code does not contain a copyright notice

#ifndef GRPR2F85_IFACES__SRV__DETAIL__SET_GRIPPER_STATE__STRUCT_H_
#define GRPR2F85_IFACES__SRV__DETAIL__SET_GRIPPER_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/SetGripperState in the package grpr2f85_ifaces.
typedef struct grpr2f85_ifaces__srv__SetGripperState_Request
{
  uint8_t position;
  uint8_t speed;
  uint8_t force;
  uint16_t wait_time;
} grpr2f85_ifaces__srv__SetGripperState_Request;

// Struct for a sequence of grpr2f85_ifaces__srv__SetGripperState_Request.
typedef struct grpr2f85_ifaces__srv__SetGripperState_Request__Sequence
{
  grpr2f85_ifaces__srv__SetGripperState_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} grpr2f85_ifaces__srv__SetGripperState_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'result'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/SetGripperState in the package grpr2f85_ifaces.
typedef struct grpr2f85_ifaces__srv__SetGripperState_Response
{
  bool ok;
  uint8_t status_code;
  rosidl_runtime_c__String result;
} grpr2f85_ifaces__srv__SetGripperState_Response;

// Struct for a sequence of grpr2f85_ifaces__srv__SetGripperState_Response.
typedef struct grpr2f85_ifaces__srv__SetGripperState_Response__Sequence
{
  grpr2f85_ifaces__srv__SetGripperState_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} grpr2f85_ifaces__srv__SetGripperState_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // GRPR2F85_IFACES__SRV__DETAIL__SET_GRIPPER_STATE__STRUCT_H_
