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
from subprocess import Popen,PIPE
from ROOT import *
import numpy as np
from array import *
import Helper as h
from os import listdir
from os.path import isfile, join

gROOT.SetBatch(True)


print ('------> Importing Root File')

## Configuration settings
USE_EMUL = True    ## Use emulated L1T muons instead of unpacked
MAX_FILE = 20        ## Maximum number of input files (use "-1" for unlimited)
MAX_EVT  = -1       ## Maximum number of events to process
PRT_EVT  = 10000     ## Print every Nth event

REQ_BX0    = True  ## Require L1T muon to be in BX 0
REQ_uGMT   = True  ## Require a final uGMT candidate, not just a TF muon
REQ_HLT    = True  ## Require tag muon to be matched to unprescaled HLT muon
## REQ_Z      = False ## Require tag and probe muon to satisfy 81 < mass < 101 GeV (not yet implemented - AWB 25.04.2019)

MAX_dR  = 0.4   ## Maximum dR for L1T-offline matching
TAG_ISO = 0.15   ## Maximum relative isolation for tag muon
TAG_PT  = 26    ## Minimum offline pT for tag muon
PRB_PT  = 26    ## Minimum offline pT for probe muon
TRG_PT  = 22    ## Minimum L1T pT for probe muon
L1_QUAL = 12    ## Minimum L1T qual for probe muon


## L1NTuple branches
evt_tree  = TChain('l1EventTree/L1EventTree')
# reco_tree = TChain('l1MuonRecoTree/Muon2RecoTree')
gen_tree = TChain('l1GeneratorTree/L1GenTree')
if not USE_EMUL:
    L1_tree = TChain('l1UpgradeTree/L1UpgradeTree')
    tf_tree = TChain('l1UpgradeTfMuonTree/L1UpgradeTfMuonTree')
else:
    L1_tree = TChain('l1UpgradeEmuTree/L1UpgradeTree')
    tf_tree = TChain('l1UpgradeTfMuonEmuTree/L1UpgradeTfMuonTree')


# dir1 = '/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/safarzad/2017/Collision2017-wRECO-l1t-integration-v96p2_updatedHFSF/'
# dir2 = 'SingleMuon/crab_Collision2017-wRECO-l1t-integration-v96p2_updatedHFSF__SingleMuon/170628_124037/0001/'
# run_str = '_297606'

# dir1 = '/eos/cms/store/user/treis/data/i96p0/SingleMuon/crab_20170622_184540/170622_164616/0000/'
# dir2 = ''
# run_str = '_2017B'

## Full list of 2018 SingleMuon data files: https://indico.cern.ch/event/806643/contributions/3362250
# dir1 = '/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/SingleMuon/L1TNtuple_unpackSingleMuon_Run2018_v2/220303_071836/0000/'
# dir1 = '/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/SingleMuon/L1TNtuple_muonEff_Run3_data_baseline_v2/220418_152119/'
# dir1 = '/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/DYToLL_M-50_TuneCP5_14TeV-pythia8/L1TNtuple_muonEff_Run3_baseline_v1/220417_091214/'
# dir1 = '/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/DYToLL_M-50_TuneCP5_14TeV-pythia8/L1TNtuple_muonEff_Run3_cclut_v2/220418_092816/'
# dir1 = '/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/SingleMuon/L1TNtuple_unpackSingleMuon_Run2018_v2/220303_071836/0000/'
# dir1 = '/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/SingleMuon/L1TNtuple_unpackSingleMuon_Run2018_v2/220303_071836/0000/'
dir1 = '/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/HTo2LongLivedTo4mu_combined/L1Ntuples_Run3/'
dir2 = ''
run_str = '_2018D'


## Load input files
print (dir1+dir2)
nFiles = 0

fileList = [f for f in listdir(dir1+dir2) if isfile(join(dir1+dir2, f))]

# for in_file_name in Popen(['ls', dir1+dir2], stdout=PIPE).communicate()[0].split():
for in_file_name in fileList:
    if not '.root' in in_file_name: continue
    file_name = '%s%s%s' % (dir1, dir2, in_file_name)
    nFiles   += 1
    print ('  * Loading file #%d: %s' % (nFiles, in_file_name))
    evt_tree.Add(file_name)
    gen_tree.Add(file_name)
    L1_tree.Add(file_name)
    tf_tree.Add(file_name)
    if nFiles == MAX_FILE: break


