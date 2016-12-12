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

// CNCPlotterView.cpp : implementation of the CCNCPlotterView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CNCPlotter.h"
#endif

#include "CNCPlotterDoc.h"
#include "CNCPlotterView.h"
#include "PlottedObject.h"
#include "MainFrm.h"

#include "Global.h"
#include "VLGraphicsHeader.h"
#include "GraphicUtils.h"
#include <vector>
#include "PlottingThread.h"
#include "DlgWarnOverrun.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCNCPlotterView

IMPLEMENT_DYNCREATE(CCNCPlotterView, CView)

BEGIN_MESSAGE_MAP(CCNCPlotterView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CCNCPlotterView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_BUTTON2, &CCNCPlotterView::OnButton2)
	ON_COMMAND(ID_BUTTON3, &CCNCPlotterView::OnButton3)
	ON_COMMAND(ID_SLIDER2, &CCNCPlotterView::OnSlider2)
	ON_COMMAND(ID_BUTTON4, &CCNCPlotterView::OnButton4)
	ON_COMMAND(ID_BUTTON5, &CCNCPlotterView::OnButton5)
	ON_COMMAND(ID_BUTTON6, &CCNCPlotterView::OnButton6)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CCNCPlotterView construction/destruction

CCNCPlotterView::CCNCPlotterView()
{
	// TODO: add construction code here
	lastSliderPos = -1;

	penDown.CreatePen(0, 1, RGB(0, 0, 255));
	penUp.CreatePen(0, 1, RGB(255, 0, 0));
}

CCNCPlotterView::~CCNCPlotterView()
{
	penDown.DeleteObject();
	penUp.DeleteObject();
}

void CCNCPlotterView::lineBresenham(CDC *pDC, int p1x, int p1y, int p2x, int p2y)
{
   int F, x, y;

   if (p1x > p2x)  // Swap points if p1 is on the right of p2
   {
       swap(p1x, p2x); 
       swap(p1y, p2y);
   }

   // Handle trivial cases separately for algorithm speed up.
   // Trivial case 1: m = +/-INF (Vertical line)
   if (p1x == p2x)
   {
       if (p1y > p2y)  // Swap y-coordinates if p1 is above p2
       {
           swap(p1y, p2y);
       }

       x = p1x;
       y = p1y;
       while (y <= p2y)
       {
           setPixel(pDC, x, y);
           y++;
       }
       return;
   }
   // Trivial case 2: m = 0 (Horizontal line)
   else if (p1y == p2y)
   {
       x = p1x;
       y = p1y;

       while (x <= p2x)
       {
           setPixel(pDC, x, y);
           x++;
       }
       return;
   }


   int dy            = p2y - p1y;  // y-increment from p1 to p2
   int dx            = p2x - p1x;  // x-increment from p1 to p2
   int dy2           = (dy << 1);  // dy << 1 == 2*dy
   int dx2           = (dx << 1);
   int dy2_minus_dx2 = dy2 - dx2;  // precompute constant for speed up
   int dy2_plus_dx2  = dy2 + dx2;


   if (dy >= 0)    // m >= 0
   {
       // Case 1: 0 <= m <= 1 (Original case)
       if (dy <= dx)   
       {
           F = dy2 - dx;    // initial F

           x = p1x;
           y = p1y;
           while (x <= p2x)
           {
               setPixel(pDC, x, y);
               if (F <= 0)
               {
                   F += dy2;
               }
               else
               {
                   y++;
                   F += dy2_minus_dx2;
               }
               x++;
           }
       }
       // Case 2: 1 < m < INF (Mirror about y=x line
       // replace all dy by dx and dx by dy)
       else
       {
           F = dx2 - dy;    // initial F

           y = p1y;
           x = p1x;
           while (y <= p2y)
           {
               setPixel(pDC, x, y);
               if (F <= 0)
               {
                   F += dx2;
                }
                else
                {
                    x++;
                    F -= dy2_minus_dx2;
                }
                y++;
            }
        }
    }
    else    // m < 0
    {
        // Case 3: -1 <= m < 0 (Mirror about x-axis, replace all dy by -dy)
        if (dx >= -dy)
        {
            F = -dy2 - dx;    // initial F

            x = p1x;
            y = p1y;
            while (x <= p2x)
            {
                setPixel(pDC, x, y);
                if (F <= 0)
                {
                    F -= dy2;
                }
                else
                {
                    y--;
                    F -= dy2_plus_dx2;
                }
                x++;
            }
        }
        // Case 4: -INF < m < -1 (Mirror about x-axis and mirror 
        // about y=x line, replace all dx by -dy and dy by dx)
        else    
        {
            F = dx2 + dy;    // initial F

            y = p1y;
            x = p1x;
            while (y >= p2y)
            {
                setPixel(pDC, x, y);
                if (F <= 0)
                {
                    F += dx2;
                }
                else
                {
                    x++;
                    F += dy2_plus_dx2;
                }
                y--;
            }
        }
    }
}

