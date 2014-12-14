/*
 * Calculate.cpp
 *
 *  Created on: December 2, 2014
 *      Author: EOS
 */
#include "Calculate.h"
#include "SceneRegister.h"

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cwchar>
#include <math.h>
#include <limits>

#define MAX		1024	// 입력 스트링의 최대 문자수
#define TOKEN	20		// 함수의 최대 문자수
#define M_PI 3.14159265358979323846
#define M_E 2.7182818284590452354

const static double RESULT_OK = 901;
const static double RESULT_INF = 902;
const static double RESULT_OVERFLOW = 903;
const static double RESULT_INVALID = 904;

using namespace Tizen::Base;

// 지원하는 수학 함수
char* Table[] = {
		// first element of table -> 키코드 -100
		"fabs", "fac", "sqrt", "pow", "exp", "ln", "log", "dtor",
		"rtod", // deg <-> rad
		"sin", "cos", "tan", "sinh", "cosh", "tanh", "asin", "acos", "atan",
		"atan2", "step", "ramp", "", // end of table
		};

// A ~ Z 까지의 26개의 변수
double Calculate::Variable[26] = { '\r', '\r', '\r', '\r', '\r', '\r', '\r',
		'\r', '\r', '\r', '\r', '\r', '\r', '\r', '\r', '\r', '\r', '\r', '\r',
		'\r', '\r', '\r', '\r', '\r', '\r', '\r', };

/////////////////////////////////////////////////////////////////////
//	Class Constructor & Destructor
/////////////////////////////////////////////////////////////////////
Calculate::Calculate() {
	Init();
}

Calculate::~Calculate() {
	m_fResult = new double[2];
	m_fResult[1] = RESULT_OK;
	m_bMode = FALSE;
	m_nTop = 0;
	if (m_nStack)
		delete[] m_nStack;
	if (m_fValue)
		delete[] m_fValue;
	if (m_strInfix)
		delete[] m_strInfix;
	if (m_strPostfix)
		delete[] m_strPostfix;
}

/////////////////////////////////////////////////////////////////////
//	Public Member Function
/////////////////////////////////////////////////////////////////////

// Main function of class
double* Calculate::Run(String input) {
	Init();

	std::string tempstring;
	tempstring = Calculate::WStringtostring(input);
	const char* string = tempstring.c_str();

	if (!Read(string)) {
		printf("   Error : Unpaired bracket...!\n");
		return NULL;
	}

	Postfix();
	Evaluate();

	return m_fResult;
}

/////////////////////////////////////////////////////////////////////
//	Protected Member Function - Main parts
/////////////////////////////////////////////////////////////////////

// Tizen::Base::String to std::string
std::string Calculate::WStringtostring(Tizen::Base::String input) {
	std::string after;
	std::wstring before(input.GetPointer());
	after.assign(before.begin(), before.end());

	return after.c_str();
}

// 초기화 한다.
void Calculate::Init() {
	m_fResult = new double[2];
	m_fResult[1] = RESULT_OK;
	m_bMode = FALSE;
	m_nTop = 0;
	m_nStack = 0;
	m_fValue = 0;
	m_strInfix = 0;
	m_strPostfix = 0;
}

// 입력 스트링을 읽어들인다.
BOOL Calculate::Read(std::string string) {
	// 괄호의 갯수가 일치하는지 조사한다.
	int k = 0;
	int nLeftb = 0, nRightb = 0;
	while (string[k]) {
		switch (string[k]) {
		case '(':
			nLeftb++;
			break;
		case ')':
			nRightb++;
			break;
		}
		k++;
	}
	if ((nLeftb != nRightb))
		return FALSE;

	// 입력 스트링에 있는 공백은 무시한다.

	char* strTemp = new char[string.length() + 1];
	int i = 0, j = 0;
	while (string[i]) {
		while ((string[i] == ' ') || (string[i] == '\t') || (string[i] == '\n'))
			i++;
		strTemp[j] = string[i];
		i++;
		j++;
	}
	strTemp[j++] = NULL;

	// 치환모드인지 연산 모드인지 판별한다. ?? 이부부 딱히 필요없는듯
	if (isalpha(strTemp[0]) && (strTemp[1] == '=') && (strTemp[2] != '=')
			&& (strTemp[2] != NULL))
		m_bMode = FALSE; // 치환모드
	else {
		m_bMode = TRUE; // 연산모드
		if (strTemp[strlen(strTemp) - 1] == '=')
			strTemp[strlen(strTemp) - 1] = '\0';
	}

	m_strInfix = new char[strlen(strTemp) + 1];
	strcpy(m_strInfix, strTemp);
	delete[] strTemp;

	return TRUE;
}

