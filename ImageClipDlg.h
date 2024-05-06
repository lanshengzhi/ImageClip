
// ImageClipDlg.h : header file
//

#pragma once


// CImageClipDlg dialog
class CImageClipDlg : public CDialogEx
{
// Construction
public:
	CImageClipDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGECLIP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSegment();
	afx_msg void OnBnClickedButtonBrowse();

private:
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	void SegmentAndSaveImages(const CString& directoryPath, int segmentHeight, int imgOverlap);

private:
	CStatic m_staticPath;
	CProgressCtrl m_progressCtrl;
	CEdit m_editInfo;
};
