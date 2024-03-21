// picTriag.cpp: файл реализации
//

#include "pch.h"
#define _USE_MATH_DEFINES
#include "Delone.h"
#include "picTriag.h"
using namespace std;
using namespace Gdiplus;

// picTriag

IMPLEMENT_DYNAMIC(picTriag, CStatic)

bool operator<(const line& left, const line& right)
{
	if (radiusOp(left.first, left.second) < radiusOp(right.first, right.second)
		&& left.first.x < right.first.x && left.first.y < right.first.y
		&& left.second.x < right.second.x && left.second.y < right.second.y
		) return true;
	else return false;
}

picTriag::picTriag()
{
	srand(time(NULL));
	GdiplusStartupInput input;
	Status s;
	s = GdiplusStartup(&token, &input, NULL);
	if (s != Ok) MessageBox(L"s != Ok", L"Error!");

	xMax = 1;
	xMin = 0;
	yMax = 1;
	yMin = 0;
	but_down = false;
	first_start = true;
	add_ellips = false;
	draw_pt = true;
	regim = true;
	do_triag = false;
	draw_setka = true;
	cur_ell = ells.size() - 1;
}

picTriag::~picTriag()
{
	GdiplusShutdown(token);
}


BEGIN_MESSAGE_MAP(picTriag, CStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
//	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// Обработчики сообщений picTriag




void picTriag::DrawKrug(Gdiplus::Graphics& gr, double radius, point center, Gdiplus::SolidBrush& brush, Gdiplus::Matrix& matr)
{
	PointF pt[2];
	pt[0] = PointF(center.x - radius, center.y + radius);
	pt[1] = PointF(center.x + radius, center.y - radius);
	matr.TransformPoints(pt, 2);
	gr.FillEllipse(&brush, RectF(pt[0].X, pt[0].Y, (pt[1].X - pt[0].X), (pt[1].Y - pt[0].Y)));
}

void picTriag::DrawOkr(Gdiplus::Graphics& gr, double radius, point center, Gdiplus::Pen& pen, Gdiplus::Matrix& matr)
{
	PointF pt[2];
	pt[0] = PointF(center.x - radius, center.y + radius);
	pt[1] = PointF(center.x + radius, center.y - radius);
	matr.TransformPoints(pt, 2);
	gr.DrawEllipse(&pen, RectF(pt[0].X, pt[0].Y, (pt[1].X - pt[0].X), (pt[1].Y - pt[0].Y)));
}

void picTriag::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  Добавьте исходный код для отображения указанного элемента

	if (first_start)
	{
		double storona = lpDrawItemStruct->rcItem.right < lpDrawItemStruct->rcItem.bottom ? lpDrawItemStruct->rcItem.right : lpDrawItemStruct->rcItem.bottom;
		otstup = 30.;
		rect_pic = RectF(
			otstup,
			otstup,
			storona - 2 * otstup,
			storona - 2 * otstup
		);

		Xscale = rect_pic.Width / (xMax - xMin);
		Yscale = rect_pic.Height / (yMax - yMin);

		Xtrans = rect_pic.X - xMin * rect_pic.Width / (xMax - xMin);
		Ytrans = rect_pic.Y + rect_pic.Height + yMin * rect_pic.Height / (yMax - yMin);

		start_point.x = rect_pic.Width / 2;
		start_point.y = rect_pic.Height / 2;

		first_start = false;
	}

	Graphics wnd(lpDrawItemStruct->hDC);
	Bitmap buffer(lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, &wnd);
	Graphics draw_in_buffer(&buffer);
	draw_in_buffer.SetSmoothingMode(SmoothingModeAntiAlias);

	draw_in_buffer.Clear(Color::White);

	Matrix matr;

	Pen p_black(Color::Black), p_gold(Color::Green), p_purp(Color::Purple);
	SolidBrush r_brush(Color::Red);

	if (!regim && !do_triag) DrawMyEllipse(draw_in_buffer, matr);

	matr.Translate(Xtrans, Ytrans);
	matr.Scale(Xscale, -Yscale);
	PointF pt[2];

	if (draw_setka)
	{
		for (auto n : my_line)
		{
			pt[0] = PointF(n.first.x, n.first.y);
			pt[1] = PointF(n.second.x, n.second.y);
			matr.TransformPoints(pt, 2);
			draw_in_buffer.DrawLine(&p_black, pt[0], pt[1]);
		}
	}

	if (!my_izoline.empty())
	{
		for (auto n : my_izoline)
		{
			pt[0] = PointF(n.first.x, n.first.y);
			pt[1] = PointF(n.second.x, n.second.y);
			matr.TransformPoints(pt, 2);
			draw_in_buffer.DrawLine(&p_purp, pt[0], pt[1]);
		}
	}

	if (!my_power.empty())
	{
		for (auto n : my_power)
		{
			pt[0] = PointF(n.first.x, n.first.y);
			pt[1] = PointF(n.second.x, n.second.y);
			matr.TransformPoints(pt, 2);
			draw_in_buffer.DrawLine(&p_gold, pt[0], pt[1]);
		}
	}

	if (draw_pt)
	{
		FontFamily my_font_family(L"Arial");
		Gdiplus::Font my_font(&my_font_family, 14, FontStyleRegular, UnitPixel);
		SolidBrush brush_font(Color::Black);
		PointF pt;
		wchar_t number[4];
		double rad_pt = (xMax - xMin + yMax - yMin) / 400;
		int iter = 0;
		for (auto n : my_point)
		{
			DrawKrug(draw_in_buffer, rad_pt, n, r_brush, matr);
			pt = PointF(n.x, n.y);
			matr.TransformPoints(&pt);
			swprintf_s(number, L"%d", iter);
			draw_in_buffer.DrawString(number, -1, &my_font, pt, &brush_font);
			iter++;
		}
	}

	if (N == 3 && regim)
	{
		if (!my_point.empty())
		{
			auto cent = centerOp(my_point[0], my_point[1], my_point[2]);
			double rad = radiusOp(cent, my_point[1]);

			DrawOkr(draw_in_buffer, rad, cent, p_gold, matr);

			cent = centerVp(my_point[0], my_point[1], my_point[2]);
			rad = radiusVp(my_point[0], my_point[1], my_point[2]);

			DrawOkr(draw_in_buffer, rad, cent, p_purp, matr);
		}
	}

	wnd.DrawImage(&buffer, 0, 0, 0, 0, lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, UnitPixel);
}


