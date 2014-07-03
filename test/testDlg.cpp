
// testDlg.cpp : ʵ���ļ�
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


// CtestDlg ��Ϣ��������

BOOL CtestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ����Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի���������С����ť������Ҫ����Ĵ���
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



void CtestDlg::OnBnClickedButtonGenerateAuto()
{
	// �Զ�����ˮӡ��ť
	bool flag = false;
	
	flag = GenerateAuto(m_Watermark);

	if (flag == false)
	{
		AfxMessageBox(_T("ˮӡ���Ϸ���"));
		return;
	}

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
	// TODO: ִ��Ƕ��ı�Ҫ����

	// �ж�ˮӡ�Ƿ�Ϸ�
	if (!WatermarkCheck( m_Watermark ))
	{
		AfxMessageBox(_T("ˮӡ���Ϸ���"));
		return;
	}
	m_Screen += _T("ˮӡ����ͨ����\r\n");
	UpdateData(false);
	// TODO���ж���Ƶ��ʽ�Ƿ�Ϸ�
	bool IfFormat_legal;
	IfFormat_legal= JudgeFormat(m_Src);
	if (!IfFormat_legal)
	{
		AfxMessageBox(_T("��Ƕ����Ƶ��ʽ���Ϸ�"));
	}
	// TODO���ж���Ƶ�Ƿ����
	if (!PathFileExists(m_Src))
	{
		AfxMessageBox(_T("��Ƕ����Ƶ������"));//������
	}
	// TODO���ж���Ƶ�����ʽ�Ƿ�Ϸ�
	
	IfFormat_legal = JudgeFormat(m_SavePath);
	if (!IfFormat_legal)
	{
		AfxMessageBox(_T("��Ƶ�����ʽ���Ϸ�"));
	}
	// TODO���ж���Ƶ����·�����ڴ��̿ռ��Ƿ񹻴�
	if(!StorageSpace(m_SavePath))
	{
		AfxMessageBox(_T("��Ƶ����·�����ڴ��̿ռ�С��2G"));
	}
	// TODO���ж���Ƶ����·���Ƿ���ڣ�û�����½�һ��
	if(!PathIsDirectory(m_SavePath))
	{
		CreateDirectory(m_SavePath,NULL) ;//�����ھ���Ŀ��·���ϴ���һ���ļ���
	}
	
	// TODO��ִ��Ƕ�룬�����ɹ���ɾ�������ļ�

	Format.m_Src = m_Src;
	Format.m_SavePath = m_SavePath;
	m_Screen += _T("ִ����Ƶ���롭\r\n");
	UpdateData(false);
	if (!Format.AudioSeparation())
	{
		AfxMessageBox(_T("AudioSeparation������"));
		m_Screen += _T("�������ڻع���\r\n");
		UpdateData(false);
		Format.DelectAll();
	}

	m_Screen += _T("ִ����Ƶ��ʽת����\r\n");
	UpdateData(false);
	if(IfNeedChangeFormat(m_Src))//�ж��Ƿ���Ҫ׼����ʽ
	{
		if (!Format.Video2YUV())
		{
			AfxMessageBox(_T("Video2YUV������"));
			m_Screen += _T("�������ڻع���\r\n");
			UpdateData(false);
			Format.DelectAll();
		}
	}

	m_Screen += _T("ִ��Ƕ�롭\r\n");
	UpdateData(false);
	if (!Format.Embed())
	{
		AfxMessageBox(_T("Embed������"));
		m_Screen += _T("�������ڻع���\r\n");
		UpdateData(false);
		Format.DelectAll();
	}

	m_Screen += _T("ִ����Ƶ��ʽ�ָ���\r\n");
	UpdateData(false);
	if (!Format.YUV2Video())
	{
		AfxMessageBox(_T("YUV2Video������"));
		m_Screen += _T("�������ڻع���\r\n");
		UpdateData(false);
		Format.DelectAll();
	}

	m_Screen += _T("ִ����Ƶ�ϳɡ�\r\n");
	UpdateData(false);
	if (!Format.AudioCombine())
	{
		AfxMessageBox(_T("AudioCombine������"));
		m_Screen += _T("�������ڻع���\r\n");
		UpdateData(false);
		Format.DelectAll();
	}

	// TODO���ж��Ƿ�ִ�гɹ��������ɹ��򷵻ش�����Ϣ

	m_Screen += _T("�󹦸�ɣ�\r\n");
	UpdateData(false);
	//CDialogEx::OnOK();
}


void CtestDlg::OnBnClickedCancel()
{
	// TODO: �жϲ���ʱ��һЩ��Ҫ����������ֹ���̡�ɾ����ʱ�ļ�֮���
	CDialogEx::OnCancel();
}

bool CtestDlg::GenerateAuto( CString& str)
{
	// TODO: �Զ�����ˮӡ����
	str = _T("haha"); // �������
	
	// �ж�ˮӡ�Ƿ�Ϸ�
	if (!WatermarkCheck( str ))
	{
		AfxMessageBox(_T("ˮӡ��������"));
		return false;
	}
	return true;
}

bool CtestDlg::WatermarkCheck( CString )
{
	// TODO: ����ˮӡ�Ƿ�Ϸ�������Ҫ�����ж�ˮӡ�Ƿ��Ѿ����ڻ����̫С�������Ϸ��ľ���ͨ���任���任�ɺϷ��ģ�ʵ�ڲ��з��ش��󣻺Ϸ��򱣴�����������ݿ���


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
bool CtestDlg::StorageSpace(CString storage_path)
{
	int p;
	float ResidualSpace;
	CString Root_path;
	p = storage_path.ReverseFind(':');
	Root_path = storage_path.Mid(1,p);
	bool fResult;
	unsigned _int64   lpFreeBytesAvailableToCaller;
	unsigned _int64   lpTotalNumberOfBytes;
	unsigned _int64   lpTotalNumberOfFreeBytes;
	fResult=::GetDiskFreeSpaceEx(Root_path,(PULARGE_INTEGER)&lpFreeBytesAvailableToCaller,(PULARGE_INTEGER)&lpTotalNumberOfBytes, (PULARGE_INTEGER)&lpTotalNumberOfFreeBytes);
	if(fResult)
	{
		ResidualSpace = (float)lpFreeBytesAvailableToCaller/1024/1024;
		if(ResidualSpace>=2000)
		{
			return true;
		}
		else
		{
			return false;
		}

	}
	else
	{
		return false;
	}
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
		return false;//����Ҫת����ʽ
	}
	else
	{
		return true;//��Ҫ׼����ʽ
	}
}