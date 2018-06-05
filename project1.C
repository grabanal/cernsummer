// The objective of this program is to compare the predictions of
// - WWW (all the WWW-like processes) 
// - 3W (the on-shell WWW-like process)
// - WHWW (the higgs-mediated WWW-like process)
// The for loop will cycle through different measurables: mlll, njets, nbjets, etc. you name it

// Declaration of the auxiliary functions
void plot (std::string, TFile*, TFile*, TFile*, TFile*);
int nrebin (std::string);
std::string xtitle (std::string);

// MAIN FUNCTION
void project1(){
  
  //  gROOT->SetBatch();
  // Let's open the files                                                                               
  TFile* in_file_1 = new TFile("/Users/grabanal/WWW/jobs-2018-05-21-15h04m35s/3w/hadd.root");   // THE ON-SHELL WWW EVENTS
  TFile* in_file_2 = new TFile("/Users/grabanal/WWW/jobs-2018-05-21-15h04m35s/whww/hadd.root"); // THE HIGGS-MEDIATED EVENTS
  TFile* in_file_3 = new TFile("/Users/grabanal/WWW/jobs-2018-05-21-15h04m35s/www/hadd.root");  // ALL THE WWW EVENTS

  std::vector<string> histnames = {
  "triboson_SFOS0_lep27_0b_sumq1/mlll",
  "triboson_SFOS0_lep27_0b_sumq1/njets",
  "triboson_SFOS0_lep27_0b_sumq1/nbjets",
  "triboson_SFOS0_lep27_0b_sumq1/q0q1",
  "triboson_SFOS0_lep27_0b_sumq1/PhiLepMet",
  "triboson_SFOS0_lep27_0b_sumq1/m01",
  "triboson_SFOS0_lep27_0b_sumq1/m12",
  "triboson_SFOS0_lep27_0b_sumq1/m02",
  "triboson_SFOS0_lep27_0b_sumq1/Met_Pt"};

  TFile* out_file = new TFile("project1.root","RECREATE");   // Let's open a TFile

  for (auto histname : histnames) {
    plot(histname,in_file_1,in_file_2,in_file_3,out_file);
  }
  
  out_file->Close();                              
}

