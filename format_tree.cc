using namespace std;

TH3* ReadXSecFile(const char* bname);
double GetXSec(TH3* h3, const double xBj, const double Q2, const double phi);

void format_tree(const char* label="sigma_ex", const char* Exy="Ex")
{
  const double M_p    = 0.938272; // Proton mass
  const double alpha  = 1.0/137;
  const double E_beam = 11.0; // 8.8; // Beam energy

  string fn_in  = Form("worksim/%s.root", label);
  string fn_out = Form("worksim/%s_fmt.root", label);
  
  TFile* file_in = new TFile(fn_in.c_str());

  TNamed* obj_n_gen   = file_in->Get<TNamed>("n_gen");
  TNamed* obj_p0_sp   = file_in->Get<TNamed>("p0_sp");
  TNamed* obj_th0_sp  = file_in->Get<TNamed>("th0_sp");
  TNamed* obj_dpp_lo  = file_in->Get<TNamed>("dpp_lo");
  TNamed* obj_dpp_hi  = file_in->Get<TNamed>("dpp_hi");
  TNamed* obj_dydz_lo = file_in->Get<TNamed>("dydz_lo");
  TNamed* obj_dydz_hi = file_in->Get<TNamed>("dydz_hi");
  TNamed* obj_dxdz_lo = file_in->Get<TNamed>("dxdz_lo");
  TNamed* obj_dxdz_hi = file_in->Get<TNamed>("dxdz_hi");

  int    n_gen   = atoi(obj_n_gen  ->GetTitle());
  double p0_sp   = atof(obj_p0_sp  ->GetTitle()) / 1000; // MeV -> GeV
  double th0_sp  = atof(obj_th0_sp ->GetTitle());
  double dpp_lo  = atof(obj_dpp_lo ->GetTitle());
  double dpp_hi  = atof(obj_dpp_hi ->GetTitle());
  double dydz_lo = atof(obj_dydz_lo->GetTitle()) / 1000; // mrad -> rad
  double dydz_hi = atof(obj_dydz_hi->GetTitle()) / 1000; // mrad -> rad
  double dxdz_lo = atof(obj_dxdz_lo->GetTitle()) / 1000; // mrad -> rad
  double dxdz_hi = atof(obj_dxdz_hi->GetTitle()) / 1000; // mrad -> rad
  cout << "n_gen       = " << n_gen << "\n"
       << "p0_sp       = " << p0_sp << "\n"
       << "th0_sp      = " << th0_sp << "\n"
       << "dpp_lo, hi  = " << dpp_lo << ", " << dpp_hi << "\n"
       << "dydz_lo, hi = " << dydz_lo << ", " << dydz_hi << "\n"
       << "dxdz_lo, hi = " << dxdz_lo << ", " << dxdz_hi << endl;
  double v_gen = (dpp_hi - dpp_lo) * (dydz_hi - dydz_lo) * (dxdz_hi - dxdz_lo);
  
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
  obj_n_gen  ->Write();
  obj_p0_sp  ->Write();
  obj_th0_sp ->Write();
  obj_dpp_lo ->Write();
  obj_dpp_hi ->Write();
  obj_dydz_lo->Write();
  obj_dydz_hi->Write();
  obj_dxdz_lo->Write();
  obj_dxdz_hi->Write();
  
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

  float xsec, weight;
  tree_out->Branch("xsec"  , &xsec  ,   "xsec/F");
  tree_out->Branch("weight", &weight, "weight/F");

  TH3* h3_xsec = ReadXSecFile(Exy);
  
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
    nu   = E_beam - p0;
    Q2   = 4 * E_beam * p0 * sin(th0/2) * sin(th0/2);
    W    = sqrt( M_p*M_p + 2*M_p*nu - Q2 );
    xBj  = Q2 / (2 * M_p * nu);

    xsec = GetXSec(h3_xsec, xBj, Q2, phi0*180/TMath::Pi());
    float jacob = (100 * nu) / (2 * pow(cos(th0), 3) * E_beam * p0_sp * p0 * xBj);
    weight = xsec * jacob * v_gen / n_gen;
    
    tree_out->Fill();
  }
  /// End of Event Loop ////////////////
  
  file_out->Write();
  file_out->Close();
  
  exit(0);
}

////  Functions  ////////////////////////////////////////////////////////////

