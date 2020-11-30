#include"Parser.h"
#define DEBUG_MODE
PARSER::PARSER()
{}
void PARSER::show_Terminal()
{
	cout << "终结符为：";
	for (auto element : Terminal)
		cout << element << ' ';
	cout << endl;
}
void PARSER::show_NonTerminal()
{
	cout << "非终结符为：";
	for (auto element : NonTerminal)
		cout << element << ' ';
	cout << endl;
}
void PARSER::show_Grammer_Rules()
{
	cout << "推导式为：" << endl;
	for (auto element : Grammar_Rules)
		cout << element.first << ' ' << element.second << endl;
	cout << endl;
}
bool PARSER::LR1(const string& grammer_in, const string& file_in)
{
	init(grammer_in);
	return true;
}
//已完成：Terminal, NonTerminal,Grammar_Rules
void PARSER::init(const string& grammer_in)
{
	infile.open(grammer_in);
	if (!infile.is_open())
	{
		cout << "Failed to open the grammer file" << endl;
		return;
	}
	//读入终结符
	string line;
	getline(infile, line);
	stringstream ss(line);
	char ch;
	while (ss >> ch)
		Terminal.insert(ch);

	//读入推导式，读入非终结符
	while (!infile.eof())
	{
		getline(infile, line);
		int cur_pos = line.find('>');
		string left_part = line.substr(0, cur_pos);
		string right_part = line.substr(cur_pos + 1);
		Grammar_Rules.push_back(make_pair(left_part, right_part));
		NonTerminal.insert(left_part[0]);
	}

	//根据推导式求First集
	for (auto ch : Terminal) {			//终结符
		string Char_To_String;
		Char_To_String.push_back(ch);
		First[Char_To_String].insert(ch);
	}

	for (auto str : NonTerminal) {		//非终结符
		string Char_To_String;
		Char_To_String.push_back(str);
		for (auto Rules : Grammar_Rules) {
			if (Rules.first == Char_To_String) {
				bool Exist_Null = true;			//判断所求First集是否含空
				for (auto Right_Element : Rules.second) {
					if (Terminal.find(Right_Element) != Terminal.end()) {//右边的符号Right_Element是终结符
						First[Char_To_String].insert(Right_Element);	//添加到First集
						Exist_Null = false;
						break;
					}
					else {													//右边的符号Right_Element是非终结符
						string Element_To_String;
						Element_To_String.push_back(Right_Element);
						//该非终结符的First集添加到产生式左部符号的First集
						First[Char_To_String].insert(First[Element_To_String].begin(), First[Element_To_String].end());
						if (First[Element_To_String].find('$') != First[Element_To_String].end()) {//该非终结符含空,需要在产生式左部符号的First集删掉空
							First[Char_To_String].erase('$');
						}
						else {
							Exist_Null = false;
							break;
						}
					}
				}
				if (Exist_Null == true) {
					First[Char_To_String].insert('$');
				}
			}
		}
	}

#ifdef DEBUG_MODE
	show_Terminal();
	show_NonTerminal();
	show_Grammer_Rules();
#endif

}

void PARSER::get_First(const string& str)			//求字符串str的First集
{
	//bool Exist_NULL = true;//判断非终结符的First集是否含空
	for (auto element : str) {
		if (NonTerminal.find(element) != NonTerminal.end()) {			//当前字符为终结符	
			First[str].insert(element);
			return;
		}
		else if (Terminal.find(element) != Terminal.end()) {			//当前字符为非终结符
			string Element_To_String;
			Element_To_String.push_back(element);
			First[str].insert(First[Element_To_String].begin(), First[Element_To_String].end());
			if (First[Element_To_String].find('$') != First[Element_To_String].end()) {
				First[str].erase('$');
			}
			else {
				//Exist_NULL = false;
				return;
			}
		}
		else {
			return;//出错处理，当前不考虑(如果不考虑出错，可以把上面两个分支合并)。
		}
	}
}

//这个函数到底是同时进行闭包和GO的计算还是单纯只计算本项目的闭包存在疑惑。(目前包括GO)
void PARSER::closure(vector<vector<I_Element>>& I)
{
	//初始的闭包I0
	I_Element I0;
	I0.num = 0;
	I0.left_part = "S'";
	I0.right_part = "S";
	I0.forward = '#';
	I[0].push_back(I0);
	int i = 0;//初始I0
	while (1) {
		//i,j,k可以改名，待修改
		//计算I[i]的闭包
		for (int j = 0; j < I[i].size(); j++) {//遍历I[i]中所有的项目
			int k = I[i][j].num + 1;
			if (k < I[i][j].right_part.size()) {//非终止，分析I[i][j].right_part[k]
				for (auto Rules : Grammar_Rules) {
					string Char_To_String;
					Char_To_String.push_back(I[i][j].right_part[k]);
					string forward_str;						//存k之后的子串和I[i][j].forward的连接
					if (Rules.first == Char_To_String) {			//判断是否是产生式左部
						//B->beta,First(forward_str)判断是否重复
						//非重复则加入
					}
				}
			}
			else {
				break;				//不会产生新的项目
			}
		}

		//通过go计算新的闭包

	}
	
}
void PARSER::go(vector<vector<I_Element>> I, char X)
{

}