#pragma once
#define _SILENCE_AMP_DEPRECATION_WARNINGS
#include <afxwin.h>
#include <vector>
#include <amp_graphics.h>
#include "Signal.h"

using namespace std;

class Drawer
{
	// ������������� ������� ���������.
	CRect frame;
	// ��������� �� ������ ����, ������� ���������.
	CWnd * wnd;
	// �������� ���������, ����������� � ������� ���������.
	CDC * dc;
	// �������� ���������, ����������� � ������.
	CDC memDC;
	// ������ ��� ��������� ��������� memDC.
	CBitmap bmp;
	// ���� ��� ������������ ��������� ������������� ������.
	bool init;
public:
	// ������������������� ������ ������ �� ������ HWND.
	void Create(HWND hWnd);

	vector<double> convert_range(vector <double>& data, double outmax, double outmin, double inmax, double inmin);

	double convert_range_d(double data, double outmax, double outmin, double inmax, double inmin);

	// ���������� ������.
	void DrawPoints(vector<double>& data, signal sig, CString Ox, CString Oy, double data_x_max, double step_x);

	double MaxF(vector<double> y);

	double MinF(vector<double> y);

	void DrawTwoSig(vector<double>& data, vector<double>& data1, signal sig, CString Ox, CString Oy, double data_x_max, double step_x);

	void DrawTwoFunc(vector<double>& data, signal sig, CString Ox, CString Oy, double data_x_max, double step_x);

	void DrawTwoFuncTwo(vector<double>& data, vector<double>& data1, signal sig, CString Ox, CString Oy, double data_x_max, double step_x);

	void DrawThreeSig(vector<double>& data, vector<double>& data1, vector<double>& data2, signal sig, CString Ox, CString Oy, double data_x_max, double step_x);

	void DrawTwoFuncThree(vector<double>& data, vector<double>& data1, vector<double>& data2, signal sig, CString Ox, CString Oy, double data_x_max, double step_x);

	void sredkv(vector<double>& data, signal sig, CString Ox, CString Oy, double data_x_min, double data_x_max, double step_x);

	void sredkvtwo(vector<double>& data, vector<double> data1, signal sig, CString Ox, CString Oy, double data_x_min, double data_x_max, double step_x);

	void sredkvthree(vector<double> data, vector<double> data1, vector<double> data2, signal sig, CString Ox, CString Oy, double data_x_min, double data_x_max, double step_x); // true - line, false - log

	void log3gr(vector<double> data, vector<double> data1, vector<double> data2, signal sig, CString Ox, CString Oy, double data_x_min, double data_x_max, double step_x);

	void log2gr(vector<double> data, vector<double> data1, signal sig, CString Ox, CString Oy, double data_x_min, double data_x_max, double step_x);

	void log1gr(vector<double> data, signal sig, CString Ox, CString Oy, double data_x_min, double data_x_max, double step_x);
};

