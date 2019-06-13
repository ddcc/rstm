/* =============================================================================
 *
 * tm.h
 *
 * Utility defines for transactional memory
 *
 * =============================================================================
 *
 * Copyright (C) Stanford University, 2006.  All Rights Reserved.
 * Authors: Chi Cao Minh and Martin Trautmann
 *
 * =============================================================================
 *
 * For the license of bayes/sort.h and bayes/sort.c, please see the header
 * of the files.
 *
 * ------------------------------------------------------------------------
 *
 * For the license of kmeans, please see kmeans/LICENSE.kmeans
 *
 * ------------------------------------------------------------------------
 *
 * For the license of ssca2, please see ssca2/COPYRIGHT
 *
 * ------------------------------------------------------------------------
 *
 * For the license of lib/mt19937ar.c and lib/mt19937ar.h, please see the
 * header of the files.
 *
 * ------------------------------------------------------------------------
 *
 * For the license of lib/rbtree.h and lib/rbtree.c, please see
 * lib/LEGALNOTICE.rbtree and lib/LICENSE.rbtree
 *
 * ------------------------------------------------------------------------
 *
 * Unless otherwise noted, the following license applies to STAMP files:
 *
 * Copyright (c) 2007, Stanford University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     * Neither the name of Stanford University nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY STANFORD UNIVERSITY ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL STANFORD UNIVERSITY BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 * =============================================================================
 */


#ifndef TM_H
#define TM_H 1

/* =============================================================================
 * Transactional Memory System Interface
 * =============================================================================
 */

# include <setjmp.h>
# include <string.h>

# include <stm.h>
# include <mod_mem.h>

# include <common/platform.hpp>

#  define TM_GET_ALGNAME()            "tinySTM"

#  define TM_CALLABLE                 /* nothing */
#  define TM_WAIVER                   /* nothing */

#  define TM_ARG_ALONE                struct stm_tx *tx
#  define TM_ARG                      , TM_ARG_ALONE
#  define TM_PARAM                    , tx
#  define TM_PARAM_ALONE              tx

#  define TM_STARTUP(numThread)       STM_STARTUP(numThread)
#  define TM_BEGIN(x)                 STM_START(0, 0)
#  define TM_BEGIN_RO()               STM_START(1, 0)
#  define TM_BEGIN_NOOVR()            STM_START(0, 1)
#  define TM_END                      STM_END()

#  define TM_BEGIN_FAST_INITIALIZATION() TM_BEGIN()
#  define TM_END_FAST_INITIALIZATION()  TM_END

#  define TM_RESTART()                stm_abort(0)
#  define TM_EARLY_RELEASE(var)       /* nothing */

#  define STM_START(ro, o)            { \
                                          stm_tx_attr_t _a = {{0}}; \
                                          _a.read_only = ro; \
                                          _a.no_overwrite = o; \
                                          sigjmp_buf *_e = stm_start(_a); \
                                          struct stm_tx *tx = stm_current_tx(); \
                                          if (_e != NULL) sigsetjmp(*_e, 0); \
                                          { \

#  define STM_END()                       stm_commit(); \
                                          } \
                                      }

#  define TM_SYS_INIT()               if (sizeof(long) != sizeof(void *)) { \
                                        fprintf(stderr, "Error: unsupported long and pointer sizes\n"); \
                                        exit(1); \
                                      } \
                                      if (getenv("STM_STATS") != NULL) \
                                        setenv("TM_STATISTICS", "1", 0); \
                                      stm_init(NULL); \
                                      mod_mem_init();
#  define TM_SYS_SHUTDOWN()           stm_exit()

#  define TM_THREAD_INIT()            stm_init_thread()
#  define TM_THREAD_SHUTDOWN()        stm_exit_thread()

#  define TM_ALLOC(size)              stm_malloc(size)
#  define TM_FREE(ptr)                stm_free(ptr, sizeof(*ptr))

/* =============================================================================
 * Transactional Memory System interface for shared memory accesses
 * =============================================================================
 */

#  include <wrappers.h>

/* We could also map macros to the stm_(load|store)_long functions if needed */

namespace stm {
  inline char stm_read(const char *addr) {
    return stm_load_char(addr);
  }

  inline unsigned char stm_read(const unsigned char *addr) {
    return stm_load_uchar(addr);
  }

  inline int stm_read(const int *addr) {
    return stm_load_int(addr);
  }

  inline unsigned int stm_read(const unsigned int *addr) {
    return stm_load_uint(addr);
  }

  inline long stm_read(const long *addr) {
    return stm_load_long(addr);
  }

  inline unsigned long stm_read(const unsigned long *addr) {
    return stm_load_ulong(addr);
  }

  inline long long stm_read(const long long *addr) {
    return stm_load_llong(addr);
  }

  inline unsigned long long stm_read(const unsigned long long *addr) {
    return stm_load_ullong(addr);
  }

