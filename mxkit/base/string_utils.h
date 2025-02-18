﻿#pragma once
#include <string>
#include <vector>
#include <functional>
#include <algorithm>

#include <sstream>

#include "mxkit.h"
#include "base/string.h"


#include <Windows.h>

_BEGIN_MX_KIT_NAME_SPACE_

class StringUtils
{
public:

    template<
        typename T
        , typename Str
#if _MX_DEFAULT_TEMPLATE_ARGUMENTS_
        = std::string
#endif
    >
        static inline T ToValue(const Str& value)
    {
        return T();
    }

    template<typename T>
    static inline T ToValue(const std::wstring& value)
    {
        std::wstringstream ss(value);
        T ret;
        ss >> ret;
        return ret;
    }

    template<typename T>
    static inline T ToValue(const std::string& value)
    {
        std::stringstream ss(value);
        T ret;
        ss >> ret;
        return ret;
    }


    template<typename T>
    static inline std::stringstream ToAStream(const T& v)
    {
        std::stringstream ss;
        ss << v;
        return ss;
    }

    template<typename T>
    static inline std::wstringstream ToWStream(const T& v)
    {
        std::wstringstream ss;
        ss << v;
        return ss;
    }

    template<
        typename T
        , typename Str
#if _MX_DEFAULT_TEMPLATE_ARGUMENTS_
        = std::string
#endif
    >
        static inline Str ToString(const T& value)
    {
        typedef typename Str::allocator_type::value_type CharType;
        if (std::is_same<CharType, wchar_t>::value && std::is_same<Str, std::wstring>::value)
        {
            std::wstringstream ss;
            ss << value;
            return Str((CharType*)ss.str().c_str());
        }
        else
        {
            std::stringstream ss;
            ss << value;
            return Str((CharType*)ss.str().c_str());
        }
    }

    template<
        typename Str
#if _MX_DEFAULT_TEMPLATE_ARGUMENTS_
        = std::string
#endif
    >
        static inline Str ToString(const RECT& rc)
    {
        typedef typename Str::allocator_type::value_type CharType;
        CharType cache[64] = { 0 };
        if (std::is_same<CharType, wchar_t>::value && std::is_same<Str, std::wstring>::value)
        {
            _snwprintf(
                cache,
                sizeof(cache) * sizeof(WCHAR),
                L"{l:%d,t:%d,r:%d,b:%d,w:%d,h:%d}",
                rc.left, rc.top, rc.right, rc.bottom, rc.right - rc.left, rc.bottom - rc.top);

        }
        else
        {
            snprintf(cache,
                sizeof(cache) * sizeof(CHAR),
                "{l:%d,t:%d,r:%d,b:%d,w:%d,h:%d}",
                rc.left, rc.top, rc.right, rc.bottom, rc.right - rc.left, rc.bottom - rc.top);
        }

        return Str((CharType)cache);
    }


    template<
        typename Str
#if _MX_DEFAULT_TEMPLATE_ARGUMENTS_
        = std::string
#endif
    >
        static inline Str ToLower(Str& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower); //将大写的都转换成小写
        return str;
    }

    template<
        typename Str
#if _MX_DEFAULT_TEMPLATE_ARGUMENTS_
        = std::string
#endif
    >
        static inline Str& ToUpper(Str& str)
    {
        transform(str.begin(), str.end(), str.begin(), ::toupper); //将大写的都转换成小写
        return str;
    }


    template<
        typename CharType
#if _MX_DEFAULT_TEMPLATE_ARGUMENTS_
        = char
#endif
    >
        static inline CharType Exchange(CharType c)
    {
        if (std::is_same<CharType, wchar_t>::value)
        {
            if (c <= L'Z' && c >= L'A')
                c = tolower(c);
            else if (c >= L'a' && c <= L'z')
                c = toupper(c);

            return c;
        }
        else
        {
            if (c <= 'Z' && c >= 'A')
                c = tolower(c);
            else if (c >= 'a' && c <= 'z')
                c = toupper(c);

            return c;
        }
    }

    template<
        typename Str
#if _MX_DEFAULT_TEMPLATE_ARGUMENTS_
        = std::string
#endif
    >
        static inline Str& ToExchange(Str& str)
    {
        typedef typename Str::allocator_type::value_type CharType;
        transform(str.begin(), str.end(), str.begin(), Exchange<CharType>);  //大小写切换
        return str;
    }

    template<
        typename Str
#if _MX_DEFAULT_TEMPLATE_ARGUMENTS_
        = std::string
#endif
    >
        static inline Str& EraseLast(Str& in, const Str& willErase, unsigned int cnt = 1)
    {
        //删除最后字符，并判断
        if (cnt == 0 || willErase.empty())
            return in;

        size_t pos = in.rfind(willErase);
        size_t len = in.length();
        size_t eraseLen = willErase.length();
        while (pos != Str::npos && pos == (len - 1))
        {
            in.resize(in.size() - eraseLen);
            pos = in.rfind(willErase);
            len = in.length();
        }

        return in;
    }



    template<
        typename Str
#if _MX_DEFAULT_TEMPLATE_ARGUMENTS_
        = std::string
