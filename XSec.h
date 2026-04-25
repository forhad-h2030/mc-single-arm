
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
  if (ix1 == 0 || ix1 > h3->GetNbinsX()) return 0;
  if (iy1 == 0 || iy1 > h3->GetNbinsY()) return 0;
  int ix2 = (xBj >= h3->GetXaxis()->GetBinCenter(ix1)  ?  ix1+1  :  ix1-1);
  int iy2 = (Q2  >= h3->GetYaxis()->GetBinCenter(iy1)  ?  iy1+1  :  iy1-1);
  //int iz2 = (phi >= h3->GetZaxis()->GetBinCenter(iz1)  ?  iz1+1  :  iz1-1);
  if (ix2 == 0 || ix2 > h3->GetNbinsX()) return 0;
  if (iy2 == 0 || iy2 > h3->GetNbinsY()) return 0;
  if (h3->GetBinContent(ix1, iy1, iz1) == 0) return 0;
  if (h3->GetBinContent(ix1, iy2, iz1) == 0) return 0;
  if (h3->GetBinContent(ix2, iy1, iz1) == 0) return 0;
  if (h3->GetBinContent(ix2, iy2, iz1) == 0) return 0;
  
  return h3->Interpolate(xBj, Q2, phi);
}
