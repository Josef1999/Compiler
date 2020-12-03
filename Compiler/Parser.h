#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <unordered_map>
#include <map>
#include <queue>
#include <stack>
#include <unordered_set>
#include <algorithm>
#include <iomanip>
#include "utils.h"
#define OK 1
#define ERROR -1

using namespace std;

struct I_Element
{
	string left_part;	//�Ƶ�ʽ��
	string right_part;	//�Ƶ�ʽ�Ҳ�
	char forward;		//LR(1)չ��
	int num;	//���λ��,".S"ʱnum=-1;
	I_Element(const string& Left_part, const string& Right_part,const int& Num=-1, const char& Forward='#')
	{
		left_part = Left_part;
		right_part = Right_part;
		forward = Forward;
		num = Num;
	}
	I_Element(){
		;
	}
};
//����unordered_set ��ϣ��ͻ��
struct I_ElementHash
{
	size_t operator()(const I_Element& rhs) const
	{
		return hash<string>()(rhs.left_part) ^ hash<string>()(rhs.right_part) ^ hash<int>()(rhs.num) ^ hash<char>()(rhs.forward);
	}
};
struct I_ElementCmp
{
	bool operator()(const I_Element& lhs, const I_Element& rhs) const {
		return lhs.left_part == rhs.left_part && lhs.right_part == rhs.right_part && lhs.forward == rhs.forward && lhs.num == rhs.num;
	}
};

class PARSER
{
public:
	PARSER();
	bool LR1(const string& grammer_in, const string& file_in);
	void show_GrammarTree();
	void output_GrammarTree();

private:
	ifstream infile;						//�����ļ�
	ofstream outfile;						//����ļ�

	string InputString;						//���봮
	int InputString_idx = 0;				//�׸�δ��������봮�ַ�����λ��
	vector<int> Status;						//״̬ջ
	vector<char> Symbol;					//����ջ
	stack<int> GrammarTree;
	unordered_set<char>NonTerminal;			//���ս����
	unordered_set<char>Terminal;			//�ս����
	
	vector<pair<string, string>> Grammar_Rules;			//Grammar_Rules[i]=pair<starter,beginer> (starter->beginer)
	map<string, unordered_set<char>> First;			//First[string]= string��First��
	vector<unordered_map<char, int>> Goto;					//Goto[S][a]= target_status, S=cur_status, a=cur_symbol

	//push_in = �ƽ� �� pop_out = ��Լ
	typedef enum { push_in=1, pop_out=2, acc=3}action;
	vector<unordered_map<char, pair<action, int>>> Action;	//Action[S][a] ��ȡ��Ҫ��ȡ���ж�

	int I_size;											//��Ŀ������
	vector<vector<I_Element>> I;						//��Ŀ��
	void init(const string& grammer_in);				//��ʼ��Goto��Action��Grammer_Rules��First��Terminal��NonTerminal
	void init_Terminal();
	void init_NonTerminal();
	void init_First();
	void init_table();									//Action��Goto��
	void get_First(const string& str);					//���ַ���str��First��
	int get_Grammar_Rules_index(const pair<string, string> &target);	//�����Ƶ�ʽ����������
	int  go(vector<I_Element> I, char X);		//GO(I��X)��CLOSURE(J) J��{�κ�����A��aX��b����Ŀ| A��a��Xb����I}
	bool analysis();
	void analysis_init(const string& file_name);
	void Lex_to_Parser(const string& file_name);			//Lex���תΪParser�ɶ�
	//DEBUG��
	void show_NonTerminal();
	void show_Terminal();
	void show_Grammer_Rules();
	void show_this_Grammar_Rules(const int& index);
	void show_First();
	void show_closure();
	void show_this_closure();
	void show_Action_and_Goto();
	void show_this_analysis_step();
	void show_Symbol();
	void show_Status();
	void show_InputString();
	void output_this_analysis_step();
};