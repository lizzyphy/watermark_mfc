
// testDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "test.h"
#include "testDlg.h"
#include "afxdlgs.h"
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include <windows.h>  
using namespace std; 


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestDlg �Ի���




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


// CtestDlg ��Ϣ�������

BOOL CtestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	((CEdit *)GetDlgItem(IDC_EDIT_WM))->SetLimitText(6);
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CtestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CtestDlg::OnBnClickedButtonGenerateAuto()//����ˮӡ--���
{
	// �Զ�����ˮӡ��ť
	bool flag = false;
	UpdateData(true);
	flag = GenerateAuto(m_Watermark,m_Watermark_en);
	if (flag == false)
	{
		AfxMessageBox(_T("ˮӡ���Ϸ���"));
		return;
	}
	m_Screen += _T("ˮӡ����ͨ����\r\n");
	UpdateData(false);
}

void CtestDlg::OnBnClickedButtonScan()
{
	// ����Ƶ�ļ���ť
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
	// ���ñ���������Ƶ·��
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
	// TODO: ִ��Ƕ��ı�Ҫ����
	
	// �ж���Ƶ��ʽ�Ƿ�Ϸ�
	bool IfFormat_legal;
	IfFormat_legal= JudgeFormat(m_Src);
	if (!IfFormat_legal)
	{
		AfxMessageBox(_T("��Ƕ����Ƶ��ʽ���Ϸ�"));
	}
	// �ж���Ƶ�Ƿ����
	if (!PathFileExists(m_Src))
	{
		AfxMessageBox(_T("��Ƕ����Ƶ������"));//������
	}
	// �ж���Ƶ�����ʽ�Ƿ�Ϸ�
	
	IfFormat_legal = JudgeFormat(m_SavePath);
	if (!IfFormat_legal)
	{
		AfxMessageBox(_T("��Ƶ�����ʽ���Ϸ�"));
	}
	if(!StorageSpace(m_SavePath))
	{
		AfxMessageBox(_T("��Ƶ����·�����ڴ��̿ռ�С��2G"));
	}
	int ipos = m_SavePath.ReverseFind('\\');
	CString m_Path = m_SavePath.Left(ipos);//�洢·��ȥ���ļ���
	if(!DirectoryExist(m_Path))
	{
		CreateDirectory(m_Path) ;//�����ھ���Ŀ��·���ϴ���һ���ļ���
	}
	
	// ִ��Ƕ�룬�����ɹ���ɾ�������ļ�

	Format.m_Src = m_Src;
	Format.m_SavePath = m_SavePath;
	m_Screen += _T("ִ����Ƶ���롭\r\n");
	UpdateData(false);
	if (!Format.AudioSeparation())
	{
		AfxMessageBox(_T("AudioSeparation����"));
		m_Screen += _T("�������ڻع���\r\n");
		UpdateData(false);
		Format.DelectAll();
	}

	m_Screen += _T("ִ����Ƶ��ʽת����\r\n");
	UpdateData(false);
	{
		if (!Format.Video2YUV(m_Src))
		{
			AfxMessageBox(_T("Video2YUV����"));
			m_Screen += _T("�������ڻع���\r\n");
			UpdateData(false);
			Format.DelectAll();
		}
		m_Src_en = _T("water315.yuv");
	}
	else
	{
		m_Src_en = m_Src;
	}
	if(IfNeedChangeFormat(m_SavePath))//�ж�Ƕˮӡ�ļ��Ƿ���Ҫת����ʽ
	{
		m_Path = _T("water315_en.yuv");
	}
	else
	{
		m_Path = m_SavePath;
	}

	m_Screen += _T("ִ��Ƕ�롭\r\n");
	UpdateData(false);
	//��ˮӡ��16����תΪ2����
	CString watermark_en2;
	CS16toCS2(m_Watermark_en,watermark_en2);
	char* watermark = CStochar(watermark_en2);
	for(int j=0;j<96;j++)
	{
		watermark[j] = watermark[j]-48;
	}
	//��ˮӡ��CStringתΪchar
	FILE *fp;
	fp = fopen("watermark.dat","wb");
	fwrite(watermark, 4, 96, fp);
	fclose(fp);

	//���򿪡������ַ��CStringתΪchar
	char* srcpath = CStochar(m_Src_en);
	char* savepath = CStochar(m_Path);
	if (!Format.Embed(srcpath,savepath))
	{
		AfxMessageBox(_T("Embed����"));
		m_Screen += _T("�������ڻع���\r\n");
		UpdateData(false);
		Format.DelectAll();
	}

	m_Screen += _T("ִ����Ƶ��ʽ�ָ���\r\n");
	UpdateData(false);
	if(IfNeedChangeFormat(m_SavePath))
	{
		if (!Format.YUV2Video(m_SavePath))
			{
				AfxMessageBox(_T("YUV2Video����"));
				m_Screen += _T("�������ڻع���\r\n");
				UpdateData(false);
				Format.DelectAll();
			}
	}
	
	m_Screen += _T("ִ����Ƶ�ϳɡ�\r\n");
	UpdateData(false);
	if (!Format.AudioCombine())
	{
		AfxMessageBox(_T("AudioCombine����"));
		m_Screen += _T("�������ڻع���\r\n");
		UpdateData(false);
		Format.DelectAll();
	}

	// TODO���ж��Ƿ�ִ�гɹ��������ɹ��򷵻ش�����Ϣ

	m_Screen += _T("�󹦸�ɣ�\r\n");
	if(!Finallydel())
	{
		AfxMessageBox(_T("ɾ���ļ�����"));
	}
	UpdateData(false);
	//CDialogEx::OnOK();
}


