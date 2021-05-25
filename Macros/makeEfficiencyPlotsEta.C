/* EMTF Efficiency Analysis plotting script
 *
 */

#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>
#include "TLatex.h"
#include <boost/algorithm/string.hpp>



TLatex cms_latex(){
  TLatex cms_label;
  cms_label.SetTextSize(0.04);
  cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary Simulation}}");
  return cms_label;
}

TLatex head(){
  TLatex header; 
  header.SetTextSize(0.03);
  header.DrawLatexNDC(0.63, 0.92, "#sqrt{s} = 13 TeV, Run 3 MC");
  return header; 
}

int DefaultColor(int j,int i){
  if (j-i==1) return 2;
  else if (j-i==2) return 4;
  else if (j-i==3) return 6;
  else if (j-i==4) return 8;
  else if (j-i==5) return 9;
  else return j;
}


int makeEfficiencyPlotsEta(){

  //read data
  // TString ntuple = "/eos/cms/store/user/eyigitba/emtf/matchedNtuples/matchedNtuple_HTo2LLTo4Mu_combined_cmssw_11_2_0_pre8_fwImplementation_NNv6.root";
  // TString ntuple = "/afs/cern.ch/user/e/eyigitba/L1T_dev/CMSSW_11_3_0_pre5_ptLUT/src/MatchedNtuple_muGun_v5.root";
  // TString ntuple = "/eos/cms/store/user/eyigitba/emtf/matchedNtuples/matchedNtuple_DisplacedMuGun_flatPt2to1000_posEndcap_flatXYZEtaPhi_11_3_0_pre5_10M.root";
  TString ntuple = "/eos/cms/store/user/eyigitba/emtf/matchedNtuples/matchedNtuple_DisplacedMuGun_flatPt2to1000_allEndcap_flatXYZEtaPhi_11_3_0_pre5_NNv6_20M.root";
  // TString ntuple = "/eos/cms/store/user/eyigitba/emtf/matchedNtuples/matchedNtuple_DisplacedMuGun_flatPt2to1000_allEndcap_flatXYZEtaPhi_11_3_0_pre5_NNv7_2M.root";
  // TString ntuple = "/eos/cms/store/user/eyigitba/emtf/matchedNtuples/matchedNtuple_HTo2LongLivedTo4mu_combined_NNv6.root";
  TChain * cc=new TChain("tree");
  cc->Add(ntuple);


  TTreeReader ccReader(cc);

  TTreeReaderArray<float> genDiMuPt(ccReader,"gendimu_pt");
  TTreeReaderArray<float> genDiMuEta(ccReader,"gendimu_eta");
  TTreeReaderArray<float> genDiMuPhi(ccReader,"gendimu_phi");
  TTreeReaderArray<float> genDiMuLxy(ccReader,"gendimu_Lxy");
  TTreeReaderArray<float> genDiMuVz(ccReader,"gendimu_vz");
  TTreeReaderArray<int> genDiMuDaughter1(ccReader,"gendimu_daughter1");
  TTreeReaderArray<int> genDiMuDaughter2(ccReader,"gendimu_daughter2");

  TTreeReaderArray<float> genMuPt(ccReader,"gen_pt");
  TTreeReaderArray<float> genMuEta(ccReader,"gen_eta");
  TTreeReaderArray<float> genMuEtaStar(ccReader,"gen_etaStar");
  TTreeReaderArray<float> genMuPhi(ccReader,"gen_phi");
  TTreeReaderArray<float> genMuPhiStar(ccReader,"gen_phiStar");
  TTreeReaderArray<float> genMuDR(ccReader,"gen_dR");
  TTreeReaderArray<float> genMuLxy(ccReader,"gen_Lxy");
  TTreeReaderArray<float> genMuVz(ccReader,"gen_vz");
  TTreeReaderArray<float> genMuD0(ccReader,"gen_d0");
  TTreeReaderArray<int> genMuIdx(ccReader,"gen_idx");
  TTreeReaderArray<int> genMuCharge(ccReader,"gen_charge");
  TTreeReaderArray<int> genMuParent(ccReader,"gen_parent");
  TTreeReaderArray<int> genMuMatchedL1MuID(ccReader,"gen_matchedL1Mu");

  TTreeReaderArray<float> l1MuPt(ccReader,"l1_pt");
  TTreeReaderArray<float> l1MuPtDxy(ccReader,"l1_ptDxy");
  TTreeReaderArray<float> l1MuEta(ccReader,"l1_eta");
  TTreeReaderArray<float> l1MuPhi(ccReader,"l1_phi");
  TTreeReaderArray<float> l1MuQual(ccReader,"l1_qual");
  TTreeReaderArray<float> l1MuDxyNN(ccReader,"l1_dxyNN");
  TTreeReaderArray<int> l1MuDxy(ccReader,"l1_dxy");
  TTreeReaderArray<int> l1MuCharge(ccReader,"l1_charge");
  TTreeReaderArray<int> l1MuEmtfMode(ccReader,"l1_emtfMode");

  gStyle->SetOptStat(0);

  std::cout<<"Running on "<<cc->GetEntries()<<" evts "<<std::endl;

  //plot containers
  std::vector<TString> canvasname;
  std::vector<TString> canvasname_2D;
  std::vector<std::string> kwds;
  std::vector<TString> legs;
  std::vector<TString> legs_2D;
  std::vector<TGraphAsymmErrors*> errors;
  std::vector<TH2F*> errors_2D;

  // cosmetic options
  std::vector<bool> grid,logY,logX;



  // initialize cuts
  float ptThreshold = 10.0;
  // float ptThresholdSecond = 5.0;
  float etaThresholdMin = 1.24;
  float etaThresholdMax = 2.5;
  float dRThreshold = 1.0;
  int qualThreshold = 11;
  int modeThreshold = 11;
  float dxyThresholdMin = 25.0;
  float dxyThresholdMax = 100.0;
  float z0Threshold = 100.0;

  Double_t etaArray[15] = {-2.5, -2.1, -1.6, -1.2, -0.9, -0.3, -0.2, 0, 0.2, 0.3, 0.9, 1.2, 1.6, 2.1, 2.5};
  Double_t etaArray2[21] = {-2.5, -2.3, -2.1, -1.85, -1.6, -1.4, -1.2, -0.9, -0.3, -0.2, 0, 0.2, 0.3, 0.9, 1.2, 1.4, 1.6, 1.85, 2.1, 2.3, 2.5};

  // eta binned plots
  TH1F *h_den_pt_eta3 = new TH1F("h_den_pt_eta3", "", 30, 0, 60);
  TH1F *h_num_pt_eta3 = new TH1F("h_num_pt_eta3", "", 30, 0, 60);
  TH1F *h_num_ptNN_eta3 = new TH1F("h_num_ptNN_eta3", "", 30, 0, 60);
  TH1F *h_num_ptComb_eta3 = new TH1F("h_num_ptComb_eta3", "", 30, 0, 60);

  TH1F *h_den_d0_eta3 = new TH1F("h_den_d0_eta3", "", 60, -120, 120);
  TH1F *h_num_d0_eta3 = new TH1F("h_num_d0_eta3", "", 60, -120, 120);
  TH1F *h_num_d0NN_eta3 = new TH1F("h_num_d0NN_eta3", "", 60, -120, 120);
  TH1F *h_num_d0Comb_eta3 = new TH1F("h_num_d0Comb_eta3", "", 60, -120, 120);

  TH1F *h_den_dxy_eta3 = new TH1F("h_den_dxy_eta3", "", 30, 0, 120);
  TH1F *h_num_dxy_eta3 = new TH1F("h_num_dxy_eta3", "", 30, 0, 120);
  TH1F *h_num_dxyNN_eta3 = new TH1F("h_num_dxyNN_eta3", "", 30, 0, 120);
  TH1F *h_num_dxyComb_eta3 = new TH1F("h_num_dxyComb_eta3", "", 30, 0, 120);

  TH1F *h_den_phi_eta3 = new TH1F("h_den_phi_eta3", "", 64, -4, 4);
  TH1F *h_num_phi_eta3 = new TH1F("h_num_phi_eta3", "", 64, -4, 4);
  TH1F *h_num_phiNN_eta3 = new TH1F("h_num_phiNN_eta3", "", 64, -4, 4);
  TH1F *h_num_phiComb_eta3 = new TH1F("h_num_phiComb_eta3", "", 64, -4, 4);

  TH1F *h_den_Lxy_eta3 = new TH1F("h_den_Lxy_eta3", "", 60, 0, 240);
  TH1F *h_num_Lxy_eta3 = new TH1F("h_num_Lxy_eta3", "", 60, 0, 240);
  TH1F *h_num_LxyNN_eta3 = new TH1F("h_num_LxyNN_eta3", "", 60, 0, 240);
  TH1F *h_num_LxyComb_eta3 = new TH1F("h_num_LxyComb_eta3", "", 60, 0, 240);

  //
  TH1F *h_den_pt_eta2 = new TH1F("h_den_pt_eta2", "", 30, 0, 60);
  TH1F *h_num_pt_eta2 = new TH1F("h_num_pt_eta2", "", 30, 0, 60);
  TH1F *h_num_ptNN_eta2 = new TH1F("h_num_ptNN_eta2", "", 30, 0, 60);
  TH1F *h_num_ptComb_eta2 = new TH1F("h_num_ptComb_eta2", "", 30, 0, 60);

  TH1F *h_den_d0_eta2 = new TH1F("h_den_d0_eta2", "", 60, -120, 120);
  TH1F *h_num_d0_eta2 = new TH1F("h_num_d0_eta2", "", 60, -120, 120);
  TH1F *h_num_d0NN_eta2 = new TH1F("h_num_d0NN_eta2", "", 60, -120, 120);
  TH1F *h_num_d0Comb_eta2 = new TH1F("h_num_d0Comb_eta2", "", 60, -120, 120);

  TH1F *h_den_dxy_eta2 = new TH1F("h_den_dxy_eta2", "", 30, 0, 120);
  TH1F *h_num_dxy_eta2 = new TH1F("h_num_dxy_eta2", "", 30, 0, 120);
  TH1F *h_num_dxyNN_eta2 = new TH1F("h_num_dxyNN_eta2", "", 30, 0, 120);
  TH1F *h_num_dxyComb_eta2 = new TH1F("h_num_dxyComb_eta2", "", 30, 0, 120);

  TH1F *h_den_phi_eta2 = new TH1F("h_den_phi_eta2", "", 64, -4, 4);
  TH1F *h_num_phi_eta2 = new TH1F("h_num_phi_eta2", "", 64, -4, 4);
  TH1F *h_num_phiNN_eta2 = new TH1F("h_num_phiNN_eta2", "", 64, -4, 4);
  TH1F *h_num_phiComb_eta2 = new TH1F("h_num_phiComb_eta2", "", 64, -4, 4);

  TH1F *h_den_Lxy_eta2 = new TH1F("h_den_Lxy_eta2", "", 60, 0, 240);
  TH1F *h_num_Lxy_eta2 = new TH1F("h_num_Lxy_eta2", "", 60, 0, 240);
  TH1F *h_num_LxyNN_eta2 = new TH1F("h_num_LxyNN_eta2", "", 60, 0, 240);
  TH1F *h_num_LxyComb_eta2 = new TH1F("h_num_LxyComb_eta2", "", 60, 0, 240);


  //
  TH1F *h_den_pt_eta1 = new TH1F("h_den_pt_eta1", "", 30, 0, 60);
  TH1F *h_num_pt_eta1 = new TH1F("h_num_pt_eta1", "", 30, 0, 60);
  TH1F *h_num_ptNN_eta1 = new TH1F("h_num_ptNN_eta1", "", 30, 0, 60);
  TH1F *h_num_ptComb_eta1 = new TH1F("h_num_ptComb_eta1", "", 30, 0, 60);

  TH1F *h_den_d0_eta1 = new TH1F("h_den_d0_eta1", "", 60, -120, 120);
  TH1F *h_num_d0_eta1 = new TH1F("h_num_d0_eta1", "", 60, -120, 120);
  TH1F *h_num_d0NN_eta1 = new TH1F("h_num_d0NN_eta1", "", 60, -120, 120);
  TH1F *h_num_d0Comb_eta1 = new TH1F("h_num_d0Comb_eta1", "", 60, -120, 120);

  TH1F *h_den_dxy_eta1 = new TH1F("h_den_dxy_eta1", "", 30, 0, 120);
  TH1F *h_num_dxy_eta1 = new TH1F("h_num_dxy_eta1", "", 30, 0, 120);
  TH1F *h_num_dxyNN_eta1 = new TH1F("h_num_dxyNN_eta1", "", 30, 0, 120);
  TH1F *h_num_dxyComb_eta1 = new TH1F("h_num_dxyComb_eta1", "", 30, 0, 120);

  TH1F *h_den_phi_eta1 = new TH1F("h_den_phi_eta1", "", 64, -4, 4);
  TH1F *h_num_phi_eta1 = new TH1F("h_num_phi_eta1", "", 64, -4, 4);
  TH1F *h_num_phiNN_eta1 = new TH1F("h_num_phiNN_eta1", "", 64, -4, 4);
  TH1F *h_num_phiComb_eta1 = new TH1F("h_num_phiComb_eta1", "", 64, -4, 4);

  TH1F *h_den_Lxy_eta1 = new TH1F("h_den_Lxy_eta1", "", 60, 0, 240);
  TH1F *h_num_Lxy_eta1 = new TH1F("h_num_Lxy_eta1", "", 60, 0, 240);
  TH1F *h_num_LxyNN_eta1 = new TH1F("h_num_LxyNN_eta1", "", 60, 0, 240);
  TH1F *h_num_LxyComb_eta1 = new TH1F("h_num_LxyComb_eta1", "", 60, 0, 240);


  // Single muon efficiencies 

  int eventCount = 0;
  while(ccReader.Next()){
    eventCount++;
    if (eventCount % 1000 == 0) std::cout << eventCount << " events read!" << std::endl;
    // if (eventCount == 1000000) break;
    // 2.1 < eta < 2.5
    for(int i=0; i<genMuPt.GetSize(); i++){
      // if (gendR[i] > dRThreshold) continue;
      if (abs(genMuEtaStar[i]) < 2.1) continue;
      if (abs(genMuEtaStar[i]) > etaThresholdMax) continue;
      if (abs(genMuVz[i]) > z0Threshold) continue;
      if (abs(genMuD0[i]) > dxyThresholdMax) continue;
      // if (abs(gendxy[i]) < dxyThresholdMin) continue;
      // if (genMuMatchedL1MuID[i] < 0) continue;
      // if (!(l1MuEmtfMode[genMuMatchedL1MuID[i]] == 11 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 13 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 14 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 15)) continue;


      h_den_pt_eta3->Fill(genMuPt[i]);

      if (genMuPt[i] > (ptThreshold + 5.0)){
        h_den_dxy_eta3->Fill(abs(genMuD0[i]));
        h_den_d0_eta3->Fill(genMuD0[i]);
        h_den_phi_eta3->Fill(genMuPhiStar[i]);
        h_den_Lxy_eta3->Fill(genMuLxy[i]);
      }

      if (genMuDR[i] > 0.6) continue;
      if (l1MuPt[genMuMatchedL1MuID[i]] < 0) continue;
      // if (!(l1MuEmtfMode[genMuMatchedL1MuID[i]] == 11 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 13 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 14 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 15)) continue;
      
      if (l1MuPt[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_pt_eta3->Fill(genMuPt[i]);
        if (genMuPt[i] > (ptThreshold + 5.0)){
          h_num_dxy_eta3->Fill(abs(genMuD0[i]));
          h_num_d0_eta3->Fill(genMuD0[i]);
          h_num_phi_eta3->Fill(genMuPhiStar[i]);
          h_num_Lxy_eta3->Fill(genMuLxy[i]);
        }

      }

      if (l1MuPtDxy[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_ptNN_eta3->Fill(genMuPt[i]);
        if (genMuPt[i] > (ptThreshold + 5.0)){
          h_num_dxyNN_eta3->Fill(abs(genMuD0[i]));
          h_num_d0NN_eta3->Fill(genMuD0[i]);
          h_num_phiNN_eta3->Fill(genMuPhiStar[i]);
          h_num_LxyNN_eta3->Fill(genMuLxy[i]);
        }

      }

      if (l1MuPtDxy[genMuMatchedL1MuID[i]] > ptThreshold || l1MuPt[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_ptComb_eta3->Fill(genMuPt[i]);
        if (genMuPt[i] > (ptThreshold + 5.0)){
          h_num_dxyComb_eta3->Fill(abs(genMuD0[i]));
          h_num_d0Comb_eta3->Fill(genMuD0[i]);
          h_num_phiComb_eta3->Fill(genMuPhiStar[i]);
          h_num_LxyComb_eta3->Fill(genMuLxy[i]);
        }

      }
    }

    // 1.6 < eta < 2.1
    for(int i=0; i<genMuPt.GetSize(); i++){
      // if (gendR[i] > dRThreshold) continue;
      if (abs(genMuEtaStar[i]) < 1.6) continue;
      if (abs(genMuEtaStar[i]) > 2.1) continue;
      if (abs(genMuVz[i]) > z0Threshold) continue;
      if (abs(genMuD0[i]) > dxyThresholdMax) continue;
      // if (abs(gendxy[i]) < dxyThresholdMin) continue;
      // if (genMuMatchedL1MuID[i] < 0) continue;
      // if (!(l1MuEmtfMode[genMuMatchedL1MuID[i]] == 11 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 13 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 14 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 15)) continue;


      h_den_pt_eta2->Fill(genMuPt[i]);

      if (genMuPt[i] > (ptThreshold + 5.0)){
        h_den_dxy_eta2->Fill(abs(genMuD0[i]));
        h_den_d0_eta2->Fill(genMuD0[i]);
        h_den_phi_eta2->Fill(genMuPhiStar[i]);
        h_den_Lxy_eta2->Fill(genMuLxy[i]);
      }

      if (genMuDR[i] > 0.6) continue;
      if (l1MuPt[genMuMatchedL1MuID[i]] < 0) continue;
      // if (!(l1MuEmtfMode[genMuMatchedL1MuID[i]] == 11 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 13 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 14 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 15)) continue;
      
      if (l1MuPt[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_pt_eta2->Fill(genMuPt[i]);
        if (genMuPt[i] > (ptThreshold + 5.0)){
          h_num_dxy_eta2->Fill(abs(genMuD0[i]));
          h_num_d0_eta2->Fill(genMuD0[i]);
          h_num_phi_eta2->Fill(genMuPhiStar[i]);
          h_num_Lxy_eta2->Fill(genMuLxy[i]);
        }

      }

      if (l1MuPtDxy[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_ptNN_eta2->Fill(genMuPt[i]);
        if (genMuPt[i] > (ptThreshold + 5.0)){
          h_num_dxyNN_eta2->Fill(abs(genMuD0[i]));
          h_num_d0NN_eta2->Fill(genMuD0[i]);
          h_num_phiNN_eta2->Fill(genMuPhiStar[i]);
          h_num_LxyNN_eta2->Fill(genMuLxy[i]);
        }

      }

      if (l1MuPtDxy[genMuMatchedL1MuID[i]] > ptThreshold || l1MuPt[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_ptComb_eta2->Fill(genMuPt[i]);
        if (genMuPt[i] > (ptThreshold + 5.0)){
          h_num_dxyComb_eta2->Fill(abs(genMuD0[i]));
          h_num_d0Comb_eta2->Fill(genMuD0[i]);
          h_num_phiComb_eta2->Fill(genMuPhiStar[i]);
          h_num_LxyComb_eta2->Fill(genMuLxy[i]);
        }

      }
    }


    // 1.24 < eta < 1.6
    for(int i=0; i<genMuPt.GetSize(); i++){
      // if (gendR[i] > dRThreshold) continue;
      if (abs(genMuEtaStar[i]) < etaThresholdMin) continue;
      if (abs(genMuEtaStar[i]) > 1.6) continue;
      if (abs(genMuVz[i]) > z0Threshold) continue;
      if (abs(genMuD0[i]) > dxyThresholdMax) continue;
      // if (abs(gendxy[i]) < dxyThresholdMin) continue;
      // if (genMuMatchedL1MuID[i] < 0) continue;
      // if (!(l1MuEmtfMode[genMuMatchedL1MuID[i]] == 11 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 13 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 14 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 15)) continue;


      h_den_pt_eta1->Fill(genMuPt[i]);

      if (genMuPt[i] > (ptThreshold + 5.0)){
        h_den_dxy_eta1->Fill(abs(genMuD0[i]));
        h_den_d0_eta1->Fill(genMuD0[i]);
        h_den_phi_eta1->Fill(genMuPhiStar[i]);
        h_den_Lxy_eta1->Fill(genMuLxy[i]);
      }

      if (genMuDR[i] > 0.6) continue;
      if (l1MuPt[genMuMatchedL1MuID[i]] < 0) continue;
      // if (!(l1MuEmtfMode[genMuMatchedL1MuID[i]] == 11 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 13 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 14 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 15)) continue;
      
      if (l1MuPt[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_pt_eta1->Fill(genMuPt[i]);
        if (genMuPt[i] > (ptThreshold + 5.0)){
          h_num_dxy_eta1->Fill(abs(genMuD0[i]));
          h_num_d0_eta1->Fill(genMuD0[i]);
          h_num_phi_eta1->Fill(genMuPhiStar[i]);
          h_num_Lxy_eta1->Fill(genMuLxy[i]);
        }

      }

      if (l1MuPtDxy[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_ptNN_eta1->Fill(genMuPt[i]);
        if (genMuPt[i] > (ptThreshold + 5.0)){
          h_num_dxyNN_eta1->Fill(abs(genMuD0[i]));
          h_num_d0NN_eta1->Fill(genMuD0[i]);
          h_num_phiNN_eta1->Fill(genMuPhiStar[i]);
          h_num_LxyNN_eta1->Fill(genMuLxy[i]);
        }

      }

      if (l1MuPtDxy[genMuMatchedL1MuID[i]] > ptThreshold || l1MuPt[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_ptComb_eta1->Fill(genMuPt[i]);
        if (genMuPt[i] > (ptThreshold + 5.0)){
          h_num_dxyComb_eta1->Fill(abs(genMuD0[i]));
          h_num_d0Comb_eta1->Fill(genMuD0[i]);
          h_num_phiComb_eta1->Fill(genMuPhiStar[i]);
          h_num_LxyComb_eta1->Fill(genMuLxy[i]);
        }

      }
    }
    


  }


  // Divide histograms

  // eta binned
  TGraphAsymmErrors * error_pt_eta3 = new TGraphAsymmErrors(h_num_pt_eta3,h_den_pt_eta3);
  TGraphAsymmErrors * error_ptNN_eta3 = new TGraphAsymmErrors(h_num_ptNN_eta3,h_den_pt_eta3);
  TGraphAsymmErrors * error_ptComb_eta3 = new TGraphAsymmErrors(h_num_ptComb_eta3,h_den_pt_eta3);

  TGraphAsymmErrors * error_d0_eta3 = new TGraphAsymmErrors(h_num_d0_eta3,h_den_d0_eta3);
  TGraphAsymmErrors * error_d0NN_eta3 = new TGraphAsymmErrors(h_num_d0NN_eta3,h_den_d0_eta3);
  TGraphAsymmErrors * error_d0Comb_eta3 = new TGraphAsymmErrors(h_num_d0Comb_eta3,h_den_d0_eta3);

  TGraphAsymmErrors * error_dxy_eta3 = new TGraphAsymmErrors(h_num_dxy_eta3,h_den_dxy_eta3);
  TGraphAsymmErrors * error_dxyNN_eta3 = new TGraphAsymmErrors(h_num_dxyNN_eta3,h_den_dxy_eta3);
  TGraphAsymmErrors * error_dxyComb_eta3 = new TGraphAsymmErrors(h_num_dxyComb_eta3,h_den_dxy_eta3);

  TGraphAsymmErrors * error_phi_eta3 = new TGraphAsymmErrors(h_num_phi_eta3,h_den_phi_eta3);
  TGraphAsymmErrors * error_phiNN_eta3 = new TGraphAsymmErrors(h_num_phiNN_eta3,h_den_phi_eta3);
  TGraphAsymmErrors * error_phiComb_eta3 = new TGraphAsymmErrors(h_num_phiComb_eta3,h_den_phi_eta3);

  TGraphAsymmErrors * error_Lxy_eta3 = new TGraphAsymmErrors(h_num_Lxy_eta3,h_den_Lxy_eta3);
  TGraphAsymmErrors * error_LxyNN_eta3 = new TGraphAsymmErrors(h_num_LxyNN_eta3,h_den_Lxy_eta3);
  TGraphAsymmErrors * error_LxyComb_eta3 = new TGraphAsymmErrors(h_num_LxyComb_eta3,h_den_Lxy_eta3);

  // eta binned
  TGraphAsymmErrors * error_pt_eta2 = new TGraphAsymmErrors(h_num_pt_eta2,h_den_pt_eta2);
  TGraphAsymmErrors * error_ptNN_eta2 = new TGraphAsymmErrors(h_num_ptNN_eta2,h_den_pt_eta2);
  TGraphAsymmErrors * error_ptComb_eta2 = new TGraphAsymmErrors(h_num_ptComb_eta2,h_den_pt_eta2);

  TGraphAsymmErrors * error_d0_eta2 = new TGraphAsymmErrors(h_num_d0_eta2,h_den_d0_eta2);
  TGraphAsymmErrors * error_d0NN_eta2 = new TGraphAsymmErrors(h_num_d0NN_eta2,h_den_d0_eta2);
  TGraphAsymmErrors * error_d0Comb_eta2 = new TGraphAsymmErrors(h_num_d0Comb_eta2,h_den_d0_eta2);

  TGraphAsymmErrors * error_dxy_eta2 = new TGraphAsymmErrors(h_num_dxy_eta2,h_den_dxy_eta2);
  TGraphAsymmErrors * error_dxyNN_eta2 = new TGraphAsymmErrors(h_num_dxyNN_eta2,h_den_dxy_eta2);
  TGraphAsymmErrors * error_dxyComb_eta2 = new TGraphAsymmErrors(h_num_dxyComb_eta2,h_den_dxy_eta2);

  TGraphAsymmErrors * error_phi_eta2 = new TGraphAsymmErrors(h_num_phi_eta2,h_den_phi_eta2);
  TGraphAsymmErrors * error_phiNN_eta2 = new TGraphAsymmErrors(h_num_phiNN_eta2,h_den_phi_eta2);
  TGraphAsymmErrors * error_phiComb_eta2 = new TGraphAsymmErrors(h_num_phiComb_eta2,h_den_phi_eta2);

  TGraphAsymmErrors * error_Lxy_eta2 = new TGraphAsymmErrors(h_num_Lxy_eta2,h_den_Lxy_eta2);
  TGraphAsymmErrors * error_LxyNN_eta2 = new TGraphAsymmErrors(h_num_LxyNN_eta2,h_den_Lxy_eta2);
  TGraphAsymmErrors * error_LxyComb_eta2 = new TGraphAsymmErrors(h_num_LxyComb_eta2,h_den_Lxy_eta2);

  // eta binned
  TGraphAsymmErrors * error_pt_eta1 = new TGraphAsymmErrors(h_num_pt_eta1,h_den_pt_eta1);
  TGraphAsymmErrors * error_ptNN_eta1 = new TGraphAsymmErrors(h_num_ptNN_eta1,h_den_pt_eta1);
  TGraphAsymmErrors * error_ptComb_eta1 = new TGraphAsymmErrors(h_num_ptComb_eta1,h_den_pt_eta1);

  TGraphAsymmErrors * error_d0_eta1 = new TGraphAsymmErrors(h_num_d0_eta1,h_den_d0_eta1);
  TGraphAsymmErrors * error_d0NN_eta1 = new TGraphAsymmErrors(h_num_d0NN_eta1,h_den_d0_eta1);
  TGraphAsymmErrors * error_d0Comb_eta1 = new TGraphAsymmErrors(h_num_d0Comb_eta1,h_den_d0_eta1);

  TGraphAsymmErrors * error_dxy_eta1 = new TGraphAsymmErrors(h_num_dxy_eta1,h_den_dxy_eta1);
  TGraphAsymmErrors * error_dxyNN_eta1 = new TGraphAsymmErrors(h_num_dxyNN_eta1,h_den_dxy_eta1);
  TGraphAsymmErrors * error_dxyComb_eta1 = new TGraphAsymmErrors(h_num_dxyComb_eta1,h_den_dxy_eta1);

  TGraphAsymmErrors * error_phi_eta1 = new TGraphAsymmErrors(h_num_phi_eta1,h_den_phi_eta1);
  TGraphAsymmErrors * error_phiNN_eta1 = new TGraphAsymmErrors(h_num_phiNN_eta1,h_den_phi_eta1);
  TGraphAsymmErrors * error_phiComb_eta1 = new TGraphAsymmErrors(h_num_phiComb_eta1,h_den_phi_eta1);

  TGraphAsymmErrors * error_Lxy_eta1 = new TGraphAsymmErrors(h_num_Lxy_eta1,h_den_Lxy_eta1);
  TGraphAsymmErrors * error_LxyNN_eta1 = new TGraphAsymmErrors(h_num_LxyNN_eta1,h_den_Lxy_eta1);
  TGraphAsymmErrors * error_LxyComb_eta1 = new TGraphAsymmErrors(h_num_LxyComb_eta1,h_den_Lxy_eta1);


  // canvasname.push_back("out_ptUnc_vs_dxy_wME11a"); 
  TString titlePt="Gen Muon p_{T} [GeV]";
  TString titleDxy="Gen Muon d_{xy} [cm]";
  TString titleLxy="Gen Muon Lxy [cm]";
  TString titleEta="Gen Muon #eta";
  TString titlePhi="Gen Muon #phi";
  TString titleZ0="Gen muon Z_{0} [cm]";

  // eta binned
  error_pt_eta3->GetXaxis()->SetTitle(titlePt);
  error_pt_eta3->GetYaxis()->SetTitle("L1T Efficiency");
  error_pt_eta3->GetYaxis()->SetRangeUser(0.00001,1.2);
  error_pt_eta3->SetMarkerStyle(25);
  error_pt_eta3->SetMarkerSize(1.2);

  error_ptNN_eta3->GetXaxis()->SetTitle(titlePt);
  error_ptNN_eta3->GetYaxis()->SetTitle("L1T Efficiency");
  error_ptNN_eta3->GetYaxis()->SetRangeUser(0.00001,1.2);
  error_ptNN_eta3->SetMarkerStyle(29);
  error_ptNN_eta3->SetMarkerSize(2);

  error_d0_eta3->GetXaxis()->SetTitle(titleDxy);
  error_d0_eta3->GetYaxis()->SetTitle("L1T Efficiency");
  error_d0_eta3->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_dxy_eta3->GetXaxis()->SetTitle(titleDxy);
  error_dxy_eta3->GetYaxis()->SetTitle("L1T Efficiency");
  error_dxy_eta3->GetYaxis()->SetRangeUser(0.00001,1.2);
  error_dxy_eta3->SetMarkerStyle(25);
  error_dxy_eta3->SetMarkerSize(1.2);

  error_dxyNN_eta3->GetXaxis()->SetTitle(titleDxy);
  error_dxyNN_eta3->GetYaxis()->SetTitle("L1T Efficiency");
  error_dxyNN_eta3->GetYaxis()->SetRangeUser(0.00001,1.2);
  error_dxyNN_eta3->SetMarkerStyle(29);
  error_dxyNN_eta3->SetMarkerSize(2);

  error_phi_eta3->GetXaxis()->SetTitle(titlePhi);
  error_phi_eta3->GetYaxis()->SetTitle("L1T Efficiency");
  error_phi_eta3->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_Lxy_eta3->GetXaxis()->SetTitle(titleLxy);
  error_Lxy_eta3->GetYaxis()->SetTitle("L1T Efficiency");
  error_Lxy_eta3->GetYaxis()->SetRangeUser(0.00001,1.2);

  // eta binned
  error_pt_eta2->GetXaxis()->SetTitle(titlePt);
  error_pt_eta2->GetYaxis()->SetTitle("L1T Efficiency");
  error_pt_eta2->GetYaxis()->SetRangeUser(0.00001,1.2);
  error_pt_eta2->SetMarkerStyle(25);
  error_pt_eta2->SetMarkerSize(1.2);

  error_ptNN_eta2->GetXaxis()->SetTitle(titlePt);
  error_ptNN_eta2->GetYaxis()->SetTitle("L1T Efficiency");
  error_ptNN_eta2->GetYaxis()->SetRangeUser(0.00001,1.2);
  error_ptNN_eta2->SetMarkerStyle(29);
  error_ptNN_eta2->SetMarkerSize(2);

  error_d0_eta2->GetXaxis()->SetTitle(titleDxy);
  error_d0_eta2->GetYaxis()->SetTitle("L1T Efficiency");
  error_d0_eta2->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_dxy_eta2->GetXaxis()->SetTitle(titleDxy);
  error_dxy_eta2->GetYaxis()->SetTitle("L1T Efficiency");
  error_dxy_eta2->GetYaxis()->SetRangeUser(0.00001,1.2);
  error_dxy_eta2->SetMarkerStyle(25);
  error_dxy_eta2->SetMarkerSize(1.2);

  error_dxyNN_eta2->GetXaxis()->SetTitle(titleDxy);
  error_dxyNN_eta2->GetYaxis()->SetTitle("L1T Efficiency");
  error_dxyNN_eta2->GetYaxis()->SetRangeUser(0.00001,1.2);
  error_dxyNN_eta2->SetMarkerStyle(29);
  error_dxyNN_eta2->SetMarkerSize(2);

  error_phi_eta2->GetXaxis()->SetTitle(titlePhi);
  error_phi_eta2->GetYaxis()->SetTitle("L1T Efficiency");
  error_phi_eta2->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_Lxy_eta2->GetXaxis()->SetTitle(titleLxy);
  error_Lxy_eta2->GetYaxis()->SetTitle("L1T Efficiency");
  error_Lxy_eta2->GetYaxis()->SetRangeUser(0.00001,1.2);

  // eta binned
  error_pt_eta1->GetXaxis()->SetTitle(titlePt);
  error_pt_eta1->GetYaxis()->SetTitle("L1T Efficiency");
  error_pt_eta1->GetYaxis()->SetRangeUser(0.00001,1.2);
  error_pt_eta1->SetMarkerStyle(25);
  error_pt_eta1->SetMarkerSize(1.2);

  error_ptNN_eta1->GetXaxis()->SetTitle(titlePt);
  error_ptNN_eta1->GetYaxis()->SetTitle("L1T Efficiency");
  error_ptNN_eta1->GetYaxis()->SetRangeUser(0.00001,1.2);
  error_ptNN_eta1->SetMarkerStyle(29);
  error_ptNN_eta1->SetMarkerSize(2);

  error_d0_eta1->GetXaxis()->SetTitle(titleDxy);
  error_d0_eta1->GetYaxis()->SetTitle("L1T Efficiency");
  error_d0_eta1->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_dxy_eta1->GetXaxis()->SetTitle(titleDxy);
  error_dxy_eta1->GetYaxis()->SetTitle("L1T Efficiency");
  error_dxy_eta1->GetYaxis()->SetRangeUser(0.00001,1.2);
  error_dxy_eta1->SetMarkerStyle(25);
  error_dxy_eta1->SetMarkerSize(1.2);

  error_dxyNN_eta1->GetXaxis()->SetTitle(titleDxy);
  error_dxyNN_eta1->GetYaxis()->SetTitle("L1T Efficiency");
  error_dxyNN_eta1->GetYaxis()->SetRangeUser(0.00001,1.2);
  error_dxyNN_eta1->SetMarkerStyle(29);
  error_dxyNN_eta1->SetMarkerSize(2);

  error_phi_eta1->GetXaxis()->SetTitle(titlePhi);
  error_phi_eta1->GetYaxis()->SetTitle("L1T Efficiency");
  error_phi_eta1->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_Lxy_eta1->GetXaxis()->SetTitle(titleLxy);
  error_Lxy_eta1->GetYaxis()->SetTitle("L1T Efficiency");
  error_Lxy_eta1->GetYaxis()->SetRangeUser(0.00001,1.2);


  TString leg = "L1 pT (prompt) > 10 GeV";
  TString leg2 = "L1 pT (disp.) > 10 GeV";
  TString leg3 = "L1 pT (disp.) > 10 GeV || L1 pT (prompt) > 10 GeV";

  TString leg_etaFull = "1.2 < |#eta| < 2.5";
  TString leg_eta1 = "1.2 < |#eta| < 1.6";
  TString leg_eta2 = "1.6 < |#eta| < 2.1";
  TString leg_eta3 = "2.1 < |#eta| < 2.5";


  TString leg_NN_eta1 = "disp. 1.2 < |#eta| < 1.6";
  TString leg_NN_eta2 = "disp. 1.6 < |#eta| < 2.1";
  TString leg_NN_eta3 = "disp. 2.1 < |#eta| < 2.5";

  TString leg_BDT_eta1 = "prompt 1.2 < |#eta| < 1.6";
  TString leg_BDT_eta2 = "prompt 1.6 < |#eta| < 2.1";
  TString leg_BDT_eta3 = "prompt 2.1 < |#eta| < 2.5";



  //eta binned
  // errors.push_back(error_pt_eta3);
  // errors.push_back(error_ptNN_eta3);
  // errors.push_back(error_ptComb_eta3);
  // errors.push_back(error_d0_eta3);
  // errors.push_back(error_d0NN_eta3);
  // errors.push_back(error_d0Comb_eta3);
  // errors.push_back(error_dxy_eta3);
  // errors.push_back(error_dxyNN_eta3);
  // errors.push_back(error_dxyComb_eta3);
  // errors.push_back(error_phi_eta3);
  // errors.push_back(error_phiNN_eta3);
  // errors.push_back(error_phiComb_eta3);
  // errors.push_back(error_Lxy_eta3);
  // errors.push_back(error_LxyNN_eta3);
  // errors.push_back(error_LxyComb_eta3);

  //eta binned
  // errors.push_back(error_pt_eta2);
  // errors.push_back(error_ptNN_eta2);
  // errors.push_back(error_ptComb_eta2);
  // errors.push_back(error_d0_eta2);
  // errors.push_back(error_d0NN_eta2);
  // errors.push_back(error_d0Comb_eta2);
  // errors.push_back(error_dxy_eta2);
  // errors.push_back(error_dxyNN_eta2);
  // errors.push_back(error_dxyComb_eta2);
  // errors.push_back(error_phi_eta2);
  // errors.push_back(error_phiNN_eta2);
  // errors.push_back(error_phiComb_eta2);
  // errors.push_back(error_Lxy_eta2);
  // errors.push_back(error_LxyNN_eta2);
  // errors.push_back(error_LxyComb_eta2);

  //eta binned
  // errors.push_back(error_pt_eta1);
  // errors.push_back(error_ptNN_eta1);
  // errors.push_back(error_ptComb_eta1);
  // errors.push_back(error_d0_eta1);
  // errors.push_back(error_d0NN_eta1);
  // errors.push_back(error_d0Comb_eta1);
  // errors.push_back(error_dxy_eta1);
  // errors.push_back(error_dxyNN_eta1);
  // errors.push_back(error_dxyComb_eta1);
  // errors.push_back(error_phi_eta1);
  // errors.push_back(error_phiNN_eta1);
  // errors.push_back(error_phiComb_eta1);
  // errors.push_back(error_Lxy_eta1);
  // errors.push_back(error_LxyNN_eta1);
  // errors.push_back(error_LxyComb_eta1);

  // eta same plot
  errors.push_back(error_pt_eta1);
  errors.push_back(error_ptNN_eta1);
  errors.push_back(error_pt_eta2);
  errors.push_back(error_ptNN_eta2);
  errors.push_back(error_pt_eta3);
  errors.push_back(error_ptNN_eta3);
  errors.push_back(error_dxy_eta1);
  errors.push_back(error_dxyNN_eta1);
  errors.push_back(error_dxy_eta2);
  errors.push_back(error_dxyNN_eta2);
  errors.push_back(error_dxy_eta3);
  errors.push_back(error_dxyNN_eta3);

  // eta binned
  // legs.push_back(leg);
  // legs.push_back(leg2);
  // legs.push_back(leg3);
  // legs.push_back(leg);
  // legs.push_back(leg2);
  // legs.push_back(leg3);
  // legs.push_back(leg);
  // legs.push_back(leg2);
  // legs.push_back(leg3);
  // legs.push_back(leg);
  // legs.push_back(leg2);
  // legs.push_back(leg3);
  // legs.push_back(leg);
  // legs.push_back(leg2);
  // legs.push_back(leg3);

  // // eta binned
  // legs.push_back(leg);
  // legs.push_back(leg2);
  // legs.push_back(leg3);
  // legs.push_back(leg);
  // legs.push_back(leg2);
  // legs.push_back(leg3);
  // legs.push_back(leg);
  // legs.push_back(leg2);
  // legs.push_back(leg3);
  // legs.push_back(leg);
  // legs.push_back(leg2);
  // legs.push_back(leg3);
  // legs.push_back(leg);
  // legs.push_back(leg2);
  // legs.push_back(leg3);

  // // eta binned
  // legs.push_back(leg);
  // legs.push_back(leg2);
  // legs.push_back(leg3);
  // legs.push_back(leg);
  // legs.push_back(leg2);
  // legs.push_back(leg3);
  // legs.push_back(leg);
  // legs.push_back(leg2);
  // legs.push_back(leg3);
  // legs.push_back(leg);
  // legs.push_back(leg2);
  // legs.push_back(leg3);
  // legs.push_back(leg);
  // legs.push_back(leg2);
  // legs.push_back(leg3);

  // eta same plot
  legs.push_back(leg_BDT_eta1);
  legs.push_back(leg_NN_eta1);
  legs.push_back(leg_BDT_eta2);
  legs.push_back(leg_NN_eta2);
  legs.push_back(leg_BDT_eta3);
  legs.push_back(leg_NN_eta3);
  legs.push_back(leg_BDT_eta1);
  legs.push_back(leg_NN_eta1);
  legs.push_back(leg_BDT_eta2);
  legs.push_back(leg_NN_eta2);
  legs.push_back(leg_BDT_eta3);
  legs.push_back(leg_NN_eta3);


  // //eta binned
  // canvasname.push_back("eff_pt_pt10_NNv6vsBDT_eta3");
  // canvasname.push_back("eff_pt_pt10_NNv6vsBDT_eta3");
  // canvasname.push_back("eff_pt_pt10_NNv6vsBDT_eta3");
  // canvasname.push_back("eff_d0_pt10_NNv6vsBDT_eta3");
  // canvasname.push_back("eff_d0_pt10_NNv6vsBDT_eta3");
  // canvasname.push_back("eff_d0_pt10_NNv6vsBDT_eta3");
  // canvasname.push_back("eff_dxy_pt10_NNv6vsBDT_eta3");
  // canvasname.push_back("eff_dxy_pt10_NNv6vsBDT_eta3");
  // canvasname.push_back("eff_dxy_pt10_NNv6vsBDT_eta3");
  // canvasname.push_back("eff_phi_pt10_NNv6vsBDT_eta3");
  // canvasname.push_back("eff_phi_pt10_NNv6vsBDT_eta3");
  // canvasname.push_back("eff_phi_pt10_NNv6vsBDT_eta3");
  // canvasname.push_back("eff_Lxy_pt10_NNv6vsBDT_eta3");
  // canvasname.push_back("eff_Lxy_pt10_NNv6vsBDT_eta3");
  // canvasname.push_back("eff_Lxy_pt10_NNv6vsBDT_eta3");

  // //eta binned
  // canvasname.push_back("eff_pt_pt10_NNv6vsBDT_eta2");
  // canvasname.push_back("eff_pt_pt10_NNv6vsBDT_eta2");
  // canvasname.push_back("eff_pt_pt10_NNv6vsBDT_eta2");
  // canvasname.push_back("eff_d0_pt10_NNv6vsBDT_eta2");
  // canvasname.push_back("eff_d0_pt10_NNv6vsBDT_eta2");
  // canvasname.push_back("eff_d0_pt10_NNv6vsBDT_eta2");
  // canvasname.push_back("eff_dxy_pt10_NNv6vsBDT_eta2");
  // canvasname.push_back("eff_dxy_pt10_NNv6vsBDT_eta2");
  // canvasname.push_back("eff_dxy_pt10_NNv6vsBDT_eta2");
  // canvasname.push_back("eff_phi_pt10_NNv6vsBDT_eta2");
  // canvasname.push_back("eff_phi_pt10_NNv6vsBDT_eta2");
  // canvasname.push_back("eff_phi_pt10_NNv6vsBDT_eta2");
  // canvasname.push_back("eff_Lxy_pt10_NNv6vsBDT_eta2");
  // canvasname.push_back("eff_Lxy_pt10_NNv6vsBDT_eta2");
  // canvasname.push_back("eff_Lxy_pt10_NNv6vsBDT_eta2");

  // //eta binned
  // canvasname.push_back("eff_pt_pt10_NNv6vsBDT_eta1");
  // canvasname.push_back("eff_pt_pt10_NNv6vsBDT_eta1");
  // canvasname.push_back("eff_pt_pt10_NNv6vsBDT_eta1");
  // canvasname.push_back("eff_d0_pt10_NNv6vsBDT_eta1");
  // canvasname.push_back("eff_d0_pt10_NNv6vsBDT_eta1");
  // canvasname.push_back("eff_d0_pt10_NNv6vsBDT_eta1");
  // canvasname.push_back("eff_dxy_pt10_NNv6vsBDT_eta1");
  // canvasname.push_back("eff_dxy_pt10_NNv6vsBDT_eta1");
  // canvasname.push_back("eff_dxy_pt10_NNv6vsBDT_eta1");
  // canvasname.push_back("eff_phi_pt10_NNv6vsBDT_eta1");
  // canvasname.push_back("eff_phi_pt10_NNv6vsBDT_eta1");
  // canvasname.push_back("eff_phi_pt10_NNv6vsBDT_eta1");
  // canvasname.push_back("eff_Lxy_pt10_NNv6vsBDT_eta1");
  // canvasname.push_back("eff_Lxy_pt10_NNv6vsBDT_eta1");
  // canvasname.push_back("eff_Lxy_pt10_NNv6vsBDT_eta1");

  // eta same plot
  canvasname.push_back("eff_pt_pt10_NNv6vsBDT_etaSame");
  canvasname.push_back("eff_pt_pt10_NNv6vsBDT_etaSame");
  canvasname.push_back("eff_pt_pt10_NNv6vsBDT_etaSame");
  canvasname.push_back("eff_pt_pt10_NNv6vsBDT_etaSame");
  canvasname.push_back("eff_pt_pt10_NNv6vsBDT_etaSame");
  canvasname.push_back("eff_pt_pt10_NNv6vsBDT_etaSame");
  canvasname.push_back("eff_dxy_pt10_NNv6vsBDT_etaSame");
  canvasname.push_back("eff_dxy_pt10_NNv6vsBDT_etaSame");
  canvasname.push_back("eff_dxy_pt10_NNv6vsBDT_etaSame");
  canvasname.push_back("eff_dxy_pt10_NNv6vsBDT_etaSame");
  canvasname.push_back("eff_dxy_pt10_NNv6vsBDT_etaSame");
  canvasname.push_back("eff_dxy_pt10_NNv6vsBDT_etaSame");


  //create canvas and save histos
  std::cout << "Drawing: " << error_pt_eta1->GetName() << std::endl;
  TCanvas * c1=new TCanvas("eff_pt_pt10_NNv6vsBDT_etaSame","eff_pt_pt10_NNv6vsBDT_etaSame",700,700);
  error_pt_eta1->Draw("A P");
  // errors[i]->SetLineWidth(3);
  error_pt_eta1->SetLineColor(1);
  error_pt_eta1->SetMarkerColor(1);
  TLatex cms_label=cms_latex();
  TLatex header=head();

  error_pt_eta2->Draw("sames P");
  error_pt_eta2->SetLineColor(2);
  error_pt_eta2->SetMarkerColor(2);

  error_pt_eta3->Draw("sames P");
  error_pt_eta3->SetLineColor(4);
  error_pt_eta3->SetMarkerColor(4);

  error_ptNN_eta1->Draw("sames P");
  error_ptNN_eta1->SetLineColor(1);
  error_ptNN_eta1->SetMarkerColor(1);

  error_ptNN_eta2->Draw("sames P");
  error_ptNN_eta2->SetLineColor(2);
  error_ptNN_eta2->SetMarkerColor(2);

  error_ptNN_eta3->Draw("sames P");
  error_ptNN_eta3->SetLineColor(4);
  error_ptNN_eta3->SetMarkerColor(4);

  gStyle->SetLegendBorderSize(0);
  gStyle->SetLegendTextSize(0.018); 
  TLegend * legBDT =new TLegend(0.4,0.8,0.6,0.88);
  legBDT->AddEntry(error_pt_eta1,leg_BDT_eta1);
  legBDT->AddEntry(error_pt_eta2,leg_BDT_eta2);
  legBDT->AddEntry(error_pt_eta3,leg_BDT_eta3);
  legBDT->Draw("sames");

  TLegend * legNN =new TLegend(0.65,0.8,0.88,0.88);
  legNN->AddEntry(error_ptNN_eta1,leg_NN_eta1);
  legNN->AddEntry(error_ptNN_eta2,leg_NN_eta2);
  legNN->AddEntry(error_ptNN_eta3,leg_NN_eta3);
  legNN->Draw("sames");

  c1->SaveAs("./output_files/comparisons/efficiencies/NNv6/eff_pt_pt10_NNv6vsBDT_etaSame_muGunFlatXYZ_newEta.pdf");


  std::cout << "Drawing: " << error_dxy_eta1->GetName() << std::endl;
  TCanvas * c2=new TCanvas("eff_dxy_pt10_NNv6vsBDT_etaSame","eff_dxy_pt10_NNv6vsBDT_etaSame",700,700);
  error_dxy_eta1->Draw("A P");
  // errors[i]->SetLineWidth(3);
  error_dxy_eta1->SetLineColor(1);
  error_dxy_eta1->SetMarkerColor(1);
  TLatex cms_label2=cms_latex();
  TLatex header2=head();

  error_dxy_eta2->Draw("sames P");
  error_dxy_eta2->SetLineColor(2);
  error_dxy_eta2->SetMarkerColor(2);

  error_dxy_eta3->Draw("sames P");
  error_dxy_eta3->SetLineColor(4);
  error_dxy_eta3->SetMarkerColor(4);

  error_dxyNN_eta1->Draw("sames P");
  error_dxyNN_eta1->SetLineColor(1);
  error_dxyNN_eta1->SetMarkerColor(1);

  error_dxyNN_eta2->Draw("sames P");
  error_dxyNN_eta2->SetLineColor(2);
  error_dxyNN_eta2->SetMarkerColor(2);

  error_dxyNN_eta3->Draw("sames P");
  error_dxyNN_eta3->SetLineColor(4);
  error_dxyNN_eta3->SetMarkerColor(4);

  gStyle->SetLegendBorderSize(0);
  gStyle->SetLegendTextSize(0.018); 
  TLegend * legBDT2 =new TLegend(0.4,0.8,0.6,0.88);
  legBDT2->AddEntry(error_dxy_eta1,leg_BDT_eta1);
  legBDT2->AddEntry(error_dxy_eta2,leg_BDT_eta2);
  legBDT2->AddEntry(error_dxy_eta3,leg_BDT_eta3);
  legBDT2->Draw("sames");

  TLegend * legNN2 =new TLegend(0.65,0.8,0.88,0.88);
  legNN2->AddEntry(error_dxyNN_eta1,leg_NN_eta1);
  legNN2->AddEntry(error_dxyNN_eta2,leg_NN_eta2);
  legNN2->AddEntry(error_dxyNN_eta3,leg_NN_eta3);
  legNN2->Draw("sames");

  c2->SaveAs("./output_files/comparisons/efficiencies/NNv6/eff_dxy_pt10_NNv6vsBDT_etaSame_muGunFlatXYZ_newEta.pdf");


  // for (int i=0; i<canvasname.size(); i++){
  //   if (canvasname[i]=="Canvas_name_already_used_action_skipping"){ 
  //     std::cout << "canvas already used. skipping..." << std::endl;
  //     continue;
  //   }
  //   //create canvas and save histos
  //   std::cout << "Drawing: " << errors[i]->GetName() << std::endl;
  //   TCanvas * c1=new TCanvas(canvasname[i],canvasname[i],700,700);
  //   errors[i]->Draw("A P");
  //   // errors[i]->SetLineWidth(3);
  //   errors[i]->SetLineColor(1);
  //   errors[i]->SetMarkerColor(1);
  //   TLatex cms_label=cms_latex();
  //   TLatex header=head();

  //   gStyle->SetLegendBorderSize(0);
  //   gStyle->SetLegendTextSize(0.018); 
  //   TLegend * leg =new TLegend(0.4,0.8,0.88,0.88);
  //   leg->AddEntry(errors[i],legs[i]);
  //   // bool printLeg=false;

  //   //put histos with same cnvas name in same plot
  //   for (int j=i+1; j<errors.size(); j++){
  //     if (canvasname[i]!=canvasname[j]) continue;
  //     errors[j]->Draw("sames P");
  //     canvasname[j]="Canvas_name_already_used_action_skipping";
  //     // printLeg=true;
  //     // errors[j]->SetLineWidth(3);
  //     // errors[j]->SetLineColor(2);
  //     int k = floor((j+1)/2);

  //     errors[j]->SetLineColor(DefaultColor(k,i));
  //     errors[j]->SetMarkerColor(DefaultColor(k,i));
  //     leg->AddEntry(errors[j],legs[j]);
  //   }
  //   leg->Draw("sames");


  //   // if (printLeg) leg->Draw("sames");
  //   // if (!grid[i]) c1->SetGrid(0);
  //   // if (logX[i]) c1->SetLogx();
  //   // if (logY[i]) c1->SetLogy();

  //   // c1->SaveAs("./plots"+canvasname[i]+".png");
  //   c1->SaveAs("./output_files/comparisons/efficiencies/NNv6/"+canvasname[i]+"_muGunFlatXYZ_newEta.pdf");
  //   canvasname[i]="Canvas_name_already_used_action_skipping";

  // }



  return 0;
 }
