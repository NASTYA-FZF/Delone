#include "pch.h"
#define _USE_MATH_DEFINES
#include "my_triag.h"
#include <fstream>
using namespace std;

std::vector<point> gen_point(int N, double error, double left_x, double right_x, double top_y, double bottom_y)
{
    vector<point> my_point(N);
    for (int i = 0; i < N; i++)
    {
        my_point[i].x = myrand(left_x, right_x) + myrand(-error, error);
        my_point[i].y = myrand(bottom_y, top_y) + myrand(-error, error);
        my_point[i].st_pt = add;
    }
    return my_point;
}

double myrand(double left, double right)
{
    return (double)rand() / RAND_MAX * (right - left) + left;
}

double radiusOp(point center, point vertex)
{
    return sqrt((center.x - vertex.x) * (center.x - vertex.x) + (center.y - vertex.y) * (center.y - vertex.y));
}

point centerOp(point p1, point p2, point p3)
{
    point center;

    double right_part12 = p2.x * p2.x - p1.x * p1.x + p2.y * p2.y - p1.y * p1.y;
    double right_part13 = p3.x * p3.x - p1.x * p1.x + p3.y * p3.y - p1.y * p1.y;
    double znamen = (p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y);

    center.x = 0.5 * (right_part12 * (p3.y - p1.y) - right_part13 * (p2.y - p1.y)) / znamen;
    center.y = 0.5 * (right_part13 * (p2.x - p1.x) - right_part12 * (p3.x - p1.x)) / znamen;

    return center;
}

point centerVp(point p1, point p2, point p3)
{
    point center;

    double ab = radiusOp(p1, p2);
    double bc = radiusOp(p2, p3);
    double ac = radiusOp(p1, p3);

    double lymda = ((p2.x - p1.x) / ab + (p3.x - p1.x) / ac) / ((p2.y - p1.y) / ab + (p3.y - p1.y) / ac);
    double mu = ((p1.x - p2.x) / ab + (p3.x - p2.x) / bc) / ((p1.y - p2.y) / ab + (p3.y - p2.y) / bc);

    center.x = (-mu * p1.x + mu * lymda * (p1.y - p2.y) + lymda * p2.x) / (lymda - mu);
    center.y = (p2.x - mu * p2.y - p1.x + lymda * p1.y) / (lymda - mu);

    return center;
}

double radiusVp(point p1, point p2, point p3)
{
    double a = radiusOp(p1, p2);
    double b = radiusOp(p2, p3);
    double c = radiusOp(p1, p3);
    double p = (a + b + c) / 2.;

    return sqrt((p - a) * (p - b) * (p - c) / p);
}

double Striag(triangle my_triag)
{
    return 0.5 * fabs((my_triag.p2.x - my_triag.p1.x) * (my_triag.p3.y - my_triag.p1.y) - (my_triag.p3.x - my_triag.p1.x) * (my_triag.p2.y - my_triag.p1.y));
}

bool peresEll(my_ellipse el1, my_ellipse el2)
{
    //double tangens = (el2.center.y - el1.center.y) / (el2.center.x - el1.center.x);

    //double cosinus = sqrt(1 / (1 + tangens * tangens));
    //double sinus = sqrt(1 - cosinus * cosinus);

    //double h1 = 2 * el1.b, h2 = 2 * el2.b, w1 = 2 * el1.a, w2 = 2 * el2.a;
    //double x1 = el1.center.x, y1 = el1.center.y, x2 = el2.center.x, y2 = el2.center.y;
    //double radius1 = fabs(w1 - x1) * fabs(h1 - y1) / sqrt((h1 - y1) * (h1 - y1) * cosinus * cosinus + (w1 - x1) * (w1 - x1) * sinus * sinus);
    //double radius2 = fabs(w2 - x2) * fabs(h2 - y2) / sqrt((h2 - y2) * (h2 - y2) * cosinus * cosinus + (w2 - x2) * (w2 - x2) * sinus * sinus);

    //if (radiusOp(el1.center, el2.center) <= radius1 + radius2) return true;
    //else return false;

    double aa1 = sqrt(el1.a * el1.a + el1.b * el1.b), bb2 = sqrt(el2.a * el2.a + el2.b * el2.b);
    double rast = radiusOp(el1.center, el2.center);
    if (rast > el1.a + el2.a) return false;
    else return true;
}

