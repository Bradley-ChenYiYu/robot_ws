// generated from rosidl_generator_py/resource/_idl_support.c.em
// with input from grpr2f85_ifaces:srv/GetGripperStatus.idl
// generated code does not contain a copyright notice
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <stdbool.h>
#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include "numpy/ndarrayobject.h"
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif
#include "rosidl_runtime_c/visibility_control.h"
#include "grpr2f85_ifaces/srv/detail/get_gripper_status__struct.h"
#include "grpr2f85_ifaces/srv/detail/get_gripper_status__functions.h"


ROSIDL_GENERATOR_C_EXPORT
bool grpr2f85_ifaces__srv__get_gripper_status__request__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[65];
    {
      char * class_name = NULL;
      char * module_name = NULL;
      {
        PyObject * class_attr = PyObject_GetAttrString(_pymsg, "__class__");
        if (class_attr) {
          PyObject * name_attr = PyObject_GetAttrString(class_attr, "__name__");
          if (name_attr) {
            class_name = (char *)PyUnicode_1BYTE_DATA(name_attr);
            Py_DECREF(name_attr);
          }
          PyObject * module_attr = PyObject_GetAttrString(class_attr, "__module__");
          if (module_attr) {
            module_name = (char *)PyUnicode_1BYTE_DATA(module_attr);
            Py_DECREF(module_attr);
          }
          Py_DECREF(class_attr);
        }
      }
      if (!class_name || !module_name) {
        return false;
      }
      snprintf(full_classname_dest, sizeof(full_classname_dest), "%s.%s", module_name, class_name);
    }
    assert(strncmp("grpr2f85_ifaces.srv._get_gripper_status.GetGripperStatus_Request", full_classname_dest, 64) == 0);
  }
  grpr2f85_ifaces__srv__GetGripperStatus_Request * ros_message = _ros_message;
  {  // get
    PyObject * field = PyObject_GetAttrString(_pymsg, "get");
    if (!field) {
      return false;
    }
    assert(PyBool_Check(field));
    ros_message->get = (Py_True == field);
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * grpr2f85_ifaces__srv__get_gripper_status__request__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of GetGripperStatus_Request */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("grpr2f85_ifaces.srv._get_gripper_status");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "GetGripperStatus_Request");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  grpr2f85_ifaces__srv__GetGripperStatus_Request * ros_message = (grpr2f85_ifaces__srv__GetGripperStatus_Request *)raw_ros_message;
  {  // get
    PyObject * field = NULL;
    field = PyBool_FromLong(ros_message->get ? 1 : 0);
    {
      int rc = PyObject_SetAttrString(_pymessage, "get", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }

  // ownership of _pymessage is transferred to the caller
  return _pymessage;
}

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
// already included above
// #include <Python.h>
// already included above
// #include <stdbool.h>
// already included above
// #include "numpy/ndarrayobject.h"
// already included above
// #include "rosidl_runtime_c/visibility_control.h"
// already included above
// #include "grpr2f85_ifaces/srv/detail/get_gripper_status__struct.h"
// already included above
// #include "grpr2f85_ifaces/srv/detail/get_gripper_status__functions.h"

#include "rosidl_runtime_c/string.h"
#include "rosidl_runtime_c/string_functions.h"


ROSIDL_GENERATOR_C_EXPORT
bool grpr2f85_ifaces__srv__get_gripper_status__response__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[66];
    {
      char * class_name = NULL;
      char * module_name = NULL;
      {
        PyObject * class_attr = PyObject_GetAttrString(_pymsg, "__class__");
        if (class_attr) {
          PyObject * name_attr = PyObject_GetAttrString(class_attr, "__name__");
          if (name_attr) {
            class_name = (char *)PyUnicode_1BYTE_DATA(name_attr);
            Py_DECREF(name_attr);
          }
          PyObject * module_attr = PyObject_GetAttrString(class_attr, "__module__");
          if (module_attr) {
            module_name = (char *)PyUnicode_1BYTE_DATA(module_attr);
            Py_DECREF(module_attr);
          }
          Py_DECREF(class_attr);
        }
      }
      if (!class_name || !module_name) {
        return false;
      }
      snprintf(full_classname_dest, sizeof(full_classname_dest), "%s.%s", module_name, class_name);
    }
    assert(strncmp("grpr2f85_ifaces.srv._get_gripper_status.GetGripperStatus_Response", full_classname_dest, 65) == 0);
  }
  grpr2f85_ifaces__srv__GetGripperStatus_Response * ros_message = _ros_message;
  {  // ok
    PyObject * field = PyObject_GetAttrString(_pymsg, "ok");
    if (!field) {
      return false;
    }
    assert(PyBool_Check(field));
    ros_message->ok = (Py_True == field);
    Py_DECREF(field);
  }
  {  // status_code
    PyObject * field = PyObject_GetAttrString(_pymsg, "status_code");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->status_code = (uint8_t)PyLong_AsUnsignedLong(field);
    Py_DECREF(field);
  }
  {  // result
    PyObject * field = PyObject_GetAttrString(_pymsg, "result");
    if (!field) {
      return false;
    }
    assert(PyUnicode_Check(field));
    PyObject * encoded_field = PyUnicode_AsUTF8String(field);
    if (!encoded_field) {
      Py_DECREF(field);
      return false;
    }
    rosidl_runtime_c__String__assign(&ros_message->result, PyBytes_AS_STRING(encoded_field));
    Py_DECREF(encoded_field);
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * grpr2f85_ifaces__srv__get_gripper_status__response__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of GetGripperStatus_Response */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("grpr2f85_ifaces.srv._get_gripper_status");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "GetGripperStatus_Response");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  grpr2f85_ifaces__srv__GetGripperStatus_Response * ros_message = (grpr2f85_ifaces__srv__GetGripperStatus_Response *)raw_ros_message;
  {  // ok
    PyObject * field = NULL;
    field = PyBool_FromLong(ros_message->ok ? 1 : 0);
    {
      int rc = PyObject_SetAttrString(_pymessage, "ok", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // status_code
    PyObject * field = NULL;
    field = PyLong_FromUnsignedLong(ros_message->status_code);
    {
      int rc = PyObject_SetAttrString(_pymessage, "status_code", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // result
    PyObject * field = NULL;
    field = PyUnicode_DecodeUTF8(
      ros_message->result.data,
      strlen(ros_message->result.data),
      "replace");
    if (!field) {
      return NULL;
    }
    {
      int rc = PyObject_SetAttrString(_pymessage, "result", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }

  // ownership of _pymessage is transferred to the caller
  return _pymessage;
}
