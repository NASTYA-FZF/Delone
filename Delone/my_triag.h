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
	double z;
	status st_pt;
	double fi;
	polog_pt is_granica;
	std::vector<int> number_triag;
	point(double _x, double _y, status stat);
	point() {}
	point(point pt, status stat);
};

typedef std::pair<point, point> line;

struct triangle
{
	point p1;
	point p2;
	point p3;
	double radiusOpis;
	point centerOpis;
	status_triag st_tr;
	double S;
	bool VnutTriag(point pt);
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
	double fi_pot;
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
	std::vector<point> pt_power;

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

enum ij
{
	iravenj,
	isosedj,
	ij0,
};

class galerkin
{
	std::vector<point> my_point, pt_power;
	std::vector<triangle> my_triag;
	int count_not_granica;
	std::vector<std::vector<double>> Aij;
	std::vector<double> Rj, Fij;
	std::vector<line> izoline;
	std::vector<std::vector<line>> power;
	std::vector<double> fi_const;
	int num_izoline;

protected:

	void SetxyzTriag(triangle& tri, point pt);

	void SetTriagPoint();

	double izox(point pt1, point pt2, double ficonst);

	double izoy(point pt1, point pt2, double ficonst);

	void FindIzoline();

	void FindPower();

	int FromTriangle(point pnt);

public:
	CRITICAL_SECTION cs;

	galerkin(std::vector<point> pts, std::vector<triangle> trings, int num_fi_const);

	void Set(std::vector<point> pts, std::vector<triangle> trings, int num_fi_const, std::vector<point> pt_pow);
	void clearvec();

	galerkin() {}

	~galerkin() { /*DeleteCriticalSection(&cs);*/ }

	ij two_point(point pt1, point pt2, std::vector<int>& num_triag);

	void Vnutiravenj(point pt, std::vector<int> num_triag, int row, int column);

	double Aplos(double y1, double y2, double y3, double z1, double z2, double z3);

	double Bplos(double x1, double x2, double x3, double z1, double z2, double z3);

	double Aplos(point pt1, point pt2, point pt3);

	double Bplos(point pt1, point pt2, point pt3);

	double AplosFi(point pt1, point pt2, point pt3);

	double BplosFi(point pt1, point pt2, point pt3);

	double Striag(point pt1, point pt2, point pt3);

	void Funcisosedj(point pti, point ptj, std::vector<int> num_triag, int row, int column);

	void FindFi();

	std::vector<line> GetIzoline();

	std::vector<std::vector<line>> GetPower();

	int GetNumIzoline();

	void SetFiConst(int num_izoline);

	void FindIzoline(int num_izoline);
};