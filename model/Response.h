#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>

using namespace std;

class Response
{
    public:
         Response();
       	virtual ~Response();
			Response(string username,string pc_name,int response);
			string getUsername();
			int getResponse();
    protected:
    private:
		string username;
		string pc_name;
		int response;
};

#endif // RESPONSE_H

