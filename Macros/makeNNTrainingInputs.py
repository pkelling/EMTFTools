#!/usr/bin/env python

import os
import sys
import numpy as np
import ROOT

ROOT.ROOT.EnableImplicitMT()

# f = ROOT.TFile.Open("/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/CRAB_PrivateMC/DisplacedMuGun_flatPt2to1000_posEndcap_flatXYZEtaPhi_11_3_0_pre5_10M/210413_105301/DisplacedMuGun_flatPt2to1000_posEndcap_flatXYZEtaPhi_11_3_0_pre5_10M.root")
f = ROOT.TFile.Open("/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/CRAB_PrivateMC/DisplacedMuGun_flatPt2to1000_negEndcap_flatXYZEtaPhi_11_3_0_pre5_10M/210413_105349/DisplacedMuGun_flatPt2to1000_negEndcap_flatXYZEtaPhi_11_3_0_pre5_10M.root")

t = f.Get("EMTFNtuple/tree")

nentries = t.GetEntries()

x_dphi = np.zeros((nentries, 6))
x_dtheta = np.zeros((nentries, 6))
x_bend_emtf = np.zeros((nentries, 4))
x_fr_emtf = np.zeros((nentries, 1))
x_track_theta = np.zeros((nentries, 1))  
x_ME11ring = np.zeros((nentries, 1)) 
x_RPCbit = np.zeros((nentries, 4)) 
x_mode = np.zeros((nentries, 1))
x_straightness = np.zeros((nentries, 1))

y_pt = np.zeros((nentries, 1))
y_q = np.zeros((nentries,1))
y_eta = np.zeros((nentries, 1))
y_phi = np.zeros((nentries, 1))
y_vx = np.zeros((nentries, 1))
y_vy = np.zeros((nentries, 1))
y_vz = np.zeros((nentries, 1))

invalid_dphi = 8191
invalid_dtheta = 127

print "Will run over %d events!!" % nentries

# y_pt = np.asarray(t.genPart_pt)

