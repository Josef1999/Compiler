#include "Lex.h"

/***************************************************************************
  �������ƣ�void LEX::read_file_to_str(string &str)
  ��    �ܣ���ȡ�ļ����ַ���
  ���������
			string &str�����ص��ַ���������
			const string &file_name�������ļ���
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void LEX::read_file_to_str(string& str)
{
	ostringstream tmp;
	tmp << infile.rdbuf();
	str = tmp.str();
}


/***************************************************************************
  �������ƣ�LEX::LEX()
  ��    �ܣ����캯��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
LEX::LEX()
{
	idx = 0;				//�ļ��ַ���ָ��
	state = 0;				//DFA��ʼ״̬�����������ַ���
	id_code = 0;			//��ʶ��(ID)�ı���
}

/***************************************************************************
  �������ƣ�LEX::~LEX()
  ��    �ܣ���������
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
LEX::~LEX()
{
	;
}


/***************************************************************************
  �������ƣ�Status LEX::strPrint(const string& type,const string &value)
  ��    �ܣ�����ַ������ͽ��
  ���������
			const string& type���ַ�����ʶ������
			const string &value:�ַ���������ֵ
  �� �� ֵ��
  ˵    ����
***************************************************************************/
Status LEX::strPrint(const string& type, const string& value)
{
	string s = type;											//��ʱ�����洢type��������typeת���ɴ�д��ʽ
	transform(s.begin(), s.end(), s.begin(), ::toupper);		//ת������
	if (s == "ID") {											//type�Ǳ�ʶ��������Ѵ�����Stable����������ţ�
		if (Stable.count(value) == 0) {							//������������Stable�У�id_code+1
			Stable[value] = id_code;
			id_code++;
		}
		outfile << "<" << s << "," << Stable[value] << ">" << endl;
		return OK;
	}
	else {
		outfile << "<" << s << "," << value << ">" << endl;
	}
	return OK;
}


/***************************************************************************
  �������ƣ�Status LEX::unaryPrint(const char& ch, const string& type)
  ��    �ܣ����һԪ������ͽ��
  ���������
			const char& ch:һԪ������ַ�
			const string& type�����ַ�����
  �� �� ֵ��
  ˵    ����
***************************************************************************/
Status LEX::unaryPrint(const char& ch, const string& type)
{
	char unary_code;	//һԪ������ı������
	switch (ch)
	{
	case '+':
		unary_code = '0';
		break;
	case '-':
		unary_code = '1';
		break;
	case '*':
		unary_code = '0';
		break;
	case '/':
		unary_code = '1';
		break;
	case '<':
		unary_code = '4';
		break;
	case '>':
		unary_code = '5';
		break;
	default:
		unary_code = '-';
		break;
	}
	outfile << "<" << type << "," << unary_code << ">" << endl;
	return OK;
}

