using namespace std;
void eval_stat(const char* label="sigma_ex")
{
  string fn_in = Form("worksim/%s_fmt.root", label);
  
  TFile* file = new TFile(fn_in.c_str());
  TTree* tree = (TTree*)file->Get("tree");

  double p0_sp  = atof(file->Get<TNamed>( "p0_sp")->GetTitle()) / 1000; // MeV -> GeV
  double th0_sp = atof(file->Get<TNamed>("th0_sp")->GetTitle());
  
  ostringstream oss;
  oss << "p0_sp = " << p0_sp << ",  th0_sp = " << th0_sp;
  string label1 = oss.str();
  
  float stop_id;
  float phi0;
  float nu, Q2, W, xBj;
  float xsec, weight;
  tree->SetBranchAddress("stop_id", &stop_id);
  tree->SetBranchAddress("phi0", &phi0);
  tree->SetBranchAddress("nu" , &nu );
  tree->SetBranchAddress("Q2" , &Q2 );
  tree->SetBranchAddress("W"  , &W  );
  tree->SetBranchAddress("xBj", &xBj);
  tree->SetBranchAddress("xsec"  , &xsec  );
  tree->SetBranchAddress("weight", &weight);

  TH1* h1_phi0   = new TH1D("h1_phi0", "", 100, 0, TMath::Pi());
  TH1* h1_nu     = new TH1D("h1_nu"    , "", 100, 0, 10);
  TH1* h1_W      = new TH1D("h1_W"     , "", 100, 0, 10);
  //TH2* h2_xBj_Q2 = new TH2D("h1_xBj_Q2", "", 100, 0, 1,  100, 0, 20);
  TH2* h2_xBj_Q2 = new TH2D("h1_xBj_Q2", "", 30, 0.0, 0.3,  20, 0, 4);
  h1_phi0  ->SetTitle(Form("%s;#phi_{0} (rad);"   , label1.c_str()));
  h1_nu    ->SetTitle(Form("%s;#nu;", label1.c_str()));
  h1_W     ->SetTitle(Form("%s;W;"  , label1.c_str()));
  h2_xBj_Q2->SetTitle(Form("%s;x_{Bj};Q^{2} (GeV^{2})", label1.c_str()));
  
  /// Event Loop ////////////////
  int n_ent = tree->GetEntries();
  cout << "n_ent = " << n_ent << endl;
  for (int i_ent = 0; i_ent < n_ent; i_ent++) {
    tree->GetEntry(i_ent);
    if (stop_id != 0) continue;
    if (weight == 0) continue;    
        
    h1_phi0  ->Fill(phi0   , weight);
    h1_nu    ->Fill(nu     , weight);
    h1_W     ->Fill(W      , weight);
    h2_xBj_Q2->Fill(xBj, Q2, weight);
  }
  /// End of Event Loop ////////////////

  /// Scale the contents with the integrated luminosity
  const double I_BEAM = 85e-9; // A
  const double ELE = 1.602e-19; // C
  const double FLUX = I_BEAM / ELE;
  const double RHO_W = 1.007; // g/cm3, weight density, ND3
  const double N_A = 6.022e23; // 1/mol
  const double RHO_N = RHO_W * N_A; // 1/cm3, nucleon density (1 g = 1 mol)
  const double L_TGT = 3.0; // cm
  double lumi = FLUX * RHO_N * L_TGT * 1e-33; // 1/(nb*s)
  const double T_MEAS = 24*3600; // s
  double lumi_inte = lumi * T_MEAS; // 1/nb
  cout << "lumi      = " << lumi << " /nb/s\n"
       << "lumi_inte = " << lumi_inte << " /nb\n";
  h1_phi0  ->Scale(lumi_inte);
  h1_nu    ->Scale(lumi_inte);
  h1_W     ->Scale(lumi_inte);
  h2_xBj_Q2->Scale(lumi_inte);
  h1_phi0  ->GetYaxis()->SetTitle("Yield per day");
  h1_nu    ->GetYaxis()->SetTitle("Yield per day");
  h1_W     ->GetYaxis()->SetTitle("Yield per day");
  h2_xBj_Q2->GetZaxis()->SetTitle("Yield per day");
  
  /// Draw plots
  gSystem->mkdir(Form("stat/%s", label), true);

  gErrorIgnoreLevel = 1111;
  TCanvas* c1 = new TCanvas("c1", "");
  c1->SetGrid();

  h1_phi0->Draw();  c1->SaveAs(Form("stat/%s/h1_phi0.png", label));
  h1_nu  ->Draw();  c1->SaveAs(Form("stat/%s/h1_nu.png", label));
  h1_W   ->Draw();  c1->SaveAs(Form("stat/%s/h1_W.png", label));

  gStyle->SetOptStat(0);
  h2_xBj_Q2->Draw("colz");  c1->SaveAs(Form("stat/%s/h2_xBj_Q2.png", label));

  double phi0_inte_err;
  double phi0_inte = h1_phi0->IntegralAndError(1, h1_phi0->GetNbinsX(), phi0_inte_err);
  
  ofstream ofs(Form("stat/%s/result.txt", label));
  ofs << "h1_phi0: " << phi0_inte << " +- " << phi0_inte_err << "\n";
  ofs.close();
  
  exit(0);
}
