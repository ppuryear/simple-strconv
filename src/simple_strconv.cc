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

#include "simple_strconv.h"

namespace {

// Converts \a c to its lower-case equivalent via ASCII bit magic. Not
// guaranteed to leave non-alphanumeric inputs untouched.
inline char ToLower(char c) {
    return c | ((c & (1 << 6)) >> 1);
}

}

namespace simple_strconv {

namespace detail {

uint GetNumberFromDigit(char digit) {
    if (digit <= '9')
        return digit - '0';
    // XXX: Explain why this works.
    return ToLower(digit - 1) - ('a' - 1) + 10;
}

char GetDigitFromNumber(uint number) {
    if (number < 10)
        return '0' + number;
    return 'a' - 10 + number;
}

bool FixupAndClassifyString(const char*& str, uint& base) {
    bool negative = (str[0] == '-');
    if (negative || str[0] == '+')
        str++;

    // Detect the radix via the numeral prefix (e.g. "0x").
    if (base == 0) {
        if (str[0] == '0') {
            char indicator = ToLower(str[1]);
            if (indicator == 'b') {
                base = 2;
            } else if (indicator == 'x')
                base = 16;
            else
                base = 8;
        } else {
            base = 10;
        }
    }

    // Advance past the numeral prefix if we have one.
    if (str[0] == '0') {
        char indicator = ToLower(str[1]);
        if ((base == 2 && indicator == 'b') ||
                (base == 16 && indicator == 'x'))
            str += 2;
    }

    return negative;
}

} // namespace detail

} // namespace simple_strconv

#define DEFINE_STRTOINT_FUNC(SHORT_TYPE, TYPE) \
    int simple_strto##SHORT_TYPE(TYPE* result, const char* str, \
                                 unsigned base) { \
        return simple_strconv::StringToInt(result, str, base); \
    }

DEFINE_STRTOINT_FUNC(s8, int8_t);
DEFINE_STRTOINT_FUNC(u8, uint8_t);
DEFINE_STRTOINT_FUNC(s16, int16_t);
DEFINE_STRTOINT_FUNC(u16, uint16_t);
DEFINE_STRTOINT_FUNC(s32, int32_t);
DEFINE_STRTOINT_FUNC(u32, uint32_t);
DEFINE_STRTOINT_FUNC(s64, int64_t);
DEFINE_STRTOINT_FUNC(u64, uint64_t);

DEFINE_STRTOINT_FUNC(c, char);
DEFINE_STRTOINT_FUNC(uc, unsigned char);
DEFINE_STRTOINT_FUNC(sc, signed char);
DEFINE_STRTOINT_FUNC(s, short);
DEFINE_STRTOINT_FUNC(us, unsigned short);
DEFINE_STRTOINT_FUNC(i, int);
DEFINE_STRTOINT_FUNC(ui, unsigned int);
DEFINE_STRTOINT_FUNC(l, long);
DEFINE_STRTOINT_FUNC(ul, unsigned long);
DEFINE_STRTOINT_FUNC(ll, long long);
DEFINE_STRTOINT_FUNC(ull, unsigned long long);

DEFINE_STRTOINT_FUNC(z, size_t);
DEFINE_STRTOINT_FUNC(pd, ptrdiff_t);
