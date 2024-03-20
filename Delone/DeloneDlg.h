
// DeloneDlg.h: файл заголовка
//

#pragma once
#include "picTriag.h"

// Диалоговое окно CDeloneDlg
class CDeloneDlg : public CDialogEx
{
// Создание
public:
	CDeloneDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DELONE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	picTriag oneTriag;
	int N;
	double error;
	afx_msg void OnBnClickedButton1();
	picTriag manyTriag;
	triangulation my_delone;
	CEdit quality;
	CEdit quality_center;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	UINT_PTR timer;
	HANDLE my_thread;
//	virtual void OnCancel();
	CRITICAL_SECTION cs;
	double step_fi;
	double step_setka;
	CButton rb_no_rekkur;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	bool rekkur;
	afx_msg void OnBnClickedButton2();
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CSliderCtrl e_ell;
	CSliderCtrl teta_ell;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CComboBox number_ell;
	afx_msg void OnCbnSelchangeCombo1();
	CButton but_ok;
	CButton but_add;
	CEdit ed_error;
	CEdit ed_N;
	CStatic str_e_ell;
	CStatic str_teta_ell;
	CStatic str_N;
	CStatic str_error;
	CEdit ed_di;
	CEdit ed_setka;
	CStatic str_setka;
	CStatic str_fi;
	CButton but_clear;
	double fi_okr;
	galerkin potencial;
	int num_izoline;
	virtual void OnCancel();
	double fi_ell;
	afx_msg void OnEnChangeEdit9();
	CButton but_draw_setka;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	CButton but_draw_izo;
	CEdit ed_fi_ell;
	CStatic st_fi_ell;
	CEdit ed_num_izo;
	CEdit ed_fi_okr;
	CStatic st_fi_okr;
	CStatic st_num_izo;
};

DWORD WINAPI findTriag(PVOID p);