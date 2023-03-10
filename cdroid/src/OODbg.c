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

#include "../inc/OODbg.h"
#ifdef __OOC_USING__ASSERT
	void  __ooc_assert(const int mode,const  char *msg1, const  char *msg2, const  char *msg3){
		fputs(msg1,stderr);
		fputs(msg2,stderr);
		fputs(msg3,stderr);
		#if defined(__OOC_USING__MAIN_CONSTRUCTOR)&&\
			defined(__OOC_USING__MAIN_DESTRUCTOR)
			__ooc_safe_exit();
		#endif
		if(1==mode){
			abort();
		}
	}
#endif
////
#ifdef __OOC_USING__EXCEPTION
	static ExceptionFrame * __ooc_exceptionFrameStack = NULL;
	static Exception (*__ooc_exceptions)[];
	static unsigned int __ooc_exceptionTotal=32;
	void __ooc_setExceptions(Exception (*reg)[],unsigned  num){
		__ooc_exceptionTotal=num;
		__ooc_exceptions = reg;
		return ;
	}
	Exception (*__ooc_getExceptions())[]{
		return __ooc_exceptions;
	}
	void __ooc_setException( int num,Exception * exception){
		memcpy(&((*__ooc_exceptions)[num]),exception,sizeof(Exception));
	}
	Exception * __ooc_getException(int num){
		return &((*__ooc_exceptions)[num]);
	}
	ExceptionFrame * __ooc_getExceptionFrameStack(void){
		return __ooc_exceptionFrameStack;
	}
	void __ooc_setExceptionFrameStack(ExceptionFrame * frame){
		__ooc_exceptionFrameStack=frame;
		return ;
	}
	void __ooc_moveExceptionFrameStackPrev(void){
		if(NULL!=__ooc_exceptionFrameStack) {
			__ooc_exceptionFrameStack=__ooc_exceptionFrameStack->prev;
		}
		return ;
	}
	void __ooc_throw(Exception *e,int flag,const  char *msg1, const  int line, const  char *msg3){
		ExceptionFrame *p = __ooc_getExceptionFrameStack();
		if (NULL == p) {
			if (NULL!=e->reason){
				fprintf(stderr, "%s\n", e->reason);
			}
			fprintf(stderr, "catched in %s:%d at %s\n", msg1, line, msg3 );
			fprintf(stderr, "aborting...\n");
			fflush(stderr);
			#if defined(__OOC_USING__MAIN_CONSTRUCTOR)&&\
				defined(__OOC_USING__MAIN_DESTRUCTOR)
				__ooc_safe_exit();
			#endif
			abort();
		}
		if(flag==Exception_entered) {
		   fprintf(stderr, "throw from %s:%d at %s\n", msg1, line, msg3 );
		}else if(flag==Exception_raised) {
		   fprintf(stderr, "throw out from %s:%d at %s\n", msg1, line, msg3 );
		}else{
			#if defined(__OOC_USING__MAIN_CONSTRUCTOR)&&\
				defined(__OOC_USING__MAIN_DESTRUCTOR)
				__ooc_safe_exit();
			#endif
			abort();
		}
		p->exception = (e);
		__ooc_moveExceptionFrameStackPrev();
		longjmp(p->env, Exception_raised);
	}
#endif


#ifdef __OOC_USING__TIMER
	static clock_t __ooc_startTime;
	static clock_t __ooc_stopTime;
	static unsigned int __ooc_flag_timer=1;
	void __ooc_enableTimer(){
		__ooc_flag_timer=1;
	}
	void __ooc_disableTimer(){
		__ooc_flag_timer=0;
	}
	int __ooc_timerStatus(){
		return __ooc_flag_timer;
	}
#endif


