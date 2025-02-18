﻿#include "StdAfx.h"
#include "ControlFactory.h"

namespace DuiLib 
{
	CControlFactory::CControlFactory()
	{
		INNER_REGISTER_DUICONTROL(CControlUI);
		INNER_REGISTER_DUICONTROL(CContainerUI);
		INNER_REGISTER_DUICONTROL(CButtonUI);
		INNER_REGISTER_DUICONTROL(CComboUI);
		INNER_REGISTER_DUICONTROL(CComboBoxUI);
		INNER_REGISTER_DUICONTROL(CDateTimeUI);
		INNER_REGISTER_DUICONTROL(CEditUI);
		INNER_REGISTER_DUICONTROL(CActiveXUI);
		INNER_REGISTER_DUICONTROL(CFlashUI);
		INNER_REGISTER_DUICONTROL(CGifAnimUI);
#ifdef USE_XIMAGE_EFFECT
		INNER_REGISTER_DUICONTROL(CGifAnimExUI);
#endif
		INNER_REGISTER_DUICONTROL(CGroupBoxUI);
		INNER_REGISTER_DUICONTROL(CIPAddressUI);
		INNER_REGISTER_DUICONTROL(CIPAddressExUI);
		INNER_REGISTER_DUICONTROL(CLabelUI);
		INNER_REGISTER_DUICONTROL(CListUI);
		INNER_REGISTER_DUICONTROL(CListHeaderUI);
		INNER_REGISTER_DUICONTROL(CListHeaderItemUI);
		INNER_REGISTER_DUICONTROL(CListLabelElementUI);
		INNER_REGISTER_DUICONTROL(CListTextElementUI);
		INNER_REGISTER_DUICONTROL(CListContainerElementUI);
		INNER_REGISTER_DUICONTROL(CMenuUI);
		INNER_REGISTER_DUICONTROL(CMenuElementUI);
		INNER_REGISTER_DUICONTROL(COptionUI);
		INNER_REGISTER_DUICONTROL(CCheckBoxUI);
		INNER_REGISTER_DUICONTROL(CProgressUI);
		INNER_REGISTER_DUICONTROL(CRichEditUI);
		INNER_REGISTER_DUICONTROL(CScrollBarUI);
		INNER_REGISTER_DUICONTROL(CSliderUI);
		INNER_REGISTER_DUICONTROL(CTextUI);
		INNER_REGISTER_DUICONTROL(CTreeNodeUI);
		INNER_REGISTER_DUICONTROL(CTreeViewUI);
		INNER_REGISTER_DUICONTROL(CWebBrowserUI);
		INNER_REGISTER_DUICONTROL(CAnimationTabLayoutUI);
		INNER_REGISTER_DUICONTROL(CChildLayoutUI);
		INNER_REGISTER_DUICONTROL(CHorizontalLayoutUI);
		INNER_REGISTER_DUICONTROL(CTabLayoutUI);
		INNER_REGISTER_DUICONTROL(CTileLayoutUI);
		INNER_REGISTER_DUICONTROL(CTileElementUI);
		INNER_REGISTER_DUICONTROL(CVerticalLayoutUI);
		INNER_REGISTER_DUICONTROL(CRollTextUI);
		INNER_REGISTER_DUICONTROL(CColorPaletteUI);
		INNER_REGISTER_DUICONTROL(CListExUI);
		INNER_REGISTER_DUICONTROL(CListContainerHeaderItemUI);
		INNER_REGISTER_DUICONTROL(CListTextExtElementUI);
		INNER_REGISTER_DUICONTROL(CHotKeyUI);
		INNER_REGISTER_DUICONTROL(CFadeButtonUI);
		INNER_REGISTER_DUICONTROL(CRingUI);
		INNER_REGISTER_DUICONTROL(CIconUI);
		INNER_REGISTER_DUICONTROL(CImageExUI);
	}

	CControlFactory::~CControlFactory()
	{
	}

	CControlUI* CControlFactory::CreateControl(CDuiString strClassName)
	{
		strClassName.MakeLower();
		MAP_DUI_CTRATECLASS::iterator iter = m_mapControl.find(strClassName);
		if ( iter == m_mapControl.end()) {
			return NULL;
		}
		else {
			return (CControlUI*) (iter->second());
		}
	}

	void CControlFactory::RegistControl(CDuiString strClassName, CreateClass pFunc)
	{
		strClassName.MakeLower();

        //loki ---后来的替换原有的
        MAP_DUI_CTRATECLASS::_Pairib ret = m_mapControl.insert(MAP_DUI_CTRATECLASS::value_type(strClassName, pFunc));
        if (!ret.second && ret.first != m_mapControl.end())
            ret.first->second = pFunc;
        //-----------------end
	}

	CControlFactory* CControlFactory::GetInstance()  
	{
		static CControlFactory* pInstance = new CControlFactory;
		return pInstance;
	}

	void CControlFactory::Release()
    {
		delete this;
	}
}