/**
 * Copyright (C)  2011-2011  Jesse Meng (pingf0@gmail.com).
 *
 * This file is part of OOC-GCC.
 *
 * OOC-GCC is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <setjmp.h>
#include "OOCfg.h"

#ifdef __OOC_USING__MEMORY_FUNCTIONS
	#define __OOC_FUNCTION__RESET_MEMORY_FUNCTIONS __ooc_resetMemoryFunctions
	#define __OOC_FUNCTION__ALLOC __ooc_alloc
	#define __OOC_FUNCTION__FREE __ooc_free
	#define __OOC_FUNCTION__REALLOC __ooc_realloc


	#ifndef __OOC_MEMORY_LOG_FILENAME
		#define __OOC_MEMORY_LOG_FILENAME "./memory.log"
	#endif

	#ifdef __cplusplus
	extern "C" {
	#endif
		int __ooc_memoryLogStatus();
		void __ooc_enableMemoryLog();
		void __ooc_disableMemoryLog();
		void __ooc_resetMemoryFunctions(void * (*real_alloc)(size_t num),\
						void * (*real_realloc)(void * origin,size_t num),\
						void (*real_free)(void *));
		void * __ooc_alloc(size_t __ooc_alloc_num);
		void * __ooc_realloc(void * __ooc_memory_origin,size_t __ooc_alloc_num_new);
		void __ooc_free(void *__ooc_free_memory);
	#ifdef __cplusplus
	};
	#endif
#endif

#ifdef __OOC_USING__TIMER
	#ifdef __cplusplus
	extern "C" {
	#endif
	void __ooc_enableTimer();
	void __ooc_disableTimer();
	int __ooc_timerStatus();
	#ifdef __cplusplus
	};
	#endif
#endif


#ifdef __OOC_USING__ASSERT
	#ifdef __cplusplus
	extern "C" {
	#endif
		void  __ooc_assert(const int mode,const  char *msg1, const  char *msg2, const  char *msg3);
	#ifdef __cplusplus
	};
	#endif
	#define __OOC_FUNCTION__ASSERT(test) \
	((test)?(void)0:__ooc_assert(1,"\n\n[ASSERT] : "#test"\n[FUNC] : ",\
	__FUNCTION__,"  [FILE] : "__FILE__"  [LINE] : "__OOC_FUNCTION__VALUE_TO_STRING(__LINE__)"\n\n"));
	#define __OOC_CHECK(test) \
	((test)?(void)0:__ooc_assert(0,"\n\n[ASSERT] : "#test"\n[FUNC] : ",\
	__FUNCTION__,"  [FILE] : "__FILE__"  [LINE] : "__OOC_FUNCTION__VALUE_TO_STRING(__LINE__)"\n\n"));
#endif


///////////////


#ifdef __OOC_USING__TRACE_LOG
	#ifndef __OOC_VALUE__TRACE_LOG_FILENAME
		#define __OOC_VALUE__TRACE_LOG_FILENAME "./trace.log"
	#endif

	#ifdef __cplusplus
	extern "C" {
	#endif
		void __cyg_profile_func_enter( void *this_fn, void *call_site )
		__attribute__ ((no_instrument_function));
		void __cyg_profile_func_exit( void *this_fn, void *call_site )
		__attribute__ ((no_instrument_function));
		int __ooc_isTraceLogEnable();
		void __ooc_enableTraceLog();
		void __ooc_disableTraceLog();
		void __ooc_traceMark(const  char * mark);
	#ifdef __cplusplus
	};
	#endif
#endif


#ifdef __OOC_USING__MANUAL_LOG
	#ifndef __OOC_VALUE__MANUAL_LOG_FILENAME
		#define __OOC_VALUE__MANUAL_LOG_FILENAME "./manual.log"
	#endif
	#ifdef __cplusplus
	extern "C" {
	#endif
		void __ooc_log(const  char * msg1,
					   const  char * msg2);
	#ifdef __cplusplus
	};
	#endif
	#define __OOC_FUNCTION__LOG(what) \
	__ooc_log(__FUNCTION__,__FILE__","__OOC_FUNCTION__VALUE_TO_STRING(__LINE__)","what);
#endif

#if defined(__OOC_USING__MAIN_CONSTRUCTOR)&&\
    defined(__OOC_USING__MAIN_DESTRUCTOR)
	#ifdef __cplusplus
	extern "C" {
	#endif
		void __ooc_safe_exit(void)
		__attribute__ ((no_instrument_function));
	#ifdef __cplusplus
	};
	#endif
#endif


//////////////////////////////////////////////////////////////////////////////

/////////////////////////////////
#ifdef __OOC_USING__EXCEPTION
	typedef struct _Exception Exception;
	struct _Exception{
		char * reason;
	};
	typedef struct _ExceptionFrame ExceptionFrame;
	struct _ExceptionFrame{
		ExceptionFrame *prev;
		jmp_buf env;
		Exception *exception;
	};
	#ifdef __cplusplus
	extern "C" {
	#endif
		enum EnumExceptionStatus{
			Exception_entered=0,Exception_raised,
			Exception_handled,Exception_finalized
		};
		ExceptionFrame * __ooc_getExceptionFrameStack(void);
		void __ooc_setExceptionFrameStack(ExceptionFrame * frame);
		void __ooc_moveExceptionFrameStackPrev(void);
		void __ooc_setExceptions(Exception (*reg)[],unsigned int num);
		Exception (*__ooc_getExceptions())[];
		void __ooc_setException( int num,Exception * exception);
		Exception * __ooc_getException( int num);
		void __ooc_throw(Exception *e,int flag,const  char *msg1, const  int line, const  char *msg3);
	#ifdef __cplusplus
	};
	#endif

	#define __OOC_FUNCTION__THROW(e) \
	__ooc_throw((e),__ooc_exceptionFlag,__FILE__, __LINE__, __FUNCTION__);
	#define __OOC_FUNCTION__TRY \
	do { \
		volatile int __ooc_exceptionFlag; \
		ExceptionFrame __ooc_exceptionFrame; \
		__ooc_exceptionFrame.prev = __ooc_getExceptionFrameStack(); \
		__ooc_setExceptionFrameStack(&__ooc_exceptionFrame); \
		__ooc_exceptionFlag = setjmp(__ooc_exceptionFrame.env); \
		if (__ooc_exceptionFlag == Exception_entered) {
	///////////////
	#define __OOC_FUNCTION__CATCH(e) \
			if (__ooc_exceptionFlag == Exception_entered) { \
				__ooc_moveExceptionFrameStackPrev(); \
			} \
		} else if (__ooc_exceptionFrame.exception->reason == (e)->reason) { \
			__ooc_exceptionFlag = Exception_handled;
	///////////////
	#define __OOC_FUNCTION__ELSE_CATCH \
			if (__ooc_exceptionFlag == Exception_entered) { \
				__ooc_moveExceptionFrameStackPrev(); \
			} \
		} else { \
			__ooc_exceptionFlag = Exception_handled;
	//////////////
	#define __OOC_FUNCTION__CATCH_ELSE __OOC_ELSE_CATCH
	//////////////
	#define __OOC_FUNCTION__FINALLY \
			if (__ooc_exceptionFlag == Exception_entered) { \
				__ooc_moveExceptionFrameStackPrev(); \
			} \
		} \
		{ \
			if (__ooc_exceptionFlag == Exception_entered) {\
				__ooc_exceptionFlag = Exception_finalized; \
			}
	///////////////
	#define __OOC_FUNCTION__END_TRY \
			if (__ooc_exceptionFlag == Exception_entered) { \
				__ooc_moveExceptionFrameStackPrev(); \
			} \
		} \
		if (__ooc_exceptionFlag == Exception_raised) { \
			__OOC_FUNCTION__THROW(__ooc_exceptionFrame.exception); \
		} \
	} while (0);
	#define __OOC_FUNCTION__SET_EXCEPTIONS __ooc_setExceptions
	#define __OOC_FUNCTION__GET_EXCEPTIONS __ooc_getExceptions
	#define __OOC_FUNCTION__SET_EXCEPTION __ooc_setException
	#define __OOC_FUNCTION__GET_EXCEPTION __ooc_getException
#endif
////////////////////////////////////////

///////////////////////


