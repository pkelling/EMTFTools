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



in_file = ROOT.TFile( 'plots/L1T_eff_Run2022_BX0_ISOHLT_eta2p4_combined.root' )

# in_file_MC = ROOT.TFile( '/afs/cern.ch/user/e/eyigitba/plots/L1TWorkshop_Apr22/prompt_plots/L1T_eff_Pt22_2018D_BX0_uGMT_HLT_UNP_baseline_DY.root' )

in_file_text = 'Run3_combined'

WP = 'MuOpen'

Pt = '3'

Qual = '4'

# legText = ['p_{T}^{%s} > %s GeV, L1_{qual} #geq %s' %(TF, Pt, Qual)]
# legText = ['p_{T}^{%s} > %s GeV, L1_{qual} #geq %s' %(TF, Pt, Qual)]


# legText = ['Run2022C']
# legText_MC = ['DY MC']

legText_summary ={}
legText_summary['uGMT']    = ['0 #leq |#eta^{#mu, offline}| #leq 2.4']
legText_summary['BMTF']    = ['0 #leq |#eta^{#mu, offline}| #leq 0.83']
legText_summary['OMTF']   = ['0.83 #leq |#eta^{#mu, offline}| #leq 1.24']
legText_summary['EMTF']   = ['1.24 #leq |#eta^{#mu, offline}| #leq 2.4']





h_pt_denom_uGMT = ROOT.TH1F()
h_pt_2_denom_uGMT = ROOT.TH1F()
h_eta_denom_uGMT = ROOT.TH1F()
h_phi_denom_uGMT = ROOT.TH1F()
h_eta_phi_denom_uGMT = ROOT.TH2D()

h_pt_num_uGMT = ROOT.TH1F()
h_pt_2_num_uGMT = ROOT.TH1F()
h_eta_num_uGMT = ROOT.TH1F()
h_phi_num_uGMT = ROOT.TH1F()
h_eta_phi_num_uGMT = ROOT.TH2D()


h_pt_denom_BMTF = ROOT.TH1F()
h_pt_2_denom_BMTF = ROOT.TH1F()
h_eta_denom_BMTF = ROOT.TH1F()
h_phi_denom_BMTF = ROOT.TH1F()
h_eta_phi_denom_BMTF = ROOT.TH2D()

h_pt_num_BMTF = ROOT.TH1F()
h_pt_2_num_BMTF = ROOT.TH1F()
h_eta_num_BMTF = ROOT.TH1F()
h_phi_num_BMTF = ROOT.TH1F()
h_eta_phi_num_BMTF = ROOT.TH2D()


h_pt_denom_OMTF = ROOT.TH1F()
h_pt_2_denom_OMTF = ROOT.TH1F()
h_eta_denom_OMTF = ROOT.TH1F()
h_phi_denom_OMTF = ROOT.TH1F()
h_eta_phi_denom_OMTF = ROOT.TH2D()

h_pt_num_OMTF = ROOT.TH1F()
h_pt_2_num_OMTF = ROOT.TH1F()
h_eta_num_OMTF = ROOT.TH1F()
h_phi_num_OMTF = ROOT.TH1F()
h_eta_phi_num_OMTF = ROOT.TH2D()


h_pt_denom_EMTF = ROOT.TH1F()
h_pt_2_denom_EMTF = ROOT.TH1F()
h_eta_denom_EMTF = ROOT.TH1F()
h_phi_denom_EMTF = ROOT.TH1F()
h_eta_phi_denom_EMTF = ROOT.TH2D()

h_pt_num_EMTF = ROOT.TH1F()
h_pt_2_num_EMTF = ROOT.TH1F()
h_eta_num_EMTF = ROOT.TH1F()
h_phi_num_EMTF = ROOT.TH1F()
h_eta_phi_num_EMTF = ROOT.TH2D()




key = 'uGMT' + '_' + WP + '_' + Pt

