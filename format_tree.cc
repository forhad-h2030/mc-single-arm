using namespace std;
void format_tree(const char* label="sigma_ex")
{
  const double M_p = 0.938272; // Proton mass
  const double alpha = 1.0/137;
  const double E0     = 8.8;

  string fn_in  = Form("worksim/%s.root", label);
  string fn_out = Form("worksim/%s_fmt.root", label);
  //string fn_par = Form("worksim/%s.param", label);
  
  //ifstream ifs(fn_par.c_str());
  //map<string, string> map_param;
  //string key, val;
  //while (ifs >> key >> val) map_param[key] = val;
  //ifs.close();
  //double p0_sp  = stof(map_param["p0_sp" ]); // Spectrometer momentum (GeV)
  //double th0_sp = stof(map_param["th0_sp"]); // Spectrometer angle (deg)
  //cout << "p0_sp  = " << p0_sp << "\n"
  //     << "th0_sp = " << th0_sp << endl;
  
  TFile* file_in = new TFile(fn_in.c_str());

  TNamed* obj_p0_sp  = file_in->Get<TNamed>( "p0_sp");
  TNamed* obj_th0_sp = file_in->Get<TNamed>("th0_sp");
  
  double p0_sp  = atof(obj_p0_sp ->GetTitle());
  double th0_sp = atof(obj_th0_sp->GetTitle());
  cout << "p0_sp  = " << p0_sp << "\n"
       << "th0_sp = " << th0_sp << endl;
  
  TTree* tree_in = (TTree*)file_in->Get("h1411");

  // 'psdeltai', 'psyptari', 'psxptari' = dpp_init, dth_init/1000 = dydz_s, dph_init/1000 = dxdz_s
  float dpp, dydz, dxdz;
  tree_in->SetBranchAddress("psdeltai", &dpp ); // init
  tree_in->SetBranchAddress("psyptari", &dydz); // init
  tree_in->SetBranchAddress("psxptari", &dxdz); // init
  //tree->SetBranchAddress("psdelta", &dpp ); // recon
  //tree->SetBranchAddress("psyptar", &dydz); // recon
  //tree->SetBranchAddress("psxptar", &dxdz); // recon
  float stop_id;
  tree_in->SetBranchAddress("stop_id", &stop_id);

  TFile* file_out = new TFile(fn_out.c_str(), "RECREATE");
  obj_p0_sp ->Write();
  obj_th0_sp->Write();
  
  TTree* tree_out = new TTree("tree", "");
  tree_out->Branch("dpp" , &dpp , "dpp/F");
  tree_out->Branch("dydz", &dydz, "dydz/F");
  tree_out->Branch("dxdz", &dxdz, "dxdz/F");
  tree_out->Branch("stop_id", &stop_id, "stop_id/F");
  
  float p0, th0y, th0, phi0;
  tree_out->Branch("p0"  , &p0  ,   "p0/F");
  tree_out->Branch("th0y", &th0y, "th0y/F");
  tree_out->Branch("th0" , &th0 ,  "th0/F");
  tree_out->Branch("phi0", &phi0, "phi0/F");

  float nu, Q2, W, xBj;
  tree_out->Branch("nu" , &nu ,  "nu/F");
  tree_out->Branch("Q2" , &Q2 ,  "Q2/F");
  tree_out->Branch("W"  , &W  ,   "W/F");
  tree_out->Branch("xBj", &xBj, "xBj/F");

  float mott, xsec, weight;
  tree_out->Branch("mott"  , &mott  ,   "mott/F");
  tree_out->Branch("xsec"  , &xsec  ,   "xsec/F");
  tree_out->Branch("weight", &weight, "weight/F");

  /// Event Loop ////////////////
  int n_ent = tree_in->GetEntries();
  cout << "n_ent = " << n_ent << endl;
  for (int i_ent = 0; i_ent < n_ent; i_ent++) {
    tree_in->GetEntry(i_ent);
    //if (stop_id != 0) continue;

    p0   = p0_sp * (1 + dpp/100);
    th0y = dydz + th0_sp * TMath::Pi() / 180;
    th0  = acos( cos(th0y)*cos(dxdz) );
    phi0 = atan2f(th0y, dxdz);
    nu   = E0 - p0;
    Q2   = 4 * E0 * p0 * sin(th0/2) * sin(th0/2);
    W    = sqrt( M_p*M_p + 2*M_p*nu - Q2 );
    xBj  = Q2 / (2 * M_p * nu);

    mott = alpha*alpha * cos(th0/2)*cos(th0/2) * p0 / (4 * pow(E0, 3) * pow(sin(th0/2), 4));
    xsec = mott; // !!TEMP!!
    weight = xsec / mott;
    
    tree_out->Fill();
  }
  /// End of Event Loop ////////////////
  
  file_out->Write();
  file_out->Close();
  
  exit(0);
}
