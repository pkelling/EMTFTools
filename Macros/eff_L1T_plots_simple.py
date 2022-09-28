#! /usr/bin/env python

###########################################
# Plot Maker for quick plots
#
# by David Curry
#
# 7.22.1014
###########################################

import sys
import os
import re
import ROOT
from matplotlib import interactive
# from ROOT import *
from eff_modules import *

ROOT.gROOT.SetBatch(True)


# Root file of Histograms



# in_file = ROOT.TFile( 'plots/L1T_eff_Run2022_BX0_ISOHLT_eta2p4_combined.root' )
# in_file = ROOT.TFile( 'plots/L1T_eff_Run2022B_BX0_ISOHLT_SingleMuon_eta2p4.root' )
# in_file = ROOT.TFile( 'plots/L1T_eff_Run2022_BX0_ISOHLT_eta2p4_Run2022C.root' )
in_file = ROOT.TFile( 'plots/L1T_eff_Run2022_BX0_ISOHLT_eta2p4_Run2022D.root' )

# in_file_MC = ROOT.TFile( '/afs/cern.ch/user/e/eyigitba/plots/L1TWorkshop_Apr22/prompt_plots/L1T_eff_Pt22_2018D_BX0_uGMT_HLT_UNP_baseline_DY.root' )
in_file_MC = ROOT.TFile( 'plots/L1T_eff_Run2022B_BX0_ISOHLT_DY_m20_eta2p4.root' )



# in_file_text = 'Run3_combined'
# in_file_text = 'Run2022B'
# in_file_text = 'Run2022C'
in_file_text = 'Run2022D'

TF = 'uGMT'

WP = 'SingleMu'

Pt = '22'

Qual = '12'

# legText = ['p_{T}^{%s} > %s GeV, L1_{qual} #geq %s' %(TF, Pt, Qual)]
# legText = ['p_{T}^{%s} > %s GeV, L1_{qual} #geq %s' %(TF, Pt, Qual)]


legText = ['Run3 data']
legText_MC = ['DY MC']

legText_summary ={}
legText_summary['uGMTSingleMu']    = ['0 #leq |#eta^{#mu, offline}| #leq 2.4']
legText_summary['BMTFSingleMu']    = ['0 #leq |#eta^{#mu, offline}| #leq 0.83']
legText_summary['OMTFSingleMu']   = ['0.83 #leq |#eta^{#mu, offline}| #leq 1.24']
legText_summary['EMTFSingleMu']   = ['1.24 #leq |#eta^{#mu, offline}| #leq 2.4']





h_pt_denom = ROOT.TH1F()
h_pt_2_denom = ROOT.TH1F()
h_eta_denom = ROOT.TH1F()
h_phi_denom = ROOT.TH1F()
h_eta_phi_denom = ROOT.TH2D()

h_pt_num = ROOT.TH1F()
h_pt_2_num = ROOT.TH1F()
h_eta_num = ROOT.TH1F()
h_phi_num = ROOT.TH1F()
h_eta_phi_num = ROOT.TH2D()


h_pt_denom_MC = ROOT.TH1F()
h_pt_2_denom_MC = ROOT.TH1F()
h_eta_denom_MC = ROOT.TH1F()
h_phi_denom_MC = ROOT.TH1F()
h_eta_phi_denom_MC = ROOT.TH2D()

h_pt_num_MC = ROOT.TH1F()
h_pt_2_num_MC = ROOT.TH1F()
h_eta_num_MC = ROOT.TH1F()
h_phi_num_MC = ROOT.TH1F()
h_eta_phi_num_MC = ROOT.TH2D()


key = TF + '_' + WP + '_' + Pt

h_pt_denom = in_file.Get('h_pt_denom_%s' % key).Clone("h_pt_denom_data")
h_pt_2_denom = in_file.Get('h_pt_2_denom_%s' % key).Clone("h_pt_2_denom_data")
h_eta_denom = in_file.Get('h_eta_denom_%s' % key).Clone("h_eta_denom_data")
h_phi_denom = in_file.Get('h_phi_denom_%s' % key).Clone("h_phi_denom_data")
h_eta_phi_denom = in_file.Get('h_eta_phi_denom_%s' % key).Clone("h_eta_phi_denom_data")

