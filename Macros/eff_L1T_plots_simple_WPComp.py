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
in_file = ROOT.TFile( 'plots/L1T_eff_Run2022B_BX0_ISOHLT_SingleMuon_eta2p4.root' )
# in_file = ROOT.TFile( 'plots/L1T_eff_Run2022_BX0_ISOHLT_eta2p4_Run2022C.root' )
# in_file = ROOT.TFile( 'plots/L1T_eff_Run2022_BX0_ISOHLT_eta2p4_Run2022D.root' )

# in_file_MC = ROOT.TFile( '/afs/cern.ch/user/e/eyigitba/plots/L1TWorkshop_Apr22/prompt_plots/L1T_eff_Pt22_2018D_BX0_uGMT_HLT_UNP_baseline_DY.root' )

# in_file_text = 'Run3_combined'
in_file_text = 'Run2022B'
# in_file_text = 'Run2022C'
# in_file_text = 'Run2022D'

TF = 'uGMT'

# legText = ['p_{T}^{%s} > %s GeV, L1_{qual} #geq %s' %(TF, Pt, Qual)]
# legText = ['p_{T}^{%s} > %s GeV, L1_{qual} #geq %s' %(TF, Pt, Qual)]


# legText = ['Run2022C']
# legText_MC = ['DY MC']

legText_summary ={}
legText_summary['SingleMu22']    = ['p_{T}^{L1} > 22 GeV, L1_{qual} #geq 12']
legText_summary['SingleMu15']    = ['p_{T}^{L1} > 15 GeV, L1_{qual} #geq 12']
legText_summary['SingleMu7']   = ['p_{T}^{L1} > 7 GeV, L1_{qual} #geq 12']
legText_summary['SingleMu3']   = ['p_{T}^{L1} > 3 GeV, L1_{qual} #geq 12']
legText_summary['DoubleMu15']    = ['p_{T}^{L1} > 15 GeV, L1_{qual} #geq 8']
legText_summary['DoubleMu7']   = ['p_{T}^{L1} > 7 GeV, L1_{qual} #geq 8']
legText_summary['MuOpen3']   = ['p_{T}^{L1} > 3 GeV, L1_{qual} #geq 3']





h_pt_denom_singleMu22 = ROOT.TH1F()
h_pt_2_denom_singleMu22 = ROOT.TH1F()
h_eta_denom_singleMu22 = ROOT.TH1F()
h_phi_denom_singleMu22 = ROOT.TH1F()
h_eta_phi_denom_singleMu22 = ROOT.TH2D()

h_pt_num_singleMu22 = ROOT.TH1F()
h_pt_2_num_singleMu22 = ROOT.TH1F()
h_eta_num_singleMu22 = ROOT.TH1F()
h_phi_num_singleMu22 = ROOT.TH1F()
h_eta_phi_num_singleMu22 = ROOT.TH2D()

h_pt_denom_singleMu15 = ROOT.TH1F()
h_pt_2_denom_singleMu15 = ROOT.TH1F()
h_eta_denom_singleMu15 = ROOT.TH1F()
h_phi_denom_singleMu15 = ROOT.TH1F()
h_eta_phi_denom_singleMu15 = ROOT.TH2D()

h_pt_num_singleMu15 = ROOT.TH1F()
h_pt_2_num_singleMu15 = ROOT.TH1F()
h_eta_num_singleMu15 = ROOT.TH1F()
h_phi_num_singleMu15 = ROOT.TH1F()
h_eta_phi_num_singleMu15 = ROOT.TH2D()

h_pt_denom_singleMu7 = ROOT.TH1F()
h_pt_2_denom_singleMu7 = ROOT.TH1F()
h_eta_denom_singleMu7 = ROOT.TH1F()
h_phi_denom_singleMu7 = ROOT.TH1F()
h_eta_phi_denom_singleMu7 = ROOT.TH2D()

h_pt_num_singleMu7 = ROOT.TH1F()
h_pt_2_num_singleMu7 = ROOT.TH1F()
h_eta_num_singleMu7 = ROOT.TH1F()
h_phi_num_singleMu7 = ROOT.TH1F()
h_eta_phi_num_singleMu7 = ROOT.TH2D()

h_pt_denom_singleMu3 = ROOT.TH1F()
h_pt_2_denom_singleMu3 = ROOT.TH1F()
h_eta_denom_singleMu3 = ROOT.TH1F()
h_phi_denom_singleMu3 = ROOT.TH1F()
h_eta_phi_denom_singleMu3 = ROOT.TH2D()

h_pt_num_singleMu3 = ROOT.TH1F()
h_pt_2_num_singleMu3 = ROOT.TH1F()
h_eta_num_singleMu3 = ROOT.TH1F()
h_phi_num_singleMu3 = ROOT.TH1F()
h_eta_phi_num_singleMu3 = ROOT.TH2D()