/***************************************************************************
  �������ƣ�bool LEX::isUnaryOperator(char ch)
  ��    �ܣ��ж��ַ��Ƿ�ΪһԪ���
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
bool LEX::isUnaryOperator(char ch)
{
	if (UnaryOP.find(ch) != UnaryOP.end()) {
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
	if (isalpha(ch) || ch == '_') {
		return true;
	}
	return false;
}


/***************************************************************************
  �������ƣ�unordered_map<string, string>::iterator LEX::isReserveWord(string id)
  ��    �ܣ��ж��ַ��Ƿ�Ϊ������
  ���������
			string id:��ʶ������
  �� �� ֵ��
			�ɹ���ReserveWord�����ҵ�ʱ������unordered_map���͵�iterator��ָ���ʶ��id��
			ʧ�ܷ���ReserveWord.end();

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
	if (file_str[idx + 1] == '/')
	{
		while (file_str[idx] != '\n')
			idx++;
		idx--;
		return OK;
	}
	else
	{
		int idx_ = idx;
		while (!(file_str[idx_] == '*' && file_str[idx_ + 1] == '/'))
		{
			idx_++;
			if (idx_ + 1 > file_str.length())
				return ERROR;
		}
		idx = idx_ + 1;
		return OK;
	}

}

/***************************************************************************
  �������ƣ�Status LEX::Number()
  ��    �ܣ�������
  ���������
  �� �� ֵ��

  ˵    ����
***************************************************************************/
Status LEX::Number()
{
	int begin = idx;//ע�Ϳ�ʼλ��
	while (idx < file_str.size()) {
		if (isdigit(file_str[idx])) {
			idx++;
		}
		else {
			string tmp = file_str.substr(begin, idx - begin);
			idx--;					//���������ֺ���
			return strPrint("NUM", tmp);
		}
		if (idx >= file_str.size()) {
			string tmp = file_str.substr(begin, idx - begin);
			return strPrint("NUM", tmp);
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
		return 0;					//���ų���
	}
	char ch = search->first;		//ֻ�е�һ�����ã���������+-*/;,(){}��Щ����
	while (idx < file_str.size()) {	
		if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == ';' || ch == ','
			|| ch == '(' || ch == ')' || ch == '{' || ch == '}') {
			//state = 1;
			return unaryPrint(search->first, search->second);
		}
		else {
			ch = file_str[idx];					//���������ţ��ڶ������
			switch (state)
			{
			case 0:
				switch (ch)
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
					return strPrint("RELOP", "0");
				}
				else {		//<
					idx--;
					return unaryPrint('<', "RELOP");
				}
				break;
			case 4:
				if (ch == '=') {//>=
					state = 5;
					return strPrint("RELOP", "3");
				}
				else {				//>
					idx--;
					return unaryPrint('>', "RELOP");
				}
				break;
			case 6:
				if (ch == '=') {	//==
					state = 7;
					return strPrint("RELOP", "2");
				}
				else {			//=
					idx--;
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
		if (idx == file_str.size()) {
			if (state == 2) {
				return unaryPrint('<', "RELOP");
			}
			else if (state == 4) {
				return unaryPrint('>', "RELOP");
			}
			else if (state == 6) {
				return unaryPrint('=', "ASSIGN");
			}
		}
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
	while (idx < file_str.size()) {
		if (isLiter(file_str[idx]) || isdigit(file_str[idx])) {//��ĸ���»��ߡ�����
			idx++;
		}
		else {		//�����ַ�
			temp = file_str.substr(begin, idx - begin);//��ȡ����ʶ��
			unordered_map<string, string>::iterator it = isReserveWord(temp);
			idx--;
			if (it != ReserveWord.end()) {		//�Ǳ�����
				return strPrint(it->first, it->second);
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
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
Status LEX::bufferScanner()
{
	read_file_to_str(file_str);
	while (idx < file_str.size() && !state) {		//�ַ���δ��������DFA�ǿ�ʼ״̬
		if (file_str[idx] == ' ' || file_str[idx] == '\t' || file_str[idx] == '\r') {
			;//�ո�TAB���س�����������������
		}
		else if (file_str[idx] == '\n') {
			//����
			strPrint("NL", "-");
			state = 0;//���η�������,����д��if��֧����
		}
		else if (file_str[idx] == '/' && (file_str[idx + 1] == '/' || file_str[idx + 1] == '*')) {

			Delcomment();//��ʱ��state�ܷ�Ӧ����״̬�Ƿ��ǽ���̬(1,4)
			state = 0;
		}
		else if (isdigit(file_str[idx])) {
			//����
			Number();
			state = 0;
		}
		else if (isUnaryOperator(file_str[idx])) {
			//һԪ�����
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
  ��    �ܣ������ַ���
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
Status LEX::analyse(const string& infile_name, const string& outfile_name)
{
	//�ж��ļ��Ƿ��
	infile.open(infile_name, ios::in);
	outfile.open(outfile_name, ios::out);
	if (infile.is_open() == 0) {
		cout << "Դ�����ļ���ʧ��" << endl;
		exit(-1);
	}
	if (outfile.is_open() == 0) {
		infile.close();
		cout << "�ʷ���������ļ���ʧ��" << endl;
		exit(-1);
	}
	bufferScanner();//���뻺����
	/*�����*/

	/*outfile << endl;
	for (auto search = Stable.begin(); search != Stable.end(); search++) {
		outfile << "<" << search->first << "," << search->second << ">" << endl;
	}*/
	infile.close();
	outfile.close();
	return OK;			//������̬
}