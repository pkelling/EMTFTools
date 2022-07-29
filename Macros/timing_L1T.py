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
USE_EMUL = False    ## Use emulated L1T muons instead of unpacked
MAX_FILE = 20        ## Maximum number of input files (use "-1" for unlimited)
MAX_EVT  = -1       ## Maximum number of events to process
PRT_EVT  = 10000     ## Print every Nth event

REQ_BX0    = True  ## Require L1T muon to be in BX 0
REQ_uGMT   = True  ## Require a final uGMT candidate, not just a TF muon
REQ_HLT    = True  ## Require tag muon to be matched to unprescaled HLT muon
## REQ_Z      = False ## Require tag and probe muon to satisfy 81 < mass < 101 GeV (not yet implemented - AWB 25.04.2019)

MAX_dR  = 0.1   ## Maximum dR for L1T-offline matching
TAG_ISO = 0.15   ## Maximum relative isolation for tag muon
TAG_PT  = 26    ## Minimum offline pT for tag muon
PRB_PT  = 26    ## Minimum offline pT for probe muon
TRG_PT  = 22    ## Minimum L1T pT for probe muon
L1_QUAL = 12    ## Minimum L1T qual for probe muon


## L1NTuple branches
evt_tree  = TChain('l1EventTree/L1EventTree')
reco_tree = TChain('l1MuonRecoTree/Muon2RecoTree')
if not USE_EMUL:
    L1_tree = TChain('l1UpgradeTree/L1UpgradeTree')
    tf_tree = TChain('l1UpgradeTfMuonTree/L1UpgradeTfMuonTree')
else:
    L1_tree = TChain('l1UpgradeEmuTree/L1UpgradeTree')
    tf_tree = TChain('l1UpgradeTfMuonEmuTree/L1UpgradeTfMuonTree')

met_filter_tree = TChain('l1MetFilterRecoTree/MetFilterRecoTree')
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
# dir1 = '/eos/cms/store/user/eyigitba/l1t/dpg/run3/L1TNtuples/jamboree13p6TeV/SingleMuon/run355380/'
# dir1 = '/eos/cms/store/user/eyigitba/l1t/dpg/run3/L1TNtuples/jamboree13p6TeV/ZB/run355207_run355208/'
# dir1 = '/eos/cms/store/user/eyigitba/l1t/dpg/run3/L1TNtuples/jamboree13p6TeV/EG/run355381/'
dir1 = '/eos/cms/store/user/eyigitba/l1t/dpg/run3/L1TNtuples/jamboree13p6TeV/JetHT/run355382_run355383/'

dir2 = ''
run_str = '_2022B'


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
    reco_tree.Add(file_name)
    L1_tree.Add(file_name)
    tf_tree.Add(file_name)
    met_filter_tree.Add(file_name)
    if nFiles == MAX_FILE: break


## Trigger settings
trig_TF = {}
trig_TF['uGMT'] = [0.00, 2.40]
trig_TF['BMTF'] = [0.00, 0.83]
trig_TF['OMTF'] = [0.83, 1.24]
trig_TF['EMTF'] = [1.24, 2.40]


l1_qual = {}
l1_qual['SingleMu']  = [12]
l1_qual['SingleMu7'] = [8]
l1_qual['DoubleMu']  = [8]
l1_qual['MuOpen']    = [4]


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
out_file = TFile('plots/L1T_eff_Pt%d%s%s%s%s%s%s_JetHT_355382_355383_timing_5GeV.root' % (TRG_PT, run_str, BX0_str, uGMT_str, HLT_str, Z_str, EMU_str), 'recreate')


## ================ Histograms ======================

h_bx_trg = {}
h_bx_trg_qual = {}
for TF in trig_TF.keys():
    h_bx_trg [TF] = TH1D('bx_trg_%s' % TF,  '', 7, -3.5, 3.5)

    for qual in l1_qual.keys():
        key = '%s_%s' % (TF, qual)
        h_bx_trg_qual [key] = TH1D('bx_trg_qual_%s' % key,  '', 7, -3.5, 3.5)



## ================================================
print('Processing %s events' % (evt_tree.GetEntries()) )

