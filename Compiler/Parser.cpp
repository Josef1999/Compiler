#include"Parser.h"
#define DEBUG_MODE
const I_Element starter("Z", "S");
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
	int count = 1;
	for (auto element : Grammar_Rules) {
		cout << count++ << " ";
		cout << element.first << "->" << element.second << endl;
	}
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
void PARSER::show_this_analysis_step()
{
	static int step = 0;
	cout <<"step:"<< setw(4) << step<<endl;
	show_Status();
	show_Symbol();
	show_InputString();
	step++;
}
void PARSER::show_Symbol()
{
	cout << "Symbol：";
	for (auto element : this->Symbol)
		cout << element;
	cout << endl;
}
void PARSER::show_Status()
{
	cout << "Status：";
	for (auto element : this->Status)
		cout << element << '|';
	cout << endl;
}
void PARSER::show_Action_and_Goto()
{
	ofstream out("../Action_Goto表.txt");
	out << setw(4) << setiosflags(ios::left) << "状态" << setw(8) << "Action" << setw(8) << "Goto" << endl;
	out << setw(4) << ' ';
	for (auto ch : Terminal)
		out << setw(4) << ch;
	for (auto ch : NonTerminal)
		out << setw(4) << ch;
	out << endl;

	for (int i = 0; i < Action.size(); i++)
	{
		out << setw(4) << i;
		for (auto ch : Terminal)
			if (Action[i].find(ch) != Action[i].end())
				switch (Action[i][ch].first)
				{
				case acc:
					out << setw(4) << "acc";
					break;
				case push_in:
					out << "s" << setw(4) << Action[i][ch].second;
					break;
				case pop_out:
					out << "r" << setw(4) << Action[i][ch].second+1;
					break;
				}
			else
				out << setw(4) << ' ';
		for (auto ch : NonTerminal)
			if (Goto[i].find(ch) != Goto[i].end())
				out << setw(4) << Goto[i][ch];
			else
				out << setw(4) << ' ';
		out << endl;
			
	}
}
void PARSER::show_this_closure()
{
	cout << "I[" << I_size - 1 << "]:" << endl;
	for (auto Item : I[I_size - 1]) {
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
void PARSER::show_InputString()
{
	cout << "InputString：";
	cout << InputString.substr(InputString_idx) << endl;
}
bool PARSER::LR1(const string& grammer_in, const string& file_name)
{
	init(grammer_in);
	analysis_init(file_name);
	return analysis();
	
}
void PARSER::init(const string& grammer_in)
{
	infile.open(grammer_in);
	if (!infile.is_open())
	{
		cout << "Failed to open the grammer file" << endl;
		exit(1);
	}
	init_Terminal();
	init_NonTerminal();
	infile.close();

	init_First();

	//求项目集闭包与Action，Goto表
	init_table();

#ifdef DEBUG_MODE
	show_Action_and_Goto();
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

	map<string, unordered_set<char>> pre_First;
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
		//FIRST集是否增大
		if(!equal(First.begin(),First.end(),pre_First.begin()))
			goto EXTEND_FIRST;
		/*auto element = First.begin();
		for (auto pre_element : pre_First)
		{
			if (pre_element.second.size() != element->second.size())
				goto EXTEND_FIRST;
			element++;
		}*/
	}
	return;
}
void PARSER::init_table()
{
	//求初始闭包I0，不用考虑规约情况
	I[0].push_back(starter);

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
					//查重
					unordered_set<I_Element, I_ElementHash, I_ElementCmp> check_unique(I[0].begin(), I[0].end());
					I_Element new_Element(Rules.first, Rules.second);
					for (auto First_In_Forward : First[forward_str]) {	//展望不同属于不同的项目集
						new_Element.forward = First_In_Forward;
						if(check_unique.find(new_Element)==check_unique.end())		
							I[0].push_back(new_Element);
					}
				}

			}
		}
		else {
			continue;
		}
	}
	const int cur_I = 0;
	I_size = 1;
	//准备可用于go的符号
	unordered_set<char> symbols_for_go;
	for (auto element : I[0])
	{

		int char_next_point = element.num + 1;
		//若.不在末尾则添加至可Go的字符集内
		if (char_next_point < element.right_part.length())
			symbols_for_go.insert(element.right_part[char_next_point]);

	}
	Goto.push_back(unordered_map<char, int>());
	Action.push_back(unordered_map<char, pair<action, int>>());
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

		int res = go(I_for_go, symbol_for_go);
		//Action的移进
		if (Terminal.find(symbol_for_go) != Terminal.end())
		{
			Action[cur_I][symbol_for_go] = make_pair(push_in, res);
		}
		//Goto
		else if (NonTerminal.find(symbol_for_go) != NonTerminal.end())
		{
			Goto[cur_I][symbol_for_go] = res;
		}

	}

}
int PARSER::go(vector<I_Element> I_to_cal, char X)
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

					unordered_set<I_Element, I_ElementHash, I_ElementCmp> check_unique(I_to_cal.begin(), I_to_cal.end());
					I_Element new_Element(Rules.first, Rules.second);
					for (auto First_In_Forward : First[forward_str]) {	//展望不同属于不同的项目集
						new_Element.forward = First_In_Forward;
						if(check_unique.find(new_Element) == check_unique.end())
							I_to_cal.push_back(new_Element);
					}
				}

			}
		}
		else {
			continue;
		}
	}

	//step2 准备go
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
		//与现有闭包重复,返回重复的项目集编号
		if (CNT == check_unique.size())
		{
			return i;
		}
	}

	const int cur_I = I_size;
	I_size++;

	Goto.push_back(unordered_map<char, int>());
	Action.push_back(unordered_map<char, pair<action, int>>());
	vector<I_Element>().swap(I_to_cal);
	for (auto element : I_to_push)
		I_to_cal.push_back(element);
	I.push_back(I_to_cal);


	//可用于go的符号集
	unordered_set<char> symbols_for_go;
	for (auto element : I[cur_I])
	{

		int char_next_point = element.num + 1;
		//若.不在右部末尾则加入至可go的符号集
		if (char_next_point < element.right_part.length())
			symbols_for_go.insert(element.right_part[char_next_point]);
		//若.在末尾则计算Action的规约
		else
		{
			if(element.left_part == starter.left_part && element.right_part == starter.right_part)
				Action[cur_I][element.forward] = make_pair(acc,-1);
			else
			{
				int index = get_Grammar_Rules_index(make_pair(element.left_part, element.right_part));
				Action[cur_I][element.forward] = make_pair(pop_out, index);

			}
			
		}
	}
	//根据可用于go的符号选出所有相关项目
	for (const char& symbol_for_go : symbols_for_go)
	{
		vector<I_Element> I_for_go;
		for (auto element : I[cur_I])
		{
			int char_next_point = element.num + 1;
			if (element.right_part[char_next_point] == symbol_for_go)
				I_for_go.push_back(I_Element(element.left_part, element.right_part, char_next_point, element.forward));
		}

		int res = go(I_for_go, symbol_for_go);
		//Action的移进
		if (Terminal.find(symbol_for_go) != Terminal.end())
		{
			Action[cur_I][symbol_for_go] = make_pair(push_in, res);
		}
		//Goto
		else if (NonTerminal.find(symbol_for_go) != NonTerminal.end())
		{
			Goto[cur_I][symbol_for_go] = res;
		}

	}

	return cur_I;

}


