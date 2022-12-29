#include "lexer.hpp"

std::string read_line(FILE* f)
{
	std::string line;
	int buff=0;
	do{
		buff = fgetc(f);
		if(buff!=(int)'\n' && buff!=(int)'\t')
			line.push_back((char)buff);
	}
	while((buff != EOF ) && (line_terminator.find(buff) == line_terminator.npos));
	if(buff == EOF)
	{
		line.append("EOF");
	}

	return line;
}