# Loop over over events in TFile
for iEvt in range(evt_tree.GetEntries()):
    if MAX_EVT > 0 and iEvt > MAX_EVT: break
    if iEvt % PRT_EVT == 0: print ('Event #', iEvt)
    # print ('Event #', iEvt)

    evt_tree.GetEntry(iEvt)
    # if not (evt_tree.Event.run == 273725 or evt_tree.Event.run == 273730):
    #     continue
    L1_tree.GetEntry(iEvt)
    tf_tree.GetEntry(iEvt)
    reco_tree.GetEntry(iEvt)
    met_filter_tree.GetEntry(iEvt)

    ## uGMT muon tree
    uGMT_tree = L1_tree.L1Upgrade
    TFs = {}  ## Track-finder trees by name
    TFs['BMTF']  = tf_tree.L1UpgradeBmtfMuon
    TFs['OMTF']  = tf_tree.L1UpgradeOmtfMuon
    TFs['EMTF']  = tf_tree.L1UpgradeEmtfMuon

    if met_filter_tree.MetFilters.goodVerticesFilter != 1: continue
    if reco_tree.Muon.nMuons < 1: continue
    ## Lists of RECO muon indices
    iProbes = []
    ## Lists of track-finder indices matching tag and probe muons
    tfProbes, tfTrigs = {}, {}
    for iTF in ['BMTF', 'OMTF', 'EMTF']:
        tfProbes[iTF], tfTrigs[iTF] = {}, {}

    ############################################################
    ###  Loop over RECO muons to find all valid offline muons  ###
    ############################################################

    for iProbe in range(reco_tree.Muon.nMuons):

        ## Compute muon coordinates at 2nd station, require to be valid
        recoEta = reco_tree.Muon.etaSt2[iProbe]
        recoPhi = reco_tree.Muon.phiSt2[iProbe]
        if (recoEta < -99 or recoPhi < -99):
          recoEta = reco_tree.Muon.etaSt1[iProbe]
          recoPhi = reco_tree.Muon.phiSt1[iProbe]
        if (recoEta < -99 or recoPhi < -99): continue


        recoPhiDeg = recoPhi * 180. / 3.14159
        recoAbsEta = abs(recoEta)
        recoPt = reco_tree.Muon.pt[iProbe]

        recoIsTight = np.array([-99], dtype=bool)
        recoIsTight = reco_tree.Muon.isTightMuon[iProbe]

        recoIsMed = np.array([-99], dtype=bool)
        recoIsMed = reco_tree.Muon.isMediumMuon[iProbe]

        recoIsLoose = np.array([-99], dtype=bool)
        recoIsLoose = reco_tree.Muon.isLooseMuon[iProbe]
        if recoAbsEta > 2.4: continue

        ## Require probe muon to pass Muon POG medium ID
        if not recoIsTight:   continue
        # if not recoIsMed:   continue
        # if not recoIsLoose:   continue
        ## Require probe muon to pass minimum pT cut
        if recoPt < 8: continue

        for jTrk in range(uGMT_tree.nMuons):
          trk_eta = uGMT_tree.muonEta[jTrk]
          trk_phi = uGMT_tree.muonPhi[jTrk]

          if h.CalcDR( trk_eta, trk_phi, recoEta, recoPhi ) > 2*MAX_dR: continue
          if uGMT_tree.muonEt[jTrk] < 5: continue

          for TF in trig_TF.keys():
            if recoAbsEta > trig_TF[TF][0] and recoAbsEta < trig_TF[TF][1]:
              h_bx_trg[TF].Fill(uGMT_tree.muonBx[jTrk])

              for qual in l1_qual.keys():
                if uGMT_tree.muonQual[jTrk] >= l1_qual[qual][0]:
                  h_bx_trg_qual['%s_%s' % (TF, qual)].Fill(uGMT_tree.muonBx[jTrk])



    ## End loop: for iProbe in range(reco_tree.Muon.nMuons):


## End loop: for iEvt in range(evt_tree.GetEntries()):



############################################################
###  Write output file with histograms and efficiencies  ###
############################################################

out_file.cd()

for TF in trig_TF.keys():
  h_bx_trg[TF].Write()
  for qual in l1_qual.keys():
    key = '%s_%s' % (TF, qual)
    h_bx_trg_qual[key].Write()


del out_file