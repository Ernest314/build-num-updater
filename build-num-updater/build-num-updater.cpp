#include "stdafx.h"

#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iostream>
#include <fstream>

using std::vector;
using std::string;

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
	string& filename,
	bool& isQuiet,
	bool& isVerbose,
	bool& isValid );

// Convert an int to std::string and pad with leading zeroes.
string pad_to_string(unsigned int x, unsigned int min_len);

// Convert std::time_t to std::tm (temporarily disables C4996).
std::tm time_t_to_tm(std::time_t input);



int main(int argc, char* argv[])
{
	string filename;		// Includes /path/to/file or is relative.
	bool isQuiet = false;
	bool isVerbose = false;
	bool isInputValid = true;	// Check to end the progam after input validation.

	isInputValid = has_args(argc);
	parse_args(argc, argv, filename, isQuiet, isVerbose, isInputValid);
	if (!isInputValid) { return 0; }

	// Read file
	vector<string> file_lines;
	std::ifstream file_in(filename);
	string buf;
	if (file_in.is_open()) {
		while (getline(file_in, buf)) {
			file_lines.push_back(buf);
		}
	}
	file_in.close();

	// Write file
	std::ofstream file_out(filename, std::ios::trunc);
	int build_num = 0;
	for (unsigned int i = 0; i < file_lines.size(); ++i) {
		string key;
		string val;
		std::stringstream line(file_lines[i]);
		getline(line, key, ':');
		getline(line, val);
		if (key == "build") {
			build_num = std::stoi(val);
			build_num++;
			val = pad_to_string(build_num, 4); // MAGIC_NUM
			file_lines[i] = key + ": " + val;
		}
		if (key == "build-date") {
			using namespace std::chrono;
			std::time_t build_time_t = system_clock::to_time_t(system_clock::now());
			std::tm build_time = time_t_to_tm(build_time_t);
			val = string(
				std::to_string(1900 + build_time.tm_year) + "-" +
				pad_to_string(build_time.tm_mon, 2) + "-" +
				std::to_string(build_time.tm_mday) );
			file_lines[i] = key + ": " + val;
		}
		if (key == "build-time") {
			using namespace std::chrono;
			std::time_t build_time_t = system_clock::to_time_t(system_clock::now());
			std::tm build_time = time_t_to_tm(build_time_t);
			val = string(
				pad_to_string(build_time.tm_hour, 2) + ":" +
				pad_to_string(build_time.tm_min, 2) + ":" +
				pad_to_string(build_time.tm_sec, 2) );
			file_lines[i] = key + ": " + val;
		}

		// write to file here
		file_out << file_lines[i] << endl;
		cout << file_lines[i] << endl;
	}
	file_out.close();

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

string pad_to_string(unsigned int x, unsigned int min_len)
{
	string x_str = std::to_string(x);
	int pad_len = 0;
	if (x_str.size() < min_len) {
		pad_len = min_len - x_str.size();
	}
	return string(pad_len, '0') + x_str;
}

std::tm time_t_to_tm(std::time_t input)
{
#pragma warning(push)
#pragma warning(disable: 4996)
	std::tm output = *(std::gmtime(&input));
#pragma warning(pop)
	return output;
}
