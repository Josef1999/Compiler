#include"Lex.h"
#include"Parser.h"
#define USE_DEBUG_FILE

#ifndef USE_DEBUG_FILE
#define SOURCE_CODE "../Դ����.txt"
#define LEX_RESULT ".. /�ʷ��������.txt"
#define GRAMMAR_INPUT "../���ʽ.txt"
#else
#define SOURCE_CODE "../Դ����.txt"
#define LEX_RESULT "../�������ļ�/�ʷ��������_test_debug.txt"
#define GRAMMAR_INPUT "../���ʽ.txt"
#endif

int main()
{
	
	LEX L;
	L.analyse(SOURCE_CODE, LEX_RESULT);
	
	PARSER P;
	if (P.LR1(GRAMMAR_INPUT, LEX_RESULT))
		cout << "acc" << endl;
	else {
		cout << "reject" << endl;
	};

	return 0;
}