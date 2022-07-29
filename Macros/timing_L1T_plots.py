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
# in_files['SingleMuon_355380'] = ROOT.TFile( 'plots/L1T_eff_Pt22_2022B_BX0_uGMT_HLT_UNP_SingleMuon_355380.root' )
# in_files['SingleMuon_355380_noTP'] = ROOT.TFile( 'plots/L1T_eff_Pt22_2022B_BX0_uGMT_HLT_UNP_SingleMuon_355380_no_TP.root' )
# in_files['ZB_355207_355208_timing_5GeV'] = ROOT.TFile( 'plots/L1T_eff_Pt22_2022B_BX0_uGMT_HLT_UNP_ZB_355207_355208_timing_5GeV.root' )
# in_files['ZB_355207_355383_timing'] = ROOT.TFile( 'plots/L1T_eff_Pt22_2022B_BX0_uGMT_HLT_UNP_ZB_355207_355383_timing.root' )
# in_files['EG_355381_timing'] = ROOT.TFile( 'plots/L1T_eff_Pt22_2022B_BX0_uGMT_HLT_UNP_EG_355381_timing.root' )
# in_files['EG_355381_timing_5GeV'] = ROOT.TFile( 'plots/L1T_eff_Pt22_2022B_BX0_uGMT_HLT_UNP_EG_355381_timing_5GeV.root' )
in_files['JetHT_355382_355383_timing_5GeV'] = ROOT.TFile( 'plots/L1T_eff_Pt22_2022B_BX0_uGMT_HLT_UNP_JetHT_355382_355383_timing_5GeV.root' )
# in_files['JetHT_355382_355383_timing'] = ROOT.TFile( 'plots/L1T_eff_Pt22_2022B_BX0_uGMT_HLT_UNP_JetHT_355382_355383_timing.root' )
# in_files['DY_cclut'] = ROOT.TFile( 'plots/L1T_eff_Pt22_2018D_BX0_uGMT_HLT_EMU_CCLUT_DY.root' )
# in_files['DY_base'] = ROOT.TFile( 'plots/L1T_eff_Pt22_2018D_BX0_uGMT_HLT_UNP_baseline_DY.root' )
# in_files['Pt7'] = ROOT.TFile( 'plots/L1T_eff_Pt7_2018D_BX0_uGMT_HLT_lowPtReco.root' )
# in_files['Pt15'] = ROOT.TFile( 'plots/L1T_eff_Pt15_2018D_BX0_uGMT_HLT_lowPtReco.root' )
# in_files['Pt22'] = ROOT.TFile( 'plots/L1T_eff_Pt22_2018D_BX0_uGMT_HLT_lowPtReco.root' )
TFs = ['uGMT', 'BMTF', 'OMTF', 'EMTF']

sample = 'ZBNoTP'

WPs = {}
WPs['SingleMu']  = [ROOT.kRed]
WPs['SingleMu7'] = [ROOT.kViolet]
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

l1_qual = {}
l1_qual['SingleMu']  = [12]
l1_qual['SingleMu7'] = [8]
l1_qual['DoubleMu']  = [8]
l1_qual['MuOpen']    = [4]

legText = {}
# for TF in TFs:
  # for WP in WPs:
    # legText[TF+WP] = ['%s L1_{qual} #geq %s' %(TF, Pt[WP][0], Qual[WP][0])]


legText_summary ={}
legText_summary['uGMTSingleMu']    = ['0 #leq |#eta^{#mu, offline}| #leq 2.4']
legText_summary['BMTFSingleMu']    = ['0 #leq |#eta^{#mu, offline}| #leq 0.83']
legText_summary['OMTFSingleMu']   = ['0.83 #leq |#eta^{#mu, offline}| #leq 1.24']
legText_summary['EMTFSingleMu']   = ['1.24 #leq |#eta^{#mu, offline}| #leq 2.4']

