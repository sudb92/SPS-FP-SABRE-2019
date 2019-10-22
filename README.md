# CompassCoincidence
Collection of ROOT macros that post-process SABRE+FocalPlane data from FSU Enge SPS 

# Particular macros in the folder:
1. **sabre_timestamp_shifter2.C**
Reads through the compass tree and shifts only the timestamps of sabre events forward by a set amount, so that they happen after the anode event which triggers eventbuilding in analyseDPP_fp_sabre3.C 
Compilable macro that's called by onlineScript4.C
<>
2. **analyseDPP_fp_sabre3.C**
Macro which looks through the (sabre-time-shifted) data tree and builds events into an output ROOT file starting with an anode event. 
Compilable macro called by onlineScript4.C
Uses:
* **DPPChannel.h**  : for Compass raw event-class definition
* **FocalPlane_SABRE.h** : to define all components of a built output event (including all FP signals + max front/back SABRE signals)
* **test_map.h** : to define the map (as defined in a file of the template **ChannelMapSabre.dat** to identify which SABRE channels belong to which Si detector, on what side, and at what position)
* **progressbar.h** : to plot a progressbar as the program moves forward building events
* **ChannelMapSabre.dat**: as a reference for the channelmap format
* **runXXX-sabre-gainmatch** : gain-matching files
* **UserInput.c, UserInput3.c** : step through all sabre singles spectra by channel number, and identify peaks in them. The former steps automatically, the latter takes a spacebar keypress to step channels while finding a peak in the chosen window.


3. **onlineScript4.c** : Uses cuts in **/cuts** . A good, strong plotter that can parse a specified range of input compass files, and timeshift, eventbuild and plot them.
While eventbuilding on each segment, the eventbuilder part plots diagnostic histograms. The main plots are 6 in number at the end, and would combine all analysed segments chosen.
4. **AnalyzeBuiltTree.c** : An attempt at a cleaned up version of (3) with better labelled plots