void picTriag::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	//if (N == 3)
	//{
	//	my_point = gen_point(N, error);
	//	my_line.clear();
	//	my_line.insert(line(my_point[0], my_point[1]));
	//	my_line.insert(line(my_point[2], my_point[1]));
	//	my_line.insert(line(my_point[0], my_point[2]));
	//	Invalidate(FALSE);
	//}
	
	but_down = true;
	start_point = point;
	CStatic::OnLButtonDown(nFlags, point);
}

void picTriag::SetLine(std::vector<triangle> my_setka)
{
	my_line.clear();
	for (auto n : my_setka)
	{
		my_line.push_back(line(n.p1, n.p2));
		my_line.push_back(line(n.p1, n.p3));
		my_line.push_back(line(n.p3, n.p2));
	}
}


BOOL picTriag::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	double scale = 20, delta_a = 0.01;
	double last_a, dopusk = 0.05;

	if (zDelta > 0)
	{
		if (!add_ellips)
		{
			Xscale += scale;
			Yscale += scale;
		}
		else
		{
			last_a = ells[cur_ell].a;
			if (last_a - delta_a > dopusk)
				ells[cur_ell].a -= delta_a;
			//ells[cur_ell].new_b();
			if (!goodEll())
			{
				//MessageBox(L"Невозможно расположить таким образом эллипс!", L"Предупреждение!");
				ells[cur_ell].a = last_a;
			}
			else ells[cur_ell].new_b();
		}
	}
	else
	{
		if (!add_ellips)
		{
			if (Xscale - scale < 0 || Yscale - scale < 0)
			{
				MessageBox(L"Дальше отдалять нельзя!", L"Предупреждение!");
				return CStatic::OnMouseWheel(nFlags, zDelta, pt);
			}
			Xscale -= scale;
			Yscale -= scale;
		}
		else
		{
			last_a = ells[cur_ell].a;
			ells[cur_ell].a += delta_a;
			if (!goodEll())
			{
				//MessageBox(L"Невозможно расположить таким образом эллипс!", L"Предупреждение!");
				ells[cur_ell].a = last_a;
			}
			else ells[cur_ell].new_b();
		}
	}

	//RECT main_rect;
	//GetWindowRect(&main_rect);

	//double xTrans = pt.x - start_point.x - main_rect.left;
	//double yTrans = pt.y - start_point.y - main_rect.top;

	//Xtrans -= xTrans;///////////////////////скалирование к точке
	//Ytrans -= yTrans;

	//CPoint pot(pt.x - main_rect.left, pt.y - main_rect.top);
	//start_point = pot;

	Invalidate(FALSE);
	return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}


