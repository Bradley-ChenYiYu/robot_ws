// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from grpr2f85_ifaces:srv/SetGripperState.idl
// generated code does not contain a copyright notice

#ifndef GRPR2F85_IFACES__SRV__DETAIL__SET_GRIPPER_STATE__STRUCT_HPP_
#define GRPR2F85_IFACES__SRV__DETAIL__SET_GRIPPER_STATE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__grpr2f85_ifaces__srv__SetGripperState_Request __attribute__((deprecated))
#else
# define DEPRECATED__grpr2f85_ifaces__srv__SetGripperState_Request __declspec(deprecated)
#endif

namespace grpr2f85_ifaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct SetGripperState_Request_
{
  using Type = SetGripperState_Request_<ContainerAllocator>;

  explicit SetGripperState_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->position = 0;
      this->speed = 255;
      this->force = 255;
      this->wait_time = 0;
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->position = 0;
      this->speed = 0;
      this->force = 0;
      this->wait_time = 0;
    }
  }

  explicit SetGripperState_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->position = 0;
      this->speed = 255;
      this->force = 255;
      this->wait_time = 0;
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->position = 0;
      this->speed = 0;
      this->force = 0;
      this->wait_time = 0;
    }
  }

  // field types and members
  using _position_type =
    uint8_t;
  _position_type position;
  using _speed_type =
    uint8_t;
  _speed_type speed;
  using _force_type =
    uint8_t;
  _force_type force;
  using _wait_time_type =
    uint16_t;
  _wait_time_type wait_time;

  // setters for named parameter idiom
  Type & set__position(
    const uint8_t & _arg)
  {
    this->position = _arg;
    return *this;
  }
  Type & set__speed(
    const uint8_t & _arg)
  {
    this->speed = _arg;
    return *this;
  }
  Type & set__force(
    const uint8_t & _arg)
  {
    this->force = _arg;
    return *this;
  }
  Type & set__wait_time(
    const uint16_t & _arg)
  {
    this->wait_time = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    grpr2f85_ifaces::srv::SetGripperState_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const grpr2f85_ifaces::srv::SetGripperState_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<grpr2f85_ifaces::srv::SetGripperState_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<grpr2f85_ifaces::srv::SetGripperState_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      grpr2f85_ifaces::srv::SetGripperState_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<grpr2f85_ifaces::srv::SetGripperState_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      grpr2f85_ifaces::srv::SetGripperState_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<grpr2f85_ifaces::srv::SetGripperState_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<grpr2f85_ifaces::srv::SetGripperState_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<grpr2f85_ifaces::srv::SetGripperState_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__grpr2f85_ifaces__srv__SetGripperState_Request
    std::shared_ptr<grpr2f85_ifaces::srv::SetGripperState_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__grpr2f85_ifaces__srv__SetGripperState_Request
    std::shared_ptr<grpr2f85_ifaces::srv::SetGripperState_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SetGripperState_Request_ & other) const
  {
    if (this->position != other.position) {
      return false;
    }
    if (this->speed != other.speed) {
      return false;
    }
    if (this->force != other.force) {
      return false;
    }
    if (this->wait_time != other.wait_time) {
      return false;
    }
    return true;
  }
  bool operator!=(const SetGripperState_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SetGripperState_Request_

// alias to use template instance with default allocator
using SetGripperState_Request =
  grpr2f85_ifaces::srv::SetGripperState_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace grpr2f85_ifaces


#ifndef _WIN32
# define DEPRECATED__grpr2f85_ifaces__srv__SetGripperState_Response __attribute__((deprecated))
#else
# define DEPRECATED__grpr2f85_ifaces__srv__SetGripperState_Response __declspec(deprecated)
#endif

namespace grpr2f85_ifaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct SetGripperState_Response_
{
  using Type = SetGripperState_Response_<ContainerAllocator>;

  explicit SetGripperState_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->ok = false;
      this->result = "";
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->ok = false;
      this->status_code = 0;
      this->result = "";
    }
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->status_code = 0;
    }
  }

  explicit SetGripperState_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : result(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->ok = false;
      this->result = "";
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->ok = false;
      this->status_code = 0;
      this->result = "";
    }
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->status_code = 0;
    }
  }

  // field types and members
  using _ok_type =
    bool;
  _ok_type ok;
  using _status_code_type =
    uint8_t;
  _status_code_type status_code;
  using _result_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _result_type result;

  // setters for named parameter idiom
  Type & set__ok(
    const bool & _arg)
  {
    this->ok = _arg;
    return *this;
  }
  Type & set__status_code(
    const uint8_t & _arg)
  {
    this->status_code = _arg;
    return *this;
  }
  Type & set__result(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->result = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    grpr2f85_ifaces::srv::SetGripperState_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const grpr2f85_ifaces::srv::SetGripperState_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<grpr2f85_ifaces::srv::SetGripperState_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<grpr2f85_ifaces::srv::SetGripperState_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      grpr2f85_ifaces::srv::SetGripperState_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<grpr2f85_ifaces::srv::SetGripperState_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      grpr2f85_ifaces::srv::SetGripperState_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<grpr2f85_ifaces::srv::SetGripperState_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<grpr2f85_ifaces::srv::SetGripperState_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<grpr2f85_ifaces::srv::SetGripperState_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__grpr2f85_ifaces__srv__SetGripperState_Response
    std::shared_ptr<grpr2f85_ifaces::srv::SetGripperState_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__grpr2f85_ifaces__srv__SetGripperState_Response
    std::shared_ptr<grpr2f85_ifaces::srv::SetGripperState_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SetGripperState_Response_ & other) const
  {
    if (this->ok != other.ok) {
      return false;
    }
    if (this->status_code != other.status_code) {
      return false;
    }
    if (this->result != other.result) {
      return false;
    }
    return true;
  }
  bool operator!=(const SetGripperState_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SetGripperState_Response_

// alias to use template instance with default allocator
using SetGripperState_Response =
  grpr2f85_ifaces::srv::SetGripperState_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace grpr2f85_ifaces

namespace grpr2f85_ifaces
{

namespace srv
{

struct SetGripperState
{
  using Request = grpr2f85_ifaces::srv::SetGripperState_Request;
  using Response = grpr2f85_ifaces::srv::SetGripperState_Response;
};

}  // namespace srv

}  // namespace grpr2f85_ifaces

#endif  // GRPR2F85_IFACES__SRV__DETAIL__SET_GRIPPER_STATE__STRUCT_HPP_