h_pt_denom_doubleMu15 = ROOT.TH1F()
h_pt_2_denom_doubleMu15 = ROOT.TH1F()
h_eta_denom_doubleMu15 = ROOT.TH1F()
h_phi_denom_doubleMu15 = ROOT.TH1F()
h_eta_phi_denom_doubleMu15 = ROOT.TH2D()

h_pt_num_doubleMu15 = ROOT.TH1F()
h_pt_2_num_doubleMu15 = ROOT.TH1F()
h_eta_num_doubleMu15 = ROOT.TH1F()
h_phi_num_doubleMu15 = ROOT.TH1F()
h_eta_phi_num_doubleMu15 = ROOT.TH2D()

h_pt_denom_doubleMu7 = ROOT.TH1F()
h_pt_2_denom_doubleMu7 = ROOT.TH1F()
h_eta_denom_doubleMu7 = ROOT.TH1F()
h_phi_denom_doubleMu7 = ROOT.TH1F()
h_eta_phi_denom_doubleMu7 = ROOT.TH2D()

h_pt_num_doubleMu7 = ROOT.TH1F()
h_pt_2_num_doubleMu7 = ROOT.TH1F()
h_eta_num_doubleMu7 = ROOT.TH1F()
h_phi_num_doubleMu7 = ROOT.TH1F()
h_eta_phi_num_doubleMu7 = ROOT.TH2D()

h_pt_denom_muOpen3 = ROOT.TH1F()
h_pt_2_denom_muOpen3 = ROOT.TH1F()
h_eta_denom_muOpen3 = ROOT.TH1F()
h_phi_denom_muOpen3 = ROOT.TH1F()
h_eta_phi_denom_muOpen3 = ROOT.TH2D()

h_pt_num_muOpen3 = ROOT.TH1F()
h_pt_2_num_muOpen3 = ROOT.TH1F()
h_eta_num_muOpen3 = ROOT.TH1F()
h_phi_num_muOpen3 = ROOT.TH1F()
h_eta_phi_num_muOpen3 = ROOT.TH2D()



key = TF + '_' + 'SingleMu_22'

h_pt_denom_singleMu22 = in_file.Get('h_pt_denom_%s' % key).Clone()
h_pt_2_denom_singleMu22 = in_file.Get('h_pt_2_denom_%s' % key).Clone()
h_eta_denom_singleMu22 = in_file.Get('h_eta_denom_%s' % key).Clone()
h_phi_denom_singleMu22 = in_file.Get('h_phi_denom_%s' % key).Clone()
h_eta_phi_denom_singleMu22 = in_file.Get('h_eta_phi_denom_%s' % key).Clone()

h_pt_num_singleMu22 = in_file.Get('h_pt_num_%s' % (key)).Clone()
h_pt_2_num_singleMu22 = in_file.Get('h_pt_2_num_%s' % (key)).Clone()
h_eta_num_singleMu22 = in_file.Get('h_eta_num_%s' % (key)).Clone()
h_phi_num_singleMu22 = in_file.Get('h_phi_num_%s' % (key)).Clone()
h_eta_phi_num_singleMu22 = in_file.Get('h_eta_phi_num_%s' % (key)).Clone()

key = TF + '_' + 'SingleMu_15'

h_pt_denom_singleMu15 = in_file.Get('h_pt_denom_%s' % key).Clone()
h_pt_2_denom_singleMu15 = in_file.Get('h_pt_2_denom_%s' % key).Clone()
h_eta_denom_singleMu15 = in_file.Get('h_eta_denom_%s' % key).Clone()
h_phi_denom_singleMu15 = in_file.Get('h_phi_denom_%s' % key).Clone()
h_eta_phi_denom_singleMu15 = in_file.Get('h_eta_phi_denom_%s' % key).Clone()

h_pt_num_singleMu15 = in_file.Get('h_pt_num_%s' % (key)).Clone()
h_pt_2_num_singleMu15 = in_file.Get('h_pt_2_num_%s' % (key)).Clone()
h_eta_num_singleMu15 = in_file.Get('h_eta_num_%s' % (key)).Clone()
h_phi_num_singleMu15 = in_file.Get('h_phi_num_%s' % (key)).Clone()
h_eta_phi_num_singleMu15 = in_file.Get('h_eta_phi_num_%s' % (key)).Clone()

key = TF + '_' + 'SingleMu_7'

h_pt_denom_singleMu7 = in_file.Get('h_pt_denom_%s' % key).Clone()
h_pt_2_denom_singleMu7 = in_file.Get('h_pt_2_denom_%s' % key).Clone()
h_eta_denom_singleMu7 = in_file.Get('h_eta_denom_%s' % key).Clone()
h_phi_denom_singleMu7 = in_file.Get('h_phi_denom_%s' % key).Clone()
h_eta_phi_denom_singleMu7 = in_file.Get('h_eta_phi_denom_%s' % key).Clone()

