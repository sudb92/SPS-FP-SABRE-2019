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
#include <cmath>
#include "progressbar.h"


//TODO: We should make this function take parameters as arguments e.g. (run number, fileName, treeName)
// to help improve workflow
void analyzeDPP_fp_sabre3() 
{
   int runNumber = 329;
   int segNumber = 1;

   bool gainmatch = true;
   double sabre_gainmatch[128] ;
//   sabre_gainmatch = new double[128];
   ifstream in;
     //map[0-127] should return the sabrechan instance with all the properties
   in.open("run401-sabre-gainmatch");
   if(!in.is_open())
   {
    std::cerr << "Error opening gainmatch file ";// << in << " ..." << std::endl;
    return;
   }

   //Gain match file has 
   // ch   peak_position  gain_value
   int ch_1;
   double temp1, temp2;
   std::cout << "Reading gain file.." << std::endl;
   std::cout << "ch vs gain found as folows.." << std::endl;
   while(!in.eof())
   {
     in>>ch_1>>temp1>>temp2;
     sabre_gainmatch[ch_1] = temp2;
     std::cout << "\n" << ch_1 << " " << sabre_gainmatch[ch_1];
   }
/*
  for(int i=0; i<128; i++)
  {
    
    if(map1[i].side_pos.first == "FRONT" && map1[i].detID == "A")
		std::cout << i <<" " << map1[i].detID << " " << map1[i].side_pos.first << " " << map1[i].side_pos.second <<std::endl;
  }*/
  in.close();
   
//  return;




   // Create a histogram for the values we read.
   Long64_t cid=0;
   test_map();

   TH1F *deltaT = new TH1F("deltaT","#Delta{T}",1000,-1000,1000);
   TH2F *chVch = new TH2F("chVch","Channel Coincidence Map",16,-0.5,15.5,16,-0.5,15.5);
   TH1D *dtsabreanode = new TH1D("sabreminusanode_dt","Sabre front anode front dt in ns",5000,-10e3,10e3);
   TH1D *tcheckFront = new TH1D("tcheckFront",
	"Focal Plane Detector Checking (front plane)",5000,-10e3,10e3);
   TH1D *tcheckBack = new TH1D("tcheckBack",
	"Focal Plane Detector Checking (back plane)",5000,-10e3,10e3);
   TH1D *anodeScintTime = new TH1D("anodeScintTime",
        "Anode / Scintillator Coincidence Time",5000,-10e3,10e3);
   TH2D *syncCheckHistogram = new TH2D("dT vs Board#", "Sync Checker Histogram", 8, 0, 7,1000,-5000,5000);
   std::vector<std::pair<int,double>> synclist;
   std::vector<std::pair<int,double>>::iterator synciter;

   // Open the file containing the CoMPASS tree.


   TFile *compassFile = TFile::Open(Form("./output/sabreShiftedFile(testing,combined)_%d_%d.root",runNumber,segNumber));
   TTree *compassTree;
    if(compassFile->IsOpen())
    {
     std::cout << "\nFound shifted file, opening.." ;
     compassFile->Print();
     compassTree = static_cast<TTree*>(compassFile->Get("sabreShiftedTree"));
     }
    else
     {
	//compassFile->Close();  can you close a file that failed to open?
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



   compassTree->Print();
//   compassTree->Scan();
//   return;

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
   cout << "Sorting Data Tree ..." << endl;
   compassTree->BuildIndex("0","Timestamp");   // order tree by Timestamp... 
//TODO: we should only do this once and save the root tree to a file. This is by far 
// the slowest and most intensive part of the event building
// For example, We don't need to resort the tree if we just want to rebuild with a different 
// coincidence window. 

// Then one can make some online diagnostic plots in this nice function rather than the 
// so-called ``onlineScript.C''
   cout << "Done sorting" << endl;
   TTreeIndex *I = (TTreeIndex*)compassTree->GetTreeIndex();
   Long64_t* index2 = I->GetIndex();
   long int nentries = compassTree->GetEntries();




   // Open event built root file
   TFile *builtFile = new TFile(Form("output/builtFile(testing,combined,gainmatched)_%d_%d.root",runNumber,segNumber),"RECREATE");
   TTree *builtTree = new TTree("builtTree","Offline Built Tree From DPP data");

   FocalPlane *FP = new FocalPlane();
   builtTree->Branch("delayTimeFL",&FP->delayTimeFL,"delayTimeFL/D");
   builtTree->Branch("delayTimeFR",&FP->delayTimeFR,"delayTimeFR/D");
   builtTree->Branch("delayTimeBL",&FP->delayTimeBL,"delayTimeBL/D");
   builtTree->Branch("delayTimeBR",&FP->delayTimeBR,"delayTimeBR/D");
   builtTree->Branch("anodeTimeMF",&FP->anodeTimeMF,"anodeTimeMF/D");
   builtTree->Branch("anodeTimeMB",&FP->anodeTimeMB,"anodeTimeMB/D");
   builtTree->Branch("scintTimeL",&FP->scintTimeL,"scintTimeL/D");//Broken channel
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

   builtTree->Branch("MaxSabreFrontLong",&FP->MaxSabreFrontLong,"MaxSabreFrontLong/D");
   builtTree->Branch("MaxSabreFrontTime",&FP->MaxSabreFrontTime,"MaxSabreFrontTime/D");
   builtTree->Branch("MaxSabreFrontCh",&FP->MaxSabreFrontCh,"MaxSabreFrontCh/D");
   builtTree->Branch("MaxSabreBackLong",&FP->MaxSabreBackLong,"MaxSabreBackLong/D");
   builtTree->Branch("MaxSabreBackTime",&FP->MaxSabreBackTime,"MaxSabreBackTime/D");
   builtTree->Branch("MaxSabreBackCh",&FP->MaxSabreBackCh,"MaxSabreBackCh/D");



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
   const UShort_t scintRchan = 0;
   const UShort_t scintLchan = 1;
   const UShort_t cathodechan = 7;*/

   const UShort_t delayFLchan = 8+16*2;
   const UShort_t delayFRchan = 9+16*2;
   const UShort_t delayBLchan = 11+16*2;
   const UShort_t delayBRchan = 10+16*2;
   const UShort_t anodeMFchan = 12+16*2;
   const UShort_t anodeMBchan = 13+16*2;
//TODO: add cathode, scint channels here
   const UShort_t scintRchan = 0+16*2;
   const UShort_t scintLchan = 1+16*2;
   const UShort_t cathodechan = 7+16*2;

/* const UShort_t delayFLchan = 8+16*2;
   const UShort_t delayFRchan = 9+16*2;
   const UShort_t delayBLchan = 10+16*2;
   const UShort_t delayBRchan = 11+16*2;
   const UShort_t anodeMFchan = 12+16*2;
   const UShort_t anodeMBchan = 13+16*2;*/
   
   //Event Building Coincidence Window
   int coincidenceWindow = 2.0 * 1e6; // Microseconds to Picoseconds
   //int coincidenceWindow = 1 * 1e6; // Microseconds to Picoseconds

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
   double tempTS=0,dt=0;
   int temp;
   std::cout << "\nNentries:" << nentries << std::flush;

   int sabrebeforeanode = 0;
   int sabreafteranode = 0;


   while (i<nentries) //Loop over tree
   {
      compassTree->GetEntry(index2[i]);

      DPPchannel *currentHit = new DPPchannel(Timestamp,Channel,Board,Energy,EnergyShort,Flags);

        int chan = currentHit->Channel + (currentHit->Board)*16 ;
//	dt = TMath::Abs(static_cast<double>(tempTS) - static_cast<double>(currentHit->Timestamp));
/*
        if(((chan == 15) || (chan == 31)||(chan==36)||(chan==48)||(chan==64)||(chan==80)||(chan==96)||(chan==112))&&(currentHit->Energy> map1[chan].Ecutlo && currentHit->Energy<map1[chan].Ecuthi))
	{

//			std::cout << "\n " <<  (double)tempTS << " " << (double)currentHit->Timestamp/1.0e3 << " " << abs((double)tempTS - (double)currentHit->Timestamp/1.0e3) ;
			//if(abs((double)tempTS - (double)currentHit->Timestamp/1.0e3)<1.0e6){
				synclist.push_back(make_pair(currentHit->Board,currentHit->Timestamp/1000));
		//	}

			std::cout << "\n " << currentHit->Board<< " " << currentHit->Timestamp/1000 << " " << dt;
		        tempTS = static_cast<double>(currentHit->Timestamp);        

	
	}

	else if(dt>1e6 && synclist.size()>1)
	{
            tempTS = 0;
            int tempcount = 0;
            for(synciter = synclist.begin();
                         synciter < synclist.end(); synciter++)
	  {
		if(tempcount==0)
			tempTS=static_cast<double>((*synciter).second);
		dt = TMath::Abs(static_cast<double>(tempTS) - static_cast<double>((*synciter).second));  //-tempTS
                syncCheckHistogram->Fill((*synciter).first,dt);
		tempcount++;
                std::cout << "\n" <<  (*synciter).first << " " << static_cast<double>((*synciter).second)-tempTS << " " << static_cast<double>((*synciter).second) << " " << tempTS;
	  }
           std::cout << "\n ***";
            synclist.clear();
	    tempTS=0;
	    dt = 0;
	}

*/

      // If the Event List is empty, start building an event
      if(eventList.empty())//TRIGGERING on the front anode only
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

               if(gainmatch)
			       FP->MaxSabreFrontLong = (*eventList_it)->Energy*sabre_gainmatch[chan];
			   else
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
               //if(gainmatch)
			   //		FP->MaxSabreBackLong = (*eventList_it)->Energy*sabre_gainmatch[chan];
			   //else
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
	    i--;
	    if(found_sabreF && found_anodeMF)
	    {
                dtsabreanode->Fill((FP->MaxSabreFrontTime - FP->anodeTimeMF));
	    }
	    if(found_scintR && found_anodeMF)
                anodeScintTime->Fill(FP->anodeTimeMF-FP->scintTimeR);

            if(found_anodeMF && found_delayFL && found_delayFR)
            {
		tcheckFront->Fill(FP->delayTimeFL/2+FP->delayTimeFR/2-FP->anodeTimeMF);
            }
            if(found_anodeMB && found_delayBL && found_delayBR)
            {
		tcheckBack->Fill(FP->delayTimeBL/2+FP->delayTimeBR/2-FP->anodeTimeMB);
            }


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
       progressbar((double)i,(double)nentries);
    }//End of while (fReader.Next())



   builtTree->Write();
   builtFile->Close();
   cout << "counter: " << counter << endl;
   cout << "coinccount: " << coinccount << endl;
  

   TCanvas *ca;
   ca = new TCanvas();
   ca->cd();
//   syncCheckHistogram->Draw();
   dtsabreanode->Rebin(8);
   dtsabreanode->Draw();
   ca->Modified();
   ca->Update();

   TCanvas *ccheck = new TCanvas("ccheck","ccheck");
   ccheck->cd();
   tcheckFront->Rebin(8);
   tcheckFront->Draw();
   tcheckBack->SetLineColor(kRed);
   tcheckBack->Draw("SAME");
   ccheck->Modified();
   ccheck->Update();


   TCanvas *cscintanode = new TCanvas("Scint-Anode-Coinc","Scint-Anode-Coinc");
   cscintanode->cd();
   anodeScintTime->SetLineColor(kRed);
   anodeScintTime->Rebin(8);
   anodeScintTime->Draw();   
   cscintanode->Modified();
   cscintanode->Update();


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





