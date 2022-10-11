#! /usr/bin/env python

########################################################
## eff_L1T.py   
## A script to find L1T efficiency from L1Ntuples
## By Andrew Brinkerhoff
##
########################################################

print ('------> Setting Environment')

import sys
import math
from ROOT import *
from array import *
from os import listdir
from os.path import isfile, join

gROOT.SetBatch(True)


print ('------> Importing Root File')

## Configuration settings
MAX_FILE = 1000        ## Maximum number of input files (use "-1" for unlimited)
MAX_EVT  = -1       ## Maximum number of events to process
PRT_EVT  = 10000     ## Print every Nth event



## L1NTuple branches
evt_tree  = TChain('l1EventTree/L1EventTree')
ugt_tree  = TChain('l1uGTTree/L1uGTTree')

# f_ZB2022 = "/afs/cern.ch/user/e/eyigitba/L1T_ana/CMSSW_12_4_8_miniAODAnalysis/src/HLTPhysics_357688.csv"
f_ZB2022 = "/afs/cern.ch/user/e/eyigitba/L1T_ana/CMSSW_12_4_8_miniAODAnalysis/src/fixed_2022EphHLTPhys_run357688.list"

## Load input files
list_ZB2022 = []

with open(f_ZB2022) as file:
    while (line := file.readline().rstrip()):
        list_ZB2022.append(line)

for f in range(len(list_ZB2022)):
    evt_tree.Add(list_ZB2022[f])
    ugt_tree.Add(list_ZB2022[f])

    if f == MAX_FILE: break


h_event_per_bx = TH1D('h_event_per_bx',  '', 56, 0, 55)
h_event_per_bx_L1_SingleMu22 = TH1D('h_event_per_bx_L1_SingleMu22',  '', 56, 0, 55)
h_event_per_bx_L1_SingleEG36er2p5 = TH1D('h_event_per_bx_L1_SingleEG36er2p5',  '', 56, 0, 55)
h_event_per_bx_L1_SingleTau120er2p1 = TH1D('h_event_per_bx_L1_SingleTau120er2p1',  '', 56, 0, 55)
h_event_per_bx_L1_SingleJet180er2p5 = TH1D('h_event_per_bx_L1_SingleJet180er2p5',  '', 56, 0, 55)
h_event_per_bx_L1_ETMHF70 = TH1D('h_event_per_bx_L1_ETMHF70',  '', 56, 0, 55)
h_event_per_bx_L1_ETMHF80 = TH1D('h_event_per_bx_L1_ETMHF80',  '', 56, 0, 55)
h_event_per_bx_L1_ETMHF90 = TH1D('h_event_per_bx_L1_ETMHF90',  '', 56, 0, 55)
h_event_per_bx_L1_ETMHF100 = TH1D('h_event_per_bx_L1_ETMHF100',  '', 56, 0, 55)
h_event_per_bx_L1_DoubleIsoTau30er2p1 = TH1D('h_event_per_bx_L1_DoubleIsoTau30er2p1',  '', 56, 0, 55)
h_event_per_bx_L1_DoubleLooseIsoEG22er2p1 = TH1D('h_event_per_bx_L1_DoubleLooseIsoEG22er2p1',  '', 56, 0, 55)
h_ratio = TH1D('h_ratio',  '', 56, 0, 55)
h_ratio_MET = TH1D('h_ratio_MET',  '', 56, 0, 55)


## ================================================

# fill scheme 2400b
mod = [114, 169, 224, 279, 334, 413, 468, 523, 578, 633,712, 767, 822, 877, 932, 1030,1085,1140,1195,1250, 1329,1384,1439,1494,1549, 1628,1683,1738,1793,1848, 1927,1982,2037,2092,2147, 2226,2281,2336,2391,2446, 2787,2842,2897,2952,3007, 3086,3141,3196,3251,3306]

