/* L1T analysis package 
 * module: main module of Orthogonal dataset method 
 * Author: George Karathanasis, georgios.karathanasis@cern.ch
 *
 *
 */

#include <iostream>
#include "TH1F.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TTree.h"
#include "TChain.h"
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "TString.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
// #include "analysis.h"
// #include "CutReader.h"

float DPhi(double phi1,double phi2){
  float temp=phi1-phi2;
  if (temp>3.14) temp=temp-6.28;
  if (temp<-3.14) temp=temp+6.28;
  return temp;
}

int makeTruthComp(){

  TString file = "/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/HTo2LongLivedTo4mu_combined/EMTFNtuple_HTo2LLTo4Mu_combined_cmssw_11_0_2_fwImplementation_NNv5.root";

  // load trees
  TString tree = "EMTFNtuple/tree";


  TChain * cc=new TChain(tree);


  cc->Add(file);

  TTreeReader reader(cc);

  TTreeReaderValue<int32_t> emtfTrackSize(reader,"emtfTrack_size");
  TTreeReaderArray<float  > emtfTrackPt(reader,"emtfTrack_pt");
  TTreeReaderArray<float  > emtfTrackPtDxy(reader,"emtfTrack_pt_dxy");
  TTreeReaderArray<float  > emtfTrackDxy(reader,"emtfTrack_dxy");
  TTreeReaderArray<float  > emtfTrackPhi(reader,"emtfTrack_phi");
  TTreeReaderArray<float  > emtfTrackTheta(reader,"emtfTrack_theta");
  TTreeReaderArray<float  > emtfTrackEta(reader,"emtfTrack_eta");
  TTreeReaderArray<int    > emtfTrackGMTPhi(reader,"emtfTrack_GMT_phi");
  TTreeReaderArray<int    > emtfTrackGMTEta(reader,"emtfTrack_GMT_eta");
  TTreeReaderArray<short  > emtfTrackMode(reader,"emtfTrack_mode");
  TTreeReaderArray<short  > emtfTrackEndcap(reader,"emtfTrack_endcap");
  TTreeReaderArray<short  > emtfTrackSector(reader,"emtfTrack_sector");
  TTreeReaderArray<short  > emtfTrackBX(reader,"emtfTrack_bx");


  TTreeReaderValue<int32_t> gmtMuonSize(reader,"gmtMuon_size");
  TTreeReaderArray<float  > gmtMuonPt(reader,"gmtMuon_pt");
  TTreeReaderArray<float  > gmtMuonPtDxy(reader,"gmtMuon_pt_dxy");
  TTreeReaderArray<float  > gmtMuonPtDxyNN(reader,"gmtMuon_pt_dxyNN");
  TTreeReaderArray<short  > gmtMuonDxy(reader,"gmtMuon_dxy");
  TTreeReaderArray<float  > gmtMuonDxyNN(reader,"gmtMuon_dxyNN");
  TTreeReaderArray<float  > gmtMuonPhi(reader,"gmtMuon_phi");
  TTreeReaderArray<float  > gmtMuonEta(reader,"gmtMuon_eta");
  TTreeReaderArray<short  > gmtMuonCharge(reader,"gmtMuon_q");
  TTreeReaderArray<short  > gmtMuonQual(reader,"gmtMuon_qual");


  TTreeReaderArray<short  > genPartCharge(reader,"genPart_q");
  TTreeReaderArray<float  > genPartPt(reader,"genPart_pt");
  TTreeReaderArray<float  > genPartDxy(reader,"genPart_dxy");
  TTreeReaderArray<float  > genPartEta(reader,"genPart_eta");
  TTreeReaderArray<float  > genPartPhi(reader,"genPart_phi");
  TTreeReaderArray<short  > genPartID(reader,"genPart_ID");
  TTreeReaderArray<int32_t> genPartParentID(reader,"genPart_parentID");
  TTreeReaderArray<float  > genPartVx(reader,"genPart_vx");
  TTreeReaderArray<float  > genPartVy(reader,"genPart_vy");
  TTreeReaderArray<float  > genPartVz(reader,"genPart_vz");




  TCanvas* canv1 = new TCanvas("c_str", "c_str", 1200, 1200);


   TH2F* h_pt = new TH2F("gen_vs_l1_pt", "gen_vs_l1_pt", 90, -5, 85, 90, -5, 85);
   TH2F* h_d0 = new TH2F("gen_vs_l1_d0", "gen_vs_l1_d0", 50, -100, 100, 50, -100, 100);

   TH1F* h_dPt = new TH1F("dPt", "dPt", 100, -100, 100);
   TH1F* h_dD0 = new TH1F("dD0", "dD0", 100, -100, 100);
   

   // TH1F* h_eta = new TH1F("eta", "eta", 100, -100, 100);
   // TH1F* h_phi = new TH1F("phi", "phi", 100, -100, 100);

  int eventCount = 0;
  int matchedCount = 0;
  int matchedEMTF = 0;

  float z_ME2 = 808.0; // ME2 z coordinate in [cm]

  float r = 0.;
  float rStar = 0.;
  
  float etaStar_gen = 0.;

  float phiStar_gen = 0.;

  while(reader.Next()){
    eventCount++;
    if (eventCount % 1000 == 0) std::cout << eventCount << " events read!" << std::endl;
    // std::cout << eventCount << " events read!" << std::endl;
    int gmtMuon = 0;

    for (unsigned int i=0; i<*gmtMuonSize; i++){

      if (abs(gmtMuonEta[i]) < 1.2) continue;
      gmtMuon++;
      // std::cout << gmtMuon << " gmt muon" << std::endl;


      // GEN muon GMT muon matching
      float dR = 5.0;
      int k = -99;
      float etaStar = -99.;
      float phiStar = -99.;
      for (int j=0; j<genPartPt.GetSize(); j++){
        if(abs(genPartID[j]) != 13 ) continue;
        // cpnvert displaced eta and phi to prompt-like and calculate dR between gen and l1 muon

        r = (z_ME2 - abs(genPartVz[j]))/abs(TMath::SinH(genPartEta[j]));
        float xStar = genPartVx[j] + r * TMath::Cos(genPartPhi[j]);
        float yStar = genPartVy[j] + r * TMath::Sin(genPartPhi[j]);
        rStar = TMath::Sqrt(xStar * xStar + yStar * yStar);
        
        etaStar_gen = TMath::ASinH(z_ME2/rStar) * (genPartEta[j]/abs(genPartEta[j]));

        // if (abs(etaStar_gen) < 1.2 || abs(etaStar_gen) > 2.5 ) continue;

        if (xStar >= 0) phiStar_gen = TMath::ATan(yStar/xStar); 
        else if (yStar >= 0 && xStar < 0) phiStar_gen = TMath::Pi() + TMath::ATan(yStar/xStar); 
        else if (yStar <= 0 && xStar < 0) phiStar_gen = TMath::ATan(yStar/xStar) - TMath::Pi(); 

        float dR_new = TMath::Sqrt((gmtMuonEta[i]-etaStar_gen)*(gmtMuonEta[i]-etaStar_gen)+DPhi(gmtMuonPhi[i],phiStar_gen)*DPhi(gmtMuonPhi[i],phiStar_gen));
        // std::cout << "dR GEN: " << dR_new << std::endl;
        if(dR_new > dR){
          continue;
        }
        else{
          k = j;
          dR = dR_new;
          etaStar = etaStar_gen;
          phiStar = phiStar_gen;
        }
          
      }

      float dR_EMTF = 5.0;
      int k_EMTF = -99;
      // GMT muon EMTF track matching
      for (int j=0; j<*emtfTrackSize; j++){
        if(emtfTrackBX[j] != 0) continue;

        float GMTEta = emtfTrackGMTEta[j] * 0.010875;

        int globPhi = (emtfTrackSector[j] - 1) * 96 + emtfTrackGMTPhi[j];

        globPhi = (globPhi + 600) % 576;

        float GMTPhi = globPhi * 0.010908;

        float dR_new_EMTF = TMath::Sqrt((gmtMuonEta[i]-GMTEta)*(gmtMuonEta[i]-GMTEta)+DPhi(gmtMuonPhi[i],GMTPhi)*DPhi(gmtMuonPhi[i],GMTPhi));
        // std::cout << "dR EMTF: " << dR_new_EMTF << std::endl;

        if(dR_new_EMTF > dR_EMTF){
          continue;
        }
        else{
          k_EMTF = j;
          dR_EMTF = dR_new_EMTF;

        }


      }
      // if (!(k > -1 && k_EMTF > -1)) std::cout << "MISMATCH!!" << std::endl;
      // else std::cout << "MATCHED!!" << std::endl;

      if (k > -1){
        matchedCount++;
        
      }
      if(k_EMTF > -1 && k > -1){
        // std::cout << "gmt  pt: " << gmtMuonPt[i] << " gmt  pt2: " << gmtMuonPtDxyNN[i] << std::endl;
        // std::cout << "EMTF pt: " << emtfTrackPt[k_EMTF] << " EMTF pt2: " << emtfTrackPtDxy[k_EMTF] << std::endl;
        // std::cout << "GEN  pt: " << genPartPt[k] << std::endl; 
        matchedEMTF++;
        float d0 = -999.0;
        float invPt = genPartCharge[k]/genPartPt[k];
        invPt = (std::abs(invPt) < 1./10000) ? (invPt < 0 ? -1./10000 : +1./10000) : invPt;
        double R = -1.0 / (0.003 * 3.811 * invPt);
        float xc = genPartVx[k] - (R * std::sin(genPartPhi[k]));
        float yc = genPartVy[k] + (R * std::cos(genPartPhi[k]));
        d0 = R - ((R < 0 ? -1. : +1.) * TMath::Sqrt(xc*xc + yc*yc));

        if (abs(d0) > 100 || abs(genPartVz[k]) > 100) continue;

        h_pt->Fill(genPartPt[k], emtfTrackPtDxy[k_EMTF]);
        h_d0->Fill(d0, emtfTrackDxy[k_EMTF]);

        h_dPt->Fill(emtfTrackPtDxy[k_EMTF] - genPartPt[k]);
        h_dD0->Fill(emtfTrackDxy[k_EMTF] - d0);


      }

      
    } // GMT muon loop
  } // end event loop
  std::cout << matchedCount << " vs " << matchedEMTF << std::endl;



  

  h_pt->GetXaxis()->SetTitle("GEN p_{T} [GeV]");
  h_pt->GetYaxis()->SetTitle("L1 displ. p_{T} [GeV]");

  h_d0->GetXaxis()->SetTitle("GEN d_{0} [cm]");
  h_d0->GetYaxis()->SetTitle("L1 d_{0} [cm]");

  h_dPt->GetXaxis()->SetTitle("L1 p_{T} - GEN p_{T} [GeV]");
  h_dPt->GetYaxis()->SetTitle("# of Muons / bin");

  h_dD0->GetXaxis()->SetTitle("L1 d_{0} - GEN d_{0} [cm]");
  h_dD0->GetYaxis()->SetTitle("# of Muons / bin");



  h_pt->Draw("colz");
  canv1->SaveAs("output_files/comparisons/2D_genVsL1_pT_NNv5_HTo2LL_combined_EMTFNtuple.pdf");

  h_d0->Draw("colz");
  canv1->SaveAs("output_files/comparisons/2D_genVsL1_d0_NNv5_HTo2LL_combined_EMTFNtuple.pdf");

  h_dD0->Draw("");
  canv1->SaveAs("output_files/comparisons/dD0_genVsL1_NNv5_HTo2LL_combined_EMTFNtuple.pdf");

  h_dPt->Draw("");
  canv1->SaveAs("output_files/comparisons/dPt_genVsL1_NNv5_HTo2LL_combined_EMTFNtuple.pdf");


  return 0;
} // end function
