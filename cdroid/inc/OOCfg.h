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

//#define __OOC_USING__ANONYMOUS_EXTENSIONS

#ifndef _MSC_VER
	//#define __OOC_USING__DEBUG 0xffff
	#ifdef __OOC_USING__DEBUG
		#define __OOC_USING__MAIN_CONSTRUCTOR
		#define __OOC_USING__MAIN_DESTRUCTOR
	#endif

	#if (__OOC_USING__DEBUG)&(1<<0)
		#define __OOC_USING__TIMER
	#endif

	#if (__OOC_USING__DEBUG)&(1<<1)
		#define __OOC_USING__MEMORY_FUNCTIONS
		#define __OOC_USING__MEMORY_LOG

	#endif

	#if (__OOC_USING__DEBUG)&(1<<2)
		#define __OOC_USING__TRACE_LOG
	#endif

	#if (__OOC_USING__DEBUG&(1<<3))
		#define __OOC_USING__MANUAL_LOG
	#endif

	#if (__OOC_USING__DEBUG&(1<<4))
		#define __OOC_USING__CLASS_FUNCTION_PRINTER
	#endif

	#if (__OOC_USING__DEBUG&(1<<5))
		#define __OOC_USING__ASSERT
	#endif

	#if (__OOC_USING__DEBUG&(1<<6))
		#define __OOC_USING__EXCEPTION
	#endif
#endif


#define __OOC_VALUE__MEMORY_LOG_FILENAME "./memory.log"
#define __OOC_VALUE__TRACE_LOG_FILENAME "./trace.log"
#define __OOC_VALUE__MANUAL_LOG_FILENAME "./manual.log"
////////////////////////////////////////////////