// 주어진 infix 형태의 수식을 postfix로 변환한다.
void Calculate::Postfix() {
	m_nTop = 0;
	m_nStack = new int[MAX];
	m_strPostfix = new char[MAX];

	// 연산 순서에 맞게 배치한다.
	int i = 0, j = 0, nCount = 0;
	while (m_strInfix[i]) {
		// 괄호 처리
		if (m_strInfix[i] == '(')
			PushStack(m_strInfix[i++]);
		else if (m_strInfix[i] == ')') {
			while (GetStack() != '(')
				m_strPostfix[j++] = PopStack();
			if (GetStack() == ')') {
				m_fResult[1] = RESULT_INVALID;
				m_fResult[0] = 0;
				return;
			}
			PopStack();
			i++;
		}

		// 연산자 처리
		else if (IsOperator(m_strInfix[i])) {
			// 부호 연산자 처리
			int flag = (nCount == 0) || m_strInfix[i - 1] == '('
					|| IsOperator(m_strInfix[i - 1]);
			if (m_strInfix[i] == '-' && flag) {
				// 마이너스 숫자일 때 앞에 ~ 넣어줌
				PushStack('~');
				i++;
			}
			/*
			 else if (m_strInfix[i] == '+' && flag)
			 i++;
			 //*/
			else {
				while (!IsEmpty()
						&& GetOrder(GetStack()) >= GetOrder(m_strInfix[i]))
					m_strPostfix[j++] = PopStack();
				PushStack(m_strInfix[i++]);
			}
		}
		// 숫자 처리
		else if (isdigit(m_strInfix[i]) || (m_strInfix[i] == '.')) {
			while (isdigit(m_strInfix[i]) || (m_strInfix[i] == '.'))
				m_strPostfix[j++] = m_strInfix[i++];
			m_strPostfix[j++] = ' ';
		}
		// 문자 처리
		else if (isalpha(m_strInfix[i])) {
			if (isalpha(m_strInfix[i + 1])) {
				char* temp = new char[20];
				int k = 0;
				while (isalpha(m_strInfix[i]))
					temp[k++] = m_strInfix[i++];
				temp[k] = '\0';

				if (int code = IsFunction(temp))
					PushStack(code);
				else if (!(strcmp(temp, "pi")))
					m_strPostfix[j++] = -99;
				delete[] temp;
			} else
				m_strPostfix[j++] = m_strInfix[i++];
		} else
			i++;
		nCount++;
	}

	while (!IsEmpty())
		m_strPostfix[j++] = PopStack();
	m_strPostfix[j++] = '\0';
	return;
}

bool Calculate::ResultRangeCheck(double value_1, double value_2, char exp) {
	double double_max = std::numeric_limits<double>::max();
	double double_min = std::numeric_limits<double>::min();

	switch (exp) {
	case '^': {
		if (pow(value_1, value_2) == 0)
			return true;
		else {
			if (pow(value_1, value_2) > double_max
					|| pow(value_1, value_2) < double_min)
				return false;
			else
				return true;
		}
	}
	case '+': {
		if ((value_1 + value_2) == 0)
			return true;
		else {
			if ((value_1 + value_2) > double_max
					|| (value_1 + value_2) < double_min)
				return false;
			else
				return true;
		}
	}
	case '-': {
		if ((value_1 - value_2) == 0)
			return true;
		else {
			if ((value_1 - value_2) > double_max
					&& (value_1 - value_2) < double_min) {
				return false;
			} else
				return true;
		}
	}
	case '*': {
		if ((value_1 * value_2) == 0)
			return true;
		else {
			if ((value_1 * value_2) > double_max
					|| (value_1 * value_2) < double_min)
				return false;
			else
				return true;
		}
	}
	case '/': {
		if ((value_1 / value_2) == 0)
			return true;
		else {
			if ((value_1 / value_2) > double_max
					|| (value_1 / value_2) < double_min)
				return false;
			else
				return true;
		}
	}
	}
}

