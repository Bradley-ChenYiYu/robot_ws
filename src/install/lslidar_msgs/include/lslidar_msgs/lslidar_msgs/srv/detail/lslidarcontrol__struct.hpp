// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from lslidar_msgs:srv/Lslidarcontrol.idl
// generated code does not contain a copyright notice

#ifndef LSLIDAR_MSGS__SRV__DETAIL__LSLIDARCONTROL__STRUCT_HPP_
#define LSLIDAR_MSGS__SRV__DETAIL__LSLIDARCONTROL__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__lslidar_msgs__srv__Lslidarcontrol_Request __attribute__((deprecated))
#else
# define DEPRECATED__lslidar_msgs__srv__Lslidarcontrol_Request __declspec(deprecated)
#endif

namespace lslidar_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct Lslidarcontrol_Request_
{
  using Type = Lslidarcontrol_Request_<ContainerAllocator>;

  explicit Lslidarcontrol_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->lasercontrol = 0l;
    }
  }

  explicit Lslidarcontrol_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->lasercontrol = 0l;
    }
  }

  // field types and members
  using _lasercontrol_type =
    int32_t;
  _lasercontrol_type lasercontrol;

  // setters for named parameter idiom
  Type & set__lasercontrol(
    const int32_t & _arg)
  {
    this->lasercontrol = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    lslidar_msgs::srv::Lslidarcontrol_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const lslidar_msgs::srv::Lslidarcontrol_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<lslidar_msgs::srv::Lslidarcontrol_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<lslidar_msgs::srv::Lslidarcontrol_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      lslidar_msgs::srv::Lslidarcontrol_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<lslidar_msgs::srv::Lslidarcontrol_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      lslidar_msgs::srv::Lslidarcontrol_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<lslidar_msgs::srv::Lslidarcontrol_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<lslidar_msgs::srv::Lslidarcontrol_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<lslidar_msgs::srv::Lslidarcontrol_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__lslidar_msgs__srv__Lslidarcontrol_Request
    std::shared_ptr<lslidar_msgs::srv::Lslidarcontrol_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__lslidar_msgs__srv__Lslidarcontrol_Request
    std::shared_ptr<lslidar_msgs::srv::Lslidarcontrol_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Lslidarcontrol_Request_ & other) const
  {
    if (this->lasercontrol != other.lasercontrol) {
      return false;
    }
    return true;
  }
  bool operator!=(const Lslidarcontrol_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Lslidarcontrol_Request_

// alias to use template instance with default allocator
using Lslidarcontrol_Request =
  lslidar_msgs::srv::Lslidarcontrol_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace lslidar_msgs


#ifndef _WIN32
# define DEPRECATED__lslidar_msgs__srv__Lslidarcontrol_Response __attribute__((deprecated))
#else
# define DEPRECATED__lslidar_msgs__srv__Lslidarcontrol_Response __declspec(deprecated)
#endif

namespace lslidar_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct Lslidarcontrol_Response_
{
  using Type = Lslidarcontrol_Response_<ContainerAllocator>;

  explicit Lslidarcontrol_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->status = false;
    }
  }

  explicit Lslidarcontrol_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->status = false;
    }
  }

  // field types and members
  using _status_type =
    bool;
  _status_type status;

  // setters for named parameter idiom
  Type & set__status(
    const bool & _arg)
  {
    this->status = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    lslidar_msgs::srv::Lslidarcontrol_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const lslidar_msgs::srv::Lslidarcontrol_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<lslidar_msgs::srv::Lslidarcontrol_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<lslidar_msgs::srv::Lslidarcontrol_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      lslidar_msgs::srv::Lslidarcontrol_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<lslidar_msgs::srv::Lslidarcontrol_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      lslidar_msgs::srv::Lslidarcontrol_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<lslidar_msgs::srv::Lslidarcontrol_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<lslidar_msgs::srv::Lslidarcontrol_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<lslidar_msgs::srv::Lslidarcontrol_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__lslidar_msgs__srv__Lslidarcontrol_Response
    std::shared_ptr<lslidar_msgs::srv::Lslidarcontrol_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__lslidar_msgs__srv__Lslidarcontrol_Response
    std::shared_ptr<lslidar_msgs::srv::Lslidarcontrol_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Lslidarcontrol_Response_ & other) const
  {
    if (this->status != other.status) {
      return false;
    }
    return true;
  }
  bool operator!=(const Lslidarcontrol_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Lslidarcontrol_Response_

// alias to use template instance with default allocator
using Lslidarcontrol_Response =
  lslidar_msgs::srv::Lslidarcontrol_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace lslidar_msgs

namespace lslidar_msgs
{

namespace srv
{

struct Lslidarcontrol
{
  using Request = lslidar_msgs::srv::Lslidarcontrol_Request;
  using Response = lslidar_msgs::srv::Lslidarcontrol_Response;
};

}  // namespace srv

}  // namespace lslidar_msgs

#endif  // LSLIDAR_MSGS__SRV__DETAIL__LSLIDARCONTROL__STRUCT_HPP_
