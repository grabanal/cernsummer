// The objective of this program is to compare the predictions of
// - WWW (all the WWW-like processes) 
// - 3W (the on-shell WWW-like process)
// - WHWW (the higgs-mediated WWW-like process)
// The for loop will cycle through different measurables: mlll, njets, nbjets, etc. you name it

void plot (std::string, TFile*, TFile*, TFile*, TFile*);
int nrebin (std::string);
std::string xtitle (std::string);
void adjust_range(std::string, TH1F*);
void show_overflow(TH1F*, bool=true, bool=true);

// MAIN FUNCTION
void project1_modified(){
  
  // Let's open the files                                                                               
  TFile* in_file_1 = new TFile("/Users/grabanal/hist_files_2018_06_19/hist_2015_2016.root");   // THE 2015+2016 events
  TFile* in_file_2 = new TFile("/Users/grabanal/hist_files_2018_06_19/hist_2017.root"); // THE 2017 events
  TFile* in_file_3 = new TFile("/Users/grabanal/hist_files_2018_06_19/hist_combined.root");  // ALL THE WWW EVENTS

  std::vector<string> histnames = {
  "triboson_SFOS0_lep27_0b_sumq1/mlll",
  "triboson_SFOS0_lep27_0b_sumq1/njets",
  "triboson_SFOS0_lep27_0b_sumq1/nbjets",
  "triboson_SFOS0_lep27_0b_sumq1/q0q1",
  "triboson_SFOS0_lep27_0b_sumq1/PhiLepMet",
  "triboson_SFOS0_lep27_0b_sumq1/m01",
  "triboson_SFOS0_lep27_0b_sumq1/m12",
  "triboson_SFOS0_lep27_0b_sumq1/m02",
  "triboson_SFOS0_lep27_0b_sumq1/Met_Pt"
  };
  TFile* out_file = new TFile("project1_modified.root", "RECREATE");   // Let's open a TFile
  for (auto histname : histnames)
    plot(histname, in_file_1, in_file_2, in_file_3, out_file);
  out_file->Close();                              
}

void plot(std::string histname, TFile* in_file_1, TFile* in_file_2, TFile* in_file_3, TFile* out_file){
  
  gStyle->SetPadRightMargin(0.05);
  // Get the histograms out of the files                                                                             
  TH1F* h1 = new TH1F();
  TH1F* h2 = new TH1F();
  TH1F* h3 = new TH1F();
  in_file_1->GetObject(histname.c_str(), h1); // Use c_str() cause GetObject() does not accept normal strings as args
  in_file_2->GetObject(histname.c_str(), h2);
  in_file_3->GetObject(histname.c_str(), h3);
  h3->SetTitle("");
  h1->SetTitle("");
  h2->SetTitle("");

  // Format histograms                                                        
  h3->Rebin( nrebin(histname) );
  h3->SetLineColor(kBlue);
  h3->SetLineWidth(3);
  h3->GetYaxis()->SetTitle("Events");
  h3->GetXaxis()->SetTitle("");  // No axis units
  h3->SetLabelSize(0.05, "xy");
  h3->SetTitleSize(0.05, "xy");
  //
  h1->Rebin( nrebin(histname) );
  h1->SetLineColor(kRed);
  h1->SetLineWidth(3);
  //
  h2->Rebin( nrebin(histname) );
  h2->SetLineColor(kGreen+2);
  h2->SetLineWidth(3);  

  // Divide the TCanvas into two pads
  std::string canvasname = histname;    
  std::replace(canvasname.begin(), canvasname.end(), '/', '_');                  // this is to avoid filename problems                                                  
  auto myCanvas = new TCanvas(canvasname.c_str(), canvasname.c_str(), 800, 800); // square canvas
  myCanvas->Divide(1, 2, 0.001, 0.001); 

  // Access first pad
  myCanvas->cd(1);

  //Crop the x-range of histograms to make plots nicer, and then show an overflow bin if there is any overflow:
  adjust_range(histname, h3);
  adjust_range(histname, h1);
  adjust_range(histname, h2);
  show_overflow(h3,false,false);
  show_overflow(h1,false,false);
  show_overflow(h2,false,false);

  // Superimpose the single histograms
  h3->Draw("histe");
  h1->Draw("histesames"); // both histograms appear with their own statistics box 
  h2->Draw("histesames"); // both histograms appear with their own statistics box                         

  // Put a legend to identify each histogram
  auto legend = new TLegend(0.6, 0.6, 0.77, 0.8, "");
  legend->AddEntry(h3, "Combined", "l");
  legend->AddEntry(h1, "2015+2016", "l");
  legend->AddEntry(h2, "2017", "l");
  legend->SetBorderSize(0); 
  legend->Draw("samehist");

  // This is to move the statistics boxes (which are superimposed) to more convenient locations                      
  gPad->Update(); // this is necessary for st3 not to be NULL
  double xstat = ( h3->GetMean() > h3->GetBinCenter( ( h3->GetNbinsX() + 1 )/2 ) ) ? 0.12 : 0.8; // Put the statistics box out of the way of the peak of the distribution
  //
  TPaveStats* st3 = (TPaveStats*)h3->FindObject("stats");
  st3->SetX1NDC(xstat);        // new x start position
  st3->SetY1NDC(0.73);         // new y start position                                          
  st3->SetX2NDC(xstat + 0.1);  // new x end position
  st3->SetY2NDC(0.9);          // new y end position                                            
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
  gPad->SetTickx(1); // 0 makes ticks in lower Xaxis, 1 makes ticks in both lower and upper Xaxis
  gPad->SetTicky(1); 

  // Access second pad
  myCanvas->cd(2);

  // Let's find the ratio of on-shell over all.                                                           
  auto result = new TH1F(*h1);
  result->Divide(h1, h3, 1.0, 1.0, "B"); // "B" means binomial error
  result->SetStats(0);                   // This deletes the statistics box of the histogram                                                     
  result->SetLineColor(kBlack);
  result->SetLineWidth(3);
  result->SetLabelSize(0.05, "xy");
  result->SetTitleSize(0.05, "xy");

  result->GetYaxis()->SetTitle("Ratio of 2015+2016/all");
  result->GetYaxis()->SetRangeUser(0, 1);
  result->GetXaxis()->SetTitle( (xtitle(histname)).c_str() );
  result->GetXaxis()->SetTitleSize(.07);

  gPad->SetTickx(1);
  gPad->SetTicky(1);
  result->Draw("histe");
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.2);
  
  myCanvas->Write();  // Write the histogram in the file
}

