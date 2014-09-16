#include "StdAfx.h"
#include "Format.h"
#include <windows.h >
#include "test.h"
#include "testDlg.h"

double Nmean;				//存储修改第一个值时，可能对均值造成影响

//存储从源文件中读取的Y,U,V分量，只保留当前处理帧
unsigned char Y[NHEIGHT][NWIDTH];
unsigned char U[(NHEIGHT/2)][(NWIDTH/2)];
unsigned char V[(NHEIGHT/2)][(NWIDTH/2)];

//存储Y分量嵌入水印区域的数据
double Ym[MHEIGHT][MWIDTH];			//水印嵌入之前
double Yw[MHEIGHT][MWIDTH];			//水印嵌入之后

//wavelet transfrom Usage
//middle variance
double TA1[MHEIGHT][MWIDTH/2];
double TA2[MHEIGHT][MWIDTH/2];

//leve1
double TCA1[MHEIGHT/2][MWIDTH/2];
double TCH1[MHEIGHT/2][MWIDTH/2];
double TCV1[MHEIGHT/2][MWIDTH/2];
double TCD1[MHEIGHT/2][MWIDTH/2];
//leve2
double TCA2[MHEIGHT/4][MWIDTH/4];
double TCH2[MHEIGHT/4][MWIDTH/4];
double TCV2[MHEIGHT/4][MWIDTH/4];
double TCD2[MHEIGHT/4][MWIDTH/4];
//leve3
double TCA3[MHEIGHT/8][MWIDTH/8];
//存储水印嵌入点位置
short WMPoint[WMLENGTH];
unsigned char watermark[WMLENGTH] = {0};				//存储生成的原始水印
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

bool CFormat::Video2YUV(CString src_path,CString save_path) //视频转换格式
{ 
	CString command_bat;
	save_path = save_path + _T("\\water315.yuv");
	command_bat = _T("ffmpeg.exe -s 720x576 -i ") + src_path + _T(" -s 720x576 ")+save_path+_T("\r\n");
	CStdioFile file;
	file.Open(_T("RUN.bat"),CFile::modeWrite);
	file.WriteString(command_bat);
	file.Close();
	WinExec("RUN.bat",SW_HIDE); //调用批处理文件
	return true;
}

bool CFormat::YUV2Video(CString save_path,CString src_path)
{
	CString command_bat;
	command_bat = _T("ffmpeg.exe -s 720x576 -i ") + src_path + _T(" -target film-dvd -r 25  -s 720x576 ")+save_path+_T("\r\n");
	CStdioFile file;
	file.Open(_T("RUN_yuv_to_mpg.bat"),CFile::modeWrite);
	file.WriteString(command_bat);
	file.Close();
	WinExec("RUN_yuv_to_mpg.bat",SW_HIDE); //调用批处理文件
	return true;
}

