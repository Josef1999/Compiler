#include "Lex.h"

/***************************************************************************
  �������ƣ�void LEX::read_file_to_str(string &str)
  ��    �ܣ���ȡ�ļ����ַ���
  ���������string &str�����ص��ַ���������
  �� �� ֵ��str
  ˵    ���������ļ����ݣ�����str�ַ�����
***************************************************************************/
void LEX::read_file_to_str(string &str)
{
	ostringstream tmp;
	tmp << infile.rdbuf();
	str = tmp.str();
	//������ std::cout << str;	
}


/***************************************************************************
  �������ƣ�LEX::LEX()
  ��    �ܣ����캯��
  �����������
  �� �� ֵ����
  ˵    ������ʼ��
***************************************************************************/
LEX::LEX()
{
	idx = 0;
	state = 0;
}


/***************************************************************************
  �������ƣ�Status LEX::strPrint(const string& type,const string &tmp)
  ��    �ܣ�����ַ������ͽ��
  ���������const string& type���ַ�����ʶ������
			const string &value:�ַ���������ֵ
  �� �� ֵ��״̬
  ˵    ����
***************************************************************************/
Status LEX::strPrint(const string& type,const string &value)
{
	string s = type;
	//ȫת��д
	transform(s.begin(), s.end(), s.begin(), ::toupper);
	if (s == "ID"&&Stable.count(value)==0) {
		Stable[value]=INT_MIN;
	}
	outfile << "<" << s << "," << value << ">" << endl;
	return OK;
}


/***************************************************************************
  �������ƣ�Status LEX::unaryPrint(const char& ch, const string& type,unordered_map<char,string>::iterator it)
  ��    �ܣ����һԪ������ͽ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
Status LEX::unaryPrint(const char& ch, const string& type)
{	char unary;
	switch(ch)
	{
		case '+':
			unary = '0';
			break;
		case '-':
			unary = '1';
			break;
		case '*':
			unary = '0';
			break;
		case '/':
			unary = '1';
			break;
		case '<':
			unary = '0';
			break;
		case '>':
			unary = '4';
			break;
		default:
			unary = '-';
			break;
	}
	outfile << "<" << type << "," << unary << ">" << endl;
	return OK;
}

/***************************************************************************
  �������ƣ�bool LEX::isUnaryOperator(char ch)
  ��    �ܣ��ж��ַ��Ƿ�ΪһԪ���
  ���������ch���ַ�
  �� �� ֵ��bool
  ˵    ����
***************************************************************************/
bool LEX::isUnaryOperator(char ch)
{
	if (UnaryOP.find(ch)!=UnaryOP.end()) {
		return true;
	}
	return false;
}

/***************************************************************************
  �������ƣ�bool LEX::isLiter(char ch)
  ��    �ܣ��ж��ַ��Ƿ�Ϊ��ĸ�����»���(_)
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
bool LEX::isLiter(char ch)
{
	if (isalpha(ch)||ch=='_') {
		return true;
	}
	return false;
}


/***************************************************************************
  �������ƣ�unordered_map<string, string>::iterator LEX::isReserveWord(string id)
  ��    �ܣ��ж��ַ��Ƿ�ΪһԪ���
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
unordered_map<string, string>::iterator LEX::isReserveWord(string id)
{
	if (ReserveWord.find(id) != ReserveWord.end()) {
		return ReserveWord.find(id);
	}
	return ReserveWord.end();
}


/***************************************************************************
  �������ƣ�Status LEX::Delcomment()
  ��    �ܣ�ɾ��ע��
  ���������
  �� �� ֵ��
			����״̬1����״̬4˵���ɹ�������̬
  ˵    ����
***************************************************************************/
Status LEX::Delcomment()
{
	//int begin = idx;//ע�Ϳ�ʼλ��
	/*
	q0����'/'��q1,����״̬
	q0����'*'��q2��������'*'��q3��q3����'/'����̬q4������������q2��
	*/
	idx++;//�Ѿ�������һ��/
	while (file_str[idx]) {
		switch (state)
		{
			case 0:
				if (file_str[idx] == '/') {//��q1
					state = 1;
				}
				else if (file_str[idx] == '*') {//��q2
					state = 2;
				}
				else {			//�﷨���󣬷���״ֵ̬
					idx -= 2;
					return state;
				}
				idx++;
				break;
			case 1:
				return state;//���ؽ���״̬
			case 2:
				if (file_str[idx] == '*') {			//����ע��/**/�﷨����ʼѰ��"*/"
					state = 3;
				}
				idx++;
				break;
			case 3:
				if (file_str[idx] == '/') {			//�ҵ�*/��ע�ͽ���
					state = 4;
				}
				else {
					state = 2;						//ֻ��*��/��*��Ч�����ص�q3
				}
				idx++;
			case 4:
				return state;
			default:
				break;
		}
	}
	return state;
}

/***************************************************************************
  �������ƣ�Status LEX::Number()
  ��    �ܣ�������
  �����������
  �� �� ֵ����
			
  ˵    ��������һ������
***************************************************************************/
Status LEX::Number()
{
	int begin = idx;//��ǿ�ʼλ��
	idx++;
	while (file_str[idx]) {
		if (isdigit(file_str[idx])) {
			idx++;
		}
		else {
			string num = file_str.substr(begin, idx - begin);
			idx--;//��bufferscanner����ǰ����������ˣ�ȷ����©�ַ�
			return strPrint("NUM", num);
		}
	}
	return 0;
}

