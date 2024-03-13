#pragma once
#include <vector>
#include <set>

enum status
{
	sverh,
	add,
	add_not,
	pt_new,
};

enum status_triag
{
	crush,
	not_crush,
};

enum polog_pt
{
	granica,
	not_granica,
};

struct point
{
	double x;
	double y;
	status st_pt;
	double fi;
	polog_pt is_granica;
	point(double _x, double _y, status stat);
	point() {}
	point(point pt, status stat);
};

struct triangle
{
	point p1;
	point p2;
	point p3;
	double radiusOpis;
	point centerOpis;
	double radiusVpis;
	point centerVpis;
	status_triag st_tr;
};

struct my_ellipse
{
	point center;
	point Ac, Bc;
	double a;
	double b;
	double teta;
	double c;
	double e;
	double fi;
	my_ellipse() {}
	my_ellipse(point center_, double a_, double e_, double teta_);
	point koordnew(double fi);
	bool Vnutri(point ctrl);
	void new_b();
};

std::vector<point> gen_point(int N, double error, double left_x = 0, double right_x = 1, double top_y = 1, double bottom_y = 0);

double myrand(double left, double right);

double radiusOp(point center, point vertex);

point centerOp(point p1, point p2, point p3);

point centerVp(point p1, point p2, point p3);

double radiusVp(point p1, point p2, point p3);

double Striag(triangle my_triag);

bool peresEll(my_ellipse el1, my_ellipse el2);

bool vneOkr(my_ellipse el, my_ellipse okr);

class triangulation
{
	std::vector<point> pryam_setka;
	std::vector<point> my_point;
	std::vector<triangle> my_setka;
	int N;
	double error;
	double quality_med, quality_center;
	std::vector<my_ellipse> ells;

protected:

	bool VneOkr(point ctrl);

	bool add_or_not(point& ctrl, double step);

public:
	point centerOkr = point(0.5, 0.5, sverh);
	double radiusOkr = 0.4;

	CRITICAL_SECTION csTriag;

	triangulation() { centerOkr.x = 0.5; centerOkr.y = 0.5; radiusOkr = 0.4; }

	~triangulation() { DeleteCriticalSection(&csTriag); }

	triangulation(int my_N, double my_error);

	triangulation(std::vector<my_ellipse> ell, double stepfi, double step_setka, double fi_okr);

	void delone_triag();

	std::vector<triangle> GetTriangle();

	std::vector<point> GetPoint();

	void CalcQuality(point p1, point p2, point p3);

	double GetQualityMed();

	double GetQualityCent();

	double Sred(double a, double b, double c);

	void rekkurent_delone_triag(double step_setka);

	void delete_crush();

	void delete_point();

	void ResultQuality();
};

bool operator<(const triangle& left, const triangle& right);

bool operator==(const triangle& left, const triangle& right);

bool operator==(const point& left, const point& right);

// ����� ��������
void kazf(std::vector<std::vector<double>> a, std::vector<double> b, std::vector<double>&x);