void plot(std::string histname, TFile* in_file_1, TFile* in_file_2, TFile* in_file_3, TFile* out_file){

  // Get the histograms out of the files                                                                             
  TH1F* h1 = new TH1F();
  TH1F* h2 = new TH1F();
  TH1F* h3 = new TH1F();
  in_file_1->GetObject( histname.c_str(),h1); // Use c_str() cause GetObject() does not accept normal strings as args
  in_file_2->GetObject( histname.c_str(),h2);
  in_file_3->GetObject( histname.c_str(),h3);
  h3->SetTitle("");
  h1->SetTitle("");
  h2->SetTitle("");

  // Format histograms                                                        
  h3->Rebin( nrebin(histname) );
  h3->SetLineColor(kBlue);
  h3->SetLineWidth(3);
  h3->GetYaxis()->SetTitle("Events");
  h3->GetXaxis()->SetTitle("");  // No axis units
  h3->SetLabelSize(0.05,"xy");
  h3->SetTitleSize(0.05,"xy");
  //
  h1->Rebin( nrebin(histname) );
  h1->SetLineColor(kRed);
  h1->SetLineWidth(3);
  //
  h2->Rebin( nrebin(histname) );
  h2->SetLineColor(kGreen+2);
  h2->SetLineWidth(3);  

  // Uncomment this if you want to get rid of the odd tail data
  // h3->GetXaxis()->SetRangeUser(0,700);                                                                  
  // h1->GetXaxis()->SetRangeUser(0,700);                                                                  
  // h2->GetXaxis()->SetRangeUser(0,700);                                                                  

  // Divide the TCanvas into two pads                                                                                        
  auto myCanvas = new TCanvas(histname.c_str());
  myCanvas->Divide(1,2,0.001,0.001);

  // Access first pad
  myCanvas->cd(1);

  // Superimpose the single histograms
  h3->Draw("histe");
  h1->Draw("histesames"); // both histograms appear with their own statistics box 
  h2->Draw("histesames"); // both histograms appear with their own statistics box                         

  // Put a legend to identify each histogram
  auto legend = new TLegend(0.6,0.7,0.77,0.9);
  legend->AddEntry(h3,"WWW (all)","l");
  legend->AddEntry(h1,"WWW (on-shell)","l");
  legend->AddEntry(h2,"WH(#rightarrow WW*)","l");
  legend->Draw("samehist");

  // This is to move the statistics boxes (which are superimposed) to more convenient locations                      
  gPad->Update();
  double xstat = ( h3->GetMean() > h3->GetBinCenter( ( h3->GetNbinsX() + 1 )/2 ) ) ? 0.1 : 0.8;
  //
  TPaveStats* st3 = (TPaveStats*)h3->FindObject("stats");
  st3->SetX1NDC(xstat);  // new x start position
  st3->SetY1NDC(0.73); // new y start position                                          
  st3->SetX2NDC(xstat + 0.1);  // new x end position
  st3->SetY2NDC(0.9);  // new y end position                                            
  st3->SetLineColor(kBlue);
  TPaveStats* st1 = (TPaveStats*)h1->FindObject("stats");
  st1->SetX1NDC(xstat);
  st1->SetY1NDC(0.53); 
  st1->SetX2NDC(xstat + 0.1);
  st1->SetY2NDC(0.7); 
  st1->SetLineColor(kRed);
  TPaveStats* st2 = (TPaveStats*)h2->FindObject("stats");
  st2->SetX1NDC(xstat);
  st2->SetY1NDC(0.33);
  st2->SetX2NDC(xstat + 0.1);
  st2->SetY2NDC(0.5);
  st2->SetLineColor(kGreen+2);
  //
  myCanvas->Modified();

  // Access second pad
  myCanvas->cd(2);

  // Let's find the ratio of on-shell over all.                                                           
  auto result = new TH1F(*h1);
  result->Divide(h1,h3,1.0,1.0,"B"); // "B" means binomial error
  result->SetStats(0);               // This deletes the statistics box of the histogram                                                     
  result->SetLineColor(kBlack);
  result->SetLineWidth(3);
  result->SetLabelSize(0.05,"xy");
  result->SetTitleSize(0.05,"xy");

  result->GetYaxis()->SetTitle("Ratio of WWW on-shell/all");
  result->GetYaxis()->SetRangeUser(0,1);
  result->GetXaxis()->SetTitle( (xtitle(histname)).c_str() );
  result->GetXaxis()->SetTitleSize(.07);

  gPad->SetTickx(1);
  gPad->SetTicky(1);

  result->Draw("histe");

  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.2);
  
  //myCanvas->Update();                                                                                   

  myCanvas->Write();                            // Write the histogram in the file
}

int nrebin(std::string histname){
  if (histname.find("mlll") != std::string::npos) return 10;
  if (histname.find("njets") != std::string::npos) return 1;
  if (histname.find("nbjets") != std::string::npos) return 1;
  if (histname.find("q0q1") != std::string::npos) return 1;
  if (histname.find("PhiLepMet") != std::string::npos) return 10;
  if (histname.find("m01") != std::string::npos) return 10;
  if (histname.find("m12") != std::string::npos) return 10;
  if (histname.find("m02") != std::string::npos) return 10;
  if (histname.find("Met_Pt") != std::string::npos) return 1;
return 1;
}

std::string xtitle(std::string histname){
  if (histname.find("mlll") != std::string::npos) return "m_{3l} [GeV]";
  if (histname.find("njets") != std::string::npos) return "N(jets)";
  if (histname.find("nbjets") != std::string::npos) return "N(b-jets)";
  if (histname.find("q0q1") != std::string::npos) return "q(lead) #times q(sub-lead)";
  if (histname.find("PhiLepMet") != std::string::npos) return "#Delta#phi (3l,MET) [rad]";
  if (histname.find("m01") != std::string::npos) return "m(lead,sub-lead) [GeV]";
  if (histname.find("m12") != std::string::npos) return "m(sub-lead,third) [GeV]";
  if (histname.find("m02") != std::string::npos) return "m(lead,third) [GeV]";
  if (histname.find("Met_Pt") != std::string::npos) return "MET [GeV]";
  return "";
}

// TO DO:
// 1. Maybe think of better default values for nrebin and xtitle
// 2. Assess the usefulness of the TCanvas-updating functions
// 3. Check why this happens: when running "root project1.C", the pop-up pads are incomplete.
// Gabriel Rabanal
