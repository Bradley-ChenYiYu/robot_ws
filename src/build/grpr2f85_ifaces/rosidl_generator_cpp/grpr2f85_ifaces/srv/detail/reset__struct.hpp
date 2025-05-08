// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from grpr2f85_ifaces:srv/Reset.idl
// generated code does not contain a copyright notice

#ifndef GRPR2F85_IFACES__SRV__DETAIL__RESET__STRUCT_HPP_
#define GRPR2F85_IFACES__SRV__DETAIL__RESET__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__grpr2f85_ifaces__srv__Reset_Request __attribute__((deprecated))
#else
# define DEPRECATED__grpr2f85_ifaces__srv__Reset_Request __declspec(deprecated)
#endif

namespace grpr2f85_ifaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct Reset_Request_
{
  using Type = Reset_Request_<ContainerAllocator>;

  explicit Reset_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->get = false;
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->get = false;
    }
  }

  explicit Reset_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->get = false;
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->get = false;
    }
  }

  // field types and members
  using _get_type =
    bool;
  _get_type get;

  // setters for named parameter idiom
  Type & set__get(
    const bool & _arg)
  {
    this->get = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    grpr2f85_ifaces::srv::Reset_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const grpr2f85_ifaces::srv::Reset_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<grpr2f85_ifaces::srv::Reset_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<grpr2f85_ifaces::srv::Reset_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      grpr2f85_ifaces::srv::Reset_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<grpr2f85_ifaces::srv::Reset_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      grpr2f85_ifaces::srv::Reset_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<grpr2f85_ifaces::srv::Reset_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<grpr2f85_ifaces::srv::Reset_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<grpr2f85_ifaces::srv::Reset_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__grpr2f85_ifaces__srv__Reset_Request
    std::shared_ptr<grpr2f85_ifaces::srv::Reset_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__grpr2f85_ifaces__srv__Reset_Request
    std::shared_ptr<grpr2f85_ifaces::srv::Reset_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Reset_Request_ & other) const
  {
    if (this->get != other.get) {
      return false;
    }
    return true;
  }
  bool operator!=(const Reset_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Reset_Request_

// alias to use template instance with default allocator
using Reset_Request =
  grpr2f85_ifaces::srv::Reset_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace grpr2f85_ifaces


#ifndef _WIN32
# define DEPRECATED__grpr2f85_ifaces__srv__Reset_Response __attribute__((deprecated))
#else
# define DEPRECATED__grpr2f85_ifaces__srv__Reset_Response __declspec(deprecated)
#endif

namespace grpr2f85_ifaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct Reset_Response_
{
  using Type = Reset_Response_<ContainerAllocator>;

  explicit Reset_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->ok = false;
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->ok = false;
    }
  }

  explicit Reset_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->ok = false;
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->ok = false;
    }
  }

  // field types and members
  using _ok_type =
    bool;
  _ok_type ok;

  // setters for named parameter idiom
  Type & set__ok(
    const bool & _arg)
  {
    this->ok = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    grpr2f85_ifaces::srv::Reset_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const grpr2f85_ifaces::srv::Reset_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<grpr2f85_ifaces::srv::Reset_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<grpr2f85_ifaces::srv::Reset_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      grpr2f85_ifaces::srv::Reset_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<grpr2f85_ifaces::srv::Reset_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      grpr2f85_ifaces::srv::Reset_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<grpr2f85_ifaces::srv::Reset_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<grpr2f85_ifaces::srv::Reset_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<grpr2f85_ifaces::srv::Reset_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__grpr2f85_ifaces__srv__Reset_Response
    std::shared_ptr<grpr2f85_ifaces::srv::Reset_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__grpr2f85_ifaces__srv__Reset_Response
    std::shared_ptr<grpr2f85_ifaces::srv::Reset_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Reset_Response_ & other) const
  {
    if (this->ok != other.ok) {
      return false;
    }
    return true;
  }
  bool operator!=(const Reset_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Reset_Response_

// alias to use template instance with default allocator
using Reset_Response =
  grpr2f85_ifaces::srv::Reset_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace grpr2f85_ifaces

namespace grpr2f85_ifaces
{

namespace srv
{

struct Reset
{
  using Request = grpr2f85_ifaces::srv::Reset_Request;
  using Response = grpr2f85_ifaces::srv::Reset_Response;
};

}  // namespace srv

}  // namespace grpr2f85_ifaces

#endif  // GRPR2F85_IFACES__SRV__DETAIL__RESET__STRUCT_HPP_
