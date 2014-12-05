#pragma once


// CModel 对话框

class CModel : public CDialog
{
	DECLARE_DYNAMIC(CModel)

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);  //设置控件背景透明
	CBrush m_brush; 
	CModel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CModel();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};
