#include"Lex.h"
#include"Parser.h"
#define SOURCE_CODE "../Դ����.txt"
#define LEX_RESULT ".. / �ʷ��������.txt"
#define GRAMMAR_INPUT "../���ʽ.txt"
//#define GRAMMAR_INPUT "../test.txt"

struct Record
{
    string name;
    int val;
};




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