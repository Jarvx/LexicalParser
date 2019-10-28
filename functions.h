#pragma once
#include"DFA.h"

/*display all the elements of a set*/
void ShowSet(set<int> s);

/* Merge two sets to the first set */

int MergeSet(set<int> &s1,set<int> &s2);

/*
 if it is accepted letter
   {* ,| ,. ,( ,) } is the only set of operator
*/
bool IsOperator(char c);

void ShowSyntaxTree(SyntaxTreeNode *Node);

void InOrderTrv(SyntaxTreeNode *Node) ;                     //Inorder traversal 

bool Calc_Nullable(SyntaxTreeNode *Node);

void PreOrdTrv(SyntaxTreeNode *Node);

/*Calculate for firstposition set*/

void Calc_FirstPos(SyntaxTreeNode *Node);

/*  Same way as function of "Calc_FirstPos" to get last position */

void Calc_LastPos(SyntaxTreeNode *Node);
/*Calculate for follow set*/
void Calc_FollowPos(SyntaxTreeNode *Node, set<int> *s);

/*
    find state sets which has'n been marked 
    return the first unmarked state set 
*/

int Unmarked_FlagState(list<FlagState> & Dstates,FlagState &ss);

bool FindFromDstate(list<FlagState> Dstates,FlagState s);
/* 
    read the regular expression from expression and initialize
    the expression
*/
void InitExpression(string &Expression);


