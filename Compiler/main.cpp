#include"Lex.h"
#include"Parser.h"
#define USE_DEBUG_FILE

#ifndef USE_DEBUG_FILE
	#define SOURCE_CODE "../Դ����.txt"
	#define LEX_RESULT ".. /�ʷ��������.txt"
	#define GRAMMAR_INPUT "../���ʽ.txt"
#else
	#define SOURCE_CODE "../Դ����.txt"
	#define LEX_RESULT "../�������ļ�/�ʷ��������_debug.txt"
	#define GRAMMAR_INPUT "../�������ļ�/���ʽ_debug.txt"
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