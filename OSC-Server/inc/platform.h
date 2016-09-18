/*
 * Platform.h
 *
 *  Created on: Jul 29, 2013
 *      Author: branton
 */

#ifndef PLATFORM_H_
#define PLATFORM_H_

#if defined (__linux__)
 #ifndef OS_IS_LINUX
	#define OS_IS_LINUX 1
 #endif
#elif (defined(__APPLE__) && defined(__MACH__))
 #ifndef OS_IS_MACOSX
	#define OS_IS_MACOSX  1
 #endif
#elif defined (__unix__)
 #ifndef OS_IS_UNIX
	#define OS_IS_UNIX 1
 #endif
#elif defined (__CYGWIN__)
 #ifndef OS_IS_CYGWIN
	#define OS_IS_CYGWIN 1
 #endif
#elif (defined (__WIN32__) || defined(_WIN32) )
 #ifndef OS_IS_WIN32
	#define OS_IS_WIN32 1
 #endif
#elif defined (__WIN64__)
 #ifndef OS_IS_WIN64
	#define OS_IS_WIN64 1
 #endif
#else
	#define OS_IS_UNKNOWN 1
#endif

#endif /* PLATFORM_H_ */
