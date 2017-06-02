#pragma once

#include "stdafx.h"
#include "PhysicsF.h"

//#define MY_INFINITE_POS 1e+50
//#define MY_INFINITE_NEG -(1e+50)
#define DT 1e-3


FILE * logFile = fopen("file.log", "w");

// Знаю, что криво, но других идей как то нет
namespace my{
	double correct(double a){
		if (isnan(a))
			return a;
		if (a > 1e+13)
			return INFINITY;
		if (a < -1e+13)
			return -INFINITY;

		return a;
	}

	double floor(double a){
		if (!isfinite(a))
			return NAN;
		return (abs(a) > ACCURACY) ? (std::floor(a * ACCURACYN + .5) / ACCURACYN) : (0);
	}
}

// Множество разделителей
std::set<char> delimiters = { ' ', '+', '-', '*', '/', '^', '(', ')', '[', ']' };

// Проверки символа
bool isDelimiter(char c){
	return delimiters.count(c) > 0;
}

// Разбиваем выражение на токены
bool createTokensFromExpression(CString & expr, std::vector<Token> & tokens){

	CString name;

	if (expr.IsEmpty()){
		MessageBox(NULL, L"Ошибка: пустое поле уравнения\n", NULL, NULL);
		return false;
	}

	int i = 0;
	while (i < expr.GetLength()){
		name = "";
		if (isDelimiter(expr[i])){
			if (expr[i] == ' '){ // пробелы пропускаем
				i++;
				continue;
			}
			name = expr[i];
			i++;
		}
		else{
			while (i < expr.GetLength() && !isDelimiter(expr[i])){
				// Если это имя команды или переменная, то считываем полностью
				name += expr[i++];
			}
		}
		// Заносим получившийся токен в список токенов
		tokens.push_back({ name, VAR });
	}

	// Классифицируем токены
	for (i = 0; i < tokens.size(); i++){
		if (tokens[i].name[0] == '('){
			tokens[i].type = OP_BR;
			continue;
		}
		if (tokens[i].name[0] == ')'){
			tokens[i].type = CL_BR;
			continue;
		}
		if (tokens[i].name[0] == '['){
			tokens[i].type = OP_SBR;
			continue;
		}
		if (tokens[i].name[0] == ']'){
			tokens[i].type = CL_SBR;
			continue;
		}
		if (isdigit(tokens[i].name[0])){
			tokens[i].type = NUM;
			continue;
		}
		if (isalpha(tokens[i].name[0])){
			if (i < tokens.size() - 1 && tokens[i + 1].name[0] == '('){
				tokens[i].type = FUNC;
				continue;
			}
		}
		if (isDelimiter(tokens[i].name[0]))
			tokens[i].type = OP;
	}

	// Проверяем все токены
	for (i = 0; i < tokens.size(); i++){
		switch (tokens[i].type){
		case VAR:
			// У нас только 3 переменные 't', 'e', 'pi'
			if (tokens[i].name != "t" && tokens[i].name != "e" && tokens[i].name != "pi"){
				CString sErr;
				sErr.Format(L"Error: unknown var \"%s\"\n", tokens[i].name.GetBuffer());
				MessageBox(NULL, sErr, NULL, NULL);
				return false;
			}
			break;
		case NUM:
			// Если в число затесался посторонний символ
			for (int j = 0; j < tokens[i].name.GetLength(); j++)
				if (!isdigit(tokens[i].name[j]) && tokens[i].name[j] != '.'){
					CString sErr;
					sErr.Format(L"Error: unknown value \"%s\"\n", tokens[i].name.GetBuffer());
					MessageBox(NULL, sErr, NULL, NULL);
					return false;
				}
			break;
		case OP_BR:
		case CL_BR:
		case OP_SBR:
		case CL_SBR:
		case OP:
			// Для всех операторов проверяем длину
			if (tokens[i].name.GetLength() > 1){
				CString sErr;
				sErr.Format(L"Error: unknown operator \"%s\"\n", tokens[i].name.GetBuffer());
				MessageBox(NULL, sErr, NULL, NULL);
				return false;
			}

			// Дополнительно проверяем '-' на префиксность
			if (tokens[i].name == "-" && (i == 0 || tokens[i - 1].type == OP_BR || tokens[i - 1].type == OP_SBR))
				tokens[i].name = "opposite";
			break;
		case FUNC:
			// У нас всего 8 различных функций
			if (tokens[i].name != "sin" && tokens[i].name != "cos" && tokens[i].name != "asin" && tokens[i].name != "acos" && tokens[i].name != "tg" && tokens[i].name != "ctg" && tokens[i].name != "atg" && tokens[i].name != "actg"){
				CString sErr;
				sErr.Format(L"Error: unknown function \"%s\"\n", tokens[i].name.GetBuffer());
				MessageBox(NULL, sErr, NULL, NULL);
				return false;
			}
			break;
		}
	}
	return true;
}