for jEvt in range(evt_tree.GetEntries()):
    if MAX_EVT > 0 and jEvt > MAX_EVT: break
    if jEvt % PRT_EVT == 0: print ('Event #', jEvt)

    evt_tree.GetEntry(jEvt)
    ugt_tree.GetEntry(jEvt)
    
    bx_mod = evt_tree.Event.bx
    if bx_mod < 113 or bx_mod > 3356: continue
    elif bx_mod < mod[0]+51: bx_mod = bx_mod - mod[0]
    elif bx_mod < mod[1]+51: bx_mod = bx_mod - mod[1]
    elif bx_mod < mod[2]+51: bx_mod = bx_mod - mod[2]
    elif bx_mod < mod[3]+51: bx_mod = bx_mod - mod[3]
    elif bx_mod < mod[4]+51: bx_mod = bx_mod - mod[4]
    elif bx_mod < mod[5]+51: bx_mod = bx_mod - mod[5]
    elif bx_mod < mod[6]+51: bx_mod = bx_mod - mod[6]
    elif bx_mod < mod[7]+51: bx_mod = bx_mod - mod[7]
    elif bx_mod < mod[8]+51: bx_mod = bx_mod - mod[8]
    elif bx_mod < mod[9]+51: bx_mod = bx_mod - mod[9]
    elif bx_mod < mod[10]+51: bx_mod = bx_mod - mod[10]
    elif bx_mod < mod[11]+51: bx_mod = bx_mod - mod[11]
    elif bx_mod < mod[12]+51: bx_mod = bx_mod - mod[12]
    elif bx_mod < mod[13]+51: bx_mod = bx_mod - mod[13]
    elif bx_mod <mod[14]+51: bx_mod = bx_mod - mod[14]
    elif bx_mod <mod[15]+51: bx_mod = bx_mod - mod[15]
    elif bx_mod <mod[16]+51: bx_mod = bx_mod - mod[16]
    elif bx_mod <mod[17]+51: bx_mod = bx_mod - mod[17]
    elif bx_mod <mod[18]+51: bx_mod = bx_mod - mod[18]
    elif bx_mod <mod[19]+51: bx_mod = bx_mod - mod[19]
    elif bx_mod <mod[20]+51: bx_mod = bx_mod - mod[20]
    elif bx_mod <mod[21]+51: bx_mod = bx_mod - mod[21]
    elif bx_mod <mod[22]+51: bx_mod = bx_mod - mod[22]
    elif bx_mod <mod[23]+51: bx_mod = bx_mod - mod[23]
    elif bx_mod <mod[24]+51: bx_mod = bx_mod - mod[24]
    elif bx_mod <mod[25]+51: bx_mod = bx_mod - mod[25]
    elif bx_mod <mod[26]+51: bx_mod = bx_mod - mod[26]
    elif bx_mod <mod[27]+51: bx_mod = bx_mod - mod[27]
    elif bx_mod <mod[28]+51: bx_mod = bx_mod - mod[28]
    elif bx_mod <mod[29]+51: bx_mod = bx_mod - mod[29]
    elif bx_mod <mod[30]+51: bx_mod = bx_mod - mod[30]
    elif bx_mod <mod[31]+51: bx_mod = bx_mod - mod[31]
    elif bx_mod <mod[32]+51: bx_mod = bx_mod - mod[32]
    elif bx_mod <mod[33]+51: bx_mod = bx_mod - mod[33]
    elif bx_mod <mod[34]+51: bx_mod = bx_mod - mod[34]
    elif bx_mod <mod[35]+51: bx_mod = bx_mod - mod[35]
    elif bx_mod <mod[36]+51: bx_mod = bx_mod - mod[36]
    elif bx_mod <mod[37]+51: bx_mod = bx_mod - mod[37]
    elif bx_mod <mod[38]+51: bx_mod = bx_mod - mod[38]
    elif bx_mod <mod[39]+51: bx_mod = bx_mod - mod[39]
    elif bx_mod <mod[40]+51: bx_mod = bx_mod - mod[40]
    elif bx_mod <mod[41]+51: bx_mod = bx_mod - mod[41]
    elif bx_mod <mod[42]+51: bx_mod = bx_mod - mod[42]
    elif bx_mod <mod[43]+51: bx_mod = bx_mod - mod[43]
    elif bx_mod <mod[44]+51: bx_mod = bx_mod - mod[44]
    elif bx_mod <mod[45]+51: bx_mod = bx_mod - mod[45]
    elif bx_mod <mod[46]+51: bx_mod = bx_mod - mod[46]
    elif bx_mod <mod[47]+51: bx_mod = bx_mod - mod[47]
    elif bx_mod <mod[48]+51: bx_mod = bx_mod - mod[48]
    else: continue



    if evt_tree.Event.lumi < 35 or evt_tree.Event.lumi >= 85: continue
    h_event_per_bx.Fill(bx_mod+2)

    if ugt_tree.L1uGT.getAlgoDecisionInitial(21):
        h_event_per_bx_L1_SingleMu22.Fill(bx_mod+2)
    if ugt_tree.L1uGT.getAlgoDecisionInitial(168):
        h_event_per_bx_L1_SingleEG36er2p5.Fill(bx_mod+2)
    if ugt_tree.L1uGT.getAlgoDecisionInitial(264):
        h_event_per_bx_L1_SingleTau120er2p1.Fill(bx_mod+2)
    if ugt_tree.L1uGT.getAlgoDecisionInitial(322):
        h_event_per_bx_L1_SingleJet180er2p5.Fill(bx_mod+2)
    if ugt_tree.L1uGT.getAlgoDecisionInitial(418):
        h_event_per_bx_L1_ETMHF70.Fill(bx_mod+2)
    if ugt_tree.L1uGT.getAlgoDecisionInitial(419):
        h_event_per_bx_L1_ETMHF80.Fill(bx_mod+2)
    if ugt_tree.L1uGT.getAlgoDecisionInitial(420):
        h_event_per_bx_L1_ETMHF90.Fill(bx_mod+2)
    if ugt_tree.L1uGT.getAlgoDecisionInitial(421):
        h_event_per_bx_L1_ETMHF100.Fill(bx_mod+2)
    if ugt_tree.L1uGT.getAlgoDecisionInitial(269):
        h_event_per_bx_L1_DoubleIsoTau30er2p1.Fill(bx_mod+2)
    if ugt_tree.L1uGT.getAlgoDecisionInitial(230):
        h_event_per_bx_L1_DoubleLooseIsoEG22er2p1.Fill(bx_mod+2)