h_pt_num = in_file.Get('h_pt_num_%s' % (key)).Clone("h_pt_num_data")
h_pt_2_num = in_file.Get('h_pt_2_num_%s' % (key)).Clone("h_pt_2_num_data")
h_eta_num = in_file.Get('h_eta_num_%s' % (key)).Clone("h_eta_num_data")
h_phi_num = in_file.Get('h_phi_num_%s' % (key)).Clone("h_phi_num_data")
h_eta_phi_num = in_file.Get('h_eta_phi_num_%s' % (key)).Clone("h_eta_phi_num_data")


# key_MC = TF + '_' + WP

h_pt_denom_MC = in_file_MC.Get('h_pt_denom_%s' % key).Clone("h_pt_denom_MC")
h_pt_2_denom_MC = in_file_MC.Get('h_pt_2_denom_%s' % key).Clone("h_pt_2_denom_MC")
h_eta_denom_MC = in_file_MC.Get('h_eta_denom_%s' % key).Clone("h_eta_denom_MC")
h_phi_denom_MC = in_file_MC.Get('h_phi_denom_%s' % key).Clone("h_phi_denom_MC")
# h_eta_phi_denom_MC = in_file.Get('h_eta_phi_%s' % TF).Clone()

h_pt_num_MC = in_file_MC.Get('h_pt_num_%s' % (key)).Clone("h_pt_num_MC")
h_pt_2_num_MC = in_file_MC.Get('h_pt_2_num_%s' % (key)).Clone("h_pt_2_num_MC")
h_eta_num_MC = in_file_MC.Get('h_eta_num_%s' % (key)).Clone("h_eta_num_MC")
h_phi_num_MC = in_file_MC.Get('h_phi_num_%s' % (key)).Clone("h_phi_num_MC")
# h_eta_phi_num_MC = in_file.Get('h_eta_phi_num_%s' % (key)).Clone()




eff_pt = ROOT.TGraphAsymmErrors(h_pt_num,h_pt_denom)
eff_pt_2 = ROOT.TGraphAsymmErrors(h_pt_2_num,h_pt_2_denom)
eff_eta = ROOT.TGraphAsymmErrors(h_eta_num,h_eta_denom)
eff_phi = ROOT.TGraphAsymmErrors(h_phi_num,h_phi_denom)

eff_pt_MC = ROOT.TGraphAsymmErrors(h_pt_num_MC,h_pt_denom_MC)
eff_pt_2_MC = ROOT.TGraphAsymmErrors(h_pt_2_num_MC,h_pt_2_denom_MC)
eff_eta_MC = ROOT.TGraphAsymmErrors(h_eta_num_MC,h_eta_denom_MC)
eff_phi_MC = ROOT.TGraphAsymmErrors(h_phi_num_MC,h_phi_denom_MC)

eff_eta_phi = h_eta_phi_num.Clone()
eff_eta_phi.Divide(h_eta_phi_denom)

eff_pt.SetLineColor(ROOT.kBlack)
eff_pt.SetMarkerColor(ROOT.kBlack)
eff_pt.SetLineWidth(1)
eff_pt.SetMarkerStyle(20)
eff_pt.SetMarkerSize(2)
eff_pt.GetXaxis().SetTitle("p_{T}(Probe Reco #mu) [GeV]")
eff_pt.GetYaxis().SetTitle("L1T Efficiency")
eff_pt.GetYaxis().SetTitleOffset(1.35)
eff_pt.GetXaxis().SetNdivisions(509)
eff_pt.GetYaxis().SetNdivisions(514)
eff_pt.SetMinimum(0)
eff_pt.SetMaximum(1.05)
eff_pt.GetXaxis().SetRangeUser(0,1000)

eff_pt_2.SetLineColor(ROOT.kBlack)
eff_pt_2.SetMarkerColor(ROOT.kBlack)
eff_pt_2.SetLineWidth(1)
eff_pt_2.SetMarkerStyle(20)
eff_pt_2.SetMarkerSize(2)
eff_pt_2.GetXaxis().SetTitle("p_{T}(Probe Reco #mu) [GeV]")
eff_pt_2.GetYaxis().SetTitle("L1T Efficiency")
eff_pt_2.GetYaxis().SetTitleOffset(1.35)
eff_pt_2.GetXaxis().SetNdivisions(509)
eff_pt_2.GetYaxis().SetNdivisions(514)
eff_pt_2.SetMinimum(0)
eff_pt_2.SetMaximum(1.05)
eff_pt_2.GetXaxis().SetRangeUser(0,60)

