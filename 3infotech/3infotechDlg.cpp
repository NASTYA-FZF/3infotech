
// 3infotechDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "3infotech.h"
#include "3infotechDlg.h"
#include "afxdialogex.h"
#include "Signal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CMy3infotechDlg



CMy3infotechDlg::CMy3infotechDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MY3INFOTECH_DIALOG, pParent)
	, A(2)
	, f(0.2)
	, fi(0)
	, fd(1)
	, N(512)
	, a(0.5)
	, akpmin(1)
	, akpmax(27)
	, num(512)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy3infotechDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, A);
	DDX_Text(pDX, IDC_EDIT6, f);
	DDX_Text(pDX, IDC_EDIT3, fi);
	DDX_Text(pDX, IDC_EDIT4, fd);
	DDX_Text(pDX, IDC_EDIT7, N);
	DDX_Text(pDX, IDC_EDIT8, a);
	DDX_Text(pDX, IDC_EDIT2, akpmin);
	DDX_Text(pDX, IDC_EDIT5, akpmax);
	DDX_Control(pDX, IDC_RADIO2, one_ex);
	DDX_Control(pDX, IDC_EDIT2, min);
	DDX_Control(pDX, IDC_EDIT5, max);
	DDX_Control(pDX, IDC_RADIO1, many_ex);
	//  DDX_Text(pDX, IDC_EDIT9, sred);
	//DDX_Control(pDX, IDC_EDIT9, sred);
	DDX_Control(pDX, IDC_EDIT10, sred);
	/*DDX_Control(pDX, IDC_RADIO3, pryamwnd);
	DDX_Control(pDX, IDC_RADIO4, triagwnd);
	DDX_Control(pDX, IDC_RADIO5, coswnd);*/
	DDX_Control(pDX, IDC_CHECK2, pryamwnd);
	DDX_Control(pDX, IDC_CHECK3, triagwnd);
	DDX_Control(pDX, IDC_CHECK4, coswnd);
	//DDX_Text(pDX, IDC_EDIT11, num);
	//  DDX_Text(pDX, IDC_EDIT12, sredtriag);
	DDX_Control(pDX, IDC_EDIT12, sredtriag);
	DDX_Control(pDX, IDC_EDIT13, sredcos);
	DDX_Control(pDX, IDC_EDIT9, colorsig);
	DDX_Control(pDX, IDC_EDIT11, colorshum);
	DDX_Control(pDX, IDC_EDIT14, colorpryam);
	DDX_Control(pDX, IDC_EDIT15, colortriag);
	DDX_Control(pDX, IDC_EDIT16, colorcos);
	DDX_Control(pDX, IDC_EDIT17, colorspecpr);
	DDX_Control(pDX, IDC_EDIT18, colorspectriag);
	DDX_Control(pDX, IDC_EDIT19, colorspeccos);
	DDX_Control(pDX, IDC_EDIT20, colorotklpr);
	DDX_Control(pDX, IDC_EDIT21, colorotkltriag);
	DDX_Control(pDX, IDC_EDIT22, colorotklcos);
	DDX_Text(pDX, IDC_EDIT23, num);
	DDX_Control(pDX, IDC_CHECK1, yeslog);
}

