
// 3infotechDlg.h: файл заголовка
//

#pragma once
#define _SILENCE_AMP_DEPRECATION_WARNINGS
#include "Drawer.h"

// Диалоговое окно CMy3infotechDlg
class CMy3infotechDlg : public CDialogEx
{
// Создание
public:
	CMy3infotechDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY3INFOTECH_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	double A;
	double f;
	double fi;
	double fd;
	int N;
	double a;
	int akpmin;
	int akpmax;
	CButton one_ex;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	CEdit min;
	CEdit max;
	CButton many_ex;
	afx_msg void OnBnClickedCancel();
	Drawer drvstart, drvshum, drvspec, drvotkl;
	afx_msg void OnBnClickedOk();
	// double sred;// double sred;
//	CEdit sred;
	CEdit sred;
	/*CButton pryamwnd;
	CButton triagwnd;
	CButton coswnd;*/
	/*afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();*/
	int typewnd;
	CButton pryamwnd;
	CButton triagwnd;
	CButton coswnd;
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
//	int num;
	bool flagpryam;
	bool flagtriag;
	bool flagcos;
	void SeachSpec(vector<double> a, vector<double>& b, vector<double>& c, signal s, int ak, int wnd, vector<double>& akp);
//	CString sredtriag;
	CEdit sredtriag;
	CEdit sredcos;
	CEdit colorsig;
	CEdit colorshum;
	CEdit colorpryam;
	CEdit colortriag;
	CEdit colorcos;
	CEdit colorspecpr;
	CEdit colorspectriag;
	CEdit colorspeccos;
	CEdit colorotklpr;
	CEdit colorotkltriag;
	CEdit colorotklcos;
	int num;
	CButton yeslog;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedButton1();
};
