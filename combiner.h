/*
Function that combines built tree segments belonging to a single CoMPASS run. 
It saves the combined tree to a new tree of the form compass_run_combined_<runID>.root

sbalak2@lsu.edu
Sudarsan.B
Oct 2019


*/
#include <TChain.h>
#include <TSystem.h>


int combine_segments(int runNumber, int segmentlimit=-1)
{
//   int runNumber = 319;
	int segNumber;
   //TFile *compassFile =  new TFile(Form("data/DAQ/run_%d/UNFILTERED/compass_run_%d.root",runNumber,runNumber));
   TChain* ch = new TChain("Data");
   TSystem* gsy;
   int ret = 0;
//   TString ret = "1";
   int number_of_runs;


  segNumber = 1;

  std::cout << "\nLooking for additional segments corresponding to run number " << runNumber;

   ret = system(Form("ls ./data/DAQ/run_%d/UNFILTERED/compass_run_%d.root 1> //dev//null 2> //dev//null",runNumber,runNumber));
   if(ret==0)
   		ch->Add(Form("./data/DAQ/run_%d/UNFILTERED/compass_run_%d.root",runNumber,runNumber),-1);   

   while(ret==0)
   {
	   ret = system(Form("ls ./data/DAQ/run_%d/UNFILTERED/compass_run_%d_%d.root 1> //dev//null 2> //dev//null",runNumber,runNumber,segNumber));
	   if(ret==0)
	   {
	   		ch->Add(Form("./data/DAQ/run_%d/UNFILTERED/compass_run_%d_%d.root",runNumber,runNumber,segNumber),-1);   
	        std::cout << "\nFound segment #" << segNumber;
	   }
	//   std::cout << "\nRet value: " << ret << " " << segNumber << std::flush;// << " " << buffer.Data();
       if(segmentlimit>0 && segNumber==segmentlimit) break;
 
	   segNumber = segNumber+1;
   }
   std::cout << "\nCombined tree size::" << ch->GetEntries();

   std::cout << "\nWriting combined tree.." << std::flush;
   TFile *file = TFile::Open(Form("./data/DAQ/run_%d/UNFILTERED/compass_run_combined_%d.root",runNumber,runNumber),"RECREATE"); 
  //If 'option' contains the word 'fast' and nentries is -1, the cloning will be done without unzipping or unstreaming the baskets (i.e., a direct copy of the raw bytes on disk).
   ch->CloneTree(-1,"fast"); //copy ALL bytes, without any unzipping
   file->Write();
   delete file;
   delete ch;
   std::cout << "done.\n";


//   ch.SaveAs(,"");
//   ret = gSystem->Exec(Form("touch test.dat"));

   return 0;
}
