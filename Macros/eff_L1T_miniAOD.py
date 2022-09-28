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
import numpy as np
from array import *
from os import listdir
from os.path import isfile, join

def CalcDPhi( phi1, phi2 ):
  dPhi = math.acos( math.cos( phi1 - phi2 ) )
  if math.sin( phi1 - phi2 ) < 0: dPhi *= -1
  return dPhi

def CalcDR( eta1, phi1, eta2, phi2 ):
  return math.sqrt( math.pow(CalcDPhi(phi1, phi2), 2) + math.pow(eta1 - eta2, 2) )
  

gROOT.SetBatch(True)


print ('------> Importing Root File')

## Configuration settings
MAX_FILE = 200        ## Maximum number of input files (use "-1" for unlimited)
MAX_EVT  = -1       ## Maximum number of events to process
PRT_EVT  = 10000     ## Print every Nth event

REQ_BX0    = True  ## Require L1T muon to be in BX 0
REQ_HLT    = True  ## Require tag muon to be matched to unprescaled HLT muon
REQ_ISOHLT = True  ## Require tag muon to be matched to unprescaled HLT muon
REQ_Z      = False ## Require tag and probe muon to satisfy 81 < mass < 101 GeV (not yet implemented - AWB 25.04.2019)

MIN_dR_TP      = 0.4   ## Maximum dR for T&P matching
MAX_dR_L1_T    = 0.1   ## Maximum dR for L1T-offline matching
MAX_dR_L1_P    = 0.2   ## Maximum dR for L1T-offline matching
TAG_PT         = 27    ## Minimum offline pT for tag muon
L1_QUAL        = 12    ## Minimum L1 quality for Tag matching


## L1NTuple branches
tree  = TChain('L1TMuonMiniAODAnalyzer/tree')

# dir1 = '/eos/cms/store/user/eyigitba/l1t/dpg/run3/ntuples/crab/Muon/L1TMuonNtuple_Run3_Muon_data_13p6TeV_eraD_miniAODAnalysis_v2/220910_144401/0000/'
# dir1 = '/eos/cms/store/user/eyigitba/l1t/dpg/run3/ntuples/crab/Muon/L1TMuonNtuple_Run3_Muon_data_13p6TeV_eraC_miniAODAnalysis_v1/220910_144255/0000/'
# dir1 = '/eos/cms/store/user/eyigitba/l1t/dpg/run3/ntuples/crab/Muon/L1TMuonNtuple_Run3_Muon_data_13p6TeV_eraD_miniAODAnalysis_v1/220910_144343/0000/'
# dir1 = '/eos/cms/store/user/eyigitba/l1t/dpg/run3/ntuples/crab/SingleMuon/L1TMuonNtuple_Run3_Muon_data_13p6TeV_eraB_miniAODAnalysis_v1/220910_144236/0000/'
dir1 = '/eos/cms/store/user/eyigitba/l1t/dpg/run3/ntuples/crab/DYToMuMu_M-20_TuneCP5_13p6TeV-pythia8/L1TMuonNtuple_Run3_DY_MC_miniAODAnalysis_full_v1/220919_122702/0000/'
# dir1 = '/eos/cms/store/user/eyigitba/l1t/dpg/run3/ntuples/crab/SingleMuon/L1TMuonNtuple_Run3_Muon_data_13p6TeV_eraC_singleMu_miniAODAnalysis_v1/220910_144320/0000/'
# dir1 = '/eos/cms/store/user/eyigitba/l1t/dpg/run3/ntuples/crab/combined/'
run_str = 'Run2022B'


## Load input files
print (dir1)
nFiles = 0

fileList = [f for f in listdir(dir1) if isfile(join(dir1, f))]

for in_file_name in fileList:
  if not '.root' in in_file_name: continue
  file_name = '%s%s' % (dir1, in_file_name)
  nFiles   += 1
  print ('  * Loading file #%d: %s' % (nFiles, in_file_name))
  tree.Add(file_name)
  if nFiles == MAX_FILE: break


