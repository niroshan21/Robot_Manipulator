// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from manipulator_msgs:action/ManipulatorTask.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "manipulator_msgs/action/manipulator_task.h"


#ifndef MANIPULATOR_MSGS__ACTION__DETAIL__MANIPULATOR_TASK__STRUCT_H_
#define MANIPULATOR_MSGS__ACTION__DETAIL__MANIPULATOR_TASK__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in action/ManipulatorTask in the package manipulator_msgs.
typedef struct manipulator_msgs__action__ManipulatorTask_Goal
{
  int32_t task_number;
} manipulator_msgs__action__ManipulatorTask_Goal;

// Struct for a sequence of manipulator_msgs__action__ManipulatorTask_Goal.
typedef struct manipulator_msgs__action__ManipulatorTask_Goal__Sequence
{
  manipulator_msgs__action__ManipulatorTask_Goal * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} manipulator_msgs__action__ManipulatorTask_Goal__Sequence;

// Constants defined in the message

/// Struct defined in action/ManipulatorTask in the package manipulator_msgs.
typedef struct manipulator_msgs__action__ManipulatorTask_Result
{
  bool success;
} manipulator_msgs__action__ManipulatorTask_Result;

// Struct for a sequence of manipulator_msgs__action__ManipulatorTask_Result.
typedef struct manipulator_msgs__action__ManipulatorTask_Result__Sequence
{
  manipulator_msgs__action__ManipulatorTask_Result * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} manipulator_msgs__action__ManipulatorTask_Result__Sequence;

// Constants defined in the message

/// Struct defined in action/ManipulatorTask in the package manipulator_msgs.
typedef struct manipulator_msgs__action__ManipulatorTask_Feedback
{
  int32_t percentage;
} manipulator_msgs__action__ManipulatorTask_Feedback;

// Struct for a sequence of manipulator_msgs__action__ManipulatorTask_Feedback.
typedef struct manipulator_msgs__action__ManipulatorTask_Feedback__Sequence
{
  manipulator_msgs__action__ManipulatorTask_Feedback * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} manipulator_msgs__action__ManipulatorTask_Feedback__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'goal_id'
#include "unique_identifier_msgs/msg/detail/uuid__struct.h"
// Member 'goal'
#include "manipulator_msgs/action/detail/manipulator_task__struct.h"

/// Struct defined in action/ManipulatorTask in the package manipulator_msgs.
typedef struct manipulator_msgs__action__ManipulatorTask_SendGoal_Request
{
  unique_identifier_msgs__msg__UUID goal_id;
  manipulator_msgs__action__ManipulatorTask_Goal goal;
} manipulator_msgs__action__ManipulatorTask_SendGoal_Request;

// Struct for a sequence of manipulator_msgs__action__ManipulatorTask_SendGoal_Request.
typedef struct manipulator_msgs__action__ManipulatorTask_SendGoal_Request__Sequence
{
  manipulator_msgs__action__ManipulatorTask_SendGoal_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} manipulator_msgs__action__ManipulatorTask_SendGoal_Request__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'stamp'
#include "builtin_interfaces/msg/detail/time__struct.h"

/// Struct defined in action/ManipulatorTask in the package manipulator_msgs.
typedef struct manipulator_msgs__action__ManipulatorTask_SendGoal_Response
{
  bool accepted;
  builtin_interfaces__msg__Time stamp;
} manipulator_msgs__action__ManipulatorTask_SendGoal_Response;

// Struct for a sequence of manipulator_msgs__action__ManipulatorTask_SendGoal_Response.
typedef struct manipulator_msgs__action__ManipulatorTask_SendGoal_Response__Sequence
{
  manipulator_msgs__action__ManipulatorTask_SendGoal_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} manipulator_msgs__action__ManipulatorTask_SendGoal_Response__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__struct.h"

// constants for array fields with an upper bound
// request
enum
{
  manipulator_msgs__action__ManipulatorTask_SendGoal_Event__request__MAX_SIZE = 1
};
// response
enum
{
  manipulator_msgs__action__ManipulatorTask_SendGoal_Event__response__MAX_SIZE = 1
};

