// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from manipulator_msgs:action/ManipulatorTask.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "manipulator_msgs/action/manipulator_task.h"


#ifndef MANIPULATOR_MSGS__ACTION__DETAIL__MANIPULATOR_TASK__FUNCTIONS_H_
#define MANIPULATOR_MSGS__ACTION__DETAIL__MANIPULATOR_TASK__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/action_type_support_struct.h"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_runtime_c/service_type_support_struct.h"
#include "rosidl_runtime_c/type_description/type_description__struct.h"
#include "rosidl_runtime_c/type_description/type_source__struct.h"
#include "rosidl_runtime_c/type_hash.h"
#include "rosidl_runtime_c/visibility_control.h"
#include "manipulator_msgs/msg/rosidl_generator_c__visibility_control.h"

#include "manipulator_msgs/action/detail/manipulator_task__struct.h"

/// Retrieve pointer to the hash of the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_type_hash_t *
manipulator_msgs__action__ManipulatorTask__get_type_hash(
  const rosidl_action_type_support_t * type_support);

/// Retrieve pointer to the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeDescription *
manipulator_msgs__action__ManipulatorTask__get_type_description(
  const rosidl_action_type_support_t * type_support);

/// Retrieve pointer to the single raw source text that defined this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource *
manipulator_msgs__action__ManipulatorTask__get_individual_type_description_source(
  const rosidl_action_type_support_t * type_support);

/// Retrieve pointer to the recursive raw sources that defined the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource__Sequence *
manipulator_msgs__action__ManipulatorTask__get_type_description_sources(
  const rosidl_action_type_support_t * type_support);

/// Initialize action/ManipulatorTask message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * manipulator_msgs__action__ManipulatorTask_Goal
 * )) before or use
 * manipulator_msgs__action__ManipulatorTask_Goal__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_Goal__init(manipulator_msgs__action__ManipulatorTask_Goal * msg);

/// Finalize action/ManipulatorTask message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_Goal__fini(manipulator_msgs__action__ManipulatorTask_Goal * msg);

/// Create action/ManipulatorTask message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * manipulator_msgs__action__ManipulatorTask_Goal__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
manipulator_msgs__action__ManipulatorTask_Goal *
manipulator_msgs__action__ManipulatorTask_Goal__create(void);

/// Destroy action/ManipulatorTask message.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_Goal__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_Goal__destroy(manipulator_msgs__action__ManipulatorTask_Goal * msg);

/// Check for action/ManipulatorTask message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_Goal__are_equal(const manipulator_msgs__action__ManipulatorTask_Goal * lhs, const manipulator_msgs__action__ManipulatorTask_Goal * rhs);

/// Copy a action/ManipulatorTask message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_Goal__copy(
  const manipulator_msgs__action__ManipulatorTask_Goal * input,
  manipulator_msgs__action__ManipulatorTask_Goal * output);

