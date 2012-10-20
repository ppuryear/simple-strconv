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

#include <climits>
#include <limits>
#include <string>
#include <type_traits>

#define SS_LIKELY(x)   __builtin_expect(!!(x), 1)
#define SS_UNLIKELY(x) __builtin_expect(!!(x), 0)

namespace simple_strconv {

typedef unsigned int uint;

const uint kMinBase = 2;
const uint kMaxBase = 16;

namespace detail {

// Given an alphanumeric \a digit, returns its numeric equivalent. For example,
// '3' becomes 3, 'c' becomes 12, 'E' becomes 14, etc. Errors are signaled via
// return values larger than kMaxBase.
uint GetNumberFromDigit(char digit);

// Given a \a number, returns its lower-case ASCII representation. For example,
// 3 becomes '3', 12 becomes 'c', 14 becomes 'e', etc.
char GetDigitFromNumber(uint number);

// Determines the base, sign, and starting position of \a str.
bool FixupAndClassifyString(const char*& str, uint& base);

} // namespace detail

#define SS_CHECK_INT_PARAM(T) \
    static_assert(std::is_integral<T>::value, \
            "parameter `T' must be an integral type");

// Converts \a str to an integer of type \a T.
// The semantics of this function are identical to those of the C-style
// variants documented in simple_strconv.h.
template<typename T>
int StringToInt(T *result, const char* str, uint base = 0) {
    SS_CHECK_INT_PARAM(T);
    if (base != 0 && (base < kMinBase || base > kMaxBase))
        return -EINVAL;

    bool negative = detail::FixupAndClassifyString(str, base);
    if (negative && std::is_unsigned<T>::value)
        return -ERANGE;

    typedef typename std::make_unsigned<T>::type uT;
    const uT t_max = negative ? -std::numeric_limits<T>::min() :
            std::numeric_limits<T>::max();
    const uT accum_max = t_max / base;
    const uint digit_max = t_max % base;

    uT accum = 0;
    do {
        uint digit = detail::GetNumberFromDigit(*str);
        if (SS_UNLIKELY(digit >= base))
            return -EINVAL;

        // Check for overflow.
        if (SS_UNLIKELY(accum >= accum_max)) {
            // We have overflowed *unless* accum == accum_max &&
            // digit <= digit_max.
            if (accum > accum_max || digit > digit_max) {
                // Check the rest of the string for invalid characters.
                while (*++str)
                    if (SS_UNLIKELY(detail::GetNumberFromDigit(*str) >= base))
                        return -EINVAL;
                return -ERANGE;
            }
        }
        accum = (accum * base) + digit;
    } while (*++str);

    *result = negative ? -accum : accum;
    return 0;
}

// Converts the given integer \a value into an ASCII string in the given
// \a base. The buffer pointed to by \a str must be sufficiently large to
// hold the result (the maximum possible output size is
// CHAR_BIT * sizeof(T) + 2, including the terminating NUL character).
//
// \return The length of the string produced, not including the terminating
//         NUL.
template<typename T>
int IntToCString(char* str, T value, uint base = 10) {
    SS_CHECK_INT_PARAM(T);
    if (base < kMinBase || base > kMaxBase)
        return -EINVAL;

    typedef typename std::make_unsigned<T>::type uT;
    typedef typename std::make_signed<T>::type sT;

    uT uvalue;
    char* start = str;
    // With non-power-of-2 bases, if the input is negative, invert it and add
    // a minus sign.
    // Explicitly cast to signed type to suppress "tautological compare"
    // warnings.
    if ((base & (base - 1)) && std::is_signed<T>::value && (sT) value < 0) {
        *start++ = '-';
        uvalue = -value;
    } else {
        uvalue = value;
    }

    // Construct the string in reverse.
    char* end = start;
    do {
        *end++ = detail::GetDigitFromNumber(uvalue % base);
        uvalue /= base;
    } while (uvalue > 0);

    int length = (int) (end - str);
    *end-- = '\0';

    // Reverse the buffer.
    while (end > start) {
        char temp = *start;
        *start++ = *end;
        *end-- = temp;
    }
    return length;
}

// Converts the given integer \a value into a std::string in the given \a base.
template<typename T>
std::string IntToString(T value, uint base = 10) {
    // Maximum-length output:
    //   Unsigned: max = 8 * sizeof(T) + 1 [NUL]
    //   Signed:   max = 1 [hyphen] + 8 * sizeof(T) + 1 [NUL]
    //                 = 8 * sizeof(T) + 2
    char buffer[CHAR_BIT * sizeof(T) + 2];
    buffer[0] = '\0';
    IntToCString(buffer, value, base);
    return std::string(buffer);
}

} // namespace simple_strconv

#undef SS_CHECK_INT_PARAM
#undef SS_LIKELY
#undef SS_UNLIKELY