## Trigger settings
trig_WP = {}
trig_WP['SingleMu']  = [12]
trig_WP['DoubleMu']  = [8]
trig_WP['MuOpen']    = [4]

trig_TF = {}
trig_TF['uGMT'] = [0.00, 2.40]
trig_TF['BMTF'] = [0.00, 0.83]
trig_TF['OMTF'] = [0.83, 1.24]
trig_TF['EMTF'] = [1.24, 2.40]


trg_pt = {}
trg_pt['SingleMu']  = [22, 15, 7, 3]
trg_pt['DoubleMu'] = [15, 7]
trg_pt['MuOpen']    = [3]

## Choose level of probe matching

## BX == 0
if REQ_BX0: BX0_str = '_BX0'
else: BX0_str = ''

## Muon.hlt_isomu == 1 && Muon.hlt_isoDeltaR < 0.1 for probe.  And Muon.iso < 0.1 && Muon.pt < TAG_PT in denom.
## isHLTMuon == 1
if REQ_HLT: HLT_str = '_HLT'
else: HLT_str = ''

## isIsoHLTMuon == 1
if REQ_ISOHLT: HLT_str = '_ISOHLT'

## Invariant mass of tag and probe muon pair must be between 81 and 101 GeV
if REQ_Z: Z_str = '_Zmass'  #(not yet implemented - AWB 25.04.2019) 
else: Z_str = ''


## Histogram filename
out_file = TFile('/afs/cern.ch/user/e/eyigitba/L1T_ana/CMSSW_12_4_8_miniAODAnalysis/src/plots/L1T_eff_%s%s%s%s_DY_m20_eta2p4.root' % (run_str, BX0_str, HLT_str, Z_str), 'recreate')
# out_file = TFile('plots/test.root', 'recreate')


## ================ Histograms ======================
scale_pt_temp = [0, 2, 3, 4, 5, 6, 7, 8, 10, 12, 14, 16, 18, 20, 22, 25, 30, 35, 45, 60, 75, 100, 140, 160, 180, 200, 250, 300, 500, 1000]
scale_pt_temp_2 = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 55, 60]
scale_pt_2  = array('f', scale_pt_temp_2)
scale_pt  = array('f', scale_pt_temp)
max_pt = scale_pt_temp[len(scale_pt_temp) - 1] - 0.01

# eta_bins = [100, -2.5, 2.5]
# phi_bins = [72, -180, 180]

eta_bins = [50, -2.5, 2.5]
phi_bins = [70, -3.5, 3.5]

h_pt_denom  = {}
h_pt_2_denom  = {}
h_eta_denom = {}
h_phi_denom = {}
h_eta_phi_denom = {}

h_pt_num  = {}
h_pt_2_num  = {}
h_eta_num = {}
h_phi_num = {}
h_eta_phi_num = {}

for TF in trig_TF.keys():
  for WP in trig_WP.keys():
    for pt in trg_pt[WP]:
      key = TF + '_' + WP + '_' + str(pt)
      h_pt_denom [key] = TH1D('h_pt_denom_%s' % key,  '', len(scale_pt_temp)-1,  scale_pt)
      h_pt_2_denom [key] = TH1D('h_pt_2_denom_%s' % key,  '', len(scale_pt_temp_2)-1,  scale_pt_2)
      h_eta_denom[key] = TH1D('h_eta_denom_%s' % key, '', eta_bins[0], eta_bins[1], eta_bins[2])
      h_phi_denom[key] = TH1D('h_phi_denom_%s' % key, '', phi_bins[0], phi_bins[1], phi_bins[2])
      h_eta_phi_denom[key] = TH2D('h_eta_phi_denom_%s' % key, '', eta_bins[0], eta_bins[1], eta_bins[2], phi_bins[0], phi_bins[1], phi_bins[2])

      h_pt_num [key] = TH1D('h_pt_num_%s' % key,  '', len(scale_pt_temp)-1,  scale_pt)
      h_pt_2_num [key] = TH1D('h_pt_2_num_%s' % key,  '', len(scale_pt_temp_2)-1,  scale_pt_2)
      h_eta_num[key] = TH1D('h_eta_num_%s' % key, '', eta_bins[0], eta_bins[1], eta_bins[2])
      h_phi_num[key] = TH1D('h_phi_num_%s' % key, '', phi_bins[0], phi_bins[1], phi_bins[2])
      h_eta_phi_num[key] = TH2D('h_eta_phi_num_%s' % key, '', eta_bins[0], eta_bins[1], eta_bins[2], phi_bins[0], phi_bins[1], phi_bins[2])