h_pt_denom_uGMT = in_file.Get('h_pt_denom_%s' % key).Clone()
h_pt_2_denom_uGMT = in_file.Get('h_pt_2_denom_%s' % key).Clone()
h_eta_denom_uGMT = in_file.Get('h_eta_denom_%s' % key).Clone()
h_phi_denom_uGMT = in_file.Get('h_phi_denom_%s' % key).Clone()
h_eta_phi_denom_uGMT = in_file.Get('h_eta_phi_denom_%s' % key).Clone()

h_pt_num_uGMT = in_file.Get('h_pt_num_%s' % (key)).Clone()
h_pt_2_num_uGMT = in_file.Get('h_pt_2_num_%s' % (key)).Clone()
h_eta_num_uGMT = in_file.Get('h_eta_num_%s' % (key)).Clone()
h_phi_num_uGMT = in_file.Get('h_phi_num_%s' % (key)).Clone()
h_eta_phi_num_uGMT = in_file.Get('h_eta_phi_num_%s' % (key)).Clone()

key = 'BMTF' + '_' + WP + '_' + Pt

h_pt_denom_BMTF = in_file.Get('h_pt_denom_%s' % key).Clone()
h_pt_2_denom_BMTF = in_file.Get('h_pt_2_denom_%s' % key).Clone()
h_eta_denom_BMTF = in_file.Get('h_eta_denom_%s' % key).Clone()
h_phi_denom_BMTF = in_file.Get('h_phi_denom_%s' % key).Clone()
h_eta_phi_denom_BMTF = in_file.Get('h_eta_phi_denom_%s' % key).Clone()

h_pt_num_BMTF = in_file.Get('h_pt_num_%s' % (key)).Clone()
h_pt_2_num_BMTF = in_file.Get('h_pt_2_num_%s' % (key)).Clone()
h_eta_num_BMTF = in_file.Get('h_eta_num_%s' % (key)).Clone()
h_phi_num_BMTF = in_file.Get('h_phi_num_%s' % (key)).Clone()
h_eta_phi_num_BMTF = in_file.Get('h_eta_phi_num_%s' % (key)).Clone()

key = 'OMTF' + '_' + WP + '_' + Pt

h_pt_denom_OMTF = in_file.Get('h_pt_denom_%s' % key).Clone()
h_pt_2_denom_OMTF = in_file.Get('h_pt_2_denom_%s' % key).Clone()
h_eta_denom_OMTF = in_file.Get('h_eta_denom_%s' % key).Clone()
h_phi_denom_OMTF = in_file.Get('h_phi_denom_%s' % key).Clone()
h_eta_phi_denom_OMTF = in_file.Get('h_eta_phi_denom_%s' % key).Clone()

h_pt_num_OMTF = in_file.Get('h_pt_num_%s' % (key)).Clone()
h_pt_2_num_OMTF = in_file.Get('h_pt_2_num_%s' % (key)).Clone()
h_eta_num_OMTF = in_file.Get('h_eta_num_%s' % (key)).Clone()
h_phi_num_OMTF = in_file.Get('h_phi_num_%s' % (key)).Clone()
h_eta_phi_num_OMTF = in_file.Get('h_eta_phi_num_%s' % (key)).Clone()

key = 'EMTF' + '_' + WP + '_' + Pt

h_pt_denom_EMTF = in_file.Get('h_pt_denom_%s' % key).Clone()
h_pt_2_denom_EMTF = in_file.Get('h_pt_2_denom_%s' % key).Clone()
h_eta_denom_EMTF = in_file.Get('h_eta_denom_%s' % key).Clone()
h_phi_denom_EMTF = in_file.Get('h_phi_denom_%s' % key).Clone()
h_eta_phi_denom_EMTF = in_file.Get('h_eta_phi_denom_%s' % key).Clone()

h_pt_num_EMTF = in_file.Get('h_pt_num_%s' % (key)).Clone()
h_pt_2_num_EMTF = in_file.Get('h_pt_2_num_%s' % (key)).Clone()
h_eta_num_EMTF = in_file.Get('h_eta_num_%s' % (key)).Clone()
h_phi_num_EMTF = in_file.Get('h_phi_num_%s' % (key)).Clone()
h_eta_phi_num_EMTF = in_file.Get('h_eta_phi_num_%s' % (key)).Clone()