/// Struct defined in action/ManipulatorTask in the package manipulator_msgs.
typedef struct manipulator_msgs__action__ManipulatorTask_SendGoal_Event
{
  service_msgs__msg__ServiceEventInfo info;
  manipulator_msgs__action__ManipulatorTask_SendGoal_Request__Sequence request;
  manipulator_msgs__action__ManipulatorTask_SendGoal_Response__Sequence response;
} manipulator_msgs__action__ManipulatorTask_SendGoal_Event;

// Struct for a sequence of manipulator_msgs__action__ManipulatorTask_SendGoal_Event.
typedef struct manipulator_msgs__action__ManipulatorTask_SendGoal_Event__Sequence
{
  manipulator_msgs__action__ManipulatorTask_SendGoal_Event * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} manipulator_msgs__action__ManipulatorTask_SendGoal_Event__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'goal_id'
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__struct.h"

/// Struct defined in action/ManipulatorTask in the package manipulator_msgs.
typedef struct manipulator_msgs__action__ManipulatorTask_GetResult_Request
{
  unique_identifier_msgs__msg__UUID goal_id;
} manipulator_msgs__action__ManipulatorTask_GetResult_Request;

// Struct for a sequence of manipulator_msgs__action__ManipulatorTask_GetResult_Request.
typedef struct manipulator_msgs__action__ManipulatorTask_GetResult_Request__Sequence
{
  manipulator_msgs__action__ManipulatorTask_GetResult_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} manipulator_msgs__action__ManipulatorTask_GetResult_Request__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'result'
// already included above
// #include "manipulator_msgs/action/detail/manipulator_task__struct.h"

/// Struct defined in action/ManipulatorTask in the package manipulator_msgs.
typedef struct manipulator_msgs__action__ManipulatorTask_GetResult_Response
{
  int8_t status;
  manipulator_msgs__action__ManipulatorTask_Result result;
} manipulator_msgs__action__ManipulatorTask_GetResult_Response;

// Struct for a sequence of manipulator_msgs__action__ManipulatorTask_GetResult_Response.
typedef struct manipulator_msgs__action__ManipulatorTask_GetResult_Response__Sequence
{
  manipulator_msgs__action__ManipulatorTask_GetResult_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} manipulator_msgs__action__ManipulatorTask_GetResult_Response__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'info'
// already included above
// #include "service_msgs/msg/detail/service_event_info__struct.h"

// constants for array fields with an upper bound
// request
enum
{
  manipulator_msgs__action__ManipulatorTask_GetResult_Event__request__MAX_SIZE = 1
};
// response
enum
{
  manipulator_msgs__action__ManipulatorTask_GetResult_Event__response__MAX_SIZE = 1
};

/// Struct defined in action/ManipulatorTask in the package manipulator_msgs.
typedef struct manipulator_msgs__action__ManipulatorTask_GetResult_Event
{
  service_msgs__msg__ServiceEventInfo info;
  manipulator_msgs__action__ManipulatorTask_GetResult_Request__Sequence request;
  manipulator_msgs__action__ManipulatorTask_GetResult_Response__Sequence response;
} manipulator_msgs__action__ManipulatorTask_GetResult_Event;

// Struct for a sequence of manipulator_msgs__action__ManipulatorTask_GetResult_Event.
typedef struct manipulator_msgs__action__ManipulatorTask_GetResult_Event__Sequence
{
  manipulator_msgs__action__ManipulatorTask_GetResult_Event * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} manipulator_msgs__action__ManipulatorTask_GetResult_Event__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'goal_id'
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__struct.h"
// Member 'feedback'
// already included above
// #include "manipulator_msgs/action/detail/manipulator_task__struct.h"

/// Struct defined in action/ManipulatorTask in the package manipulator_msgs.
typedef struct manipulator_msgs__action__ManipulatorTask_FeedbackMessage
{
  unique_identifier_msgs__msg__UUID goal_id;
  manipulator_msgs__action__ManipulatorTask_Feedback feedback;
} manipulator_msgs__action__ManipulatorTask_FeedbackMessage;

// Struct for a sequence of manipulator_msgs__action__ManipulatorTask_FeedbackMessage.
typedef struct manipulator_msgs__action__ManipulatorTask_FeedbackMessage__Sequence
{
  manipulator_msgs__action__ManipulatorTask_FeedbackMessage * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} manipulator_msgs__action__ManipulatorTask_FeedbackMessage__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // MANIPULATOR_MSGS__ACTION__DETAIL__MANIPULATOR_TASK__STRUCT_H_