bool CFormat::Embed(char* srcpath,char* savepath,int frame)
{
	FILE *op;
	op = fopen("watermark.dat","rb");
	fread(watermark, 1, WMLENGTH, op);
	fclose(op);

	FILE *fp;
	__int64 offset;				//偏移计算使用long long型，方便访问大于2G的文件
	int num;
	int i,j;
	double CalValue, CalFloor;				//为了将实数取整的中间变量
	char combel[100] = "del ";
/*	CString str;
	str.Format(_T("%d"),frame);
	AfxMessageBox(str);*/
	for (num = 0; num < frame; num++)
	{
		//读一帧YUV视频
		if (fp = fopen(srcpath,"rb"))
		{
			offset = (long long)(num+Nbeg)*(long long)(NWIDTH*NHEIGHT*3/2);//防止数据溢出，使用long long
			_fseeki64(fp, offset, SEEK_SET);
			fread(Y, 1, NWIDTH*NHEIGHT, fp);
			_fseeki64(fp, offset + (long long)(NWIDTH*NHEIGHT), SEEK_SET);
			fread(U, 1, (NWIDTH/2)*(NHEIGHT/2), fp);
			_fseeki64(fp, offset + (long long)(NWIDTH*NHEIGHT + (NWIDTH/2)*(NHEIGHT/2)), SEEK_SET);
			fread(V, 1, (NWIDTH/2)*(NHEIGHT/2), fp);
		}
		fclose(fp);

		//处理过程
		//取出Y分量中间部分，并完成char———>short的转换
		for (i = 0; i < MHEIGHT; i++)
		{
			for (j = 0; j < MWIDTH; j++)
			{
				Ym[i][j] = Y[i + MHSTART][j + MWSTART];
			}
		}
		
		//Wavelet Transform : Ym
		WaveletTrans2D_1();
		WaveletTrans2D_2();
		WaveletTrans2D_3();

		//找特征点
		SearchPoint();
		//修改值，嵌入水印
		ChangePoint();
		
		//Inverse Wavelet Transform
		WaveletTransI2D_2();
		WaveletTransI2D_1();

		//合成Y分量，数据转换：short-->unsigned char
		for (i = 0; i < MHEIGHT; i++)//找出不合理值
		{
			for (j = 0; j < MWIDTH; j++)
			{
				CalValue = Yw[i][j];
				if (CalValue > 255)
				{
					Y[i + MHSTART][j + MWSTART] = 255;
				}
				else if (CalValue < 0)
				{
					Y[i + MHSTART][j + MWSTART] = 0;
				}
				else
				{
					CalFloor = floor(CalValue);
					if ((CalValue - CalFloor) == 0)
					{
						Y[i + MHSTART][j + MWSTART] = (unsigned char)(CalValue);
					}
					else
					{
						AfxMessageBox(_T("check WatermarkEmbed function: 最终变换结果出现小数了！！！"));
					}
				}
			}
		}
	//	AfxMessageBox(_T("变换结束"));
		//写一帧YUV视频，存储过程
		if ( fp=fopen(savepath,"ab" ) )   //"ab"
		{
	//		AfxMessageBox(_T("写入"));
			fwrite(Y, 1, NWIDTH*NHEIGHT, fp);
			fseek(fp,NWIDTH*NHEIGHT,SEEK_SET);
			fwrite(U, 1, (NWIDTH/2)*(NHEIGHT/2), fp);
			fseek(fp,NWIDTH*NHEIGHT + (NWIDTH/2)*(NHEIGHT/2),SEEK_SET);
			fwrite(V, 1, (NWIDTH/2)*(NHEIGHT/2), fp);
		}
		fclose(fp);

	}
	
	AfxMessageBox(_T("!!The Watermark Embed Done!!"));
	return true;
}

void  CFormat::ChangePoint()
{
	short k;
	short Nrow, Ncolumn;
	short shtq22,shtq21;

	double Ntc;
	double Nmid;
	double q11,q12,q13;
	double q21,q22,q23;
	double q31,q32,q33;

	for(k = 0; k < WMLENGTH; k++)
	{
		//计算嵌入位置
		Nrow = 3 + ((WMPoint[k] - 1)/(MWIDTH/24 - 1)) * 3;
		Ncolumn = 3 + ((WMPoint[k] - 1)%(MWIDTH/24 - 1)) * 3;

		Nrow = 2 * Nrow;
		Ncolumn = 2 * Ncolumn;

		//嵌入数据
		q11 = TCH2[Nrow-2][Ncolumn-2];
		q12 = TCH2[Nrow-2][Ncolumn-1];
		q13 = TCH2[Nrow-2][Ncolumn];
		q21 = TCH2[Nrow-1][Ncolumn-2];
		q22 = TCH2[Nrow-1][Ncolumn-1];
		q23 = TCH2[Nrow-1][Ncolumn];
		q31 = TCH2[Nrow][Ncolumn-2];
		q32 = TCH2[Nrow][Ncolumn-1];
		q33 = TCH2[Nrow][Ncolumn];

		Nmean = (fabs(q11) + fabs(q12) + fabs(q13)
				+ fabs(q23)
				+ fabs(q31) + fabs(q32) + fabs(q33))/8;
		Nmid = (fabs(q22) + fabs(q21))/2;

		if (q22 == 0)
		{
			q22 = 0.0001;
		}
		if (q21 == 0)
		{
			q21 = 0.0001;
		}
		if (Nmid == 0)
		{
			Nmid = 0.0001;
			if (Nmean == 0)
			{
				Nmean = 0.0001;
			}
		}

		shtq21 = (short)q21;
		shtq22 = (short)q22;
		Ntc = Nmid/Nmean;
		if (Nmid > Nmean)
		{
			if (1 == watermark[k])
			{
				if (Ntc < WMDEPTH)
				{
					q22 = SetValue(+1,shtq22, Nrow, Ncolumn);
					//q22 = (1+WMSTRENGTH)*Signq22*Nmean;
					TCH2[Nrow-1][Ncolumn-1] = q22;
					q21 = SetValue(+1,shtq21, Nrow, Ncolumn);
					//q21 = (1+WMSTRENGTH)*Signq21*Nmean;
					TCH2[Nrow-1][Ncolumn-2] = q21;
				}
			}
			else
			{
				q22 = SetValue(-1,shtq22, Nrow, Ncolumn);
				//q22 = (1-WMSTRENGTH)*Signq22*Nmean;
				TCH2[Nrow-1][Ncolumn-1] = q22;
				q21 = SetValue(-1,shtq21, Nrow, Ncolumn);
				//q21 = (1-WMSTRENGTH)*Signq21*Nmean;
				TCH2[Nrow-1][Ncolumn-2] = q21;
			}
		}
		else
		{
			if (1 == watermark[k])
			{
				if ((q22 == 0.0001)&(q21 == 0.0001)&(Nmean == 0.0001))
				{
					TCH2[Nrow-1][Ncolumn-1] = 16;
					TCH2[Nrow-1][Ncolumn-2] = -16;
				}
				else
				{
					q22 = SetValue(+1,shtq22, Nrow, Ncolumn);
					//q22 = (1+WMSTRENGTH)*Signq22*Nmean;
					TCH2[Nrow-1][Ncolumn-1] = q22;
					q21 = SetValue(+1,shtq21, Nrow, Ncolumn);
					//q21 = (1+WMSTRENGTH)*Signq21*Nmean;
					TCH2[Nrow-1][Ncolumn-2] = q21;
				}
			}
		}
	}
	
}


