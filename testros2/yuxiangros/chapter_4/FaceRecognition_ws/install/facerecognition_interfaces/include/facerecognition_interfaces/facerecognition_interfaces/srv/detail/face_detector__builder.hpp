// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from facerecognition_interfaces:srv/FaceDetector.idl
// generated code does not contain a copyright notice

#ifndef FACERECOGNITION_INTERFACES__SRV__DETAIL__FACE_DETECTOR__BUILDER_HPP_
#define FACERECOGNITION_INTERFACES__SRV__DETAIL__FACE_DETECTOR__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "facerecognition_interfaces/srv/detail/face_detector__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace facerecognition_interfaces
{

namespace srv
{

namespace builder
{

class Init_FaceDetector_Request_image
{
public:
  Init_FaceDetector_Request_image()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::facerecognition_interfaces::srv::FaceDetector_Request image(::facerecognition_interfaces::srv::FaceDetector_Request::_image_type arg)
  {
    msg_.image = std::move(arg);
    return std::move(msg_);
  }

private:
  ::facerecognition_interfaces::srv::FaceDetector_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::facerecognition_interfaces::srv::FaceDetector_Request>()
{
  return facerecognition_interfaces::srv::builder::Init_FaceDetector_Request_image();
}

}  // namespace facerecognition_interfaces


namespace facerecognition_interfaces
{

namespace srv
{

namespace builder
{

class Init_FaceDetector_Response_left
{
public:
  explicit Init_FaceDetector_Response_left(::facerecognition_interfaces::srv::FaceDetector_Response & msg)
  : msg_(msg)
  {}
  ::facerecognition_interfaces::srv::FaceDetector_Response left(::facerecognition_interfaces::srv::FaceDetector_Response::_left_type arg)
  {
    msg_.left = std::move(arg);
    return std::move(msg_);
  }

private:
  ::facerecognition_interfaces::srv::FaceDetector_Response msg_;
};

class Init_FaceDetector_Response_bottom
{
public:
  explicit Init_FaceDetector_Response_bottom(::facerecognition_interfaces::srv::FaceDetector_Response & msg)
  : msg_(msg)
  {}
  Init_FaceDetector_Response_left bottom(::facerecognition_interfaces::srv::FaceDetector_Response::_bottom_type arg)
  {
    msg_.bottom = std::move(arg);
    return Init_FaceDetector_Response_left(msg_);
  }

private:
  ::facerecognition_interfaces::srv::FaceDetector_Response msg_;
};

class Init_FaceDetector_Response_right
{
public:
  explicit Init_FaceDetector_Response_right(::facerecognition_interfaces::srv::FaceDetector_Response & msg)
  : msg_(msg)
  {}
  Init_FaceDetector_Response_bottom right(::facerecognition_interfaces::srv::FaceDetector_Response::_right_type arg)
  {
    msg_.right = std::move(arg);
    return Init_FaceDetector_Response_bottom(msg_);
  }

private:
  ::facerecognition_interfaces::srv::FaceDetector_Response msg_;
};

class Init_FaceDetector_Response_top
{
public:
  explicit Init_FaceDetector_Response_top(::facerecognition_interfaces::srv::FaceDetector_Response & msg)
  : msg_(msg)
  {}
  Init_FaceDetector_Response_right top(::facerecognition_interfaces::srv::FaceDetector_Response::_top_type arg)
  {
    msg_.top = std::move(arg);
    return Init_FaceDetector_Response_right(msg_);
  }

private:
  ::facerecognition_interfaces::srv::FaceDetector_Response msg_;
};

class Init_FaceDetector_Response_usr_time
{
public:
  explicit Init_FaceDetector_Response_usr_time(::facerecognition_interfaces::srv::FaceDetector_Response & msg)
  : msg_(msg)
  {}
  Init_FaceDetector_Response_top usr_time(::facerecognition_interfaces::srv::FaceDetector_Response::_usr_time_type arg)
  {
    msg_.usr_time = std::move(arg);
    return Init_FaceDetector_Response_top(msg_);
  }

private:
  ::facerecognition_interfaces::srv::FaceDetector_Response msg_;
};

class Init_FaceDetector_Response_number
{
public:
  Init_FaceDetector_Response_number()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_FaceDetector_Response_usr_time number(::facerecognition_interfaces::srv::FaceDetector_Response::_number_type arg)
  {
    msg_.number = std::move(arg);
    return Init_FaceDetector_Response_usr_time(msg_);
  }

private:
  ::facerecognition_interfaces::srv::FaceDetector_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::facerecognition_interfaces::srv::FaceDetector_Response>()
{
  return facerecognition_interfaces::srv::builder::Init_FaceDetector_Response_number();
}

}  // namespace facerecognition_interfaces

#endif  // FACERECOGNITION_INTERFACES__SRV__DETAIL__FACE_DETECTOR__BUILDER_HPP_
