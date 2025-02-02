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

#include <Windows.h>
#include <string.h>
#include <winreg.h>
#include <malloc.h>
#include "env_path.h"

#define ENV_MAX_SIZE 32767

static const wchar_t SUBKEY_ENV_SYSTEM[] = L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
static const wchar_t SUBKEY_ENV_USER[] = L"Environment";
static const wchar_t ENV_NAME_PATH[] = L"Path";

static LSTATUS change_path_add(const wchar_t* const s, size_t slen, wchar_t* const buf, wchar_t* const pos, DWORD* const pcbData) {
    if (NULL != pos) {
        return ERROR_SUCCESS;
    }

    // append path to buf;
    // if successfully appended, len will be the total length of the string, including '\0' at the end;
    size_t len = *pcbData / 2 + slen + 1;
    if (len > ENV_MAX_SIZE) {
        return ERROR_BUFFER_OVERFLOW;
    }
    wcscpy_s(buf + *pcbData / 2 - 1, slen + 1, s);
    // write ";" to the end
    buf[len - 2] = L';';
    buf[len - 1] = 0;
    *pcbData += (slen + 1) * 2;
    return ERROR_SUCCESS;
}

static LSTATUS change_path_remove(const wchar_t* const s, size_t slen, wchar_t* const buf, wchar_t* const pos, DWORD* const pcbData) {
    if (NULL == pos) {
        return ERROR_SUCCESS;
    }

    wcscpy_s(pos, buf + ENV_MAX_SIZE - pos, pos + slen + 1); // don't forget the ';'
    *pcbData -= (slen + 1) * 2;
    return ERROR_SUCCESS;
}

typedef LSTATUS(*fn_change_path)(const wchar_t* const s, size_t slen, wchar_t* const buf, wchar_t* pos, DWORD* const pcbData);

static LSTATUS change_env_path(const wchar_t* const s, size_t slen, int scope, fn_change_path change_path) {
    LSTATUS status;
    HKEY hkey;
    LPCWSTR subkey;
    DWORD dwType;
    DWORD cbData = ENV_MAX_SIZE * 2; // in bytes
    wchar_t* buf = (wchar_t*)malloc(cbData);
    if (NULL == buf) {
        return ERROR_OUTOFMEMORY;
    }

    if (ENV_SCOPE_SYSTEM__ == scope) {
        hkey = HKEY_LOCAL_MACHINE;
        subkey = SUBKEY_ENV_SYSTEM;
    }
    else if (ENV_SCOPE_USER__ == scope) {
        hkey = HKEY_CURRENT_USER;
        subkey = SUBKEY_ENV_USER;
    }
    else {
        status = ERROR_BAD_ARGUMENTS;
        goto FREE;
    }
    status = RegGetValueW(hkey, subkey, ENV_NAME_PATH, RRF_RT_REG_EXPAND_SZ | RRF_RT_REG_MULTI_SZ | RRF_RT_REG_SZ, &dwType, buf, &cbData);
    if (ERROR_SUCCESS != status) {
        goto FREE;
    }

    wchar_t* pos = wcsstr(buf, s);
    status = change_path(s, slen, buf, pos, &cbData);
    if (ERROR_SUCCESS != status) {
        goto FREE;
    }

    // set key value
    status = RegSetKeyValueW(hkey, subkey, ENV_NAME_PATH, dwType, buf, cbData);
FREE:
    free(buf);
    return status;
}

static inline LSTATUS check_s(const wchar_t* s, size_t slen) {
    if (NULL == s || slen > MAX_PATH - 1) {
        return ERROR_FILENAME_EXCED_RANGE;
    }
    return ERROR_SUCCESS;
}

LSTATUS env_path_add(const wchar_t* s, size_t slen, int scope) {
    LSTATUS status = check_s(s, slen);
    if (ERROR_SUCCESS != status) {
        return status;
    }
    return change_env_path(s, slen, scope, change_path_add);
}

LSTATUS env_path_rmv(const wchar_t* s, size_t slen, int scope) {
    LSTATUS status = check_s(s, slen);
    if (ERROR_SUCCESS != status) {
        return status;
    }
    return change_env_path(s, slen, scope, change_path_remove);
}