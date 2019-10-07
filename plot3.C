{
   TFile *f[3];
   f[0] = new TFile("builtFile_5.root");
   f[1] = new TFile("builtFile_6.root");
   f[2] = new TFile("builtFile_7.root");

   TH1F *h[3];
   TTree *t[3];

   for(int i = 0; i<3; i++)
   { 
      t[i] = static_cast<TTree*>(f[i]->Get("builtTree"));
      string name = Form("hist%d",i);
      h[i] = new TH1F(name.c_str(),name.c_str(),1500,-1500,1500);
      t[i]->Draw(("delayTimeFL-delayTimeFR>>"+name).c_str(),"delayTimeFL!=0 && delayTimeFR!=0 && abs(anodeTimeMB-anodeTimeMF)<100",(i==0) ? "" : "SAME");
   }


}
