/*
Header file with two separate functions:

1. check_new_segments(name, runID, segIDnow)

Function that checks for new .root segments being saved in a compass run. Returns the updated new max segment number if there are new segments saved since the prev call, 
and -1 if there are no new segments, and -2 in case of any incomplete file removal.

2. check_new_output_segments(name, runID, segIDnow)
Function that checks for new output .root segments being saved after eventbuild() is executed. Has the same return scheme as check_new_segments()

----------------------

B.Sudarsan, E.Good, K.Macon
sbalak2@lsu.edu
Fall2019 - Spring 2020
*/
#include <TSystem.h>
#include <TString.h>
#include <iostream>

int segmentlimit = 400;

int check_new_segments(TString runName, int runNumber, int segNumberNow)
{
	int segNumber=0;
	int ret = 0;

	std::cout << "\n------------------";
	std::cout << "\nLooking for compass segments in run # " << runNumber << "...";

	//Look for zeroth segment because it's named different
	ret = system(Form("ls ./data/DAQ/%s_%d/UNFILTERED/compass_%s_%d.root 1> //dev//null 2> //dev//null",runName.Data(),runNumber,runName.Data(),runNumber));
	if(ret==0)
		; //Found segment, continue
	else
	{
		std::cout << "\nNo recorded segments in run# " << runNumber; //If there's no zeroth segment, no data has been saved yet. 
		return -1;
	}

	while(ret==0)
	{
		segNumber = segNumber+1;
		ret = system(Form("ls ./data/DAQ/%s_%d/UNFILTERED/compass_%s_%d_%d.root 1> //dev//null 2> //dev//null",runName.Data(),runNumber,runName.Data(),runNumber,segNumber));
		if(ret==0)
		{
			; // Found, continue
		}
		if(segNumber==segmentlimit) // Prevents infinite loop
		{
			std::cout << "\nReached max limit of segments(=400) to look for in run# " << runNumber << ".. exiting..";
			break;
		}
	}
	if(segNumber >= segNumberNow && segNumber>=0)
	{
		std::cout << "\nNew input segments found until seg# "<<segNumber-1 << std::flush;
		return segNumber;
	}
	else if(segNumber == segNumberNow)
	{
		std::cout << "\nNo new compass segments found!" << std::flush;
		return -1;
	}
	else
	{
		std::cout << "\nError! Check if input segments were removed/deleted from run folder, or if the run name is set correctly."<< std::flush;
		return -2;
	}
}


// Same as the function above, implemented for built files
int check_new_output_segments(TString runName, int runNumber, int segNumberNow)
{
	int segNumber=0;
	int ret = 0;

	std::cout << "\n------------------";
	std::cout << "\nLooking for built output segments corresponding to run number " << runNumber << "...";
	while(ret==0)
	{
		segNumber = segNumber+1;
		ret = system(Form("ls ./output/built_preprocessed_trees/builtFile_%d_%d.root 1> //dev//null 2> //dev//null",runNumber,segNumber));
		if(ret==0)
			;// Found, continue
		if(segNumber==segmentlimit) // Prevents infinite loop
		{
			std::cout << "\nReached max limit of segments(=400) to look for in run# " << runNumber << ".. exiting..";
			break;
		}
	}

	segNumber = segNumber - 1; // Adjust for builtFile_n being the n-1th segment. This helps compare with segNumberNow correctly.

	std::cout << "\n--------------------------";
	if(segNumber >= segNumberNow && segNumber>=0)
	{
		if(segNumber==0) 
		{
			std::cout << "\nRun not analysed yet, starting.. "<<std::flush;
			return segNumber;
		}
		std::cout << "\nNew output segments found until seg# "<< segNumber << std::flush;
		return segNumber;
	}
	else if(segNumber == segNumberNow)
	{
		std::cout << "\nNo new output segments found!" << std::flush;
		return -1;
	}
	else
	{
		std::cout << "\nError! Check if output segments were removed/deleted from run folder, or if the run name is set correctly."<< std::flush;
		return -2;
	}
}