void CCNCPlotterView::setPixel(CDC* pDC, int x, int y)
{
	pDC->SetPixel(x, y, RGB(255, 0, 0));
	TRACE("\n %d, %d", x, y);
}

void CCNCPlotterView::swap(int &a, int &b)
{
	int c = a;
	a = b;
	b = c;
}


BOOL CCNCPlotterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CCNCPlotterView::drawBackground(CDC *pDC)
{
   COLORREF color = ::GetSysColor(DC_BACKGROUND_COLOR);

   CBrush brushBlue(color);
   CBrush* pOldBrush = pDC->SelectObject(&brushBlue);

   //draw left stripe
   POINT top_left; 
   top_left.x = 0; top_left.y = 0;
   POINT bottom_right; 
   bottom_right.x = VP_AREA_LEFT; bottom_right.y = 2 * WND_SCROLL_SIZE;
   CRect rect(top_left, bottom_right);
   pDC->FillRect(rect, &brushBlue);

   //draw right stripe
   top_left.x = VP_AREA_RIGHT+1; top_left.y = 0;
   bottom_right.x = 2 * WND_SCROLL_SIZE; bottom_right.y = 2 * WND_SCROLL_SIZE;
   rect.SetRect(top_left, bottom_right);
   pDC->FillRect(rect, &brushBlue);

   //draw top stripe
   top_left.x = 0; top_left.y = 0;
   bottom_right.x = 2 * WND_SCROLL_SIZE; bottom_right.y = VP_AREA_BOTTOM;
   rect.SetRect(top_left, bottom_right);
   pDC->FillRect(rect, &brushBlue);

   //draw bottom stripe
   top_left.x = 0; top_left.y = VP_AREA_TOP+1;
   bottom_right.x = 2 * WND_SCROLL_SIZE; bottom_right.y = 2 * WND_SCROLL_SIZE;
   rect.SetRect(top_left, bottom_right);
   pDC->FillRect(rect, &brushBlue);

   pDC->SelectObject(pOldBrush);
}

void CCNCPlotterView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = WND_SCROLL_SIZE;

	initWindowAndViewport();

	CMainFrame* pFrame= (CMainFrame *) theApp.m_pMainWnd;
	if(pFrame)
		pFrame->SetStatusBarText("Ready");
}


// CCNCPlotterView drawing

void CCNCPlotterView::OnDraw(CDC* dc)
{
	CMyMemDC pDC(dc);

	CCNCPlotterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

   CPen pen(0, DC_PEN_WIDTH, DC_PEN_COLOR);
   pDC->SelectObject(pen);

   drawBackground(pDC);
   vlg_Vizualizare(pDC);
   drawBackground(pDC);

   if(theApp.getPlottingThread() && theApp.getPlottingThread()->GetDrawPath())
   {
	   for(int i = 0; i < theApp.getPlottingThread()->GetDrawPath()->GetPlottedPointsCount(); i++)
	   {
		   PltPoint point;
		   theApp.getPlottingThread()->GetDrawPath()->GetPlottedPointAt(i, &point);
		   DrawCursor(pDC, point.xCoord, point.yCoord, point.isPenDown, DC_CURSOR_PEN_SIZE);
	   }
   }

   //ReleaseDC(pDC);
   //pen.DeleteObject();
}


