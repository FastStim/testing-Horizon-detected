#include "main.h"

int main(int argc, char** argv)
{
	string in;
    CommandLineParser parser(argc, argv, "{@input|../file.mp4|input file}{nblocks n|128|number block cov}{resize rs|10|reduce image size}{start s|0|starting position video}{help h| |show help message}");
    parser.about("\033[1;33mFor start program set input file path\033[0m");
    in = parser.get<string>("@input");
    if (parser.has("help") || in == "")
    {   
        parser.printMessage();
        return 0;
    }
    
    int n = parser.get<int>("n");
    int rs = parser.get<int>("rs");
    int s = parser.get<int>("s");

	horizon* hor = new horizon(in, n, rs, s);
    return 0;
}