bool vneOkr(my_ellipse el, my_ellipse okr)
{
   /* double tangens = (okr.center.y - el.center.y) / (okr.center.x - el.center.x);

    double cosinus = sqrt(1 / (1 + tangens * tangens));
    double sinus = sqrt(1 - cosinus * cosinus);

    double h1 = 2 * el.b, h2 = 2 * okr.b, w1 = 2 * el.a, w2 = 2 * okr.a;
    double x1 = el.center.x, y1 = el.center.y, x2 = okr.center.x, y2 = okr.center.y;
    double radius1 = fabs(w1 - x1) * fabs(h1 - y1) / sqrt((h1 - y1) * (h1 - y1) * cosinus * cosinus + (w1 - x1) * (w1 - x1) * sinus * sinus);
    double radius2 = fabs(w2 - x2) * fabs(h2 - y2) / sqrt((h2 - y2) * (h2 - y2) * cosinus * cosinus + (w2 - x2) * (w2 - x2) * sinus * sinus);

    if (okr.a - radiusOp(el.center, okr.center) < radius1) return true;
    else return false;*/

    double aa1 = sqrt(el.a * el.a + el.b * el.b), bb2 = sqrt(okr.a * okr.a + okr.b * okr.b);
    double rast = radiusOp(el.center, okr.center);
    if (rast + el.a < okr.a) return false;
    else return true;
}

bool operator<(const triangle& left, const triangle& right)
{
    if (Striag(left) < Striag(right)) return true;
    else return false;
}

bool operator==(const triangle& left, const triangle& right)
{
    if (
        (left.p1 == right.p1 || left.p1 == right.p2 || left.p1 == right.p3) &&
        (left.p2 == right.p1 || left.p2 == right.p2 || left.p2 == right.p3) &&
        (left.p3 == right.p1 || left.p3 == right.p2 || left.p3 == right.p3)
        ) return true;
    else return false;
}

bool operator==(const point& left, const point& right)
{
    if (left.x == right.x && left.y == right.y) return true;
    else return false;
}

void kazf(vector<vector<double>> a, vector<double> b, vector<double>& x)
{
    // nn - количество неизвестных;  ny - количество уравнений
    int nn = a[0].size();
    int ny = a.size();

    double eps = 1.e-8f;
    //float s;
    int i, j, k;
    double s1, s2, fa1, t;
    vector<double> x1(x.size());

    x[0] = 0.5;
    for (i = 1; i < nn; i++)  x[i] = 0.;

    s1 = s2 = 1.;
    while (s1 > eps * s2)
    {
        for (i = 0; i < nn; i++) x1[i] = x[i];

        for (i = 0; i < ny; i++)
        {
            s1 = 0.0;
            s2 = 0.0;
            for (j = 0; j < nn; j++)
            {
                fa1 = a[i][j];
                s1 += fa1 * x[j];
                s2 += fa1 * fa1;
            }
            t = (b[i] - s1) / s2;
            for (k = 0; k < nn; k++) x[k] += a[i][k] * t;
        }

        s1 = 0.0;
        s2 = 0.0;
        for (i = 0; i < nn; i++)
        {
            s1 += (x[i] - x1[i]) * (x[i] - x1[i]);
            s2 += x[i] * x[i];
        }
        s1 = (double)sqrt(s1);
        s2 = (double)sqrt(s2);
    }
    x1.clear();
}

bool triangulation::VneOkr(point ctrl)
{
    if (radiusOp(centerOkr, ctrl) < radiusOkr) return false;
    else return true;
}

bool triangulation::add_or_not(point& ctrl, double step)
{
    bool res = true;
    
    if (VneOkr(ctrl)) res = false;

    for (auto el : ells)
    {
        if (el.Vnutri(ctrl)) res = false;
    }

    if (res)
    {
        my_point.push_back(ctrl);
    }

    double fi_start = 0;
    if (ctrl.x + step >= 1)
    {
        ctrl = point(step + myrand(-error, error), ctrl.y + step + myrand(-error, error), pt_new);
        ctrl.fi = fi_start;
        ctrl.is_granica = not_granica;
    }
    else ctrl.x += step + myrand(-error, error);

    return res;
}

triangulation::triangulation(int my_N, double my_error)
{
    N = my_N;
    error = my_error;
    my_point = gen_point(N, error);
    quality_center = quality_med = 0.;
    InitializeCriticalSection(&csTriag);
}