double  CFormat::SetValue(short Badd, short Nori, short Nrow, short Ncol)
{
	/*Badd 计算是加法（+1）或者减法（-1）
	  Nori 需要修改的原始值，调用时需double强制转换为short型
	  Ncmp 计算时的参考值，即Nmean*/
	double Rtemp,Rvalue;
	double Rchg;
	short Rsht,Rrem,Rremn;

	Rtemp = (1+(Badd*WMSTRENGTH))*Nmean;
	Rsht = ((short)Rtemp)/16;
	Rremn = Nori%16;

	Rrem = (short)(fabs((double)Rremn));

	//Rvalue = GetSign(Nori)*Rsht*16 + Rrem;

	if (Rsht == 0)
	{
		if ((Rrem <= Rtemp)&(Badd == 1))
		{
			Rsht = 1;
		}
		else if ((Rrem >= Rtemp)&(Badd == -1))
		{
			while (Rrem >= Rtemp)
			{
				Rchg = ModMin(Nrow,Ncol);
				if (Rchg != 16)
				{
					AfxMessageBox(_T("check SetValue function:特征点值修改幅度不是16的倍数"));
				}
				Nmean = Nmean + 2;
				Rtemp = (1-WMSTRENGTH)*Nmean;
			}
		}
	}
	Rvalue = GetSign(Nori)*Rsht*16 + Rremn;
	if ((Badd == 1)&(fabs(Rvalue) <= Nmean))
	{
		Rvalue = GetSign(Nori)*(fabs(Rvalue) + 16);
	}
	else if ((Badd == -1)&(fabs(Rvalue) > (Rtemp/(1-WMSTRENGTH))))
	{
		AfxMessageBox(_T("check SetValue function"));
	}
	return Rvalue;
}


double  CFormat::ModMin(short Nrow, short Ncol)
{
	double Nmin,q[7];
	double Rchg;
	short i,loc;
	short Nr, Nc;
	
	q[0] = fabs(TCH2[Nrow-2][Ncol-2]);
	q[1] = fabs(TCH2[Nrow-2][Ncol-1]);
	q[2] = fabs(TCH2[Nrow-2][Ncol]);
	q[3] = fabs(TCH2[Nrow-1][Ncol]);
	q[4] = fabs(TCH2[Nrow][Ncol-2]);
	q[5] = fabs(TCH2[Nrow][Ncol-1]);
	q[6] = fabs(TCH2[Nrow][Ncol]);
	Nmin = q[0];
	loc = 0;
	for (i = 0; i < 6; i++)
	{
		if (q[i+1] < Nmin)
		{
			Nmin = q[i+1];
			loc = i+1;
		}
	}
	if (loc == 3)
	{
		Nr = 1;
		Nc = 0;
	}
	else if (loc < 3)
	{
		Nr = 2;
		Nc = 2-loc;
	}
	else
	{
		Nr = 0;
		Nc = 6-loc;
	}
	if (TCH2[Nrow-Nr][Ncol-Nc] > 0)
	{
		TCH2[Nrow-Nr][Ncol-Nc] = TCH2[Nrow-Nr][Ncol-Nc] + 16;
	}
	else
	{
		TCH2[Nrow-Nr][Ncol-Nc] = TCH2[Nrow-Nr][Ncol-Nc] - 16;
	}
	Rchg = fabs(TCH2[Nrow-Nr][Ncol-Nc]) - q[loc];
	return Rchg;
}

