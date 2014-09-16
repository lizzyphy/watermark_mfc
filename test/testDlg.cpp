
// testDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "test.h"
#include "testDlg.h"
#include "afxdlgs.h"
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include <windows.h>  
#include <io.h>
#include <sys/stat.h>
#include <afx.h>
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
	CString cmdStr = _T("SELECT   * FROM watermark"); //设置要连接的数据库
	m_Progress.SetRange(0,5);
	m_Progress.SetStep(1);
	m_Progress.SetPos(0);

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
	//AfxMessageBox(m_Watermark_en);
	if (flag == false)
	{
		AfxMessageBox(_T("水印不合法！"));
		return;
	}
	m_Screen += _T("水印检验通过！\r\n");
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
	CFileDialog pCFileDialog(false,m_Ext,NULL,0,_T("(*.yuv)|*.yuv|(*.mpg)|*.mpg|(*.vob)|*.vob|(*.m2v)|*.m2v||"),NULL);
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
	CString m_Src_en;
	CString m_Path_en;
	// TODO: 执行嵌入的必要操作

	// 判断水印是否合法
	
	// 判断视频格式是否合法
	if (!JudgeFormat(m_Src))
	{
		AfxMessageBox(_T("待嵌入视频格式不合法"));
	}
	// 判断视频是否存在
	if (!PathFileExists(m_Src))
	{
		AfxMessageBox(_T("待嵌入视频不存在"));//不存在
	}
	// 判断视频保存格式是否合法
	
	m_Screen += _T("待嵌入视频格式检测完成…\r\n");
	m_Progress.SetPos(1);
	UpdateData(false);
	if (!JudgeFormat(m_SavePath))
	{
		AfxMessageBox(_T("视频保存格式不合法"));
	}
	if(!StorageSpace(m_SavePath))
	{
		AfxMessageBox(_T("视频保存路径所在磁盘空间小于2G"));
	}
	int ipos = m_SavePath.ReverseFind('\\');
	CString m_Path = m_SavePath.Left(ipos);//存储路径去除文件名
	if(!DirectoryExist(m_Path))
	{
		CreateDirectory(m_Path) ;//不存在就在目标路径上创建一个文件夹
	}
	m_Screen += _T("视频保存格式检测完成…\r\n");
	m_Progress.SetPos(2);
	UpdateData(false);
	// 执行嵌入，若不成功则删除残留文件
	MSG msg; 
	while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) 
	{ 
		TranslateMessage(&msg); 
		DispatchMessage(&msg);
	}
	Format.m_Src = m_Src;
	Format.m_SavePath = m_SavePath;
/*	m_Screen += _T("执行音频分离…\r\n");
	UpdateData(false);
	if (!Format.AudioSeparation())
	{
		AfxMessageBox(_T("AudioSeparation出错！"));
		m_Screen += _T("程序正在回滚…\r\n");
		UpdateData(false);
		Finallydel();
	}
*/
	if(IfNeedChangeFormat(m_Src))
	{
		m_Screen += _T("执行视频格式转换…\r\n");
		UpdateData(false);
		if (!Format.Video2YUV(m_Src,m_Path))
		{
			AfxMessageBox(_T("Video2YUV出错！"));
			m_Screen += _T("程序正在回滚…\r\n");
			UpdateData(false);
			Finallydel(m_Path);
		}
		m_Src_en =m_Path + _T("\\water315.yuv");
	}
	else
	{
		m_Src_en = m_Src;
	}
//	AfxMessageBox(m_Src_en);
	
	if(IfNeedChangeFormat(m_SavePath))//判断嵌水印文件是否需要转换格式
	{
		
		m_Path_en =m_Path + _T("\\water315_en.yuv");
	}
	else
	{
		m_Path_en = m_SavePath;
	}
//	AfxMessageBox(m_Path_en);
	m_Screen += _T("执行嵌入…\r\n");
	m_Progress.SetPos(3);
	UpdateData(false);
	//将水印从16进制转为2进制
	CString watermark_en2;
	CS16toCS2(m_Watermark_en,watermark_en2);
	char* watermark = CStochar(watermark_en2);
	for(int j=0;j<96;j++)
	{
		watermark[j] = watermark[j]-48;
	}
	//将水印从CString转为char
	FILE *fp;
	fp = fopen("watermark.dat","wb");
	fwrite(watermark, 4, 96, fp);
	fclose(fp);
	Sleep(1000);
	//将打开、保存地址从CString转为char
	char* srcpath = CStochar(m_Src_en);
	int frame = Readframe(srcpath);
	char* savepath = CStochar(m_Path_en);
	if (!Format.Embed(srcpath,savepath,frame))
	{
		AfxMessageBox(_T("Embed出错！"));
		m_Screen += _T("程序正在回滚…\r\n");
		UpdateData(false);
		Finallydel(m_Path);
	}

	if(IfNeedChangeFormat(m_SavePath))
	{
		m_Screen += _T("执行保存视频格式恢复…\r\n");
		UpdateData(false);
		if (!Format.YUV2Video(m_SavePath,m_Path_en))
			{
				AfxMessageBox(_T("YUV2Video出错！"));
				m_Screen += _T("程序正在回滚…\r\n");
				UpdateData(false);
				Finallydel(m_Path);
			}
	}
	Sleep(1000);