triangulation::triangulation(std::vector<my_ellipse> ell, double stepfi, double step_setka, double fi_okr)
{
    ells = ell;
    error = 1e-6;
    InitializeCriticalSection(&csTriag);

    my_point.push_back(point(0, 0, sverh));
    my_point.push_back(point(0, 1, sverh));
    my_point.push_back(point(1, 0, sverh));
    my_point.push_back(point(1, 1, sverh));

    point na_el;
    for (auto& el : ells)
    {
        double ficur = el.fi_pot;
        el.teta = el.teta * M_PI / 180;
        el = my_ellipse(el.center, el.a, el.e, el.teta);
        el.fi_pot = ficur;

        my_point.push_back(point(el.center.x, el.center.y, sverh));
        if (el.e >= 0.7)
        {
            my_point.push_back(point(el.Ac.x, el.Ac.y, sverh));
            my_point.push_back(point(el.Bc.x, el.Bc.y, sverh));
            if (el.e >= 0.8)
            {
                my_point.push_back(point(el.Ac.x - (el.Ac.x - el.center.x) / 2, el.Ac.y - (el.Ac.y - el.center.y) / 2, sverh));
                my_point.push_back(point(el.Bc.x - (el.Bc.x - el.center.x) / 2, el.Bc.y - (el.Bc.y - el.center.y) / 2, sverh));
                if (el.e >= 0.9)
                {
                    my_point.push_back(point(el.Ac.x - (el.Ac.x - el.center.x) / 4, el.Ac.y - (el.Ac.y - el.center.y) / 4, sverh));
                    my_point.push_back(point(el.Bc.x - (el.Bc.x - el.center.x) / 4, el.Bc.y - (el.Bc.y - el.center.y) / 4, sverh));
                    my_point.push_back(point(el.Ac.x - (el.Ac.x - el.center.x) / 4 * 3, el.Ac.y - (el.Ac.y - el.center.y) / 4 * 3, sverh));
                    my_point.push_back(point(el.Bc.x - (el.Bc.x - el.center.x) / 4 * 3, el.Bc.y - (el.Bc.y - el.center.y) / 4 * 3, sverh));
                }
            }
        }

        for (double fi = 0.; fi < 2 * M_PI; fi += stepfi * 4)
        {
            na_el = el.koordnew(fi);
            my_point.push_back(point(na_el.x, na_el.y, add));
            my_point.back().fi = el.fi_pot;
            my_point.back().is_granica = granica;

            na_el = el.koordnew(fi / 2);
            pt_power.push_back(point(na_el.x, na_el.y, add));
            pt_power.back().fi = el.fi_pot;
            pt_power.back().is_granica = granica;
        }
    }

    for (double fi = 0.; fi < 2 * M_PI; fi += stepfi)
    {
        my_point.push_back(point(radiusOkr * cos(fi) + centerOkr.x + myrand(-error, error), radiusOkr * sin(fi) + centerOkr.y + myrand(-error, error), add));
        my_point.back().fi = fi_okr;
        my_point.back().is_granica = granica;

        pt_power.push_back(point(radiusOkr * cos(fi / 2) + centerOkr.x + myrand(-error, error), radiusOkr * sin(fi / 2) + centerOkr.y + myrand(-error, error), add));
        pt_power.back().is_granica = granica;
        pt_power.back().fi = fi_okr;
    }
}

void triangulation::delone_triag()
{
    double radius;
    point center;
    triangle cur_triangle;
    bool add_triag = true;
    bool only_back = false;
    N = my_point.size();

    quality_center = 0;
    quality_med = 0;

    for (int i = N - 1; i > 1; i--)
    {
        if (my_point[i].st_pt == add_not) continue;
        if (my_point[i].st_pt == pt_new) only_back = true;
        cur_triangle.p1 = my_point[i];
        for (int j = i - 1; j > 0; j--)
        {
            if (j == i || my_point[j].st_pt == add_not) continue;
            cur_triangle.p2 = my_point[j];
            for (int k = j - 1; k >= 0; k--)
            {
                if (k == j || k == i || my_point[k].st_pt == add_not) continue;
                cur_triangle.p3 = my_point[k];

                cur_triangle.centerOpis = centerOp(my_point[i], my_point[j], my_point[k]);
                cur_triangle.radiusOpis = radiusOp(cur_triangle.centerOpis, my_point[i]);
                for (int m = 0; m < N; m++)
                {
                    if (m == i || m == j || m == k) continue;
                    if (radiusOp(my_point[m], cur_triangle.centerOpis) < cur_triangle.radiusOpis)
                    {
                        add_triag = false;
                        break;
                    }
                }
                if (add_triag)
                {
                    EnterCriticalSection(&csTriag);

                    if (cur_triangle.p1.st_pt == sverh || cur_triangle.p2.st_pt == sverh || cur_triangle.p3.st_pt == sverh) cur_triangle.st_tr = not_crush;
                    else cur_triangle.st_tr = crush;

                    my_setka.push_back(cur_triangle);
                    LeaveCriticalSection(&csTriag);
                }
                add_triag = true;
            }
        }
        if (only_back) break;
    }
    EnterCriticalSection(&csTriag);
    ResultQuality();
    LeaveCriticalSection(&csTriag);

    for (int i = 0; i < my_point.size(); i++)
    {
        if (my_point[i].st_pt == add || my_point[i].st_pt == pt_new) my_point[i].st_pt = add_not;
    }
}

std::vector<triangle> triangulation::GetTriangle()
{
    return my_setka;
}

