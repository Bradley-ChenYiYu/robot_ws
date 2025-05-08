// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from navigation_interfaces:srv/NavigateToPose.idl
// generated code does not contain a copyright notice

#ifndef NAVIGATION_INTERFACES__SRV__DETAIL__NAVIGATE_TO_POSE__TRAITS_HPP_
#define NAVIGATION_INTERFACES__SRV__DETAIL__NAVIGATE_TO_POSE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "navigation_interfaces/srv/detail/navigate_to_pose__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace navigation_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const NavigateToPose_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: x
  {
    out << "x: ";
    rosidl_generator_traits::value_to_yaml(msg.x, out);
    out << ", ";
  }

  // member: y
  {
    out << "y: ";
    rosidl_generator_traits::value_to_yaml(msg.y, out);
    out << ", ";
  }

  // member: z
  {
    out << "z: ";
    rosidl_generator_traits::value_to_yaml(msg.z, out);
    out << ", ";
  }

  // member: w
  {
    out << "w: ";
    rosidl_generator_traits::value_to_yaml(msg.w, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const NavigateToPose_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: x
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "x: ";
    rosidl_generator_traits::value_to_yaml(msg.x, out);
    out << "\n";
  }

  // member: y
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "y: ";
    rosidl_generator_traits::value_to_yaml(msg.y, out);
    out << "\n";
  }

  // member: z
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "z: ";
    rosidl_generator_traits::value_to_yaml(msg.z, out);
    out << "\n";
  }

  // member: w
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "w: ";
    rosidl_generator_traits::value_to_yaml(msg.w, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const NavigateToPose_Request & msg, bool use_flow_style = false)
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

}  // namespace navigation_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use navigation_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const navigation_interfaces::srv::NavigateToPose_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  navigation_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use navigation_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const navigation_interfaces::srv::NavigateToPose_Request & msg)
{
  return navigation_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<navigation_interfaces::srv::NavigateToPose_Request>()
{
  return "navigation_interfaces::srv::NavigateToPose_Request";
}

template<>
inline const char * name<navigation_interfaces::srv::NavigateToPose_Request>()
{
  return "navigation_interfaces/srv/NavigateToPose_Request";
}

template<>
struct has_fixed_size<navigation_interfaces::srv::NavigateToPose_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<navigation_interfaces::srv::NavigateToPose_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<navigation_interfaces::srv::NavigateToPose_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace navigation_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const NavigateToPose_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: success
  {
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << ", ";
  }

  // member: message
  {
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const NavigateToPose_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: success
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << "\n";
  }

  // member: message
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const NavigateToPose_Response & msg, bool use_flow_style = false)
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

}  // namespace navigation_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use navigation_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const navigation_interfaces::srv::NavigateToPose_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  navigation_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use navigation_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const navigation_interfaces::srv::NavigateToPose_Response & msg)
{
  return navigation_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<navigation_interfaces::srv::NavigateToPose_Response>()
{
  return "navigation_interfaces::srv::NavigateToPose_Response";
}

template<>
inline const char * name<navigation_interfaces::srv::NavigateToPose_Response>()
{
  return "navigation_interfaces/srv/NavigateToPose_Response";
}

template<>
struct has_fixed_size<navigation_interfaces::srv::NavigateToPose_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<navigation_interfaces::srv::NavigateToPose_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<navigation_interfaces::srv::NavigateToPose_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<navigation_interfaces::srv::NavigateToPose>()
{
  return "navigation_interfaces::srv::NavigateToPose";
}

template<>
inline const char * name<navigation_interfaces::srv::NavigateToPose>()
{
  return "navigation_interfaces/srv/NavigateToPose";
}

template<>
struct has_fixed_size<navigation_interfaces::srv::NavigateToPose>
  : std::integral_constant<
    bool,
    has_fixed_size<navigation_interfaces::srv::NavigateToPose_Request>::value &&
    has_fixed_size<navigation_interfaces::srv::NavigateToPose_Response>::value
  >
{
};

template<>
struct has_bounded_size<navigation_interfaces::srv::NavigateToPose>
  : std::integral_constant<
    bool,
    has_bounded_size<navigation_interfaces::srv::NavigateToPose_Request>::value &&
    has_bounded_size<navigation_interfaces::srv::NavigateToPose_Response>::value
  >
{
};

template<>
struct is_service<navigation_interfaces::srv::NavigateToPose>
  : std::true_type
{
};

template<>
struct is_service_request<navigation_interfaces::srv::NavigateToPose_Request>
  : std::true_type
{
};

template<>
struct is_service_response<navigation_interfaces::srv::NavigateToPose_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // NAVIGATION_INTERFACES__SRV__DETAIL__NAVIGATE_TO_POSE__TRAITS_HPP_