/// Read the cross-section table from the csv files.
TH3* ReadXSecFile(const char* Exy)
{
  cout << "Exy = " << Exy << endl; // Ex or Ey
  string fname = (string)"xsec/sigma_" + Exy + ".csv";
  ifstream ifs(fname.c_str());
  string line;
  getline(ifs, line); // Discard the header line

  /// Read all entries
  vector<double> list_xBj;
  vector<double> list_Q2;
  vector<double> list_phi; // deg
  vector<double> list_xsec;
  set<double> set_xBj;
  set<double> set_Q2;
  set<double> set_phi;
  while (getline(ifs, line)) {
    istringstream iss(line);
    string field;
    vector<string> list_field;
    while (getline(iss, field, ',')) list_field.push_back(field);
    // label, x, Q2_GeV2, phi_rad, phi_deg, y, F2d, F1_fixedR, U, K2_nb_GeV2, Delta_spec, T, sigma_Ex_nb_GeV4
    double xBj  = stof(list_field[ 1]);
    double Q2   = stof(list_field[ 2]);
    double phi  = stof(list_field[ 4]);
    double xsec = stof(list_field[12]);
    list_xBj .push_back(xBj );
    list_Q2  .push_back(Q2  );
    list_phi .push_back(phi );
    list_xsec.push_back(xsec);
    set_xBj.insert(xBj);
    set_Q2 .insert(Q2 );
    set_phi.insert(phi);
  }
  ifs.close();
  if (list_xBj.size() < 100) {
    cout << "!!ERROR!!  Only " << list_xBj.size() << " entries were read." << endl;
    exit(1);
  }

  /// Extract the (xBj, Q2, phi) binning
  vector<double> vec;
  for (auto it = set_xBj.begin(); it != set_xBj.end(); it++) vec.push_back(*it);
  int n_xBj = vec.size();
  double bin_xBj[99];
  bin_xBj[0] = vec[0] - (vec[1] - vec[0]) / 2;
  for (auto ii = 0; ii < n_xBj - 1; ii++) bin_xBj[ii+1] = (vec[ii] + vec[ii+1]) / 2;
  bin_xBj[n_xBj] = vec[n_xBj-1] + (vec[n_xBj-1] - vec[n_xBj-2]) / 2;
  //cout << "bin_xBj: ";
  //for (int ii = 0; ii <= n_xBj; ii++) cout << " " << bin_xBj[ii];
  //cout << endl;
  
  vec.clear();
  for (auto it = set_Q2.begin(); it != set_Q2.end(); it++) vec.push_back(*it);
  int n_Q2 = vec.size();
  double bin_Q2[99];
  bin_Q2[0] = vec[0] - (vec[1] - vec[0]) / 2;
  for (auto ii = 0; ii < n_Q2 - 1; ii++) bin_Q2[ii+1] = (vec[ii] + vec[ii+1]) / 2;
  bin_Q2[n_Q2] = vec[n_Q2-1] + (vec[n_Q2-1] - vec[n_Q2-2]) / 2;
  //cout << "bin_Q2: ";
  //for (int ii = 0; ii <= n_Q2; ii++) cout << " " << bin_Q2[ii];
  //cout << endl;

  vec.clear();
  for (auto it = set_phi.begin(); it != set_phi.end(); it++) vec.push_back(*it);
  int n_phi = vec.size();
  double bin_phi[99];
  bin_phi[0] = vec[0] - (vec[1] - vec[0]) / 2;
  for (auto ii = 0; ii < n_phi - 1; ii++) bin_phi[ii+1] = (vec[ii] + vec[ii+1]) / 2;
  bin_phi[n_phi] = vec[n_phi-1] + (vec[n_phi-1] - vec[n_phi-2]) / 2;
  //cout << "bin_phi: ";
  //for (int ii = 0; ii <= n_phi; ii++) cout << " " << bin_phi[ii];
  //cout << endl;

  /// Set the xsec values on TH3.
  string hname = (string)"h3_sigma_" + Exy;
  TH3* h3 = new TH3D(hname.c_str(), ";x_{Bj};Q2;phi", n_xBj, bin_xBj,  n_Q2, bin_Q2,  n_phi, bin_phi);
  for (auto i_ent = 0; i_ent < list_xBj.size(); i_ent++) {
    double xBj  = list_xBj [i_ent];
    double Q2   = list_Q2  [i_ent];
    double phi  = list_phi [i_ent];
    double xsec = list_xsec[i_ent];
    int bin = h3->FindBin(xBj, Q2, phi);
    if (h3->GetBinContent(bin) != 0) {
      cout << "!!ERROR!!  Duplicated entry at (xBj Q2 phi)=(" << xBj << " " << Q2 << " " << phi << ")." << endl;
      exit(1);
    }
    h3->SetBinContent(bin, xsec);
  }
  
  return h3;
}

/// Return the cross section at the given kinematics using TH3::Interpolate().
/**
 * This function checks if the four (xBj, Q2) points used in the interpolation are valid (i.e. non-zero value).
 * Return 0 if it is invalid.
 * Here it is assumed that the interpolation in phi is always valid.
 */
double GetXSec(TH3* h3, const double xBj, const double Q2, const double phi)
{
  int ix1, iy1, iz1;
  h3->GetBinXYZ(h3->FindBin(xBj, Q2, phi), ix1, iy1, iz1);
  int ix2 = (xBj >= h3->GetXaxis()->GetBinCenter(ix1)  ?  ix1+1  :  ix1-1);
  int iy2 = (Q2  >= h3->GetYaxis()->GetBinCenter(iy1)  ?  iy1+1  :  iy1-1);
  //int iz2 = (phi >= h3->GetZaxis()->GetBinCenter(iz1)  ?  iz1+1  :  iz1-1);
  if (h3->GetBinContent(ix1, iy1, iz1) == 0) return 0;
  if (h3->GetBinContent(ix1, iy2, iz1) == 0) return 0;
  if (h3->GetBinContent(ix2, iy1, iz1) == 0) return 0;
  if (h3->GetBinContent(ix2, iy2, iz1) == 0) return 0;
  
  return h3->Interpolate(xBj, Q2, phi);
}
