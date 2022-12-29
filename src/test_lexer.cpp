#include "lexer.hpp"

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		printf("Error: Wrong number of argument\nUsage: %s \"file to parse\"\n",argv[0]);
		return 1;
	}
	std::string line;
	
	FILE* f = fopen(argv[1],"r");
	do
	{
		line = read_line(f);
		std::cout<<line<<std::endl;
		std::cout<<"-----------------"<<std::endl;
	}
	while(line.find("EOF")==line.npos);
	return 0;
}