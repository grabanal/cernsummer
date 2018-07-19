void triggerEfficiencies(){

  TFile* in_file = new TFile("/n/atlasfs/atlascode/triboson/ntup/jobs-2018-06-12-17h25m35s/mc16_13TeV.364242.\
Sherpa_222_NNPDF30NNLO_WWW_3l3v_EW6.deriv.DAOD_STDM5.e5887_e5984_s3126_r10201_r10210_\
p3404/hadd.root");
  TFile* out_file = new TFile("triggerEfficiencies.root", "RECREATE");
  TTree* tree = (TTree*)in_file->Get("WWWNtuple");
  Int_t nel;
  vector<string>* passedTriggers = 0;
  vector<float>* el_pt = 0;
  TH1F* hist_e26  = new TH1F("hist_e26",  "",100,0,200);
  TH1F* hist_e60  = new TH1F("hist_e60",  "",100,0,200);
  TH1F* hist_e140 = new TH1F("hist_e140", "",100,0,200);

  bool triggered_e26, triggered_e60, triggered_e140;

  tree->SetBranchAddress("nel", &nel);
  tree->SetBranchAddress("passedTriggers", &passedTriggers);
  tree->SetBranchAddress("el_pt", &el_pt);

  int totalEntries = tree->GetEntries();
  int totalSingleElectronEvents = 0;
  for (int i = 0; i<totalEntries; ++i){
    tree->GetEntry(i);
    if (nel == 1){
      totalSingleElectronEvents++;
      triggered_e26  = false;
      triggered_e60  = false;
      triggered_e140 = false;
      for (auto str : *passedTriggers) {
        if (str.find("HLT_e26")  != std::string::npos) triggered_e26  = true;
	if (str.find("HLT_e60")  != std::string::npos) triggered_e60  = true;
	if (str.find("HLT_e140") != std::string::npos) triggered_e140 = true;
      } // YOU CAN IMPROVE THIS
      if (triggered_e26)   hist_e26->Fill((el_pt)->at(0));
      if (triggered_e60)   hist_e60->Fill((el_pt)->at(0));
      if (triggered_e140)  hist_e140->Fill((el_pt)->at(0));
    }
  }
  auto myCanvas = new TCanvas("efficiencies", "efficiencies", 800, 800); // square canvas

  hist_e26->SetLineColor(kBlue);
  hist_e26->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  hist_e26->GetYaxis()->SetTitle("Events");
  hist_e26->SetLabelSize(0.05, "xy");
  hist_e26->SetTitleSize(0.05, "xy");
  hist_e26->Draw("histe");

  hist_e60->SetLineColor(kRed);
  hist_e60->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  hist_e60->SetLabelSize(0.05, "xy");
  hist_e60->SetTitleSize(0.05, "xy");
  hist_e60->Draw("histesames");

  hist_e140->SetLineColor(kOrange);
  hist_e140->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  hist_e140->SetLabelSize(0.05, "xy");
  hist_e140->SetTitleSize(0.05, "xy");
  hist_e140->Draw("histesames");

  auto legend = new TLegend(0.6, 0.6, 0.77, 0.8, "");
  legend->AddEntry(hist_e26, "Trigger HLT_e26", "l");
  legend->AddEntry(hist_e60, "Trigger HLT_e60", "l");
  legend->AddEntry(hist_e140, "Trigger HLT_e140", "l");
  legend->SetBorderSize(1);
  legend->Draw("samehist");

  myCanvas->Write();
  std::cout << totalSingleElectronEvents << std::endl;
  out_file->Close();
  return;
}
