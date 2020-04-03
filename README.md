March2020 branch
Dated 31 March 2020

### File list:

#### Directories:
* cuts - stores all the graphical cuts used in ./include/plotter_X.h (X is a suitable label for the run, here it's Feb2020)
* data - must be set to be a symlink so that input files are found at the location ./data/DAQ/run_XXX/UNFILTERED/compass_run_XXX*.root (XXX is the run number)
* etc - folder to store the channelmap, gainmatch files and the mass_info.txt
* include - where all the important functions used in the program are defined. preprocess.h, eventbuild.h and plotter_X.h are to be updated for each experiment.
* output - stores all the processed output
* src - houses processRun.C which calls the functions defined in ./include, in the order preprocess(), eventbuild() and plotter()

#### Files:
* ./cleanup.sh 101 deletes all output files generated for run number 101
* ./processRun.sh 101 processes run 101 if it exists. Uses rootbuild to generate pdfs from histograms, and uses evince as the pdf reader.
* ./batchProcess.sh XXX YYY processes all runs from XXX to YYY by calling processRun.sh

### Build/Runtime info
* Compile the program by running 'make'
* Tested with root-6.13.08
* Sample output pdfs for ./processRun.sh 102 and ./processRun.sh 90 are stored in ./output/ : ensure that the test data generates matching histograms before adapting the program to a new experiment

### How to adapt this framework to a brand new run
* Generate a suitable channel map for digitizer channels, as etc/SUITABLENAME_channel_map.txt. 
* Generate a suitable gain-match file for SABRE channels, as etc/SUITABLENAME_gainmatch.txt. The filename in 1 and 2 must reflect in preprocess.h and eventbuild.h. Follow the format in etc/Feb2020_gainmatch.txt and etc/Feb2020_channel_map.txt
* Edit preprocess.h to reflect a suitable time offset to add to Scint/SABRE channels, typically 1us, expressed as 1e6ps. Also set a suitable BASKET_SIZE in this file to use more/less RAM as needed within reason to optimize processing speed.
* Edit eventbuild.h to include all necessary runtime parameters and the coincidence window
* Comment out the plotter() function call in processRun.C for now, and run 'make'
* Run ./processRun.sh XXX for a suitable test-run XXX to generate sample builtTree_XXX.root
* Inspect these ROOT trees to generate suitable graphical cut files, and store them in ./cuts
* Edit the plotter_X.h file to draw suitable histograms that process data in builtTree_XXX.root. 
* Remember to uncomment the plotter() function in processRun.C and recompile when done, to test progress in plotter_X.h 
* Repeat the creation of graphical cuts and editing plotter() until a smooth workflow is established.