// CCNCPlotterView printing


void CCNCPlotterView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CCNCPlotterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCNCPlotterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCNCPlotterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CCNCPlotterView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCNCPlotterView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCNCPlotterView diagnostics

#ifdef _DEBUG
void CCNCPlotterView::AssertValid() const
{
	CView::AssertValid();
}

void CCNCPlotterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCNCPlotterDoc* CCNCPlotterView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCNCPlotterDoc)));
	return (CCNCPlotterDoc*)m_pDocument;
}
#endif //_DEBUG


// CCNCPlotterView message handlers


void CCNCPlotterView::DrawCursor(CDC *pDC, int orig_x, int orig_y, BOOL isRealPoint, int raza)
{
	if(isRealPoint)
		pDC->SelectObject(penDown);
	else 
		pDC->SelectObject(penUp);
	
	pDC->Ellipse(orig_x - raza, orig_y + raza, orig_x + raza, orig_y - raza);
}

void CCNCPlotterView::initVars()
{
	CCNCPlotterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//init slider
	if(lastSliderPos == -1)
		lastSliderPos = GetSliderPos();

	//init window, viewport, wired points etc
	if(theApp.getPlottedObjectManager() && theApp.getPlottedObjectManager()->GetPlottedObject()){
		CPlottedObject *pPlottedObject = theApp.getPlottedObjectManager()->GetPlottedObject();
		if(pPlottedObject && pPlottedObject->GetPrimitivePointsCount() > 0)
		{
			std::vector<CPoint> vPuncte;
			pPlottedObject->GetPrimitivePoints(vPuncte);
			
			std::vector<CPoint> vLinii = *pPlottedObject->GetWiredPoints();
			
			vlg_CitesteVector(vPuncte, vLinii);

			//initWindowAndViewport();
		
			CDC *pDC=GetDC();
			vlg_Vizualizare(pDC);
			ReleaseDC(pDC);
		}
	}
}

void CCNCPlotterView::initWindowAndViewport() //initializare fereastra si viewport
{
    vlg_LimiteWindow lw;
    lw.stanga = WND_AREA_LEFT;
    lw.jos = WND_AREA_BOTTOM;
    lw.dreapta = WND_AREA_RIGHT;
    lw.sus = WND_AREA_TOP;
    
    vlg_LimiteViewport lvp;
    lvp.left = VP_AREA_LEFT;
    lvp.down = VP_AREA_BOTTOM;
    lvp.right = VP_AREA_RIGHT;
    lvp.up = VP_AREA_TOP;
    
    vlg_SetCurrentWindow(lw.stanga, lw.dreapta, lw.jos, lw.sus);
    vlg_SetCurrentViewport(lvp.left, lvp.right, lvp.down, lvp.up);
    
    vlg_width = 0;
    vlg_height = 0;
}

