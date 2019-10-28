#include"DFA.h"
#include"functions.h"


/*Define the edge of the Graph of the DFA*/

DFADiagram::DFADiagram(){	
	memset(StateNo,0,sizeof(StateNo));
	current_state_number=0;
	current_edge_number=0;
}

int DFADiagram::ConstructDFA(string Expression){
	/*define the abstract syntax tree to describe the regular expression*/
	AbstractSyntaxTree AST;        
	/* root of abstract syntax trees */
	SyntaxTreeNode *root=NULL; 
	/* set for result of function of Calc_FollowPos  */
	
	set<int> *follow_position_set=NULL;
	list<FlagState> Dstates;
	
	char *CharTable=NULL;
	
        InitExpression(Expression);
    
	AST.Construct(Expression);
	root=AST.GetRoot();
	CharTable=AST.getCharTable();
	

	follow_position_set=new set<int>[AST.GetPos()];
	Calc_Nullable(root);
	Calc_FirstPos(root);
	Calc_LastPos(root);	
	Calc_FollowPos(root,FollowPositionSet);
	
	/* construct the DFA*/
	
	FlagState S;
	S.flag=false;;
	S.state=root->FirstPosition;
	Dstates.push_back(S);
	
	while(Unmarked_FlagState(Dstates,S)!=1){
	   
	   set<char> Dtrans;
	   for(set<int>::iterator it=S.state.begin();it!=S.state.end();++it){
	   	
			 Dtrans.insert(CharTable[*it]);
	   }
	   for(set<char>::iterator it=Dtrans.begin();it!=Dtrans.end();++it){
	   	  set<int> Uset;
	   	  for(set<int>::iterator it1=S.state.begin();it1!=S.state.end();++it1){
	   	  	if(CharTable[*it1]==*it){
	   	  		MergeSet(Uset,FollowPositionSet[(*it1)-1]);
	   	  	}
	   	  }
	   	  FlagState FlagUset;
	   	  FlagUset.state=Uset;
	   	  FlagUset.flag=false;
	   	  if(FindFromDstate(Dstates,FlagUset)==false){
	   	  	Dstates.push_back(FlagUset);	
	   	  }

		  char cTemp=*it;
		  switch (cTemp)
		  {
			case '0':cTemp='*';
				break;
			case '1':cTemp='/';
				break;
			case '2':cTemp='(';
				break;
			case '3':cTemp=')';
				break;
			default: break;
		   }
	  	  AddEdge(S.state,Uset,cTemp);  
	   }
	}
	return 0;
}

void DFADiagram::DFARead(string s){
	ifstream in;
	in>>CurrentStateNumber;
	in>>CurrentEdgeNumber;
	for(int i=1;i<=CurrentEdgeNumber;i++){
		unsigned int temp;
		Edge ee;
		in>>ee.start;
		in>>ee.end;
		in>>temp;
		ee.ch=static_cast<char>(temp);		    		
	}
}

void DFADiagram::DFAWrite(string s){
	ofstream out(s.c_str(),ios::out);
	if(!out){
		cout<<"Failed to open the file:mindfa.txt"<<endl;
		return ;	
	}
	else{
		cout<<"OK"<<endl;
	}
	out<<CurrentStateNumber<<" "<<endl;
	out<<CurrentEdgeNumber<<" "<<endl;
    for(list<Edge>::iterator it=EdgeSet.begin();it!=EdgeSet.end();++it){
    	out<<it->start<<" "<<it->end<<" "<<static_cast<int>(it->ch)<<endl;
    }
    out.close();
}

int DFADiagram::getStateNo(set<int> s ){
    for(int i=1;i<=CurrentStateNumber;i++){
    	set<int> temp;
    	temp=s;
    	if(temp.size()==State[i].size()){
    		MergeSet(temp,State[i]);
    		if(temp.size()==State[i].size()){
    			return i;
    		}
    	}
    }
    return -1;	
}


/*  add an edge to the graph */
void DFADiagram::addEdge(set<int> s,set<int> d,char e){
	if(s.empty()) return;
	if(d.empty()) return;
	int st=0,en=0;
	st=GetStateNo(s);
	en=GetStateNo(d);
	if(st==-1){
	  st =++current+state_number;  
	  State[current_state_number]=s;
	}
	if(en==-1){
		en=++current_state_number;
		State[current_state_number]=d;
	}
	Edge  ee(st,en,e);
	EdgeSet.push_back(ee);
	current_edge_number++;
}

