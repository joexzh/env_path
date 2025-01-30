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

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "env_path.h"

#define STR_MAX_BUF 1024

static const wchar_t ERR_UNKNOWN[] = L"Unknown Error";
static const wchar_t ERR_CMD_ARG[] = L"Bad commandline arguments";

static inline void err_msg(DWORD error_code, wchar_t* buf) {
    if (0 == FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, error_code, 0,
        buf, STR_MAX_BUF, NULL)) {
        // function call failed
        wcscpy_s(buf, STR_MAX_BUF, ERR_UNKNOWN);
    }
}

int wmain(int argc, wchar_t** argv) {
    if (argc != 3) {
        fwprintf(stderr, L"%s\n", ERR_CMD_ARG);
        return 1;
    }

    LSTATUS status;
    size_t slen = wcsnlen_s(argv[2], STR_MAX_BUF);

    if (0 == wcscmp(L"1", argv[1])) {
        status = env_path_add(argv[2], slen, ENV_SCOPE_USER__);
    }
    else if (0 == wcscmp(L"0", argv[1])) {
        status = env_path_rmv(argv[2], slen, ENV_SCOPE_USER__);
    }
    else {
        fwprintf(stderr, L"%s\n", ERR_CMD_ARG);
        return 1;
    }

    if (ERROR_SUCCESS != status) {
        wchar_t buf[STR_MAX_BUF];
        err_msg(status, buf);
        fwprintf(stderr, L"%s\n", buf);
        return 1;
    }

    return 0;
}