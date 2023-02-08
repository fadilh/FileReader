
// FileEditorDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "FileEditor.h"
#include "FileEditorDlg.h"
#include "afxdialogex.h"
#include "fstream"
#include "string"
using namespace std;

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


// CFileEditorDlg dialog



CFileEditorDlg::CFileEditorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILEEDITOR_DIALOG, pParent)
	, editResult(_T(""))
	, descriptionLabel(_T(""))
{
	lineNumber = 0, lines = 0;
	currentDescription = currentDisplay = _T("");
	readData = false;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, editResult);
	DDX_Text(pDX, IDC_DESCRIPTION, descriptionLabel);
}

BEGIN_MESSAGE_MAP(CFileEditorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CFileEditorDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFileEditorDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CFileEditorDlg::OnBnClickedButton1)
	ON_BN_CLICKED(ReadDataBTN, &CFileEditorDlg::OnBnClickedReaddatabtn)
END_MESSAGE_MAP()


// CFileEditorDlg message handlers

BOOL CFileEditorDlg::OnInitDialog()
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

void CFileEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFileEditorDlg::OnPaint()
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
HCURSOR CFileEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


CString descriptions[50];
CString displays[50];

void CFileEditorDlg::OnEnChangeEdit1()
{
	UpdateData(TRUE);
	displays[lineNumber] = editResult;
}


void CFileEditorDlg::OnBnClickedButton2()
{
	if (lineNumber == 0)
	{
		lineNumber = lines - 1;
	}
	else if (lineNumber > 0) {
		lineNumber--;
	}
	currentDescription = descriptions[lineNumber];
	currentDisplay = displays[lineNumber];
	descriptionLabel = currentDescription;
	editResult = currentDisplay;
	UpdateData(FALSE);
}


void CFileEditorDlg::OnBnClickedButton1()
{
	if (lineNumber == lines - 1)
	{
		lineNumber = 0;
	}
	else if (lineNumber < lines - 1) {
		lineNumber++;
	}
	currentDescription = descriptions[lineNumber];
	currentDisplay = displays[lineNumber];
	descriptionLabel = currentDescription;
	editResult = currentDisplay;
	UpdateData(FALSE);
}


void CFileEditorDlg::OnBnClickedReaddatabtn()
{
	if (!readData)
	{
		ifstream inputFile;
		string currentLine;
		inputFile.open("..\\..\\..\\..\\Downloads\\FileToRead.txt");
		while (inputFile.is_open())
		{
			while (getline(inputFile, currentLine)) {

				size_t descStart = currentLine.find("\"");
				string temp = currentLine.substr(descStart + 1);
				size_t descEnd = temp.find("\"");
				string description = temp.substr(0, descEnd);
				descriptions[lines] = description.c_str();
				temp = temp.substr(descEnd + 1);

				size_t displayStart = temp.find("\"");
				temp = temp.substr(displayStart + 1);
				size_t displayEnd = temp.find("\"");
				string display = temp.substr(0, displayEnd);
				displays[lines] = display.c_str();
				lines++;
			}
			inputFile.close();
		}
		currentDescription = descriptions[0];
		currentDisplay = displays[0];
		descriptionLabel = currentDescription;
		editResult = currentDisplay;
		readData = true;
	}
	UpdateData(FALSE);
}

