#pragma once


// CModel �Ի���

class CModel : public CDialog
{
	DECLARE_DYNAMIC(CModel)

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);  //���ÿؼ�����͸��
	CBrush m_brush; 
	CModel(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CModel();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};
