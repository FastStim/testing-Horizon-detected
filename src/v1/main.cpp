#include "main.h"

int main(int argc, char** argv)
{
	string in;
    CommandLineParser parser(argc, argv, "{@input|../file.mp4|input file}{help h| |show help message}");
    parser.about("\033[1;33mFor start program set input file path\033[0m");
    in = parser.get<string>("@input");\

    if (parser.has("help") || in == "")
    {	
        parser.printMessage();
        return 0;
    }
    
	horizon* hor = new horizon(in);
    return 0;
}