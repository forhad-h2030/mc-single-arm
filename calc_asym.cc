#include "Lumi.h"
#include "HistSet.h"
using namespace std;
//const double phi0_lo = 1.45;
//const double phi0_hi = 1.60;
const double phi0_lo = 1.40;
const double phi0_hi = 1.70;

//// Main ////
void calc_asym(const char* kin="p00_th00")
{
  double lumi = GetLumi();
  const double T_MEAS = 24*3600; // s
  double lumi_inte = lumi * T_MEAS; // 1/nb
  cout << "lumi      = " << lumi << " /nb/s\n"
       << "lumi_inte = " << lumi_inte << " /nb\n";
  
  string fn_in_ex = Form("worksim/sigma_ex_%s_fmt.root", kin);
  string fn_in_ey = Form("worksim/sigma_ey_%s_fmt.root", kin);

  TFile* file_ex = new TFile(fn_in_ex.c_str());
  TTree* tree_ex = (TTree*)file_ex->Get("tree");
  HistSet hs_ex_all("ex_all");
  HistSet hs_ex_acc("ex_acc");
  FillHistSet(tree_ex, &hs_ex_all, &hs_ex_acc);
  hs_ex_all.Scale(lumi_inte);
  hs_ex_acc.Scale(lumi_inte);
  
  TFile* file_ey = new TFile(fn_in_ey.c_str());
  TTree* tree_ey = (TTree*)file_ey->Get("tree");
  HistSet hs_ey_all("ey_all");
  HistSet hs_ey_acc("ey_acc");
  FillHistSet(tree_ey, &hs_ey_all, &hs_ey_acc);    
  hs_ey_all.Scale(lumi_inte);
  hs_ey_acc.Scale(lumi_inte);

  /// Draw yields
  gSystem->mkdir(Form("asym/%s", kin), true);

  gErrorIgnoreLevel = 1111;
  TCanvas* c1 = new TCanvas("c1", "");
  c1->SetGrid();

  hs_ex_all.h1_phi0->SetLineColor(kRed);
  hs_ex_all.h1_phi0->GetYaxis()->SetTitle("Yield per day");
  hs_ex_all.h1_phi0->Draw();
  hs_ex_acc.h1_phi0->Draw("same");
  c1->SaveAs(Form("asym/%s/h1_phi0_ex.png", kin));
  
  hs_ey_all.h1_phi0->SetLineColor(kRed);
  hs_ey_all.h1_phi0->GetYaxis()->SetTitle("Yield per day");
  hs_ey_all.h1_phi0->Draw();
  hs_ey_acc.h1_phi0->Draw("same");
  c1->SaveAs(Form("asym/%s/h1_phi0_ey.png", kin));
  
  /// Compute and draw the acceptance factor
  TH1* h1_acc_phi0_ex = (TH1*)hs_ex_acc.h1_phi0->Clone("h1_acc_phi0_ex");
  h1_acc_phi0_ex->Divide(hs_ex_all.h1_phi0);
  TH2* h2_acc_xBj_Q2_ex = (TH2*)hs_ex_acc.h2_xBj_Q2->Clone("h2_acc_xBj_Q2_ex");
  h2_acc_xBj_Q2_ex->Divide(hs_ex_all.h2_xBj_Q2);

  TH1* h1_acc_phi0_ey = (TH1*)hs_ey_acc.h1_phi0->Clone("h1_acc_phi0_ey");
  h1_acc_phi0_ey->Divide(hs_ey_all.h1_phi0);
  TH2* h2_acc_xBj_Q2_ey = (TH2*)hs_ey_acc.h2_xBj_Q2->Clone("h2_acc_xBj_Q2_ey");
  h2_acc_xBj_Q2_ey->Divide(hs_ey_all.h2_xBj_Q2);

  //double p0_sp  = atof(file_ex->Get<TNamed>( "p0_sp")->GetTitle()) / 1000; // MeV -> GeV
  //double th0_sp = atof(file_ex->Get<TNamed>("th0_sp")->GetTitle());
  //ostringstream oss;
  //oss << "p0_sp = " << p0_sp << ",  th0_sp = " << th0_sp;
  //string label1 = oss.str();

  h1_acc_phi0_ex->Draw();
  h1_acc_phi0_ex->GetYaxis()->SetTitle("Acceptance");
  c1->SaveAs(Form("asym/%s/h1_acc_phi0_ex.png", kin));

  h1_acc_phi0_ey->Draw();
  h1_acc_phi0_ey->GetYaxis()->SetTitle("Acceptance");
  c1->SaveAs(Form("asym/%s/h1_acc_phi0_ey.png", kin));

  int opt_stat_org = gStyle->GetOptStat();
  gStyle->SetOptStat(0);

  //hs_ex_acc.h2_xBj_Q2->Draw("colz");
  //c1->SaveAs(Form("asym/%s/h2_xBj_Q2_ex_acc.png", kin));
  //
  //hs_ex_all.h2_xBj_Q2->Draw("colz");
  //c1->SaveAs(Form("asym/%s/h2_xBj_Q2_ex_all.png", kin));
  
  h2_acc_xBj_Q2_ex->Draw("colz");
  h2_acc_xBj_Q2_ex->GetZaxis()->SetTitle("Acceptance");
  c1->SaveAs(Form("asym/%s/h2_acc_xBj_Q2_ex.png", kin));

  h2_acc_xBj_Q2_ey->Draw("colz");
  h2_acc_xBj_Q2_ey->GetZaxis()->SetTitle("Acceptance");
  c1->SaveAs(Form("asym/%s/h2_acc_xBj_Q2_ey.png", kin));

  gStyle->SetOptStat(opt_stat_org);

  /// Compute and draw the asymmetry.
  /// The "all" yields are used, assuming the acc. correction works perfectly.
  TH1* h1_asym = (TH1*)hs_ex_all.h1_phi0->Clone("h1_asym");
  h1_asym->Reset();
  for (int ib = 1; ib <= h1_asym->GetNbinsX(); ib++) {
    double c1 = hs_ex_all.h1_phi0->GetBinContent(ib);
    double c2 = hs_ey_all.h1_phi0->GetBinContent(ib);
    if (c1 == 0 || c2 == 0) continue;
    double e1 = hs_ex_all.h1_phi0->GetBinError  (ib);
    double e2 = hs_ey_all.h1_phi0->GetBinError  (ib);
    double asym = (c1 - c2) / (c1 + c2);
    double err2 = 4 * (pow(c2*e1, 2) + pow(c1*e2, 2)) / pow(c1+c2, 4);
    h1_asym->SetBinContent(ib, asym);
    h1_asym->SetBinError  (ib, sqrt(err2));
  }
  h1_asym->SetLineColor(kBlack);
  h1_asym->Draw();
  h1_asym->GetYaxis()->SetRangeUser(-0.05, 0.05);
  h1_asym->GetYaxis()->SetTitle("Asymmetry: A_Exy");
  gStyle->SetOptFit(true);

  TF1* f1 = new TF1("f1", "[0]*cos(2*x)", phi0_lo, phi0_hi);
  h1_asym->Fit("f1", "R");

  TLatex tex;
  tex.SetNDC(true);
  tex.DrawLatex(0.15, 0.85, Form("Fit range: %.2f < #phi_{0} < %.2f", phi0_lo, phi0_hi));
  
  c1->SaveAs(Form("asym/%s/h1_asym.png", kin));
  
  exit(0);
}
