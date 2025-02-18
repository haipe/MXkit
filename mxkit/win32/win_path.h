﻿#pragma once

#include <string>
#include "mxkit.h"
#include "base/string_utils.h"


_BEGIN_MX_KIT_NAME_SPACE_


template<typename Str>
class Win32Path
{
public:
    typedef typename Str::allocator_type::value_type CharType;
    static const Str& CurrentDirectory(HMODULE module = NULL)
    {
        if (appDirectory.empty())
        {
            if (std::is_same<CharType, wchar_t>::value)
            {
                WCHAR path[MAX_PATH] = { 0 };
                GetModuleFileNameW(module, path, MAX_PATH);
                appDirectory = (CharType*)path;
                appDirectory = appDirectory.substr(0, appDirectory.find_last_of('\\') + 1);
            }
            else
            {
                CHAR path[MAX_PATH] = { 0 };
                GetModuleFileNameA(module, path, MAX_PATH);
                appDirectory = (CharType*)path;
                appDirectory = appDirectory.substr(0, appDirectory.find_last_of('\\') + 1);
            }
        }

        return appDirectory;
    }

    static const Str& ModuleDirectory(HMODULE module)
    {
        if (module == NULL)
        {
            _ASSERT(!module);
            static Str s;
            return s;
        }

        if (moduleDirectory.empty())
        {
            if (std::is_same<CharType, wchar_t>::value)
            {
                WCHAR path[MAX_PATH] = { 0 };
                GetModuleFileNameW(module, path, MAX_PATH);
                moduleDirectory = (CharType*)path;
                moduleDirectory = moduleDirectory.substr(0, moduleDirectory.find_last_of('\\') + 1);
            }
            else
            {
                CHAR path[MAX_PATH] = { 0 };
                GetModuleFileNameA(module, path, MAX_PATH);
                moduleDirectory = (CharType*)path;
                moduleDirectory = moduleDirectory.substr(0, moduleDirectory.find_last_of('\\') + 1);
            }
        }

        return moduleDirectory;
    }


private:
    static Str appDirectory;
    static Str moduleDirectory;
};

template<typename Str>
Str Win32Path<Str>::appDirectory;

template<typename Str>
Str Win32Path<Str>::moduleDirectory;


_END_MX_KIT_NAME_SPACE_