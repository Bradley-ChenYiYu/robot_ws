// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from lslidar_msgs:srv/Lslidarcontrol.idl
// generated code does not contain a copyright notice

#ifndef LSLIDAR_MSGS__SRV__DETAIL__LSLIDARCONTROL__BUILDER_HPP_
#define LSLIDAR_MSGS__SRV__DETAIL__LSLIDARCONTROL__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "lslidar_msgs/srv/detail/lslidarcontrol__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace lslidar_msgs
{

namespace srv
{

namespace builder
{

class Init_Lslidarcontrol_Request_lasercontrol
{
public:
  Init_Lslidarcontrol_Request_lasercontrol()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::lslidar_msgs::srv::Lslidarcontrol_Request lasercontrol(::lslidar_msgs::srv::Lslidarcontrol_Request::_lasercontrol_type arg)
  {
    msg_.lasercontrol = std::move(arg);
    return std::move(msg_);
  }

private:
  ::lslidar_msgs::srv::Lslidarcontrol_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::lslidar_msgs::srv::Lslidarcontrol_Request>()
{
  return lslidar_msgs::srv::builder::Init_Lslidarcontrol_Request_lasercontrol();
}

}  // namespace lslidar_msgs


namespace lslidar_msgs
{

namespace srv
{

namespace builder
{

class Init_Lslidarcontrol_Response_status
{
public:
  Init_Lslidarcontrol_Response_status()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::lslidar_msgs::srv::Lslidarcontrol_Response status(::lslidar_msgs::srv::Lslidarcontrol_Response::_status_type arg)
  {
    msg_.status = std::move(arg);
    return std::move(msg_);
  }

private:
  ::lslidar_msgs::srv::Lslidarcontrol_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::lslidar_msgs::srv::Lslidarcontrol_Response>()
{
  return lslidar_msgs::srv::builder::Init_Lslidarcontrol_Response_status();
}

}  // namespace lslidar_msgs

#endif  // LSLIDAR_MSGS__SRV__DETAIL__LSLIDARCONTROL__BUILDER_HPP_
