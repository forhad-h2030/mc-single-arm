using namespace std;

// label,x,Q2_GeV2,phi_rad,phi_deg,y,F2d,F1_fixedR,U,K2_nb_GeV2,Delta_spec,T,sigma_Ex_nb_GeV4
void ReadXSecFile(const char* bname)
{
  cout << "bname = " << bname << endl;
  string fname = (string)bname + ".csv";
  ifstream ifs(fname.c_str());
  string line;
  getline(ifs, line); // Discard the header line

  TCanvas* c1 = new TCanvas("c1", "");
  c1->SetGrid();
  TGraph* gr = new TGraph();
  
  string label_pre = "";
  while (getline(ifs, line)) {
    istringstream iss(line);
    string field;
    vector<string> list_field;
    while (getline(iss, field, ',')) list_field.push_back(field);
    string label =      list_field[ 0];
    double xBj   = stof(list_field[ 1]);
    double Q2    = stof(list_field[ 2]);
    double phi   = stof(list_field[ 3]);
    double xsec  = stof(list_field[12]);
    //cout << label << "  " << xBj << "  " << Q2 << "  " << phi << "  " << xsec << endl;
    if (label != label_pre) {
      gr->SetPoint(gr->GetN(), xBj, Q2);
      cout << "  " << label << "  " << xBj << "  " << Q2 << endl;
      label_pre = label;
    }
  }
  ifs.close();

  c1->DrawFrame(0, 0, 1, 20, Form("%s;x_{Bj};Q2", fname.c_str()));
  gr->SetMarkerStyle(21);
  gr->Draw("Psame");
  gr->GetXaxis()->SetRangeUser(0,  1);
  gr->GetYaxis()->SetRangeUser(0, 20);

  c1->SaveAs(Form("gr_%s.png", bname));
  delete gr;
  delete c1;
}

void draw()
{
  ReadXSecFile("sigma_Ex");
  ReadXSecFile("sigma_Ey");
  exit(0);
}
