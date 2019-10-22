#include "sabre_timestamp_shifter2.h"
#include "analyzeDPP_fp_sabre3.h"
#include <TCutG.h>
#include <TROOT.h>
#include <TStyle.h>
#include <iostream>
#include <TLegend.h>
#include <TH1F.h>

void onlineScript4()
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

int runNumber_start = 317;
int runNumber_end = 321;
int maxSegments = 7;

TChain* dataTree = new TChain("builtTree");  

for(int i=runNumber_start; i<=runNumber_end; i++)
{
   for(int j=0; j<=maxSegments; j++)
   {
        std::cout << "\n*********************************************";
        std::cout << "\nTimeshifting SABRE in run#" << i <<  " seg#" << j << " ....";
		//sabre_timestamp_shifter2(i,j);
		std::cout << " done.";
        std::cout << "\n******************";
        std::cout << "\nEventbuilding in run#" << i <<  " seg#" << j << " ....";
		//analyzeDPP_fp_sabre3(i,j);
		std::cout << " done.";
        std::cout << "\n******************";
        std::cout << "\nAdding built events to chain..";
        dataTree->Add(Form("output/builtFile(testing,combined,gainmatched)_%d_%d.root",i,j),-1);   
        std::cout << "done.\n"<<std::flush;
   }   
}

//std::cout << "\n FOO!" << std::flush;
  // int run_number = 319;
/*

 int run_number = 317;
//   TFile *f = new TFile(Form("output/builtFile(testing,combined)_%d.root",run_number));
//   TTree *dataTree = static_cast<TTree*>(f->Get("builtTree"));
dataTree->Add(Form("output/builtFile(testing,combined,gainmatched)_%d.root",run_number),-1);   
   run_number = 318;
//   TFile *f2 = new TFile(Form("output/builtFile(testing,combined)_%d.root",run_number));
//   TTree *dataTree2 = static_cast<TTree*>(f2->Get("builtTree"));
dataTree->Add(Form("output/builtFile(testing,combined,gainmatched)_%d.root",run_number),-1);   

   run_number = 319;
//   TFile *f2 = new TFile(Form("output/builtFile(testing,combined)_%d.root",run_number));
//   TTree *dataTree2 = static_cast<TTree*>(f2->Get("builtTree"));
dataTree->Add(Form("output/builtFile(testing,combined,gainmatched)_%d.root",run_number),-1);   

   run_number = 320;
//   TFile *f2 = new TFile(Form("output/builtFile(testing,combined)_%d.root",run_number));
//   TTree *dataTree2 = static_cast<TTree*>(f2->Get("builtTree"));
dataTree->Add(Form("output/builtFile(testing,combined,gainmatched)_%d.root",run_number),-1);   

   run_number = 321;
//   TFile *f2 = new TFile(Form("output/builtFile(testing,combined)_%d.root",run_number));
//   TTree *dataTree2 = static_cast<TTree*>(f2->Get("builtTree"));
dataTree->Add(Form("output/builtFile(testing,combined,gainmatched)_%d.root",run_number),-1);   

   run_number = 322;
//   TFile *f2 = new TFile(Form("output/builtFile(testing,combined)_%d.root",run_number));
//   TTree *dataTree2 = static_cast<TTree*>(f2->Get("builtTree"));
dataTree->Add(Form("output/builtFile(testing,combined,gainmatched)_%d.root",run_number),-1);   
*/

//   TFile *f = new TFile(Form("output/builtFile(testing,combined,gainmatched)_%d.root",run_number));
//   TTree *dataTree = static_cast<TTree*>(f->Get("builtTree"));
   gStyle->SetOptStat("nei");
   gStyle->SetPalette(kInvertedDarkBodyRadiator);//kDarkBodyRadiator   kInvertedDarkBodyRadiator

   TFile *fprotoncut = new TFile("cuts/protonCutHG.root");
   TCutG *fprotons = static_cast<TCutG*>(fprotoncut->Get("protons"));
   fprotons->SetName("protons");

   TFile *fdeuteroncut = new TFile("cuts/deuteronCutHG.root");
   TCutG *fdeuterons = static_cast<TCutG*>(fdeuteroncut->Get("deuterons"));
   fdeuterons->SetName("deuterons");

