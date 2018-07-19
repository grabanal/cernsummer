void plot (std::string, TFile*, TFile*, int, std::string);

// MAIN FUNCTION
void nice_cutflow(){
  // Let's open the files                                                                               

  TFile* in_file_1 = new TFile(
    "/Users/grabanal/cernsummer/hists_unlim_detailed/hist-mc16_13TeV.364242.Sherpa_222_NNPDF30NNLO_WWW_3l3v_EW6.deriv.DAOD_STDM5.e5887_e5984_s3126_r10201_r10210_p3404.root");
  TFile* in_file_2 = new TFile(
    "/Users/grabanal/cernsummer/hists_unlim_detailed/hist-mc16_13TeV.364253.Sherpa_222_NNPDF30NNLO_lllv.deriv.DAOD_STDM5.e5916_e5984_s3126_r10201_r10210_p3552.root");
  TFile* in_file_3 = new TFile(
    "/Users/grabanal/cernsummer/hists_unlim_detailed/hist-mc16_13TeV.410472.PhPy8EG_A14_ttbar_hdamp258p75_dil.deriv.DAOD_STDM5.e6348_e5984_s3126_r10201_r10210_p3495.root");
  TFile* in_file_4 = new TFile(
    "/Users/grabanal/cernsummer/hists_unlim_detailed/hist-mc16_13TeV.410155.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttW.deriv.DAOD_STDM5.e5070_e5984_s3126_r10201_r10210_p3495.root");
  std::vector<TFile*> in_files = {in_file_1, in_file_2, in_file_3, in_file_4};
  TFile* out_file = new TFile("nice_cutflow.root", "RECREATE");   // Let's open a TFile
  std::string cutFlowName = "CutFlowNoWeight";
  std::vector<Color_t> colors = {400, 880, 600, 632};
  std::vector<std::string> titles = {"Cutflow for WWW", "Cutflow for WZ", "Cutflow for tt", "Cutflow for ttW"};
  int i = 0;
  for (auto in_file : in_files){
    plot(cutFlowName, in_file, out_file, colors[i], titles[i]);  // Keeping cutFlowName here in case I need to plot more cutflows like the weighted ones, etc.
    i++;
  };
  out_file->Close();                              
}