h_bx_trg = {}
h_bx_trg_qual = {}
for TF in TFs:
    h_bx_trg [TF] = ROOT.TH1D()

    for qual in l1_qual.keys():
        key = '%s_%s' % (TF, qual)
        h_bx_trg_qual [key] = ROOT.TH1D()


for TF in TFs:
  for in_file_key in in_files.keys():
    h_bx_trg [TF] = in_files[in_file_key].Get('bx_trg_%s' % (TF)).Clone()

    for qual in l1_qual.keys():
      key = '%s_%s' % (TF, qual)
      h_bx_trg_qual[key] = in_files[in_file_key].Get('bx_trg_qual_%s' % (key)).Clone()
      print(key)





      h_bx_trg_qual[key].SetLineColor(Colors[TF][0])
      h_bx_trg_qual[key].SetMarkerColor(Colors[TF][0])
      h_bx_trg_qual[key].SetLineWidth(1)
      h_bx_trg_qual[key].SetMarkerStyle(Markers[TF][0])
      h_bx_trg_qual[key].SetMarkerSize(2)
      h_bx_trg_qual[key].GetXaxis().SetTitle("BX")
      h_bx_trg_qual[key].GetYaxis().SetTitle("Fraction of muons (Qual >= %s)" % l1_qual[qual][0])
      h_bx_trg_qual[key].GetYaxis().SetTitleOffset(1.35)
      h_bx_trg_qual[key].GetXaxis().SetNdivisions(509)
      h_bx_trg_qual[key].GetYaxis().SetNdivisions(514)
      h_bx_trg_qual[key].SetMinimum(0)
      h_bx_trg_qual[key].SetMaximum(12)
      h_bx_trg_qual[key].Scale(1/h_bx_trg_qual[key].GetBinContent(4))



c_bx  = ROOT.TCanvas('c_bx', '', 1280, 1024)
# c_eta = ROOT.TCanvas('c_eta', '', 1280, 1024)
# c_phi = ROOT.TCanvas('c_phi', '', 1280, 1024)

# c_pt_bmtf  = ROOT.TCanvas('c_pt_bmtf')
# c_eta_bmtf = ROOT.TCanvas('c_eta_bmtf')
# c_phi_bmtf = ROOT.TCanvas('c_phi_bmtf')


# c_bx.SetGridx()
# c_bx.SetGridy()
c_bx.SetLogy()

# c_pt_bmtf.SetGridx()
# c_pt_bmtf.SetGridy()

# c_eta.SetGridx()
# c_eta.SetGridy()

# c_phi.SetGridx()
# c_phi.SetGridy()


ROOT.gStyle.SetLegendBorderSize(0)
ROOT.gStyle.SetLegendTextSize(0.02)
ROOT.gStyle.SetOptStat(0)

for WP in WPs.keys():
  leg = ROOT.TLegend(0.6,0.55,0.88,0.88)
  for TF in TFs:
    if TF == 'uGMT': continue
    c_bx.cd()
    key = '%s_%s' % (TF, WP)
    print(key)
    if TF == 'BMTF': h_bx_trg_qual[key].Draw("hist")
    else: h_bx_trg_qual[key].Draw("hist same")

    leg.AddEntry(h_bx_trg_qual[key],legText_summary[TF+'SingleMu'][0])

  leg.Draw('sames')
  cms_label = ROOT.TLatex()
  cms_label.SetTextSize(0.04)
  cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary} Simulation}")

  header = ROOT.TLatex()
  header.SetTextSize(0.03)
  header.DrawLatexNDC(0.67, 0.92, "#sqrt{s} = 13.6 TeV, Run 3 data")

  c_bx.SaveAs('plots/pdf/timing/%s_BX_%s.pdf' % (WP, in_file_key))

# for TF in TFs:
#   c_pt.cd()
#   for in_file_key in in_files.keys():
#     leg2 = ROOT.TLegend(0.6,0.15,0.88,0.48)
#     for WP in WPs.keys():
#       if WP == 'SingleMu7': eff_pt_2[in_file_key+TF+WP].Draw("AP")
#       else: eff_pt_2[in_file_key+TF+WP].Draw("sames P")

