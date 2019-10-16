
int i=0;
char buffer[50];  

int runNumber = 319;
//TFile *_file1 =  new TFile(Form("/media/sud/AB60-C55F/Sept-Oct2019/SABRE+FP-SepOct2019-Testing/DAQ/run_%d/UNFILTERED/compass_run_%d.root",runNumber,runNumber));
TFile *_file1 =  new TFile(Form("./data/DAQ/run_%d/UNFILTERED/compass_run_%d.root",runNumber,runNumber));

//TFile *_file1 = TFile::Open("13.root");
TTree *Tdata1 = (TTree*)_file1->Get("Data");
//TFile *_file2 = TFile::Open("wave12coincidence.root");
//TTree *Tdata2 = (TTree*)_file2->Get("Tcoinc");

TCanvas *ca1 = new TCanvas("Orig spectrum", "Orig.spectrum",0,0,600,400);
//TCanvas *c_coinc1 = new TCanvas("Coincident spectrum", "Coincident spectrum",0,500,600,400);
void SelectPeak();

void compass_file_quick_looker()
{   
	printf("\nPrint q to quit, any key to plot graphs in order..\n");
	//gApplication->Connect("ReturnPressed(const char*)", 0, 0, "NextHisto(const char*)");    
	i=0;
    int temp;
	while(i<120)
	{
	 
	  //printf("%s\n",sbuffer);
   	  //if (sbuffer[0]=='q' || i==129) gApplication->Terminate(0);

	  sprintf(buffer, "(Board==%d)&&(Channel==%d)", i/16,i%16);

	  ca1->cd();
	  Tdata1->Draw("Energy>>dataEhist_raw(16374,10,16383)",buffer,"");
     // SelectPeak();
	  ca1->Modified();
	  ca1->Update();
	  //TH1D *dataEhist_raw = (TH1D*)gDirectory->Get("dataEhist_raw");
	  //printf("\nCh:%d\tIntegral_raw:%g\tRate(Hz):%g",i,dataEhist_raw->Integral(),dataEhist_raw->Integral()/20.0);	
/*
	  c_coinc1->cd();
	  Tdata2->Draw("Energy>>dataEhist_coinc(16384,0,16383)",buffer,"");
	  c_coinc1->Modified();
	  c_coinc1->Update();
  	  TH1D *dataEhist_coinc = (TH1D*)gDirectory->Get("dataEhist_coinc");
	  printf("Integral_coinc:%g\n",dataEhist_coinc->Integral(300,4000));*/
      system("sleep 1");
	  i = i+1;
      //scanf("%d",&temp);
	}
	exit(0);
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