h_nTags_vs_nProbes = TH2D('h_nTags_vs_nProbes', '', 5, -0.5, 4.5, 5, -0.5, 4.5)


## ================================================

# Loop over over events in TFile
for iEvt in range(tree.GetEntries()):
  if MAX_EVT > 0 and iEvt > MAX_EVT: break
  if iEvt % PRT_EVT == 0: print ('Event #', iEvt)

  tree.GetEntry(iEvt)

  # Require HLT muon trigger
  if tree.HLT_IsoMu27 != 1 or tree.HLT_Mu50 != 1: continue

  # good vertex
  if tree.Flag_goodVertices != 1: continue

  # need 2 reco muons for T&P
  if tree.muon_size < 2: continue

  ## Lists of tag and probe RECO muon indices
  iTags, iL1Tags, iProbes = [], [], []

  ##########################################################
  ###  Loop over RECO muons to find all valid tag muons  ###
  ##########################################################
  for iTag in range(tree.muon_size):

    ## Compute tag muon coordinates at 2nd station, require to be valid
    recoEta = tree.muon_etaAtSt2[iTag]
    recoPhi = tree.muon_phiAtSt2[iTag]
    if (recoEta < -99 or recoPhi < -99):
      recoEta = tree.muon_etaAtSt1[iTag]
      recoPhi = tree.muon_phiAtSt1[iTag]
      if (recoEta < -99 or recoPhi < -99): continue

    recoAbsEta = abs(recoEta)

    if recoAbsEta > 2.5: continue

    recoIsTight = tree.muon_PassTightID[iTag]
    recoIsLoose = tree.muon_PassLooseID[iTag]

    ## Require tag muon to pass Muon POG tight ID
    if not recoIsTight: continue
    # if not recoIsLoose: continue

    ## Require prompt muons
    recoDxy = tree.muon_dxy[iTag]
    recoDz  = tree.muon_dz[iTag]
    if abs(recoDxy) > 0.2 or abs(recoDz) > 0.5: continue

    ## Require tag muon to be matched to HLT single muon trigger
    if REQ_HLT and tree.muon_isHLTMuon[iTag] != 1:   continue
    if REQ_ISOHLT and tree.muon_isIsoHLTMuon[iTag] != 1: continue
    ## Require tag muon to pass pT and isolation cuts
    if tree.muon_pt[iTag]  < TAG_PT:  continue

    # find matching L1 muon
    for iL1 in range(tree.l1mu_size):
      if tree.l1mu_qual[iL1] < L1_QUAL: continue
      if tree.l1mu_pt[iL1]   < TAG_PT - 4.01: continue
      l1_eta = tree.l1mu_eta[iL1]
      l1_phi = tree.l1mu_phi[iL1]
      if CalcDR( l1_eta, l1_phi, recoEta, recoPhi ) > MAX_dR_L1_T: continue
      if not iTag in iTags:
        iTags.append(iTag)
        iL1Tags.append(iL1)

  ## End loop: for iTag in range(reco_tree.Muon.nMuons):

  ## Quit the event if there are no tag muons
  if len(iTags) == 0: continue

  ############################################################
  ###  Loop over RECO muons to find all valid probe muons  ###
  ############################################################

  xTags = []
  for iProbe in range(tree.muon_size):

    ## Compute tag muon coordinates at 2nd station, require to be valid
    recoEta = tree.muon_etaAtSt2[iProbe]
    recoPhi = tree.muon_phiAtSt2[iProbe]
    if (recoEta < -99 or recoPhi < -99):
      recoEta = tree.muon_etaAtSt1[iProbe]
      recoPhi = tree.muon_phiAtSt1[iProbe]
      if (recoEta < -99 or recoPhi < -99): continue

    recoAbsEta = abs(recoEta)

    if recoAbsEta > 2.5: continue

    recoIsTight = tree.muon_PassTightID[iProbe]
    recoIsLoose = tree.muon_PassLooseID[iProbe]

    ## Require probe muon to pass Muon POG tight ID
    if not recoIsTight: continue
    # if not recoIsLoose: continue

    ## Require prompt muons
    recoDxy = tree.muon_dxy[iProbe]
    recoDz  = tree.muon_dz[iProbe]
    if abs(recoDxy) > 0.2 or abs(recoDz) > 0.5: continue


    ## Try to find at least on valid tag muon for this probe
    xTag = -1
    ## Loop over tag muon candidates
    for iTag in iTags:
        ## Make sure tag and probe are not the same muon
        if iTag == iProbe: continue
        # if (iTag in xTags): continue
        ## Require tag and probe muons to be well separated
        tagEta = tree.muon_etaAtSt2[iTag]
        tagPhi = tree.muon_phiAtSt2[iTag]
        if (tagEta < -99 or tagPhi < -99):
          tagEta = tree.muon_etaAtSt1[iTag]
          tagPhi = tree.muon_phiAtSt1[iTag]
        if CalcDR( tagEta, tagPhi, recoEta, recoPhi ) < MIN_dR_TP: continue
        ## If tag passes requirements, store its index and quit the loop
        xTag = iTag
        break
    ## End loop: for iTag in iTags

    ## Valid probe only if there is a corresponding tag
    if xTag < 0: continue
    iProbes.append(iProbe)
    xTags.append(xTag)

  ## End loop: for iProbe in range(tree.muon_size):

  ## Quit the event if there are no probe muons
  if(len(iProbes) == 0): continue

  h_nTags_vs_nProbes.Fill(len(iProbes), len(iTags))

  for iProbe in iProbes:

    matched_tag = xTags[iProbes.index(iProbe)]
    matched_tag_l1 = iL1Tags[iTags.index(matched_tag)]

    # print("probe: %d" % iProbe)
    # print("tag: %d" % matched_tag)
    # print("l1 tag: %d" % matched_tag_l1)

    ## Compute tag muon coordinates at 2nd station, require to be valid
    recoEta = tree.muon_etaAtSt2[iProbe]
    recoPhi = tree.muon_phiAtSt2[iProbe]
    if (recoEta < -99 or recoPhi < -99):
      recoEta = tree.muon_etaAtSt1[iProbe]
      recoPhi = tree.muon_phiAtSt1[iProbe]
      if (recoEta < -99 or recoPhi < -99): continue

    recoAbsEta = abs(recoEta)

    if recoAbsEta > 2.5: continue

    recoIsTight = tree.muon_PassTightID[iProbe]
    recoIsLoose = tree.muon_PassLooseID[iProbe]

    ## Require probe muon to pass Muon POG tight ID
    if not recoIsTight: continue
    # if not recoIsLoose: continue

    ## Require prompt muons
    recoDxy = tree.muon_dxy[iProbe]
    recoDz  = tree.muon_dz[iProbe]
    if abs(recoDxy) > 0.2 or abs(recoDz) > 0.5: continue

    recoPt = tree.muon_pt[iProbe]
    recoCharge = tree.muon_charge[iProbe]

    for TF in trig_TF.keys():
      ## Only consider reco muons in TF eta range
      if not (recoAbsEta > trig_TF[TF][0] and recoAbsEta < trig_TF[TF][1]): continue
      for WP in trig_WP.keys():
        for pt in trg_pt[WP]:
          key = TF + '_' + WP + '_' + str(pt)
          ## Fill denominator distributions for the probe muon
          h_pt_denom[key].Fill(recoPt)
          h_pt_2_denom [key].Fill(recoPt)
          if recoPt > pt + 4:
            h_eta_denom[key].Fill(recoEta)
            h_phi_denom[key].Fill(recoPhi)
            h_eta_phi_denom[key].Fill(recoEta, recoPhi)

          filled = False
          for iL1 in range(tree.l1mu_size):
            if iL1 == matched_tag_l1: continue
            l1_eta = tree.l1mu_eta[iL1]
            l1_phi = tree.l1mu_phi[iL1]

            ## dR match L1, probe
            if CalcDR( l1_eta, l1_phi, recoEta, recoPhi ) > MAX_dR_L1_P: continue

            ## BX == 0 for L1
            if REQ_BX0 and tree.l1mu_bx[iL1] != 0: continue

            ## L1 quality cut
            if tree.l1mu_qual[iL1] < trig_WP[WP][0]: continue

            ## L1 pT cut
            if tree.l1mu_pt[iL1] < pt: continue

            ## fill the numerator histos at most once per probe muon
            if filled: continue
            filled = True
            ## Fill numerator histos
            h_pt_num[key].Fill(recoPt)
            h_pt_2_num[key].Fill(recoPt)
            if recoPt > pt + 4:
              h_eta_num[key].Fill(recoEta)
              h_phi_num[key].Fill(recoPhi)
              h_eta_phi_num[key].Fill(recoEta, recoPhi)

          ## End loop: for iL1 in range(tree.l1mu_size):
        ## End loop: for pt in trg_pt[WP]:
      ## End loop: for WP in trig_WP.keys():
    ## End loop: for TF in trig_TF.keys():
  ## End loop: for iProbe in iProbes:
