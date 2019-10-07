#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <iostream>
#include "DPPchannel.h"
#include "FocalPlane.h"

void analyzeDPP() 
{
   // Create a histogram for the values we read.
   TH1F *deltaT = new TH1F("deltaT","#Delta{T}",1000,-1000,1000);
   TH2F *chVch = new TH2F("chVch","Channel Coincidence Map",16,-0.5,15.5,16,-0.5,15.5);
	
   // Open the file containing the CoMPASS tree.
   TFile *compassFile = TFile::Open("data/DAQ/run_3/UNFILTERED/compass_run_3.root");
   TTree *compassTree = static_cast<TTree*>(compassFile->Get("Data"));


   // Open event built root file
   TFile *builtFile = new TFile("builtFile_3.root","RECREATE");
   TTree *builtTree = new TTree("builtTree","Offline Built Tree From DPP data");
   
   FocalPlane *FP = new FocalPlane();
   builtTree->Branch("delayTimeFL",&FP->delayTimeFL,"delayTimeFL/D");
   builtTree->Branch("delayTimeFR",&FP->delayTimeFR,"delayTimeFR/D");
   builtTree->Branch("delayTimeBL",&FP->delayTimeBL,"delayTimeBL/D");
   builtTree->Branch("delayTimeBR",&FP->delayTimeBR,"delayTimeBR/D");
   builtTree->Branch("anodeTimeMF",&FP->anodeTimeMF,"anodeTimeMF/D");
   builtTree->Branch("anodeTimeMB",&FP->anodeTimeMB,"anodeTimeMB/D");

   builtTree->Branch("delayShortFL",&FP->delayShortFL,"delayShortFL/D");
   builtTree->Branch("delayShortFR",&FP->delayShortFR,"delayShortFR/D");
   builtTree->Branch("delayShortBL",&FP->delayShortBL,"delayShortBL/D");
   builtTree->Branch("delayShortBR",&FP->delayShortBR,"delayShortBR/D");
   builtTree->Branch("anodeShortMF",&FP->anodeShortMF,"anodeShortMF/D");
   builtTree->Branch("anodeShortMB",&FP->anodeShortMB,"anodeShortMB/D");

   builtTree->Branch("delayLongFL",&FP->delayLongFL,"delayLongFL/D");
   builtTree->Branch("delayLongFR",&FP->delayLongFR,"delayLongFR/D");
   builtTree->Branch("delayLongBL",&FP->delayLongBL,"delayLongBL/D");
   builtTree->Branch("delayLongBR",&FP->delayLongBR,"delayLongBR/D");
   builtTree->Branch("anodeLongMF",&FP->anodeLongMF,"anodeLongMF/D");
   builtTree->Branch("anodeLongMB",&FP->anodeLongMB,"anodeLongMB/D");
   //builtTree->Branch("hits",&FP->hits,"hits/I"); horrible idea

   // Lovely new way to read a tree
   TTreeReader fReader;
   fReader.SetTree(compassTree); 
   fReader.GetTree()->BuildIndex("Timestamp");   // order tree by Timestamp

   // Readers to access the data
   TTreeReaderValue<UShort_t> Channel = {fReader, "Channel"};
   TTreeReaderValue<ULong64_t> Timestamp = {fReader, "Timestamp"};
   TTreeReaderValue<UShort_t> Board = {fReader, "Board"};
   TTreeReaderValue<UShort_t> Energy = {fReader, "Energy"};
   TTreeReaderValue<UShort_t> EnergyShort = {fReader, "EnergyShort"};
   TTreeReaderValue<UInt_t> Flags = {fReader, "Flags"};
  
   // Channel Map definitions for reference
   // (make more elegant for final version)
   const UShort_t delayFLchan = 8;
   const UShort_t delayFRchan = 9;
   const UShort_t delayBLchan = 10;
   const UShort_t delayBRchan = 11;
   const UShort_t anodeMFchan = 12;
   const UShort_t anodeMBchan = 13;
   
   //Event Building Coincidence Window
   int coincidenceWindow = 5.0 * 1e6; // Microseconds to Picoseconds

   //Initialize some variables
   ULong64_t startTime = 0;
   ULong64_t previousEventTime = 0;
   ULong64_t counter = 0;
   ULong64_t eventCounter = 0;

   //list of hits in current event window
   vector <DPPchannel*> eventList; 
   vector <DPPchannel*>::iterator eventList_it;

   while (fReader.Next()) //Loop over tree
   {
      DPPchannel *currentHit = new DPPchannel(*Timestamp,*Channel,*Board,*Energy,*EnergyShort,*Flags);

      // If the Event List is empty, start building an event
      if(eventList.empty()) 
      {
           startTime = currentHit->Timestamp;
           previousEventTime = startTime;
           //cout << "(" << counter << ") First entry at time = " << startTime/1.0e12 << " seconds: "  << endl;
	   //cout << "     " << eventList.size() << " " 
	   //   << currentHit->Timestamp/1.0e6 << "  " << currentHit->Name << endl;
           eventList.push_back(currentHit);
      }
      else
      {
         // Diagnostic to make sure the tree is sorted by Timestamp
         if(currentHit->Timestamp < previousEventTime) {  cerr << "Out of order!!!" << endl;}

      
         // Ignore board offsets for now. Small compared to coincidence window
         if( static_cast<int>(currentHit->Timestamp - startTime) < coincidenceWindow) 
         {
            //cout << "     " << eventList.size() << " " 
		//<< currentHit->Timestamp/1.0e6 << "  " << currentHit->Name << endl;
            eventList.push_back(currentHit);
	    previousEventTime = currentHit->Timestamp;
         }
         else
         {
            FP->Reset();
            //process the event
            for(eventList_it = eventList.begin();
                         eventList_it < eventList.end(); eventList_it++)
            {
                switch((*eventList_it)->Channel)
                {
                    case delayFLchan:
                       FP->delayTimeFL=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
                       FP->delayShortFL=(*eventList_it)->EnergyShort;
                       FP->delayLongFL=(*eventList_it)->Energy;
                       FP->hits+= 0x100000; //horrible
                       break;
                    case delayFRchan:
                       FP->delayTimeFR=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
                       FP->delayShortFR=(*eventList_it)->EnergyShort;
                       FP->delayLongFR=(*eventList_it)->Energy;
                       FP->hits+= 0x010000; //horrible
                       break;
                    case delayBLchan:
                       FP->delayTimeBL=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
                       FP->delayShortBL=(*eventList_it)->EnergyShort;
                       FP->delayLongBL=(*eventList_it)->Energy;
                       FP->hits+= 0x001000; //horrible
                       break;
                    case delayBRchan:
                       FP->delayTimeBR=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
                       FP->delayShortBR=(*eventList_it)->EnergyShort;
                       FP->delayLongBR=(*eventList_it)->Energy;
                       FP->hits+= 0x000100; //horrible
                       break;
                    case anodeMFchan:
                       FP->anodeTimeMF=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
                       FP->anodeShortMF=(*eventList_it)->EnergyShort; 
                       FP->anodeLongMF=(*eventList_it)->Energy;
                       FP->hits+= 0x000010; //horrible
                       break;
                    case anodeMBchan:
                       FP->anodeTimeMB=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
                       FP->anodeShortMB=(*eventList_it)->EnergyShort; 
                       FP->anodeLongMB=(*eventList_it)->Energy;
                       FP->hits+= 0x000001; //horrible
                       break;
                }
            }
            builtTree->Fill();
            //end of process event 

            //Reset Event List 
            //cout << "          resetting eventlist (" << eventList.size() << " coincident events)" << endl;
            for (eventList_it = eventList.begin(); 
                           eventList_it < eventList.end(); eventList_it++) 
            {
	       //cout <<"deleting " << (*eventList_it)->Name << endl;
               delete *eventList_it;
            }
            eventList.clear();
            //End of Reset Event List
          }//end else
       }//if(eventList.empty()) 
        //cout << "     " << counter << ends;
       counter++;
       //cout << "counter: " << counter << endl;
    }//End of while (fReader.Next())

   builtTree->Write();
   builtFile->Close();
   delete FP;
}