  template <typename T>
  inline T* stm_read(T** addr) {
    return reinterpret_cast<T *>(stm_load_ptr(const_cast<const void **>(reinterpret_cast<void **>(addr))));
  }

  template <typename T>
  inline T* const stm_read(T* const *addr) {
    return const_cast<T* const>(reinterpret_cast<T *>(stm_load_ptr(reinterpret_cast<const void **>(const_cast<const T **>(addr)))));
  }

  template <typename T>
  inline T* stm_read(const T** addr) {
    return stm_load_ptr(addr);
  }

  inline float stm_read(const float *addr) {
    return stm_load_float(addr);
  }

  inline double stm_read(const double *addr) {
    return stm_load_double(addr);
  }

  template<typename T>
  inline typename std::enable_if<std::is_enum<T>::value, T>::type stm_read(const T *enm) {
    return static_cast<T>(stm_read(reinterpret_cast<typename std::underlying_type<T>::type *>(const_cast<T *>(enm))));
  }

  inline void stm_write(char *addr, const char val) {
    stm_store_char(addr, val);
  }

  inline void stm_write(unsigned char *addr, const unsigned char val) {
    stm_store_uchar(addr, val);
  }

  inline void stm_write(int *addr, const int val) {
    stm_store_int(addr, val);
  }

  inline void stm_write(unsigned int *addr, const unsigned int val) {
    stm_store_uint(addr, val);
  }

  inline void stm_write(long *addr, const long val) {
    stm_store_long(addr, val);
  }

  inline void stm_write(unsigned long *addr, const unsigned long val) {
    stm_store_ulong(addr, val);
  }

  inline void stm_write(long long *addr, const long long val) {
    stm_store_llong(addr, val);
  }

  inline void stm_write(unsigned long long *addr, const unsigned long long val) {
    stm_store_ullong(addr, val);
  }

  template <typename T>
  inline void stm_write(T **addr, const T *val) {
    stm_store_ptr(reinterpret_cast<void **>(addr), reinterpret_cast<const void *>(val));
  }

  inline void stm_write(float *addr, const float val) {
    stm_store_float(addr, val);
  }

  inline void stm_write(double *addr, const float val) {
    stm_store_double(addr, val);
  }

  template<typename T>
  inline typename std::enable_if<std::is_enum<T>::value, void>::type stm_write(T *enm, T val) {
    stm_write(reinterpret_cast<typename std::underlying_type<T>::type *>(enm), static_cast<typename std::underlying_type<T>::type>(val));
  }
}

#define TM_READ(var)                        stm::stm_read(&var)
#define TM_WRITE(var, val)                  stm::stm_write(&var, val)

#  ifndef ORIGINAL
#   define TM_LOG_OP_DECLARE(o, f, r, a, m, ...) stm_op_id_t o = { .idx = STM_BAD_IDX }
#   define TM_FINISH_MERGE()                stm_finish_merge()
#   define TM_LOG_FFI_DECLARE               ffi_cif fi
#   define TM_LOG_TYPE_DECLARE_INIT(v, ...) static ffi_type v = __VA_ARGS__
#   define TM_LOG_OP_INIT(o, f, r, a, m, ...) const stm_merge_policy_t o##_policy[2] = { __VA_ARGS__ }; const ffi_status o##_ffi = ffi_prep_cif(&fi, FFI_DEFAULT_ABI, !__builtin_types_compatible_p(typeof(a), typeof(&a[0])) ? sizeof(a) / sizeof(a[0]) : 0, r, a); ASSERT_FAIL(o##_ffi == FFI_OK); o = stm_new_opcode(#o, &fi, FFI_FN(f), m, o##_policy); ASSERT_FAIL(!(stm_get_features() & STM_FEATURE_OPLOG_SIMPLE) || STM_VALID_OPID(o))
#   ifndef NDEBUG
#    define TM_LOG_BEGIN(o, m, ...)         int o##_ret = stm_begin_op(o, m, ##__VA_ARGS__); assert(!( stm_get_features() & STM_FEATURE_OPLOG_SIMPLE) || o##_ret)
#    define TM_LOG_END(o, rv)               o##_ret = stm_end_op(o, rv); assert(!(stm_get_features() & STM_FEATURE_OPLOG_SIMPLE) || o##_ret)
#   else
#    define TM_LOG_BEGIN(o, m, ...)         stm_begin_op(o, m, ##__VA_ARGS__)
#    define TM_LOG_END(o, rv)               stm_end_op(o, rv)
#   endif /* NDEBUG */
#   ifdef PARTIAL_MERGE
#    define TM_SET_MERGE(m, p)              /* nothing */
#    define TM_NEW_RESTORE(id)              stm_new_restore(id)
#    define TM_RESTORE(id)                  stm_restore(id)
#   elif defined (SINGLE_MERGE)
#    define TM_SET_MERGE(m, p)              stm_set_merge(m, p)
#    define TM_NEW_RESTORE(id)              stm_new_restore(id)
#    define TM_RESTORE(id)                  stm_restore(id)
#   else
#    define TM_SET_MERGE(m, p)              /* nothing */
#    define TM_NEW_RESTORE(id)              /* nothing */
#    define TM_RESTORE(id)                  /* nothing */
#   endif

