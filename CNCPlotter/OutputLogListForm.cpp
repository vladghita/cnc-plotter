// ListForm.cpp : implementation file
//

#include "stdafx.h"
#include "CNCPlotter.h"
#include "OutputLogListForm.h"


// COutputLogListForm

IMPLEMENT_DYNCREATE(COutputLogListForm, CFormView)

COutputLogListForm::COutputLogListForm()
	: CFormView(COutputLogListForm::IDD)
{
	m_pEditBkBrush = new CBrush(RGB(255, 255, 255));
}

COutputLogListForm::~COutputLogListForm()
{
}

void COutputLogListForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit_log);
}

BEGIN_MESSAGE_MAP(COutputLogListForm, CFormView)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// COutputLogListForm diagnostics

#ifdef _DEBUG
void COutputLogListForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void COutputLogListForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// COutputLogListForm message handlers


void COutputLogListForm::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	CRect rc; GetClientRect( rc );
	if ( m_edit_log.m_hWnd )
	{
		m_edit_log.SetWindowPos ( 0, rc.left + 2, rc.top + 2, rc.Width() - 4, rc.Height() - 4, SWP_NOZORDER );
	}
}


HBRUSH COutputLogListForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor) 
	{
          case CTLCOLOR_STATIC:
			  if (pWnd->GetDlgCtrlID() == IDC_EDIT1)
			   {
			      pDC->SetTextColor(RGB(0, 0, 0));
			      pDC->SetBkColor(RGB(255, 255, 255));
			   }
              return (HBRUSH)(m_pEditBkBrush->GetSafeHandle());

          default:
              return hbr;
    }
}


void COutputLogListForm::OnDestroy()
{
	CFormView::OnDestroy();

	delete m_pEditBkBrush;
}