#       leg2.AddEntry(eff_pt_2[in_file_key+TF+WP],legText[TF+WP][0])

#     leg2.Draw('sames')
#     cms_label = ROOT.TLatex()
#     cms_label.SetTextSize(0.04)
#     cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary} Simulation}")

#     header = ROOT.TLatex()
#     header.SetTextSize(0.03)
#     header.DrawLatexNDC(0.67, 0.92, "#sqrt{s} = 13 TeV, Run 3 MC")

#     c_pt.SaveAs('plots/pdf/%s/%s_eff_pT_2_%s.pdf' % (sample, TF, in_file_key))

# for TF in TFs:
#   c_eta.cd()
#   for in_file_key in in_files.keys():
#     leg3 = ROOT.TLegend(0.6,0.15,0.88,0.48)
#     for WP in WPs.keys():
#       if WP == 'SingleMu7': eff_eta[in_file_key+TF+WP].Draw("AP")
#       else: eff_eta[in_file_key+TF+WP].Draw("sames P")

#       leg3.AddEntry(eff_eta[in_file_key+TF+WP],legText[TF+WP][0])

#     leg3.Draw('sames')
#     cms_label = ROOT.TLatex()
#     cms_label.SetTextSize(0.04)
#     cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary} Simulation}")

#     header = ROOT.TLatex()
#     header.SetTextSize(0.03)
#     header.DrawLatexNDC(0.67, 0.92, "#sqrt{s} = 13 TeV, Run 3 MC")

#     c_eta.SaveAs('plots/pdf/%s/%s_eff_eta_%s.pdf' % (sample,TF, in_file_key))

# for TF in TFs:
#   c_phi.cd()
#   for in_file_key in in_files.keys():
#     leg4 = ROOT.TLegend(0.6,0.15,0.88,0.48)
#     for WP in WPs.keys():
#       if WP == 'SingleMu7': eff_phi[in_file_key+TF+WP].Draw("AP")
#       else: eff_phi[in_file_key+TF+WP].Draw("sames P")

#       leg4.AddEntry(eff_phi[in_file_key+TF+WP],legText[TF+WP][0])

#     leg4.Draw('sames')
#     cms_label = ROOT.TLatex()
#     cms_label.SetTextSize(0.04)
#     cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary} Simulation}")

#     header = ROOT.TLatex()
#     header.SetTextSize(0.03)
#     header.DrawLatexNDC(0.67, 0.92, "#sqrt{s} = 13 TeV, Run 3 MC")

#     c_phi.SaveAs('plots/pdf/%s/%s_eff_phi_%s.pdf' % (sample,TF, in_file_key))

# c_pt.cd()
# for in_file_key in in_files.keys():
#   leg = ROOT.TLegend(0.6,0.15,0.88,0.48)
#   TF = 'uGMT'
#   WP = 'SingleMu7'
#   eff_pt[in_file_key+TF+WP].SetLineColor(ROOT.kBlack)
#   eff_pt[in_file_key+TF+WP].SetMarkerColor(ROOT.kBlack)
#   eff_pt[in_file_key+TF+WP].Draw("AP")

#   leg.AddEntry(eff_pt[in_file_key+TF+WP],legText[TF+WP][0])

#   leg.Draw('sames')
#   cms_label = ROOT.TLatex()
#   cms_label.SetTextSize(0.04)
#   cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary} Simulation}")

#   header = ROOT.TLatex()
#   header.SetTextSize(0.03)
#   header.DrawLatexNDC(0.67, 0.92, "#sqrt{s} = 13 TeV, Run 3 MC")

#   c_pt.SaveAs('plots/pdf/%s/%s_eff_pT_%s_summary.pdf' % (sample,TF, in_file_key))

