// Physics2Dlg.cpp : файл реализации
//

#include "stdafx.h"
#include "Physics2.h"
#include "Physics2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#undef max
#undef min

// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// диалоговое окно CPhysics2Dlg
CPhysics2Dlg::CPhysics2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPhysics2Dlg::IDD, pParent)
	, radio(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// Собираем шрифт
	LOGFONT lfontAcross = { 13, 7, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL };
	LOGFONT lfontDown = { 11, 6, 900, 900, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL };
	acrossFont.CreateFontIndirect(&lfontAcross);
	downFont.CreateFontIndirect(&lfontDown);
	lineBrush.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));	// Создаем новое перо
}

void CPhysics2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_2D, radio);
	DDX_Control(pDX, IDC_EDIT_2D_X, xVarEditCtrl);
	DDX_Control(pDX, IDC_GRAPHIC, graphicCtrl);
	DDX_Control(pDX, IDC_EDIT_2D_Y, yVarEditCtrl);
	DDX_Control(pDX, IDC_EDIT_RND, fVarEditCtrl);
	DDX_Control(pDX, IDC_2D_X, Symb2D_XCtrl);
	DDX_Control(pDX, IDC_2D_Y, Symb2D_YCtrl);
	DDX_Control(pDX, IDC_RND_F, SymbRND_FCtrl);
	DDX_Control(pDX, IDC_V_2D, Symb2D_VCtrl);
	DDX_Control(pDX, IDC_V_RND, SymbRND_VCtrl);
	DDX_Control(pDX, IDC_LIST_OF_GRAPHS, listOfGraphsCtrl);
	DDX_Control(pDX, IDC_AT, Symb_AtCtrl);
	DDX_Control(pDX, IDC_AN, Symb_AnCtrl);
	DDX_Control(pDX, IDC_A, Symb_ACtrl);
	DDX_Control(pDX, IDC_H_X, Symb_H_XCtrl);
	DDX_Control(pDX, IDC_H_T, Symb_H_TCtrl);
	DDX_Control(pDX, IDC_T_STVAL_EDIT, tStValCtrl);
	DDX_Control(pDX, IDC_T_EDVAL_EDIT, tEdValCtrl);
	DDX_Control(pDX, IDC_T_STEPVAL_EDIT, tStepValCtrl);
}

BEGIN_MESSAGE_MAP(CPhysics2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_CALC, &CPhysics2Dlg::OnBnClickedCalc)
	ON_BN_CLICKED(IDC_RADIO_2D, &CPhysics2Dlg::OnBnClickedRadio2d)
	ON_BN_CLICKED(IDC_RADIO_RND, &CPhysics2Dlg::OnBnClickedRadioRnd)
END_MESSAGE_MAP()


