// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from grpr2f85_ifaces:srv/Reset.idl
// generated code does not contain a copyright notice

#ifndef GRPR2F85_IFACES__SRV__DETAIL__RESET__BUILDER_HPP_
#define GRPR2F85_IFACES__SRV__DETAIL__RESET__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "grpr2f85_ifaces/srv/detail/reset__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace grpr2f85_ifaces
{

namespace srv
{

namespace builder
{

class Init_Reset_Request_get
{
public:
  Init_Reset_Request_get()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::grpr2f85_ifaces::srv::Reset_Request get(::grpr2f85_ifaces::srv::Reset_Request::_get_type arg)
  {
    msg_.get = std::move(arg);
    return std::move(msg_);
  }

private:
  ::grpr2f85_ifaces::srv::Reset_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::grpr2f85_ifaces::srv::Reset_Request>()
{
  return grpr2f85_ifaces::srv::builder::Init_Reset_Request_get();
}

}  // namespace grpr2f85_ifaces


namespace grpr2f85_ifaces
{

namespace srv
{

namespace builder
{

class Init_Reset_Response_ok
{
public:
  Init_Reset_Response_ok()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::grpr2f85_ifaces::srv::Reset_Response ok(::grpr2f85_ifaces::srv::Reset_Response::_ok_type arg)
  {
    msg_.ok = std::move(arg);
    return std::move(msg_);
  }

private:
  ::grpr2f85_ifaces::srv::Reset_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::grpr2f85_ifaces::srv::Reset_Response>()
{
  return grpr2f85_ifaces::srv::builder::Init_Reset_Response_ok();
}

}  // namespace grpr2f85_ifaces

#endif  // GRPR2F85_IFACES__SRV__DETAIL__RESET__BUILDER_HPP_
