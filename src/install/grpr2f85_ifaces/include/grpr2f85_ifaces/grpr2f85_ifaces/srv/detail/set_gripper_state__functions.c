// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from grpr2f85_ifaces:srv/SetGripperState.idl
// generated code does not contain a copyright notice
#include "grpr2f85_ifaces/srv/detail/set_gripper_state__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"

bool
grpr2f85_ifaces__srv__SetGripperState_Request__init(grpr2f85_ifaces__srv__SetGripperState_Request * msg)
{
  if (!msg) {
    return false;
  }
  // position
  msg->position = 0;
  // speed
  msg->speed = 255;
  // force
  msg->force = 255;
  // wait_time
  msg->wait_time = 0;
  return true;
}

void
grpr2f85_ifaces__srv__SetGripperState_Request__fini(grpr2f85_ifaces__srv__SetGripperState_Request * msg)
{
  if (!msg) {
    return;
  }
  // position
  // speed
  // force
  // wait_time
}

bool
grpr2f85_ifaces__srv__SetGripperState_Request__are_equal(const grpr2f85_ifaces__srv__SetGripperState_Request * lhs, const grpr2f85_ifaces__srv__SetGripperState_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // position
  if (lhs->position != rhs->position) {
    return false;
  }
  // speed
  if (lhs->speed != rhs->speed) {
    return false;
  }
  // force
  if (lhs->force != rhs->force) {
    return false;
  }
  // wait_time
  if (lhs->wait_time != rhs->wait_time) {
    return false;
  }
  return true;
}

bool
grpr2f85_ifaces__srv__SetGripperState_Request__copy(
  const grpr2f85_ifaces__srv__SetGripperState_Request * input,
  grpr2f85_ifaces__srv__SetGripperState_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // position
  output->position = input->position;
  // speed
  output->speed = input->speed;
  // force
  output->force = input->force;
  // wait_time
  output->wait_time = input->wait_time;
  return true;
}

