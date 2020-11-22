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
	LEX();				//���캯��
	~LEX();								//��������
	Status analyse(const string& infile_name, const string& outfile_name);	//�����������

private:
	ifstream infile;					//�����ļ�
	ofstream outfile;					//����ļ�
	string file_str;							//�ļ�����ת�ɵ��ַ���
	int	idx;							//�ַ�����ָ��
	int id_code;						//��ʶ�����
	int state;					//״̬
	unordered_map<string, int> Stable;		//��ʶ����
	unordered_map<char, string> UnaryOP =
	{ {'+',"OP1"},{'-',"OP1"},{'*',"OP2"},{'/',"OP2"},{'<',"RELOP"},{'>',"RELOP"},{'=',"ASSIGN"},{';',"DEL"},{',',"SEP"},{'(',"LP"},{')',"RP"},{'{',"LB"},{'}',"RB"}, {'!',"RELOP"} };				//һԪ�����
	//{ '+','-','*','/','<','>','=',';',',','(',')','{','}', '!' };			
	unordered_map<string, string> ReserveWord =
	{ {"int","0"}, {"void","1"}, {"while","2"}, {"if","3"}, {"else","4"}, {"return","5"} };		//�����ֱ�
	//{ {"int",0}, {"void",1}, {"while",2}, {"if",3}, {"else",4}, {"return",5} }
	unordered_map<string, string> strOP =
	{ {"<=","0"},{"!=","1"},{"==","2"},{">=","3"} };			//�ַ��������
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
	}tokenType;				//�Ǻ�����*/

	/**************************��Ա����***************************/
	void read_file_to_str(string &str);//�ļ���ȡ���ַ���
	Status strPrint(const string& type, const string &value);				//���
	Status unaryPrint(const char& ch, const string& type);
	bool isUnaryOperator(char ch);					//�Ƿ�ΪһԪ�����(δ���Ƕ�Ԫ)
	bool isLiter(char ch);							//�Ƿ�Ϊ��ĸ���»���(_)
	unordered_map<string, string>::iterator isReserveWord(string id);					//�Ƿ�Ϊ������
	Status Delcomment();
	Status Number();
	Status Operator();
	Status Identifier();
	Status bufferScanner();
};