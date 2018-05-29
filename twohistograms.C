void twohistograms(){

  // Let's open the files
  TFile* in_file_1 = new TFile("/Users/grabanal/WWW/jobs-2018-05-21-15h04m35s/3w/hadd.root");
  TFile* in_file_2 = new TFile("/Users/grabanal/WWW/jobs-2018-05-21-15h04m35s/whww/hadd.root");

  // Get the Histogram(s) out
  TH1F* h1 = new TH1F();
  TH1F* h2 = new TH1F();
  in_file_1->GetObject("triboson_SFOS0_lep27_0b_sumq1/mlll",h1);
  in_file_2->GetObject("triboson_SFOS0_lep27_0b_sumq1/mlll",h2);

  // Let's open the files                                                                               
  TFile* in_file_3 = new TFile("/Users/grabanal/WWW/jobs-2018-05-21-15h04m35s/www/hadd.root");

  // Get the Histogram(s) out                                                                           
  TH1F* h3 = new TH1F();
  in_file_3->GetObject("triboson_SFOS0_lep27_0b_sumq1/mlll",h3);
  h3->Rebin(10);







  // Format histogram 1 for 3W
  h1->SetTitle("3W vs. WHWW;m_{3l};Events");
  h1->Rebin(10);
  h1->SetLineColor(kBlue);
  h1->SetLineWidth(2);
  //  h1->SetFillStyle(2);
  //  h1->SetLineStyle(9);
 
  // Format histogram 2 for WHWW
  h2->Rebin(10);
  h2->SetLineColor(kRed);
  h2->SetLineWidth(2);
  
  // Draw it(them)
  auto myCanvas = new TCanvas();
  myCanvas->Divide(1,2,0,0);




  myCanvas->cd(1);
  myCanvas->GetPad(1)->SetRightMargin(.01);

  h3->Draw("histe");
  h1->Draw("histesames");
  h2->Draw("histesames"); // This makes both histograms appear with their own statistics box

  // Put a legend to identify which histogram is which:
  auto legend = new TLegend(0.1,0.8,0.23,0.9);
  legend->SetHeader("The Legend Title","C"); // option "C" allows to center the header
  legend->AddEntry(h3,"WWW (all)","l");  
legend->AddEntry(h1,"WWW (on-shell)","l");
  legend->AddEntry(h2,"WH(#rightarrow WW*)","l");
  legend->Draw("samehist");

  // This is to move the statistics box that is hidden to a more convenient location
  gPad->Update();                                                                                
  TPaveStats* st = (TPaveStats*)h2->FindObject("stats");
  st->SetY1NDC(0.6); //new y start position                                                          
  st->SetY2NDC(0.75); //new y end position                                                            
  myCanvas->Modified();




  // Now I need to find the fraction of 3W respect to the total. But I can't simply divide 3W and WHWW, I have to divide 3W and the sum of 3W and WHWW


  // Sum
  auto sum_h= new TH1F(*h1);
  sum_h->Add(h2,1.);
  sum_h->SetTitle("3W+WHWW;m_{3l};Events");
  //  format_h(sum_h,kOrange);

  // Divide
  //  auto dividend = new TH1F(*h1);
  //  dividend->Divide(sum_h);




  // Graphical Maquillage
  ////  dividend->SetTitle(";m_{3l};Fraction");
  ////   dividend->SetLineColor(kOrange);
  ////  dividend->SetStats(0);
  //  gStyle->SetOptStat(0);

  myCanvas->cd(2);


  //  dividend->GetYaxis()->SetRangeUser(0,2.49);
  //  myCanvas->GetPad(2)->SetGridy();
  // //  myCanvas->GetPad(2)->SetRightMargin(.01);
  // //  dividend->Draw();
  auto result = new TH1F(*h1);
  cout << h1->GetNbinsX() << " ";
  cout << h3->GetNbinsX() << " ";
  cout << result->GetNbinsX() << " " << endl; 
result->Divide(h1,h3,1.0,1.0,"B");
  result->Draw();


  // STORING THE FILE should I make this dynamic?
  // Let's open a TFile
  //  TFile out_file("twohistograms.root","RECREATE");
  // Write the histogram in the file
  //  h.Write();
  // Close the file
  //  out_file.Close();

}
