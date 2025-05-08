// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from grpr2f85_ifaces:srv/GetGripperStatus.idl
// generated code does not contain a copyright notice

#ifndef GRPR2F85_IFACES__SRV__DETAIL__GET_GRIPPER_STATUS__TRAITS_HPP_
#define GRPR2F85_IFACES__SRV__DETAIL__GET_GRIPPER_STATUS__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "grpr2f85_ifaces/srv/detail/get_gripper_status__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace grpr2f85_ifaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const GetGripperStatus_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: get
  {
    out << "get: ";
    rosidl_generator_traits::value_to_yaml(msg.get, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const GetGripperStatus_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: get
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "get: ";
    rosidl_generator_traits::value_to_yaml(msg.get, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GetGripperStatus_Request & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace grpr2f85_ifaces

namespace rosidl_generator_traits
{

[[deprecated("use grpr2f85_ifaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const grpr2f85_ifaces::srv::GetGripperStatus_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  grpr2f85_ifaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use grpr2f85_ifaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const grpr2f85_ifaces::srv::GetGripperStatus_Request & msg)
{
  return grpr2f85_ifaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<grpr2f85_ifaces::srv::GetGripperStatus_Request>()
{
  return "grpr2f85_ifaces::srv::GetGripperStatus_Request";
}

template<>
inline const char * name<grpr2f85_ifaces::srv::GetGripperStatus_Request>()
{
  return "grpr2f85_ifaces/srv/GetGripperStatus_Request";
}

template<>
struct has_fixed_size<grpr2f85_ifaces::srv::GetGripperStatus_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<grpr2f85_ifaces::srv::GetGripperStatus_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<grpr2f85_ifaces::srv::GetGripperStatus_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace grpr2f85_ifaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const GetGripperStatus_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: ok
  {
    out << "ok: ";
    rosidl_generator_traits::value_to_yaml(msg.ok, out);
    out << ", ";
  }

  // member: status_code
  {
    out << "status_code: ";
    rosidl_generator_traits::value_to_yaml(msg.status_code, out);
    out << ", ";
  }

  // member: result
  {
    out << "result: ";
    rosidl_generator_traits::value_to_yaml(msg.result, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const GetGripperStatus_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: ok
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "ok: ";
    rosidl_generator_traits::value_to_yaml(msg.ok, out);
    out << "\n";
  }

  // member: status_code
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "status_code: ";
    rosidl_generator_traits::value_to_yaml(msg.status_code, out);
    out << "\n";
  }

  // member: result
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "result: ";
    rosidl_generator_traits::value_to_yaml(msg.result, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GetGripperStatus_Response & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace grpr2f85_ifaces

namespace rosidl_generator_traits
{

[[deprecated("use grpr2f85_ifaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const grpr2f85_ifaces::srv::GetGripperStatus_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  grpr2f85_ifaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use grpr2f85_ifaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const grpr2f85_ifaces::srv::GetGripperStatus_Response & msg)
{
  return grpr2f85_ifaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<grpr2f85_ifaces::srv::GetGripperStatus_Response>()
{
  return "grpr2f85_ifaces::srv::GetGripperStatus_Response";
}

template<>
inline const char * name<grpr2f85_ifaces::srv::GetGripperStatus_Response>()
{
  return "grpr2f85_ifaces/srv/GetGripperStatus_Response";
}

template<>
struct has_fixed_size<grpr2f85_ifaces::srv::GetGripperStatus_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<grpr2f85_ifaces::srv::GetGripperStatus_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<grpr2f85_ifaces::srv::GetGripperStatus_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<grpr2f85_ifaces::srv::GetGripperStatus>()
{
  return "grpr2f85_ifaces::srv::GetGripperStatus";
}

template<>
inline const char * name<grpr2f85_ifaces::srv::GetGripperStatus>()
{
  return "grpr2f85_ifaces/srv/GetGripperStatus";
}

template<>
struct has_fixed_size<grpr2f85_ifaces::srv::GetGripperStatus>
  : std::integral_constant<
    bool,
    has_fixed_size<grpr2f85_ifaces::srv::GetGripperStatus_Request>::value &&
    has_fixed_size<grpr2f85_ifaces::srv::GetGripperStatus_Response>::value
  >
{
};

template<>
struct has_bounded_size<grpr2f85_ifaces::srv::GetGripperStatus>
  : std::integral_constant<
    bool,
    has_bounded_size<grpr2f85_ifaces::srv::GetGripperStatus_Request>::value &&
    has_bounded_size<grpr2f85_ifaces::srv::GetGripperStatus_Response>::value
  >
{
};

template<>
struct is_service<grpr2f85_ifaces::srv::GetGripperStatus>
  : std::true_type
{
};

template<>
struct is_service_request<grpr2f85_ifaces::srv::GetGripperStatus_Request>
  : std::true_type
{
};

template<>
struct is_service_response<grpr2f85_ifaces::srv::GetGripperStatus_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // GRPR2F85_IFACES__SRV__DETAIL__GET_GRIPPER_STATUS__TRAITS_HPP_
