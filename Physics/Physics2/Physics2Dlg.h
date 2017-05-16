
// Physics2Dlg.h : файл заголовка
//

#pragma once
#include "afxwin.h"
#include "PhysicsF.h"
#include "afxcmn.h"

enum{LG_2D_XT = 0, LG_2D_YT, LG_2D_VT, LG_2D_AT, LG_2D_ANT, LG_2D_ATT, LG_2D_YX};
enum{LG_RND_FT = 0, LG_RND_VT, LG_RND_AT, LG_RND_ANT, LG_RND_ATT};

// диалоговое окно CPhysics2Dlg
class CPhysics2Dlg : public CDialogEx
{
// Создание
public:
	CPhysics2Dlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
	enum { IDD = IDD_PHYSICS2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV

	// Функции отрисовки графиков
	void writeGraphic(CAnimateCtrl & graphCntrl, std::vector<std::pair<float, float>> & XY);

// Реализация
protected:
	HICON m_hIcon;

	// Нужно ли отрисовать графики
	bool needWrite;

	CFont acrossFont,	// Горизонтальный шрифт
		downFont;		// Вертикальный шрифт
	CPen lineBrush,		// Перо для рисования осей
		netBrush,		// Перо для отрисвоки сетки
		graphBrush;		// Перо для отрисовки графика

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// Выбор: двумерные координаты или окружность
	int radio;
	// Кнопка "Построить"
	afx_msg void OnBnClickedCalc();
	// Контроллер поля ввода X
	CEdit xVarEditCtrl;
	// Контроллер поля ввода уравнения y(t)
	CEdit yVarEditCtrl;
	// Контроллер ввода уравнения ф(t)
	CEdit fVarEditCtrl;

	// Контроллер графика x(t)
	CAnimateCtrl graphicCtrl;
	// Переключение между двумя режимами
	afx_msg void OnBnClickedRadio2d();
	afx_msg void OnBnClickedRadioRnd();
	// Котроллеры обозначений
	// По вертикали
	CStatic Symb2D_XCtrl;
	CStatic Symb2D_YCtrl;
	CStatic SymbRND_FCtrl;
	CStatic Symb2D_VCtrl;
	CStatic SymbRND_VCtrl;
	CStatic Symb_AtCtrl;
	CStatic Symb_AnCtrl;
	CStatic Symb_ACtrl;
	// По горизонтали
	CStatic Symb_H_XCtrl;
	CStatic Symb_H_TCtrl;

	void SetSymb(int familyID, int symbID);

	// Контроллер списка графов
	CComboBox listOfGraphsCtrl;
	CEdit tStValCtrl;
	CEdit tEdValCtrl;
	CEdit tStepValCtrl;
};
