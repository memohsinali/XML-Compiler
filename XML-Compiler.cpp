#include<iostream>
#include<fstream>
#include "string"

using namespace std;

template<class T>
class Stack {
private:
	class node
	{
	public:
		T data;
		int line_no;
		node* next;
	};
public:
	node* top;
	//constructor
	Stack()
	{
		top = NULL;
	}


	bool isEmpty()
	{
		if (top == NULL)
		{
			return true;
		}
		else
		{
			return false;
		}
	}


	bool Push(const T& val, const int line_no)
	{
		node* insert = new node();
		if (insert == NULL)             // case when there is no memory space to make node
		{
			cout << "Stack overflow\n";
			return false;
		}
		else
		{
			insert->data = val;
			insert->line_no = line_no;
			if (top == NULL)
			{
				top = insert;
				top->next = NULL;
				return true;
			}
			else
			{
				insert->next = top;
				top = insert;
				return true;
			}
		}
	}



	bool Pop(string& val, int& line)
	{
		if (!isEmpty())
		{
			val = top->data;   //stores the data of node being popped in n variable
			line = top->line_no;
			node* del;
			del = top;
			top = top->next;
			delete del;
			del = NULL;
			return true;
		}
		else
		{
			return false;     // stack is empty there is nothing to pop
		}
	}



	T Top()
	{
		T val;
		if (!isEmpty())
		{
			val = top->data;
			return val;
		}
	}


	~Stack()
	{
		node* temp1=top;
		node* temp2=top;
		while (temp1 != NULL)
		{
			temp2 = temp1->next;
			delete temp1;
			temp1 = temp2;
		}
	}


};


class xmlData : public Stack<char>
{
private:
	int line_no;
	string line;
	static bool header_found;

public:
	// constructor
	xmlData()
	{
		line_no = 0;
		line = "";
	}

	static bool getHeader()
	{
		return header_found;
	}

	static void setHeader(bool v)
	{
		header_found = v;
	}

	//displays error
	void dislpayError(char data)
	{
		cout << "----Error Detected------\n";
		cout << "Line number : " << line_no << endl;
		cout << "Mismatched tag :  " << data << endl;
	}

	string closingTag(int& index)
	{
		int length = getLength();
		index++;
		string tag;
		while (line[index] != '>' && index < length && line[index] != ' ')    //side case closing tag would not involve space
		{
			tag += line[index];
			index++;
		}
		if (line[index] == '>')
		{
			//index++;
			return tag;
		}
		else
		{
			return "";
		}
	}
	bool checkAttributes(Stack <string>& myStack, int& index)
	{
		int length =getLength();
		index++;
		if (index < length)
		{
			while (line[index] != '=' && index < length && line[index] != '>' && line[index] != '<' && line[index] != ' ')
			{
				index++;
			}
			if (index == length)
			{
				cout << "ERROR~ Tag defintion is not complete at line:" << line_no << endl;
				return false;
			}
			else if (index == '>' || index == ' ')
			{
				cout << "ERROR~ Space not allowed in tags ~ want to write attribute?" << line_no << endl;
				return false;
			}
			else if (line[index] == '<')
			{
				cout << "ERROR~ Missing > at line:" << line_no << endl;
				return false;

			}
			else // = case
			{
				index++;
				if (index == length || (line[index] != '\"' && line[index] != '\''))
				{
					cout << "ERROR~ Attribute defintion faulty at line:" << line_no << endl;
					return false;
				}
				else
				{
					if (line[index] == '\"' || line[index] == '\'')
					{
						string str;
						str += line[index]; // because stack accept string
						myStack.Push(str, line_no);
					}
					index++;
					while ((line[index] != '\"' && line[index] != '\'') && index < length)
					{
						index++;
					}

					if (line[index] == '\'' || line[index] == '\"')
					{
						string d, o; int l;
						myStack.Pop(d, l);
						o += line[index];

						if (o != d)
						{
							cout << "ERROR~" << d << " mismatch at line:" << l << endl;
							return false;
						}
						index++;

						if (index == length)
						{
							cout << "ERROR~ tag defintion not complete at line:" << line_no << endl;
							return false;
						}
						else
						{
							if (line[index] == ' ')      //time to find another attribute in same line
							{
								index++;
								if (index == length)
								{
									cout << "ERROR~ Tag defintion not complete at line:" << line_no << endl;
									return false;
								}
								return checkAttributes(myStack, index);
							}
							else if (line[index] == '>')
							{
								return true;
							}

							else if (line[index] == '?')   //time for closing of header
							{
								string str;
								str += line[index];
								string o, p;
								myStack.Pop(p, line_no);
								if (p==str)
								{
									index++;
									if (line[index] == '>')
									{
										return true;
									}
									else
									{
										cout << "closing angular bracket missing > at line:" << line_no << endl;
										return false;
									}
								}
								else
								{
									cout << "Starting tag of header ? is missing at line:" << line_no << endl;
									return false;
								}
							}

							else
							{
								return false;
							}
						}
					}
					else
					{
						cout << "ERROR~ Attribute defintion not complete:" << line_no << endl;
						return false;
					}
				}

			}
		}
		else
		{
			cout << "ERROR~ Tag defintion not completed at line:" << line_no << endl;
			return false;
		}

	}

