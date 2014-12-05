#pragma once
//嵌入水印部分
#define WMLENGTH    96		//水印长度：bits
#define WMSTRENGTH  0.6		//水印嵌入强度
#define WMDEPTH		1.5		//水印嵌入深度

#define NWIDTH      720		//视频一帧宽度
#define NHEIGHT		576		//视频一帧高度

#define MWIDTH		528		//水印嵌入区域宽度
#define MHEIGHT		320		//水印嵌入区域高度
#define MWSTART		(NWIDTH - MWIDTH) / 2		//水印嵌入区域起点
#define MHSTART		(NHEIGHT - MHEIGHT) / 2

#define UDoff		0			//提取时的上下偏移，上负下正
#define LRoff		0			//提取时的左右偏移，左负右正
#define Nbeg		0			//嵌入或提取水印起始帧号，默认为0

#define DELFILE				//是否删除上次嵌入水印操作保存的视频
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
	bool Video2YUV(CString,CString);
	bool YUV2Video(CString,CString);
	bool Embed(char*,char*,int);
	bool MySystem(const CString&, const CString&, int );//调用新的进程，完成视频格式转换
};

