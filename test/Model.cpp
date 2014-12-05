// Model.cpp : 实现文件
//

#include "stdafx.h"
#include "test.h"
#include "Model.h"
#include "afxdialogex.h"


// CModel 对话框

IMPLEMENT_DYNAMIC(CModel, CDialog)

CModel::CModel(CWnd* pParent /*=NULL*/)
	: CDialog(CModel::IDD, pParent)
{

}

CModel::~CModel()
{
}

void CModel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CModel, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()  //设置控件背景透明
END_MESSAGE_MAP()

void CModel::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
	}
	else
	{
		CRect rect;
		CPaintDC dc(this); // 用于绘制的设备上下文
		GetClientRect(&rect);
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap bmpBackground;
		bmpBackground.LoadBitmap(IDB_BG_CHILD);
		BITMAP bitmap;
		bmpBackground.GetBitmap(&bitmap);
		CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground);
		dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
		//CDialogEx::OnPaint();
	}
}

//设置控件背景透明
HBRUSH CModel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)   
{  
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);  
	if( nCtlColor == CTLCOLOR_STATIC)       //设置静态框背景透明 
	{      
		pDC->SetBkMode(TRANSPARENT);   //设置背景透明   
		return HBRUSH(GetStockObject(HOLLOW_BRUSH));  
	}

	return hbr;  
}  
// CModel 消息处理程序
