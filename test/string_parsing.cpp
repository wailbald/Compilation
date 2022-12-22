#include <string>
#include <fstream>
#include <iostream>

int main()
{

	std::fstream file("test.txt");
	std::string line;
	while(!file.eof())
	{
		getline(file,line);
		std::cout<<line<<std::endl;
	}

	return 0;
}