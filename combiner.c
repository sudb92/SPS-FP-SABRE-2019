int combiner()
{
   int runNumber = 319;
   int segNumber = 1;
   //TFile *compassFile =  new TFile(Form("data/DAQ/run_%d/UNFILTERED/compass_run_%d.root",runNumber,runNumber));
   TChain ch("Data");
   int ret = 0;
//   TString ret = "1";

   ret = gSystem->Exec(Form("ls ./data/DAQ/run_%d/UNFILTERED/compass_run_%d.root 1> //dev//null 2> //dev//null",runNumber,runNumber));
   if(ret==0)
   		ch.Add(Form("./data/DAQ/run_%d/UNFILTERED/compass_run_%d.root",runNumber,runNumber),-1);   

   while(ret==0)
   {
	   ret = gSystem->Exec(Form("ls ./data/DAQ/run_%d/UNFILTERED/compass_run_%d_%d.root 1> //dev//null 2> //dev//null",runNumber,runNumber,segNumber));
	   if(ret==0)
	   		ch.Add(Form("./data/DAQ/run_%d/UNFILTERED/compass_run_%d_%d.root",runNumber,runNumber,segNumber),-1);   
	   std::cout << "\nRet value: " << ret << " " << segNumber << std::flush;// << " " << buffer.Data();
	   segNumber = segNumber+1;
   }
   std::cout << "\nGetEntries:" << ch.GetEntries() << std::endl;
   ch.SaveAs(Form("./data/DAQ/run_%d/UNFILTERED/compass_run_combined_%d.root",runNumber,runNumber),"");

   return 0;
}
