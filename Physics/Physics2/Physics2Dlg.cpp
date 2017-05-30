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
	lineBrush.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));			// Линии
	netBrush.CreatePen(PS_DOT, 1, RGB(0, 0, 0));			// Сетка
	graphBrush.CreatePen(PS_SOLID, 4, RGB(0, 150, 100));	// График
}

void CPhysics2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_2D_X, xVarEditCtrl);
	DDX_Control(pDX, IDC_GRAPHIC, graphicCtrl);
	DDX_Control(pDX, IDC_EDIT_2D_Y, yVarEditCtrl);
	DDX_Control(pDX, IDC_2D_X, Symb2D_XCtrl);
	DDX_Control(pDX, IDC_2D_Y, Symb2D_YCtrl);
	DDX_Control(pDX, IDC_V_2D, Symb2D_VCtrl);
	DDX_Control(pDX, IDC_LIST_OF_GRAPHS, listOfGraphsCtrl);
	DDX_Control(pDX, IDC_AT, Symb_AtCtrl);
	DDX_Control(pDX, IDC_AN, Symb_AnCtrl);
	DDX_Control(pDX, IDC_A, Symb_ACtrl);
	DDX_Control(pDX, IDC_H_X, Symb_H_XCtrl);
	DDX_Control(pDX, IDC_H_T, Symb_H_TCtrl);
	DDX_Control(pDX, IDC_ABSC_STVAL_EDIT, abscStValCtrl);
	DDX_Control(pDX, IDC_ABSC_EDVAL_EDIT, abscEdValCtrl);
	DDX_Control(pDX, IDC_ORD_STVAL_EDIT, ordStValCtrl);
	DDX_Control(pDX, IDC_ORD_EDVAL_EDIT, ordEdValCtrl);
}