#ifdef __OOC_USING__MEMORY_FUNCTIONS
	static void * (*__ooc_real_alloc)(size_t num);
	static void * (*__ooc_real_realloc)(void * origin,size_t num);
	static void (*__ooc_real_free)(void *);

	#ifdef __OOC_USING__MEMORY_LOG
		static FILE * __ooc_memoryLog=NULL;
		static int __ooc_flag_memoryLog=1;
		static int __ooc_memoryTimes=0;
		void __ooc_enableMemoryLog(){
			__ooc_flag_memoryLog=1;
		}
		void __ooc_disableMemoryLog(){
			__ooc_flag_memoryLog=0;
		}
		int __ooc_memoryLogStatus(){
			return __ooc_flag_memoryLog;
		}
	#endif

	void __ooc_resetMemoryFunctions(void * (*real_alloc)(size_t num),\
			void * (*real_realloc)(void * origin,size_t num),\
			void (*real_free)(void *)){
		__ooc_real_alloc=real_alloc;
		__ooc_real_realloc=real_realloc;
		__ooc_real_free=real_free;
	}
	void * __ooc_alloc(size_t __ooc_alloc_num){
		void * __ooc_memory=NULL;
		if(NULL!=__ooc_real_realloc){
			__ooc_memory=(void *)__ooc_real_alloc(__ooc_alloc_num);
		}else{
			__ooc_memory=(void *)malloc(__ooc_alloc_num);
		}
		if(NULL!=__ooc_memory){
			memset(__ooc_memory,'\0',__ooc_alloc_num);
		}
		#ifdef __OOC_USING__MEMORY_LOG
			if(1==__ooc_flag_memoryLog&&NULL!=__ooc_memory) {
				#ifdef _WIN32
					fprintf(__ooc_memoryLog,"+,0x%p,%d;\n",__ooc_memory,__ooc_alloc_num);
					//printf("+,0x%p,%d;\n",__ooc_memory,__ooc_alloc_num);
				#else
					fprintf(__ooc_memoryLog,"+,%p,%d;\n",__ooc_memory,__ooc_alloc_num);
				#endif
				++__ooc_memoryTimes;
			}
		#endif
		return __ooc_memory;
	}
	void * __ooc_realloc(void * __ooc_memory_origin,size_t __ooc_num_new){
		void * __ooc_memory=NULL;
		if(NULL!=__ooc_real_realloc){
			__ooc_memory=__ooc_real_realloc(__ooc_memory_origin,__ooc_num_new);
		}else{
			__ooc_memory=realloc(__ooc_memory_origin,__ooc_num_new);
		}
		#ifdef __OOC_USING__MEMORY_LOG
			if(1==__ooc_flag_memoryLog&&NULL!=__ooc_memory) {
					#ifdef _WIN32
						fprintf(__ooc_memoryLog,"-,0x%p;\n",__ooc_memory_origin);
						//printf("-,0x%p;R\n",__ooc_memory_origin);
						fprintf(__ooc_memoryLog,"+,0x%p,%d;\n",__ooc_memory,__ooc_num_new);
						//printf("+,0x%p,%d;R\n",__ooc_memory,__ooc_num_new);
					#else
						fprintf(__ooc_memoryLog,"-,%p;\n",__ooc_memory_origin);
						fprintf(__ooc_memoryLog,"+,%p,%d;\n",__ooc_memory,__ooc_num_new);
					#endif
			}
		#endif
		return __ooc_memory;
	}
	void __ooc_free(void *__ooc_free_memory){
		#ifdef __OOC_USING__MEMORY_LOG
			if(1==__ooc_flag_memoryLog) {
				#ifdef _WIN32
					fprintf(__ooc_memoryLog,"-,0x%p;\n",__ooc_free_memory);
					//printf("-,0x%p;\n",__ooc_free_memory);
				#else
					fprintf(__ooc_memoryLog,"-,%p;\n",__ooc_free_memory);
				#endif
				--__ooc_memoryTimes;
			}
		#endif
		if(NULL!=__ooc_real_free){
			__ooc_real_free(__ooc_free_memory);
		}else{
			free(__ooc_free_memory);
		}
	}
#endif



#ifdef __OOC_USING__TRACE_LOG
	static FILE * __ooc_traceLog=NULL;
	static int __ooc_flag_traceLog=1;
	static int __ooc_traceLevel=0;
	void __ooc_enableTraceLog(){
		__ooc_flag_traceLog=1;
	}
	void __ooc_disableTraceLog(){
		__ooc_flag_traceLog=0;
	}
	int __ooc_traceLogStatus(){
		return __ooc_flag_traceLog;
	}
	void __ooc_traceMark(const  char * mark){
		static  int flag=0;
		if(1==__ooc_flag_traceLog){
			if(NULL!=mark){
				fprintf(__ooc_traceLog,"0,%d,%s;\n",flag++,mark);
			}else{
				fprintf(__ooc_traceLog,"0,%d;\n",flag++);
			}
		}
		return ;
	}
	void __cyg_profile_func_enter( void *this_fn, void *call_site ){
		if(1==__ooc_flag_traceLog) {
			#ifdef _WIN32
				fprintf(__ooc_traceLog, "+,%d,0x%p,0x%p;\n",
						++__ooc_traceLevel,call_site,this_fn);
			#else
				fprintf(__ooc_traceLog, "+,%d,%p,%p;\n",
						++__ooc_traceLevel,call_site,this_fn);
			#endif
		}
		return ;
	}
	void __cyg_profile_func_exit( void *this_fn, void *call_site ){
		if(1==__ooc_flag_traceLog) {
			#ifdef _WIN32
				fprintf(__ooc_traceLog, "-,%d,0x%p,0x%p;\n",
						__ooc_traceLevel--,call_site,this_fn);
			#else
				fprintf(__ooc_traceLog, "-,%d,%p,%p;\n",
						__ooc_traceLevel--,call_site,this_fn);
			#endif
		}
		return ;
	}
