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
	bool Video2YUV(CString);
	bool YUV2Video(CString);
	bool Embed(char*,char*);
	bool DelectAll();
	bool MySystem(const CString&, const CString&, int );//�����µĽ��̣������Ƶ��ʽת��
};

