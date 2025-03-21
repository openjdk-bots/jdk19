/*
 * Copyright (c) 2017, 2022, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 *
 */

#ifndef SHARE_UTILITIES_COMPILERWARNINGS_GCC_HPP
#define SHARE_UTILITIES_COMPILERWARNINGS_GCC_HPP

// Macros related to control of compiler warnings.

#ifndef ATTRIBUTE_PRINTF
#define ATTRIBUTE_PRINTF(fmt,vargs)  __attribute__((format(printf, fmt, vargs)))
#endif
#ifndef ATTRIBUTE_SCANF
#define ATTRIBUTE_SCANF(fmt,vargs)  __attribute__((format(scanf, fmt, vargs)))
#endif

#define PRAGMA_DISABLE_GCC_WARNING_AUX(x) _Pragma(#x)
#define PRAGMA_DISABLE_GCC_WARNING(option_string) \
  PRAGMA_DISABLE_GCC_WARNING_AUX(GCC diagnostic ignored option_string)

#define PRAGMA_FORMAT_NONLITERAL_IGNORED                \
  PRAGMA_DISABLE_GCC_WARNING("-Wformat-nonliteral")     \
  PRAGMA_DISABLE_GCC_WARNING("-Wformat-security")

#define PRAGMA_FORMAT_IGNORED PRAGMA_DISABLE_GCC_WARNING("-Wformat")

// Disable -Wstringop-truncation which is introduced in GCC 8.
// https://gcc.gnu.org/gcc-8/changes.html
#if !defined(__clang_major__) && (__GNUC__ >= 8)
#define PRAGMA_STRINGOP_TRUNCATION_IGNORED PRAGMA_DISABLE_GCC_WARNING("-Wstringop-truncation")
#endif

// Disable -Wstringop-overflow which is introduced in GCC 7.
// https://gcc.gnu.org/gcc-7/changes.html
#if !defined(__clang_major__) && (__GNUC__ >= 7)
#define PRAGMA_STRINGOP_OVERFLOW_IGNORED PRAGMA_DISABLE_GCC_WARNING("-Wstringop-overflow")
#endif

#define PRAGMA_NONNULL_IGNORED \
  PRAGMA_DISABLE_GCC_WARNING("-Wnonnull")

#if defined(__clang_major__) && \
      (__clang_major__ >= 4 || \
      (__clang_major__ >= 3 && __clang_minor__ >= 1)) || \
    ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
// Tested to work with clang version 3.1 and better.
#define PRAGMA_DIAG_PUSH             _Pragma("GCC diagnostic push")
#define PRAGMA_DIAG_POP              _Pragma("GCC diagnostic pop")

#endif // clang/gcc version check

#if (__GNUC__ >= 9) || (defined(__clang_major__) && (__clang_major__ >= 14))

// Use "warning" attribute to detect uses of "forbidden" functions.
//
// Note: _FORTIFY_SOURCE transforms calls to certain functions into calls to
// associated "checking" functions, and that transformation seems to occur
// *before* the attribute check.  We use fortification in fastdebug builds,
// so uses of functions that are both forbidden and fortified won't cause
// forbidden warnings in such builds.
#define FORBID_C_FUNCTION(signature, alternative) \
  extern "C" __attribute__((__warning__(alternative))) signature;

// Disable warning attribute over the scope of the affected statement.
// The name serves only to document the intended function.
#define ALLOW_C_FUNCTION(name, ...)                     \
  PRAGMA_DIAG_PUSH                                      \
  PRAGMA_DISABLE_GCC_WARNING("-Wattribute-warning")     \
  __VA_ARGS__                                           \
  PRAGMA_DIAG_POP

#endif // gcc9+ or clang14+

#endif // SHARE_UTILITIES_COMPILERWARNINGS_GCC_HPP