short  CFormat::GetSign(double svalue)
{
	if (svalue >= 0)
		return 1;
//	else if (svalue < 0 )
//		return -1;
	else
		return -1;
}

void  CFormat::SearchPoint()
{
	short i, j;
	short k, kmax, koff;
	double Tmean;
	double TempSum = 0;
	double ComOrder;
	double STm[MHEIGHT/8][MWIDTH/8];
	double TempSTm[4*WMLENGTH];
	short  STm_Index[4*WMLENGTH];
	double ValPoint[4*WMLENGTH];
	double q11,q12,q13;
	double q21,q22,q23;
	double q31,q32,q33;
	short Isgo,Isdone;

	//计算均值差
	for (i = 1; i < MHEIGHT/8 - 1; i++)
	{
		for (j = 1; j < MWIDTH/8 - 1; j++)
		{
			q11 = TCA3[i-1][j-1];
			q12 = TCA3[i-1][j];
			q13 = TCA3[i-1][j+1];
			q21 = TCA3[i][j-1];
			q22 = TCA3[i][j];
			q23 = TCA3[i][j+1];
			q31 = TCA3[i+1][j-1];
			q32 = TCA3[i+1][j];
			q33 = TCA3[i+1][j+1];

			Tmean = (q11 + q12 + q13 + q21 + q23 + q31 + q32 + q33)/8;
			STm[i][j] = (fabs(Tmean - q11) + fabs(Tmean - q12) + fabs(Tmean - q13)
						+ fabs(Tmean - q21) + fabs(Tmean - q22) + fabs(Tmean - q23)
						+ fabs(Tmean - q31) + fabs(Tmean - q32) + fabs(Tmean - q33))/8;

		}
	}
	
	//计算3x3小块能量和及总能量
	for (k=0, i=2; i < MHEIGHT/8 - 2; i=i+3)
	{
		for (j = 2; j < MWIDTH/8 - 2; j=j+3)
		{
			TempSTm[k] = STm[i-1][j-1] + STm[i-1][j] + STm[i-1][j+1]
						+ STm[i][j+1]
						+ STm[i+1][j-1] + STm[i+1][j] + STm[i+1][j+1];
			TempSum = TempSum + TempSTm[k];
			k++;
		}
	}

	//搜索分块值最大的一部分，要求大于96个
	kmax = k;
	TempSum = 0.8*(TempSum/kmax);
	k = 0;

	while (k < WMLENGTH)
	{
		for (i = 0; i < kmax; i++)
		{
			if (TempSTm[i] > TempSum)
			{
				ValPoint[k] = TempSTm[i];
				TempSTm[i] = 0;
				STm_Index[k] = i + 1;
				k++;
			}
		}
		TempSum = 0.65*TempSum;
	}

	//找到能量最大的96个块
	kmax = k;
	koff = kmax - WMLENGTH;
	for (i = 0; i < koff; i++)
	{
		for (j = 0; j < kmax-i-1; j++)
		{
			if (ValPoint[j] < ValPoint[j+1])
			{
				ComOrder = ValPoint[j];
				ValPoint[j] = ValPoint[j+1];
				ValPoint[j+1] = ComOrder;
				k = STm_Index[j];
				STm_Index[j] = STm_Index[j+1];
				STm_Index[j+1] = k;
			}
		}
	}

	for (i = 0; i < WMLENGTH; i++)
	{
		WMPoint[i] = STm_Index[i];
	}
	
	//对96个小块按序号从小到大排序
	Isdone = 0;
	Isgo = 0;
	k = 1;
	while (Isdone == 0)
	{
		for (i = 0; i < WMLENGTH - k; i++)
		{
			if (WMPoint[i] > WMPoint[i+1])
			{
				j = WMPoint[i];
				WMPoint[i] = WMPoint[i+1];
				WMPoint[i+1] = j;
				Isgo = 1;
			}
		}
		k++;
		if (Isgo == 0)
		{
			Isdone = 1;
		}
		Isgo = 0;
	}
}


