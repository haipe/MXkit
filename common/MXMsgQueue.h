﻿#pragma once

#include <assert.h>
#include "MXCommon.h"
#include "MXSemaphore.h"
#include "MXElementAllocator.h"


namespace mxtoolkit
{

template <class T>
class MXMsgQueue
{
    class AutoLock
    {
    public:
        AutoLock(MXMsgQueue* mq)
            : m_auto(mq->m_mutex) {}
    private:
        std::lock_guard<std::recursive_mutex> m_auto;
    };

public:
    MXMsgQueue(uint32 maxMsgCount = 1000) :
        m_wasStop(FALSE),
        m_maxMsgCount(maxMsgCount),
        m_userData(0),
        m_flag(0),
        m_msgCount(0),
        m_headElement(0),
        m_tailElement(0),
        m_sem(maxMsgCount),
        m_elementAllocator(NULL)
    {
        m_pMsg = new T*[maxMsgCount + 1];
#ifdef _MX_WIN
        m_evStop = CreateEvent(NULL, TRUE, FALSE, NULL);
#endif
    };
    virtual ~MXMsgQueue()
    {
        SetStop();

        AutoLock autoLock(this);

        delete[] m_pMsg;
        m_msgCount = 0;
        m_headElement = m_tailElement = 0;
        
#ifdef _MX_WIN
        CloseHandle(m_evStop);
#endif
    };
    typedef MXElementAllocator<T>	Alloctor;

public:
    BOOL PushMsg(T *pMsg, uint32 *pdwMsgCount = NULL)
    {
#ifdef _MX_WIN
        if (WaitForSingleObject(m_evStop, 0) == WAIT_OBJECT_0)
            return FALSE;
#else
        if (m_wasStop)
            return FALSE;
#endif

        AutoLock autoLock(this);

        if (m_msgCount >= m_maxMsgCount)
        {
            return FALSE;
        }
        m_pMsg[m_tailElement++] = pMsg;
        m_tailElement = (uint32)m_tailElement > m_maxMsgCount ? 0 : m_tailElement;
        ++m_msgCount;
        if (pdwMsgCount)
            *pdwMsgCount = m_msgCount;
        
        m_sem.Signal();

        return TRUE;
    }
    T*	PopMsg(uint32 dwWaitTime = INFINITE, uint32 *pdwMsgCount = NULL)
    {
        if (m_wasStop)
            return NULL;

        m_sem.Wait(dwWaitTime);

        if (m_wasStop)
            return NULL;

        AutoLock autoLock(this);

        T* pMsg = m_pMsg[m_headElement++];
        m_headElement = (uint32)m_headElement > m_maxMsgCount ? 0 : m_headElement;
        --m_msgCount;
        if (pdwMsgCount)
            *pdwMsgCount = m_msgCount;
        
        return pMsg;
    };
    VOID	SetStop()
    {
#ifdef _MX_WIN
        SetEvent(m_evStop);
#endif
        m_wasStop = TRUE;
    };
    VOID	ResetStop()
    {
#ifdef _MX_WIN
        ResetEvent(m_evStop);
#endif
        m_wasStop = FALSE;
    };
    uint32	GetMsgCount() { return m_msgCount; };
    uint32	GetMaxMsgCount() { return m_maxMsgCount; };
    VOID		SetMaxMsgCount(uint32 dwMaxMsgCount) { m_maxMsgCount = dwMaxMsgCount; };
    BOOL		IsFull() { return m_msgCount >= m_maxMsgCount; };
    VOID		SetFlag(uint32 dwFlag) { m_flag = dwFlag; };
    uint32	GetFlag() { return m_flag; };
    VOID		SetUserData(autoBit dwUserData) { m_userData = dwUserData; };
    autoBit		GetUserData() { return m_userData; };
    T*			Alloc() { return m_elementAllocator ? m_elementAllocator->Alloc() : NULL; };
    void		Free(T* pMsg) { if (m_elementAllocator) m_elementAllocator->Free(pMsg); }
    void		SetAllocator(MXElementAllocator<T> *pAllocator) { m_elementAllocator = pAllocator; };

protected:
    BOOL	m_wasStop;
#ifdef _MX_WIN
    HANDLE	m_evStop;
#endif
    uint32	m_maxMsgCount;
    autoBit	m_userData;
    uint32	m_flag;
    uint32	m_msgCount;

    T**	m_pMsg;
    int32	m_headElement;
    int32	m_tailElement;

    MXSemaphore m_sem;
    std::recursive_mutex	m_mutex;
    Alloctor*	m_elementAllocator;
};


}