## Trigger settings
trig_WP = {}
trig_WP['SingleMu']  = [12, 13, 14, 15]
trig_WP['SingleMu7'] = [11, 12, 13, 14, 15]
trig_WP['DoubleMu']  = [8, 9, 10, 11, 12, 13, 14, 15]
trig_WP['MuOpen']    = [4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]

trig_TF = {}
trig_TF['uGMT'] = [0.00, 2.40]
trig_TF['BMTF'] = [0.00, 0.83]
trig_TF['OMTF'] = [0.83, 1.24]
trig_TF['EMTF'] = [1.24, 2.40]


l1_qual = {}
l1_qual['SingleMu']  = [12]
l1_qual['SingleMu7'] = [8]
l1_qual['DoubleMu']  = [4]
l1_qual['MuOpen']    = [0]

trg_pt = {}
trg_pt['SingleMu']  = [22]
trg_pt['SingleMu7'] = [15]
trg_pt['DoubleMu']  = [7]
trg_pt['MuOpen']    = [3]

## Choose level of probe matching

## L1Upgrade__tfMuon.tfMuon.Bx == 0
if REQ_BX0: BX0_str = '_BX0'
else: BX0_str = ''

## L1Upgrade__tfMuon matches L1Upgrade.muon (Bx, HwQual <--> Qual, HwEta <--> lEta, HwPt <--> lEt) 
if REQ_uGMT: uGMT_str = '_uGMT'
else: uGMT_str = ''

## Muon.hlt_isomu == 1 && Muon.hlt_isoDeltaR < 0.1 for probe.  And Muon.iso < 0.1 && Muon.pt < TAG_PT in denom.
if REQ_HLT: HLT_str = '_HLT'
else: HLT_str = ''

## Invariant mass of tag and probe muon pair must be between 81 and 101 GeV
## if REQ_Z: Z_str = '_Zmass'  (not yet implemented - AWB 25.04.2019) 
## else: Z_str = ''
Z_str = ''

# EMU or UNP
if USE_EMUL: EMU_str = '_EMU'
else: EMU_str = '_UNP'

## Histogram filename
out_file = TFile('plots/L1T_eff_Pt%d%s%s%s%s%s%s_displaced.root' % (TRG_PT, run_str, BX0_str, uGMT_str, HLT_str, Z_str, EMU_str), 'recreate')


## ================ Histograms ======================
scale_pt_temp = [0, 2, 3, 4, 5, 6, 7, 8, 10, 12, 14, 16, 18, 20, 22, 25, 30, 35, 45, 60, 75, 100, 140, 160, 180, 200, 250, 300, 500, 1000]
scale_pt_temp_2 = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 55, 60]
scale_pt_2  = array('f', scale_pt_temp_2)
scale_pt  = array('f', scale_pt_temp)
max_pt = scale_pt_temp[len(scale_pt_temp) - 1] - 0.01

# eta_bins = [100, -2.5, 2.5]
# phi_bins = [72, -180, 180]

eta_bins = [48, -2.4, 2.4]
phi_bins = [70, -3.5, 3.5]
dxy_bins = [101, -0.5, 100.5]

h_pt  = {}
h_pt_2  = {}
h_eta = {}
h_phi = {}
h_dxy = {}
h_pt_trg  = {}
h_pt_2_trg  = {}
h_eta_trg = {}
h_phi_trg = {}
h_eta_trg_vs_PF = {}
h_phi_trg_vs_PF = {}
h_dxy_trg = {}
for TF in trig_TF.keys():
    h_pt [TF] = TH1D('h_pt_%s' % TF,  '', len(scale_pt_temp)-1,  scale_pt)
    h_pt_2 [TF] = TH1D('h_pt_2_%s' % TF,  '', len(scale_pt_temp_2)-1,  scale_pt_2)
    h_eta[TF] = TH1D('h_eta_%s' % TF, '', eta_bins[0], eta_bins[1], eta_bins[2])
    h_phi[TF] = TH1D('h_phi_%s' % TF, '', phi_bins[0], phi_bins[1], phi_bins[2])
    h_dxy[TF] = TH1D('h_dxy_%s' % TF, '', dxy_bins[0], dxy_bins[1], dxy_bins[2])

    for WP in trig_WP.keys():
        key = '%s_%s' % (TF, WP)
        h_pt_trg [key] = TH1D('h_pt_%s' % key,  '', len(scale_pt_temp)-1,  scale_pt)
        h_pt_2_trg [key] = TH1D('h_pt_2_%s' % key,  '', len(scale_pt_temp_2)-1,  scale_pt_2)
        h_eta_trg[key] = TH1D('h_eta_%s' % key, '', eta_bins[0], eta_bins[1], eta_bins[2])
        h_phi_trg[key] = TH1D('h_phi_%s' % key, '', phi_bins[0], phi_bins[1], phi_bins[2])
        h_dxy_trg[key] = TH1D('h_dxy_%s' % key, '', dxy_bins[0], dxy_bins[1], dxy_bins[2])

        h_eta_trg_vs_PF[key] = TH2D('h_eta_%s_vs_PF' % key, '', eta_bins[0], eta_bins[1], eta_bins[2], eta_bins[0], eta_bins[1], eta_bins[2])
        h_phi_trg_vs_PF[key] = TH2D('h_phi_%s_vs_PF' % key, '', phi_bins[0], phi_bins[1], phi_bins[2], phi_bins[0], phi_bins[1], phi_bins[2])

