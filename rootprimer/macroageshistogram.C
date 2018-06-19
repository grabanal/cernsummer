void macroageshistogram()
{
//=========Macro generated from canvas: c1/c1
//=========  (Thu May 24 16:40:04 2018) by ROOT version 6.12/06
   TCanvas *c1 = new TCanvas("c1", "c1",10,45,700,500);
   c1->Range(-5,-0.2625,45,2.3625);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   
   TH1F *h__2 = new TH1F("h__2","histogram of ages of people here",100,0,40);
   h__2->SetBinContent(63,2);
   h__2->SetBinContent(68,1);
   h__2->SetBinContent(76,1);
   h__2->SetEntries(4);
   
   TPaveStats *ptstats = new TPaveStats(0.78,0.775,0.98,0.935,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(1);
   ptstats->SetFillColor(0);
   ptstats->SetTextAlign(12);
   ptstats->SetTextFont(42);
   TText *ptstats_LaTex = ptstats->AddText("h");
   ptstats_LaTex->SetTextSize(0.0368);
   ptstats_LaTex = ptstats->AddText("Entries = 4      ");
   ptstats_LaTex = ptstats->AddText("Mean  =  26.75");
   ptstats_LaTex = ptstats->AddText("Std Dev   =  2.046");
   ptstats->SetOptStat(1111);
   ptstats->SetOptFit(0);
   ptstats->Draw();
   h__2->GetListOfFunctions()->Add(ptstats);
   ptstats->SetParent(h__2);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   h__2->SetLineColor(ci);
   h__2->GetXaxis()->SetLabelFont(42);
   h__2->GetXaxis()->SetLabelSize(0.035);
   h__2->GetXaxis()->SetTitleSize(0.035);
   h__2->GetXaxis()->SetTitleFont(42);
   h__2->GetYaxis()->SetLabelFont(42);
   h__2->GetYaxis()->SetLabelSize(0.035);
   h__2->GetYaxis()->SetTitleSize(0.035);
   h__2->GetYaxis()->SetTitleOffset(0);
   h__2->GetYaxis()->SetTitleFont(42);
   h__2->GetZaxis()->SetLabelFont(42);
   h__2->GetZaxis()->SetLabelSize(0.035);
   h__2->GetZaxis()->SetTitleSize(0.035);
   h__2->GetZaxis()->SetTitleFont(42);
   h__2->Draw("");
   
   TPaveText *pt = new TPaveText(0.2199427,0.9368947,0.7800573,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *pt_LaTex = pt->AddText("histogram of ages of people here");
   pt->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
