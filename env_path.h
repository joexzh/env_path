/*
MIT License

Copyright (c) 2025 Joe Tse

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <winreg.h>

#define ENV_SCOPE_SYSTEM__ 0
#define ENV_SCOPE_USER__ 1

/**
 * @brief Add a string to SYSTEM or USER environment variable 'Path' in Registry.
 * 
 * If string exists in 'Path' do nothing and returns success.
 * Underfined behavior if s is not a C string.
 * 
 * @param s string to be added
 * @param slen number of wchar_t of s, not including null terminator L'\0'
 * @param scope ENV_SCOPE_SYSTEM__ for system scope, may return error, or ENV_SCOPE_USER__ for user scope.
 * @return ERROR_SUCCESS when no error, else error code that can be consumed by FormatMessage.
 */
LSTATUS env_path_add(const wchar_t* s, size_t slen, int scope);

/**
* @brief Remove a string from SYSTEM or USER environment variable 'Path' in Registry.
* 
* If string not in 'Path' do nothing and return success.
* Underfined behavior if s is not a C string.
* 
* @param s string to be removed
* @param slen number of wchar_t of s, not including null terminator L'\0'
* @param scope ENV_SCOPE_SYSTEM__ for system scope, may return error, or ENV_SCOPE_USER__ for user scope.
* @return ERROR_SUCCESS when no error, else error code that can be consumed by FormatMessage.
*/
LSTATUS env_path_rmv(const wchar_t* s, size_t slen, int scope);