#include "pch.h"
#include "Drawer.h"

void Drawer::Create(HWND hWnd)
{
	// Получаем указатель на окно.
	wnd = CWnd::FromHandle(hWnd);
	// Получаем прямоугольник окна.
	wnd->GetClientRect(frame);
	// Получаем контекст для рисования в этом окне.
	dc = wnd->GetDC();

	// Создаем буфер-контекст.
	memDC.CreateCompatibleDC(dc);
	// Создаем растр для контекста рисования.
	bmp.CreateCompatibleBitmap(dc, frame.Width(), frame.Height());
	// Выбираем растр для использования буфер-контекстом.
	memDC.SelectObject(&bmp);
	init = true;
}

vector<double> Drawer::convert_range(vector<double>& data, double outmax, double outmin, double inmax, double inmin)
{
	vector<double> output = data;
	double k = (outmax - outmin) / (inmax - inmin);
	for (auto& item : output)
	{
		item = (item - inmin) * k + outmin;
	}

	return output;
}

double Drawer::convert_range_d(double data, double outmax, double outmin, double inmax, double inmin)
{
	double output = data;
	double k = (outmax - outmin) / (inmax - inmin);

	return (output - inmin) * k + outmin;
}

void Drawer::DrawPoints(vector<double>& data, signal s, CString Ox, CString Oy, double data_x_max, double step_x)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen points_pen(PS_SOLID, 2, RGB(0, 0, 255));
	CBrush points_brush;
	points_brush.CreateSolidBrush(RGB(0, 0, 255));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
		);

	int padding = 40;
	int left_keys_padding = 15;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	double data_y_max, data_y_min;
	if ((MaxF(data)) != 0 || (MinF(data)) != 0)
	{
		data_y_max = MaxF(data);
		data_y_min = MinF(data);
	}
	else
	{
		data_y_max = 0.5;
		data_y_min = -0.5;
	}
	double data_x_min(0);

	if (data.empty()) return;

	memDC.SelectObject(&points_pen);
	memDC.SelectObject(&points_brush);

	vector<double> y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(0, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i /*/ s.Fd*/ * step_x, actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	CString str;
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{
		
		str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}
	//str.Format(L"x");
	memDC.TextOutW(actual_left - 10, actual_top - 30, Oy);

	//str.Format(L"t");
	memDC.TextOutW(actual_right + 20, actual_bottom + 20, Ox);

	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}

double Drawer::MaxF(vector<double> y)
{
	double max = y[0], pr;
	for (int i = 1; i < y.size(); i++)
	{
		if (y[i] > max)
		{
			pr = max;
			max = y[i];
			y[i] = pr;
		}
	}

	return max;
}

double Drawer::MinF(vector<double> y)
{
	double min = y[0], pr;
	for (int i = 1; i < y.size(); i++)
	{
		if (y[i] < min)
		{
			pr = min;
			min = y[i];
			y[i] = pr;
		}
	}

	return min;
}