h_pt_num_singleMu7 = in_file.Get('h_pt_num_%s' % (key)).Clone()
h_pt_2_num_singleMu7 = in_file.Get('h_pt_2_num_%s' % (key)).Clone()
h_eta_num_singleMu7 = in_file.Get('h_eta_num_%s' % (key)).Clone()
h_phi_num_singleMu7 = in_file.Get('h_phi_num_%s' % (key)).Clone()
h_eta_phi_num_singleMu7 = in_file.Get('h_eta_phi_num_%s' % (key)).Clone()

key = TF + '_' + 'SingleMu_3'

h_pt_denom_singleMu3 = in_file.Get('h_pt_denom_%s' % key).Clone()
h_pt_2_denom_singleMu3 = in_file.Get('h_pt_2_denom_%s' % key).Clone()
h_eta_denom_singleMu3 = in_file.Get('h_eta_denom_%s' % key).Clone()
h_phi_denom_singleMu3 = in_file.Get('h_phi_denom_%s' % key).Clone()
h_eta_phi_denom_singleMu3 = in_file.Get('h_eta_phi_denom_%s' % key).Clone()

h_pt_num_singleMu3 = in_file.Get('h_pt_num_%s' % (key)).Clone()
h_pt_2_num_singleMu3 = in_file.Get('h_pt_2_num_%s' % (key)).Clone()
h_eta_num_singleMu3 = in_file.Get('h_eta_num_%s' % (key)).Clone()
h_phi_num_singleMu3 = in_file.Get('h_phi_num_%s' % (key)).Clone()
h_eta_phi_num_singleMu3 = in_file.Get('h_eta_phi_num_%s' % (key)).Clone()

key = TF + '_' + 'DoubleMu_15'

h_pt_denom_doubleMu15 = in_file.Get('h_pt_denom_%s' % key).Clone()
h_pt_2_denom_doubleMu15 = in_file.Get('h_pt_2_denom_%s' % key).Clone()
h_eta_denom_doubleMu15 = in_file.Get('h_eta_denom_%s' % key).Clone()
h_phi_denom_doubleMu15 = in_file.Get('h_phi_denom_%s' % key).Clone()
h_eta_phi_denom_doubleMu15 = in_file.Get('h_eta_phi_denom_%s' % key).Clone()

h_pt_num_doubleMu15 = in_file.Get('h_pt_num_%s' % (key)).Clone()
h_pt_2_num_doubleMu15 = in_file.Get('h_pt_2_num_%s' % (key)).Clone()
h_eta_num_doubleMu15 = in_file.Get('h_eta_num_%s' % (key)).Clone()
h_phi_num_doubleMu15 = in_file.Get('h_phi_num_%s' % (key)).Clone()
h_eta_phi_num_doubleMu15 = in_file.Get('h_eta_phi_num_%s' % (key)).Clone()

key = TF + '_' + 'DoubleMu_7'

h_pt_denom_doubleMu7 = in_file.Get('h_pt_denom_%s' % key).Clone()
h_pt_2_denom_doubleMu7 = in_file.Get('h_pt_2_denom_%s' % key).Clone()
h_eta_denom_doubleMu7 = in_file.Get('h_eta_denom_%s' % key).Clone()
h_phi_denom_doubleMu7 = in_file.Get('h_phi_denom_%s' % key).Clone()
h_eta_phi_denom_doubleMu7 = in_file.Get('h_eta_phi_denom_%s' % key).Clone()

h_pt_num_doubleMu7 = in_file.Get('h_pt_num_%s' % (key)).Clone()
h_pt_2_num_doubleMu7 = in_file.Get('h_pt_2_num_%s' % (key)).Clone()
h_eta_num_doubleMu7 = in_file.Get('h_eta_num_%s' % (key)).Clone()
h_phi_num_doubleMu7 = in_file.Get('h_phi_num_%s' % (key)).Clone()
h_eta_phi_num_doubleMu7 = in_file.Get('h_eta_phi_num_%s' % (key)).Clone()

key = TF + '_' + 'MuOpen_3'

h_pt_denom_muOpen3 = in_file.Get('h_pt_denom_%s' % key).Clone()
h_pt_2_denom_muOpen3 = in_file.Get('h_pt_2_denom_%s' % key).Clone()
h_eta_denom_muOpen3 = in_file.Get('h_eta_denom_%s' % key).Clone()
h_phi_denom_muOpen3 = in_file.Get('h_phi_denom_%s' % key).Clone()
h_eta_phi_denom_muOpen3 = in_file.Get('h_eta_phi_denom_%s' % key).Clone()

h_pt_num_muOpen3 = in_file.Get('h_pt_num_%s' % (key)).Clone()
h_pt_2_num_muOpen3 = in_file.Get('h_pt_2_num_%s' % (key)).Clone()
h_eta_num_muOpen3 = in_file.Get('h_eta_num_%s' % (key)).Clone()
h_phi_num_muOpen3 = in_file.Get('h_phi_num_%s' % (key)).Clone()
h_eta_phi_num_muOpen3 = in_file.Get('h_eta_phi_num_%s' % (key)).Clone()

