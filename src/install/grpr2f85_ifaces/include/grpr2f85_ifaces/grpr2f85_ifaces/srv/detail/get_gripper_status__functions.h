// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from grpr2f85_ifaces:srv/GetGripperStatus.idl
// generated code does not contain a copyright notice

#ifndef GRPR2F85_IFACES__SRV__DETAIL__GET_GRIPPER_STATUS__FUNCTIONS_H_
#define GRPR2F85_IFACES__SRV__DETAIL__GET_GRIPPER_STATUS__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "grpr2f85_ifaces/msg/rosidl_generator_c__visibility_control.h"

#include "grpr2f85_ifaces/srv/detail/get_gripper_status__struct.h"

/// Initialize srv/GetGripperStatus message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * grpr2f85_ifaces__srv__GetGripperStatus_Request
 * )) before or use
 * grpr2f85_ifaces__srv__GetGripperStatus_Request__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
bool
grpr2f85_ifaces__srv__GetGripperStatus_Request__init(grpr2f85_ifaces__srv__GetGripperStatus_Request * msg);

/// Finalize srv/GetGripperStatus message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
void
grpr2f85_ifaces__srv__GetGripperStatus_Request__fini(grpr2f85_ifaces__srv__GetGripperStatus_Request * msg);

/// Create srv/GetGripperStatus message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * grpr2f85_ifaces__srv__GetGripperStatus_Request__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
grpr2f85_ifaces__srv__GetGripperStatus_Request *
grpr2f85_ifaces__srv__GetGripperStatus_Request__create();