BEGIN_MESSAGE_MAP(CMy3infotechDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RADIO1, &CMy3infotechDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CMy3infotechDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDCANCEL, &CMy3infotechDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CMy3infotechDlg::OnBnClickedOk)
	/*ON_BN_CLICKED(IDC_RADIO3, &CMy3infotechDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CMy3infotechDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &CMy3infotechDlg::OnBnClickedRadio5)*/
	ON_BN_CLICKED(IDC_CHECK2, &CMy3infotechDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CMy3infotechDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CMy3infotechDlg::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK1, &CMy3infotechDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON1, &CMy3infotechDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// Обработчики сообщений CMy3infotechDlg

BOOL CMy3infotechDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	many_ex.SetCheck(BST_CHECKED);
	pryamwnd.SetCheck(BST_CHECKED);
	//yeslog.SetCheck(BST_UNCHECKED);
	typewnd = 0;
	flagpryam = true;
	flagtriag = false;
	flagcos = false;
	drvstart.Create(GetDlgItem(IDC_STARTSIG)->GetSafeHwnd());

	drvshum.Create(GetDlgItem(IDC_SHUMSIG)->GetSafeHwnd());

	drvspec.Create(GetDlgItem(IDC_SPECTR)->GetSafeHwnd());

	drvotkl.Create(GetDlgItem(IDC_OTKL)->GetSafeHwnd());
	srand(time(NULL));
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CMy3infotechDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CMy3infotechDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy3infotechDlg::OnBnClickedRadio1()
{
	// TODO: добавьте свой код обработчика уведомлений
	one_ex.SetCheck(BST_UNCHECKED);
	many_ex.SetCheck(BST_CHECKED);
	min.SetReadOnly(0);
	max.SetReadOnly(0);
}


void CMy3infotechDlg::OnBnClickedRadio2()
{
	// TODO: добавьте свой код обработчика уведомлений
	one_ex.SetCheck(BST_CHECKED);
	many_ex.SetCheck(BST_UNCHECKED);
	min.SetReadOnly(1);
	max.SetReadOnly(1);
}


void CMy3infotechDlg::OnBnClickedCancel()
{
	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnCancel();
}

vector<double> /*funcpryam, functriag, funccos,*/ func, sko, skotriag, skocos;

void CMy3infotechDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(true);
	//int num = N;
	int numflag(0), numexper = 1000;
	double otkl = 0., otkltriag = 0., otklcos = 0.;
	signal sig(A, f, fi, fd, a, N, akpmin, akpmax);
	//vector<double> func, sko;
	vector<double> bfuncpryam, bfunctriag, bfunccos, specpryam, spectriag, speccos,  akppryam, akptriag, akpcos;
	//funcpryam.clear();
	//functriag.clear();
	//funccos.clear();
	func.clear();
	sko.clear();
	skotriag.clear();
	skocos.clear();

	sig.CreateSig(func);

	if (flagpryam)
	{
		//sig.CreateSig(funcpryam);
		numflag++;
	}

	if (flagtriag)
	{
		//sig.CreateSig(functriag);
		numflag++;
	}

	if (flagcos)
	{
		//sig.CreateSig(funccos);
		numflag++;
	}

	if (one_ex.GetCheck())
	{
		if (numflag == 0) MessageBox(L"Выберите окно", L"Ошибка", MB_OK);
		if (numflag == 1)
		{
			if (flagpryam)
			{
				SeachSpec(func, bfuncpryam, specpryam, sig, num, 0, akppryam);
				drvstart.DrawTwoSig(func, bfuncpryam, sig, L"t", L"x", N / fd, 1 / fd);
				drvshum.DrawPoints(akppryam, sig, L"t", L"x", N / fd, 1 / fd);
				//drvshum.DrawPoints(bfuncpryam, sig, L"t", L"x", N / fd, 1 / fd);
				drvspec.DrawTwoFunc(specpryam, sig, L"f", L"|X|", fd, fd / N);
				CString str;
				str.Format(L"%f", sig.SearchF(specpryam));
				sred.SetWindowTextW(str);
				sredtriag.SetWindowTextW(L"ничего");
				sredcos.SetWindowTextW(L"ничего");
				colorsig.SetWindowTextW(L"синий");
				colorshum.SetWindowTextW(L"красный");
				colorpryam.SetWindowTextW(L"синий");
				colortriag.SetWindowTextW(L"нет");
				colorcos.SetWindowTextW(L"нет");
				colorspecpr.SetWindowTextW(L"синий");
				colorspectriag.SetWindowTextW(L"нет");
				colorspeccos.SetWindowTextW(L"нет");
				colorotklpr.SetWindowTextW(L"нет");
				colorotkltriag.SetWindowTextW(L"нет");
				colorotklcos.SetWindowTextW(L"нет");
				UpdateData(false);
			}
			else if (flagtriag)
			{
				SeachSpec(func, bfunctriag, spectriag, sig, num, 1, akptriag);
				drvstart.DrawTwoSig(func, bfunctriag, sig, L"t", L"x", N / fd, 1 / fd);
				drvshum.DrawPoints(akptriag, sig, L"t", L"x", N / fd, 1 / fd);
				//drvshum.DrawPoints(bfunctriag, sig, L"t", L"x", N / fd, 1 / fd);
				drvspec.DrawTwoFunc(spectriag, sig, L"f", L"|X|", fd, fd / N);
				CString str;
				str.Format(L"%f", sig.SearchF(spectriag));
				sredtriag.SetWindowTextW(str);
				sred.SetWindowTextW(L"ничего");
				sredcos.SetWindowTextW(L"ничего");
				colorsig.SetWindowTextW(L"синий");
				colorshum.SetWindowTextW(L"красный");
				colorpryam.SetWindowTextW(L"нет");
				colortriag.SetWindowTextW(L"синий");
				colorcos.SetWindowTextW(L"нет");
				colorspecpr.SetWindowTextW(L"нет");
				colorspectriag.SetWindowTextW(L"синий");
				colorspeccos.SetWindowTextW(L"нет");
				colorotklpr.SetWindowTextW(L"нет");
				colorotkltriag.SetWindowTextW(L"нет");
				colorotklcos.SetWindowTextW(L"нет");
				UpdateData(false);
			}
			else
			{
				SeachSpec(func, bfunccos, speccos, sig, num, 2, akpcos);
				drvstart.DrawTwoSig(func, bfunccos, sig, L"t", L"x", N / fd, 1 / fd);
				drvshum.DrawPoints(akpcos, sig, L"t", L"x", N / fd, 1 / fd);
				//drvshum.DrawPoints(bfunccos, sig, L"t", L"x", N / fd, 1 / fd);
				drvspec.DrawTwoFunc(speccos, sig, L"f", L"|X|", fd, fd / N);
				CString str;
				str.Format(L"%f", sig.SearchF(speccos));
				sredcos.SetWindowTextW(str);
				sredtriag.SetWindowTextW(L"ничего");
				sred.SetWindowTextW(L"ничего");
				colorsig.SetWindowTextW(L"синий");
				colorshum.SetWindowTextW(L"красный");
				colorpryam.SetWindowTextW(L"нет");
				colortriag.SetWindowTextW(L"нет");
				colorcos.SetWindowTextW(L"синий");
				colorspecpr.SetWindowTextW(L"нет");
				colorspectriag.SetWindowTextW(L"нет");
				colorspeccos.SetWindowTextW(L"синий");
				colorotklpr.SetWindowTextW(L"нет");
				colorotkltriag.SetWindowTextW(L"нет");
				colorotklcos.SetWindowTextW(L"нет");
				UpdateData(false);
			}
		}
		if (numflag == 2)
		{
			if (flagpryam && flagtriag)
			{
				SeachSpec(func, bfuncpryam, specpryam, sig, num, 0, akppryam);
				SeachSpec(func, bfunctriag, spectriag, sig, num, 1, akptriag);
				drvstart.DrawTwoSig(func, bfunctriag, sig, L"t", L"x", N / fd, 1 / fd);
				drvshum.DrawTwoSig(akppryam, akptriag, sig, L"t", L"x", N / fd, 1 / fd);
				drvspec.DrawTwoFuncTwo(specpryam, spectriag, sig, L"f", L"|X|", fd, fd / N);
				CString str;
				str.Format(L"%f", sig.SearchF(specpryam));
				sred.SetWindowTextW(str);
				str.Format(L"%f", sig.SearchF(spectriag));
				sredtriag.SetWindowTextW(str);
				sredcos.SetWindowTextW(L"ничего");
				colorsig.SetWindowTextW(L"синий");
				colorshum.SetWindowTextW(L"красный");
				colorpryam.SetWindowTextW(L"синий");
				colortriag.SetWindowTextW(L"красный");
				colorcos.SetWindowTextW(L"нет");
				colorspecpr.SetWindowTextW(L"синий");
				colorspectriag.SetWindowTextW(L"зеленый");
				colorspeccos.SetWindowTextW(L"нет");
				colorotklpr.SetWindowTextW(L"нет");
				colorotkltriag.SetWindowTextW(L"нет");
				colorotklcos.SetWindowTextW(L"нет");
				UpdateData(false);
			}
			else if (flagpryam && flagcos)
			{
				SeachSpec(func, bfuncpryam, specpryam, sig, num, 0, akppryam);
				SeachSpec(func, bfunccos, speccos, sig, num, 2, akpcos);
				drvstart.DrawTwoSig(func, bfunccos, sig, L"t", L"x", N / fd, 1 / fd);
				drvshum.DrawTwoSig(akppryam, akpcos, sig, L"t", L"x", N / fd, 1 / fd);
				drvspec.DrawTwoFuncTwo(specpryam, speccos, sig, L"f", L"|X|", fd, fd / N);
				CString str;
				str.Format(L"%f", sig.SearchF(specpryam));
				sred.SetWindowTextW(str);
				str.Format(L"%f", sig.SearchF(speccos));
				sredcos.SetWindowTextW(str);
				sredtriag.SetWindowTextW(L"ничего");
				colorsig.SetWindowTextW(L"синий");
				colorshum.SetWindowTextW(L"красный");
				colorpryam.SetWindowTextW(L"синий");
				colortriag.SetWindowTextW(L"нет");
				colorcos.SetWindowTextW(L"красный");
				colorspecpr.SetWindowTextW(L"синий");
				colorspectriag.SetWindowTextW(L"нет");
				colorspeccos.SetWindowTextW(L"зеленый");
				colorotklpr.SetWindowTextW(L"нет");
				colorotkltriag.SetWindowTextW(L"нет");
				colorotklcos.SetWindowTextW(L"нет");
				UpdateData(false);
			}
			else
			{
				SeachSpec(func, bfunccos, speccos, sig, num, 2, akpcos);
				SeachSpec(func, bfunctriag, spectriag, sig, num, 1, akptriag);
				drvstart.DrawTwoSig(func, bfunctriag, sig, L"t", L"x", N / fd, 1 / fd);
				drvshum.DrawTwoSig(akpcos, akptriag, sig, L"t", L"x", N / fd, 1 / fd);
				drvspec.DrawTwoFuncTwo(speccos, spectriag, sig, L"f", L"|X|", fd, fd / N);
				CString str;
				str.Format(L"%f", sig.SearchF(speccos));
				sredcos.SetWindowTextW(str);
				str.Format(L"%f", sig.SearchF(spectriag));
				sredtriag.SetWindowTextW(str);
				sred.SetWindowTextW(L"ничего");
				colorsig.SetWindowTextW(L"синий");
				colorshum.SetWindowTextW(L"красный");
				colorpryam.SetWindowTextW(L"нет");
				colortriag.SetWindowTextW(L"красный");
				colorcos.SetWindowTextW(L"синий");
				colorspecpr.SetWindowTextW(L"нет");
				colorspectriag.SetWindowTextW(L"зеленый");
				colorspeccos.SetWindowTextW(L"синий");
				colorotklpr.SetWindowTextW(L"нет");
				colorotkltriag.SetWindowTextW(L"нет");
				colorotklcos.SetWindowTextW(L"нет");
				UpdateData(false);
			}
		}
		if (numflag == 3)
		{
			SeachSpec(func, bfuncpryam, specpryam, sig, num, 0, akppryam);
			SeachSpec(func, bfunctriag, spectriag, sig, num, 1, akptriag);
			SeachSpec(func, bfunccos, speccos, sig, num, 2, akpcos);
			drvstart.DrawTwoSig(func, bfunctriag, sig, L"t", L"x", N / fd, 1 / fd);
			drvshum.DrawThreeSig(akppryam, akpcos, akptriag, sig, L"t", L"x", N / fd, 1 / fd);
			drvspec.DrawTwoFuncThree(specpryam, speccos, spectriag, sig, L"f", L"|X|", fd, fd / N);
			CString str;
			str.Format(L"%f", sig.SearchF(speccos));
			sredcos.SetWindowTextW(str);
			str.Format(L"%f", sig.SearchF(spectriag));
			sredtriag.SetWindowTextW(str);
			str.Format(L"%f", sig.SearchF(specpryam));
			sred .SetWindowTextW(str);
			colorsig.SetWindowTextW(L"синий");
			colorshum.SetWindowTextW(L"красный");
			colorpryam.SetWindowTextW(L"синий");
			colortriag.SetWindowTextW(L"зеленый");
			colorcos.SetWindowTextW(L"красный");
			colorspecpr.SetWindowTextW(L"синий");
			colorspectriag.SetWindowTextW(L"коричневый");
			colorspeccos.SetWindowTextW(L"зеленый");
			colorotklpr.SetWindowTextW(L"нет");
			colorotkltriag.SetWindowTextW(L"нет");
			colorotklcos.SetWindowTextW(L"нет");
			UpdateData(false);
		}
	}

	else
	{
		if (numflag == 0) MessageBox(L"Выберите окно", L"Ошибка", MB_OK);
		if (numflag == 1)
		{
			if (flagpryam)
			{
				colorotklpr.SetWindowTextW(L"синий");
				colorotkltriag.SetWindowTextW(L"нет");
				colorotklcos.SetWindowTextW(L"нет");
				UpdateData(false);
				for (int kol = akpmin; kol < akpmax + 1; kol++)
				{
					otkl = 0;
					for (int exper = 0; exper < numexper; exper++)
					{
						sig.CreateShum();
						bfuncpryam.clear();
						specpryam.clear();
						SeachSpec(func, bfuncpryam, specpryam, sig, kol, 0, akppryam);
						otkl += sig.SearchF(specpryam) * sig.SearchF(specpryam);
						UpdateData(false);
					}
					sko.push_back(sqrt(otkl / numexper));
					if (!yeslog.GetCheck()) drvotkl.sredkv(sko, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
					else drvotkl.log1gr(sko, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
				}
			}
			else if (flagtriag)
			{
				colorotklpr.SetWindowTextW(L"нет");
				colorotkltriag.SetWindowTextW(L"синий");
				colorotklcos.SetWindowTextW(L"нет");
				UpdateData(false);
				for (int kol = akpmin; kol < akpmax + 1; kol++)
				{
					//sig.CreateSig(functriag, 1, kol);
					otkl = 0;
					for (int exper = 0; exper < numexper; exper++)
					{
						sig.CreateShum();
						bfunctriag.clear();
						spectriag.clear();
						SeachSpec(func, bfunctriag, spectriag, sig, kol, 1, akptriag);
						otkl += sig.SearchF(spectriag) * sig.SearchF(spectriag);
						UpdateData(false);
					}
					skotriag.push_back(sqrt(otkl / numexper));
					if (!yeslog.GetCheck()) drvotkl.sredkv(skotriag, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
					else drvotkl.log1gr(skotriag, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
				}
				
			}
			else
			{
				colorotklpr.SetWindowTextW(L"нет");
				colorotkltriag.SetWindowTextW(L"нет");
				colorotklcos.SetWindowTextW(L"синий");
				UpdateData(false);
				for (int kol = akpmin; kol < akpmax + 1; kol++)
				{
					otkl = 0;
					for (int exper = 0; exper < numexper; exper++)
					{
						sig.CreateShum();
						bfunccos.clear();
						speccos.clear();
						SeachSpec(func, bfunccos, speccos, sig, kol, 2, akpcos);
						otkl += sig.SearchF(speccos) * sig.SearchF(speccos);
						UpdateData(false);
					}
					skocos.push_back(sqrt(otkl / numexper));
					if (!yeslog.GetCheck()) drvotkl.sredkv(skocos, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
					else drvotkl.log1gr(skocos, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
				}
				
			}
		}
		if (numflag == 2)
		{
			if (flagpryam && flagtriag)
			{
				colorotklpr.SetWindowTextW(L"синий");
				colorotkltriag.SetWindowTextW(L"красный");
				colorotklcos.SetWindowTextW(L"нет");
				UpdateData(false);
				for (int kol = akpmin; kol < akpmax + 1; kol++)
				{
					otkl = 0.;
					otkltriag = 0.;
					for (int exper = 0; exper < numexper; exper++)
					{
						sig.CreateShum();
						bfuncpryam.clear();
						specpryam.clear();
						bfunctriag.clear();
						spectriag.clear();
						SeachSpec(func, bfuncpryam, specpryam, sig, kol, 0, akppryam);
						SeachSpec(func, bfunctriag, spectriag, sig, kol, 1, akptriag);
						otkl += sig.SearchF(specpryam) * sig.SearchF(specpryam);
						otkltriag += sig.SearchF(spectriag) * sig.SearchF(spectriag);
						UpdateData(false);
					}
					sko.push_back(sqrt(otkl / numexper));
					skotriag.push_back(sqrt(otkltriag / numexper));
					if (!yeslog.GetCheck()) drvotkl.sredkvtwo(sko, skotriag, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
					else drvotkl.log2gr(sko, skotriag, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
				}
				
			}
			else if (flagpryam && flagcos)
			{
				colorotklpr.SetWindowTextW(L"синий");
				colorotkltriag.SetWindowTextW(L"нет");
				colorotklcos.SetWindowTextW(L"красный");
				UpdateData(false);
				for (int kol = akpmin; kol < akpmax + 1; kol++)
				{
					otkl = 0.;
					otklcos = 0.;
					for (int exper = 0; exper < numexper; exper++)
					{
						sig.CreateShum();
						bfuncpryam.clear();
						specpryam.clear();
						bfunccos.clear();
						speccos.clear();
						SeachSpec(func, bfuncpryam, specpryam, sig, kol, 0, akppryam);
						SeachSpec(func, bfunccos, speccos, sig, kol, 2, akpcos);
						otkl += sig.SearchF(specpryam) * sig.SearchF(specpryam);
						otklcos += sig.SearchF(speccos) * sig.SearchF(speccos);
						UpdateData(false);
					}
					sko.push_back(sqrt(otkl / numexper));
					skocos.push_back(sqrt(otklcos / numexper));
					if (!yeslog.GetCheck()) drvotkl.sredkvtwo(sko, skocos, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
					else drvotkl.log2gr(sko, skocos, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
				}
				
			}
			else
			{
			colorotklpr.SetWindowTextW(L"нет");
			colorotkltriag.SetWindowTextW(L"синий");
			colorotklcos.SetWindowTextW(L"красный");
			UpdateData(false);
				for (int kol = akpmin; kol < akpmax + 1; kol++)
				{
					otkltriag = 0.;
					otklcos = 0.;
					for (int exper = 0; exper < numexper; exper++)
					{
						sig.CreateShum();
						bfunctriag.clear();
						spectriag.clear();
						bfunccos.clear();
						speccos.clear();
						SeachSpec(func, bfunccos, speccos, sig, kol, 2, akpcos);
						SeachSpec(func, bfunctriag, spectriag, sig, kol, 1, akptriag);
						otkltriag += sig.SearchF(spectriag) * sig.SearchF(spectriag);
						otklcos += sig.SearchF(speccos) * sig.SearchF(speccos);
						UpdateData(false);
					}
					skotriag.push_back(sqrt(otkltriag / numexper));
					skocos.push_back(sqrt(otklcos / numexper));
					if (!yeslog.GetCheck()) drvotkl.sredkvtwo(skotriag, skocos, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
					else drvotkl.log2gr(skotriag, skocos, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
				}
			}
		}
		if (numflag == 3)
		{
			colorotklpr.SetWindowTextW(L"зеленый");
			colorotkltriag.SetWindowTextW(L"синий");
			colorotklcos.SetWindowTextW(L"красный");
			UpdateData(false);
			for (int kol = akpmin; kol < akpmax + 1; kol++)
			{
				otkltriag = 0.;
				otklcos = 0.;
				otkl = 0.;
				for (int exper = 0; exper < numexper; exper++)
				{
					sig.CreateShum();
					bfunctriag.clear();
					spectriag.clear();
					bfunccos.clear();
					speccos.clear();
					bfuncpryam.clear();
					specpryam.clear();
					SeachSpec(func, bfuncpryam, specpryam, sig, kol, 0, akppryam);
					SeachSpec(func, bfunctriag, spectriag, sig, kol, 1, akptriag);
					SeachSpec(func, bfunccos, speccos, sig, kol, 2, akpcos);
					otkltriag += sig.SearchF(spectriag) * sig.SearchF(spectriag);
					otklcos += sig.SearchF(speccos) * sig.SearchF(speccos);
					otkl += sig.SearchF(specpryam) * sig.SearchF(specpryam);
					UpdateData(false);
				}
				skotriag.push_back(sqrt(otkltriag / numexper));
				skocos.push_back(sqrt(otklcos / numexper));
				sko.push_back(sqrt(otkl / numexper));
				if (!yeslog.GetCheck()) drvotkl.sredkvthree(skotriag, skocos, sko, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
				else drvotkl.log3gr(skotriag, skocos, sko, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
			}
		}
	}
}

void CMy3infotechDlg::OnBnClickedCheck2()
{
	// TODO: добавьте свой код обработчика уведомлений
	/*if (pryamwnd.GetCheck()) pryamwnd.SetCheck(BST_UNCHECKED);
	else pryamwnd.SetCheck(BST_CHECKED);*/
	typewnd = 0;
	flagpryam = !flagpryam;
}

void CMy3infotechDlg::OnBnClickedCheck3()
{
	// TODO: добавьте свой код обработчика уведомлений
	/*if (triagwnd.GetCheck()) triagwnd.SetCheck(BST_UNCHECKED);
	else triagwnd.SetCheck(BST_CHECKED);*/
	typewnd = 1;
	flagtriag = !flagtriag;
}

void CMy3infotechDlg::OnBnClickedCheck4()
{
	// TODO: добавьте свой код обработчика уведомлений
	/*if (coswnd.GetCheck()) coswnd.SetCheck(BST_UNCHECKED);
	else coswnd.SetCheck(BST_CHECKED);*/
	typewnd = 2;
	flagcos = !flagcos;
}

void CMy3infotechDlg::SeachSpec(vector<double> a, vector<double>& b, vector<double>& c, signal sig, int ak, int wnd, vector<double>& akp)
{
	if (akp.size() != 0) akp.clear();

	sig.bad_f(a, b);

	sig.GetAKP(b, akp, 0, ak, wnd);
	//if (wnd != 0) sig.MultiWnd(akp, wnd);
	vector<base> specakp;
	for (int i = 0; i < akp.size(); i++)
	{
		base zn(akp[i], 0.);
		specakp.push_back(zn);
	}
	sig.fft(specakp, true);

	for (int i = 0; i < specakp.size(); i++)
	{
		c.push_back(sqrt(specakp[i].real() * specakp[i].real() + specakp[i].imag() * specakp[i].imag()));
	}
}

void CMy3infotechDlg::OnBnClickedCheck1()
{
	// TODO: добавьте свой код обработчика уведомлений
	/*if (yeslog.GetCheck()) yeslog.SetCheck(BST_UNCHECKED);
	else yeslog.SetCheck(BST_CHECKED);*/
}


void CMy3infotechDlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(true);
	signal sig(A, f, fi, fd, a, N, akpmin, akpmax);
	int numflag(0);
	if (sko.size() != 0) numflag++;
	if (skotriag.size() != 0) numflag++;
	if (skocos.size() != 0) numflag++;

	if (numflag == 0) MessageBox(L"Посчитайте", L"Ошибка", MB_OK);
	if (numflag == 1)
	{
		if (sko.size() != 0)
		{
			if (!yeslog.GetCheck()) drvotkl.sredkv(sko, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
			else drvotkl.log1gr(sko, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
		}
		else if (skotriag.size() != 0)
		{
			if (!yeslog.GetCheck()) drvotkl.sredkv(skotriag, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
			else drvotkl.log1gr(skotriag, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
		}
		else
		{
			if (!yeslog.GetCheck()) drvotkl.sredkv(skocos, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
			else drvotkl.log1gr(skocos, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
		}
	}
	if (numflag == 2)
	{
		if (sko.size() != 0 && skotriag.size() != 0)
		{
			if (!yeslog.GetCheck()) drvotkl.sredkvtwo(sko, skotriag, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
			else drvotkl.log2gr(sko, skotriag, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
		}
		else if (sko.size() != 0 && skocos.size() != 0) 
		{
			if (!yeslog.GetCheck()) drvotkl.sredkvtwo(sko, skocos, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
			else drvotkl.log2gr(sko, skocos, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
		}
		else
		{
			if (!yeslog.GetCheck()) drvotkl.sredkvtwo(skotriag, skocos, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
			else drvotkl.log2gr(skotriag, skocos, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
		}
	}
	if (numflag == 3)
	{
		if (!yeslog.GetCheck()) drvotkl.sredkvthree(skotriag, skocos, sko, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
		else drvotkl.log3gr(skotriag, skocos, sko, sig, L"m", L"f", akpmin, akpmax - akpmin, 1.);
	}
}
