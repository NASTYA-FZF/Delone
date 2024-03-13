#include "pch.h"
#define _USE_MATH_DEFINES
#include "my_triag.h"
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
    //vector<point> my_point;
    //for (double i = 0; i < 1; i += 0.1)
    //{
    //    for (double j = 0; j < 1; j += 0.1)
    //    {
    //        my_point.push_back(point(i + myrand(-error, error), j + myrand(-error, error), add));
    //    }
    //}

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

    double eps = 1.e-6f;
    //float s;
    int i, j, k;
    double s1, s2, fa1, t;
    vector<double> x1(x.size());

    x[0] = 0.5f;
    for (i = 1; i < nn; i++)  x[i] = 0.f;

    s1 = s2 = 1.f;
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
        //N++;
    }

    if (ctrl.x + step >= 1) ctrl = point(step + myrand(-error, error), ctrl.y + step + myrand(-error, error), pt_new);
    else ctrl.x += step + myrand(-error, error);

    //if (ctrl.y + step >= 1) ctrl = point(ctrl.x + step + myrand(-error, error), step + myrand(-error, error), pt_new);
    //else ctrl.y += step + myrand(-error, error);

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
    error = 1e-3;
    InitializeCriticalSection(&csTriag);

    //radiusOkr = 0.4;
    //centerOkr.y = centerOkr.x = 0.5;

    //my_point.push_back(point(myrand(-error, error), myrand(-error, error), sverh));
    //my_point.push_back(point(myrand(-error, error), 1 + myrand(-error, error), sverh));
    //my_point.push_back(point(1 + myrand(-error, error), myrand(-error, error), sverh));
    //my_point.push_back(point(1 + myrand(-error, error), 1 + myrand(-error, error), sverh));

    //point na_el;
    //for (auto el : ells)
    //{
    //    //my_point.push_back(point(el.center.x + myrand(-error, error), el.center.y + myrand(-error, error), sverh));
    //    //my_point.push_back(point(el.Ac.x + myrand(-error, error), el.Ac.y + myrand(-error, error), sverh));
    //    my_point.push_back(point(el.Bc.x + myrand(-error, error), el.Bc.y + myrand(-error, error), sverh));

    //    for (double fi = 0.; fi <= 2 * M_PI + stepfi * 4; fi += stepfi * 3)
    //    {
    //        na_el = el.koordnew(fi);
    //        my_point.push_back(point(na_el.x + myrand(-error, error), na_el.y + myrand(-error, error), add));
    //    }
    //}

    //for (double fi = 0.; fi < 2 * M_PI; fi += stepfi)
    //{
    //    my_point.push_back(point(radiusOkr * cos(fi) + centerOkr.x + myrand(-error, error), radiusOkr * sin(fi) + centerOkr.y + myrand(-error, error), add));
    //    //for (auto el : ells)
    //    //{
    //    //    na_el = el.koordnew(fi);
    //    //    my_point.push_back(point(na_el.x + myrand(-error, error), na_el.y + myrand(-error, error), add));
    //    //}
    //}

    //for (double x = 0; x < 1; x += step_setka)
    //{
    //    for (double y = 0; y < 1; y += step_setka)
    //    {
    //        pryam_setka.push_back(point(x, y, pt_new));
    //    }
    //}

    my_point.push_back(point(0, 0, sverh));
    my_point.push_back(point(0, 1, sverh));
    my_point.push_back(point(1, 0, sverh));
    my_point.push_back(point(1, 1, sverh));

    point na_el;
    for (auto& el : ells)
    {
        el.teta = el.teta * M_PI / 180;
        el = my_ellipse(el.center, el.a, el.e, el.teta);

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

        //my_point.push_back(point(el.Ac.x, el.Ac.y, sverh));
        //my_point.push_back(point(el.Bc.x, el.Bc.y, sverh));

        for (double fi = 0.; fi <= 2 * M_PI + stepfi * 4; fi += stepfi * 3)
        {
            na_el = el.koordnew(fi);
            my_point.push_back(point(na_el.x, na_el.y, add));
        }
    }

    for (double fi = 0.; fi < 2 * M_PI; fi += stepfi)
    {
        my_point.push_back(point(radiusOkr * cos(fi) + centerOkr.x + myrand(-error, error), radiusOkr * sin(fi) + centerOkr.y + myrand(-error, error), add));
    }

    //for (double x = 0; x < 1; x += step_setka)
    //{
    //    for (double y = 0; y < 1; y += step_setka)
    //    {
    //        pryam_setka.push_back(point(x, y, pt_new));
    //    }
    //}
}

void triangulation::delone_triag()
{
    double radius;
    point center;
    triangle cur_triangle;
    bool add_triag = true;
    bool only_back = false;
    N = my_point.size();

    //for (int i = 0; i < 4 + 3 * ells.size(); i++) my_point[i].st_pt = sverh;

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
                    //CalcQuality(cur_triangle.p1, cur_triangle.p2, cur_triangle.p3);
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
    point add_point(step_setka + myrand(-error, error), step_setka + myrand(-error, error), pt_new);
    int iter = 0;
    while (true)
    {
        if (add_point.y > centerOkr.y + radiusOkr) break;
        if (!add_or_not(add_point, step_setka)) continue;
        //add_point = point(0.39 + myrand(-error, error), 0.5 + myrand(-error, error), pt_new);
        //my_point.push_back(add_point);
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
                if (my_point[j] == my_setka[i].p1 || my_point[j] == my_setka[i].p2 || my_point[j] == my_setka[i].p3
                    /*|| my_setka[i].p1.st_pt != sverh || my_setka[i].p2.st_pt != sverh
                    || my_setka[i].p3.st_pt != sverh*/) my_point[j].st_pt = (my_point[j].st_pt == sverh) ? sverh : add;
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