bool Calculate::ResultRangeCheck(double value_1) {
	double double_max = std::numeric_limits<double>::max();
	double double_min = std::numeric_limits<double>::min();

	if (value_1 > double_max || value_1 < double_min)
		return false;
	else
		return true;
}

void Calculate::Evaluate() {
	m_nTop = 0;
	m_fValue = new double[MAX];
	int i = 0;
	char variable;

	while (m_strPostfix[i]) {
		// 숫자 처리 부분
		if (isdigit(m_strPostfix[i]) || m_strPostfix[i] == '.') {
			char *number = new char[TOKEN];
			int j = 0;
			while (isdigit(m_strPostfix[i]) || m_strPostfix[i] == '.')
				number[j++] = m_strPostfix[i++];
			number[j] = NULL;
			PushValue(atof(number));
		}
		// 연산자 처리 부분
		else if (m_strPostfix[i] == '~') // 부호연산자
				{
			double rvar = PopValue();
			PushValue(-1 * rvar);
			i++;
		} else if (m_strPostfix[i] == '+') {
			double rvar = PopValue();
			double lvar = PopValue();
			if (ResultRangeCheck(lvar, rvar, '+'))
				PushValue(lvar + rvar);
			else {
				m_fResult[1] = RESULT_OVERFLOW;
				m_fResult[0] = 0;
				return;
			}
			//PushValue(lvar + rvar);
			i++;
		} else if (m_strPostfix[i] == '-') {
			double rvar = PopValue();
			double lvar = PopValue();
			if (ResultRangeCheck(lvar, rvar, '-'))
				PushValue(lvar - rvar);
			else {
				m_fResult[1] = RESULT_OVERFLOW;
				m_fResult[0] = 0;
				return;
			}
			//PushValue(lvar - rvar);
			i++;
		} else if (m_strPostfix[i] == '*') {
			double rvar = PopValue();
			double lvar = PopValue();
			if (ResultRangeCheck(lvar, rvar, '*'))
				PushValue(lvar * rvar);
			else {
				m_fResult[1] = RESULT_OVERFLOW;
				m_fResult[0] = 0;
				return;
			}
			//PushValue(lvar * rvar);
			i++;
		} else if (m_strPostfix[i] == '/') {
			double rvar = PopValue();
			double lvar = PopValue();
			PushValue(lvar / rvar);
			i++;
		} else if (m_strPostfix[i] == '%') {
			double rvar = PopValue();
			double lvar = PopValue();
			PushValue(fmod(lvar, rvar));
			i++;
		} else if (m_strPostfix[i] == '^') {
			double rvar = PopValue();
			double lvar = PopValue();
			if (ResultRangeCheck(lvar, rvar, '^'))
				PushValue(pow(lvar, rvar));
			else {
				m_fResult[1] = RESULT_OVERFLOW;
				m_fResult[0] = 0;
				return;
			}
			i++;
		}
		// factorial
		else if (m_strPostfix[i] == '!') {
			double rvar = (int) PopValue();
			double result = 1;
			for (rvar; rvar > 0; rvar--) {
				result *= rvar;
				if (!ResultRangeCheck(result)) {
					m_fResult[1] = RESULT_OVERFLOW;
					m_fResult[0] = 0;
					return;
				}
			}
			PushValue(result);
			i++;
		}
		//Pi
		else if (m_strPostfix[i] == 'p') {
			PushValue(M_PI);
			i++;
		}
		//e
		else if (m_strPostfix[i] == 'e') {
			PushValue(M_E);
			i++;
		}
		//absolute
		else if (m_strPostfix[i] == 'a') {
			double rvar = PopValue();
			PushValue(fabs(rvar));
			i++;
		}
		//sqrt
		else if (m_strPostfix[i] == 'r') {
			double rvar = PopValue();
			PushValue(sqrt(rvar));
			i++;
		}
		//power
		else if (m_strPostfix[i] == '^') {
			double rvar = PopValue();
			double lvar = PopValue();
			PushValue(pow(lvar, rvar));
			i++;
		}
		//exp
		else if (m_strPostfix[i] == 'm') {
			double rvar = PopValue();
			PushValue(exp(rvar));
			i++;
		}
		//ln
		else if (m_strPostfix[i] == 'l') {
			double rvar = PopValue();
			PushValue(log(rvar) / log(M_E));
			i++;
		}
		//log
		else if (m_strPostfix[i] == 'g') {
			double rvar = PopValue();
			PushValue(log(rvar));
			i++;
		}
		//sin
		else if (m_strPostfix[i] == 's') {
			double rvar = PopValue();
			PushValue(sin(rvar * M_PI / 180));

			i++;
		}
		//cos
		else if (m_strPostfix[i] == 'c') {
			double rvar = PopValue();
			PushValue(cos(rvar * M_PI / 180));
			i++;
		}
		//tan
		else if (m_strPostfix[i] == 't') {
			double rvar = PopValue();
			if (fmod(rvar, 180) == 90) {
				m_fResult[1] = RESULT_INF;
				m_fResult[0] = 0;
				return;
			}
			PushValue(tan(rvar * M_PI / 180));
			i++;
		} else
			i++;
	}

	m_fResult[0] = PopValue();

	return;
}

