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
		AfxMessageBox(_T("转换视频出错！"));
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
	ShExecInfo.lpFile = cmd.c_str();//调用的程序名
	ShExecInfo.lpParameters = par.c_str();//调用程序的命令行参数
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;//窗口状态为隐藏
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess,INFINITE);//等到该进程结束
}*/