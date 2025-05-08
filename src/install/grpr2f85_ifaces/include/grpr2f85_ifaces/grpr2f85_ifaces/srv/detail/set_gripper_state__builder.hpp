// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from grpr2f85_ifaces:srv/SetGripperState.idl
// generated code does not contain a copyright notice

#ifndef GRPR2F85_IFACES__SRV__DETAIL__SET_GRIPPER_STATE__BUILDER_HPP_
#define GRPR2F85_IFACES__SRV__DETAIL__SET_GRIPPER_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "grpr2f85_ifaces/srv/detail/set_gripper_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace grpr2f85_ifaces
{

namespace srv
{

namespace builder
{

class Init_SetGripperState_Request_wait_time
{
public:
  explicit Init_SetGripperState_Request_wait_time(::grpr2f85_ifaces::srv::SetGripperState_Request & msg)
  : msg_(msg)
  {}
  ::grpr2f85_ifaces::srv::SetGripperState_Request wait_time(::grpr2f85_ifaces::srv::SetGripperState_Request::_wait_time_type arg)
  {
    msg_.wait_time = std::move(arg);
    return std::move(msg_);
  }

private:
  ::grpr2f85_ifaces::srv::SetGripperState_Request msg_;
};

class Init_SetGripperState_Request_force
{
public:
  explicit Init_SetGripperState_Request_force(::grpr2f85_ifaces::srv::SetGripperState_Request & msg)
  : msg_(msg)
  {}
  Init_SetGripperState_Request_wait_time force(::grpr2f85_ifaces::srv::SetGripperState_Request::_force_type arg)
  {
    msg_.force = std::move(arg);
    return Init_SetGripperState_Request_wait_time(msg_);
  }

private:
  ::grpr2f85_ifaces::srv::SetGripperState_Request msg_;
};

class Init_SetGripperState_Request_speed
{
public:
  explicit Init_SetGripperState_Request_speed(::grpr2f85_ifaces::srv::SetGripperState_Request & msg)
  : msg_(msg)
  {}
  Init_SetGripperState_Request_force speed(::grpr2f85_ifaces::srv::SetGripperState_Request::_speed_type arg)
  {
    msg_.speed = std::move(arg);
    return Init_SetGripperState_Request_force(msg_);
  }

private:
  ::grpr2f85_ifaces::srv::SetGripperState_Request msg_;
};

class Init_SetGripperState_Request_position
{
public:
  Init_SetGripperState_Request_position()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SetGripperState_Request_speed position(::grpr2f85_ifaces::srv::SetGripperState_Request::_position_type arg)
  {
    msg_.position = std::move(arg);
    return Init_SetGripperState_Request_speed(msg_);
  }

private:
  ::grpr2f85_ifaces::srv::SetGripperState_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::grpr2f85_ifaces::srv::SetGripperState_Request>()
{
  return grpr2f85_ifaces::srv::builder::Init_SetGripperState_Request_position();
}

}  // namespace grpr2f85_ifaces


namespace grpr2f85_ifaces
{

namespace srv
{

namespace builder
{

class Init_SetGripperState_Response_result
{
public:
  explicit Init_SetGripperState_Response_result(::grpr2f85_ifaces::srv::SetGripperState_Response & msg)
  : msg_(msg)
  {}
  ::grpr2f85_ifaces::srv::SetGripperState_Response result(::grpr2f85_ifaces::srv::SetGripperState_Response::_result_type arg)
  {
    msg_.result = std::move(arg);
    return std::move(msg_);
  }

private:
  ::grpr2f85_ifaces::srv::SetGripperState_Response msg_;
};

class Init_SetGripperState_Response_status_code
{
public:
  explicit Init_SetGripperState_Response_status_code(::grpr2f85_ifaces::srv::SetGripperState_Response & msg)
  : msg_(msg)
  {}
  Init_SetGripperState_Response_result status_code(::grpr2f85_ifaces::srv::SetGripperState_Response::_status_code_type arg)
  {
    msg_.status_code = std::move(arg);
    return Init_SetGripperState_Response_result(msg_);
  }

private:
  ::grpr2f85_ifaces::srv::SetGripperState_Response msg_;
};

class Init_SetGripperState_Response_ok
{
public:
  Init_SetGripperState_Response_ok()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SetGripperState_Response_status_code ok(::grpr2f85_ifaces::srv::SetGripperState_Response::_ok_type arg)
  {
    msg_.ok = std::move(arg);
    return Init_SetGripperState_Response_status_code(msg_);
  }

private:
  ::grpr2f85_ifaces::srv::SetGripperState_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::grpr2f85_ifaces::srv::SetGripperState_Response>()
{
  return grpr2f85_ifaces::srv::builder::Init_SetGripperState_Response_ok();
}

}  // namespace grpr2f85_ifaces

#endif  // GRPR2F85_IFACES__SRV__DETAIL__SET_GRIPPER_STATE__BUILDER_HPP_
