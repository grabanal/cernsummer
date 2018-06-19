void fiveminutes(){

  TH1F *h = new TH1F("h","example histogram",100,0.,5.);
  ifstream inp; 
  double x;
  cout << "openingfile" << endl;
  inp.open("/Users/grabanal/myroot/expo.dat");

  while ( inp >> x) {
    cout << x << endl;
    h->Fill(x); 
    }
  
  h->Draw();
  inp.close();
  cout << "closingfile" << endl;
}
