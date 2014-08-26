
// testDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "test.h"
#include "testDlg.h"
#include "afxdialogex.h"
#include <shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")
#include <windows.h>  
using namespace std; 


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestDlg 对话框




CtestDlg::CtestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CtestDlg::IDD, pParent)
	, m_Watermark(_T(""))
	, m_Src(_T(""))
	, m_Screen(_T(""))
	, m_SavePath(_T(""))
	, m_Ext(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WM, m_Watermark);
	DDX_Text(pDX, IDC_EDIT_VIDEO, m_Src);
	DDX_Text(pDX, IDC_EDIT_SCREEN, m_Screen);
	DDX_Text(pDX, IDC_EDIT_SAVE, m_SavePath);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
}

BEGIN_MESSAGE_MAP(CtestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_GENERATE_AUTO, &CtestDlg::OnBnClickedButtonGenerateAuto)
	ON_BN_CLICKED(IDC_BUTTON_SCAN, &CtestDlg::OnBnClickedButtonScan)
	ON_BN_CLICKED(IDOK, &CtestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CtestDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CtestDlg::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CtestDlg 消息处理程序

BOOL CtestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	((CEdit *)GetDlgItem(IDC_EDIT_WM))->SetLimitText(6);
	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CtestDlg::OnBnClickedButtonGenerateAuto()//生成水印--完成
{
	// 自动生成水印按钮
	bool flag = false;
	UpdateData(true);
	flag = GenerateAuto(m_Watermark,m_Watermark_en);
	AfxMessageBox(m_Watermark_en);
	if (flag == false)
	{
		AfxMessageBox(_T("水印不合法！"));
		return;
	}

	UpdateData(false);
}


void CtestDlg::OnBnClickedButtonScan()
{
	// 打开视频文件按钮
	CFileDialog pCFileDialog(true,NULL,NULL,0,_T("(*.yuv)|*.yuv|(*.mpg)|*.mpg|(*.vob)|*.vob|(*.m2v)|*.m2v||"));
	if(pCFileDialog.DoModal()!=IDOK) 
	{ 
		return; 
	}
	m_Src = pCFileDialog.GetPathName();
	m_Ext = pCFileDialog.GetFileExt();
	UpdateData(false);
}


void CtestDlg::OnBnClickedButtonSave()
{
	// 设置保存生成视频路径
	CFileDialog pCFileDialog(false,m_Ext,NULL,0,_T("(*.yuv)|*.yuv|(*.mpg)|*.mpg|(*.vob)|*.vob|(*.m2v)|*.m2v||"));
	if(pCFileDialog.DoModal()!=IDOK) 
	{ 
		return; 
	}

	m_SavePath = pCFileDialog.GetPathName();
	UpdateData(false);
}


void CtestDlg::OnBnClickedOk()
{
	UpdateData(true);
	// TODO: 执行嵌入的必要操作

	// 判断水印是否合法
	if (!WatermarkCheck( m_Watermark ))
	{
		AfxMessageBox(_T("水印不合法！"));
		return;
	}
	m_Screen += _T("水印检验通过！\r\n");
	UpdateData(false);
	// TODO：判断视频格式是否合法
	bool IfFormat_legal;
	IfFormat_legal= JudgeFormat(m_Src);
	if (!IfFormat_legal)
	{
		AfxMessageBox(_T("待嵌入视频格式不合法"));
	}
	// TODO：判断视频是否存在
	if (!PathFileExists(m_Src))
	{
		AfxMessageBox(_T("待嵌入视频不存在"));//不存在
	}
	// TODO：判断视频保存格式是否合法
	
	IfFormat_legal = JudgeFormat(m_SavePath);
	if (!IfFormat_legal)
	{
		AfxMessageBox(_T("视频保存格式不合法"));
	}
	// TODO：判断视频保存路径所在磁盘空间是否够大
	if(!StorageSpace(m_SavePath))
	{
		AfxMessageBox(_T("视频保存路径所在磁盘空间小于2G"));
	}
	// TODO：判断视频保存路径是否存在，没有则新建一个
	if(!PathIsDirectory(m_SavePath))
	{
		CreateDirectory(m_SavePath,NULL) ;//不存在就在目标路径上创建一个文件夹
	}
	
	// TODO：执行嵌入，若不成功则删除残留文件

	Format.m_Src = m_Src;
	Format.m_SavePath = m_SavePath;
	m_Screen += _T("执行音频分离…\r\n");
	UpdateData(false);
	if (!Format.AudioSeparation())
	{
		AfxMessageBox(_T("AudioSeparation出错！"));
		m_Screen += _T("程序正在回滚…\r\n");
		UpdateData(false);
		Format.DelectAll();
	}

	m_Screen += _T("执行视频格式转换…\r\n");
	UpdateData(false);
	if(IfNeedChangeFormat(m_Src))//判断是否需要准换格式
	{
		if (!Format.Video2YUV())
		{
			AfxMessageBox(_T("Video2YUV出错！"));
			m_Screen += _T("程序正在回滚…\r\n");
			UpdateData(false);
			Format.DelectAll();
		}
	}

	m_Screen += _T("执行嵌入…\r\n");
	UpdateData(false);
	if (!Format.Embed())
	{
		AfxMessageBox(_T("Embed出错！"));
		m_Screen += _T("程序正在回滚…\r\n");
		UpdateData(false);
		Format.DelectAll();
	}

	m_Screen += _T("执行视频格式恢复…\r\n");
	UpdateData(false);
	if (!Format.YUV2Video())
	{
		AfxMessageBox(_T("YUV2Video出错！"));
		m_Screen += _T("程序正在回滚…\r\n");
		UpdateData(false);
		Format.DelectAll();
	}

	m_Screen += _T("执行音频合成…\r\n");
	UpdateData(false);
	if (!Format.AudioCombine())
	{
		AfxMessageBox(_T("AudioCombine出错！"));
		m_Screen += _T("程序正在回滚…\r\n");
		UpdateData(false);
		Format.DelectAll();
	}

	// TODO：判断是否执行成功，若不成功则返回错误信息

	m_Screen += _T("大功告成！\r\n");
	UpdateData(false);
	//CDialogEx::OnOK();
}


void CtestDlg::OnBnClickedCancel()
{
	// TODO: 中断操作时的一些必要处理，如终止进程、删除临时文件之类的
	CDialogEx::OnCancel();
}

bool CtestDlg::GenerateAuto( CString input,CString& output)//汉字编码
{
	char buf[13] = {0};
	memcpy(buf,input.GetBuffer(0),input.GetLength()*2);
	output =  _T("");
	CString temp = _T("");
	for(int offset = 0;offset < 12;offset += 2)
	{
		temp.Format(_T("%02X"),(unsigned char)buf[offset + 1]);
		output += temp;
		temp.Format(_T("%02X"),(unsigned char)buf[offset]);
		output += temp;
	}

	// TODO: 自动生成水印程序	

	// 判断水印是否合法
	if (!WatermarkCheck(output))
	{
		AfxMessageBox(_T("水印生成有误！"));
		return false;
	}
	return true;
}

bool CtestDlg::WatermarkCheck( CString )
{
	// TODO: 检验水印是否合法（可能要联网判断水印是否已经存在或码距太小），不合法的尽量通过变换，变换成合法的，实在不行返回错误；合法则保存入服务器数据库中


	return true;
}
bool CtestDlg::JudgeFormat(CString path)
{
	int p, p1, p2 ,p3, p4;
	CString m_Src_format1;
	CString m_Src_format2;
	CString m_Src_format3;
	CString m_Src_format4;

	m_Src_format1 = _T(".yuv");
	m_Src_format2 = _T(".mpg");
	m_Src_format3 = _T(".vob");
	m_Src_format4 = _T(".m2v");
	
	p = path.ReverseFind('.');
	CString m_Src_suffix;
	m_Src_suffix = path.Mid(p);
	CString strp;
	strp.Format(_T("%d"),p);
	AfxMessageBox(m_Src_suffix);
	p1 = m_Src_format1.CompareNoCase(m_Src_suffix);
	p2 = m_Src_format2.CompareNoCase(m_Src_suffix);
	p3 = m_Src_format3.CompareNoCase(m_Src_suffix);
	p4 = m_Src_format4.CompareNoCase(m_Src_suffix);
	if((p1==0) || (p2==0) || (p3==0) || (p4==0))
	{
		AfxMessageBox(_T("fight"));
		return true;
	}
	else
	{
		return false;
	}
}
bool CtestDlg::StorageSpace(CString path)
{
	int ipos = path.ReverseFind('\\');
	path = path.Left(ipos);//存储路径去除文件名
	ULARGE_INTEGER uliUserFree,uliTotal,uliRealFree;
	GetDiskFreeSpaceEx(path,&uliUserFree,&uliTotal,&uliRealFree);//获取剩余存储空间
	if(uliRealFree.QuadPart/(1024*1024*1024) < 2)
	{
		return 0;
	}
	return 1;
}
bool CtestDlg::IfNeedChangeFormat(CString src_path)
{
	int p;
	CString m_Src_format;

	m_Src_format = _T(".yuv");

	p = src_path.ReverseFind('.');
	CString m_Src_suffix;
	m_Src_suffix = src_path.Mid(p);
	CString strp;
	strp.Format(_T("%d"),p);
	AfxMessageBox(m_Src_suffix);
	p = m_Src_format.CompareNoCase(m_Src_suffix);
	if(p==0)
	{
		return false;//不需要转换格式
	}
	else
	{
		return true;//需要准换格式
	}
}
void CtestDlg::Reverse(CString output)//解码部分
{
	int nCount = 1;
	wchar_t* buff = new wchar_t[nCount+1];
	memset(buff, 0, sizeof(wchar_t)*(nCount+1));
	int nIndex = 0;

	TCHAR seps[] = _T(" ");
	TCHAR* token = _tcstok(output.GetBuffer(output.GetLength()), seps);
	while(NULL != token)
	{
		buff[nIndex++] = _tcstoul(token, NULL, 16);
		token = _tcstok(NULL, seps);
	}
	AfxMessageBox(CString(buff));
	delete[] buff;
	buff = NULL;
}