std::vector<point> triangulation::GetPoint()
{
    return my_point;
}

void triangulation::CalcQuality(point p1, point p2, point p3)
{
    point center_op = centerOp(p1, p2, p3);
    double R = radiusOp(center_op, p1);
    double r = radiusVp(p1, p2, p3);

    double a = radiusOp(p1, p2);
    double b = radiusOp(p1, p3);
    double c = radiusOp(p3, p2);

    if (a > b && a > c) quality_med += r / a;
    else
        if (b > a && b > c) quality_med += r / b;
        else quality_med += r / c;

    quality_center += 1 - sqrt(R * R - 2 * R * r) / R;
}

double triangulation::GetQualityMed()
{
    return quality_med;
}

double triangulation::GetQualityCent()
{
    return quality_center;
}

double triangulation::Sred(double a, double b, double c)
{
    double max, min;

    if (a > b && a > c) max = a;
    else
        if (b > a && b > c) max = b;
        else max = c;

    if (a < b && a < c) min = a;
    else
        if (b < a && b < c) min = b;
        else min = c;

    if (a < max && a > min) return a;
    else
        if (b < max && b > min) return b;
        else return c;
}

void triangulation::rekkurent_delone_triag(double step_setka)
{
    delone_triag();
    double fi_start = 0;
    point add_point(step_setka + myrand(-error, error), step_setka + myrand(-error, error), pt_new);
    add_point.fi = fi_start;
    add_point.is_granica = not_granica;
    int iter = 0;
    while (true)
    {
        if (add_point.y > centerOkr.y + radiusOkr) break;
        if (!add_or_not(add_point, step_setka)) continue;
        delete_crush();
        delone_triag();
        iter++;
    }
    delete_point();
}

void triangulation::delete_crush()
{
    for (int i = 0; i < my_setka.size(); i++)
    {
        if (my_setka[i].st_tr == not_crush) continue;
        if (radiusOp(my_setka[i].centerOpis, my_point.back()) < my_setka[i].radiusOpis)
        {
            for (int j = 0; j < my_point.size(); j++)
            {
                if (my_point[j] == my_setka[i].p1 || my_point[j] == my_setka[i].p2 || my_point[j] == my_setka[i].p3)
                    my_point[j].st_pt = (my_point[j].st_pt == sverh) ? sverh : add;
            }
            my_setka.erase(my_setka.begin() + i);
            i--;
        }
    }
}

void triangulation::delete_point()
{
    int size_del = 4 + 3 * ells.size();
    int size = my_point.size();
    //my_point.erase(my_point.begin(), my_point.begin() + size_del);

    for (int i = 0; i < size; i++)
    {
        if (my_point[i].st_pt == sverh)
        {
            my_point.erase(my_point.begin() + i);
            i--;
            size--;
        }
    }

    for (int i = 0; i < my_setka.size(); i++)
    {
        if (my_setka[i].p1.st_pt == sverh
            || my_setka[i].p2.st_pt == sverh
            || my_setka[i].p3.st_pt == sverh)
        {
            my_setka.erase(my_setka.begin() + i);
            i--;
        }
    }
}

void triangulation::ResultQuality()
{
    for (auto& tri : my_setka)
    {
        CalcQuality(tri.p1, tri.p2, tri.p3);
    }
    quality_med = quality_med * 6 / (my_setka.size() * sqrt(3));
    quality_center = quality_center / my_setka.size();
}

point::point(double _x, double _y, status stat)
{
    x = _x;
    y = _y;
    st_pt = stat;
    fi = 0;
    number_triag.clear();
}

point::point(point pt, status stat)
{
    x = pt.x;
    y = pt.y;
    st_pt = stat;
}

my_ellipse::my_ellipse(point center_, double a_, double e_, double teta_)
{
    center = center_;
    a = a_;
    e = e_;
    b = sqrt(a * a - e * e * a * a);
    teta = teta_;
    c = sqrt(fabs(a * a - b * b));
    Ac = point(center.x - c * cos(teta), center.y - c * sin(teta), sverh);
    Bc = point(center.x + c * cos(teta), center.y + c * sin(teta), sverh);
}

point my_ellipse::koordnew(double fi)
{
    double x = a * cos(fi) * cos(teta) - b * sin(fi) * sin(teta) + center.x;
    double y = a * cos(fi) * sin(teta) + b * sin(fi) * cos(teta) + center.y;
    return point(x, y, add);
}

bool my_ellipse::Vnutri(point ctrl)
{
    /*double lyambda = acos((ctrl.x - center.x) / radiusOp(ctrl, center));
    double fi = lyambda - teta;
    point na_ell = koordnew(fi);
    if (radiusOp(ctrl, center) < radiusOp(na_ell, center)) return true;
    else return false;*/

    double r1 = radiusOp(Ac, ctrl);
    double r2 = radiusOp(Bc, ctrl);
    if (r1 + r2 <= 2 * a) return true;
    else return false;
}