eff_pt_singleMu22 = ROOT.TGraphAsymmErrors(h_pt_num_singleMu22,h_pt_denom_singleMu22)
eff_pt_2_singleMu22 = ROOT.TGraphAsymmErrors(h_pt_2_num_singleMu22,h_pt_2_denom_singleMu22)
eff_eta_singleMu22 = ROOT.TGraphAsymmErrors(h_eta_num_singleMu22,h_eta_denom_singleMu22)
eff_phi_singleMu22 = ROOT.TGraphAsymmErrors(h_phi_num_singleMu22,h_phi_denom_singleMu22)

eff_pt_singleMu15 = ROOT.TGraphAsymmErrors(h_pt_num_singleMu15,h_pt_denom_singleMu15)
eff_pt_2_singleMu15 = ROOT.TGraphAsymmErrors(h_pt_2_num_singleMu15,h_pt_2_denom_singleMu15)
eff_eta_singleMu15 = ROOT.TGraphAsymmErrors(h_eta_num_singleMu15,h_eta_denom_singleMu15)
eff_phi_singleMu15 = ROOT.TGraphAsymmErrors(h_phi_num_singleMu15,h_phi_denom_singleMu15)

eff_pt_singleMu7 = ROOT.TGraphAsymmErrors(h_pt_num_singleMu7,h_pt_denom_singleMu7)
eff_pt_2_singleMu7 = ROOT.TGraphAsymmErrors(h_pt_2_num_singleMu7,h_pt_2_denom_singleMu7)
eff_eta_singleMu7 = ROOT.TGraphAsymmErrors(h_eta_num_singleMu7,h_eta_denom_singleMu7)
eff_phi_singleMu7 = ROOT.TGraphAsymmErrors(h_phi_num_singleMu7,h_phi_denom_singleMu7)

eff_pt_singleMu3 = ROOT.TGraphAsymmErrors(h_pt_num_singleMu3,h_pt_denom_singleMu3)
eff_pt_2_singleMu3 = ROOT.TGraphAsymmErrors(h_pt_2_num_singleMu3,h_pt_2_denom_singleMu3)
eff_eta_singleMu3 = ROOT.TGraphAsymmErrors(h_eta_num_singleMu3,h_eta_denom_singleMu3)
eff_phi_singleMu3 = ROOT.TGraphAsymmErrors(h_phi_num_singleMu3,h_phi_denom_singleMu3)

eff_pt_doubleMu15 = ROOT.TGraphAsymmErrors(h_pt_num_doubleMu15,h_pt_denom_doubleMu15)
eff_pt_2_doubleMu15 = ROOT.TGraphAsymmErrors(h_pt_2_num_doubleMu15,h_pt_2_denom_doubleMu15)
eff_eta_doubleMu15 = ROOT.TGraphAsymmErrors(h_eta_num_doubleMu15,h_eta_denom_doubleMu15)
eff_phi_doubleMu15 = ROOT.TGraphAsymmErrors(h_phi_num_doubleMu15,h_phi_denom_doubleMu15)

eff_pt_doubleMu7 = ROOT.TGraphAsymmErrors(h_pt_num_doubleMu7,h_pt_denom_doubleMu7)
eff_pt_2_doubleMu7 = ROOT.TGraphAsymmErrors(h_pt_2_num_doubleMu7,h_pt_2_denom_doubleMu7)
eff_eta_doubleMu7 = ROOT.TGraphAsymmErrors(h_eta_num_doubleMu7,h_eta_denom_doubleMu7)
eff_phi_doubleMu7 = ROOT.TGraphAsymmErrors(h_phi_num_doubleMu7,h_phi_denom_doubleMu7)

eff_pt_muOpen3 = ROOT.TGraphAsymmErrors(h_pt_num_muOpen3,h_pt_denom_muOpen3)
eff_pt_2_muOpen3 = ROOT.TGraphAsymmErrors(h_pt_2_num_muOpen3,h_pt_2_denom_muOpen3)
eff_eta_muOpen3 = ROOT.TGraphAsymmErrors(h_eta_num_muOpen3,h_eta_denom_muOpen3)
eff_phi_muOpen3 = ROOT.TGraphAsymmErrors(h_phi_num_muOpen3,h_phi_denom_muOpen3)


eff_pt_singleMu22.SetLineColor(ROOT.kBlack)
eff_pt_singleMu22.SetMarkerColor(ROOT.kBlack)
eff_pt_singleMu22.SetLineWidth(1)
eff_pt_singleMu22.SetMarkerStyle(20)
eff_pt_singleMu22.SetMarkerSize(2)
eff_pt_singleMu22.GetXaxis().SetTitle("p_{T}(Probe Reco #mu) [GeV]")
eff_pt_singleMu22.GetYaxis().SetTitle("L1T Efficiency")
eff_pt_singleMu22.GetYaxis().SetTitleOffset(1.35)
eff_pt_singleMu22.GetXaxis().SetNdivisions(509)
eff_pt_singleMu22.GetYaxis().SetNdivisions(514)
eff_pt_singleMu22.SetMinimum(0)
eff_pt_singleMu22.SetMaximum(1.05)
eff_pt_singleMu22.GetXaxis().SetRangeUser(0,1000)

