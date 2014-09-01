
// testDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "Format.h"

// CtestDlg �Ի���
class CtestDlg : public CDialogEx
{
// ����
public:
	CtestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonGenerateAuto(); // �Զ�����ˮӡ��ť
	afx_msg void OnBnClickedButtonScan(); // ����ļ���ť
	afx_msg void OnBnClickedOk(); // ִ��Ƕ�밴ť
	afx_msg void OnBnClickedCancel(); // ��ֹ���˳���ť
	afx_msg void OnBnClickedButtonSave();
	CString m_Watermark; // ˮӡ
	CString m_Watermark_en;//ˮӡ����
	CString m_Src; // ԭʼ��Ƶ·��
	CString m_Screen; // ������ʾ
	CString m_SavePath; // ����·��
	CString m_Ext; // ����Ƶ�ļ�����չ��
	CProgressCtrl m_Progress; // ������
	CFormat Format;
private:
	bool GenerateAuto(CString,CString&); // ˮӡ�Զ����ɺ���
	bool WatermarkCheck(CString); // ˮӡ�Ϸ���⺯��
	bool JudgeFormat(CString); // �ж�������Ƶ��ʽ�Ƿ���ȷ
	bool StorageSpace(CString);//�ж���Ƶ����·�����ڴ��̿ռ��Ƿ񹻴�
	bool IfNeedChangeFormat(CString);//�ж�������Ƶ��ʽ�Ƿ���Ҫת��
	bool DirectoryExist(CString);//�ж�·���Ƿ����
	bool CreateDirectory(CString);//����·��
	void Reverse(CString);
};
 