BEGIN_MESSAGE_MAP(CPhysics2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_CALC, &CPhysics2Dlg::OnBnClickedCalc)
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
	// Настраиваем список графиков
	listOfGraphsCtrl.ResetContent();
	listOfGraphsCtrl.InsertString(LG_2D_XT, L"x(t)");
	listOfGraphsCtrl.InsertString(LG_2D_YT, L"y(t)");
	listOfGraphsCtrl.InsertString(LG_2D_VT, L"V(t)");
	listOfGraphsCtrl.InsertString(LG_2D_AT, L"a(t)");
	listOfGraphsCtrl.InsertString(LG_2D_ANT, L"an(t)");
	listOfGraphsCtrl.InsertString(LG_2D_ATT, L"aт(t)");
	listOfGraphsCtrl.InsertString(LG_2D_YX, L"y(x)");
	listOfGraphsCtrl.SetCurSel(0);

	// Устанавливаем нужные символы
	SetSymb(0);

	ordStValCtrl.SetWindowTextW(L"0");
	abscStValCtrl.SetWindowTextW(L"0");

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
void CPhysics2Dlg::writeGraphic(CAnimateCtrl & graphCtrl, std::vector<std::pair<double, double>> & XY, FRECT valRect){
	CPaintDC * dc = (CPaintDC*)graphCtrl.GetDC();
	CFont * oldFont = dc->SelectObject(&acrossFont);		// Подключаем Горизонтальный шрифт

	// Область отрисовки графика
	RECT rect;
	graphCtrl.GetClientRect(&rect);

	// Очистка окна
	dc->FillRect(&rect, &CBrush(RGB(255, 255, 255)));

	// Если выражение пустое
	if (XY.empty()){
		return;
	}

	// Определяем границы по оси X
	double maxX = valRect.right,
		minX = valRect.left;
	size_t i;
	//for (i = 0; i < XY.size() && !isfinite(XY[i].first); i++); // ищем первое конечное число
	//if (i < XY.size())					// если нашли
	//	maxX = minX = XY[i].first;	// запоминаем его как максимум и минимум

	//for (std::vector<std::pair<double, double>>::const_iterator ci = XY.begin() + i; ci < XY.end(); ci++){
	//	if (ci->first > maxX && isfinite(ci->first)) // Ищем максимум
	//		maxX = ci->first;
	//	else if (ci->first < minX && isfinite(ci->first)) // и мимнимум фукции
	//		minX = ci->first;
	//}

	// Определяем границы по оси y
	double maxY = 0, minY = 0;

	for (i = 0; i < XY.size() && !isfinite(XY[i].second); i++); // ищем первое конечное число
	if (i < XY.size())					// если нашли
		maxY = minY = XY[i].second;	// запоминаем его как максимум и минимум

	if (isfinite(valRect.top))
		maxY = valRect.top;
	else
		for (std::vector<std::pair<double, double>>::const_iterator ci = XY.begin() + i; ci < XY.end(); ci++){
			if (ci->second > maxY && isfinite(ci->second)) // Ищем максимум
				maxY = ci->second;
		}

	if (isfinite(valRect.bottom))
		minY = valRect.bottom;
	else
		for (std::vector<std::pair<double, double>>::const_iterator ci = XY.begin() + i; ci < XY.end(); ci++){
			if (ci->second < minY && isfinite(ci->second)) // и мимнимум фукции
				minY = ci->second;
		}

	// Считаем диапазон значений y и X
	double xDiap = MAX3(maxX - minX, maxX, abs(minX));
	double yDiap = MAX3(maxY - minY, maxY, abs(minY));

	// Считаем размер шага штрихов по оси Y
	double stepX = xDiap / 10;
	double stepY = yDiap / 10;

	// Коэффициенты для вывода графика в окно
	double kx = (double)(rect.right - (rect.left + (minX >= 0 || maxX <= 0) ? 14 : 0)) / (xDiap * 1.2/*Запас, чтобы красиво было*/);
	double ky = (double)(rect.bottom - (rect.top + (minY >= 0 || maxY <= 0) ? 14 : 0)) / (yDiap * 1.2/*Запас, чтобы красиво было*/);


	// Сдвиг графика относительно окна
	int dx = (int)((minX >= 0) ? ((maxX == 0) ? (rect.right / 2) : stepX*kx) : ((maxX <= 0) ? (rect.right - stepX*kx) : abs(minX * kx - stepX*kx)));
	int dy = (int)((minY >= 0) ? ((maxY == 0) ? (rect.bottom / 2) : stepY*ky) : ((maxY <= 0) ? (rect.bottom - stepY*ky) : abs(minY * ky - stepY*ky)));

	dc->SelectObject(netBrush);	// Подключаем перо для рисования графиков

	// Сетка значений на осях
	// X
	if (abs(stepX) > ACCURACY){ // если шаг не равен 0
		for (int x = rect.left + dx; x > rect.left; x -= (int)(stepX * kx)){
			dc->MoveTo({ x, rect.top });
			dc->LineTo({ x, rect.bottom });
		}
		for (int x = rect.left + dx; x < rect.right; x += (int)(stepX * kx)){
			dc->MoveTo({ x, rect.top });
			dc->LineTo({ x, rect.bottom });
		}
	}
	// Y
	if (abs(stepY) > ACCURACY){ // если шаг не равен 0
		for (int y = rect.bottom - dy; y > rect.top; y -= (int)(stepY * ky)){
			dc->MoveTo({ rect.left, y });
			dc->LineTo({ rect.right, y });
		}
		for (int y = rect.bottom - dy; y < rect.bottom; y += (int)(stepY * ky)){
			dc->MoveTo({ rect.left, y });
			dc->LineTo({ rect.right, y });
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
	dc->SelectObject(graphBrush);


	// Пропускаем все нечисла
	for (i = 0; i < XY.size() && (isnan(XY[i].first) || isnan(XY[i].second)); i++);

	// Нужно ли сделать разрыв(то есть не нужно ли рисовать линию на этой итерации)
	bool moveLine = true;
	for (size_t i = 0; i < XY.size(); i++){
		if (isnan(XY[i].first) || isnan(XY[i].second)){ // Если не число
			moveLine = true;
			continue;
		}
		if (i && ((isinf(XY[i].first) && isinf(XY[i - 1].first)) || (isinf(XY[i].second) && isinf(XY[i - 1].second)))){	// или две подряд идущие бесконечности
			moveLine = true; // то делаем разрыв
			continue;
		}

		// Рассчитваем координаты точки в окне
		POINT pI, pO;

		if (isinf(XY[i].second)){ // Если график по y уходит в бесконечность
			// сначала урезаем его до границы окна
			if (XY[i].second < 0)
				pI.y = pO.y = rect.bottom;
			else
				pI.y = pO.y = rect.top;

			// сдвигаем по x в сторону предыдущей точки
			int k = 1;
			double sum = XY[i].first;
			if (i && isfinite(XY[i - 1].first) && isfinite(XY[i - 1].second)){ // Если есть предыдущая точка
				sum += 7 * XY[i - 1].first;	 // то сдвигаем точку в её сторону
				k += 7;
			}
			pI.x = (int)(rect.left + dx + sum / k * kx);

			// сдвигаем по x в сторону следующей точки
			k = 1;
			sum = XY[i].first;
			if (i < XY.size() - 1 && isfinite(XY[i + 1].first) && isfinite(XY[i + 1].second)){ // Если есть следующая точка
				sum += 7 * XY[i + 1].first;					// то сдвигаем точку в её сторону
				k += 7;
			}
			pO.x = (int)(rect.left + dx + sum / k * kx);
		}
		else if (isinf(XY[i].first)){ // Если график по x уходит в бесконечность
			// сначала урезаем его до границы окна
			if (XY[i].first < 0)
				pI.x = pO.x = rect.left;
			else
				pI.x = pO.x = rect.right;

			// сдвигаем по y в сторону предыдущей точки
			int k = 1;
			double sum = XY[i].second;
			if (i && isfinite(XY[i - 1].first) && isfinite(XY[i - 1].second)){ // Если есть предыдущая точка
				sum += 7 * XY[i - 1].second;	 // то сдвигаем точку в её сторону
				k += 7;
			}
			pI.y = (int)(rect.bottom - dy - sum / k * ky);

			// сдвигаем по y в сторону следующей точки
			k = 1;
			sum = XY[i].second;
			if (i < XY.size() && isfinite(XY[i + 1].first) && isfinite(XY[i + 1].second)){ // Если есть предыдущая точка
				sum += 7 * XY[i + 1].second;	 // то сдвигаем точку в её сторону
				k += 7;
			}
			pO.y = (int)(rect.bottom - dy - sum / k * ky);
		}
		else{ // Если это обычная точка графика, то линии входащая и исходящая пересекаются в этой точке
			pI.x = pO.x = (int)(rect.left + dx + XY[i].first * kx);
			pI.y = pO.y = (int)(rect.bottom - dy - XY[i].second * ky);
		}


		// Делаем разрыв или рисуем линию
		if (moveLine){
			moveLine = false;
			dc->MoveTo(pO);
		}
		else
			dc->LineTo(pI);
	}

	// Отображаем значения шагов
	CString numX, numY;
	int textX, textY;
	numX.Format(L"%.3g", (maxX < abs(minX)) ? (-stepX) : (stepX));
	// Задаем координаты вывода
	textX = rect.left + dx;
	textY = rect.bottom - dy;
	if (numX[0] == '-'){ // III четверь
		textX -= (int)((stepX * kx) + (numX.GetLength() - 1) * 7);
		textY += 3;
	}
	else if (numX == L"0"){ // Если x == 0
		textX += 3;
		textY += 3;
	}
	else{	// I четверь
		textX += (int)(stepX * kx) - 7;
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
		textY += (int)(stepY * ky + 7 * MAX2(numY.GetLength() - 2, 0));
	}
	else if (numY == L"0"){ // Если y == 0
		textX -= 3;
		textY += 3;
	}
	else{	// II четверь
		textX -= 15;
		textY -= (int)(stepY * ky) - 7;
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
	std::vector<std::pair<double, double>> vals, valsX, valsY, valsXY;

	CString tmp;

	// границы абсциссы
	abscStValCtrl.GetWindowTextW(tmp);
	double abscStVal = (double)_wtof(tmp);
	if (abscStVal > 0){
		abscStVal = 0;
		abscStValCtrl.SetWindowTextW(L"0");
	}
	abscEdValCtrl.GetWindowTextW(tmp);
	double abscEdVal = (double)_wtof(tmp);
	if (abscEdVal < 0){
		abscEdVal = 0;
		abscEdValCtrl.SetWindowTextW(L"0");
	}

	// ординаты
	ordStValCtrl.GetWindowTextW(tmp);
	double ordStVal = (tmp.IsEmpty()) ? NAN : (double)_wtof(tmp);
	if (isfinite(ordStVal) && ordStVal > 0){
		ordStVal = 0;
		ordStValCtrl.SetWindowTextW(L"0");
	}
	ordEdValCtrl.GetWindowTextW(tmp);
	double ordEdVal = (tmp.IsEmpty()) ? NAN : (double)_wtof(tmp);
	if (isfinite(ordEdVal) && ordEdVal < 0){
		ordEdVal = 0;
		ordEdValCtrl.SetWindowTextW(L"0");
	}

	if (abscStVal > abscEdVal){
		MessageBox(L"Ошибка, начальное значение абсциссы больше конечного!");
		return;
	}

	if (ordStVal > ordEdVal){
		MessageBox(L"Ошибка, начальное значение ординаты больше конечного!");
		return;
	}
	
	//fprintf(logFile, "stOrd: %f\t edOrd: %f\tstAbsc: %f\t edAbsc: %f\t\n", ordStVal, ordEdVal, abscStVal, abscEdVal);

	FRECT valRect = { abscStVal, ordEdVal, abscEdVal, ordStVal };
	
	switch (radio){
	case 0: // Выбрана двумерная система координат
		switch (listOfGraphsCtrl.GetCurSel()){
		case LG_2D_XT:	// График x(t)
			xVarEditCtrl.GetWindowTextW(exprX);
			if (createTokensFromExpression(exprX, tokensX))
				if (createPostfixFromTokens(postfixX, tokensX)){
					valsX = getVals(calculate, postfixX, abscStVal, abscEdVal);	// Рассчитываем значения x(t),
					writeGraphic(graphicCtrl, valsX, valRect);		// выводим их на график
					graphicCtrl.UpdateWindow();				// и перерисовываем окно
					
					SetSymb(LG_2D_XT); // Выводим необходимые символы
				}
			break;
		case LG_2D_YT:	// График y(t)
			yVarEditCtrl.GetWindowTextW(exprY);
			if (createTokensFromExpression(exprY, tokensY))
				if (createPostfixFromTokens(postfixY, tokensY)){
					valsY = getVals(calculate, postfixY, abscStVal, abscEdVal);	// Рассчитываем значения y(t),
					writeGraphic(graphicCtrl, valsY, valRect);		// выводим их на график
					graphicCtrl.UpdateWindow();				// и перерисовываем окно

					SetSymb(LG_2D_YT); // Выводим необходимые символы
				}
			break;
		case LG_2D_VT:	// График V(t)
			xVarEditCtrl.GetWindowTextW(exprX);
			yVarEditCtrl.GetWindowTextW(exprY);
			if (createTokensFromExpression(exprX, tokensX) && createTokensFromExpression(exprY, tokensY)) // Получаем токены из строк
				if (createPostfixFromTokens(postfixX, tokensX) && createPostfixFromTokens(postfixY, tokensY)){	// Получаем выражения из токенов
					vals = getVals(speed, postfixX, postfixY, abscStVal, abscEdVal);	// Рассчитываем значения скорости,
					writeGraphic(graphicCtrl, vals, valRect);
					graphicCtrl.UpdateWindow();					// и перерисовываем окно

					SetSymb(LG_2D_VT); // Выводим необходимые символы
				}
			break;
		case LG_2D_AT:	// График a(t)
			xVarEditCtrl.GetWindowTextW(exprX);
			yVarEditCtrl.GetWindowTextW(exprY);
			if (createTokensFromExpression(exprX, tokensX) && createTokensFromExpression(exprY, tokensY)) // Получаем токены из строк
				if (createPostfixFromTokens(postfixX, tokensX) && createPostfixFromTokens(postfixY, tokensY)){	// Получаем выражения из токенов
					vals = getVals(acceleration, postfixX, postfixY, abscStVal, abscEdVal);	// Рассчитываем значения ускорения,
					writeGraphic(graphicCtrl, vals, valRect);					// выводим их на график
					graphicCtrl.UpdateWindow();							// и перерисовываем окно

					SetSymb(LG_2D_AT); // Выводим необходимые символы
				}
			break;
		case LG_2D_ATT:	// График aт(t)
			xVarEditCtrl.GetWindowTextW(exprX);
			yVarEditCtrl.GetWindowTextW(exprY);
			if (createTokensFromExpression(exprX, tokensX) && createTokensFromExpression(exprY, tokensY)) // Получаем токены из строк
				if (createPostfixFromTokens(postfixX, tokensX) && createPostfixFromTokens(postfixY, tokensY)){	// Получаем выражения из токенов
					vals = getVals(accelerationT, postfixX, postfixY, abscStVal, abscEdVal);	// Рассчитываем значения ускорения,
					writeGraphic(graphicCtrl, vals, valRect);					// выводим их на график
					graphicCtrl.UpdateWindow();							// и перерисовываем окно

					SetSymb(LG_2D_ATT); // Выводим необходимые символы
				}
			break;
		case LG_2D_ANT:	// График an(t)
			xVarEditCtrl.GetWindowTextW(exprX);
			yVarEditCtrl.GetWindowTextW(exprY);
			if (createTokensFromExpression(exprX, tokensX) && createTokensFromExpression(exprY, tokensY)) // Получаем токены из строк
				if (createPostfixFromTokens(postfixX, tokensX) && createPostfixFromTokens(postfixY, tokensY)){	// Получаем выражения из токенов
					vals = getVals(accelerationN, postfixX, postfixY, abscStVal, abscEdVal);	// Рассчитываем значения ускорения,
					writeGraphic(graphicCtrl, vals, valRect);					// выводим их на график
					graphicCtrl.UpdateWindow();							// и перерисовываем окно

					SetSymb(LG_2D_ANT); // Выводим необходимые символы
				}
			break;
		case LG_2D_YX:	// График пути
			xVarEditCtrl.GetWindowTextW(exprX);
			yVarEditCtrl.GetWindowTextW(exprY);
			if (createTokensFromExpression(exprX, tokensX) && createTokensFromExpression(exprY, tokensY)) // Получаем токены из строк
				if (createPostfixFromTokens(postfixX, tokensX) && createPostfixFromTokens(postfixY, tokensY)){	// Получаем выражения из токенов
					valsX = getVals(calculate, postfixX, abscStVal, abscEdVal);
					valsY = getVals(calculate, postfixY, abscStVal, abscEdVal);

					// Собираем x(t) и y(t) в y(x)
					for (size_t i = 0, j = 0; i < valsX.size() && j < valsY.size(); i++, j++){
						valsXY.push_back({valsX[i].second, valsY[j].second});

						// Если одному значению t соответствуют два значения x
						if (i < valsX.size() - 1 && valsX[i].first == valsX[i + 1].first){
							valsXY.push_back({ valsX[++i].second, valsY[j].second });
						}

						// Если одному значению t соответствуют два значения y
						if (j < valsY.size() - 1 && valsY[j].first == valsY[j + 1].first){
							valsXY.push_back({ valsX[i].second, valsY[++j].second });
						}
					}

					
					writeGraphic(graphicCtrl, valsXY, valRect);	// Выводим график пути
					graphicCtrl.UpdateWindow();					// и перерисовываем окно

					SetSymb(LG_2D_YX); // Выводим необходимые символы
				}
			break;
		}
		break;
	case 1: // Выбрана окружность

		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void CPhysics2Dlg::SetSymb(int symbID){
	// Скрываем все ненужые символы
	Symb2D_XCtrl.ShowWindow(SW_HIDE);
	Symb2D_YCtrl.ShowWindow(SW_HIDE);
	Symb2D_VCtrl.ShowWindow(SW_HIDE);
	Symb_AtCtrl.ShowWindow(SW_HIDE);
	Symb_AnCtrl.ShowWindow(SW_HIDE);
	Symb_ACtrl.ShowWindow(SW_HIDE);
	//
	Symb_H_XCtrl.ShowWindow(SW_HIDE);
	Symb_H_TCtrl.ShowWindow(SW_HIDE);
	//
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
	//
	Symb2D_XCtrl.UpdateWindow();
	Symb2D_YCtrl.UpdateWindow();
	Symb2D_VCtrl.UpdateWindow();
	Symb_AtCtrl.UpdateWindow();
	Symb_AnCtrl.UpdateWindow();
	Symb_ACtrl.UpdateWindow();
	//
	Symb_H_XCtrl.UpdateWindow();
	Symb_H_TCtrl.UpdateWindow();
}