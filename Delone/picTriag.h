#pragma once
#include "my_triag.h"
#include <gdiplus.h>
// picTriag


class picTriag : public CStatic
{
	DECLARE_DYNAMIC(picTriag)

public:
	picTriag();
	virtual ~picTriag();

protected:
	DECLARE_MESSAGE_MAP()

	void DrawKrug(Gdiplus::Graphics& gr, double radius, point center, Gdiplus::SolidBrush& brush, Gdiplus::Matrix& matr);
	void DrawOkr(Gdiplus::Graphics& gr, double radius, point center, Gdiplus::Pen& pen, Gdiplus::Matrix& matr);
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//std::set<line> my_line;
	std::vector<line> my_line, my_izoline;
	std::vector<std::vector<line>> my_power;
	std::vector<point> my_point;
	ULONG_PTR token;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	double xMax, xMin, yMax, yMin;
	double error;
	int N;
	void SetLine(std::vector<triangle> my_setka);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	bool but_down;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CPoint start_point;

	double Xscale, Yscale, Xtrans, Ytrans;
	bool first_start, draw_pt;
	Gdiplus::RectF rect_pic;

	bool add_ellips;
	std::vector<my_ellipse> ells;
	void DrawMyEllipse(Gdiplus::Graphics& draw, Gdiplus::Matrix& matr);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	double otstup;
	int cur_ell;
	bool goodEll();
	bool regim, do_triag, draw_setka;
};

bool operator<(const line& left, const line& right);