############################################################
###  Write output file with histograms and efficiencies  ###
############################################################

h_ratio = h_event_per_bx.Clone('h_ratio')
h_ratio.Divide(h_event_per_bx_L1_SingleMu22)

h_ratio_MET = h_event_per_bx_L1_ETMHF80.Clone('h_ratio_MET')
h_ratio_MET.Divide(h_event_per_bx_L1_SingleMu22)

h_event_per_bx.SetLineColor(kBlack)
h_event_per_bx.SetTitle('')
h_event_per_bx.SetMarkerColor(kBlack)
h_event_per_bx.SetLineWidth(1)
h_event_per_bx.SetMarkerStyle(20)
h_event_per_bx.SetMarkerSize(1)
h_event_per_bx.GetXaxis().SetTitle("Global BX (mapped to 0-55)")
h_event_per_bx.GetYaxis().SetTitle("# of L1As")
h_event_per_bx.GetYaxis().SetTitleOffset(1.45)
h_event_per_bx.GetXaxis().SetNdivisions(509)
h_event_per_bx.GetYaxis().SetNdivisions(514)
# h_event_per_bx.GetXaxis().SetRangeUser(0,3700)

h_event_per_bx_L1_SingleMu22.SetLineColor(kBlack)
h_event_per_bx_L1_SingleMu22.SetTitle('')
h_event_per_bx_L1_SingleMu22.SetMarkerColor(kBlack)
h_event_per_bx_L1_SingleMu22.SetLineWidth(1)
h_event_per_bx_L1_SingleMu22.SetMarkerStyle(20)
h_event_per_bx_L1_SingleMu22.SetMarkerSize(1)
h_event_per_bx_L1_SingleMu22.GetXaxis().SetTitle("Global BX (mapped to 0-55)")
h_event_per_bx_L1_SingleMu22.GetYaxis().SetTitle("# of SingleMu22")
h_event_per_bx_L1_SingleMu22.GetYaxis().SetTitleOffset(1.35)
h_event_per_bx_L1_SingleMu22.GetXaxis().SetNdivisions(509)
h_event_per_bx_L1_SingleMu22.GetYaxis().SetNdivisions(514)
# h_event_per_bx_L1_SingleMu22.GetXaxis().SetRangeUser(0,3700)