// обработчики сообщений CPhysics2Dlg
BOOL CPhysics2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	OnBnClickedRadio2d();

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CPhysics2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CPhysics2Dlg::OnPaint()
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
HCURSOR CPhysics2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Функция отрисовки графика (для простоты не парюсь с отрицательными значениями x > 0, значит мне нужны 1 и 4 четверти гафика, ось OX всегда слева)
void CPhysics2Dlg::writeGraphic(CAnimateCtrl & graphCtrl, std::vector<std::pair<float, float>> & vals, float stVal, float edVal, float stepVal){
	// Первым делом избавляемся от аномалий в числах (аля 0 == 10^[-11])
	for (std::vector<std::pair<float, float>>::iterator i = vals.begin(); i < vals.end(); i++){
		i->first = std::floor((i->first) * ACCURACYN + .5) / ACCURACYN;
		i->second = std::floor((i->second) * ACCURACYN + .5) / ACCURACYN;
	}
	
	CPaintDC * dc = (CPaintDC*)graphCtrl.GetDC();
	CFont * oldFont = dc->SelectObject(&acrossFont);		// Подключаем Горизонтальный шрифт

	// Область отрисовки графика
	RECT rect;
	graphCtrl.GetClientRect(&rect);

	// Очистка окна
	dc->FillRect(&rect, &CBrush(RGB(255, 255, 255)));

	// Если выражение пустое
	if (vals.empty()){
		return;
	}

	// Определяем границы по оси y
	float maxVal=0, minVal=0; 
	size_t i;
	for (i = 0; i < vals.size() && !isfinite(vals[i].second); i++); // ищем первое конечное число
	if (i < vals.size())					// если нашли
		maxVal = minVal = vals[i].second;	// запоминаем его как максимум и минимум

	for (std::vector<std::pair<float, float>>::const_iterator ci = vals.begin() + i; ci < vals.end(); ci++){
		if (ci->second > maxVal && isfinite(ci->second)) // Ищем максимум
			maxVal = ci->second;
		else if (ci->second < minVal && isfinite(ci->second)) // и мимнимум функции
			minVal = ci->second;
	}

	// Считаем диапазон значений y и X
	float xDiap = MAX3(edVal - stVal, abs(edVal), abs(stVal));
	float yDiap = MAX3(maxVal - minVal, abs(maxVal), abs(minVal));

	// Считаем размер шага штрихов по оси Y
	float stepX = xDiap / 10;
	float stepY = yDiap / 10;

	// Коэффициенты для вывода графика в окно
	float kx = (rect.right - (rect.left + (stVal >= 0 || edVal <= 0) ? 14 : 0)) / (xDiap * 1.2/*Запас, чтобы красиво было*/);
	float ky = (rect.bottom - (rect.top + (minVal >= 0 || maxVal <= 0) ? 14 : 0)) / (yDiap * 1.2/*Запас, чтобы красиво было*/);

	// Сдвиг графика относительно окна (15 - отступ под вывод размера шага)
	int dx = (stVal >= 0) ? ((edVal == 0) ? (rect.right / 2) : stepX*kx) : ((edVal <= 0) ? (rect.right - stepX*kx) : abs(stVal * kx - stepX*kx));
	int dy = (minVal >= 0) ? ((maxVal == 0) ? (rect.bottom/2) : stepY*ky) : ((maxVal <= 0) ? (rect.bottom - stepY*ky) : abs(minVal * ky - stepY*ky));

	CPen drowBrush;
	drowBrush.CreatePen(PS_DOT, 1, RGB(0, 0, 0));	// Создаем новое перо
	dc->SelectObject(drowBrush);					// и подключаем его

	// Сетка значений на осях
	for (float x = rect.left + (dx % (int)(stepX * kx)); x < rect.right; x += (stepX * kx)){
		dc->MoveTo({ (int)x, rect.bottom});
		dc->LineTo({ (int)x, rect.top});
	}
	if (abs(stepY) > ACCURACY){ // если шаг не равен 0
		for (float y = rect.bottom - dy; y > rect.top; y -= (stepY * ky)){
			dc->MoveTo({ rect.left, (int)y });
			dc->LineTo({ rect.right, (int)y });
		}
		for (float y = rect.bottom - dy; y < rect.bottom; y += (stepY * ky)){
			dc->MoveTo({ rect.left, (int)y });
			dc->LineTo({ rect.right, (int)y });
		}
	}

	// Отрисовка осей графика
	dc->SelectObject(lineBrush); // Переключаемся на кисть для рисования осей
	// OX
	dc->MoveTo({ rect.left, rect.bottom - dy });
	dc->LineTo({ rect.right, rect.bottom - dy });
	// OY
	dc->MoveTo({ rect.left + dx, rect.bottom });
	dc->LineTo({ rect.left + dx, rect.top });

	// Если в массиве есть значения, то начинаем отрисовку
	if (!vals.empty()){
		drowBrush.DeleteObject();
		drowBrush.CreatePen(PS_SOLID, 4, RGB(0, 150, 100));	// Создаем новое перо
		dc->SelectObject(drowBrush);						// и подключаем его

		// Пропускаем все нечисла
		size_t i;
		for (i = 0; i < vals.size() && isnan(vals[i].second); i++); 

		// Рисуем график
		bool moveLine = true;
		for (std::vector<std::pair<float, float>>::const_iterator ci = vals.begin() + i; ci < vals.end(); ci++){
			if (isnan(ci->second)){ // Если не число
				moveLine = true;
				continue;
			}
			if (ci > vals.begin() && isinf(ci->second) && isinf(((ci - 1)->second)))	// или две подряд идущие бесконечности
				moveLine = true; // то делаем разрыв

			// Рассчитваем координаты точки в окне
			POINT pI, pO;
			if (isinf(ci->second)){ // Если график по y уходит в бесконечность
				// сначала урезаем его до границы окна
				if (ci->second < 0)
					pI.y = pO.y = rect.bottom;
				else
					pI.y = pO.y = rect.top;

				// сдвигаем по x в сторону предыдущей точки
				int k = 1;
				float sum = ci->first;
				if (ci > vals.begin() && isfinite((ci-1)->second)){ // Если есть предыдущая точка
					sum += 10 * (ci - 1)->first;							// то сдвигаем точку в её сторону
					k += 10;
				}
				pI.x = rect.left + dx + sum / k * kx;

				// сдвигаем по x в сторону следующей точки
				k = 1;
				sum = ci->first;
				if (ci < vals.end()-1 && isfinite((ci+1)->second)){ // Если есть следующая точка
					sum += 10 * (ci + 1)->first;							// то сдвигаем точку в её сторону
					k += 10;
				}
				pO.x = rect.left + dx + sum / k * kx;
			}
			else{ // Если это обычная точка графика, то линии входащая и исходящая пересекаются в этой точке
				pI.x = pO.x = rect.left + dx + ci->first * kx;
				pI.y = pO.y = rect.bottom - dy - ci->second * ky;
			}


			// Делаем разрыв или рисуем линию
			if (moveLine)
				moveLine = false;
			else
				dc->LineTo(pI);

			dc->MoveTo(pO);
		}
	}

	// Отображаем значения шагов
	CString numX, numY;
	int textX, textY;
	numX.Format(L"%.3g", (edVal < abs(stVal)) ? (-stepX) : (stepX));
	// Задаем координаты вывода
	textX = rect.left + dx;
	textY = rect.bottom - dy;
	if (numX[0] == '-'){ // III четверь
		textX -= (stepX * kx) + (numX.GetLength() - 1) * 7;
		textY += 3;
	}
	else if (numX == L"0"){ // Если x == 0
		textX += 3;
		textY += 3;
	}
	else{	// I четверь
		textX += (stepX * kx) - 7;
		textY -= 15;
	}
	// отображаем значение шага
	dc->TextOutW(textX, textY, numX);
	// и по Y
	dc->SelectObject(&downFont);	// Переключаем шрифт на вертикальный
	numY.Format(L"%.3g", (maxVal < abs(minVal)) ? (-stepY) : (stepY));
	// Задаем координаты вывода
	textX = rect.left + dx;
	textY = rect.bottom - dy;
	if (numY[0] == '-'){ // IV четверь
		textX += 3;
		textY += stepY * ky + 7 * MAX2(numY.GetLength() - 2, 0);
	}
	else if (numY == L"0"){ // Если y == 0
		textX -= 3;
		textY += 3;
	}
	else{	// II четверь
		textX -= 15;
		textY -= stepY * ky - 7;
	}
	// отображаем значение шага
	dc->TextOutW(textX, textY, numY);

	// Возвращаем шрифт
	dc->SelectObject(*oldFont);
}

