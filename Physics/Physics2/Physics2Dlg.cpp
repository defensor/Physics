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
void CPhysics2Dlg::writeGraphic(CAnimateCtrl & graphCtrl, std::vector<std::pair<float, float>> & XY){
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

	// Первым делом избавляемся от аномалий в числах (аля 0 == 10^[-11])
	for (std::vector<std::pair<float, float>>::iterator i = XY.begin(); i < XY.end(); i++){
		i->first = std::floor((i->first) * ACCURACYN + .5) / ACCURACYN;
		i->second = std::floor((i->second) * ACCURACYN + .5) / ACCURACYN;
	}

	// Определяем границы по оси X
	float maxX = 0, minX = 0;
	size_t i;
	for (i = 0; i < XY.size() && !isfinite(XY[i].first); i++); // ищем первое конечное число
	if (i < XY.size())					// если нашли
		maxX = minX = XY[i].first;	// запоминаем его как максимум и минимум

	for (std::vector<std::pair<float, float>>::const_iterator ci = XY.begin() + i; ci < XY.end(); ci++){
		if (ci->first > maxX && isfinite(ci->first)) // Ищем максимум
			maxX = ci->first;
		else if (ci->first < minX && isfinite(ci->first)) // и мимнимум фукции
			minX = ci->first;
	}

	// Определяем границы по оси y
	float maxY = 0, minY = 0;
	for (i = 0; i < XY.size() && !isfinite(XY[i].second); i++); // ищем первое конечное число
	if (i < XY.size())					// если нашли
		maxY = minY = XY[i].second;	// запоминаем его как максимум и минимум

	for (std::vector<std::pair<float, float>>::const_iterator ci = XY.begin() + i; ci < XY.end(); ci++){
		if (ci->second > maxY && isfinite(ci->second)) // Ищем максимум
			maxY = ci->second;
		else if (ci->second < minY && isfinite(ci->second)) // и мимнимум фукции
			minY = ci->second;
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


	// Сдвиг графика относительно окна
	int dx = (minX >= 0) ? ((maxX == 0) ? (rect.right / 2) : stepX*kx) : ((maxX <= 0) ? (rect.right - stepX*kx) : abs(minX * kx - stepX*kx));
	int dy = (minY >= 0) ? ((maxY == 0) ? (rect.bottom / 2) : stepY*ky) : ((maxY <= 0) ? (rect.bottom - stepY*ky) : abs(minY * ky - stepY*ky));

	dc->SelectObject(netBrush);	// Подключаем перо для рисования графиков

	// Сетка значений на осях
	// X
	if (abs(stepX) > ACCURACY){ // если шаг не равен 0
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
		if (i && ((isinf(XY[i].first) && isinf(XY[i - 1].first)) || (isinf(XY[i].second) && isinf(XY[i - 1].second))))	// или две подряд идущие бесконечности
			moveLine = true; // то делаем разрыв

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
			float sum = XY[i].first;
			if (i && isfinite(XY[i - 1].first) && isfinite(XY[i - 1].second)){ // Если есть предыдущая точка
				sum += 10 * XY[i - 1].first;	 // то сдвигаем точку в её сторону
				k += 10;
			}
			pI.x = rect.left + dx + sum / k * kx;

			// сдвигаем по x в сторону следующей точки
			k = 1;
			sum = XY[i].first;
			if (i < XY.size() - 1 && isfinite(XY[i + 1].first) && isfinite(XY[i + 1].second)){ // Если есть следующая точка
				sum += 10 * XY[i + 1].first;					// то сдвигаем точку в её сторону
				k += 10;
			}
			pO.x = rect.left + dx + sum / k * kx;
		}
		else if (isinf(XY[i].first)){ // Если график по x уходит в бесконечность
			// сначала урезаем его до границы окна
			if (XY[i].first < 0)
				pI.x = pO.x = rect.left;
			else
				pI.x = pO.x = rect.right;

			// сдвигаем по y в сторону предыдущей точки
			int k = 1;
			float sum = XY[i].second;
			if (i && isfinite(XY[i - 1].first) && isfinite(XY[i - 1].second)){ // Если есть предыдущая точка
				sum += 10 * XY[i - 1].second;	 // то сдвигаем точку в её сторону
				k += 10;
			}
			pI.y = rect.bottom - dy - sum / k * ky;

			// сдвигаем по y в сторону следующей точки
			k = 1;
			sum = XY[i].second;
			if (i < XY.size() && isfinite(XY[i + 1].first) && isfinite(XY[i + 1].second)){ // Если есть предыдущая точка
				sum += 10 * XY[i + 1].second;	 // то сдвигаем точку в её сторону
				k += 10;
			}
			pO.y = rect.bottom - dy - sum / k * ky;
		}
		else{ // Если это обычная точка графика, то линии входащая и исходящая пересекаются в этой точке
			pI.x = pO.x = rect.left + dx + XY[i].first * kx;
			pI.y = pO.y = rect.bottom - dy - XY[i].second * ky;
		}


		// Делаем разрыв или рисуем линию
		if (moveLine)
			moveLine = false;
		else
			dc->LineTo(pI);

		dc->MoveTo(pO);
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
	}
	else{	// I четверь
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
	std::vector<std::pair<float, float>> vals, valsX, valsY, valsXY;

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
					writeGraphic(graphicCtrl, valsX);		// выводим их на график
					graphicCtrl.UpdateWindow();				// и перерисовываем окно
					
					SetSymb(0, LG_2D_XT); // Выводим необходимые символы
				}
			break;
		case LG_2D_YT:	// График y(t)
			yVarEditCtrl.GetWindowTextW(exprY);
			if (createTokensFromExpression(exprY, tokensY))
				if (createPostfixFromTokens(postfixY, tokensY)){
					valsY = getVals(calculate, postfixY, stVal, edVal, stepVal);	// Рассчитываем значения y(t),
					writeGraphic(graphicCtrl, valsY);		// выводим их на график
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
					writeGraphic(graphicCtrl, vals);
					graphicCtrl.UpdateWindow();					// и перерисовываем окно

					SetSymb(0, LG_2D_VT); // Выводим необходимые символы
				}
			break;
		case LG_2D_AT:	// График a(t)
			yVarEditCtrl.GetWindowTextW(exprX);
			xVarEditCtrl.GetWindowTextW(exprY);
			if (createTokensFromExpression(exprX, tokensX) && createTokensFromExpression(exprY, tokensY)) // Получаем токены из строк
				if (createPostfixFromTokens(postfixX, tokensX) && createPostfixFromTokens(postfixY, tokensY)){	// Получаем выражения из токенов
					vals = getVals(acceleration, postfixX, postfixY, stVal, edVal, stepVal);	// Рассчитываем значения ускорения,
					writeGraphic(graphicCtrl, vals);					// выводим их на график
					graphicCtrl.UpdateWindow();							// и перерисовываем окно

					SetSymb(0, LG_2D_AT); // Выводим необходимые символы
				}
			break;
		case LG_2D_YX:	// График пути
			xVarEditCtrl.GetWindowTextW(exprX);
			yVarEditCtrl.GetWindowTextW(exprY);
			if (createTokensFromExpression(exprX, tokensX) && createTokensFromExpression(exprY, tokensY)) // Получаем токены из строк
				if (createPostfixFromTokens(postfixX, tokensX) && createPostfixFromTokens(postfixY, tokensY)){	// Получаем выражения из токенов
					valsX = getVals(calculate, postfixX, stVal, edVal, stepVal);
					valsY = getVals(calculate, postfixY, stVal, edVal, stepVal);

					// Собираем x(t) и y(t) в y(x)
					for (int i = 0, j = 0; i < valsX.size() && j < valsY.size(); i++, j++){
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

					writeGraphic(graphicCtrl, valsXY);	// Выводим график пути
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