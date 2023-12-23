#include "pch.h"
#define _USE_MATH_DEFINES
#include "Signal.h"
#include <math.h>

signal::signal(double Aa, double fa, double fia, double fda, double aa, int Na, int mina, int maxa)
{
	A = Aa; f = fa; fi = fia; fd = fda; a = aa; N = Na; akpmin = mina; akpmax = maxa;
	CreateShum();
}

void signal::CreateSig(vector<double>& s/*, int wnd, int akpnum*/)
{
	/*if (wnd == 0)
	{
		for (int i = 0; i < akpnum; i++)
		{
			s.push_back(A * sin(2 * M_PI * f * i / fd + fi));
		}
	}
	else if (wnd == 1)
	{
		if (akpnum % 2 == 1)
		{
			for (int i = 0; i < (akpnum + 1) / 2; i++)
			{
				s.push_back(A * sin(2 * M_PI * f * i / fd + fi) * 2 * i / (akpnum + 1));
			}

			for (int i = (akpnum + 1) / 2; i < akpnum; i++)
			{
				s.push_back(A * sin(2 * M_PI * f * i / fd + fi) * 2 * (akpnum - i + 1) / akpnum);
			}
		}

		else
		{
			for (int i = 0; i < akpnum / 2; i++)
			{
				s.push_back(A * sin(2 * M_PI * f * i / fd + fi) * (2 * i - 1) / akpnum);
			}

			for (int i = akpnum / 2; i < akpnum; i++)
			{
				s.push_back(A * sin(2 * M_PI * f * i / fd + fi) * 2 * (akpnum - i + 1) / akpnum);
			}
		}
	}
	else 
	{
		for (int i = 0; i < akpnum; i++)
		{
			s.push_back(A * sin(2 * M_PI * f * i / fd + fi) * cos(M_PI * i / akpnum - M_PI / 2));
		}
	}*/

	for (int i = 0; i < N; i++)
	{
		s.push_back(A * sin(2 * M_PI * f * i / fd + fi));
	}
}

void signal::CreateShum()
{
	shum.clear();
	for (int i = 0; i < N; i++)
	{
		shum.push_back((double)(rand()) / RAND_MAX * 2 - 1);
	}

	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < N; j++)
		{
			shum[j] += (double)(rand()) / RAND_MAX * 2 - 1;
		}
	}
}

void signal::bad_f(vector<double> func, vector<double>& badfunc)
{
	double SumSh(0);
	for (int k = 0; k < func.size(); k++)
	{
		SumSh += shum[k] * shum[k];
	}

	double bet = sqrt(a * Es(func) / SumSh);

	for (int i = 0; i < func.size(); i++)
	{
		badfunc.push_back(func[i] + bet * shum[i]);
	}
}

void signal::fft(vector<base>& a, bool invert)
{
	int n = (int)a.size();
	if (n == 1)  return;

	vector<base> a0(n / 2), a1(n / 2);
	for (int i = 0, j = 0; i < n; i += 2, ++j) {
		a0[j] = a[i];
		a1[j] = a[i + 1];
	}
	fft(a0, invert);
	fft(a1, invert);

	double ang = 2 * M_PI / n * (invert ? -1 : 1);
	base w(1), wn(cos(ang), sin(ang));
	for (int i = 0; i < n / 2; ++i) {
		a[i] = a0[i] + w * a1[i];
		a[i + n / 2] = a0[i] - w * a1[i];
		if (invert)
			a[i] /= 2, a[i + n / 2] /= 2;
		w *= wn;
	}
}

double signal::Es(vector<double> func)
{
	double Esig(0);
	for (int i = 0; i < func.size(); i++)
	{
		Esig += func[i] * func[i];
	}
	return Esig;
}

