#include "StdAfx.h"
#include "Format.h"
#include <windows.h >
#include "test.h"
#include "testDlg.h"


CFormat::CFormat(void)
{
	m_Src = _T("");
	m_DirAudio = _T("");
	m_DirYUV = _T("");
}

CFormat::CFormat( CString str)
{
	m_Src = str;
	m_DirAudio = _T("");
	m_DirYUV = _T("");
}


CFormat::~CFormat(void)
{
}

bool CFormat::AudioSeparation()
{
	return true;
}

bool CFormat::AudioCombine()
{
	return true;
}

bool CFormat::Video2YUV()
{
	CString mm;
	mm.Format(_T("ffmpeg.exe -s 720x576 -i %s -s 720x576  2.yuv"), m_Src);
	int tt = WinExec((LPCSTR)mm.GetBuffer(),5);
	if   (tt <32) 
	{
		AfxMessageBox(_T("ת����Ƶ����"));
		return false;
	}	
	else
	{
		return true;
	}

	
}

bool CFormat::YUV2Video()
{
	return true;
}

bool CFormat::Embed()
{
	return true;
}

bool CFormat::DelectAll()
{
	return true;
}

/*bool CFormat::MySystem(const CString& cmd, const CString& par, int nShow)
{
	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = cmd.c_str();//���õĳ�����
	ShExecInfo.lpParameters = par.c_str();//���ó���������в���
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;//����״̬Ϊ����
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess,INFINITE);//�ȵ��ý��̽���
}*/