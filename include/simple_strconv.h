// Copyright 2012 Philip Puryear
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
