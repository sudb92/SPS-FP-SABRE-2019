/*
preprocess.h

Function that timeshifts SABRE and Scintillator channels forward in time by a fixed offset so that correlated physics events are bunched as narrowly as possible,
offseting propagation delays for correlated pulses to travel through and make it to the digitizer channels. 

Typically, offseting SABRE and Scintillator signals forward by 1us is sufficient, use 1e6 for offset because the raw timestamp units is in ps.

B.Sudarsan, E.Good, K.Macon
sbalak2@lsu.edu
Fall2019-Spring2020

*/



#pragma once
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
#include <TROOT.h>

#include "load_channel_map.h"
#include "DPPchannel.h"
#include "progressbar.h"

#define BASKET_SIZE 4000000000

void preprocess(int runNumber,int segNumber)
{

	UInt_t offset=1e6; // Time offset to shift SABRE, Scint Channels forward by

	//Part 0 : Load channel map
	load_channel_map();

	//gROOT->ProcessLine("gErrorIgnoreLevel = 1001;"); //magically turns off annoying ROOT messages.

	//Part 1 : Open compass output tree, 
	TFile *compassFile;
	if(segNumber==0)
	{ 
		std::cout << "\n Opening segment " << 0 << " in run " << runNumber << "...";
		compassFile =  new TFile(Form("data/DAQ/run_%d/UNFILTERED/compass_run_%d.root",runNumber,runNumber));
		if(!compassFile->IsOpen()) 
		{
			std::cout << "\nFile open error!!";
			return;
		}
		if(compassFile->IsZombie()) 
		{
			std::cout << "\nFile is Zombie. Can't drive without keys! ";
			return;
		}
	}
	else
	{ 
		std::cout << "\n Opening segment " << segNumber << " in run " << runNumber << "..."; 
		compassFile =  new TFile(Form("data/DAQ/run_%d/UNFILTERED/compass_run_%d_%d.root",runNumber,runNumber,segNumber));
		if(!compassFile->IsOpen())
		{
			std::cout << "\nFile open error, not found!";
			return;
		}
		if(compassFile->IsZombie()) 
		{
			std::cout << "\nFile is Zombie. Can't drive without keys! ";
			return;
		}
	}

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

	// Part 2: Open and set branches on the timeshifted tree.
	TFile *sabreShiftedFile = new TFile(Form("output/built_preprocessed_trees/sabshiftedFile_temp.root"),"RECREATE");
	TTree *sabreShiftedTree = new TTree("sabreShiftedTree","Offline Tree with all SABRE timestamps offset, built From raw DPP data");
	sabreShiftedTree->Branch("Energy",&Energy,"Energy/s");
	sabreShiftedTree->Branch("EnergyShort",&EnergyShort,"EnergyShort/s");
	sabreShiftedTree->Branch("Timestamp",&Timestamp_shifted,"Timestamp_shifted/l");
	sabreShiftedTree->Branch("Channel",&Channel,"Channel/s");
	sabreShiftedTree->Branch("Board",&Board,"Board/s");
	sabreShiftedTree->Branch("Flags",&Flags,"Flags/i");
	int counterSi = 0;
	int counterAllElse = 0;

	compassTree->SetMaxVirtualSize(BASKET_SIZE);
	compassTree->LoadBaskets(BASKET_SIZE);

	int i=0;
	std::cout<<std::endl;
	while(i < nentries)
	{
		compassTree->GetEntry(i);
		int chan = Board*16+Channel; 
		if(0)
//		if((sps_channelmap[chan].side_pos.first == "FRONT")||(sps_channelmap[chan].side_pos.first == "BACK"))// || (sps_channelmap[chan].side_pos.first == "SCINT") || (sps_channelmap[chan].side_pos.first == "SCINT")  )
		{
			Timestamp_shifted = Timestamp + offset;
			counterSi++;
		}
		else
		{
			counterAllElse++;
			Timestamp_shifted = Timestamp;
		}
//		if((Board==8 && Channel==0)||(Board==8 && Channel==1))//Hand-written shift added to scintillator
//			Timestamp = Timestamp + offset;

		sabreShiftedTree->Fill(); //Write all elements,offset timestamps up for only the fronts/backs in Si
		i++;
		if(i%100==0) progressbar((double)i,(double)nentries);
	}
	std::cout << std::endl << "Counters(Si,allelse): " << counterSi << ", " << counterAllElse << std::endl;

	//Part 3: Sort the shifted tree in timestamp, and save it to the output tree
	TFile *preProcessedTreeOutput = new TFile(Form("output/built_preprocessed_trees/preProcessedTree_%d_%d.root",runNumber,segNumber),"RECREATE");
	TTree *preProcessedTree = new TTree("preProcessedTree","Offline Tree with all SABRE timestamps offset and then sorted, built From raw DPP data");
	preProcessedTree->Branch("Energy",&Energy,"Energy/s");
	preProcessedTree->Branch("EnergyShort",&EnergyShort,"EnergyShort/s");
	preProcessedTree->Branch("Timestamp",&Timestamp_shifted,"Timestamp_shifted/l");
	preProcessedTree->Branch("Channel",&Channel,"Channel/s");
	preProcessedTree->Branch("Board",&Board,"Board/s");
	preProcessedTree->Branch("Flags",&Flags,"Flags/i");

	//Reference for sorting : https://root-forum.cern.ch/t/sorting-a-tree/12560/4
	std::cout << "Sorting shifted Tree ..." << std::endl;
	sabreShiftedTree->BuildIndex("0","Timestamp");   // order tree by Timestamp... uses std::sort, of O(NlogN) complexity
	std::cout << "Done sorting" << std::endl;
	TTreeIndex *I = (TTreeIndex*)sabreShiftedTree->GetTreeIndex();
	Long64_t* index2 = I->GetIndex();
	nentries = sabreShiftedTree->GetEntries();

	sabreShiftedTree->SetMaxVirtualSize(BASKET_SIZE);
	sabreShiftedTree->LoadBaskets(BASKET_SIZE);

	std::cout << "Saving sorted tree..\n";
	i=0;
	while(i < nentries)
	{
		sabreShiftedTree->GetEntry(index2[i]);
		preProcessedTree->Fill();
		i++;
		if(i%100==0) progressbar((double)i,(double)nentries);
	}
	//   std::cout << "\nDone." << std::endl;
	preProcessedTree->Write();


	preProcessedTreeOutput->Close();
	compassFile->Close();

}
