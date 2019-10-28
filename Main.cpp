#include"DFA.h"
#include"functions.h"
#include"LexcialParser.h"
using namespace std;

// first is the type
// 0 : key words
// 1 : number
// 2 : Identifier
// 3 : symbol
int main(){
    LexcialParser Par;         //set a  lexcial parser
    list<pair<int,list<pair<int,string>>>> TokenSet; //this is a token set 
    TokenSet=Par.Parser();              
    // Show the token set 
    for(list<pair<int,list<pair<int,string>>>>::iterator it=TokenSet.begin();it!=TokenSet.end();++it){
    	cout<<it->first<<"       ";  //row number 
    	for(list<pair<int,string>>::iterator it1=it->second.begin();it1!=it->second.end();++it1){

    		cout<<it1->first<<" "<<it1->second<<" ";
    	}
    	cout<<endl;
    }
    cout<<endl<<"Press any key to exit"<<endl;
    getchar();
    return 0;
}