// Функция отрисовки графика (для простоты не парюсь с отрицательными значениями x > 0, значит мне нужны 1 и 4 четверти гафика, ось OX всегда слева)
void CPhysics2Dlg::writeGraphic2(CAnimateCtrl & graphCtrl, std::vector<std::pair<float, float>> & X, std::vector<std::pair<float, float>> & Y){
	// Первым делом избавляемся от аномалий в числах (аля 0 == 10^[-11])
	// В данном случае важны только значения координат(время лесом)
	for (std::vector<std::pair<float, float>>::iterator i = X.begin(); i < X.end(); i++){
		i->second = std::floor((i->second) * 1e+3 + .5) / 1e+3;
	}
	for (std::vector<std::pair<float, float>>::iterator i = Y.begin(); i < Y.end(); i++){
		i->second = std::floor((i->second) * 1e+3 + .5) / 1e+3;
	}

	CPaintDC * dc = (CPaintDC*)graphCtrl.GetDC();
	CFont * oldFont = dc->SelectObject(&acrossFont);		// Подключаем Горизонтальный шрифт
	
	// Область отрисовки графика
	RECT rect;
	graphCtrl.GetClientRect(&rect);

	// Очистка окна
	dc->FillRect(&rect, &CBrush(RGB(255, 255, 255)));

	// Если выражение пустое
	if (X.empty() || Y.empty()){
		return;
	}

	// Определяем границы по оси X
	float maxX = 0, minX = 0;
	size_t i;
	for (i = 0; i < X.size() && !isfinite(X[i].second); i++); // ищем первое конечное число
	if (i < X.size())					// если нашли
		maxX = minX = X[i].second;	// запоминаем его как максимум и минимум

	for (std::vector<std::pair<float, float>>::const_iterator ci = X.begin(); ci < X.end(); ci++){
		if (ci->second > maxX && isfinite(ci->second)) // Ищем максимум
			maxX = ci->second;
		else if (ci->second < minX && isfinite(ci->second)) // и мимнимум фукции
			minX = ci->second;
	}

	// Определяем границы по оси y
	float maxY = 0, minY = 0;
	for (i = 0; i < Y.size() && !isfinite(Y[i].second); i++); // ищем первое конечное число
	if (i < Y.size())					// если нашли
		maxX = minX = Y[i].second;	// запоминаем его как максимум и минимум

	for (std::vector<std::pair<float, float>>::const_iterator ci = Y.begin(); ci < Y.end(); ci++){
		if (ci->second > maxX && isfinite(ci->second)) // Ищем максимум
			maxX = ci->second;
		else if (ci->second < minX && isfinite(ci->second)) // и мимнимум фукции
			minX = ci->second;
	}

	// Считаем диапазон значений y и X
	float xDiap = MAX3(maxX - minX, maxX, abs(minX));
	float yDiap = MAX3(maxY - minY, maxY, abs(minY));

	// Считаем размер шага штрихов по оси Y
	float stepX = xDiap / 10;
	float stepY = yDiap / 10;

	// Коэффициенты для вывода графика в окно
	float kx = (rect.right - (rect.left + (minX >= 0 || maxX <= 0) ? 14 : 0)) / (xDiap * 1.2/*Запас, чтобы красиво было*/);
	float ky = (rect.bottom - (rect.top + (minY >= 0 || maxY <= 0) ? 14 : 0)) / (yDiap * 1.2/*Запас, чтобы красиво было*/);
	

	// Сдвиг графика относительно окна (15 - отступ под вывод размера шага)
	int dx = (minX >= 0) ? ((maxX == 0) ? (rect.right / 2) : stepX*kx) : ((maxX <= 0) ? (rect.right - stepX*kx) : abs(minX * kx - stepX*kx));
	int dy = (minY >= 0) ? ((maxY == 0) ? (rect.bottom / 2) : stepY*ky) : ((maxY <= 0) ? (rect.bottom - stepY*ky) : abs(minY * ky - stepY*ky));

	CPen drowBrush;
	drowBrush.CreatePen(PS_DOT, 1, RGB(0, 0, 0));	// Создаем новое перо
	dc->SelectObject(drowBrush);					// и подключаем его

	// Сетка значений на осях
	// X
	if (abs(stepX) > 0.001){ // если шаг не равен 0
		for (float x = rect.left + dx; x > rect.left; x -= (stepX * kx)){
			dc->MoveTo({ (int)x, rect.top });
			dc->LineTo({ (int)x, rect.bottom });
		}
		for (float x = rect.left + dx; x < rect.right; x += (stepX * kx)){
			dc->MoveTo({ (int)x, rect.top });
			dc->LineTo({ (int)x, rect.bottom });
		}
	}
	// Y
	if (abs(stepY) > 0.001){ // если шаг не равен 0
		for (float y = rect.bottom - dy; y > rect.top; y -= (stepY * ky)){
			dc->MoveTo({ rect.left, (int)y });
			dc->LineTo({ rect.right, (int)y });
		}
		for (float y = rect.bottom - dy; y < rect.bottom; y += (stepY * ky)){
			dc->MoveTo({ rect.left, (int)y });
			dc->LineTo({ rect.right, (int)y });
		}
	}

	// Отрисовка осей графика
	dc->SelectObject(lineBrush); // Переключаемся на кисть для рисования осей
	// OX
	dc->MoveTo({ rect.left, rect.bottom - dy });
	dc->LineTo({ rect.right, rect.bottom - dy });
	// OY
	dc->MoveTo({ rect.left + dx, rect.bottom });
	dc->LineTo({ rect.left + dx, rect.top });

	// Начинаем отрисовку непосредсвтенно графика
	drowBrush.DeleteObject();
	drowBrush.CreatePen(PS_SOLID, 4, RGB(0, 250, 0));	// Создаем новое перо
	dc->SelectObject(drowBrush);						// и подключаем его


	// Пропускаем все нечисла
	for (i = 0; i < X.size() && isnan(X[i].second) && isnan(Y[i].second); i++);
	// Устанавливаем кисть на стартовую позицию
	dc->MoveTo({ rect.left + dx + (int)(X[i].first * kx), rect.bottom - dy - (int)(Y[i].second * ky) });

	bool moveLine = false;
	for (size_t i = 0; i < X.size(); i++){
		if (isnan(X[i].second) || isnan(Y[i].second)){ // Если не число
			moveLine = true;
			continue;
		}
		if (i && ((isinf(X[i].second) && isinf(X[i - 1].second)) || (isinf(Y[i].second) && isinf(Y[i - 1].second))))	// или две подряд идущие бесконечности
			moveLine = true; // то делаем разрыв

		// Рассчитваем координаты точки в окне
		POINT p;
		p.x = rect.left + dx + (int)(X[i].second * kx);
		p.y = rect.bottom - dy - (int)(Y[i].second * ky);

		// Делаем разрыв или рисуем линию
		if (moveLine){
			dc->MoveTo(p);
			moveLine = false;
		}
		else
			dc->LineTo(p);
	}

	// Отображаем значения шагов
	CString numX, numY;
	int textX, textY;
	numX.Format(L"%.3g", (maxX < abs(minX)) ? (-stepX) : (stepX));
	// Задаем координаты вывода
	textX = rect.left + dx;
	textY = rect.bottom - dy;
	if (numX[0] == '-'){ // III четверь
		textX -= (stepX * kx) + (numX.GetLength() - 1) * 7;
		textY += 3;
	}
	else if (numX == L"0"){ // Если x == 0
		textX += 3;
		textY += 3;
	} else{	// I четверь
		textX += (stepX * kx) - 7;
		textY -= 15;
	}
	// отображаем значение шага
	dc->TextOutW(textX, textY, numX); 
	// и по Y
	dc->SelectObject(&downFont);	// Переключаем шрифт на вертикальный
	numY.Format(L"%.3g", (maxY < abs(minY)) ? (-stepY) : (stepY));
	// Задаем координаты вывода
	textX = rect.left + dx;
	textY = rect.bottom - dy;
	if (numY[0] == '-'){ // IV четверь
		textX += 3;
		textY += stepY * ky + 7 * MAX2(numY.GetLength() - 2, 0);
	}
	else if (numY == L"0"){ // Если y == 0
		textX -= 3;
		textY += 3;
	}
	else{	// II четверь
		textX -= 15;
		textY -= stepY * ky - 7;
	}
	// отображаем значение шага
	dc->TextOutW(textX, textY, numY); 

	// Возвращаем шрифт
	dc->SelectObject(*oldFont);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPhysics2Dlg::OnBnClickedCalc()
{
	CString exprX, exprY, exprF;
	std::vector<Token> tokensX, tokensY, tokensF;
	std::vector<Token> postfixX, postfixY, postfixF;
	std::vector<std::pair<float, float>> vals, valsX, valsY;

	CString tmp;

	tStValCtrl.GetWindowTextW(tmp);
	float stVal = _wtof(tmp);
	tEdValCtrl.GetWindowTextW(tmp);
	float edVal = _wtof(tmp);
	tStepValCtrl.GetWindowTextW(tmp);
	float stepVal = _wtof(tmp);
	
	switch (radio){
	case 0: // Выбрана двумерная система координат
		switch (listOfGraphsCtrl.GetCurSel()){
		case LG_2D_XT:	// График x(t)
			xVarEditCtrl.GetWindowTextW(exprX);
			if (createTokensFromExpression(exprX, tokensX))
				if (createPostfixFromTokens(postfixX, tokensX)){
					valsX = getVals(calculate, postfixX, stVal, edVal, stepVal);	// Рассчитываем значения x(t),
					writeGraphic(graphicCtrl, valsX, stVal, edVal, stepVal);		// выводим их на график
					graphicCtrl.UpdateWindow();				// и перерисовываем окно
					
					SetSymb(0, LG_2D_XT); // Выводим необходимые символы
				}
			break;
		case LG_2D_YT:	// График y(t)
			yVarEditCtrl.GetWindowTextW(exprY);
			if (createTokensFromExpression(exprY, tokensY))
				if (createPostfixFromTokens(postfixY, tokensY)){
					valsY = getVals(calculate, postfixY, stVal, edVal, stepVal);	// Рассчитываем значения y(t),
					writeGraphic(graphicCtrl, valsY, stVal, edVal, stepVal);		// выводим их на график
					graphicCtrl.UpdateWindow();				// и перерисовываем окно

					SetSymb(0, LG_2D_YT); // Выводим необходимые символы
				}
			break;
		case LG_2D_VT:	// График V(t)
			yVarEditCtrl.GetWindowTextW(exprX);
			xVarEditCtrl.GetWindowTextW(exprY);
			if (createTokensFromExpression(exprX, tokensX) && createTokensFromExpression(exprY, tokensY)) // Получаем токены из строк
				if (createPostfixFromTokens(postfixX, tokensX) && createPostfixFromTokens(postfixY, tokensY)){	// Получаем выражения из токенов
					vals = getVals(speed, postfixX, postfixY, stVal, edVal, stepVal);	// Рассчитываем значения скорости,
					writeGraphic(graphicCtrl, vals, stVal, edVal, stepVal);
					graphicCtrl.UpdateWindow();					// и перерисовываем окно

					SetSymb(0, LG_2D_VT); // Выводим необходимые символы
				}
			break;
		case LG_2D_ATT:	// График aт(t)
			yVarEditCtrl.GetWindowTextW(exprX);
			xVarEditCtrl.GetWindowTextW(exprY);
			if (createTokensFromExpression(exprX, tokensX) && createTokensFromExpression(exprY, tokensY)) // Получаем токены из строк
				if (createPostfixFromTokens(postfixX, tokensX) && createPostfixFromTokens(postfixY, tokensY)){	// Получаем выражения из токенов
					vals = getVals(acceleration, postfixX, postfixY, stVal, edVal, stepVal);	// Рассчитываем значения ускорения,
					writeGraphic(graphicCtrl, vals, stVal, edVal, stepVal);					// выводим их на график
					graphicCtrl.UpdateWindow();							// и перерисовываем окно

					SetSymb(0, LG_2D_AT); // Выводим необходимые символы
				}
			break;
		case LG_2D_YX:	// График пути
			yVarEditCtrl.GetWindowTextW(exprX);
			xVarEditCtrl.GetWindowTextW(exprY);
			if (createTokensFromExpression(exprX, tokensX) && createTokensFromExpression(exprY, tokensY)) // Получаем токены из строк
				if (createPostfixFromTokens(postfixX, tokensX) && createPostfixFromTokens(postfixY, tokensY)){	// Получаем выражения из токенов
					valsX = getVals(calculate, postfixX, stVal, edVal, stepVal);
					valsY = getVals(calculate, postfixY, stVal, edVal, stepVal);
					writeGraphic2(graphicCtrl, valsX, valsY);	// Выводим график пути
					graphicCtrl.UpdateWindow();					// и перерисовываем окно

					SetSymb(0, LG_2D_YX); // Выводим необходимые символы
				}
			break;
		}
		break;
	case 1: // Выбрана окружность

		break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CPhysics2Dlg::OnBnClickedRadio2d(){
	listOfGraphsCtrl.ResetContent();

	listOfGraphsCtrl.InsertString(LG_2D_XT,  L"x(t)");
	listOfGraphsCtrl.InsertString(LG_2D_YT,  L"y(t)");
	listOfGraphsCtrl.InsertString(LG_2D_VT,	 L"V(t)");
	listOfGraphsCtrl.InsertString(LG_2D_AT,	 L"a(t)");
	listOfGraphsCtrl.InsertString(LG_2D_ANT, L"an(t)");
	listOfGraphsCtrl.InsertString(LG_2D_ATT, L"aт(t)");
	listOfGraphsCtrl.InsertString(LG_2D_YX,  L"y(x)");

	listOfGraphsCtrl.SetCurSel(0);

	// Устанавливаем нужные символы
	SetSymb(0, 0);

	//// Поля ввода переменных
	xVarEditCtrl.EnableWindow(TRUE);
	yVarEditCtrl.EnableWindow(TRUE);
	fVarEditCtrl.EnableWindow(FALSE);
	//
	xVarEditCtrl.UpdateWindow();
	yVarEditCtrl.UpdateWindow();
	fVarEditCtrl.UpdateWindow();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void CPhysics2Dlg::OnBnClickedRadioRnd()
{
	listOfGraphsCtrl.ResetContent();
	
	listOfGraphsCtrl.InsertString(LG_RND_FT,  L"φ(t)");
	listOfGraphsCtrl.InsertString(LG_RND_VT,  L"ω(t)");
	listOfGraphsCtrl.InsertString(LG_RND_AT,  L"a(t)");
	listOfGraphsCtrl.InsertString(LG_RND_ANT, L"an(t)");
	listOfGraphsCtrl.InsertString(LG_RND_ATT, L"aт(t)");

	listOfGraphsCtrl.SetCurSel(0);

	// Устанавливаем нужные символы
	SetSymb(1, 0);

	//// Поля ввода переменных
	xVarEditCtrl.EnableWindow(FALSE);
	yVarEditCtrl.EnableWindow(FALSE);
	fVarEditCtrl.EnableWindow(TRUE);
	//
	xVarEditCtrl.UpdateWindow();
	yVarEditCtrl.UpdateWindow();
	fVarEditCtrl.UpdateWindow();
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void CPhysics2Dlg::SetSymb(int familyID, int symbID){
	// Скрываем все ненужые символы
	Symb2D_XCtrl.ShowWindow(SW_HIDE);
	Symb2D_YCtrl.ShowWindow(SW_HIDE);
	SymbRND_FCtrl.ShowWindow(SW_HIDE);
	Symb2D_VCtrl.ShowWindow(SW_HIDE);
	SymbRND_VCtrl.ShowWindow(SW_HIDE);
	Symb_AtCtrl.ShowWindow(SW_HIDE);
	Symb_AnCtrl.ShowWindow(SW_HIDE);
	Symb_ACtrl.ShowWindow(SW_HIDE);
	//
	Symb_H_XCtrl.ShowWindow(SW_HIDE);
	Symb_H_TCtrl.ShowWindow(SW_HIDE);
	//
	switch (familyID){
	case 0:
		switch (symbID){
		case LG_2D_XT:
			Symb2D_XCtrl.ShowWindow(SW_SHOW);
			Symb_H_TCtrl.ShowWindow(SW_SHOW);
			break;
		case LG_2D_YT:
			Symb2D_YCtrl.ShowWindow(SW_SHOW);
			Symb_H_TCtrl.ShowWindow(SW_SHOW);
			break;
		case LG_2D_VT:
			Symb2D_VCtrl.ShowWindow(SW_SHOW);
			Symb_H_TCtrl.ShowWindow(SW_SHOW);
			break;
		case LG_2D_AT:
			Symb_ACtrl.ShowWindow(SW_SHOW);
			Symb_H_TCtrl.ShowWindow(SW_SHOW);
			break;
		case LG_2D_ANT:
			Symb_AnCtrl.ShowWindow(SW_SHOW);
			Symb_H_TCtrl.ShowWindow(SW_SHOW);
			break;
		case LG_2D_ATT:
			Symb_AtCtrl.ShowWindow(SW_SHOW);
			Symb_H_TCtrl.ShowWindow(SW_SHOW);
			break;
		case LG_2D_YX:
			Symb2D_YCtrl.ShowWindow(SW_SHOW);
			Symb_H_XCtrl.ShowWindow(SW_SHOW);
			break;
		}
		break;
	case 1:
		switch (symbID){
		case LG_RND_FT:
			SymbRND_FCtrl.ShowWindow(SW_SHOW);
			Symb_H_TCtrl.ShowWindow(SW_SHOW);
			break;
		case LG_RND_VT:
			SymbRND_VCtrl.ShowWindow(SW_SHOW);
			Symb_H_TCtrl.ShowWindow(SW_SHOW);
			break;
		case LG_RND_AT:
			Symb_ACtrl.ShowWindow(SW_SHOW);
			Symb_H_TCtrl.ShowWindow(SW_SHOW);
			break;
		case LG_RND_ANT:
			Symb_AnCtrl.ShowWindow(SW_SHOW);
			Symb_H_TCtrl.ShowWindow(SW_SHOW);
			break;
		case LG_RND_ATT:
			Symb_AtCtrl.ShowWindow(SW_SHOW);
			Symb_H_TCtrl.ShowWindow(SW_SHOW);
			break;
		}
		break;
	}
	//
	Symb2D_XCtrl.UpdateWindow();
	Symb2D_YCtrl.UpdateWindow();
	SymbRND_FCtrl.UpdateWindow();
	Symb2D_VCtrl.UpdateWindow();
	SymbRND_VCtrl.UpdateWindow();
	Symb_AtCtrl.UpdateWindow();
	Symb_AnCtrl.UpdateWindow();
	Symb_ACtrl.UpdateWindow();
	//
	Symb_H_XCtrl.UpdateWindow();
	Symb_H_TCtrl.UpdateWindow();
}