h_nTags_vs_nProbes = TH2D('h_nTags_vs_nProbes', '', 5, -0.5, 4.5, 5, -0.5, 4.5)


## ================================================

# Loop over over events in TFile
for iEvt in range(evt_tree.GetEntries()):
    if MAX_EVT > 0 and iEvt > MAX_EVT: break
    if iEvt % PRT_EVT == 0: print ('Event #', iEvt)
    
    evt_tree.GetEntry(iEvt)
    # if not (evt_tree.Event.run == 273725 or evt_tree.Event.run == 273730):
    #     continue
    L1_tree.GetEntry(iEvt)
    tf_tree.GetEntry(iEvt)
    gen_tree.GetEntry(iEvt)

    ## uGMT muon tree
    uGMT_tree = L1_tree.L1Upgrade

    TFs = {}  ## Track-finder trees by name
    TFs['BMTF']  = tf_tree.L1UpgradeBmtfMuon
    TFs['OMTF']  = tf_tree.L1UpgradeOmtfMuon
    TFs['EMTF']  = tf_tree.L1UpgradeEmtfMuon

    ## Lists of tag and probe RECO muon indices
    iParts, iL1Tags = [], []
    ## Lists of track-finder indices matching tag and probe muons
    tfTags, tfProbes, tfTrigs = {}, {}, {}
    for iTF in ['BMTF', 'OMTF', 'EMTF']:
        tfTags[iTF], tfProbes[iTF], tfTrigs[iTF] = {}, {}, {}

    ##########################################################
    ###  Loop over RECO muons to find all valid tag muons  ###
    ##########################################################
    for iPart in range(gen_tree.Generator.nPart):
      if not (abs(gen_tree.Generator.partId[iPart]) == 13): continue

      gen_eta_st1 = gen_tree.Generator.partEtaSt1[iPart]
      gen_phi_st1 = gen_tree.Generator.partPhiSt1[iPart]

      gen_eta_st2 = gen_tree.Generator.partEtaSt2[iPart]
      gen_phi_st2 = gen_tree.Generator.partPhiSt2[iPart]

      if gen_eta_st1 < 0 and gen_eta_st2 < 0: continue

      genPt = gen_tree.Generator.PartPt[iPart]
      genEta = gen_tree.Generator.PartEta[iPart]
      genPhi = gen_tree.Generator.PartPhi[iPart]

      genParent = gen_tree.Generator.PartParent[iPart]
      genCharge = gen_tree.Generator.PartParent[iPart]
      genVx = gen_tree.Generator.PartVx[iPart]
      genVy = gen_tree.Generator.PartVy[iPart]
      genVz = gen_tree.Generator.PartVz[iPart]


      d0 = -999.0
      invPt = genCharge/genPt
      if (abs(invPt) < 1./10000):
        if (invPt < 0): invPt = -1./10000
        else: invPt = 1./10000

      R = -1.0 / (0.003 * 3.811 * invPt)
      xc = genVx - (R * sin(genPhi))
      yc = genVy + (R * cos(genPhi))
      sign = 0
      if R < 0: sign = -1
      else:     sign = 1
      d0 = R - (sign * sqrt(xc*xc + yc*yc))

      Lxy = sqrt(genVx*genVx + genVy*genVy)


      if not (abs(genParent) == 6000113): continue

      if gen_eta_st2 < 0: continue
      if gen_eta_st2 > 0.83 and gen_eta_st2 < 1.24: continue
      if abs(genVz) > 120: continue




      h_pt ['uGMT'].Fill(genPt)
      h_pt_2 ['uGMT'].Fill(genPt)
      if genPt > PRB_PT:
        h_eta['uGMT'].Fill(gen_eta_st2)
        h_phi['uGMT'].Fill(gen_phi_st2)
        h_dxy['uGMT'].Fill(d0)

      match = False
      matchMu = {}
      for iMu in range(uGMT_tree.nMuons):
        if uGMT_tree.muonEt[iMu] < TAG_PT - 4.01: continue
        trk_eta = uGMT_tree.muonEta[iMu]
        trk_phi = uGMT_tree.muonPhi[iMu]
        if h.CalcDR( trk_eta, trk_phi, gen_eta_st2, gen_phi_st2 ) > MAX_dR: continue
        matchMu[iMu] = h.CalcDR( trk_eta, trk_phi, gen_eta_st2, gen_phi_st2 )

      muId = min(matchMu, key=matchMu.get)
      print(matchId)
      if not muId in iL1Tags:
          iParts.append(iPart)
          iL1Tags.append(muId)
          match = True

      if not match: continue

      if uGMT_tree.muonEta[muId] > 0.83 and uGMT_tree.muonEta[muId] < 1.24: continue
      if uGMT_tree.muonEt[muId] < TAG_PT - 4.01: continue
      if h.CalcDR( uGMT_tree.muonEta[muId], uGMT_tree.muonPhi[muId], gen_eta_st2, gen_phi_st2 ) > MAX_dR: continue

      h_pt_trg       ['uGMT_SingleMu'].Fill(genPt)
      h_pt_2_trg     ['uGMT_SingleMu'].Fill(genPt)
      if genPt > PRB_PT: h_eta_trg      ['uGMT_SingleMu'].Fill(gen_eta_st2)
      if genPt > PRB_PT: h_phi_trg      ['uGMT_SingleMu'].Fill(gen_phi_st2)
      if genPt > PRB_PT: h_dxy_trg      ['uGMT_SingleMu'].Fill(d0)

    ## End loop: for iPart in range(reco_tree.Muon.nMuons):

