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


int makeEfficiencyPlots(){

  //read data
  // TString ntuple = "/eos/cms/store/user/eyigitba/emtf/matchedNtuples/matchedNtuple_HTo2LLTo4Mu_combined_cmssw_11_2_0_pre8_fwImplementation_NNv6.root";
  // TString ntuple = "/afs/cern.ch/user/e/eyigitba/L1T_dev/CMSSW_11_3_0_pre5_ptLUT/src/MatchedNtuple_muGun_v5.root";
  // TString ntuple = "/eos/cms/store/user/eyigitba/emtf/matchedNtuples/matchedNtuple_DisplacedMuGun_flatPt2to1000_posEndcap_flatXYZEtaPhi_11_3_0_pre5_10M.root";
  // TString ntuple = "/eos/cms/store/user/eyigitba/emtf/matchedNtuples/matchedNtuple_DisplacedMuGun_flatPt2to1000_allEndcap_flatXYZEtaPhi_11_3_0_pre5_NNv6_20M.root";
  // TString ntuple = "/eos/cms/store/user/eyigitba/emtf/matchedNtuples/matchedNtuple_DisplacedMuGun_flatPt2to1000_allEndcap_flatXYZEtaPhi_11_3_0_pre5_NNv7_2M.root";
  // TString ntuple = "/eos/cms/store/user/eyigitba/emtf/matchedNtuples/matchedNtuple_HTo2LongLivedTo4mu_combined_NNv6.root";
  TString ntuple = "/eos/cms/store/user/eyigitba/emtf/matchedNtuples/matchedNtuple_DisplacedMuGun_flatPt2to1000_allEndcap_flatXYZEtaPhi_11_3_0_pre5_NNv8_1M.root";

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

  TH1F *h_den_pt = new TH1F("h_den_pt", "", 30, 0, 60);
  TH1F *h_num_pt = new TH1F("h_num_pt", "", 30, 0, 60);
  TH1F *h_num_ptNN = new TH1F("h_num_ptNN", "", 30, 0, 60);
  TH1F *h_num_ptComb = new TH1F("h_num_ptComb", "", 30, 0, 60);

  TH1F *h_den_d0 = new TH1F("h_den_d0", "", 60, -120, 120);
  TH1F *h_num_d0 = new TH1F("h_num_d0", "", 60, -120, 120);
  TH1F *h_num_d0NN = new TH1F("h_num_d0NN", "", 60, -120, 120);
  TH1F *h_num_d0Comb = new TH1F("h_num_d0Comb", "", 60, -120, 120);

  TH1F *h_den_dxy = new TH1F("h_den_dxy", "", 30, 0, 120);
  TH1F *h_num_dxy = new TH1F("h_num_dxy", "", 30, 0, 120);
  TH1F *h_num_dxyNN = new TH1F("h_num_dxyNN", "", 30, 0, 120);
  TH1F *h_num_dxyComb = new TH1F("h_num_dxyComb", "", 30, 0, 120);

  TH1F *h_den_phi = new TH1F("h_den_phi", "", 64, -4, 4);
  TH1F *h_num_phi = new TH1F("h_num_phi", "", 64, -4, 4);
  TH1F *h_num_phiNN = new TH1F("h_num_phiNN", "", 64, -4, 4);
  TH1F *h_num_phiComb = new TH1F("h_num_phiComb", "", 64, -4, 4);

  TH1F *h_den_Lxy = new TH1F("h_den_Lxy", "", 60, 0, 240);
  TH1F *h_num_Lxy = new TH1F("h_num_Lxy", "", 60, 0, 240);
  TH1F *h_num_LxyNN = new TH1F("h_num_LxyNN", "", 60, 0, 240);
  TH1F *h_num_LxyComb = new TH1F("h_num_LxyComb", "", 60, 0, 240);

  Double_t etaArray[15] = {-2.5, -2.1, -1.6, -1.2, -0.9, -0.3, -0.2, 0, 0.2, 0.3, 0.9, 1.2, 1.6, 2.1, 2.5};
  Double_t etaArray2[21] = {-2.5, -2.3, -2.1, -1.85, -1.6, -1.4, -1.2, -0.9, -0.3, -0.2, 0, 0.2, 0.3, 0.9, 1.2, 1.4, 1.6, 1.85, 2.1, 2.3, 2.5};

  TH1F *h_den_eta = new TH1F("h_den_eta", "", 14, etaArray);
  TH1F *h_num_eta = new TH1F("h_num_eta", "", 14, etaArray);
  TH1F *h_num_etaNN = new TH1F("h_num_etaNN", "", 14, etaArray);
  TH1F *h_num_etaComb = new TH1F("h_num_etaComb", "", 14, etaArray);

  TH1F *h_den_fine_eta = new TH1F("h_den_fine_eta", "", 20, etaArray2);
  TH1F *h_num_fine_eta = new TH1F("h_num_fine_eta", "", 20, etaArray2);
  TH1F *h_num_fine_etaNN = new TH1F("h_num_fine_etaNN", "", 20, etaArray2);
  TH1F *h_num_fine_etaComb = new TH1F("h_num_fine_etaComb", "", 20, etaArray2);

  TH1F *h_den_fine_fine_eta = new TH1F("h_den_fine_fine_eta", "", 50, -2.5, 2.5);
  TH1F *h_num_fine_fine_eta = new TH1F("h_num_fine_fine_eta", "", 50, -2.5, 2.5);
  TH1F *h_num_fine_fine_etaNN = new TH1F("h_num_fine_fine_etaNN", "", 50, -2.5, 2.5);
  TH1F *h_num_fine_fine_etaComb = new TH1F("h_num_fine_fine_etaComb", "", 50, -2.5, 2.5);

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


  // 2D plots
  TH2F *h_den_d0_z0 = new TH2F("h_den_d0_z0", "", 100, 0, 100, 100, 0, 100);
  TH2F *h_num_d0_z0 = new TH2F("h_num_d0_z0", "", 100, 0, 100, 100, 0, 100);
  TH2F *h_num_d0_z0NN = new TH2F("h_num_d0_z0NN", "", 100, 0, 100, 100, 0, 100);
  TH2F *h_num_d0_z0Comb = new TH2F("h_num_d0_z0Comb", "", 100, 0, 100, 100, 0, 100);

  TH2F *h_den_d0_z0_eta1 = new TH2F("h_den_d0_z0_eta1", "", 100, 0, 100, 100, 0, 100);
  TH2F *h_num_d0_z0_eta1 = new TH2F("h_num_d0_z0_eta1", "", 100, 0, 100, 100, 0, 100);
  TH2F *h_num_d0_z0NN_eta1 = new TH2F("h_num_d0_z0NN_eta1", "", 100, 0, 100, 100, 0, 100);
  TH2F *h_num_d0_z0Comb_eta1 = new TH2F("h_num_d0_z0Comb_eta1", "", 100, 0, 100, 100, 0, 100);

  TH2F *h_den_d0_z0_eta2 = new TH2F("h_den_d0_z0_eta2", "", 100, 0, 100, 100, 0, 100);
  TH2F *h_num_d0_z0_eta2 = new TH2F("h_num_d0_z0_eta2", "", 100, 0, 100, 100, 0, 100);
  TH2F *h_num_d0_z0NN_eta2 = new TH2F("h_num_d0_z0NN_eta2", "", 100, 0, 100, 100, 0, 100);
  TH2F *h_num_d0_z0Comb_eta2 = new TH2F("h_num_d0_z0Comb_eta2", "", 100, 0, 100, 100, 0, 100);

  TH2F *h_den_d0_z0_eta3 = new TH2F("h_den_d0_z0_eta3", "", 100, 0, 100, 100, 0, 100);
  TH2F *h_num_d0_z0_eta3 = new TH2F("h_num_d0_z0_eta3", "", 100, 0, 100, 100, 0, 100);
  TH2F *h_num_d0_z0NN_eta3 = new TH2F("h_num_d0_z0NN_eta3", "", 100, 0, 100, 100, 0, 100);
  TH2F *h_num_d0_z0Comb_eta3 = new TH2F("h_num_d0_z0Comb_eta3", "", 100, 0, 100, 100, 0, 100);


  // Single muon efficiencies 

  int eventCount = 0;
  while(ccReader.Next()){
    eventCount++;
    if (eventCount % 1000 == 0) std::cout << eventCount << " events read!" << std::endl;
    // if (eventCount == 10000) break;
    for(int i=0; i<genMuPt.GetSize(); i++){
      // if (gendR[i] > dRThreshold) continue;
      if (abs(genMuEtaStar[i]) < etaThresholdMin) continue;
      if (abs(genMuEtaStar[i]) > etaThresholdMax) continue;
      if (abs(genMuVz[i]) > z0Threshold) continue;
      if (abs(genMuD0[i]) > dxyThresholdMax) continue;
      // if (abs(gendxy[i]) < dxyThresholdMin) continue;
      if (genMuMatchedL1MuID[i] < 0) continue;
      if (!(l1MuEmtfMode[genMuMatchedL1MuID[i]] == 11 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 13 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 14 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 15)) continue;


      h_den_pt->Fill(genMuPt[i]);

      if (genMuPt[i] > (ptThreshold + 5.0)){
        h_den_eta->Fill(genMuEtaStar[i]);
        h_den_fine_eta->Fill(genMuEtaStar[i]);
        h_den_fine_fine_eta->Fill(genMuEtaStar[i]);
        h_den_dxy->Fill(abs(genMuD0[i]));
        h_den_d0->Fill(genMuD0[i]);
        h_den_phi->Fill(genMuPhiStar[i]);
        h_den_Lxy->Fill(genMuLxy[i]);
        h_den_d0_z0->Fill(abs(genMuD0[i]),abs(genMuVz[i]));
      }

      if (genMuDR[i] > 0.6) continue;
      if (l1MuPt[genMuMatchedL1MuID[i]] < 0) continue;
      // if (!(l1MuEmtfMode[genMuMatchedL1MuID[i]] == 11 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 13 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 14 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 15)) continue;
      
      if (l1MuPt[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_pt->Fill(genMuPt[i]);
        if (genMuPt[i] > (ptThreshold + 5.0)){
          h_num_eta->Fill(genMuEtaStar[i]);
          h_num_fine_eta->Fill(genMuEtaStar[i]);
          h_num_fine_fine_eta->Fill(genMuEtaStar[i]);
          h_num_dxy->Fill(abs(genMuD0[i]));
          h_num_d0->Fill(genMuD0[i]);
          h_num_phi->Fill(genMuPhiStar[i]);
          h_num_Lxy->Fill(genMuLxy[i]);
          h_num_d0_z0->Fill(abs(genMuD0[i]),abs(genMuVz[i]));
        }

      }

      if (l1MuPtDxy[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_ptNN->Fill(genMuPt[i]);
        if (genMuPt[i] > (ptThreshold + 5.0)){
          h_num_etaNN->Fill(genMuEtaStar[i]);
          h_num_fine_etaNN->Fill(genMuEtaStar[i]);
          h_num_fine_fine_etaNN->Fill(genMuEtaStar[i]);
          h_num_dxyNN->Fill(abs(genMuD0[i]));
          h_num_d0NN->Fill(genMuD0[i]);
          h_num_phiNN->Fill(genMuPhiStar[i]);
          h_num_LxyNN->Fill(genMuLxy[i]);
          h_num_d0_z0NN->Fill(abs(genMuD0[i]),abs(genMuVz[i]));
        }

      }

      if (l1MuPtDxy[genMuMatchedL1MuID[i]] > ptThreshold || l1MuPt[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_ptComb->Fill(genMuPt[i]);
        if (genMuPt[i] > (ptThreshold + 5.0)){
          h_num_etaComb->Fill(genMuEtaStar[i]);
          h_num_fine_etaComb->Fill(genMuEtaStar[i]);
          h_num_fine_fine_etaComb->Fill(genMuEtaStar[i]);
          h_num_dxyComb->Fill(abs(genMuD0[i]));
          h_num_d0Comb->Fill(genMuD0[i]);
          h_num_phiComb->Fill(genMuPhiStar[i]);
          h_num_LxyComb->Fill(genMuLxy[i]);
          h_num_d0_z0Comb->Fill(abs(genMuD0[i]),abs(genMuVz[i]));
        }

      }
    }


    // 2.1 < eta < 2.5
    for(int i=0; i<genMuPt.GetSize(); i++){
      // if (gendR[i] > dRThreshold) continue;
      if (abs(genMuEtaStar[i]) < 2.1) continue;
      if (abs(genMuEtaStar[i]) > etaThresholdMax) continue;
      if (abs(genMuVz[i]) > z0Threshold) continue;
      if (abs(genMuD0[i]) > dxyThresholdMax) continue;
      // if (abs(gendxy[i]) < dxyThresholdMin) continue;
      if (genMuMatchedL1MuID[i] < 0) continue;
      if (!(l1MuEmtfMode[genMuMatchedL1MuID[i]] == 11 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 13 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 14 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 15)) continue;


      h_den_pt_eta3->Fill(genMuPt[i]);

      if (genMuPt[i] > (ptThreshold + 5.0)){
        h_den_dxy_eta3->Fill(abs(genMuD0[i]));
        h_den_d0_eta3->Fill(genMuD0[i]);
        h_den_phi_eta3->Fill(genMuPhiStar[i]);
        h_den_Lxy_eta3->Fill(genMuLxy[i]);
        h_den_d0_z0_eta3->Fill(abs(genMuD0[i]),abs(genMuVz[i]));
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
          h_num_d0_z0_eta3->Fill(abs(genMuD0[i]),abs(genMuVz[i]));
        }

      }

      if (l1MuPtDxy[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_ptNN_eta3->Fill(genMuPt[i]);
        if (genMuPt[i] > (ptThreshold + 5.0)){
          h_num_dxyNN_eta3->Fill(abs(genMuD0[i]));
          h_num_d0NN_eta3->Fill(genMuD0[i]);
          h_num_phiNN_eta3->Fill(genMuPhiStar[i]);
          h_num_LxyNN_eta3->Fill(genMuLxy[i]);
          h_num_d0_z0NN_eta3->Fill(abs(genMuD0[i]),abs(genMuVz[i]));
        }

      }

      if (l1MuPtDxy[genMuMatchedL1MuID[i]] > ptThreshold || l1MuPt[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_ptComb_eta3->Fill(genMuPt[i]);
        if (genMuPt[i] > (ptThreshold + 5.0)){
          h_num_dxyComb_eta3->Fill(abs(genMuD0[i]));
          h_num_d0Comb_eta3->Fill(genMuD0[i]);
          h_num_phiComb_eta3->Fill(genMuPhiStar[i]);
          h_num_LxyComb_eta3->Fill(genMuLxy[i]);
          h_num_d0_z0Comb_eta3->Fill(abs(genMuD0[i]),abs(genMuVz[i]));
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
      if (genMuMatchedL1MuID[i] < 0) continue;
      if (!(l1MuEmtfMode[genMuMatchedL1MuID[i]] == 11 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 13 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 14 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 15)) continue;


      h_den_pt_eta2->Fill(genMuPt[i]);

      if (genMuPt[i] > (ptThreshold + 5.0)){
        h_den_dxy_eta2->Fill(abs(genMuD0[i]));
        h_den_d0_eta2->Fill(genMuD0[i]);
        h_den_phi_eta2->Fill(genMuPhiStar[i]);
        h_den_Lxy_eta2->Fill(genMuLxy[i]);
        h_den_d0_z0_eta2->Fill(abs(genMuD0[i]),abs(genMuVz[i]));
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
          h_num_d0_z0_eta2->Fill(abs(genMuD0[i]),abs(genMuVz[i]));
        }

      }

      if (l1MuPtDxy[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_ptNN_eta2->Fill(genMuPt[i]);
        if (genMuPt[i] > (ptThreshold + 5.0)){
          h_num_dxyNN_eta2->Fill(abs(genMuD0[i]));
          h_num_d0NN_eta2->Fill(genMuD0[i]);
          h_num_phiNN_eta2->Fill(genMuPhiStar[i]);
          h_num_LxyNN_eta2->Fill(genMuLxy[i]);
          h_num_d0_z0NN_eta2->Fill(abs(genMuD0[i]),abs(genMuVz[i]));
        }

      }

      if (l1MuPtDxy[genMuMatchedL1MuID[i]] > ptThreshold || l1MuPt[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_ptComb_eta2->Fill(genMuPt[i]);
        if (genMuPt[i] > (ptThreshold + 5.0)){
          h_num_dxyComb_eta2->Fill(abs(genMuD0[i]));
          h_num_d0Comb_eta2->Fill(genMuD0[i]);
          h_num_phiComb_eta2->Fill(genMuPhiStar[i]);
          h_num_LxyComb_eta2->Fill(genMuLxy[i]);
          h_num_d0_z0Comb_eta2->Fill(abs(genMuD0[i]),abs(genMuVz[i]));
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
      if (genMuMatchedL1MuID[i] < 0) continue;
      if (!(l1MuEmtfMode[genMuMatchedL1MuID[i]] == 11 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 13 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 14 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 15)) continue;


      h_den_pt_eta1->Fill(genMuPt[i]);

      if (genMuPt[i] > (ptThreshold + 5.0)){
        h_den_dxy_eta1->Fill(abs(genMuD0[i]));
        h_den_d0_eta1->Fill(genMuD0[i]);
        h_den_phi_eta1->Fill(genMuPhiStar[i]);
        h_den_Lxy_eta1->Fill(genMuLxy[i]);
        h_den_d0_z0_eta1->Fill(abs(genMuD0[i]),abs(genMuVz[i]));
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
          h_num_d0_z0_eta1->Fill(abs(genMuD0[i]),abs(genMuVz[i]));
        }

      }

      if (l1MuPtDxy[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_ptNN_eta1->Fill(genMuPt[i]);
        if (genMuPt[i] > (ptThreshold + 5.0)){
          h_num_dxyNN_eta1->Fill(abs(genMuD0[i]));
          h_num_d0NN_eta1->Fill(genMuD0[i]);
          h_num_phiNN_eta1->Fill(genMuPhiStar[i]);
          h_num_LxyNN_eta1->Fill(genMuLxy[i]);
          h_num_d0_z0NN_eta1->Fill(abs(genMuD0[i]),abs(genMuVz[i]));
        }

      }

      if (l1MuPtDxy[genMuMatchedL1MuID[i]] > ptThreshold || l1MuPt[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_ptComb_eta1->Fill(genMuPt[i]);
        if (genMuPt[i] > (ptThreshold + 5.0)){
          h_num_dxyComb_eta1->Fill(abs(genMuD0[i]));
          h_num_d0Comb_eta1->Fill(genMuD0[i]);
          h_num_phiComb_eta1->Fill(genMuPhiStar[i]);
          h_num_LxyComb_eta1->Fill(genMuLxy[i]);
          h_num_d0_z0Comb_eta1->Fill(abs(genMuD0[i]),abs(genMuVz[i]));
        }

      }
    }
    


  }


  // Dimuon efficiencies
  // while(ccReader.Next()){
  //   int count =0;
  //   vector<int> k;
  //   k.clear();

  //   for(int i=0; i<gendimupt.GetSize(); i++){

  //     if (gendimudxy[i] > 120) continue;
  //     if (gendimudxy[i] < 25) continue;
  //     if (gendimuvz[i] > 120) continue;
      
  //     int daughter1 = -99;
  //     int daughter2 = -99;
  //     for (int j=0; j<genidx.GetSize(); j++){
  //       if (genidx[j] == gendimudaughter1[i]) daughter1 = j;
  //       else if (genidx[j] == gendimudaughter2[i]) daughter2 = j;
  //     }

  //     if (abs(genetaStar[daughter1]) < etaThresholdMin || abs(genetaStar[daughter1]) > etaThresholdMax) continue;
  //     if (abs(genetaStar[daughter2]) < etaThresholdMin || abs(genetaStar[daughter2]) > etaThresholdMax) continue;

  //     h_den_pt->Fill(gendimupt[i]);

  //     if (gendimupt[i] > 10.0){
  //       h_den_eta->Fill(gendimueta[i]);
  //       h_den_dxy->Fill(gendimudxy[i]);
  //     }
    

  //     // dimu case
  //     if (genMatchedL1MuID[daughter1] < 0 || genMatchedL1MuID[daughter2] < 0) continue;
  //     if (genMatchedL1MuID[daughter1] == genMatchedL1MuID[daughter2]) continue;
  //     if (abs(l1eta[genMatchedL1MuID[daughter1]]) < etaThresholdMin) continue;
  //     if (abs(l1eta[genMatchedL1MuID[daughter2]]) < etaThresholdMin) continue;
  //     if (gendR[daughter1] > 0.6 || gendR[daughter2] > 0.6) continue;
  //     if (l1charge[daughter1] == l1charge[daughter2]) continue;
  //     // if (l1pt[genMatchedL1MuID[daughter1]] < ptThreshold || l1pt[genMatchedL1MuID[daughter2]] < ptThreshold) continue;
  //     // if (l1ptUnc[genMatchedL1MuID[daughter1]] < ptThreshold || l1ptUnc[genMatchedL1MuID[daughter2]] < ptThreshold) continue;
  //     // if (l1pt[genMatchedL1MuID[daughter1]] < ptThreshold && l1ptUnc[genMatchedL1MuID[daughter1]] < ptThreshold) continue;
  //     // if (l1pt[genMatchedL1MuID[daughter2]] < ptThreshold && l1ptUnc[genMatchedL1MuID[daughter2]] < ptThreshold) continue;

  //     // if (l1pt[genMatchedL1MuID[daughter1]] < ptThreshold && l1pt[genMatchedL1MuID[daughter2]] < ptThreshold) continue;
  //     // if (l1pt[genMatchedL1MuID[daughter1]] < ptThreshold2 || l1pt[genMatchedL1MuID[daughter2]] < ptThreshold2) continue;

  //     if (l1ptUnc[genMatchedL1MuID[daughter1]] < ptThreshold && l1ptUnc[genMatchedL1MuID[daughter2]] < ptThreshold) continue;
  //     if (l1ptUnc[genMatchedL1MuID[daughter1]] < ptThreshold2 || l1ptUnc[genMatchedL1MuID[daughter2]] < ptThreshold2) continue;

  //     // std::cout << l1pt[genMatchedL1MuID[daughter1]] << " " << l1pt[genMatchedL1MuID[daughter2]] << std::endl;

  //     h_num_pt->Fill(gendimupt[i]);
  //     if (gendimupt[i] < 10.0) continue;

  //     h_num_eta->Fill(gendimueta[i]);
  //     h_num_dxy->Fill(gendimudxy[i]);


  //     // single mu case 
  //     // if (genMatchedL1MuID[daughter1] < 0 && genMatchedL1MuID[daughter2] < 0) continue;
  //     // if (abs(l1eta[genMatchedL1MuID[daughter1]]) < etaThresholdMin) continue;
  //     // if (abs(l1eta[genMatchedL1MuID[daughter2]]) < etaThresholdMin) continue;
  //     // if (gendR[daughter1] > 0.6 || gendR[daughter2] > 0.6) continue;
  //     // if (l1charge[daughter1] == l1charge[daughter2]) continue;
  //     // // if (l1pt[genMatchedL1MuID[daughter1]] < ptThreshold || l1pt[genMatchedL1MuID[daughter2]] < ptThreshold) continue;
  //     // // if (l1ptUnc[genMatchedL1MuID[daughter1]] < ptThreshold || l1ptUnc[genMatchedL1MuID[daughter2]] < ptThreshold) continue;
  //     // // if (l1pt[genMatchedL1MuID[daughter1]] < ptThreshold && l1ptUnc[genMatchedL1MuID[daughter1]] < ptThreshold) continue;
  //     // // if (l1pt[genMatchedL1MuID[daughter2]] < ptThreshold && l1ptUnc[genMatchedL1MuID[daughter2]] < ptThreshold) continue;

  //     // // if (l1pt[genMatchedL1MuID[daughter1]] < ptThreshold && l1pt[genMatchedL1MuID[daughter2]] < ptThreshold) continue;
  //     // // if (l1pt[genMatchedL1MuID[daughter1]] < ptThreshold2 || l1pt[genMatchedL1MuID[daughter2]] < ptThreshold2) continue;

  //     // if (l1ptUnc[genMatchedL1MuID[daughter1]] < ptThreshold && l1ptUnc[genMatchedL1MuID[daughter2]] < ptThreshold) continue;
  //     // if (l1ptUnc[genMatchedL1MuID[daughter1]] < ptThreshold2 || l1ptUnc[genMatchedL1MuID[daughter2]] < ptThreshold2) continue;

  //     // // std::cout << l1pt[genMatchedL1MuID[daughter1]] << " " << l1pt[genMatchedL1MuID[daughter2]] << std::endl;

  //     // h_num_pt->Fill(gendimupt[i]);
  //     // if (gendimupt[i] < 10.0) continue;

  //     // h_num_eta->Fill(gendimueta[i]);
  //     // h_num_dxy->Fill(gendimudxy[i]);


  //   }

  // }






  // Divide histograms
  TGraphAsymmErrors * error_pt = new TGraphAsymmErrors(h_num_pt,h_den_pt);
  TGraphAsymmErrors * error_ptNN = new TGraphAsymmErrors(h_num_ptNN,h_den_pt);
  TGraphAsymmErrors * error_ptComb = new TGraphAsymmErrors(h_num_ptComb,h_den_pt);

  TGraphAsymmErrors * error_d0 = new TGraphAsymmErrors(h_num_d0,h_den_d0);
  TGraphAsymmErrors * error_d0NN = new TGraphAsymmErrors(h_num_d0NN,h_den_d0);
  TGraphAsymmErrors * error_d0Comb = new TGraphAsymmErrors(h_num_d0Comb,h_den_d0);

  TGraphAsymmErrors * error_dxy = new TGraphAsymmErrors(h_num_dxy,h_den_dxy);
  TGraphAsymmErrors * error_dxyNN = new TGraphAsymmErrors(h_num_dxyNN,h_den_dxy);
  TGraphAsymmErrors * error_dxyComb = new TGraphAsymmErrors(h_num_dxyComb,h_den_dxy);

  TGraphAsymmErrors * error_eta = new TGraphAsymmErrors(h_num_eta,h_den_eta);
  TGraphAsymmErrors * error_etaNN = new TGraphAsymmErrors(h_num_etaNN,h_den_eta);
  TGraphAsymmErrors * error_etaComb = new TGraphAsymmErrors(h_num_etaComb,h_den_eta);

  TGraphAsymmErrors * error_fine_eta = new TGraphAsymmErrors(h_num_fine_eta,h_den_fine_eta);
  TGraphAsymmErrors * error_fine_etaNN = new TGraphAsymmErrors(h_num_fine_etaNN,h_den_fine_eta);
  TGraphAsymmErrors * error_fine_etaComb = new TGraphAsymmErrors(h_num_fine_etaComb,h_den_fine_eta);

  TGraphAsymmErrors * error_fine_fine_eta = new TGraphAsymmErrors(h_num_fine_fine_eta,h_den_fine_fine_eta);
  TGraphAsymmErrors * error_fine_fine_etaNN = new TGraphAsymmErrors(h_num_fine_fine_etaNN,h_den_fine_fine_eta);
  TGraphAsymmErrors * error_fine_fine_etaComb = new TGraphAsymmErrors(h_num_fine_fine_etaComb,h_den_fine_fine_eta);

  TGraphAsymmErrors * error_phi = new TGraphAsymmErrors(h_num_phi,h_den_phi);
  TGraphAsymmErrors * error_phiNN = new TGraphAsymmErrors(h_num_phiNN,h_den_phi);
  TGraphAsymmErrors * error_phiComb = new TGraphAsymmErrors(h_num_phiComb,h_den_phi);

  TGraphAsymmErrors * error_Lxy = new TGraphAsymmErrors(h_num_Lxy,h_den_Lxy);
  TGraphAsymmErrors * error_LxyNN = new TGraphAsymmErrors(h_num_LxyNN,h_den_Lxy);
  TGraphAsymmErrors * error_LxyComb = new TGraphAsymmErrors(h_num_LxyComb,h_den_Lxy);

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

  // 2D plots
  // TGraphAsymmErrors * error_d0_z0 = new TGraphAsymmErrors(h_num_d0_z0,h_den_d0_z0);
  // TGraphAsymmErrors * error_d0_z0NN = new TGraphAsymmErrors(h_num_d0_z0NN,h_den_d0_z0);
  // TGraphAsymmErrors * error_d0_z0Comb = new TGraphAsymmErrors(h_num_d0_z0Comb,h_den_d0_z0);

  // TGraphAsymmErrors * error_d0_z0_eta3 = new TGraphAsymmErrors(h_num_d0_z0_eta3,h_den_d0_z0_eta3);
  // TGraphAsymmErrors * error_d0_z0NN_eta3 = new TGraphAsymmErrors(h_num_d0_z0NN_eta3,h_den_d0_z0_eta3);
  // TGraphAsymmErrors * error_d0_z0Comb_eta3 = new TGraphAsymmErrors(h_num_d0_z0Comb_eta3,h_den_d0_z0_eta3);

  // TGraphAsymmErrors * error_d0_z0_eta2 = new TGraphAsymmErrors(h_num_d0_z0_eta2,h_den_d0_z0_eta2);
  // TGraphAsymmErrors * error_d0_z0NN_eta2 = new TGraphAsymmErrors(h_num_d0_z0NN_eta2,h_den_d0_z0_eta2);
  // TGraphAsymmErrors * error_d0_z0Comb_eta2 = new TGraphAsymmErrors(h_num_d0_z0Comb_eta2,h_den_d0_z0_eta2);

  // TGraphAsymmErrors * error_d0_z0_eta1 = new TGraphAsymmErrors(h_num_d0_z0_eta1,h_den_d0_z0_eta1);
  // TGraphAsymmErrors * error_d0_z0NN_eta1 = new TGraphAsymmErrors(h_num_d0_z0NN_eta1,h_den_d0_z0_eta1);
  // TGraphAsymmErrors * error_d0_z0Comb_eta1 = new TGraphAsymmErrors(h_num_d0_z0Comb_eta1,h_den_d0_z0_eta1);

  TH2F* error_d0_z0 = new TH2F("error_d0_z0","",100, 0, 100, 100, 0, 100);
  TH2F* error_d0_z0NN = new TH2F("error_d0_z0NN","",100, 0, 100, 100, 0, 100);
  TH2F* error_d0_z0Comb = new TH2F("error_d0_z0Comb","",100, 0, 100, 100, 0, 100);

  TH2F* error_d0_z0_eta1 = new TH2F("error_d0_z0_eta1","",100, 0, 100, 100, 0, 100);
  TH2F* error_d0_z0NN_eta1 = new TH2F("error_d0_z0NN_eta1","",100, 0, 100, 100, 0, 100);
  TH2F* error_d0_z0Comb_eta1 = new TH2F("error_d0_z0Comb_eta1","",100, 0, 100, 100, 0, 100);

  TH2F* error_d0_z0_eta2 = new TH2F("error_d0_z0_eta2","",100, 0, 100, 100, 0, 100);
  TH2F* error_d0_z0NN_eta2 = new TH2F("error_d0_z0NN_eta2","",100, 0, 100, 100, 0, 100);
  TH2F* error_d0_z0Comb_eta2 = new TH2F("error_d0_z0Comb_eta2","",100, 0, 100, 100, 0, 100);

  TH2F* error_d0_z0_eta3 = new TH2F("error_d0_z0_eta3","",100, 0, 100, 100, 0, 100);
  TH2F* error_d0_z0NN_eta3 = new TH2F("error_d0_z0NN_eta3","",100, 0, 100, 100, 0, 100);
  TH2F* error_d0_z0Comb_eta3 = new TH2F("error_d0_z0Comb_eta3","",100, 0, 100, 100, 0, 100);

  error_d0_z0 = (TH2F*)h_num_d0_z0->Clone();
  error_d0_z0->Divide(h_den_d0_z0);

  error_d0_z0NN = (TH2F*)h_num_d0_z0NN->Clone();
  error_d0_z0NN->Divide(h_den_d0_z0);

  error_d0_z0Comb = (TH2F*)h_num_d0_z0Comb->Clone();
  error_d0_z0Comb->Divide(h_den_d0_z0);

  error_d0_z0_eta1 = (TH2F*)h_num_d0_z0_eta1->Clone();
  error_d0_z0_eta1->Divide(h_den_d0_z0_eta1);

  error_d0_z0NN_eta1 = (TH2F*)h_num_d0_z0NN_eta1->Clone();
  error_d0_z0NN_eta1->Divide(h_den_d0_z0_eta1);

  error_d0_z0Comb_eta1 = (TH2F*)h_num_d0_z0Comb_eta1->Clone();
  error_d0_z0Comb_eta1->Divide(h_den_d0_z0_eta1);

  error_d0_z0_eta2 = (TH2F*)h_num_d0_z0_eta2->Clone();
  error_d0_z0_eta2->Divide(h_den_d0_z0_eta2);

  error_d0_z0NN_eta2 = (TH2F*)h_num_d0_z0NN_eta2->Clone();
  error_d0_z0NN_eta2->Divide(h_den_d0_z0_eta2);

  error_d0_z0Comb_eta2 = (TH2F*)h_num_d0_z0Comb_eta2->Clone();
  error_d0_z0Comb_eta2->Divide(h_den_d0_z0_eta2);

  error_d0_z0_eta3 = (TH2F*)h_num_d0_z0_eta3->Clone();
  error_d0_z0_eta3->Divide(h_den_d0_z0_eta3);

  error_d0_z0NN_eta3 = (TH2F*)h_num_d0_z0NN_eta3->Clone();
  error_d0_z0NN_eta3->Divide(h_den_d0_z0_eta3);

  error_d0_z0Comb_eta3 = (TH2F*)h_num_d0_z0Comb_eta3->Clone();
  error_d0_z0Comb_eta3->Divide(h_den_d0_z0_eta3);

  // canvasname.push_back("out_ptUnc_vs_dxy_wME11a"); 
  TString titlePt="Gen Muon p_{T} [GeV]";
  TString titleDxy="Gen Muon d_{xy} [cm]";
  TString titleLxy="Gen Muon Lxy [cm]";
  TString titleEta="Gen Muon #eta";
  TString titlePhi="Gen Muon #phi";
  TString titleZ0="Gen muon Z_{0} [cm]";

  error_pt->GetXaxis()->SetTitle(titlePt);
  error_pt->GetYaxis()->SetTitle("L1T Efficiency");
  error_pt->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_d0->GetXaxis()->SetTitle(titleDxy);
  error_d0->GetYaxis()->SetTitle("L1T Efficiency");
  error_d0->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_dxy->GetXaxis()->SetTitle(titleDxy);
  error_dxy->GetYaxis()->SetTitle("L1T Efficiency");
  error_dxy->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_eta->GetXaxis()->SetTitle(titleEta);
  error_eta->GetYaxis()->SetTitle("L1T Efficiency");
  error_eta->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_fine_eta->GetXaxis()->SetTitle(titleEta);
  error_fine_eta->GetYaxis()->SetTitle("L1T Efficiency");
  error_fine_eta->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_fine_fine_eta->GetXaxis()->SetTitle(titleEta);
  error_fine_fine_eta->GetYaxis()->SetTitle("L1T Efficiency");
  error_fine_fine_eta->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_phi->GetXaxis()->SetTitle(titlePhi);
  error_phi->GetYaxis()->SetTitle("L1T Efficiency");
  error_phi->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_Lxy->GetXaxis()->SetTitle(titleLxy);
  error_Lxy->GetYaxis()->SetTitle("L1T Efficiency");
  error_Lxy->GetYaxis()->SetRangeUser(0.00001,1.2);

  // eta binned
  error_pt_eta3->GetXaxis()->SetTitle(titlePt);
  error_pt_eta3->GetYaxis()->SetTitle("L1T Efficiency");
  error_pt_eta3->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_d0_eta3->GetXaxis()->SetTitle(titleDxy);
  error_d0_eta3->GetYaxis()->SetTitle("L1T Efficiency");
  error_d0_eta3->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_dxy_eta3->GetXaxis()->SetTitle(titleDxy);
  error_dxy_eta3->GetYaxis()->SetTitle("L1T Efficiency");
  error_dxy_eta3->GetYaxis()->SetRangeUser(0.00001,1.2);

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

  error_d0_eta2->GetXaxis()->SetTitle(titleDxy);
  error_d0_eta2->GetYaxis()->SetTitle("L1T Efficiency");
  error_d0_eta2->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_dxy_eta2->GetXaxis()->SetTitle(titleDxy);
  error_dxy_eta2->GetYaxis()->SetTitle("L1T Efficiency");
  error_dxy_eta2->GetYaxis()->SetRangeUser(0.00001,1.2);

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

  error_ptNN_eta1->GetXaxis()->SetTitle(titlePt);
  error_ptNN_eta1->GetYaxis()->SetTitle("L1T Efficiency");
  error_ptNN_eta1->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_d0_eta1->GetXaxis()->SetTitle(titleDxy);
  error_d0_eta1->GetYaxis()->SetTitle("L1T Efficiency");
  error_d0_eta1->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_dxy_eta1->GetXaxis()->SetTitle(titleDxy);
  error_dxy_eta1->GetYaxis()->SetTitle("L1T Efficiency");
  error_dxy_eta1->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_dxyNN_eta1->GetXaxis()->SetTitle(titleDxy);
  error_dxyNN_eta1->GetYaxis()->SetTitle("L1T Efficiency");
  error_dxyNN_eta1->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_phi_eta1->GetXaxis()->SetTitle(titlePhi);
  error_phi_eta1->GetYaxis()->SetTitle("L1T Efficiency");
  error_phi_eta1->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_Lxy_eta1->GetXaxis()->SetTitle(titleLxy);
  error_Lxy_eta1->GetYaxis()->SetTitle("L1T Efficiency");
  error_Lxy_eta1->GetYaxis()->SetRangeUser(0.00001,1.2);

  // 2D plots
  error_d0_z0->GetXaxis()->SetTitle(titleDxy);
  error_d0_z0->GetYaxis()->SetTitle(titleZ0);
  error_d0_z0->SetTitle("1.2 < |#eta| < 2.5");
  error_d0_z0->GetZaxis()->SetRangeUser(0.01,1);
  error_d0_z0->GetZaxis()->SetTitle("L1T Efficiency");

  error_d0_z0NN->GetXaxis()->SetTitle(titleDxy);
  error_d0_z0NN->GetYaxis()->SetTitle(titleZ0);
  error_d0_z0NN->SetTitle("1.2 < |#eta| < 2.5");
  error_d0_z0NN->GetZaxis()->SetRangeUser(0.01,1);
  error_d0_z0NN->GetZaxis()->SetTitle("L1T Efficiency");

  error_d0_z0Comb->GetXaxis()->SetTitle(titleDxy);
  error_d0_z0Comb->GetYaxis()->SetTitle(titleZ0);
  error_d0_z0Comb->SetTitle("1.2 < |#eta| < 2.5");
  error_d0_z0Comb->GetZaxis()->SetRangeUser(0.01,1);
  error_d0_z0Comb->GetZaxis()->SetTitle("L1T Efficiency");

  error_d0_z0_eta3->GetXaxis()->SetTitle(titleDxy);
  error_d0_z0_eta3->GetYaxis()->SetTitle(titleZ0);
  error_d0_z0_eta3->SetTitle("2.1 < |#eta| < 2.5");
  error_d0_z0_eta3->GetZaxis()->SetRangeUser(0.01,1);
  error_d0_z0_eta3->GetZaxis()->SetTitle("L1T Efficiency");

  error_d0_z0NN_eta3->GetXaxis()->SetTitle(titleDxy);
  error_d0_z0NN_eta3->GetYaxis()->SetTitle(titleZ0);
  error_d0_z0NN_eta3->SetTitle("2.1 < |#eta| < 2.5");
  error_d0_z0NN_eta3->GetZaxis()->SetRangeUser(0.01,1);
  error_d0_z0NN_eta3->GetZaxis()->SetTitle("L1T Efficiency");

  error_d0_z0Comb_eta3->GetXaxis()->SetTitle(titleDxy);
  error_d0_z0Comb_eta3->GetYaxis()->SetTitle(titleZ0);
  error_d0_z0Comb_eta3->SetTitle("2.1 < |#eta| < 2.5");
  error_d0_z0Comb_eta3->GetZaxis()->SetRangeUser(0.01,1);
  error_d0_z0Comb_eta3->GetZaxis()->SetTitle("L1T Efficiency");

  error_d0_z0_eta2->GetXaxis()->SetTitle(titleDxy);
  error_d0_z0_eta2->GetYaxis()->SetTitle(titleZ0);
  error_d0_z0_eta2->SetTitle("1.6 < |#eta| < 2.1");
  error_d0_z0_eta2->GetZaxis()->SetRangeUser(0.01,1);
  error_d0_z0_eta2->GetZaxis()->SetTitle("L1T Efficiency");

  error_d0_z0NN_eta2->GetXaxis()->SetTitle(titleDxy);
  error_d0_z0NN_eta2->GetYaxis()->SetTitle(titleZ0);
  error_d0_z0NN_eta2->SetTitle("1.6 < |#eta| < 2.1");
  error_d0_z0NN_eta2->GetZaxis()->SetRangeUser(0.01,1);
  error_d0_z0NN_eta2->GetZaxis()->SetTitle("L1T Efficiency");

  error_d0_z0Comb_eta2->GetXaxis()->SetTitle(titleDxy);
  error_d0_z0Comb_eta2->GetYaxis()->SetTitle(titleZ0);
  error_d0_z0Comb_eta2->SetTitle("1.6 < |#eta| < 2.1");
  error_d0_z0Comb_eta2->GetZaxis()->SetRangeUser(0.01,1);
  error_d0_z0Comb_eta2->GetZaxis()->SetTitle("L1T Efficiency");

  error_d0_z0_eta1->GetXaxis()->SetTitle(titleDxy);
  error_d0_z0_eta1->GetYaxis()->SetTitle(titleZ0);
  error_d0_z0_eta1->SetTitle("1.2 < |#eta| < 1.6");
  error_d0_z0_eta1->GetZaxis()->SetRangeUser(0.01,1);
  error_d0_z0_eta1->GetZaxis()->SetTitle("L1T Efficiency");

  error_d0_z0NN_eta1->GetXaxis()->SetTitle(titleDxy);
  error_d0_z0NN_eta1->GetYaxis()->SetTitle(titleZ0);
  error_d0_z0NN_eta1->SetTitle("1.2 < |#eta| < 1.6");
  error_d0_z0NN_eta1->GetZaxis()->SetRangeUser(0.01,1);
  error_d0_z0NN_eta1->GetZaxis()->SetTitle("L1T Efficiency");


  error_d0_z0Comb_eta1->GetXaxis()->SetTitle(titleDxy);
  error_d0_z0Comb_eta1->GetYaxis()->SetTitle(titleZ0);
  error_d0_z0Comb_eta1->SetTitle("1.2 < |#eta| < 1.6");
  error_d0_z0Comb_eta1->GetZaxis()->SetRangeUser(0.01,1);
  error_d0_z0Comb_eta1->GetZaxis()->SetTitle("L1T Efficiency");




  TString leg = "L1 pT (prompt) > 10 GeV";
  TString leg2 = "L1 pT (disp.) > 10 GeV";
  TString leg3 = "L1 pT (disp.) > 10 GeV || L1 pT (prompt) > 10 GeV";

  TString leg_etaFull = "1.2 < |#eta| < 2.5";
  TString leg_eta1 = "1.2 < |#eta| < 1.6";
  TString leg_eta2 = "1.6 < |#eta| < 2.1";
  TString leg_eta3 = "2.1 < |#eta| < 2.5";





  // kwds.push_back(options);
  errors.push_back(error_pt);
  errors.push_back(error_ptNN);
  errors.push_back(error_ptComb);
  errors.push_back(error_d0);
  errors.push_back(error_d0NN);
  errors.push_back(error_d0Comb);
  errors.push_back(error_dxy);
  errors.push_back(error_dxyNN);
  errors.push_back(error_dxyComb);
  errors.push_back(error_eta);
  errors.push_back(error_etaNN);
  errors.push_back(error_etaComb);
  errors.push_back(error_fine_eta);
  errors.push_back(error_fine_etaNN);
  errors.push_back(error_fine_etaComb);
  errors.push_back(error_fine_fine_eta);
  errors.push_back(error_fine_fine_etaNN);
  errors.push_back(error_fine_fine_etaComb);
  errors.push_back(error_phi);
  errors.push_back(error_phiNN);
  errors.push_back(error_phiComb);
  errors.push_back(error_Lxy);
  errors.push_back(error_LxyNN);
  errors.push_back(error_LxyComb);

  //eta binned
  errors.push_back(error_pt_eta3);
  errors.push_back(error_ptNN_eta3);
  errors.push_back(error_ptComb_eta3);
  errors.push_back(error_d0_eta3);
  errors.push_back(error_d0NN_eta3);
  errors.push_back(error_d0Comb_eta3);
  errors.push_back(error_dxy_eta3);
  errors.push_back(error_dxyNN_eta3);
  errors.push_back(error_dxyComb_eta3);
  errors.push_back(error_phi_eta3);
  errors.push_back(error_phiNN_eta3);
  errors.push_back(error_phiComb_eta3);
  errors.push_back(error_Lxy_eta3);
  errors.push_back(error_LxyNN_eta3);
  errors.push_back(error_LxyComb_eta3);

  //eta binned
  errors.push_back(error_pt_eta2);
  errors.push_back(error_ptNN_eta2);
  errors.push_back(error_ptComb_eta2);
  errors.push_back(error_d0_eta2);
  errors.push_back(error_d0NN_eta2);
  errors.push_back(error_d0Comb_eta2);
  errors.push_back(error_dxy_eta2);
  errors.push_back(error_dxyNN_eta2);
  errors.push_back(error_dxyComb_eta2);
  errors.push_back(error_phi_eta2);
  errors.push_back(error_phiNN_eta2);
  errors.push_back(error_phiComb_eta2);
  errors.push_back(error_Lxy_eta2);
  errors.push_back(error_LxyNN_eta2);
  errors.push_back(error_LxyComb_eta2);

  //eta binned
  errors.push_back(error_pt_eta1);
  errors.push_back(error_ptNN_eta1);
  errors.push_back(error_ptComb_eta1);
  errors.push_back(error_d0_eta1);
  errors.push_back(error_d0NN_eta1);
  errors.push_back(error_d0Comb_eta1);
  errors.push_back(error_dxy_eta1);
  errors.push_back(error_dxyNN_eta1);
  errors.push_back(error_dxyComb_eta1);
  errors.push_back(error_phi_eta1);
  errors.push_back(error_phiNN_eta1);
  errors.push_back(error_phiComb_eta1);
  errors.push_back(error_Lxy_eta1);
  errors.push_back(error_LxyNN_eta1);
  errors.push_back(error_LxyComb_eta1);

  // eta same plot
  errors.push_back(error_ptNN_eta1);
  errors.push_back(error_ptNN_eta2);
  errors.push_back(error_ptNN_eta3);
  errors.push_back(error_dxyNN_eta1);
  errors.push_back(error_dxyNN_eta2);
  errors.push_back(error_dxyNN_eta3);

  // 2D plots
  errors_2D.push_back(error_d0_z0);
  errors_2D.push_back(error_d0_z0NN);
  errors_2D.push_back(error_d0_z0Comb);
  errors_2D.push_back(error_d0_z0_eta3);
  errors_2D.push_back(error_d0_z0NN_eta3);
  errors_2D.push_back(error_d0_z0Comb_eta3);
  errors_2D.push_back(error_d0_z0_eta2);
  errors_2D.push_back(error_d0_z0NN_eta2);
  errors_2D.push_back(error_d0_z0Comb_eta2);
  errors_2D.push_back(error_d0_z0_eta1);
  errors_2D.push_back(error_d0_z0NN_eta1);
  errors_2D.push_back(error_d0_z0Comb_eta1);


  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg3);
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg3);
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg3);
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg3);
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg3);
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg3);
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg3);
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg3);

  // eta binned
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg3);
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg3);
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg3);
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg3);
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg3);

  // eta binned
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg3);
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg3);
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg3);
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg3);
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg3);

  // eta binned
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg3);
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg3);
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg3);
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg3);
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg3);

  // eta same plot
  legs.push_back(leg_eta1);
  legs.push_back(leg_eta2);
  legs.push_back(leg_eta3);
  legs.push_back(leg_eta1);
  legs.push_back(leg_eta2);
  legs.push_back(leg_eta3);

  // 2D plots
  legs_2D.push_back(leg_etaFull);
  legs_2D.push_back(leg_etaFull);
  legs_2D.push_back(leg_etaFull);
  legs_2D.push_back(leg_eta3);
  legs_2D.push_back(leg_eta3);
  legs_2D.push_back(leg_eta3);
  legs_2D.push_back(leg_eta2);
  legs_2D.push_back(leg_eta2);
  legs_2D.push_back(leg_eta2);
  legs_2D.push_back(leg_eta1);
  legs_2D.push_back(leg_eta1);
  legs_2D.push_back(leg_eta1);

  // delete h_den_pt;
  // delete h_num_pt;
  // delete h_num_ptNN;
  // delete h_num_ptNN;

  // delete h_den_dxy;
  // delete h_num_dxy;
  // delete h_num_dxyNN;
  // delete h_num_dxyNN;

  // delete h_den_d0;
  // delete h_num_d0;
  // delete h_num_d0NN;
  // delete h_num_d0NN;

  // delete h_den_phi;
  // delete h_num_phi;
  // delete h_num_phiNN;
  // delete h_num_phiNN;

  // delete h_den_Lxy;
  // delete h_num_Lxy;
  // delete h_num_LxyNN;
  // delete h_num_LxyNN;

  // delete h_den_eta;
  // delete h_num_eta;
  // delete h_num_etaNN;
  // delete h_num_etaNN;
     
  // canvasname.push_back("eff_pt_pt10_5_dxy25_BDT");
  // canvasname.push_back("eff_dxy_pt10_5_dxy25_BDT");
  // canvasname.push_back("eff_eta_pt10_5_dxy25_BDT");

  canvasname.push_back("eff_pt_pt10_NNv8vsBDT");
  canvasname.push_back("eff_pt_pt10_NNv8vsBDT");
  canvasname.push_back("eff_pt_pt10_NNv8vsBDT");
  canvasname.push_back("eff_d0_pt10_NNv8vsBDT");
  canvasname.push_back("eff_d0_pt10_NNv8vsBDT");
  canvasname.push_back("eff_d0_pt10_NNv8vsBDT");
  canvasname.push_back("eff_dxy_pt10_NNv8vsBDT");
  canvasname.push_back("eff_dxy_pt10_NNv8vsBDT");
  canvasname.push_back("eff_dxy_pt10_NNv8vsBDT");
  canvasname.push_back("eff_eta_pt10_NNv8vsBDT");
  canvasname.push_back("eff_eta_pt10_NNv8vsBDT");
  canvasname.push_back("eff_eta_pt10_NNv8vsBDT");
  canvasname.push_back("eff_fine_eta_pt10_NNv8vsBDT");
  canvasname.push_back("eff_fine_eta_pt10_NNv8vsBDT");
  canvasname.push_back("eff_fine_eta_pt10_NNv8vsBDT");
  canvasname.push_back("eff_fine_fine_eta_pt10_NNv8vsBDT");
  canvasname.push_back("eff_fine_fine_eta_pt10_NNv8vsBDT");
  canvasname.push_back("eff_fine_fine_eta_pt10_NNv8vsBDT");
  canvasname.push_back("eff_phi_pt10_NNv8vsBDT");
  canvasname.push_back("eff_phi_pt10_NNv8vsBDT");
  canvasname.push_back("eff_phi_pt10_NNv8vsBDT");
  canvasname.push_back("eff_Lxy_pt10_NNv8vsBDT");
  canvasname.push_back("eff_Lxy_pt10_NNv8vsBDT");
  canvasname.push_back("eff_Lxy_pt10_NNv8vsBDT");

  //eta binned
  canvasname.push_back("eff_pt_pt10_NNv8vsBDT_eta3");
  canvasname.push_back("eff_pt_pt10_NNv8vsBDT_eta3");
  canvasname.push_back("eff_pt_pt10_NNv8vsBDT_eta3");
  canvasname.push_back("eff_d0_pt10_NNv8vsBDT_eta3");
  canvasname.push_back("eff_d0_pt10_NNv8vsBDT_eta3");
  canvasname.push_back("eff_d0_pt10_NNv8vsBDT_eta3");
  canvasname.push_back("eff_dxy_pt10_NNv8vsBDT_eta3");
  canvasname.push_back("eff_dxy_pt10_NNv8vsBDT_eta3");
  canvasname.push_back("eff_dxy_pt10_NNv8vsBDT_eta3");
  canvasname.push_back("eff_phi_pt10_NNv8vsBDT_eta3");
  canvasname.push_back("eff_phi_pt10_NNv8vsBDT_eta3");
  canvasname.push_back("eff_phi_pt10_NNv8vsBDT_eta3");
  canvasname.push_back("eff_Lxy_pt10_NNv8vsBDT_eta3");
  canvasname.push_back("eff_Lxy_pt10_NNv8vsBDT_eta3");
  canvasname.push_back("eff_Lxy_pt10_NNv8vsBDT_eta3");

  //eta binned
  canvasname.push_back("eff_pt_pt10_NNv8vsBDT_eta2");
  canvasname.push_back("eff_pt_pt10_NNv8vsBDT_eta2");
  canvasname.push_back("eff_pt_pt10_NNv8vsBDT_eta2");
  canvasname.push_back("eff_d0_pt10_NNv8vsBDT_eta2");
  canvasname.push_back("eff_d0_pt10_NNv8vsBDT_eta2");
  canvasname.push_back("eff_d0_pt10_NNv8vsBDT_eta2");
  canvasname.push_back("eff_dxy_pt10_NNv8vsBDT_eta2");
  canvasname.push_back("eff_dxy_pt10_NNv8vsBDT_eta2");
  canvasname.push_back("eff_dxy_pt10_NNv8vsBDT_eta2");
  canvasname.push_back("eff_phi_pt10_NNv8vsBDT_eta2");
  canvasname.push_back("eff_phi_pt10_NNv8vsBDT_eta2");
  canvasname.push_back("eff_phi_pt10_NNv8vsBDT_eta2");
  canvasname.push_back("eff_Lxy_pt10_NNv8vsBDT_eta2");
  canvasname.push_back("eff_Lxy_pt10_NNv8vsBDT_eta2");
  canvasname.push_back("eff_Lxy_pt10_NNv8vsBDT_eta2");

  //eta binned
  canvasname.push_back("eff_pt_pt10_NNv8vsBDT_eta1");
  canvasname.push_back("eff_pt_pt10_NNv8vsBDT_eta1");
  canvasname.push_back("eff_pt_pt10_NNv8vsBDT_eta1");
  canvasname.push_back("eff_d0_pt10_NNv8vsBDT_eta1");
  canvasname.push_back("eff_d0_pt10_NNv8vsBDT_eta1");
  canvasname.push_back("eff_d0_pt10_NNv8vsBDT_eta1");
  canvasname.push_back("eff_dxy_pt10_NNv8vsBDT_eta1");
  canvasname.push_back("eff_dxy_pt10_NNv8vsBDT_eta1");
  canvasname.push_back("eff_dxy_pt10_NNv8vsBDT_eta1");
  canvasname.push_back("eff_phi_pt10_NNv8vsBDT_eta1");
  canvasname.push_back("eff_phi_pt10_NNv8vsBDT_eta1");
  canvasname.push_back("eff_phi_pt10_NNv8vsBDT_eta1");
  canvasname.push_back("eff_Lxy_pt10_NNv8vsBDT_eta1");
  canvasname.push_back("eff_Lxy_pt10_NNv8vsBDT_eta1");
  canvasname.push_back("eff_Lxy_pt10_NNv8vsBDT_eta1");

  // eta same plot
  canvasname.push_back("eff_pt_pt10_NNv8vsBDT_etaSame");
  canvasname.push_back("eff_pt_pt10_NNv8vsBDT_etaSame");
  canvasname.push_back("eff_pt_pt10_NNv8vsBDT_etaSame");
  canvasname.push_back("eff_dxy_pt10_NNv8vsBDT_etaSame");
  canvasname.push_back("eff_dxy_pt10_NNv8vsBDT_etaSame");
  canvasname.push_back("eff_dxy_pt10_NNv8vsBDT_etaSame");

  // 2D plots
  canvasname_2D.push_back("eff_d0_z0_pt10_BDT_etaFull");
  canvasname_2D.push_back("eff_d0_z0_pt10_NN_etaFull");
  canvasname_2D.push_back("eff_d0_z0_pt10_Comb_etaFull");
  canvasname_2D.push_back("eff_d0_z0_pt10_BDT_eta3");
  canvasname_2D.push_back("eff_d0_z0_pt10_NN_eta3");
  canvasname_2D.push_back("eff_d0_z0_pt10_Comb_eta3");
  canvasname_2D.push_back("eff_d0_z0_pt10_BDT_eta2");
  canvasname_2D.push_back("eff_d0_z0_pt10_NN_eta2");
  canvasname_2D.push_back("eff_d0_z0_pt10_Comb_eta2");
  canvasname_2D.push_back("eff_d0_z0_pt10_BDT_eta1");
  canvasname_2D.push_back("eff_d0_z0_pt10_NN_eta1");
  canvasname_2D.push_back("eff_d0_z0_pt10_Comb_eta1");
  


  // canvasname.push_back("eff_pt_pt10_10_combined");
  // canvasname.push_back("eff_dxy_pt10_10_combined");
  // canvasname.push_back("eff_eta_pt10_10_combined");

  // for (int i=0; i<canvasname.size(); i++){
  //   std::cout << errors[i]->GetName() << std::endl;
  // }

  for (int i=0; i<canvasname.size(); i++){
    if (canvasname[i]=="Canvas_name_already_used_action_skipping"){ 
      std::cout << "canvas already used. skipping..." << std::endl;
      continue;
    }
    //create canvas and save histos
    std::cout << "Drawing: " << errors[i]->GetName() << std::endl;
    TCanvas * c1=new TCanvas(canvasname[i],canvasname[i],700,700);
    errors[i]->Draw("A P");
    errors[i]->SetLineWidth(3);
    errors[i]->SetLineColor(1);
    TLatex cms_label=cms_latex();
    TLatex header=head();

    gStyle->SetLegendBorderSize(0);
    gStyle->SetLegendTextSize(0.018); 
    TLegend * leg =new TLegend(0.4,0.8,0.88,0.88);
    leg->AddEntry(errors[i],legs[i]);
    // bool printLeg=false;

    //put histos with same cnvas name in same plot
    for (int j=i+1; j<errors.size(); j++){
      if (canvasname[i]!=canvasname[j]) continue;
      errors[j]->Draw("sames P");
      canvasname[j]="Canvas_name_already_used_action_skipping";
      // printLeg=true;
      errors[j]->SetLineWidth(3);
      // errors[j]->SetLineColor(2);
      errors[j]->SetLineColor(DefaultColor(j,i));
      leg->AddEntry(errors[j],legs[j]);
    }
    leg->Draw("sames");


    // if (printLeg) leg->Draw("sames");
    // if (!grid[i]) c1->SetGrid(0);
    // if (logX[i]) c1->SetLogx();
    // if (logY[i]) c1->SetLogy();

    // c1->SaveAs("./plots"+canvasname[i]+".png");
    c1->SaveAs("./output_files/comparisons/efficiencies/NNv8/"+canvasname[i]+"_muGunFlatXYZ_denomModeCut.pdf");
    canvasname[i]="Canvas_name_already_used_action_skipping";

  }

  for (int i=0; i<canvasname_2D.size(); i++){
    if (canvasname_2D[i]=="Canvas_name_already_used_action_skipping"){ 
      std::cout << "canvas already used. skipping..." << std::endl;
      continue;
    }
    //create canvas and save histos
    std::cout << "Drawing: " << errors_2D[i]->GetName() << std::endl;
    TCanvas * c1=new TCanvas(canvasname_2D[i],canvasname_2D[i],700,700);
    c1->SetRightMargin(0.14);
    c1->SetLeftMargin(0.12);
    errors_2D[i]->GetZaxis()->SetTitleOffset(1.3);
    errors_2D[i]->Draw("colz");
    // errors_2D[i]->SetLineWidth(3);
    // errors_2D[i]->SetLineColor(1);
    // TLatex cms_label=cms_latex();
    // TLatex header=head();

    // gStyle->SetLegendBorderSize(0);
    // gStyle->SetLegendTextSize(0.018); 
    // TLegend * leg =new TLegend(0.45,0.75,0.88,0.88);
    // leg->AddEntry(errors_2D[i],legs_2D[i]);
    // bool printLeg=false;

    //put histos with same cnvas name in same plot
    // for (int j=i+1; j<errors_2D.size(); j++){
    //   if (canvasname_2D[i]!=canvasname_2D[j]) continue;
    //   errors_2D[j]->Draw("sames colz");
    //   canvasname_2D[j]="Canvas_name_already_used_action_skipping";
    //   // printLeg=true;
    //   // errors[j]->SetLineWidth(3);
    //   // errors[j]->SetLineColor(2);
    //   errors_2D[j]->SetLineColor(DefaultColor(j,i));
    //   leg->AddEntry(errors[j],legs[j]);
    // }
    // leg->Draw("sames");


    // if (printLeg) leg->Draw("sames");
    // if (!grid[i]) c1->SetGrid(0);
    // if (logX[i]) c1->SetLogx();
    // if (logY[i]) c1->SetLogy();

    // c1->SaveAs("./plots"+canvasname_2D[i]+".png");
    c1->SaveAs("./output_files/comparisons/efficiencies/NNv8/"+canvasname_2D[i]+"_muGunFlatXYZ_denomModeCut.pdf");
    canvasname_2D[i]="Canvas_name_already_used_action_skipping";

  }



  return 0;
 }
