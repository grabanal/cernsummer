void twohistograms(){

  std::vector<string> myvector = {};
  myvector.push_back("gabriel");
  myvector.push_back("ann");
  for (auto name : myvector) {
    cout << name << endl;
  }

  // Let's open the files
  TFile* in_file_1 = new TFile("/Users/grabanal/WWW/jobs-2018-05-21-15h04m35s/3w/hadd.root");
  TFile* in_file_2 = new TFile("/Users/grabanal/WWW/jobs-2018-05-21-15h04m35s/whww/hadd.root");
  TFile* in_file_3 = new TFile("/Users/grabanal/WWW/jobs-2018-05-21-15h04m35s/www/hadd.root");

  // Get the Histogram(s) out
  TH1F* h1 = new TH1F();
  TH1F* h2 = new TH1F();
  TH1F* h3 = new TH1F();
  in_file_1->GetObject("triboson_SFOS0_lep27_0b_sumq1/mlll",h1);
  in_file_2->GetObject("triboson_SFOS0_lep27_0b_sumq1/mlll",h2);
  in_file_3->GetObject("triboson_SFOS0_lep27_0b_sumq1/mlll",h3);

  // Format histogram 3 for WWW (all WWW events)
  h3->Rebin(10);
  h3->SetLineColor(kBlue);
  h3->SetTitle(";m_{3l} (GeV);Events");
  h3->SetLabelSize(0.05,"xy");
  h3->SetTitleSize(0.052,"xy");
  //  h3->GetXaxis()->SetTitle("m");
  //  h3->GetXaxis()->SetTitleSize(0.05);

  // Format histogram 1 for 3W (on-shell WWW events)
  h1->Rebin(10);
  h1->SetLineColor(kRed);
 
  // Format histogram 2 for WHWW
  h2->Rebin(10);
  h2->SetLineColor(kGreen+2);

  // Draw it(them)
  auto myCanvas = new TCanvas();
  myCanvas->Divide(1,2,0.001,0.001);
  myCanvas->cd(1);

  h1->GetXaxis()->SetRangeUser(0,700);
  h2->GetXaxis()->SetRangeUser(0,700);
  h3->GetXaxis()->SetRangeUser(0,700);

  h3->Draw("histe");
  h1->Draw("histesames");
  h2->Draw("histesames"); // both histograms appear with their own statistics box

  // Put a legend to identify which histogram is which:
  auto legend = new TLegend(0.6,0.7,0.77,0.9);
  legend->AddEntry(h3,"WWW (all)","l");  
  legend->AddEntry(h1,"WWW (on-shell)","l");
  legend->AddEntry(h2,"WH(#rightarrow WW*)","l");
  legend->Draw("samehist");

  // This is to move the statistics box that is hidden to a more convenient location
  gPad->Update();                                                                     

  TPaveStats* st3 = (TPaveStats*)h3->FindObject("stats");
  st3->SetX1NDC(0.8);
  st3->SetY1NDC(0.73); //new y start position                                          
  st3->SetX2NDC(0.9);
  st3->SetY2NDC(0.9); //new y end position
  st3->SetLineColor(kBlue);
  TPaveStats* st1 = (TPaveStats*)h1->FindObject("stats");
  st1->SetX1NDC(0.8);
  st1->SetY1NDC(0.53); //new y start position                                           
  st1->SetX2NDC(0.9);
  st1->SetY2NDC(0.7); //new y end position
  st1->SetLineColor(kRed);                                            
  TPaveStats* st2 = (TPaveStats*)h2->FindObject("stats");
  st2->SetX1NDC(0.8);
  st2->SetY1NDC(0.33);
  st2->SetX2NDC(0.9);
  st2->SetY2NDC(0.5);
  st2->SetLineColor(kGreen+2);

  myCanvas->Modified();




  // Now I need to find the fraction of 3W respect to the total. But I can't simply divide 3W and WHWW, I have to divide 3W and the sum of 3W and WHWW


  myCanvas->cd(2);

  auto result = new TH1F(*h1);
  result->Divide(h1,h3,1.0,1.0,"B");
  result->SetStats(0);  // Deletes its statistics box
  result->SetLineColor(kBlack);
  result->SetTitle(";m_{3l} (GeV);Ratio of WWW on-shell/all");
  result->SetLabelSize(0.05,"xy");
  result->SetTitleSize(0.052,"xy");
  result->Draw("histe");


  //  myCanvas->Update();
  // STORING THE FILE should I make this dynamic?
  // Let's open a TFile
  //  TFile out_file("twohistograms.root","RECREATE");
  // Write the histogram in the file
  //  h.Write();
  // Close the file
  //  out_file.Close();

}