void plot(std::string cutFlowName, TFile* in_file, TFile* out_file, int color, std::string title){
  std::string canvasname = cutFlowName + title;    
  //  std::replace(canvasname.begin(), canvasname.end(), '/', '_');                  // this is to avoid filename problems                                                  
  auto myCanvas = new TCanvas(canvasname.c_str(), canvasname.c_str(), 800, 800); // square canvas

  //  gStyle->SetPadRightMargin(0.05);
  // Get the cutflow histogram out of the files                                                                             
  TH1D* h = new TH1D();
  in_file->GetObject(cutFlowName.c_str(), h); // Use c_str() cause GetObject() does not accept normal strings as args
  h->SetTitle(title.c_str());
  h->SetFillColor(color);
  h->SetFillStyle(3001);
  h->SetLineWidth(3);
  h->GetYaxis()->SetTitle("Events");
  h->GetXaxis()->SetTitle("");  // No axis units
  h->SetLabelSize(0.04, "xy");
  h->SetTitleSize(0.03, "xy");
  h->SetStats(0);

  //Crop the x-range of histograms to make plots nicer
  int xmax = h->GetXaxis()->FindBin("SFOS0");
  h->GetXaxis()->SetRange(1, xmax);

  //  auto legend = new TLegend(0.6, 0.6, 0.77, 0.8, title.c_str());
  //  legend->AddEntry(h, "cutFlow", "l");
  //  legend->SetBorderSize(0); 

  gPad->SetTicky(1); // 0 makes ticks in left Yaxis, 1 makes ticks in both left and right Yaxis
  gPad->SetGridx();
  
  //BY HAND I AM MANIPULATING THE BIG ERROR IN THE FIRST BIN
  h->SetBinError(1, sqrt(h->GetBinContent(1)) );

  //Make the labels Melissa-readable:
  h->GetXaxis()->SetBinLabel( h->GetXaxis()->FindBin("PreSel"),         "All simulated"          );
  h->GetXaxis()->SetBinLabel( h->GetXaxis()->FindBin("All"),            "DAOD/ntupler"           );
  h->GetXaxis()->SetBinLabel( h->GetXaxis()->FindBin("PassGRL"),        "GRL"                    );
  h->GetXaxis()->SetBinLabel( h->GetXaxis()->FindBin("PassTrig"),       "Trigger"                );
  h->GetXaxis()->SetBinLabel( h->GetXaxis()->FindBin("PassJetClean"),   "Jet cleaning"           );
  h->GetXaxis()->SetBinLabel( h->GetXaxis()->FindBin("passNLepton"),    "N(presel. leptons) = 3" );

  //  h->GetXaxis()->SetBinLabel( h->GetXaxis()->FindBin("3lpresel"),       "ERROR"                  );
  h->GetXaxis()->SetBinLabel( h->GetXaxis()->FindBin("3lpresel_pt"),    "Lepton pt"              );
  h->GetXaxis()->SetBinLabel( h->GetXaxis()->FindBin("3lpresel_qual"),  "Lepton ID"              );
  h->GetXaxis()->SetBinLabel( h->GetXaxis()->FindBin("3lpresel_track"), "Lepton d0/z0"           );
  h->GetXaxis()->SetBinLabel( h->GetXaxis()->FindBin("3lpresel_iso"),   "Lepton iso."            );

  h->GetXaxis()->SetBinLabel( h->GetXaxis()->FindBin("3lsel"),          "N(sel. leptons) = 3"    );
  h->GetXaxis()->SetBinLabel( h->GetXaxis()->FindBin("lep27"),          "N(lep., pt > 27) > 0"   );
  h->GetXaxis()->SetBinLabel( h->GetXaxis()->FindBin("sumq1"),          "|#Sigma Q_{lep.}| = 1"    );
  h->GetXaxis()->SetBinLabel( h->GetXaxis()->FindBin("0b"),             "N(bjets) = 0"           );
  h->GetXaxis()->SetBinLabel( h->GetXaxis()->FindBin("SFOS0"),          "N(SF,OS) = 0"           );

  h->Draw("histe");
  myCanvas->Write();  // Write the histogram in the file

  gPad->SetLogy();
  myCanvas->Write();  

  double AllContent   = h->GetBinContent( h->GetXaxis()->FindBin("All simulated") );
  double DAOD         = h->GetBinContent( h->GetXaxis()->FindBin("DAOD/ntupler") );
  double SFOS0Content = h->GetBinContent( h->GetXaxis()->FindBin("N(SF,OS) = 0") );
  double SFOS0Error   = h->GetBinError( h->GetXaxis()->FindBin("N(SF,OS) = 0") );
  std::cout << AllContent << std::endl;
  std::cout << SFOS0Content << std::endl;
  std::cout << title << ": " << 100.0*SFOS0Content/AllContent << "%" << " ± " << 100.0*SFOS0Error/AllContent << "%" << std::endl;


  TH1D* efficiency = (TH1D*)h->Clone();
  TH1D* base = (TH1D*)h->Clone();
  for (int j=0; j<=(h->GetXaxis()->GetNbins()); j++){
    base->SetBinContent(j, h->GetBinContent(1));
    base->SetBinError(j, h->GetBinError(1));
  };
  efficiency->Divide(h,base,1.0,1.0,"B");
  efficiency->GetYaxis()->SetTitle("Efficiency");
  efficiency->Draw("histe");
  myCanvas->Write();

  double eSFOS0Content = efficiency->GetBinContent( h->GetXaxis()->FindBin("N(SF,OS) = 0") );
  double eSFOS0Error   = efficiency->GetBinError( h->GetXaxis()->FindBin("N(SF,OS) = 0") );
  std::cout << title << ": " << 100.0*eSFOS0Content << "%" << " ± " << 100.0*eSFOS0Error << "%" << std::endl;

}
