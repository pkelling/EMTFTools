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
pt_cut = 'Pt3'
suffix = ''
#suffix = '_sectM6'

# in_file = TFile( 'plots/L1T_eff_%s_297606_dEta_BX0_uGMT_HLT%s.root' % (pt_cut, suffix) )
in_files = {}
# in_files['SingleMuon_355872'] = ROOT.TFile( 'plots/L1T_eff_Pt22_2022C_BX0_uGMT_HLT_EMU_SingleMuon_355872.root' )
in_files['SingleMuon_355872'] = ROOT.TFile( 'plots/L1T_eff_Pt22_2022C_BX0_uGMT_HLT_UNP_SingleMuon_355872.root' )
# in_files['DY_cclut'] = ROOT.TFile( 'plots/L1T_eff_Pt22_2018D_BX0_uGMT_HLT_EMU_CCLUT_DY.root' )
# in_files['DY_base'] = ROOT.TFile( 'plots/L1T_eff_Pt22_2018D_BX0_uGMT_HLT_UNP_baseline_DY.root' )
# in_files['Pt7'] = ROOT.TFile( 'plots/L1T_eff_Pt7_2018D_BX0_uGMT_HLT_lowPtReco.root' )
# in_files['Pt15'] = ROOT.TFile( 'plots/L1T_eff_Pt15_2018D_BX0_uGMT_HLT_lowPtReco.root' )
# in_files['Pt22'] = ROOT.TFile( 'plots/L1T_eff_Pt22_2018D_BX0_uGMT_HLT_lowPtReco.root' )
TFs = ['uGMT', 'BMTF', 'OMTF', 'EMTF']
sample = 'SingleMuUnp'

WPs = {}
WPs['SingleMu']  = [ROOT.kRed]
WPs['DoubleMu']  = [ROOT.kBlue]
WPs['MuOpen']    = [ROOT.kBlack]

Colors = {}
Colors['MuOpen']    = [ROOT.kBlack]
Colors['DoubleMu']    = [ROOT.kRed]
Colors['SingleMu7']   = [ROOT.kViolet]
Colors['SingleMu']   = [ROOT.kBlue]
Colors['uGMT']    = [ROOT.kBlack]
Colors['BMTF']    = [ROOT.kRed]
Colors['OMTF']   = [ROOT.kViolet]
Colors['EMTF']   = [ROOT.kBlue]

Markers = {}
Markers['MuOpen']    = [20]
Markers['DoubleMu']    = [23]
Markers['SingleMu7']   = [22]
Markers['SingleMu']   = [21]
Markers['uGMT']    = [20]
Markers['BMTF']    = [23]
Markers['OMTF']   = [22]
Markers['EMTF']   = [21]

Pt = {}
Pt['MuOpen']    = [3]
Pt['DoubleMu']    = [7]
Pt['SingleMu7']   = [15]
Pt['SingleMu']   = [22]

Qual = {}
Qual['MuOpen']    = [4]
Qual['DoubleMu']    = [8]
Qual['SingleMu7']   = [8]
Qual['SingleMu']   = [12]

legText = {}
for TF in TFs:
  for WP in WPs:
    legText[TF+WP] = ['p_{T}^{%s} > %s GeV, L1_{qual} #geq %s' %(TF, Pt[WP][0], Qual[WP][0])]


legText_summary ={}
legText_summary['uGMTSingleMu']    = ['0 #leq |#eta^{#mu, offline}| #leq 2.4']
legText_summary['BMTFSingleMu']    = ['0 #leq |#eta^{#mu, offline}| #leq 0.83']
legText_summary['OMTFSingleMu']   = ['0.83 #leq |#eta^{#mu, offline}| #leq 1.24']
legText_summary['EMTFSingleMu']   = ['1.24 #leq |#eta^{#mu, offline}| #leq 2.4']

h_pt = {}
h_pt_2 = {}
h_eta  = {}
h_phi = {}

h_pt_trg = {}
h_pt_2_trg = {}
h_eta_trg = {}
h_phi_trg = {}

eff_pt = {}
eff_pt_2 = {}
eff_eta = {}
eff_phi = {}