/// Retrieve pointer to the hash of the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_type_hash_t *
manipulator_msgs__action__ManipulatorTask_Goal__get_type_hash(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeDescription *
manipulator_msgs__action__ManipulatorTask_Goal__get_type_description(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the single raw source text that defined this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource *
manipulator_msgs__action__ManipulatorTask_Goal__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the recursive raw sources that defined the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource__Sequence *
manipulator_msgs__action__ManipulatorTask_Goal__get_type_description_sources(
  const rosidl_message_type_support_t * type_support);

/// Initialize array of action/ManipulatorTask messages.
/**
 * It allocates the memory for the number of elements and calls
 * manipulator_msgs__action__ManipulatorTask_Goal__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_Goal__Sequence__init(manipulator_msgs__action__ManipulatorTask_Goal__Sequence * array, size_t size);

/// Finalize array of action/ManipulatorTask messages.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_Goal__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_Goal__Sequence__fini(manipulator_msgs__action__ManipulatorTask_Goal__Sequence * array);

/// Create array of action/ManipulatorTask messages.
/**
 * It allocates the memory for the array and calls
 * manipulator_msgs__action__ManipulatorTask_Goal__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
manipulator_msgs__action__ManipulatorTask_Goal__Sequence *
manipulator_msgs__action__ManipulatorTask_Goal__Sequence__create(size_t size);

/// Destroy array of action/ManipulatorTask messages.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_Goal__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_Goal__Sequence__destroy(manipulator_msgs__action__ManipulatorTask_Goal__Sequence * array);

/// Check for action/ManipulatorTask message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_Goal__Sequence__are_equal(const manipulator_msgs__action__ManipulatorTask_Goal__Sequence * lhs, const manipulator_msgs__action__ManipulatorTask_Goal__Sequence * rhs);

/// Copy an array of action/ManipulatorTask messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_Goal__Sequence__copy(
  const manipulator_msgs__action__ManipulatorTask_Goal__Sequence * input,
  manipulator_msgs__action__ManipulatorTask_Goal__Sequence * output);

/// Initialize action/ManipulatorTask message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * manipulator_msgs__action__ManipulatorTask_Result
 * )) before or use
 * manipulator_msgs__action__ManipulatorTask_Result__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_Result__init(manipulator_msgs__action__ManipulatorTask_Result * msg);

/// Finalize action/ManipulatorTask message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_Result__fini(manipulator_msgs__action__ManipulatorTask_Result * msg);

/// Create action/ManipulatorTask message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * manipulator_msgs__action__ManipulatorTask_Result__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
manipulator_msgs__action__ManipulatorTask_Result *
manipulator_msgs__action__ManipulatorTask_Result__create(void);

/// Destroy action/ManipulatorTask message.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_Result__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_Result__destroy(manipulator_msgs__action__ManipulatorTask_Result * msg);

/// Check for action/ManipulatorTask message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_Result__are_equal(const manipulator_msgs__action__ManipulatorTask_Result * lhs, const manipulator_msgs__action__ManipulatorTask_Result * rhs);

/// Copy a action/ManipulatorTask message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_Result__copy(
  const manipulator_msgs__action__ManipulatorTask_Result * input,
  manipulator_msgs__action__ManipulatorTask_Result * output);

/// Retrieve pointer to the hash of the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_type_hash_t *
manipulator_msgs__action__ManipulatorTask_Result__get_type_hash(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeDescription *
manipulator_msgs__action__ManipulatorTask_Result__get_type_description(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the single raw source text that defined this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource *
manipulator_msgs__action__ManipulatorTask_Result__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the recursive raw sources that defined the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource__Sequence *
manipulator_msgs__action__ManipulatorTask_Result__get_type_description_sources(
  const rosidl_message_type_support_t * type_support);

/// Initialize array of action/ManipulatorTask messages.
/**
 * It allocates the memory for the number of elements and calls
 * manipulator_msgs__action__ManipulatorTask_Result__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_Result__Sequence__init(manipulator_msgs__action__ManipulatorTask_Result__Sequence * array, size_t size);

/// Finalize array of action/ManipulatorTask messages.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_Result__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_Result__Sequence__fini(manipulator_msgs__action__ManipulatorTask_Result__Sequence * array);

/// Create array of action/ManipulatorTask messages.
/**
 * It allocates the memory for the array and calls
 * manipulator_msgs__action__ManipulatorTask_Result__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
manipulator_msgs__action__ManipulatorTask_Result__Sequence *
manipulator_msgs__action__ManipulatorTask_Result__Sequence__create(size_t size);

/// Destroy array of action/ManipulatorTask messages.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_Result__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_Result__Sequence__destroy(manipulator_msgs__action__ManipulatorTask_Result__Sequence * array);

/// Check for action/ManipulatorTask message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_Result__Sequence__are_equal(const manipulator_msgs__action__ManipulatorTask_Result__Sequence * lhs, const manipulator_msgs__action__ManipulatorTask_Result__Sequence * rhs);

/// Copy an array of action/ManipulatorTask messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_Result__Sequence__copy(
  const manipulator_msgs__action__ManipulatorTask_Result__Sequence * input,
  manipulator_msgs__action__ManipulatorTask_Result__Sequence * output);

/// Initialize action/ManipulatorTask message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * manipulator_msgs__action__ManipulatorTask_Feedback
 * )) before or use
 * manipulator_msgs__action__ManipulatorTask_Feedback__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_Feedback__init(manipulator_msgs__action__ManipulatorTask_Feedback * msg);

/// Finalize action/ManipulatorTask message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_Feedback__fini(manipulator_msgs__action__ManipulatorTask_Feedback * msg);

/// Create action/ManipulatorTask message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * manipulator_msgs__action__ManipulatorTask_Feedback__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
manipulator_msgs__action__ManipulatorTask_Feedback *
manipulator_msgs__action__ManipulatorTask_Feedback__create(void);

/// Destroy action/ManipulatorTask message.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_Feedback__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_Feedback__destroy(manipulator_msgs__action__ManipulatorTask_Feedback * msg);

/// Check for action/ManipulatorTask message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_Feedback__are_equal(const manipulator_msgs__action__ManipulatorTask_Feedback * lhs, const manipulator_msgs__action__ManipulatorTask_Feedback * rhs);

/// Copy a action/ManipulatorTask message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_Feedback__copy(
  const manipulator_msgs__action__ManipulatorTask_Feedback * input,
  manipulator_msgs__action__ManipulatorTask_Feedback * output);

/// Retrieve pointer to the hash of the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_type_hash_t *
manipulator_msgs__action__ManipulatorTask_Feedback__get_type_hash(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeDescription *
manipulator_msgs__action__ManipulatorTask_Feedback__get_type_description(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the single raw source text that defined this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource *
manipulator_msgs__action__ManipulatorTask_Feedback__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the recursive raw sources that defined the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource__Sequence *
manipulator_msgs__action__ManipulatorTask_Feedback__get_type_description_sources(
  const rosidl_message_type_support_t * type_support);

/// Initialize array of action/ManipulatorTask messages.
/**
 * It allocates the memory for the number of elements and calls
 * manipulator_msgs__action__ManipulatorTask_Feedback__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_Feedback__Sequence__init(manipulator_msgs__action__ManipulatorTask_Feedback__Sequence * array, size_t size);

/// Finalize array of action/ManipulatorTask messages.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_Feedback__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_Feedback__Sequence__fini(manipulator_msgs__action__ManipulatorTask_Feedback__Sequence * array);

/// Create array of action/ManipulatorTask messages.
/**
 * It allocates the memory for the array and calls
 * manipulator_msgs__action__ManipulatorTask_Feedback__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
manipulator_msgs__action__ManipulatorTask_Feedback__Sequence *
manipulator_msgs__action__ManipulatorTask_Feedback__Sequence__create(size_t size);

/// Destroy array of action/ManipulatorTask messages.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_Feedback__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_Feedback__Sequence__destroy(manipulator_msgs__action__ManipulatorTask_Feedback__Sequence * array);

/// Check for action/ManipulatorTask message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_Feedback__Sequence__are_equal(const manipulator_msgs__action__ManipulatorTask_Feedback__Sequence * lhs, const manipulator_msgs__action__ManipulatorTask_Feedback__Sequence * rhs);

/// Copy an array of action/ManipulatorTask messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_Feedback__Sequence__copy(
  const manipulator_msgs__action__ManipulatorTask_Feedback__Sequence * input,
  manipulator_msgs__action__ManipulatorTask_Feedback__Sequence * output);

/// Retrieve pointer to the hash of the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_type_hash_t *
manipulator_msgs__action__ManipulatorTask_SendGoal__get_type_hash(
  const rosidl_service_type_support_t * type_support);

/// Retrieve pointer to the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeDescription *
manipulator_msgs__action__ManipulatorTask_SendGoal__get_type_description(
  const rosidl_service_type_support_t * type_support);

/// Retrieve pointer to the single raw source text that defined this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource *
manipulator_msgs__action__ManipulatorTask_SendGoal__get_individual_type_description_source(
  const rosidl_service_type_support_t * type_support);

/// Retrieve pointer to the recursive raw sources that defined the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource__Sequence *
manipulator_msgs__action__ManipulatorTask_SendGoal__get_type_description_sources(
  const rosidl_service_type_support_t * type_support);

/// Initialize action/ManipulatorTask message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Request
 * )) before or use
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Request__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_SendGoal_Request__init(manipulator_msgs__action__ManipulatorTask_SendGoal_Request * msg);

/// Finalize action/ManipulatorTask message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_SendGoal_Request__fini(manipulator_msgs__action__ManipulatorTask_SendGoal_Request * msg);

/// Create action/ManipulatorTask message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Request__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
manipulator_msgs__action__ManipulatorTask_SendGoal_Request *
manipulator_msgs__action__ManipulatorTask_SendGoal_Request__create(void);

/// Destroy action/ManipulatorTask message.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Request__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_SendGoal_Request__destroy(manipulator_msgs__action__ManipulatorTask_SendGoal_Request * msg);

/// Check for action/ManipulatorTask message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_SendGoal_Request__are_equal(const manipulator_msgs__action__ManipulatorTask_SendGoal_Request * lhs, const manipulator_msgs__action__ManipulatorTask_SendGoal_Request * rhs);

/// Copy a action/ManipulatorTask message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_SendGoal_Request__copy(
  const manipulator_msgs__action__ManipulatorTask_SendGoal_Request * input,
  manipulator_msgs__action__ManipulatorTask_SendGoal_Request * output);

/// Retrieve pointer to the hash of the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_type_hash_t *
manipulator_msgs__action__ManipulatorTask_SendGoal_Request__get_type_hash(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeDescription *
manipulator_msgs__action__ManipulatorTask_SendGoal_Request__get_type_description(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the single raw source text that defined this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource *
manipulator_msgs__action__ManipulatorTask_SendGoal_Request__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the recursive raw sources that defined the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource__Sequence *
manipulator_msgs__action__ManipulatorTask_SendGoal_Request__get_type_description_sources(
  const rosidl_message_type_support_t * type_support);

/// Initialize array of action/ManipulatorTask messages.
/**
 * It allocates the memory for the number of elements and calls
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Request__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_SendGoal_Request__Sequence__init(manipulator_msgs__action__ManipulatorTask_SendGoal_Request__Sequence * array, size_t size);

/// Finalize array of action/ManipulatorTask messages.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Request__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_SendGoal_Request__Sequence__fini(manipulator_msgs__action__ManipulatorTask_SendGoal_Request__Sequence * array);

/// Create array of action/ManipulatorTask messages.
/**
 * It allocates the memory for the array and calls
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Request__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
manipulator_msgs__action__ManipulatorTask_SendGoal_Request__Sequence *
manipulator_msgs__action__ManipulatorTask_SendGoal_Request__Sequence__create(size_t size);

/// Destroy array of action/ManipulatorTask messages.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Request__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_SendGoal_Request__Sequence__destroy(manipulator_msgs__action__ManipulatorTask_SendGoal_Request__Sequence * array);

/// Check for action/ManipulatorTask message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_SendGoal_Request__Sequence__are_equal(const manipulator_msgs__action__ManipulatorTask_SendGoal_Request__Sequence * lhs, const manipulator_msgs__action__ManipulatorTask_SendGoal_Request__Sequence * rhs);

/// Copy an array of action/ManipulatorTask messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_SendGoal_Request__Sequence__copy(
  const manipulator_msgs__action__ManipulatorTask_SendGoal_Request__Sequence * input,
  manipulator_msgs__action__ManipulatorTask_SendGoal_Request__Sequence * output);

/// Initialize action/ManipulatorTask message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Response
 * )) before or use
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Response__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_SendGoal_Response__init(manipulator_msgs__action__ManipulatorTask_SendGoal_Response * msg);

/// Finalize action/ManipulatorTask message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_SendGoal_Response__fini(manipulator_msgs__action__ManipulatorTask_SendGoal_Response * msg);

/// Create action/ManipulatorTask message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Response__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
manipulator_msgs__action__ManipulatorTask_SendGoal_Response *
manipulator_msgs__action__ManipulatorTask_SendGoal_Response__create(void);

/// Destroy action/ManipulatorTask message.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Response__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_SendGoal_Response__destroy(manipulator_msgs__action__ManipulatorTask_SendGoal_Response * msg);

/// Check for action/ManipulatorTask message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_SendGoal_Response__are_equal(const manipulator_msgs__action__ManipulatorTask_SendGoal_Response * lhs, const manipulator_msgs__action__ManipulatorTask_SendGoal_Response * rhs);

/// Copy a action/ManipulatorTask message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_SendGoal_Response__copy(
  const manipulator_msgs__action__ManipulatorTask_SendGoal_Response * input,
  manipulator_msgs__action__ManipulatorTask_SendGoal_Response * output);

/// Retrieve pointer to the hash of the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_type_hash_t *
manipulator_msgs__action__ManipulatorTask_SendGoal_Response__get_type_hash(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeDescription *
manipulator_msgs__action__ManipulatorTask_SendGoal_Response__get_type_description(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the single raw source text that defined this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource *
manipulator_msgs__action__ManipulatorTask_SendGoal_Response__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the recursive raw sources that defined the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource__Sequence *
manipulator_msgs__action__ManipulatorTask_SendGoal_Response__get_type_description_sources(
  const rosidl_message_type_support_t * type_support);

/// Initialize array of action/ManipulatorTask messages.
/**
 * It allocates the memory for the number of elements and calls
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Response__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_SendGoal_Response__Sequence__init(manipulator_msgs__action__ManipulatorTask_SendGoal_Response__Sequence * array, size_t size);

/// Finalize array of action/ManipulatorTask messages.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Response__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_SendGoal_Response__Sequence__fini(manipulator_msgs__action__ManipulatorTask_SendGoal_Response__Sequence * array);

/// Create array of action/ManipulatorTask messages.
/**
 * It allocates the memory for the array and calls
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Response__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
manipulator_msgs__action__ManipulatorTask_SendGoal_Response__Sequence *
manipulator_msgs__action__ManipulatorTask_SendGoal_Response__Sequence__create(size_t size);

/// Destroy array of action/ManipulatorTask messages.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Response__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_SendGoal_Response__Sequence__destroy(manipulator_msgs__action__ManipulatorTask_SendGoal_Response__Sequence * array);

/// Check for action/ManipulatorTask message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_SendGoal_Response__Sequence__are_equal(const manipulator_msgs__action__ManipulatorTask_SendGoal_Response__Sequence * lhs, const manipulator_msgs__action__ManipulatorTask_SendGoal_Response__Sequence * rhs);

/// Copy an array of action/ManipulatorTask messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_SendGoal_Response__Sequence__copy(
  const manipulator_msgs__action__ManipulatorTask_SendGoal_Response__Sequence * input,
  manipulator_msgs__action__ManipulatorTask_SendGoal_Response__Sequence * output);

/// Initialize action/ManipulatorTask message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Event
 * )) before or use
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Event__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_SendGoal_Event__init(manipulator_msgs__action__ManipulatorTask_SendGoal_Event * msg);

/// Finalize action/ManipulatorTask message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_SendGoal_Event__fini(manipulator_msgs__action__ManipulatorTask_SendGoal_Event * msg);

/// Create action/ManipulatorTask message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Event__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
manipulator_msgs__action__ManipulatorTask_SendGoal_Event *
manipulator_msgs__action__ManipulatorTask_SendGoal_Event__create(void);

/// Destroy action/ManipulatorTask message.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Event__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_SendGoal_Event__destroy(manipulator_msgs__action__ManipulatorTask_SendGoal_Event * msg);

/// Check for action/ManipulatorTask message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_SendGoal_Event__are_equal(const manipulator_msgs__action__ManipulatorTask_SendGoal_Event * lhs, const manipulator_msgs__action__ManipulatorTask_SendGoal_Event * rhs);

/// Copy a action/ManipulatorTask message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_SendGoal_Event__copy(
  const manipulator_msgs__action__ManipulatorTask_SendGoal_Event * input,
  manipulator_msgs__action__ManipulatorTask_SendGoal_Event * output);

/// Retrieve pointer to the hash of the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_type_hash_t *
manipulator_msgs__action__ManipulatorTask_SendGoal_Event__get_type_hash(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeDescription *
manipulator_msgs__action__ManipulatorTask_SendGoal_Event__get_type_description(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the single raw source text that defined this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource *
manipulator_msgs__action__ManipulatorTask_SendGoal_Event__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the recursive raw sources that defined the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource__Sequence *
manipulator_msgs__action__ManipulatorTask_SendGoal_Event__get_type_description_sources(
  const rosidl_message_type_support_t * type_support);

/// Initialize array of action/ManipulatorTask messages.
/**
 * It allocates the memory for the number of elements and calls
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Event__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_SendGoal_Event__Sequence__init(manipulator_msgs__action__ManipulatorTask_SendGoal_Event__Sequence * array, size_t size);

/// Finalize array of action/ManipulatorTask messages.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Event__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_SendGoal_Event__Sequence__fini(manipulator_msgs__action__ManipulatorTask_SendGoal_Event__Sequence * array);

/// Create array of action/ManipulatorTask messages.
/**
 * It allocates the memory for the array and calls
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Event__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
manipulator_msgs__action__ManipulatorTask_SendGoal_Event__Sequence *
manipulator_msgs__action__ManipulatorTask_SendGoal_Event__Sequence__create(size_t size);

/// Destroy array of action/ManipulatorTask messages.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_SendGoal_Event__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_SendGoal_Event__Sequence__destroy(manipulator_msgs__action__ManipulatorTask_SendGoal_Event__Sequence * array);

/// Check for action/ManipulatorTask message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_SendGoal_Event__Sequence__are_equal(const manipulator_msgs__action__ManipulatorTask_SendGoal_Event__Sequence * lhs, const manipulator_msgs__action__ManipulatorTask_SendGoal_Event__Sequence * rhs);

/// Copy an array of action/ManipulatorTask messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_SendGoal_Event__Sequence__copy(
  const manipulator_msgs__action__ManipulatorTask_SendGoal_Event__Sequence * input,
  manipulator_msgs__action__ManipulatorTask_SendGoal_Event__Sequence * output);

/// Retrieve pointer to the hash of the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_type_hash_t *
manipulator_msgs__action__ManipulatorTask_GetResult__get_type_hash(
  const rosidl_service_type_support_t * type_support);

/// Retrieve pointer to the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeDescription *
manipulator_msgs__action__ManipulatorTask_GetResult__get_type_description(
  const rosidl_service_type_support_t * type_support);

/// Retrieve pointer to the single raw source text that defined this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource *
manipulator_msgs__action__ManipulatorTask_GetResult__get_individual_type_description_source(
  const rosidl_service_type_support_t * type_support);

/// Retrieve pointer to the recursive raw sources that defined the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource__Sequence *
manipulator_msgs__action__ManipulatorTask_GetResult__get_type_description_sources(
  const rosidl_service_type_support_t * type_support);

/// Initialize action/ManipulatorTask message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * manipulator_msgs__action__ManipulatorTask_GetResult_Request
 * )) before or use
 * manipulator_msgs__action__ManipulatorTask_GetResult_Request__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_GetResult_Request__init(manipulator_msgs__action__ManipulatorTask_GetResult_Request * msg);

/// Finalize action/ManipulatorTask message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_GetResult_Request__fini(manipulator_msgs__action__ManipulatorTask_GetResult_Request * msg);

/// Create action/ManipulatorTask message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * manipulator_msgs__action__ManipulatorTask_GetResult_Request__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
manipulator_msgs__action__ManipulatorTask_GetResult_Request *
manipulator_msgs__action__ManipulatorTask_GetResult_Request__create(void);

/// Destroy action/ManipulatorTask message.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_GetResult_Request__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_GetResult_Request__destroy(manipulator_msgs__action__ManipulatorTask_GetResult_Request * msg);

/// Check for action/ManipulatorTask message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_GetResult_Request__are_equal(const manipulator_msgs__action__ManipulatorTask_GetResult_Request * lhs, const manipulator_msgs__action__ManipulatorTask_GetResult_Request * rhs);

/// Copy a action/ManipulatorTask message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_GetResult_Request__copy(
  const manipulator_msgs__action__ManipulatorTask_GetResult_Request * input,
  manipulator_msgs__action__ManipulatorTask_GetResult_Request * output);

/// Retrieve pointer to the hash of the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_type_hash_t *
manipulator_msgs__action__ManipulatorTask_GetResult_Request__get_type_hash(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeDescription *
manipulator_msgs__action__ManipulatorTask_GetResult_Request__get_type_description(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the single raw source text that defined this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource *
manipulator_msgs__action__ManipulatorTask_GetResult_Request__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the recursive raw sources that defined the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource__Sequence *
manipulator_msgs__action__ManipulatorTask_GetResult_Request__get_type_description_sources(
  const rosidl_message_type_support_t * type_support);

/// Initialize array of action/ManipulatorTask messages.
/**
 * It allocates the memory for the number of elements and calls
 * manipulator_msgs__action__ManipulatorTask_GetResult_Request__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_GetResult_Request__Sequence__init(manipulator_msgs__action__ManipulatorTask_GetResult_Request__Sequence * array, size_t size);

/// Finalize array of action/ManipulatorTask messages.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_GetResult_Request__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_GetResult_Request__Sequence__fini(manipulator_msgs__action__ManipulatorTask_GetResult_Request__Sequence * array);

/// Create array of action/ManipulatorTask messages.
/**
 * It allocates the memory for the array and calls
 * manipulator_msgs__action__ManipulatorTask_GetResult_Request__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
manipulator_msgs__action__ManipulatorTask_GetResult_Request__Sequence *
manipulator_msgs__action__ManipulatorTask_GetResult_Request__Sequence__create(size_t size);

/// Destroy array of action/ManipulatorTask messages.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_GetResult_Request__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_GetResult_Request__Sequence__destroy(manipulator_msgs__action__ManipulatorTask_GetResult_Request__Sequence * array);

/// Check for action/ManipulatorTask message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_GetResult_Request__Sequence__are_equal(const manipulator_msgs__action__ManipulatorTask_GetResult_Request__Sequence * lhs, const manipulator_msgs__action__ManipulatorTask_GetResult_Request__Sequence * rhs);

/// Copy an array of action/ManipulatorTask messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_GetResult_Request__Sequence__copy(
  const manipulator_msgs__action__ManipulatorTask_GetResult_Request__Sequence * input,
  manipulator_msgs__action__ManipulatorTask_GetResult_Request__Sequence * output);

/// Initialize action/ManipulatorTask message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * manipulator_msgs__action__ManipulatorTask_GetResult_Response
 * )) before or use
 * manipulator_msgs__action__ManipulatorTask_GetResult_Response__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_GetResult_Response__init(manipulator_msgs__action__ManipulatorTask_GetResult_Response * msg);

/// Finalize action/ManipulatorTask message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_GetResult_Response__fini(manipulator_msgs__action__ManipulatorTask_GetResult_Response * msg);

/// Create action/ManipulatorTask message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * manipulator_msgs__action__ManipulatorTask_GetResult_Response__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
manipulator_msgs__action__ManipulatorTask_GetResult_Response *
manipulator_msgs__action__ManipulatorTask_GetResult_Response__create(void);

/// Destroy action/ManipulatorTask message.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_GetResult_Response__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_GetResult_Response__destroy(manipulator_msgs__action__ManipulatorTask_GetResult_Response * msg);

/// Check for action/ManipulatorTask message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_GetResult_Response__are_equal(const manipulator_msgs__action__ManipulatorTask_GetResult_Response * lhs, const manipulator_msgs__action__ManipulatorTask_GetResult_Response * rhs);

/// Copy a action/ManipulatorTask message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_GetResult_Response__copy(
  const manipulator_msgs__action__ManipulatorTask_GetResult_Response * input,
  manipulator_msgs__action__ManipulatorTask_GetResult_Response * output);

/// Retrieve pointer to the hash of the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_type_hash_t *
manipulator_msgs__action__ManipulatorTask_GetResult_Response__get_type_hash(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeDescription *
manipulator_msgs__action__ManipulatorTask_GetResult_Response__get_type_description(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the single raw source text that defined this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource *
manipulator_msgs__action__ManipulatorTask_GetResult_Response__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the recursive raw sources that defined the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource__Sequence *
manipulator_msgs__action__ManipulatorTask_GetResult_Response__get_type_description_sources(
  const rosidl_message_type_support_t * type_support);

/// Initialize array of action/ManipulatorTask messages.
/**
 * It allocates the memory for the number of elements and calls
 * manipulator_msgs__action__ManipulatorTask_GetResult_Response__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_GetResult_Response__Sequence__init(manipulator_msgs__action__ManipulatorTask_GetResult_Response__Sequence * array, size_t size);

/// Finalize array of action/ManipulatorTask messages.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_GetResult_Response__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_GetResult_Response__Sequence__fini(manipulator_msgs__action__ManipulatorTask_GetResult_Response__Sequence * array);

/// Create array of action/ManipulatorTask messages.
/**
 * It allocates the memory for the array and calls
 * manipulator_msgs__action__ManipulatorTask_GetResult_Response__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
manipulator_msgs__action__ManipulatorTask_GetResult_Response__Sequence *
manipulator_msgs__action__ManipulatorTask_GetResult_Response__Sequence__create(size_t size);

/// Destroy array of action/ManipulatorTask messages.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_GetResult_Response__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_GetResult_Response__Sequence__destroy(manipulator_msgs__action__ManipulatorTask_GetResult_Response__Sequence * array);

/// Check for action/ManipulatorTask message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_GetResult_Response__Sequence__are_equal(const manipulator_msgs__action__ManipulatorTask_GetResult_Response__Sequence * lhs, const manipulator_msgs__action__ManipulatorTask_GetResult_Response__Sequence * rhs);

/// Copy an array of action/ManipulatorTask messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_GetResult_Response__Sequence__copy(
  const manipulator_msgs__action__ManipulatorTask_GetResult_Response__Sequence * input,
  manipulator_msgs__action__ManipulatorTask_GetResult_Response__Sequence * output);

/// Initialize action/ManipulatorTask message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * manipulator_msgs__action__ManipulatorTask_GetResult_Event
 * )) before or use
 * manipulator_msgs__action__ManipulatorTask_GetResult_Event__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_GetResult_Event__init(manipulator_msgs__action__ManipulatorTask_GetResult_Event * msg);

/// Finalize action/ManipulatorTask message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_GetResult_Event__fini(manipulator_msgs__action__ManipulatorTask_GetResult_Event * msg);

/// Create action/ManipulatorTask message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * manipulator_msgs__action__ManipulatorTask_GetResult_Event__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
manipulator_msgs__action__ManipulatorTask_GetResult_Event *
manipulator_msgs__action__ManipulatorTask_GetResult_Event__create(void);

/// Destroy action/ManipulatorTask message.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_GetResult_Event__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_GetResult_Event__destroy(manipulator_msgs__action__ManipulatorTask_GetResult_Event * msg);

/// Check for action/ManipulatorTask message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_GetResult_Event__are_equal(const manipulator_msgs__action__ManipulatorTask_GetResult_Event * lhs, const manipulator_msgs__action__ManipulatorTask_GetResult_Event * rhs);

/// Copy a action/ManipulatorTask message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_GetResult_Event__copy(
  const manipulator_msgs__action__ManipulatorTask_GetResult_Event * input,
  manipulator_msgs__action__ManipulatorTask_GetResult_Event * output);

/// Retrieve pointer to the hash of the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_type_hash_t *
manipulator_msgs__action__ManipulatorTask_GetResult_Event__get_type_hash(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeDescription *
manipulator_msgs__action__ManipulatorTask_GetResult_Event__get_type_description(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the single raw source text that defined this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource *
manipulator_msgs__action__ManipulatorTask_GetResult_Event__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the recursive raw sources that defined the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource__Sequence *
manipulator_msgs__action__ManipulatorTask_GetResult_Event__get_type_description_sources(
  const rosidl_message_type_support_t * type_support);

/// Initialize array of action/ManipulatorTask messages.
/**
 * It allocates the memory for the number of elements and calls
 * manipulator_msgs__action__ManipulatorTask_GetResult_Event__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_GetResult_Event__Sequence__init(manipulator_msgs__action__ManipulatorTask_GetResult_Event__Sequence * array, size_t size);

/// Finalize array of action/ManipulatorTask messages.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_GetResult_Event__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_GetResult_Event__Sequence__fini(manipulator_msgs__action__ManipulatorTask_GetResult_Event__Sequence * array);

/// Create array of action/ManipulatorTask messages.
/**
 * It allocates the memory for the array and calls
 * manipulator_msgs__action__ManipulatorTask_GetResult_Event__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
manipulator_msgs__action__ManipulatorTask_GetResult_Event__Sequence *
manipulator_msgs__action__ManipulatorTask_GetResult_Event__Sequence__create(size_t size);

/// Destroy array of action/ManipulatorTask messages.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_GetResult_Event__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_GetResult_Event__Sequence__destroy(manipulator_msgs__action__ManipulatorTask_GetResult_Event__Sequence * array);

/// Check for action/ManipulatorTask message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_GetResult_Event__Sequence__are_equal(const manipulator_msgs__action__ManipulatorTask_GetResult_Event__Sequence * lhs, const manipulator_msgs__action__ManipulatorTask_GetResult_Event__Sequence * rhs);

/// Copy an array of action/ManipulatorTask messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_GetResult_Event__Sequence__copy(
  const manipulator_msgs__action__ManipulatorTask_GetResult_Event__Sequence * input,
  manipulator_msgs__action__ManipulatorTask_GetResult_Event__Sequence * output);

/// Initialize action/ManipulatorTask message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * manipulator_msgs__action__ManipulatorTask_FeedbackMessage
 * )) before or use
 * manipulator_msgs__action__ManipulatorTask_FeedbackMessage__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_FeedbackMessage__init(manipulator_msgs__action__ManipulatorTask_FeedbackMessage * msg);

/// Finalize action/ManipulatorTask message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_FeedbackMessage__fini(manipulator_msgs__action__ManipulatorTask_FeedbackMessage * msg);

/// Create action/ManipulatorTask message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * manipulator_msgs__action__ManipulatorTask_FeedbackMessage__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
manipulator_msgs__action__ManipulatorTask_FeedbackMessage *
manipulator_msgs__action__ManipulatorTask_FeedbackMessage__create(void);

/// Destroy action/ManipulatorTask message.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_FeedbackMessage__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_FeedbackMessage__destroy(manipulator_msgs__action__ManipulatorTask_FeedbackMessage * msg);

/// Check for action/ManipulatorTask message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_FeedbackMessage__are_equal(const manipulator_msgs__action__ManipulatorTask_FeedbackMessage * lhs, const manipulator_msgs__action__ManipulatorTask_FeedbackMessage * rhs);

/// Copy a action/ManipulatorTask message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_FeedbackMessage__copy(
  const manipulator_msgs__action__ManipulatorTask_FeedbackMessage * input,
  manipulator_msgs__action__ManipulatorTask_FeedbackMessage * output);

/// Retrieve pointer to the hash of the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_type_hash_t *
manipulator_msgs__action__ManipulatorTask_FeedbackMessage__get_type_hash(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeDescription *
manipulator_msgs__action__ManipulatorTask_FeedbackMessage__get_type_description(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the single raw source text that defined this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource *
manipulator_msgs__action__ManipulatorTask_FeedbackMessage__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the recursive raw sources that defined the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
const rosidl_runtime_c__type_description__TypeSource__Sequence *
manipulator_msgs__action__ManipulatorTask_FeedbackMessage__get_type_description_sources(
  const rosidl_message_type_support_t * type_support);

/// Initialize array of action/ManipulatorTask messages.
/**
 * It allocates the memory for the number of elements and calls
 * manipulator_msgs__action__ManipulatorTask_FeedbackMessage__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_FeedbackMessage__Sequence__init(manipulator_msgs__action__ManipulatorTask_FeedbackMessage__Sequence * array, size_t size);

/// Finalize array of action/ManipulatorTask messages.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_FeedbackMessage__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_FeedbackMessage__Sequence__fini(manipulator_msgs__action__ManipulatorTask_FeedbackMessage__Sequence * array);

/// Create array of action/ManipulatorTask messages.
/**
 * It allocates the memory for the array and calls
 * manipulator_msgs__action__ManipulatorTask_FeedbackMessage__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
manipulator_msgs__action__ManipulatorTask_FeedbackMessage__Sequence *
manipulator_msgs__action__ManipulatorTask_FeedbackMessage__Sequence__create(size_t size);

/// Destroy array of action/ManipulatorTask messages.
/**
 * It calls
 * manipulator_msgs__action__ManipulatorTask_FeedbackMessage__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
void
manipulator_msgs__action__ManipulatorTask_FeedbackMessage__Sequence__destroy(manipulator_msgs__action__ManipulatorTask_FeedbackMessage__Sequence * array);

/// Check for action/ManipulatorTask message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_FeedbackMessage__Sequence__are_equal(const manipulator_msgs__action__ManipulatorTask_FeedbackMessage__Sequence * lhs, const manipulator_msgs__action__ManipulatorTask_FeedbackMessage__Sequence * rhs);

/// Copy an array of action/ManipulatorTask messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_manipulator_msgs
bool
manipulator_msgs__action__ManipulatorTask_FeedbackMessage__Sequence__copy(
  const manipulator_msgs__action__ManipulatorTask_FeedbackMessage__Sequence * input,
  manipulator_msgs__action__ManipulatorTask_FeedbackMessage__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // MANIPULATOR_MSGS__ACTION__DETAIL__MANIPULATOR_TASK__FUNCTIONS_H_