void CCNCPlotterView::fitModelToScreen()
{
	CCNCPlotterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	if(theApp.getPlottedObjectManager() && theApp.getPlottedObjectManager()->GetPlottedObject())
	{
		CDC *pDC=GetDC();

		CPlottedObject *pPlottedObject = theApp.getPlottedObjectManager()->GetPlottedObject();

		CRect rect;
		theApp.getPlottedObjectManager()->GetBoundingBoxOfPrimitivePoints(rect);
		//calculez coordonatele punctului central al rect-ului care inconjoara modelul
		int offset_model_x = (rect.right - rect.left)/2 + rect.left;
		int offset_model_y = (rect.top - rect.bottom)/2 + rect.bottom;
		int offset_vp_x = (VP_AREA_RIGHT - VP_AREA_LEFT)/2 + VP_AREA_LEFT;
		int offset_vp_y = (VP_AREA_TOP - VP_AREA_BOTTOM)/2 + VP_AREA_BOTTOM;
		
		//calculez raportul intre minimele laturilor (in caz ca folosim dreptunghi si nu patrat)
		int latura_model = max(rect.right - rect.left, rect. top - rect. bottom);
		int latura_viewport = max(VP_AREA_TOP - VP_AREA_BOTTOM, VP_AREA_RIGHT - VP_AREA_LEFT);
		float scale_factor = (float)1/(latura_model/latura_viewport);
		
	
		Invalidate();
		//vlg_Zoom1(z.cx, z.cy, z.fs, pDC);
		vlg_Translatia(-offset_model_x, -offset_model_y, 0, pDC);
		vlg_Scalarea(scale_factor, scale_factor, scale_factor, pDC);
		vlg_Translatia(offset_vp_x, offset_vp_y, 0, pDC);
	
		ReleaseDC(pDC);
	}
}

void CCNCPlotterView::initDrawing()
{
	initVars();
	fitModelToScreen();
}

//move left
void CCNCPlotterView::OnButton2()
{
	CDC *pDC=GetDC();

	vlg_Translatie t;
	t.tx = -RIBBON_TRANSLATE_VAL;
	t.ty = 0;
	t.tz = 0;

	Invalidate();
	vlg_Translatia(t.tx, t.ty, t.tz, pDC);
	ReleaseDC(pDC);
}

//move right
void CCNCPlotterView::OnButton3()
{
	CDC *pDC=GetDC();

	vlg_Translatie t;
	t.tx = RIBBON_TRANSLATE_VAL;
	t.ty = 0;
	t.tz = 0;

	Invalidate();
	vlg_Translatia(t.tx, t.ty, t.tz, pDC);
	ReleaseDC(pDC);
}

//move up
void CCNCPlotterView::OnButton4()
{
	CDC *pDC=GetDC();

	vlg_Translatie t;
	t.tx = 0;
	t.ty = RIBBON_TRANSLATE_VAL;
	t.tz = 0;

	Invalidate();
	vlg_Translatia(t.tx, t.ty, t.tz, pDC);
	ReleaseDC(pDC);
}

//move down
void CCNCPlotterView::OnButton5()
{
	CDC *pDC=GetDC();

	vlg_Translatie t;
	t.tx = 0;
	t.ty = -RIBBON_TRANSLATE_VAL;
	t.tz = 0;

	Invalidate();
	vlg_Translatia(t.tx, t.ty, t.tz, pDC);
	ReleaseDC(pDC);
}

int CCNCPlotterView::GetSliderPos()
{
    // Get a pointer to the ribbon bar
    CMFCRibbonBar* pRibbon = ((CFrameWndEx*) AfxGetMainWnd())->GetRibbonBar();
    ASSERT_VALID(pRibbon);

    CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, pRibbon->FindByID(ID_SLIDER2));
    // Get current position
    return pSlider->GetPos();
}

void CCNCPlotterView::OnSlider2()
{
	CCNCPlotterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	int dir = MODEL_NO_ZOOM;
	float factor = 0;
	if(GetSliderPos() > lastSliderPos){
		dir = MODEL_ZOOM_IN;
		factor = (float) pow(RIBBON_ZOOM_FACTOR, GetSliderPos() - lastSliderPos);
	}
	else if(GetSliderPos() < lastSliderPos){
		dir = MODEL_ZOOM_OUT;
		factor = (float)1/pow(RIBBON_ZOOM_FACTOR, lastSliderPos - GetSliderPos());
	}
	lastSliderPos = GetSliderPos();

	if(dir != MODEL_NO_ZOOM)
	{
	
		CString str; str.Format("\ndir: %d, factor: %f", dir, factor);
		TRACE(str);
	
		CDC *pDC=GetDC();
	
		//scalarea se face ca procent din unitate. Ex: ptr marire model cu 0.2 se foloseste factor de scalare 1.2, pentru
		//micsorare model cu 0.01 se foloseste factor de scalare 0.99
		vlg_Scalare s;
		s.fsx = s.fsy = s.fsz = factor;
		
		Invalidate();
		vlg_Scalarea(s.fsx, s.fsy, s.fsz, pDC);
		ReleaseDC(pDC);
	
		pDoc->UpdateAllViews(NULL);
	}
}

