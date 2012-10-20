/* Copyright 2012 Philip Puryear
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * simple_strconv.h - public interface to the simple-strconv library
 *
 * This library has two interfaces: a C-style function interface and a
 * C++-style template interface. In C programs, only the C interface is
 * available (obviously). In C++ programs, either interface can be used.
 *
 * C Interface:
 *   The C interface consists of functions with the following signature:
 *     int simple_strtoX(T *result, const char *str, unsigned int base);
 *   where T is the result type (e.g. 'int') and X is a short name for that
 *   type (e.g. 'i'). See the SS_DECLARE_STRTOINT_FUNC() macros at the bottom
 *   of this file to learn which types are available.
 *
 *   The input string may be any alphanumeric ASCII string, possibly with a
 *   leading prefix to indicate the radix. Leading whitespace and interstitial
 *   punctuation are prohibited. Valid examples (with the base in parentheses):
 *     "4096" (10), "0x1337face" (16), "101010" (2), "0755" (8), "DEADBEEF" (16)
 *   Invalid examples:
 *     "  3" (10), "1,000,000" (10), "0xlivebeef" (16), "42u" (10)
 *
 *   The base may be any number in the range [2, 16]. Characters are
 *   interpreted the standard way, e.g. '3' => 3, 'a' => 10, 'C' => 12 , etc.
 *   The base may also be 0, in which case the function will try to determine
 *   the radix based on a prefix, viz.:
 *     "0b" or "0B"  => base 2
 *     "0"           => base 8
 *     "0x" or "0X"  => base 16
 *     anything else => base 10
 *
 *   The return value is a standard error code. 0 indicates a successful
 *   conversion, and anything less than zero indicates a failure. The following
 *   error codes are used:
 *     -EINVAL: The input string contains characters that cannot be converted,
 *              or the supplied base is not supported.
 *     -ERANGE: The input string is valid but represents a number too large or
 *              small to fit in the requested type.
 *
 * C++ Interface:
 *   The C++ interface is documented in the file __impl.h.
 */

#ifndef SIMPLE_STRCONV_H_
#define SIMPLE_STRCONV_H_

#include <errno.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
#include "simple_strconv/__impl.h"
extern "C" {
#endif

#define SS_DECLARE_STRTOINT_FUNC(SHORT_TYPE, TYPE) \
    int simple_strto##SHORT_TYPE(TYPE* result, const char* str, unsigned base);

SS_DECLARE_STRTOINT_FUNC(s8, int8_t);
SS_DECLARE_STRTOINT_FUNC(u8, uint8_t);
SS_DECLARE_STRTOINT_FUNC(s16, int16_t);
SS_DECLARE_STRTOINT_FUNC(u16, uint16_t);
SS_DECLARE_STRTOINT_FUNC(s32, int32_t);
SS_DECLARE_STRTOINT_FUNC(u32, uint32_t);
SS_DECLARE_STRTOINT_FUNC(s64, int64_t);
SS_DECLARE_STRTOINT_FUNC(u64, uint64_t);

SS_DECLARE_STRTOINT_FUNC(c, char);
SS_DECLARE_STRTOINT_FUNC(uc, unsigned char);
SS_DECLARE_STRTOINT_FUNC(sc, signed char);
SS_DECLARE_STRTOINT_FUNC(s, short);
SS_DECLARE_STRTOINT_FUNC(us, unsigned short);
SS_DECLARE_STRTOINT_FUNC(i, int);
SS_DECLARE_STRTOINT_FUNC(ui, unsigned int);
SS_DECLARE_STRTOINT_FUNC(l, long);
SS_DECLARE_STRTOINT_FUNC(ul, unsigned long);
SS_DECLARE_STRTOINT_FUNC(ll, long long);
SS_DECLARE_STRTOINT_FUNC(ull, unsigned long long);

SS_DECLARE_STRTOINT_FUNC(z, size_t);
SS_DECLARE_STRTOINT_FUNC(pd, ptrdiff_t);

#undef SS_DECLARE_STRTOINT_FUNC

#ifdef __cplusplus
}
#endif

#endif /* SIMPLE_STRCONV_H_ */