# c_pt.cd()
# for in_file_key in in_files.keys():
#   leg2 = ROOT.TLegend(0.6,0.15,0.88,0.48)
#   TF = 'uGMT'
#   WP = 'SingleMu7'
#   eff_pt_2[in_file_key+TF+WP].SetLineColor(ROOT.kBlack)
#   eff_pt_2[in_file_key+TF+WP].SetMarkerColor(ROOT.kBlack)
#   eff_pt_2[in_file_key+TF+WP].Draw("AP")
#   leg2.AddEntry(eff_pt_2[in_file_key+TF+WP],legText[TF+WP][0])

#   leg2.Draw('sames')
#   cms_label = ROOT.TLatex()
#   cms_label.SetTextSize(0.04)
#   cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary} Simulation}")

#   header = ROOT.TLatex()
#   header.SetTextSize(0.03)
#   header.DrawLatexNDC(0.67, 0.92, "#sqrt{s} = 13 TeV, Run 3 MC")

#   c_pt.SaveAs('plots/pdf/%s/%s_eff_pT_2_%s_summary.pdf' % (sample, TF, in_file_key))

# c_eta.cd()
# for in_file_key in in_files.keys():
#   leg3 = ROOT.TLegend(0.6,0.15,0.88,0.48)
#   TF = 'uGMT'
#   WP = 'SingleMu7'
#   eff_eta[in_file_key+TF+WP].SetLineColor(ROOT.kBlack)
#   eff_eta[in_file_key+TF+WP].SetMarkerColor(ROOT.kBlack)
#   eff_eta[in_file_key+TF+WP].Draw("AP")

#   leg3.AddEntry(eff_eta[in_file_key+TF+WP],legText[TF+WP][0])

#   leg3.Draw('sames')
#   cms_label = ROOT.TLatex()
#   cms_label.SetTextSize(0.04)
#   cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary} Simulation}")

#   header = ROOT.TLatex()
#   header.SetTextSize(0.03)
#   header.DrawLatexNDC(0.67, 0.92, "#sqrt{s} = 13 TeV, Run 3 MC")

#   c_eta.SaveAs('plots/pdf/%s/%s_eff_eta_%s_summary.pdf' % (sample, TF, in_file_key))

# for in_file_key in in_files.keys():
#   c_pt.cd()
#   leg2 = ROOT.TLegend(0.6,0.15,0.88,0.48)
#   for TF in TFs:
#     WP = 'SingleMu7'
#     eff_pt_2[in_file_key+TF+WP].SetLineColor(Colors[TF][0])
#     eff_pt_2[in_file_key+TF+WP].SetMarkerColor(Colors[TF][0])
#     eff_pt_2[in_file_key+TF+WP].SetMarkerStyle(Markers[TF][0])
#     if TF == 'uGMT': eff_pt_2[in_file_key+TF+WP].Draw("AP")
#     else: eff_pt_2[in_file_key+TF+WP].Draw("sames P")

#     leg2.AddEntry(eff_pt_2[in_file_key+TF+WP],legText_summary[TF+WP][0])

#   leg2.Draw('sames')
#   cms_label = ROOT.TLatex()
#   cms_label.SetTextSize(0.04)
#   cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary} Simulation}")

#   header = ROOT.TLatex()
#   header.SetTextSize(0.03)
#   header.DrawLatexNDC(0.67, 0.92, "#sqrt{s} = 13 TeV, Run 3 MC")

#   box = ROOT.TBox(40,0.5,50,0.7)
#   box.SetLineColor(ROOT.kWhite)
#   box.SetFillColorAlpha(ROOT.kWhite,1)
#   box.SetFillStyle(1)
#   box.Draw()

#   pt_label = ROOT.TLatex()
#   pt_label.SetTextSize(0.025)
#   pt_label.DrawLatexNDC(0.64, 0.58, "p_{T}^{#mu, L1} #geq 22 GeV")

#   qual_label = ROOT.TLatex()
#   qual_label.SetTextSize(0.025)
#   qual_label.DrawLatexNDC(0.64, 0.52, "L1_{qual} #geq 12")

#   c_pt.SaveAs('plots/pdf/%s/%s_eff_pT_2_summaryTF.pdf' % (sample, in_file_key))