void my_ellipse::new_b()
{
    b = sqrt(a * a - e * e * a * a);
}

void galerkin::SetxyzTriag(triangle& tri, point pt)
{
    if (tri.p1 == pt) tri.p1.z = 1.;
    else tri.p1.z = 0.;

    if (tri.p2 == pt) tri.p2.z = 1.;
    else tri.p2.z = 0.;

    if (tri.p3 == pt) tri.p3.z = 1.;
    else tri.p3.z = 0.;
}

void galerkin::SetTriagPoint()
{
    double first = my_point.size() - count_not_granica;
    vector<int> my_num;
    for (int i = 0; i < Fij.size(); i++)
    {
        my_point[i + first].fi = -Fij[i];
        my_num = my_point[i + first].number_triag;
        for (int j = 0; j < my_num.size(); j++)
        {
            if (my_triag[my_num[j]].p1 == my_point[i + first])
                my_triag[my_num[j]].p1.fi = -Fij[i];
            if (my_triag[my_num[j]].p2 == my_point[i + first])
                my_triag[my_num[j]].p2.fi = -Fij[i];
            if (my_triag[my_num[j]].p3 == my_point[i + first])
                my_triag[my_num[j]].p3.fi = -Fij[i];
        }
    }

    ofstream out("fi.txt");
    for (int i = 0; i < my_point.size(); i++)
        out << i << ") " << my_point[i].fi << endl;
    out.close();
}

void galerkin::FindIzoline()
{
    izoline.clear();
    vector<point> pt;
    double x, y;

    for (auto tri : my_triag)
    {
        for (auto fic : fi_const)
        {
            pt.clear();
            //point 1 and point 2
            if ((fic >= tri.p1.fi && fic <= tri.p2.fi)
                /*|| (fic <= tri.p1.fi && fic >= tri.p2.fi)*/)
            {
                x = izox(tri.p1, tri.p2, fic);
                y = izoy(tri.p1, tri.p2, fic);
                pt.push_back(point(x, y, add));
            }
            else
                if (/*(fic >= tri.p1.fi && fic <= tri.p2.fi)
                    || */(fic <= tri.p1.fi && fic >= tri.p2.fi))
                {
                    x = izox(tri.p2, tri.p1, fic);
                    y = izoy(tri.p2, tri.p1, fic);
                    pt.push_back(point(x, y, add));
                }

            //point 1 and point 3
            if ((fic >= tri.p1.fi && fic <= tri.p3.fi)
                /*|| (fic <= tri.p1.fi && fic >= tri.p3.fi)*/)
            {
                x = izox(tri.p1, tri.p3, fic);
                y = izoy(tri.p1, tri.p3, fic);
                pt.push_back(point(x, y, add));
            }
            else
                if (/*(fic >= tri.p1.fi && fic <= tri.p3.fi)
                    || */(fic <= tri.p1.fi && fic >= tri.p3.fi))
                {
                    x = izox(tri.p3, tri.p1, fic);
                    y = izoy(tri.p3, tri.p1, fic);
                    pt.push_back(point(x, y, add));
                }

            //point 2 and point 3
            if ((fic >= tri.p3.fi && fic <= tri.p2.fi)
                /*|| (fic <= tri.p3.fi && fic >= tri.p2.fi)*/)
            {
                x = izox(tri.p3, tri.p2, fic);
                y = izoy(tri.p3, tri.p2, fic);
                pt.push_back(point(x, y, add));
            }
            else
                if (/*(fic >= tri.p3.fi && fic <= tri.p2.fi)
                    || */(fic <= tri.p3.fi && fic >= tri.p2.fi))
                {
                    x = izox(tri.p2, tri.p3, fic);
                    y = izoy(tri.p2, tri.p3, fic);
                    pt.push_back(point(x, y, add));
                }

            EnterCriticalSection(&cs);
            if (pt.size() == 2)
                izoline.push_back(line(pt[0], pt[1]));
            LeaveCriticalSection(&cs);
        }
    }
}

