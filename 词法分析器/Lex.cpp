#include "Lex.h"

/***************************************************************************
  函数名称：void LEX::read_file_to_str(string &str)
  功    能：读取文件到字符串
  输入参数：string &str：返回的字符串的引用
  返 回 值：str
  说    明：读入文件内容，存入str字符串内
***************************************************************************/
void LEX::read_file_to_str(string &str)
{
	ostringstream tmp;
	tmp << infile.rdbuf();
	str = tmp.str();
	//测试用 std::cout << str;	
}


/***************************************************************************
  函数名称：LEX::LEX()
  功    能：构造函数
  输入参数：无
  返 回 值：无
  说    明：初始化
***************************************************************************/
LEX::LEX()
{
	idx = 0;
	state = 0;
}


/***************************************************************************
  函数名称：Status LEX::strPrint(const string& type,const string &tmp)
  功    能：输出字符串解释结果
  输入参数：const string& type：字符串标识的种类
			const string &value:字符串的属性值
  返 回 值：状态
  说    明：
***************************************************************************/
Status LEX::strPrint(const string& type,const string &value)
{
	string s = type;
	//全转大写
	transform(s.begin(), s.end(), s.begin(), ::toupper);
	if (s == "ID"&&Stable.count(value)==0) {
		Stable[value]=INT_MIN;
	}
	outfile << "<" << s << "," << value << ">" << endl;
	return OK;
}


/***************************************************************************
  函数名称：Status LEX::unaryPrint(const char& ch, const string& type,unordered_map<char,string>::iterator it)
  功    能：输出一元算符解释结果
  输入参数：
  返 回 值：
  说    明：
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
  函数名称：bool LEX::isUnaryOperator(char ch)
  功    能：判断字符是否为一元算符
  输入参数：ch：字符
  返 回 值：bool
  说    明：
***************************************************************************/
bool LEX::isUnaryOperator(char ch)
{
	if (UnaryOP.find(ch)!=UnaryOP.end()) {
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
	if (isalpha(ch)||ch=='_') {
		return true;
	}
	return false;
}


/***************************************************************************
  函数名称：unordered_map<string, string>::iterator LEX::isReserveWord(string id)
  功    能：判断字符是否为一元算符
  输入参数：
  返 回 值：
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
	//int begin = idx;//注释开始位置
	/*
	q0输入'/'到q1,接收状态
	q0输入'*'到q2，再输入'*'到q3，q3输入'/'到终态q4，输入其他到q2。
	*/
	idx++;//已经读到了一个/
	while (file_str[idx]) {
		switch (state)
		{
			case 0:
				if (file_str[idx] == '/') {//到q1
					state = 1;
				}
				else if (file_str[idx] == '*') {//到q2
					state = 2;
				}
				else {			//语法错误，返回状态值
					idx -= 2;
					return state;
				}
				idx++;
				break;
			case 1:
				return state;//返回接收状态
			case 2:
				if (file_str[idx] == '*') {			//多行注释/**/语法，开始寻找"*/"
					state = 3;
				}
				idx++;
				break;
			case 3:
				if (file_str[idx] == '/') {			//找到*/，注释结束
					state = 4;
				}
				else {
					state = 2;						//只有*无/，*无效，返回到q3
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
  函数名称：Status LEX::Number()
  功    能：读数字
  输入参数：无
  返 回 值：无
			
  说    明：读入一个数字
***************************************************************************/
Status LEX::Number()
{
	int begin = idx;//标记开始位置
	idx++;
	while (file_str[idx]) {
		if (isdigit(file_str[idx])) {
			idx++;
		}
		else {
			string num = file_str.substr(begin, idx - begin);
			idx--;//在bufferscanner中有前进，故需后退，确保不漏字符
			return strPrint("NUM", num);
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
		return 0;//符号出错
	}
	char ch = search->first;//只有第一次有用
	while (file_str[idx]) {
		//search = UnaryOP.find(file_str[idx]); 	
		//第二次是这些怎么办？
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
	idx++;//开头已经是字母或者下划线
	while (file_str[idx]) {
		if (isLiter(file_str[idx]) || isdigit(file_str[idx])) {//字母、下划线、数字
			idx++;
		}
		else {		//其他字符
			string temp = file_str.substr(begin, idx - begin);//提取出标识符
			unordered_map<string, string>::iterator it= isReserveWord(temp);

			if (it!=ReserveWord.end()) {		//是保留字
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
  函数名称：Status LEX::bufferScanner()
  功    能：分析字符串
  输入参数：无
  返 回 值：状态（OK，INPUT_ERROR，OUTPUT_ERROR）
  说    明：首先读取文件内容，
***************************************************************************/
Status LEX::bufferScanner()
{
	read_file_to_str(this->file_str);
	while (this->file_str[idx] && !state) {		//字符串未结束并且DFA是开始状态
		//跳过空格、tab
		//换行
		if (file_str[idx] == '\n') {
			
			strPrint("NL","-");
			state=0;//单次分析结束,可以写在if分支以外
		}
		//去除注释
		else if (file_str[idx] == '/' && (file_str[idx+1] == '/'|| file_str[idx] == '*')) {
			Delcomment();//这时的state能反应结束状态是否是接受态(1,4)
			state=0;
		}
		//数字
		else if (isdigit(file_str[idx])) {
			
			Number();
			state=0;
		}
		//一元运算符
		else if (isUnaryOperator(file_str[idx])) {

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
  功    能：词法分析器主函数，输出词法分析结果
  输入参数：file_name:		输入文件名, 
			outfile_name:	输出文件名
  返 回 值：状态（OK，INPUT_ERROR，OUTPUT_ERROR）
  说    明：打开输入输出文件后进行词法分析，最终将结果输出在文件内
***************************************************************************/
Status LEX::analyse(const string& infile_name, const string& outfile_name)
{
	//判断文件是否打开
	infile.open(infile_name, ios::in);
	if (infile.is_open() == 0)
		return INPUT_ERROR;

	outfile.open(outfile_name, ios::out);
	if (outfile.is_open() == 0)
	{
		infile.close();
		return OUTPUT_ERROR;
	}

	bufferScanner();//输入缓冲区

	/*输出标识符表*/
	for (auto search = Stable.begin(); search != Stable.end(); search++) {
		outfile << search->first<< endl;
	}
	infile.close();
	outfile.close();
	return OK;			//返回终态
}