double xBj_lo = 0.0;
double xBj_hi = 1.0;
double  Q2_lo = 1.0;
double  Q2_hi = 1e10;

struct HistSet {
  TH1* h1_phi0;
  TH2* h2_xBj_Q2;
  HistSet(const string name) {
    h1_phi0   = new TH1D(("h1_phi0_"+name).c_str(), "", 100, 0, TMath::Pi());
    h2_xBj_Q2 = new TH2D(("h1_xBj_Q2_"+name).c_str(), "", 10, xBj_lo, xBj_hi,  10, Q2_lo, Q2_hi);
    h1_phi0  ->SetTitle(";#phi_{0} (rad);");
    h2_xBj_Q2->SetTitle(";x_{Bj};Q^{2} (GeV^{2})");
  };
  void Scale(const double scale) {
     h1_phi0  ->Scale(scale);
     h2_xBj_Q2->Scale(scale);
  };
};

void FillHistSet(TTree* tree, HistSet* hs_all, HistSet* hs_acc)
{
  float stop_id, phi0, Q2, xBj, weight;
  tree->SetBranchAddress("stop_id", &stop_id);
  tree->SetBranchAddress("phi0", &phi0);
  tree->SetBranchAddress("Q2" , &Q2 );
  tree->SetBranchAddress("xBj", &xBj);
  tree->SetBranchAddress("weight", &weight);
  
  int n_ent = tree->GetEntries();
  //cout << "n_ent = " << n_ent << endl;
  for (int i_ent = 0; i_ent < n_ent; i_ent++) {
    tree->GetEntry(i_ent);
    if (weight == 0 ||
        xBj < xBj_lo || xBj > xBj_hi ||
        Q2  <  Q2_lo || Q2  >  Q2_hi   ) continue;
    hs_all->h1_phi0  ->Fill(phi0   , weight);
    hs_all->h2_xBj_Q2->Fill(xBj, Q2, weight);
    if (stop_id == 0) {
      hs_acc->h1_phi0  ->Fill(phi0   , weight);
      hs_acc->h2_xBj_Q2->Fill(xBj, Q2, weight);
    }
  }
}

