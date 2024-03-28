
// DeloneDlg.cpp: файл реализации
//

#include "pch.h"
#define _USE_MATH_DEFINES
#include "framework.h"
#include "Delone.h"
#include "DeloneDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CDeloneDlg



CDeloneDlg::CDeloneDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DELONE_DIALOG, pParent)
	, N(3)
	, error(1e-3)
	, step_fi(0.1)
	, step_setka(0.05)
	, fi_okr(0)
	, num_izoline(5)
	, fi_ell(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDeloneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_PICONE, oneTriag);
	DDX_Text(pDX, IDC_EDIT1, N);
	DDX_Text(pDX, IDC_EDIT2, error);
	DDX_Control(pDX, IDC_MANYPIC, manyTriag);
	DDX_Control(pDX, IDC_EDIT3, quality);
	DDX_Control(pDX, IDC_EDIT4, quality_center);
	DDX_Text(pDX, IDC_EDIT5, step_fi);
	DDX_Text(pDX, IDC_EDIT6, step_setka);
	DDX_Control(pDX, IDC_RADIO2, rb_no_rekkur);
	DDX_Control(pDX, IDC_SLIDER1, e_ell);
	DDX_Control(pDX, IDC_SLIDER2, teta_ell);
	DDX_Control(pDX, IDC_COMBO1, number_ell);
	DDX_Control(pDX, IDC_BUTTON1, but_ok);
	DDX_Control(pDX, IDC_BUTTON2, but_add);
	DDX_Control(pDX, IDC_EDIT2, ed_error);
	DDX_Control(pDX, IDC_EDIT1, ed_N);
	DDX_Control(pDX, IDC_ESTR, str_e_ell);
	DDX_Control(pDX, IDC_TETASTR, str_teta_ell);
	DDX_Control(pDX, IDC_STRN, str_N);
	DDX_Control(pDX, IDC_STRERROR, str_error);
	DDX_Control(pDX, IDC_EDIT5, ed_di);
	DDX_Control(pDX, IDC_EDIT6, ed_setka);
	DDX_Control(pDX, IDC_STRSETKA, str_setka);
	DDX_Control(pDX, IDC_STRFI, str_fi);
	DDX_Text(pDX, IDC_EDIT7, fi_okr);
	DDX_Text(pDX, IDC_EDIT8, num_izoline);
	DDX_Text(pDX, IDC_EDIT9, fi_ell);
	DDX_Control(pDX, IDC_BUTTON3, but_draw_setka);
	DDX_Control(pDX, IDC_BUTTON4, but_draw_izo);
	DDX_Control(pDX, IDC_EDIT9, ed_fi_ell);
	DDX_Control(pDX, IDC_STFIELL, st_fi_ell);
	DDX_Control(pDX, IDC_EDIT8, ed_num_izo);
	DDX_Control(pDX, IDC_EDIT7, ed_fi_okr);
	DDX_Control(pDX, IDC_STFIOKR, st_fi_okr);
	DDX_Control(pDX, IDC_STNUMIZO, st_num_izo);
}