grpr2f85_ifaces__srv__SetGripperState_Request *
grpr2f85_ifaces__srv__SetGripperState_Request__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  grpr2f85_ifaces__srv__SetGripperState_Request * msg = (grpr2f85_ifaces__srv__SetGripperState_Request *)allocator.allocate(sizeof(grpr2f85_ifaces__srv__SetGripperState_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(grpr2f85_ifaces__srv__SetGripperState_Request));
  bool success = grpr2f85_ifaces__srv__SetGripperState_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
grpr2f85_ifaces__srv__SetGripperState_Request__destroy(grpr2f85_ifaces__srv__SetGripperState_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    grpr2f85_ifaces__srv__SetGripperState_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
grpr2f85_ifaces__srv__SetGripperState_Request__Sequence__init(grpr2f85_ifaces__srv__SetGripperState_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  grpr2f85_ifaces__srv__SetGripperState_Request * data = NULL;

  if (size) {
    data = (grpr2f85_ifaces__srv__SetGripperState_Request *)allocator.zero_allocate(size, sizeof(grpr2f85_ifaces__srv__SetGripperState_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = grpr2f85_ifaces__srv__SetGripperState_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        grpr2f85_ifaces__srv__SetGripperState_Request__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
grpr2f85_ifaces__srv__SetGripperState_Request__Sequence__fini(grpr2f85_ifaces__srv__SetGripperState_Request__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      grpr2f85_ifaces__srv__SetGripperState_Request__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

grpr2f85_ifaces__srv__SetGripperState_Request__Sequence *
grpr2f85_ifaces__srv__SetGripperState_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  grpr2f85_ifaces__srv__SetGripperState_Request__Sequence * array = (grpr2f85_ifaces__srv__SetGripperState_Request__Sequence *)allocator.allocate(sizeof(grpr2f85_ifaces__srv__SetGripperState_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = grpr2f85_ifaces__srv__SetGripperState_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
grpr2f85_ifaces__srv__SetGripperState_Request__Sequence__destroy(grpr2f85_ifaces__srv__SetGripperState_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    grpr2f85_ifaces__srv__SetGripperState_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
grpr2f85_ifaces__srv__SetGripperState_Request__Sequence__are_equal(const grpr2f85_ifaces__srv__SetGripperState_Request__Sequence * lhs, const grpr2f85_ifaces__srv__SetGripperState_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!grpr2f85_ifaces__srv__SetGripperState_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
grpr2f85_ifaces__srv__SetGripperState_Request__Sequence__copy(
  const grpr2f85_ifaces__srv__SetGripperState_Request__Sequence * input,
  grpr2f85_ifaces__srv__SetGripperState_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(grpr2f85_ifaces__srv__SetGripperState_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    grpr2f85_ifaces__srv__SetGripperState_Request * data =
      (grpr2f85_ifaces__srv__SetGripperState_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!grpr2f85_ifaces__srv__SetGripperState_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          grpr2f85_ifaces__srv__SetGripperState_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!grpr2f85_ifaces__srv__SetGripperState_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `result`
#include "rosidl_runtime_c/string_functions.h"

bool
grpr2f85_ifaces__srv__SetGripperState_Response__init(grpr2f85_ifaces__srv__SetGripperState_Response * msg)
{
  if (!msg) {
    return false;
  }
  // ok
  msg->ok = false;
  // status_code
  // result
  if (!rosidl_runtime_c__String__init(&msg->result)) {
    grpr2f85_ifaces__srv__SetGripperState_Response__fini(msg);
    return false;
  }
  {
    bool success = rosidl_runtime_c__String__assign(&msg->result, "");
    if (!success) {
      goto abort_init_0;
    }
  }
  return true;
abort_init_0:
  return false;
}

void
grpr2f85_ifaces__srv__SetGripperState_Response__fini(grpr2f85_ifaces__srv__SetGripperState_Response * msg)
{
  if (!msg) {
    return;
  }
  // ok
  // status_code
  // result
  rosidl_runtime_c__String__fini(&msg->result);
}

bool
grpr2f85_ifaces__srv__SetGripperState_Response__are_equal(const grpr2f85_ifaces__srv__SetGripperState_Response * lhs, const grpr2f85_ifaces__srv__SetGripperState_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // ok
  if (lhs->ok != rhs->ok) {
    return false;
  }
  // status_code
  if (lhs->status_code != rhs->status_code) {
    return false;
  }
  // result
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->result), &(rhs->result)))
  {
    return false;
  }
  return true;
}

bool
grpr2f85_ifaces__srv__SetGripperState_Response__copy(
  const grpr2f85_ifaces__srv__SetGripperState_Response * input,
  grpr2f85_ifaces__srv__SetGripperState_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // ok
  output->ok = input->ok;
  // status_code
  output->status_code = input->status_code;
  // result
  if (!rosidl_runtime_c__String__copy(
      &(input->result), &(output->result)))
  {
    return false;
  }
  return true;
}

grpr2f85_ifaces__srv__SetGripperState_Response *
grpr2f85_ifaces__srv__SetGripperState_Response__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  grpr2f85_ifaces__srv__SetGripperState_Response * msg = (grpr2f85_ifaces__srv__SetGripperState_Response *)allocator.allocate(sizeof(grpr2f85_ifaces__srv__SetGripperState_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(grpr2f85_ifaces__srv__SetGripperState_Response));
  bool success = grpr2f85_ifaces__srv__SetGripperState_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
grpr2f85_ifaces__srv__SetGripperState_Response__destroy(grpr2f85_ifaces__srv__SetGripperState_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    grpr2f85_ifaces__srv__SetGripperState_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
grpr2f85_ifaces__srv__SetGripperState_Response__Sequence__init(grpr2f85_ifaces__srv__SetGripperState_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  grpr2f85_ifaces__srv__SetGripperState_Response * data = NULL;

  if (size) {
    data = (grpr2f85_ifaces__srv__SetGripperState_Response *)allocator.zero_allocate(size, sizeof(grpr2f85_ifaces__srv__SetGripperState_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = grpr2f85_ifaces__srv__SetGripperState_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        grpr2f85_ifaces__srv__SetGripperState_Response__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
grpr2f85_ifaces__srv__SetGripperState_Response__Sequence__fini(grpr2f85_ifaces__srv__SetGripperState_Response__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      grpr2f85_ifaces__srv__SetGripperState_Response__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

grpr2f85_ifaces__srv__SetGripperState_Response__Sequence *
grpr2f85_ifaces__srv__SetGripperState_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  grpr2f85_ifaces__srv__SetGripperState_Response__Sequence * array = (grpr2f85_ifaces__srv__SetGripperState_Response__Sequence *)allocator.allocate(sizeof(grpr2f85_ifaces__srv__SetGripperState_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = grpr2f85_ifaces__srv__SetGripperState_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
grpr2f85_ifaces__srv__SetGripperState_Response__Sequence__destroy(grpr2f85_ifaces__srv__SetGripperState_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    grpr2f85_ifaces__srv__SetGripperState_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
grpr2f85_ifaces__srv__SetGripperState_Response__Sequence__are_equal(const grpr2f85_ifaces__srv__SetGripperState_Response__Sequence * lhs, const grpr2f85_ifaces__srv__SetGripperState_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!grpr2f85_ifaces__srv__SetGripperState_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
grpr2f85_ifaces__srv__SetGripperState_Response__Sequence__copy(
  const grpr2f85_ifaces__srv__SetGripperState_Response__Sequence * input,
  grpr2f85_ifaces__srv__SetGripperState_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(grpr2f85_ifaces__srv__SetGripperState_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    grpr2f85_ifaces__srv__SetGripperState_Response * data =
      (grpr2f85_ifaces__srv__SetGripperState_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!grpr2f85_ifaces__srv__SetGripperState_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          grpr2f85_ifaces__srv__SetGripperState_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!grpr2f85_ifaces__srv__SetGripperState_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
