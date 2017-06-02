#pragma once

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <locale.h>
#include <math.h>

#define ACCURACY 1e-7
#define ACCURACYN 1e+7

#define STEP 1e-3

#define MAX2(a, b) (((a) > (b)) ? (a) : (b))
#define MIN2(a, b) (((a) < (b)) ? (a) : (b))
#define MAX3(a,b,c) (MAX2(MAX2((a), (b)), (c)))

namespace my{
	double correct(double a);
	double floor(double a);
}

struct FPOINT{
	double x;
	double y;
};

enum tokentype{
	VAR, NUM, OP_BR, CL_BR, FUNC, OP, OP_SBR, CL_SBR
};

struct Token{
	CString name;
	tokentype type;
};

// Проверки символа
bool isDelimiter(char c);

// Разбиваем выражение на токены
bool createTokensFromExpression(CString & expr, std::vector<Token> & tokens);

// Расставляем токены в порядке постфиксной записи
bool createPostfixFromTokens(std::vector<Token> & postfix, std::vector<Token> & tokens);

// Подсчет постфиксного выражения записанного токенами
double calculate(std::vector<Token> & postfix, double t);

//f` = (f(x + h) - f(x - h)) / 2h
double derivative(double(*pFunc)(std::vector<Token>&, double), std::vector<Token> & postfix, double t);
double derivative(double(*pFunc)(std::vector<Token>&, std::vector<Token>&, double), std::vector<Token> & postfixX, std::vector<Token> & postfixY, double t);

//f`` = (f(x+h) - 2f(x) + f(x-h)) / h^2
double derivative2(std::vector<Token> & postfix, double t);

// Функция рассчитывающая массив значений выражения
std::vector<std::pair<double, double>> getVals(double(*pFunc)(std::vector<Token>&, double), std::vector<Token> & postfix, double stVal, double edVal);
std::vector<std::pair<double, double>> getVals(double(*pFunc)(std::vector<Token>&, std::vector<Token>&, double), std::vector<Token> & postfix1, std::vector<Token> & postfix2, double stVal, double edVal);

// Рассчет полной скорости
double speed(std::vector<Token> & postfixX, std::vector<Token> & postfixY, double t);
double angleSpeed(std::vector<Token> & postfixX, std::vector<Token> & postfixY, double t);
// полного ускорения
double acceleration(std::vector<Token> & postfixX, std::vector<Token> & postfixY, double t);
// Ускорения тангенциального
double accelerationT(std::vector<Token> & postfixX, std::vector<Token> & postfixY, double t);
// и нормального
double accelerationN(std::vector<Token> & postfixX, std::vector<Token> & postfixY, double t);