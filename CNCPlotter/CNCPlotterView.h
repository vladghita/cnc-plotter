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

// CNCPlotterView.h : interface of the CCNCPlotterView class
//

#if !defined(__CNC_PLOTTER_VIEW__)
#define __CNC_PLOTTER_VIEW__


class CCNCPlotterView : public CView
{
protected: // create from serialization only
	CCNCPlotterView();
	DECLARE_DYNCREATE(CCNCPlotterView)
	virtual void OnInitialUpdate();

// Attributes
public:
	CCNCPlotterDoc* GetDocument() const;
	int lastSliderPos;

private:
	CPen penDown, penUp;

// Operations
public:
	void initVars();         //initializare variabile
	void initDrawing();      //initializare desen
	void initWindowAndViewport(); //initializare fereastra si viewport
	void fitModelToScreen(); //incadreaza modelul in viewport (prin operatii de scalare, translatie etc)
	void drawBackground(CDC *pDC);   //desenare background viewport
	void moveZoomSlider(int zoom_type);   //pentru a muta sliderul din taste

	void DrawCursor(CDC *pDC, int orig_x, int orig_y, BOOL isRealPoint, int raza);

	//PTR TESTE DESENARE LINIE ////////////////
	void lineBresenham(CDC *pDC, int p1x, int p1y, int p2x, int p2y);
	void setPixel(CDC* pDC, int x, int y);
	void swap(int &a, int &b);
	///////////////////////////////////////////

	int GetSliderPos();					//get the current position of slider

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CCNCPlotterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnSlider2();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnButton6();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in CNCPlotterView.cpp
inline CCNCPlotterDoc* CCNCPlotterView::GetDocument() const
   { return reinterpret_cast<CCNCPlotterDoc*>(m_pDocument); }
#endif

#endif /*__CNC_PLOTTER_VIEW__*/