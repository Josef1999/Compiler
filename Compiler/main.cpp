#include"Lex.h"
#include"Parser.h"
#define SOURCE_CODE "../源代码.txt"
#define LEX_RESULT ".. / 词法分析结果.txt"
#define GRAMMAR_INPUT "../表达式.txt"
int main()
{
	/*
	LEX L;
	L.analyse(SOURCE_CODE, LEX_RESULT);
	*/
	PARSER P;
	P.LR1(GRAMMAR_INPUT, LEX_RESULT);
	return 0;
}