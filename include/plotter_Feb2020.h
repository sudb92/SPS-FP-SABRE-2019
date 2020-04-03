/*

plotter_<x>.h

Header file that reads in an event-built tree and draws elements from it to plot. 
Configured to save histograms in a .root file.

B.Sudarsan, E.Good, K.Macon
sbalak2@lsu.edu
Fall2019-Spring2020
*/




#include <TCutG.h>
#include <TROOT.h>
#include <TStyle.h>
#include <iostream>
#include <TLegend.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TTree.h>
#include <TCanvas.h>
#include "BuiltEvent.h"
#include "preprocess.h"

void plotter(int runID, int segID, bool skip_evb)
{
/*  Anything but the rainbow colormap
kDeepSea=51,          kGreyScale=52,    kDarkBodyRadiator=53,
kBlueYellow= 54,                        kInvertedDarkBodyRadiator=56,
kBird=57,             kCubehelix=58,    kGreenRedViolet=59,
kBlueRedYellow=60,    kOcean=61,        kColorPrintableOnGrey=62,
kAlpine=63,           kAquamarine=64,   kArmy=65,
kAtlantic=66,         kAurora=67,       kAvocado=68,
kBeach=69,            kBlackBody=70,    kBlueGreenYellow=71,
kBrownCyan=72,        kCMYK=73,         kCandy=74,
kCherry=75,           kCoffee=76,       kDarkRainBow=77,
kDarkTerrain=78,      kFall=79,         kFruitPunch=80,
kFuchsia=81,          kGreyYellow=82,   kGreenBrownTerrain=83,
kGreenPink=84,        kIsland=85,       kLake=86,
kLightTemperature=87, kLightTerrain=88, kMint=89,
kNeon=90,             kPastel=91,       kPearl=92,
kPigeon=93,           kPlum=94,         kRedBlue=95,
kRose=96,             kRust=97,         kSandyTerrain=98,
kSienna=99,           kSolar=100,       kSouthWest=101,
kStarryNight=102,     kSunset=103,      kTemperatureMap=104,
kThermometer=105,     kValentine=106,   kVisibleSpectrum=107,
kWaterMelon=108,      kCool=109,        kCopper=110,
kGistEarth=111,       kViridis=112,     kCividis=113*/


	int i = runID;
    int j = segID;

	gStyle->SetOptStat("nei");
	gStyle->SetPalette(kInvertedDarkBodyRadiator);//kDarkBodyRadiator   kInvertedDarkBodyRadiator
	//gROOT->ProcessLine("gErrorIgnoreLevel = 1001;"); //magically turns off annoying ROOT messages.
	
	//Erin's cuts
	TFile *tightgate = new TFile("cuts/tightgate.root");
	TCutG *tighttriton = static_cast<TCutG*>(tightgate->Get("tighttriton"));
	tighttriton->SetName("tighttriton");
	
	TFile *deuteronsfile = new TFile("cuts/deuterons.root");
	TCutG *deuterons = static_cast<TCutG*>(deuteronsfile->Get("deuterons"));
	deuterons->SetName("deuterons");
	
	TFile *alphafile = new TFile("cuts/alphas.root");
	TCutG *alphas = static_cast<TCutG*>(alphafile->Get("alphas"));
	alphas->SetName("alphas");
	
	TFile *backanode = new TFile("cuts/backanode.root");
	TCutG *backtritons = static_cast<TCutG*>(backanode->Get("backtritons"));
	backtritons->SetName("backtritons");

	TFile *Nov5alphasf = new TFile("cuts/scintcathode_alphas5Nov19.root");
	TCutG *Nov5alphas = static_cast<TCutG*>(Nov5alphasf->Get("scintcathode_alphas5Nov19"));
	Nov5alphas->SetName("Nov5alphas");

	TFile *Nov5deuteronsf = new TFile("cuts/scintcathode_deuterons5Nov19.root");
	TCutG *Nov5deuterons = static_cast<TCutG*>(Nov5deuteronsf->Get("scintcathode_deuterons5Nov19"));
	Nov5deuterons->SetName("Nov5deuterons");
	
	TFile *Nov5alphasf2 = new TFile("cuts/anodebkfpbk_alphas5Nov19.root");
	TCutG *Nov5alphas2 = static_cast<TCutG*>(Nov5alphasf2->Get("anodebkfpbk_alphas5Nov19"));
	Nov5alphas2->SetName("Nov5alphas2");

	TFile *Nov5deuteronsf2 = new TFile("cuts/anodebkfpbk_deuterons5Nov19.root");
	TCutG *Nov5deuterons2 = static_cast<TCutG*>(Nov5deuteronsf2->Get("anodebkfpbk_deuterons5Nov19"));
	Nov5deuterons2->SetName("Nov5deuterons2");

	TFile *Nov5tritonsf2 = new TFile("cuts/anodebkfpbk_tritons5Nov19.root");
	TCutG *Nov5tritons2 = static_cast<TCutG*>(Nov5tritonsf2->Get("anodebkfpbk_tritons5Nov19"));
	Nov5tritons2->SetName("Nov5tritons2");


   std::cout << "\n******************";
   std::cout << "\nGenerating histograms..";

   std::cout << "\nlook in segment " << j << " in run " << i << "..." << std::flush;
 
   TFile* builtFile =  new TFile(Form("output/built_preprocessed_trees/builtFile_%d_%d.root",i,j));
   if(!builtFile->IsOpen()) std::cout << "\nBuilt File open error!!";

   TTree* builtTree = static_cast<TTree*>(builtFile->Get("builtTree"));


   builtTree->SetAlias("tdiffF","(delayTimeFL-delayTimeFR)/2.0");
   builtTree->SetAlias("tdiffB","(delayTimeBL-delayTimeBR)/2.0");
   builtTree->SetAlias("tcheckF","(delayTimeFL+delayTimeFR)/2.0-anodeTimeMF");

   TFile *histograms = new TFile(Form("./output/histogram_root/histograms_run=%d,seg=%d.root",i,j),"RECREATE");

   TH2F *anodevsfp = new TH2F("anodevsfp","anodevsfp",800,-500,500,400,0,6000);
   TH2F *scintvsfpL = new TH2F("scintvsfpL","scintvsfpL",800,-500,500,400,0,6000);
   TH2F *scintvsfpR = new TH2F("scintvsfpR","scintvsfpR",800,-500,500,400,0,6000);
   TH2F *anodevsfp_scintalpha = new TH2F("anodevsfp_scintalpha","anodevsfp_scintalpha",800,-500,500,400,0,6000);
   TH2F *anodevsfp_scinttriton = new TH2F("anodevsfp_scinttriton","anodevsfp_scinttriton",800,-500,500,400,0,6000);
   TH2F *anodevsfp_scintdeuteron = new TH2F("anodevsfp_scintdeuteron","anodevsfp_scintdeuteron",800,-500,500,400,0,6000);
   
   TH2F *anodebkvsfpbk = new TH2F("anodebkvsfpbk","anodebkvsfpbk",800,-500,500,400,0,6000);
   TH2F *delayLeftEvsfp = new TH2F("delayLeftEvsfp","delayLeftEvsfp",800,-500,500,400,0,4096);
   TH2F *cathodevsScintL  = new TH2F("cathodevsScintL","cathodevsScintL",400,0,4000,400,0,6000);
   
   TH1F *tdiff_all = new TH1F("tdiff_all","all fp singles",500,-500,500);
   TH1F *tdiff_tritons_delayLE = new TH1F("tritons_delayLE","fp singles gated on tritons using delayLE",500,-500,500);
   TH1F *tdiff_deuterons = new TH1F("tdiff_deuterons","all fp singles gated on deuterons",500,-500,500);
   TH1F *tdiff_tritons = new TH1F("tdiff_tritons","fp singles gated on tritons",500,-500,500);
   TH1F *tdiff_alphas = new TH1F("tdiff_alphas","fp singles gated on alphas",500,-500,500);

   TH2F *EvsChFront = new TH2F("EvsChFront","EvsChFront",144,0,143,800,0,8000);
   TH2F *EvsChBack = new TH2F("EvsChBack","EvsChBack",144,0,143,800,0,8000);
   TH1F *fpsabre_dt = new TH1F("fpsabre_dt","fpsabre_dt",200,-5000,5000);
   TH1F *fpsabre_dt_select = new TH1F("fpsabre_dt_select","fpsabre_dt_select",200,-5000,5000);

   TH2F *fpsabrecoinc = new TH2F("fpsabrecoinc","fpsabrecoinc",800,-500,500,1024,0,8000);

   TH2F *fpsabrecoinc_t = new TH2F("fpsabrecoinc_t","fpsabrecoinc_t",800,-500,500,1024,0,8000);
   TH2F *fpsabrecoinc_d = new TH2F("fpsabrecoinc_d","fpsabrecoinc_d",800,-500,500,1024,0,8000);
   TH2F *fpsabrecoinc_a = new TH2F("fpsabrecoinc_a","fpsabrecoinc_a",800,-500,500,1024,0,8000);
   
   TH2F *Tdiff_vs_anode_scintLtime = new TH2F("Tdiff_vs_anode_scintLtime","focal plane v front anode - scintL time",800,-500,500,1024,0,2047);
   TH2F *Theta_vs_anode_scintLtime = new TH2F("Theta_vs_anode_scintLtime","Theta vs front anode-scintL time",2000,-500,500,240,-30,30);
   
   BuiltEvent *BE = new BuiltEvent();
   builtTree->SetBranchAddress("delayTimeFL",&BE->delayTimeFL);
   builtTree->SetBranchAddress("delayTimeFR",&BE->delayTimeFR);
   builtTree->SetBranchAddress("delayTimeBL",&BE->delayTimeBL);
   builtTree->SetBranchAddress("delayTimeBR",&BE->delayTimeBR);
   builtTree->SetBranchAddress("anodeTimeMF",&BE->anodeTimeMF);
   builtTree->SetBranchAddress("anodeTimeBF",&BE->anodeTimeBF);
   builtTree->SetBranchAddress("anodeTimeMB",&BE->anodeTimeMB);
   builtTree->SetBranchAddress("scintTimeL",&BE->scintTimeL);
   builtTree->SetBranchAddress("scintTimeR",&BE->scintTimeR);
   builtTree->SetBranchAddress("cathodeTime",&BE->cathodeTime);

   builtTree->SetBranchAddress("delayShortFL",&BE->delayShortFL);
   builtTree->SetBranchAddress("delayShortFR",&BE->delayShortFR);
   builtTree->SetBranchAddress("delayShortBL",&BE->delayShortBL);
   builtTree->SetBranchAddress("delayShortBR",&BE->delayShortBR);
   builtTree->SetBranchAddress("anodeShortMF",&BE->anodeShortMF);
   builtTree->SetBranchAddress("anodeShortBF",&BE->anodeShortMF);
   builtTree->SetBranchAddress("anodeShortMB",&BE->anodeShortMB);
   builtTree->SetBranchAddress("scintShortL",&BE->scintShortL);
   builtTree->SetBranchAddress("scintShortR",&BE->scintShortR);
   builtTree->SetBranchAddress("cathodeShort",&BE->cathodeShort);

   builtTree->SetBranchAddress("delayLongFL",&BE->delayLongFL);
   builtTree->SetBranchAddress("delayLongFR",&BE->delayLongFR);
   builtTree->SetBranchAddress("delayLongBL",&BE->delayLongBL);
   builtTree->SetBranchAddress("delayLongBR",&BE->delayLongBR);
   builtTree->SetBranchAddress("anodeLongMF",&BE->anodeLongMF);
   builtTree->SetBranchAddress("anodeLongBF",&BE->anodeLongBF);
   builtTree->SetBranchAddress("anodeLongMB",&BE->anodeLongMB);
   builtTree->SetBranchAddress("scintLongL",&BE->scintLongL);
   builtTree->SetBranchAddress("scintLongR",&BE->scintLongR);
   builtTree->SetBranchAddress("cathodeLong",&BE->cathodeLong);

   builtTree->SetBranchAddress("MaxSabreFrontLong",&BE->MaxSabreFrontLong);
   builtTree->SetBranchAddress("MaxSabreFrontTime",&BE->MaxSabreFrontTime);
   builtTree->SetBranchAddress("MaxSabreFrontCh",&BE->MaxSabreFrontCh);
   builtTree->SetBranchAddress("MaxSabreBackLong",&BE->MaxSabreBackLong);
   builtTree->SetBranchAddress("MaxSabreBackTime",&BE->MaxSabreBackTime);
   builtTree->SetBranchAddress("MaxSabreBackCh",&BE->MaxSabreBackCh);

   builtTree->SetBranchAddress("xavg",&BE->xavg);
   builtTree->SetBranchAddress("yavg",&BE->yavg);
   builtTree->SetBranchAddress("theta",&BE->theta);
   builtTree->SetBranchAddress("phi",&BE->phi);
   builtTree->SetBranchAddress("plastic_esum",&BE->plastic_esum);
   builtTree->SetBranchAddress("rf_scint_time",&BE->rf_scint_time);

   builtTree->SetMaxVirtualSize(BASKET_SIZE);
   builtTree->LoadBaskets(BASKET_SIZE);



//    Double_t front2mm = 0.545;
//    Double_t back2mm = 0.51;

    Double_t front2mm = 1.0;//0.545;
    Double_t back2mm = 1.0;//0.51;
    
    //total entries in tree to loop over:
    Int_t nentries = builtTree->GetEntries();
	
    for (Long64_t k = 0; k < nentries; k++)
	{
		builtTree->GetEntry(k);
        //FOCAL PLANE TIMING ITEMS-----------------------------------------------------
        
        //calculate FP time differences:
        Double_t FrontTdiff = 0;  Double_t BackTdiff=0;
        Double_t FrontTdiffT = 0;  Double_t BackTdiffT=0;
		Double_t FrontAveT = 0;	Double_t FrontAveTmm = 0;
		Double_t Scint = 0; Double_t Cathode_PSD = 0; 
		Double_t Timing = 0;
		
		//Timing: anode - scint
		Timing = (BE->anodeTimeMF - BE->scintTimeL);
        
        //FP in ns
        FrontTdiffT = (BE->delayTimeFL-BE->delayTimeFR)/2.0;
        BackTdiffT = (BE->delayTimeBL-BE->delayTimeBR)/2.0;
        
        //FP in mm
        FrontTdiff = FrontTdiffT*front2mm;
        BackTdiff = BackTdiffT*back2mm;
        
		//FP averaged time: 
		FrontAveT = (BE->delayTimeFL+BE->delayTimeFR)/2.0;
		FrontAveTmm = FrontAveT*front2mm; 

		if(BE->delayTimeBR>0 && BE->delayTimeBL>0)
		{
			if(BE->anodeLongMB>0)
				{anodebkvsfpbk->Fill(BackTdiff,BE->anodeLongMB);}
		}

        //Fill FP time difference histograms:
        if(BE->delayTimeFR > 0 && BE->delayTimeFL > 0 && BE->delayTimeBR > 0 && BE->delayTimeBL > 0)
		{     //empty events are filled with digits 0 or less on digitizers

			//std::cout << "\n" << BE->delayTimeFL;
            tdiff_all->Fill(FrontTdiff);          //FP in ns            
			
			//front delay line gated on alphas, tritons, and deuterons
//			if(deuterons->IsInside(BackTdiff,BE->anodeLongMB))
//			if(Nov5deuterons->IsInside(BE->cathodeLong,BE->scintLongL))
			if(Nov5deuterons2->IsInside(BackTdiff,BE->anodeLongMB))
			{	tdiff_deuterons->Fill(FrontTdiff);}
//			if(tighttriton->IsInside(BE->cathodeLong,BE->scintLongL))
   		    if(Nov5tritons2->IsInside(BackTdiff,BE->anodeLongMB))
			{	tdiff_tritons->Fill(FrontTdiff);}
//			if(alphas->IsInside(BackTdiff,BE->anodeLongMB))
//			if(Nov5alphas->IsInside(BE->cathodeLong,BE->scintLongL))
			if(Nov5alphas2->IsInside(BackTdiff,BE->anodeLongMB))
			{	tdiff_alphas->Fill(FrontTdiff);}
			if(BE->anodeLongMF>0)
			{
			
			Tdiff_vs_anode_scintLtime->Fill(FrontTdiff,Timing);
			Theta_vs_anode_scintLtime->Fill(FrontTdiff,BE->theta);
			
			//anode energy v front delay line gated on particle groups
				anodevsfp->Fill(FrontTdiff,BE->anodeLongMF);
//				if(alphas->IsInside(BackTdiff,BE->anodeLongMB))
//			if(Nov5alphas->IsInside(BE->cathodeLong,BE->scintLongL))
				if(Nov5alphas2->IsInside(BackTdiff,BE->anodeLongMB))
					{anodevsfp_scintalpha->Fill(FrontTdiff,BE->anodeLongMF);}
//				if(tighttriton->IsInside(BE->cathodeLong,BE->scintLongL))
				if(Nov5tritons2->IsInside(BackTdiff,BE->anodeLongMB))
					{anodevsfp_scinttriton->Fill(FrontTdiff,BE->anodeLongMF);}
//				if(deuterons->IsInside(BackTdiff,BE->anodeLongMB))
//			if(Nov5deuterons->IsInside(BE->cathodeLong,BE->scintLongL))
			if(Nov5deuterons2->IsInside(BackTdiff,BE->anodeLongMB))
					{anodevsfp_scintdeuteron->Fill(FrontTdiff,BE->anodeLongMF);}
			}
			if(BE->delayLongFL>0)
			{
				delayLeftEvsfp->Fill(FrontTdiff,BE->delayLongFL);
				if(tighttriton->IsInside(BE->cathodeLong,BE->scintLongL))
					tdiff_tritons_delayLE->Fill(FrontTdiff);
			}
       }
       
       //cathode v scint energies: ungated and gated
       if(BE->cathodeLong>0 && BE->scintLongL>0 && BE->delayTimeBR > 0 && BE->delayTimeBL > 0)
       {
           cathodevsScintL->Fill(BE->scintLongL,BE->cathodeLong);
      
       }
       
       //scintillator v front focal plane 
       if(BE->anodeLongMF>0)
       {
       		scintvsfpR->Fill(FrontTdiff,BE->scintLongR);
       		scintvsfpL->Fill(FrontTdiff,BE->scintLongL);
       }
    
		//SABRE histograms
		if(BE->MaxSabreFrontLong>0 && BE->MaxSabreFrontCh>-1){
			EvsChFront->Fill(BE->MaxSabreFrontCh,BE->MaxSabreFrontLong);
		}
		if(BE->MaxSabreBackLong>0 && BE->MaxSabreBackCh>-1){
			EvsChBack->Fill(BE->MaxSabreBackCh,BE->MaxSabreBackLong);
		}
		if(
			BE->MaxSabreFrontLong>0 && BE->MaxSabreFrontCh > -1 
			&& BE->delayTimeFL>0 && BE->delayTimeFR>0 
			&& BE->delayTimeBL>0 && BE->delayTimeBR>0
			//&& (BE->MaxSabreFrontTime - BE->anodeTimeMF)>-1000 
			//&& (BE->MaxSabreFrontTime - BE->anodeTimeMF)<3000
		//	&&((BE->delayLongFL>100) && (BE->delayLongFR>100) && (BE->delayLongBL>100) && (BE->delayLongBR>100))//Dont count on delay noise x_x
		  )
		{
		
					//if((BE->delayLongFL>10) && (BE->delayLongFR>10) && (BE->delayLongBL>10) && (BE->delayLongBR>10))
//			if (tighttriton->IsInside(BE->cathodeLong,BE->scintLongL)){
				if(Nov5tritons2->IsInside(BackTdiff,BE->anodeLongMB)){
				fpsabrecoinc_t->Fill(FrontTdiff,BE->MaxSabreFrontLong);
			}

//			if (deuterons->IsInside(BackTdiff,BE->anodeLongMB)){
//			if(Nov5deuterons->IsInside(BE->cathodeLong,BE->scintLongL)){
			if(Nov5deuterons2->IsInside(BackTdiff,BE->anodeLongMB)){
				fpsabrecoinc_d->Fill(FrontTdiff,BE->MaxSabreFrontLong);
			}
			
//			if (alphas->IsInside(BackTdiff,BE->anodeLongMB)){
//			if(Nov5alphas->IsInside(BE->cathodeLong,BE->scintLongL)){
			if(Nov5alphas2->IsInside(BackTdiff,BE->anodeLongMB)){
				fpsabrecoinc_a->Fill(FrontTdiff,BE->MaxSabreFrontLong);
			}
		}


		//if(BE->anodeTimeMF>0 && BE->MaxSabreFrontTime>0 && BE->MaxSabreFrontCh > -1)
		if(BE->scintTimeL>0 && BE->MaxSabreFrontTime>0 && BE->MaxSabreFrontCh > -1 && BE->delayTimeFL>0 && BE->delayTimeFR>0 	)
		{
			double dtfpsabre = -(BE->scintTimeL - BE->MaxSabreFrontTime);
			fpsabre_dt->Fill(BE->MaxSabreFrontTime - BE->scintTimeL);
			if(dtfpsabre>300 && dtfpsabre<600 )
				{fpsabre_dt_select->Fill(BE->MaxSabreFrontTime - BE->scintTimeL);}
			fpsabrecoinc->Fill(FrontTdiff,BE->MaxSabreFrontLong);


		}
	}


   auto* legend = new TLegend(0.32,0.16);
//   legend->SetEntrySeparation(4);
   TCanvas* c0 = new TCanvas("c0","c0",0,0,600,400);
   //TCanvas* c0 = new TCanvas();
   c0->cd();
   tdiff_all->SetLineColor(kBlue);
   tdiff_all->Draw("");

   
   tdiff_tritons_delayLE->SetLineColor(kBlack);
   tdiff_tritons_delayLE->Draw("SAME");
  
   tdiff_deuterons->SetLineColor(kRed);
   tdiff_deuterons->Draw("SAME");

   tdiff_tritons->SetLineColor(kMagenta);
   tdiff_tritons->Draw("SAME");

   tdiff_alphas->SetLineColor(kGreen);
   tdiff_alphas->Draw("SAME");

   legend->AddEntry("tdiff_all","fp singles");
   legend->AddEntry("tdiff_protons","fp+proton+sabre_coinc");
   legend->AddEntry("tdiff_deuterons","fp+deuterons+sabre_coinc");
   legend->AddEntry("tdiff_tritons","fp+tritons+sabre_coinc");
   legend->AddEntry("tdiff_alphas","fp+alphas+sabre_coinc");
   legend->Draw("SAME");
   c0->Modified();
   c0->Update();

   TCanvas* c1 = new TCanvas("c1","",0,420,600,400);
   c1->cd();
   //dataTree->Draw("anodeLongMF:tdiffF>>anodevsfp(800,-700,700,400,0,2000)","delayTimeFL>0 && delayTimeFR>0 && anodeLongMF>0 ","colz");
   //anodevsfp = static_cast<TH2F*>(gDirectory->Get("anodevsfp"));
   anodevsfp->Draw("");
   anodevsfp->SetOption("colz");
   c1->Modified();
   c1->Update();
  
   TCanvas* c2 = new TCanvas("c2","",610,0,600,400);
   c2->cd();
   c2->Modified();
   c2->Update();
   if(segID==0)
   {
   	;//c1->SaveAs(Form("./output/histogram_pdfs/anodef_fpf_with_cuts_run=%d_seg=0.png",runID));
 	;//c2->SaveAs(Form("./output/histogram_pdfs/anodef_scintR_with_cuts_run=%d_seg=0.png",runID));
   }


   TCanvas* c3 = new TCanvas("c3","",610,420,600,400);
   c3->cd();
   EvsChFront->Draw("");
   EvsChFront->SetOption("colz");
   c3->Modified();
   c3->Update();


   TCanvas* c4 = new TCanvas("c4","",1220,0,600,400);
   c4->cd();
   EvsChBack->Draw("");
   EvsChBack->SetOption("colz");
   c4->Modified();
   c4->Update();

   TCanvas* c5 = new TCanvas("c5","",1220,420,600,400);
   c5->cd();
   fpsabre_dt->SetLineColor(kBlack);
   fpsabre_dt->Draw("");
   fpsabre_dt_select->SetLineColor(kRed);   
   fpsabre_dt_select->Draw("SAME");
   c5->Modified();
   c5->Update();

   TCanvas* c6 = new TCanvas("c6","",600,200,600,400);
   c6->cd();
   fpsabrecoinc->Draw("");
   fpsabrecoinc->SetOption("colz");
   c6->Modified();
   c6->Update();
   
   TCanvas *test = new TCanvas("test","",600,200,600,400);
   anodebkvsfpbk->Draw("");
   anodebkvsfpbk->SetOption("colz");
   test->cd();
   test->Modified();
   test->Update();

   //TCanvas *one = new TCanvas();
   delayLeftEvsfp->SetOption("colz");
//   delayLeftEvsfp->Draw("");

  // scanf("%d",&temp);
   
   anodevsfp_scintalpha->SetOption("colz");
   anodevsfp_scintdeuteron->SetOption("colz");
   scintvsfpL->SetOption("colz");
   scintvsfpR->SetOption("colz");
   Tdiff_vs_anode_scintLtime->SetOption("colz");
   Theta_vs_anode_scintLtime->SetOption("colz");
    
    
   builtFile->Close();
   histograms->Write();
   histograms->Close();
   c0->Close();
   c1->Close();
   c2->Close();
   c3->Close();
   c4->Close();
   c5->Close();
   c6->Close();
   test->Close();

   std::cout << "..done.";
  // }   
///}

}