void PARSER::get_First(const string& str)
{
	//求字符串str的First集
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
int PARSER::get_Grammar_Rules_index(const pair<string, string>& target)
{
	for (int i = 0; i < Grammar_Rules.size(); i++)
		if (Grammar_Rules[i] == target)
			return i;
	return -1;
}
void PARSER::analysis_init(const string& file_name)
{
	Lex_to_Parser(file_name);
	InputString.push_back('#');
	//符号栈、状态栈初始化
	Status.push_back(0);
	Symbol.push_back('#');
}
bool PARSER::analysis()
{
	show_closure();
	while (1) {
		show_this_analysis_step();
		//show_Grammer_Rules();
		int cur_status = Status.back();
		char cur_input_symbol=InputString[InputString_idx];//当前分析符号
		int size;

			
		switch (Action[cur_status][cur_input_symbol].first)
		{
			case push_in:
				if (Status.back() == '$')
					Status.pop_back();
				Status.push_back(Action[cur_status][cur_input_symbol].second);				//移进状态
				if (cur_input_symbol!='$')
					Symbol.push_back(cur_input_symbol);											//移进符号
				InputString_idx++;															//字符串
				break;
			case pop_out:
				size=Grammar_Rules[Action[cur_status][cur_input_symbol].second].second.size();		//规约语法右部符号个数
				for (int i = 1; i <= size; i++) {
					Symbol.pop_back();		
					Status.pop_back();
				}
				Symbol.push_back(Grammar_Rules[Action[cur_status][cur_input_symbol].second].first[0]);		//规约左部
				Status.push_back(Goto[Status.back()][Grammar_Rules[Action[cur_status][cur_input_symbol].second].first[0]]);//规约之后进行符号栈的更新
				break;
			case acc:
				return true;
				break;
			default:
				if (Symbol.back() != '$') {
					InputString[--InputString_idx] = '$';
					Symbol.push_back('$');
				}
				else {
					return false;
				}
				break;
		}
	}
	return true;
}
void PARSER::Lex_to_Parser(const string& file_name)
{
	unordered_map<string, char> Lex_translate = {
		{"LP",'a'},{"RP",'b'},{"INT",'c'},{"VOID",'d'},
		{"LB",'e'},{"RB",'f'},{"DEL",'g'},{"RETURN",'h'},
		{"WHILE",'i'},//{"RELOP",'-'},{"OP1",'-'},{"OP2",'-'},
		{"SEP",'t'},{"ID",'u'},{"NUM",'v'} ,{"ASSIGN",'w'},{"IF",'x'}
	};
	infile.clear();
	infile.open(file_name);
	if (!infile.is_open()) {
		cout << "Lex输出结果文件打开失败" << endl;
		exit(1);
	}
	string line;
	int start, mid,end;
	while (!infile.eof()) {
		getline(infile, line);
		if (line == "") {
			break;
		}
		start = line.find('<');
		mid = line.find(',');
		end = line.find('>');
		string first_part = line.substr(start + 1, mid - start - 1);
		string second_part = line.substr(mid + 1, end - mid - 1);
		int type = atoi(second_part.c_str());			//RELOP,OP1,OP2集合的内部符号的种类判断
		if (first_part == "RELOP") {
			switch (type)
			{
				//"<=","0"},{"!=","1"},{"==","2"},{">=","3" (<,4)(>,5)
				case 0:
					InputString.push_back('k');
					break;
				case 1:
					InputString.push_back('o');
					break;
				case 2:
					InputString.push_back('n');
					break;
				case 3:
					InputString.push_back('m');
					break;
				case 4:
					InputString.push_back('j');
					break;
				case 5:
					InputString.push_back('l');
					break;
				default:
					break;
			}
		}
		else if (first_part == "OP1") {
			switch (type)
			{
				case 0:
					InputString.push_back('p');
					break;
				case 1:
					InputString.push_back('q');
					break;
				default:
					break;
			}
		}
		else if (first_part == "OP2") {
			switch (type)
			{
				case 0:
					InputString.push_back('r');
					break;
				case 1:
					InputString.push_back('s');
					break;
				default:
					break;
			}
		}
		else if (first_part == "NL") {
			;
		}
		else {
			InputString.push_back(Lex_translate[first_part]);
		}
	}
	cout << InputString;
}