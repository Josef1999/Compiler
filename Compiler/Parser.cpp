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
	cout << "�ս��Ϊ��" << endl;
	for (auto element : Terminal)
		cout << element << ' ';
	cout << endl;
}
void PARSER::show_NonTerminal()
{
	cout << "���ս��Ϊ��" << endl;
	for (auto element : NonTerminal)
		cout << element << ' ';
	cout << endl;
}
void PARSER::show_Grammer_Rules()
{
	cout << "�Ƶ�ʽΪ��" << endl;
	for (auto element : Grammar_Rules)
		cout << element.first << "->" << element.second << endl;
	cout << endl;
}
void PARSER::show_First()
{
	cout << "�ս��First��Ϊ��" << endl;
	for (auto ch : Terminal) {
		string Char_To_String;
		Char_To_String.push_back(ch);
		cout << "First[" << ch << "]: ";
		for (auto First_item : First[Char_To_String]) {
			cout << First_item << " ";
		}
		cout << endl;
	}

	cout << "���ս��First��Ϊ��" << endl;
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
	cout << "�հ�Ϊ��" << endl;
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

void PARSER::show_Action_and_Goto()
{
	cout << setw(4) << setiosflags(ios::left) << "״̬" << setw(8) << "Action" << setw(8) << "Goto" << endl;
	cout << setw(4) << ' ';
	for (auto ch : Terminal)
		cout << setw(4) << ch;
	for (auto ch : NonTerminal)
		cout << setw(4) << ch;
	cout << endl;

	for (int i = 0; i < Action.size(); i++)
	{
		cout << setw(4) << i;
		for (auto ch : Terminal)
			if (Action[i].find(ch) != Action[i].end())
				switch (Action[i][ch].first)
				{
				case acc:
					cout << setw(4) << "acc";
					break;
				case push_in:
					cout << "s" << setw(4) << Action[i][ch].second;
					break;
				case pop_out:
					cout << "r" << setw(4) << Action[i][ch].second;
					break;
				}
			else
				cout << setw(4) << ' ';
		for (auto ch : NonTerminal)
			if (Goto[i].find(ch) != Goto[i].end())
				cout << setw(4) << Goto[i][ch];
			else
				cout << setw(4) << ' ';
		cout << endl;
			
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
	//����Ŀ���հ�
	init_closure();

#ifdef DEBUG_MODE
	show_Action_and_Goto();
#endif

}
void PARSER::init_Terminal()
{
	//�����ս��
	string line;
	getline(infile, line);
	stringstream ss(line);
	char ch;
	while (ss >> ch)
		Terminal.insert(ch);
}
void PARSER::init_NonTerminal()
{
	//�����Ƶ�ʽ��������ս��
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

	for (auto ch : Terminal) {			//�ս��
		string Char_To_String;
		Char_To_String.push_back(ch);
		First[Char_To_String].insert(ch);
	}

	unordered_map<string, unordered_set<char>> pre_First;
EXTEND_FIRST:
	pre_First = First;
	for (auto str : NonTerminal) {		//���ս��
		string Char_To_String;
		Char_To_String.push_back(str);
		for (auto Rules : Grammar_Rules) {
			if (Rules.first == Char_To_String) {
				bool Exist_Null = true;			//�ж�����First���Ƿ񺬿�
				for (auto Right_Element : Rules.second) {
					if (Terminal.find(Right_Element) != Terminal.end()) {//�ұߵķ���Right_Element���ս��
						First[Char_To_String].insert(Right_Element);	//��ӵ�First��
						Exist_Null = false;
						break;
					}
					else {													//�ұߵķ���Right_Element�Ƿ��ս��
						string Element_To_String;
						Element_To_String.push_back(Right_Element);
						//�÷��ս����First����ӵ�����ʽ�󲿷��ŵ�First��
						First[Char_To_String].insert(First[Element_To_String].begin(), First[Element_To_String].end());
						if (First[Element_To_String].find('$') != First[Element_To_String].end()) {//�÷��ս������,��Ҫ�ڲ���ʽ�󲿷��ŵ�First��ɾ����
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

//���ʼ�հ�I0�����ÿ��ǹ�Լ���
void PARSER::init_closure()
{
	//���ʼ�հ�I0
	I[0].push_back(starter);

	//int count = 0;//������

	for (int j = 0; j < I[0].size(); j++) {//����I[i]�����е���Ŀ
		int k = I[0][j].num + 1;
		if (k < I[0][j].right_part.size()) {//����ֹ������I[i][j].right_part[k]
			for (auto Rules : Grammar_Rules) {
				string Char_To_String;
				Char_To_String.push_back(I[0][j].right_part[k]);
				if (Rules.first == Char_To_String) {			//�ж��Ƿ��ǲ���ʽ��
					//B->beta,First(forward_str)�ж��Ƿ��ظ���Ŀǰ�Ȳ���
					//���ظ������

					string forward_str;						//��k֮����Ӵ���I[i][j].forward������
					forward_str = I[0][j].right_part.substr(k + 1) + I[0][j].forward;
					get_First(forward_str);									//��First����Ϊ����Ŀ����չ��

					I_Element new_Element(Rules.first, Rules.second);
					for (auto First_In_Forward : First[forward_str]) {	//չ����ͬ���ڲ�ͬ����Ŀ��
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
	const int cur_I = 0;
	I_size = 1;
	//׼��������go�ķ���
	unordered_set<char> symbols_for_go;
	for (auto element : I[0])
	{

		int char_next_point = element.num + 1;
		//��.����ĩβ���������Go���ַ�����
		if (char_next_point < element.right_part.length())
			symbols_for_go.insert(element.right_part[char_next_point]);

	}
	Goto.push_back(unordered_map<char, int>());
	Action.push_back(unordered_map<char, pair<action, int>>());
	//���ݿ�����go�ķ���ѡ�����������Ŀ
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
		//Action���ƽ�
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
	//step1 ����հ�
	for (int j = 0; j < I_to_cal.size(); j++) {//����I_to_cal�����е���Ŀ
		int k = I_to_cal[j].num + 1;
		if (k < I_to_cal[j].right_part.size()) {//����ֹ������I_to_cal[j].right_part[k]
			for (auto Rules : Grammar_Rules) {
				string Char_To_String;
				Char_To_String.push_back(I_to_cal[j].right_part[k]);
				if (Rules.first == Char_To_String) {			//�ж��Ƿ��ǲ���ʽ��
					//B->beta,First(forward_str)�ж��Ƿ��ظ���Ŀǰ�Ȳ���
					//���ظ������
					string forward_str;						//��k֮����Ӵ���I_to_cal[j].forward������
					forward_str = I_to_cal[j].right_part.substr(k + 1) + I_to_cal[j].forward;
					get_First(forward_str);									//��First����Ϊ����Ŀ����չ��

					I_Element new_Element(Rules.first, Rules.second);
					for (auto First_In_Forward : First[forward_str]) {	//չ����ͬ���ڲ�ͬ����Ŀ��
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

	//step2 ׼��go
	unordered_set<I_Element, I_ElementHash, I_ElementCmp> I_to_push(I_to_cal.begin(), I_to_cal.end());
	//�ж��Ƿ������бհ��ظ�
	for (int i = 0; i < I_size; i++)
	{
		unordered_set<I_Element, I_ElementHash, I_ElementCmp> check_unique;
		for (auto element : I[i])
			check_unique.insert(element);

		//��Ŀ����С��ͬ�ز��ظ�
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
		//�����бհ��ظ�,�����ظ�����Ŀ�����
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


	//������go�ķ��ż�
	unordered_set<char> symbols_for_go;
	for (auto element : I[cur_I])
	{

		int char_next_point = element.num + 1;
		//��.�����Ҳ�ĩβ���������go�ķ��ż�
		if (char_next_point < element.right_part.length())
			symbols_for_go.insert(element.right_part[char_next_point]);
		//��.��ĩβ�����Action�Ĺ�Լ
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
	//���ݿ�����go�ķ���ѡ�����������Ŀ
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
		//Action���ƽ�
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

//���ַ���str��First��
void PARSER::get_First(const string& str)
{
	//bool Exist_NULL = true;//�жϷ��ս����First���Ƿ񺬿�
	for (auto element : str) {
		//��ǰ�ַ�Ϊ�ս��
		if (Terminal.find(element) != Terminal.end()) {
			First[str].insert(element);
			return;
		}
		//��ǰ�ַ�Ϊ���ս��
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
			return;//��������ǰ������(��������ǳ������԰�����������֧�ϲ�)��
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