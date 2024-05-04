
// ImageClipDlg.cpp : implementation file
//
#include "pch.h"
#include "framework.h"
#include "ImageClip.h"
#include "ImageClipDlg.h"
#include "afxdialogex.h"

#include <gdiplus.h>
#include <filesystem>
#include <iostream>
#include <memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CImageClipDlg dialog



CImageClipDlg::CImageClipDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMAGECLIP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageClipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PATH, m_staticPath);
}

BEGIN_MESSAGE_MAP(CImageClipDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEGMENT, &CImageClipDlg::OnBnClickedButtonSegment)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CImageClipDlg::OnBnClickedButtonBrowse)
END_MESSAGE_MAP()


// CImageClipDlg message handlers

BOOL CImageClipDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CImageClipDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CImageClipDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CImageClipDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CImageClipDlg::OnBnClickedButtonSegment()
{
	// TODO: Add your control notification handler code here
	CString path;
	m_staticPath.GetWindowTextW(path);
	SegmentAndSaveImages(path, 3300, 80);
}


void CImageClipDlg::OnBnClickedButtonBrowse()
{
	// TODO: Add your control notification handler code here
	// 创建并初始化文件夹选择对话框
	CFolderPickerDialog folderPickerDialog(NULL, 0, this, sizeof(OPENFILENAME));

	// 显示对话框并检查用户的操作
	if (folderPickerDialog.DoModal() == IDOK)
	{
		// 获取选中的文件夹路径
		CString folderPath = folderPickerDialog.GetFolderPath();

		// 使用选中的文件夹路径
		m_staticPath.SetWindowTextW(folderPath.GetString());
	}
}

// Reference code : https://learn.microsoft.com/en-us/windows/win32/gdiplus/-gdiplus-retrieving-the-class-identifier-for-an-encoder-use
int CImageClipDlg::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

void CImageClipDlg::SegmentAndSaveImages(const CString& directoryPath, int segmentHeight, int imgOverlap) {

	std::filesystem::path inputDir { directoryPath.GetString() };
	std::filesystem::path outputDir { inputDir / "clips" }; // Create the output directory path 

	
	if (std::filesystem::exists(outputDir)) {
		std::filesystem::remove_all(outputDir);
	}

	// Create the output directory, ignore if it already exists
	std::filesystem::create_directories(outputDir);

	// Traverse all files in the input directory
	for (const auto& entry : std::filesystem::directory_iterator(inputDir)) {
		const auto& filePath = entry.path();

		if (entry.is_directory()) {	// Ensure it is a file
			continue;
		}
		
		auto fileExtension = filePath.extension(); // Get file extension

		// Load the image
		std::unique_ptr<Gdiplus::Bitmap> image(new Gdiplus::Bitmap(filePath.c_str()));

		if (image->GetLastStatus() != Gdiplus::Ok) {
			std::wcout << L"Failed to load image: " << filePath << std::endl;
			continue; // Skip if load fails
		}

		int width = image->GetWidth();
		int height = image->GetHeight();
		int currentTop = 0;
		int segmentId = 0;  // Image segment index

		while (currentTop < height) {
			int currentBottom = min(currentTop + segmentHeight, height);
			Gdiplus::Rect rect(0, currentTop, width, currentBottom - currentTop);

			// Create a new bitmap to save the clipped image
			std::unique_ptr<Gdiplus::Bitmap> newImg(new Gdiplus::Bitmap(width, rect.Height, PixelFormat24bppRGB));
			Gdiplus::Graphics graphics(newImg.get());
			graphics.DrawImage(image.get(), 0, 0, rect.X, rect.Y, rect.Width, rect.Height, Gdiplus::UnitPixel);

			// Build the new file path
			std::filesystem::path newFileName = outputDir / filePath.filename();
			newFileName.replace_filename(filePath.stem().wstring() + L"_" + std::to_wstring(segmentId++) + fileExtension.wstring());

			// Save the image
			CLSID clsid;
			GetEncoderClsid(L"image/jpeg", &clsid);

			newImg->Save(newFileName.c_str(), &clsid, NULL);

			currentTop += segmentHeight - imgOverlap; // Reduce overlap by 80 pixels
		}
	}
}