//   TFile *falphacut = new TFile("cuts/alphaCutForTheWin.root");
	TFile *falphacut = new TFile("alphas_319.root");
//   TFile *falphacut = new TFile("alphaCutHG.root");
   TCutG *falphas = static_cast<TCutG*>(falphacut->Get("alphas_319"));
   falphas->SetName("alphas");

   TFile *ftritoncut = new TFile("cuts/tritonCutHG.root");
   TCutG *ftritons = static_cast<TCutG*>(ftritoncut->Get("tritons"));
   ftritons->SetName("tritons");

   TFile *coinc_cut_file = new TFile("cuts/coinc_cut_fp_sabre.root");
   TCutG *coinc_cut_fp_sabre = static_cast<TCutG*>(coinc_cut_file->Get("coinc_cut_fp_sabre"));
   coinc_cut_fp_sabre->SetName("coinc_cut_diag");

   dataTree->SetAlias("tdiffF","(delayTimeFL-delayTimeFR)/2.0");
   dataTree->SetAlias("tdiffB","(delayTimeBL-delayTimeBR)/2.0");
   dataTree->SetAlias("tcheckF","(delayTimeFL+delayTimeFR)/2.0-anodeTimeMF");

   TH1F *tdiff_alphas = new TH1F("tdiff_alphas","fp singles gated on alphas",500,-500,500);
   TH1F *tdiff_alphas2 = new TH1F("tdiff_alphas2","fp singles gated on alphas and fp-sabre coinc peak",500,-500,500);
   TH1F *tdiff_alphas3 = new TH1F("tdiff_alphas3","fp singles gated on alphas, fp-sabre coinc peak and diagonal energy coincidence",500,-500,500);

   dataTree->Draw("tdiffF>>tdiff_alphas(500,-500,500)","delayTimeFL>0 && delayTimeFR>0 && alphas","goff");
   tdiff_alphas = static_cast<TH1F*>(gDirectory->Get("tdiff_alphas"));
   dataTree->Draw("tdiffF>>tdiff_alphas2(500,-500,500)","delayTimeFL>0 && delayTimeFR>0 && alphas && (MaxSabreFrontTime - anodeTimeMF)>-300 && (MaxSabreFrontTime - anodeTimeMF)<700","goff");
   tdiff_alphas2 = static_cast<TH1F*>(gDirectory->Get("tdiff_alphas2"));
   dataTree->Draw("tdiffF>>tdiff_alphas3(500,-500,500)","delayTimeFL>0 && delayTimeFR>0 && alphas && coinc_cut_diag && (MaxSabreFrontTime - anodeTimeMF)>-300 && (MaxSabreFrontTime - anodeTimeMF)<700","goff");
   tdiff_alphas3 = static_cast<TH1F*>(gDirectory->Get("tdiff_alphas3"));
   //return;

   auto* legend = new TLegend(0.32,0.16);
//   legend->SetEntrySeparation(4);
   TCanvas* c0 = new TCanvas("c0","c0",0,0,600,400);
   //TCanvas* c0 = new TCanvas();
   c0->cd();
   tdiff_alphas->SetLineColor(kBlue);
   //tdiff_alphas->Scale(0.25);
   tdiff_alphas->Draw("L");

   tdiff_alphas2->SetLineColor(kBlack);
   tdiff_alphas2->Draw("SAME");
  
   tdiff_alphas3->SetLineColor(kRed);
   tdiff_alphas3->Draw("SAME");

   legend->AddEntry("tdiff_alphas","fp singles && alphas only");
   legend->AddEntry("tdiff_alphas2","added sabre_timing_coinc_gate");
   legend->AddEntry("tdiff_alphas3","added timing_gate and diag_energy_cut");
   legend->Draw("SAME");
