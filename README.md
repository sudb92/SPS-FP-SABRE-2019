# CompassCoincidence
Collection of ROOT macros that post-process SABRE+FocalPlane data from FSU Enge SPS 

# Particular macros in the folder:
1. **sabre_timestamp_shifter.C**
Reads through the compass tree and shifts only the timestamps of sabre events forward by a set amount, so that they happen after the anode event which triggers eventbuilding in analyseDPP_fp_sabre3.C 
<>
2. **analyseDPP_fp_sabre3.C**
Macro which looks through the (sabre-time-shifted) data tree and builds events into an output ROOT file starting with an anode event. 
Uses:
* **DPPChannel.h**  : for Compass raw event-class definition
* **FocalPlane_SABRE.h** : to define all components of a built output event (including all FP signals + max front/back SABRE signals)
* **test_map.h** : to define the map (as defined in a file of the template **ChannelMapSabre.dat** to identify which SABRE channels belong to which Si detector, on what side, and at what position)
* **progressbar.h** : to plot a progressbar as the program moves forward building events

3. **onlineScript2.c** : Rough and ready plotting of finished events. Defines cuts as saved in the folder **/cuts** 
4. **AnalyzeBuiltTree.c** : An attempt at a cleaned up version of (3) with better labelled plots
