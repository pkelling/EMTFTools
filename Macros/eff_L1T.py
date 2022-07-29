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
MAX_FILE = 200        ## Maximum number of input files (use "-1" for unlimited)
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
dir1 = '/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/SingleMuon/L1TNtuple_Run3_SingleMuon_data_13p6TeV_355872_v1/220725_163256/0000/'
# dir1 = '/eos/cms/store/user/eyigitba/l1t/dpg/run3/L1TNtuples/jamboree13p6TeV/ZB/all/'

dir2 = ''
run_str = '_2022C'


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
l1_qual['DoubleMu']  = [8]
l1_qual['MuOpen']    = [4]

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
out_file = TFile('plots/L1T_eff_Pt%d%s%s%s%s%s%s_SingleMuon_355872.root' % (TRG_PT, run_str, BX0_str, uGMT_str, HLT_str, Z_str, EMU_str), 'recreate')


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

h_pt  = {}
h_pt_2  = {}
h_eta = {}
h_phi = {}
h_pt_trg  = {}
h_pt_2_trg  = {}
h_eta_trg = {}
h_phi_trg = {}
h_eta_trg_vs_PF = {}
h_phi_trg_vs_PF = {}
for TF in trig_TF.keys():
    h_pt [TF] = TH1D('h_pt_%s' % TF,  '', len(scale_pt_temp)-1,  scale_pt)
    h_pt_2 [TF] = TH1D('h_pt_2_%s' % TF,  '', len(scale_pt_temp_2)-1,  scale_pt_2)
    h_eta[TF] = TH1D('h_eta_%s' % TF, '', eta_bins[0], eta_bins[1], eta_bins[2])
    h_phi[TF] = TH1D('h_phi_%s' % TF, '', phi_bins[0], phi_bins[1], phi_bins[2])

    for WP in trig_WP.keys():
        key = '%s_%s' % (TF, WP)
        h_pt_trg [key] = TH1D('h_pt_%s' % key,  '', len(scale_pt_temp)-1,  scale_pt)
        h_pt_2_trg [key] = TH1D('h_pt_2_%s' % key,  '', len(scale_pt_temp_2)-1,  scale_pt_2)
        h_eta_trg[key] = TH1D('h_eta_%s' % key, '', eta_bins[0], eta_bins[1], eta_bins[2])
        h_phi_trg[key] = TH1D('h_phi_%s' % key, '', phi_bins[0], phi_bins[1], phi_bins[2])

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
    reco_tree.GetEntry(iEvt)
    met_filter_tree.GetEntry(iEvt)

    ## uGMT muon tree
    uGMT_tree = L1_tree.L1Upgrade
    TFs = {}  ## Track-finder trees by name
    TFs['BMTF']  = tf_tree.L1UpgradeBmtfMuon
    TFs['OMTF']  = tf_tree.L1UpgradeOmtfMuon
    TFs['EMTF']  = tf_tree.L1UpgradeEmtfMuon

    # if met_filter_tree.MetFilters.goodVerticesFilter != 1: continue

    if reco_tree.Muon.nMuons < 2: continue

    ## Lists of tag and probe RECO muon indices
    iTags, iL1Tags, iProbes = [], [], []
    ## Lists of track-finder indices matching tag and probe muons
    tfTags, tfProbes, tfTrigs = {}, {}, {}
    for iTF in ['BMTF', 'OMTF', 'EMTF']:
        tfTags[iTF], tfProbes[iTF], tfTrigs[iTF] = {}, {}, {}

    ##########################################################
    ###  Loop over RECO muons to find all valid tag muons  ###
    ##########################################################
    for iTag in range(reco_tree.Muon.nMuons):

        ## Compute tag muon coordinates at 2nd station, require to be valid
        recoEta = reco_tree.Muon.etaSt2[iTag]
        recoPhi = reco_tree.Muon.phiSt2[iTag]
        if (recoEta < -99 or recoPhi < -99):
          recoEta = reco_tree.Muon.etaSt1[iTag]
          recoPhi = reco_tree.Muon.phiSt1[iTag]
          if (recoEta < -99 or recoPhi < -99): continue

        recoAbsEta = abs(recoEta)

        if recoAbsEta > 2.4: continue

        recoIsTight = np.array([-99], dtype=bool)
        recoIsTight = reco_tree.Muon.isTightMuon[iTag]

        recoIsMed = np.array([-99], dtype=bool)
        recoIsMed = reco_tree.Muon.isMediumMuon[iTag]

        # recoIsLoose = np.array([-99], dtype=bool)
        # recoIsLoose = reco_tree.Muon.isLooseMuon[iTag]

        ## Require tag muon to pass Muon POG medium ID
        if not recoIsTight: continue
        # if not recoIsMed: continue
        # if not recoIsLoose: continue
        ## Require tag muon to be matched to HLT single muon trigger
        # if REQ_HLT and reco_tree.Muon.hlt_isomu[iTag]    != 1:   continue
        # if REQ_HLT and reco_tree.Muon.hlt_isoDeltaR[iTag] > 0.1: continue
        ## Require tag muon to pass pT and isolation cuts
        if reco_tree.Muon.pt[iTag]  < TAG_PT:  continue
        if reco_tree.Muon.iso[iTag] > TAG_ISO: continue

        ## Find track-finder muons which could be matched to the tag muon

        ## Loop over all tracks in each track-finder
        # for iTF in ['BMTF', 'OMTF', 'EMTF']:
        #     ## Loop over the tracks in each track-finder
        #     for iTrk in range(TFs[iTF].nTfMuons):
        #         ## Require track-finder to fire with SingleMuon quality (>= 12)
        #         if not TFs[iTF].tfMuonHwQual[iTrk] in trig_WP['SingleMu']: continue
        #         ## Require track-finder pT to be >= offline cut - 4 GeV
        #         if    (TFs[iTF].tfMuonHwPt[iTrk] - 1)*0.5 < TAG_PT - 4.01: continue
        #         ## Scaling from hardware eta to integer value
        #         trk_eta = TFs[iTF].tfMuonHwEta[iTrk]*0.010875
        #         ## Scaling from degrees to radians
        #         trk_phi = TFs[iTF].tfMuonGlobalPhi[iTrk]*3.14159/180.
        #         ## For some reason, it seems this is filled in a buggy way and needs to be rescaled - AWB 22.04.2019
        #         trk_phi *= (1.0/1.6)
        #         ## Wrap-around values > pi
        #         if (trk_phi > 3.14159): trk_phi -= 2*3.14159
        #         ## Require track with be within specified dR of RECO muon
        #         if h.CalcDR( trk_eta, trk_phi, recoEta, recoPhi ) > MAX_dR: continue

        #         ## If all requirements are met, we have an L1 track for this tag muon
        #         if not iTag in iTags:
        #             iTags.append(iTag)
        #         if not iTag in tfTags[iTF].keys():
        #             tfTags[iTF][iTag] = [iTrk]
        #         else:
        #             tfTags[iTF][iTag].append(iTrk)

        for iMu in range(uGMT_tree.nMuons):
          if not (uGMT_tree.muonQual[iMu] >= L1_QUAL): continue
          if uGMT_tree.muonEt[iMu] < TAG_PT - 4.01: continue
          trk_eta = uGMT_tree.muonEta[iMu]
          trk_phi = uGMT_tree.muonPhi[iMu]
          if h.CalcDR( trk_eta, trk_phi, recoEta, recoPhi ) > MAX_dR: continue
          if not iTag in iTags:
            iTags.append(iTag)
            iL1Tags.append(iMu)


    ## End loop: for iTag in range(reco_tree.Muon.nMuons):


    ## Quit the event if there are no tag muons
    if len(iTags) == 0: continue
    # tag_pt = 0
    # best_tag = -1
    # best_l1_tag = -1
    # for iTag in iTags:
    #   if (reco_tree.Muon.pt[iTag] > tag_pt):
    #     tag_pt = reco_tree.Muon.pt[iTag]
    #     best_tag = iTag
    #     best_l1_tag = iL1Tags[iTags.index(iTag)]

    # iTags = []
    # iTags.append(best_tag)
    # iL1Tags = []
    # iL1Tags.append(best_l1_tag)

    ############################################################
    ###  Loop over RECO muons to find all valid probe muons  ###
    ############################################################

    xTags = []
    for iProbe in range(reco_tree.Muon.nMuons):

        ## Compute tag muon coordinates at 2nd station, require to be valid
        recoEta = reco_tree.Muon.etaSt2[iProbe]
        recoPhi = reco_tree.Muon.phiSt2[iProbe]
        if (recoEta < -99 or recoPhi < -99):
          recoEta = reco_tree.Muon.etaSt1[iProbe]
          recoPhi = reco_tree.Muon.phiSt1[iProbe]
        if (recoEta < -99 or recoPhi < -99): continue


        recoPhiDeg = recoPhi * 180. / 3.14159
        recoAbsEta = abs(recoEta)
        recoPt = min(reco_tree.Muon.pt[iProbe], max_pt)

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
        # if recoPt < PRB_PT: continue

        ## Try to find at least on valid tag muon for this probe
        xTag = -1
        ## Loop over tag muon candidates
        for iTag in iTags:
            ## Make sure tag and probe are not the same muon
            if iTag == iProbe: continue
            # if (iTag in xTags): continue
            ## Require tag and probe muons to be well separated
            tagEta = reco_tree.Muon.etaSt2[iTag]
            tagPhi = reco_tree.Muon.phiSt2[iTag]
            if (tagEta < -99 or tagPhi < -99):
              tagEta = reco_tree.Muon.etaSt1[iTag]
              tagPhi = reco_tree.Muon.phiSt1[iTag]
            if h.CalcDR( tagEta, tagPhi, recoEta, recoPhi ) < 4*MAX_dR: continue
            ## If tag passes requirements, store its index and quit the loop
            xTag = iTag
            break
        ## End loop: for iTag in iTags

        ## Valid probe only if there is a corresponding tag
        if xTag < 0: continue
        else: iProbes.append(iProbe)
        xTags.append(xTag)

    # end 1st probe loop

    if(len(iProbes) < 1): continue

    h_nTags_vs_nProbes.Fill(len(iProbes), len(iTags))

    for iProbe in iProbes:


        matched_tag = xTags[iProbes.index(iProbe)]
        matched_tag_l1 = iL1Tags[iTags.index(matched_tag)]

        # print("probe: %d" % iProbe)
        # print("tag: %d" % matched_tag)
        # print("l1 tag: %d" % matched_tag_l1)
        recoEta = reco_tree.Muon.etaSt2[iProbe]
        recoPhi = reco_tree.Muon.phiSt2[iProbe]
        if (recoEta < -99 or recoPhi < -99):
          recoEta = reco_tree.Muon.etaSt1[iProbe]
          recoPhi = reco_tree.Muon.phiSt1[iProbe]
        if (recoEta < -99 or recoPhi < -99): continue


        recoPhiDeg = recoPhi * 180. / 3.14159
        recoAbsEta = abs(recoEta)
        recoPt = min(reco_tree.Muon.pt[iProbe], max_pt)
        if recoAbsEta > 2.4: continue

        recoIsTight = np.array([-99], dtype=bool)
        recoIsTight = reco_tree.Muon.isTightMuon[iProbe]

        recoIsMed = np.array([-99], dtype=bool)
        recoIsMed = reco_tree.Muon.isMediumMuon[iProbe]

        recoIsLoose = np.array([-99], dtype=bool)
        recoIsLoose = reco_tree.Muon.isLooseMuon[iProbe]

        ## Require probe muon to pass Muon POG medium ID
        if not recoIsTight:   continue
        # if not recoIsMed:   continue
        # if not recoIsLoose:   continue
        ## Require probe muon to pass minimum pT cut
        # if recoPt < PRB_PT: continue

        ## Fill denominator distributions for the probe muon in uGMT
        h_pt ['uGMT'].Fill(recoPt)
        h_pt_2 ['uGMT'].Fill(recoPt)
        if recoPt > PRB_PT:
          h_eta['uGMT'].Fill(recoEta)
          h_phi['uGMT'].Fill(recoPhi)

        ## Count number of fired triggers from each track-finder, and the uGMT
        nTrig = {}
        for iTF in ['uGMT', 'BMTF', 'OMTF', 'EMTF']:
            nTrig[iTF] = {}
            for WP in trig_WP.keys():
                nTrig[iTF][WP] = 0

        ## Mapping from L1Upgrade__tfMuon to L1Upgrade.muon : Bx <--> Bx, HwQual <--> Qual, HwEta <--> lEta, HwPt <--> lEt

        ## Loop over track-finder regions for efficiency plots
        for jTF in ['BMTF', 'OMTF', 'EMTF']:

            ## Fill denominator distribution for eta for all track-finders
            if recoPt > PRB_PT: h_eta[jTF].Fill(recoEta)
            ## For other distributions, require RECO muon to fall in correct eta range
            if recoAbsEta > trig_TF[jTF][0] and recoAbsEta < trig_TF[jTF][1]:
                h_pt [jTF].Fill(recoPt)
                h_pt_2 [jTF].Fill(recoPt)
                if recoPt > PRB_PT: h_phi[jTF].Fill(recoPhi)
            ## If muon is not in track-finder eta range, skip loop over tracks
            else: continue

            for jTrk in range(uGMT_tree.nMuons):

              if jTrk == matched_tag_l1: continue

              trk_eta = uGMT_tree.muonEta[jTrk]
              trk_phi = uGMT_tree.muonPhi[jTrk]

              if h.CalcDR( trk_eta, trk_phi, recoEta, recoPhi ) > 2*MAX_dR: continue


              if REQ_BX0 and uGMT_tree.muonBx[jTrk] != 0: continue

              for WP in trig_WP.keys():

                if not uGMT_tree.muonQual[jTrk] >= l1_qual[WP][0]: continue

                if uGMT_tree.muonEt[jTrk] < trg_pt[WP][0] - 0.01: continue

                ## Count the tracks matching this probe muon and passing this working point
                nTrig['uGMT'][WP] += 1
                nTrig   [jTF][WP] += 1

                ## Don't need to fill any plots if we already found a matching track in this track-finder
                if nTrig[jTF][WP] > 1: continue

                ## Fill numerator distribution for eta
                if recoPt > PRB_PT: h_eta_trg      ['%s_%s' % (jTF, WP)].Fill(recoEta)
                if recoPt > PRB_PT: h_eta_trg_vs_PF['%s_%s' % (jTF, WP)].Fill(recoEta, trk_eta)

                ## For the remainder of the plots, only fill them if we haven't already
                if nTrig['uGMT'][WP] > 1: continue

                h_pt_trg       ['%s_%s' % (jTF, WP)].Fill(recoPt)
                h_pt_2_trg       ['%s_%s' % (jTF, WP)].Fill(recoPt)
                if recoPt > PRB_PT: h_phi_trg      ['%s_%s' % (jTF, WP)].Fill(recoPhi)
                if recoPt > PRB_PT: h_phi_trg_vs_PF['%s_%s' % (jTF, WP)].Fill(recoPhi, trk_phi)

                h_pt_trg       ['uGMT_%s' % WP ].Fill(recoPt)
                h_pt_2_trg       ['uGMT_%s' % WP ].Fill(recoPt)
                if recoPt > PRB_PT: h_eta_trg      ['uGMT_%s' % WP ].Fill(recoEta)
                if recoPt > PRB_PT: h_phi_trg      ['uGMT_%s' % WP ].Fill(recoPhi)
                if recoPt > PRB_PT: h_eta_trg_vs_PF['uGMT_%s' % WP ].Fill(recoEta, trk_eta)
                if recoPt > PRB_PT: h_phi_trg_vs_PF['uGMT_%s' % WP ].Fill(recoPhi, trk_phi)


                    ## End loop: for WP in trig_WP.keys()
                ## End loop: for jTrk in range(TFs[jTF].nTfMuons)
            ## End loop: for jTF in ['BMTF', 'OMTF', 'EMTF']:
        ## End loop: for iTF in ['BMTF', 'OMTF', 'EMTF']:
    ## End loop: for iProbe in range(reco_tree.Muon.nMuons):


## End loop: for iEvt in range(evt_tree.GetEntries()):



############################################################
###  Write output file with histograms and efficiencies  ###
############################################################

out_file.cd()

nWP = 0
for WP in trig_WP.keys():
    nWP += 1
    print ('\n***********************************')
    print ('*** %s_%d%s%s%s%s efficiency ***' % (WP, TRG_PT, BX0_str, uGMT_str, HLT_str, Z_str))
    print ('***********************************')
    for TF in trig_TF.keys():
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