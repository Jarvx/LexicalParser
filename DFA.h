#include<iostream>
#include<fstream>
#include<stack>
#include<list>
#include<string>
#include<cstring>
#include<stdlib.h>
#include<set>
using namespace std;

class Edge
{

	public:
	    int start; 
		int end;
		char ch;// character which change the state from one position to next postion
		Edge(){
			start=0;  //start positon
			end=0;    //end   position
		}
		Edge(int a,int b,char c){
			start=a;
			end=b;
			ch=c;
		}
};

/* DFA diagram  */
class DFADiagram  {
	private:
		static const int MaxStateNum=50;   //Maxmium state amount
		list<Edge> EdgeSet;                //set of the edges of the DFA;
		int StateNo[MaxStateNum];            //array for state  sequence number
		set<int> State[MaxStateNum];          //states array
		/*
		  get state sequence number from state,state consists of a series of number
		  such as (1,2,3)
		 */
		int getStateNo(set<int>s);             
 		void DFAWrite(string s); //wirte the DFA to the file named after string s;
 		void DFARead(string s);  //read the file named after string s and contruct the DFA
		void AddEdge(set<int> s,set<int> d,char e);
        		
	public:
		int current_state_number;         //Current state amount number
		int Current_edge_number;

		/*Get Next state  if sucess return state number else return -1 */
 		int getNextState(int Now,char c); 
		/*Construct a DFA */
		int constructDFA(string Expression);
 		
		DFADiagram();
		~DFADiagram(){}
};

/*
  class for decribing a sytax tree node
*/

class SyntaxTreeNode
{

 public:

	char ele;
	SyntaxTreeNode *left;   //left child pointer
	SyntaxTreeNode *right;  //right child pointer

    int pos;                  //position
    set<int>  first_position;   //result of FirstPosition
    set<int>  last_position;   //result of LastPosition
    bool nullable_value;        // value of Nullable function
    bool nullable_value_exist;  //show whether nullable has been calculated
    bool first_position_exist;  //show whether first position set has generated
    bool last_position_exist;  //show whether Last position set has generated
    bool follow_position_exist;

    void showFirstPosition(){
    	for(set<int>::iterator it=first_position.begin();it!=FirstPosition.end();++it){
    		cout<<*it;
    	}
    }
    void ShowLastPosition(){
    	for(set<int>::iterator it=last_position.begin();it!=LastPosition.end();++it){
    		cout<<*it;
    	}
    }
    SyntaxTreeNode(char c){
    	ele=c;
    	left=right=NULL;
    	pos=-1;
    	nullable_value=false;
    	nullable_value_exist=false;
    	first_position_exist=false;
    	last_position_exist=false;
    	follow_position_exist=false;
    }
     ~SyntaxTreeNode(){}
};

/*class for abstract syntax tree which describe the regular expression */
class AbstractSyntaxTree
{
   private:
          SyntaxTreeNode *root;
	  int getPriority(char c);
	  char CharTable[30]; 
	     
   public:
            static unsigned int pos;
	    AbstractSyntaxTree();
	    virtual ~AbstractSyntaxTree(){}
	    bool construct(string expression);               //construct the abstract syntax tree
	    SyntaxTreeNode* getRoot();
	    int getPos(){
	   	return pos;
	    }
	    char* getCharTable(){
	    	return CharTable;
	   }
};

/*
   Flag State, state with a flag,the flag shows whether the state set has been marked
*/

class FlagState {
	public:
	bool flag;     //indicate whether the state set has been marked
	set<int> state;   //the state set
	FlagState(){
		flag=false;
	}
};
