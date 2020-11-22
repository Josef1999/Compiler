#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <unordered_map>
#include <algorithm>
#define OK 1
using namespace std;
typedef int Status;
class LEX
{
public:
	LEX();				//构造函数
	~LEX();								//析构函数
	Status analyse(const string& infile_name, const string& outfile_name);	//整体分析函数

private:
	ifstream infile;					//输入文件
	ofstream outfile;					//输出文件
	string file_str;							//文件内容转成的字符串
	int	idx;							//字符串的指针
	int id_code;						//标识符编号
	int state;					//状态
	unordered_map<string, int> Stable;		//标识符表
	unordered_map<char, string> UnaryOP =
	{ {'+',"OP1"},{'-',"OP1"},{'*',"OP2"},{'/',"OP2"},{'<',"RELOP"},{'>',"RELOP"},{'=',"ASSIGN"},{';',"DEL"},{',',"SEP"},{'(',"LP"},{')',"RP"},{'{',"LB"},{'}',"RB"}, {'!',"RELOP"} };				//一元算符表
	//{ '+','-','*','/','<','>','=',';',',','(',')','{','}', '!' };			
	unordered_map<string, string> ReserveWord =
	{ {"int","0"}, {"void","1"}, {"while","2"}, {"if","3"}, {"else","4"}, {"return","5"} };		//保留字表
	//{ {"int",0}, {"void",1}, {"while",2}, {"if",3}, {"else",4}, {"return",5} }
	unordered_map<string, string> strOP =
	{ {"<=","0"},{"!=","1"},{"==","2"},{">=","3"} };			//字符运算符表
/*	typedef enum {
		INT,								//0
		VOID,								//1
		ID,									//2
		LP,									//3
		LP,									//4
		RP,									//5
		LB,									//6
		WHILE,								//7
		IF,									//8
		ELSE,								//9
		RETURN,								//10
		ASSIGN,								//11
		OP1,								//12
		OP2,								//13
		RELOP,								//14
		DEL,								//15
		SEP,								//16
		NUM,								//17
		NL									//18
	}tokenType;				//记号类型*/

	/**************************成员函数***************************/
	void read_file_to_str(string &str);//文件读取到字符串
	Status strPrint(const string& type, const string &value);				//输出
	Status unaryPrint(const char& ch, const string& type);
	bool isUnaryOperator(char ch);					//是否为一元运算符(未考虑多元)
	bool isLiter(char ch);							//是否为字母或下划线(_)
	unordered_map<string, string>::iterator isReserveWord(string id);					//是否为保留字
	Status Delcomment();
	Status Number();
	Status Operator();
	Status Identifier();
	Status bufferScanner();
};