void galerkin::FindPower()
{
    //double step = 0.02, A, B, dlina = 0.1, a, b, c;
    //int num, sign;
    //std::vector<line> pow;
    //point p;
    //for (int j = 0; j < pt_power.size(); j += 1)
    //{
    //    p = pt_power[j];
    //    num = FromTriangle(p);
    //    //pow.clear();
    //    EnterCriticalSection(&cs);
    //    power.push_back(vector<line>());
    //    LeaveCriticalSection(&cs);
    //    while (num >= 0)
    //    {
    //        //a = radiusOp(p, my_triag[num].p1);
    //        //b = radiusOp(p, my_triag[num].p2);
    //        //c = radiusOp(p, my_triag[num].p3);

    //        //if (a < b && a < c) p.fi = my_triag[num].p1.fi;
    //        //if (b < a && b < c) p.fi = my_triag[num].p2.fi;
    //        //if (c < a && c < b) p.fi = my_triag[num].p3.fi;

    //        //p.fi = (my_triag[num].p1.fi + my_triag[num].p2.fi + my_triag[num].p3.fi) / 3;

    //        A = AplosFi(my_triag[num].p1, my_triag[num].p2, my_triag[num].p3);
    //        B = BplosFi(my_triag[num].p1, my_triag[num].p2, my_triag[num].p3);

    //        //if (p.fi > 0) sign = 1;
    //        //else sign = -1;
    //        sign = -1;

    //        EnterCriticalSection(&cs);
    //        power.back().push_back(line(p, point(p.x + sign * A * dlina, p.y + sign * B * dlina, pt_new)));
    //        LeaveCriticalSection(&cs);
    //        p = power.back().back().second;

    //        num = FromTriangle(p);
    //        if (num == -1) power.back().pop_back();
    //    }
    //    //EnterCriticalSection(&cs);
    //    //power.push_back(pow);
    //    //LeaveCriticalSection(&cs);
    //}

    double step = 0.01, dlinaA = 0.002, dlinaB = 0.1;
    double A, B;
    int num;
    std::vector<line> pow;
    double a;
    for (double x = 0; x < 1; x += step)
    {
        for (double y = 0; y < 1; y += step)
        {
            num = FromTriangle(point(x, y, add));
            if (num == -1) continue;
            A = -AplosFi(my_triag[num].p1, my_triag[num].p2, my_triag[num].p3);
            B = BplosFi(my_triag[num].p1, my_triag[num].p2, my_triag[num].p3);
            a = sqrt(A * A + B * B);
            A /= a;
            B /= a;
            pow.push_back(line(point(x, y, add), point(x - A * dlinaA, y - B * dlinaA, add)));
            pow.push_back(line(point(x, y, add), point(x + A * dlinaA, y + B * dlinaA, add)));
        }
    }
    EnterCriticalSection(&cs);
    power.push_back(pow);
    LeaveCriticalSection(&cs);
}

int galerkin::FromTriangle(point pnt)
{
    for (int i = 0; i < my_triag.size(); i++)
    {
        double a = (my_triag[i].p1.x - pnt.x) * (my_triag[i].p2.y - my_triag[i].p1.y) - (my_triag[i].p2.x - my_triag[i].p1.x) * (my_triag[i].p1.y - pnt.y);
        double b = (my_triag[i].p2.x - pnt.x) * (my_triag[i].p3.y - my_triag[i].p2.y) - (my_triag[i].p3.x - my_triag[i].p2.x) * (my_triag[i].p2.y - pnt.y);
        double c = (my_triag[i].p3.x - pnt.x) * (my_triag[i].p1.y - my_triag[i].p3.y) - (my_triag[i].p1.x - my_triag[i].p3.x) * (my_triag[i].p3.y - pnt.y);

        if ((a >= 0 && b >= 0 && c >= 0) || (a <= 0 && b <= 0 && c <= 0)) return i;
    }
    return -1;
}

double galerkin::izox(point pt1, point pt2, double ficonst)
{
    return pt2.x - (pt2.fi - ficonst) * ((pt2.x - pt1.x) / (pt2.fi - pt1.fi));;
}

double galerkin::izoy(point pt1, point pt2, double ficonst)
{
    return pt2.y - (pt2.fi - ficonst) * ((pt2.y - pt1.y) / (pt2.fi - pt1.fi));
}

galerkin::galerkin(std::vector<point> pts, std::vector<triangle> trings, int num_fi_const)
{
    my_point = pts;
    my_triag = trings;
    for (auto& tri : my_triag)
        tri.S = Striag(tri.p1, tri.p2, tri.p3);

    count_not_granica = 0;
    double fi_max = 0, fi_min = 0;
    for (int i = 0; i < my_point.size(); i++)
    {
        if (!my_point[i].number_triag.empty()) my_point[i].number_triag.clear();
        if (my_point[i].is_granica == not_granica) count_not_granica++;
        for (int j = 0; j < my_triag.size(); j++)
        {
            if (my_point[i] == my_triag[j].p1 ||
                my_point[i] == my_triag[j].p2 ||
                my_point[i] == my_triag[j].p3) my_point[i].number_triag.push_back(j);
        }

        if (my_point[i].fi > fi_max) fi_max = my_point[i].fi;
        if (my_point[i].fi < fi_min) fi_min = my_point[i].fi;
    }
    //InitializeCriticalSection(&cs);
    Aij = vector<vector<double>>(count_not_granica, vector<double>(count_not_granica));
    Rj = Fij = vector<double>(count_not_granica);
    fi_const.clear();

    for (double my_fi = fi_min; my_fi < fi_max; my_fi += (fi_max - fi_min) / num_fi_const)
        fi_const.push_back(my_fi);
}