## End loop: for iEvt in range(evt_tree.GetEntries()):



############################################################
###  Write output file with histograms and efficiencies  ###
############################################################

out_file.cd()

nWP = 0
for WP in trig_WP.keys():
    if not (WP == 'SingleMu'): continue 
    nWP += 1
    print ('\n***********************************')
    print ('*** %s_%d%s%s%s%s efficiency ***' % (WP, TRG_PT, BX0_str, uGMT_str, HLT_str, Z_str))
    print ('***********************************')
    for TF in trig_TF.keys():
        if not (TF == 'uGMT'): continue 
        key = '%s_%s' % (TF, WP)
        print ('%s: %.1f +/- %.1f%%' % ( TF, 100 * h_phi_trg[key].Integral() / h_phi[TF].Integral(), 
                                                (100 * h_phi_trg[key].Integral() / h_phi[TF].Integral()) * 
                                                math.sqrt(h_phi[TF].Integral()) / h_phi[TF].Integral() ))

        if nWP == 1: h_pt[TF].Write() 
        h_pt_trg[key].Write() 
        h_pt_trg[key].Divide(h_pt[TF])
        h_pt_trg[key].SetName(h_pt_trg[key].GetName()+'_eff')
        h_pt_trg[key].Write()

        if nWP == 1: h_pt_2[TF].Write() 
        h_pt_2_trg[key].Write() 
        h_pt_2_trg[key].Divide(h_pt_2[TF])
        h_pt_2_trg[key].SetName(h_pt_2_trg[key].GetName()+'_eff')
        h_pt_2_trg[key].Write()

        if nWP == 1: h_eta[TF].Write() 
        h_eta_trg[key].Write() 
        h_eta_trg[key].Divide(h_eta[TF])
        h_eta_trg[key].SetName(h_eta_trg[key].GetName()+'_eff')
        h_eta_trg[key].Write()

        if nWP == 1: h_phi[TF].Write() 
        h_phi_trg[key].Write() 
        h_phi_trg[key].Divide(h_phi[TF])
        h_phi_trg[key].SetName(h_phi_trg[key].GetName()+'_eff')
        h_phi_trg[key].Write()

        h_eta_trg_vs_PF[key].Write()
        h_phi_trg_vs_PF[key].Write()

    ## End loop: for TF in trig_TF.keys()
## End loop: for WP in trig_WP.keys()

h_nTags_vs_nProbes.Write()


del out_file