void Drawer::DrawTwoSig(vector<double>& data, vector<double>& data1, signal s, CString Ox, CString Oy, double data_x_max, double step_x)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen one_pen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen two_pen(PS_SOLID, 2, RGB(255, 0, 0));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 40;
	int left_keys_padding = 15;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	double data_y_max/*(MaxF(data))*/, data_y_min/*(MinF(data))*/;
	if (MaxF(data) > MaxF(data1)) data_y_max = MaxF(data);
	else data_y_max = MaxF(data1);
	if (MinF(data) > MinF(data1)) data_y_min = MinF(data1);
	else data_y_min = MinF(data);
	double data_x_min(0)/*, data_x_max(s.Fd)*/;

	if (data.empty()) return;

	memDC.SelectObject(&one_pen);

	vector<double> y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(0, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i * /*s.Fd / s.n*/ step_x , actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&two_pen);

	y = convert_range(data1, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(0, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i * /*s.Fd / s.n*/ step_x , actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	CString str;
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{

		str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}
	//str.Format(L"x");
	memDC.TextOutW(actual_left - 10, actual_top - 30, Oy);

	//str.Format(L"t");
	memDC.TextOutW(actual_right + 20, actual_bottom + 20, Ox);

	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}

void Drawer::DrawTwoFunc(vector<double>& data, signal sig, CString Ox, CString Oy, double data_x_max, double step_x)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen points_pen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen two_pen(PS_SOLID, 2, RGB(255, 0, 0));
	CBrush points_brush;
	points_brush.CreateSolidBrush(RGB(0, 0, 255));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 40;
	int left_keys_padding = 15;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	double data_y_max, data_y_min;
	if ((MaxF(data)) != 0 || (MinF(data)) != 0)
	{
		data_y_max = MaxF(data);
		data_y_min = MinF(data);
	}
	else
	{
		data_y_max = 0.5;
		data_y_min = -0.5;
	}
	double data_x_min(0);

	if (data.empty()) return;

	memDC.SelectObject(&points_pen);
	memDC.SelectObject(&points_brush);

	vector<double> y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(0, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i /*/ s.Fd*/ * step_x, actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&two_pen);

	memDC.MoveTo(convert_range_d(sig.GetF(), actual_right, actual_left, data_x_max, data_x_min), convert_range_d(data_y_max, actual_top, actual_bottom, data_y_max, data_y_min));
	memDC.LineTo(convert_range_d(sig.GetF(), actual_right, actual_left, data_x_max, data_x_min), convert_range_d(0, actual_top, actual_bottom, data_y_max, data_y_min));

	memDC.MoveTo(convert_range_d(sig.GetFD() - sig.GetF(), actual_right, actual_left, data_x_max, data_x_min), convert_range_d(data_y_max, actual_top, actual_bottom, data_y_max, data_y_min));
	memDC.LineTo(convert_range_d(sig.GetFD() - sig.GetF(), actual_right, actual_left, data_x_max, data_x_min), convert_range_d(0, actual_top, actual_bottom, data_y_max, data_y_min));

	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	CString str;
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{

		str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}
	//str.Format(L"x");
	memDC.TextOutW(actual_left - 10, actual_top - 30, Oy);

	//str.Format(L"t");
	memDC.TextOutW(actual_right + 20, actual_bottom + 20, Ox);

	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}

void Drawer::DrawTwoFuncTwo(vector<double>& data, vector<double>& data1, signal sig, CString Ox, CString Oy, double data_x_max, double step_x)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen points_pen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen two_pen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen three_pen(PS_SOLID, 2, RGB(0, 255, 0));
	CBrush points_brush;
	points_brush.CreateSolidBrush(RGB(0, 0, 255));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 40;
	int left_keys_padding = 15;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	double data_y_max, data_y_min;
	/*if ((MaxF(data)) != 0 || (MinF(data)) != 0)
	{
		data_y_max = MaxF(data);
		data_y_min = MinF(data);
	}
	else
	{
		data_y_max = 0.5;
		data_y_min = -0.5;
	}*/
	if (MaxF(data) > MaxF(data1)) data_y_max = MaxF(data);
	else data_y_max = MaxF(data1);
	if (MinF(data) > MinF(data1)) data_y_min = MinF(data1);
	else data_y_min = MinF(data);
	double data_x_min(0);

	if (data.empty()) return;

	memDC.SelectObject(&points_pen);
	memDC.SelectObject(&points_brush);

	vector<double> y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(0, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i /*/ s.Fd*/ * step_x, actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&three_pen);

	y = convert_range(data1, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(0, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i /*/ s.Fd*/ * step_x, actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&two_pen);

	memDC.MoveTo(convert_range_d(sig.GetF(), actual_right, actual_left, data_x_max, data_x_min), convert_range_d(data_y_max, actual_top, actual_bottom, data_y_max, data_y_min));
	memDC.LineTo(convert_range_d(sig.GetF(), actual_right, actual_left, data_x_max, data_x_min), convert_range_d(0, actual_top, actual_bottom, data_y_max, data_y_min));

	memDC.MoveTo(convert_range_d(sig.GetFD() - sig.GetF(), actual_right, actual_left, data_x_max, data_x_min), convert_range_d(data_y_max, actual_top, actual_bottom, data_y_max, data_y_min));
	memDC.LineTo(convert_range_d(sig.GetFD() - sig.GetF(), actual_right, actual_left, data_x_max, data_x_min), convert_range_d(0, actual_top, actual_bottom, data_y_max, data_y_min));

	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	CString str;
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{

		str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}
	//str.Format(L"x");
	memDC.TextOutW(actual_left - 10, actual_top - 30, Oy);

	//str.Format(L"t");
	memDC.TextOutW(actual_right + 20, actual_bottom + 20, Ox);

	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}