eff_pt_uGMT = ROOT.TGraphAsymmErrors(h_pt_num_uGMT,h_pt_denom_uGMT)
eff_pt_2_uGMT = ROOT.TGraphAsymmErrors(h_pt_2_num_uGMT,h_pt_2_denom_uGMT)
eff_eta_uGMT = ROOT.TGraphAsymmErrors(h_eta_num_uGMT,h_eta_denom_uGMT)
eff_phi_uGMT = ROOT.TGraphAsymmErrors(h_phi_num_uGMT,h_phi_denom_uGMT)

eff_pt_BMTF = ROOT.TGraphAsymmErrors(h_pt_num_BMTF,h_pt_denom_BMTF)
eff_pt_2_BMTF = ROOT.TGraphAsymmErrors(h_pt_2_num_BMTF,h_pt_2_denom_BMTF)
eff_eta_BMTF = ROOT.TGraphAsymmErrors(h_eta_num_BMTF,h_eta_denom_BMTF)
eff_phi_BMTF = ROOT.TGraphAsymmErrors(h_phi_num_BMTF,h_phi_denom_BMTF)

eff_pt_OMTF = ROOT.TGraphAsymmErrors(h_pt_num_OMTF,h_pt_denom_OMTF)
eff_pt_2_OMTF = ROOT.TGraphAsymmErrors(h_pt_2_num_OMTF,h_pt_2_denom_OMTF)
eff_eta_OMTF = ROOT.TGraphAsymmErrors(h_eta_num_OMTF,h_eta_denom_OMTF)
eff_phi_OMTF = ROOT.TGraphAsymmErrors(h_phi_num_OMTF,h_phi_denom_OMTF)

eff_pt_EMTF = ROOT.TGraphAsymmErrors(h_pt_num_EMTF,h_pt_denom_EMTF)
eff_pt_2_EMTF = ROOT.TGraphAsymmErrors(h_pt_2_num_EMTF,h_pt_2_denom_EMTF)
eff_eta_EMTF = ROOT.TGraphAsymmErrors(h_eta_num_EMTF,h_eta_denom_EMTF)
eff_phi_EMTF = ROOT.TGraphAsymmErrors(h_phi_num_EMTF,h_phi_denom_EMTF)


eff_pt_uGMT.SetLineColor(ROOT.kBlack)
eff_pt_uGMT.SetMarkerColor(ROOT.kBlack)
eff_pt_uGMT.SetLineWidth(1)
eff_pt_uGMT.SetMarkerStyle(20)
eff_pt_uGMT.SetMarkerSize(2)
eff_pt_uGMT.GetXaxis().SetTitle("p_{T}(Probe Reco #mu) [GeV]")
eff_pt_uGMT.GetYaxis().SetTitle("L1T Efficiency")
eff_pt_uGMT.GetYaxis().SetTitleOffset(1.35)
eff_pt_uGMT.GetXaxis().SetNdivisions(509)
eff_pt_uGMT.GetYaxis().SetNdivisions(514)
eff_pt_uGMT.SetMinimum(0)
eff_pt_uGMT.SetMaximum(1.05)
eff_pt_uGMT.GetXaxis().SetRangeUser(0,1000)

eff_pt_2_uGMT.SetLineColor(ROOT.kBlack)
eff_pt_2_uGMT.SetMarkerColor(ROOT.kBlack)
eff_pt_2_uGMT.SetLineWidth(1)
eff_pt_2_uGMT.SetMarkerStyle(20)
eff_pt_2_uGMT.SetMarkerSize(2)
eff_pt_2_uGMT.GetXaxis().SetTitle("p_{T}(Probe Reco #mu) [GeV]")
eff_pt_2_uGMT.GetYaxis().SetTitle("L1T Efficiency")
eff_pt_2_uGMT.GetYaxis().SetTitleOffset(1.35)
eff_pt_2_uGMT.GetXaxis().SetNdivisions(509)
eff_pt_2_uGMT.GetYaxis().SetNdivisions(514)
eff_pt_2_uGMT.SetMinimum(0)
eff_pt_2_uGMT.SetMaximum(1.05)
eff_pt_2_uGMT.GetXaxis().SetRangeUser(0,60)

