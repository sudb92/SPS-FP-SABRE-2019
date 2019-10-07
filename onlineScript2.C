{
   gStyle->SetOptStat("nei");
   gStyle->SetPalette(53);
   TFile *f = new TFile("output/builtFile_89.root");
  // TFile *fcut = new TFile("
   TTree *dataTree = static_cast<TTree*>(f->Get("builtTree"));
   
   dataTree->SetAlias("tdiffF","(delayTimeFR-delayTimeFL)/2.0");
   dataTree->SetAlias("tdiffB","(delayTimeBL-delayTimeBR)/2.0");
   dataTree->SetAlias("tcheckF","(delayTimeFL+delayTimeFR)/2.0");

   TCanvas *ca = new TCanvas("onlineScript_output","Processing PSD/PHA histograms",0,0,1600,1024);
   ca->Divide(3,2,0.01,0.01,0);

   ca->cd(1);
   dataTree->Draw("tdiffF>>h1(2000,-1000,1000)","delayTimeFL>0 && delayTimeFR>0");
   ca->Modified();
   ca->Update();

   //new TCanvas();
   ca->cd(2);
   dataTree->Draw("tdiffB>>h2(2000,-1000,1000)","delayTimeBL>0 && delayTimeBR>0");
   ca->Modified();
   ca->Update();

//   new TCanvas();
   ca->cd(3);
   dataTree->Draw("delayLongFL:tdiffF>>hblah3(2000,-500,500,1000,0,2000)","delayTimeFL>0 && delayTimeFR>0","colz");
   ca->Modified();
   ca->Update();

//   new TCanvas();
   ca->cd(4);
   dataTree->Draw("anodeLongMF>>blahblah(2000,0,8000)","delayTimeFL>0 && delayTimeFR>0");
   ca->Modified();
   ca->Update();

//   new TCanvas();
   ca->cd(5);
   dataTree->Draw("tcheckF>>hblah(2000,-1000,1000)","delayTimeFL>0 && delayTimeFR>0");
   ca->Modified();
   ca->Update();


//   new TCanvas();

   ca->cd(6);
   dataTree->Draw("anodeLongMF:tdiffF>>hbblah3(2000,-500,500,1000,0,2000)","delayTimeFL>0 && delayTimeFR>0","colz");
   ca->Modified();
   ca->Update();


}