void CtestDlg::OnBnClickedCancel()
{
	// TODO: �жϲ���ʱ��һЩ��Ҫ��������ֹ���̡�ɾ����ʱ�ļ�֮���
	CDialogEx::OnCancel();
}

bool CtestDlg::GenerateAuto( CString input,CString& output)//���ֱ���
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
	CString cmdStr = _T("SELECT   * FROM watermark"); //����Ҫ���ӵ����ݿ�
	// �ж�ˮӡ�Ƿ�Ϸ�	
	if (!WatermarkCheck( cmdStr,output ))
	{
		return false;
	}
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
	path = path.Left(ipos);//�洢·��ȥ���ļ���
	ULARGE_INTEGER uliUserFree,uliTotal,uliRealFree;
	GetDiskFreeSpaceEx(path,&uliUserFree,&uliTotal,&uliRealFree);//��ȡʣ��洢�ռ�
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
	p = m_Src_format.CompareNoCase(m_Src_suffix);
	if(p==0)
	{
		return false;//����Ҫת����ʽ
	}
	else
	{
		return true;//��Ҫת����ʽ
	}
}

void CtestDlg::Reverse(CString output)//���벿��
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
	delete[] buff;
	buff = NULL;
}

bool CtestDlg::WatermarkCheck(CString cmdStr, CString output)
{
	// TODO: ����ˮӡ�Ƿ�Ϸ�������Ҫ�����ж�ˮӡ�Ƿ��Ѿ����ڻ����̫С�������Ϸ��ľ���ͨ���任���任�ɺϷ��ģ�ʵ�ڲ��з��ش��󣻺Ϸ��򱣴�����������ݿ���
	CString output_to_2 ;
	CS16toCS2(output,output_to_2);
	char* output_to_char = CStochar( output_to_2);
	char m_dif;
	//CArray <CString,CString&> m_Array,m_Array2;
	//m_Array.SetSize(50);
	//m_Array2.SetSize(50);
	CDatabase db;
	db.Open(NULL,FALSE,FALSE,L"ODBC;DSN=mysql5.6;UID=root;PWD=");
	CRecordset rs( &db );
	rs.Open( CRecordset::forwardOnly, (L"%s", cmdStr));
	//int i=0;
	while(!rs.IsEOF())
	{

		CString WMData, WMData2;
		rs.GetFieldValue(L"WMData", WMData); 
		rs.GetFieldValue(L"WMData2", WMData2);
		//m_Array.SetAtGrow(i,WMData);
		//m_Array2.SetAtGrow(i,WMData2);
		//AfxMessageBox(m_Array.GetAt(i));
		//i++;
		CString m_Array_2, m_Array2_2;
		CS16toCS2(WMData,m_Array_2);
		char* m_Array_char = CStochar(m_Array_2);
		int dif_bit = 0, j = 0;
		while (j<96)
		{
			if (output_to_char[j] == m_Array_char[j])
			{
				dif_bit++;
			}
			j++;
		}
		 
		if (dif_bit>70)
		{
			rs.Close();
			db.Close();
			return false;
		}
		CS16toCS2(WMData2,m_Array2_2);
		char* m_Array2_char = CStochar(m_Array2_2);
		dif_bit = 0;
		j = 0;
		while (j<96)
		{
			if (output_to_char[j] == m_Array2_char[j])
			{
				dif_bit++;
			}
			j++;
		}
		if (dif_bit>70)
		{
			rs.Close();
			db.Close();
			return false;
		}
		rs.MoveNext();
	}

	rs.Close();
	db.Close();
	
	/*while (i >= 0)
	{ 
		CString m_Array_2, aa;
		
		CS16toCS2(m_Array.GetAt(i),m_Array_2);
		char* m_Array_char = CStochar(m_Array_2);
		char m_dif = *output_to_char ^ *m_Array_char;
		
		int dif_bit = 0, j = 0;
		while (j<96)
		{
			
		}
		i--;
	}*/
	return true;
}

bool CtestDlg::DirectoryExist(CString Path)
{
	WIN32_FIND_DATA fd;
	BOOL ret = FALSE;
	HANDLE hFind = FindFirstFile(Path, &fd);
	if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		//Ŀ¼����
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
	//Ϊ���ֽ��ַ���������ռ䣬�����СΪ���ֽڼ���Ŀ��ֽ��ֽڴ�С
	char * p = new char[len+1];  //���ֽ�Ϊ��λ
	//���ֽڱ���ת���ɶ��ֽڱ���
	WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),p,len,NULL,NULL);
	p[len] = '\0';   //���ֽ��ַ���'\0'����
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

bool  CtestDlg::Finallydel()
{
	CFileFind find;
	if (find.FindFile(_T("watermark.dat")))
	{
		CFileFind find;
		if (find.FindFile(_T("watermark.dat")))
		{
			DeleteFile(_T("watermark.dat"));
		}
		if (find.FindFile(_T("water315.yuv")))
		{
			DeleteFile(_T("water315.yuv"));
		}
		if (find.FindFile(_T("water315_en.yuv")))
		{
			DeleteFile(_T("water315_en.yuv"));
		}
		return true;
	}
	return true;
}