eff_eta_uGMT.SetLineColor(ROOT.kBlack)
eff_eta_uGMT.SetMarkerColor(ROOT.kBlack)
eff_eta_uGMT.SetLineWidth(1)
eff_eta_uGMT.SetMarkerStyle(20)
eff_eta_uGMT.SetMarkerSize(2)
eff_eta_uGMT.GetXaxis().SetTitle("#eta (Probe Reco #mu)")
eff_eta_uGMT.GetYaxis().SetTitle("L1T Efficiency")
eff_eta_uGMT.GetYaxis().SetTitleOffset(1.35)
eff_eta_uGMT.GetXaxis().SetNdivisions(509)
eff_eta_uGMT.GetYaxis().SetNdivisions(514)
eff_eta_uGMT.SetMinimum(0)
eff_eta_uGMT.SetMaximum(1.05)
# eff_eta[in_file_key+TF+WP].GetXaxis().SetRangeUser(0,60)

eff_phi_uGMT.SetLineColor(ROOT.kBlack)
eff_phi_uGMT.SetMarkerColor(ROOT.kBlack)
eff_phi_uGMT.SetLineWidth(1)
eff_phi_uGMT.SetMarkerStyle(20)
eff_phi_uGMT.SetMarkerSize(2)
eff_phi_uGMT.GetXaxis().SetTitle("#phi (Probe Reco #mu)")
eff_phi_uGMT.GetYaxis().SetTitle("L1T Efficiency")
eff_phi_uGMT.GetYaxis().SetTitleOffset(1.35)
eff_phi_uGMT.GetXaxis().SetNdivisions(509)
eff_phi_uGMT.GetYaxis().SetNdivisions(514)
eff_phi_uGMT.SetMinimum(0)
eff_phi_uGMT.SetMaximum(1.05)
# eff_pt[in_file_key+TF+WP].GetXaxis().SetRangeUser(0,60)


## BMTF
eff_pt_BMTF.SetLineColor(ROOT.kRed)
eff_pt_BMTF.SetMarkerColor(ROOT.kRed)
eff_pt_BMTF.SetLineWidth(1)
eff_pt_BMTF.SetMarkerStyle(23)
eff_pt_BMTF.SetMarkerSize(2)

eff_pt_2_BMTF.SetLineColor(ROOT.kRed)
eff_pt_2_BMTF.SetMarkerColor(ROOT.kRed)
eff_pt_2_BMTF.SetLineWidth(1)
eff_pt_2_BMTF.SetMarkerStyle(23)
eff_pt_2_BMTF.SetMarkerSize(2)

eff_eta_BMTF.SetLineColor(ROOT.kRed)
eff_eta_BMTF.SetMarkerColor(ROOT.kRed)
eff_eta_BMTF.SetLineWidth(1)
eff_eta_BMTF.SetMarkerStyle(23)
eff_eta_BMTF.SetMarkerSize(2)

eff_phi_BMTF.SetLineColor(ROOT.kRed)
eff_phi_BMTF.SetMarkerColor(ROOT.kRed)
eff_phi_BMTF.SetLineWidth(1)
eff_phi_BMTF.SetMarkerStyle(23)
eff_phi_BMTF.SetMarkerSize(2)

## OMTF
eff_pt_OMTF.SetLineColor(ROOT.kViolet)
eff_pt_OMTF.SetMarkerColor(ROOT.kViolet)
eff_pt_OMTF.SetLineWidth(1)
eff_pt_OMTF.SetMarkerStyle(22)
eff_pt_OMTF.SetMarkerSize(2)

eff_pt_2_OMTF.SetLineColor(ROOT.kViolet)
eff_pt_2_OMTF.SetMarkerColor(ROOT.kViolet)
eff_pt_2_OMTF.SetLineWidth(1)
eff_pt_2_OMTF.SetMarkerStyle(22)
eff_pt_2_OMTF.SetMarkerSize(2)

