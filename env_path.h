/*
MIT License

Copyright(c) 2025 Joe Tse

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <winreg.h>

#define ENV_SCOPE_SYSTEM__ 0
#define ENV_SCOPE_USER__ 1

/// <summary>
/// Add a string to SYSTEM or USER environment variable 'Path' in Registry.
/// If string exists in 'Path' do nothing and return success.
/// </summary>
/// <param name="s">string to be added</param>
/// <param name="slen">number of wchar_t of <strong>s</strong>, not including null terminator L'\0'</param>
/// <param name="scope">
/// <strong>ENV_SCOPE_SYSTEM__</strong> may return error if no permission, or
/// <strong>ENV_SCOPE_USER__</strong>
/// </param>
/// <returns>
/// <strong>ERROR_SUCCESS</strong> when no error,
/// else error code that can be processed by <strong>FormatMessage.</strong>
/// </returns>
LSTATUS env_path_add(const wchar_t* s, size_t slen, int scope);

/// <summary>
/// Remove a string from SYSTEM or USER environment variable 'Path' in Registry.
/// If string not in 'Path' do nothing and return success..
/// </summary>
/// <param name="s">string to be removed</param>
/// <param name="slen">number of wchar_t of <strong>s</strong>, not including null terminator L'\0'</param>
/// <param name="scope">
/// <strong>ENV_SCOPE_SYSTEM__</strong> may return error if no permission, or
/// <strong>ENV_SCOPE_USER__</strong>
/// </param>
/// <returns>
/// <strong>ERROR_SUCCESS</strong> when no error,
/// else error code that can be processed by <strong>FormatMessage.
/// </strong></returns>
LSTATUS env_path_rmv(const wchar_t* s, size_t slen, int scope);