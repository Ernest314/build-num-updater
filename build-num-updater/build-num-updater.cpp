#include "stdafx.h"

#include <vector>
#include <string>
#include <chrono>
#include <sstream>
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::cin;

// Display the help screen text (available flags, etc.).
void print_help();

// Makes sure there is at least 1 passed arg; if not, prints error messages.
bool has_args(int num);

// Parses through the data and prints error messages if needed.
void parse_args(
	int num,
	char* args[],
	std::string& filename,
	bool& isQuiet,
	bool& isVerbose,
	bool& isValid );



int main(int argc, char* argv[])
{
	std::string filename;		// Includes /path/to/file or is relative.
	bool isQuiet = false;
	bool isVerbose = false;
	bool isInputValid = true;	// Check to end the progam after input validation.

	isInputValid = has_args(argc);
	parse_args(argc, argv, filename, isQuiet, isVerbose, isInputValid);
	if (!isInputValid) { return 0; }

	// Open file
	std::vector<std::string> file_lines;

	cout << endl;
    return 0;
}



void print_help()
{
	std::cout << "build-num-updater will increment the version number of a given\n" <<
		"The available options are --file --help --quiet --verbose";
}

bool has_args(int num)
{
	bool has_args = true;
	if (num <= 0) {
		cout << "Congratulations! You triggered an exotic error.\n" <<
			"Error: fewer than 0 arguments passed to program." << endl;
		has_args = false;
	}
	else  if (num == 1) {
		cout << "Error: You must pass at least one parameter.\n" <<
			"Use \"build-num-updater -?\" to view available options." << endl;
		has_args = false;
	}
	return has_args;
}

void parse_args(
	int num,
	char* args[],
	std::string& filename,
	bool& isQuiet,
	bool& isVerbose,
	bool& isValid )
{
	// Start at i=1 because argv[0] contains the name of the program.
	for (int i = 1; i < num; ++i) {
		std::string arg = args[i];
		if (arg == "-?" || arg == "-h" || arg == "--help") {
			print_help();
			isValid = false;
		}
		else if (arg == "-q" || arg == "--quiet") {
			isQuiet = true;
		}
		else if (arg == "-v" || arg == "--verbose") {
			isVerbose = true;
		}
		else if (arg == "-f" || arg == "--file") {
			++i;
			if (i >= num) {
				cout << "Error: File not specified. Please enter the path to\n" <<
					"the file immediately following the \"-f\"/\"--file\" flag." <<
					endl;
				isValid = false;
			}
			filename = args[i];
		}
	}

	if (filename == "") {
		cout << "Error: You must specify a file as a target.\n" <<
			"Use \"build-num-updater -?\" to view available options." <<
			endl;
		isValid = false;
	}
	if (isQuiet && isVerbose) {
		cout <<
			"Error: You cannot specify both quiet mode *and* verbose mode." <<
			endl;
		isValid = false;
	}
}
