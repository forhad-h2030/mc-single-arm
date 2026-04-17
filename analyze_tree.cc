using namespace std;
void analyze_tree(const char* label="sigma_ex")
{
  string fn_in = Form("worksim/%s_fmt.root", label);
  
  TFile* file = new TFile(fn_in.c_str());
  TTree* tree = (TTree*)file->Get("tree");

  double p0_sp  = atof(file->Get<TNamed>( "p0_sp")->GetTitle());
  double th0_sp = atof(file->Get<TNamed>("th0_sp")->GetTitle());
  //cout << "p0_sp, th0_sp  =  " << p0_sp << ", " << th0_sp << endl;
  
  bool in_acc = true;
  //string label_acc = in_acc ? "In Acceptance" : "All Generated";
  //string label_acc = Form("p0_sp = %f, th0_sp = %f, %s", p0_sp, th0_sp, (in_acc ? "In Acceptance" : "All Generated"));
  ostringstream oss;
  oss << "p0_sp = " << p0_sp << ",  th0_sp = " << th0_sp << ",  "
      << (in_acc ? "In Acceptance" : "All Generated");
  string label_acc = oss.str();
  
  float dpp, dydz, dxdz;
  tree->SetBranchAddress("dpp" , &dpp );
  tree->SetBranchAddress("dydz", &dydz);
  tree->SetBranchAddress("dxdz", &dxdz);
  TH1* h1_dpp  = new TH1D("h1_dpp" , "" , 100, -50, 50);
  TH1* h1_dydz = new TH1D("h1_dydz", "", 100, -0.1, 0.1);
  TH1* h1_dxdz = new TH1D("h1_dxdz", "", 100, -0.1, 0.1);
  h1_dpp ->SetTitle(Form("%s;dpp;" , label_acc.c_str()));
  h1_dydz->SetTitle(Form("%s;dydz;", label_acc.c_str()));
  h1_dxdz->SetTitle(Form("%s;dxdz;", label_acc.c_str()));

  float stop_id;
  tree->SetBranchAddress("stop_id", &stop_id);
  TH1* h1_stop_id = new TH1D("h1_stop_id", "",  30, -0.5, 29.5);
  h1_stop_id->SetTitle(Form("%s;SHMS Stop ID;", label_acc.c_str()));
  
  float p0, th0y, th0, phi0;
  tree->SetBranchAddress("p0"  , &p0  );
  tree->SetBranchAddress("th0y", &th0y);
  tree->SetBranchAddress("th0" , &th0 );
  tree->SetBranchAddress("phi0", &phi0);
  TH1* h1_p0   = new TH1D("h1_p0"  , "", 100, 0, 10);
  TH1* h1_th0y = new TH1D("h1_th0y", "", 100, 0, TMath::Pi());
  TH1* h1_th0  = new TH1D("h1_th0" , "", 100, 0, TMath::Pi());
  TH1* h1_phi0 = new TH1D("h1_phi0", "", 100, -TMath::Pi(), TMath::Pi());
  TH2* h2_p0_th0 = new TH2D("h2_p0_th0", "", 100, 0, 10,  100, 0, TMath::Pi());
  h1_p0    ->SetTitle(Form("%s;p_{0} (GeV);"      , label_acc.c_str()));
  h1_th0y  ->SetTitle(Form("%s;#theta_{0y} (rad);", label_acc.c_str()));
  h1_th0   ->SetTitle(Form("%s;#theta_{0} (rad);" , label_acc.c_str()));
  h1_phi0  ->SetTitle(Form("%s;#phi_{0} (rad);"   , label_acc.c_str()));
  h2_p0_th0->SetTitle(Form("%s;p_{0} (GeV);#theta_{0} (rad)", label_acc.c_str()));
  
  float nu, Q2, W, xBj;
  tree->SetBranchAddress("nu" , &nu );
  tree->SetBranchAddress("Q2" , &Q2 );
  tree->SetBranchAddress("W"  , &W  );
  tree->SetBranchAddress("xBj", &xBj);
  TH1* h1_nu     = new TH1D("h1_nu"    , "", 100, 0, 10);
  TH1* h1_W      = new TH1D("h1_W"     , "", 100, 0, 10);
  TH2* h2_xBj_Q2 = new TH2D("h1_xBj_Q2", "", 100, 0, 1,  100, 0, 20);
  h1_nu    ->SetTitle(Form("%s;#nu;", label_acc.c_str()));
  h1_W     ->SetTitle(Form("%s;W;"  , label_acc.c_str()));
  h2_xBj_Q2->SetTitle(Form("%s;x_{Bj};Q^{2} (GeV^{2})", label_acc.c_str()));

  //float mott, xsec, weight;
  // ...to be written...
  
  /// Event Loop ////////////////
  int n_ent = tree->GetEntries();
  cout << "n_ent = " << n_ent << endl;
  for (int i_ent = 0; i_ent < n_ent; i_ent++) {
    tree->GetEntry(i_ent);
    if (in_acc && stop_id != 0) continue;

    h1_dpp ->Fill(dpp);
    h1_dydz->Fill(dydz);
    h1_dxdz->Fill(dxdz);

    h1_stop_id->Fill(stop_id);
    
    h1_p0    ->Fill(p0  );
    h1_th0y  ->Fill(th0y);
    h1_th0   ->Fill(th0 );
    h1_phi0  ->Fill(phi0);
    h2_p0_th0->Fill(p0, th0);

    h1_nu    ->Fill(nu);
    h1_W     ->Fill(W);
    h2_xBj_Q2->Fill(xBj, Q2);
  }
  /// End of Event Loop ////////////////
  
  gSystem->mkdir(Form("result/%s", label), true);

  TCanvas* c1 = new TCanvas("c1", "");
  c1->SetGrid();

  h1_dpp ->Draw();  c1->SaveAs(Form("result/%s/h1_dpp.png", label));
  h1_dydz->Draw();  c1->SaveAs(Form("result/%s/h1_dydz.png", label));
  h1_dxdz->Draw();  c1->SaveAs(Form("result/%s/h1_dxdz.png", label));
  
  h1_stop_id->Draw();  c1->SaveAs(Form("result/%s/h1_stop_id.png", label));

  h1_p0  ->Draw();  c1->SaveAs(Form("result/%s/h1_p0.png", label));
  h1_th0y->Draw();  c1->SaveAs(Form("result/%s/h1_th0y.png", label));
  h1_th0 ->Draw();  c1->SaveAs(Form("result/%s/h1_th0.png", label));
  h1_phi0->Draw();  c1->SaveAs(Form("result/%s/h1_phi0.png", label));

  h1_nu->Draw();  c1->SaveAs(Form("result/%s/h1_nu.png", label));
  h1_W ->Draw();  c1->SaveAs(Form("result/%s/h1_W.png", label));

  gStyle->SetOptStat(0);
  h2_p0_th0->Draw("colz");  c1->SaveAs(Form("result/%s/h2_p0_th0.png", label));
  h2_xBj_Q2->Draw("colz");  c1->SaveAs(Form("result/%s/h2_xBj_Q2.png", label));

  exit(0);
}
