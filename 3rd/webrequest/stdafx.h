﻿// stdafx.h: 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>



// 在此处引用程序需要的其他标头
#include "MXCommon.h"
#include "MXStringKit.h"


#define _URL_TASK_TO_MANAGER_MSG_ 10241

namespace mxwebrequest
{
    typedef void(CALLBACK* WEBREQUSET_CALLBACK)(mxtoolkit::uint32 msg, mxtoolkit::uint32 param, mxtoolkit::autoBit reserver1, mxtoolkit::autoBit reserve2, mxtoolkit::autoBit userData);
}