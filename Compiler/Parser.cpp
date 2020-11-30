#include"Parser.h"
#define DEBUG_MODE
PARSER::PARSER()
{}
void PARSER::show_Terminal()
{
	cout << "�ս��Ϊ��";
	for (auto element : Terminal)
		cout << element << ' ';
	cout << endl;
}
void PARSER::show_NonTerminal()
{
	cout << "���ս��Ϊ��";
	for (auto element : NonTerminal)
		cout << element << ' ';
	cout << endl;
}
void PARSER::show_Grammer_Rules()
{
	cout << "�Ƶ�ʽΪ��" << endl;
	for (auto element : Grammar_Rules)
		cout << element.first << ' ' << element.second << endl;
	cout << endl;
}
bool PARSER::LR1(const string& grammer_in, const string& file_in)
{
	init(grammer_in);
	return true;
}
//����ɣ�Terminal, NonTerminal,Grammar_Rules
void PARSER::init(const string& grammer_in)
{
	infile.open(grammer_in);
	if (!infile.is_open())
	{
		cout << "Failed to open the grammer file" << endl;
		return;
	}
	//�����ս��
	string line;
	getline(infile, line);
	stringstream ss(line);
	char ch;
	while (ss >> ch)
		Terminal.insert(ch);

	//�����Ƶ�ʽ��������ս��
	while (!infile.eof())
	{
		getline(infile, line);
		int cur_pos = line.find('>');
		string left_part = line.substr(0, cur_pos);
		string right_part = line.substr(cur_pos + 1);
		Grammar_Rules.push_back(make_pair(left_part, right_part));
		NonTerminal.insert(left_part[0]);
	}

	//�����Ƶ�ʽ��First��
	for (auto ch : Terminal) {			//�ս��
		string Char_To_String;
		Char_To_String.push_back(ch);
		First[Char_To_String].insert(ch);
	}

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

#ifdef DEBUG_MODE
	show_Terminal();
	show_NonTerminal();
	show_Grammer_Rules();
#endif

}

void PARSER::get_First(const string& str)			//���ַ���str��First��
{
	//bool Exist_NULL = true;//�жϷ��ս����First���Ƿ񺬿�
	for (auto element : str) {
		if (NonTerminal.find(element) != NonTerminal.end()) {			//��ǰ�ַ�Ϊ�ս��	
			First[str].insert(element);
			return;
		}
		else if (Terminal.find(element) != Terminal.end()) {			//��ǰ�ַ�Ϊ���ս��
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

//�������������ͬʱ���бհ���GO�ļ��㻹�ǵ���ֻ���㱾��Ŀ�ıհ������ɻ�(Ŀǰ����GO)
void PARSER::closure(vector<vector<I_Element>>& I)
{
	//��ʼ�ıհ�I0
	I_Element I0;
	I0.num = 0;
	I0.left_part = "S'";
	I0.right_part = "S";
	I0.forward = '#';
	I[0].push_back(I0);
	int i = 0;//��ʼI0
	while (1) {
		//i,j,k���Ը��������޸�
		//����I[i]�ıհ�
		for (int j = 0; j < I[i].size(); j++) {//����I[i]�����е���Ŀ
			int k = I[i][j].num + 1;
			if (k < I[i][j].right_part.size()) {//����ֹ������I[i][j].right_part[k]
				for (auto Rules : Grammar_Rules) {
					string Char_To_String;
					Char_To_String.push_back(I[i][j].right_part[k]);
					string forward_str;						//��k֮����Ӵ���I[i][j].forward������
					if (Rules.first == Char_To_String) {			//�ж��Ƿ��ǲ���ʽ��
						//B->beta,First(forward_str)�ж��Ƿ��ظ�
						//���ظ������
					}
				}
			}
			else {
				break;				//��������µ���Ŀ
			}
		}

		//ͨ��go�����µıհ�

	}
	
}
void PARSER::go(vector<vector<I_Element>> I, char X)
{

}