// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

#include "stdafx.h"

#include "OutputLogWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "CNCPlotter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

COutputLogWnd::COutputLogWnd()
	:	CDockablePane()
	,	m_listForm( (COutputLogListForm *)RUNTIME_CLASS(COutputLogListForm)->CreateObject() )
{
}

COutputLogWnd::~COutputLogWnd()
{
}

BEGIN_MESSAGE_MAP(COutputLogWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void COutputLogWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	int cyCmb = rectCombo.Size().cy;

	m_listForm->SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height()/*500*/, SWP_NOACTIVATE | SWP_NOZORDER);
}

int COutputLogWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_listForm->Create("CListForm", "List", WS_VISIBLE | WS_CHILD, rectDummy, this, 3, NULL))
	{
		TRACE0("Failed to create form\n");
		return -1;      // fail to create
	}

	SetMinSize(CSize(250, 400));

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	InitPropList();

	AdjustLayout();
	return 0;
}

void COutputLogWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void COutputLogWnd::OnExpandAllProperties()
{
	//m_wndPropList.ExpandAll();
}

void COutputLogWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void COutputLogWnd::OnSortProperties()
{
//	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void COutputLogWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
//	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void COutputLogWnd::OnProperties1()
{
	// TODO: Add your command handler code here
}

void COutputLogWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void COutputLogWnd::OnProperties2()
{
	// TODO: Add your command handler code here
}

void COutputLogWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void COutputLogWnd::InitPropList()
{
	
}

void COutputLogWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
//	m_wndPropList.SetFocus();
}

void COutputLogWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void COutputLogWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

//	m_wndPropList.SetFont(&m_fntPropList);
//	m_wndObjectCombo.SetFont(&m_fntPropList);
}