void Drawer::DrawThreeSig(vector<double>& data, vector<double>& data1, vector<double>& data2, signal sig, CString Ox, CString Oy, double data_x_max, double step_x)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen one_pen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen two_pen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen three_pen(PS_SOLID, 2, RGB(0, 255, 0));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 40;
	int left_keys_padding = 15;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	double data_y_max/*(MaxF(data))*/, data_y_min/*(MinF(data))*/;
	if (MaxF(data) > MaxF(data1))
		if (MaxF(data) > MaxF(data2)) data_y_max = MaxF(data);
		else if (MaxF(data2) > MaxF(data1))data_y_max = MaxF(data2);
		else data_y_max = MaxF(data1);
	else if (MaxF(data1) > MaxF(data2)) data_y_max = MaxF(data1);
	else data_y_max = MaxF(data2);
	if (MinF(data) > MinF(data1))
		if (MinF(data1) < MinF(data2)) data_y_min = MinF(data1);
		else if (MinF(data) > MinF(data2))data_y_min = MinF(data2);
		else data_y_min = MinF(data);
	else if (MinF(data) < MinF(data2)) data_y_min = MinF(data);
	else data_y_min = MinF(data2);
	double data_x_min(0)/*, data_x_max(s.Fd)*/;

	if (data.empty()) return;

	memDC.SelectObject(&one_pen);

	vector<double> y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(0, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i * /*s.Fd / s.n*/ step_x, actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&two_pen);

	y = convert_range(data1, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(0, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i * /*s.Fd / s.n*/ step_x, actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&three_pen);

	y = convert_range(data2, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(0, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i /*/ s.Fd*/ * step_x, actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	CString str;
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{

		str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}
	//str.Format(L"x");
	memDC.TextOutW(actual_left - 10, actual_top - 30, Oy);

	//str.Format(L"t");
	memDC.TextOutW(actual_right + 20, actual_bottom + 20, Ox);

	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}

void Drawer::DrawTwoFuncThree(vector<double>& data, vector<double>& data1, vector<double>& data2, signal sig, CString Ox, CString Oy, double data_x_max, double step_x)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen points_pen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen two_pen(PS_SOLID, 1, RGB(255, 0, 0));
	CPen three_pen(PS_SOLID, 2, RGB(0, 255, 0));
	CPen four_pen(PS_SOLID, 2, RGB(150, 100, 100));
	CBrush points_brush;
	points_brush.CreateSolidBrush(RGB(0, 0, 255));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 40;
	int left_keys_padding = 15;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	double data_y_max, data_y_min;
	/*if ((MaxF(data)) != 0 || (MinF(data)) != 0)
	{
		data_y_max = MaxF(data);
		data_y_min = MinF(data);
	}
	else
	{
		data_y_max = 0.5;
		data_y_min = -0.5;
	}*/
	if (MaxF(data) > MaxF(data1))
		if (MaxF(data) > MaxF(data2)) data_y_max = MaxF(data);
		else if (MaxF(data2) > MaxF(data1))data_y_max = MaxF(data2);
		else data_y_max = MaxF(data1);
	else if (MaxF(data1) > MaxF(data2)) data_y_max = MaxF(data1);
	else data_y_max = MaxF(data2);
	if (MinF(data) > MinF(data1))
		if (MinF(data1) < MinF(data2)) data_y_min = MinF(data1);
		else if (MinF(data) > MinF(data2))data_y_min = MinF(data2);
		else data_y_min = MinF(data);
	else if (MinF(data) < MinF(data2)) data_y_min = MinF(data);
	else data_y_min = MinF(data2);
	double data_x_min(0);

	if (data.empty()) return;

	memDC.SelectObject(&points_pen);
	memDC.SelectObject(&points_brush);

	vector<double> y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(0, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i /*/ s.Fd*/ * step_x, actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&three_pen);

	y = convert_range(data1, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(0, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i /*/ s.Fd*/ * step_x, actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&four_pen);

	y = convert_range(data2, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(0, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i /*/ s.Fd*/ * step_x, actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&two_pen);

	memDC.MoveTo(convert_range_d(sig.GetF(), actual_right, actual_left, data_x_max, data_x_min), convert_range_d(data_y_max, actual_top, actual_bottom, data_y_max, data_y_min));
	memDC.LineTo(convert_range_d(sig.GetF(), actual_right, actual_left, data_x_max, data_x_min), convert_range_d(0, actual_top, actual_bottom, data_y_max, data_y_min));

	memDC.MoveTo(convert_range_d(sig.GetFD() - sig.GetF(), actual_right, actual_left, data_x_max, data_x_min), convert_range_d(data_y_max, actual_top, actual_bottom, data_y_max, data_y_min));
	memDC.LineTo(convert_range_d(sig.GetFD() - sig.GetF(), actual_right, actual_left, data_x_max, data_x_min), convert_range_d(0, actual_top, actual_bottom, data_y_max, data_y_min));

	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	CString str;
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{

		str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}
	//str.Format(L"x");
	memDC.TextOutW(actual_left - 10, actual_top - 30, Oy);

	//str.Format(L"t");
	memDC.TextOutW(actual_right + 20, actual_bottom + 20, Ox);

	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}

void Drawer::sredkv(vector<double>& data, signal sig, CString Ox, CString Oy, double data_x_min, double data_x_max, double step_x)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen points_pen(PS_SOLID, 2, RGB(0, 0, 255));
	CBrush points_brush;
	points_brush.CreateSolidBrush(RGB(0, 0, 255));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 40;
	int left_keys_padding = 15;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	double data_y_max, data_y_min;
	if ((MaxF(data)) != 0 || (MinF(data)) != 0)
	{
		data_y_max = MaxF(data);
		data_y_min = MinF(data);
	}
	else
	{
		data_y_max = 0.5;
		data_y_min = -0.5;
	}

	if (data.empty()) return;

	memDC.SelectObject(&points_pen);
	memDC.SelectObject(&points_brush);

	vector<double> y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(data_x_min, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i /*/ s.Fd*/ * step_x, actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	CString str;
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{

		str.Format(L"%.0f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.4f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}
	//str.Format(L"x");
	memDC.TextOutW(actual_left - 10, actual_top - 30, Oy);

	//str.Format(L"t");
	memDC.TextOutW(actual_right + 20, actual_bottom + 20, Ox);

	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}

void Drawer::sredkvtwo(vector<double>& data, vector<double> data1, signal sig, CString Ox, CString Oy, double data_x_min, double data_x_max, double step_x)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen one_pen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen two_pen(PS_SOLID, 2, RGB(255, 0, 0));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 40;
	int left_keys_padding = 15;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	double data_y_max/*(MaxF(data))*/, data_y_min/*(MinF(data))*/;
	if (MaxF(data) > MaxF(data1)) data_y_max = MaxF(data);
	else data_y_max = MaxF(data1);
	if (MinF(data) > MinF(data1)) data_y_min = MinF(data1);
	else data_y_min = MinF(data);

	if (data.empty()) return;

	memDC.SelectObject(&one_pen);

	vector<double> y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(data_x_min, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i * /*s.Fd / s.n*/ step_x, actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&two_pen);

	y = convert_range(data1, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(data_x_min, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i * /*s.Fd / s.n*/ step_x, actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	CString str;
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{

		str.Format(L"%.0f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.4f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}
	//str.Format(L"x");
	memDC.TextOutW(actual_left - 10, actual_top - 30, Oy);

	//str.Format(L"t");
	memDC.TextOutW(actual_right + 20, actual_bottom + 20, Ox);

	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}

void Drawer::sredkvthree(vector<double> data, vector<double> data1, vector<double> data2, signal sig, CString Ox, CString Oy, double data_x_min, double data_x_max, double step_x) // true - line, false - log
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen one_pen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen two_pen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen three_pen(PS_SOLID, 2, RGB(0, 255, 0));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 40;
	int left_keys_padding = 15;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);

	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	double data_y_max/*(MaxF(data))*/, data_y_min/*(MinF(data))*/;
	if (MaxF(data) > MaxF(data1))
		if (MaxF(data) > MaxF(data2)) data_y_max = MaxF(data);
		else if (MaxF(data2) > MaxF(data1))data_y_max = MaxF(data2);
		else data_y_max = MaxF(data1);
	else if (MaxF(data1) > MaxF(data2)) data_y_max = MaxF(data1);
	else data_y_max = MaxF(data2);
	if (MinF(data) > MinF(data1))
		if (MinF(data1) < MinF(data2)) data_y_min = MinF(data1);
		else if (MinF(data) > MinF(data2))data_y_min = MinF(data2);
		else data_y_min = MinF(data);
	else if (MinF(data) < MinF(data2)) data_y_min = MinF(data);
	else data_y_min = MinF(data2);

	if (data.empty()) return;

	memDC.SelectObject(&one_pen);

	vector<double> y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(data_x_min, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i * /*s.Fd / s.n*/ step_x, actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&two_pen);

	y = convert_range(data1, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(data_x_min, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i * /*s.Fd / s.n*/ step_x, actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&three_pen);

	y = convert_range(data2, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(data_x_min, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i /*/ s.Fd*/ * step_x, actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	CString str;
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{

		str.Format(L"%.0f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.4f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}
	//str.Format(L"x");
	memDC.TextOutW(actual_left - 10, actual_top - 30, Oy);

	//str.Format(L"t");
	memDC.TextOutW(actual_right + 20, actual_bottom + 20, Ox);

	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}

void Drawer::log3gr(vector<double> data, vector<double> data1, vector<double> data2, signal sig, CString Ox, CString Oy, double data_x_min, double data_x_max, double step_x)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen one_pen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen two_pen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen three_pen(PS_SOLID, 2, RGB(0, 255, 0));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 40;
	int left_keys_padding = 15;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);

	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	double data_y_max/*(MaxF(data))*/, data_y_min/*(MinF(data))*/;
	if (MaxF(data) > MaxF(data1))
		if (MaxF(data) > MaxF(data2)) data_y_max = MaxF(data);
		else if (MaxF(data2) > MaxF(data1))data_y_max = MaxF(data2);
		else data_y_max = MaxF(data1);
	else if (MaxF(data1) > MaxF(data2)) data_y_max = MaxF(data1);
	else data_y_max = MaxF(data2);
	if (MinF(data) > MinF(data1))
		if (MinF(data1) < MinF(data2)) data_y_min = MinF(data1);
		else if (MinF(data) > MinF(data2))data_y_min = MinF(data2);
		else data_y_min = MinF(data);
	else if (MinF(data) < MinF(data2)) data_y_min = MinF(data);
	else data_y_min = MinF(data2);

	double max1 = MaxF(data), max2 = MaxF(data1), max3 = MaxF(data2);

	for (int i = 0; i < data.size(); i++) data[i] = 10 * log10(data[i] / max1);
	for (int i = 0; i < data1.size(); i++) data1[i] = 10 * log10(data1[i] / max2);
	for (int i = 0; i < data2.size(); i++) data2[i] = 10 * log10(data2[i] / max3);

	data_y_max = 10 * log10(data_y_max / max1);
	data_y_min = 10 * log10(data_y_min / max1);

	if (data.empty()) return;

	memDC.SelectObject(&one_pen);

	vector<double> y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(data_x_min, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i * /*s.Fd / s.n*/ step_x, actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&two_pen);

	y = convert_range(data1, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(data_x_min, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i * /*s.Fd / s.n*/ step_x, actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&three_pen);

	y = convert_range(data2, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(data_x_min, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i /*/ s.Fd*/ * step_x, actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	CString str;
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{

		str.Format(L"%.0f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.0f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}
	//str.Format(L"x");
	memDC.TextOutW(actual_left - 10, actual_top - 30, Oy);

	//str.Format(L"t");
	memDC.TextOutW(actual_right + 20, actual_bottom + 20, Ox);

	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}

void Drawer::log2gr(vector<double> data, vector<double> data1, signal sig, CString Ox, CString Oy, double data_x_min, double data_x_max, double step_x)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen one_pen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen two_pen(PS_SOLID, 2, RGB(255, 0, 0));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 40;
	int left_keys_padding = 15;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	double data_y_max/*(MaxF(data))*/, data_y_min/*(MinF(data))*/;
	if (MaxF(data) > MaxF(data1)) data_y_max = MaxF(data);
	else data_y_max = MaxF(data1);
	if (MinF(data) > MinF(data1)) data_y_min = MinF(data1);
	else data_y_min = MinF(data);

	double max1 = MaxF(data), max2 = MaxF(data1);

	for (int i = 0; i < data.size(); i++) data[i] = 10 * log10(data[i] / max1);
	for (int i = 0; i < data1.size(); i++) data1[i] = 10 * log10(data1[i] / max2);

	data_y_max = 10 * log10(data_y_max / max1);
	data_y_min = 10 * log10(data_y_min / max1);

	if (data.empty()) return;

	memDC.SelectObject(&one_pen);

	vector<double> y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(data_x_min, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i * /*s.Fd / s.n*/ step_x, actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&two_pen);

	y = convert_range(data1, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(data_x_min, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i * /*s.Fd / s.n*/ step_x, actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	CString str;
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{

		str.Format(L"%.0f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.0f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}
	//str.Format(L"x");
	memDC.TextOutW(actual_left - 10, actual_top - 30, Oy);

	//str.Format(L"t");
	memDC.TextOutW(actual_right + 20, actual_bottom + 20, Ox);

	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}

void Drawer::log1gr(vector<double> data, signal sig, CString Ox, CString Oy, double data_x_min, double data_x_max, double step_x)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen points_pen(PS_SOLID, 2, RGB(0, 0, 255));
	CBrush points_brush;
	points_brush.CreateSolidBrush(RGB(0, 0, 255));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 40;
	int left_keys_padding = 15;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	double data_y_max, data_y_min;
	if ((MaxF(data)) != 0 || (MinF(data)) != 0)
	{
		data_y_max = MaxF(data);
		data_y_min = MinF(data);
	}
	else
	{
		data_y_max = 0.5;
		data_y_min = -0.5;
	}

	double max1 = MaxF(data);

	for (int i = 0; i < data.size(); i++) data[i] = 10 * log10(data[i] / max1);

	data_y_max = 10 * log10(data_y_max / max1);
	data_y_min = 10 * log10(data_y_min / max1);

	if (data.empty()) return;

	memDC.SelectObject(&points_pen);
	memDC.SelectObject(&points_brush);

	vector<double> y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);

	memDC.MoveTo(convert_range_d(data_x_min, actual_right, actual_left, data_x_max, data_x_min), y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(convert_range_d((double)i /*/ s.Fd*/ * step_x, actual_right, actual_left, data_x_max, data_x_min), y[i]);
	}

	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	CString str;
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{

		str.Format(L"%.0f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.0f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}
	//str.Format(L"x");
	memDC.TextOutW(actual_left - 10, actual_top - 30, Oy);

	//str.Format(L"t");
	memDC.TextOutW(actual_right + 20, actual_bottom + 20, Ox);

	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}