## End loop: for iEvt in range(tree.GetEntries()):


############################################################
###  Write output file with histograms and efficiencies  ###
############################################################

out_file.cd()

for TF in trig_TF.keys():
  for WP in trig_WP.keys():
    for pt in trg_pt[WP]:
      key = TF + '_' + WP + '_' + str(pt)
      print ('\n***********************************')
      print ('*** %s_%s%s%s efficiency ***' % (key, BX0_str, HLT_str, Z_str))
      print ('***********************************')
      print ('%s: %.1f +/- %.1f%%' % ( TF, 100 * h_phi_num[key].Integral() / h_phi_denom[key].Integral(), 
                                              (100 * h_phi_num[key].Integral() / h_phi_denom[key].Integral()) * 
                                              math.sqrt(h_phi_num[key].Integral()) / h_phi_denom[key].Integral() ))

      h_pt_denom[key].Write()
      h_pt_num[key].Write()
      h_pt_num[key].Divide(h_pt_denom[key])
      h_pt_num[key].SetName(h_pt_num[key].GetName()+'_eff')
      h_pt_num[key].Write()

      h_pt_2_denom[key].Write()
      h_pt_2_num[key].Write()
      h_pt_2_num[key].Divide(h_pt_2_denom[key])
      h_pt_2_num[key].SetName(h_pt_2_num[key].GetName()+'_eff')
      h_pt_2_num[key].Write()

      h_eta_denom[key].Write()
      h_eta_num[key].Write()
      h_eta_num[key].Divide(h_eta_denom[key])
      h_eta_num[key].SetName(h_eta_num[key].GetName()+'_eff')
      h_eta_num[key].Write()

      h_phi_denom[key].Write()
      h_phi_num[key].Write()
      h_phi_num[key].Divide(h_phi_denom[key])
      h_phi_num[key].SetName(h_phi_num[key].GetName()+'_eff')
      h_phi_num[key].Write()

      h_eta_phi_denom[key].Write()
      h_eta_phi_num[key].Write()
      h_eta_phi_num[key].Divide(h_eta_phi_denom[key])
      h_eta_phi_num[key].SetName(h_eta_phi_num[key].GetName()+'_eff')
      h_eta_phi_num[key].Write()

    ## End loop: for pt in trg_pt[WP]:
  ## End loop: for WP in trig_WP.keys():
## End loop: for TF in trig_TF.keys():

h_nTags_vs_nProbes.Write()


del out_file