/////////////////////////////////////////////////////////////////////
//	Protected Member Function - Sub Parts
/////////////////////////////////////////////////////////////////////

// 스택의 모든 항이 비어있는지를 확인한다.
BOOL Calculate::IsEmpty(void) {
	if (m_nTop < 1)
		return TRUE;
	else
		return FALSE;
}

// 스택의 가장 마지막 항에 주어진 문자를 입력한다.
int Calculate::PushStack(int ch) {
	if (m_nTop >= MAX) {
		printf("  Error : Stack overflow...!\n");
		exit(1);
	}
	m_nStack[m_nTop++] = ch;
	return ch;
}

// 현재 스택에 저장되어 있는 가장 마지막 문자를 읽어들인다.
int Calculate::PopStack(void) {
	if (m_nTop < 1) {
		printf("  Error : Stack Underflow...!\n");
		exit(1);
	}
	return m_nStack[--m_nTop];
}

int Calculate::GetStack(void) {
	return (m_nTop < 1) ? -1 : m_nStack[m_nTop - 1];
}

// 스택의 가장 마지막 항에 주어진 값을 입력한다.
double Calculate::PushValue(double value) {
	if (m_nTop >= MAX) {
		printf("  Error : Buffer overflow...!\n");
		exit(1);
	}
	m_fValue[m_nTop++] = value;
	return m_fValue[m_nTop];
}

// 현재 버퍼에 저장되어 있는 가장 마지막 값을 읽어들인다.
double Calculate::PopValue(void) {
	if (m_nTop < 1) {
		printf("  Error : Buffer underflow...!\n");
		exit(1);
	}
	return m_fValue[--m_nTop];
}

// 변수값을 치환
void Calculate::PushVar(char ch, double value) {
	if (!isalpha(ch)) {
		printf("  Error : It is not a variable...!\n");
		exit(1);
	}
	int var = toupper(ch) - 'A';
	Variable[var] = value;
	return;
}

// 메모리에서 변수값을 일어들임
double Calculate::PopVar(char ch) {
	if (!isalpha(ch)) {
		printf("  Error : This is not a variable...!\n");
		exit(1);
	}
	return Variable[toupper(ch) - 'A'];
}

// string을 받아들이고 만약 함수라면 키코드를 반환한다.
int Calculate::IsFunction(const char* str) {
	for (int k = 0; *Table[k]; k++)
		if (!strcmp(Table[k], str))
			return -100 - k;
	return 0;
}

// 주어진 문자가 연산자인지 아닌지 판별한다.
int Calculate::IsOperator(int ch) {
	if (strchr("+-*^/%!,=<>rsctlga", ch) || (ch >= -95 && ch <= -90))
		return ch;
	else
		return 0;
}

// 연산자의 우선순위를 반환한다. 반환되는 숫자가 클수록 순위가 낮다.
int Calculate::GetOrder(int ch) {
	if (ch == '=')
		return 0;
	else if (ch == '(' || ch == '[')
		return 1;
	else if (ch == ',')
		return 2;
	else if (ch >= -95 && ch <= -90) // 비교연산자
		return 3;
	else if (ch == '+' || ch == '-') // 이항연산자
		return 4;
	else if (ch == '*' || ch == '/' || ch == '%')
		return 5;
	else if (ch == '~') // 부호연산자
		return 6;
	else if (ch == '^' || ch == '!') // 단항연산자
		return 7;
	else
		return 8;
}
