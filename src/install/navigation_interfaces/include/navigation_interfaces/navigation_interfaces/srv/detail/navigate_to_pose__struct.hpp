// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from navigation_interfaces:srv/NavigateToPose.idl
// generated code does not contain a copyright notice

#ifndef NAVIGATION_INTERFACES__SRV__DETAIL__NAVIGATE_TO_POSE__STRUCT_HPP_
#define NAVIGATION_INTERFACES__SRV__DETAIL__NAVIGATE_TO_POSE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__navigation_interfaces__srv__NavigateToPose_Request __attribute__((deprecated))
#else
# define DEPRECATED__navigation_interfaces__srv__NavigateToPose_Request __declspec(deprecated)
#endif

namespace navigation_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct NavigateToPose_Request_
{
  using Type = NavigateToPose_Request_<ContainerAllocator>;

  explicit NavigateToPose_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->x = 0.0f;
      this->y = 0.0f;
      this->z = 0.0f;
      this->w = 0.0f;
    }
  }

  explicit NavigateToPose_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->x = 0.0f;
      this->y = 0.0f;
      this->z = 0.0f;
      this->w = 0.0f;
    }
  }

  // field types and members
  using _x_type =
    float;
  _x_type x;
  using _y_type =
    float;
  _y_type y;
  using _z_type =
    float;
  _z_type z;
  using _w_type =
    float;
  _w_type w;

  // setters for named parameter idiom
  Type & set__x(
    const float & _arg)
  {
    this->x = _arg;
    return *this;
  }
  Type & set__y(
    const float & _arg)
  {
    this->y = _arg;
    return *this;
  }
  Type & set__z(
    const float & _arg)
  {
    this->z = _arg;
    return *this;
  }
  Type & set__w(
    const float & _arg)
  {
    this->w = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    navigation_interfaces::srv::NavigateToPose_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const navigation_interfaces::srv::NavigateToPose_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<navigation_interfaces::srv::NavigateToPose_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<navigation_interfaces::srv::NavigateToPose_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      navigation_interfaces::srv::NavigateToPose_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<navigation_interfaces::srv::NavigateToPose_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      navigation_interfaces::srv::NavigateToPose_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<navigation_interfaces::srv::NavigateToPose_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<navigation_interfaces::srv::NavigateToPose_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<navigation_interfaces::srv::NavigateToPose_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__navigation_interfaces__srv__NavigateToPose_Request
    std::shared_ptr<navigation_interfaces::srv::NavigateToPose_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__navigation_interfaces__srv__NavigateToPose_Request
    std::shared_ptr<navigation_interfaces::srv::NavigateToPose_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const NavigateToPose_Request_ & other) const
  {
    if (this->x != other.x) {
      return false;
    }
    if (this->y != other.y) {
      return false;
    }
    if (this->z != other.z) {
      return false;
    }
    if (this->w != other.w) {
      return false;
    }
    return true;
  }
  bool operator!=(const NavigateToPose_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct NavigateToPose_Request_

// alias to use template instance with default allocator
using NavigateToPose_Request =
  navigation_interfaces::srv::NavigateToPose_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace navigation_interfaces


#ifndef _WIN32
# define DEPRECATED__navigation_interfaces__srv__NavigateToPose_Response __attribute__((deprecated))
#else
# define DEPRECATED__navigation_interfaces__srv__NavigateToPose_Response __declspec(deprecated)
#endif

namespace navigation_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct NavigateToPose_Response_
{
  using Type = NavigateToPose_Response_<ContainerAllocator>;

  explicit NavigateToPose_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
      this->message = "";
    }
  }

  explicit NavigateToPose_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : message(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
      this->message = "";
    }
  }

  // field types and members
  using _success_type =
    bool;
  _success_type success;
  using _message_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _message_type message;

  // setters for named parameter idiom
  Type & set__success(
    const bool & _arg)
  {
    this->success = _arg;
    return *this;
  }
  Type & set__message(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->message = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    navigation_interfaces::srv::NavigateToPose_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const navigation_interfaces::srv::NavigateToPose_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<navigation_interfaces::srv::NavigateToPose_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<navigation_interfaces::srv::NavigateToPose_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      navigation_interfaces::srv::NavigateToPose_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<navigation_interfaces::srv::NavigateToPose_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      navigation_interfaces::srv::NavigateToPose_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<navigation_interfaces::srv::NavigateToPose_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<navigation_interfaces::srv::NavigateToPose_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<navigation_interfaces::srv::NavigateToPose_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__navigation_interfaces__srv__NavigateToPose_Response
    std::shared_ptr<navigation_interfaces::srv::NavigateToPose_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__navigation_interfaces__srv__NavigateToPose_Response
    std::shared_ptr<navigation_interfaces::srv::NavigateToPose_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const NavigateToPose_Response_ & other) const
  {
    if (this->success != other.success) {
      return false;
    }
    if (this->message != other.message) {
      return false;
    }
    return true;
  }
  bool operator!=(const NavigateToPose_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct NavigateToPose_Response_

// alias to use template instance with default allocator
using NavigateToPose_Response =
  navigation_interfaces::srv::NavigateToPose_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace navigation_interfaces

namespace navigation_interfaces
{

namespace srv
{

struct NavigateToPose
{
  using Request = navigation_interfaces::srv::NavigateToPose_Request;
  using Response = navigation_interfaces::srv::NavigateToPose_Response;
};

}  // namespace srv

}  // namespace navigation_interfaces

#endif  // NAVIGATION_INTERFACES__SRV__DETAIL__NAVIGATE_TO_POSE__STRUCT_HPP_