//muta sliderul la stanga sau dreapta in mod programatic
void CCNCPlotterView::moveZoomSlider(int zoom_type)
{
	if(zoom_type == MODEL_ZOOM_IN)
	{
		CMFCRibbonBar* pRibbon = ((CFrameWndEx*) AfxGetMainWnd())->GetRibbonBar();
		ASSERT_VALID(pRibbon);
		CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, pRibbon->FindByID(ID_SLIDER2));
		if(pSlider->GetPos()+1 < pSlider->GetRangeMax())
			pSlider->SetPos(pSlider->GetPos()+1);
	}
	else if(zoom_type == MODEL_ZOOM_OUT)
	{
		CMFCRibbonBar* pRibbon = ((CFrameWndEx*) AfxGetMainWnd())->GetRibbonBar();
		ASSERT_VALID(pRibbon);
		CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, pRibbon->FindByID(ID_SLIDER2));
		if(pSlider->GetPos()-1 > pSlider->GetRangeMin())
			pSlider->SetPos(pSlider->GetPos()-1);
	}

	OnSlider2();
}

BOOL CCNCPlotterView::PreTranslateMessage(MSG* pMsg)
{
	if ( !pMsg )
		return FALSE;
	BOOL altDown = GetKeyState( VK_MENU ) & 0x1;

	switch( pMsg->message )
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
		{
			if ( pMsg->wParam == VK_DOWN ){
				OnButton5();
				return 1;	// supress further processing
			}
			else if ( pMsg->wParam == VK_UP ){
				OnButton4();
				return 1;	// supress further processing
			}
			else if ( pMsg->wParam == VK_LEFT ){
				OnButton2();
				return 1;	// supress further processing
			}
			else if ( pMsg->wParam == VK_RIGHT ){
				OnButton3();
				return 1;	// supress further processing
			}
		}break;
	case WM_SYSKEYDOWN:		//ALT keys
	case WM_SYSKEYUP:
		{
			if ( pMsg->wParam == VK_UP ){
				moveZoomSlider(ZOOM_IN);

				return 1;	// supress further processing
			}
			else if ( pMsg->wParam == VK_DOWN ){
				moveZoomSlider(ZOOM_OUT);

				return 1;	// supress further processing
			}
		}break;
	default:
		return CView::PreTranslateMessage( pMsg );
	};

	return 0;
}


void CCNCPlotterView::OnButton6()
{
	if(theApp.getPlottedObjectManager() && theApp.getPlottedObjectManager()->GetPlottedObject())
	{
		theApp.getPlottedObjectManager()->ComputeModeledLinesFromGraphics();
		
		BOOL is_inside_vp = theApp.getPlottedObjectManager()->IsBoundingBoxOfModeledPointsInsideViewport();
		if(!is_inside_vp) 
		{
			//::MessageBoxA(this->GetSafeHwnd(), "The modeled object exceeds the plotting area.\n Translate/scale the model in order to be fully contained\n within the plotting surface.", "WARNING", MB_OK | MB_ICONWARNING);
			CDlgWarnOverrun dlg;
			MessageBeep(MB_OK);
			dlg.DoModal();

			return;
		}
	}

	if ( theApp.getPlottingThread() )
		theApp.getPlottingThread()->StartThread();
}


BOOL CCNCPlotterView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return FALSE;
}
