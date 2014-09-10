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

	//子函数声明
	void WatermarkEmbed();	            	//水印嵌入过程

	void WaveletTrans2D_1();					//第一层二维小波变换
	void WaveletTrans2D_2();					//第二层二维小波变换
	void WaveletTrans2D_3();					//第三层二维小波变换
	void WaveletTransI2D_1();					//第一层二维小波逆变换
	void WaveletTransI2D_2();					//第二层二维小波逆变换

	void SearchPoint();							//查找特征点位置

	void ChangePoint();							//修改特征点值
	double SetValue(short, short, short,short);	//具体给特征点赋值
	double ModMin(short,short);					//特殊情况时，修改特征点周围的值
	short GetSign(double);						//取数据的符号

	bool AudioSeparation();
	bool AudioCombine();
	bool Video2YUV(CString);
	bool YUV2Video(CString);
	bool Embed(char*,char*);
	bool DelectAll();
	bool MySystem(const CString&, const CString&, int );//调用新的进程，完成视频格式转换
};

