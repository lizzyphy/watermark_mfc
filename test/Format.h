#pragma once
class CFormat
{
public:
	CFormat(void);
	CFormat(CString str);
	~CFormat(void);
public:
	CString m_Src; // ԭʼ��Ƶ·��
	CString m_SavePath; // ������Ƶ·��
	CString m_DirAudio; // ��Ƶ��ʱ����·��
	CString m_DirVideo; // ��Ƶ��ʱ����·��
	CString m_DirYUV; // YUV��ʱ����·��
	bool AudioSeparation();
	bool AudioCombine();
	bool Video2YUV(CString,CString);
	bool YUV2Video();
	bool Embed();
	bool DelectAll();
	bool MySystem(const CString&, const CString&, int );//�����µĽ��̣������Ƶ��ʽת��
};

