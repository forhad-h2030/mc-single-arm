"""
g2p2_kinematics.py
2D plot of Q2 (log) vs W for accepted events.
Run: python g2p2_kinematics.py
"""

import ROOT
ROOT.gROOT.SetBatch(True)

ROOTFILE  = "g2p_1.root"
P0        = 3.607    # GeV
TH0       = 6.5      # deg
E0        = 4.4      # GeV
MP        = 0.938272 # proton mass (GeV)

th0 = TH0 * ROOT.TMath.DegToRad()

f    = ROOT.TFile.Open(ROOTFILE)
tree = f.Get("h1412")
n_total = int(tree.GetEntries())

import math
n_q2bins = 200
q2_lo, q2_hi = -1.0, 1.0   # log10 range
q2_bins = [10**(q2_lo + i*(q2_hi - q2_lo)/n_q2bins) for i in range(n_q2bins + 1)]
q2_arr  = ROOT.std.vector('double')(q2_bins)

hQ2W = ROOT.TH2F("hQ2W", ";W (GeV);Q^{2} (GeV^{2})",
                  200, 0.0, 3.0,
                  n_q2bins, q2_arr.data())

n_accepted = 0
for evt in tree:
    if evt.sflag != 0:
        continue

    pp   = P0 * (1.0 + evt.sdelta / 100.0)
    scat = ROOT.TMath.ACos(ROOT.TMath.Cos(th0 + evt.syptar) * ROOT.TMath.Cos(evt.sxptar))
    sin2 = ROOT.TMath.Sin(scat / 2.0) ** 2

    nu  = E0 - pp
    Q2  = 4.0 * E0 * pp * sin2
    W   = ROOT.TMath.Sqrt(max(0.0, MP**2 + 2.0*MP*nu - Q2))

    hQ2W.Fill(W, Q2)
    n_accepted += 1

acceptance = 100.0 * n_accepted / n_total
print(f"Total entries : {n_total}")
print(f"Accepted      : {n_accepted}")
print(f"Acceptance    : {acceptance:.1f}%")

# ── Canvas ────────────────────────────────────────────────
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetPalette(ROOT.kBird)

c = ROOT.TCanvas("c", "Q2 vs W", 900, 700)
c.SetLeftMargin(0.12)
c.SetRightMargin(0.15)
c.SetBottomMargin(0.12)
c.SetLogy()

hQ2W.GetXaxis().SetTitleSize(0.05)
hQ2W.GetYaxis().SetTitleSize(0.05)
hQ2W.GetXaxis().SetLabelSize(0.04)
hQ2W.GetYaxis().SetLabelSize(0.04)
hQ2W.Draw("colz")

label = ROOT.TPaveText(0.14, 0.80, 0.50, 0.92, "NDC")
label.SetFillColor(0)
label.SetBorderSize(1)
label.SetTextSize(0.035)
label.AddText(f"Accepted: {n_accepted}")
label.AddText(f"Acceptance: {acceptance:.1f}%")
label.Draw()

c.Update()
c.SaveAs("g2p2_kinematics.pdf")

out = ROOT.TFile("g2p2_kinematics.root", "RECREATE")
hQ2W.Write()
out.Close()
f.Close()
print("Done. Saved: g2p2_kinematics.pdf  g2p2_kinematics.root")