BEGIN_MESSAGE_MAP(CDeloneDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CDeloneDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO1, &CDeloneDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDeloneDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_BUTTON2, &CDeloneDlg::OnBnClickedButton2)
//	ON_WM_KEYDOWN()
ON_WM_HSCROLL()
ON_CBN_SELCHANGE(IDC_COMBO1, &CDeloneDlg::OnCbnSelchangeCombo1)
ON_EN_CHANGE(IDC_EDIT9, &CDeloneDlg::OnEnChangeEdit9)
ON_BN_CLICKED(IDC_BUTTON3, &CDeloneDlg::OnBnClickedButton3)
ON_BN_CLICKED(IDC_BUTTON4, &CDeloneDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// Обработчики сообщений CDeloneDlg

BOOL CDeloneDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	InitializeCriticalSection(&my_delone.csTriag);
	InitializeCriticalSection(&cs);
	InitializeCriticalSection(&potencial.cs);
	rb_no_rekkur.SetCheck(BST_CHECKED);
	rekkur = false;
	srand(time(NULL));

	my_ellipse add_ell(point(my_delone.centerOkr.x, my_delone.centerOkr.y, sverh), my_delone.radiusOkr, 0, 0);
	add_ell.fi_pot = 0;

	manyTriag.ells.push_back(add_ell);
	e_ell.SetRange(0, 90);
	e_ell.SetPos(70);
	teta_ell.SetRange(0, M_PI * 100);
	teta_ell.SetPos(0);

	but_add.ShowWindow(SW_HIDE);
	number_ell.ShowWindow(SW_HIDE);
	e_ell.ShowWindow(SW_HIDE);
	teta_ell.ShowWindow(SW_HIDE);
	str_e_ell.ShowWindow(SW_HIDE);
	str_teta_ell.ShowWindow(SW_HIDE);
	str_fi.ShowWindow(SW_HIDE);
	str_setka.ShowWindow(SW_HIDE);
	ed_di.ShowWindow(SW_HIDE);
	ed_setka.ShowWindow(SW_HIDE);

	add_ell = my_ellipse(point(0.3, 0.4, add), 0.1, (double)e_ell.GetPos() / 100, (double)teta_ell.GetPos() / 100 * 180. / M_PI);
	add_ell.fi_pot = 10;
	manyTriag.ells.push_back(add_ell);
	add_ell.center.x = 0.7;
	add_ell.center.y = 0.6;
	add_ell.fi_pot = -10;
	manyTriag.ells.push_back(add_ell);
	number_ell.InsertString(-1, L"1");
	number_ell.InsertString(-1, L"2");

	st_fi_okr.ShowWindow(SW_HIDE);
	st_fi_ell.ShowWindow(SW_HIDE);
	ed_fi_okr.ShowWindow(SW_HIDE);
	ed_fi_ell.ShowWindow(SW_HIDE);
	ed_num_izo.ShowWindow(SW_HIDE);
	but_draw_izo.ShowWindow(SW_HIDE);
	but_draw_setka.ShowWindow(SW_HIDE);
	st_num_izo.ShowWindow(SW_HIDE);
	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CDeloneDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CDeloneDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDeloneDlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	DWORD exit;
	GetExitCodeThread(my_thread, &exit);
	if (exit == STILL_ACTIVE) TerminateThread(my_thread, NULL);
	if (timer != NULL) KillTimer(timer);
	if (my_thread != NULL) CloseHandle(my_thread);

	UpdateData();

	if (!rekkur)
	{
		my_delone = triangulation(N, error);
		manyTriag.my_point = my_delone.GetPoint();
		manyTriag.N = N;
		manyTriag.error = error;
	}
	else
	{
		if (manyTriag.add_ellips)
		{
			MessageBox(L"Нажмите Enter!", L"Оповещение!");
			return;
		}
		manyTriag.my_izoline.clear();
		manyTriag.my_power.clear();
		manyTriag.do_triag = true;
		std::vector<my_ellipse> el(manyTriag.ells);
		el.erase(el.begin());
		my_delone = triangulation(el , step_fi, step_setka, fi_okr);
	}

	my_thread = CreateThread(NULL, NULL, findTriag, this, NULL, NULL);
	timer = SetTimer(123, 16, NULL);
}

void CDeloneDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	EnterCriticalSection(&my_delone.csTriag);

	manyTriag.SetLine(my_delone.GetTriangle());
	manyTriag.my_point = my_delone.GetPoint();

	CString str_quality;
	str_quality.Format(L"%.3f", my_delone.GetQualityMed());
	quality.SetWindowTextW(str_quality);

	str_quality.Format(L"%.3f", my_delone.GetQualityCent());
	quality_center.SetWindowTextW(str_quality);
	LeaveCriticalSection(&my_delone.csTriag);

	EnterCriticalSection(&potencial.cs);
	manyTriag.my_izoline = potencial.GetIzoline();
	manyTriag.my_power = potencial.GetPower();
	LeaveCriticalSection(&potencial.cs);


	Invalidate(FALSE);
	CDialogEx::OnTimer(nIDEvent);
}

DWORD __stdcall findTriag(PVOID p)
{
	CDeloneDlg* my = (CDeloneDlg*)p;

	my->potencial.clearvec();
	if (!my->rekkur) my->my_delone.delone_triag();
	else my->my_delone.rekkurent_delone_triag(my->step_setka);
	my->potencial.Set(my->my_delone.GetPoint(), my->my_delone.GetTriangle(), my->num_izoline, my->my_delone.pt_power);
	my->potencial.FindFi();
	return 0;
}


