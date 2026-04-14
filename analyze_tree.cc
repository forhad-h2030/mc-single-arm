using namespace std;
void analyze_tree(const char* fn_in="worksim/sigma_ex.root")
{
  TFile* file = new TFile(fn_in);
  TTree* tree = (TTree*)file->Get("h1411");

  // 'psdeltai', 'psyptari', 'psxptari' = dpp_init, dth_init/1000 = dydz_s, dph_init/1000 = dxdz_s
  float dpp_init, dydz_init, dxdz_init;
  tree->SetBranchAddress("psdeltai", &dpp_init);
  tree->SetBranchAddress("psyptari", &dydz_init);
  tree->SetBranchAddress("psxptari", &dxdz_init);
  TH1* h1_dpp_init  = new TH1D("h1_dpp_init" , "", 100, -50, 50);
  TH1* h1_dydz_init = new TH1D("h1_dydz_init", "", 100, -0.1, 0.1);
  TH1* h1_dxdz_init = new TH1D("h1_dxdz_init", "", 100, -0.1, 0.1);

  // 'psdelta', 'psyptar', 'psxptar' = dpp_recon, dth_recon/1000, dph_recon/1000 
  float dpp_recon, dydz_recon, dxdz_recon;
  tree->SetBranchAddress("psdelta", &dpp_recon);
  tree->SetBranchAddress("psyptar", &dydz_recon);
  tree->SetBranchAddress("psxptar", &dxdz_recon);
  TH1* h1_dpp_recon  = new TH1D("h1_dpp_recon" , "", 100, -50, 50);
  TH1* h1_dydz_recon = new TH1D("h1_dydz_recon", "", 100, -0.1, 0.1);
  TH1* h1_dxdz_recon = new TH1D("h1_dxdz_recon", "", 100, -0.1, 0.1);

  
  // 'psztari','psytari' = ztar_init, ytar_init
  // 'psxtari'           = xtar_init
  float xtar_init, ytar_init, ztar_init;
  tree->SetBranchAddress("psxtari", &xtar_init);
  tree->SetBranchAddress("psytari", &ytar_init);
  tree->SetBranchAddress("psztari", &ztar_init);
  TH1* h1_xtar_init = new TH1D("h1_xtar_init", "", 100, -5, 5);
  TH1* h1_ytar_init = new TH1D("h1_ytar_init", "", 100, -5, 5);
  TH1* h1_ztar_init = new TH1D("h1_ztar_init", "", 100, -5, 5);

  // 'psztar','psytar' = ztar_recon, ytar_recon
  float ytar_recon, ztar_recon;
  tree->SetBranchAddress("psytar", &ytar_recon);
  tree->SetBranchAddress("psztar", &ztar_recon);
  TH1* h1_ytar_recon = new TH1D("h1_ytar_recon", "", 100, -5, 5);
  TH1* h1_ztar_recon = new TH1D("h1_ztar_recon", "", 100, -5, 5);
  
  // 'psxfp', 'psyfp'   = x_fp, y_fp 
  // 'psxpfp', 'psypfp' = dx_fp, dy_fp
  float x_fp, y_fp, dx_fp, dy_fp;
  tree->SetBranchAddress( "psxfp", & x_fp);
  tree->SetBranchAddress( "psyfp", & y_fp);
  tree->SetBranchAddress("psxpfp", &dx_fp);
  tree->SetBranchAddress("psypfp", &dy_fp);
  TH1* h1_x_fp  = new TH1D("h1_x_fp" , "", 100, -50, 50);
  TH1* h1_y_fp  = new TH1D("h1_y_fp" , "", 100, -50, 50);
  TH1* h1_dx_fp = new TH1D("h1_dx_fp", "", 100, -0.1, 0.1);
  TH1* h1_dy_fp = new TH1D("h1_dy_fp", "", 100, -0.1, 0.1);
  
  // 'psvxi','psvyi'   = x, y
  float vxi, vyi;
  tree->SetBranchAddress( "psvxi", &vxi);
  tree->SetBranchAddress( "psvyi", &vyi);
  TH1* h1_vxi = new TH1D("h1_vxi" , "", 100, -5, 5);
  TH1* h1_vyi = new TH1D("h1_vyi" , "", 100, -5, 5);

  // 'fry'     = fry
  // 'stop_id' = shmsSTOP_id
  float fry, stop_id;
  tree->SetBranchAddress("fry", &fry);
  tree->SetBranchAddress("stop_id", &stop_id);
  TH1* h1_fry     = new TH1D("h1_fry"    , "", 100, -2.5, 2.5);
  TH1* h1_stop_id = new TH1D("h1_stop_id", "",  30, -0.5, 29.5);
  
  // 'xsnum','ysnum'   = xsfr_num  , ysfr_num    or  xs_num  , ys_num
  // 'xsieve','ysieve' = sc_frsieve, yc_frsieve  or  xc_sieve, yc_sieve

  /// Event Loop ////////////////
  int n_ent = tree->GetEntries();
  cout << "n_ent = " << n_ent << endl;
  for (int i_ent = 0; i_ent < n_ent; i_ent++) {
    tree->GetEntry(i_ent);
    //cout << "tar_init = " << xtar_init << " " << xtar_init << " " << xtar_init << endl;
    h1_dpp_init ->Fill(dpp_init);
    h1_dydz_init->Fill(dydz_init);
    h1_dxdz_init->Fill(dxdz_init);

    h1_dpp_recon ->Fill(dpp_recon);
    h1_dydz_recon->Fill(dydz_recon);
    h1_dxdz_recon->Fill(dxdz_recon);

    h1_xtar_init->Fill(xtar_init);
    h1_ytar_init->Fill(ytar_init);
    h1_ztar_init->Fill(ztar_init);
    h1_ytar_recon->Fill(ytar_recon);
    h1_ztar_recon->Fill(ztar_recon);
    h1_x_fp ->Fill( x_fp);
    h1_y_fp ->Fill( y_fp);
    h1_dx_fp->Fill(dx_fp);
    h1_dy_fp->Fill(dy_fp);

    h1_vxi->Fill(vxi);
    h1_vyi->Fill(vyi);

    h1_fry    ->Fill(fry);
    h1_stop_id->Fill(stop_id);
  }
  /// End of Event Loop ////////////////
  
  gSystem->Exec("mkdir -p result");

  TCanvas* c1 = new TCanvas("c1", "");
  c1->SetGrid();

  h1_dpp_init ->Draw();  c1->SaveAs("result/h1_dpp_init.png");
  h1_dydz_init->Draw();  c1->SaveAs("result/h1_dydz_init.png");
  h1_dxdz_init->Draw();  c1->SaveAs("result/h1_dxdz_init.png");
  
  h1_dpp_recon ->Draw();  c1->SaveAs("result/h1_dpp_recon.png");
  h1_dydz_recon->Draw();  c1->SaveAs("result/h1_dydz_recon.png");
  h1_dxdz_recon->Draw();  c1->SaveAs("result/h1_dxdz_recon.png");
  
  h1_xtar_init->Draw();  c1->SaveAs("result/h1_xtar_init.png");
  h1_ytar_init->Draw();  c1->SaveAs("result/h1_ytar_init.png");
  h1_ztar_init->Draw();  c1->SaveAs("result/h1_ztar_init.png");

  h1_ytar_recon->Draw();  c1->SaveAs("result/h1_ytar_recon.png");
  h1_ztar_recon->Draw();  c1->SaveAs("result/h1_ztar_recon.png");
  
  h1_x_fp ->Draw();  c1->SaveAs("result/h1_x_fp.png");
  h1_y_fp ->Draw();  c1->SaveAs("result/h1_y_fp.png");
  h1_dx_fp->Draw();  c1->SaveAs("result/h1_dx_fp.png");
  h1_dy_fp->Draw();  c1->SaveAs("result/h1_dy_fp.png");

  h1_vxi->Draw();  c1->SaveAs("result/h1_vxi.png");
  h1_vyi->Draw();  c1->SaveAs("result/h1_vyi.png");

  h1_fry    ->Draw();  c1->SaveAs("result/h1_fry.png");
  h1_stop_id->Draw();  c1->SaveAs("result/h1_stop_id.png");
  
  exit(0);
}
