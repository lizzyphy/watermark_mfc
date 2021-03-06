
// testDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "Format.h"
#include "afxwin.h"

// CtestDlg 对话框
class CtestDlg : public CDialogEx
{
// 构造
public:
	CtestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonGenerateAuto(); // 自动生成水印按钮
	afx_msg void OnBnClickedButtonScan(); // 浏览文件按钮
	afx_msg void OnBnClickedOk(); // 执行嵌入按钮
	afx_msg void OnBnClickedCancel(); // 中止或退出按钮
	afx_msg void OnBnClickedButtonSave();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);  //设置控件背景透明
	CBrush m_brush; 
	CString m_Watermark; // 水印
	CString m_Watermark_en;//水印编码
	CString m_Src; // 原始视频路径
	CString m_Screen; // 进程显示
	CString m_SavePath; // 保存路径
	CString m_Ext; // 打开视频文件的扩展名
	CProgressCtrl m_Progress; // 进度条
	CFormat Format;
private:
	bool GenerateAuto(CString,CString&); // 水印自动生成函数
	bool WatermarkCheck(CString,CString); // 水印合法检测函数
	bool JudgeFormat(CString); // 判断输入视频格式是否正确
	bool StorageSpace(CString);//判断视频保存路径所在磁盘空间是否够大
	bool IfNeedChangeFormat(CString);//判断输入视频格式是否需要转换
	bool DirectoryExist(CString);//判断路径是否存在
	bool CreateDirectory(CString);//创建路径
	void Reverse(CString);
	char* CStochar(CString);
	void CS16toCS2(CString,CString&); //十六进制转二进制   
	bool Finallydel(CString);
	CString chartoCS(char*);      //char*类型转换为CString类型
	int Readframe(char*);
	void Show_now();
	CString HTTP_GET_DATA(CString);//利用HTTP协议联网判断水印是否合法
public:
	//afx_msg void OnEnChangeEditWm();
	
};
 