void CDeloneDlg::OnBnClickedRadio1()
{
	// TODO: добавьте свой код обработчика уведомлений
	rb_no_rekkur.SetCheck(BST_UNCHECKED);
	rekkur = true;
	manyTriag.do_triag = false;
	manyTriag.regim = false;
	manyTriag.my_line.clear();
	manyTriag.my_point.clear();

	DWORD exit;
	GetExitCodeThread(my_thread, &exit);
	if (exit == STILL_ACTIVE) TerminateThread(my_thread, NULL);
	if (timer != NULL) KillTimer(timer);
	if (my_thread != NULL) CloseHandle(my_thread);

	Invalidate(FALSE);

	but_add.ShowWindow(SW_SHOW);
	number_ell.ShowWindow(SW_SHOW);
	ed_N.ShowWindow(SW_HIDE);
	ed_error.ShowWindow(SW_HIDE);
	str_N.ShowWindow(SW_HIDE);
	str_error.ShowWindow(SW_HIDE);
	str_fi.ShowWindow(SW_SHOW);
	str_setka.ShowWindow(SW_SHOW);
	ed_di.ShowWindow(SW_SHOW);
	ed_setka.ShowWindow(SW_SHOW);

	st_fi_okr.ShowWindow(SW_SHOW);
	ed_fi_okr.ShowWindow(SW_SHOW);
	ed_num_izo.ShowWindow(SW_SHOW);
	st_num_izo.ShowWindow(SW_SHOW);
	but_draw_izo.ShowWindow(SW_SHOW);
	but_draw_setka.ShowWindow(SW_SHOW);
}


void CDeloneDlg::OnBnClickedRadio2()
{
	// TODO: добавьте свой код обработчика уведомлений
	rb_no_rekkur.SetCheck(BST_CHECKED);
	rekkur = false;
	manyTriag.do_triag = false;
	manyTriag.regim = true;
	manyTriag.my_line.clear();
	manyTriag.my_point.clear();

	DWORD exit;
	GetExitCodeThread(my_thread, &exit);
	if (exit == STILL_ACTIVE) TerminateThread(my_thread, NULL);
	if (timer != NULL) KillTimer(timer);
	if (my_thread != NULL) CloseHandle(my_thread);

	Invalidate(FALSE);

	but_add.ShowWindow(SW_HIDE);
	number_ell.ShowWindow(SW_HIDE);
	ed_N.ShowWindow(SW_SHOW);
	ed_error.ShowWindow(SW_SHOW);
	str_N.ShowWindow(SW_SHOW);
	str_error.ShowWindow(SW_SHOW);
	str_fi.ShowWindow(SW_HIDE);
	str_setka.ShowWindow(SW_HIDE);
	ed_di.ShowWindow(SW_HIDE);
	ed_setka.ShowWindow(SW_HIDE);

	st_fi_okr.ShowWindow(SW_HIDE);
	st_fi_ell.ShowWindow(SW_HIDE);
	ed_fi_okr.ShowWindow(SW_HIDE);
	ed_fi_ell.ShowWindow(SW_HIDE);
	ed_num_izo.ShowWindow(SW_HIDE);
	but_draw_izo.ShowWindow(SW_HIDE);
	but_draw_setka.ShowWindow(SW_HIDE);
	st_num_izo.ShowWindow(SW_HIDE);
}


void CDeloneDlg::OnBnClickedButton2()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData();
	manyTriag.do_triag = false;
	manyTriag.my_line.clear();
	manyTriag.my_point.clear();
	manyTriag.my_izoline.clear();
	manyTriag.my_power.clear();

	DWORD exit;
	GetExitCodeThread(my_thread, &exit);
	if (exit == STILL_ACTIVE) TerminateThread(my_thread, NULL);
	if (timer != NULL) KillTimer(timer);
	if (my_thread != NULL) CloseHandle(my_thread);

	int max_try = 0;
	while (true)
	{
		manyTriag.cur_ell = manyTriag.ells.size();
		my_ellipse add_ell(point(myrand(0, 1), myrand(0, 1), add), 0.05, (double)e_ell.GetPos() / 100, (double)teta_ell.GetPos() / 100 * 180. / M_PI);
		add_ell.fi_pot = 10;
		manyTriag.ells.push_back(add_ell);
		if (!manyTriag.goodEll())
		{
			manyTriag.ells.pop_back();
			manyTriag.cur_ell--;
		}
		else
		{
			manyTriag.cur_ell = manyTriag.ells.size() - 1;
			break;
		}
		if (max_try > 500)
		{
			MessageBox(L"Мало места!", L"Оповещение!");
			manyTriag.add_ellips = false;
			e_ell.ShowWindow(SW_HIDE);
			teta_ell.ShowWindow(SW_HIDE);
			str_e_ell.ShowWindow(SW_HIDE);
			str_teta_ell.ShowWindow(SW_HIDE);
			//break;
			return;
		}
		max_try++;
	}
	manyTriag.add_ellips = true;

	CString str;
	str.Format(L"%d", manyTriag.cur_ell);

	number_ell.InsertString(-1, str);
	Invalidate(FALSE);

	e_ell.ShowWindow(SW_SHOW);
	teta_ell.ShowWindow(SW_SHOW);
	str_e_ell.ShowWindow(SW_SHOW);
	str_teta_ell.ShowWindow(SW_SHOW);

	fi_ell = manyTriag.ells[manyTriag.cur_ell].fi_pot;

	st_fi_ell.ShowWindow(SW_SHOW);
	ed_fi_ell.ShowWindow(SW_SHOW);

	UpdateData(FALSE);
}