#endif

#ifdef __OOC_USING__MANUAL_LOG
    static FILE * __ooc_manualLog=NULL;
    void __ooc_log(const char *msg1,const char *msg2){
		#ifdef __OOC_USING__TIMER
			if(1==__ooc_flag_timer){
				__ooc_stopTime=clock();
				printf("%fs,%s,%s;\n", (double)(__ooc_stopTime - __ooc_startTime) / CLOCKS_PER_SEC,msg1,msg2);
				return ;
			}
		#endif
		fprintf(__ooc_manualLog, "%s,%s;\n",msg1,msg2);
    }
#endif

#if defined(__OOC_USING__MAIN_CONSTRUCTOR)&&\
	defined(__OOC_USING__MAIN_DESTRUCTOR)
	void __attribute__ ((no_instrument_function))
	__ooc_safe_exit(void){
		#ifdef __OOC_USING__TIMER
			if(1==__ooc_flag_timer){
				__ooc_stopTime=clock();
				printf("\n\nTime Elapsed : %fs\n",
						(double)(__ooc_stopTime - __ooc_startTime) / CLOCKS_PER_SEC);
			}
		#endif

		#if defined(__OOC_USING__MAIN_CONSTRUCTOR)&&\
			defined(__OOC_USING__MAIN_DESTRUCTOR)&&\
			defined(__OOC_USING__MEMORY_LOG)
			fprintf(__ooc_memoryLog,"!,%d;\n",__ooc_memoryTimes);
			fclose(__ooc_memoryLog);
		#endif
		#ifdef __OOC_USING__TRACE_LOG
			fclose(__ooc_traceLog);
		#endif
		#ifdef __OOC_USING__MANUAL_LOG
			fclose(__ooc_manualLog);
		#endif
	}
	void __attribute__ ((no_instrument_function, constructor))
	__ooc_main_constructor( void ){
		setvbuf(stdout, NULL, _IONBF, 0);
		setvbuf(stderr, NULL, _IONBF, 0);
		#if defined(__OOC_USING__MAIN_CONSTRUCTOR)&&\
			defined(__OOC_USING__MAIN_DESTRUCTOR) &&\
			defined(__OOC_USING__MEMORY_LOG)
			__ooc_memoryLog = fopen(__OOC_VALUE__MEMORY_LOG_FILENAME, "w" );
			if(NULL==__ooc_memoryLog){
				abort();
			}
			#ifndef _WIN32
				setlinebuf(__ooc_memoryLog);
			#endif
			fprintf(__ooc_memoryLog,"[,][;]\n");
		#endif
		#ifdef __OOC_USING__TRACE_LOG
			__ooc_traceLog = fopen(__OOC_VALUE__TRACE_LOG_FILENAME, "w" );
			if(NULL==__ooc_traceLog){
				abort();
			}
			#ifndef _WIN32
				setlinebuf(__ooc_traceLog);
			#endif
			fprintf(__ooc_traceLog,"[,][;]\n");
		#endif
		#ifdef __OOC_USING__MANUAL_LOG
			__ooc_manualLog = fopen(__OOC_VALUE__MANUAL_LOG_FILENAME, "w" );
			if(NULL==__ooc_manualLog){
				abort();
			}
			#ifndef _WIN32
				setlinebuf(__ooc_manualLog);
			#endif
			fprintf(__ooc_manualLog,"[,][;]\n");
		#endif
		#ifdef __OOC_USING__TIMER
			__ooc_startTime=clock();
		#endif
	}
	void __attribute__ ((no_instrument_function, destructor))
	__ooc_main_destructor( void ) {
		__ooc_safe_exit();
	}
#endif




