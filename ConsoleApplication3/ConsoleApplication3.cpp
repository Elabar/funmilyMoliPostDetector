// ConsoleApplication3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <urlmon.h>
#include <iostream>
#include <string>
#include <fstream>
#include<stdio.h>

using namespace std;

int main()
{
	char webID[10];
	
	bool found = false;
	string potential[200];
	int count = 0;

	char TESTaddress[256] =  "https://apps2.funmily.com/mc";
	ifstream baseFile;
	
	HRESULT hr = URLDownloadToFile(NULL, TESTaddress, "test.html", 0, NULL);

	baseFile.open("test.html");
	string line;
	size_t pos;
	while (!baseFile.eof()) {
		getline(baseFile, line);
		pos = line.find("?action=notice&id=");
		if (pos != string::npos) {
			line = line.substr(pos+18);
			line = line.substr(0,5);
			potential[count] = line;
			count++;
		}
	}
	baseFile.close();
	remove("test.html");

	//convert all IDs to integer type
	int IDs[200];
	for (int i = 0; i < count; i++) {
		char ID[10];
		strcpy_s(ID, potential[i].c_str());
		IDs[i] = atoi(ID);
	}

	//find the largest ID
	int largestID = 0;
	for (int i = 0; i < count; i++) {
		if (IDs[i] > largestID) {
			largestID = IDs[i];
		}
	}

	int startingID = largestID + 1;
	int saveID = startingID;
	int times = 0;

	while (!found) {
		_itoa_s(startingID, webID, 10);
		char address[256] = "https://apps2.funmily.com/mc?action=notice&id=";
		strcat_s(address, webID);

		HRESULT hr = URLDownloadToFile(NULL, address, "result.html", 0, NULL);
		if (hr == S_OK)
		{
			cout << "id: " << webID << "\tSUCCESS" << endl;
			ShellExecute(0, 0, address, 0, 0, SW_SHOW);
			found = true;
		}
		else
		{
			cout << "id: " << webID << "\tFAIL" << endl;
		}
		times++;
		startingID++;
		if (times > 100) {
			startingID = saveID;
			cout << "search over 100 times, retrying from id:" << startingID << endl;
			times = 0;
		}
	}
	
	remove("result.html");
	
	system("pause");
    return 0;
}

