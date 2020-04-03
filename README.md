Feb2020 branch, pushed last by Sudarsan



Changes made:
	- change all of preprocess(), eventbuild() and plotter() to have them live in .h files
	- processRun.C is compiled using the Makefile to generate an executable
	- executable is called by ./processRun.sh <runID> which takes care of calls to hadd, rootprint, and evince
	- calls to progressbar() have been made sparser by 1/100, since upon profiling this was found to be expensive
	- Overall formatting of code, addition of more comments, in particular for check_new_segments()
	- ./cleanup.sh <runID> gets rid of everything generated by the particular run number
	- sample data analysis results stored in ./output/*.pdf for comparison, for test runs

To do in future:
	- Incorporate boost_program_options or another library to use command-line arguments
	- Change preprocess(), eventbuild() and plotter() to their own independent executables
	- Implement a better structure for the project overall
	- Inspect arrival timing of different signals within the coincidence window so as to set limits
	  on possible dead-times caused by the eventbuilding logic. 

Dated 13 March 2020