#include "Response.h"

Response::Response(){
    //ctor
}

Response::~Response(){
    //dtor
}



Response::Response(string username,string pc_name,int response){
	this->username=username;
	this->pc_name=pc_name;
	this->response=response;
}


string Response::getUsername(){
	return username;
}

int Response::getResponse(){
	return response;
}
