﻿// Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <windows.h>
#include <WinBase.h>
#include <iostream>
#include <string>
#include "IWebRequest.h"
#include "MXDllExportDefine.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iosfwd>
#include "MXSemaphore.h"

mxwebrequest::IWebRequest* wr = nullptr;

int webCount = 0;
mxtoolkit::MXSemaphore se(0);

class WRNotify : public mxwebrequest::IRespondNotify
{
public:
    virtual void OnCompleteRespond(uint32 nID, uint32 nCode, const char *pData, uint32 nSize) override
    {
        printf("Url :%d, data:%s.\n", nID, pData);

        webCount++;
        if (webCount <= 1)
        {
            mxwebrequest::Request rq;
            rq.request_type = mxwebrequest::REQUEST_TYPE_GET;
            rq.request_protocol = mxwebrequest::REQUEST_PROTOCOLTYPE_HTTPS;
            rq.respond_data_protocol = mxwebrequest::RESPOND_PROTOCOL_JSON;
            rq.request_host = (CHAR*)"https://v1.hitokoto.cn";

            unsigned int id = wr->AsynRequest(&rq);
            std::cout << "rquest ID:" << id << std::endl;
        }
        else
        {
            std::cout << "end.\n";
            se.Signal();
        }
    }

};

#include <boost/interprocess/ipc/message_queue.hpp>  
#include <iostream>  
#include <vector>  

using namespace boost::interprocess;

int main()
{
    std::cout << "Hello World!\n";
    


    try {
        message_queue mq
        (
            open_only        //only create  
            , "message_queue"  //name  
        );

        //Create a message_queue.  
//         message_queue mq
//         (
//             create_only               //only create  
//             , "message_queue"           //name  
//             , 100                       //max message number  
//             , sizeof(int)               //max message size  
//         );
        
        unsigned int priority;
        message_queue::size_type recvd_size;

        //Receive 100 numbers  
        for (int i = 0; i < 100; ++i)
        {
            int number;
            //boost::posix_time::ptime abs_time;
            //mq.timed_receive(&number, sizeof(number), recvd_size, priority, abs_time);
            mq.receive(&number, sizeof(number), recvd_size, priority);
            printf("I:%d Rec:%d\n", i, number);
            if (recvd_size != sizeof(number))
                break;
        }
    }
    catch (interprocess_exception &ex)
    {
        message_queue::remove("message_queue");
        std::cout << ex.what() << std::endl;
        return 1;
    }

    message_queue::remove("message_queue");


    return 0;
    mxwebrequest::IWebRequest* webrequest = nullptr;

    mxtoolkit::MXDllObject mxDllObj;

#ifdef _DEBUG
    HMODULE webrequest_dll = MXInitDll(mxDllObj, "MXWebRequestD.dll");
#else
    HMODULE webrequest_dll = MXInitDll(mxDllObj, "MXWebRequest.dll");
#endif

    std::cout << "GetLastError: " << (int)GetLastError << std::endl;
    if (mxDllObj.dllInit)
        mxDllObj.dllInit();

    mxtoolkit::MXDllExportInfo* all_export = nullptr;
    if (mxDllObj.getExportInfo)
    {
        mxDllObj.getExportInfo(&all_export);
    }

    mxtoolkit::MXInterfaceInfo info = *all_export->interfaceInfo;

    if (mxDllObj.getInterfaceInfo)
    {
        mxDllObj.getInterfaceInfo(&info, (void**)&wr);
    }

    WRNotify notify;
    if (wr)
    {
        wr->Initialize(dynamic_cast<mxwebrequest::IRespondNotify*>(&notify));

        mxwebrequest::Request rq;
        rq.request_type = mxwebrequest::REQUEST_TYPE_GET;
        rq.request_protocol = mxwebrequest::REQUEST_PROTOCOLTYPE_HTTPS;
        rq.respond_data_protocol = mxwebrequest::RESPOND_PROTOCOL_JSON;
        rq.request_host = (CHAR*)"https://v1.hitokoto.cn";

        unsigned int id = wr->AsynRequest(&rq);
        std::cout << "rquest ID:" << id << std::endl;
    }


    se.Wait(-1);

    std::cout << "end 111.\n";
    wr->Uninstall();

    std::cout << "end2222.\n";
    mxDllObj.dllUninit();
    
    std::cout << "end3333333333333.\n";
    FreeLibrary(webrequest_dll);
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
