//macro for analyzing built compass events from FP that were built in analyzeDPP_fp_sabre2.C

#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TH1.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH1D.h"
#include <iomanip>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "TCutG.h"
#include "TCanvas.h"

void AnalyzeBuiltTree(){
    
    //Open built file with built coincidence tree, read in branches:
    TFile *f = new TFile("~/CompassCoincidence/output/builtFile_fponly_265.root");

//    TFile *f = new TFile("~/CompassCoincidence/processedData/builtFile_112.root");
    TTree *t1 = (TTree*)f->Get("builtTree");
    
    Double_t    delayTimeFL, delayTimeFR, delayTimeBL, delayTimeBR, anodeTimeMF, anodeTimeMB, scintTimeL, scintTimeR, cathodeTime, delayShortFL, delayShortFR, delayShortBL, delayShortBR, anodeShortMF, anodeShortMB, scintShortL, scintShortR, cathodeShort, delayLongFL, delayLongFR, delayLongBL, delayLongBR, anodeLongMF, anodeLongMB, scintLongL, scintLongR, cathodeLong;
    
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //All branches read in from tree and what they are:
    
    //time variables:
    t1->SetBranchAddress("delayTimeFL", &delayTimeFL);  //front delay line left
    t1->SetBranchAddress("delayTimeFR", &delayTimeFR);  //front delay line right
    t1->SetBranchAddress("delayTimeBL", &delayTimeBL);  //back delay line left
    t1->SetBranchAddress("delayTimeBR", &delayTimeBR);  //back delay line right
    t1->SetBranchAddress("anodeTimeMF", &anodeTimeMF);  //middle front anode
    t1->SetBranchAddress("anodeTimeMB", &anodeTimeMB);  //middle back anode
    t1->SetBranchAddress("scintTimeL", &scintTimeL);    //left scintillator
    t1->SetBranchAddress("scintTimeR", &scintTimeR);    //right scintillator
    t1->SetBranchAddress("cathodeTime", &cathodeTime);  //cathode
    //short gate energy
    t1->SetBranchAddress("delayShortFL", &delayShortFL);    //front delay line left
    t1->SetBranchAddress("delayShortFR", &delayShortFR);    //front delay line right
    t1->SetBranchAddress("delayShortBL", &delayShortBL);    //back delay line left
    t1->SetBranchAddress("delayShortBR", &delayShortBR);    //back delay line left
    t1->SetBranchAddress("anodeShortMF", &anodeShortMF);    //middle front anode
    t1->SetBranchAddress("anodeShortMB", &anodeShortMB);    //middle back anode
    t1->SetBranchAddress("scintShortL", &scintShortL);      //left scintillator
    t1->SetBranchAddress("scintShortR", &scintShortR);      //right scintillator
    t1->SetBranchAddress("cathodeShort", &cathodeShort);    //cathode
    //long gate energy: TOTAL ENERGY
    t1->SetBranchAddress("delayLongFL", &delayLongFL);      //front delay line left
    t1->SetBranchAddress("delayLongFR", &delayLongFR);      //front delay line right
    t1->SetBranchAddress("delayLongBL", &delayLongBL);      //back delay line left
    t1->SetBranchAddress("delayLongBR", &delayLongBR);      //back delay line right
    t1->SetBranchAddress("anodeLongMF", &anodeLongMF);      //middle front anode
    t1->SetBranchAddress("anodeLongMB", &anodeLongMB);      //middle back anode
    t1->SetBranchAddress("scintLongL", &scintLongL);        //left scintillator
    t1->SetBranchAddress("scintLongR", &scintLongR);        //right scintillator
    t1->SetBranchAddress("cathodeLong", &cathodeLong);      //cathode
    
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //create 1D histograms and declare parameter variables:
    
    //timing histograms for FP:
    Float_t bins1 = 1000;
    Float_t min1 = -500;
    Float_t max1 = 500;
    
    TH1F *fp1 = new TH1F("fp1","fp1",bins1,min1,max1);
    TH1F *fp2 = new TH1F("fp2","fp2",bins1,min1,max1);
    
    Float_t bins2 = 800;
    Float_t min2 = -400;
    Float_t max2 = 400;
    
    TH1F *fp3 = new TH1F("fp3","fp3",bins2,min2,max2);
    TH1F *fp4 = new TH1F("fp4","fp4",bins2,min2,max2);
   
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //create 2D histograms and declare parameter variables:
    
    //Anode energy v fp
    Float_t xbins1 = 800;
    Float_t x_min1 = -400;
    Float_t x_max1 = 400;
    Float_t ybins1 = 2050;
    Float_t y_min1 = 0;
    Float_t y_max1 = 4100;
    
    TH2F *H1 = new TH2F("H1","H1",xbins1,x_min1,x_max1,ybins1,y_min1,y_max1);
    TH2F *H2 = new TH2F("H2","H2",xbins1,x_min1,x_max1,ybins1,y_min1,y_max1);
    
    Float_t xbins2 = 800;
    Float_t x_min2 = -400;
    Float_t x_max2 = 40000;
    Float_t ybins2 = 2050;
    Float_t y_min2 = 0;
    Float_t y_max2 = 8100;
    
   
    TH2F *H3 = new TH2F("H3","H3",xbins1,x_min1,x_max1,ybins1,y_min1,y_max1);
    TH2F *H4 = new TH2F("H4","H4",xbins1,x_min1,x_max1,ybins1,y_min1,y_max1);
    TH2F *H5 = new TH2F("H5","H5",xbins1,x_min1,x_max1,ybins1,y_min1,y_max1);
    TH2F *H6 = new TH2F("H6","H6",xbins1,x_min1,x_max1,ybins1,y_min1,y_max1);
    TH2F *H7 = new TH2F("H7","H7",xbins2,x_min2,x_max2,ybins1,y_min1,y_max1);
    TH2F *H8 = new TH2F("H8","H8",xbins1,x_min1,x_max1,ybins1,y_min1,y_max1);
    
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //Variables for sorting loop:
    
    //conversion of front and rear times to mm:
    //multiply this by time in ns
    Double_t front2mm = 0.545;
    Double_t back2mm = 0.51;
    
    //total entries in tree to loop over:
    Int_t nentries = t1->GetEntries();
    
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //main loop filling and creating histograms:
    
    for (Long64_t k = 0; k < nentries; k++){
        t1->GetEntry(k);
        
        
        
        //FOCAL PLANE TIMING ITEMS------------------------------------------------------------------------
        
        //calculate FP time differences:
            Double_t FrontTdiff = 0;  Double_t BackTdiff=0;
            Double_t FrontTdiffT = 0;  Double_t BackTdiffT=0;
		Double_t FrontAveT = 0;	Double_t FrontAveTmm = 0;
        
            //FP in ns
            FrontTdiffT = (delayTimeFL-delayTimeFR)/2.0;
            BackTdiffT = (delayTimeBL-delayTimeBR)/2.0;
        
            //FP in mm
            FrontTdiff = FrontTdiffT*front2mm;
            BackTdiff = BackTdiffT*back2mm;
        
		//FP averaged time: 
		FrontAveT = (delayTimeFL+delayTimeFR)/2.0;
		FrontAveTmm = FrontAveT*front2mm; 

        //Fill FP time difference histograms:
        if(delayTimeFR > 0 && delayTimeFL > 0){     //empty events are filled with digits 0 or less on digitizers
            fp1->Fill(FrontTdiffT);          //FP in ns
            fp3->Fill(FrontTdiff);          //FP in mm
        }
        
        if(delayTimeBR > 0 && delayTimeBL > 0){
            fp2->Fill(BackTdiffT);           //FP in ns
            fp4->Fill(BackTdiff);           //FP in mm
        }
        
        
        
        //ENERGY V FP SIGNALS-----------------------------------------------------------------------------
        
        //Middle anode v FP timing:
        if(delayTimeFR > 0 && delayTimeFL > 0){
            H1->Fill(FrontTdiff, anodeLongMF);
            H2->Fill(FrontTdiff, delayLongFL);
            H3->Fill(FrontTdiff, delayLongFR);
        }
        
        if(delayTimeBR > 0 && delayTimeBL > 0){
            H4->Fill(BackTdiff, anodeLongMB);
            H5->Fill(BackTdiff, delayLongBL);
            H6->Fill(BackTdiff, delayLongBR);
        }

	if(delayTimeFR > 0 && delayTimeFL > 0){
		H7->Fill(FrontAveTmm, anodeLongMF);
	}
        
        
    
        
    }
    
    //--------------------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------------
    
    //create and fill canvases:
    TCanvas *c1 = new TCanvas("c1");
    fp3->SetLineColor(kBlue);
    fp4->SetLineColor(kRed);
    fp3->Draw("");
    fp4->Draw("same");
    fp3->SetXTitle("FP distance in mm");
    fp3->SetTitle("FP Time difference: front wire in blue, rear wire in red (UNGATED)");
    
    TCanvas *c2 = new TCanvas("c2");
    H1->Draw("COLZ");
    H1->SetXTitle("FP distance in mm");
    H1->SetYTitle("Anode Front Middle Wire Energy (keV)");
    H1->SetTitle("FRONT: Anode v FP (UNGATED)");
    c2->SetLogz();
    
    TCanvas *c3 = new TCanvas("c3");
    H2->Draw("COLZ");
    H2->SetXTitle("FP distance in mm");
    H2->SetTitle("Front Left delay energy v Front FP (UNGATED)");
    //c3->SetLogz();
    
    TCanvas *c4 = new TCanvas("c4");
    H3->Draw("COLZ");
    H3->SetXTitle("FP distance in mm");
    H3->SetTitle("Front Right delay energy v Front FP (UNGATED)");
   // c4->SetLogz();
    
    TCanvas *c5 = new TCanvas("c5");
    H4->Draw("COLZ");
    H4->SetXTitle("FP distance in mm");
    H4->SetYTitle("Anode Back Middle Wire Energy (keV)");
    H4->SetTitle("BACK: Anode v FP (UNGATED)");
    //c3->SetLogz();
    
    TCanvas *c6 = new TCanvas("c6");
    H5->Draw("COLZ");
    H5->SetXTitle("FP distance in mm");
    H5->SetTitle("Back Left delay energy v Back FP (UNGATED)");
    //c6->SetLogz();
    
    TCanvas *c7 = new TCanvas("c7");
    H6->Draw("COLZ");
    H6->SetXTitle("FP distance in mm");
    H6->SetTitle("Back Right delay energy v Back FP (UNGATED)");
    //c7->SetLogz();

    TCanvas *c8 = new TCanvas("c8");
    H7->Draw("COLZ");
    H7->SetTitle("FRONT: delay line average v anode middle (UNGATED)");
    
}