/*	m_Screen += _T("执行音频合成…\r\n");
	UpdateData(false);
	if (!Format.AudioCombine())
	{
		AfxMessageBox(_T("AudioCombine出错！"));
		m_Screen += _T("程序正在回滚…\r\n");
		UpdateData(false);
		Finallydel();
	}
*/
	// TODO：判断是否执行成功，若不成功则返回错误信息
	if(!Finallydel(m_Path))
	{
		AfxMessageBox(_T("删除文件出错"));
	}
	m_Screen += _T("大功告成！\r\n");
	m_Progress.SetPos(5);
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

	// 判断水印是否合法	
	/*if (!WatermarkCheck( cmdStr,output ))
	{
		AfxMessageBox(_T("水印不合法！"));
		return false;
	}*/
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
	//AfxMessageBox(m_Src_suffix);
	p1 = m_Src_format1.CompareNoCase(m_Src_suffix);
	p2 = m_Src_format2.CompareNoCase(m_Src_suffix);
	p3 = m_Src_format3.CompareNoCase(m_Src_suffix);
	p4 = m_Src_format4.CompareNoCase(m_Src_suffix);
	if((p1==0) || (p2==0) || (p3==0) || (p4==0))
	{
		//AfxMessageBox(_T("fight"));
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
	//AfxMessageBox(m_Src_suffix);
	p = m_Src_format.CompareNoCase(m_Src_suffix);
	if(p==0)
	{
		return false;//不需要转换格式
	}
	else
	{
		return true;//需要转换格式
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
	//AfxMessageBox(CString(buff));
	delete[] buff;
	buff = NULL;
}

/*bool CtestDlg::WatermarkCheck(CString cmdStr, CString output)
{
	// TODO: 检验水印是否合法（可能要联网判断水印是否已经存在或码距太小），不合法的尽量通过变换，变换成合法的，实在不行返回错误；合法则保存入服务器数据库中
	CString m_dif;
	CArray <CString,CString&> m_Array,m_Array2;
	m_Array.SetSize(50);
	m_Array2.SetSize(50);
	CDatabase db;
	db.Open(NULL,FALSE,FALSE,L"ODBC;DSN=mysql5.6;UID=root;PWD=");
	CRecordset rs( &db );
	rs.Open( CRecordset::forwardOnly, (L"%s", cmdStr));
	//short nFields = rs.GetODBCFieldCount();
	int i=0;
	while(!rs.IsEOF())
	{

		CString WMData, WMData2;
		rs.GetFieldValue(L"WMData", WMData); 
		rs.GetFieldValue(L"WMData2", WMData2);
		m_Array.SetAtGrow(i,WMData);
		m_Array2.SetAtGrow(i,WMData2);
		i++;
		rs.MoveNext();
	}
	rs.Close();
	db.Close();
	//m_dif = m_Array[1]^output;
	//AfxMessageBox(m_dif);
	return true;
}*/

bool CtestDlg::DirectoryExist(CString Path)
{
	WIN32_FIND_DATA fd;
	BOOL ret = FALSE;
	HANDLE hFind = FindFirstFile(Path, &fd);
	if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		//目录存在
		ret = TRUE;

	}
	FindClose(hFind);
	return ret;
}

bool CtestDlg::CreateDirectory(CString path)
{
	SECURITY_ATTRIBUTES attrib;
	attrib.bInheritHandle = FALSE;
	attrib.lpSecurityDescriptor = NULL;
	attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
	return ::CreateDirectory( path, &attrib);
}

char* CtestDlg::CStochar(CString str)
{
	int len =WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),NULL,0,NULL,NULL);
	//为多字节字符数组申请空间，数组大小为按字节计算的宽字节字节大小
	char * p = new char[len+1];  //以字节为单位
	//宽字节编码转换成多字节编码
	WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),p,len,NULL,NULL);
	p[len] = '\0';   //多字节字符以'\0'结束
	return p;
}

void CtestDlg::CS16toCS2(CString str,CString& restr)
{
	int len = str.GetLength();
	restr = _T("");
	for(int i = 0; i < len; i ++)
	{
		switch (str[i]){
			case '0':restr = restr + _T("0000"); break;
			case '1':restr = restr + _T("0001"); break; 
			case '2':restr = restr + _T("0010"); break;
			case '3':restr = restr + _T("0011"); break;
			case '4':restr = restr + _T("0100"); break;
			case '5':restr = restr + _T("0101"); break;
			case '6':restr = restr + _T("0110"); break;
			case '7':restr = restr + _T("0111"); break;
			case '8':restr = restr + _T("1000"); break;
			case '9':restr = restr + _T("1001"); break;
			case 'A':restr = restr + _T("1010"); break;
			case 'B':restr = restr + _T("1011"); break;
			case 'C':restr = restr + _T("1100"); break;
			case 'D':restr = restr + _T("1101"); break;
			case 'E':restr = restr + _T("1110"); break;
			case 'F':restr = restr + _T("1111"); break;
			default:restr = restr + _T("0000"); break;
		}
		
	}
}
bool  CtestDlg::Finallydel(CString path)
{
	CFileFind find;
	if (find.FindFile(path + _T("\\watermark.dat")))
	{
		DeleteFile(path + _T("\\watermark.dat"));
	}
	if (find.FindFile(path + _T("\\water315.yuv")))
	{
		DeleteFile(path + _T("\\water315.yuv"));
	}
	if (find.FindFile(path + _T("\\water315_en.yuv")))
	{
		DeleteFile(path + _T("\\water315_en.yuv"));
	}
	return true;
}

int CtestDlg::Readframe(char* srcpath)
{
	struct _stat info;
	_stat(srcpath, &info);
	int size = info.st_size;
	int frame = size/(720*576*1.5);
	return frame;
}
	
