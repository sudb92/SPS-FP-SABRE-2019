//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Sep 11 17:57:42 2019 by ROOT version 6.13/08
// from TTree Data/CoMPaSS events TTree
// found on file: compass_run_0.root
//////////////////////////////////////////////////////////

#ifndef coinc_h
#define coinc_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <map>
#include <string>

// Headers needed by this particular selector
#include "DPPchannel.h"

class coinc : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<UShort_t> Channel = {fReader, "Channel"};
   TTreeReaderValue<ULong64_t> Timestamp = {fReader, "Timestamp"};
   TTreeReaderValue<UShort_t> Board = {fReader, "Board"};
   TTreeReaderValue<UShort_t> Energy = {fReader, "Energy"};
   TTreeReaderValue<UShort_t> EnergyShort = {fReader, "EnergyShort"};
   TTreeReaderValue<UInt_t> Flags = {fReader, "Flags"};


   vector <DPPchannel> channelList;

   // Channel Map definitions for reference
   // (make more elegant for final version)
   UShort_t delayFL = 8;
   UShort_t delayFR = 9;
   UShort_t delayBL = 10;
   UShort_t delayBR = 11;
   UShort_t anodeMF = 12;
   UShort_t anodeMB = 13;


   
   coinc(TTree * /*tree*/ = 0) { }
   virtual ~coinc() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(coinc,0);

};

#endif

#ifdef coinc_cxx
void coinc::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).
   cout << "Init" << endl;
   tree->BuildIndex("Timestamp");
   fReader.SetTree(tree);
}

Bool_t coinc::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef coinc_cxx