	//stores the data in members of class
	void take(string l, int n)
	{
		line = l;
		line_no = n;
	}

	int getLength()
	{
		int count = 0;
		int i = 0;
		while (line[i] != '\0')
		{
			count++;
			i++;
		}
		return count;
	}


	//will check line of xml
	void validate(Stack <string>& myStack, int i)
	{
		int length =getLength();
		if (line[i] != '<') // assumption: new line should begin with < (expecting a opening/closing tag)
		{
			cout << "ERROR~ < expected at line:" << line_no << endl;
			return;
		}
		else
		{
			
			i++;
			if (i < length && line[i] == '/') // what if it's a closing tag
			{
				header_found = true;
				string tag = closingTag(i);
				if (tag == "")
				{
					cout << "wrong closing tag" << endl;
					return;
				}
				else
				{
					//time to pop out of the stack and compare with tag
					string pop_tag; int l;
					if (!myStack.isEmpty())
					{
						myStack.Pop(pop_tag, l);
						if (tag != pop_tag)
						{
							cout << pop_tag << "from line:" << l << "is not closed" << endl;
						}
					}
					else
					{
						cout << "opening tag for " << tag << " at line:" << line_no << " not found" << endl;
					}

					return;
				}

			}

			else if (i < length && line[i] == '!')   //handling comments
			{
				header_found = true;
				i++;
				if (i != length && line[i] == '-')
				{
					i++;
					if (i != length && line[i] == '-')
					{
						i++;
						while (i != length && line[i] != '-')
						{
							i++;
						}
						if (line[i] == '-' && i != length)
						{
							i++;
							if (line[i] == '-')
							{
								i++;
								if (line[i] == '>')
								{
									return;
								}
							}
							else
							{
								cout << "ERROR~ comments not handeled at line :" << line_no << endl;
							}
						}
						else
						{
							cout << "ERROR~ comments not handeled at line :" << line_no << endl;
						}

					}
					else
					{
						cout << "ERROR~ comments not handeled at line :" << line_no << endl;
					}
				}
				else
				{
					cout << "ERROR~ comments not handeled at line :" << line_no << endl;
				}
			}

			else if (line[i] == '?')
			{
				if (header_found == true)
				{
					cout << "ERROR~ header should be at the start ; line number" << line_no << endl;
					return;
				}
				else
				{
					string str;
					str += line[i];
					myStack.Push(str, line_no);
					i++;
					string tag;
					while (i < length && line[i] != ' ')
					{
						tag += line[i];
						i++;
					}
					if (tag != "xml")
					{
						cout << "XMl tag is missing in header\n";
					}
					else
					{
						i++;
						if (!checkAttributes(myStack, i))
						{
							return;
						}

					}
				}
			}

			else if (line[i] != '/' && line[i]!='?' && line[i]!='!') // time to check opening tag
			{
				string tag;
				while (line[i] != '>' && i < length && line[i] != ' ' && line[i] != '=')
				{
					tag += line[i];
					i++;

				}
				if (i == length)
				{
					cout << "ERROR~ Tag definition not completed at line:" << line_no << endl;
					return;
				}
				if (line[i] == '=')
				{
					cout << "ERROR~ = not allowed in tag keyword at line:" << line_no << endl;
					return;
				}
				else if (line[i] == '>')
				{
					//what if tag is empty (<>)
					if (tag == "")
					{
						cout << "ERROR~ Tag keyword is mising at line:" << line_no << endl;
						return;
					}
					myStack.Push(tag, line_no);
					i++;
					while (i < length && line[i] != '<')
					{
						i++;
					}
					if (line[i] == '<')
					{
						validate(myStack, i);
						return;
					}
					else
						return;
				}
				else
				{
					//what if tag is empty (< )
					if (tag == "")
					{
						cout << "ERROR~ Space not allowed in tag keyword at line:" << line_no << endl;
						return;
					}
					else if (checkAttributes(myStack, i)) // only if tag is complete
					{
						
						myStack.Push(tag, line_no);
						i++;
						while (i < length && line[i] != '<')
						{
							i++;
						}
						if (line[i] == '<')
						{
							validate(myStack, i);
							return;
						}
						else
							return;

					}
					else
						return;
				}
			}

		}
	}

	//prints the xml.file line
	void print()
	{
		cout << line;
		cout << endl;
		cout << line_no;
	}

	void ignoreSpaces(int& i)
	{
		i++;
		while (line[i] == ' ' && i < line.size())
			i++;
		return;
	}

};

bool xmlData::header_found = false;


void read_xml_file()
{
	ifstream file("file.txt");
	string line;
	int lineNo = 0;
	bool header_found = false;
	Stack<string> myStack;

	if (file.is_open())
	{
		xmlData obj;
		while (!file.eof())
		{
			getline(file, line);
			lineNo++;		   //increment line no after reading from file
			if (line[0] != '\0') // line must not be empty- no need to read blank line
			{
				obj.take(line, lineNo);
				obj.validate(myStack, 0); // stack and index of line passed as arguments
				header_found = true;
			}
		}
		while (!myStack.isEmpty())
		{
			int l; string s;
			myStack.Pop(s, l);
			cout << s << " not closed at line: " << l << endl;
		}
		file.close();
	}
}
int main()
{
	read_xml_file(); // read the file

	return 0;

}