//   c0->BuildLegend();
   c0->Modified();
   c0->Update();
  // new TCanvas();
//   dataTree->Draw("tdiffF>>h1other(2000,-1000,1000)","delayTimeFL>0 && delayTimeFR>0 && deuterons");
  // new TCanvas();
  // dataTree->Draw("tdiffB>>h2(2000,-1000,1000)","delayTimeBL>0 && delayTimeBR>0");

  // new TCanvas();
  // dataTree->Draw("delayLongFL:tdiffF>>hblah3(800,-700,700,800,0,4000)","delayTimeFL>0 && delayTimeFR>0 && delayLongFL>0","colz");

  // new TCanvas();
 //  dataTree->Draw("anodeLongMF>>blahblah(800,0,8000)","delayTimeFL>0 && delayTimeFR>0");

 // new TCanvas("tcheckF","tcheckF");
 //  dataTree->Draw("tcheckF>>hblah(200,-1000,1000)","delayTimeFL>0 && delayTimeFR>0 && alphas");

  // new TCanvas();
  // dataTree->Draw("delayLongFR:tdiffF>>hbblah3(800,-700,700,400,0,4000)","delayTimeFL>0 && delayTimeFR>0 && delayLongFL>0","colz");

  // new TCanvas();
  // dataTree->Draw("anodeLongMB:tdiffB>>hbbblah3(800,-700,700,400,0,4000)","delayTimeBL>0 && delayTimeBR>0","colz");

//   new TCanvas();
//   dataTree->Draw("anodeLongMF:tdiffB>>hbbblahblahblah3(2000,-700,700,1000,0,4000)","delayTimeBL>0 && delayTimeBR>0","colz");

//   new TCanvas();
//   dataTree->Draw("anodeLongMB:tdiffF>>Banode_tdiffF(2000,-700,700,1000,0,4000)","delayTimeFL>0 && delayTimeFR>0","colz");
   TCanvas* c1 = new TCanvas("c1","",0,420,600,400);
   c1->cd();
   dataTree->Draw("anodeLongMF:tdiffF>>Fanode_tdiffF_old(800,-700,700,400,0,4000)","delayTimeFL>0 && delayTimeFR>0 ","colz");
   falphas->Draw("SAME");
   c1->Modified();
   c1->Update();

/*
   TCanvas* ca = new TCanvas("ca","",0,840,600,400);
   ca->cd();
   ca->SetFillStyle(4000);
   dataTree->Draw("anodeLongMF:tdiffF>>Fanode_tdiffF(800,-600,500,400,0,1500)","delayTimeFL>0 && delayTimeFR>0 ","colz");
   falphas->Draw("SAME");// &&  anodeTimeMF>scintTimeR-
   ca->Modified();
   ca->Update();
*/
   
//   new TCanvas("ScintAnodeTime","ScintAnodeTime");
//   dataTree->Draw("anodeTimeMF-scintTimeR>>hanodescintcheck(2000,-12000,40000)","scintTimeR>0 && anodeTimeMF>0","colz");

//   new TCanvas();
//   dataTree->Draw("anodeLongMF:scintLongR>>Ede(500,20,5000,500,20,5000)","anodeLongMF>0 && scintLongR>0","colz");

   TCanvas* c2 = new TCanvas("c2","",610,0,600,400);
   c2->cd();
   dataTree->Draw("MaxSabreFrontLong:MaxSabreFrontCh>>maxSabreF(128,0,127,16384,0,16383","MaxSabreFrontLong>0 && MaxSabreFrontCh > -1","colz");
   c2->Modified();
   c2->Update();


   TCanvas* c3 = new TCanvas("c3","",610,420,600,400);
   c3->cd();
   dataTree->Draw("MaxSabreBackLong:MaxSabreBackCh>>maxSabreB(128,0,127,500,0,5000)","MaxSabreBackLong>0 && MaxSabreBackCh > -1","colz");
   c3->Modified();
   c3->Update();