eff_pt_2_singleMu22.SetLineColor(ROOT.kBlack)
eff_pt_2_singleMu22.SetMarkerColor(ROOT.kBlack)
eff_pt_2_singleMu22.SetLineWidth(1)
eff_pt_2_singleMu22.SetMarkerStyle(20)
eff_pt_2_singleMu22.SetMarkerSize(2)
eff_pt_2_singleMu22.GetXaxis().SetTitle("p_{T}(Probe Reco #mu) [GeV]")
eff_pt_2_singleMu22.GetYaxis().SetTitle("L1T Efficiency")
eff_pt_2_singleMu22.GetYaxis().SetTitleOffset(1.35)
eff_pt_2_singleMu22.GetXaxis().SetNdivisions(509)
eff_pt_2_singleMu22.GetYaxis().SetNdivisions(514)
eff_pt_2_singleMu22.SetMinimum(0)
eff_pt_2_singleMu22.SetMaximum(1.05)
eff_pt_2_singleMu22.GetXaxis().SetRangeUser(0,60)

eff_eta_singleMu22.SetLineColor(ROOT.kBlack)
eff_eta_singleMu22.SetMarkerColor(ROOT.kBlack)
eff_eta_singleMu22.SetLineWidth(1)
eff_eta_singleMu22.SetMarkerStyle(20)
eff_eta_singleMu22.SetMarkerSize(2)
eff_eta_singleMu22.GetXaxis().SetTitle("#eta (Probe Reco #mu)")
eff_eta_singleMu22.GetYaxis().SetTitle("L1T Efficiency")
eff_eta_singleMu22.GetYaxis().SetTitleOffset(1.35)
eff_eta_singleMu22.GetXaxis().SetNdivisions(509)
eff_eta_singleMu22.GetYaxis().SetNdivisions(514)
eff_eta_singleMu22.SetMinimum(0)
eff_eta_singleMu22.SetMaximum(1.05)
# eff_eta[in_file_key+TF+WP].GetXaxis().SetRangeUser(0,60)

eff_phi_singleMu22.SetLineColor(ROOT.kBlack)
eff_phi_singleMu22.SetMarkerColor(ROOT.kBlack)
eff_phi_singleMu22.SetLineWidth(1)
eff_phi_singleMu22.SetMarkerStyle(20)
eff_phi_singleMu22.SetMarkerSize(2)
eff_phi_singleMu22.GetXaxis().SetTitle("#phi (Probe Reco #mu)")
eff_phi_singleMu22.GetYaxis().SetTitle("L1T Efficiency")
eff_phi_singleMu22.GetYaxis().SetTitleOffset(1.35)
eff_phi_singleMu22.GetXaxis().SetNdivisions(509)
eff_phi_singleMu22.GetYaxis().SetNdivisions(514)
eff_phi_singleMu22.SetMinimum(0)
eff_phi_singleMu22.SetMaximum(1.05)
# eff_pt[in_file_key+TF+WP].GetXaxis().SetRangeUser(0,60)


## SingleMu15
eff_pt_singleMu15.SetLineColor(ROOT.kRed)
eff_pt_singleMu15.SetMarkerColor(ROOT.kRed)
eff_pt_singleMu15.SetLineWidth(1)
eff_pt_singleMu15.SetMarkerStyle(23)
eff_pt_singleMu15.SetMarkerSize(2)

eff_pt_2_singleMu15.SetLineColor(ROOT.kRed)
eff_pt_2_singleMu15.SetMarkerColor(ROOT.kRed)
eff_pt_2_singleMu15.SetLineWidth(1)
eff_pt_2_singleMu15.SetMarkerStyle(23)
eff_pt_2_singleMu15.SetMarkerSize(2)

eff_eta_singleMu15.SetLineColor(ROOT.kRed)
eff_eta_singleMu15.SetMarkerColor(ROOT.kRed)
eff_eta_singleMu15.SetLineWidth(1)
eff_eta_singleMu15.SetMarkerStyle(23)
eff_eta_singleMu15.SetMarkerSize(2)

eff_phi_singleMu15.SetLineColor(ROOT.kRed)
eff_phi_singleMu15.SetMarkerColor(ROOT.kRed)
eff_phi_singleMu15.SetLineWidth(1)
eff_phi_singleMu15.SetMarkerStyle(23)
eff_phi_singleMu15.SetMarkerSize(2)

## DoubleMu15
eff_pt_doubleMu15.SetLineColor(ROOT.kRed)
eff_pt_doubleMu15.SetMarkerColor(ROOT.kRed)
eff_pt_doubleMu15.SetLineWidth(1)
eff_pt_doubleMu15.SetMarkerStyle(23)
eff_pt_doubleMu15.SetMarkerSize(2)