int DFADiagram::getNextState(int now,char c){
    for(list<Edge>::iterator it=EdgeSet.begin();it!=EdgeSet.end();++it){
    	if(it->start==now && it->ch==c){
		return it->end;
    	}
    }
    return -1;
}

unsigned int AbstractSyntaxTree::pos=0;

AbstractSyntaxTree::AbstractSyntaxTree(){
   memset(CharTable,0,sizeof(CharTable));
}

int AbstractSyntaxTree::getPriority(char c){
    switch(c){
    	case '|':return 1;
    	case '.':return 2;
    	case '*':return 3;
    	default: return 0;
    }
}
bool AbstractSyntaxTree::construct(string expression){
     stack<char> OperatorStack;
     stack<SyntaxTreeNode*> SyntaxTreeNodeStack;
    
     for(int i=0;i<expression.length();i++){
		if(expression[i]=='('){
			OperatorStack.push(expression[i]);
			continue;
		}
		if(!IsOperator(expression[i])){
			SyntaxTreeNode *temp=new SyntaxTreeNode(expression[i]);
			temp->pos=++pos;
			CharTable[temp->pos]=temp->ele;
			SyntaxTreeNodeStack.push(temp);
			continue;
		}
		if(expression[i]=='|'||expression[i]=='.'||expression[i]=='*'){
			
			if(OperatorStack.empty()||OperatorStack.top()=='('){
				OperatorStack.push(expression[i]);
		     		continue;
			}
			else{
				while(!OperatorStack.empty()&&GetPriority(OperatorStack.top())>=GetPriority(expression[i])){
					SyntaxTreeNode* Node=new SyntaxTreeNode(OperatorStack.top());
				
				    if(OperatorStack.top()=='*'){
				    	Node->left=SyntaxTreeNodeStack.top();
					    SyntaxTreeNodeStack.pop();
				    	SyntaxTreeNodeStack.push(Node);
				    	OperatorStack.pop();
				    	continue;
			     		}
					OperatorStack.pop();

                    
					Node->right=SyntaxTreeNodeStack.top();
					SyntaxTreeNodeStack.pop();

					Node->left=SyntaxTreeNodeStack.top();
					SyntaxTreeNodeStack.pop();

					SyntaxTreeNodeStack.push(Node);
				
				}
				OperatorStack.push(expression[i]);
			}
		}
		if(expression[i]==')')
		{
			while(OperatorStack.top()!='(')
			{
			    SyntaxTreeNode *Node=new SyntaxTreeNode(OperatorStack.top());
			    if(OperatorStack.top()=='*'){
			    	Node->left=SyntaxTreeNodeStack.top();
				    SyntaxTreeNodeStack.pop();
			    	SyntaxTreeNodeStack.push(Node);
			    	OperatorStack.pop();
			    	continue;
			    }
				OperatorStack.pop();
				Node->right=SyntaxTreeNodeStack.top();
				SyntaxTreeNodeStack.pop();
				Node->left=SyntaxTreeNodeStack.top();
				SyntaxTreeNodeStack.pop();
				SyntaxTreeNodeStack.push(Node);
			}
			OperatorStack.pop();             //pop '('
		}
	}

	
	while(!OperatorStack.empty())
	{
	     SyntaxTreeNode *Node=new SyntaxTreeNode(OperatorStack.top());
	     if(OperatorStack.top()=='*'){
	     	Node->left=SyntaxTreeNodeStack.top();
	     	SyntaxTreeNodeStack.pop();
	     	SyntaxTreeNodeStack.push(Node);
	     	OperatorStack.pop(); 	
	     	continue;
	     }
	     OperatorStack.pop();
	     Node->right=SyntaxTreeNodeStack.top();
	     SyntaxTreeNodeStack.pop();
	     Node->left=SyntaxTreeNodeStack.top();
	     SyntaxTreeNodeStack.pop();
	     SyntaxTreeNodeStack.push(Node);
	}
	
	root=new SyntaxTreeNode('.');
	root->left=SyntaxTreeNodeStack.top();
	SyntaxTreeNodeStack.pop();
	root->right=new SyntaxTreeNode('#');
	root->right->pos=++pos;
	CharTable[root->right->pos]=root->right->ele;
	return true;
}

SyntaxTreeNode* AbstractSyntaxTree::getRoot(){
	return root;
}


