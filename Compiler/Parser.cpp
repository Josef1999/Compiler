#include"Parser.h"
#define DEBUG_MODE
PARSER::PARSER()
{
	I_size = 0;
	I = vector<vector<I_Element>>(1);
}
void PARSER::show_Terminal()
{
	cout << "终结符为：" << endl;
	for (auto element : Terminal)
		cout << element << ' ';
	cout << endl;
}
void PARSER::show_NonTerminal()
{
	cout << "非终结符为：" << endl;
	for (auto element : NonTerminal)
		cout << element << ' ';
	cout << endl;
}
void PARSER::show_Grammer_Rules()
{
	cout << "推导式为：" << endl;
	for (auto element : Grammar_Rules)
		cout << element.first << "->" << element.second << endl;
	cout << endl;
}
void PARSER::show_First()
{
	cout << "终结符First集为：" << endl;
	for (auto ch : Terminal) {
		string Char_To_String;
		Char_To_String.push_back(ch);
		cout << "First[" << ch << "]: ";
		for (auto First_item : First[Char_To_String]) {
			cout << First_item << " ";
		}
		cout << endl;
	}

	cout << "非终结符First集为：" << endl;
	for (auto str : NonTerminal) {
		string Char_To_String;
		Char_To_String.push_back(str);
		cout << "First[" << str << "]: ";
		for (auto First_item : First[Char_To_String]) {
			cout << First_item << " ";
		}
		cout << endl;
	}
	cout << endl;
}
void PARSER::show_closure()
{
	cout << "闭包为：" << endl;
	int count = 0;
	for (auto Item_Set : I) {
		cout << "I[" << count++ << "]:" << endl;
		for (auto Item : Item_Set) {
			cout << Item.left_part << "->";
			for (int i = 0; i <= Item.right_part.size(); i++) {
				if (i == Item.num + 1) {
					cout << ".";
				}
				if (i == Item.right_part.size())
					break;
				cout << Item.right_part[i];
			}
			cout << "," << Item.forward << endl;
		}
	}

	cout << endl;
}
void PARSER::show_this_closure()
{
	cout << "I[" << I_size-1 << "]:" << endl;
	for (auto Item : I[I_size-1]) {
		cout << Item.left_part << "->";
		for (int i = 0; i <= Item.right_part.size(); i++) {
			if (i == Item.num + 1) {
				cout << ".";
			}
			if (i == Item.right_part.size())
				break;
			cout << Item.right_part[i];
		}
		cout << "," << Item.forward << endl;
	}


	cout << endl;
}
bool PARSER::LR1(const string& grammer_in, const string& file_in)
{
	init(grammer_in);
	return true;
}
void PARSER::init(const string& grammer_in)
{
	infile.open(grammer_in);
	if (!infile.is_open())
	{
		cout << "Failed to open the grammer file" << endl;
		return;
	}
	init_Terminal();
	init_NonTerminal();
	infile.close();

	init_First();
	//求项目集闭包
	init_closure();

#ifdef DEBUG_MODE
	show_closure();
#endif

}
void PARSER::init_Terminal()
{
	//读入终结符
	string line;
	getline(infile, line);
	stringstream ss(line);
	char ch;
	while (ss >> ch)
		Terminal.insert(ch);
}
void PARSER::init_NonTerminal()
{
	//读入推导式，读入非终结符
	string line;
	while (!infile.eof())
	{
		getline(infile, line);
		int cur_pos = line.find('>');
		string left_part = line.substr(0, cur_pos);
		string right_part = line.substr(cur_pos + 1);
		Grammar_Rules.push_back(make_pair(left_part, right_part));
		NonTerminal.insert(left_part[0]);
	}
}
void PARSER::init_First()
{

	for (auto ch : Terminal) {			//终结符
		string Char_To_String;
		Char_To_String.push_back(ch);
		First[Char_To_String].insert(ch);
	}

	unordered_map<string, unordered_set<char>> pre_First;
EXTEND_FIRST:
	pre_First = First;
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

	if (pre_First.size() != First.size())
		goto EXTEND_FIRST;
	else
	{
		auto element = First.begin();
		for (auto pre_element : pre_First)
		{
			if (pre_element.second.size() != element->second.size())
				goto EXTEND_FIRST;
			element++;
		}
	}
	return;
}

