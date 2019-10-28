#include"functions.h"

/* Merge two sets to the first set */
int MergeSet(set<int> &s1,set<int> &s2)  {
	for(set<int>::iterator it=s2.begin();it!=s2.end();++it){
		s1.insert(*it);
	}
	return 0;
}
/*
 if it is accepted letter
   {* ,| ,. ,( ,) } is the set of operators
*/
bool IsOperator(char c){
	if(c=='.'||c=='*'||c=='|'||c=='('||c==')')
	return true;
    else
    return false;
}


void ShowSyntaxTree(SyntaxTreeNode *Node){
     if(Node!=NULL){
     	ShowSyntaxTree(Node->left);
     	cout<<Node->ele;
     	ShowSyntaxTree(Node->right);
     }
}
void InOrderTrv(SyntaxTreeNode *Node){
     if(Node!=NULL){
     	InOrderTrv(Node->left);
     	cout<<Node->ele<<"   "<<endl;
     	if(Node->Nullable_Value==true){
     		cout<<"true"<<endl;
     	}
     	else{
     		cout<<"false"<<endl;
     	}
     	cout<<"First Position"<<endl;
     	Node->ShowFirstPosition();		
     	cout<<endl<<endl;
     	cout<<"Last Position"<<endl;
     	Node->ShowLastPosition();
     	cout<<endl<<endl;
     	InOrderTrv(Node->right);
     }



}

bool Calc_Nullable(SyntaxTreeNode *Node)
{
    if(Node->Nullable_Value_Exist){
	return Node->Nullable_Value;
    }
    if(Node->left==NULL&&Node->right==NULL){
	Node->Nullable_Value=false;
	Node->Nullable_Value_Exist=true;
	return Node->Nullable_Value;
    }
    else{
    	if(Node->ele=='.'){
    	  Node->Nullable_Value=Calc_Nullable(Node->left)&&Calc_Nullable(Node->right);
    	  Node->Nullable_Value_Exist=true;
    	  return Node->Nullable_Value;
    	}
    	if(Node->ele=='|'){
    	  Node->Nullable_Value=Calc_Nullable(Node->left)||Calc_Nullable(Node->right);
    	  Node->Nullable_Value_Exist=true;
    	  return Node->Nullable_Value;
    	}
    	if(Node->ele=='*'){
    	  Node->Nullable_Value=true;
    	  Node->Nullable_Value_Exist=true;
    	  return Node->Nullable_Value;
    	}
    }
}

void PreOrdTrv(SyntaxTreeNode *Node){
     if(Node!=NULL){
     	cout<<Node->ele;
        PreOrdTrv(Node->left);
        PreOrdTrv(Node->right);
     }
}

/*Calculate for firstposition set*/

void Calc_FirstPos(SyntaxTreeNode *Node)
{
   if(Node==NULL)	return;
   if(Node->FirstPosition_Exist) return;
   if(Node->left==NULL&&Node->right==NULL){
   	Node->FirstPosition.insert(Node->pos);
	Node->FirstPosition_Exist=true;	
	return ;
   }
   Calc_FirstPos(Node->left);
   Calc_FirstPos(Node->right);
   if(Node->ele=='|'){
   	MergeSet(Node->FirstPosition,Node->left->FirstPosition);
   	MergeSet(Node->FirstPosition,Node->right->FirstPosition);
   	Node->FirstPosition_Exist=true;
   	return;
   }
   if(Node->ele=='.'){
	if(Node->left->Nullable_Value){
	  MergeSet(Node->FirstPosition,Node->left->FirstPosition);
	  MergeSet(Node->FirstPosition,Node->right->FirstPosition);
	}
	else{
	    MergeSet(Node->FirstPosition,Node->left->FirstPosition);
	}
	Node->FirstPosition_Exist=true;
   	return;
   }
   if(Node->ele=='*'){
     MergeSet(Node->FirstPosition,Node->left->FirstPosition);
     Node->FirstPosition_Exist=true;
     return;
   }
}


/*  Same way as function of "Calc_FirstPos" to get last position */