for TF in TFs:
  for in_file_key in in_files.keys():
    h_pt[in_file_key+TF] = ROOT.TH1F()
    h_pt_2[in_file_key+TF] = ROOT.TH1F()
    h_eta[in_file_key+TF] = ROOT.TH1F()
    h_phi[in_file_key+TF] = ROOT.TH1F()

    for WP in WPs.keys():
      h_pt_trg[in_file_key+TF+WP] = ROOT.TH1F()
      h_pt_2_trg[in_file_key+TF+WP] = ROOT.TH1F()
      h_eta_trg[in_file_key+TF+WP] = ROOT.TH1F()
      h_phi_trg[in_file_key+TF+WP] = ROOT.TH1F()


for TF in TFs:
  for in_file_key in in_files.keys():
    h_pt[in_file_key+TF] = in_files[in_file_key].Get('h_pt_%s' % TF).Clone()
    h_pt_2[in_file_key+TF] = in_files[in_file_key].Get('h_pt_2_%s' % TF).Clone()
    h_eta[in_file_key+TF] = in_files[in_file_key].Get('h_eta_%s' % TF).Clone()
    h_phi[in_file_key+TF] = in_files[in_file_key].Get('h_phi_%s' % TF).Clone()

    for WP in WPs.keys():
      h_pt_trg[in_file_key+TF+WP] = in_files[in_file_key].Get('h_pt_%s_%s' % (TF, WP)).Clone()
      h_pt_2_trg[in_file_key+TF+WP] = in_files[in_file_key].Get('h_pt_2_%s_%s' % (TF, WP)).Clone()
      h_eta_trg[in_file_key+TF+WP] = in_files[in_file_key].Get('h_eta_%s_%s' % (TF, WP)).Clone()
      h_phi_trg[in_file_key+TF+WP] = in_files[in_file_key].Get('h_phi_%s_%s' % (TF, WP)).Clone()



eff_pt = {}

