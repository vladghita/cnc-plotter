#include "StdAfx.h"
#include "Logger.h"

#include "CNCPlotter.h"
#include "CNCPlotterDoc.h"
#include "CNCPlotterView.h"
#include "MainFrm.h"


CLogger::CLogger(void)
{
	::InitializeCriticalSection(&cs);
}

CLogger::~CLogger(void)
{
	::DeleteCriticalSection(&cs);
}

void CLogger::AppendTextToOutputLog(LPCTSTR pszText)
{
	if( theApp.m_pMainWnd && ((CMainFrame *)theApp.m_pMainWnd)->GetOutputLogWnd() && ((CMainFrame *)theApp.m_pMainWnd)->GetOutputLogWnd()->GetListForm())
	{
		EnterCriticalSection(&cs);
	
	    HWND hEdit = ((CMainFrame *)theApp.m_pMainWnd)->GetOutputLogWnd()->GetListForm()->GetSafeHwnd();
	
		DWORD iStart, iEnd;
		::SendDlgItemMessage(hEdit, IDC_EDIT1, EM_GETSEL, (WPARAM)&iStart, (LPARAM)&iEnd);
		int ndx = iStart >= iEnd? iStart : iEnd;
	
		#ifdef WIN32
			::SendDlgItemMessage(hEdit, IDC_EDIT1, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
		#else
			::SendDlgItemMessage(hEdit, IDC_EDIT1, EM_SETSEL, 0, MAKELONG (ndx, ndx));
		#endif
	
		::SendDlgItemMessage(hEdit, IDC_EDIT1, EM_REPLACESEL, NULL, (LPARAM)pszText);
	
		LeaveCriticalSection(&cs);
	}
}

void CLogger::AppendTextToOutputLogWithNewLine(LPCTSTR pszText)
{
   CString strLine;
   // add CR/LF to text
   strLine.Format(_T("\r\n%s"), pszText);
   AppendTextToOutputLog(strLine);
}

void CLogger::AppendLogMessageWithTimestamp(LPCTSTR pszText)
{
	SYSTEMTIME now;
	::GetLocalTime(&now);
	CString strTime;
	strTime.Format("\r\n@ %02u:%02u:%02u > ", now.wHour, now.wMinute, now.wSecond);

    AppendTextToOutputLogWithNewLine(strTime);
	AppendTextToOutputLog(pszText);
}

void CLogger::AppendLogMessage(LPCTSTR pszText)
{
	AppendTextToOutputLogWithNewLine(pszText);
}