h_event_per_bx_L1_SingleEG36er2p5.SetLineColor(kBlack)
h_event_per_bx_L1_SingleEG36er2p5.SetTitle('')
h_event_per_bx_L1_SingleEG36er2p5.SetMarkerColor(kBlack)
h_event_per_bx_L1_SingleEG36er2p5.SetLineWidth(1)
h_event_per_bx_L1_SingleEG36er2p5.SetMarkerStyle(20)
h_event_per_bx_L1_SingleEG36er2p5.SetMarkerSize(1)
h_event_per_bx_L1_SingleEG36er2p5.GetXaxis().SetTitle("Global BX (mapped to 0-55)")
h_event_per_bx_L1_SingleEG36er2p5.GetYaxis().SetTitle("# of SingleEG36er2p5")
h_event_per_bx_L1_SingleEG36er2p5.GetYaxis().SetTitleOffset(1.35)
h_event_per_bx_L1_SingleEG36er2p5.GetXaxis().SetNdivisions(509)
h_event_per_bx_L1_SingleEG36er2p5.GetYaxis().SetNdivisions(514)
# h_event_per_bx_L1_SingleEG36er2p5.GetXaxis().SetRangeUser(0,3700)

h_event_per_bx_L1_SingleTau120er2p1.SetLineColor(kBlack)
h_event_per_bx_L1_SingleTau120er2p1.SetTitle('')
h_event_per_bx_L1_SingleTau120er2p1.SetMarkerColor(kBlack)
h_event_per_bx_L1_SingleTau120er2p1.SetLineWidth(1)
h_event_per_bx_L1_SingleTau120er2p1.SetMarkerStyle(20)
h_event_per_bx_L1_SingleTau120er2p1.SetMarkerSize(1)
h_event_per_bx_L1_SingleTau120er2p1.GetXaxis().SetTitle("Global BX (mapped to 0-55)")
h_event_per_bx_L1_SingleTau120er2p1.GetYaxis().SetTitle("# of SingleTau120er2p1")
h_event_per_bx_L1_SingleTau120er2p1.GetYaxis().SetTitleOffset(1.35)
h_event_per_bx_L1_SingleTau120er2p1.GetXaxis().SetNdivisions(509)
h_event_per_bx_L1_SingleTau120er2p1.GetYaxis().SetNdivisions(514)
# h_event_per_bx_L1_SingleTau120er2p1.GetXaxis().SetRangeUser(0,3700)

h_event_per_bx_L1_SingleJet180er2p5.SetLineColor(kBlack)
h_event_per_bx_L1_SingleJet180er2p5.SetTitle('')
h_event_per_bx_L1_SingleJet180er2p5.SetMarkerColor(kBlack)
h_event_per_bx_L1_SingleJet180er2p5.SetLineWidth(1)
h_event_per_bx_L1_SingleJet180er2p5.SetMarkerStyle(20)
h_event_per_bx_L1_SingleJet180er2p5.SetMarkerSize(1)
h_event_per_bx_L1_SingleJet180er2p5.GetXaxis().SetTitle("Global BX (mapped to 0-55)")
h_event_per_bx_L1_SingleJet180er2p5.GetYaxis().SetTitle("# of SingleJet180er2p5")
h_event_per_bx_L1_SingleJet180er2p5.GetYaxis().SetTitleOffset(1.35)
h_event_per_bx_L1_SingleJet180er2p5.GetXaxis().SetNdivisions(509)
h_event_per_bx_L1_SingleJet180er2p5.GetYaxis().SetNdivisions(514)
# h_event_per_bx_L1_SingleJet180er2p5.GetXaxis().SetRangeUser(0,3700)

