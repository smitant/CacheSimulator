// CacheSimulator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;
struct cacheset {
	bool dirtyBit = false, validBit = false, firstTagReplace = true;
	string tag;
};

int main()
{
	

	ofstream outFile;
	//ifstream parameterFile;
	FILE * parameterFile;
	FILE * accessesFile;
	
	string stringLine;
	int directMapped, offsetBits, numIndexBits;
	string allocationPolicy, writePolicy;


	fopen_s(&parameterFile, "parameters.txt", "r");
	//parameterFile.open("parameters.txt")
	fopen_s(&accessesFile, "accesses.txt", "r");
	outFile.open("statistics.txt");
	//parameter file reading
	if (parameterFile)
	{
		fscanf_s(parameterFile, "%d", &directMapped);
		fscanf_s(parameterFile, "%d", &offsetBits);
		fscanf_s(parameterFile, "%d", &numIndexBits);
		fscanf_s(parameterFile, "%s", &allocationPolicy);
		fscanf_s(parameterFile, "%s", &writePolicy);

		fclose(parameterFile);
	}
	else {
		cout << "Unable to open parameters file";
		return -1;
	}
	//build cache based on parameters
	
	cacheset * cache1 = new cacheset[pow(2.0, numIndexBits)];
	if (directMapped == 2) {
		cacheset * cache2 = new cacheset[pow(2.0, numIndexBits)];
	}



	char readOrWrite;
	unsigned int addressLine;
	//access file processing
	if (accessesFile)
	{
		while (fscanf_s(accessesFile, "%c %x", &readOrWrite, &addressLine) == 2) {
			//shift the address line over by the offset amount. Offset doesnt matter to us
			addressLine = addressLine >> offsetBits;
			unsigned int indexBits = (addressLine << (32 - numIndexBits)) >> (32 - numIndexBits);
			unsigned int tag = addressLine >> numIndexBits;
			//findHit(cache1, readOrWrite, tag, indexBits);

		}

		fclose(accessesFile);
	}
	else {
		cout << "Unable to open accesses file";
		return -1;
	}
    return 0;
}

//This method looks through a cache and determines if we hit or miss.
bool findHit(cacheset * cache, char readOrWrite, unsigned int tag, unsigned int indexBits) {

	return false;
}

