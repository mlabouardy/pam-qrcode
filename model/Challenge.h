#ifndef CHALLENGE_H
#define CHALLENGE_H

#include <string>

using namespace std;

class Challenge
{
    public:
         Challenge();
       	virtual ~Challenge();
			Challenge(string username,int challenge);
			string getUsername();
			int getChallenge();
    protected:
    private:
		string username;
		int challenge;
};

#endif // CHALLENGE_H

