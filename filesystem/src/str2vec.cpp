#include "assign3.h"

std::vector<std::string> str2vec(std::string s, std::string sep)
{
	std::string temp = s;
	std::vector<std::string> ret;
	std::string token;
	size_t pos = 0;
	
	while ((pos = temp.find(sep)) != std::string::npos) {
		token = temp.substr(0, pos);
		ret.push_back(token);
		temp.erase(0, pos + sep.length());
	}
	
	ret.push_back(temp);
	
	return ret;
}