eff_eta.SetLineColor(ROOT.kBlack)
eff_eta.SetMarkerColor(ROOT.kBlack)
eff_eta.SetLineWidth(1)
eff_eta.SetMarkerStyle(20)
eff_eta.SetMarkerSize(2)
eff_eta.GetXaxis().SetTitle("#eta (Probe Reco #mu)")
eff_eta.GetYaxis().SetTitle("L1T Efficiency")
eff_eta.GetYaxis().SetTitleOffset(1.35)
eff_eta.GetXaxis().SetNdivisions(509)
eff_eta.GetYaxis().SetNdivisions(514)
eff_eta.SetMinimum(0)
eff_eta.SetMaximum(1.05)
# eff_eta[in_file_key+TF+WP].GetXaxis().SetRangeUser(0,60)

eff_phi.SetLineColor(ROOT.kBlack)
eff_phi.SetMarkerColor(ROOT.kBlack)
eff_phi.SetLineWidth(1)
eff_phi.SetMarkerStyle(20)
eff_phi.SetMarkerSize(2)
eff_phi.GetXaxis().SetTitle("#phi (Probe Reco #mu)")
eff_phi.GetYaxis().SetTitle("L1T Efficiency")
eff_phi.GetYaxis().SetTitleOffset(1.35)
eff_phi.GetXaxis().SetNdivisions(509)
eff_phi.GetYaxis().SetNdivisions(514)
eff_phi.SetMinimum(0)
eff_phi.SetMaximum(1.05)
# eff_pt[in_file_key+TF+WP].GetXaxis().SetRangeUser(0,60)

eff_eta_phi.GetXaxis().SetRangeUser(-2.4,2.4)
eff_eta_phi.GetYaxis().SetRangeUser(-3.14,3.14)


## MC
eff_pt_MC.SetLineColor(ROOT.kRed)
eff_pt_MC.SetMarkerColor(ROOT.kRed)
eff_pt_MC.SetLineWidth(1)
eff_pt_MC.SetMarkerStyle(23)
eff_pt_MC.SetMarkerSize(2)

eff_pt_2_MC.SetLineColor(ROOT.kRed)
eff_pt_2_MC.SetMarkerColor(ROOT.kRed)
eff_pt_2_MC.SetLineWidth(1)
eff_pt_2_MC.SetMarkerStyle(23)
eff_pt_2_MC.SetMarkerSize(2)

eff_eta_MC.SetLineColor(ROOT.kRed)
eff_eta_MC.SetMarkerColor(ROOT.kRed)
eff_eta_MC.SetLineWidth(1)
eff_eta_MC.SetMarkerStyle(23)
eff_eta_MC.SetMarkerSize(2)


eff_phi_MC.SetLineColor(ROOT.kRed)
eff_phi_MC.SetMarkerColor(ROOT.kRed)
eff_phi_MC.SetLineWidth(1)
eff_phi_MC.SetMarkerStyle(23)
eff_phi_MC.SetMarkerSize(2)



c_pt  = ROOT.TCanvas('c_pt', '', 1280, 1024)
c_eta = ROOT.TCanvas('c_eta', '', 1280, 1024)
c_phi = ROOT.TCanvas('c_phi', '', 1280, 1024)

c_pt.SetGridx()
c_pt.SetGridy()

c_eta.SetGridx()
c_eta.SetGridy()

c_phi.SetGridx()
c_phi.SetGridy()


ROOT.gStyle.SetLegendBorderSize(0)
ROOT.gStyle.SetLegendTextSize(0.02)
ROOT.gStyle.SetOptStat(0)



c_pt.cd()
leg = ROOT.TLegend(0.6,0.15,0.88,0.33)
eff_pt.Draw("AP")
eff_pt_MC.Draw("sames P")

