// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from grpr2f85_ifaces:srv/SetGripperState.idl
// generated code does not contain a copyright notice

#ifndef GRPR2F85_IFACES__SRV__DETAIL__SET_GRIPPER_STATE__TRAITS_HPP_
#define GRPR2F85_IFACES__SRV__DETAIL__SET_GRIPPER_STATE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "grpr2f85_ifaces/srv/detail/set_gripper_state__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace grpr2f85_ifaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const SetGripperState_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: position
  {
    out << "position: ";
    rosidl_generator_traits::value_to_yaml(msg.position, out);
    out << ", ";
  }

  // member: speed
  {
    out << "speed: ";
    rosidl_generator_traits::value_to_yaml(msg.speed, out);
    out << ", ";
  }

  // member: force
  {
    out << "force: ";
    rosidl_generator_traits::value_to_yaml(msg.force, out);
    out << ", ";
  }

  // member: wait_time
  {
    out << "wait_time: ";
    rosidl_generator_traits::value_to_yaml(msg.wait_time, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SetGripperState_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: position
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "position: ";
    rosidl_generator_traits::value_to_yaml(msg.position, out);
    out << "\n";
  }

  // member: speed
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "speed: ";
    rosidl_generator_traits::value_to_yaml(msg.speed, out);
    out << "\n";
  }

  // member: force
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "force: ";
    rosidl_generator_traits::value_to_yaml(msg.force, out);
    out << "\n";
  }

  // member: wait_time
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "wait_time: ";
    rosidl_generator_traits::value_to_yaml(msg.wait_time, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SetGripperState_Request & msg, bool use_flow_style = false)
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
  const grpr2f85_ifaces::srv::SetGripperState_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  grpr2f85_ifaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use grpr2f85_ifaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const grpr2f85_ifaces::srv::SetGripperState_Request & msg)
{
  return grpr2f85_ifaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<grpr2f85_ifaces::srv::SetGripperState_Request>()
{
  return "grpr2f85_ifaces::srv::SetGripperState_Request";
}

template<>
inline const char * name<grpr2f85_ifaces::srv::SetGripperState_Request>()
{
  return "grpr2f85_ifaces/srv/SetGripperState_Request";
}

template<>
struct has_fixed_size<grpr2f85_ifaces::srv::SetGripperState_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<grpr2f85_ifaces::srv::SetGripperState_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<grpr2f85_ifaces::srv::SetGripperState_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace grpr2f85_ifaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const SetGripperState_Response & msg,
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
  const SetGripperState_Response & msg,
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

inline std::string to_yaml(const SetGripperState_Response & msg, bool use_flow_style = false)
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
  const grpr2f85_ifaces::srv::SetGripperState_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  grpr2f85_ifaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use grpr2f85_ifaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const grpr2f85_ifaces::srv::SetGripperState_Response & msg)
{
  return grpr2f85_ifaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<grpr2f85_ifaces::srv::SetGripperState_Response>()
{
  return "grpr2f85_ifaces::srv::SetGripperState_Response";
}

template<>
inline const char * name<grpr2f85_ifaces::srv::SetGripperState_Response>()
{
  return "grpr2f85_ifaces/srv/SetGripperState_Response";
}

template<>
struct has_fixed_size<grpr2f85_ifaces::srv::SetGripperState_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<grpr2f85_ifaces::srv::SetGripperState_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<grpr2f85_ifaces::srv::SetGripperState_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<grpr2f85_ifaces::srv::SetGripperState>()
{
  return "grpr2f85_ifaces::srv::SetGripperState";
}

template<>
inline const char * name<grpr2f85_ifaces::srv::SetGripperState>()
{
  return "grpr2f85_ifaces/srv/SetGripperState";
}

template<>
struct has_fixed_size<grpr2f85_ifaces::srv::SetGripperState>
  : std::integral_constant<
    bool,
    has_fixed_size<grpr2f85_ifaces::srv::SetGripperState_Request>::value &&
    has_fixed_size<grpr2f85_ifaces::srv::SetGripperState_Response>::value
  >
{
};

template<>
struct has_bounded_size<grpr2f85_ifaces::srv::SetGripperState>
  : std::integral_constant<
    bool,
    has_bounded_size<grpr2f85_ifaces::srv::SetGripperState_Request>::value &&
    has_bounded_size<grpr2f85_ifaces::srv::SetGripperState_Response>::value
  >
{
};

template<>
struct is_service<grpr2f85_ifaces::srv::SetGripperState>
  : std::true_type
{
};

template<>
struct is_service_request<grpr2f85_ifaces::srv::SetGripperState_Request>
  : std::true_type
{
};

template<>
struct is_service_response<grpr2f85_ifaces::srv::SetGripperState_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // GRPR2F85_IFACES__SRV__DETAIL__SET_GRIPPER_STATE__TRAITS_HPP_
