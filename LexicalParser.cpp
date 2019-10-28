#pragma once
#include"LexcialParser.h"
#include"functions.h"
LexcialParser::LexcialParser()
{
}
LexcialParser::~LexcialParser()
{
}
list<pair<int,list<pair<int,string>>>> LexcialParser::Parser()
{
	ifstream source("source_char.txt",ios::in);
	ifstream Exp("RegularExpression.txt",ios::in);
	if(!source)
	{
		cout<<"Failed to open the file:source_char.txt\nPress any key to exit"<<endl;
		getchar();
		exit(1);
	}
	if(!Exp)
	{
		cout<<"Failed to open the file:RegularExpression.txt\nPress any key to exit"<<endl;
		getchar();
		exit(1);
	}
	string LineCode;
	DFADiagram DFA;

	list<pair<int,list<pair<int,string>>>> TokenList;
	
	string Expression;
	getline(Exp,Expression);
	DFA.ConstructDFA(Expression);

	int lineCount=-1;
	for(;getline(source,LineCode);)
	{
		++lineCount;
		list<pair<int,string>> TokenLine;

		/*push a space to generator last token int the line  */

		LineCode.push_back(' ');
		char buffer;
		int pos=1,oldpos=1;   //position at DFA
		string words;
		int l=LineCode.length();
		int i=0;
		for(;i<l;)
		{
			char buffer=LineCode[i];
			oldpos=pos;
			pos=DFA.GetNextState(pos,CharFormat(LineCode[i]));
			if(pos==-1)
			{
				if(!words.empty()) 
				{
					pair<int,string> pa;
					pa=ToToken(words);
					//TokenLine.push_back(make_pair(lineCount,pa));
					TokenLine.push_back(pa);
				}
				words.clear();
				pos=1;
				if(oldpos!=1)
					i--;
			}
			else
			{
				words.push_back(buffer);
			}
			i++;
		}
		TokenList.push_back(make_pair(lineCount,TokenLine));
	}
	source.close();
	return TokenList;
}

char  LexcialParser:: CharFormat(char c)
{
	if(c>='a'&&c<='z'||c>='A'&&c<='Z')
	return 'l';
	if(c>='0'&&c<='9')
	return 'd';
	
}
bool LexcialParser::IsKey(string words)
{
	string KeyWords[KEYWORDSNUMBER]={"else","if","int","return","void","while"};
	for(int i=0;i<KEYWORDSNUMBER;i++)
	{
		if(KeyWords[i]==words)
		{
			return true;
		}
	}
	return false;
}

bool LexcialParser::IsNumber(string words)
{
	int l=words.length();
	for(int i=0;i<l;i++)
	{
		if(words[i]<'0'||words[i]>'9')
		return false;
	}
	return true;
}

bool LexcialParser::IsSymbol(string words)
{
	string SymbolSet[SYMBOLSNUMBER]={"+","-","*","/","<","<=",">",">=","==","!=","=",";",",","(",")","[","]","{","}"};
	for(int i=0;i<SYMBOLSNUMBER;i++)
	{
		if(words==SymbolSet[i])
			return true;
	}
	return false;	
}

bool LexcialParser::IsIden(string words)
{
	for(int i=0;i<words.length();i++)
    {
    	if(words[i]<'A'&&words[i]<'Z'&&words[i]<'a'&&words[i]>'z'&&words[i]<'0'&&words[i]>'9')
    	return false;
    }
    return true;
}


pair<int,string> LexcialParser::ToToken(string word)
{
  	if(IsKey(word))
	{
		return pair<int,string>(0,word);
	}
	if(IsNumber(word))
	{
		return pair<int,string>(1,word);
	}
	if(IsIden(word))
	{
		return pair<int,string>(2,word);
	}
	if(IsSymbol(word))
	{
		return pair<int,string>(3,word);
	}
}