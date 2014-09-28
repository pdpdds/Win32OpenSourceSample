// xlsLibEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <xlslib.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

using namespace xlslib_core;
using namespace std;

#pragma comment(lib, "xlslib_dll.lib")

vector<string> splitLine(const string &line)
{
	vector<string> items;
	string item;
	bool insideQuotes(false);
	bool itemComplete(false);

	for (int i = 0, len = line.length(); i<len; i++) {
		char c = line.at(i);

		if (c == '"') {
			// quote
			if (!insideQuotes && item.length() == 0) {
				// at the beginning
				insideQuotes = true;
			}
			else if (i == len - 1 || line.at(i + 1) == '\t') {
				// at the end
				if (!insideQuotes) {
					item += c;
				}
				i++; // miss tab at the next iteration
				itemComplete = true;
			}
			else if (i != len - 1 && line.at(i + 1) == '"') {
				// before quote
				// two quotes is one real quote
				item += c;
				i++;
			}
			else {
				// in other place
				// this should be exception
				item += c;
			}
		}
		else if (c == '\t') {
			if (insideQuotes) {
				item += c;
			}
			else {
				itemComplete = true;
			}
		}
		else {
			item += c;
		}

		if (itemComplete || i >= len - 1) {
			items.push_back(item);
			item = "";
			insideQuotes = false;
			itemComplete = false;
		}
	}

	return items;
}

int main() {
	
	
	workbook wb;	
	worksheet* sh = wb.sheet("sheet");

	vector< vector<string> > lines;
	ifstream input;
	input.open("test.tsv");
	if (!input) {
		cout << "Open error\n";
		return 1;
	}

	string line;
	while (getline(input, line)) {
		lines.push_back(splitLine(line));
	}
	cout << time(NULL) << "readed\n";

	for (int i = 0, l = lines.size(); i<l; i++) {
		for (int j = 0, s = lines[i].size(); j<s; j++) {
			sh->label(i, j, lines[i][j]);
		}
	}

	//cout << time(NULL) << "created\n";
	//cout << "See memory usage and press enter\n";
	//char c;
	//cin >> c;

	//cout << time(NULL) << "writing\n";
	wb.Dump("Real.xls");
	//cout << time(NULL) << "writed\n";

	return 0;
}