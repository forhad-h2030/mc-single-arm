#include "XSec.h"
using namespace std;

// phi in degree
void PrintXSec(TH3* h3, const double xBj, const double Q2, const double phi)
{
  double xsec = GetXSec(h3, xBj, Q2, phi);
  cout << xBj << "\t" << Q2 << "\t" << phi << "\t" << xsec << "\n";
}

// phi in rad
void PrintXSecRad(TH3* h3, const double xBj, const double Q2, const double phi)
{
  PrintXSec(h3, xBj, Q2, phi*180/TMath::Pi());
}

/// Main.  Modify as you need.
void test_xsec()
{
  TH3* h3_ex = ReadXSecFile("Ex");
  TH3* h3_ey = ReadXSecFile("Ey");
  
  //PrintXSec(h3_ex, 0.2, 1.0,  0);
  //PrintXSec(h3_ex, 0.2, 1.0, 90);

  PrintXSecRad(h3_ex, 0.07, 1.1, 1.6);
  PrintXSecRad(h3_ey, 0.07, 1.1, 1.2);
  //PrintXSec(h3_ey, 0.07, 1.1, 30);
  //PrintXSec(h3_ey, 0.07, 1.1, 60);
  //PrintXSec(h3_ey, 0.07, 1.1, 90);

  PrintXSecRad(h3_ex, 0.19, 1.5, 1.6);
  PrintXSecRad(h3_ey, 0.19, 1.5, 1.5);
  
  exit(0);
}

