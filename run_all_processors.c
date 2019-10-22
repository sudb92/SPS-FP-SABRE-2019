{
  gROOT->LoadMacro("sabre_timestamp_shifter3.C++");
  gROOT->LoadMacro("analyseDPP_fp_sabre4.C++");

  int run_number = 319;
  sabre_timestamp_shifter3(run_number);
  analyseDPP_fp_sabre4(run_number);

  gROOT->Macro(Form(".x sabre_timestamp_shifter3.C(%d)",run_number));
  gROOT->Macro(Form(".x analyseDPP_fp_sabre4.C(%d)",run_number));
  gROOT->Macro("onlineScript.C");

}

