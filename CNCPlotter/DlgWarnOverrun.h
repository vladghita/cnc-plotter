#pragma once


// CDlgWarnOverrun dialog

class CDlgWarnOverrun : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgWarnOverrun)

public:
	CDlgWarnOverrun(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgWarnOverrun();

// Dialog Data
	enum { IDD = IDD_WARN_OVERRUN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
