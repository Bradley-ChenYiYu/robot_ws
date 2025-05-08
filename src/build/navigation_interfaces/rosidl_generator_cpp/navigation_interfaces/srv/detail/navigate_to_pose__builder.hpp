// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from navigation_interfaces:srv/NavigateToPose.idl
// generated code does not contain a copyright notice

#ifndef NAVIGATION_INTERFACES__SRV__DETAIL__NAVIGATE_TO_POSE__BUILDER_HPP_
#define NAVIGATION_INTERFACES__SRV__DETAIL__NAVIGATE_TO_POSE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "navigation_interfaces/srv/detail/navigate_to_pose__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace navigation_interfaces
{

namespace srv
{

namespace builder
{

class Init_NavigateToPose_Request_w
{
public:
  explicit Init_NavigateToPose_Request_w(::navigation_interfaces::srv::NavigateToPose_Request & msg)
  : msg_(msg)
  {}
  ::navigation_interfaces::srv::NavigateToPose_Request w(::navigation_interfaces::srv::NavigateToPose_Request::_w_type arg)
  {
    msg_.w = std::move(arg);
    return std::move(msg_);
  }

private:
  ::navigation_interfaces::srv::NavigateToPose_Request msg_;
};

class Init_NavigateToPose_Request_z
{
public:
  explicit Init_NavigateToPose_Request_z(::navigation_interfaces::srv::NavigateToPose_Request & msg)
  : msg_(msg)
  {}
  Init_NavigateToPose_Request_w z(::navigation_interfaces::srv::NavigateToPose_Request::_z_type arg)
  {
    msg_.z = std::move(arg);
    return Init_NavigateToPose_Request_w(msg_);
  }

private:
  ::navigation_interfaces::srv::NavigateToPose_Request msg_;
};

class Init_NavigateToPose_Request_y
{
public:
  explicit Init_NavigateToPose_Request_y(::navigation_interfaces::srv::NavigateToPose_Request & msg)
  : msg_(msg)
  {}
  Init_NavigateToPose_Request_z y(::navigation_interfaces::srv::NavigateToPose_Request::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_NavigateToPose_Request_z(msg_);
  }

private:
  ::navigation_interfaces::srv::NavigateToPose_Request msg_;
};

class Init_NavigateToPose_Request_x
{
public:
  Init_NavigateToPose_Request_x()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_NavigateToPose_Request_y x(::navigation_interfaces::srv::NavigateToPose_Request::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_NavigateToPose_Request_y(msg_);
  }

private:
  ::navigation_interfaces::srv::NavigateToPose_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::navigation_interfaces::srv::NavigateToPose_Request>()
{
  return navigation_interfaces::srv::builder::Init_NavigateToPose_Request_x();
}

}  // namespace navigation_interfaces


namespace navigation_interfaces
{

namespace srv
{

namespace builder
{

class Init_NavigateToPose_Response_message
{
public:
  explicit Init_NavigateToPose_Response_message(::navigation_interfaces::srv::NavigateToPose_Response & msg)
  : msg_(msg)
  {}
  ::navigation_interfaces::srv::NavigateToPose_Response message(::navigation_interfaces::srv::NavigateToPose_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::navigation_interfaces::srv::NavigateToPose_Response msg_;
};

class Init_NavigateToPose_Response_success
{
public:
  Init_NavigateToPose_Response_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_NavigateToPose_Response_message success(::navigation_interfaces::srv::NavigateToPose_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_NavigateToPose_Response_message(msg_);
  }

private:
  ::navigation_interfaces::srv::NavigateToPose_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::navigation_interfaces::srv::NavigateToPose_Response>()
{
  return navigation_interfaces::srv::builder::Init_NavigateToPose_Response_success();
}

}  // namespace navigation_interfaces

#endif  // NAVIGATION_INTERFACES__SRV__DETAIL__NAVIGATE_TO_POSE__BUILDER_HPP_