//这个函数到底是同时进行闭包和GO的计算还是单纯只计算本项目的闭包存在疑惑。(目前包括GO)
void PARSER::init_closure()
{
	//求初始闭包I0

	I[0].push_back(I_Element("Z", "S"));

	//int count = 0;//调试用

	for (int j = 0; j < I[0].size(); j++) {//遍历I[i]中所有的项目
		int k = I[0][j].num + 1;
		if (k < I[0][j].right_part.size()) {//非终止，分析I[i][j].right_part[k]
			for (auto Rules : Grammar_Rules) {
				string Char_To_String;
				Char_To_String.push_back(I[0][j].right_part[k]);
				if (Rules.first == Char_To_String) {			//判断是否是产生式左部
					//B->beta,First(forward_str)判断是否重复，目前先不做
					//非重复则加入

					string forward_str;						//存k之后的子串和I[i][j].forward的连接
					forward_str = I[0][j].right_part.substr(k + 1) + I[0][j].forward;
					get_First(forward_str);									//求First集作为新项目集的展望

					I_Element new_Element(Rules.first, Rules.second);
					for (auto First_In_Forward : First[forward_str]) {	//展望不同属于不同的项目集
						new_Element.forward = First_In_Forward;
						I[0].push_back(new_Element);
					}
				}

			}
		}
		else {
			continue;
		}
	}

	I_size = 1;
	//准备可用于go的符号
	unordered_set<char> symbols_for_go;
	for (auto element : I[0])
	{
		//若.在右部末尾则无法GO
		int char_next_point = element.num + 1;
		if (char_next_point < element.right_part.length())
			symbols_for_go.insert(element.right_part[char_next_point]);
	}
	//根据可用于go的符号选出所有相关项目
	for (const char& symbol_for_go : symbols_for_go)
	{
		
		vector<I_Element> I_for_go;
		for (auto element : I[0])
		{
			int char_next_point = element.num + 1;
			if (element.right_part[char_next_point] == symbol_for_go)
				I_for_go.push_back(I_Element(element.left_part, element.right_part, char_next_point, element.forward));
		}
		go(I_for_go, symbol_for_go);
	}

}


void PARSER::go(vector<I_Element> I_to_cal, char X)
{
	//step1 计算闭包
	for (int j = 0; j < I_to_cal.size(); j++) {//遍历I_to_cal中所有的项目
		int k = I_to_cal[j].num + 1;
		if (k < I_to_cal[j].right_part.size()) {//非终止，分析I_to_cal[j].right_part[k]
			for (auto Rules : Grammar_Rules) {
				string Char_To_String;
				Char_To_String.push_back(I_to_cal[j].right_part[k]);
				if (Rules.first == Char_To_String) {			//判断是否是产生式左部
					//B->beta,First(forward_str)判断是否重复，目前先不做
					//非重复则加入
					string forward_str;						//存k之后的子串和I_to_cal[j].forward的连接
					forward_str = I_to_cal[j].right_part.substr(k + 1) + I_to_cal[j].forward;
					get_First(forward_str);									//求First集作为新项目集的展望

					I_Element new_Element(Rules.first, Rules.second);
					for (auto First_In_Forward : First[forward_str]) {	//展望不同属于不同的项目集
						new_Element.forward = First_In_Forward;
						I_to_cal.push_back(new_Element); 
					}
				}

			}
		}
		else {
			continue;
		}
	}
	/*
	cout << "I[1]:";
		for (auto Item : I_to_cal) {
			cout << Item.left_part << "->";
			for (int i = 0; i <= Item.right_part.size(); i++) {
				if (i == Item.num + 1) {
					cout << ".";
				}
				if (i == Item.right_part.size())
					break;
				cout << Item.right_part[i];
			}
			cout << "," << Item.forward << endl;
		}
	cout << endl;
	*/

	unordered_set<I_Element, I_ElementHash, I_ElementCmp> I_to_push(I_to_cal.begin(), I_to_cal.end());


	//判断是否与现有闭包重复
	for (int i = 0; i < I_size; i++)
	{
		unordered_set<I_Element, I_ElementHash, I_ElementCmp> check_unique;
		for (auto element : I[i])
			check_unique.insert(element);

		//项目集大小不同必不重复
		if (check_unique.size() != I_to_push.size())
			continue;
		int CNT = 0;
		for (auto element : I_to_push)
		{
			if (check_unique.find(element) == check_unique.end())
				break;
			else
				CNT++;
		}
		//与现有闭包重复,返回
		if (CNT == check_unique.size())
		{
			return;
		}
	}
#ifdef DEBUG_MODE
	
#endif
	const int cur_I_size = I_size;
	I_size++;

	vector<I_Element>().swap(I_to_cal);
	for (auto element : I_to_push)
		I_to_cal.push_back(element);
	I.push_back(I_to_cal);
	show_this_closure();
	//计算可用于go的符号
	unordered_set<char> symbols_for_go;
	for (auto element : I[cur_I_size])
	{
		//若.在右部末尾则无法GO
		int char_next_point = element.num + 1;
		if (char_next_point < element.right_part.length())
			symbols_for_go.insert(element.right_part[char_next_point]);
	}
	//根据可用于go的符号选出所有相关项目
	for (const char& symbol_for_go : symbols_for_go)
	{
		vector<I_Element> I_for_go;
		for (auto element : I[cur_I_size])
		{
			int char_next_point = element.num + 1;
			if (element.right_part[char_next_point] == symbol_for_go)
				I_for_go.push_back(I_Element(element.left_part, element.right_part, char_next_point,element.forward));
		}
		go(I_for_go, symbol_for_go);
	}

}

//求字符串str的First集
void PARSER::get_First(const string& str)
{
	//bool Exist_NULL = true;//判断非终结符的First集是否含空
	for (auto element : str) {
		//当前字符为终结符
		if (Terminal.find(element) != Terminal.end()) {
			First[str].insert(element);
			return;
		}
		//当前字符为非终结符
		else if (NonTerminal.find(element) != NonTerminal.end()) {
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