BOOL CDeloneDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: добавьте специализированный код или вызов базового класса
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			manyTriag.add_ellips = false;
			Invalidate(FALSE);

			e_ell.ShowWindow(SW_HIDE);
			teta_ell.ShowWindow(SW_HIDE);
			str_e_ell.ShowWindow(SW_HIDE);
			str_teta_ell.ShowWindow(SW_HIDE);
			st_fi_ell.ShowWindow(SW_HIDE);
			ed_fi_ell.ShowWindow(SW_HIDE);

			return TRUE;                    // DO NOT process further
		}
		if (pMsg->wParam == VK_DELETE)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			if (manyTriag.add_ellips)
			{
				manyTriag.add_ellips = false;
				int s = manyTriag.ells.size() - 2;
				number_ell.DeleteString(s);
				manyTriag.ells.erase(manyTriag.ells.begin() + manyTriag.cur_ell);
				manyTriag.cur_ell--;
				e_ell.ShowWindow(SW_HIDE);
				teta_ell.ShowWindow(SW_HIDE);
				str_e_ell.ShowWindow(SW_HIDE);
				str_teta_ell.ShowWindow(SW_HIDE);
				st_fi_ell.ShowWindow(SW_HIDE);
				ed_fi_ell.ShowWindow(SW_HIDE);
				Invalidate(FALSE);
			}
			return TRUE;                    // DO NOT process further
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDeloneDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного

	if (manyTriag.cur_ell == 0 || !manyTriag.add_ellips) return;
	int position_e = e_ell.GetPos();
	int position_teta = teta_ell.GetPos();
	double cur_fi = manyTriag.ells[manyTriag.cur_ell].fi_pot;
	my_ellipse cur(manyTriag.ells[manyTriag.cur_ell].center, manyTriag.ells[manyTriag.cur_ell].a, (double)position_e / 100, (double)position_teta / 100 * 180. / M_PI);
	if (!manyTriag.goodEll()) return;
	cur.fi_pot = cur_fi;
	manyTriag.ells[manyTriag.cur_ell] = cur;

	Invalidate(FALSE);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDeloneDlg::OnCbnSelchangeCombo1()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData();
	manyTriag.do_triag = false;
	manyTriag.my_line.clear();
	manyTriag.my_point.clear();
	manyTriag.my_izoline.clear();
	manyTriag.my_power.clear();

	DWORD exit;
	GetExitCodeThread(my_thread, &exit);
	if (exit == STILL_ACTIVE) TerminateThread(my_thread, NULL);
	if (timer != NULL) KillTimer(timer);
	if (my_thread != NULL) CloseHandle(my_thread);

	manyTriag.cur_ell = number_ell.GetCurSel() + 1;
	manyTriag.add_ellips = true;
	e_ell.SetPos(manyTriag.ells[manyTriag.cur_ell].e * 100);
	teta_ell.SetPos(manyTriag.ells[manyTriag.cur_ell].teta * 100 * M_PI / 180);
	fi_ell = manyTriag.ells[manyTriag.cur_ell].fi_pot;
	Invalidate(FALSE);
	e_ell.ShowWindow(SW_SHOW);
	teta_ell.ShowWindow(SW_SHOW);
	str_e_ell.ShowWindow(SW_SHOW);
	str_teta_ell.ShowWindow(SW_SHOW);
	st_fi_ell.ShowWindow(SW_SHOW);
	ed_fi_ell.ShowWindow(SW_SHOW);
	UpdateData(FALSE);
}

void CDeloneDlg::OnCancel()
{
	// TODO: добавьте специализированный код или вызов базового класса
	DeleteCriticalSection(&potencial.cs);
	CDialogEx::OnCancel();
}


void CDeloneDlg::OnEnChangeEdit9()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// функция и вызов CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
	UpdateData();
	manyTriag.ells[manyTriag.cur_ell].fi_pot = fi_ell;
	UpdateData(FALSE);
}


void CDeloneDlg::OnBnClickedButton3()
{
	// TODO: добавьте свой код обработчика уведомлений
	manyTriag.draw_setka = !manyTriag.draw_setka;
	if (!manyTriag.draw_setka)
		but_draw_setka.SetWindowTextW(L"Показать сетку");
	else 
		but_draw_setka.SetWindowTextW(L"Скрыть сетку");
}


void CDeloneDlg::OnBnClickedButton4()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData();
	potencial.FindIzoline(num_izoline);
}