void galerkin::Set(std::vector<point> pts, std::vector<triangle> trings, int num_fi_const, std::vector<point> pt_pow)
{
    my_point = pts;
    my_triag = trings;
    for (auto& tri : my_triag)
        tri.S = Striag(tri.p1, tri.p2, tri.p3);

    count_not_granica = 0;
    double fi_max = 0, fi_min = 0;
    for (int i = 0; i < my_point.size(); i++)
    {
        if (!my_point[i].number_triag.empty()) my_point[i].number_triag.clear();
        if (my_point[i].is_granica == not_granica) count_not_granica++;
        for (int j = 0; j < my_triag.size(); j++)
        {
            if (my_point[i] == my_triag[j].p1 ||
                my_point[i] == my_triag[j].p2 ||
                my_point[i] == my_triag[j].p3) my_point[i].number_triag.push_back(j);
        }

        if (my_point[i].fi > fi_max) fi_max = my_point[i].fi;
        if (my_point[i].fi < fi_min) fi_min = my_point[i].fi;
    }
    //InitializeCriticalSection(&cs);
    Aij = vector<vector<double>>(count_not_granica, vector<double>(count_not_granica));
    Rj = Fij = vector<double>(count_not_granica);
    fi_const.clear();
    izoline.clear();
    num_izoline = num_fi_const;
    //for (double my_fi = fi_min; my_fi < fi_max; my_fi += (fi_max - fi_min) / num_fi_const)
    //    fi_const.push_back(my_fi);

    pt_power = pt_pow;
}

void galerkin::clearvec()
{
    power.clear();
    izoline.clear();
}

ij galerkin::two_point(point pt1, point pt2, std::vector<int>& num_triag)
{
    if (pt1 == pt2)
    {
        num_triag = pt1.number_triag;
        return iravenj;
    }
    num_triag.clear();
    for (int i = 0; i < pt1.number_triag.size(); i++)
    {
        for (int j = 0; j < pt2.number_triag.size(); j++)
        {
            if (pt1.number_triag[i] == pt2.number_triag[j]) 
                num_triag.push_back(pt1.number_triag[i]);
        }
    }

    if (num_triag.empty()) return ij0;
    else return isosedj;
}

void galerkin::Vnutiravenj(point pt, std::vector<int> num_triag, int row, int column)
{
    double A, B;
    for (int i = 0; i < num_triag.size(); i++)
    {
        SetxyzTriag(my_triag[num_triag[i]], pt);
        A = Aplos(my_triag[num_triag[i]].p1, my_triag[num_triag[i]].p2, my_triag[num_triag[i]].p3);
        B = Bplos(my_triag[num_triag[i]].p1, my_triag[num_triag[i]].p2, my_triag[num_triag[i]].p3);
        Aij[row][column] += (A * A + B * B) * my_triag[num_triag[i]].S;
    }
    //for (int i = 0; i < num_triag.size(); i++)
    //{
    //    A = Aplos(my_triag[num_triag[i]].p1, my_triag[num_triag[i]].p2, my_triag[num_triag[i]].p3);
    //    B = Bplos(my_triag[num_triag[i]].p1, my_triag[num_triag[i]].p2, my_triag[num_triag[i]].p3);
    //    Aij[row][column] += (A * A + B * B) * my_triag[num_triag[i]].S;
    //}
}

double galerkin::Aplos(double y1, double y2, double y3, double z1, double z2, double z3)
{
    return (y2 - y1) * (z3 - z1) - (y3 - y1) * (z2 - z1);
}

double galerkin::Bplos(double x1, double x2, double x3, double z1, double z2, double z3)
{
    return (x2 - x1) * (z3 - z1) - (z2 - z1) * (x3 - x1);
}

double galerkin::Aplos(point pt1, point pt2, point pt3)
{
    return (pt2.y - pt1.y) * (pt3.z - pt1.z) - (pt3.y - pt1.y) * (pt2.z - pt1.z);
}

double galerkin::Bplos(point pt1, point pt2, point pt3)
{
    return (pt2.x - pt1.x) * (pt3.z - pt1.z) - (pt2.z - pt1.z) * (pt3.x - pt1.x);
}

double galerkin::AplosFi(point pt1, point pt2, point pt3)
{
    return (pt2.y - pt1.y) * (pt3.fi - pt1.fi) - (pt3.y - pt1.y) * (pt2.fi - pt1.fi);
}