void picTriag::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	but_down = false;
	CStatic::OnLButtonUp(nFlags, point);
}


void picTriag::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (but_down)
	{

		double xTrans = point.x - start_point.x;
		double yTrans = point.y - start_point.y;
		double dopusk = 1;

		if (!add_ellips)
		{
			Xtrans += xTrans;
			Ytrans += yTrans;
		}
		else
		{
			//PointF pt(point.x - rect_pic.X, point.y - rect_pic.Y);
			PointF pt(xTrans, yTrans);
			PointF last_center(ells[cur_ell].center.x, ells[cur_ell].center.y);
			Matrix matr;
			matr.Scale(1 / Xscale, 1 / Yscale);
			matr.TransformPoints(&pt);
			/*ells[cur_ell].center.x = pt.X;
			ells[cur_ell].center.y = 1 - pt.Y;*/
			ells[cur_ell].center.x += pt.X;
			ells[cur_ell].center.y += -pt.Y;

			if (!goodEll())
			{
				//MessageBox(L"Невозможно расположить таким образом эллипс!", L"Предупреждение!");
				ells[cur_ell].center.x = last_center.X;
				ells[cur_ell].center.y = last_center.Y;
				//but_down = false;
			}
		}

		start_point = point;
		Invalidate(FALSE);
	}

	CStatic::OnMouseMove(nFlags, point);
}

void picTriag::DrawMyEllipse(Gdiplus::Graphics& draw, Gdiplus::Matrix& matr)
{
	Pen p_ell(Color::Red, 0.001), p_blue(Color::Blue, 0.001);

	FontFamily my_font_family(L"Arial");
	Gdiplus::Font my_font(&my_font_family, 14, FontStyleRegular, UnitPixel);
	SolidBrush brush_font(Color::Black);
	PointF pt;
	wchar_t number[3];
	int iter = 0;

	for (auto el : ells)
	{
		matr.Reset();
		matr.Translate(Xtrans, Ytrans);
		matr.Scale(Xscale, -Yscale);

		if (iter > 0)
		{
			pt = PointF(el.center.x, el.center.y);
			matr.TransformPoints(&pt);
			swprintf_s(number, L"%d", iter);
			draw.DrawString(number, -1, &my_font, pt, &brush_font);
		}

		matr.RotateAt(el.teta /** 180 / M_PI*/, PointF(el.center.x, el.center.y));
		draw.SetTransform(&matr);
		if (iter == cur_ell && add_ellips) 
			draw.DrawEllipse(&p_blue, RectF(el.center.x - el.a, el.center.y - el.b, 2 * el.a, 2 * el.b));
		else
			draw.DrawEllipse(&p_ell, RectF(el.center.x - el.a, el.center.y - el.b, 2 * el.a, 2 * el.b));
		draw.ResetTransform();
		iter++;
	}
	matr.Reset();
}


void picTriag::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	draw_pt = !draw_pt;
	CStatic::OnRButtonDown(nFlags, point);
}

bool picTriag::goodEll()
{
	int size = ells.size();
	bool res = true;

	for (int i = 1; i < size; i++)
	{
		if (i != cur_ell)
			if (peresEll(ells[cur_ell], ells[i])) 
				res = false;
	}

	if (vneOkr(ells[cur_ell], ells[0])) res = false;

	return res;
}