std::map<CString, int> priority = {
	{ L"+", 10 },
	{ L"-", 10 },
	{ L"*", 20 },
	{ L"/", 20 },
	{ L"^", 30 },
	{ L"opposite", 10 },
	{ L"!", 30 }
};

// Расставляем токены в порядке постфиксной записи
bool createPostfixFromTokens(std::vector<Token> & postfix, std::vector<Token> & tokens){
	// Но для начала проверяем выражение на вменяемость по 5 правилам
	// 1) нельзя чтобы рядом стояли представители одного типа токенов(кроме скобок)
	for (int i = 0; i < tokens.size() - 1; i++)
		if (tokens[i].type == tokens[i + 1].type && tokens[i].type != OP_BR && tokens[i].type != CL_BR && tokens[i].type != OP_SBR && tokens[i].type != CL_SBR){
			CString sErr;
			sErr.Format(L"Ошибка: Выражение \"%s %s\" не имеет смысла\n", tokens[i].name.GetBuffer(), tokens[i + 1].name.GetBuffer());
			MessageBox(NULL, sErr, NULL, NULL);
			return false;
		}
	// 1.1) переменные и числа, тоже не очень сочетаются
	for (int i = 0; i < tokens.size() - 1; i++)
		if ((tokens[i].type == VAR && tokens[i + 1].type == NUM) || (tokens[i].type == NUM && tokens[i + 1].type == VAR)){
			CString sErr;
			sErr.Format(L"Ошибка: Выражение \"%s %s\" не имеет смысла\n", tokens[i].name.GetBuffer(), tokens[i + 1].name.GetBuffer());
			MessageBox(NULL, sErr, NULL, NULL);
			return false;
		}
	// 2) выражение не может начинаться и заканчиваться оператором(кроме opposite)
	if ((tokens[0].type == OP && tokens[0].name != "opposite") || tokens[tokens.size() - 1].type == OP){
		CString sErr;
		sErr.Format(L"Ошибка: выражение не может начинаться или заканчиваться оператором\n");
		MessageBox(NULL, sErr, NULL, NULL);
		return false;
	}
	// 3) выражение должно быть сбалансировано по скобкам
	int balanceCntr = 0;
	for (int i = 0; i < tokens.size() && balanceCntr >= 0; i++){
		if (tokens[i].type == OP_BR)
			balanceCntr++;
		else if (tokens[i].type == CL_BR)
			balanceCntr--;
	}
	if (balanceCntr != 0){
		CString sErr;
		sErr.Format(L"Ошибка: выражение не сбалансировано по скобкам\n");
		MessageBox(NULL, sErr, NULL, NULL);
		return false;
	}

	// 4) у фунций должен быть задан параметр
	for (int i = 0; i < tokens.size(); i++){
		if (tokens[i].type != FUNC){ // нас интересуют только функции
			continue;
		}

		// Ищем какое либо значение внутри скобок
		bool isVal = false;
		for (int j = i + 1; tokens[j].type != CL_BR && !isVal; j++){
			if (tokens[j].type == VAR || tokens[j].type == NUM)
				isVal = true;
		}

		if (!isVal){
			CString sErr;
			sErr.Format(L"Ошибка: функция %s должна принимать аргумент\n", tokens[i].name.GetBuffer());
			MessageBox(NULL, sErr, NULL, NULL);
			return false;
		}
	}
	// 5) оператор не должен 'заворачиваться' в скобку (по сути правило 2 применимое к скобкам)
	for (int i = 0; i < tokens.size() - 1; i++){
		if ((tokens[i].type == OP_BR && tokens[i + 1].type == OP  && tokens[i + 1].name != "opposite") || (tokens[i].type == OP && tokens[i + 1].type == CL_BR)){
			CString sErr;
			sErr.Format(L"Ошибка: выражение не может начинаться или заканчиваться оператором\n");
			MessageBox(NULL, sErr, NULL, NULL);
			return false;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Приступаем непосредственно к преобразованию выражения к постфиксной нотации
	std::stack<Token> TStack;

	for (int i = 0; i < tokens.size(); i++){
		switch (tokens[i].type){
		case VAR:
		case NUM:
			postfix.push_back(tokens[i]);
			break;
		case OP_BR:
			TStack.push(tokens[i]);
			break;
		case CL_BR:
			while (TStack.top().type != OP_BR){
				postfix.push_back(TStack.top());
				TStack.pop();
			}
			TStack.pop();
			break;
		case OP:
			while (!TStack.empty() && ((TStack.top().type == OP && priority[TStack.top().name] >= priority[tokens[i].name]) || TStack.top().type == FUNC)){
				postfix.push_back(TStack.top());
				TStack.pop();
			}
			TStack.push(tokens[i]);
			break;
		case FUNC:
			while (!TStack.empty() && TStack.top().type == FUNC){
				postfix.push_back(TStack.top());
				TStack.pop();
			}
			TStack.push(tokens[i]);
			break;
		}
	}

	// Выгребаем стек
	while (!TStack.empty()){
		postfix.push_back(TStack.top());
		TStack.pop();
	}


	return true;
}


// Подсчет постфиксного выражения записанного токенами
double calculate(std::vector<Token> & postfix, double t){
	double pi = 3.14159265;
	double e = 2.71828182;

	std::stack<double> fStack;

	double a, b;

	for (int i = 0; i < postfix.size(); i++){
		switch (postfix[i].type){
		case VAR:
			switch (postfix[i].name[0]){
			case 'e':
				fStack.push(e);
				break;
			case 't':
				fStack.push(t);
				break;
			case 'p':
				fStack.push(pi);
				break;
			}
			break;
		case NUM:
			fStack.push(_wtof(postfix[i].name));
			break;
		case OP:
			switch (postfix[i].name[0]){
			case '+':
				a = fStack.top();
				fStack.pop();
				b = fStack.top();
				fStack.pop();
				a += b;
				fStack.push(a);
				break;
			case '-':
				a = fStack.top();
				fStack.pop();
				b = fStack.top();
				fStack.pop();
				b -= a;
				fStack.push(b);
				break;
			case '*':
				a = fStack.top();
				fStack.pop();
				b = fStack.top();
				fStack.pop();
				a *= b;
				fStack.push(a);
				break;
			case '/':
				a = fStack.top();
				fStack.pop();
				b = fStack.top();
				fStack.pop();
				b /= a;
				fStack.push(b);
				break;
			case '^': // pow
				a = fStack.top();
				fStack.pop();
				b = fStack.top();
				fStack.pop();
				a = pow(b, a);
				fStack.push(a);
				break;
			case 'o': // opposite
				a = fStack.top();
				fStack.pop();
				a *= -1;
				fStack.push(a);
				break;
			}
			break;
		case FUNC:
			a = fStack.top();
			fStack.pop();
			if (postfix[i].name == "sin"){
				fStack.push(sin(a));
			}
			else if (postfix[i].name == "cos"){
				fStack.push(cos(a));
			}
			else if (postfix[i].name == "asin"){
				fStack.push(asin(a));
			}
			else if (postfix[i].name == "acos"){
				fStack.push(acos(a));
			}
			else if (postfix[i].name == "tg"){
				fStack.push(tan(a));
			}
			else if (postfix[i].name == "ctg"){
				fStack.push(1 / tan(a));
			}
			else if (postfix[i].name == "atg"){
				fStack.push(atan(a));
			}
			else if (postfix[i].name == "actg"){
				fStack.push(pi / 2 - atan(a));
			}
			break;
		}
	}

	double result = fStack.top();
	fStack.pop();

	return my::correct(result);
}


double derivative(double(*pFunc)(std::vector<Token>&, double), std::vector<Token> & postfix, double t){		
	double a = pFunc(postfix, t + DT);
	double b = pFunc(postfix, t);

	if (!(isfinite(a) && isfinite(b))){
		if (isnan(a) || isnan(b))
			return NAN;

		if (isinf(a) && isinf(b) && FSIGN_C(a) == FSIGN_C(b)) // Если значения функций это бесконечности одного знака
			return NAN; // то результат неопределенность

		if (isinf(a))
			return a;
		
		if (isinf(b))
			return NAN;
	}
	a = my::correct(a);
	b = my::correct(b);

	double d = (a - b) / DT;

	return my::correct(d);
}

double derivative(double(*pFunc)(std::vector<Token>&, std::vector<Token>&, double), std::vector<Token> & postfixX, std::vector<Token> & postfixY, double t){
	double n = pFunc(postfixX, postfixY, t);

	if (!isfinite(n)){
		return NAN;
	}

	double a = pFunc(postfixX, postfixY, t + DT);
	double b = pFunc(postfixX, postfixY, t);

	if (!(isfinite(a) && isfinite(b))){
		if (isnan(a) || isnan(b))
			return NAN;

		if (isinf(a) && isinf(b) && FSIGN_C(a) == FSIGN_C(b)) // Если значения функций это бесконечности одного знака
			return NAN; // то результат неопределенность

		if (isinf(a))
			return a;

		if (isinf(b))
			return NAN;
	}
	a = my::correct(a);
	b = my::correct(b);

	double d = my::correct(a - b) / DT;

	return my::correct(d);
}


double derivative2(std::vector<Token> & postfix, double t){
	double a = my::correct(calculate(postfix, t + DT));
	double b = my::correct(calculate(postfix, t));
	double c = my::correct(calculate(postfix, t - DT));
	 
	if (!(isfinite(a) && isfinite(b) && isfinite(c))){
		if (isnan(a) || isnan(b) || isnan(c))
			return NAN;

		if (isinf(a))
			return a;

		if (isinf(b))
			return NAN;

		if (isinf(c))
			return -c;

		return NAN;
	}

	double d2 = my::correct(a - b) / DT;
	d2 += my::correct(c - b) / DT;
	d2 /= DT;

	//double d2 = (a - b + c - b) / (DT*DT);

	return my::correct(d2);
}

// Функция рассчитывающая массив значений выражения
std::vector<std::pair<double, double>> getVals(double(*pFunc)(std::vector<Token>&, double), std::vector<Token> & postfix, double stVal, double edVal){
	std::vector<std::pair<double, double>> val;

	for (double x = stVal; x <= edVal; x = my::correct(x + STEP)){
		if (abs(x) < ACCURACY) // Если есть 0, то пусть будет и -0
			val.push_back({ -x, my::correct(pFunc(postfix, -x)) });

		val.push_back({ x, my::correct(pFunc(postfix, x)) });
	}

	fprintf(logFile, "postfix: ");
	for (int i = 0; i < postfix.size(); i++)
		fprintf(logFile, "%s ", postfix[i]);

	fprintf(logFile, "\n");

	for (int i = 0; i < val.size(); i++)
		fprintf(logFile, "%f : %f\n", val[i].first, val[i].second);

	return val;
}
// Она же, только для 2 выражений
std::vector<std::pair<double, double>> getVals(double(*pFunc)(std::vector<Token>&, std::vector<Token>&, double), std::vector<Token> & postfix1, std::vector<Token> & postfix2, double stVal, double edVal){
	std::vector<std::pair<double, double>> val;
	
	for (double x = stVal; x <= edVal; x = my::correct(x + STEP)){
		if (abs(x) < ACCURACY) // Если есть 0, то пусть будет и -0
			val.push_back({ -x, my::correct(pFunc(postfix1, postfix2, -x)) });
		val.push_back({ x, my::correct(pFunc(postfix1, postfix2, x)) });
	}


	fprintf(logFile, "postfix1: ");
	for (int i = 0; i < postfix1.size(); i++)
		fprintf(logFile, "%s ", postfix1[i]);
	fprintf(logFile, "\n");

	fprintf(logFile, "postfix2: ");
	for (int i = 0; i < postfix2.size(); i++)
		fprintf(logFile, "%s ", postfix2[i]);
	fprintf(logFile, "\n");

	for (int i = 0; i < val.size(); i++)
		fprintf(logFile, "%f : %f\n", val[i].first, val[i].second);


	return val;
}

// Рассчет полной скорости
double speed(std::vector<Token> & postfixX, std::vector<Token> & postfixY, double t){
	double Vx = my::correct(derivative(calculate, postfixX, t));
	double Vy = my::correct(derivative(calculate, postfixY, t));

	if (!(isfinite(Vx) && isfinite(Vy))){
		if (isnan(Vx) || isnan(Vy))
			return NAN;

		if (isinf(Vx) && isinf(Vy) && FSIGN_C(Vx) != FSIGN_C(Vy)) // Если скорости это бесконечности разных знаков
			return NAN; // то результат неопределенность

		if (isinf(Vx))
			return Vx;

		if (isinf(Vy))
			return Vy;
	}

	double V = my::correct(sqrt(Vx*Vx + Vy*Vy));
	return V;
}

// Рассчет угловой скорости
double angleSpeed(std::vector<Token> & postfixX, std::vector<Token> & postfixY, double t){
	FPOINT p1;
	FPOINT p2;

	p1.x = calculate(postfixX, t);
	p1.y = calculate(postfixY, t);

	p2.x = calculate(postfixX, t+DT);
	p2.y = calculate(postfixY, t+DT);

	double len1 = sqrt(p1.x*p1.x + p1.y*p1.y);
	double len2 = sqrt(p2.x*p2.x + p2.y*p2.y);
	double mul12 = p1.x*p2.x + p1.y*p2.y;

	double angle = mul12 / (len1 * len2);

	return angle;
}

// Рассчет полного ускорения
double acceleration(std::vector<Token> & postfixX, std::vector<Token> & postfixY, double t){
	double ax = my::correct(derivative2(postfixX, t));
	double ay = my::correct(derivative2(postfixY, t));

	if (!(isfinite(ax) && isfinite(ay))){
		if (isnan(ax) || isnan(ay))
			return NAN;

		if (isinf(ax) && isinf(ay) && FSIGN_C(ax) != FSIGN_C(ay)) // Если ускорения это бесконечности разных знаков
			return NAN; // то результат неопределенность

		if (isinf(ax))
			return abs(ax);

		if (isinf(ay))
			return abs(ay);
	}

	double a = my::correct(sqrt(ax*ax + ay*ay));
	return a;
}

// Рассчет тангенциального ускорения
double accelerationT(std::vector<Token> & postfixX, std::vector<Token> & postfixY, double t){
	double at = my::correct(derivative(speed, postfixX, postfixY, t));
	return at;
}

// Рассчет тангенциального ускорения
double accelerationN(std::vector<Token> & postfixX, std::vector<Token> & postfixY, double t){
	double an = my::correct(derivative(angleSpeed, postfixX, postfixY, t));
	return an;
}

// Рассчет нормального ускорения
//double accelerationN(std::vector<Token> & postfixX, std::vector<Token> & postfixY, double t){
//	double a = acceleration(postfixX, postfixY, t);
//	double at = accelerationT(postfixX, postfixY, t);
//
//	if (!(isfinite(a) && isfinite(at))){
//		if (isnan(a) || isnan(at))
//			return NAN;
//
//		if (isinf(a) && isinf(at)) // Если ускорения это две бесконечности
//			return NAN; // то результат неопределенность
//		
//		if (isinf(a))
//			return abs(a);
//
//		if (isinf(at))
//			return NAN;
//
//		return NAN;
//	}
//
//	a = my::correct(a);
//	at = my::correct(at);
//
//	double an = my::correct(sqrt(a*a - at*at));
//	return an;
//}

//int main(){
//	setlocale(LC_CTYPE, "rus");
//
//	string expr = "-3 + 4 - [e] * sin(2) + cos(t) / pi^(actg(t/2) - atg(-t/2)) + asin(0.5) * acos(0.4) + sin(cos(3))";
//	vector<Token> tokens;
//	vector<Token> postfix;
//	if (createTokensFromExpression(expr, tokens))
//		if (createPostfixFromTokens(postfix, tokens)){
//			for (int i = 0; i < postfix.size(); i++)
//				printf("%s ", postfix[i].name.c_str());
//
//			printf("\n");
//		}
//
//	for (double t = 9.8; t < 1000; t += 5){
//		cout << "f(" << t << ") = " << calculate(postfix, t) << endl;
//		cout << "f`(" << t << ") = " << derivative(postfix, t) << endl;
//		cout << "f``(" << t << ") = " << derivative2(postfix, t) << endl;
//		cout << endl;
//	}
//
//	return 0;
//}