for TF in TFs:
  for in_file_key in in_files.keys():
    for WP in WPs.keys():
      eff_pt[in_file_key+TF+WP] = ROOT.TGraphAsymmErrors(h_pt_trg[in_file_key+TF+WP],h_pt[in_file_key+TF])
      eff_pt_2[in_file_key+TF+WP] = ROOT.TGraphAsymmErrors(h_pt_2_trg[in_file_key+TF+WP],h_pt_2[in_file_key+TF])
      eff_eta[in_file_key+TF+WP] = ROOT.TGraphAsymmErrors(h_eta_trg[in_file_key+TF+WP],h_eta[in_file_key+TF])
      eff_phi[in_file_key+TF+WP] = ROOT.TGraphAsymmErrors(h_phi_trg[in_file_key+TF+WP],h_phi[in_file_key+TF])

      eff_pt[in_file_key+TF+WP].SetLineColor(Colors[WP][0])
      eff_pt[in_file_key+TF+WP].SetMarkerColor(Colors[WP][0])
      eff_pt[in_file_key+TF+WP].SetLineWidth(1)
      eff_pt[in_file_key+TF+WP].SetMarkerStyle(Markers[WP][0])
      eff_pt[in_file_key+TF+WP].SetMarkerSize(2)
      eff_pt[in_file_key+TF+WP].GetXaxis().SetTitle("p_{T}(Probe Reco #mu) [GeV]")
      eff_pt[in_file_key+TF+WP].GetYaxis().SetTitle("L1T Efficiency")
      eff_pt[in_file_key+TF+WP].GetYaxis().SetTitleOffset(1.35)
      eff_pt[in_file_key+TF+WP].GetXaxis().SetNdivisions(509)
      eff_pt[in_file_key+TF+WP].GetYaxis().SetNdivisions(514)
      eff_pt[in_file_key+TF+WP].SetMinimum(0)
      eff_pt[in_file_key+TF+WP].SetMaximum(1.05)
      eff_pt[in_file_key+TF+WP].GetXaxis().SetRangeUser(0,1000)

      eff_pt_2[in_file_key+TF+WP].SetLineColor(Colors[WP][0])
      eff_pt_2[in_file_key+TF+WP].SetMarkerColor(Colors[WP][0])
      eff_pt_2[in_file_key+TF+WP].SetLineWidth(1)
      eff_pt_2[in_file_key+TF+WP].SetMarkerStyle(Markers[WP][0])
      eff_pt_2[in_file_key+TF+WP].SetMarkerSize(2)
      eff_pt_2[in_file_key+TF+WP].GetXaxis().SetTitle("p_{T}(Probe Reco #mu) [GeV]")
      eff_pt_2[in_file_key+TF+WP].GetYaxis().SetTitle("L1T Efficiency")
      eff_pt_2[in_file_key+TF+WP].GetYaxis().SetTitleOffset(1.35)
      eff_pt_2[in_file_key+TF+WP].GetXaxis().SetNdivisions(509)
      eff_pt_2[in_file_key+TF+WP].GetYaxis().SetNdivisions(514)
      eff_pt_2[in_file_key+TF+WP].SetMinimum(0)
      eff_pt_2[in_file_key+TF+WP].SetMaximum(1.05)
      eff_pt_2[in_file_key+TF+WP].GetXaxis().SetRangeUser(0,60)

      eff_eta[in_file_key+TF+WP].SetLineColor(Colors[WP][0])
      eff_eta[in_file_key+TF+WP].SetMarkerColor(Colors[WP][0])
      eff_eta[in_file_key+TF+WP].SetLineWidth(1)
      eff_eta[in_file_key+TF+WP].SetMarkerStyle(Markers[WP][0])
      eff_eta[in_file_key+TF+WP].SetMarkerSize(2)
      eff_eta[in_file_key+TF+WP].GetXaxis().SetTitle("#eta (Probe Reco #mu)")
      eff_eta[in_file_key+TF+WP].GetYaxis().SetTitle("L1T Efficiency")
      eff_eta[in_file_key+TF+WP].GetYaxis().SetTitleOffset(1.35)
      eff_eta[in_file_key+TF+WP].GetXaxis().SetNdivisions(509)
      eff_eta[in_file_key+TF+WP].GetYaxis().SetNdivisions(514)
      eff_eta[in_file_key+TF+WP].SetMinimum(0)
      eff_eta[in_file_key+TF+WP].SetMaximum(1.05)
      # eff_eta[in_file_key+TF+WP].GetXaxis().SetRangeUser(0,60)

      eff_phi[in_file_key+TF+WP].SetLineColor(Colors[WP][0])
      eff_phi[in_file_key+TF+WP].SetMarkerColor(Colors[WP][0])
      eff_phi[in_file_key+TF+WP].SetLineWidth(1)
      eff_phi[in_file_key+TF+WP].SetMarkerStyle(Markers[WP][0])
      eff_phi[in_file_key+TF+WP].SetMarkerSize(2)
      eff_phi[in_file_key+TF+WP].GetXaxis().SetTitle("#phi (Probe Reco #mu)")
      eff_phi[in_file_key+TF+WP].GetYaxis().SetTitle("L1T Efficiency")
      eff_phi[in_file_key+TF+WP].GetYaxis().SetTitleOffset(1.35)
      eff_phi[in_file_key+TF+WP].GetXaxis().SetNdivisions(509)
      eff_phi[in_file_key+TF+WP].GetYaxis().SetNdivisions(514)
      eff_phi[in_file_key+TF+WP].SetMinimum(0)
      eff_phi[in_file_key+TF+WP].SetMaximum(1.05)
      # eff_pt[in_file_key+TF+WP].GetXaxis().SetRangeUser(0,60)


c_pt  = ROOT.TCanvas('c_pt', '', 1280, 1024)
c_eta = ROOT.TCanvas('c_eta', '', 1280, 1024)
c_phi = ROOT.TCanvas('c_phi', '', 1280, 1024)

# c_pt_bmtf  = ROOT.TCanvas('c_pt_bmtf')
# c_eta_bmtf = ROOT.TCanvas('c_eta_bmtf')
# c_phi_bmtf = ROOT.TCanvas('c_phi_bmtf')


c_pt.SetGridx()
c_pt.SetGridy()

# c_pt_bmtf.SetGridx()
# c_pt_bmtf.SetGridy()

c_eta.SetGridx()
c_eta.SetGridy()

c_phi.SetGridx()
c_phi.SetGridy()


ROOT.gStyle.SetLegendBorderSize(0)
ROOT.gStyle.SetLegendTextSize(0.02)