/*
   TCanvas* c4 = new TCanvas("c4","",610,840,600,400);
   c4->cd();
   dataTree->Draw("MaxSabreFrontLong:tdiffF>>ESumGraph(100,-500,500,100,0,1000)","MaxSabreFrontLong>50 && MaxSabreFrontCh > -1 && delayTimeFL>0 && delayTimeFR>0 && alphas ","colz");// &&
   c4->Modified();
   c4->Update();
*/

   TCanvas* c5 = new TCanvas("c5","",1220,0,600,400);
   c5->cd();
   dataTree->Draw("((5.486/750)*MaxSabreFrontLong):tdiffF>>ESumGraphPeakcut(100,-500,500,100,0,8)","MaxSabreFrontLong>50 && MaxSabreFrontCh > -1 && delayTimeFL>0 && delayTimeFR>0 && alphas && (MaxSabreFrontTime - anodeTimeMF)>-300 && (MaxSabreFrontTime - anodeTimeMF)<700","colz");
   coinc_cut_fp_sabre->Draw("SAME");
   c5->Modified();
   c5->Update();



//   TF2 *func = new TF2("func","y*(5.486/750)",0,16384);
//   ESumGraphPeakcut->Multiply(func);



   TCanvas* c6 = new TCanvas("c6","",1220,420,600,400);
   c6->cd();
   dataTree->SetLineColor(kBlack);
   dataTree->Draw("(MaxSabreFrontTime - anodeTimeMF)>>dtsabreanode(200,-5000,5000)","anodeTimeMF>0 && MaxSabreFrontTime>0 && MaxSabreFrontCh > -1","");// && anodeTimeMF>scintTimeR
//   new TCanvas();
   dataTree->SetLineColor(kRed);
   dataTree->Draw("(MaxSabreFrontTime - anodeTimeMF)>>dtsabreanodepeakcut(200,-5000,5000)","anodeTimeMF>0 && MaxSabreFrontTime>0 && MaxSabreFrontCh > -1 && (MaxSabreFrontTime - anodeTimeMF)>-300 && (MaxSabreFrontTime - anodeTimeMF)<700","SAME");// && anodeTimeMF>scintTimeR
   c6->Modified();
   c6->Update();

//dataTree->Draw("MaxSabreFrontLong:tdiffF>>ESumGraph(2000,-1000,1000,16384,0,16383)","MaxSabreFrontLong>0 && MaxSabreFrontCh > -1 && delayTimeFL>0 && delayTimeFR>0","colz");

//   new TCanvas();
//   dataTree->Draw("MaxSabreFrontLong:tdiffF>>ESumGraphOther(2000,-1000,1000,16384,0,16383)","MaxSabreFrontLong>0 && MaxSabreFrontCh > -1 && delayTimeFL>0 && delayTimeFR>0 && alphas","colz");

//   new TCanvas();
//   dataTree->Draw("MaxSabreFrontTime	-anodeTimeMF:tdiffF>>TimeCorrelation(100,-100,100,100,-50000,50000)","MaxSabreFrontLong>0 && MaxSabreFrontCh > -1 && delayTimeFL>0 && delayTimeFR>0 && alphas","colz");


//   new TCanvas();
//   dataTree->Draw("MaxSabreFrontTime-anodeTimeMF:MaxSabreFrontLong>>TimeEnergyCorrelation(100,0,16000,100,-20000,20000)","MaxSabreFrontLong>0 && MaxSabreFrontCh > -1 && delayTimeFL>0 && delayTimeFR>0 && alphas","colz");

 //  new TCanvas();
 //  dataTree->Draw("cathodeLong:tdiffF>>CathodeVsTdiffF(2000,-1000,1000,16384,0,16383)","cathodeLong>0 && delayTimeFL>0 && delayTimeFR>0 ","colz");



/*   new TCanvas();
   dataTree->Draw("anodeShortMF/anodeLongMF:tdiffF>>hpsdFrontAnode(500,-1000,1000,500,50,5000)","delayTimeFL>0 && delayTimeFR>0 && deuterons","colz");
*/


}
