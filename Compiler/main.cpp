#include"Lex.h"
#include"Parser.h"
#define USE_DEBUG_FILE

#ifndef USE_DEBUG_FILE
	#define SOURCE_CODE "../源代码.txt"
	#define LEX_RESULT ".. /词法分析结果.txt"
	#define GRAMMAR_INPUT "../表达式.txt"
#else
	#define SOURCE_CODE "../源代码.txt"
	#define LEX_RESULT "../测试用文件/词法分析结果_debug.txt"
	#define GRAMMAR_INPUT "../测试用文件/表达式_debug.txt"
#endif

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