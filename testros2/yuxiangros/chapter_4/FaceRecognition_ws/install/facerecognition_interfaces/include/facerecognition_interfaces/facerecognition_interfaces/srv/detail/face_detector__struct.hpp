// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from facerecognition_interfaces:srv/FaceDetector.idl
// generated code does not contain a copyright notice

#ifndef FACERECOGNITION_INTERFACES__SRV__DETAIL__FACE_DETECTOR__STRUCT_HPP_
#define FACERECOGNITION_INTERFACES__SRV__DETAIL__FACE_DETECTOR__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'image'
#include "sensor_msgs/msg/detail/image__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__facerecognition_interfaces__srv__FaceDetector_Request __attribute__((deprecated))
#else
# define DEPRECATED__facerecognition_interfaces__srv__FaceDetector_Request __declspec(deprecated)
#endif

namespace facerecognition_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct FaceDetector_Request_
{
  using Type = FaceDetector_Request_<ContainerAllocator>;

  explicit FaceDetector_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : image(_init)
  {
    (void)_init;
  }

  explicit FaceDetector_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : image(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _image_type =
    sensor_msgs::msg::Image_<ContainerAllocator>;
  _image_type image;

  // setters for named parameter idiom
  Type & set__image(
    const sensor_msgs::msg::Image_<ContainerAllocator> & _arg)
  {
    this->image = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    facerecognition_interfaces::srv::FaceDetector_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const facerecognition_interfaces::srv::FaceDetector_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<facerecognition_interfaces::srv::FaceDetector_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<facerecognition_interfaces::srv::FaceDetector_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      facerecognition_interfaces::srv::FaceDetector_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<facerecognition_interfaces::srv::FaceDetector_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      facerecognition_interfaces::srv::FaceDetector_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<facerecognition_interfaces::srv::FaceDetector_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<facerecognition_interfaces::srv::FaceDetector_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<facerecognition_interfaces::srv::FaceDetector_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__facerecognition_interfaces__srv__FaceDetector_Request
    std::shared_ptr<facerecognition_interfaces::srv::FaceDetector_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__facerecognition_interfaces__srv__FaceDetector_Request
    std::shared_ptr<facerecognition_interfaces::srv::FaceDetector_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const FaceDetector_Request_ & other) const
  {
    if (this->image != other.image) {
      return false;
    }
    return true;
  }
  bool operator!=(const FaceDetector_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct FaceDetector_Request_

// alias to use template instance with default allocator
using FaceDetector_Request =
  facerecognition_interfaces::srv::FaceDetector_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace facerecognition_interfaces


#ifndef _WIN32
# define DEPRECATED__facerecognition_interfaces__srv__FaceDetector_Response __attribute__((deprecated))
#else
# define DEPRECATED__facerecognition_interfaces__srv__FaceDetector_Response __declspec(deprecated)
#endif

namespace facerecognition_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct FaceDetector_Response_
{
  using Type = FaceDetector_Response_<ContainerAllocator>;

  explicit FaceDetector_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->number = 0;
      this->usr_time = 0.0f;
    }
  }

  explicit FaceDetector_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->number = 0;
      this->usr_time = 0.0f;
    }
  }

  // field types and members
  using _number_type =
    int16_t;
  _number_type number;
  using _usr_time_type =
    float;
  _usr_time_type usr_time;
  using _top_type =
    std::vector<int32_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<int32_t>>;
  _top_type top;
  using _right_type =
    std::vector<int32_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<int32_t>>;
  _right_type right;
  using _bottom_type =
    std::vector<int32_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<int32_t>>;
  _bottom_type bottom;
  using _left_type =
    std::vector<int32_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<int32_t>>;
  _left_type left;

  // setters for named parameter idiom
  Type & set__number(
    const int16_t & _arg)
  {
    this->number = _arg;
    return *this;
  }
  Type & set__usr_time(
    const float & _arg)
  {
    this->usr_time = _arg;
    return *this;
  }
  Type & set__top(
    const std::vector<int32_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<int32_t>> & _arg)
  {
    this->top = _arg;
    return *this;
  }
  Type & set__right(
    const std::vector<int32_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<int32_t>> & _arg)
  {
    this->right = _arg;
    return *this;
  }
  Type & set__bottom(
    const std::vector<int32_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<int32_t>> & _arg)
  {
    this->bottom = _arg;
    return *this;
  }
  Type & set__left(
    const std::vector<int32_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<int32_t>> & _arg)
  {
    this->left = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    facerecognition_interfaces::srv::FaceDetector_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const facerecognition_interfaces::srv::FaceDetector_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<facerecognition_interfaces::srv::FaceDetector_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<facerecognition_interfaces::srv::FaceDetector_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      facerecognition_interfaces::srv::FaceDetector_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<facerecognition_interfaces::srv::FaceDetector_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      facerecognition_interfaces::srv::FaceDetector_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<facerecognition_interfaces::srv::FaceDetector_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<facerecognition_interfaces::srv::FaceDetector_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<facerecognition_interfaces::srv::FaceDetector_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__facerecognition_interfaces__srv__FaceDetector_Response
    std::shared_ptr<facerecognition_interfaces::srv::FaceDetector_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__facerecognition_interfaces__srv__FaceDetector_Response
    std::shared_ptr<facerecognition_interfaces::srv::FaceDetector_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const FaceDetector_Response_ & other) const
  {
    if (this->number != other.number) {
      return false;
    }
    if (this->usr_time != other.usr_time) {
      return false;
    }
    if (this->top != other.top) {
      return false;
    }
    if (this->right != other.right) {
      return false;
    }
    if (this->bottom != other.bottom) {
      return false;
    }
    if (this->left != other.left) {
      return false;
    }
    return true;
  }
  bool operator!=(const FaceDetector_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct FaceDetector_Response_

// alias to use template instance with default allocator
using FaceDetector_Response =
  facerecognition_interfaces::srv::FaceDetector_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace facerecognition_interfaces

namespace facerecognition_interfaces
{

namespace srv
{

struct FaceDetector
{
  using Request = facerecognition_interfaces::srv::FaceDetector_Request;
  using Response = facerecognition_interfaces::srv::FaceDetector_Response;
};

}  // namespace srv

}  // namespace facerecognition_interfaces

#endif  // FACERECOGNITION_INTERFACES__SRV__DETAIL__FACE_DETECTOR__STRUCT_HPP_
