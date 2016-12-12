#include "StdAfx.h"
#include "PlottingThread.h"

#include "CNCPlotter.h"
#include "CNCPlotterDoc.h"
#include "CNCPlotterView.h"
#include "MainFrm.h"
#include "Global.h"

CPlottingThread::CPlottingThread(void)
		: line_idx(0)
		, line_count(0)
		, step_count(0)
		, startTime(0)
{
	m_pDrawPath = new CPlottingPath();
	m_pPlottedObject = NULL;

	//penDown.CreatePen(0, 1, RGB(0, 0, 255));
	//penUp.CreatePen(0, 1, RGB(255, 0, 0));
}

CPlottingThread::~CPlottingThread(void)
{
	delete m_pDrawPath;

	//penDown.DeleteObject();
	//penUp.DeleteObject();
}

void CPlottingThread::Initialize()
{
	line_idx = 0;

	if(theApp.getPlottedObjectManager())
	{
		m_pPlottedObject = theApp.getPlottedObjectManager()->GetPlottedObject();
		line_count = m_pPlottedObject->GetModeledLinesCount();

		PltLine *line = m_pPlottedObject->GetModeledLinetAt(line_idx);
		m_pDrawPath->SetCurrentPlottingPath(line->x_start, line->y_start, line->x_end, line->y_end, line->isRealPath);
	}
	else 
		line_count = 0;
}

BOOL CPlottingThread::HasNext()		
{
	//daca nu s-a terminat cursorul pe linia curenta sau cursorul pe vectorul de linii, inseamna ca exista urmatorul punct
	if(m_pDrawPath->HasNextStep() || (line_idx+1 < line_count))
	   return TRUE;

	return FALSE;
}

BOOL CPlottingThread::NextStep()	
{
	if(m_pDrawPath->HasNextStep())	//daca exista punct urmator pe linia curenta, deplaseaza
	{
		m_pDrawPath->StepForward();
		return TRUE;
	}
	else   //altfel, treci la linia urmatoare
	{
		if(line_idx+1 < line_count)
		{
			line_idx ++;

			PltLine *line = m_pPlottedObject->GetModeledLinetAt(line_idx);
			m_pDrawPath->SetCurrentPlottingPath(line->x_start, line->y_start, line->x_end, line->y_end, line->isRealPath);

			step_count += m_pDrawPath->GetBresenhamPointsCount();

			CLogger log;
			CString logMsg;
			logMsg.Format("Path #%d: %s (%ld steps)", line_idx, line->isRealPath == TRUE ? "REAL" : "TRANSIENT", m_pDrawPath->GetBresenhamPointsCount());
			log.AppendLogMessageWithTimestamp(logMsg);

			return TRUE;
		}
	}

	return FALSE;
}

CPoint *CPlottingThread::GetCurrentPoint()
{
	return m_pDrawPath->GetCurrentPoint();
}

BOOL CPlottingThread::IsRealPoint()
{
	return m_pDrawPath->IsRealPath();
}
/*
void CPlottingThread::DrawCursor(int orig_x, int orig_y, BOOL isRealPoint, int raza)
{
	if(theApp.m_pMainWnd && ((CMainFrame *)theApp.m_pMainWnd)->GetActiveView() )
	{
		CMainFrame *pframe = (CMainFrame *) theApp.m_pMainWnd;
		CDC *pDC = pframe->GetActiveView()->GetDC();
	
		if(isRealPoint)
			pDC->SelectObject(penDown);
		else 
			pDC->SelectObject(penUp);
	
		pDC->Ellipse(orig_x - raza, orig_y + raza, orig_x + raza, orig_y - raza);
	}
}
*/
void __cdecl CPlottingThread::ThreadPreLoop(void)
{
	Initialize();

	startTime = ::GetTickCount();

	CLogger log;
	CString logMsg;
	
	logMsg.Format("#   %ld paths to be plotted.", line_count);
	log.AppendLogMessageWithTimestamp("Plotting started");
	log.AppendLogMessage(logMsg);

	CMainFrame* pFrame= (CMainFrame *) theApp.m_pMainWnd;
	if(pFrame)
		pFrame->SetStatusBarText("Working...");
};

void __cdecl CPlottingThread::ThreadLoop(void)
{
	while(CBaseThread::IsRunning() && this->HasNext())
	{
		try
		{
			if((theApp.m_pMainWnd)&&((CMainFrame *) theApp.m_pMainWnd)->GetActiveView())
			{
				((CMainFrame *) theApp.m_pMainWnd)->GetActiveView()->Invalidate();
			}

			CString str;
			str.Format("\n## (%d, %d, %d) ##",this->GetCurrentPoint()->x, this->GetCurrentPoint()->y, this->IsRealPoint());
			TRACE(str);
			//DrawCursor(this->GetCurrentPoint()->x, this->GetCurrentPoint()->y, this->IsRealPoint(), DC_CURSOR_PEN_SIZE);
			
			m_pDrawPath->AddPlottedPoint(this->GetCurrentPoint()->x, this->GetCurrentPoint()->y, this->IsRealPoint());


		    this->NextStep();

			Sleep(5);
		}
		catch(const std::exception &e)
		{
			TRACE("%s => Exception: %s", __FUNCTION__, e.what());
			this->PhStop();
		}
		catch(...)
		{
			TRACE("%s => Unknown Exception", __FUNCTION__);
			this->PhStop();
		}
	}
}

void __cdecl CPlottingThread::ThreadPostLoop(void)
{
	CLogger log;
	CString logMsg;
	
	log.AppendLogMessageWithTimestamp("Plotting finished.");
	logMsg.Format("# Process summary:");
	log.AppendLogMessage(logMsg);
	logMsg.Format("#   %ld paths plotted.", line_count);
	log.AppendLogMessage(logMsg);
	logMsg.Format("#   %ld steps.", step_count);
	log.AppendLogMessage(logMsg);
	logMsg.Format("#   Duration: %ld seconds.", (::GetTickCount() - startTime)/1000);
	log.AppendLogMessage(logMsg);

	CMainFrame* pFrame= (CMainFrame *) theApp.m_pMainWnd;
	if(pFrame)
		pFrame->SetStatusBarText("Ready");
};