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
  TString ntuple = "/eos/cms/store/user/eyigitba/emtf/matchedNtuples/matchedNtuple_HTo2LLTo4Mu_combined_cmssw_11_0_2_fwImplementation_NNv4.root";
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

  TH1F *h_den_eta = new TH1F("h_den_eta", "", 14, etaArray);
  TH1F *h_num_eta = new TH1F("h_num_eta", "", 14, etaArray);
  TH1F *h_num_etaNN = new TH1F("h_num_etaNN", "", 14, etaArray);
  TH1F *h_num_etaComb = new TH1F("h_num_etaComb", "", 14, etaArray);


  // Single muon efficiencies 

  int eventCount = 0;
  while(ccReader.Next()){
    eventCount++;
    if (eventCount % 1000 == 0) std::cout << eventCount << " events read!" << std::endl;
    for(int i=0; i<genMuPt.GetSize(); i++){
      // if (gendR[i] > dRThreshold) continue;
      if (abs(genMuEtaStar[i]) < etaThresholdMin) continue;
      if (abs(genMuEtaStar[i]) > etaThresholdMax) continue;
      if (abs(genMuVz[i]) > z0Threshold) continue;
      if (abs(genMuD0[i]) > dxyThresholdMax) continue;
      // if (abs(gendxy[i]) < dxyThresholdMin) continue;
      // if (genMatchedL1MuID[i] < 0) continue;


      h_den_pt->Fill(genMuPt[i]);

      if (genMuPt[i] > (ptThreshold - 5.0)){
        h_den_eta->Fill(genMuEtaStar[i]);
        h_den_dxy->Fill(abs(genMuD0[i]));
        h_den_d0->Fill(genMuD0[i]);
        h_den_phi->Fill(genMuPhiStar[i]);
        h_den_Lxy->Fill(genMuLxy[i]);
      }

      if (genMuDR[i] > 0.6) continue;
      if (l1MuPt[genMuMatchedL1MuID[i]] < 0) continue;
      
      if (l1MuPt[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_pt->Fill(genMuPt[i]);
        h_num_eta->Fill(genMuEtaStar[i]);
        h_num_dxy->Fill(abs(genMuD0[i]));
        h_num_d0->Fill(genMuD0[i]);
        h_num_phi->Fill(genMuPhiStar[i]);
        h_num_Lxy->Fill(genMuLxy[i]);

      }

      if (l1MuPtDxy[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_ptNN->Fill(genMuPt[i]);
        h_num_etaNN->Fill(genMuEtaStar[i]);
        h_num_dxyNN->Fill(abs(genMuD0[i]));
        h_num_d0NN->Fill(genMuD0[i]);
        h_num_phiNN->Fill(genMuPhiStar[i]);
        h_num_LxyNN->Fill(genMuLxy[i]);

      }

      if (l1MuPtDxy[genMuMatchedL1MuID[i]] > ptThreshold || l1MuPt[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_ptComb->Fill(genMuPt[i]);
        h_num_etaComb->Fill(genMuEtaStar[i]);
        h_num_dxyComb->Fill(abs(genMuD0[i]));
        h_num_d0Comb->Fill(genMuD0[i]);
        h_num_phiComb->Fill(genMuPhiStar[i]);
        h_num_LxyComb->Fill(genMuLxy[i]);

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

  TGraphAsymmErrors * error_phi = new TGraphAsymmErrors(h_num_phi,h_den_phi);
  TGraphAsymmErrors * error_phiNN = new TGraphAsymmErrors(h_num_phiNN,h_den_phi);
  TGraphAsymmErrors * error_phiComb = new TGraphAsymmErrors(h_num_phiComb,h_den_phi);

  TGraphAsymmErrors * error_Lxy = new TGraphAsymmErrors(h_num_Lxy,h_den_Lxy);
  TGraphAsymmErrors * error_LxyNN = new TGraphAsymmErrors(h_num_LxyNN,h_den_Lxy);
  TGraphAsymmErrors * error_LxyComb = new TGraphAsymmErrors(h_num_LxyComb,h_den_Lxy);

  // canvasname.push_back("out_ptUnc_vs_dxy_wME11a"); 
  TString titlePt="Gen Muon pT [GeV]";
  TString titleDxy="Gen Muon D_{0} [cm]";
  TString titleLxy="Gen Muon Lxy [cm]";
  TString titleEta="Gen Muon #eta";
  TString titlePhi="Gen Muon #phi";

  error_pt->GetXaxis()->SetTitle(titlePt);
  error_pt->GetYaxis()->SetTitle("L1T Efficiency");
  error_pt->GetYaxis()->SetRangeUser(0.00001,1.1);

  error_d0->GetXaxis()->SetTitle(titleDxy);
  error_d0->GetYaxis()->SetTitle("L1T Efficiency");
  error_d0->GetYaxis()->SetRangeUser(0.00001,1.1);

  error_dxy->GetXaxis()->SetTitle(titleDxy);
  error_dxy->GetYaxis()->SetTitle("L1T Efficiency");
  error_dxy->GetYaxis()->SetRangeUser(0.00001,1.1);

  error_eta->GetXaxis()->SetTitle(titleEta);
  error_eta->GetYaxis()->SetTitle("L1T Efficiency");
  error_eta->GetYaxis()->SetRangeUser(0.00001,1.1);

  error_phi->GetXaxis()->SetTitle(titlePhi);
  error_phi->GetYaxis()->SetTitle("L1T Efficiency");
  error_phi->GetYaxis()->SetRangeUser(0.00001,1.1);

  error_Lxy->GetXaxis()->SetTitle(titleLxy);
  error_Lxy->GetYaxis()->SetTitle("L1T Efficiency");
  error_Lxy->GetYaxis()->SetRangeUser(0.00001,1.1);
 
  TString leg = "L1 pT (BDT) > 20 GeV";
  TString leg2 = "L1 pT (NN) > 20 GeV";
  TString leg3 = "L1 pT (NN) > 20 GeV || L1 pT (BDT) > 20 GeV";




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
  errors.push_back(error_phi);
  errors.push_back(error_phiNN);
  errors.push_back(error_phiComb);
  errors.push_back(error_Lxy);
  errors.push_back(error_LxyNN);
  errors.push_back(error_LxyComb);

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

  canvasname.push_back("eff_pt_pt20_NNv4vsBDT");
  canvasname.push_back("eff_pt_pt20_NNv4vsBDT");
  canvasname.push_back("eff_pt_pt20_NNv4vsBDT");
  canvasname.push_back("eff_d0_pt20_NNv4vsBDT");
  canvasname.push_back("eff_d0_pt20_NNv4vsBDT");
  canvasname.push_back("eff_d0_pt20_NNv4vsBDT");
  canvasname.push_back("eff_dxy_pt20_NNv4vsBDT");
  canvasname.push_back("eff_dxy_pt20_NNv4vsBDT");
  canvasname.push_back("eff_dxy_pt20_NNv4vsBDT");
  canvasname.push_back("eff_eta_pt20_NNv4vsBDT");
  canvasname.push_back("eff_eta_pt20_NNv4vsBDT");
  canvasname.push_back("eff_eta_pt20_NNv4vsBDT");
  canvasname.push_back("eff_phi_pt20_NNv4vsBDT");
  canvasname.push_back("eff_phi_pt20_NNv4vsBDT");
  canvasname.push_back("eff_phi_pt20_NNv4vsBDT");
  canvasname.push_back("eff_Lxy_pt20_NNv4vsBDT");
  canvasname.push_back("eff_Lxy_pt20_NNv4vsBDT");
  canvasname.push_back("eff_Lxy_pt20_NNv4vsBDT");

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
    gStyle->SetLegendTextSize(0.018); 
    TLegend * leg =new TLegend(0.45,0.75,0.88,0.88);
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
    c1->SaveAs("./output_files/comparisons/efficiencies/"+canvasname[i]+".pdf");
    canvasname[i]="Canvas_name_already_used_action_skipping";

  }



  return 0;
 }