eff_pt_2_doubleMu15.SetLineColor(ROOT.kRed)
eff_pt_2_doubleMu15.SetMarkerColor(ROOT.kRed)
eff_pt_2_doubleMu15.SetLineWidth(1)
eff_pt_2_doubleMu15.SetMarkerStyle(23)
eff_pt_2_doubleMu15.SetMarkerSize(2)

eff_eta_doubleMu15.SetLineColor(ROOT.kRed)
eff_eta_doubleMu15.SetMarkerColor(ROOT.kRed)
eff_eta_doubleMu15.SetLineWidth(1)
eff_eta_doubleMu15.SetMarkerStyle(23)
eff_eta_doubleMu15.SetMarkerSize(2)

eff_phi_doubleMu15.SetLineColor(ROOT.kRed)
eff_phi_doubleMu15.SetMarkerColor(ROOT.kRed)
eff_phi_doubleMu15.SetLineWidth(1)
eff_phi_doubleMu15.SetMarkerStyle(23)
eff_phi_doubleMu15.SetMarkerSize(2)

## SingleMu7
eff_pt_singleMu7.SetLineColor(ROOT.kViolet)
eff_pt_singleMu7.SetMarkerColor(ROOT.kViolet)
eff_pt_singleMu7.SetLineWidth(1)
eff_pt_singleMu7.SetMarkerStyle(22)
eff_pt_singleMu7.SetMarkerSize(2)

eff_pt_2_singleMu7.SetLineColor(ROOT.kViolet)
eff_pt_2_singleMu7.SetMarkerColor(ROOT.kViolet)
eff_pt_2_singleMu7.SetLineWidth(1)
eff_pt_2_singleMu7.SetMarkerStyle(22)
eff_pt_2_singleMu7.SetMarkerSize(2)

eff_eta_singleMu7.SetLineColor(ROOT.kViolet)
eff_eta_singleMu7.SetMarkerColor(ROOT.kViolet)
eff_eta_singleMu7.SetLineWidth(1)
eff_eta_singleMu7.SetMarkerStyle(22)
eff_eta_singleMu7.SetMarkerSize(2)


eff_phi_singleMu7.SetLineColor(ROOT.kViolet)
eff_phi_singleMu7.SetMarkerColor(ROOT.kViolet)
eff_phi_singleMu7.SetLineWidth(1)
eff_phi_singleMu7.SetMarkerStyle(22)
eff_phi_singleMu7.SetMarkerSize(2)

## DoubleMu7
eff_pt_doubleMu7.SetLineColor(ROOT.kViolet)
eff_pt_doubleMu7.SetMarkerColor(ROOT.kViolet)
eff_pt_doubleMu7.SetLineWidth(1)
eff_pt_doubleMu7.SetMarkerStyle(22)
eff_pt_doubleMu7.SetMarkerSize(2)

eff_pt_2_doubleMu7.SetLineColor(ROOT.kViolet)
eff_pt_2_doubleMu7.SetMarkerColor(ROOT.kViolet)
eff_pt_2_doubleMu7.SetLineWidth(1)
eff_pt_2_doubleMu7.SetMarkerStyle(22)
eff_pt_2_doubleMu7.SetMarkerSize(2)

eff_eta_doubleMu7.SetLineColor(ROOT.kViolet)
eff_eta_doubleMu7.SetMarkerColor(ROOT.kViolet)
eff_eta_doubleMu7.SetLineWidth(1)
eff_eta_doubleMu7.SetMarkerStyle(22)
eff_eta_doubleMu7.SetMarkerSize(2)


eff_phi_doubleMu7.SetLineColor(ROOT.kViolet)
eff_phi_doubleMu7.SetMarkerColor(ROOT.kViolet)
eff_phi_doubleMu7.SetLineWidth(1)
eff_phi_doubleMu7.SetMarkerStyle(22)
eff_phi_doubleMu7.SetMarkerSize(2)

## SingleMu3
eff_pt_singleMu3.SetLineColor(ROOT.kBlue)
eff_pt_singleMu3.SetMarkerColor(ROOT.kBlue)
eff_pt_singleMu3.SetLineWidth(1)
eff_pt_singleMu3.SetMarkerStyle(21)
eff_pt_singleMu3.SetMarkerSize(2)

eff_pt_2_singleMu3.SetLineColor(ROOT.kBlue)
eff_pt_2_singleMu3.SetMarkerColor(ROOT.kBlue)
eff_pt_2_singleMu3.SetLineWidth(1)
eff_pt_2_singleMu3.SetMarkerStyle(21)
eff_pt_2_singleMu3.SetMarkerSize(2)

eff_eta_singleMu3.SetLineColor(ROOT.kBlue)
eff_eta_singleMu3.SetMarkerColor(ROOT.kBlue)
eff_eta_singleMu3.SetLineWidth(1)
eff_eta_singleMu3.SetMarkerStyle(21)
eff_eta_singleMu3.SetMarkerSize(2)


