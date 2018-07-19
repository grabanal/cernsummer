void plot(TH1D*, std::string);

void comparison(){

  TFile* in_file_1 = new TFile("/Users/grabanal/cernsummer/hists_mSFOS_testing/hist-mc16_13TeV.364242.Sherpa_222_NNPDF30NNLO_WWW_3l3v_EW6.deriv.DAOD_STDM5.e5887_e5984_s3126_r10201_r10210_p3404.root");
  TFile* in_file_2 = new TFile("/Users/grabanal/cernsummer/hists_mSFOS_testing/hist-mc16_13TeV.364253.Sherpa_222_NNPDF30NNLO_lllv.deriv.DAOD_STDM5.e5916_e5984_s3126_r10201_r10210_p3552.root");
  
  //  TH1D* h_WWW_SFOS0_mSFOSclosestToZ = new TH1D();
  //  h_WWW_SFOS0_mSFOSclosestToZ = (TH1D*)in_file_1->Get("triboson_SFOS0_lep27_0b_sumq1/msfos_closestToZ");
  TH1D* h_WWW_SFOS1_mSFOSclosestToZ = new TH1D();
  h_WWW_SFOS1_mSFOSclosestToZ = (TH1D*)in_file_1->Get("triboson_SFOS1_lep27_0b_sumq1/msfos_closestToZ");
  TH1D* h_WWW_SFOS2_mSFOSclosestToZ = new TH1D();
  h_WWW_SFOS2_mSFOSclosestToZ = (TH1D*)in_file_1->Get("triboson_SFOS2_lep27_0b_sumq1/msfos_closestToZ");
  //  TH1D* h_WZ_SFOS0_mSFOSclosestToZ = new TH1D();
  //  h_WZ_SFOS0_mSFOSclosestToZ = (TH1D*)in_file_2->Get("triboson_SFOS0_lep27_0b_sumq1/msfos_closestToZ");
  TH1D* h_WZ_SFOS1_mSFOSclosestToZ = new TH1D();
  h_WZ_SFOS1_mSFOSclosestToZ = (TH1D*)in_file_2->Get("triboson_SFOS1_lep27_0b_sumq1/msfos_closestToZ");
  TH1D* h_WZ_SFOS2_mSFOSclosestToZ = new TH1D();
  h_WZ_SFOS2_mSFOSclosestToZ = (TH1D*)in_file_2->Get("triboson_SFOS2_lep27_0b_sumq1/msfos_closestToZ");

  
  TFile* out_file = new TFile("comparison.root", "RECREATE");

  TH1D* h_WWW = (TH1D*)h_WWW_SFOS1_mSFOSclosestToZ->Clone();
  //  h_WWW->Add(h_WWW_SFOS0_mSFOSclosestToZ);
  h_WWW->Add(h_WWW_SFOS2_mSFOSclosestToZ);
  TH1D* h_WZ = (TH1D*)h_WZ_SFOS1_mSFOSclosestToZ->Clone();
  //  h_WZ->Add(h_WZ_SFOS0_mSFOSclosestToZ);
  h_WZ->Add(h_WZ_SFOS2_mSFOSclosestToZ);
  
  h_WWW->SetName("SFOS1 + SFOS2 for WWW");
  h_WZ ->SetName("SFOS1 + SFOS2 for WZ");
  h_WZ ->SetTitle("m(SFOS) closest to m(Z)");
  auto myCanvas = new TCanvas("mSFOS closest to Z", "mSFOS closest to Z", 800, 800); 
  plot(h_WZ, "WZ");
  plot(h_WWW, "WWW");
  auto legend = new TLegend(0.5, 0.5, 0.8, 0.63, "");
  legend->AddEntry(h_WZ, "SFOS1 + SFOS2 for WZ", "f");
  legend->AddEntry(h_WWW, "SFOS1 + SFOS2 for WWW", "f");
  legend->SetBorderSize(0); 
  legend->Draw();
  myCanvas->Write();
  myCanvas->SaveAs("gabs.pdf");
  out_file->Close();
}


void plot(TH1D* h, std::string str){
  int color = 0;
  if (str == "WZ") color = 880;
  if (str == "WWW") color = 400;
  h->SetLineColor(kBlack);
  h->SetFillColor(color);
  h->SetFillStyle(3001);
  h->SetLineWidth(1);
  h->SetLabelSize(0.03, "xy");
  h->SetTitleSize(0.03, "xy");
  h->GetXaxis()->SetTitle("m(SF, OS) [GeV]");
  h->SetStats(0);
  h->Rebin(10);
  h->Draw("histesames");
  Double_t norm = h->GetEntries();
  h->Scale(1/norm);
}
