#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TTreeReader.h>
#include <TTreeIndex.h>
#include <TTreeReaderValue.h>
#include <iostream>
#include "DPPchannel.h"
#include "FocalPlane_SABRE.h"
#include "test_map.h"
void analyzeDPP_fp_sabre2() //sorts the tree manually
{
   // Create a histogram for the values we read.
   Long64_t cid=0;
   test_map();

   TH1F *deltaT = new TH1F("deltaT","#Delta{T}",1000,-1000,1000);
   TH2F *chVch = new TH2F("chVch","Channel Coincidence Map",16,-0.5,15.5,16,-0.5,15.5);


   // Open the file containing the CoMPASS tree.
   int runNumber = 253;
   TFile *compassFile = TFile::Open(Form("data/DAQ/run_%d/UNFILTERED/compass_run_%d.root",runNumber,runNumber));

// TFile *compassFile = TFile::Open("/home/splitpole/Compass/SABRE+FP-SepOct2019-Testing/DAQ/run_112/UNFILTERED/compass_run_112.root");


   TTree *compassTree = static_cast<TTree*>(compassFile->Get("Data"));

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

//Reference for sorting : https://root-forum.cern.ch/t/sorting-a-tree/12560/4
   compassTree->BuildIndex("0","Timestamp");   // order tree by Timestamp
   TTreeIndex *I = (TTreeIndex*)compassTree->GetTreeIndex();
   Long64_t* index2 = I->GetIndex();
   long int nentries = compassTree->GetEntries();




   // Open event built root file
   TFile *builtFile = new TFile(Form("output/builtFile_fponly_%d.root",runNumber),"RECREATE");
   TTree *builtTree = new TTree("builtTree","Offline Built Tree From DPP data");

   FocalPlane *FP = new FocalPlane();
   builtTree->Branch("delayTimeFL",&FP->delayTimeFL,"delayTimeFL/D");
   builtTree->Branch("delayTimeFR",&FP->delayTimeFR,"delayTimeFR/D");
   builtTree->Branch("delayTimeBL",&FP->delayTimeBL,"delayTimeBL/D");
   builtTree->Branch("delayTimeBR",&FP->delayTimeBR,"delayTimeBR/D");
   builtTree->Branch("anodeTimeMF",&FP->anodeTimeMF,"anodeTimeMF/D");
   builtTree->Branch("anodeTimeMB",&FP->anodeTimeMB,"anodeTimeMB/D");
   builtTree->Branch("scintTimeL",&FP->scintTimeL,"scintTimeL/D");
   builtTree->Branch("scintTimeR",&FP->scintTimeR,"scintTimeR/D");
   builtTree->Branch("cathodeTime",&FP->cathodeTime,"cathodeTime/D");

   builtTree->Branch("delayShortFL",&FP->delayShortFL,"delayShortFL/D");
   builtTree->Branch("delayShortFR",&FP->delayShortFR,"delayShortFR/D");
   builtTree->Branch("delayShortBL",&FP->delayShortBL,"delayShortBL/D");
   builtTree->Branch("delayShortBR",&FP->delayShortBR,"delayShortBR/D");
   builtTree->Branch("anodeShortMF",&FP->anodeShortMF,"anodeShortMF/D");
   builtTree->Branch("anodeShortMB",&FP->anodeShortMB,"anodeShortMB/D");
   builtTree->Branch("scintShortL",&FP->scintShortL,"scintShortL/D");
   builtTree->Branch("scintShortR",&FP->scintShortR,"scintShortR/D");
   builtTree->Branch("cathodeShort",&FP->cathodeShort,"cathodeShort/D");


   builtTree->Branch("delayLongFL",&FP->delayLongFL,"delayLongFL/D");
   builtTree->Branch("delayLongFR",&FP->delayLongFR,"delayLongFR/D");
   builtTree->Branch("delayLongBL",&FP->delayLongBL,"delayLongBL/D");
   builtTree->Branch("delayLongBR",&FP->delayLongBR,"delayLongBR/D");
   builtTree->Branch("anodeLongMF",&FP->anodeLongMF,"anodeLongMF/D");
   builtTree->Branch("anodeLongMB",&FP->anodeLongMB,"anodeLongMB/D");
   builtTree->Branch("scintLongL",&FP->scintLongL,"scintLongL/D");
   builtTree->Branch("scintLongR",&FP->scintLongR,"scintLongR/D");
   builtTree->Branch("cathodeLong",&FP->cathodeLong,"cathodeLong/D");

/*   builtTree->Branch("MaxSabreFrontLong",&FP->MaxSabreFrontLong,"MaxSabreFrontLong/D");
   builtTree->Branch("MaxSabreFrontTime",&FP->MaxSabreFrontTime,"MaxSabreFrontTime/D");
   builtTree->Branch("MaxSabreFrontCh",&FP->MaxSabreFrontCh,"MaxSabreFrontCh/D");
   builtTree->Branch("MaxSabreBackLong",&FP->MaxSabreBackLong,"MaxSabreBackLong/D");
   builtTree->Branch("MaxSabreBackTime",&FP->MaxSabreBackTime,"MaxSabreBackTime/D");
   builtTree->Branch("MaxSabreBackCh",&FP->MaxSabreBackCh,"MaxSabreBackCh/D");*/



//TODO:Add scint, cathode branches here

   //builtTree->Branch("hits",&FP->hits,"hits/I"); horrible idea
  



  
   // Channel Map definitions for reference
   // (make more elegant for final version)
/*   const UShort_t delayFLchan = 8;
   const UShort_t delayFRchan = 9;
   const UShort_t delayBLchan = 11;
   const UShort_t delayBRchan = 10;
   const UShort_t anodeMFchan = 12;
   const UShort_t anodeMBchan = 13;
//TODO: add cathode, scint channels here
   const UShort_t scintRchan = 0;
   const UShort_t scintLchan = 1;
   const UShort_t cathodechan = 7;*/

   const UShort_t delayFLchan = 8+32;
   const UShort_t delayFRchan = 9+32;
   const UShort_t delayBLchan = 11+32;
   const UShort_t delayBRchan = 10+32;
   const UShort_t anodeMFchan = 12+32;
   const UShort_t anodeMBchan = 13+32;
//TODO: add cathode, scint channels here
   const UShort_t scintRchan = 0+32;
   const UShort_t scintLchan = 1+32;
   const UShort_t cathodechan = 7+32;


   
   //Event Building Coincidence Window
//   int coincidenceWindow = 2.0 * 1e6; // Microseconds to Picoseconds
   int coincidenceWindow = 3 * 1e6; // Microseconds to Picoseconds

   //Initialize some variables
   ULong64_t startTime = 0;
   ULong64_t previousEventTime = 0;
   ULong64_t counter = 0;
   ULong64_t coinccount = 0;
   ULong64_t eventCounter = 0;
   ULong64_t outOfOrderCounter = 0;

   //list of hits in current event window
   vector <DPPchannel*> eventList; 
   vector <DPPchannel*>::iterator eventList_it;

   int fp1s=0, sabre1s=0, coinc=0, other=0; //Counters to keep track of event types

   long int i = 0;

   int temp;
   while (i<nentries) //Loop over tree
   {
      compassTree->GetEntry(index2[i]);

      DPPchannel *currentHit = new DPPchannel(Timestamp,Channel,Board,Energy,EnergyShort,Flags);



      // If the Event List is empty, start building an event
      if(eventList.empty() && Board==2 && (Channel == 12 || Channel == 13))
     // if(eventList.empty())  
      {
           startTime = currentHit->Timestamp;
          // previousEventTime = startTime;
 //          cout << "(" << counter << ") First entry at time = " << startTime/1.0e12 << " seconds: "  << endl;
//	   cout << "     " << eventList.size() << " " 
//	      << currentHit->Timestamp/1.0e6 << "  " << currentHit->Name << endl;
 //           cout << "     " << eventList.size() << " " 
//		<< currentHit->Timestamp/1.0e6 << "  " << currentHit->Name << " " << currentHit->Energy << " " << currentHit->Channel << " " << currentHit->Board << endl;
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
  //          cout << "     " << eventList.size() << " " 
//		<< currentHit->Timestamp/1.0e6 << "  " << currentHit->Name << " " << currentHit->Energy << " " << currentHit->Channel << " " << currentHit->Board << endl;
            eventList.push_back(currentHit);
         }
         else
         {
            FP->Reset();
  	    bool found_delayFL=false; //Bools to help us classify the type of coincidence
	    bool found_delayFR=false;
	    bool found_delayBL=false;
	    bool found_delayBR=false;
	    bool found_anodeMF=false;
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
//                switch((*eventList_it)->Channel)
		switch(chan)
                {
                    case delayFLchan:
                       FP->delayTimeFL=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
                       FP->delayShortFL=(*eventList_it)->EnergyShort;
                       FP->delayLongFL=(*eventList_it)->Energy;
                       FP->hits+= 0x100000; //horrible
		       found_delayFL = true;
                       break;
                    case delayFRchan:
                       FP->delayTimeFR=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
                       FP->delayShortFR=(*eventList_it)->EnergyShort;
                       FP->delayLongFR=(*eventList_it)->Energy;
                       FP->hits+= 0x010000; //horrible
		       found_delayFR = true;
                       break;
                    case delayBLchan:
                       FP->delayTimeBL=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
                       FP->delayShortBL=(*eventList_it)->EnergyShort;
                       FP->delayLongBL=(*eventList_it)->Energy;
                       FP->hits+= 0x001000; //horrible
		       found_delayBL = true;
                       break;
                    case delayBRchan:
                       FP->delayTimeBR=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
                       FP->delayShortBR=(*eventList_it)->EnergyShort;
                       FP->delayLongBR=(*eventList_it)->Energy;
                       FP->hits+= 0x000100; //horrible
		       found_delayBR = true;
                       break;
                    case anodeMFchan:
                       FP->anodeTimeMF=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
                       FP->anodeShortMF=(*eventList_it)->EnergyShort; 
                       FP->anodeLongMF=(*eventList_it)->Energy;
                       FP->hits+= 0x000010; //horrible
		       found_anodeMF = true;
                       break;
                    case anodeMBchan:
                       FP->anodeTimeMB=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
                       FP->anodeShortMB=(*eventList_it)->EnergyShort; 
                       FP->anodeLongMB=(*eventList_it)->Energy;
                       FP->hits+= 0x000001; //horrible
		       found_anodeMB = true;
                       break;
                    case scintLchan:
                       FP->scintTimeL=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
                       FP->scintShortL=(*eventList_it)->EnergyShort; 
                       FP->scintLongL=(*eventList_it)->Energy;
//                       FP->hits+= 0x000001; //horrible
		       found_scintL = true;
                       break;
                    case scintRchan:
                       FP->scintTimeR=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
                       FP->scintShortR=(*eventList_it)->EnergyShort; 
                       FP->scintLongR=(*eventList_it)->Energy;
//                       FP->hits+= 0x000001; //horrible
		       found_scintR = true;
                       break;
                    case cathodechan:
                       FP->cathodeTime=static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
                       FP->cathodeShort=(*eventList_it)->EnergyShort; 
                       FP->cathodeLong=(*eventList_it)->Energy;
//                       FP->hits+= 0x000001; //horrible
		       found_cathode = true;
                       break;

                }



		  if(map1[chan].side_pos.first == "FRONT")
		  {
			 int PULSER_PEAK_GATE_LOW = map1[chan].Ecutlo;
			 int PULSER_PEAK_GATE_HIGH = map1[chan].Ecuthi;
			 //Store if 
			 //1. It's the channel with maximum E value in all fronts
			 //2. The Energy value is within reasonable limits, avoiding the 32768 bad event
			 //3. The Energy value doesn't fall in the pulser-peak window in each channel
                        //std::cout << std::endl << "Front: " << (*eventList_it)->Energy ;
			if((*eventList_it)->Energy > FP->MaxSabreFrontLong && (*eventList_it)->Energy < 16384 && !((*eventList_it)->Energy>PULSER_PEAK_GATE_LOW && (*eventList_it)->Energy<PULSER_PEAK_GATE_HIGH) )
		       {						
			       FP->MaxSabreFrontLong = (*eventList_it)->Energy;
			       FP->MaxSabreFrontTime = static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
			       FP->MaxSabreFrontCh = (*eventList_it)->Channel + ((*eventList_it)->Board)*16;   
		               found_sabreF=true;
			      // std::cout << " ***"; std::cin>>temp;	
			} 
			
		 }
		 else if(map1[chan].side_pos.first == "BACK")
		 {
			 int PULSER_PEAK_GATE_LOW = map1[chan].Ecutlo;
			 int PULSER_PEAK_GATE_HIGH = map1[chan].Ecuthi;
                        //std::cout << std::endl << "Back: " << (*eventList_it)->Energy ;

			 //Same approach as for fronts but with backs
			 if((*eventList_it)->Energy > FP->MaxSabreBackLong && (*eventList_it)->Energy < 16384 && !((*eventList_it)->Energy>PULSER_PEAK_GATE_LOW && (*eventList_it)->Energy<PULSER_PEAK_GATE_HIGH) )
			 {
			   FP->MaxSabreBackLong = (*eventList_it)->Energy;
			   FP->MaxSabreBackTime = static_cast<double>((*eventList_it)->Timestamp)/1.0e3;
			   FP->MaxSabreBackCh = (*eventList_it)->Channel + ((*eventList_it)->Board)*16;   
			   found_sabreB=true;
   		          // std::cout << " ***";	std::cin>>temp;
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
            builtTree->Fill();
  	    coinccount++;
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
            //std::cout << "\nEmpty!";
            //End of Reset Event List
          }//end else
       }//if(eventList.empty())
        //cout << "     " << counter << ends;
       previousEventTime = currentHit->Timestamp;
       counter++;
       i++;
    }//End of while (fReader.Next())

   builtTree->Write();
   builtFile->Close();
   cout << "counter: " << counter << endl;
   cout << "coinccount: " << coinccount << endl;
  
/*
  for(int i=0; i<128; i++)
  {
    
    if(map1[i].side_pos.first == "FRONT" && map1[i].detID == "A")
		std::cout << i <<" " << map1[i].detID << " " << map1[i].side_pos.first << " " << map1[i].side_pos.second <<std::endl;
  }*/
   cout << "Fp1s:" <<fp1s << " sabre1s:" << sabre1s << " coinc:" << coinc << " others: " << other << " outOfOrder: " << outOfOrderCounter << " Nentries:" << nentries << endl << flush ;
   delete FP;

  return;

}