h_event_per_bx_L1_ETMHF70.SetLineColor(kBlack)
h_event_per_bx_L1_ETMHF70.SetTitle('')
h_event_per_bx_L1_ETMHF70.SetMarkerColor(kBlack)
h_event_per_bx_L1_ETMHF70.SetLineWidth(1)
h_event_per_bx_L1_ETMHF70.SetMarkerStyle(20)
h_event_per_bx_L1_ETMHF70.SetMarkerSize(1)
h_event_per_bx_L1_ETMHF70.GetXaxis().SetTitle("Global BX (mapped to 0-55)")
h_event_per_bx_L1_ETMHF70.GetYaxis().SetTitle("# of ETMHF70")
h_event_per_bx_L1_ETMHF70.GetYaxis().SetTitleOffset(1.35)
h_event_per_bx_L1_ETMHF70.GetXaxis().SetNdivisions(509)
h_event_per_bx_L1_ETMHF70.GetYaxis().SetNdivisions(514)
# h_event_per_bx_L1_ETMHF70.GetXaxis().SetRangeUser(0,3700)

h_event_per_bx_L1_ETMHF80.SetLineColor(kBlack)
h_event_per_bx_L1_ETMHF80.SetTitle('')
h_event_per_bx_L1_ETMHF80.SetMarkerColor(kBlack)
h_event_per_bx_L1_ETMHF80.SetLineWidth(1)
h_event_per_bx_L1_ETMHF80.SetMarkerStyle(20)
h_event_per_bx_L1_ETMHF80.SetMarkerSize(1)
h_event_per_bx_L1_ETMHF80.GetXaxis().SetTitle("Global BX (mapped to 0-55)")
h_event_per_bx_L1_ETMHF80.GetYaxis().SetTitle("# of ETMHF80")
h_event_per_bx_L1_ETMHF80.GetYaxis().SetTitleOffset(1.35)
h_event_per_bx_L1_ETMHF80.GetXaxis().SetNdivisions(509)
h_event_per_bx_L1_ETMHF80.GetYaxis().SetNdivisions(514)
# h_event_per_bx_L1_ETMHF80.GetXaxis().SetRangeUser(0,3700)

h_event_per_bx_L1_ETMHF90.SetLineColor(kBlack)
h_event_per_bx_L1_ETMHF90.SetTitle('')
h_event_per_bx_L1_ETMHF90.SetMarkerColor(kBlack)
h_event_per_bx_L1_ETMHF90.SetLineWidth(1)
h_event_per_bx_L1_ETMHF90.SetMarkerStyle(20)
h_event_per_bx_L1_ETMHF90.SetMarkerSize(1)
h_event_per_bx_L1_ETMHF90.GetXaxis().SetTitle("Global BX (mapped to 0-55)")
h_event_per_bx_L1_ETMHF90.GetYaxis().SetTitle("# of ETMHF90")
h_event_per_bx_L1_ETMHF90.GetYaxis().SetTitleOffset(1.35)
h_event_per_bx_L1_ETMHF90.GetXaxis().SetNdivisions(509)
h_event_per_bx_L1_ETMHF90.GetYaxis().SetNdivisions(514)
# h_event_per_bx_L1_ETMHF90.GetXaxis().SetRangeUser(0,3700)

h_event_per_bx_L1_ETMHF100.SetLineColor(kBlack)
h_event_per_bx_L1_ETMHF100.SetTitle('')
h_event_per_bx_L1_ETMHF100.SetMarkerColor(kBlack)
h_event_per_bx_L1_ETMHF100.SetLineWidth(1)
h_event_per_bx_L1_ETMHF100.SetMarkerStyle(20)
h_event_per_bx_L1_ETMHF100.SetMarkerSize(1)
h_event_per_bx_L1_ETMHF100.GetXaxis().SetTitle("Global BX (mapped to 0-55)")
h_event_per_bx_L1_ETMHF100.GetYaxis().SetTitle("# of ETMHF100")
h_event_per_bx_L1_ETMHF100.GetYaxis().SetTitleOffset(1.35)
h_event_per_bx_L1_ETMHF100.GetXaxis().SetNdivisions(509)
h_event_per_bx_L1_ETMHF100.GetYaxis().SetNdivisions(514)
# h_event_per_bx_L1_ETMHF100.GetXaxis().SetRangeUser(0,3700)

