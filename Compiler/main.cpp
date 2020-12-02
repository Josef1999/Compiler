#include"Lex.h"
#include"Parser.h"
#include"utils.h"
int main()
{
	/*
	LEX L;
	L.analyse(SOURCE_CODE, LEX_RESULT);
	*/
	PARSER P;
	if (P.LR1(GRAMMAR_INPUT, LEX_RESULT))
	{
		cout << "acc" << endl;
		
		P.output_GrammarTree();
	}
	else {
		cout << "reject" << endl;
	};

	return 0;
}