leg.AddEntry(eff_pt,legText[0])
leg.AddEntry(eff_pt_MC,legText_MC[0])

leg.Draw('sames')
cms_label = ROOT.TLatex()
cms_label.SetTextSize(0.04)
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")

header = ROOT.TLatex()
header.SetTextSize(0.03)
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run2022D")

pt_label = ROOT.TLatex()
pt_label.SetTextSize(0.025)
pt_label.DrawLatexNDC(0.64, 0.43, "p_{T}^{#mu, L1} #geq " + Pt +" GeV")

qual_label = ROOT.TLatex()
qual_label.SetTextSize(0.025)
qual_label.DrawLatexNDC(0.64, 0.37, "L1_{qual} #geq " + Qual)

c_pt.SaveAs('plots/pdf/%s_eff_pT_%s.pdf' % (TF, in_file_text))


c_pt.cd()
leg = ROOT.TLegend(0.6,0.15,0.88,0.33)
eff_pt_2.Draw("AP")
eff_pt_2_MC.Draw("sames P")

leg.AddEntry(eff_pt_2,legText[0])
leg.AddEntry(eff_pt_2_MC,legText_MC[0])

leg.Draw('sames')
cms_label = ROOT.TLatex()
cms_label.SetTextSize(0.04)
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")

header = ROOT.TLatex()
header.SetTextSize(0.03)
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run2022D")

pt_label = ROOT.TLatex()
pt_label.SetTextSize(0.025)
pt_label.DrawLatexNDC(0.64, 0.43, "p_{T}^{#mu, L1} #geq " + Pt +" GeV")

qual_label = ROOT.TLatex()
qual_label.SetTextSize(0.025)
qual_label.DrawLatexNDC(0.64, 0.37, "L1_{qual} #geq " + Qual)

c_pt.SaveAs('plots/pdf/%s_eff_pT_2_%s.pdf' % (TF, in_file_text))


c_eta.cd()
leg = ROOT.TLegend(0.6,0.15,0.88,0.33)
eff_eta.Draw("AP")
eff_eta_MC.Draw("sames P")

leg.AddEntry(eff_eta,legText[0])
leg.AddEntry(eff_eta_MC,legText_MC[0])

leg.Draw('sames')
cms_label = ROOT.TLatex()
cms_label.SetTextSize(0.04)
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")

header = ROOT.TLatex()
header.SetTextSize(0.03)
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run2022D")
pt_label = ROOT.TLatex()

pt_label.SetTextSize(0.025)
pt_label.DrawLatexNDC(0.64, 0.43, "p_{T}^{#mu, L1} #geq " + Pt +" GeV")

qual_label = ROOT.TLatex()
qual_label.SetTextSize(0.025)
qual_label.DrawLatexNDC(0.64, 0.37, "L1_{qual} #geq " + Qual)

c_eta.SaveAs('plots/pdf/%s_eff_eta_%s.pdf' % (TF, in_file_text))


c_phi.cd()
leg = ROOT.TLegend(0.6,0.15,0.88,0.33)
eff_phi.Draw("AP")
eff_phi_MC.Draw("sames P")

leg.AddEntry(eff_phi,legText[0])
leg.AddEntry(eff_phi_MC,legText_MC[0])

leg.Draw('sames')
cms_label = ROOT.TLatex()
cms_label.SetTextSize(0.04)
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")

header = ROOT.TLatex()
header.SetTextSize(0.03)
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run2022D")

pt_label = ROOT.TLatex()
pt_label.SetTextSize(0.025)
pt_label.DrawLatexNDC(0.64, 0.43, "p_{T}^{#mu, L1} #geq " + Pt +" GeV")

qual_label = ROOT.TLatex()
qual_label.SetTextSize(0.025)
qual_label.DrawLatexNDC(0.64, 0.37, "L1_{qual} #geq " + Qual)

c_phi.SaveAs('plots/pdf/%s_eff_phi_%s.pdf' % (TF, in_file_text))

# ROOT.gStyle.SetOptStat(0)

# c_eta.cd()
# eff_eta_phi.Draw("colz")
# c_eta.SaveAs('plots/pdf/%s_eff_eta_phi_%s.pdf' % (TF, in_file_text))
