#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <unordered_map>
#include<unordered_set>
#include <algorithm>
#include <stack>
#define OK 1
#define ERROR -1

using namespace std;

struct I_Element
{
	string left_part;	//推导式左部
	string right_part;	//推导式右部
	int num;	//点的位置
};
class Parser
{
public:
	Parser();
	bool LR1(const string& grammer_in, const string& file_in);
private:
	ifstream infile;						//输入文件
	ofstream outfile;						//输出文件

	string input_sting;						//输入串
	stack<int> Status;						//状态栈
	stack<char> Symbol;						//符号栈

	unordered_set<char>NonTerminal;			//非终结符集
	unordered_set<char>Terminal;			//终结符集

	unordered_map<char, unordered_set<char>> First;			//First[char]= char的First集
	vector<unordered_map<char, int>> Goto;					//Goto[S][a]= target_status, S=cur_status, a=cur_symbol

	typedef enum { push_in, pop_out, acc }action;
	vector<unordered_map<char, pair<action, int>>> Action;	//Action[S][a] 获取所要采取的行动

	vector<pair<string, string>> Grammer_Rules;			//Grammer_Rules[i]=pair<starter,beginer> (starter->beginer)

	vector<I_Element> I_element;						//所有项目
	int I_size;											//项目集个数
	vector<vector<I_Element>> I;						//项目集
	void init();										//初始化Goto、Action、Grammer_Rules、First、Terminal、NonTerminal
	void closure(vector<vector<I_Element>>& I);			//求项目集闭包
	//void go(vector<vector<I_Element>> I, char X);		//GO(I，X)＝CLOSURE(J) J＝{任何形如A→aX·b的项目| A→a·Xb属于I}

};