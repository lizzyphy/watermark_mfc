#pragma once
//Ƕ��ˮӡ����
#define WMLENGTH    96		//ˮӡ���ȣ�bits
#define WMSTRENGTH  0.6		//ˮӡǶ��ǿ��
#define WMDEPTH		1.5		//ˮӡǶ�����

#define NWIDTH      720		//��Ƶһ֡���
#define NHEIGHT		576		//��Ƶһ֡�߶�

#define MWIDTH		528		//ˮӡǶ��������
#define MHEIGHT		320		//ˮӡǶ������߶�
#define MWSTART		(NWIDTH - MWIDTH) / 2		//ˮӡǶ���������
#define MHSTART		(NHEIGHT - MHEIGHT) / 2

#define UDoff		0			//��ȡʱ������ƫ�ƣ��ϸ�����
#define LRoff		0			//��ȡʱ������ƫ�ƣ�������
#define Nbeg		0			//Ƕ�����ȡˮӡ��ʼ֡�ţ�Ĭ��Ϊ0

#define DELFILE				//�Ƿ�ɾ���ϴ�Ƕ��ˮӡ�����������Ƶ
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

	//�Ӻ�������
	void WatermarkEmbed();	            	//ˮӡǶ�����

	void WaveletTrans2D_1();					//��һ���άС���任
	void WaveletTrans2D_2();					//�ڶ����άС���任
	void WaveletTrans2D_3();					//�������άС���任
	void WaveletTransI2D_1();					//��һ���άС����任
	void WaveletTransI2D_2();					//�ڶ����άС����任

	void SearchPoint();							//����������λ��

	void ChangePoint();							//�޸�������ֵ
	double SetValue(short, short, short,short);	//����������㸳ֵ
	double ModMin(short,short);					//�������ʱ���޸���������Χ��ֵ
	short GetSign(double);						//ȡ���ݵķ���

	bool AudioSeparation();
	bool AudioCombine();
	bool Video2YUV(CString,CString);
	bool YUV2Video(CString,CString);
	bool Embed(char*,char*,int);
	bool MySystem(const CString&, const CString&, int );//�����µĽ��̣������Ƶ��ʽת��
};

