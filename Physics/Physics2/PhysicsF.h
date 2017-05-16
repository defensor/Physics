#pragma once

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <locale.h>
#include <math.h>

#define ACCURACY 1e-6
#define ACCURACYN 1e+6


#define MAX2(a, b) (((a) > (b)) ? (a) : (b))
#define MIN2(a, b) (((a) < (b)) ? (a) : (b))
#define MAX3(a,b,c) (MAX2(MAX2((a), (b)), (c)))

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
float calculate(std::vector<Token> & postfix, float t);

//f` = (f(x + h) - f(x - h)) / 2h
float derivative(std::vector<Token> & postfix, float t);

//f`` = (f(x+h) - 2f(x) + f(x-h)) / h^2
float derivative2(std::vector<Token> & postfix, float t);

// Функция рассчитывающая массив значений выражения
std::vector<std::pair<float, float>> getVals(float(*pFunc)(std::vector<Token>&, float), std::vector<Token> & postfix, float stVal, float edVal, float stepVal);
std::vector<std::pair<float, float>> getVals(float(*pFunc)(std::vector<Token>&, std::vector<Token>&, float), std::vector<Token> & postfix1, std::vector<Token> & postfix2, float stVal, float edVal, float stepVal);

// Рассчет полной скорости
float speed(std::vector<Token> & postfixX, std::vector<Token> & postfixY, float t);
// и полного ускорения
float acceleration(std::vector<Token> & postfixX, std::vector<Token> & postfixY, float t);