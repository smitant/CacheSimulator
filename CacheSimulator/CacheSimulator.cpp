// CacheSimulator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;
struct cacheset {
	bool dirtyBit = false, validBit = false, firstTagReplace = true;
	string tag;
};

//output variables
int writeBacks, writeThroughs, hits, misses;
bool findHit(cacheset * cache, string tag, unsigned int indexBits);

int main()
{
	FILE * outFile;
	FILE * parameterFile;
	FILE * accessesFile;

	string stringLine;
	int directMapped, offsetBits, numIndexBits;
	char * allocationPolicy = new char[2];
	char * writePolicy = new char[2];

	parameterFile = fopen("parameters.txt", "r");
	//parameterFile.open("parameters.txt");
	accessesFile = fopen("accesses.txt", "r");
	outFile = fopen("statistics.txt", "w");
	//outFile.open("statistics.txt");
	//parameter file reading
	if (parameterFile)
	{
		fscanf(parameterFile, "%d %d %d", &directMapped, &offsetBits, &numIndexBits);
		fscanf(parameterFile, "%s", allocationPolicy);
		fscanf(parameterFile, "%s", writePolicy);

		fclose(parameterFile);
	}
	else {
		fprintf(outFile, "Unable to open parameters file");
		return -1;
	}
	//build cache based on parameters

	cacheset * cache1 = new cacheset[pow(2.0, numIndexBits)];
	cacheset * cache2 = NULL;
	if (directMapped == 2) {
		cacheset * cache2 = new cacheset[pow(2.0, numIndexBits)];
		for (int i = 0; i < pow(2.0, numIndexBits); i++) {
			cache2[i].firstTagReplace = 1;
		}
	}



	char readOrWrite;
	unsigned int addressLine;
	//access file processing
	if (accessesFile)
	{
		while (fscanf(accessesFile, "%c %x\n", &readOrWrite, &addressLine) == 2) {
			//shift the address line over by the offset amount. Offset doesnt matter to us
			addressLine = addressLine >> offsetBits;
			unsigned int indexBits = (addressLine << (32 - numIndexBits)) >> (32 - numIndexBits);
			string tag = to_string(addressLine >> numIndexBits);


			if (findHit(cache1, tag, indexBits) == true) {
				hits++;
			}
			else {
				if (cache2 != NULL) {
					if (findHit(cache2, tag, indexBits) == true)
						hits++;
				}
				misses++;
				if (readOrWrite == 'w') {
					//write allocate
					if (writePolicy == "wt") {
						writeThroughs++;
					}
					else {
						//write-back policy(wb)
						//we know we have a miss, so therefor no tags match
						//THIS IS THE LAST PORTION WE NEED BEFORE ITS FINISHED.
						//dont forget to change the dirty bit to 0!!!!
						writeBacks++;
						if (cache2 != NULL) {
							if (cache1[indexBits].firstTagReplace == 0) {
								cache1[indexBits].dirtyBit = 0;
							}
							else {
								cache2[indexBits].dirtyBit = 0;
							}
						}

					}
					//miss occured, if cache2 == null replace in 
					if (allocationPolicy == "wa") {
						//have 2 cahces, check which one hasnt been written to recently
						if (cache2 != NULL) {
							if (cache1[indexBits].firstTagReplace == 0) {
								//then write to that cache and swap the firstReplace tags
								cache1[indexBits].tag = tag;
								cache1[indexBits].validBit = 1;
								cache1[indexBits].dirtyBit = 1;
								//swapppppp
								cache1[indexBits].firstTagReplace = 1;
								cache2[indexBits].firstTagReplace = 0;
							}
							else {
								//write to the second cache instead and swappppp
								cache2[indexBits].tag = tag;
								cache2[indexBits].validBit = 1;
								cache2[indexBits].dirtyBit = 1;
								//swapppppp
								cache2[indexBits].firstTagReplace = 1;
								cache1[indexBits].firstTagReplace = 0;
							}
						}
						//there is no second cache so our first Tag Replace bits dont matter...
						else {
							cache1[indexBits].tag = tag;
							cache1[indexBits].validBit = 1;
							cache1[indexBits].dirtyBit = 1;
						}
					}
					//else its a write-no-allocate (wna)
					else {
						//this if check is to make sure we are not double incrementing because of earlier steps...
						if (writePolicy != "wt") {
							writeThroughs++;
						}
					}


					//set indexed positon's dirty bit to 1

				}
				else {
					//its a read
					//case of 
					if (cache1[indexBits].dirtyBit == 1) {
						writeBacks++;
						//set dirtybit back to 0
						cache1[indexBits].dirtyBit = 0;
					}

				}
			}


		}

		fclose(accessesFile);
	}
	else {
		fprintf(outFile, "Unable to open accesses file");
		return -1;
	}

	//write the variables to the outfile
	double hitRate = hits / misses;
	fprintf(outFile, "hits: %d\nmisses: %d\nhrate: %f\nwb: %d\nwt: %d", hits, misses, hitRate, writeBacks, writeThroughs);
	fcloseall();
	return 0;
}

//This method looks through a cache and determines if we hit or miss.
bool findHit(cacheset * cache, string tag, unsigned int indexBits) {
	if (cache[indexBits].validBit == 0) {
		return false;
	}
	else
	{
		if (cache[indexBits].tag == tag) {
			return true;
		}
	}

}

