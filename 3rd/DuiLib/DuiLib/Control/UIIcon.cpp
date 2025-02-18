﻿#include "StdAfx.h"
#include "UIIcon.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CIconUI)
	//////////////////////////////////////////////////////////////////////////
	// CIconUI
	CIconUI::CIconUI(void)
	{
		m_hIcon = NULL;
		memset(&m_rcInset, 0, sizeof(m_rcInset));
		SetAttribute(_T("bkcolor"), _T("#00000000"));
	}

	CIconUI::~CIconUI(void)
	{
		if (m_hIcon)
		{
			DeleteObject(m_hIcon);
			m_hIcon = NULL;
		}
	}

	LPCTSTR CIconUI::GetClass() const
	{
		return _T("IconUI");
	}

	LPVOID CIconUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcscmp(pstrName, DUI_CTR_ICON) == 0) return static_cast<CIconUI*>(this);
		return __super::GetInterface(pstrName);
	}

	void CIconUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcscmp(pstrName, _T("icon")) == 0)
			m_sIcoImage = pstrValue;
		if (_tcscmp(pstrName, _T("inset")) == 0)
		{
			RECT rcInset = { 0 };
			LPTSTR pstr = NULL;
			rcInset.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
			rcInset.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
			rcInset.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
			rcInset.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
			SetInset(rcInset);
		}
		Invalidate();
		__super::SetAttribute(pstrName, pstrValue);
	}

	void CIconUI::SetPos(RECT rc)
	{
		CControlUI::SetPos(rc);
	}

	void CIconUI::SetInset(RECT rc)
	{
		m_rcInset = rc;
	}

	void CIconUI::SetIcon(HICON hIcon)
	{
		if (m_hIcon)
		{
			DeleteObject(m_hIcon);
			m_hIcon = NULL;
		}
		m_hIcon = hIcon;
	}

	void CIconUI::DoEvent(TEventUI& event)
	{
		CControlUI::DoEvent(event);
	}

	void CIconUI::SetVisible(bool bVisible)
	{
		CControlUI::SetVisible(bVisible);
	}

	void CIconUI::SetInternVisible(bool bVisible)
	{
		CControlUI::SetInternVisible(bVisible);

	}

	bool CIconUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
	{
		RECT rcTemp = { 0 };
		if (!::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem)) return false;

		if (NULL != m_hIcon)
		{
			RECT rect;
			rect.left = m_rcItem.left + m_rcInset.left;
			rect.right = m_rcItem.right - m_rcInset.right;
			rect.top = m_rcItem.top + m_rcInset.top;
			rect.bottom = m_rcItem.bottom - m_rcInset.bottom;
			::DrawIconEx(hDC, rect.left, rect.top, m_hIcon, rect.right - rect.left,
				rect.bottom - rect.top, 0, NULL, DI_NORMAL);
			
		}

		return true;
	}


}