#   define TM_READ_TAG(var, tag)            stm_load_tag((const stm_word_t *)(void *)&(var), (uintptr_t)tag)
#   define TM_READ_TAG_P(var, tag)          stm_load_tag_ptr((const void **)(void *)&(var), (uintptr_t)tag)
#   define TM_READ_TAG_F(var, tag)          stm_load_tag_float((const float *)(void *)&(var), (uintptr_t)tag)
#   define TM_READ_TAG_D(var, tag)          stm_load_tag_double((const double *)(void *)&(var), (uintptr_t)tag)

#   define TM_READ_UPDATE(r, var, val)      stm_load_update(r, (const stm_word_t *)(void *)&(var), (stm_word_t *)&val)
#   define TM_READ_UPDATE_P(r, var, val)    stm_load_update_ptr(r, (const void **)(void *)&(var), (void *)&val)
#   define TM_READ_UPDATE_F(r, var, val)    stm_load_update_float(r, (const float *)(void *)&(var), &val)
#   define TM_READ_UPDATE_D(r, var, val)    stm_load_update_double(r, (const double *)(void *)&(var), &val)

#   define TM_WRITE_UPDATE(w, var, val)     stm_store_update(w, (void *)(void *)&(var), val)
#   define TM_WRITE_UPDATE_P(w, var, val)   stm_store_update_ptr(w, (void **)(void *)&(var), val)
#   define TM_WRITE_UPDATE_F(w, var, val)   stm_store_update_float(w, (float *)(void *)&(var), val)
#   define TM_WRITE_UPDATE_D(w, var, val)   stm_store_update_double(w, (double *)(void *)&(var), val)

#   define TM_READ_VALUE(r, var, val)       stm_load_value(r, (const stm_word_t *)(void *)&(var), (stm_word_t *)&val)
#   define TM_READ_VALUE_P(r, var, val)     stm_load_value_ptr(r, (const void **)(void *)&(var), (void *)&val)
#   define TM_READ_VALUE_F(r, var, val)     stm_load_value_float(r, (const float *)(void *)&(var), &val)
#   define TM_READ_VALUE_D(r, var, val)     stm_load_value_double(r, (const double *)(void *)&(var), &val)

#   define TM_WRITE_VALUE(r, var, val)      stm_store_value(r, (const stm_word_t *)(void *)&(var), (stm_word_t *)&val)
#   define TM_WRITE_VALUE_P(r, var, val)    stm_store_value_ptr(r, (const void **)(void *)&(var), (void *)&val)
#   define TM_WRITE_VALUE_F(r, var, val)    stm_store_value_float(r, (const float *)(void *)&(var), &val)
#   define TM_WRITE_VALUE_D(r, var, val)    stm_store_value_double(r, (const double *)(void *)&(var), &val)

#   define TM_GET_TAG(r)                    (void *)stm_get_load_tag(r)
#   define TM_SET_TAG(r, tag)               stm_set_load_tag(r, (uintptr_t)tag)

#   define TM_DID_READ(var)                 stm_did_load((stm_word_t *)&(var))
#   define TM_DID_WRITE(var)                stm_did_store((stm_word_t *)&(var))
#   define TM_UNDO_READ(e)                  stm_undo_load(e)
#   define TM_UNDO_WRITE(e)                 stm_undo_store(e)

#   define TM_DID_MALLOC(var)               stm_did_malloc((const void *)var)
#   define TM_DID_FREE(var)                 stm_did_free((const void *)var)
#   define TM_UNDO_MALLOC(e)                stm_undo_malloc(e)
#   define TM_UNDO_FREE(e)                  stm_undo_free(e)
#  else
#   define TM_LOG_FFI_DECLARE               /* nothing */
#   define TM_LOG_TYPE_DECLARE_INIT(v, ...) /* nothing */
#   define TM_LOG_OP_DECLARE(o, f, r, a, m, ...) /* nothing */
#   define TM_LOG_OP_INIT(o, f, r, a, m, ...) /* nothing */
#   define TM_LOG_BEGIN(o, m, ...)          /* nothing */
#   define TM_LOG_END(o, rv)                /* nothing */

#   define TM_FINISH_MERGE()                /* nothing */
#   define TM_SET_MERGE(m, p)               /* nothing */
#   define TM_NEW_RESTORE(id)               /* nothing */
#   define TM_RESTORE(id)                   /* nothing */

#   define TM_READ_TAG(var, base)           TM_READ(var)
#   define TM_READ_TAG_P(var, base)         TM_READ_P(var)
#   define TM_READ_TAG_F(var, base)         TM_READ_F(var)
#  endif /* ORIGINAL */

#endif /* TM_H */

/* =============================================================================
 *
 * End of tm.h
 *
 * =============================================================================
 */