h_event_per_bx_L1_DoubleIsoTau30er2p1.SetLineColor(kBlack)
h_event_per_bx_L1_DoubleIsoTau30er2p1.SetTitle('')
h_event_per_bx_L1_DoubleIsoTau30er2p1.SetMarkerColor(kBlack)
h_event_per_bx_L1_DoubleIsoTau30er2p1.SetLineWidth(1)
h_event_per_bx_L1_DoubleIsoTau30er2p1.SetMarkerStyle(20)
h_event_per_bx_L1_DoubleIsoTau30er2p1.SetMarkerSize(1)
h_event_per_bx_L1_DoubleIsoTau30er2p1.GetXaxis().SetTitle("Global BX (mapped to 0-55)")
h_event_per_bx_L1_DoubleIsoTau30er2p1.GetYaxis().SetTitle("# of DoubleIsoTau30er2p1")
h_event_per_bx_L1_DoubleIsoTau30er2p1.GetYaxis().SetTitleOffset(1.35)
h_event_per_bx_L1_DoubleIsoTau30er2p1.GetXaxis().SetNdivisions(509)
h_event_per_bx_L1_DoubleIsoTau30er2p1.GetYaxis().SetNdivisions(514)
# h_event_per_bx_L1_DoubleIsoTau30er2p1.GetXaxis().SetRangeUser(0,3700)

h_event_per_bx_L1_DoubleLooseIsoEG22er2p1.SetLineColor(kBlack)
h_event_per_bx_L1_DoubleLooseIsoEG22er2p1.SetTitle('')
h_event_per_bx_L1_DoubleLooseIsoEG22er2p1.SetMarkerColor(kBlack)
h_event_per_bx_L1_DoubleLooseIsoEG22er2p1.SetLineWidth(1)
h_event_per_bx_L1_DoubleLooseIsoEG22er2p1.SetMarkerStyle(20)
h_event_per_bx_L1_DoubleLooseIsoEG22er2p1.SetMarkerSize(1)
h_event_per_bx_L1_DoubleLooseIsoEG22er2p1.GetXaxis().SetTitle("Global BX (mapped to 0-55)")
h_event_per_bx_L1_DoubleLooseIsoEG22er2p1.GetYaxis().SetTitle("# of DoubleLooseIsoEG22er2p1")
h_event_per_bx_L1_DoubleLooseIsoEG22er2p1.GetYaxis().SetTitleOffset(1.35)
h_event_per_bx_L1_DoubleLooseIsoEG22er2p1.GetXaxis().SetNdivisions(509)
h_event_per_bx_L1_DoubleLooseIsoEG22er2p1.GetYaxis().SetNdivisions(514)
# h_event_per_bx_L1_DoubleLooseIsoEG22er2p1.GetXaxis().SetRangeUser(0,3700)

h_ratio.SetLineColor(kBlack)
h_ratio.SetTitle('')
h_ratio.SetMarkerColor(kBlack)
h_ratio.SetLineWidth(1)
h_ratio.SetMarkerStyle(20)
h_ratio.SetMarkerSize(1)
h_ratio.GetXaxis().SetTitle("Global BX (mapped to 0-55)")
h_ratio.GetYaxis().SetTitle("Ratio(L1A/SingleMu22)")
h_ratio.GetYaxis().SetTitleOffset(1.45)
h_ratio.GetXaxis().SetNdivisions(509)
h_ratio.GetYaxis().SetNdivisions(514)
h_ratio.GetYaxis().SetRangeUser(0,20)

