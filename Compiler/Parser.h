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
	string left_part;	//�Ƶ�ʽ��
	string right_part;	//�Ƶ�ʽ�Ҳ�
	char forward;		//LR(1)չ��
	int num;	//���λ��,".S"ʱnum=-1;
};
class PARSER
{
public:
	PARSER();
	bool LR1(const string& grammer_in, const string& file_in);
private:
	ifstream infile;						//�����ļ�
	ofstream outfile;						//����ļ�

	string input_sting;						//���봮
	stack<int> Status;						//״̬ջ
	stack<char> Symbol;						//����ջ

	unordered_set<char>NonTerminal;			//���ս����
	unordered_set<char>Terminal;			//�ս����

	vector<pair<string, string>> Grammar_Rules;			//Grammar_Rules[i]=pair<starter,beginer> (starter->beginer)

	unordered_map<string, unordered_set<char>> First;			//First[string]= string��First��
	vector<unordered_map<char, int>> Goto;					//Goto[S][a]= target_status, S=cur_status, a=cur_symbol

	typedef enum { push_in, pop_out, acc }action;
	vector<unordered_map<char, pair<action, int>>> Action;	//Action[S][a] ��ȡ��Ҫ��ȡ���ж�


	vector<I_Element> I_element;						//������Ŀ
	int I_size;											//��Ŀ������
	vector<vector<I_Element>> I;						//��Ŀ��
	void init(const string& grammer_in);				//��ʼ��Goto��Action��Grammer_Rules��First��Terminal��NonTerminal
	void closure(vector<vector<I_Element>>& I);			//����Ŀ���հ�
	void go(vector<vector<I_Element>> I, char X);		//GO(I��X)��CLOSURE(J) J��{�κ�����A��aX��b����Ŀ| A��a��Xb����I}
	void get_First(const string& str);					//���ַ���str��First��
	//DEBUG��
	void show_NonTerminal();
	void show_Terminal();
	void show_Grammer_Rules();
};