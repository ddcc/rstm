/**
 *  Copyright (C) 2011
 *  University of Rochester Department of Computer Science
 *    and
 *  Lehigh University Department of Computer Science and Engineering
 *
 * License: Modified BSD
 *          Please see the file LICENSE.RSTM for licensing information
 */

/**
 *  This is the configured header for libstm. It records all of the
 *  configuration parameters, and should be included before anything that
 *  depends on a #define that is prefixed with 'STM'
 */

#ifndef RSTM_STM_INCLUDE_CONFIG_H
#define RSTM_STM_INCLUDE_CONFIG_H

// Target processor architecture
#define STM_CPU_X86

// Target compiler
#define STM_CC_GCC

// Target OS
#define STM_OS_LINUX

// The kind of build we're doing
#define STM_O3

// Histogram generation
//#define STM_COUNTCONSEC_YES

// ProfileTMtrigger
//#define STM_PROFILETMTRIGGER_ALL
//#define STM_PROFILETMTRIGGER_PATHOLOGY
#define STM_PROFILETMTRIGGER_NONE

// Configured thread-local-storage model
#define STM_TLS_GCC
//#cmakedefine STM_TLS_PTHREAD

// Configured logging granularity
//#define STM_WS_WORDLOG
//#define STM_WS_BYTELOG
//#define STM_USE_WORD_LOGGING_VALUELIST

// Configured options
//#define STM_PROTECT_STACK
//#define STM_ABORT_ON_THROW

// Defined when we want to optimize for SSE execution
#define STM_USE_SSE

#endif // RSTM_STM_INCLUDE_CONFIG_H
