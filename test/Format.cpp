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

bool CFormat::Video2YUV(CString src_path,CString save_path)
{ 
	CString command_bat;
	command_bat = _T("ffmpeg.exe -s 720x576 -i ") + src_path + _T(" -s 720x576 ")+save_path+_T("\r\n");
	CStdioFile file;
	file.Open(_T("RUN.bat"),CFile::modeWrite);
	file.WriteString(command_bat);
	file.Close();
	WinExec("RUN.bat",SW_SHOW); 
	return true;
	
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