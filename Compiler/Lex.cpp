#include "Lex.h"

/***************************************************************************
  函数名称：void LEX::read_file_to_str(string &str)
  功    能：读取文件到字符串
  输入参数：
			string &str：返回的字符串的引用
			const string &file_name：读入文件名
  返 回 值：
  说    明：
***************************************************************************/
void LEX::read_file_to_str(string& str)
{
	ostringstream tmp;
	tmp << infile.rdbuf();
	str = tmp.str();
}


/***************************************************************************
  函数名称：LEX::LEX()
  功    能：构造函数
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
LEX::LEX()
{
	idx = 0;				//文件字符串指针
	state = 0;				//DFA初始状态，用来分析字符串
	id_code = 0;			//标识符(ID)的编码
}

/***************************************************************************
  函数名称：LEX::~LEX()
  功    能：析构函数
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
LEX::~LEX()
{
	;
}


/***************************************************************************
  函数名称：Status LEX::strPrint(const string& type,const string &value)
  功    能：输出字符串解释结果
  输入参数：
			const string& type：字符串标识的种类
			const string &value:字符串的属性值
  返 回 值：
  说    明：
***************************************************************************/
Status LEX::strPrint(const string& type, const string& value)
{
	string s = type;											//临时变量存储type，用来把type转换成大写形式
	transform(s.begin(), s.end(), s.begin(), ::toupper);		//转换函数
	if (s == "ID") {											//type是标识符，如果已存在于Stable中则输出其编号，
		if (Stable.count(value) == 0) {							//如果不在则加入Stable中，id_code+1
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
  函数名称：Status LEX::unaryPrint(const char& ch, const string& type)
  功    能：输出一元算符解释结果
  输入参数：
			const char& ch:一元运算的字符
			const string& type：该字符类型
  返 回 值：
  说    明：
***************************************************************************/
Status LEX::unaryPrint(const char& ch, const string& type)
{
	char unary_code;	//一元运算符的编码符号
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
  函数名称：bool LEX::isUnaryOperator(char ch)
  功    能：判断字符是否为一元算符
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
bool LEX::isUnaryOperator(char ch)
{
	if (UnaryOP.find(ch) != UnaryOP.end()) {
		return true;
	}
	return false;
}

/***************************************************************************
  函数名称：bool LEX::isLiter(char ch)
  功    能：判断字符是否为字母或者下划线(_)
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
bool LEX::isLiter(char ch)
{
	if (isalpha(ch) || ch == '_') {
		return true;
	}
	return false;
}


/***************************************************************************
  函数名称：unordered_map<string, string>::iterator LEX::isReserveWord(string id)
  功    能：判断字符是否为保留字
  输入参数：
			string id:标识符名称
  返 回 值：
			成功在ReserveWord表中找到时，返回unordered_map类型的iterator，指向标识符id，
			失败返回ReserveWord.end();

  说    明：
***************************************************************************/
unordered_map<string, string>::iterator LEX::isReserveWord(string id)
{
	if (ReserveWord.find(id) != ReserveWord.end()) {
		return ReserveWord.find(id);
	}
	return ReserveWord.end();
}


/***************************************************************************
  函数名称：Status LEX::Delcomment()
  功    能：删除注释
  输入参数：
  返 回 值：
			返回状态1或者状态4说明成功到达终态
  说    明：
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
  函数名称：Status LEX::Number()
  功    能：读数字
  输入参数：
  返 回 值：

  说    明：
***************************************************************************/
Status LEX::Number()
{
	int begin = idx;//注释开始位置
	while (idx < file_str.size()) {
		if (isdigit(file_str[idx])) {
			idx++;
		}
		else {
			string tmp = file_str.substr(begin, idx - begin);
			idx--;					//读到非数字后退
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
  函数名称：Status LEX::Operator()
  功    能：读操作符
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
Status LEX::Operator()
{
	auto search = UnaryOP.find(file_str[idx]);
	if (search == UnaryOP.end()) {
		return 0;					//符号出错
	}
	char ch = search->first;		//只有第一次有用，用来处理+-*/;,(){}这些符号
	while (idx < file_str.size()) {	
		if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == ';' || ch == ','
			|| ch == '(' || ch == ')' || ch == '{' || ch == '}') {
			//state = 1;
			return unaryPrint(search->first, search->second);
		}
		else {
			ch = file_str[idx];					//非上述符号，第二种情况
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
  函数名称：Status LEX::Identifier()
  功    能：读操作符
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
Status LEX::Identifier()
{
	int begin = idx;//标识符开头
	string temp;//记录标识符
	while (idx < file_str.size()) {
		if (isLiter(file_str[idx]) || isdigit(file_str[idx])) {//字母、下划线、数字
			idx++;
		}
		else {		//其他字符
			temp = file_str.substr(begin, idx - begin);//提取出标识符
			unordered_map<string, string>::iterator it = isReserveWord(temp);
			idx--;
			if (it != ReserveWord.end()) {		//是保留字
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
  函数名称：Status LEX::bufferScanner()
  功    能：分析字符串
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
Status LEX::bufferScanner()
{
	read_file_to_str(file_str);
	while (idx < file_str.size() && !state) {		//字符串未结束并且DFA是开始状态
		if (file_str[idx] == ' ' || file_str[idx] == '\t' || file_str[idx] == '\r') {
			;//空格、TAB、回车，不做处理跳过。
		}
		else if (file_str[idx] == '\n') {
			//换行
			strPrint("NL", "-");
			state = 0;//单次分析结束,可以写在if分支以外
		}
		else if (file_str[idx] == '/' && (file_str[idx + 1] == '/' || file_str[idx + 1] == '*')) {

			Delcomment();//这时的state能反应结束状态是否是接受态(1,4)
			state = 0;
		}
		else if (isdigit(file_str[idx])) {
			//数字
			Number();
			state = 0;
		}
		else if (isUnaryOperator(file_str[idx])) {
			//一元运算符
			Operator();
			state = 0;
		}
		else if (isLiter(file_str[idx])) {
			//标识符
			Identifier();
			state = 0;
		}
		else {
			;		//未知情况，待补充
		}
		idx++;
	}
	return state;
}



/***************************************************************************
  函数名称：Status LEX::analyse(const string file_name,const string outfile_name)
  功    能：分析字符串
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
Status LEX::analyse(const string& infile_name, const string& outfile_name)
{
	//判断文件是否打开
	infile.open(infile_name, ios::in);
	outfile.open(outfile_name, ios::out);
	if (infile.is_open() == 0) {
		cout << "源代码文件打开失败" << endl;
		exit(-1);
	}
	if (outfile.is_open() == 0) {
		infile.close();
		cout << "词法分析结果文件打开失败" << endl;
		exit(-1);
	}
	bufferScanner();//输入缓冲区
	/*输出表*/

	/*outfile << endl;
	for (auto search = Stable.begin(); search != Stable.end(); search++) {
		outfile << "<" << search->first << "," << search->second << ">" << endl;
	}*/
	infile.close();
	outfile.close();
	return OK;			//返回终态
}