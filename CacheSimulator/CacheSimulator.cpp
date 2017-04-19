// CacheSimulator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;

int main()
{
	
	ofstream outFile;
	FILE * parameterFile;
	FILE * accessesFile;
	//ifstream inputFile1;
	string stringLine;
	int directMapped, offsetBits, indexBits;
	string allocationPolicy, writePolicy;


	fopen_s(&parameterFile, "parameters.txt", "r");
	fopen_s(&accessesFile, "accesses.txt", "r");
	outFile.open("statistics.txt");
	//parameter file reading
	if (parameterFile)
	{
		fscanf_s(parameterFile, "%d", &directMapped) == 1;
		fscanf_s(parameterFile, "%d", &offsetBits) == 1;
		fscanf_s(parameterFile, "%d", &indexBits) == 1;
		fscanf_s(parameterFile, "%s", &allocationPolicy) == 1;
		fscanf_s(parameterFile, "%s", &writePolicy) == 1;

		fclose(parameterFile);
	}
	else {
		cout << "Unable to open parameters file";
		return -1;
	}
	//build cache based on parameters


	char readOrWrite;
	unsigned int addressLine;
	//access file processing
	if (accessesFile)
	{
		while (fscanf_s(accessesFile, "%c %x", &readOrWrite, &addressLine) == 2) {
			
		}

		fclose(accessesFile);
	}
	else {
		cout << "Unable to open accesses file";
		return -1;
	}
    return 0;
}

