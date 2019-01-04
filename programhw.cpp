#include<iostream>
#include<stack>
using namespace std;

int isconnection(char c)
{
	if (c == '!' || c == '&' || c == '|' || c == '~' || c == '^'||c=='('||c==')')
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int assignpriority(char c)
{
	if (c == '~')
	{
		return 1;
	}
	if (c == '^')
	{
		return 2;
	}
	if (c == '|')
	{
		return 3;
	}
	if (c == '&')
	{
		return 4;
	}
	if (c == '!')
	{
		return 5;
	}
	if (c == '(')
	{
		return 0;
	}
}

int morepriority(char c1, char c2)
{
	return (assignpriority(c2) > assignpriority(c1));
}

int iscapital(char c)
{
	if (c > 64 && c < 91)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void printdata(char* data,int len)
{
	for (int i = 0; i < len; i++)
	{
		cout << data[i];
	}
}

int interpretation(char* data,int len,char* result)
{
	int point = 0;
	stack<char> S;
	int i = 0;
	int numberofcapital = 0;
	while (i < len)
	{
		char c = data[i];
		if (iscapital(c))
		{
			result[point++] = c;
			i++;
			numberofcapital++;
			continue;
		}
		else if (isconnection(c))
		{
			if (S.empty())
			{
				S.push(c);
			}
			else
			{
				if (c != '('&&c != ')')
				{
					char top = S.top();
					if (morepriority(top, c))
					{
						S.push(c);
					}
					else
					{
						while(!morepriority(top, c))
						{
							result[point++] = top;
							S.pop();
							if (S.empty())
							{
								break;
							}
							top = S.top();
						} 
						S.push(c);
					}
				}
				else if(c=='(')
				{
					S.push(c);
				}
				else
				{
					while (S.top() != '(')
					{
						if (S.empty())
						{
							cerr << "Check the brackets!" << endl;
						}
						result[point++] = S.top();
						S.pop();
					}
					S.pop();
				}
			}
			i++;
			continue;
		}
		else
		{
			cerr << "Invaild data!" << endl;
			break;
		}
	}
	while (!S.empty())
	{
		result[point++] = S.top();
		S.pop();
	}
	result[point++] = '\0';
	return numberofcapital;
}

int computematrix[5][4] = { 1,0,0,1, 1,1,0,1, 0,1,1,1, 0,0,0,1, 1,1,0,0};

int compute(int n1, int n2, char c)
{
	int index1 = n1 * 2 + n2;
	int index2 = assignpriority(c)-1;
	//cout << index1 << " " << index2 << endl;
	return computematrix[index2][index1];
}

int computeall(char* data, int len, int* value)
{
	stack<int> S;
	int i = 0;
	int valueindex = 0;
	while (i < len)
	{
		char c = data[i];
		if (iscapital(c))
		{
			S.push(value[valueindex]);
			valueindex++;
		}
		else if (isconnection(c))
		{
			if (c == '!')
			{
				if (S.empty())
				{
					cerr << "Invaild data!" << endl;
					break;
				}
				else
				{
					int temp = S.top();
					S.pop();
					S.push(compute(temp, temp, c));
				}
			}
			else
			{
				if (S.empty())
				{
					cerr << "Invaild data!" << endl;
					break;
				}
				else
				{
					int temp1 = S.top();
					S.pop();
					if (S.empty())
					{
						cerr << "Invaild data!" << endl;
						break;
					}
					else
					{
						int temp2 = S.top();
						S.pop();
						S.push(compute(temp2, temp1, c));
					}
				}
			}
		}
		else
		{
			cerr << "Invaild data!" << endl;
			break;
		}
		i++;
	}
	if (S.empty()||S.size()>1)
	{
		cerr << "Invaild data!" << endl;
		return -1;
	}
	else
	{
		return S.top();
	}
}

int truthtable(char*data, int len ,int*table)
{
	char interpret[1000];
	int value[100];
	int numberofcapital=interpretation(data, len, interpret);
	//cout << numberofcapital << endl;
	//cout << interpret << endl;
	int row = 1 << numberofcapital;
	//cout << row << endl;
	for (int i = 0; i < row; i++)
	{
		//cout << "i=" << i << endl;
		int temp = i;
		for (int j = numberofcapital-1; j >= 0; j--)
		{
			value[numberofcapital-1-j] = temp / (1 << j);
			cout << value[numberofcapital - 1 - j];
			temp -= value[numberofcapital - 1 - j] * (1 << j);
		}
		table[i] = computeall(interpret, strlen(interpret), value);
		cout << " " << table[i] << endl;
	}
	return numberofcapital;
}

void printfromtable_1(int* table, int numberofcapital)
{
	int row = 1 << numberofcapital;
	char begin = 'A';
	int value[100];
	int last = row - 1;
	while (table[last] != 1 && last >= 0)
	{
		if (table[last] != 0)
		{
			cerr << "Invaild table!" << endl;
		}
		last--;
	}
	if (last < 0)
	{
		return;
	}
	for (int i = 0; i < last; i++)
	{
		if (table[i] == 1)
		{
			int temp = i;
			char tempchar = begin;
			cout << "(";
			for (int j = numberofcapital - 1; j > 0; j--)
			{
				value[numberofcapital - 1 - j] = temp / (1 << j);
				if (value[numberofcapital - 1 - j] == 0)
				{
					cout << "!" << tempchar << "&";
					tempchar++;
				}
				else
				{
					cout << tempchar <<"&";
					tempchar++;
				}
				temp -= value[numberofcapital - 1 - j] * (1 << j);
			}
			value[numberofcapital - 1 ] = temp;
			if (value[numberofcapital - 1 ] == 0)
			{
				cout << "!" << tempchar << ")|";
				tempchar++;
			}
			else
			{
				cout << tempchar << ")|";
				tempchar++;
			}
		}
		else if (table[i] == 0)
		{
			continue;
		}
		else
		{
			cerr << "Invaild table!" << endl;
		}
	}
	if (table[last] == 1)
	{
		int temp = last;
		char tempchar = begin;
		cout << "(";
		for (int j = numberofcapital - 1; j > 0; j--)
		{
			value[numberofcapital - 1 - j] = temp / (1 << j);
			if (value[numberofcapital - 1 - j] == 0)
			{
				cout << "!" << tempchar << "&";
				tempchar++;
			}
			else
			{
				cout << tempchar << "&";
				tempchar++;
			}
			temp -= value[numberofcapital - 1 - j] * (1 << j);
		}
		value[numberofcapital - 1] = temp;
		if (value[numberofcapital - 1] == 0)
		{
			cout << "!" << tempchar << ")";
			tempchar++;
		}
		else
		{
			cout << tempchar << ")";
			tempchar++;
		}
		cout << endl;
	}
	else
	{
		cerr << "Invaild table!" << endl;
	}
}

void printfromtable_2(int* table, int numberofcapital)
{
	int row = 1 << numberofcapital;
	char begin = 'A';
	int value[100];
	int first = 0;
	while (table[first] != 0&&first<row)
	{
		if (table[first] != 1)
		{
			cerr << "Invaild table!" << endl;
		}
		first++;
	}
	if (first==row)
	{
		return;
	}
	for (int i = row-1; i > first; i--)
	{
		if (table[i] == 0)
		{
			int temp = i;
			char tempchar = begin;
			cout << "(";
			for (int j = numberofcapital - 1; j > 0; j--)
			{
				value[numberofcapital - 1 - j] = temp / (1 << j);
				if (value[numberofcapital - 1 - j] == 0)
				{
					cout << tempchar << "|";
					tempchar++;
				}
				else
				{
					cout << "!" << tempchar << "|";
					tempchar++;
				}
				temp -= value[numberofcapital - 1 - j] * (1 << j);
			}
			value[numberofcapital - 1] = temp;
			if (value[numberofcapital - 1] == 0)
			{
				cout << tempchar << ")&";
				tempchar++;
			}
			else
			{
				cout << "!" << tempchar << ")&";
				tempchar++;
			}
		}
		else if (table[i] == 1)
		{
			continue;
		}
		else
		{
			cerr << "Invaild table!" << endl;
		}
	}
	if (table[first] == 0)
	{
		int temp = first;
		char tempchar = begin;
		cout << "(";
		for (int j = numberofcapital - 1; j > 0; j--)
		{
			value[numberofcapital - 1 - j] = temp / (1 << j);
			if (value[numberofcapital - 1 - j] == 0)
			{
				cout << tempchar << "|";
				tempchar++;
			}
			else
			{
				cout << "!" << tempchar << "|";
				tempchar++;
			}
			temp -= value[numberofcapital - 1 - j] * (1 << j);
		}
		value[numberofcapital - 1] = temp;
		if (value[numberofcapital - 1] == 0)
		{
			cout << tempchar << ")";
			tempchar++;
		}
		else
		{
			cout << "!" << tempchar << ")";
			tempchar++;
		}
		cout << endl;
	}
	else
	{
		cerr << "Invaild table!" << endl;
	}
}

int homework1_1()
{
	char data[1000];
	char result[1000];
	cin >> data;
	int len = strlen(data);
	interpretation(data, len, result);
	cout << result << endl;
	return 0;
}

int homework2_1()
{
	char data[1000];
	int table[10000];
	cin >> data;
	int len = strlen(data);
	truthtable(data, len, table);
	return 0;
}

int homework2_2()
{
	int table[10000];
	int i = 0;
	while (i<10000)
	{
		int temp;
		cin >> temp;
		if (temp == 0 || temp == 1)
		{
			table[i] = temp;
			i++;
		}
		else
		{
			break;
		}
	}
	int j = 1;
	int numberofcapital = 0;
	for (; j < i; j = j * 2 )
	{
		numberofcapital++;
	}
	if (j > i|| numberofcapital==0)
	{
		cerr << "Table incomplete!" << endl;
		return 1;
	}
	else
	{
		printfromtable_1(table, numberofcapital);
		return 0;
	}
}

int homework2_3()
{
	char data[1000];
	int table[10000];
	cin >> data;
	int len = strlen(data);
	int numberofcapital=truthtable(data, len, table);
	printfromtable_1(table, numberofcapital);
	printfromtable_2(table, numberofcapital);
	return 0;
}

int main()
{
	while (true)
	{
		cout << "输入数字，实现功能：" << endl;
		cout << "1.hw1:输入命题公式，输出逆波兰表达式" << endl;
		cout << "2.hw2-1:输入命题公式，列出真值表" << endl;
		cout << "3.hw2-2:输入真值表（空格隔开，非0/1则停止），输出命题公式" << endl;
		cout << "4.hw2-3:输入命题公式，输出主析取范式和主合取范式（方便起见，一并输出真值表）" << endl;
		cout << "5.退出" << endl;
		int i;
		cin >> i;
		if (i == 1)
		{
			homework1_1();
		}
		else if (i == 2)
		{
			homework2_1();
		}
		else if (i == 3)
		{
			homework2_2();
		}
		else if (i == 4)
		{
			homework2_3();
		}
		else if (i == 5)
		{
			break;
		}
		else
		{
			cerr << "输入错误！" << endl;
			break;
		}
	}
	return 0;
}