#endif
    >
        static inline Str& Replace(Str& in, const Str& pattern, const Str& newpat, unsigned int* cnt = nullptr)
    {
        int count = 0;
        const size_t nsize = newpat.size();
        const size_t psize = pattern.size();

        for (size_t pos = in.find(pattern, 0); pos != Str::npos; pos = in.find(pattern, pos + nsize))
        {
            in.replace(pos, psize, newpat);
            count++;
        }

        if (cnt)
            *cnt = count;

        return in;
    }


    template<
        typename Str
#if _MX_DEFAULT_TEMPLATE_ARGUMENTS_
        = std::string
#endif
    >
        static inline bool Split(const Str& in, const Str& sp, Str* left, Str* right, bool removeSplit = true)
    {
        if (!left && !right)
            return false;

        const size_t nsize = sp.size();
        size_t pos = in.find(sp);
        if (left)
            *left = in.substr(0, pos);

        if (right)
        {
            if (pos == Str::npos)
                right->clear();
            else
                *right = in.substr(pos + (removeSplit ?  nsize : 0));
        }

        return true;
    }


    template<
        typename Str
#if _MX_DEFAULT_TEMPLATE_ARGUMENTS_
        = std::string
#endif
        , typename Tout
#if _MX_DEFAULT_TEMPLATE_ARGUMENTS_
        = std::vector<Str>
#endif
    >
        static inline unsigned int Split(const Str& in, const Str& sp, Tout* out, bool removeEmpty = false)
    {
        if (in.empty() || out == nullptr)
            return 0;

        out->clear();
        size_t bpos = 0;
        size_t pos = in.find(sp);

        Str x = in.substr(bpos, pos);

        if (!removeEmpty || !x.empty())
            out->push_back(x);

        if (pos == Str::npos)
            return (unsigned int)out->size();

        size_t sp_len = sp.length();
        size_t in_len = in.length();

        bpos = pos + sp_len;

        while (true)
        {
            pos = in.find(sp, bpos);

            x = in.substr(bpos, pos - bpos);
            if (!removeEmpty || !x.empty())
                out->push_back(x);

            if (pos == Str::npos)
                break;

            bpos = pos + sp_len;
            if (bpos >= in_len)
            {
                if (!removeEmpty)
                    out->push_back(Str());

                break;
            }
        }

        return (unsigned int)out->size();
    }

    template<
        typename Str
#if _MX_DEFAULT_TEMPLATE_ARGUMENTS_
        = std::string
#endif
        , typename Tout
#if _MX_DEFAULT_TEMPLATE_ARGUMENTS_
        = std::vector<Str>
#endif
    >
        static inline unsigned int Split(const Str& in, const Str& sp, std::function<void(unsigned int index, const Str& item)> onOneString)
    {
        if (in.empty() || !onOneString)
            return 0;

        size_t bpos = 0;
        size_t pos = in.find(sp);

        Str x = in.substr(bpos, pos);

        unsigned int index = 0;
        onOneString(index++, x);

        if (pos == Str::npos)
            return index;

        size_t sp_len = sp.length();
        size_t in_len = in.length();
        bpos = pos + sp_len;

        while (true)
        {
            pos = in.find(sp, bpos);

            x = in.substr(bpos, pos - bpos);
            onOneString(index++, x);

            if (pos == Str::npos)
                break;

            bpos = pos + sp_len;
            if (bpos >= in_len)
            {
                onOneString(index++, Str());
                return index;
            }
        }

        return index;
    }


    template<
        typename Str
#if _MX_DEFAULT_TEMPLATE_ARGUMENTS_
        = std::string
#endif
    >
        static inline int VersionStringCompare(const Str& left, const Str& right)
    {
        size_t left_begin = 0, left_pos = 0;
        size_t  right_begin = 0, right_pos = 0;
        while (true)
        {
            left_pos = left.find('.', left_begin);
            right_pos = right.find('.', right_begin);

            Str left_str = left.substr(left_begin, left_pos - left_begin);
            Str right_str = right.substr(right_begin, right_pos - right_begin);

            int left_v = 0;
            int right_v = 0;

            try { left_v = std::stoi(left_str); }
            catch (...) {}

            try { right_v = std::stoi(right_str); }
            catch (...) {}

            if (left_v != right_v)
                return left_v < right_v ? -1 : 1;

            left_begin = left_pos + 1;
            right_begin = right_pos + 1;

            if (left_pos == Str::npos && right_pos == Str::npos)
                return 0;
            else if (left_pos == Str::npos)
                return -1;
            else if (right_pos == Str::npos)
                return 1;
        }

        return 0;
    }

};

template<
    typename Str
#if _MX_DEFAULT_TEMPLATE_ARGUMENTS_
    = std::string
#endif
>
struct VersionString
{
    VersionString(const Str& v) : ref(v) {}
    const Str& ref;

    bool operator < (const VersionString& other)const
    {
        return StringUtils::VersionStringCompare(ref, other.ref) < 0;
    }

    bool operator <= (const VersionString& other)const
    {
        return StringUtils::VersionStringCompare(ref, other.ref) <= 0;
    }

    bool operator > (const VersionString& other)const
    {
        return StringUtils::VersionStringCompare(ref, other.ref) > 0;
    }

    bool operator >= (const VersionString& other)const
    {
        return StringUtils::VersionStringCompare(ref, other.ref) >= 0;
    }

    bool operator == (const VersionString& other)const
    {
        return StringUtils::VersionStringCompare(ref, other.ref) == 0;
    }
};


_END_MX_KIT_NAME_SPACE_