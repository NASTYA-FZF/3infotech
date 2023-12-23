#pragma once
#include <vector>
#include <complex>
using namespace std;

typedef complex<double> base;

class signal
{
	double A, f, fi, fd, a;
	int N, akpmin, akpmax;
	vector<double> shum;

public:
	//конструктор
	signal(double Aa, double fa, double fia, double fda, double aa, int Na, int mina, int maxa);
	//создание сигнала (синусоида)
	void CreateSig(vector<double>& s);
	//создание отсчетов шума
	void CreateShum();
	//наложение шума
	void bad_f(vector<double> func, vector<double>& badfunc);
	//БПФ
	void fft(vector<base>& a, bool invert);
	//энергия сигнала
	double Es(vector<double> func);
	//вычисление АКП
	void GetAKP(vector<double> func, vector<double>& akp, int min, int max, int wnd);
	//получить значение частоты
	double GetF();
	//получить частоту дискретизации
	double GetFD();
	//поиск отклонения от заданной частоты
	double SearchF(vector<double> func);
	//поиск среднеквадратичного значения
	//double SKO(vector<double> input, int numexp, int max);
	//мат ожидание
	double MatOg(vector<double> chislo);
	//умножение на функцию окна
	void MultiWnd(vector<double>& s, int wnd); //0 - pryam, 1 - triag, 2 - cos
};