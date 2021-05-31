
#include <iostream>
#include <fstream>
#include "Parser&Interpretter/parse.cpp"
using namespace std;
//main driver program
int main(int argc,char*argv[]){
	bool status;
	int lineNumber;
	ifstream file;
	istream *in = NULL;
	for(int i=1;i<argc; i++){
		string arg = argv[i];
		if( in != NULL ){
			cout<< "One file name is allowed only."<<endl;
			return 1;
		}else{
			file.open(arg.c_str());
			if(!(file.is_open())){
				cout<<"Can't open file: "<< arg << endl;
				return 1;
			}else{
			in = &file;
			}
		}
	}
    lineNumber=1;
    status=Prog(*in,lineNumber);
    if(!status){
    	cout<<"Program did not execute correctly."<<endl<<"Errors: "<< ErrCount()<<endl;
	}
}
