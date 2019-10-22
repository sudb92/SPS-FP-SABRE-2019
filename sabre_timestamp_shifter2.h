#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TTreeReader.h>
#include <TTreeIndex.h>
#include <TTreeReaderValue.h>
#include <TCanvas.h>
#include <TMath.h>
#include <iostream>
#include <vector>
#include <cmath>

#ifndef _ALL_ANALYSIS_HDR_FILES
#define _ALL_ANALYSIS_HDR_FILES
#include "progressbar.h"
#include "test_map.h"
#include "DPPchannel.h"
#include "FocalPlane_SABRE.h"
#include "combiner.h"
#endif

//Version 2 tries to handle the chaining of multiple segments
void sabre_timestamp_shifter2(int runNumber,int segNumber)
{
//   int runNumber = 317;//til 322
//   int segNumber = 1;


   
//   combine_segments(runNumber,5);
//   TFile *compassFile =  new TFile(Form("./data/DAQ/run_%d/UNFILTERED/compass_run_combined_%d.root",runNumber,runNumber));

  TFile *compassFile;

  if(segNumber==0)
 { std::cout << "\n Opening segment " << 0 << " in run " << runNumber << "...";
   compassFile =  new TFile(Form("data/DAQ/run_%d/UNFILTERED/compass_run_%d.root",runNumber,runNumber));
   if(!compassFile->IsOpen()) std::cout << "\nFile open error!!";
 }
  else
 { 
   std::cout << "\n Opening segment " << segNumber << " in run " << runNumber << "..."; 
   compassFile =  new TFile(Form("data/DAQ/run_%d/UNFILTERED/compass_run_%d_%d.root",runNumber,runNumber,segNumber));
   if(!compassFile->IsOpen()) std::cout << "\nFile open error!!";
 }

   test_map();
   UInt_t offset = 1e6; //Offset in ps

   TTree *compassTree = static_cast<TTree*>(compassFile->Get("Data"));

   UShort_t Energy, EnergyShort;
   ULong64_t Timestamp;
   ULong64_t Timestamp_shifted;
   UShort_t Channel, Board; 
   UInt_t Flags;

   compassTree->SetBranchAddress("Energy",&Energy);
   compassTree->SetBranchAddress("EnergyShort",&EnergyShort);
   compassTree->SetBranchAddress("Timestamp",&Timestamp);
   compassTree->SetBranchAddress("Channel",&Channel);
   compassTree->SetBranchAddress("Board",&Board);
   compassTree->SetBranchAddress("Flags",&Flags);


   long int nentries = compassTree->GetEntries();

   TFile *sabreShiftedFile = new TFile(Form("output/sabreShiftedFile(testing,combined)_%d_%d.root",runNumber,segNumber),"RECREATE");
   TTree *sabreShiftedTree = new TTree("sabreShiftedTree","Offline Tree with all SABRE timestamps offset, built From raw DPP data");

   sabreShiftedTree->Branch("Energy",&Energy,"Energy/s");
   sabreShiftedTree->Branch("EnergyShort",&EnergyShort,"EnergyShort/s");
   sabreShiftedTree->Branch("Timestamp",&Timestamp_shifted,"Timestamp_shifted/l");
   sabreShiftedTree->Branch("Channel",&Channel,"Channel/s");
   sabreShiftedTree->Branch("Board",&Board,"Board/s");
   sabreShiftedTree->Branch("Flags",&Flags,"Flags/i");

   int counterSi = 0;
   int counterAllElse = 0;
   compassTree->BuildIndex("Timestamp");
   int i=0;
   while(i < nentries)
   {
       	compassTree->GetEntry(i);
	int chan = Board*16+Channel;
     
        if((map1[chan].side_pos.first == "FRONT")||(map1[chan].side_pos.first == "BACK"))
//	if(Board!=2)
	{
		Timestamp_shifted = Timestamp + offset;
		counterSi++;
        if(runNumber==330)
			Energy = 2*Energy;
	}
	else
	{
		counterAllElse++;
		Timestamp_shifted = Timestamp;
	}

	if(Board==2 && Channel==0)
		Timestamp = Timestamp + 0.5e6;
		
//	std::cout << "\nBoard: " << Board << " Channel:" << Channel << map1[chan].side_pos.first;
	sabreShiftedTree->Fill(); //Write all elements,offset timestamps up for only the fronts/backs in Si
	progressbar((double)i,(double)nentries);
    i++;
   }
   std::cout << std::endl << "Counters: " << counterSi << ", " << counterAllElse << std::endl;
   sabreShiftedTree->Write();
//   compassTree->Print();
//   sabreShiftedTree->Print();


   sabreShiftedFile->Close();
   compassFile->Close();

}