double galerkin::BplosFi(point pt1, point pt2, point pt3)
{
    return (pt2.x - pt1.x) * (pt3.fi - pt1.fi) - (pt2.fi - pt1.fi) * (pt3.x - pt1.x);
}

double galerkin::Striag(point pt1, point pt2, point pt3)
{
    double ast = radiusOp(pt1, pt2);
    double bst = radiusOp(pt1, pt3);
    double cst = radiusOp(pt2, pt3);
    double polup = (ast + bst + cst) / 2.;
    return sqrt(polup * (polup - ast) * (polup - bst) * (polup - cst));
}

void galerkin::Funcisosedj(point pti, point ptj, std::vector<int> num_triag, int row, int column)
{
    bool R_or_A = false;
    double Ai, Bi, Aj, Bj, summa = 0.;

    if (pti.is_granica == granica || ptj.is_granica == granica)
        R_or_A = true;

    for (int i = 0; i < num_triag.size(); i++)
    {
        SetxyzTriag(my_triag[num_triag[i]], pti);
        Ai = Aplos(my_triag[num_triag[i]].p1, my_triag[num_triag[i]].p2, my_triag[num_triag[i]].p3);
        Bi = Bplos(my_triag[num_triag[i]].p1, my_triag[num_triag[i]].p2, my_triag[num_triag[i]].p3);

        SetxyzTriag(my_triag[num_triag[i]], ptj);
        Aj = Aplos(my_triag[num_triag[i]].p1, my_triag[num_triag[i]].p2, my_triag[num_triag[i]].p3);
        Bj = Bplos(my_triag[num_triag[i]].p1, my_triag[num_triag[i]].p2, my_triag[num_triag[i]].p3);

        if (R_or_A)
            summa += (Ai * Aj + Bi * Bj) * my_triag[num_triag[i]].S;
        else 
            Aij[row][column] += (Ai * Aj + Bi * Bj) * my_triag[num_triag[i]].S;
    }
    if (R_or_A)
        Rj[row] += pti.fi * summa;
}

void galerkin::FindFi()
{
    vector<int> my_num_triag;
    double first_vnut = my_point.size() - count_not_granica;
    for (int numpt1 = first_vnut; numpt1 < my_point.size(); numpt1++)
    {
        for (int numpt2 = 0; numpt2 < my_point.size(); numpt2++)
        {
            switch (two_point(my_point[numpt2], my_point[numpt1], my_num_triag))
            {
            case ij0: 
                if (numpt2 < first_vnut) Rj[numpt1 - first_vnut] += 0.;
                else Aij[numpt1 - first_vnut][numpt2 - first_vnut] += 0.; break;
            case iravenj:
                Vnutiravenj(my_point[numpt1], my_num_triag, numpt1 - first_vnut, numpt2 - first_vnut); break;
            case isosedj:
                Funcisosedj(my_point[numpt2], my_point[numpt1], my_num_triag, numpt1 - first_vnut, numpt2 - first_vnut); break;
            }
        }
    }
    kazf(Aij, Rj, Fij);
    SetTriagPoint();
    FindIzoline(num_izoline);
    FindPower();
}

std::vector<line> galerkin::GetIzoline()
{
    return izoline;
}

std::vector<std::vector<line>> galerkin::GetPower()
{
    return power;
}

int galerkin::GetNumIzoline()
{
    return izoline.size();
}

void galerkin::SetFiConst(int num_izoline)
{
    double fi_max = 0, fi_min = 0;
    for (int i = 0; i < my_point.size(); i++)
    {
        if (my_point[i].fi > fi_max) fi_max = my_point[i].fi;
        if (my_point[i].fi < fi_min) fi_min = my_point[i].fi;
    }
    fi_const.clear();

    double step = (fi_max - fi_min) / num_izoline;
    double new_step = (fi_max - fi_min - 2 * step) / num_izoline;

    for (double my_fi = fi_min + step; my_fi < fi_max - step; my_fi += new_step)
        fi_const.push_back(my_fi);
}

void galerkin::FindIzoline(int num_izoline)
{
    SetFiConst(num_izoline);
    FindIzoline();
}

bool triangle::VnutTriag(point pt)
{
    bool res = false;

    if ((pt.x > p1.x && (pt.x < p2.x || pt.x < p3.x))
        || (pt.x > p2.x && (pt.x < p1.x || pt.x < p3.x))
        || (pt.x > p3.x && (pt.x < p2.x || pt.x < p1.x)))
        if ((pt.y > p1.y && (pt.y < p2.y || pt.y < p3.y))
            || (pt.y > p2.y && (pt.y < p1.y || pt.y < p3.y))
            || (pt.y > p3.y && (pt.y < p2.y || pt.y < p1.y)))
            res = true;

    return res;
}
