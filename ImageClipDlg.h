
// ImageClipDlg.h : header file
//

#pragma once

#include <filesystem>
#include <functional>
#include <string>

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
	int GetEncoderClsid(const std::wstring& format, CLSID& clsid);
	void UpateImageDirectory(CString &imgSrcDir);
	void SegmentAndSaveImages(std::filesystem::path &imgSrcDir, std::filesystem::path &imgDstDir, std::function<void(int)> updateProgress);

private:
	CStatic m_staticPath;
	CProgressCtrl m_progressCtrl;
	CEdit m_editInfo;
	std::filesystem::path m_imageSrcDir;	// Image source directory, initialized in OnBnClickedButtonBrowse()
	std::filesystem::path m_imageDstDir;	// Image destination directory

	static constexpr int IMAGE_OVERLAP = 80;	
	static constexpr int SEGMENT_HEIGHT = 3400;
};