int nrebin(std::string histname){
  if (histname.find("mlll")      != std::string::npos) return 10;
  if (histname.find("njets")     != std::string::npos) return 1;
  if (histname.find("nbjets")    != std::string::npos) return 1;
  if (histname.find("q0q1")      != std::string::npos) return 1;
  if (histname.find("PhiLepMet") != std::string::npos) return 10;
  if (histname.find("m01")       != std::string::npos) return 10;
  if (histname.find("m12")       != std::string::npos) return 10;
  if (histname.find("m02")       != std::string::npos) return 10;
  if (histname.find("Met_Pt")    != std::string::npos) return 1;
  return 1;
}

std::string xtitle(std::string histname){
  if (histname.find("mlll")      != std::string::npos) return "m_{3l} [GeV]";
  if (histname.find("njets")     != std::string::npos) return "N(jets)";
  if (histname.find("nbjets")    != std::string::npos) return "N(b-jets)";
  if (histname.find("q0q1")      != std::string::npos) return "q(lead) #times q(sub-lead)";
  if (histname.find("PhiLepMet") != std::string::npos) return "#Delta#phi (3l,MET) [rad]";
  if (histname.find("m01")       != std::string::npos) return "m(lead,sub-lead) [GeV]";
  if (histname.find("m12")       != std::string::npos) return "m(sub-lead,third) [GeV]";
  if (histname.find("m02")       != std::string::npos) return "m(lead,third) [GeV]";
  if (histname.find("Met_Pt")    != std::string::npos) return "MET [GeV]";
  return "";
}

void adjust_range(std::string histname,TH1F* hist){
  double xmax;
  if (histname.find("mlll")      != std::string::npos) xmax = 700;
  if (histname.find("njets")     != std::string::npos) return;
  if (histname.find("nbjets")    != std::string::npos) return;
  if (histname.find("q0q1")      != std::string::npos) return;
  if (histname.find("PhiLepMet") != std::string::npos) return;
  if (histname.find("m01")       != std::string::npos) xmax = 600;
  if (histname.find("m12")       != std::string::npos) xmax = 460;
  if (histname.find("m02")       != std::string::npos) xmax = 440;
  if (histname.find("Met_Pt")    != std::string::npos) xmax = 250;
  hist->GetXaxis()->SetRangeUser(0, xmax);
  return;
}

void show_overflow(TH1F* hist, bool show_underflow=true, bool show_overflow=true){    
  int nbins             = hist->GetNbinsX();
  TAxis* xaxis          = hist->GetXaxis();
  int bin;  
  double underflow      = 0;
  double overflow       = 0;
  double underflowerror = 0;
  double overflowerror  = 0;
  int firstbin          = hist->GetBinContent(xaxis->GetFirst());
  int lastbin           = hist->GetBinContent(xaxis->GetLast());
  double firstbinerror  = hist->GetBinError(xaxis->GetFirst());
  double lastbinerror   = hist->GetBinError(xaxis->GetLast());
  double newcontent;
  double newerror;

  for (bin = 0; bin < (xaxis->GetFirst()); bin++)
    underflow += hist->GetBinContent(bin);
  for (bin = (xaxis->GetLast())+1; bin < nbins+2; bin++)
    overflow += hist->GetBinContent(bin);
  for (bin = 0; bin < (xaxis->GetFirst()); bin++)
    underflowerror += std::pow(hist->GetBinError(bin), 2);
  underflowerror = std::sqrt(underflowerror);
  for (bin = (xaxis->GetLast())+1; bin < nbins+2; bin++)
    overflowerror += std::pow(hist->GetBinError(bin), 2);
  overflowerror = std::sqrt(overflowerror);
  //  std::cout << "the overflow is: " <<  overflow << endl;
  
  if (show_underflow && underflow!=0){ 
    newcontent = underflow + firstbin; 
    if (firstbin == 0)
      newerror = underflowerror;
    else
      newerror = std::sqrt( underflowerror * underflowerror + firstbinerror * firstbinerror );
    hist->SetBinContent(xaxis->GetFirst(), newcontent);
    hist->SetBinError(xaxis->GetFirst(), newerror);
    for (bin=0; bin < (xaxis->GetFirst()); bin++){
      hist->SetBinContent(bin, 0);
      hist->SetBinError(bin, 0);
    }
  }

  if (show_overflow && overflow!=0){
    newcontent = overflow + lastbin;
    if (lastbin == 0)
      newerror = overflowerror;
    else
      newerror = std::sqrt( overflowerror * overflowerror + lastbinerror * lastbinerror );
    hist->SetBinContent(xaxis->GetLast(), newcontent);
    hist->SetBinError(xaxis->GetLast(), newerror);
    for (bin = (xaxis->GetLast())+1; bin < nbins+2; bin++){
      hist->SetBinContent(bin, 0);
      hist->SetBinError(bin, 0);
    }
  }
  return;
}
