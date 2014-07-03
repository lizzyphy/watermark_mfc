#pragma once
class CFormat
{
public:
	CFormat(void);
	CFormat(CString str);
	~CFormat(void);
public:
	CString m_Src; // 原始视频路径
	CString m_SavePath; // 保存视频路径
	CString m_DirAudio; // 音频临时保存路径
	CString m_DirVideo; // 视频临时保存路径
	CString m_DirYUV; // YUV临时保存路径
	bool AudioSeparation();
	bool AudioCombine();
	bool Video2YUV();
	bool YUV2Video();
	bool Embed();
	bool DelectAll();
	bool MySystem(const CString&, const CString&, int );//调用新的进程，完成视频格式转换
};

