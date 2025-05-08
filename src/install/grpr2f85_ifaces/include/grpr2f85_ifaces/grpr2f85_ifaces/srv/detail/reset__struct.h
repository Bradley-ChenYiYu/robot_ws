// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from grpr2f85_ifaces:srv/Reset.idl
// generated code does not contain a copyright notice

#ifndef GRPR2F85_IFACES__SRV__DETAIL__RESET__STRUCT_H_
#define GRPR2F85_IFACES__SRV__DETAIL__RESET__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/Reset in the package grpr2f85_ifaces.
typedef struct grpr2f85_ifaces__srv__Reset_Request
{
  bool get;
} grpr2f85_ifaces__srv__Reset_Request;

// Struct for a sequence of grpr2f85_ifaces__srv__Reset_Request.
typedef struct grpr2f85_ifaces__srv__Reset_Request__Sequence
{
  grpr2f85_ifaces__srv__Reset_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} grpr2f85_ifaces__srv__Reset_Request__Sequence;


// Constants defined in the message

/// Struct defined in srv/Reset in the package grpr2f85_ifaces.
typedef struct grpr2f85_ifaces__srv__Reset_Response
{
  bool ok;
} grpr2f85_ifaces__srv__Reset_Response;

// Struct for a sequence of grpr2f85_ifaces__srv__Reset_Response.
typedef struct grpr2f85_ifaces__srv__Reset_Response__Sequence
{
  grpr2f85_ifaces__srv__Reset_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} grpr2f85_ifaces__srv__Reset_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // GRPR2F85_IFACES__SRV__DETAIL__RESET__STRUCT_H_
