// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from facerecognition_interfaces:srv/FaceDetector.idl
// generated code does not contain a copyright notice

#ifndef FACERECOGNITION_INTERFACES__SRV__DETAIL__FACE_DETECTOR__STRUCT_H_
#define FACERECOGNITION_INTERFACES__SRV__DETAIL__FACE_DETECTOR__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'image'
#include "sensor_msgs/msg/detail/image__struct.h"

/// Struct defined in srv/FaceDetector in the package facerecognition_interfaces.
typedef struct facerecognition_interfaces__srv__FaceDetector_Request
{
  /// 原始图像
  sensor_msgs__msg__Image image;
} facerecognition_interfaces__srv__FaceDetector_Request;

// Struct for a sequence of facerecognition_interfaces__srv__FaceDetector_Request.
typedef struct facerecognition_interfaces__srv__FaceDetector_Request__Sequence
{
  facerecognition_interfaces__srv__FaceDetector_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} facerecognition_interfaces__srv__FaceDetector_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'top'
// Member 'right'
// Member 'bottom'
// Member 'left'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in srv/FaceDetector in the package facerecognition_interfaces.
typedef struct facerecognition_interfaces__srv__FaceDetector_Response
{
  /// 人脸数
  int16_t number;
  /// 识别耗时
  float usr_time;
  /// 人脸在图像中的位置
  rosidl_runtime_c__int32__Sequence top;
  rosidl_runtime_c__int32__Sequence right;
  rosidl_runtime_c__int32__Sequence bottom;
  rosidl_runtime_c__int32__Sequence left;
} facerecognition_interfaces__srv__FaceDetector_Response;

// Struct for a sequence of facerecognition_interfaces__srv__FaceDetector_Response.
typedef struct facerecognition_interfaces__srv__FaceDetector_Response__Sequence
{
  facerecognition_interfaces__srv__FaceDetector_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} facerecognition_interfaces__srv__FaceDetector_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // FACERECOGNITION_INTERFACES__SRV__DETAIL__FACE_DETECTOR__STRUCT_H_
