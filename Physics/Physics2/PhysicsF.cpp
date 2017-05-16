#pragma once

#include "stdafx.h"
#include "PhysicsF.h"

//#define MY_INFINITE_POS 1e+50
//#define MY_INFINITE_NEG -(1e+50)
#define DT 1e-3

//#define CORRECTION(a) (isinf(a) ? (((a) > 0) ? MY_INFINITE_POS : MY_INFINITE_NEG) : (MAX2(MIN2((a), MY_INFINITE_POS), MY_INFINITE_NEG)))

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
float calculate(std::vector<Token> & postfix, float t){
	float pi = 3.14159265;
	float e = 2.71828182;

	std::stack<float> fStack;

	float a, b;

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
				fStack.push(sin(a * pi / 180));
			}
			else if (postfix[i].name == "cos"){
				fStack.push(cos(a * pi / 180));
			}
			else if (postfix[i].name == "asin"){
				fStack.push(asin(a));
			}
			else if (postfix[i].name == "acos"){
				fStack.push(acos(a));
			}
			else if (postfix[i].name == "tg"){
				fStack.push(tan(a * pi / 180));
			}
			else if (postfix[i].name == "ctg"){
				fStack.push(1 / tan(a * pi / 180));
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

	float result = fStack.top();
	fStack.pop();

	return result;
}

//f` = (f(x + h) - f(x - h)) / 2h
float derivative(float(*pFunc)(std::vector<Token>&, float), std::vector<Token> & postfix, float t){
	if (!isfinite(pFunc(postfix, t)))
		return NAN;
	
	float a = pFunc(postfix, t + DT);
	float b = pFunc(postfix, t - DT);

	a = std::floor(a * ACCURACYN + .5) / ACCURACYN;
	b = std::floor(b * ACCURACYN + .5) / ACCURACYN;

	return (a - b) / (2 * DT);
}

float derivative(float(*pFunc)(std::vector<Token>&, std::vector<Token>&, float), std::vector<Token> & postfixX, std::vector<Token> & postfixY, float t){
	if (!isfinite(pFunc(postfixX, postfixY, t)))
		return NAN;

	float a = pFunc(postfixX, postfixY, t + DT);
	float b = pFunc(postfixX, postfixY, t - DT);

	a = std::floor(a * ACCURACYN + .5) / ACCURACYN;
	b = std::floor(b * ACCURACYN + .5) / ACCURACYN;

	return (a - b) / (2 * DT);
}

//f`` = (f(x+h) - 2f(x) + f(x-h)) / h^2
float derivative2(std::vector<Token> & postfix, float t){
	if (!isfinite(derivative(calculate, postfix, t)))
		return NAN;
	
	float a = calculate(postfix, t + DT);
	float b = calculate(postfix, t);
	float c = calculate(postfix, t - DT);
	 
	a = std::floor(a * ACCURACYN + .5) / ACCURACYN;
	b = std::floor(b * ACCURACYN + .5) / ACCURACYN;
	c = std::floor(c * ACCURACYN + .5) / ACCURACYN;

	return (a - 2 * b + c) / (DT * DT);
}

// Функция рассчитывающая массив значений выражения
std::vector<std::pair<float, float>> getVals(float(*pFunc)(std::vector<Token>&, float), std::vector<Token> & postfix, float stVal, float edVal, float stepVal){
	std::vector<std::pair<float, float>> val;

	for (float x = stVal; x <= edVal; x = std::floor((x + stepVal) * ACCURACYN + .5) / ACCURACYN){
		if (abs(x) < ACCURACY) // Если есть 0, то пусть будет и -0
			val.push_back({ -x, pFunc(postfix, -x) });

		val.push_back({ x, pFunc(postfix, x) });
	}

	return val;
}
// Она же, только для 2 выражений
std::vector<std::pair<float, float>> getVals(float(*pFunc)(std::vector<Token>&, std::vector<Token>&, float), std::vector<Token> & postfix1, std::vector<Token> & postfix2, float stVal, float edVal, float stepVal){
	std::vector<std::pair<float, float>> val;
	
	for (float x = stVal; x <= edVal; x = std::floor((x + stepVal) * ACCURACYN + .5) / ACCURACYN){
		if (abs(x) < ACCURACY) // Если есть 0, то пусть будет и -0
			val.push_back({ -x, pFunc(postfix1, postfix2, -x) });
		val.push_back({ x, pFunc(postfix1, postfix2, x) });
	}

	return val;
}

// Рассчет полной скорости
float speed(std::vector<Token> & postfixX, std::vector<Token> & postfixY, float t){
	float Vx = derivative(calculate, postfixX, t);
	float Vy = derivative(calculate, postfixY, t);

	if (!(isfinite(Vx) && isfinite(Vy)))
		return NAN;

	float V = sqrt(Vx*Vx + Vy*Vy);
	return V;
}

// Рассчет полного ускорения
float acceleration(std::vector<Token> & postfixX, std::vector<Token> & postfixY, float t){
	float ax = derivative2(postfixX, t);
	float ay = derivative2(postfixY, t);

	if (!(isfinite(ax) && isfinite(ay)))
		return NAN;

	float a = sqrt(ax*ax + ay*ay);
	return a;
}

// Рассчет тангенциального ускорения
float accelerationT(std::vector<Token> & postfixX, std::vector<Token> & postfixY, float t){

	if (!isfinite(speed(postfixX, postfixY, t)))
		return NAN;

	float at = derivative(speed, postfixX, postfixY, t);
	return at;
}

// Рассчет нормального ускорения
float accelerationN(std::vector<Token> & postfixX, std::vector<Token> & postfixY, float t){
	
	if (!isfinite(speed(postfixX, postfixY, t)))
		return NAN;

	float a = acceleration(postfixX, postfixY, t);
	float at = accelerationT(postfixX, postfixY, t);

	float an = sqrt(a*a - at*at);
	return an;
}

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
//	for (float t = 9.8; t < 1000; t += 5){
//		cout << "f(" << t << ") = " << calculate(postfix, t) << endl;
//		cout << "f`(" << t << ") = " << derivative(postfix, t) << endl;
//		cout << "f``(" << t << ") = " << derivative2(postfix, t) << endl;
//		cout << endl;
//	}
//
//	return 0;
//}

