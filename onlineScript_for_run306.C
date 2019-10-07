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
   TFile *f = new TFile("output/builtFile_306.root");
   TTree *dataTree = static_cast<TTree*>(f->Get("builtTree"));
   gStyle->SetOptStat("nei");
   gStyle->SetPalette(kInvertedDarkBodyRadiator);//kDarkBodyRadiator   kInvertedDarkBodyRadiator
   TFile *fprotoncut = new TFile("protonCutHG.root");
   TCutG *fprotons = static_cast<TCutG*>(fprotoncut->Get("protons"));
   fprotons->SetName("protons");
   TFile *fdeuteroncut = new TFile("deuteronCutHG.root");
   TCutG *fdeuterons = static_cast<TCutG*>(fdeuteroncut->Get("deuterons"));
   fdeuterons->SetName("deuterons");
   TFile *falphacut = new TFile("alphaCutForTheWin.root");
//   TFile *falphacut = new TFile("alphaCutHG.root");
   TCutG *falphas = static_cast<TCutG*>(falphacut->Get("alphas"));
   falphas->SetName("alphas");
   TFile *ftritoncut = new TFile("tritonCutHG.root");
   TCutG *ftritons = static_cast<TCutG*>(ftritoncut->Get("tritons"));
   ftritons->SetName("tritons");
  
   dataTree->SetAlias("tdiffF","(delayTimeFL-delayTimeFR)/2.0");
   dataTree->SetAlias("tdiffB","(delayTimeBL-delayTimeBR)/2.0");
   dataTree->SetAlias("tcheckF","(delayTimeFL+delayTimeFR)/2.0-anodeTimeMF");


   dataTree->Draw("tdiffF>>h1(2000,-1000,1000)","delayTimeFL>0 && delayTimeFR>0 && deuterons");

  // new TCanvas();
//   dataTree->Draw("tdiffF>>h1other(2000,-1000,1000)","delayTimeFL>0 && delayTimeFR>0 && deuterons");
  // new TCanvas();
  // dataTree->Draw("tdiffB>>h2(2000,-1000,1000)","delayTimeBL>0 && delayTimeBR>0");

   new TCanvas();
   dataTree->Draw("delayLongFL:tdiffF>>hblah3(800,-700,700,800,0,4000)","delayTimeFL>0 && delayTimeFR>0 && delayLongFL>0","colz");

  // new TCanvas();
 //  dataTree->Draw("anodeLongMF>>blahblah(800,0,8000)","delayTimeFL>0 && delayTimeFR>0");

  new TCanvas("tcheckF","tcheckF");
   dataTree->Draw("tcheckF>>hblah(200,-1000,1000)","delayTimeFL>0 && delayTimeFR>0 && alphas");

   new TCanvas();
   dataTree->Draw("delayLongFR:tdiffF>>hbblah3(800,-700,700,400,0,4000)","delayTimeFL>0 && delayTimeFR>0 && delayLongFL>0","colz");

   new TCanvas();
   dataTree->Draw("anodeLongMB:tdiffB>>hbbblah3(800,-700,700,400,0,4000)","delayTimeBL>0 && delayTimeBR>0","colz");

//   new TCanvas();
//   dataTree->Draw("anodeLongMF:tdiffB>>hbbblahblahblah3(2000,-700,700,1000,0,4000)","delayTimeBL>0 && delayTimeBR>0","colz");

//   new TCanvas();
//   dataTree->Draw("anodeLongMB:tdiffF>>Banode_tdiffF(2000,-700,700,1000,0,4000)","delayTimeFL>0 && delayTimeFR>0","colz");

   new TCanvas();
   dataTree->Draw("anodeLongMF:tdiffF>>Fanode_tdiffF(800,-700,700,400,0,2000)","delayTimeFL>0 && delayTimeFR>0 ","colz");

//   new TCanvas("ScintAnodeTime","ScintAnodeTime");
//   dataTree->Draw("anodeTimeMF-scintTimeR>>hanodescintcheck(2000,-12000,40000)","scintTimeR>0 && anodeTimeMF>0","colz");

   new TCanvas();
   dataTree->Draw("anodeLongMF:scintLongR>>Ede(500,20,5000,500,20,5000)","delayTimeFL>0 && delayTimeFR>0","colz");


//   new TCanvas();
//   dataTree->Draw("MaxSabreFrontLong:MaxSabreFrontCh>>maxSabreF(128,0,127,16384,0,16383","MaxSabreFrontLong>0 && MaxSabreFrontCh > -1","colz");

   new TCanvas();
   dataTree->Draw("MaxSabreBackLong:MaxSabreBackCh>>maxSabreB(128,0,127,500,0,5000)","MaxSabreBackLong>0 && MaxSabreBackCh > -1","colz");

   new TCanvas();
   //dataTree->Draw("MaxSabreFrontLong:tdiffF>>ESumGraph(200,-1000,1000,500,0,5000)","MaxSabreFrontLong>50 && MaxSabreFrontCh > -1 && delayTimeFL>0 && delayTimeFR>0 && alphas && anodeTimeMF>scintTimeR","");// 

//dataTree->Draw("MaxSabreFrontLong:tdiffF>>ESumGraph(2000,-1000,1000,16384,0,16383)","MaxSabreFrontLong>0 && MaxSabreFrontCh > -1 && delayTimeFL>0 && delayTimeFR>0","colz");

//   new TCanvas();
//   dataTree->Draw("MaxSabreFrontLong:tdiffF>>ESumGraphOther(2000,-1000,1000,16384,0,16383)","MaxSabreFrontLong>0 && MaxSabreFrontCh > -1 && delayTimeFL>0 && delayTimeFR>0 && alphas","colz");

//   new TCanvas();
//   dataTree->Draw("MaxSabreFrontTime-anodeTimeMF:tdiffF>>TimeCorrelation(100,-100,100,100,-50000,50000)","MaxSabreFrontLong>0 && MaxSabreFrontCh > -1 && delayTimeFL>0 && delayTimeFR>0 && alphas","colz");


//   new TCanvas();
//   dataTree->Draw("MaxSabreFrontTime-anodeTimeMF:MaxSabreFrontLong>>TimeEnergyCorrelation(100,0,16000,100,-20000,20000)","MaxSabreFrontLong>0 && MaxSabreFrontCh > -1 && delayTimeFL>0 && delayTimeFR>0 && alphas","colz");

 //  new TCanvas();
 //  dataTree->Draw("cathodeLong:tdiffF>>CathodeVsTdiffF(2000,-1000,1000,16384,0,16383)","cathodeLong>0 && delayTimeFL>0 && delayTimeFR>0 ","colz");



/*   new TCanvas();
   dataTree->Draw("anodeShortMF/anodeLongMF:tdiffF>>hpsdFrontAnode(500,-1000,1000,500,50,5000)","delayTimeFL>0 && delayTimeFR>0 && deuterons","colz");
*/


}
