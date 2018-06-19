void ageshistogram() {

  // Create a histogram that can accept the data
  TH1F* h = new TH1F("h","histogram of ages of people here",100,0.,40.);
  // Open the file with the data
  ifstream inp;
  double x;
  inp.open("/Users/grabanal/myroot/ages.dat");
  // Let's fill the histogram with the stuff of ages.dat
  while (inp >> x) {
    h->Fill(x);
  }
  // Let's draw the histogram and close the stream
  h->Draw();
  inp.close();

  // Let's open a TFile, with the option RECREATE to force ROOT to create a new file even if a file with the same name exists on disk
  TFile out_file("/Users/grabanal/ageshistogram.root","RECREATE");
  // Write my histogram in the file
  h->Write();
  // Close the file
  out_file.Close();

}
