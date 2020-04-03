/*
eventbuild.h

Header file building events by looking through time sorted compass raw data.
The builder scans through all data, and divides them up into 'clumps' of a constant specified width - the coincidence window.
The division is not done in uniformly spaced clumps because data arrives at random but the width stays constant. 

The function also writes a few test histograms that keep track of the arrival time difference histograms within sabre.

B.Sudarsan,  E.Good, K.Macon
Fall2019-Spring2020
sbalak2@lsu.edu
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

#include "progressbar.h"
#include "load_channel_map.h"
#include "DPPchannel.h"
#include "BuiltEvent.h"
#include "FP_kinematics.h" //Thanks, Ken & Gordon!

#define BASKET_SIZE 4000000000

double target_Z = 13;
double target_A = 27;
double beam_A = 3;
double beam_Z = 2; 
double beam_E = 24; //in MeV
double ejectile_A = 3; //
double ejectile_Z = 1; //
double angle = 3.0;//in degrees
double B_field = 12150;//in gauss

int coincidenceWindow = 3.0 * 1e6; // Microseconds to Picoseconds

// Channel Map definitions for reference
const UShort_t scintRchan = 0+16*8;
const UShort_t scintLchan = 1+16*8;

const UShort_t delayFLchan = 8+16*8;
const UShort_t delayFRchan = 9+16*8;

const UShort_t delayBLchan = 10+16*8;
const UShort_t delayBRchan = 11+16*8;
const UShort_t anodeMFchan = 13+16*8;
const UShort_t anodeBFchan = 14+16*8;
const UShort_t anodeMBchan = 15+16*8;

const UShort_t cathodechan = 7+16*8;


void eventbuild(int runNumber, int segNumber) 
{
	//Part 1: Read in gain-matching information.

	// GAINMATCH prep begin
	bool gainmatch = true;
	double sabre_gainmatch[144] ;
	ifstream in;
	in.open("etc/Feb2020_gainmatch.txt");
	if(!in.is_open())
	{
		std::cerr << "Error opening gainmatch file ";// << in << " ..." << std::endl;
		return;
	}
	//Gain match file has contents organized as
	// ch   peak_position  gain_value
	int ch_1;
	double temp1, temp2;
	std::cout << "Reading gain file.." << std::endl;
	while(!in.eof())
	{
		in>>ch_1>>temp1>>temp2;
		sabre_gainmatch[ch_1] = temp2;
	}
	in.close();
	//GAINMATCH prep end


	//Load the channel map for all channels
	load_channel_map();

	TFile *histograms_builder = new TFile(Form("./output/histogram_root/histograms_builder_run=%d,seg=%d.root",runNumber,segNumber),"RECREATE");
	static TH1D *dtsabreanode = new TH1D("sabreminusanode_dt","Sabre front anode front dt in ns",240,-coincidenceWindow/1.e3 - 400,coincidenceWindow/1.e3 + 400);
	static TH1D *tcheckFront = new TH1D("tcheckFront","Focal Plane Detector Checking (front plane)",240,-coincidenceWindow/1.e3 - 400,coincidenceWindow/1.e3 + 400);
	static TH1D *tcheckBack = new TH1D("tcheckBack", "Focal Plane Detector Checking (back plane)",240,-coincidenceWindow/1.e3 - 400,coincidenceWindow/1.e3 + 400);
	static TH1D *anodeScintTime = new TH1D("anodeScintTime", "Anode / Scintillator Coincidence Time",240,-coincidenceWindow/1.e3 - 400,coincidenceWindow/1.e3 + 400);
	std::vector<std::pair<int,double>> synclist;
	std::vector<std::pair<int,double>>::iterator synciter;

	// Open the file containing the CoMPASS tree.
	TFile *compassFile = TFile::Open(Form("output/built_preprocessed_trees/preProcessedTree_%d_%d.root",runNumber,segNumber));
	TTree *compassTree;
	if(compassFile->IsOpen())
	{
		std::cout << "\nFound shifted file, opening.." ;
		compassFile->Print();
		compassTree = static_cast<TTree*>(compassFile->Get("preProcessedTree"));
	}
	else
	{
		std::cout << "\nNo shifted file found, trying to open the raw compass file.." ;
		compassFile->Print();
		compassFile = TFile::Open(Form("data/DAQ/run_%d/UNFILTERED/compass_run_%d_%d.root",runNumber,runNumber,segNumber));
		if(compassFile->IsZombie()) 
		{
			std::cout << "\nFile is Zombie. Can't drive without keys! ";
			return;
		}
		compassTree = static_cast<TTree*>(compassFile->Get("Data"));
	}

	UShort_t Energy,EnergyShort;
	ULong64_t Timestamp;
	UShort_t Channel, Board; 
	UInt_t Flags;

	compassTree->SetBranchAddress("Energy",&Energy);
	compassTree->SetBranchAddress("EnergyShort",&EnergyShort);
	compassTree->SetBranchAddress("Timestamp",&Timestamp);
	compassTree->SetBranchAddress("Channel",&Channel);
	compassTree->SetBranchAddress("Board",&Board);
	compassTree->SetBranchAddress("Flags",&Flags);

	compassTree->SetMaxVirtualSize(BASKET_SIZE);
	compassTree->LoadBaskets(BASKET_SIZE);

	//compassTree->BuildIndex("0","Timestamp");   // order tree by Timestamp... 
	long int nentries = compassTree->GetEntries();

	// Open event built root file
	TFile *builtFile = new TFile(Form("output/built_preprocessed_trees/builtFile_%d_%d.root",runNumber,segNumber),"RECREATE");
	TTree *builtTree = new TTree("builtTree","Offline Built Tree From DPP data");

	BuiltEvent *BE = new BuiltEvent();
	builtTree->Branch("delayTimeFL",&BE->delayTimeFL,"delayTimeFL/D");
	builtTree->Branch("delayTimeFR",&BE->delayTimeFR,"delayTimeFR/D");
	builtTree->Branch("delayTimeBL",&BE->delayTimeBL,"delayTimeBL/D");
	builtTree->Branch("delayTimeBR",&BE->delayTimeBR,"delayTimeBR/D");
	builtTree->Branch("anodeTimeMF",&BE->anodeTimeMF,"anodeTimeMF/D");
	builtTree->Branch("anodeTimeBF",&BE->anodeTimeBF,"anodeTimeBF/D");
	builtTree->Branch("anodeTimeMB",&BE->anodeTimeMB,"anodeTimeMB/D");
	builtTree->Branch("scintTimeL",&BE->scintTimeL,"scintTimeL/D");//Broken channel
	builtTree->Branch("scintTimeR",&BE->scintTimeR,"scintTimeR/D");
	builtTree->Branch("cathodeTime",&BE->cathodeTime,"cathodeTime/D");

	builtTree->Branch("delayShortFL",&BE->delayShortFL,"delayShortFL/D");
	builtTree->Branch("delayShortFR",&BE->delayShortFR,"delayShortFR/D");
	builtTree->Branch("delayShortBL",&BE->delayShortBL,"delayShortBL/D");
	builtTree->Branch("delayShortBR",&BE->delayShortBR,"delayShortBR/D");
	builtTree->Branch("anodeShortMF",&BE->anodeShortMF,"anodeShortMF/D");
	builtTree->Branch("anodeShortBF",&BE->anodeShortMF,"anodeShortBF/D");
	builtTree->Branch("anodeShortMB",&BE->anodeShortMB,"anodeShortMB/D");
	builtTree->Branch("scintShortL",&BE->scintShortL,"scintShortL/D");
	builtTree->Branch("scintShortR",&BE->scintShortR,"scintShortR/D");
	builtTree->Branch("cathodeShort",&BE->cathodeShort,"cathodeShort/D");


	builtTree->Branch("delayLongFL",&BE->delayLongFL,"delayLongFL/D");
	builtTree->Branch("delayLongFR",&BE->delayLongFR,"delayLongFR/D");
	builtTree->Branch("delayLongBL",&BE->delayLongBL,"delayLongBL/D");
	builtTree->Branch("delayLongBR",&BE->delayLongBR,"delayLongBR/D");
	builtTree->Branch("anodeLongMF",&BE->anodeLongMF,"anodeLongMF/D");
	builtTree->Branch("anodeLongBF",&BE->anodeLongBF,"anodeLongBF/D");
	builtTree->Branch("anodeLongMB",&BE->anodeLongMB,"anodeLongMB/D");
	builtTree->Branch("scintLongL",&BE->scintLongL,"scintLongL/D");
	builtTree->Branch("scintLongR",&BE->scintLongR,"scintLongR/D");
	builtTree->Branch("cathodeLong",&BE->cathodeLong,"cathodeLong/D");

	builtTree->Branch("MaxSabreFrontLong",&BE->MaxSabreFrontLong,"MaxSabreFrontLong/D");
	builtTree->Branch("MaxSabreFrontTime",&BE->MaxSabreFrontTime,"MaxSabreFrontTime/D");
	builtTree->Branch("MaxSabreFrontCh",&BE->MaxSabreFrontCh,"MaxSabreFrontCh/D");
	builtTree->Branch("MaxSabreBackLong",&BE->MaxSabreBackLong,"MaxSabreBackLong/D");
	builtTree->Branch("MaxSabreBackTime",&BE->MaxSabreBackTime,"MaxSabreBackTime/D");
	builtTree->Branch("MaxSabreBackCh",&BE->MaxSabreBackCh,"MaxSabreBackCh/D");

	builtTree->Branch("xavg",&BE->xavg,"xavg/D");
	builtTree->Branch("yavg",&BE->yavg,"yavg/D");
	builtTree->Branch("theta",&BE->theta,"theta/D");
	builtTree->Branch("phi",&BE->phi,"phi/D");
	builtTree->Branch("plastic_esum",&BE->plastic_esum,"plastic_esum/D");
	builtTree->Branch("rf_scint_time",&BE->rf_scint_time,"rf_scint_time/D");


	//Initialize some variables
	ULong64_t startTime = 0;
	ULong64_t previousEventTime = 0;
	ULong64_t counter = 0;
	ULong64_t builtcount = 0;
	ULong64_t eventCounter = 0;
	ULong64_t outOfOrderCounter = 0;

	//list of hits in current event window
	vector <DPPchannel*> eventList; 
	vector <DPPchannel*>::iterator eventList_it;

	int fp1s=0, sabre1s=0, coinc=0, other=0; //Counters to keep track of event types

	long int i = 0;
	double tempTS=0,dt=0;
	int temp;
	std::cout << "\nNentries:" << nentries << std::endl<<std::flush ;

	int sabrebeforeanode = 0;
	int sabreafteranode = 0;
	std::cout << "Building events...\n";

	while (i<nentries) //Loop over tree
	{
		//compassTree->GetEntry(index2[i]);
		compassTree->GetEntry(i);

		DPPchannel *currentHit = new DPPchannel(Timestamp,Channel,Board,Energy,EnergyShort,Flags);
		int chan = currentHit->Channel + (currentHit->Board)*16 ;

		// If the Event List is empty, start building an event
		if(eventList.empty())// && (Board==2) && ((Channel==0)||(Channel==1))  )//TRIGGERING on the front anode or back anode is not necessary.
		// if(eventList.empty())  
		{
			startTime = currentHit->Timestamp;
			eventList.push_back(currentHit);
		}
		else
		{
			// Diagnostic to make sure the tree is sorted by Timestamp
			if(currentHit->Timestamp < previousEventTime) 
			{
				cerr << currentHit->Timestamp << " " << previousEventTime << " " << ends;
				cerr << eventList.size() << " !!!Out of order!!!" << endl;
				outOfOrderCounter++;
			}
			// Ignore board offsets for now. Small compared to coincidence window
			if( static_cast<int>(currentHit->Timestamp - startTime) < coincidenceWindow) 
			{
				eventList.push_back(currentHit);
			}
			else
			{
				BE->Reset();
				bool found_delayFL=false; //Bools to help us classify the type of coincidence
				bool found_delayFR=false;
				bool found_delayBL=false;
				bool found_delayBR=false;
				bool found_anodeMF=false;
				bool found_anodeBF = false;
				bool found_anodeMB=false;
				bool found_sabreF = false;
				bool found_sabreB = false;
				bool found_scintL = false;
				bool found_scintR = false;
				bool found_cathode = false;

				//process the event
				for(eventList_it = eventList.begin();
				eventList_it < eventList.end(); eventList_it++)
				{
					int chan = (*eventList_it)->Channel + ((*eventList_it)->Board)*16 ;
					switch(chan)
					{
						case delayFLchan:
								BE->delayTimeFL=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
								BE->delayShortFL=(*eventList_it)->EnergyShort;
								BE->delayLongFL=(*eventList_it)->Energy;
								found_delayFL = true;
								break;
						case delayFRchan:
								BE->delayTimeFR=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
								BE->delayShortFR=(*eventList_it)->EnergyShort;
								BE->delayLongFR=(*eventList_it)->Energy;
								found_delayFR = true;
								break;
						case delayBLchan:
								BE->delayTimeBL=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
								BE->delayShortBL=(*eventList_it)->EnergyShort;
								BE->delayLongBL=(*eventList_it)->Energy;
								found_delayBL = true;
								break;
						case delayBRchan:
								BE->delayTimeBR=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
								BE->delayShortBR=(*eventList_it)->EnergyShort;
								BE->delayLongBR=(*eventList_it)->Energy;
								found_delayBR = true;
								break;
						case anodeMFchan:
								BE->anodeTimeMF=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
								BE->anodeShortMF=(*eventList_it)->EnergyShort; 
								BE->anodeLongMF=(*eventList_it)->Energy;
								found_anodeMF = true;
								break;
						case anodeBFchan:
								BE->anodeTimeBF=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
								BE->anodeShortBF=(*eventList_it)->EnergyShort; 
								BE->anodeLongBF=(*eventList_it)->Energy;
								found_anodeBF = true;
								break;
						case anodeMBchan:
								BE->anodeTimeMB=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
								BE->anodeShortMB=(*eventList_it)->EnergyShort; 
								BE->anodeLongMB=(*eventList_it)->Energy;
								found_anodeMB = true;
								break;
						case scintLchan:
								BE->scintTimeL=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
								BE->scintShortL=(*eventList_it)->EnergyShort; 
								BE->scintLongL=(*eventList_it)->Energy;
								found_scintL = true;
								break;
						case scintRchan:
								BE->scintTimeR=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
								BE->scintShortR=(*eventList_it)->EnergyShort; 
								BE->scintLongR=(*eventList_it)->Energy;
								found_scintR = true;
								break;
						case cathodechan:
								BE->cathodeTime=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
								BE->cathodeShort=(*eventList_it)->EnergyShort; 
								BE->cathodeLong=(*eventList_it)->Energy;
								found_cathode = true;
								break;
					}

					if(sps_channelmap[chan].side_pos.first == "FRONT")
					{
						int PULSER_PEAK_GATE_LOW = sps_channelmap[chan].Ecutlo;
						int PULSER_PEAK_GATE_HIGH = sps_channelmap[chan].Ecuthi;
						//Store if 
						//1. It's the channel with maximum E value in all fronts
						//2. The Energy value is within reasonable limits, avoiding the 32768 bad event
						//3. The Energy value doesn't fall in the pulser-peak window in each channel

						if((*eventList_it)->Energy*sabre_gainmatch[chan] > BE->MaxSabreFrontLong && (*eventList_it)->Energy < 16384 
								&& !((*eventList_it)->Energy>PULSER_PEAK_GATE_LOW && (*eventList_it)->Energy<PULSER_PEAK_GATE_HIGH) )
						{						
							if(gainmatch)
								BE->MaxSabreFrontLong = (*eventList_it)->Energy*sabre_gainmatch[chan];
							else
								BE->MaxSabreFrontLong = (*eventList_it)->Energy;

							BE->MaxSabreFrontTime = static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
							BE->MaxSabreFrontCh = (*eventList_it)->Channel + ((*eventList_it)->Board)*16;   
							found_sabreF=true;
						} 
					}
					else if(sps_channelmap[chan].side_pos.first == "BACK")
					{
						int PULSER_PEAK_GATE_LOW = sps_channelmap[chan].Ecutlo;
						int PULSER_PEAK_GATE_HIGH = sps_channelmap[chan].Ecuthi;

						//Same approach as for fronts but with backs
						if((*eventList_it)->Energy*sabre_gainmatch[chan] > BE->MaxSabreBackLong && (*eventList_it)->Energy < 16384 && !((*eventList_it)->Energy>PULSER_PEAK_GATE_LOW && (*eventList_it)->Energy<PULSER_PEAK_GATE_HIGH) )
						{
							if(gainmatch)
								BE->MaxSabreBackLong = (*eventList_it)->Energy*sabre_gainmatch[chan];
							else
								BE->MaxSabreBackLong = (*eventList_it)->Energy;
		
							BE->MaxSabreBackTime = static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
							BE->MaxSabreBackCh = (*eventList_it)->Channel + ((*eventList_it)->Board)*16;   
							found_sabreB=true;
						}
					}
				}//end of for loop

				bool foundfp = ((found_delayFL && found_delayFR) || (found_delayBL && found_delayBR));

				/*Define, for now, a 'coincidence' to be:
				1. At least one pair of delay-line signals in FP,
				2. At least one SABRE **front** in coincidence with (1), outside of the pulser peak.
				*/
				if(foundfp)
				{
					fp1s++;
					if(found_sabreF) coinc++;
				}
				else
				{
					if(found_sabreF) sabre1s++;
					else other++;
				}

				if(found_sabreF && found_anodeMF)
				{
					dtsabreanode->Fill((BE->MaxSabreFrontTime - BE->anodeTimeMF));
				}
				if(found_scintR && found_anodeMF)
					anodeScintTime->Fill(BE->anodeTimeMF-BE->scintTimeR);

				if(found_anodeMF && found_delayFL && found_delayFR)
				{
					tcheckFront->Fill(BE->delayTimeFL/2+BE->delayTimeFR/2-BE->anodeTimeMF);
				}
				if(found_anodeMB && found_delayBL && found_delayBR)
				{
					tcheckBack->Fill(BE->delayTimeBL/2+BE->delayTimeBR/2-BE->anodeTimeMB);
				}

				if(found_delayFL && found_delayFR && found_delayBL && found_delayBR) // Both t1diff and t2diff needed for theta
				{
					double alpha = (Wire_Dist()/2.0 - Delta_Z(target_Z, target_A, beam_Z, beam_A, ejectile_Z, ejectile_A, beam_E, angle, B_field))/Wire_Dist();
					double fp1_tdiff = (BE->delayTimeFL-BE->delayTimeFR)/2*1/(1.83);
					double fp2_tdiff = (BE->delayTimeBL-BE->delayTimeBR)/2*1/(1.969);
					BE->xavg =  fp1_tdiff*(alpha) + fp2_tdiff*(1.0-alpha);
					BE->theta = (fp2_tdiff - fp1_tdiff)/36.0;
				}
				if(found_cathode && found_anodeMB && found_anodeMF)
				{
					double fp1_y = (BE->anodeTimeMF - BE->cathodeTime)/0.0625;
					double fp2_y = (BE->anodeTimeMB - BE->cathodeTime)/0.0625;
					BE->yavg =  fp1_y*0.8 + fp2_y*0.2;
					BE->phi = fp2_y - fp1_y;
				}
				if(found_scintL && found_scintR)
				{
					BE->plastic_esum = sqrt(BE->scintLongR*BE->scintLongL);
					BE->rf_scint_time = fmod((BE->scintTimeR-BE->scintTimeL),164.95);
				}
				if(foundfp)
				{
					builtTree->Fill();
					builtcount++;
				}
				i--;

				//end of process event
				//Reset Event List
				for (eventList_it = eventList.begin(); 
						   eventList_it < eventList.end(); eventList_it++) 
				{
					delete *eventList_it;
				}
				eventList.clear();
			}//end else part of if(event not in coincidence window)
		}//if(eventList.empty())

	previousEventTime = currentHit->Timestamp;
	counter++;
	i++;
	if(i%100==0) progressbar((double)i,(double)nentries);

	}//End of while (reading through the file entries with GetEntry(i))



	builtTree->Write();
	builtFile->Close();
	cout << "\ncounter(all input frags): " << counter << endl;
	cout << "count(# of built events): " << builtcount << endl;


	static TCanvas ca("dt-checks","dt-checks",0,0,1200,800);
	ca.Divide(3,1,0,0);
	ca.cd(1);
	//   syncCheckHistogram->Draw();
	//   dtsabreanode->Rebin(8);
	dtsabreanode->Draw();
	ca.Modified();
	ca.Update();

	ca.cd(2);
	//   tcheckFront->Rebin(8);
	tcheckFront->Draw();
	tcheckBack->SetLineColor(kRed);
	tcheckBack->Draw("SAME");
	ca.Modified();
	ca.Update();


	ca.cd(3);
	anodeScintTime->SetLineColor(kRed);
	//   anodeScintTime->Rebin(8);
	anodeScintTime->Draw();   
	ca.Modified();
	ca.Update();


	histograms_builder->Write();

	cout << "Fp1s:" <<fp1s << " sabre1s:" << sabre1s << " coinc:" << coinc << " others: " << other << " outOfOrder: " << outOfOrderCounter << " Nentries:" << nentries << endl << flush ;

	delete BE;
	compassTree->Delete();
	builtFile->Close();
	compassFile->Close();

	return;
}





