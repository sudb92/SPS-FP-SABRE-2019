int i=0;
char buffer[50];  
//TFile *_file1 = TFile::Open("6.root");
int runNumber = 401;
TFile *_file1 =  new TFile(Form("./data/DAQ/run_%d/UNFILTERED/compass_run_%d.root",runNumber,runNumber));

TTree *Tdata1 = (TTree*)_file1->Get("Data");
//TFile *_file2 = TFile::Open("wave12coincidence.root");
//TTree *Tdata2 = (TTree*)_file2->Get("Tcoinc");

TCanvas *ca1 = new TCanvas("Orig spectrum", "Orig.spectrum",0,0,600,400);
//TCanvas *c_coinc1 = new TCanvas("Coincident spectrum", "Coincident spectrum",0,500,600,400);


void UserInput3()
{   
	printf("\nPrint q to quit, any key to plot graphs in order..\n");
	gApplication->Connect("KeyPressed(Int_t)", 0, 0, "NextHisto(Int_t)");
//	gApplication->Connect("KeyPressed(Int_t)", 0, 0, "EchoKey(Int_t)");
    
//	c_coinc1->Connect("    
//	c_coinc1->Connect("TCanvas","ProcessedEvent(Int_t,Int_t,Int_t,TObject*)",0,0,"NextHistoCaller(Int_t,Int_t,Int_t,TObject*)");
}

/*
void NextHistoCaller(Int_t event,,Int_t,Int_t,TObject*)
{
  if(event==11)
	NextHisto("");
}*/
void NextHisto(Int_t key)
{
	  i = i+1;
	  //printf("%s\n",sbuffer);
   	  if (key ==113 || i==145) gApplication->Terminate(0); //113 is code for 'q'

	  sprintf(buffer, "(Board==%d)&&(Channel==%d)", (i-1)/16,(i-1)%16);

	  ca1->cd();
	  Tdata1->Draw("Energy>>dataEhist_raw(8188,8,16384)",buffer,"");
	  ca1->Modified();
	  ca1->Update();
	  TH1D *dataEhist_raw = (TH1D*)gDirectory->Get("dataEhist_raw");
	  //printf("Ch:%d\tIntegral_raw:%g\t",i-1,dataEhist_raw->Integral(300,4000));
	  printf("\nCh:%d ",i-1);		


/*	  c_coinc1->cd();
	  //c_coinc1->clear();
	  Tdata2->Draw("Energy>>dataEhist_coinc(4096,0,16383)",buffer,"");
	  c_coinc1->Modified();
	  c_coinc1->Update();*/
  	  TH1D *dataEhist_coinc = (TH1D*)gDirectory->Get("dataEhist_raw");
	  //printf("Integral_coinc:%g\t",dataEhist_coinc->Integral(75,1000));
   	  TSpectrum *s = new TSpectrum(16);
	  Int_t nfound = s->Search(dataEhist_coinc,1,"",0.8);
   	  printf("Peaks:%d\t",nfound);
      ca1->Update();
	  ca1->AddExec("ex","SelectPeak()");

	  TList *functions = dataEhist_coinc->GetListOfFunctions();
	  TPolyMarker *pm = (TPolyMarker*)functions->FindObject("TPolyMarker");
	  if(!pm) return;
   	  double *xp = pm->GetX();
      double *yp = pm->GetY();
   	  int      n = pm->GetN();
   	  for (int i = 0; i<n; i++) {
      auto t = new TText(xp[i],yp[i],Form("%g", xp[i]));
      printf("Peakpos:%g ",xp[i]); 
      t->Draw();
	  }
      ca1->Update();
//		  c_coinc1->WaitPrimitive();

}

void SelectPeak()
{
   int event = gPad->GetEvent();
   if (event != 11) return; //may be comment this line
   auto select = gPad->GetSelected();
   if (!select) return;
   if (select->InheritsFrom("TText")) {
      auto pl = (TText*)select;
	  printf("xpeak:%g\n",pl->GetX());
	  return;
   }
   else
		printf("xpeak:%g\n",-1);
   return;	
}
