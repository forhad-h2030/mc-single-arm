#include "XSec.h"
using namespace std;

TH3* h3_xsec;

// phi in degree
void PrintXSec(const double xBj, const double Q2, const double phi)
{
  double xsec = GetXSec(h3_xsec, xBj, Q2, phi);
  cout << xBj << "\t" << Q2 << "\t" << phi << "\t" << xsec << "\n";
}

void test_xsec(const char* Exy="Ex")
{
  h3_xsec = ReadXSecFile(Exy);
  
  PrintXSec(0.2, 1.0,  0);
  PrintXSec(0.2, 1.0, 90);
  
  exit(0);
}