for TF in TFs:
  c_pt.cd()
  for in_file_key in in_files.keys():
    leg = ROOT.TLegend(0.6,0.15,0.88,0.48)
    for WP in WPs.keys():
      if WP == 'SingleMu': eff_pt[in_file_key+TF+WP].Draw("AP")
      else: eff_pt[in_file_key+TF+WP].Draw("sames P")

      leg.AddEntry(eff_pt[in_file_key+TF+WP],legText[TF+WP][0])

    leg.Draw('sames')
    cms_label = ROOT.TLatex()
    cms_label.SetTextSize(0.04)
    cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary} Simulation}")

    header = ROOT.TLatex()
    header.SetTextSize(0.03)
    header.DrawLatexNDC(0.67, 0.92, "#sqrt{s} = 13.6 TeV, #approx 42 pb^{-1}")

    c_pt.SaveAs('plots/pdf/%s/%s_eff_pT_%s.pdf' % (sample, TF, in_file_key))

for TF in TFs:
  c_pt.cd()
  for in_file_key in in_files.keys():
    leg2 = ROOT.TLegend(0.6,0.15,0.88,0.48)
    for WP in WPs.keys():
      if WP == 'SingleMu': eff_pt_2[in_file_key+TF+WP].Draw("AP")
      else: eff_pt_2[in_file_key+TF+WP].Draw("sames P")

      leg2.AddEntry(eff_pt_2[in_file_key+TF+WP],legText[TF+WP][0])

    leg2.Draw('sames')
    cms_label = ROOT.TLatex()
    cms_label.SetTextSize(0.04)
    cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary} Simulation}")

    header = ROOT.TLatex()
    header.SetTextSize(0.03)
    header.DrawLatexNDC(0.67, 0.92, "#sqrt{s} = 13.6 TeV, #approx 42 pb^{-1}")

    c_pt.SaveAs('plots/pdf/%s/%s_eff_pT_2_%s.pdf' % (sample, TF, in_file_key))

for TF in TFs:
  c_eta.cd()
  for in_file_key in in_files.keys():
    leg3 = ROOT.TLegend(0.6,0.15,0.88,0.48)
    for WP in WPs.keys():
      if WP == 'SingleMu': eff_eta[in_file_key+TF+WP].Draw("AP")
      else: eff_eta[in_file_key+TF+WP].Draw("sames P")

      leg3.AddEntry(eff_eta[in_file_key+TF+WP],legText[TF+WP][0])

    leg3.Draw('sames')
    cms_label = ROOT.TLatex()
    cms_label.SetTextSize(0.04)
    cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary} Simulation}")

    header = ROOT.TLatex()
    header.SetTextSize(0.03)
    header.DrawLatexNDC(0.67, 0.92, "#sqrt{s} = 13.6 TeV, #approx 42 pb^{-1}")

    c_eta.SaveAs('plots/pdf/%s/%s_eff_eta_%s.pdf' % (sample, TF, in_file_key))

for TF in TFs:
  c_phi.cd()
  for in_file_key in in_files.keys():
    leg4 = ROOT.TLegend(0.6,0.15,0.88,0.48)
    for WP in WPs.keys():
      if WP == 'SingleMu': eff_phi[in_file_key+TF+WP].Draw("AP")
      else: eff_phi[in_file_key+TF+WP].Draw("sames P")

      leg4.AddEntry(eff_phi[in_file_key+TF+WP],legText[TF+WP][0])

    leg4.Draw('sames')
    cms_label = ROOT.TLatex()
    cms_label.SetTextSize(0.04)
    cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary} Simulation}")

    header = ROOT.TLatex()
    header.SetTextSize(0.03)
    header.DrawLatexNDC(0.67, 0.92, "#sqrt{s} = 13.6 TeV, #approx 42 pb^{-1}")

    c_phi.SaveAs('plots/pdf/%s/%s_eff_phi_%s.pdf' % (sample, TF, in_file_key))

c_pt.cd()
for in_file_key in in_files.keys():
  leg = ROOT.TLegend(0.6,0.15,0.88,0.48)
  TF = 'uGMT'
  WP = 'SingleMu'
  eff_pt[in_file_key+TF+WP].SetLineColor(ROOT.kBlack)
  eff_pt[in_file_key+TF+WP].SetMarkerColor(ROOT.kBlack)
  eff_pt[in_file_key+TF+WP].Draw("AP")

  leg.AddEntry(eff_pt[in_file_key+TF+WP],legText[TF+WP][0])

  leg.Draw('sames')
  cms_label = ROOT.TLatex()
  cms_label.SetTextSize(0.04)
  cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary} Simulation}")

  header = ROOT.TLatex()
  header.SetTextSize(0.03)
  header.DrawLatexNDC(0.67, 0.92, "#sqrt{s} = 13.6 TeV, #approx 42 pb^{-1}")

  c_pt.SaveAs('plots/pdf/%s/%s_eff_pT_%s_summary.pdf' % (sample, TF, in_file_key))

