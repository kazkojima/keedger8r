/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (C) 2019 National Institute of Advanced Industrial Science
 *                           and Technology (AIST)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* Keystone version of edger macros for generated wrappers. */

#ifndef EDGER_MACROS_H
#define EDGER_MACROS_H

#include <stddef.h>
#include "edge_call.h"

#define EDGE_ENUM_MAX 255
typedef int edge_result_t;
typedef void (*edge_ocall_func_t)(void*);

// Error numbers
#define EDGE_OK			CALL_STATUS_OK
#define EDGE_FAILURE		CALL_STATUS_ERROR
#define EDGE_BAD_OFFSET		CALL_STATUS_BAD_OFFSET
#define EDGE_BAD_PTR		CALL_STATUS_BAD_PTR
#define EDGE_BAD_CALL_ID	CALL_STATUS_BAD_CALL_ID
// Temporal error number
#define EDGE_OUT_OF_MEMORY	CALL_STATUS_ERROR

// c/c++ switch
#if defined(__cplusplus)
#define EDGE_EXTERNC extern "C"
#define EDGE_EXTERNC_BEGIN \
    extern "C"           \
    {
#define EDGE_EXTERNC_END }
#else
#define EDGE_EXTERNC
#define EDGE_EXTERNC_BEGIN
#define EDGE_EXTERNC_END
#endif

#define EDGE_BUFFER_ALIGN (2*sizeof(void*))

static inline edge_result_t
edge_add_size(size_t* total, size_t size)
{
  size_t align = EDGE_BUFFER_ALIGN;
  size_t sum = 0;
  size_t round = ((size + align - 1) / align) * align;
  if (round < size)
    return EDGE_BAD_OFFSET;
  sum = *total + round;
  if (sum < *total)
    return EDGE_BAD_OFFSET;
  *total = sum;
  return EDGE_OK;
}

#define EDGE_ADD_SIZE(total, size) \
  do { \
       if (edge_add_size(&total, size) != EDGE_OK) { \
         _result = EDGE_BAD_OFFSET; \
         goto done; \
       } \
   } while (0)

#define EDGE_SET_IN_POINTER(argname, argsize, argtype) \
  do { \
    if (pargs_in->argname) { \
      pargs_in->argname = (argtype)(input_buffer + input_buffer_offset); \
      EDGE_ADD_SIZE(input_buffer_offset, (size_t)(argsize)); \
      if (input_buffer_offset > input_buffer_size) { \
        _result = EDGE_BAD_OFFSET; \
        goto done; \
      } \
    } \
  } while (0)

#define EDGE_SET_IN_OUT_POINTER EDGE_SET_IN_POINTER

#define EDGE_SET_OUT_POINTER(argname, argsize, argtype) \
  do { \
    if (pargs_in->argname) { \
      pargs_in->argname = (argtype)(output_buffer + output_buffer_offset); \
      EDGE_ADD_SIZE(output_buffer_offset, (size_t)(argsize)); \
      if (edge_call_check_ptr_valid((uintptr_t)output_buffer, \
				    output_buffer_offset)) {  \
        _result = EDGE_BAD_OFFSET; \
        goto done; \
      } \
    } \
  } while (0)

#define EDGE_COPY_AND_SET_IN_OUT_POINTER(argname, argsize, argtype) \
  do { \
    if (pargs_in->argname) { \
      argtype _p_in = (argtype)pargs_in->argname; \
      pargs_in->argname = (argtype)(output_buffer + output_buffer_offset); \
      EDGE_ADD_SIZE(output_buffer_offset, (size_t)argsize); \
      if (edge_call_check_ptr_valid((uintptr_t)output_buffer, \
				    output_buffer_offset)) {  \
        _result = EGGE_BAD_OFFSET; \
        goto done; \
      } \
      memcpy(pargs_in->argname, _p_in, (size_t)(argsize)); \
    } \
  } while (0)

#define EDGE_WRITE_IN_PARAM(argname, argsize, argtype) \
  do { \
    if (argname) { \
      _args.argname = (argtype)(_input_buffer + _input_buffer_offset); \
      EDGE_ADD_SIZE(_input_buffer_offset, (size_t)(argsize)); \
      memcpy((void*)_args.argname, argname, (size_t)(argsize)); \
    } \
  } while (0)

#define EDGE_WRITE_IN_OUT_PARAM EDGE_WRITE_IN_PARAM

#define EDGE_READ_OUT_PARAM(argname, argsize) \
  do { \
    if (argname) { \
      memcpy((void*)argname, \
             _output_buffer + _output_buffer_offset, \
             (size_t)(argsize)); \
      EDGE_ADD_SIZE(_output_buffer_offset, (size_t)(argsize)); \
    } \
  } while (0)

#define EDGE_CHECK_NULL_TERMINATOR(str, size) \
  do { \
    const char* _str = (const char*)(str); \
    size_t _size = (size_t)(size); \
    if (_str && (_size == 0 || _str[_size - 1] != '\0')) { \
      _result = EDGE_BAD_PTR; \
      goto done; \
    } \
  } while (0)

#define EDGE_CHECK_NULL_TERMINATOR_WIDE(str, size) \
  do { \
    const wchar_t* _str = (const wchar_t*)(str); \
    size_t _size = (size_t)(size); \
    if (_str && (_size == 0 || _str[_size - 1] != L'\0')) { \
      _result = EDGE_BAD_PTR; \
      goto done; \
    } \
  } while (0)

#if defined(EDGE_IGNORE_EGDE_RETURN)
# define EDGE_RETURN_ARGP
# define EDGE_SET_EDGE_RESULT(result)
#else
# define EDGE_RETURN_ARGP edge_return_t *_edge_return,

# define EDGE_SET_EDGE_RESULT(result) \
  do { \
    *_edge_return = (result); \
  } while (0)
#endif

#endif // EDGER_MACROS_H
