#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "stats.hpp"

void printTop();
void printBottom();
void readQueryString(char []);
void createRandomName(char []);

int main()
{
    char card[512];
    char iofile[13];
    readQueryString(card);
    std::cout << "Content-Type:text/html;charset=US-ASCII\n\n";
    printTop();

    // initial pass - print the form
    if (strcmp(card, "step1") == 0) {
        std::cout << "<form action=\"textan.cgi?step2\" method=\"post\" enctype=\"multipart/form-data\">";
        std::cout << "<input type=\"file\" name=\"textfile\"><input type=\"submit\" value=\"Upload\"></form>";
    }
    // parse the form and analyze the text
    else if (strcmp(card, "step2") == 0) {
        // read past cgi control data
        std::string line;
        getline(std::cin, line);
        getline(std::cin, line);
        getline(std::cin, line);
        getline(std::cin, line);
        getline(std::cin, line);
        // write the csv lines to the data file
        createRandomName(iofile);
        std::fstream sequential;
        sequential.open(iofile, std::ios::out);
        sequential << line << std::endl;
        while (getline(std::cin, line)) {
            if (line.substr(0, 1) == "--") break;
            sequential << line << std::endl;
        }
        sequential.close();
        TextAnalyzer textfile(iofile);
        textfile.displayTable();
        std::cout << "\n <div>textfile</div>";
        std::cout << "\n <div>n = " << textfile.getSize() << "</div>";
        textfile.printReport();
        std::cout << "\n <div>index of coincidence = " << textfile.getIoc() << "</div>";
        std::cout << "\n <div>english correlation = " << textfile.getCorr() << "</div>";
        remove(iofile);
    }
    printBottom();
    return 0;
}

void printTop()
{
    std::cout << "<!DOCTYPE HTML>";
    std::cout << "\n<html>";
    std::cout << "\n<head>";
    std::cout << "\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
    std::cout << "\n<link rel=\"stylesheet\" media=\"all\"  href=\"/includes/style.css\">";
    std::cout << "\n<title>CGI C++ English Text Analyzer</title>";
    std::cout << "\n</head>";
    std::cout << "\n<body>";
    std::cout << "\n<header><p>CGI C++ English Text Analyzer</p></header>";
    std::cout << "\n<div><a href=\"/index.php\">Home</a> | <a href=\"/textan.html\">Back</a></div>";
}

void printBottom()
{
    std::cout << "\n<div><a href=\"/index.php\">Home</a> | <a href=\"/textan.html\">Back</a></div>";
    std::cout << "\n<footer><p>CopyLeft 2017 Josh Roybal - all wrongs reserved</p></footer>";
    std::cout << "\n</body>";
    std::cout << "\n</html>";
}    

void readQueryString(char card[])
{
    strcpy(card, getenv("QUERY_STRING"));
}

void createRandomName(char filename[])
{
   int seed = time(NULL);
   if (seed % 2 == 0) ++seed;
   srand(seed);
   memset(filename, '\0', 13);
   for (int i = 0; i < 8; i++) filename[i] = char(97 + rand() % 26);
   strcat(filename, ".txt");
}
