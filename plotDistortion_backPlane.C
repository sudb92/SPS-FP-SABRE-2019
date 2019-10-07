{
   TFile *f[4];
   f[0] = new TFile("builtFile_0.root");
   f[1] = new TFile("builtFile_5.root");
   f[2] = new TFile("builtFile_6.root");
   f[3] = new TFile("builtFile_7.root");

   TH2F *hLeft[4];
   TH2F *hRight[4];
   TTree *t[4];

   for(int i=0; i<4; i++)
   { 
      t[i] = static_cast<TTree*>(f[i]->Get("builtTree"));
      string nameL = Form("histFront_%d",i);
      string nameR = Form("histBack_%d",i);
      hLeft[i] = new TH2F(nameL.c_str(),nameL.c_str(),4000,-500,500,200,0,2);
      hRight[i] = new TH2F(nameR.c_str(),nameR.c_str(),4000,-500,500,200,0,2);
      hLeft[i]->SetMarkerStyle(6);
      hRight[i]->SetMarkerStyle(6);

      t[i]->Draw(("delayShortBL/delayLongBL-0.5:(delayTimeBL-delayTimeBR)/2>>"+nameL).c_str(),"delayTimeBL!=0 && delayTimeBR!=0 && abs(anodeTimeMB-anodeTimeMF)<200 && delayLongBL>0 && delayShortBL>0 && anodeLongMB>800",(i==0)? "col":"colSAME");
      t[i]->Draw(("delayShortBR/delayLongBR+0.5:(delayTimeBL-delayTimeBR)/2>>"+nameR).c_str(),"delayTimeBL!=0 && delayTimeBR!=0 && abs(anodeTimeMB-anodeTimeMF)<200 && delayLongBR>0 && delayShortBR>0 && anodeLongMB>800","SAME");

   }


}