eff_phi_singleMu3.SetLineColor(ROOT.kBlue)
eff_phi_singleMu3.SetMarkerColor(ROOT.kBlue)
eff_phi_singleMu3.SetLineWidth(1)
eff_phi_singleMu3.SetMarkerStyle(21)
eff_phi_singleMu3.SetMarkerSize(2)

## MuOpen3
eff_pt_muOpen3.SetLineColor(ROOT.kBlue)
eff_pt_muOpen3.SetMarkerColor(ROOT.kBlue)
eff_pt_muOpen3.SetLineWidth(1)
eff_pt_muOpen3.SetMarkerStyle(21)
eff_pt_muOpen3.SetMarkerSize(2)

eff_pt_2_muOpen3.SetLineColor(ROOT.kBlue)
eff_pt_2_muOpen3.SetMarkerColor(ROOT.kBlue)
eff_pt_2_muOpen3.SetLineWidth(1)
eff_pt_2_muOpen3.SetMarkerStyle(21)
eff_pt_2_muOpen3.SetMarkerSize(2)

eff_eta_muOpen3.SetLineColor(ROOT.kBlue)
eff_eta_muOpen3.SetMarkerColor(ROOT.kBlue)
eff_eta_muOpen3.SetLineWidth(1)
eff_eta_muOpen3.SetMarkerStyle(21)
eff_eta_muOpen3.SetMarkerSize(2)


eff_phi_muOpen3.SetLineColor(ROOT.kBlue)
eff_phi_muOpen3.SetMarkerColor(ROOT.kBlue)
eff_phi_muOpen3.SetLineWidth(1)
eff_phi_muOpen3.SetMarkerStyle(21)
eff_phi_muOpen3.SetMarkerSize(2)



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
eff_pt_singleMu22.Draw("AP")
eff_pt_singleMu15.Draw("sames P")
eff_pt_singleMu7.Draw("sames P")
eff_pt_singleMu3.Draw("sames P")

leg.AddEntry(eff_pt_singleMu22,legText_summary['SingleMu22'][0])
leg.AddEntry(eff_pt_singleMu15,legText_summary['SingleMu15'][0])
leg.AddEntry(eff_pt_singleMu7,legText_summary['SingleMu7'][0])
leg.AddEntry(eff_pt_singleMu3,legText_summary['SingleMu3'][0])

leg.Draw('sames')
cms_label = ROOT.TLatex()
cms_label.SetTextSize(0.04)
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")

header = ROOT.TLatex()
header.SetTextSize(0.03)
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run2022B")


c_pt.SaveAs('plots/pdf/WPComp/%s_eff_pT_q12_%s.pdf' % (TF, in_file_text))



c_pt.cd()
leg = ROOT.TLegend(0.6,0.15,0.88,0.48)
eff_pt_2_singleMu22.Draw("AP")
eff_pt_2_singleMu15.Draw("sames P")
eff_pt_2_singleMu7.Draw("sames P")
eff_pt_2_singleMu3.Draw("sames P")

leg.AddEntry(eff_pt_2_singleMu22,legText_summary['SingleMu22'][0])
leg.AddEntry(eff_pt_2_singleMu15,legText_summary['SingleMu15'][0])
leg.AddEntry(eff_pt_2_singleMu7,legText_summary['SingleMu7'][0])
leg.AddEntry(eff_pt_2_singleMu3,legText_summary['SingleMu3'][0])

leg.Draw('sames')
cms_label = ROOT.TLatex()
cms_label.SetTextSize(0.04)
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")

header = ROOT.TLatex()
header.SetTextSize(0.03)
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run2022B")


c_pt.SaveAs('plots/pdf/WPComp/%s_eff_pT_2_q12_%s.pdf' % (TF, in_file_text))


c_eta.cd()
leg = ROOT.TLegend(0.6,0.15,0.88,0.48)
eff_eta_singleMu22.Draw("AP")
eff_eta_singleMu15.Draw("sames P")
eff_eta_singleMu7.Draw("sames P")
eff_eta_singleMu3.Draw("sames P")

leg.AddEntry(eff_eta_singleMu22,legText_summary['SingleMu22'][0])
leg.AddEntry(eff_eta_singleMu15,legText_summary['SingleMu15'][0])
leg.AddEntry(eff_eta_singleMu7,legText_summary['SingleMu7'][0])
leg.AddEntry(eff_eta_singleMu3,legText_summary['SingleMu3'][0])

leg.Draw('sames')
cms_label = ROOT.TLatex()
cms_label.SetTextSize(0.04)
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")

header = ROOT.TLatex()
header.SetTextSize(0.03)
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run2022B")


c_eta.SaveAs('plots/pdf/WPComp/%s_eff_eta_q12_%s.pdf' % (TF, in_file_text))



c_phi.cd()
leg = ROOT.TLegend(0.6,0.15,0.88,0.48)
eff_phi_singleMu22.Draw("AP")
eff_phi_singleMu15.Draw("sames P")
eff_phi_singleMu7.Draw("sames P")
eff_phi_singleMu3.Draw("sames P")

