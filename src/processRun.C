/*
processRun.C

generates an executable procesRun that takes the run# to process as its first argument.
Use ./processRun  <runID> to process all events in runs titled "run".with run number <runID>.

ex: 
for <runID> =  91, input file segments look like compass_run_91.root, compass_run_91_1.root, compass_run_91_2.root etc. 

B.Sudarsan, E.Good, K.Macon
sbalak2@lsu.edu
Fall2019-Spring2020

*/
#include "check_new_segments.h"
#include "plotter_Feb2020.h"
#include "preprocess.h"
#include "eventbuild.h"
#include <iostream>

int main(int argc, char* argv[])
{
	bool skip_evb = false;  //Set to true to only generate new plots, without eventbuilding
	bool reprocess = false; //Set to true to only process additional segments without starting from the zeroth segment in a particular runID.

	int runID = atoi(argv[1]);
	int max_segments_found_compass = check_new_segments("run",runID,0);
	int max_segments_found_output = check_new_output_segments("run",runID,0);


	std::cout << "\n# of compass segs found: " << max_segments_found_compass;
	std::cout << "\nMax built segment index found: " << max_segments_found_output;

	bool error =  (max_segments_found_compass<0)||(max_segments_found_output<0);
	int start;

	int retval;

	if(reprocess)
		start = 0;
	else 
		start = max_segments_found_output;

	if(skip_evb)
	{
		//Delete all old plots and .root files of plots
		retval = system(Form("rm  output/histogram_root/combined_hist_run%d_seg%d_to_%d.root ",runID,0,max_segments_found_output));
		retval = system(Form("rm  output/histogram_pdfs/combined_hist_run%d_seg%d_to_%d.pdf ",runID,0,max_segments_found_output));

		for(int j=0; j<=max_segments_found_output; j++)
			retval = system(Form("rm output/histogram_root/histograms_run=%d,seg=%d.root  ",runID,j));

		//Also force to start from 0 so that all segments get combined		
		start = 0;
	}


	for(int j=start; j<max_segments_found_compass && !error; j++)
	{
		std::cout << "\n****processing One segment" << std::endl;

		std::cout << "\n*********************************************";
		std::cout << "\nTimeshifting SABRE in run#" << runID <<  " seg#" << j << " ...." << std::flush;
	
		if(!skip_evb)
			preprocess(runID,j);
	
		std::cout << "\n******************";
		std::cout << "\nEventbuilding in run#" << runID	 <<  " seg#" << j << " ...." << std::flush;
		if(!skip_evb)	
			eventbuild(runID,j);

		plotter(runID,j,skip_evb);

	}

	std::cout << "\nDone."<< max_segments_found_compass-1 << std::endl << std::flush;
	return max_segments_found_compass-1;
}