void  CFormat::WaveletTrans2D_1()
{
	short i, j;

	for (i = 0; i < MHEIGHT; i++)
	{
		for(j = 0; j < MWIDTH; j = j+2)
		{
			TA1[i][j/2] = Ym[i][j] + Ym[i][j+1];
			TA2[i][j/2] = Ym[i][j] - Ym[i][j+1];
		}
	}

	for (i = 0; i < MHEIGHT; i = i+2)
	{
		for (j = 0; j< MWIDTH/2; j++)
		{
			TCA1[i/2][j] = TA1[i][j] + TA1[i+1][j];
			TCH1[i/2][j] = TA1[i][j] - TA1[i+1][j];
			TCV1[i/2][j] = TA2[i][j] + TA2[i+1][j];
			TCD1[i/2][j] = TA2[i][j] - TA2[i+1][j];
		}
	}
}


void  CFormat::WaveletTrans2D_2()
{
	int i, j;

	for (i = 0; i < MHEIGHT/2; i++)
	{
		for(j = 0; j < MWIDTH/2; j = j+2)
		{
			TA1[i][j/2] = TCA1[i][j] + TCA1[i][j+1];
			TA2[i][j/2] = TCA1[i][j] - TCA1[i][j+1];
		}
	}

	for (i = 0; i < MHEIGHT/2; i = i+2)
	{
		for (j = 0; j< MWIDTH/4; j++)
		{
			TCA2[i/2][j] = TA1[i][j] + TA1[i+1][j];
			TCH2[i/2][j] = TA1[i][j] - TA1[i+1][j];
			TCV2[i/2][j] = TA2[i][j] + TA2[i+1][j];
			TCD2[i/2][j] = TA2[i][j] - TA2[i+1][j];
		}
	}
}


void  CFormat::WaveletTrans2D_3()
{
	int i, j;

	for (i = 0; i < MHEIGHT/4; i++)
	{
		for(j = 0; j < MWIDTH/4; j = j+2)
		{
			TA1[i][j/2] = (TCA2[i][j] + TCA2[i][j+1])/2;
		}
	}

	for (i = 0; i < MHEIGHT/4; i = i+2)
	{
		for (j = 0; j< MWIDTH/8; j++)
		{
			TCA3[i/2][j] = (TA1[i][j] + TA1[i+1][j])/2;
		}
	}
}


void  CFormat::WaveletTransI2D_2()
{
	short i,j;

	for(i = 0; i < MHEIGHT/4; i++)
	{
		for(j = 0; j < MWIDTH/4; j++)
		{
			TA1[2*i][j] = (TCA2[i][j] + TCH2[i][j])/2;
			TA1[2*i+1][j] = (TCA2[i][j] - TCH2[i][j])/2;
			TA2[2*i][j] = (TCV2[i][j] + TCD2[i][j])/2;
			TA2[2*i+1][j] = (TCV2[i][j] - TCD2[i][j])/2;
		}
	}

	for(i = 0; i < MHEIGHT/2; i++)
	{
		for(j = 0; j < MWIDTH/4; j++)
		{
			TCA1[i][2*j] = (TA1[i][j] + TA2[i][j])/2;
			TCA1[i][2*j+1] = (TA1[i][j] - TA2[i][j])/2;
		}
	}
}


void  CFormat::WaveletTransI2D_1()
{
	short i,j;

	for(i = 0; i < MHEIGHT/2; i++)
	{
		for(j = 0; j < MWIDTH/2; j++)
		{
			TA1[2*i][j] = (TCA1[i][j] + TCH1[i][j])/2;
			TA1[2*i+1][j] = (TCA1[i][j] - TCH1[i][j])/2;
			TA2[2*i][j] = (TCV1[i][j] + TCD1[i][j])/2;
			TA2[2*i+1][j] = (TCV1[i][j] - TCD1[i][j])/2;
		}
	}

	for(i = 0; i < MHEIGHT; i++)
	{
		for(j = 0; j < MWIDTH/2; j++)
		{
			Yw[i][2*j] = (TA1[i][j] + TA2[i][j])/2;
			Yw[i][2*j+1] = (TA1[i][j] - TA2[i][j])/2;
		}
	}
}