eff_eta_OMTF.SetLineColor(ROOT.kViolet)
eff_eta_OMTF.SetMarkerColor(ROOT.kViolet)
eff_eta_OMTF.SetLineWidth(1)
eff_eta_OMTF.SetMarkerStyle(22)
eff_eta_OMTF.SetMarkerSize(2)


eff_phi_OMTF.SetLineColor(ROOT.kViolet)
eff_phi_OMTF.SetMarkerColor(ROOT.kViolet)
eff_phi_OMTF.SetLineWidth(1)
eff_phi_OMTF.SetMarkerStyle(22)
eff_phi_OMTF.SetMarkerSize(2)

## EMTF
eff_pt_EMTF.SetLineColor(ROOT.kBlue)
eff_pt_EMTF.SetMarkerColor(ROOT.kBlue)
eff_pt_EMTF.SetLineWidth(1)
eff_pt_EMTF.SetMarkerStyle(21)
eff_pt_EMTF.SetMarkerSize(2)

eff_pt_2_EMTF.SetLineColor(ROOT.kBlue)
eff_pt_2_EMTF.SetMarkerColor(ROOT.kBlue)
eff_pt_2_EMTF.SetLineWidth(1)
eff_pt_2_EMTF.SetMarkerStyle(21)
eff_pt_2_EMTF.SetMarkerSize(2)

eff_eta_EMTF.SetLineColor(ROOT.kBlue)
eff_eta_EMTF.SetMarkerColor(ROOT.kBlue)
eff_eta_EMTF.SetLineWidth(1)
eff_eta_EMTF.SetMarkerStyle(21)
eff_eta_EMTF.SetMarkerSize(2)


eff_phi_EMTF.SetLineColor(ROOT.kBlue)
eff_phi_EMTF.SetMarkerColor(ROOT.kBlue)
eff_phi_EMTF.SetLineWidth(1)
eff_phi_EMTF.SetMarkerStyle(21)
eff_phi_EMTF.SetMarkerSize(2)



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



c_pt.cd()
leg = ROOT.TLegend(0.6,0.15,0.88,0.48)
eff_pt_uGMT.Draw("AP")
eff_pt_BMTF.Draw("sames P")
eff_pt_OMTF.Draw("sames P")
eff_pt_EMTF.Draw("sames P")

leg.AddEntry(eff_pt_uGMT,legText_summary['uGMT'][0])
leg.AddEntry(eff_pt_BMTF,legText_summary['BMTF'][0])
leg.AddEntry(eff_pt_OMTF,legText_summary['OMTF'][0])
leg.AddEntry(eff_pt_EMTF,legText_summary['EMTF'][0])

leg.Draw('sames')
cms_label = ROOT.TLatex()
cms_label.SetTextSize(0.04)
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")

header = ROOT.TLatex()
header.SetTextSize(0.03)
header.DrawLatexNDC(0.67, 0.92, "#sqrt{s} = 13.6 TeV, Run2022BCD")

pt_label = ROOT.TLatex()
pt_label.SetTextSize(0.025)
pt_label.DrawLatexNDC(0.64, 0.58, "p_{T}^{#mu, L1} #geq " + Pt +" GeV")

qual_label = ROOT.TLatex()
qual_label.SetTextSize(0.025)
qual_label.DrawLatexNDC(0.64, 0.52, "L1_{qual} #geq " + Qual)

c_pt.SaveAs('plots/pdf/TFComp/%s_eff_pT_%s.pdf' % (WP+Pt, in_file_text))


c_pt.cd()
leg = ROOT.TLegend(0.6,0.15,0.88,0.48)
eff_pt_2_uGMT.Draw("AP")
eff_pt_2_BMTF.Draw("sames P")
eff_pt_2_OMTF.Draw("sames P")
eff_pt_2_EMTF.Draw("sames P")

