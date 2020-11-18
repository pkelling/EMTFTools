/* L1T Analysis plotting script
 * module: Efficiency plotter
 * Author: George Karathanasis georgios.karathanasis@cern.ch
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


int makeEfficiencyPlotsOldVsNew(){

  //read data
  TString ntuple_old = "/eos/cms/store/user/eyigitba/emtf/matchedNtuples/matchedNtuple_HTo2LLTo4Mu_combined_cmssw_11_0_2_fwImplementation_NNv4.root";
  TString ntuple_new = "/eos/cms/store/user/eyigitba/emtf/matchedNtuples/matchedNtuple_HTo2LLTo4Mu_combined_cmssw_11_0_2_fwImplementation_NNv5.root";
  TChain * cc_old=new TChain("tree");
  TChain * cc_new=new TChain("tree");
  cc_old->Add(ntuple_old);
  cc_new->Add(ntuple_new);


  TTreeReader ccReaderOld(cc_old);
  TTreeReader ccReaderNew(cc_new);

  TTreeReaderArray<float> genDiMuPtOld(ccReaderOld,"gendimu_pt");
  TTreeReaderArray<float> genDiMuEtaOld(ccReaderOld,"gendimu_eta");
  TTreeReaderArray<float> genDiMuPhiOld(ccReaderOld,"gendimu_phi");
  TTreeReaderArray<float> genDiMuLxyOld(ccReaderOld,"gendimu_Lxy");
  TTreeReaderArray<float> genDiMuVzOld(ccReaderOld,"gendimu_vz");
  TTreeReaderArray<int> genDiMuDaughter1Old(ccReaderOld,"gendimu_daughter1");
  TTreeReaderArray<int> genDiMuDaughter2Old(ccReaderOld,"gendimu_daughter2");

  TTreeReaderArray<float> genMuPtOld(ccReaderOld,"gen_pt");
  TTreeReaderArray<float> genMuEtaOld(ccReaderOld,"gen_eta");
  TTreeReaderArray<float> genMuEtaStarOld(ccReaderOld,"gen_etaStar");
  TTreeReaderArray<float> genMuPhiOld(ccReaderOld,"gen_phi");
  TTreeReaderArray<float> genMuPhiStarOld(ccReaderOld,"gen_phiStar");
  TTreeReaderArray<float> genMuDROld(ccReaderOld,"gen_dR");
  TTreeReaderArray<float> genMuLxyOld(ccReaderOld,"gen_Lxy");
  TTreeReaderArray<float> genMuVzOld(ccReaderOld,"gen_vz");
  TTreeReaderArray<float> genMuD0Old(ccReaderOld,"gen_d0");
  TTreeReaderArray<int> genMuIdxOld(ccReaderOld,"gen_idx");
  TTreeReaderArray<int> genMuChargeOld(ccReaderOld,"gen_charge");
  TTreeReaderArray<int> genMuParentOld(ccReaderOld,"gen_parent");
  TTreeReaderArray<int> genMuMatchedL1MuIDOld(ccReaderOld,"gen_matchedL1Mu");

  TTreeReaderArray<float> l1MuPtOld(ccReaderOld,"l1_pt");
  TTreeReaderArray<float> l1MuPtDxyOld(ccReaderOld,"l1_ptDxy");
  TTreeReaderArray<float> l1MuEtaOld(ccReaderOld,"l1_eta");
  TTreeReaderArray<float> l1MuPhiOld(ccReaderOld,"l1_phi");
  TTreeReaderArray<float> l1MuQualOld(ccReaderOld,"l1_qual");
  TTreeReaderArray<float> l1MuDxyNNOld(ccReaderOld,"l1_dxyNN");
  TTreeReaderArray<int> l1MuDxyOld(ccReaderOld,"l1_dxy");
  TTreeReaderArray<int> l1MuChargeOld(ccReaderOld,"l1_charge");
  TTreeReaderArray<int> l1MuEmtfModeOld(ccReaderOld,"l1_emtfMode");





  TTreeReaderArray<float> genDiMuPtNew(ccReaderNew,"gendimu_pt");
  TTreeReaderArray<float> genDiMuEtaNew(ccReaderNew,"gendimu_eta");
  TTreeReaderArray<float> genDiMuPhiNew(ccReaderNew,"gendimu_phi");
  TTreeReaderArray<float> genDiMuLxyNew(ccReaderNew,"gendimu_Lxy");
  TTreeReaderArray<float> genDiMuVzNew(ccReaderNew,"gendimu_vz");
  TTreeReaderArray<int> genDiMuDaughter1New(ccReaderNew,"gendimu_daughter1");
  TTreeReaderArray<int> genDiMuDaughter2New(ccReaderNew,"gendimu_daughter2");

  TTreeReaderArray<float> genMuPtNew(ccReaderNew,"gen_pt");
  TTreeReaderArray<float> genMuEtaNew(ccReaderNew,"gen_eta");
  TTreeReaderArray<float> genMuEtaStarNew(ccReaderNew,"gen_etaStar");
  TTreeReaderArray<float> genMuPhiNew(ccReaderNew,"gen_phi");
  TTreeReaderArray<float> genMuPhiStarNew(ccReaderNew,"gen_phiStar");
  TTreeReaderArray<float> genMuDRNew(ccReaderNew,"gen_dR");
  TTreeReaderArray<float> genMuLxyNew(ccReaderNew,"gen_Lxy");
  TTreeReaderArray<float> genMuVzNew(ccReaderNew,"gen_vz");
  TTreeReaderArray<float> genMuD0New(ccReaderNew,"gen_d0");
  TTreeReaderArray<int> genMuIdxNew(ccReaderNew,"gen_idx");
  TTreeReaderArray<int> genMuChargeNew(ccReaderNew,"gen_charge");
  TTreeReaderArray<int> genMuParentNew(ccReaderNew,"gen_parent");
  TTreeReaderArray<int> genMuMatchedL1MuIDNew(ccReaderNew,"gen_matchedL1Mu");

  TTreeReaderArray<float> l1MuPtNew(ccReaderNew,"l1_pt");
  TTreeReaderArray<float> l1MuPtDxyNew(ccReaderNew,"l1_ptDxy");
  TTreeReaderArray<float> l1MuEtaNew(ccReaderNew,"l1_eta");
  TTreeReaderArray<float> l1MuPhiNew(ccReaderNew,"l1_phi");
  TTreeReaderArray<float> l1MuQualNew(ccReaderNew,"l1_qual");
  TTreeReaderArray<float> l1MuDxyNNNew(ccReaderNew,"l1_dxyNN");
  TTreeReaderArray<int> l1MuDxyNew(ccReaderNew,"l1_dxy");
  TTreeReaderArray<int> l1MuChargeNew(ccReaderNew,"l1_charge");
  TTreeReaderArray<int> l1MuEmtfModeNew(ccReaderNew,"l1_emtfMode");

  gStyle->SetOptStat(0);

  std::cout<<"Running on "<<cc_old->GetEntries()<<" evts "<<std::endl;

  //plot containers
  std::vector<TString> canvasname;
  std::vector<std::string> kwds;
  std::vector<TString> legs;
  std::vector<TGraphAsymmErrors*> errors;

  // cosmetic options
  std::vector<bool> grid,logY,logX;



  // initialize cuts
  float ptThreshold = 20.0;
  // float ptThresholdSecond = 5.0;
  float etaThresholdMin = 1.24;
  float etaThresholdMax = 2.5;
  float dRThreshold = 1.0;
  int qualThreshold = 11;
  int modeThreshold = 11;
  float dxyThresholdMin = 25.0;
  float dxyThresholdMax = 100.0;
  float z0Threshold = 100.0;

  TH1F *h_den_ptOld = new TH1F("h_den_ptOld", "", 30, 0, 60);
  TH1F *h_den_ptNew = new TH1F("h_den_ptNew", "", 30, 0, 60);
  TH1F *h_num_ptOld = new TH1F("h_num_ptOld", "", 30, 0, 60);
  TH1F *h_num_ptNew = new TH1F("h_num_ptNew", "", 30, 0, 60);


  TH1F *h_den_d0Old = new TH1F("h_den_d0Old", "", 60, -120, 120);
  TH1F *h_den_d0New = new TH1F("h_den_d0New", "", 60, -120, 120);
  TH1F *h_num_d0Old = new TH1F("h_num_d0Old", "", 60, -120, 120);
  TH1F *h_num_d0New = new TH1F("h_num_d0New", "", 60, -120, 120);


  TH1F *h_den_dxyOld = new TH1F("h_den_dxyOld", "", 30, 0, 120);
  TH1F *h_den_dxyNew = new TH1F("h_den_dxyNew", "", 30, 0, 120);
  TH1F *h_num_dxyOld = new TH1F("h_num_dxyOld", "", 30, 0, 120);
  TH1F *h_num_dxyNew = new TH1F("h_num_dxyNew", "", 30, 0, 120);


  TH1F *h_den_phiOld = new TH1F("h_den_phiOld", "", 64, -4, 4);
  TH1F *h_den_phiNew = new TH1F("h_den_phiNew", "", 64, -4, 4);
  TH1F *h_num_phiOld = new TH1F("h_num_phiOld", "", 64, -4, 4);
  TH1F *h_num_phiNew = new TH1F("h_num_phiNew", "", 64, -4, 4);


  TH1F *h_den_LxyOld = new TH1F("h_den_LxyOld", "", 60, 0, 240);
  TH1F *h_den_LxyNew = new TH1F("h_den_LxyNew", "", 60, 0, 240);
  TH1F *h_num_LxyOld = new TH1F("h_num_LxyOld", "", 60, 0, 240);
  TH1F *h_num_LxyNew = new TH1F("h_num_LxyNew", "", 60, 0, 240);


  Double_t etaArray[15] = {-2.5, -2.1, -1.6, -1.2, -0.9, -0.3, -0.2, 0, 0.2, 0.3, 0.9, 1.2, 1.6, 2.1, 2.5};

  TH1F *h_den_etaOld = new TH1F("h_den_etaOld", "", 14, etaArray);
  TH1F *h_den_etaNew = new TH1F("h_den_etaNew", "", 14, etaArray);
  TH1F *h_num_etaOld = new TH1F("h_num_etaOld", "", 14, etaArray);
  TH1F *h_num_etaNew = new TH1F("h_num_etaNew", "", 14, etaArray);



  // Single muon efficiencies 

  int eventCount = 0;
  while(ccReaderOld.Next()){
    eventCount++;
    if (eventCount % 1000 == 0) std::cout << eventCount << " events read!" << std::endl;
    for(int i=0; i<genMuPtOld.GetSize(); i++){
      // if (gendROld[i] > dRThreshold) continue;
      if (abs(genMuEtaStarOld[i]) < etaThresholdMin) continue;
      if (abs(genMuEtaStarOld[i]) > etaThresholdMax) continue;
      if (abs(genMuVzOld[i]) > z0Threshold) continue;
      if (abs(genMuD0Old[i]) > dxyThresholdMax) continue;
      // if (abs(gendxyOld[i]) < dxyThresholdMin) continue;
      // if (genMatchedL1MuIDOld[i] < 0) continue;


      h_den_ptOld->Fill(genMuPtOld[i]);

      if (genMuPtOld[i] > (ptThreshold - 5.0)){
        h_den_etaOld->Fill(genMuEtaStarOld[i]);
        h_den_dxyOld->Fill(abs(genMuD0Old[i]));
        h_den_d0Old->Fill(genMuD0Old[i]);
        h_den_phiOld->Fill(genMuPhiStarOld[i]);
        h_den_LxyOld->Fill(genMuLxyOld[i]);
      }

      if (genMuDROld[i] > 0.6) continue;
      if (l1MuPtOld[genMuMatchedL1MuIDOld[i]] < 0) continue;
      
      if (l1MuPtDxyOld[genMuMatchedL1MuIDOld[i]] > ptThreshold){
        h_num_ptOld->Fill(genMuPtOld[i]);
        h_num_etaOld->Fill(genMuEtaStarOld[i]);
        h_num_dxyOld->Fill(abs(genMuD0Old[i]));
        h_num_d0Old->Fill(genMuD0Old[i]);
        h_num_phiOld->Fill(genMuPhiStarOld[i]);
        h_num_LxyOld->Fill(genMuLxyOld[i]);

      }




    }
    


  }

  while(ccReaderNew.Next()){
    // eventCount++;
    // if (eventCount % 1000 == 0) std::cout << eventCount << " events read!" << std::endl;
    for(int i=0; i<genMuPtNew.GetSize(); i++){
      // if (gendRNew[i] > dRThreshold) continue;
      if (abs(genMuEtaStarNew[i]) < etaThresholdMin) continue;
      if (abs(genMuEtaStarNew[i]) > etaThresholdMax) continue;
      if (abs(genMuVzNew[i]) > z0Threshold) continue;
      if (abs(genMuD0New[i]) > dxyThresholdMax) continue;
      // if (abs(gendxyNew[i]) < dxyThresholdMin) continue;
      // if (genMatchedL1MuIDNew[i] < 0) continue;


      h_den_ptNew->Fill(genMuPtNew[i]);

      if (genMuPtNew[i] > (ptThreshold - 5.0)){
        h_den_etaNew->Fill(genMuEtaStarNew[i]);
        h_den_dxyNew->Fill(abs(genMuD0New[i]));
        h_den_d0New->Fill(genMuD0New[i]);
        h_den_phiNew->Fill(genMuPhiStarNew[i]);
        h_den_LxyNew->Fill(genMuLxyNew[i]);
      }

      if (genMuDRNew[i] > 0.6) continue;
      if (l1MuPtNew[genMuMatchedL1MuIDNew[i]] < 0) continue;
      
      if (l1MuPtDxyNew[genMuMatchedL1MuIDNew[i]] > ptThreshold){
        h_num_ptNew->Fill(genMuPtNew[i]);
        h_num_etaNew->Fill(genMuEtaStarNew[i]);
        h_num_dxyNew->Fill(abs(genMuD0New[i]));
        h_num_d0New->Fill(genMuD0New[i]);
        h_num_phiNew->Fill(genMuPhiStarNew[i]);
        h_num_LxyNew->Fill(genMuLxyNew[i]);

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
  TGraphAsymmErrors * error_ptOld = new TGraphAsymmErrors(h_num_ptOld,h_den_ptOld);
  TGraphAsymmErrors * error_ptNew = new TGraphAsymmErrors(h_num_ptNew,h_den_ptNew);

  TGraphAsymmErrors * error_d0Old = new TGraphAsymmErrors(h_num_d0Old,h_den_d0Old);
  TGraphAsymmErrors * error_d0New = new TGraphAsymmErrors(h_num_d0New,h_den_d0New);

  TGraphAsymmErrors * error_dxyOld = new TGraphAsymmErrors(h_num_dxyOld,h_den_dxyOld);
  TGraphAsymmErrors * error_dxyNew = new TGraphAsymmErrors(h_num_dxyNew,h_den_dxyNew);

  TGraphAsymmErrors * error_etaOld = new TGraphAsymmErrors(h_num_etaOld,h_den_etaOld);
  TGraphAsymmErrors * error_etaNew = new TGraphAsymmErrors(h_num_etaNew,h_den_etaNew);

  TGraphAsymmErrors * error_phiOld = new TGraphAsymmErrors(h_num_phiOld,h_den_phiOld);
  TGraphAsymmErrors * error_phiNew = new TGraphAsymmErrors(h_num_phiNew,h_den_phiNew);

  TGraphAsymmErrors * error_LxyOld = new TGraphAsymmErrors(h_num_LxyOld,h_den_LxyOld);
  TGraphAsymmErrors * error_LxyNew = new TGraphAsymmErrors(h_num_LxyNew,h_den_LxyNew);

  // canvasname.push_back("out_ptUnc_vs_dxy_wME11a"); 
  TString titlePt="Gen Muon pT [GeV]";
  TString titleDxy="Gen Muon D_{0} [cm]";
  TString titleLxy="Gen Muon Lxy [cm]";
  TString titleEta="Gen Muon #eta";
  TString titlePhi="Gen Muon #phi";

  error_ptOld->GetXaxis()->SetTitle(titlePt);
  error_ptOld->GetYaxis()->SetTitle("L1T Efficiency");
  error_ptOld->GetYaxis()->SetRangeUser(0.00001,1.1);

  error_d0Old->GetXaxis()->SetTitle(titleDxy);
  error_d0Old->GetYaxis()->SetTitle("L1T Efficiency");
  error_d0Old->GetYaxis()->SetRangeUser(0.00001,1.1);

  error_dxyOld->GetXaxis()->SetTitle(titleDxy);
  error_dxyOld->GetYaxis()->SetTitle("L1T Efficiency");
  error_dxyOld->GetYaxis()->SetRangeUser(0.00001,1.1);

  error_etaOld->GetXaxis()->SetTitle(titleEta);
  error_etaOld->GetYaxis()->SetTitle("L1T Efficiency");
  error_etaOld->GetYaxis()->SetRangeUser(0.00001,1.1);

  error_phiOld->GetXaxis()->SetTitle(titlePhi);
  error_phiOld->GetYaxis()->SetTitle("L1T Efficiency");
  error_phiOld->GetYaxis()->SetRangeUser(0.00001,1.1);

  error_LxyOld->GetXaxis()->SetTitle(titleLxy);
  error_LxyOld->GetYaxis()->SetTitle("L1T Efficiency");
  error_LxyOld->GetYaxis()->SetRangeUser(0.00001,1.1);
 
  TString leg = "L1 pT (NNv4) > 20 GeV";
  TString leg2 = "L1 pT (NNv5) > 20 GeV";
  // TString leg3 = "L1 pT (NN) > 20 GeV || L1 pT (BDT) > 20 GeV";




  // kwds.push_back(options);
  errors.push_back(error_ptOld);
  errors.push_back(error_ptNew);
  errors.push_back(error_d0Old);
  errors.push_back(error_d0New);
  errors.push_back(error_dxyOld);
  errors.push_back(error_dxyNew);
  errors.push_back(error_etaOld);
  errors.push_back(error_etaNew);
  errors.push_back(error_phiOld);
  errors.push_back(error_phiNew);
  errors.push_back(error_LxyOld);
  errors.push_back(error_LxyNew);

  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg);
  legs.push_back(leg2);
  legs.push_back(leg);
  legs.push_back(leg2);

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
     
  // canvasname.push_back("eff_pt_pt20_5_dxy25_BDT");
  // canvasname.push_back("eff_dxy_pt20_5_dxy25_BDT");
  // canvasname.push_back("eff_eta_pt20_5_dxy25_BDT");

  canvasname.push_back("eff_pt_pt20_NNv4vsNNv5");
  canvasname.push_back("eff_pt_pt20_NNv4vsNNv5");
  canvasname.push_back("eff_d0_pt20_NNv4vsNNv5");
  canvasname.push_back("eff_d0_pt20_NNv4vsNNv5");
  canvasname.push_back("eff_dxy_pt20_NNv4vsNNv5");
  canvasname.push_back("eff_dxy_pt20_NNv4vsNNv5");
  canvasname.push_back("eff_eta_pt20_NNv4vsNNv5");
  canvasname.push_back("eff_eta_pt20_NNv4vsNNv5");
  canvasname.push_back("eff_phi_pt20_NNv4vsNNv5");
  canvasname.push_back("eff_phi_pt20_NNv4vsNNv5");
  canvasname.push_back("eff_Lxy_pt20_NNv4vsNNv5");
  canvasname.push_back("eff_Lxy_pt20_NNv4vsNNv5");

  // canvasname.push_back("eff_pt_pt20_10_combined");
  // canvasname.push_back("eff_dxy_pt20_10_combined");
  // canvasname.push_back("eff_eta_pt20_10_combined");

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
    gStyle->SetLegendTextSize(0.02); 
    TLegend * leg =new TLegend(0.6,0.75,0.88,0.88);    leg->AddEntry(errors[i],legs[i]);
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
    c1->SaveAs("./output_files/comparisons/efficiencies/NNv4_NNv5/"+canvasname[i]+".pdf");
    canvasname[i]="Canvas_name_already_used_action_skipping";

  }



  return 0;
 }