void Calc_LastPos(SyntaxTreeNode *Node)
{
     if(Node==NULL) return;
     if(Node->LastPosition_Exist) return;
     Calc_LastPos(Node->left);
     Calc_LastPos(Node->right);
     if(Node->left==NULL&&Node->right==NULL){
     	Node->LastPosition.insert(Node->pos);
     	Node->LastPosition_Exist=true;
     	return ;
     }
     if(Node->ele=='|'){
     	MergeSet(Node->LastPosition,Node->left->LastPosition);
     	MergeSet(Node->LastPosition,Node->right->LastPosition);
     	Node->LastPosition_Exist=true;
     	return ;
     }
     
     if(Node->ele=='*'){
     	MergeSet(Node->LastPosition,Node->left->LastPosition);
     	Node->LastPosition_Exist=true;
     	return ;
     }
     
     if(Node->ele=='.'){
     	if(Node->right->Nullable_Value){
	  MergeSet(Node->LastPosition,Node->left->LastPosition);
     	  MergeSet(Node->LastPosition,Node->right->LastPosition);
     	}
     	else{
	    MergeSet(Node->LastPosition,Node->right->LastPosition);
     	}
     	Node->LastPosition_Exist=true;
     	return ;
     }
	
}

/*Calculate for follow set*/
void Calc_FollowPos(SyntaxTreeNode *Node, set<int> *s){
     if(Node==NULL) return ;
       if(Node->ele=='.'){
	 for(set<int>::iterator it=Node->left->LastPosition.begin();it!=Node->left->LastPosition.end();++it){
  	    MergeSet(s[(*it)-1],Node->right->FirstPosition);
	 }
       }
     
       if(Node->ele=='*'){
	for(set<int>::iterator it=Node->LastPosition.begin();it!=Node->LastPosition.end();++it){
	   MergeSet(s[(*it)-1],Node->FirstPosition);
	}
       }
       Calc_FollowPos(Node->left,s);
       Calc_FollowPos(Node->right,s);
}




/*
    find state sets which has'n been marked 
    return the first unmarked state set 
*/

int Unmarked_FlagState(list<FlagState> & Dstates,FlagState &ss){
    for(list<FlagState>::iterator it1=Dstates.begin();it1!=Dstates.end();++it1){
    	if(it1->flag==false){
    		ss=*it1;
    		it1->flag=true;
    		//ShowSet(ss.state);
    		return 0 ; /*get the unmarked state set*/
    	}
    }
    return 1;   /*All the state sets marked*/
}

bool FindFromDstate(list<FlagState> Dstates,FlagState s){
     for(list<FlagState>::iterator it1=Dstates.begin();it1!=Dstates.end();++it1){
     	if(it1->state==s.state){
     		return true;
     	}
     }
     return false;
}

/* 
    read the regular expression from expression and initialize
    the expression
*/
void InitExpression(string &Expression)
{
     int l=Expression.length();
     string temp;
     for(int i=0;i<l;){
     	if(Expression[i]=='/'&&i<l-1){
	  i++;
	  if(Expression[i]=='*')
	  	temp.push_back('0');
	  if(Expression[i]=='/')
	  	temp.push_back('1');
	  if(Expression[i]=='(')
	  	temp.push_back('2');
	  if(Expression[i]==')')
	  	temp.push_back('3');
	  i++;
	  continue;
     	}
     	temp.push_back(Expression[i]);
     	i++;
     }
     Expression=temp;
     for(int i=0;i<l;i++){
     	const char t[]=".";
     	if(i==0)
     	continue;
     	
     	if(!IsOperator(Expression[i-1])&&!IsOperator(Expression[i])){
     		Expression.insert(i,t,1);
     		i++;
     	}
     	if(!IsOperator(Expression[i-1])&&Expression[i]=='('){
     		Expression.insert(i,t,1);
     		i++;
     	}
     	if(Expression[i-1]=='*'&&(Expression[i]=='('||!IsOperator(Expression[i]))){
     		Expression.insert(i,t,1);
     		i++;
     	}
     
     	if(Expression[i-1]==')'&&Expression[i]=='('){
     		Expression.insert(i,t,1);
     		i++;
     	}
     }
}

