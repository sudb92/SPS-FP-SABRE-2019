#!/bin/python

import os
runID = 92


pathtodata = "./data/DAQ/"
outputfile = "compass_run_"+str(runID)+".root"


boardNumArray = ["1725_323","1725_324","1725_325","1725_334","1725_336", "1725_379", "1725_405", "1730_82", "1730_89"];
#boardNumArray = ["1730_89","1725_324","1725_325","1725_334","1725_336", "1725_379", "1730_80", "1730_82", "1730_89"];
callthis = "/home/sud/binconverter/software/analysisCode/build/CoMPASSbin2ROOT "


for board in boardNumArray:
	for ch in range(0,16):
		#print board,ch
		string_to_add = " -i "+pathtodata+"run_"+str(runID)+"/UNFILTERED/CH"+str(ch)+"@V"+board+"_Data_run_"+str(runID)+".bin "
		callthis = callthis + string_to_add
		


callthis = callthis + " -o "+pathtodata+"run_"+str(runID)+"/UNFILTERED/"+outputfile
callthis = callthis + " --numberofboards 9 --numberofchannels 16"
	
'''

 -i /home/splitpole/Compass/191104_SABRE_PHA_3links/DAQ/run_439/UNFILTERED/CH_13@V1730_89_Data_run_439.bin -i /home/splitpole/Compass/191104_SABRE_PHA_3links/DAQ/run_439/UNFILTERED/CH_15@V1730_89_Data_run_439.bin 
 
 
 -o output.root --numberofboards 9 --numberofchannels 16

'''
print callthis
os.system(callthis)
#os.system("ls -l")
#print 'testing'

