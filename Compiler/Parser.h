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
	string left_part;	//推导式左部
	string right_part;	//推导式右部
	char forward;		//LR(1)展望
	int num;	//点的位置,".S"时num=-1;
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
//处理unordered_set 哈希冲突用
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
	ifstream infile;						//输入文件
	ofstream outfile;						//输出文件

	string InputString;						//输入串
	int InputString_idx = 0;				//首个未处理的输入串字符所在位置
	vector<int> Status;						//状态栈
	vector<char> Symbol;					//符号栈
	stack<int> GrammarTree;
	unordered_set<char>NonTerminal;			//非终结符集
	unordered_set<char>Terminal;			//终结符集
	
	vector<pair<string, string>> Grammar_Rules;			//Grammar_Rules[i]=pair<starter,beginer> (starter->beginer)
	map<string, unordered_set<char>> First;			//First[string]= string的First集
	vector<unordered_map<char, int>> Goto;					//Goto[S][a]= target_status, S=cur_status, a=cur_symbol

	//push_in = 移进 ； pop_out = 规约
	typedef enum { push_in=1, pop_out=2, acc=3}action;
	vector<unordered_map<char, pair<action, int>>> Action;	//Action[S][a] 获取所要采取的行动

	int I_size;											//项目集个数
	vector<vector<I_Element>> I;						//项目集
	void init(const string& grammer_in);				//初始化Goto、Action、Grammer_Rules、First、Terminal、NonTerminal
	void init_Terminal();
	void init_NonTerminal();
	void init_First();
	void init_table();									//Action、Goto表
	void get_First(const string& str);					//求字符串str的First集
	int get_Grammar_Rules_index(const pair<string, string> &target);	//输入推导式，返回其编号
	int  go(vector<I_Element> I, char X);		//GO(I，X)＝CLOSURE(J) J＝{任何形如A→aX·b的项目| A→a·Xb属于I}
	bool analysis();
	void analysis_init(const string& file_name);
	void Lex_to_Parser(const string& file_name);			//Lex输出转为Parser可读
	//DEBUG用
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