/*  Copyright 2005-2006 Guillaume Duhamel
    Copyright 2005 Theo Berkau

    This file is part of Yabause.

    Yabause is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Yabause is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Yabause; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#ifndef DEBUG_H
#define DEBUG_H

#include "core.h"
#include <stdio.h>

typedef enum { DEBUG_STRING, DEBUG_STREAM , DEBUG_STDOUT, DEBUG_STDERR, DEBUG_CALLBACK } DebugOutType;

typedef struct {
	DebugOutType output_type;
	union {
		FILE * stream;
		char * string;
	        void  (*callback) (char*);
	} output;
	char * name;
} Debug;

Debug * DebugInit(const char *, DebugOutType, char *);
void DebugDeInit(Debug *);

void DebugChangeOutput(Debug *, DebugOutType, char *);

void DebugPrintf(Debug *, const char *, u32, const char *, ...);

extern Debug * MainLog;

void LogStart(void);
void LogStop(void);
void LogChangeOutput(DebugOutType t, char * s);

#ifdef DEBUG
#	if defined(_WIN32) && (_MSC_VER<=1200)
#		define LOG DebugPrintfLOG
#	else
#		define LOG(...) DebugPrintf(MainLog, __FILE__, __LINE__, __VA_ARGS__)
#	endif
#else
#	define LOG
#endif

#ifdef CDDEBUG
#	if defined(_WIN32) && (_MSC_VER<=1200)
#		define CDLOG DebugPrintfCDLOG
#	else
#		define CDLOG(...) DebugPrintf(MainLog, __FILE__, __LINE__, __VA_ARGS__)
#	endif
#else
#	define CDLOG
#endif

#ifdef NETLINK_DEBUG
#	if defined(_WIN32) && (_MSC_VER<=1200)
#		define NETLINK_LOG DebugPrintfNETLINK
#	else
#		define NETLINK_LOG(...) DebugPrintf(MainLog, __FILE__, __LINE__, __VA_ARGS__)
#	endif
#else
#	define NETLINK_LOG()
#endif

#ifdef SCSP_DEBUG
#	if defined(_WIN32) && (_MSC_VER<=1200)
#		define SCSPLOG DebugPrintfSCSP
#	else
#		define SCSPLOG(...) DebugPrintf(MainLog, __FILE__, __LINE__, __VA_ARGS__)
#	endif
#else
#	define SCSPLOG
#endif

#ifdef VDP1_DEBUG
#	if defined(_WIN32) && (_MSC_VER<=1200)
#		define VDP1LOG DebugPrintfVDP1
#	else
#		define VDP1LOG(...) DebugPrintf(MainLog, __FILE__, __LINE__, __VA_ARGS__)
#	endif
#else
#	define VDP1LOG
#endif

#ifdef VDP2_DEBUG
#	if defined(_WIN32) && (_MSC_VER<=1200)
#		define VDP2LOG DebugPrintfVDP2
#	else
#		define VDP2LOG(...) DebugPrintf(MainLog, __FILE__, __LINE__, __VA_ARGS__)
#	endif
#else
#	define VDP2LOG
#endif

#ifdef SMPC_DEBUG
#	if defined(_WIN32) && (_MSC_VER<=1200)
#		define SMPCLOG DebugPrintfSMPC
#	else
#		define SMPCLOG(...) DebugPrintf(MainLog, __FILE__, __LINE__, __VA_ARGS__)
#	endif
#else
#	define SMPCLOG
#endif

#endif
