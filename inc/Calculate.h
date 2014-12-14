/*
 * Calculate.h
 *
 *  Created on: Nov 26, 2014
 *      Author: EOS
 */
#ifndef CALCULATE_H_
#define CALCULATE_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>

using namespace Tizen::Base;

typedef enum BOOL {
	FALSE, TRUE
};

class Calculate {
// Protected Member Variables
protected:
	static double Variable[26];
	BOOL m_bMode; // 0 이면 치환, 1이면 연산
	//m_fResult[0] -> Result Value
	//m_fResult[1] -> Error Code
	double* m_fResult;
	int m_nTop;
	int* m_nStack;
	double* m_fValue;
	char* m_strInfix;
	char* m_strPostfix;
// Protected Member Functions
protected:
	// Sub Parts
	BOOL IsEmpty();
	int PushStack(int);
	int PopStack();
	int GetStack();
	double PushValue(double);
	double PopValue();
	void PushVar(char, double);
	double PopVar(char);
	int IsFunction(const char*);
	int IsOperator(int);
	int GetOrder(int);
	bool ResultRangeCheck(double, double, char);
	bool ResultRangeCheck(double);
protected:
	// Main Parts
	void Init(); // Initialize
	BOOL Read(std::string string); // Read input string, Detect which mode
	void Postfix(); // Write infix form equation to postfix form
	void Evaluate(); // Evaluate postfix equation
// Public Member Functions
public:
	Calculate();
	~Calculate();
	std::string WStringtostring(Tizen::Base::String input);
	double* Run(String string);
};

#endif /* CALCULATE_H_ */