void signal::GetAKP(vector<double> func, vector<double>& akp, int min, int max, int wnd)
{
	double work = 0.;
	int power2(0), r;
	for (int m = min; m < max; m++)
	{
		for (int k = 0; k < func.size() - m; k++)
		{
			work += func[k] * func[k + m];
		}
		akp.push_back(work / (N - m));
		work = 0.;
	}
	/*int base = akp.size();
	bool fl = false;
	while (true)
	{
		r = base % 2;
		base /= 2;
		if (base != 0) power2++;
		if (r != 0) fl = true;
		if (base == 1 || base == 0) break;
	}*/

	int size = akp.size();
	/*if (fl)
	{
		for (int nol = 0; nol < pow(2, power2 + 1) - size; nol++)
		{
			akp.push_back(0.);
		}
	}*/

	if (wnd != 0) MultiWnd(akp, wnd);

	for (int nol = 0; nol < N - size; nol++)
	{
		akp.push_back(0.);
	}
}

double signal::GetF()
{
	return f;
}

double signal::GetFD()
{
	return fd;
}

double signal::SearchF(vector<double> func)
{
	double max = func[0], findf = 0;;
	for (int i = 0; i < func.size() / 2; i++)
	{
		if (func[i] > max)
		{
			max = func[i];
			findf = i * fd / N;
		}
	}
	/*if (findf < fd / 2) findf = fd - findf;
	findf = abs(findf - fd + f);*/
	//if (findf > f) findf = abs(findf - fd);
	findf = abs(findf - f); 
	return findf;
}

//double signal::SKO(vector<double> input, int numexp, int max)
//{
//	vector<double> zn, zn2, bad, akp, spectr;
//	int min = 0;
//	for (int i = 0; i < numexp; i++)
//	{
//		CreateShum();
//		bad_f(input, bad);
//		//if (akp.size() != 0) akp.clear();
//		GetAKP(bad, akp, min, max);
//		vector<base> specakp, znakp;
//		for (int i = 0; i < akp.size(); i++)
//		{
//			base znac(akp[i], 0.);
//			//specakp.push_back(znac);
//			znakp.push_back(znac);
//		}
//		fft(specakp, true);
//		//dft(znakp, specakp);
//
//		for (int i = 0; i < specakp.size(); i++)
//		{
//			spectr.push_back(sqrt(specakp[i].real() * specakp[i].real() + specakp[i].imag() * specakp[i].imag()));
//		}
//		zn.push_back(SearchF(spectr));
//		zn2.push_back(zn[i] * zn[i]);
//
//		akp.clear();
//		bad.clear();
//		specakp.clear();
//		znakp.clear();
//		spectr.clear();
//	}
//
//	return sqrt(MatOg(zn2) - MatOg(zn) * MatOg(zn));
//}

double signal::MatOg(vector<double> chislo)
{
	double m = 0;
	for (int i = 0; i < chislo.size(); i++)
	{
		m += chislo[i];
	}
	m = m / chislo.size();
	return m;
}

void signal::MultiWnd(vector<double>& s, int wnd)
{
	int size = s.size();
	if (wnd == 1)
	{
		if (size % 2 == 1)
		{
			for (int i = 1; i <= (size + 1) / 2; i++)
			{
				//s.push_back(A * sin(2 * M_PI * f * i / fd + fi) * 2 * i / (akpnum + 1));
				s[i - 1] = s[i - 1] * 2 * i / (size + 1);
			}

			for (int i = (size + 3) / 2; i <= size; i++)
			{
				//s.push_back(A * sin(2 * M_PI * f * i / fd + fi) * 2 * (akpnum - i + 1) / akpnum);
				s[i - 1] = s[i - 1] * 2 * (size - i + 1) / (size + 1);
			}
		}

		else
		{
			for (int i = 1; i <= size / 2; i++)
			{
				//s.push_back(A * sin(2 * M_PI * f * i / fd + fi) * (2 * i - 1) / akpnum);
				s[i - 1] = s[i - 1] * (2 * i - 1) / size;
			}

			for (int i = size / 2 + 1; i <= size; i++)
			{
				//s.push_back(A * sin(2 * M_PI * f * i / fd + fi) * 2 * (akpnum - i + 1) / akpnum);
				s[i - 1] = s[i - 1] * 2 * (size - i + 1) / size;
			}
		}
	}
	else
	{
		for (int i = 1; i <= size; i++)
		{
			//s.push_back(A * sin(2 * M_PI * f * i / fd + fi) * cos(M_PI * i / akpnum - M_PI / 2));
			s[i - 1] *= cos(M_PI * i / size - M_PI / 2);
		}
	}
}
