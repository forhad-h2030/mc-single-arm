
double GetLumi()
{
  const double I_BEAM = 85e-9; // A
  const double ELE = 1.602e-19; // C
  const double FLUX = I_BEAM / ELE;
  const double RHO_W = 1.007; // g/cm3, weight density, ND3
  const double N_A = 6.022e23; // 1/mol
  const double RHO_N = RHO_W * N_A; // 1/cm3, nucleon density (1 g = 1 mol)
  const double L_TGT = 3.0; // cm
  return FLUX * RHO_N * L_TGT * 1e-33; // 1/(nb*s)
}
