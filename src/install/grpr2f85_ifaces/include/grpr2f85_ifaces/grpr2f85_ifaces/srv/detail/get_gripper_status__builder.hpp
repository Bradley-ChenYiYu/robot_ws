// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from grpr2f85_ifaces:srv/GetGripperStatus.idl
// generated code does not contain a copyright notice

#ifndef GRPR2F85_IFACES__SRV__DETAIL__GET_GRIPPER_STATUS__BUILDER_HPP_
#define GRPR2F85_IFACES__SRV__DETAIL__GET_GRIPPER_STATUS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "grpr2f85_ifaces/srv/detail/get_gripper_status__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace grpr2f85_ifaces
{

namespace srv
{

namespace builder
{

class Init_GetGripperStatus_Request_get
{
public:
  Init_GetGripperStatus_Request_get()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::grpr2f85_ifaces::srv::GetGripperStatus_Request get(::grpr2f85_ifaces::srv::GetGripperStatus_Request::_get_type arg)
  {
    msg_.get = std::move(arg);
    return std::move(msg_);
  }

private:
  ::grpr2f85_ifaces::srv::GetGripperStatus_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::grpr2f85_ifaces::srv::GetGripperStatus_Request>()
{
  return grpr2f85_ifaces::srv::builder::Init_GetGripperStatus_Request_get();
}

}  // namespace grpr2f85_ifaces


namespace grpr2f85_ifaces
{

namespace srv
{

namespace builder
{

class Init_GetGripperStatus_Response_result
{
public:
  explicit Init_GetGripperStatus_Response_result(::grpr2f85_ifaces::srv::GetGripperStatus_Response & msg)
  : msg_(msg)
  {}
  ::grpr2f85_ifaces::srv::GetGripperStatus_Response result(::grpr2f85_ifaces::srv::GetGripperStatus_Response::_result_type arg)
  {
    msg_.result = std::move(arg);
    return std::move(msg_);
  }

private:
  ::grpr2f85_ifaces::srv::GetGripperStatus_Response msg_;
};

class Init_GetGripperStatus_Response_status_code
{
public:
  explicit Init_GetGripperStatus_Response_status_code(::grpr2f85_ifaces::srv::GetGripperStatus_Response & msg)
  : msg_(msg)
  {}
  Init_GetGripperStatus_Response_result status_code(::grpr2f85_ifaces::srv::GetGripperStatus_Response::_status_code_type arg)
  {
    msg_.status_code = std::move(arg);
    return Init_GetGripperStatus_Response_result(msg_);
  }

private:
  ::grpr2f85_ifaces::srv::GetGripperStatus_Response msg_;
};

class Init_GetGripperStatus_Response_ok
{
public:
  Init_GetGripperStatus_Response_ok()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_GetGripperStatus_Response_status_code ok(::grpr2f85_ifaces::srv::GetGripperStatus_Response::_ok_type arg)
  {
    msg_.ok = std::move(arg);
    return Init_GetGripperStatus_Response_status_code(msg_);
  }

private:
  ::grpr2f85_ifaces::srv::GetGripperStatus_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::grpr2f85_ifaces::srv::GetGripperStatus_Response>()
{
  return grpr2f85_ifaces::srv::builder::Init_GetGripperStatus_Response_ok();
}

}  // namespace grpr2f85_ifaces

#endif  // GRPR2F85_IFACES__SRV__DETAIL__GET_GRIPPER_STATUS__BUILDER_HPP_
