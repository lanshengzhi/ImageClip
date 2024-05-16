
// ImageClipDlg.cpp : implementation file
//
#include "pch.h"
#include "framework.h"
#include "ImageClip.h"
#include "ImageClipDlg.h"
#include "afxdialogex.h"

#include <gdiplus.h>
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
	DDX_Control(pDX, IDC_PROGRESS_SEGMENT, m_progressCtrl);
	DDX_Control(pDX, IDC_EDIT_INFO, m_editInfo);
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
	m_progressCtrl.SetRange(0, 100);
	m_progressCtrl.SetPos(0);
	m_editInfo.SetWindowTextW(_T("����ѡ��ͼƬ�����ļ���"));

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

	if (std::filesystem::exists(m_imageSrcDir) 
		&& std::filesystem::is_directory(m_imageSrcDir)) {
		
		m_editInfo.SetWindowTextW(_T("�ü���"));

		if (std::filesystem::exists(m_imageDstDir)) {
			std::filesystem::remove_all(m_imageDstDir);
		}

		// Create the output directory
		std::filesystem::create_directories(m_imageDstDir);

		UpdateWindow();	// Show prev windows text

		SegmentAndSaveImages(m_imageSrcDir, m_imageDstDir, [this](int progress) {m_progressCtrl.SetPos(progress); });

		std::wstring message{ _T("�ü��������ü�ͼƬ����Ŀ¼��") + m_imageDstDir.wstring()};
		m_editInfo.SetWindowTextW(message.c_str());
	} 
	else {
		m_editInfo.SetWindowTextW(_T("ѡ���ͼƬ�ļ��в����ڣ���ѡ��ͼƬ�ļ��С�"));
	}
}


void CImageClipDlg::OnBnClickedButtonBrowse()
{
	// TODO: Add your control notification handler code here
	// Create and initialize the folder picker dialog
	CFolderPickerDialog folderPickerDialog(NULL, 0, this, sizeof(OPENFILENAME));

	// Display the dialog and check the user's action
	if (folderPickerDialog.DoModal() == IDOK) {
		// Retrieve the selected folder path
		CString folderPath = folderPickerDialog.GetFolderPath();

		// Use the selected folder path
		m_staticPath.SetWindowTextW(folderPath.GetString());
		m_editInfo.SetWindowTextW(_T("��һ�����Կ�ʼ�ü�"));

		UpateImageDirectory(folderPath);
	}
	else {
		m_editInfo.SetWindowTextW(_T("δѡ��ͼƬ�ļ��У���ѡ��ͼƬ�����ļ��С�"));
	}
}

void CImageClipDlg::UpateImageDirectory(CString& imgSrcDir)
{
	m_imageSrcDir = imgSrcDir.GetString();
	m_imageDstDir = m_imageSrcDir / "Clips";
}

// Reference code : https://learn.microsoft.com/en-us/windows/win32/gdiplus/-gdiplus-retrieving-the-class-identifier-for-an-encoder-use
int CImageClipDlg::GetEncoderClsid(const std::wstring& format, CLSID& clsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	Gdiplus::Status stat = Gdiplus::GetImageEncodersSize(&num, &size);
	if (stat != Gdiplus::Ok || size == 0 || num == 0) {
		return -1;  // Failure
	}

	Gdiplus::ImageCodecInfo* pImageCodecInfo = (Gdiplus::ImageCodecInfo*)malloc(size);
	if (pImageCodecInfo == nullptr) {
		return -1;  // Failure
	}

	if (Gdiplus::Ok != Gdiplus::GetImageEncoders(num, size, pImageCodecInfo)) {
		free(pImageCodecInfo);
		return -1;
	}

	for (UINT j = 0; j < num; ++j) {
		if (std::wstring(pImageCodecInfo[j].MimeType) == format) {
			clsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

void CImageClipDlg::SegmentAndSaveImages(std::filesystem::path &imgSrcDir, std::filesystem::path &imgDstDir, std::function<void(int)> updateProgress)
{
	CLSID clsid;
	if (GetEncoderClsid(L"image/jpeg", clsid) < 0) {
		std::wcout << L"Get jpeg encoder fail" << std::endl;
		return;
	}

	int totalFiles = std::distance(std::filesystem::directory_iterator(imgSrcDir), std::filesystem::directory_iterator{});
	int processedFiles = 0;

	// Traverse all files in the input directory
	for (const auto& entry : std::filesystem::directory_iterator(imgSrcDir)) {
		const auto& filePath = entry.path();

		if (entry.is_directory()) {	// Ensure it is a file
			--totalFiles;
			continue;
		}
		
		auto fileExtension = filePath.extension(); // Get file extension

		// Load the image
		std::unique_ptr<Gdiplus::Bitmap> image(new Gdiplus::Bitmap(filePath.c_str()));

		if (image->GetLastStatus() != Gdiplus::Ok) {
			std::wcout << L"Failed to load image: " << filePath << std::endl;
			--totalFiles;
			continue; // Skip if load fails, not image file
		}

		int width = image->GetWidth();
		int height = image->GetHeight();
		int currentTop = 0;
		int segmentId = 0;  // Image segment index

		while (currentTop < height) {
			int currentBottom = min(currentTop + SEGMENT_HEIGHT, height);
			Gdiplus::Rect rect(0, currentTop, width, currentBottom - currentTop);

			// Create a new bitmap to save the clipped image
			std::unique_ptr<Gdiplus::Bitmap> newImg(new Gdiplus::Bitmap(width, rect.Height, PixelFormat24bppRGB));
			Gdiplus::Graphics graphics(newImg.get());
			graphics.DrawImage(image.get(), 0, 0, rect.X, rect.Y, rect.Width, rect.Height, Gdiplus::UnitPixel);

			// Build the new file path
			std::filesystem::path newFileName = imgDstDir / filePath.filename();
			newFileName.replace_filename(filePath.stem().wstring() + L"_" + std::to_wstring(segmentId++) + fileExtension.wstring());

			// Save the image
			newImg->Save(newFileName.c_str(), &clsid, nullptr);

			currentTop += SEGMENT_HEIGHT - IMAGE_OVERLAP; // Reduce overlap by imgOverlap pixels
		}

		++processedFiles;
		int progress = static_cast<int>(100.0 * processedFiles / totalFiles);
		updateProgress(progress);
	}
}