for i in range(0,nentries):
# for i in range(0,1000):
  t.GetEntry(i)
  if i % 10000 == 0:
    print "%d entries done!" % i
  if t.emtfTrack_size == 0:
    x_dphi[i,:] = np.nan
    x_dtheta[i,:] = np.nan
    x_bend_emtf[i,:] = np.nan
    x_fr_emtf[i,0] = np.nan
    x_track_theta[i,0] = np.nan
    x_ME11ring[i,0] = np.nan
    x_RPCbit[i,:] = np.nan 
    x_mode[i,0] = np.nan
    x_straightness[i,0] = np.nan

    y_pt[i,0] = np.nan
    y_q[i,0] = np.nan
    y_eta[i,0] = np.nan
    y_phi[i,0] = np.nan
    y_vx[i,0] = np.nan
    y_vy[i,0] = np.nan
    y_vz[i,0] = np.nan
    continue

  y_pt[i,0] = np.asarray(list(t.genPart_pt)[0])
  y_q[i,0] = np.asarray(list(t.genPart_q)[0])
  y_eta[i,0] = np.asarray(list(t.genPart_eta)[0])
  y_phi[i,0] = np.asarray(list(t.genPart_phi)[0])
  y_vx[i,0] = np.asarray(list(t.genPart_vx)[0])
  y_vy[i,0] = np.asarray(list(t.genPart_vy)[0])
  y_vz[i,0] = np.asarray(list(t.genPart_vz)[0])

  idx = 0

  if t.emtfTrack_size > 1:
    mode = np.asarray(list(t.emtfTrack_mode))
    mode_max = np.flatnonzero(mode == mode.max())
    
    if len(mode_max) == 2:
      if list(t.emtfTrack_ptLUT_deltaPh[mode_max[0]]).count(8191) > list(t.emtfTrack_ptLUT_deltaPh[mode_max[1]]).count(8191):
        idx = mode_max[1]
      else:
        idx = mode_max[0]

    elif len(mode_max) > 2:
      deltaPh_count = np.empty((len(mode_max), 1))
      
      for k in range(0,len(mode_max)):
        deltaPh_count[k] = list(t.emtfTrack_ptLUT_deltaPh[mode_max[k]]).count(8191)

      idx = np.argmin(deltaPh_count)

    else:
      idx = mode_max[0]



  # Calculate dPh and dTh
  dPhi_12 = list(t.emtfTrack_ptLUT_deltaPh)[idx][0] if t.emtfTrack_ptLUT_deltaPh[idx][0] != invalid_dphi else 0
  dPhi_13 = list(t.emtfTrack_ptLUT_deltaPh)[idx][1] if t.emtfTrack_ptLUT_deltaPh[idx][1] != invalid_dphi else 0
  dPhi_14 = list(t.emtfTrack_ptLUT_deltaPh)[idx][2] if t.emtfTrack_ptLUT_deltaPh[idx][2] != invalid_dphi else 0
  dPhi_23 = list(t.emtfTrack_ptLUT_deltaPh)[idx][3] if t.emtfTrack_ptLUT_deltaPh[idx][3] != invalid_dphi else 0
  dPhi_24 = list(t.emtfTrack_ptLUT_deltaPh)[idx][4] if t.emtfTrack_ptLUT_deltaPh[idx][4] != invalid_dphi else 0
  dPhi_34 = list(t.emtfTrack_ptLUT_deltaPh)[idx][5] if t.emtfTrack_ptLUT_deltaPh[idx][5] != invalid_dphi else 0

  dTh_12 = list(t.emtfTrack_ptLUT_deltaTh)[idx][0] if t.emtfTrack_ptLUT_deltaTh[idx][0] != invalid_dtheta else 0
  dTh_13 = list(t.emtfTrack_ptLUT_deltaTh)[idx][1] if t.emtfTrack_ptLUT_deltaTh[idx][1] != invalid_dtheta else 0
  dTh_14 = list(t.emtfTrack_ptLUT_deltaTh)[idx][2] if t.emtfTrack_ptLUT_deltaTh[idx][2] != invalid_dtheta else 0
  dTh_23 = list(t.emtfTrack_ptLUT_deltaTh)[idx][3] if t.emtfTrack_ptLUT_deltaTh[idx][3] != invalid_dtheta else 0
  dTh_24 = list(t.emtfTrack_ptLUT_deltaTh)[idx][4] if t.emtfTrack_ptLUT_deltaTh[idx][4] != invalid_dtheta else 0
  dTh_34 = list(t.emtfTrack_ptLUT_deltaTh)[idx][5] if t.emtfTrack_ptLUT_deltaTh[idx][5] != invalid_dtheta else 0

  sPh_12 = 1 if t.emtfTrack_ptLUT_signPh[idx][0] == 1 else -1
  sPh_13 = 1 if t.emtfTrack_ptLUT_signPh[idx][1] == 1 else -1
  sPh_14 = 1 if t.emtfTrack_ptLUT_signPh[idx][2] == 1 else -1
  sPh_23 = 1 if t.emtfTrack_ptLUT_signPh[idx][3] == 1 else -1
  sPh_24 = 1 if t.emtfTrack_ptLUT_signPh[idx][4] == 1 else -1
  sPh_34 = 1 if t.emtfTrack_ptLUT_signPh[idx][5] == 1 else -1

  sTh_12 = 1 if t.emtfTrack_ptLUT_signTh[idx][0] == 1 else -1
  sTh_13 = 1 if t.emtfTrack_ptLUT_signTh[idx][1] == 1 else -1
  sTh_14 = 1 if t.emtfTrack_ptLUT_signTh[idx][2] == 1 else -1
  sTh_23 = 1 if t.emtfTrack_ptLUT_signTh[idx][3] == 1 else -1
  sTh_24 = 1 if t.emtfTrack_ptLUT_signTh[idx][4] == 1 else -1
  sTh_34 = 1 if t.emtfTrack_ptLUT_signTh[idx][5] == 1 else -1


  x_dphi[i,0] = np.asarray(dPhi_12 * sPh_12)
  x_dphi[i,1] = np.asarray(dPhi_13 * sPh_13)
  x_dphi[i,2] = np.asarray(dPhi_14 * sPh_14)
  x_dphi[i,3] = np.asarray(dPhi_23 * sPh_23)
  x_dphi[i,4] = np.asarray(dPhi_24 * sPh_24)
  x_dphi[i,5] = np.asarray(dPhi_34 * sPh_34)


  x_dtheta[i,0] = np.asarray(dTh_12 * sTh_12)
  x_dtheta[i,1] = np.asarray(dTh_13 * sTh_13)
  x_dtheta[i,2] = np.asarray(dTh_14 * sTh_14)
  x_dtheta[i,3] = np.asarray(dTh_23 * sTh_23)
  x_dtheta[i,4] = np.asarray(dTh_24 * sTh_24)
  x_dtheta[i,5] = np.asarray(dTh_34 * sTh_34)

  # Calculate bend, FR, RPC
  st1 = (t.emtfTrack_mode[idx] >= 8)
  st2 = ((t.emtfTrack_mode[idx] % 8) >= 4)
  st3 = ((t.emtfTrack_mode[idx] % 4) >= 2)
  st4 = ((t.emtfTrack_mode[idx] % 2) == 1)

  # Fix dPh and dTh if stations don't exist
  if not st1:
    x_dphi[i,0] = np.asarray(-999)
    x_dphi[i,1] = np.asarray(-999)
    x_dphi[i,2] = np.asarray(-999)

    x_dtheta[i,0] = np.asarray(-999)
    x_dtheta[i,1] = np.asarray(-999)
    x_dtheta[i,2] = np.asarray(-999)

  if not st2:
    x_dphi[i,0] = np.asarray(-999)
    x_dphi[i,3] = np.asarray(-999)
    x_dphi[i,4] = np.asarray(-999)

    x_dtheta[i,0] = np.asarray(-999)
    x_dtheta[i,3] = np.asarray(-999)
    x_dtheta[i,4] = np.asarray(-999)

  if not st3:
    x_dphi[i,1] = np.asarray(-999)
    x_dphi[i,3] = np.asarray(-999)
    x_dphi[i,5] = np.asarray(-999)

    x_dtheta[i,1] = np.asarray(-999)
    x_dtheta[i,3] = np.asarray(-999)
    x_dtheta[i,5] = np.asarray(-999)

  if not st4:
    x_dphi[i,2] = np.asarray(-999)
    x_dphi[i,4] = np.asarray(-999)
    x_dphi[i,5] = np.asarray(-999)

    x_dtheta[i,2] = np.asarray(-999)
    x_dtheta[i,4] = np.asarray(-999)
    x_dtheta[i,5] = np.asarray(-999)


  pat1 = -99
  pat2 = -99
  pat3 = -99
  pat4 = -99

  if st1:
    pat1 = list(t.emtfTrack_ptLUT_cpattern)[idx][0]
  if st2:
    pat2 = list(t.emtfTrack_ptLUT_cpattern)[idx][1]
  if st3:
    pat3 = list(t.emtfTrack_ptLUT_cpattern)[idx][2]
  if st4:
    pat4 = list(t.emtfTrack_ptLUT_cpattern)[idx][3]

  x_fr_emtf[i,0] = np.asarray(list(t.emtfTrack_ptLUT_fr)[idx][0])

  x_RPCbit[i,0] = 1 if (st1 and pat1 == 0) else 0
  x_RPCbit[i,1] = 1 if (st2 and pat2 == 0) else 0
  x_RPCbit[i,2] = 1 if (st3 and pat3 == 0) else 0
  x_RPCbit[i,3] = 1 if (st4 and pat4 == 0) else 0

  if pat1 == -99:
    x_bend_emtf[i,0] = np.asarray(0)
  elif pat1 == 10:
    x_bend_emtf[i,0] = np.asarray(0)
  elif (pat1 % 2 == 0):
    x_bend_emtf[i,0] = np.asarray((10 - pat1)/2)
  elif (pat1 % 2 == 1):
    x_bend_emtf[i,0] = np.asarray(-1 * (11 - pat1)/2)

  if pat2 == -99:
    x_bend_emtf[i,1] = np.asarray(0)
  elif pat2 == 10:
    x_bend_emtf[i,1] = np.asarray(0)
  elif (pat2 % 2 == 0):
    x_bend_emtf[i,1] = np.asarray((10 - pat2)/2)
  elif (pat2 % 2 == 1):
    x_bend_emtf[i,1] = np.asarray(-1 * (11 - pat2)/2)

  if pat3 == -99:
    x_bend_emtf[i,2] = np.asarray(0)
  elif pat3 == 10:
    x_bend_emtf[i,2] = np.asarray(0)
  elif (pat3 % 2 == 0):
    x_bend_emtf[i,2] = np.asarray((10 - pat3)/2)
  elif (pat3 % 2 == 1):
    x_bend_emtf[i,2] = np.asarray(-1 * (11 - pat3)/2)

  if pat4 == -99:
    x_bend_emtf[i,3] = np.asarray(0)
  elif pat4 == 10:
    x_bend_emtf[i,3] = np.asarray(0)
  elif (pat4 % 2 == 0):
    x_bend_emtf[i,3] = np.asarray((10 - pat4)/2)
  elif (pat4 % 2 == 1):
    x_bend_emtf[i,3] = np.asarray(-1 * (11 - pat4)/2)

  # Reverse to match dPhi convention

  if t.emtfTrack_endcap[idx] == 1:
    x_bend_emtf[i,0] *= -1
    x_bend_emtf[i,1] *= -1
    x_bend_emtf[i,2] *= -1
    x_bend_emtf[i,3] *= -1

  # Fix RPC bend
  if x_RPCbit[i,0] == 1 and abs(x_bend_emtf[i,0]) == 5:
    x_bend_emtf[i,0] = np.asarray(0)
  if x_RPCbit[i,1] == 1 and abs(x_bend_emtf[i,1]) == 5:
    x_bend_emtf[i,1] = np.asarray(0)
  if x_RPCbit[i,2] == 1 and abs(x_bend_emtf[i,2]) == 5:
    x_bend_emtf[i,2] = np.asarray(0)
  if x_RPCbit[i,3] == 1 and abs(x_bend_emtf[i,3]) == 5:
    x_bend_emtf[i,3] = np.asarray(0)


  # Assign track theta and ME11 ring
  x_track_theta[i,0] = np.asarray(list(t.emtfTrack_theta_fp)[idx])
  x_ME11ring[i,0] = np.asarray(list(t.emtfTrack_ptLUT_st1_ring2)[idx]) 

  # track mode and straightness for training
  x_mode[i,0] = np.asarray(list(t.emtfTrack_mode)[idx])
  x_straightness[i,0] = np.asarray(list(t.emtfTrack_straightness)[idx])
  # print "features: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d" % (x_dphi[i,0], x_dphi[i,1], x_dphi[i,2], x_dphi[i,3], x_dphi[i,4], x_dphi[i,5], x_dtheta[i,0], x_dtheta[i,1], x_dtheta[i,2], x_dtheta[i,3], x_dtheta[i,4], x_dtheta[i,5], x_bend_emtf[i,0], x_bend_emtf[i,1], x_bend_emtf[i,2], x_bend_emtf[i,3], x_fr_emtf[i,0], x_track_theta[i,0], x_ME11ring[i,0], x_RPCbit[i,0], x_RPCbit[i,1], x_RPCbit[i,2], x_RPCbit[i,3] )



variables = np.hstack((x_dphi, x_dtheta, x_bend_emtf, x_fr_emtf, x_track_theta, x_ME11ring, x_RPCbit, x_mode, x_straightness))
parameters = np.hstack((np.true_divide(y_q, y_pt), y_phi, y_eta, y_vx, y_vy, y_vz))

np.savez_compressed('NN_input_params_negEndcapFlatXYZ.npz', parameters=parameters, variables=variables)

print "done!!"