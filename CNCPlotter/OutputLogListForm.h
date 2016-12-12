#pragma once

#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"

// COutputLogListForm form view

class COutputLogListForm : public CFormView
{
	DECLARE_DYNCREATE(COutputLogListForm)

protected:
	COutputLogListForm();           // protected constructor used by dynamic creation
	virtual ~COutputLogListForm();
public:
	BOOL Create(LPCTSTR classname, LPCTSTR wndname, DWORD style, const RECT & rect, CWnd * parent, UINT id, CCreateContext *ctx)
	{
		return CFormView::Create(classname, wndname, style, rect, parent, id, ctx);
	}
	void OnInitialUpdate()
	{
		CFormView::OnInitialUpdate();

		CRect rc; GetClientRect( rc );
		if ( m_edit_log.m_hWnd )
			m_edit_log.SetWindowPos ( 0, rc.left + 2, rc.top + 2, rc.Width() - 4, rc.Height() - 4, SWP_NOZORDER );
		
		//AppendLogMessageWithTimestamp("Program started.");
		m_edit_log.SetLimitText(0);
	}

public:
	enum { IDD = IDD_FORMVIEW };
	CBrush* m_pEditBkBrush;
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit_log;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
};


