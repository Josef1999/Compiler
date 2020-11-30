#include"Parser.h"
#define DEBUG_MODE
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
	for (auto Item_Set : I) {
		for (auto Item : Item_Set) {
			cout << Item.left_part << "->";
			for (int i = 0; i < Item.right_part.size(); i++) {
				if (i == Item.num + 1) {
					cout << ".";
				}
				cout << Item.right_part[i];
			}
			cout << "," << Item.forward << endl;
		}
	}
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
	show_Terminal();
	show_NonTerminal();
	show_Grammer_Rules();
	show_First();
	show_closure();
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

//�������������ͬʱ���бհ���GO�ļ��㻹�ǵ���ֻ���㱾��Ŀ�ıհ������ɻ�(Ŀǰ����GO)
void PARSER::init_closure()
{
	//���ʼ�հ�I0

	I[0].push_back(I_Element("Z", "S"));

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

	I_size = 1;
	//׼��������go�ķ���
	unordered_set<char> symbols_for_go;
	for (auto element : I[0])
	{
		//��.���Ҳ�ĩβ���޷�GO
		int char_next_point = element.num + 1;
		if (char_next_point < element.right_part.length())
			symbols_for_go.insert(element.right_part[char_next_point]);
	}
	//���ݿ�����go�ķ���ѡ�����������Ŀ
	for (const char& symbol_for_go : symbols_for_go)
	{
		cout << symbol_for_go << ':';
		vector<I_Element> I_for_go;
		for (auto element : I[0])
		{
			int char_next_point = element.num + 1;
			if (element.right_part[char_next_point] == symbol_for_go)
				I_for_go.push_back(I_Element(element.left_part, element.right_part, element.forward, char_next_point));
		}
		go(I_for_go, symbol_for_go);
	}

}
//void PARSER::get_closure()
void PARSER::go(vector<I_Element> I_for_go, char X)
{

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

