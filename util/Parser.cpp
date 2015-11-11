#include "Parser.h"
#include <iostream>

#define FILE_NAME "/etc/pamqrcode/challenges.txt"

using namespace std;

Parser::Parser()
{
    //ctor
}

Parser::~Parser()
{
    //dtor
}

vector<Challenge> Parser::parse(){
	vector<Challenge> challenges;
	string username;
	int challenge;
  ifstream myfile (FILE_NAME);
  if (myfile.is_open()){
    while(myfile>>username>>challenge){
			Challenge *ch=new Challenge(username,challenge);
			challenges.push_back(*ch);
	 }//end while
    myfile.close();
  }
	return challenges;
}

