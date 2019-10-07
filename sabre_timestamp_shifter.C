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
#include "DPPchannel.h"
#include "FocalPlane_SABRE.h"
#include "test_map.h"
#include <vector>
#include "progressbar.h"
#include <cmath>
void sabre_timestamp_shifter()
{
   int runNumber = 319;
   TFile *compassFile =  new TFile(Form("data/DAQ/run_%d/UNFILTERED/compass_run_%d.root",runNumber,runNumber));
//   if(!compassFile->IsOpen()) std::cout << "\nFile open error!!";
   test_map();
   UInt_t offset = 1e6; //Offset in ps

   TTree *compassTree = static_cast<TTree*>(compassFile->Get("Data"));

   UShort_t Energy,EnergyShort;
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

   TFile *sabreShiftedFile = new TFile(Form("output/sabreShiftedFile(testing)_%d.root",runNumber),"RECREATE");
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
   std::cout << std::endl << "Counters: " << counterSi << ", " << counterAllElse;
   sabreShiftedTree->Write();
   compassTree->Print();
   sabreShiftedTree->Print();
   sabreShiftedFile->Close();
   compassFile->Close();


}
