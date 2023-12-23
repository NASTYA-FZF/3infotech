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
	//�����������
	signal(double Aa, double fa, double fia, double fda, double aa, int Na, int mina, int maxa);
	//�������� ������� (���������)
	void CreateSig(vector<double>& s);
	//�������� �������� ����
	void CreateShum();
	//��������� ����
	void bad_f(vector<double> func, vector<double>& badfunc);
	//���
	void fft(vector<base>& a, bool invert);
	//������� �������
	double Es(vector<double> func);
	//���������� ���
	void GetAKP(vector<double> func, vector<double>& akp, int min, int max, int wnd);
	//�������� �������� �������
	double GetF();
	//�������� ������� �������������
	double GetFD();
	//����� ���������� �� �������� �������
	double SearchF(vector<double> func);
	//����� ������������������� ��������
	//double SKO(vector<double> input, int numexp, int max);
	//��� ��������
	double MatOg(vector<double> chislo);
	//��������� �� ������� ����
	void MultiWnd(vector<double>& s, int wnd); //0 - pryam, 1 - triag, 2 - cos
};