h_ratio_MET.SetLineColor(kBlack)
h_ratio_MET.SetTitle('')
h_ratio_MET.SetMarkerColor(kBlack)
h_ratio_MET.SetLineWidth(1)
h_ratio_MET.SetMarkerStyle(20)
h_ratio_MET.SetMarkerSize(1)
h_ratio_MET.GetXaxis().SetTitle("Global BX (mapped to 0-55)")
h_ratio_MET.GetYaxis().SetTitle("Ratio(ETMHF80/SingleMu22)")
h_ratio_MET.GetYaxis().SetTitleOffset(1.45)
h_ratio_MET.GetXaxis().SetNdivisions(509)
h_ratio_MET.GetYaxis().SetNdivisions(514)
h_ratio.GetYaxis().SetRangeUser(0,20)

c_pt  = TCanvas('c_pt', '', 1280, 1024)

gStyle.SetOptStat(0)

c_pt.cd()
h_event_per_bx.Draw("pe")

cms_label = TLatex()
cms_label.SetTextSize(0.04)
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")

header = TLatex()
header.SetTextSize(0.03)
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")

c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_0_55.pdf')



h_event_per_bx_L1_SingleMu22.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_L1_SingleMu22_0_55.pdf')

h_event_per_bx_L1_SingleEG36er2p5.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_L1_SingleEG36er2p5_0_55.pdf')

h_event_per_bx_L1_SingleTau120er2p1.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_L1_SingleTau120er2p1_0_55.pdf')

h_event_per_bx_L1_SingleJet180er2p5.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_L1_SingleJet180er2p5_0_55.pdf')

h_event_per_bx_L1_ETMHF70.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_L1_ETMHF70_0_55.pdf')

h_event_per_bx_L1_ETMHF80.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_L1_ETMHF80_0_55.pdf')

h_event_per_bx_L1_ETMHF90.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_L1_ETMHF90_0_55.pdf')

h_event_per_bx_L1_ETMHF100.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_L1_ETMHF100_0_55.pdf')

h_event_per_bx_L1_DoubleIsoTau30er2p1.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_L1_DoubleIsoTau30er2p1_0_55.pdf')

h_event_per_bx_L1_DoubleLooseIsoEG22er2p1.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_L1_DoubleLooseIsoEG22er2p1_0_55.pdf')

h_ratio.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_ratio_0_55.pdf')

h_ratio_MET.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_ratio_MET_0_55.pdf')


gPad.SetLogy()

h_event_per_bx.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")

header = TLatex()
header.SetTextSize(0.03)
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_0_55_log.pdf')



h_event_per_bx_L1_SingleMu22.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_L1_SingleMu22_0_55_log.pdf')

h_event_per_bx_L1_SingleEG36er2p5.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_L1_SingleEG36er2p5_0_55_log.pdf')

h_event_per_bx_L1_SingleTau120er2p1.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_L1_SingleTau120er2p1_0_55_log.pdf')

h_event_per_bx_L1_SingleJet180er2p5.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_L1_SingleJet180er2p5_0_55_log.pdf')

h_event_per_bx_L1_ETMHF70.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_L1_ETMHF70_0_55_log.pdf')

h_event_per_bx_L1_ETMHF80.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_L1_ETMHF80_0_55_log.pdf')

h_event_per_bx_L1_ETMHF90.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_L1_ETMHF90_0_55_log.pdf')

h_event_per_bx_L1_ETMHF100.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_L1_ETMHF100_0_55_log.pdf')

h_event_per_bx_L1_DoubleIsoTau30er2p1.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_L1_DoubleIsoTau30er2p1_0_55_log.pdf')

h_event_per_bx_L1_DoubleLooseIsoEG22er2p1.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_L1_DoubleLooseIsoEG22er2p1_0_55_log.pdf')

h_ratio.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_ratio_0_55_log.pdf')

h_ratio_MET.Draw("pe")
cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")
header.DrawLatexNDC(0.62, 0.92, "#sqrt{s} = 13.6 TeV, Run 357688")
c_pt.SaveAs('plots/pdf/rate_vs_bx_new/h_event_per_bx_pe_ratio_MET_0_55_log.pdf')