/// Destroy srv/GetGripperStatus message.
/**
 * It calls
 * grpr2f85_ifaces__srv__GetGripperStatus_Request__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
void
grpr2f85_ifaces__srv__GetGripperStatus_Request__destroy(grpr2f85_ifaces__srv__GetGripperStatus_Request * msg);

/// Check for srv/GetGripperStatus message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
bool
grpr2f85_ifaces__srv__GetGripperStatus_Request__are_equal(const grpr2f85_ifaces__srv__GetGripperStatus_Request * lhs, const grpr2f85_ifaces__srv__GetGripperStatus_Request * rhs);

/// Copy a srv/GetGripperStatus message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
bool
grpr2f85_ifaces__srv__GetGripperStatus_Request__copy(
  const grpr2f85_ifaces__srv__GetGripperStatus_Request * input,
  grpr2f85_ifaces__srv__GetGripperStatus_Request * output);

/// Initialize array of srv/GetGripperStatus messages.
/**
 * It allocates the memory for the number of elements and calls
 * grpr2f85_ifaces__srv__GetGripperStatus_Request__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
bool
grpr2f85_ifaces__srv__GetGripperStatus_Request__Sequence__init(grpr2f85_ifaces__srv__GetGripperStatus_Request__Sequence * array, size_t size);

/// Finalize array of srv/GetGripperStatus messages.
/**
 * It calls
 * grpr2f85_ifaces__srv__GetGripperStatus_Request__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
void
grpr2f85_ifaces__srv__GetGripperStatus_Request__Sequence__fini(grpr2f85_ifaces__srv__GetGripperStatus_Request__Sequence * array);

/// Create array of srv/GetGripperStatus messages.
/**
 * It allocates the memory for the array and calls
 * grpr2f85_ifaces__srv__GetGripperStatus_Request__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
grpr2f85_ifaces__srv__GetGripperStatus_Request__Sequence *
grpr2f85_ifaces__srv__GetGripperStatus_Request__Sequence__create(size_t size);

/// Destroy array of srv/GetGripperStatus messages.
/**
 * It calls
 * grpr2f85_ifaces__srv__GetGripperStatus_Request__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
void
grpr2f85_ifaces__srv__GetGripperStatus_Request__Sequence__destroy(grpr2f85_ifaces__srv__GetGripperStatus_Request__Sequence * array);

/// Check for srv/GetGripperStatus message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
bool
grpr2f85_ifaces__srv__GetGripperStatus_Request__Sequence__are_equal(const grpr2f85_ifaces__srv__GetGripperStatus_Request__Sequence * lhs, const grpr2f85_ifaces__srv__GetGripperStatus_Request__Sequence * rhs);

/// Copy an array of srv/GetGripperStatus messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
bool
grpr2f85_ifaces__srv__GetGripperStatus_Request__Sequence__copy(
  const grpr2f85_ifaces__srv__GetGripperStatus_Request__Sequence * input,
  grpr2f85_ifaces__srv__GetGripperStatus_Request__Sequence * output);

/// Initialize srv/GetGripperStatus message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * grpr2f85_ifaces__srv__GetGripperStatus_Response
 * )) before or use
 * grpr2f85_ifaces__srv__GetGripperStatus_Response__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
bool
grpr2f85_ifaces__srv__GetGripperStatus_Response__init(grpr2f85_ifaces__srv__GetGripperStatus_Response * msg);

/// Finalize srv/GetGripperStatus message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
void
grpr2f85_ifaces__srv__GetGripperStatus_Response__fini(grpr2f85_ifaces__srv__GetGripperStatus_Response * msg);

/// Create srv/GetGripperStatus message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * grpr2f85_ifaces__srv__GetGripperStatus_Response__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
grpr2f85_ifaces__srv__GetGripperStatus_Response *
grpr2f85_ifaces__srv__GetGripperStatus_Response__create();

/// Destroy srv/GetGripperStatus message.
/**
 * It calls
 * grpr2f85_ifaces__srv__GetGripperStatus_Response__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
void
grpr2f85_ifaces__srv__GetGripperStatus_Response__destroy(grpr2f85_ifaces__srv__GetGripperStatus_Response * msg);

/// Check for srv/GetGripperStatus message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
bool
grpr2f85_ifaces__srv__GetGripperStatus_Response__are_equal(const grpr2f85_ifaces__srv__GetGripperStatus_Response * lhs, const grpr2f85_ifaces__srv__GetGripperStatus_Response * rhs);

/// Copy a srv/GetGripperStatus message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
bool
grpr2f85_ifaces__srv__GetGripperStatus_Response__copy(
  const grpr2f85_ifaces__srv__GetGripperStatus_Response * input,
  grpr2f85_ifaces__srv__GetGripperStatus_Response * output);

/// Initialize array of srv/GetGripperStatus messages.
/**
 * It allocates the memory for the number of elements and calls
 * grpr2f85_ifaces__srv__GetGripperStatus_Response__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
bool
grpr2f85_ifaces__srv__GetGripperStatus_Response__Sequence__init(grpr2f85_ifaces__srv__GetGripperStatus_Response__Sequence * array, size_t size);

/// Finalize array of srv/GetGripperStatus messages.
/**
 * It calls
 * grpr2f85_ifaces__srv__GetGripperStatus_Response__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
void
grpr2f85_ifaces__srv__GetGripperStatus_Response__Sequence__fini(grpr2f85_ifaces__srv__GetGripperStatus_Response__Sequence * array);

/// Create array of srv/GetGripperStatus messages.
/**
 * It allocates the memory for the array and calls
 * grpr2f85_ifaces__srv__GetGripperStatus_Response__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
grpr2f85_ifaces__srv__GetGripperStatus_Response__Sequence *
grpr2f85_ifaces__srv__GetGripperStatus_Response__Sequence__create(size_t size);

/// Destroy array of srv/GetGripperStatus messages.
/**
 * It calls
 * grpr2f85_ifaces__srv__GetGripperStatus_Response__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
void
grpr2f85_ifaces__srv__GetGripperStatus_Response__Sequence__destroy(grpr2f85_ifaces__srv__GetGripperStatus_Response__Sequence * array);

/// Check for srv/GetGripperStatus message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
bool
grpr2f85_ifaces__srv__GetGripperStatus_Response__Sequence__are_equal(const grpr2f85_ifaces__srv__GetGripperStatus_Response__Sequence * lhs, const grpr2f85_ifaces__srv__GetGripperStatus_Response__Sequence * rhs);

/// Copy an array of srv/GetGripperStatus messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_grpr2f85_ifaces
bool
grpr2f85_ifaces__srv__GetGripperStatus_Response__Sequence__copy(
  const grpr2f85_ifaces__srv__GetGripperStatus_Response__Sequence * input,
  grpr2f85_ifaces__srv__GetGripperStatus_Response__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // GRPR2F85_IFACES__SRV__DETAIL__GET_GRIPPER_STATUS__FUNCTIONS_H_
