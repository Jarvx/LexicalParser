#pragma once
#include"DFA.h"

#define KEYWORDSNUMBER 6
#define SYMBOLSNUMBER 19

class LexcialParser{
	public:
		LexcialParser();
		virtual ~LexcialParser();
		list<pair<int,list<pair<int,string>>>> Parser();

	protected:

		/*Transform string to token */
		pair<int,string> ToToken(string word);

		char CharFormat(char c);
		
		bool IsNumber(string words);

		bool IsSymbol(string words);

		bool IsKey(string words);

		bool IsIden(string words);

};