c_pt.cd()
for in_file_key in in_files.keys():
  leg2 = ROOT.TLegend(0.6,0.15,0.88,0.48)
  TF = 'uGMT'
  WP = 'SingleMu'
  eff_pt_2[in_file_key+TF+WP].SetLineColor(ROOT.kBlack)
  eff_pt_2[in_file_key+TF+WP].SetMarkerColor(ROOT.kBlack)
  eff_pt_2[in_file_key+TF+WP].Draw("AP")
  leg2.AddEntry(eff_pt_2[in_file_key+TF+WP],legText[TF+WP][0])

  leg2.Draw('sames')
  cms_label = ROOT.TLatex()
  cms_label.SetTextSize(0.04)
  cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary} Simulation}")

  header = ROOT.TLatex()
  header.SetTextSize(0.03)
  header.DrawLatexNDC(0.67, 0.92, "#sqrt{s} = 13.6 TeV, #approx 42 pb^{-1}")

  c_pt.SaveAs('plots/pdf/%s/%s_eff_pT_2_%s_summary.pdf' % (sample, TF, in_file_key))

c_eta.cd()
for in_file_key in in_files.keys():
  leg3 = ROOT.TLegend(0.6,0.15,0.88,0.48)
  TF = 'uGMT'
  WP = 'SingleMu'
  eff_eta[in_file_key+TF+WP].SetLineColor(ROOT.kBlack)
  eff_eta[in_file_key+TF+WP].SetMarkerColor(ROOT.kBlack)
  eff_eta[in_file_key+TF+WP].Draw("AP")

  leg3.AddEntry(eff_eta[in_file_key+TF+WP],legText[TF+WP][0])

  leg3.Draw('sames')
  cms_label = ROOT.TLatex()
  cms_label.SetTextSize(0.04)
  cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary} Simulation}")

  header = ROOT.TLatex()
  header.SetTextSize(0.03)
  header.DrawLatexNDC(0.67, 0.92, "#sqrt{s} = 13.6 TeV, #approx 42 pb^{-1}")

  c_eta.SaveAs('plots/pdf/%s/%s_eff_eta_%s_summary.pdf' % (sample, TF, in_file_key))

for in_file_key in in_files.keys():
  c_pt.cd()
  leg2 = ROOT.TLegend(0.6,0.15,0.88,0.48)
  for TF in TFs:
    WP = 'SingleMu'
    eff_pt_2[in_file_key+TF+WP].SetLineColor(Colors[TF][0])
    eff_pt_2[in_file_key+TF+WP].SetMarkerColor(Colors[TF][0])
    eff_pt_2[in_file_key+TF+WP].SetMarkerStyle(Markers[TF][0])
    if TF == 'uGMT': eff_pt_2[in_file_key+TF+WP].Draw("AP")
    else: eff_pt_2[in_file_key+TF+WP].Draw("sames P")

    leg2.AddEntry(eff_pt_2[in_file_key+TF+WP],legText_summary[TF+WP][0])

  leg2.Draw('sames')
  cms_label = ROOT.TLatex()
  cms_label.SetTextSize(0.04)
  cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary} Simulation}")

  header = ROOT.TLatex()
  header.SetTextSize(0.03)
  header.DrawLatexNDC(0.67, 0.92, "#sqrt{s} = 13.6 TeV, #approx 42 pb^{-1}")

  box = ROOT.TBox(40,0.5,50,0.7)
  box.SetLineColor(ROOT.kWhite)
  box.SetFillColorAlpha(ROOT.kWhite,1)
  box.SetFillStyle(1)
  box.Draw()

  pt_label = ROOT.TLatex()
  pt_label.SetTextSize(0.025)
  pt_label.DrawLatexNDC(0.64, 0.58, "p_{T}^{#mu, L1} #geq 22 GeV")

  qual_label = ROOT.TLatex()
  qual_label.SetTextSize(0.025)
  qual_label.DrawLatexNDC(0.64, 0.52, "L1_{qual} #geq 12")

  c_pt.SaveAs('plots/pdf/%s/%s_eff_pT_2_summaryTF.pdf' % (sample, in_file_key))