leg.AddEntry(eff_phi_singleMu22,legText_summary['SingleMu22'][0])
leg.AddEntry(eff_phi_singleMu15,legText_summary['SingleMu15'][0])
leg.AddEntry(eff_phi_singleMu7,legText_summary['SingleMu7'][0])
leg.AddEntry(eff_phi_singleMu3,legText_summary['SingleMu3'][0])

leg.Draw('sames')
cms_label = ROOT.TLatex()
cms_label.SetTextSize(0.04)
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")

header = ROOT.TLatex()
header.SetTextSize(0.03)
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run2022B")


c_phi.SaveAs('plots/pdf/WPComp/%s_eff_phi_q12_%s.pdf' % (TF, in_file_text))



c_pt.cd()
leg = ROOT.TLegend(0.6,0.15,0.88,0.48)
eff_pt_singleMu22.Draw("AP")
eff_pt_doubleMu15.Draw("sames P")
eff_pt_doubleMu7.Draw("sames P")
eff_pt_muOpen3.Draw("sames P")

leg.AddEntry(eff_pt_singleMu22,legText_summary['SingleMu22'][0])
leg.AddEntry(eff_pt_doubleMu15,legText_summary['DoubleMu15'][0])
leg.AddEntry(eff_pt_doubleMu7,legText_summary['DoubleMu7'][0])
leg.AddEntry(eff_pt_muOpen3,legText_summary['MuOpen3'][0])

leg.Draw('sames')
cms_label = ROOT.TLatex()
cms_label.SetTextSize(0.04)
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")

header = ROOT.TLatex()
header.SetTextSize(0.03)
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run2022B")

c_pt.SaveAs('plots/pdf/WPComp/%s_eff_pT_qAll_%s.pdf' % (TF, in_file_text))


c_pt.cd()
leg = ROOT.TLegend(0.6,0.15,0.88,0.48)
eff_pt_2_singleMu22.Draw("AP")
eff_pt_2_doubleMu15.Draw("sames P")
eff_pt_2_doubleMu7.Draw("sames P")
eff_pt_2_muOpen3.Draw("sames P")

leg.AddEntry(eff_pt_2_singleMu22,legText_summary['SingleMu22'][0])
leg.AddEntry(eff_pt_2_doubleMu15,legText_summary['DoubleMu15'][0])
leg.AddEntry(eff_pt_2_doubleMu7,legText_summary['DoubleMu7'][0])
leg.AddEntry(eff_pt_2_muOpen3,legText_summary['MuOpen3'][0])

leg.Draw('sames')
cms_label = ROOT.TLatex()
cms_label.SetTextSize(0.04)
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")

header = ROOT.TLatex()
header.SetTextSize(0.03)
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run2022B")


c_pt.SaveAs('plots/pdf/WPComp/%s_eff_pT_2_qAll_%s.pdf' % (TF, in_file_text))


c_eta.cd()
leg = ROOT.TLegend(0.6,0.15,0.88,0.48)
eff_eta_singleMu22.Draw("AP")
eff_eta_doubleMu15.Draw("sames P")
eff_eta_doubleMu7.Draw("sames P")
eff_eta_muOpen3.Draw("sames P")

leg.AddEntry(eff_eta_singleMu22,legText_summary['SingleMu22'][0])
leg.AddEntry(eff_eta_doubleMu15,legText_summary['DoubleMu15'][0])
leg.AddEntry(eff_eta_doubleMu7,legText_summary['DoubleMu7'][0])
leg.AddEntry(eff_eta_muOpen3,legText_summary['MuOpen3'][0])

leg.Draw('sames')
cms_label = ROOT.TLatex()
cms_label.SetTextSize(0.04)
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")

header = ROOT.TLatex()
header.SetTextSize(0.03)
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run2022B")


c_eta.SaveAs('plots/pdf/WPComp/%s_eff_eta_qAll_%s.pdf' % (TF, in_file_text))


c_phi.cd()
leg = ROOT.TLegend(0.6,0.15,0.88,0.48)
eff_phi_singleMu22.Draw("AP")
eff_phi_doubleMu15.Draw("sames P")
eff_phi_doubleMu7.Draw("sames P")
eff_phi_muOpen3.Draw("sames P")

leg.AddEntry(eff_phi_singleMu22,legText_summary['SingleMu22'][0])
leg.AddEntry(eff_phi_doubleMu15,legText_summary['DoubleMu15'][0])
leg.AddEntry(eff_phi_doubleMu7,legText_summary['DoubleMu7'][0])
leg.AddEntry(eff_phi_muOpen3,legText_summary['MuOpen3'][0])

leg.Draw('sames')
cms_label = ROOT.TLatex()
cms_label.SetTextSize(0.04)
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")

header = ROOT.TLatex()
header.SetTextSize(0.03)
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run2022B")


c_phi.SaveAs('plots/pdf/WPComp/%s_eff_phi_qAll_%s.pdf' % (TF, in_file_text))
