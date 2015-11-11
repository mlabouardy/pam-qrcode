#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "../model/Challenge.h"
#include <vector>
#include <fstream>
using namespace std;

class Parser
{
    public:
        Parser();
        virtual ~Parser();
		  vector<Challenge> parse();
    protected:
    private:
};

#endif // PARSER_H