/***************************************************************************
  �������ƣ�Status LEX::Operator()
  ��    �ܣ���������
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
Status LEX::Operator()
{
	auto search = UnaryOP.find(file_str[idx]);
	if (search == UnaryOP.end()) {
		return 0;//���ų���
	}
	char ch = search->first;//ֻ�е�һ������
	while (file_str[idx]) {
		//search = UnaryOP.find(file_str[idx]); 	
		//�ڶ�������Щ��ô�죿
		if (ch == '+' || ch == '-'||ch=='*'||ch=='/'||ch==';' || ch == ',' 
			|| ch == '(' || ch ==')' || ch == '{' || ch == '}') {
			//state = 1;
			return unaryPrint(search->first, search->second);
		}
		else {
			ch = file_str[idx];
			int begin = idx;
			switch(state)
			{
				case 0:
					switch(ch)
					{
						case '<':
							state = 2;
							break;
						case '>':
							state = 4;
							break;
						case '=':
							state = 6;
							break;
						case '!':
							state = 8;
							break;
						default:
							break;
					}
					break;
				case 2:
					if (ch == '=') {//<=
						state = 3;
						return strPrint("RELOP","0");
					}
					else {		//<
						return unaryPrint('<',"RELOP");
					}
					break;
				case 4:
					if (ch == '=') {//>=
						state = 5;
						return strPrint("RELOP", "3");
					}
					else {//>
						return unaryPrint('>', "RELOP");
					}
					break;
				case 6:
					if (ch == '='){	//==
						state = 7;
						return strPrint("RELOP", "2");
					}
					else {
						return unaryPrint('=', "ASSIGN");
					}
					break;
				case 8:
					if (ch == '=') {	//!=
						state = 9;
						return strPrint("RELOP", "1");
					}
					break;
				default:
					break;
			}
		}
		idx++;
	}
	return OK;
}

/***************************************************************************
  �������ƣ�Status LEX::Identifier()
  ��    �ܣ���������
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
Status LEX::Identifier()
{
	int begin = idx;//��ʶ����ͷ
	string temp;//��¼��ʶ��
	idx++;//��ͷ�Ѿ�����ĸ�����»���
	while (file_str[idx]) {
		if (isLiter(file_str[idx]) || isdigit(file_str[idx])) {//��ĸ���»��ߡ�����
			idx++;
		}
		else {		//�����ַ�
			string temp = file_str.substr(begin, idx - begin);//��ȡ����ʶ��
			unordered_map<string, string>::iterator it= isReserveWord(temp);

			if (it!=ReserveWord.end()) {		//�Ǳ�����
				return strPrint(it->first,it->second);
			}
			else {
				return strPrint("ID", temp);
			}
		}
	}
	return strPrint("ID", temp);
}


/***************************************************************************
  �������ƣ�Status LEX::bufferScanner()
  ��    �ܣ������ַ���
  �����������
  �� �� ֵ��״̬��OK��INPUT_ERROR��OUTPUT_ERROR��
  ˵    �������ȶ�ȡ�ļ����ݣ�
***************************************************************************/
Status LEX::bufferScanner()
{
	read_file_to_str(this->file_str);
	while (this->file_str[idx] && !state) {		//�ַ���δ��������DFA�ǿ�ʼ״̬
		//�����ո�tab
		//����
		if (file_str[idx] == '\n') {
			
			strPrint("NL","-");
			state=0;//���η�������,����д��if��֧����
		}
		//ȥ��ע��
		else if (file_str[idx] == '/' && (file_str[idx+1] == '/'|| file_str[idx] == '*')) {
			Delcomment();//��ʱ��state�ܷ�Ӧ����״̬�Ƿ��ǽ���̬(1,4)
			state=0;
		}
		//����
		else if (isdigit(file_str[idx])) {
			
			Number();
			state=0;
		}
		//һԪ�����
		else if (isUnaryOperator(file_str[idx])) {

			Operator();
			state = 0;
		}
		else if (isLiter(file_str[idx])) {
			//��ʶ��
			Identifier();
			state = 0;
		}
		else {
			;		//δ֪�����������
		}
		idx++;
	}
	return state;
}



/***************************************************************************
  �������ƣ�Status LEX::analyse(const string file_name,const string outfile_name)
  ��    �ܣ��ʷ�������������������ʷ��������
  ���������file_name:		�����ļ���, 
			outfile_name:	����ļ���
  �� �� ֵ��״̬��OK��INPUT_ERROR��OUTPUT_ERROR��
  ˵    ��������������ļ�����дʷ����������ս����������ļ���
***************************************************************************/
Status LEX::analyse(const string& infile_name, const string& outfile_name)
{
	//�ж��ļ��Ƿ��
	infile.open(infile_name, ios::in);
	if (infile.is_open() == 0)
		return INPUT_ERROR;

	outfile.open(outfile_name, ios::out);
	if (outfile.is_open() == 0)
	{
		infile.close();
		return OUTPUT_ERROR;
	}

	bufferScanner();//���뻺����

	/*�����ʶ����*/
	for (auto search = Stable.begin(); search != Stable.end(); search++) {
		outfile << search->first<< endl;
	}
	infile.close();
	outfile.close();
	return OK;			//������̬
}