leg.AddEntry(eff_pt_2_uGMT,legText_summary['uGMT'][0])
leg.AddEntry(eff_pt_2_BMTF,legText_summary['BMTF'][0])
leg.AddEntry(eff_pt_2_OMTF,legText_summary['OMTF'][0])
leg.AddEntry(eff_pt_2_EMTF,legText_summary['EMTF'][0])

leg.Draw('sames')
cms_label = ROOT.TLatex()
cms_label.SetTextSize(0.04)
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")

header = ROOT.TLatex()
header.SetTextSize(0.03)
header.DrawLatexNDC(0.67, 0.92, "#sqrt{s} = 13.6 TeV, Run2022BCD")

pt_label = ROOT.TLatex()
pt_label.SetTextSize(0.025)
pt_label.DrawLatexNDC(0.64, 0.58, "p_{T}^{#mu, L1} #geq " + Pt +" GeV")

qual_label = ROOT.TLatex()
qual_label.SetTextSize(0.025)
qual_label.DrawLatexNDC(0.64, 0.52, "L1_{qual} #geq " + Qual)

c_pt.SaveAs('plots/pdf/TFComp/%s_eff_pT_2_%s.pdf' % (WP+Pt, in_file_text))


c_eta.cd()
leg = ROOT.TLegend(0.6,0.15,0.88,0.48)
eff_eta_uGMT.Draw("AP")
eff_eta_BMTF.Draw("sames P")
eff_eta_OMTF.Draw("sames P")
eff_eta_EMTF.Draw("sames P")

leg.AddEntry(eff_eta_uGMT,legText_summary['uGMT'][0])
leg.AddEntry(eff_eta_BMTF,legText_summary['BMTF'][0])
leg.AddEntry(eff_eta_OMTF,legText_summary['OMTF'][0])
leg.AddEntry(eff_eta_EMTF,legText_summary['EMTF'][0])

leg.Draw('sames')
cms_label = ROOT.TLatex()
cms_label.SetTextSize(0.04)
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")

header = ROOT.TLatex()
header.SetTextSize(0.03)
header.DrawLatexNDC(0.67, 0.92, "#sqrt{s} = 13.6 TeV, Run2022BCD")

pt_label = ROOT.TLatex()
pt_label.SetTextSize(0.025)
pt_label.DrawLatexNDC(0.64, 0.58, "p_{T}^{#mu, L1} #geq " + Pt +" GeV")

qual_label = ROOT.TLatex()
qual_label.SetTextSize(0.025)
qual_label.DrawLatexNDC(0.64, 0.52, "L1_{qual} #geq " + Qual)

c_eta.SaveAs('plots/pdf/TFComp/%s_eff_eta_%s.pdf' % (WP+Pt, in_file_text))


c_phi.cd()
leg = ROOT.TLegend(0.6,0.15,0.88,0.48)
eff_phi_uGMT.Draw("AP")
eff_phi_BMTF.Draw("sames P")
eff_phi_OMTF.Draw("sames P")
eff_phi_EMTF.Draw("sames P")

leg.AddEntry(eff_phi_uGMT,legText_summary['uGMT'][0])
leg.AddEntry(eff_phi_BMTF,legText_summary['BMTF'][0])
leg.AddEntry(eff_phi_OMTF,legText_summary['OMTF'][0])
leg.AddEntry(eff_phi_EMTF,legText_summary['EMTF'][0])

leg.Draw('sames')
cms_label = ROOT.TLatex()
cms_label.SetTextSize(0.04)
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")

header = ROOT.TLatex()
header.SetTextSize(0.03)
header.DrawLatexNDC(0.67, 0.92, "#sqrt{s} = 13.6 TeV, Run2022BCD")

pt_label = ROOT.TLatex()
pt_label.SetTextSize(0.025)
pt_label.DrawLatexNDC(0.64, 0.58, "p_{T}^{#mu, L1} #geq " + Pt +" GeV")

qual_label = ROOT.TLatex()
qual_label.SetTextSize(0.025)
qual_label.DrawLatexNDC(0.64, 0.52, "L1_{qual} #geq " + Qual)

c_phi.SaveAs('plots/pdf/TFComp/%s_eff_phi_%s.pdf' % (WP+Pt, in_file_text))
