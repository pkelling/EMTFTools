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


int makeTruthCompOldVsNew(){

  //read data
  // TString ntuple_old = "/eos/cms/store/user/eyigitba/emtf/matchedNtuples/matchedNtuple_DisplacedMuGun_flatPt2to1000_allEndcap_flatXYZEtaPhi_11_3_0_pre5_NNv5_20M.root";
  // TString ntuple_new = "/eos/cms/store/user/eyigitba/emtf/matchedNtuples/matchedNtuple_DisplacedMuGun_flatPt2to1000_allEndcap_flatXYZEtaPhi_11_3_0_pre5_NNv6_20M.root";
  TString ntuple_old = "/eos/cms/store/user/eyigitba/emtf/matchedNtuples/matchedNtuple_DisplacedMuGun_flatPt2to1000_allEndcap_flatXYZEtaPhi_11_3_0_pre5_NNv6_20M.root";
  TString ntuple_new = "/eos/cms/store/user/eyigitba/emtf/matchedNtuples/matchedNtuple_DisplacedMuGun_flatPt2to1000_allEndcap_flatXYZEtaPhi_11_3_0_pre5_NNv8_1M.root";
  TChain * cc_old=new TChain("tree");
  TChain * cc_new=new TChain("tree");
  cc_old->Add(ntuple_old);
  cc_new->Add(ntuple_new);


  TTreeReader ccReaderOld(cc_old);
  TTreeReader ccReaderNew(cc_new);



  TTreeReaderArray<float> genMuPtOld(ccReaderOld,"gen_pt");
  TTreeReaderArray<float> genMuDxyOld(ccReaderOld,"gen_d0");
  TTreeReaderArray<float> genMuZ0Old(ccReaderOld,"gen_vz");
  TTreeReaderArray<float> genMuEtaStarOld(ccReaderOld,"gen_etaStar");
  TTreeReaderArray<int> genMuChargeOld(ccReaderOld,"gen_charge");


  TTreeReaderArray<float> l1MuPtDxyOld(ccReaderOld,"l1_ptDxy");
  TTreeReaderArray<float> l1MuDxyNNOld(ccReaderOld,"l1_dxyNN");
  TTreeReaderArray<int> genMuMatchedL1MuIDOld(ccReaderOld,"gen_matchedL1Mu");
  TTreeReaderArray<int> l1MuEmtfModeOld(ccReaderOld,"l1_emtfMode");
  TTreeReaderArray<int> l1MuChargeOld(ccReaderOld,"l1_charge");


  TTreeReaderArray<float> genMuPtNew(ccReaderNew,"gen_pt");
  TTreeReaderArray<float> genMuDxyNew(ccReaderNew,"gen_d0");
  TTreeReaderArray<float> genMuZ0New(ccReaderNew,"gen_vz");
  TTreeReaderArray<float> genMuEtaStarNew(ccReaderNew,"gen_etaStar");
  TTreeReaderArray<int> genMuChargeNew(ccReaderNew,"gen_charge");

  TTreeReaderArray<float> l1MuPtDxyNew(ccReaderNew,"l1_ptDxy");
  TTreeReaderArray<float> l1MuDxyNNNew(ccReaderNew,"l1_dxyNN");
  TTreeReaderArray<int> genMuMatchedL1MuIDNew(ccReaderNew,"gen_matchedL1Mu");
  TTreeReaderArray<int> l1MuEmtfModeNew(ccReaderNew,"l1_emtfMode");
  TTreeReaderArray<int> l1MuChargeNew(ccReaderNew,"l1_charge");


  gStyle->SetOptStat(0);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetLegendTextSize(0.03);

  std::cout<<"Running on "<<cc_old->GetEntries()<<" evts "<<std::endl;

  //plot containers
  std::vector<TString> canvasname;
  std::vector<TString> legs;

  // cosmetic options
  std::vector<bool> grid,logY,logX;


  // TH1F *h_ptOld = new TH1F("h_ptOld", "", 40, 0, 80);
  // TH1F *h_ptNew = new TH1F("h_ptNew", "", 40, 0, 80);

  // TH1F *h_InvPtOld = new TH1F("h_InvPtOld", "", 30, 0, 60);
  // TH1F *h_InvPtNew = new TH1F("h_InvPtNew", "", 30, 0, 60);


  // TH1F *h_dxyOld = new TH1F("h_den_dxyOld", "", 75, -150, 150);
  // TH1F *h_dxyNew = new TH1F("h_den_dxyNew", "", 75, -150, 150);

  TH1F *h_ptDiffOld = new TH1F("h_ptDiffOld", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffOld = new TH1F("h_InvPtDiffOld", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelOld = new TH1F("h_InvPtDiffRelOld", "", 20, -3.5,6.5);
  TH1F *h_ptDiffRelOld = new TH1F("h_ptDiffRelOld", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffOld = new TH1F("h_dxyDiffOld", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelOld = new TH1F("h_dxyDiffRelOld", "", 14, -2.5, 4.5);

  TH2F *h_ptDiffOld_2D_pt = new TH2F("h_ptDiffOld_2D_pt", "", 18, 2, 20, 11, -5.5, 5.5);
  TH2F *h_InvPtDiffOld_2D_pt = new TH2F("h_InvPtDiffOld_2D_pt", "", 18, 2, 20, 81, -40.5, 40.5);
  TH2F *h_dxyDiffOld_2D_pt = new TH2F("h_dxyDiffOld_2D_pt", "", 18, 2, 20, 201, -100.5, 100.5);

  TH2F *h_ptDiffOld_2D_dxy = new TH2F("h_ptDiffOld_2D_dxy", "", 100, 0, 100, 11, -5.5, 5.5);
  TH2F *h_InvPtDiffOld_2D_dxy = new TH2F("h_InvPtDiffOld_2D_dxy", "", 100, 0, 100, 81, -40.5, 40.5);
  TH2F *h_dxyDiffOld_2D_dxy = new TH2F("h_dxyDiffOld_2D_dxy", "", 100, 0, 100, 201, -100.5, 100.5);

  TH2F *h_ptDiffOld_2D_eta = new TH2F("h_ptDiffOld_2D_eta", "", 26, 1.2, 2.5, 11, -5.5, 5.5);
  TH2F *h_InvPtDiffOld_2D_eta = new TH2F("h_InvPtDiffOld_2D_eta", "", 26, 1.2, 2.5, 81, -40.5, 40.5);
  TH2F *h_dxyDiffOld_2D_eta = new TH2F("h_dxyDiffOld_2D_eta", "", 26, 1.2, 2.5, 201, -100.5, 100.5);


  TH1F *h_ptDiffNew = new TH1F("h_ptDiffNew", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffNew = new TH1F("h_InvPtDiffNew", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelNew = new TH1F("h_InvPtDiffRelNew", "", 31, -5.5, 25.5);
  TH1F *h_ptDiffRelNew = new TH1F("h_ptDiffRelNew", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffNew = new TH1F("h_dxyDiffNew", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelNew = new TH1F("h_dxyDiffRelNew", "", 14, -2.5, 4.5);

  TH2F *h_ptDiffNew_2D_pt = new TH2F("h_ptDiffNew_2D_pt", "", 18, 2, 20, 11, -5.5, 5.5);
  TH2F *h_InvPtDiffNew_2D_pt = new TH2F("h_InvPtDiffNew_2D_pt", "", 18, 2, 20, 81, -40.5, 40.5);
  TH2F *h_dxyDiffNew_2D_pt = new TH2F("h_dxyDiffNew_2D_pt", "", 18, 2, 20, 201, -100.5, 100.5);

  TH2F *h_ptDiffNew_2D_dxy = new TH2F("h_ptDiffNew_2D_dxy", "", 100, 0, 100, 11, -5.5, 5.5);
  TH2F *h_InvPtDiffNew_2D_dxy = new TH2F("h_InvPtDiffNew_2D_dxy", "", 100, 0, 100, 81, -40.5, 40.5);
  TH2F *h_dxyDiffNew_2D_dxy = new TH2F("h_dxyDiffNew_2D_dxy", "", 100, 0, 100, 201, -100.5, 100.5);

  TH2F *h_ptDiffNew_2D_eta = new TH2F("h_ptDiffNew_2D_eta", "", 26, 1.2, 2.5, 11, -5.5, 5.5);
  TH2F *h_InvPtDiffNew_2D_eta = new TH2F("h_InvPtDiffNew_2D_eta", "", 26, 1.2, 2.5, 81, -40.5, 40.5);
  TH2F *h_dxyDiffNew_2D_eta = new TH2F("h_dxyDiffNew_2D_eta", "", 26, 1.2, 2.5, 201, -100.5, 100.5);


  // pt < 5

  TH1F *h_ptDiffOld_pt5 = new TH1F("h_ptDiffOld_pt5", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffOld_pt5 = new TH1F("h_InvPtDiffOld_pt5", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelOld_pt5 = new TH1F("h_InvPtDiffRelOld_pt5", "", 20, -3.5,6.5);
  TH1F *h_ptDiffRelOld_pt5 = new TH1F("h_ptDiffRelOld_pt5", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffOld_pt5 = new TH1F("h_dxyDiffOld_pt5", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelOld_pt5 = new TH1F("h_dxyDiffRelOld_pt5", "", 14, -2.5, 4.5);

  TH1F *h_ptDiffNew_pt5 = new TH1F("h_ptDiffNew_pt5", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffNew_pt5 = new TH1F("h_InvPtDiffNew_pt5", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelNew_pt5 = new TH1F("h_InvPtDiffRelNew_pt5", "", 31, -5.5, 25.5);
  TH1F *h_ptDiffRelNew_pt5 = new TH1F("h_ptDiffRelNew_pt5", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffNew_pt5 = new TH1F("h_dxyDiffNew_pt5", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelNew_pt5= new TH1F("h_dxyDiffRelNew_pt5", "", 14, -2.5, 4.5);

  // 5 < pt < 10
  TH1F *h_ptDiffOld_5pt10 = new TH1F("h_ptDiffOld_5pt10", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffOld_5pt10 = new TH1F("h_InvPtDiffOld_5pt10", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelOld_5pt10 = new TH1F("h_InvPtDiffRelOld_5pt10", "", 20, -3.5,6.5);
  TH1F *h_ptDiffRelOld_5pt10 = new TH1F("h_ptDiffRelOld_5pt10", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffOld_5pt10 = new TH1F("h_dxyDiffOld_5pt10", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelOld_5pt10 = new TH1F("h_dxyDiffRelOld_5pt10", "", 14, -2.5, 4.5);

  TH1F *h_ptDiffNew_5pt10 = new TH1F("h_ptDiffNew_5pt10", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffNew_5pt10 = new TH1F("h_InvPtDiffNew_5pt10", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelNew_5pt10 = new TH1F("h_InvPtDiffRelNew_5pt10", "", 31, -5.5, 25.5);
  TH1F *h_ptDiffRelNew_5pt10 = new TH1F("h_ptDiffRelNew_5pt10", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffNew_5pt10 = new TH1F("h_dxyDiffNew_5pt10", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelNew_5pt10= new TH1F("h_dxyDiffRelNew_5pt10", "", 14, -2.5, 4.5);

  // 10 < pt < 20

  TH1F *h_ptDiffOld_10pt20 = new TH1F("h_ptDiffOld_10pt20", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffOld_10pt20 = new TH1F("h_InvPtDiffOld_10pt20", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelOld_10pt20 = new TH1F("h_InvPtDiffRelOld_10pt20", "", 20, -3.5,6.5);
  TH1F *h_ptDiffRelOld_10pt20 = new TH1F("h_ptDiffRelOld_10pt20", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffOld_10pt20 = new TH1F("h_dxyDiffOld_10pt20", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelOld_10pt20 = new TH1F("h_dxyDiffRelOld_10pt20", "", 14, -2.5, 4.5);

  TH1F *h_ptDiffNew_10pt20 = new TH1F("h_ptDiffNew_10pt20", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffNew_10pt20 = new TH1F("h_InvPtDiffNew_10pt20", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelNew_10pt20 = new TH1F("h_InvPtDiffRelNew_10pt20", "", 31, -5.5, 25.5);
  TH1F *h_ptDiffRelNew_10pt20 = new TH1F("h_ptDiffRelNew_10pt20", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffNew_10pt20 = new TH1F("h_dxyDiffNew_10pt20", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelNew_10pt20= new TH1F("h_dxyDiffRelNew_10pt20", "", 14, -2.5, 4.5);

  // 20 < pt
  TH1F *h_ptDiffOld_20pt = new TH1F("h_ptDiffOld_20pt", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffOld_20pt = new TH1F("h_InvPtDiffOld_20pt", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelOld_20pt = new TH1F("h_InvPtDiffRelOld_20pt", "", 20, -3.5,6.5);
  TH1F *h_ptDiffRelOld_20pt = new TH1F("h_ptDiffRelOld_20pt", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffOld_20pt = new TH1F("h_dxyDiffOld_20pt", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelOld_20pt = new TH1F("h_dxyDiffRelOld_20pt", "", 14, -2.5, 4.5);

  TH1F *h_ptDiffNew_20pt = new TH1F("h_ptDiffNew_20pt", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffNew_20pt = new TH1F("h_InvPtDiffNew_20pt", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelNew_20pt = new TH1F("h_InvPtDiffRelNew_20pt", "", 31, -5.5, 25.5);
  TH1F *h_ptDiffRelNew_20pt = new TH1F("h_ptDiffRelNew_20pt", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffNew_20pt = new TH1F("h_dxyDiffNew_20pt", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelNew_20pt= new TH1F("h_dxyDiffRelNew_20pt", "", 14, -2.5, 4.5);

  // d0 < 25
  TH1F *h_ptDiffOld_d025 = new TH1F("h_ptDiffOld_d025", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffOld_d025 = new TH1F("h_InvPtDiffOld_d025", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelOld_d025 = new TH1F("h_InvPtDiffRelOld_d025", "", 20, -3.5,6.5);
  TH1F *h_ptDiffRelOld_d025 = new TH1F("h_ptDiffRelOld_d025", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffOld_d025 = new TH1F("h_dxyDiffOld_d025", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelOld_d025 = new TH1F("h_dxyDiffRelOld_d025", "", 14, -2.5, 4.5);

  TH1F *h_ptDiffNew_d025 = new TH1F("h_ptDiffNew_d025", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffNew_d025 = new TH1F("h_InvPtDiffNew_d025", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelNew_d025 = new TH1F("h_InvPtDiffRelNew_d025", "", 31, -5.5, 25.5);
  TH1F *h_ptDiffRelNew_d025 = new TH1F("h_ptDiffRelNew_d025", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffNew_d025 = new TH1F("h_dxyDiffNew_d025", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelNew_d025= new TH1F("h_dxyDiffRelNew_d025", "", 14, -2.5, 4.5);

  // 25 < d0 < 50
  TH1F *h_ptDiffOld_25d050 = new TH1F("h_ptDiffOld_25d050", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffOld_25d050 = new TH1F("h_InvPtDiffOld_25d050", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelOld_25d050 = new TH1F("h_InvPtDiffRelOld_25d050", "", 20, -3.5,6.5);
  TH1F *h_ptDiffRelOld_25d050 = new TH1F("h_ptDiffRelOld_25d050", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffOld_25d050 = new TH1F("h_dxyDiffOld_25d050", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelOld_25d050 = new TH1F("h_dxyDiffRelOld_25d050", "", 14, -2.5, 4.5);

  TH1F *h_ptDiffNew_25d050 = new TH1F("h_ptDiffNew_25d050", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffNew_25d050 = new TH1F("h_InvPtDiffNew_25d050", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelNew_25d050 = new TH1F("h_InvPtDiffRelNew_25d050", "", 31, -5.5, 25.5);
  TH1F *h_ptDiffRelNew_25d050 = new TH1F("h_ptDiffRelNew_25d050", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffNew_25d050 = new TH1F("h_dxyDiffNew_25d050", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelNew_25d050= new TH1F("h_dxyDiffRelNew_25d050", "", 14, -2.5, 4.5);

  // 50 < d0
  TH1F *h_ptDiffOld_50d0 = new TH1F("h_ptDiffOld_50d0", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffOld_50d0 = new TH1F("h_InvPtDiffOld_50d0", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelOld_50d0 = new TH1F("h_InvPtDiffRelOld_50d0", "", 20, -3.5,6.5);
  TH1F *h_ptDiffRelOld_50d0 = new TH1F("h_ptDiffRelOld_50d0", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffOld_50d0 = new TH1F("h_dxyDiffOld_50d0", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelOld_50d0 = new TH1F("h_dxyDiffRelOld_50d0", "", 14, -2.5, 4.5);

  TH1F *h_ptDiffNew_50d0 = new TH1F("h_ptDiffNew_50d0", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffNew_50d0 = new TH1F("h_InvPtDiffNew_50d0", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelNew_50d0 = new TH1F("h_InvPtDiffRelNew_50d0", "", 31, -5.5, 25.5);
  TH1F *h_ptDiffRelNew_50d0 = new TH1F("h_ptDiffRelNew_50d0", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffNew_50d0 = new TH1F("h_dxyDiffNew_50d0", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelNew_50d0= new TH1F("h_dxyDiffRelNew_50d0", "", 14, -2.5, 4.5);

  // z0 < 25
  TH1F *h_ptDiffOld_z025 = new TH1F("h_ptDiffOld_z025", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffOld_z025 = new TH1F("h_InvPtDiffOld_z025", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelOld_z025 = new TH1F("h_InvPtDiffRelOld_z025", "", 20, -3.5,6.5);
  TH1F *h_ptDiffRelOld_z025 = new TH1F("h_ptDiffRelOld_z025", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffOld_z025 = new TH1F("h_dxyDiffOld_z025", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelOld_z025 = new TH1F("h_dxyDiffRelOld_z025", "", 14, -2.5, 4.5);

  TH1F *h_ptDiffNew_z025 = new TH1F("h_ptDiffNew_z025", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffNew_z025 = new TH1F("h_InvPtDiffNew_z025", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelNew_z025 = new TH1F("h_InvPtDiffRelNew_z025", "", 31, -5.5, 25.5);
  TH1F *h_ptDiffRelNew_z025 = new TH1F("h_ptDiffRelNew_z025", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffNew_z025 = new TH1F("h_dxyDiffNew_z025", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelNew_z025= new TH1F("h_dxyDiffRelNew_z025", "", 14, -2.5, 4.5);

  // 25 < z0 < 50
  TH1F *h_ptDiffOld_25z050 = new TH1F("h_ptDiffOld_25z050", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffOld_25z050 = new TH1F("h_InvPtDiffOld_25z050", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelOld_25z050 = new TH1F("h_InvPtDiffRelOld_25z050", "", 20, -3.5,6.5);
  TH1F *h_ptDiffRelOld_25z050 = new TH1F("h_ptDiffRelOld_25z050", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffOld_25z050 = new TH1F("h_dxyDiffOld_25z050", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelOld_25z050 = new TH1F("h_dxyDiffRelOld_25z050", "", 14, -2.5, 4.5);

  TH1F *h_ptDiffNew_25z050 = new TH1F("h_ptDiffNew_25z050", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffNew_25z050 = new TH1F("h_InvPtDiffNew_25z050", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelNew_25z050 = new TH1F("h_InvPtDiffRelNew_25z050", "", 31, -5.5, 25.5);
  TH1F *h_ptDiffRelNew_25z050 = new TH1F("h_ptDiffRelNew_25z050", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffNew_25z050 = new TH1F("h_dxyDiffNew_25z050", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelNew_25z050= new TH1F("h_dxyDiffRelNew_25z050", "", 14, -2.5, 4.5);

  // 50 < z0
  TH1F *h_ptDiffOld_50z0 = new TH1F("h_ptDiffOld_50z0", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffOld_50z0 = new TH1F("h_InvPtDiffOld_50z0", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelOld_50z0 = new TH1F("h_InvPtDiffRelOld_50z0", "", 20, -3.5,6.5);
  TH1F *h_ptDiffRelOld_50z0 = new TH1F("h_ptDiffRelOld_50z0", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffOld_50z0 = new TH1F("h_dxyDiffOld_50z0", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelOld_50z0 = new TH1F("h_dxyDiffRelOld_50z0", "", 14, -2.5, 4.5);

  TH1F *h_ptDiffNew_50z0 = new TH1F("h_ptDiffNew_50z0", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffNew_50z0 = new TH1F("h_InvPtDiffNew_50z0", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelNew_50z0 = new TH1F("h_InvPtDiffRelNew_50z0", "", 31, -5.5, 25.5);
  TH1F *h_ptDiffRelNew_50z0 = new TH1F("h_ptDiffRelNew_50z0", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffNew_50z0 = new TH1F("h_dxyDiffNew_50z0", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelNew_50z0= new TH1F("h_dxyDiffRelNew_50z0", "", 14, -2.5, 4.5);

  // 1.2 < eta < 1.6
  TH1F *h_ptDiffOld_eta1 = new TH1F("h_ptDiffOld_eta1", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffOld_eta1 = new TH1F("h_InvPtDiffOld_eta1", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelOld_eta1 = new TH1F("h_InvPtDiffRelOld_eta1", "", 20, -3.5,6.5);
  TH1F *h_ptDiffRelOld_eta1 = new TH1F("h_ptDiffRelOld_eta1", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffOld_eta1 = new TH1F("h_dxyDiffOld_eta1", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelOld_eta1 = new TH1F("h_dxyDiffRelOld_eta1", "", 14, -2.5, 4.5);

  TH1F *h_ptDiffNew_eta1 = new TH1F("h_ptDiffNew_eta1", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffNew_eta1 = new TH1F("h_InvPtDiffNew_eta1", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelNew_eta1 = new TH1F("h_InvPtDiffRelNew_eta1", "", 31, -5.5, 25.5);
  TH1F *h_ptDiffRelNew_eta1 = new TH1F("h_ptDiffRelNew_eta1", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffNew_eta1 = new TH1F("h_dxyDiffNew_eta1", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelNew_eta1= new TH1F("h_dxyDiffRelNew_eta1", "", 14, -2.5, 4.5);

  // 1.6 < eta < 2.1
  TH1F *h_ptDiffOld_eta2 = new TH1F("h_ptDiffOld_eta2", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffOld_eta2 = new TH1F("h_InvPtDiffOld_eta2", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelOld_eta2 = new TH1F("h_InvPtDiffRelOld_eta2", "", 20, -3.5,6.5);
  TH1F *h_ptDiffRelOld_eta2 = new TH1F("h_ptDiffRelOld_eta2", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffOld_eta2 = new TH1F("h_dxyDiffOld_eta2", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelOld_eta2 = new TH1F("h_dxyDiffRelOld_eta2", "", 14, -2.5, 4.5);

  TH1F *h_ptDiffNew_eta2 = new TH1F("h_ptDiffNew_eta2", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffNew_eta2 = new TH1F("h_InvPtDiffNew_eta2", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelNew_eta2 = new TH1F("h_InvPtDiffRelNew_eta2", "", 31, -5.5, 25.5);
  TH1F *h_ptDiffRelNew_eta2 = new TH1F("h_ptDiffRelNew_eta2", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffNew_eta2 = new TH1F("h_dxyDiffNew_eta2", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelNew_eta2= new TH1F("h_dxyDiffRelNew_eta2", "", 14, -2.5, 4.5);

  // 2.1 < eta < 2.5
  TH1F *h_ptDiffOld_eta3 = new TH1F("h_ptDiffOld_eta3", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffOld_eta3 = new TH1F("h_InvPtDiffOld_eta3", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelOld_eta3 = new TH1F("h_InvPtDiffRelOld_eta3", "", 20, -3.5,6.5);
  TH1F *h_ptDiffRelOld_eta3 = new TH1F("h_ptDiffRelOld_eta3", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffOld_eta3 = new TH1F("h_dxyDiffOld_eta3", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelOld_eta3 = new TH1F("h_dxyDiffRelOld_eta3", "", 14, -2.5, 4.5);

  TH1F *h_ptDiffNew_eta3 = new TH1F("h_ptDiffNew_eta3", "", 41, -20.5, 20.5);
  TH1F *h_InvPtDiffNew_eta3 = new TH1F("h_InvPtDiffNew_eta3", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiffRelNew_eta3 = new TH1F("h_InvPtDiffRelNew_eta3", "", 31, -5.5, 25.5);
  TH1F *h_ptDiffRelNew_eta3 = new TH1F("h_ptDiffRelNew_eta3", "", 12, -2.5, 3.5);
  TH1F *h_dxyDiffNew_eta3 = new TH1F("h_dxyDiffNew_eta3", "", 201, -100.5, 100.5);
  TH1F *h_dxyDiffRelNew_eta3= new TH1F("h_dxyDiffRelNew_eta3", "", 14, -2.5, 4.5);






  // Single muon efficiencies 

  int eventCount = 0;
  while(ccReaderOld.Next()){
    eventCount++;
    if (eventCount % 10000 == 0) std::cout << eventCount << " events read!" << std::endl;
    if (eventCount == 2000000) break;
    for(int i=0; i<genMuMatchedL1MuIDOld.GetSize(); i++){

      if (genMuMatchedL1MuIDOld[i] < 0) continue;
      // if (!(l1MuEmtfModeOld[genMuMatchedL1MuIDOld[i]] == 11 || l1MuEmtfModeOld[genMuMatchedL1MuIDOld[i]] == 13 || l1MuEmtfModeOld[genMuMatchedL1MuIDOld[i]] == 14 || l1MuEmtfModeOld[genMuMatchedL1MuIDOld[i]] == 15)) continue;
      // if ((l1MuEmtfModeOld[genMuMatchedL1MuIDOld[i]] == 11 || l1MuEmtfModeOld[genMuMatchedL1MuIDOld[i]] == 13 || l1MuEmtfModeOld[genMuMatchedL1MuIDOld[i]] == 14 || l1MuEmtfModeOld[genMuMatchedL1MuIDOld[i]] == 15)) continue;


      h_ptDiffOld->Fill(l1MuPtDxyOld[i] - genMuPtOld[i]);
      h_InvPtDiffOld->Fill(100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i]);
      h_InvPtDiffRelOld->Fill((100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i])/(100.0*genMuChargeOld[i]/genMuPtOld[i]));
      h_ptDiffRelOld->Fill((l1MuPtDxyOld[i] - genMuPtOld[i])/genMuPtOld[i]);
      h_dxyDiffOld->Fill(abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]));
      h_dxyDiffRelOld->Fill((abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]))/abs(genMuDxyOld[i]));

      h_ptDiffOld_2D_pt->Fill(genMuPtOld[i],l1MuPtDxyOld[i] - genMuPtOld[i]);
      h_InvPtDiffOld_2D_pt->Fill(genMuPtOld[i], 100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i]);
      h_dxyDiffOld_2D_pt->Fill(genMuPtOld[i],abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]));

      h_ptDiffOld_2D_dxy->Fill(abs(genMuDxyOld[i]),l1MuPtDxyOld[i] - genMuPtOld[i]);
      h_InvPtDiffOld_2D_dxy->Fill(abs(genMuDxyOld[i]), 100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i]);
      h_dxyDiffOld_2D_dxy->Fill(abs(genMuDxyOld[i]),abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]));

      h_ptDiffOld_2D_eta->Fill(abs(genMuEtaStarOld[i]),l1MuPtDxyOld[i] - genMuPtOld[i]);
      h_InvPtDiffOld_2D_eta->Fill(abs(genMuEtaStarOld[i]), 100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i]);
      h_dxyDiffOld_2D_eta->Fill(abs(genMuEtaStarOld[i]),abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]));


      if (genMuPtOld[i] < 5){
        h_ptDiffOld_pt5->Fill(l1MuPtDxyOld[i] - genMuPtOld[i]);
        h_InvPtDiffOld_pt5->Fill(100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i]);
        h_InvPtDiffRelOld_pt5->Fill((100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i])/(100.0*genMuChargeOld[i]/genMuPtOld[i]));
        h_ptDiffRelOld_pt5->Fill((l1MuPtDxyOld[i] - genMuPtOld[i])/genMuPtOld[i]);
        h_dxyDiffOld_pt5->Fill(abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]));
        h_dxyDiffRelOld_pt5->Fill((abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]))/abs(genMuDxyOld[i]));
      } else if (genMuPtOld[i] < 10){
        h_ptDiffOld_5pt10->Fill(l1MuPtDxyOld[i] - genMuPtOld[i]);
        h_InvPtDiffOld_5pt10->Fill(100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i]);
        h_InvPtDiffRelOld_5pt10->Fill((100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i])/(100.0*genMuChargeOld[i]/genMuPtOld[i]));
        h_ptDiffRelOld_5pt10->Fill((l1MuPtDxyOld[i] - genMuPtOld[i])/genMuPtOld[i]);
        h_dxyDiffOld_5pt10->Fill(abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]));
        h_dxyDiffRelOld_5pt10->Fill((abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]))/abs(genMuDxyOld[i]));
      } else if (genMuPtOld[i] < 20){
        h_ptDiffOld_10pt20->Fill(l1MuPtDxyOld[i] - genMuPtOld[i]);
        h_InvPtDiffOld_10pt20->Fill(100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i]);
        h_InvPtDiffRelOld_10pt20->Fill((100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i])/(100.0*genMuChargeOld[i]/genMuPtOld[i]));
        h_ptDiffRelOld_10pt20->Fill((l1MuPtDxyOld[i] - genMuPtOld[i])/genMuPtOld[i]);
        h_dxyDiffOld_10pt20->Fill(abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]));
        h_dxyDiffRelOld_10pt20->Fill((abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]))/abs(genMuDxyOld[i]));
      } else {
        h_ptDiffOld_20pt->Fill(l1MuPtDxyOld[i] - genMuPtOld[i]);
        h_InvPtDiffOld_20pt->Fill(100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i]);
        h_InvPtDiffRelOld_20pt->Fill((100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i])/(100.0*genMuChargeOld[i]/genMuPtOld[i]));
        h_ptDiffRelOld_20pt->Fill((l1MuPtDxyOld[i] - genMuPtOld[i])/genMuPtOld[i]);
        h_dxyDiffOld_20pt->Fill(abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]));
        h_dxyDiffRelOld_20pt->Fill((abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]))/abs(genMuDxyOld[i]));
      }

      if (abs(genMuDxyOld[i]) < 25){
        h_ptDiffOld_d025->Fill(l1MuPtDxyOld[i] - genMuPtOld[i]);
        h_InvPtDiffOld_d025->Fill(100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i]);
        h_InvPtDiffRelOld_d025->Fill((100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i])/(100.0*genMuChargeOld[i]/genMuPtOld[i]));
        h_ptDiffRelOld_d025->Fill((l1MuPtDxyOld[i] - genMuPtOld[i])/genMuPtOld[i]);
        h_dxyDiffOld_d025->Fill(abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]));
        h_dxyDiffRelOld_d025->Fill((abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]))/abs(genMuDxyOld[i]));
      } else if (abs(genMuDxyOld[i]) < 50){
        h_ptDiffOld_25d050->Fill(l1MuPtDxyOld[i] - genMuPtOld[i]);
        h_InvPtDiffOld_25d050->Fill(100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i]);
        h_InvPtDiffRelOld_25d050->Fill((100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i])/(100.0*genMuChargeOld[i]/genMuPtOld[i]));
        h_ptDiffRelOld_25d050->Fill((l1MuPtDxyOld[i] - genMuPtOld[i])/genMuPtOld[i]);
        h_dxyDiffOld_25d050->Fill(abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]));
        h_dxyDiffRelOld_25d050->Fill((abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]))/abs(genMuDxyOld[i]));
      } else {
        h_ptDiffOld_50d0->Fill(l1MuPtDxyOld[i] - genMuPtOld[i]);
        h_InvPtDiffOld_50d0->Fill(100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i]);
        h_InvPtDiffRelOld_50d0->Fill((100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i])/(100.0*genMuChargeOld[i]/genMuPtOld[i]));
        h_ptDiffRelOld_50d0->Fill((l1MuPtDxyOld[i] - genMuPtOld[i])/genMuPtOld[i]);
        h_dxyDiffOld_50d0->Fill(abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]));
        h_dxyDiffRelOld_50d0->Fill((abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]))/abs(genMuDxyOld[i]));
      } 

      if (abs(genMuZ0Old[i]) < 25){
        h_ptDiffOld_z025->Fill(l1MuPtDxyOld[i] - genMuPtOld[i]);
        h_InvPtDiffOld_z025->Fill(100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i]);
        h_InvPtDiffRelOld_z025->Fill((100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i])/(100.0*genMuChargeOld[i]/genMuPtOld[i]));
        h_ptDiffRelOld_z025->Fill((l1MuPtDxyOld[i] - genMuPtOld[i])/genMuPtOld[i]);
        h_dxyDiffOld_z025->Fill(abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]));
        h_dxyDiffRelOld_z025->Fill((abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]))/abs(genMuDxyOld[i]));
      } else if (abs(genMuZ0Old[i]) < 50){
        h_ptDiffOld_25z050->Fill(l1MuPtDxyOld[i] - genMuPtOld[i]);
        h_InvPtDiffOld_25z050->Fill(100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i]);
        h_InvPtDiffRelOld_25z050->Fill((100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i])/(100.0*genMuChargeOld[i]/genMuPtOld[i]));
        h_ptDiffRelOld_25z050->Fill((l1MuPtDxyOld[i] - genMuPtOld[i])/genMuPtOld[i]);
        h_dxyDiffOld_25z050->Fill(abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]));
        h_dxyDiffRelOld_25z050->Fill((abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]))/abs(genMuDxyOld[i]));
      } else {
        h_ptDiffOld_50z0->Fill(l1MuPtDxyOld[i] - genMuPtOld[i]);
        h_InvPtDiffOld_50z0->Fill(100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i]);
        h_InvPtDiffRelOld_50z0->Fill((100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i])/(100.0*genMuChargeOld[i]/genMuPtOld[i]));
        h_ptDiffRelOld_50z0->Fill((l1MuPtDxyOld[i] - genMuPtOld[i])/genMuPtOld[i]);
        h_dxyDiffOld_50z0->Fill(abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]));
        h_dxyDiffRelOld_50z0->Fill((abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]))/abs(genMuDxyOld[i]));
      } 

      if (abs(genMuEtaStarOld[i]) > 1.2 && abs(genMuEtaStarOld[i]) < 1.6){
        h_ptDiffOld_eta1->Fill(l1MuPtDxyOld[i] - genMuPtOld[i]);
        h_InvPtDiffOld_eta1->Fill(100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i]);
        h_InvPtDiffRelOld_eta1->Fill((100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i])/(100.0*genMuChargeOld[i]/genMuPtOld[i]));
        h_ptDiffRelOld_eta1->Fill((l1MuPtDxyOld[i] - genMuPtOld[i])/genMuPtOld[i]);
        h_dxyDiffOld_eta1->Fill(abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]));
        h_dxyDiffRelOld_eta1->Fill((abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]))/abs(genMuDxyOld[i]));
      } else if (abs(genMuEtaStarOld[i]) > 1.6 && abs(genMuEtaStarOld[i]) < 2.1){
        h_ptDiffOld_eta2->Fill(l1MuPtDxyOld[i] - genMuPtOld[i]);
        h_InvPtDiffOld_eta2->Fill(100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i]);
        h_InvPtDiffRelOld_eta2->Fill((100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i])/(100.0*genMuChargeOld[i]/genMuPtOld[i]));
        h_ptDiffRelOld_eta2->Fill((l1MuPtDxyOld[i] - genMuPtOld[i])/genMuPtOld[i]);
        h_dxyDiffOld_eta2->Fill(abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]));
        h_dxyDiffRelOld_eta2->Fill((abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]))/abs(genMuDxyOld[i]));
      } else if (abs(genMuEtaStarOld[i]) > 2.1 && abs(genMuEtaStarOld[i]) < 2.5){
        h_ptDiffOld_eta3->Fill(l1MuPtDxyOld[i] - genMuPtOld[i]);
        h_InvPtDiffOld_eta3->Fill(100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i]);
        h_InvPtDiffRelOld_eta3->Fill((100.0*genMuChargeOld[i]/l1MuPtDxyOld[i] - 100.0*genMuChargeOld[i]/genMuPtOld[i])/(100.0*genMuChargeOld[i]/genMuPtOld[i]));
        h_ptDiffRelOld_eta3->Fill((l1MuPtDxyOld[i] - genMuPtOld[i])/genMuPtOld[i]);
        h_dxyDiffOld_eta3->Fill(abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]));
        h_dxyDiffRelOld_eta3->Fill((abs(l1MuDxyNNOld[i]) - abs(genMuDxyOld[i]))/abs(genMuDxyOld[i]));
      } 


    }

    // for(int i=0; i<genMuMatchedL1MuIDNew.GetSize(); i++){

    //     if (genMuMatchedL1MuIDNew[i] < 0) continue;
    //     if (!(l1MuEmtfModeNew[genMuMatchedL1MuIDNew[i]] == 11 || l1MuEmtfModeNew[genMuMatchedL1MuIDNew[i]] == 13 || l1MuEmtfModeNew[genMuMatchedL1MuIDNew[i]] == 14 || l1MuEmtfModeNew[genMuMatchedL1MuIDNew[i]] == 15)) continue;
        
    //     h_ptDiffNew->Fill(l1MuPtDxyNew[i] - genMuPtNew[i]);
    //     h_InvPtDiffNew->Fill(100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i]);
    //     h_InvPtDiffRelNew->Fill((100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i])/(100.0*genMuChargeNew[i]/genMuPtNew[i]));
    //     h_ptDiffRelNew->Fill((l1MuPtDxyNew[i] - genMuPtNew[i])/genMuPtNew[i]);
    //     h_dxyDiffNew->Fill(l1MuDxyNNNew[i] - genMuDxyNew[i]);
    //     h_dxyDiffRelNew->Fill((l1MuDxyNNNew[i] - genMuDxyNew[i])/genMuDxyNew[i]);


    // }

    
  
  }

  eventCount = 0;
  while(ccReaderNew.Next()){
    eventCount++;
    if (eventCount % 10000 == 0) std::cout << eventCount << " events read!" << std::endl;
    // if (eventCount == 1000000) break;

    for(int i=0; i<genMuMatchedL1MuIDNew.GetSize(); i++){

        if (genMuMatchedL1MuIDNew[i] < 0) continue;
        // if (!(l1MuEmtfModeNew[genMuMatchedL1MuIDNew[i]] == 11 || l1MuEmtfModeNew[genMuMatchedL1MuIDNew[i]] == 13 || l1MuEmtfModeNew[genMuMatchedL1MuIDNew[i]] == 14 || l1MuEmtfModeNew[genMuMatchedL1MuIDNew[i]] == 15)) continue;
        // if ((l1MuEmtfModeNew[genMuMatchedL1MuIDNew[i]] == 11 || l1MuEmtfModeNew[genMuMatchedL1MuIDNew[i]] == 13 || l1MuEmtfModeNew[genMuMatchedL1MuIDNew[i]] == 14 || l1MuEmtfModeNew[genMuMatchedL1MuIDNew[i]] == 15)) continue;
        
        h_ptDiffNew->Fill(l1MuPtDxyNew[i] - genMuPtNew[i]);
        h_InvPtDiffNew->Fill(100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i]);
        h_InvPtDiffRelNew->Fill((100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i])/(100.0*genMuChargeNew[i]/genMuPtNew[i]));
        h_ptDiffRelNew->Fill((l1MuPtDxyNew[i] - genMuPtNew[i])/genMuPtNew[i]);
        h_dxyDiffNew->Fill(abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]));
        h_dxyDiffRelNew->Fill((abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]))/abs(genMuDxyNew[i]));

        h_ptDiffNew_2D_pt->Fill(genMuPtNew[i],l1MuPtDxyNew[i] - genMuPtNew[i]);
        h_InvPtDiffNew_2D_pt->Fill(genMuPtNew[i], 100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i]);
        h_dxyDiffNew_2D_pt->Fill(genMuPtNew[i],abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]));

        h_ptDiffNew_2D_dxy->Fill(abs(genMuDxyNew[i]),l1MuPtDxyNew[i] - genMuPtNew[i]);
        h_InvPtDiffNew_2D_dxy->Fill(abs(genMuDxyNew[i]), 100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i]);
        h_dxyDiffNew_2D_dxy->Fill(abs(genMuDxyNew[i]),abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]));

        h_ptDiffNew_2D_eta->Fill(abs(genMuEtaStarNew[i]),l1MuPtDxyNew[i] - genMuPtNew[i]);
        h_InvPtDiffNew_2D_eta->Fill(abs(genMuEtaStarNew[i]), 100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i]);
        h_dxyDiffNew_2D_eta->Fill(abs(genMuEtaStarNew[i]),abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]));


        if (genMuPtNew[i] < 5){
        h_ptDiffNew_pt5->Fill(l1MuPtDxyNew[i] - genMuPtNew[i]);
        h_InvPtDiffNew_pt5->Fill(100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i]);
        h_InvPtDiffRelNew_pt5->Fill((100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i])/(100.0*genMuChargeNew[i]/genMuPtNew[i]));
        h_ptDiffRelNew_pt5->Fill((l1MuPtDxyNew[i] - genMuPtNew[i])/genMuPtNew[i]);
        h_dxyDiffNew_pt5->Fill(abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]));
        h_dxyDiffRelNew_pt5->Fill((abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]))/abs(genMuDxyNew[i]));
      } else if (genMuPtNew[i] < 10){
        h_ptDiffNew_5pt10->Fill(l1MuPtDxyNew[i] - genMuPtNew[i]);
        h_InvPtDiffNew_5pt10->Fill(100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i]);
        h_InvPtDiffRelNew_5pt10->Fill((100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i])/(100.0*genMuChargeNew[i]/genMuPtNew[i]));
        h_ptDiffRelNew_5pt10->Fill((l1MuPtDxyNew[i] - genMuPtNew[i])/genMuPtNew[i]);
        h_dxyDiffNew_5pt10->Fill(abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]));
        h_dxyDiffRelNew_5pt10->Fill((abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]))/abs(genMuDxyNew[i]));
      } else if (genMuPtNew[i] < 20){
        h_ptDiffNew_10pt20->Fill(l1MuPtDxyNew[i] - genMuPtNew[i]);
        h_InvPtDiffNew_10pt20->Fill(100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i]);
        h_InvPtDiffRelNew_10pt20->Fill((100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i])/(100.0*genMuChargeNew[i]/genMuPtNew[i]));
        h_ptDiffRelNew_10pt20->Fill((l1MuPtDxyNew[i] - genMuPtNew[i])/genMuPtNew[i]);
        h_dxyDiffNew_10pt20->Fill(abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]));
        h_dxyDiffRelNew_10pt20->Fill((abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]))/abs(genMuDxyNew[i]));
      } else {
        h_ptDiffNew_20pt->Fill(l1MuPtDxyNew[i] - genMuPtNew[i]);
        h_InvPtDiffNew_20pt->Fill(100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i]);
        h_InvPtDiffRelNew_20pt->Fill((100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i])/(100.0*genMuChargeNew[i]/genMuPtNew[i]));
        h_ptDiffRelNew_20pt->Fill((l1MuPtDxyNew[i] - genMuPtNew[i])/genMuPtNew[i]);
        h_dxyDiffNew_20pt->Fill(abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]));
        h_dxyDiffRelNew_20pt->Fill((abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]))/abs(genMuDxyNew[i]));
      }

      if (abs(genMuDxyNew[i]) < 25){
        h_ptDiffNew_d025->Fill(l1MuPtDxyNew[i] - genMuPtNew[i]);
        h_InvPtDiffNew_d025->Fill(100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i]);
        h_InvPtDiffRelNew_d025->Fill((100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i])/(100.0*genMuChargeNew[i]/genMuPtNew[i]));
        h_ptDiffRelNew_d025->Fill((l1MuPtDxyNew[i] - genMuPtNew[i])/genMuPtNew[i]);
        h_dxyDiffNew_d025->Fill(abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]));
        h_dxyDiffRelNew_d025->Fill((abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]))/abs(genMuDxyNew[i]));
      } else if (abs(genMuDxyNew[i]) < 50){
        h_ptDiffNew_25d050->Fill(l1MuPtDxyNew[i] - genMuPtNew[i]);
        h_InvPtDiffNew_25d050->Fill(100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i]);
        h_InvPtDiffRelNew_25d050->Fill((100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i])/(100.0*genMuChargeNew[i]/genMuPtNew[i]));
        h_ptDiffRelNew_25d050->Fill((l1MuPtDxyNew[i] - genMuPtNew[i])/genMuPtNew[i]);
        h_dxyDiffNew_25d050->Fill(abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]));
        h_dxyDiffRelNew_25d050->Fill((abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]))/abs(genMuDxyNew[i]));
      } else {
        h_ptDiffNew_50d0->Fill(l1MuPtDxyNew[i] - genMuPtNew[i]);
        h_InvPtDiffNew_50d0->Fill(100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i]);
        h_InvPtDiffRelNew_50d0->Fill((100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i])/(100.0*genMuChargeNew[i]/genMuPtNew[i]));
        h_ptDiffRelNew_50d0->Fill((l1MuPtDxyNew[i] - genMuPtNew[i])/genMuPtNew[i]);
        h_dxyDiffNew_50d0->Fill(abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]));
        h_dxyDiffRelNew_50d0->Fill((abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]))/abs(genMuDxyNew[i]));
      } 

      if (abs(genMuZ0New[i]) < 25){
        h_ptDiffNew_z025->Fill(l1MuPtDxyNew[i] - genMuPtNew[i]);
        h_InvPtDiffNew_z025->Fill(100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i]);
        h_InvPtDiffRelNew_z025->Fill((100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i])/(100.0*genMuChargeNew[i]/genMuPtNew[i]));
        h_ptDiffRelNew_z025->Fill((l1MuPtDxyNew[i] - genMuPtNew[i])/genMuPtNew[i]);
        h_dxyDiffNew_z025->Fill(abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]));
        h_dxyDiffRelNew_z025->Fill((abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]))/abs(genMuDxyNew[i]));
      } else if (abs(genMuZ0New[i]) < 50){
        h_ptDiffNew_25z050->Fill(l1MuPtDxyNew[i] - genMuPtNew[i]);
        h_InvPtDiffNew_25z050->Fill(100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i]);
        h_InvPtDiffRelNew_25z050->Fill((100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i])/(100.0*genMuChargeNew[i]/genMuPtNew[i]));
        h_ptDiffRelNew_25z050->Fill((l1MuPtDxyNew[i] - genMuPtNew[i])/genMuPtNew[i]);
        h_dxyDiffNew_25z050->Fill(abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]));
        h_dxyDiffRelNew_25z050->Fill((abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]))/abs(genMuDxyNew[i]));
      } else {
        h_ptDiffNew_50z0->Fill(l1MuPtDxyNew[i] - genMuPtNew[i]);
        h_InvPtDiffNew_50z0->Fill(100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i]);
        h_InvPtDiffRelNew_50z0->Fill((100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i])/(100.0*genMuChargeNew[i]/genMuPtNew[i]));
        h_ptDiffRelNew_50z0->Fill((l1MuPtDxyNew[i] - genMuPtNew[i])/genMuPtNew[i]);
        h_dxyDiffNew_50z0->Fill(abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]));
        h_dxyDiffRelNew_50z0->Fill((abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]))/abs(genMuDxyNew[i]));
      } 

      if (abs(genMuEtaStarNew[i]) > 1.2 && abs(genMuEtaStarNew[i]) < 1.6){
        h_ptDiffNew_eta1->Fill(l1MuPtDxyNew[i] - genMuPtNew[i]);
        h_InvPtDiffNew_eta1->Fill(100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i]);
        h_InvPtDiffRelNew_eta1->Fill((100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i])/(100.0*genMuChargeNew[i]/genMuPtNew[i]));
        h_ptDiffRelNew_eta1->Fill((l1MuPtDxyNew[i] - genMuPtNew[i])/genMuPtNew[i]);
        h_dxyDiffNew_eta1->Fill(abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]));
        h_dxyDiffRelNew_eta1->Fill((abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]))/abs(genMuDxyNew[i]));
      } else if (abs(genMuEtaStarNew[i]) > 1.6 && abs(genMuEtaStarNew[i]) < 2.1){
        h_ptDiffNew_eta2->Fill(l1MuPtDxyNew[i] - genMuPtNew[i]);
        h_InvPtDiffNew_eta2->Fill(100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i]);
        h_InvPtDiffRelNew_eta2->Fill((100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i])/(100.0*genMuChargeNew[i]/genMuPtNew[i]));
        h_ptDiffRelNew_eta2->Fill((l1MuPtDxyNew[i] - genMuPtNew[i])/genMuPtNew[i]);
        h_dxyDiffNew_eta2->Fill(abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]));
        h_dxyDiffRelNew_eta2->Fill((abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]))/abs(genMuDxyNew[i]));
      } else if (abs(genMuEtaStarNew[i]) > 2.1 && abs(genMuEtaStarNew[i]) < 2.5){
        h_ptDiffNew_eta3->Fill(l1MuPtDxyNew[i] - genMuPtNew[i]);
        h_InvPtDiffNew_eta3->Fill(100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i]);
        h_InvPtDiffRelNew_eta3->Fill((100.0*genMuChargeNew[i]/l1MuPtDxyNew[i] - 100.0*genMuChargeNew[i]/genMuPtNew[i])/(100.0*genMuChargeNew[i]/genMuPtNew[i]));
        h_ptDiffRelNew_eta3->Fill((l1MuPtDxyNew[i] - genMuPtNew[i])/genMuPtNew[i]);
        h_dxyDiffNew_eta3->Fill(abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]));
        h_dxyDiffRelNew_eta3->Fill((abs(l1MuDxyNNNew[i]) - abs(genMuDxyNew[i]))/abs(genMuDxyNew[i]));
      } 


    }
  }


  h_ptDiffOld->GetXaxis()->SetTitle("p_{T} (NN) - p_{T} (GEN) [GeV]");
  h_ptDiffOld->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffOld->GetYaxis()->SetRangeUser(1,h_ptDiffNew->GetMaximum()*1.2);

  h_InvPtDiffOld->GetXaxis()->SetTitle("100*q/p_{T} (NN) - 100*q/p_{T} (GEN) [GeV]^{-1}");
  h_InvPtDiffOld->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffOld->GetYaxis()->SetRangeUser(1,h_InvPtDiffNew->GetMaximum()*1.2);

  h_InvPtDiffRelOld->GetXaxis()->SetTitle("(1/p_{T} (NN) - 1/p_{T} (GEN))/(1/p_{T} (GEN))");
  h_InvPtDiffRelOld->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffRelOld->GetYaxis()->SetRangeUser(1,h_InvPtDiffRelNew->GetMaximum()*1.2);

  h_ptDiffRelOld->GetXaxis()->SetTitle("(p_{T} (NN) - p_{T} (GEN))/(p_{T} (GEN))");
  h_ptDiffRelOld->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffRelOld->GetYaxis()->SetRangeUser(1,h_ptDiffRelNew->GetMaximum()*1.2);

  h_dxyDiffOld->GetXaxis()->SetTitle("Dxy (NN) - Dxy (GEN) [cm]");
  h_dxyDiffOld->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffOld->GetYaxis()->SetRangeUser(1,h_dxyDiffNew->GetMaximum()*1.2);

  h_dxyDiffRelOld->GetXaxis()->SetTitle("(Dxy (NN) - Dxy (GEN))/(Dxy (GEN))");
  h_dxyDiffRelOld->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffRelOld->GetYaxis()->SetRangeUser(1,h_dxyDiffRelNew->GetMaximum()*1.2);

  // 2D plots
  h_ptDiffOld_2D_pt->GetYaxis()->SetTitle("p_{T} (NN) - p_{T} (GEN) [GeV]");
  h_ptDiffOld_2D_pt->GetXaxis()->SetTitle("p_{T} (GEN) [GeV]");
  h_ptDiffOld_2D_pt->GetZaxis()->SetRangeUser(100,h_ptDiffOld_2D_pt->GetMaximum());
  // h_ptDiffOld_2D_pt->Scale(1/h_ptDiffOld_2D_pt->Integral());

  h_ptDiffOld_2D_dxy->GetYaxis()->SetTitle("p_{T} (NN) - p_{T} (GEN) [GeV]");
  h_ptDiffOld_2D_dxy->GetXaxis()->SetTitle("Dxy (GEN) [cm]");
  h_ptDiffOld_2D_dxy->GetZaxis()->SetRangeUser(100,h_ptDiffOld_2D_dxy->GetMaximum());
  // h_ptDiffOld_2D_dxy->Scale(1/h_ptDiffOld_2D_dxy->Integral());

  h_ptDiffOld_2D_eta->GetYaxis()->SetTitle("p_{T} (NN) - p_{T} (GEN) [GeV]");
  h_ptDiffOld_2D_eta->GetXaxis()->SetTitle("#eta (GEN)");
  h_ptDiffOld_2D_eta->GetZaxis()->SetRangeUser(100,h_ptDiffOld_2D_eta->GetMaximum());
  // h_ptDiffOld_2D_eta->Scale(1/h_ptDiffOld_2D_eta->Integral());

  h_InvPtDiffOld_2D_pt->GetYaxis()->SetTitle("100*q/p_{T} (NN) - 100*q/p_{T} (GEN) [GeV]^{-1}");
  h_InvPtDiffOld_2D_pt->GetXaxis()->SetTitle("p_{T} (GEN) [GeV]");
  h_InvPtDiffOld_2D_pt->GetZaxis()->SetRangeUser(100,h_InvPtDiffOld_2D_pt->GetMaximum());
  // h_InvPtDiffOld_2D_pt->Scale(1/h_InvPtDiffOld_2D_pt->Integral());

  h_InvPtDiffOld_2D_dxy->GetYaxis()->SetTitle("100*q/p_{T} (NN) - 100*q/p_{T} (GEN) [GeV]^{-1}");
  h_InvPtDiffOld_2D_dxy->GetXaxis()->SetTitle("Dxy (GEN) [cm]");
  h_InvPtDiffOld_2D_dxy->GetZaxis()->SetRangeUser(100,h_InvPtDiffOld_2D_dxy->GetMaximum());
  // h_InvPtDiffOld_2D_dxy->Scale(1/h_InvPtDiffOld_2D_dxy->Integral());

  h_InvPtDiffOld_2D_eta->GetYaxis()->SetTitle("100*q/p_{T} (NN) - 100*q/p_{T} (GEN) [GeV]^{-1}");
  h_InvPtDiffOld_2D_eta->GetXaxis()->SetTitle("#eta (GEN)");
  h_InvPtDiffOld_2D_eta->GetZaxis()->SetRangeUser(100,h_InvPtDiffOld_2D_eta->GetMaximum());
  // h_InvPtDiffOld_2D_eta->Scale(1/h_InvPtDiffOld_2D_eta->Integral());

  h_dxyDiffOld_2D_pt->GetYaxis()->SetTitle("Dxy (NN) - Dxy (GEN) [cm]");
  h_dxyDiffOld_2D_pt->GetXaxis()->SetTitle("p_{T} (GEN) [GeV]");
  h_dxyDiffOld_2D_pt->GetZaxis()->SetRangeUser(100,h_dxyDiffOld_2D_pt->GetMaximum());
  // h_dxyDiffOld_2D_pt->Scale(1/h_dxyDiffOld_2D_pt->Integral());

  h_dxyDiffOld_2D_dxy->GetYaxis()->SetTitle("Dxy (NN) - Dxy (GEN) [cm]");
  h_dxyDiffOld_2D_dxy->GetXaxis()->SetTitle("Dxy (GEN) [cm]");
  h_dxyDiffOld_2D_dxy->GetZaxis()->SetRangeUser(100,h_dxyDiffOld_2D_dxy->GetMaximum());
  // h_dxyDiffOld_2D_dxy->Scale(1/h_dxyDiffOld_2D_dxy->Integral());

  h_dxyDiffOld_2D_eta->GetYaxis()->SetTitle("Dxy (NN) - Dxy (GEN) [cm]");
  h_dxyDiffOld_2D_eta->GetXaxis()->SetTitle("#eta (GEN)");
  h_dxyDiffOld_2D_eta->GetZaxis()->SetRangeUser(100,h_dxyDiffOld_2D_eta->GetMaximum());
  // h_dxyDiffOld_2D_eta->Scale(1/h_dxyDiffOld_2D_eta->Integral());

  h_ptDiffNew_2D_pt->GetYaxis()->SetTitle("p_{T} (NN) - p_{T} (GEN) [GeV]");
  h_ptDiffNew_2D_pt->GetXaxis()->SetTitle("p_{T} (GEN) [GeV]");
  h_ptDiffNew_2D_pt->GetZaxis()->SetRangeUser(100,h_ptDiffNew_2D_pt->GetMaximum());
  // h_ptDiffNew_2D_pt->Scale(1/h_ptDiffNew_2D_pt->Integral());

  h_ptDiffNew_2D_dxy->GetYaxis()->SetTitle("p_{T} (NN) - p_{T} (GEN) [GeV]");
  h_ptDiffNew_2D_dxy->GetXaxis()->SetTitle("Dxy (GEN) [cm]");
  h_ptDiffNew_2D_dxy->GetZaxis()->SetRangeUser(100,h_ptDiffNew_2D_dxy->GetMaximum());
  // h_ptDiffNew_2D_dxy->Scale(1/h_ptDiffNew_2D_dxy->Integral());

  h_ptDiffNew_2D_eta->GetYaxis()->SetTitle("p_{T} (NN) - p_{T} (GEN) [GeV]");
  h_ptDiffNew_2D_eta->GetXaxis()->SetTitle("#eta (GEN)");
  h_ptDiffNew_2D_eta->GetZaxis()->SetRangeUser(100,h_ptDiffNew_2D_eta->GetMaximum());
  // h_ptDiffNew_2D_eta->Scale(1/h_ptDiffNew_2D_eta->Integral());

  h_InvPtDiffNew_2D_pt->GetYaxis()->SetTitle("100*q/p_{T} (NN) - 100*q/p_{T} (GEN) [GeV]^{-1}");
  h_InvPtDiffNew_2D_pt->GetXaxis()->SetTitle("p_{T} (GEN) [GeV]");
  h_InvPtDiffNew_2D_pt->GetZaxis()->SetRangeUser(100,h_InvPtDiffNew_2D_pt->GetMaximum());
  // h_InvPtDiffNew_2D_pt->Scale(1/h_InvPtDiffNew_2D_pt->Integral());

  h_InvPtDiffNew_2D_dxy->GetYaxis()->SetTitle("100*q/p_{T} (NN) - 100*q/p_{T} (GEN) [GeV]^{-1}");
  h_InvPtDiffNew_2D_dxy->GetXaxis()->SetTitle("Dxy (GEN) [cm]");
  h_InvPtDiffNew_2D_dxy->GetZaxis()->SetRangeUser(100,h_InvPtDiffNew_2D_dxy->GetMaximum());
  // h_InvPtDiffNew_2D_dxy->Scale(1/h_InvPtDiffNew_2D_dxy->Integral());

  h_InvPtDiffNew_2D_eta->GetYaxis()->SetTitle("100*q/p_{T} (NN) - 100*q/p_{T} (GEN) [GeV]^{-1}");
  h_InvPtDiffNew_2D_eta->GetXaxis()->SetTitle("#eta (GEN)");
  h_InvPtDiffNew_2D_eta->GetZaxis()->SetRangeUser(100,h_InvPtDiffNew_2D_eta->GetMaximum());
  // h_InvPtDiffNew_2D_eta->Scale(1/h_InvPtDiffNew_2D_eta->Integral());

  h_dxyDiffNew_2D_pt->GetYaxis()->SetTitle("Dxy (NN) - Dxy (GEN) [cm]");
  h_dxyDiffNew_2D_pt->GetXaxis()->SetTitle("p_{T} (GEN) [GeV]");
  h_dxyDiffNew_2D_pt->GetZaxis()->SetRangeUser(100,h_dxyDiffNew_2D_pt->GetMaximum());
  // h_dxyDiffNew_2D_pt->Scale(1/h_dxyDiffNew_2D_pt->Integral());

  h_dxyDiffNew_2D_dxy->GetYaxis()->SetTitle("Dxy (NN) - Dxy (GEN) [cm]");
  h_dxyDiffNew_2D_dxy->GetXaxis()->SetTitle("Dxy (GEN) [cm]");
  h_dxyDiffNew_2D_dxy->GetZaxis()->SetRangeUser(100,h_dxyDiffNew_2D_dxy->GetMaximum());
  // h_dxyDiffNew_2D_dxy->Scale(1/h_dxyDiffNew_2D_dxy->Integral());

  h_dxyDiffNew_2D_eta->GetYaxis()->SetTitle("Dxy (NN) - Dxy (GEN) [cm]");
  h_dxyDiffNew_2D_eta->GetXaxis()->SetTitle("#eta (GEN)");
  h_dxyDiffNew_2D_eta->GetZaxis()->SetRangeUser(100,h_dxyDiffNew_2D_eta->GetMaximum());
  // h_dxyDiffNew_2D_eta->Scale(1/h_dxyDiffNew_2D_eta->Integral());



  // pt < 5
  h_ptDiffOld_pt5->GetXaxis()->SetTitle("p_{T} (NN) - p_{T} (GEN) [GeV]");
  h_ptDiffOld_pt5->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffOld_pt5->GetYaxis()->SetRangeUser(1,h_ptDiffNew_pt5->GetMaximum()*1.2);

  h_InvPtDiffOld_pt5->GetXaxis()->SetTitle("100*q/p_{T} (NN) - 100*q/p_{T} (GEN) [GeV]^{-1}");
  h_InvPtDiffOld_pt5->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffOld_pt5->GetYaxis()->SetRangeUser(1,h_InvPtDiffNew_pt5->GetMaximum()*1.2);

  h_InvPtDiffRelOld_pt5->GetXaxis()->SetTitle("(1/p_{T} (NN) - 1/p_{T} (GEN))/(1/p_{T} (GEN))");
  h_InvPtDiffRelOld_pt5->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffRelOld_pt5->GetYaxis()->SetRangeUser(1,h_InvPtDiffRelNew_pt5->GetMaximum()*1.2);

  h_ptDiffRelOld_pt5->GetXaxis()->SetTitle("(p_{T} (NN) - p_{T} (GEN))/(p_{T} (GEN))");
  h_ptDiffRelOld_pt5->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffRelOld_pt5->GetYaxis()->SetRangeUser(1,h_ptDiffRelNew_pt5->GetMaximum()*1.2);

  h_dxyDiffOld_pt5->GetXaxis()->SetTitle("Dxy (NN) - Dxy (GEN) [cm]");
  h_dxyDiffOld_pt5->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffOld_pt5->GetYaxis()->SetRangeUser(1,h_dxyDiffNew_pt5->GetMaximum()*1.2);

  h_dxyDiffRelOld_pt5->GetXaxis()->SetTitle("(Dxy (NN) - Dxy (GEN))/(Dxy (GEN))");
  h_dxyDiffRelOld_pt5->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffRelOld_pt5->GetYaxis()->SetRangeUser(1,h_dxyDiffRelNew_pt5->GetMaximum()*1.2);

  // 5 < pt < 10
  h_ptDiffOld_5pt10->GetXaxis()->SetTitle("p_{T} (NN) - p_{T} (GEN) [GeV]");
  h_ptDiffOld_5pt10->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffOld_5pt10->GetYaxis()->SetRangeUser(1,h_ptDiffNew_5pt10->GetMaximum()*1.2);

  h_InvPtDiffOld_5pt10->GetXaxis()->SetTitle("100*q/p_{T} (NN) - 100*q/p_{T} (GEN) [GeV]^{-1}");
  h_InvPtDiffOld_5pt10->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffOld_5pt10->GetYaxis()->SetRangeUser(1,h_InvPtDiffNew_5pt10->GetMaximum()*1.2);

  h_InvPtDiffRelOld_5pt10->GetXaxis()->SetTitle("(1/p_{T} (NN) - 1/p_{T} (GEN))/(1/p_{T} (GEN))");
  h_InvPtDiffRelOld_5pt10->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffRelOld_5pt10->GetYaxis()->SetRangeUser(1,h_InvPtDiffRelNew_5pt10->GetMaximum()*1.2);

  h_ptDiffRelOld_5pt10->GetXaxis()->SetTitle("(p_{T} (NN) - p_{T} (GEN))/(p_{T} (GEN))");
  h_ptDiffRelOld_5pt10->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffRelOld_5pt10->GetYaxis()->SetRangeUser(1,h_ptDiffRelNew_5pt10->GetMaximum()*1.2);

  h_dxyDiffOld_5pt10->GetXaxis()->SetTitle("Dxy (NN) - Dxy (GEN) [cm]");
  h_dxyDiffOld_5pt10->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffOld_5pt10->GetYaxis()->SetRangeUser(1,h_dxyDiffNew_5pt10->GetMaximum()*1.2);

  h_dxyDiffRelOld_5pt10->GetXaxis()->SetTitle("(Dxy (NN) - Dxy (GEN))/(Dxy (GEN))");
  h_dxyDiffRelOld_5pt10->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffRelOld_5pt10->GetYaxis()->SetRangeUser(1,h_dxyDiffRelNew_5pt10->GetMaximum()*1.2);

  // 10 < pt < 20
  h_ptDiffOld_10pt20->GetXaxis()->SetTitle("p_{T} (NN) - p_{T} (GEN) [GeV]");
  h_ptDiffOld_10pt20->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffOld_10pt20->GetYaxis()->SetRangeUser(1,h_ptDiffNew_10pt20->GetMaximum()*1.2);

  h_InvPtDiffOld_10pt20->GetXaxis()->SetTitle("100*q/p_{T} (NN) - 100*q/p_{T} (GEN) [GeV]^{-1}");
  h_InvPtDiffOld_10pt20->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffOld_10pt20->GetYaxis()->SetRangeUser(1,h_InvPtDiffNew_10pt20->GetMaximum()*1.2);

  h_InvPtDiffRelOld_10pt20->GetXaxis()->SetTitle("(1/p_{T} (NN) - 1/p_{T} (GEN))/(1/p_{T} (GEN))");
  h_InvPtDiffRelOld_10pt20->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffRelOld_10pt20->GetYaxis()->SetRangeUser(1,h_InvPtDiffRelNew_10pt20->GetMaximum()*1.2);

  h_ptDiffRelOld_10pt20->GetXaxis()->SetTitle("(p_{T} (NN) - p_{T} (GEN))/(p_{T} (GEN))");
  h_ptDiffRelOld_10pt20->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffRelOld_10pt20->GetYaxis()->SetRangeUser(1,h_ptDiffRelNew_10pt20->GetMaximum()*1.2);

  h_dxyDiffOld_10pt20->GetXaxis()->SetTitle("Dxy (NN) - Dxy (GEN) [cm]");
  h_dxyDiffOld_10pt20->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffOld_10pt20->GetYaxis()->SetRangeUser(1,h_dxyDiffNew_10pt20->GetMaximum()*1.2);

  h_dxyDiffRelOld_10pt20->GetXaxis()->SetTitle("(Dxy (NN) - Dxy (GEN))/(Dxy (GEN))");
  h_dxyDiffRelOld_10pt20->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffRelOld_10pt20->GetYaxis()->SetRangeUser(1,h_dxyDiffRelNew_10pt20->GetMaximum()*1.2);

  // 20 < pt
  h_ptDiffOld_20pt->GetXaxis()->SetTitle("p_{T} (NN) - p_{T} (GEN) [GeV]");
  h_ptDiffOld_20pt->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffOld_20pt->GetYaxis()->SetRangeUser(1,h_ptDiffNew_20pt->GetMaximum()*1.2);

  h_InvPtDiffOld_20pt->GetXaxis()->SetTitle("100*q/p_{T} (NN) - 100*q/p_{T} (GEN) [GeV]^{-1}");
  h_InvPtDiffOld_20pt->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffOld_20pt->GetYaxis()->SetRangeUser(1,h_InvPtDiffNew_20pt->GetMaximum()*1.2);

  h_InvPtDiffRelOld_20pt->GetXaxis()->SetTitle("(1/p_{T} (NN) - 1/p_{T} (GEN))/(1/p_{T} (GEN))");
  h_InvPtDiffRelOld_20pt->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffRelOld_20pt->GetYaxis()->SetRangeUser(1,h_InvPtDiffRelNew_20pt->GetMaximum()*1.2);

  h_ptDiffRelOld_20pt->GetXaxis()->SetTitle("(p_{T} (NN) - p_{T} (GEN))/(p_{T} (GEN))");
  h_ptDiffRelOld_20pt->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffRelOld_20pt->GetYaxis()->SetRangeUser(1,h_ptDiffRelNew_20pt->GetMaximum()*1.2);

  h_dxyDiffOld_20pt->GetXaxis()->SetTitle("Dxy (NN) - Dxy (GEN) [cm]");
  h_dxyDiffOld_20pt->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffOld_20pt->GetYaxis()->SetRangeUser(1,h_dxyDiffNew_20pt->GetMaximum()*1.2);

  h_dxyDiffRelOld_20pt->GetXaxis()->SetTitle("(Dxy (NN) - Dxy (GEN))/(Dxy (GEN))");
  h_dxyDiffRelOld_20pt->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffRelOld_20pt->GetYaxis()->SetRangeUser(1,h_dxyDiffRelNew_20pt->GetMaximum()*1.2);

  // d0 < 25
  h_ptDiffOld_d025->GetXaxis()->SetTitle("p_{T} (NN) - p_{T} (GEN) [GeV]");
  h_ptDiffOld_d025->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffOld_d025->GetYaxis()->SetRangeUser(1,h_ptDiffNew_d025->GetMaximum()*1.2);

  h_InvPtDiffOld_d025->GetXaxis()->SetTitle("100*q/p_{T} (NN) - 100*q/p_{T} (GEN) [GeV]^{-1}");
  h_InvPtDiffOld_d025->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffOld_d025->GetYaxis()->SetRangeUser(1,h_InvPtDiffNew_d025->GetMaximum()*1.2);

  h_InvPtDiffRelOld_d025->GetXaxis()->SetTitle("(1/p_{T} (NN) - 1/p_{T} (GEN))/(1/p_{T} (GEN))");
  h_InvPtDiffRelOld_d025->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffRelOld_d025->GetYaxis()->SetRangeUser(1,h_InvPtDiffRelNew_d025->GetMaximum()*1.2);

  h_ptDiffRelOld_d025->GetXaxis()->SetTitle("(p_{T} (NN) - p_{T} (GEN))/(p_{T} (GEN))");
  h_ptDiffRelOld_d025->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffRelOld_d025->GetYaxis()->SetRangeUser(1,h_ptDiffRelNew_d025->GetMaximum()*1.2);

  h_dxyDiffOld_d025->GetXaxis()->SetTitle("Dxy (NN) - Dxy (GEN) [cm]");
  h_dxyDiffOld_d025->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffOld_d025->GetYaxis()->SetRangeUser(1,h_dxyDiffNew_d025->GetMaximum()*1.2);

  h_dxyDiffRelOld_d025->GetXaxis()->SetTitle("(Dxy (NN) - Dxy (GEN))/(Dxy (GEN))");
  h_dxyDiffRelOld_d025->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffRelOld_d025->GetYaxis()->SetRangeUser(1,h_dxyDiffRelNew_d025->GetMaximum()*1.2);

  // 25 < d0 < 50
  h_ptDiffOld_25d050->GetXaxis()->SetTitle("p_{T} (NN) - p_{T} (GEN) [GeV]");
  h_ptDiffOld_25d050->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffOld_25d050->GetYaxis()->SetRangeUser(1,h_ptDiffNew_25d050->GetMaximum()*1.2);

  h_InvPtDiffOld_25d050->GetXaxis()->SetTitle("100*q/p_{T} (NN) - 100*q/p_{T} (GEN) [GeV]^{-1}");
  h_InvPtDiffOld_25d050->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffOld_25d050->GetYaxis()->SetRangeUser(1,h_InvPtDiffNew_25d050->GetMaximum()*1.2);

  h_InvPtDiffRelOld_25d050->GetXaxis()->SetTitle("(1/p_{T} (NN) - 1/p_{T} (GEN))/(1/p_{T} (GEN))");
  h_InvPtDiffRelOld_25d050->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffRelOld_25d050->GetYaxis()->SetRangeUser(1,h_InvPtDiffRelNew_25d050->GetMaximum()*1.2);

  h_ptDiffRelOld_25d050->GetXaxis()->SetTitle("(p_{T} (NN) - p_{T} (GEN))/(p_{T} (GEN))");
  h_ptDiffRelOld_25d050->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffRelOld_25d050->GetYaxis()->SetRangeUser(1,h_ptDiffRelNew_25d050->GetMaximum()*1.2);

  h_dxyDiffOld_25d050->GetXaxis()->SetTitle("Dxy (NN) - Dxy (GEN) [cm]");
  h_dxyDiffOld_25d050->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffOld_25d050->GetYaxis()->SetRangeUser(1,h_dxyDiffNew_25d050->GetMaximum()*1.2);

  h_dxyDiffRelOld_25d050->GetXaxis()->SetTitle("(Dxy (NN) - Dxy (GEN))/(Dxy (GEN))");
  h_dxyDiffRelOld_25d050->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffRelOld_25d050->GetYaxis()->SetRangeUser(1,h_dxyDiffRelNew_25d050->GetMaximum()*1.2);

  // 50 < d0
  h_ptDiffOld_50d0->GetXaxis()->SetTitle("p_{T} (NN) - p_{T} (GEN) [GeV]");
  h_ptDiffOld_50d0->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffOld_50d0->GetYaxis()->SetRangeUser(1,h_ptDiffNew_50d0->GetMaximum()*1.2);

  h_InvPtDiffOld_50d0->GetXaxis()->SetTitle("100*q/p_{T} (NN) - 100*q/p_{T} (GEN) [GeV]^{-1}");
  h_InvPtDiffOld_50d0->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffOld_50d0->GetYaxis()->SetRangeUser(1,h_InvPtDiffNew_50d0->GetMaximum()*1.2);

  h_InvPtDiffRelOld_50d0->GetXaxis()->SetTitle("(1/p_{T} (NN) - 1/p_{T} (GEN))/(1/p_{T} (GEN))");
  h_InvPtDiffRelOld_50d0->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffRelOld_50d0->GetYaxis()->SetRangeUser(1,h_InvPtDiffRelNew_50d0->GetMaximum()*1.2);

  h_ptDiffRelOld_50d0->GetXaxis()->SetTitle("(p_{T} (NN) - p_{T} (GEN))/(p_{T} (GEN))");
  h_ptDiffRelOld_50d0->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffRelOld_50d0->GetYaxis()->SetRangeUser(1,h_ptDiffRelNew_50d0->GetMaximum()*1.2);

  h_dxyDiffOld_50d0->GetXaxis()->SetTitle("Dxy (NN) - Dxy (GEN) [cm]");
  h_dxyDiffOld_50d0->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffOld_50d0->GetYaxis()->SetRangeUser(1,h_dxyDiffNew_50d0->GetMaximum()*1.2);

  h_dxyDiffRelOld_50d0->GetXaxis()->SetTitle("(Dxy (NN) - Dxy (GEN))/(Dxy (GEN))");
  h_dxyDiffRelOld_50d0->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffRelOld_50d0->GetYaxis()->SetRangeUser(1,h_dxyDiffRelNew_50d0->GetMaximum()*1.2);

  // z0 < 25
  h_ptDiffOld_z025->GetXaxis()->SetTitle("p_{T} (NN) - p_{T} (GEN) [GeV]");
  h_ptDiffOld_z025->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffOld_z025->GetYaxis()->SetRangeUser(1,h_ptDiffNew_z025->GetMaximum()*1.2);

  h_InvPtDiffOld_z025->GetXaxis()->SetTitle("100*q/p_{T} (NN) - 100*q/p_{T} (GEN) [GeV]^{-1}");
  h_InvPtDiffOld_z025->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffOld_z025->GetYaxis()->SetRangeUser(1,h_InvPtDiffNew_z025->GetMaximum()*1.2);

  h_InvPtDiffRelOld_z025->GetXaxis()->SetTitle("(1/p_{T} (NN) - 1/p_{T} (GEN))/(1/p_{T} (GEN))");
  h_InvPtDiffRelOld_z025->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffRelOld_z025->GetYaxis()->SetRangeUser(1,h_InvPtDiffRelNew_z025->GetMaximum()*1.2);

  h_ptDiffRelOld_z025->GetXaxis()->SetTitle("(p_{T} (NN) - p_{T} (GEN))/(p_{T} (GEN))");
  h_ptDiffRelOld_z025->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffRelOld_z025->GetYaxis()->SetRangeUser(1,h_ptDiffRelNew_z025->GetMaximum()*1.2);

  h_dxyDiffOld_z025->GetXaxis()->SetTitle("Dxy (NN) - Dxy (GEN) [cm]");
  h_dxyDiffOld_z025->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffOld_z025->GetYaxis()->SetRangeUser(1,h_dxyDiffNew_z025->GetMaximum()*1.2);

  h_dxyDiffRelOld_z025->GetXaxis()->SetTitle("(Dxy (NN) - Dxy (GEN))/(Dxy (GEN))");
  h_dxyDiffRelOld_z025->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffRelOld_z025->GetYaxis()->SetRangeUser(1,h_dxyDiffRelNew_z025->GetMaximum()*1.2);

  // 25 < z0 < 50
  h_ptDiffOld_25z050->GetXaxis()->SetTitle("p_{T} (NN) - p_{T} (GEN) [GeV]");
  h_ptDiffOld_25z050->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffOld_25z050->GetYaxis()->SetRangeUser(1,h_ptDiffNew_25z050->GetMaximum()*1.2);

  h_InvPtDiffOld_25z050->GetXaxis()->SetTitle("100*q/p_{T} (NN) - 100*q/p_{T} (GEN) [GeV]^{-1}");
  h_InvPtDiffOld_25z050->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffOld_25z050->GetYaxis()->SetRangeUser(1,h_InvPtDiffNew_25z050->GetMaximum()*1.2);

  h_InvPtDiffRelOld_25z050->GetXaxis()->SetTitle("(1/p_{T} (NN) - 1/p_{T} (GEN))/(1/p_{T} (GEN))");
  h_InvPtDiffRelOld_25z050->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffRelOld_25z050->GetYaxis()->SetRangeUser(1,h_InvPtDiffRelNew_25z050->GetMaximum()*1.2);

  h_ptDiffRelOld_25z050->GetXaxis()->SetTitle("(p_{T} (NN) - p_{T} (GEN))/(p_{T} (GEN))");
  h_ptDiffRelOld_25z050->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffRelOld_25z050->GetYaxis()->SetRangeUser(1,h_ptDiffRelNew_25z050->GetMaximum()*1.2);

  h_dxyDiffOld_25z050->GetXaxis()->SetTitle("Dxy (NN) - Dxy (GEN) [cm]");
  h_dxyDiffOld_25z050->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffOld_25z050->GetYaxis()->SetRangeUser(1,h_dxyDiffNew_25z050->GetMaximum()*1.2);

  h_dxyDiffRelOld_25z050->GetXaxis()->SetTitle("(Dxy (NN) - Dxy (GEN))/(Dxy (GEN))");
  h_dxyDiffRelOld_25z050->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffRelOld_25z050->GetYaxis()->SetRangeUser(1,h_dxyDiffRelNew_25z050->GetMaximum()*1.2);

  // 50 < z0
  h_ptDiffOld_50z0->GetXaxis()->SetTitle("p_{T} (NN) - p_{T} (GEN) [GeV]");
  h_ptDiffOld_50z0->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffOld_50z0->GetYaxis()->SetRangeUser(1,h_ptDiffNew_50z0->GetMaximum()*1.2);

  h_InvPtDiffOld_50z0->GetXaxis()->SetTitle("100*q/p_{T} (NN) - 100*q/p_{T} (GEN) [GeV]^{-1}");
  h_InvPtDiffOld_50z0->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffOld_50z0->GetYaxis()->SetRangeUser(1,h_InvPtDiffNew_50z0->GetMaximum()*1.2);

  h_InvPtDiffRelOld_50z0->GetXaxis()->SetTitle("(1/p_{T} (NN) - 1/p_{T} (GEN))/(1/p_{T} (GEN))");
  h_InvPtDiffRelOld_50z0->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffRelOld_50z0->GetYaxis()->SetRangeUser(1,h_InvPtDiffRelNew_50z0->GetMaximum()*1.2);

  h_ptDiffRelOld_50z0->GetXaxis()->SetTitle("(p_{T} (NN) - p_{T} (GEN))/(p_{T} (GEN))");
  h_ptDiffRelOld_50z0->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffRelOld_50z0->GetYaxis()->SetRangeUser(1,h_ptDiffRelNew_50z0->GetMaximum()*1.2);

  h_dxyDiffOld_50z0->GetXaxis()->SetTitle("Dxy (NN) - Dxy (GEN) [cm]");
  h_dxyDiffOld_50z0->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffOld_50z0->GetYaxis()->SetRangeUser(1,h_dxyDiffNew_50z0->GetMaximum()*1.2);

  h_dxyDiffRelOld_50z0->GetXaxis()->SetTitle("(Dxy (NN) - Dxy (GEN))/(Dxy (GEN))");
  h_dxyDiffRelOld_50z0->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffRelOld_50z0->GetYaxis()->SetRangeUser(1,h_dxyDiffRelNew_50z0->GetMaximum()*1.2);

  // 1.2 < eta < 1.6
  h_ptDiffOld_eta1->GetXaxis()->SetTitle("p_{T} (NN) - p_{T} (GEN) [GeV]");
  h_ptDiffOld_eta1->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffOld_eta1->GetYaxis()->SetRangeUser(1,h_ptDiffNew_eta1->GetMaximum()*1.2);

  h_InvPtDiffOld_eta1->GetXaxis()->SetTitle("100*q/p_{T} (NN) - 100*q/p_{T} (GEN) [GeV]^{-1}");
  h_InvPtDiffOld_eta1->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffOld_eta1->GetYaxis()->SetRangeUser(1,h_InvPtDiffNew_eta1->GetMaximum()*1.2);

  h_InvPtDiffRelOld_eta1->GetXaxis()->SetTitle("(1/p_{T} (NN) - 1/p_{T} (GEN))/(1/p_{T} (GEN))");
  h_InvPtDiffRelOld_eta1->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffRelOld_eta1->GetYaxis()->SetRangeUser(1,h_InvPtDiffRelNew_eta1->GetMaximum()*1.2);

  h_ptDiffRelOld_eta1->GetXaxis()->SetTitle("(p_{T} (NN) - p_{T} (GEN))/(p_{T} (GEN))");
  h_ptDiffRelOld_eta1->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffRelOld_eta1->GetYaxis()->SetRangeUser(1,h_ptDiffRelNew_eta1->GetMaximum()*1.2);

  h_dxyDiffOld_eta1->GetXaxis()->SetTitle("Dxy (NN) - Dxy (GEN) [cm]");
  h_dxyDiffOld_eta1->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffOld_eta1->GetYaxis()->SetRangeUser(1,h_dxyDiffNew_eta1->GetMaximum()*1.2);

  h_dxyDiffRelOld_eta1->GetXaxis()->SetTitle("(Dxy (NN) - Dxy (GEN))/(Dxy (GEN))");
  h_dxyDiffRelOld_eta1->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffRelOld_eta1->GetYaxis()->SetRangeUser(1,h_dxyDiffRelNew_eta1->GetMaximum()*1.2);

  // 1.6 < eta < 2.1
  h_ptDiffOld_eta2->GetXaxis()->SetTitle("p_{T} (NN) - p_{T} (GEN) [GeV]");
  h_ptDiffOld_eta2->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffOld_eta2->GetYaxis()->SetRangeUser(1,h_ptDiffNew_eta2->GetMaximum()*1.2);

  h_InvPtDiffOld_eta2->GetXaxis()->SetTitle("100*q/p_{T} (NN) - 100*q/p_{T} (GEN) [GeV]^{-1}");
  h_InvPtDiffOld_eta2->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffOld_eta2->GetYaxis()->SetRangeUser(1,h_InvPtDiffNew_eta2->GetMaximum()*1.2);

  h_InvPtDiffRelOld_eta2->GetXaxis()->SetTitle("(1/p_{T} (NN) - 1/p_{T} (GEN))/(1/p_{T} (GEN))");
  h_InvPtDiffRelOld_eta2->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffRelOld_eta2->GetYaxis()->SetRangeUser(1,h_InvPtDiffRelNew_eta2->GetMaximum()*1.2);

  h_ptDiffRelOld_eta2->GetXaxis()->SetTitle("(p_{T} (NN) - p_{T} (GEN))/(p_{T} (GEN))");
  h_ptDiffRelOld_eta2->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffRelOld_eta2->GetYaxis()->SetRangeUser(1,h_ptDiffRelNew_eta2->GetMaximum()*1.2);

  h_dxyDiffOld_eta2->GetXaxis()->SetTitle("Dxy (NN) - Dxy (GEN) [cm]");
  h_dxyDiffOld_eta2->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffOld_eta2->GetYaxis()->SetRangeUser(1,h_dxyDiffNew_eta2->GetMaximum()*1.2);

  h_dxyDiffRelOld_eta2->GetXaxis()->SetTitle("(Dxy (NN) - Dxy (GEN))/(Dxy (GEN))");
  h_dxyDiffRelOld_eta2->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffRelOld_eta2->GetYaxis()->SetRangeUser(1,h_dxyDiffRelNew_eta2->GetMaximum()*1.2);

  // 2.1 < eta < 2.5
  h_ptDiffOld_eta3->GetXaxis()->SetTitle("p_{T} (NN) - p_{T} (GEN) [GeV]");
  h_ptDiffOld_eta3->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffOld_eta3->GetYaxis()->SetRangeUser(1,h_ptDiffNew_eta3->GetMaximum()*1.2);

  h_InvPtDiffOld_eta3->GetXaxis()->SetTitle("100*q/p_{T} (NN) - 100*q/p_{T} (GEN) [GeV]^{-1}");
  h_InvPtDiffOld_eta3->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffOld_eta3->GetYaxis()->SetRangeUser(1,h_InvPtDiffNew_eta3->GetMaximum()*1.2);

  h_InvPtDiffRelOld_eta3->GetXaxis()->SetTitle("(1/p_{T} (NN) - 1/p_{T} (GEN))/(1/p_{T} (GEN))");
  h_InvPtDiffRelOld_eta3->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiffRelOld_eta3->GetYaxis()->SetRangeUser(1,h_InvPtDiffRelNew_eta3->GetMaximum()*1.2);

  h_ptDiffRelOld_eta3->GetXaxis()->SetTitle("(p_{T} (NN) - p_{T} (GEN))/(p_{T} (GEN))");
  h_ptDiffRelOld_eta3->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiffRelOld_eta3->GetYaxis()->SetRangeUser(1,h_ptDiffRelNew_eta3->GetMaximum()*1.2);

  h_dxyDiffOld_eta3->GetXaxis()->SetTitle("Dxy (NN) - Dxy (GEN) [cm]");
  h_dxyDiffOld_eta3->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffOld_eta3->GetYaxis()->SetRangeUser(1,h_dxyDiffNew_eta3->GetMaximum()*1.2);

  h_dxyDiffRelOld_eta3->GetXaxis()->SetTitle("(Dxy (NN) - Dxy (GEN))/(Dxy (GEN))");
  h_dxyDiffRelOld_eta3->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiffRelOld_eta3->GetYaxis()->SetRangeUser(1,h_dxyDiffRelNew_eta3->GetMaximum()*1.2);


  h_ptDiffOld->SetLineWidth(3);
  h_ptDiffOld->SetLineColor(1);

  h_InvPtDiffOld->SetLineWidth(3);
  h_InvPtDiffOld->SetLineColor(1);

  h_InvPtDiffRelOld->SetLineWidth(3);
  h_InvPtDiffRelOld->SetLineColor(1);

  h_ptDiffRelOld->SetLineWidth(3);
  h_ptDiffRelOld->SetLineColor(1);

  h_dxyDiffOld->SetLineWidth(3);
  h_dxyDiffOld->SetLineColor(1);

  h_dxyDiffRelOld->SetLineWidth(3);
  h_dxyDiffRelOld->SetLineColor(1);

  h_ptDiffNew->SetLineWidth(3);
  h_ptDiffNew->SetLineColor(2);

  h_InvPtDiffNew->SetLineWidth(3);
  h_InvPtDiffNew->SetLineColor(2);

  h_InvPtDiffRelNew->SetLineWidth(3);
  h_InvPtDiffRelNew->SetLineColor(2);

  h_ptDiffRelNew->SetLineWidth(3);
  h_ptDiffRelNew->SetLineColor(2);

  h_dxyDiffNew->SetLineWidth(3);
  h_dxyDiffNew->SetLineColor(2);

  h_dxyDiffRelNew->SetLineWidth(3);
  h_dxyDiffRelNew->SetLineColor(2);


  // pt < 5
  h_ptDiffOld_pt5->SetLineWidth(3);
  h_ptDiffOld_pt5->SetLineColor(1);

  h_InvPtDiffOld_pt5->SetLineWidth(3);
  h_InvPtDiffOld_pt5->SetLineColor(1);

  h_InvPtDiffRelOld_pt5->SetLineWidth(3);
  h_InvPtDiffRelOld_pt5->SetLineColor(1);

  h_ptDiffRelOld_pt5->SetLineWidth(3);
  h_ptDiffRelOld_pt5->SetLineColor(1);

  h_dxyDiffOld_pt5->SetLineWidth(3);
  h_dxyDiffOld_pt5->SetLineColor(1);

  h_dxyDiffRelOld_pt5->SetLineWidth(3);
  h_dxyDiffRelOld_pt5->SetLineColor(1);

  h_ptDiffNew_pt5->SetLineWidth(3);
  h_ptDiffNew_pt5->SetLineColor(2);

  h_InvPtDiffNew_pt5->SetLineWidth(3);
  h_InvPtDiffNew_pt5->SetLineColor(2);

  h_InvPtDiffRelNew_pt5->SetLineWidth(3);
  h_InvPtDiffRelNew_pt5->SetLineColor(2);

  h_ptDiffRelNew_pt5->SetLineWidth(3);
  h_ptDiffRelNew_pt5->SetLineColor(2);

  h_dxyDiffNew_pt5->SetLineWidth(3);
  h_dxyDiffNew_pt5->SetLineColor(2);

  h_dxyDiffRelNew_pt5->SetLineWidth(3);
  h_dxyDiffRelNew_pt5->SetLineColor(2);

  // 5 < pt < 10
  h_ptDiffOld_5pt10->SetLineWidth(3);
  h_ptDiffOld_5pt10->SetLineColor(1);

  h_InvPtDiffOld_5pt10->SetLineWidth(3);
  h_InvPtDiffOld_5pt10->SetLineColor(1);

  h_InvPtDiffRelOld_5pt10->SetLineWidth(3);
  h_InvPtDiffRelOld_5pt10->SetLineColor(1);

  h_ptDiffRelOld_5pt10->SetLineWidth(3);
  h_ptDiffRelOld_5pt10->SetLineColor(1);

  h_dxyDiffOld_5pt10->SetLineWidth(3);
  h_dxyDiffOld_5pt10->SetLineColor(1);

  h_dxyDiffRelOld_5pt10->SetLineWidth(3);
  h_dxyDiffRelOld_5pt10->SetLineColor(1);

  h_ptDiffNew_5pt10->SetLineWidth(3);
  h_ptDiffNew_5pt10->SetLineColor(2);

  h_InvPtDiffNew_5pt10->SetLineWidth(3);
  h_InvPtDiffNew_5pt10->SetLineColor(2);

  h_InvPtDiffRelNew_5pt10->SetLineWidth(3);
  h_InvPtDiffRelNew_5pt10->SetLineColor(2);

  h_ptDiffRelNew_5pt10->SetLineWidth(3);
  h_ptDiffRelNew_5pt10->SetLineColor(2);

  h_dxyDiffNew_5pt10->SetLineWidth(3);
  h_dxyDiffNew_5pt10->SetLineColor(2);

  h_dxyDiffRelNew_5pt10->SetLineWidth(3);
  h_dxyDiffRelNew_5pt10->SetLineColor(2);

  // 10 < pt < 20
  h_ptDiffOld_10pt20->SetLineWidth(3);
  h_ptDiffOld_10pt20->SetLineColor(1);

  h_InvPtDiffOld_10pt20->SetLineWidth(3);
  h_InvPtDiffOld_10pt20->SetLineColor(1);

  h_InvPtDiffRelOld_10pt20->SetLineWidth(3);
  h_InvPtDiffRelOld_10pt20->SetLineColor(1);

  h_ptDiffRelOld_10pt20->SetLineWidth(3);
  h_ptDiffRelOld_10pt20->SetLineColor(1);

  h_dxyDiffOld_10pt20->SetLineWidth(3);
  h_dxyDiffOld_10pt20->SetLineColor(1);

  h_dxyDiffRelOld_10pt20->SetLineWidth(3);
  h_dxyDiffRelOld_10pt20->SetLineColor(1);

  h_ptDiffNew_10pt20->SetLineWidth(3);
  h_ptDiffNew_10pt20->SetLineColor(2);

  h_InvPtDiffNew_10pt20->SetLineWidth(3);
  h_InvPtDiffNew_10pt20->SetLineColor(2);

  h_InvPtDiffRelNew_10pt20->SetLineWidth(3);
  h_InvPtDiffRelNew_10pt20->SetLineColor(2);

  h_ptDiffRelNew_10pt20->SetLineWidth(3);
  h_ptDiffRelNew_10pt20->SetLineColor(2);

  h_dxyDiffNew_10pt20->SetLineWidth(3);
  h_dxyDiffNew_10pt20->SetLineColor(2);

  h_dxyDiffRelNew_10pt20->SetLineWidth(3);
  h_dxyDiffRelNew_10pt20->SetLineColor(2);

  // 20 < pt
  h_ptDiffOld_20pt->SetLineWidth(3);
  h_ptDiffOld_20pt->SetLineColor(1);

  h_InvPtDiffOld_20pt->SetLineWidth(3);
  h_InvPtDiffOld_20pt->SetLineColor(1);

  h_InvPtDiffRelOld_20pt->SetLineWidth(3);
  h_InvPtDiffRelOld_20pt->SetLineColor(1);

  h_ptDiffRelOld_20pt->SetLineWidth(3);
  h_ptDiffRelOld_20pt->SetLineColor(1);

  h_dxyDiffOld_20pt->SetLineWidth(3);
  h_dxyDiffOld_20pt->SetLineColor(1);

  h_dxyDiffRelOld_20pt->SetLineWidth(3);
  h_dxyDiffRelOld_20pt->SetLineColor(1);

  h_ptDiffNew_20pt->SetLineWidth(3);
  h_ptDiffNew_20pt->SetLineColor(2);

  h_InvPtDiffNew_20pt->SetLineWidth(3);
  h_InvPtDiffNew_20pt->SetLineColor(2);

  h_InvPtDiffRelNew_20pt->SetLineWidth(3);
  h_InvPtDiffRelNew_20pt->SetLineColor(2);

  h_ptDiffRelNew_20pt->SetLineWidth(3);
  h_ptDiffRelNew_20pt->SetLineColor(2);

  h_dxyDiffNew_20pt->SetLineWidth(3);
  h_dxyDiffNew_20pt->SetLineColor(2);

  h_dxyDiffRelNew_20pt->SetLineWidth(3);
  h_dxyDiffRelNew_20pt->SetLineColor(2);

  // d0 < 25
  h_ptDiffOld_d025->SetLineWidth(3);
  h_ptDiffOld_d025->SetLineColor(1);

  h_InvPtDiffOld_d025->SetLineWidth(3);
  h_InvPtDiffOld_d025->SetLineColor(1);

  h_InvPtDiffRelOld_d025->SetLineWidth(3);
  h_InvPtDiffRelOld_d025->SetLineColor(1);

  h_ptDiffRelOld_d025->SetLineWidth(3);
  h_ptDiffRelOld_d025->SetLineColor(1);

  h_dxyDiffOld_d025->SetLineWidth(3);
  h_dxyDiffOld_d025->SetLineColor(1);

  h_dxyDiffRelOld_d025->SetLineWidth(3);
  h_dxyDiffRelOld_d025->SetLineColor(1);

  h_ptDiffNew_d025->SetLineWidth(3);
  h_ptDiffNew_d025->SetLineColor(2);

  h_InvPtDiffNew_d025->SetLineWidth(3);
  h_InvPtDiffNew_d025->SetLineColor(2);

  h_InvPtDiffRelNew_d025->SetLineWidth(3);
  h_InvPtDiffRelNew_d025->SetLineColor(2);

  h_ptDiffRelNew_d025->SetLineWidth(3);
  h_ptDiffRelNew_d025->SetLineColor(2);

  h_dxyDiffNew_d025->SetLineWidth(3);
  h_dxyDiffNew_d025->SetLineColor(2);

  h_dxyDiffRelNew_d025->SetLineWidth(3);
  h_dxyDiffRelNew_d025->SetLineColor(2);

  // 25 < d0 < 50
  h_ptDiffOld_25d050->SetLineWidth(3);
  h_ptDiffOld_25d050->SetLineColor(1);

  h_InvPtDiffOld_25d050->SetLineWidth(3);
  h_InvPtDiffOld_25d050->SetLineColor(1);

  h_InvPtDiffRelOld_25d050->SetLineWidth(3);
  h_InvPtDiffRelOld_25d050->SetLineColor(1);

  h_ptDiffRelOld_25d050->SetLineWidth(3);
  h_ptDiffRelOld_25d050->SetLineColor(1);

  h_dxyDiffOld_25d050->SetLineWidth(3);
  h_dxyDiffOld_25d050->SetLineColor(1);

  h_dxyDiffRelOld_25d050->SetLineWidth(3);
  h_dxyDiffRelOld_25d050->SetLineColor(1);

  h_ptDiffNew_25d050->SetLineWidth(3);
  h_ptDiffNew_25d050->SetLineColor(2);

  h_InvPtDiffNew_25d050->SetLineWidth(3);
  h_InvPtDiffNew_25d050->SetLineColor(2);

  h_InvPtDiffRelNew_25d050->SetLineWidth(3);
  h_InvPtDiffRelNew_25d050->SetLineColor(2);

  h_ptDiffRelNew_25d050->SetLineWidth(3);
  h_ptDiffRelNew_25d050->SetLineColor(2);

  h_dxyDiffNew_25d050->SetLineWidth(3);
  h_dxyDiffNew_25d050->SetLineColor(2);

  h_dxyDiffRelNew_25d050->SetLineWidth(3);
  h_dxyDiffRelNew_25d050->SetLineColor(2);

  // 50 < d0
  h_ptDiffOld_50d0->SetLineWidth(3);
  h_ptDiffOld_50d0->SetLineColor(1);

  h_InvPtDiffOld_50d0->SetLineWidth(3);
  h_InvPtDiffOld_50d0->SetLineColor(1);

  h_InvPtDiffRelOld_50d0->SetLineWidth(3);
  h_InvPtDiffRelOld_50d0->SetLineColor(1);

  h_ptDiffRelOld_50d0->SetLineWidth(3);
  h_ptDiffRelOld_50d0->SetLineColor(1);

  h_dxyDiffOld_50d0->SetLineWidth(3);
  h_dxyDiffOld_50d0->SetLineColor(1);

  h_dxyDiffRelOld_50d0->SetLineWidth(3);
  h_dxyDiffRelOld_50d0->SetLineColor(1);

  h_ptDiffNew_50d0->SetLineWidth(3);
  h_ptDiffNew_50d0->SetLineColor(2);

  h_InvPtDiffNew_50d0->SetLineWidth(3);
  h_InvPtDiffNew_50d0->SetLineColor(2);

  h_InvPtDiffRelNew_50d0->SetLineWidth(3);
  h_InvPtDiffRelNew_50d0->SetLineColor(2);

  h_ptDiffRelNew_50d0->SetLineWidth(3);
  h_ptDiffRelNew_50d0->SetLineColor(2);

  h_dxyDiffNew_50d0->SetLineWidth(3);
  h_dxyDiffNew_50d0->SetLineColor(2);

  h_dxyDiffRelNew_50d0->SetLineWidth(3);
  h_dxyDiffRelNew_50d0->SetLineColor(2);

  // z0 < 25
  h_ptDiffOld_z025->SetLineWidth(3);
  h_ptDiffOld_z025->SetLineColor(1);

  h_InvPtDiffOld_z025->SetLineWidth(3);
  h_InvPtDiffOld_z025->SetLineColor(1);

  h_InvPtDiffRelOld_z025->SetLineWidth(3);
  h_InvPtDiffRelOld_z025->SetLineColor(1);

  h_ptDiffRelOld_z025->SetLineWidth(3);
  h_ptDiffRelOld_z025->SetLineColor(1);

  h_dxyDiffOld_z025->SetLineWidth(3);
  h_dxyDiffOld_z025->SetLineColor(1);

  h_dxyDiffRelOld_z025->SetLineWidth(3);
  h_dxyDiffRelOld_z025->SetLineColor(1);

  h_ptDiffNew_z025->SetLineWidth(3);
  h_ptDiffNew_z025->SetLineColor(2);

  h_InvPtDiffNew_z025->SetLineWidth(3);
  h_InvPtDiffNew_z025->SetLineColor(2);

  h_InvPtDiffRelNew_z025->SetLineWidth(3);
  h_InvPtDiffRelNew_z025->SetLineColor(2);

  h_ptDiffRelNew_z025->SetLineWidth(3);
  h_ptDiffRelNew_z025->SetLineColor(2);

  h_dxyDiffNew_z025->SetLineWidth(3);
  h_dxyDiffNew_z025->SetLineColor(2);

  h_dxyDiffRelNew_z025->SetLineWidth(3);
  h_dxyDiffRelNew_z025->SetLineColor(2);

  // 25 < z0 < 50
  h_ptDiffOld_25z050->SetLineWidth(3);
  h_ptDiffOld_25z050->SetLineColor(1);

  h_InvPtDiffOld_25z050->SetLineWidth(3);
  h_InvPtDiffOld_25z050->SetLineColor(1);

  h_InvPtDiffRelOld_25z050->SetLineWidth(3);
  h_InvPtDiffRelOld_25z050->SetLineColor(1);

  h_ptDiffRelOld_25z050->SetLineWidth(3);
  h_ptDiffRelOld_25z050->SetLineColor(1);

  h_dxyDiffOld_25z050->SetLineWidth(3);
  h_dxyDiffOld_25z050->SetLineColor(1);

  h_dxyDiffRelOld_25z050->SetLineWidth(3);
  h_dxyDiffRelOld_25z050->SetLineColor(1);

  h_ptDiffNew_25z050->SetLineWidth(3);
  h_ptDiffNew_25z050->SetLineColor(2);

  h_InvPtDiffNew_25z050->SetLineWidth(3);
  h_InvPtDiffNew_25z050->SetLineColor(2);

  h_InvPtDiffRelNew_25z050->SetLineWidth(3);
  h_InvPtDiffRelNew_25z050->SetLineColor(2);

  h_ptDiffRelNew_25z050->SetLineWidth(3);
  h_ptDiffRelNew_25z050->SetLineColor(2);

  h_dxyDiffNew_25z050->SetLineWidth(3);
  h_dxyDiffNew_25z050->SetLineColor(2);

  h_dxyDiffRelNew_25z050->SetLineWidth(3);
  h_dxyDiffRelNew_25z050->SetLineColor(2);

  // 50 < z0
  h_ptDiffOld_50z0->SetLineWidth(3);
  h_ptDiffOld_50z0->SetLineColor(1);

  h_InvPtDiffOld_50z0->SetLineWidth(3);
  h_InvPtDiffOld_50z0->SetLineColor(1);

  h_InvPtDiffRelOld_50z0->SetLineWidth(3);
  h_InvPtDiffRelOld_50z0->SetLineColor(1);

  h_ptDiffRelOld_50z0->SetLineWidth(3);
  h_ptDiffRelOld_50z0->SetLineColor(1);

  h_dxyDiffOld_50z0->SetLineWidth(3);
  h_dxyDiffOld_50z0->SetLineColor(1);

  h_dxyDiffRelOld_50z0->SetLineWidth(3);
  h_dxyDiffRelOld_50z0->SetLineColor(1);

  h_ptDiffNew_50z0->SetLineWidth(3);
  h_ptDiffNew_50z0->SetLineColor(2);

  h_InvPtDiffNew_50z0->SetLineWidth(3);
  h_InvPtDiffNew_50z0->SetLineColor(2);

  h_InvPtDiffRelNew_50z0->SetLineWidth(3);
  h_InvPtDiffRelNew_50z0->SetLineColor(2);

  h_ptDiffRelNew_50z0->SetLineWidth(3);
  h_ptDiffRelNew_50z0->SetLineColor(2);

  h_dxyDiffNew_50z0->SetLineWidth(3);
  h_dxyDiffNew_50z0->SetLineColor(2);

  h_dxyDiffRelNew_50z0->SetLineWidth(3);
  h_dxyDiffRelNew_50z0->SetLineColor(2);

  // 1.2 < eta < 1.6
  h_ptDiffOld_eta1->SetLineWidth(3);
  h_ptDiffOld_eta1->SetLineColor(1);

  h_InvPtDiffOld_eta1->SetLineWidth(3);
  h_InvPtDiffOld_eta1->SetLineColor(1);

  h_InvPtDiffRelOld_eta1->SetLineWidth(3);
  h_InvPtDiffRelOld_eta1->SetLineColor(1);

  h_ptDiffRelOld_eta1->SetLineWidth(3);
  h_ptDiffRelOld_eta1->SetLineColor(1);

  h_dxyDiffOld_eta1->SetLineWidth(3);
  h_dxyDiffOld_eta1->SetLineColor(1);

  h_dxyDiffRelOld_eta1->SetLineWidth(3);
  h_dxyDiffRelOld_eta1->SetLineColor(1);

  h_ptDiffNew_eta1->SetLineWidth(3);
  h_ptDiffNew_eta1->SetLineColor(2);

  h_InvPtDiffNew_eta1->SetLineWidth(3);
  h_InvPtDiffNew_eta1->SetLineColor(2);

  h_InvPtDiffRelNew_eta1->SetLineWidth(3);
  h_InvPtDiffRelNew_eta1->SetLineColor(2);

  h_ptDiffRelNew_eta1->SetLineWidth(3);
  h_ptDiffRelNew_eta1->SetLineColor(2);

  h_dxyDiffNew_eta1->SetLineWidth(3);
  h_dxyDiffNew_eta1->SetLineColor(2);

  h_dxyDiffRelNew_eta1->SetLineWidth(3);
  h_dxyDiffRelNew_eta1->SetLineColor(2);

  // 1.6 < eta < 2.1
  h_ptDiffOld_eta2->SetLineWidth(3);
  h_ptDiffOld_eta2->SetLineColor(1);

  h_InvPtDiffOld_eta2->SetLineWidth(3);
  h_InvPtDiffOld_eta2->SetLineColor(1);

  h_InvPtDiffRelOld_eta2->SetLineWidth(3);
  h_InvPtDiffRelOld_eta2->SetLineColor(1);

  h_ptDiffRelOld_eta2->SetLineWidth(3);
  h_ptDiffRelOld_eta2->SetLineColor(1);

  h_dxyDiffOld_eta2->SetLineWidth(3);
  h_dxyDiffOld_eta2->SetLineColor(1);

  h_dxyDiffRelOld_eta2->SetLineWidth(3);
  h_dxyDiffRelOld_eta2->SetLineColor(1);

  h_ptDiffNew_eta2->SetLineWidth(3);
  h_ptDiffNew_eta2->SetLineColor(2);

  h_InvPtDiffNew_eta2->SetLineWidth(3);
  h_InvPtDiffNew_eta2->SetLineColor(2);

  h_InvPtDiffRelNew_eta2->SetLineWidth(3);
  h_InvPtDiffRelNew_eta2->SetLineColor(2);

  h_ptDiffRelNew_eta2->SetLineWidth(3);
  h_ptDiffRelNew_eta2->SetLineColor(2);

  h_dxyDiffNew_eta2->SetLineWidth(3);
  h_dxyDiffNew_eta2->SetLineColor(2);

  h_dxyDiffRelNew_eta2->SetLineWidth(3);
  h_dxyDiffRelNew_eta2->SetLineColor(2);

  // 2.1 < eta < 2.5
  h_ptDiffOld_eta3->SetLineWidth(3);
  h_ptDiffOld_eta3->SetLineColor(1);

  h_InvPtDiffOld_eta3->SetLineWidth(3);
  h_InvPtDiffOld_eta3->SetLineColor(1);

  h_InvPtDiffRelOld_eta3->SetLineWidth(3);
  h_InvPtDiffRelOld_eta3->SetLineColor(1);

  h_ptDiffRelOld_eta3->SetLineWidth(3);
  h_ptDiffRelOld_eta3->SetLineColor(1);

  h_dxyDiffOld_eta3->SetLineWidth(3);
  h_dxyDiffOld_eta3->SetLineColor(1);

  h_dxyDiffRelOld_eta3->SetLineWidth(3);
  h_dxyDiffRelOld_eta3->SetLineColor(1);

  h_ptDiffNew_eta3->SetLineWidth(3);
  h_ptDiffNew_eta3->SetLineColor(2);

  h_InvPtDiffNew_eta3->SetLineWidth(3);
  h_InvPtDiffNew_eta3->SetLineColor(2);

  h_InvPtDiffRelNew_eta3->SetLineWidth(3);
  h_InvPtDiffRelNew_eta3->SetLineColor(2);

  h_ptDiffRelNew_eta3->SetLineWidth(3);
  h_ptDiffRelNew_eta3->SetLineColor(2);

  h_dxyDiffNew_eta3->SetLineWidth(3);
  h_dxyDiffNew_eta3->SetLineColor(2);

  h_dxyDiffRelNew_eta3->SetLineWidth(3);
  h_dxyDiffRelNew_eta3->SetLineColor(2);


 
  TString leg = "NNv6";
  TString leg2 = "NNv8";

  //create canvas and save histos
  TCanvas * c1=new TCanvas("c1","c1",1200,1200);

  TPad* pad = new TPad("","",0.1,0.1,1,1);  

  pad->SetLeftMargin(0.2);

  // gPad->SetLogy();
  
  h_ptDiffOld->Draw("");
  h_ptDiffNew->Draw("sames");
  
  TLegend * leg11 =new TLegend(0.75,0.75,0.88,0.88);    
  leg11->AddEntry(h_ptDiffOld,leg);
  leg11->AddEntry(h_ptDiffNew,leg2);
  leg11->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiff_NNv6VsNNv8.pdf");

  h_InvPtDiffOld->Draw("");
  h_InvPtDiffNew->Draw("sames");
 
  TLegend * leg22 =new TLegend(0.75,0.75,0.88,0.88);    
  leg22->AddEntry(h_InvPtDiffOld,leg);
  leg22->AddEntry(h_InvPtDiffNew,leg2);
  leg22->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiff_NNv6VsNNv8.pdf");

  h_InvPtDiffRelOld->Draw("");
  h_InvPtDiffRelNew->Draw("sames");
 
  TLegend * leg222 =new TLegend(0.75,0.75,0.88,0.88);    
  leg222->AddEntry(h_InvPtDiffRelOld,leg);
  leg222->AddEntry(h_InvPtDiffRelNew,leg2);
  leg222->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiffRel_NNv6VsNNv8.pdf");

  h_ptDiffRelOld->Draw("");
  h_ptDiffRelNew->Draw("sames");
  
  TLegend * leg111 =new TLegend(0.75,0.75,0.88,0.88);    
  leg111->AddEntry(h_ptDiffRelOld,leg);
  leg111->AddEntry(h_ptDiffRelNew,leg2);
  leg111->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiffRel_NNv6VsNNv8.pdf");

  h_dxyDiffOld->Draw("");
  h_dxyDiffNew->Draw("sames");
 
  TLegend * leg33 =new TLegend(0.75,0.75,0.88,0.88);    
  leg33->AddEntry(h_dxyDiffOld,leg);
  leg33->AddEntry(h_dxyDiffNew,leg2);
  leg33->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiff_NNv6VsNNv8.pdf");

  h_dxyDiffRelOld->Draw("");
  h_dxyDiffRelNew->Draw("sames");
 
  TLegend * leg333 =new TLegend(0.75,0.75,0.88,0.88);    
  leg333->AddEntry(h_dxyDiffRelOld,leg);
  leg333->AddEntry(h_dxyDiffRelNew,leg2);
  leg333->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiffRel_NNv6VsNNv8.pdf");

  // pt < 5
  h_ptDiffOld_pt5->Draw("");
  h_ptDiffNew_pt5->Draw("sames");
  
  TLegend * leg11_pt5 =new TLegend(0.75,0.75,0.88,0.88);    
  leg11_pt5->AddEntry(h_ptDiffOld_pt5,leg);
  leg11_pt5->AddEntry(h_ptDiffNew_pt5,leg2);
  leg11_pt5->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiff_NNv6VsNNv8_pt5.pdf");

  h_InvPtDiffOld_pt5->Draw("");
  h_InvPtDiffNew_pt5->Draw("sames");
 
  TLegend * leg22_pt5 =new TLegend(0.75,0.75,0.88,0.88);    
  leg22_pt5->AddEntry(h_InvPtDiffOld_pt5,leg);
  leg22_pt5->AddEntry(h_InvPtDiffNew_pt5,leg2);
  leg22_pt5->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiff_NNv6VsNNv8_pt5.pdf");

  h_InvPtDiffRelOld_pt5->Draw("");
  h_InvPtDiffRelNew_pt5->Draw("sames");
 
  TLegend * leg222_pt5 =new TLegend(0.75,0.75,0.88,0.88);    
  leg222_pt5->AddEntry(h_InvPtDiffRelOld_pt5,leg);
  leg222_pt5->AddEntry(h_InvPtDiffRelNew_pt5,leg2);
  leg222_pt5->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiffRel_NNv6VsNNv8_pt5.pdf");

  h_ptDiffRelOld_pt5->Draw("");
  h_ptDiffRelNew_pt5->Draw("sames");
  
  TLegend * leg111_pt5 =new TLegend(0.75,0.75,0.88,0.88);    
  leg111_pt5->AddEntry(h_ptDiffRelOld_pt5,leg);
  leg111_pt5->AddEntry(h_ptDiffRelNew_pt5,leg2);
  leg111_pt5->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiffRel_NNv6VsNNv8_pt5.pdf");

  h_dxyDiffOld_pt5->Draw("");
  h_dxyDiffNew_pt5->Draw("sames");
 
  TLegend * leg33_pt5 =new TLegend(0.75,0.75,0.88,0.88);    
  leg33_pt5->AddEntry(h_dxyDiffOld_pt5,leg);
  leg33_pt5->AddEntry(h_dxyDiffNew_pt5,leg2);
  leg33_pt5->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiff_NNv6VsNNv8_pt5.pdf");

  h_dxyDiffRelOld_pt5->Draw("");
  h_dxyDiffRelNew_pt5->Draw("sames");
 
  TLegend * leg333_pt5 =new TLegend(0.75,0.75,0.88,0.88);    
  leg333_pt5->AddEntry(h_dxyDiffRelOld_pt5,leg);
  leg333_pt5->AddEntry(h_dxyDiffRelNew_pt5,leg2);
  leg333_pt5->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiffRel_NNv6VsNNv8_pt5.pdf");

  // 5 < pt < 10
  h_ptDiffOld_5pt10->Draw("");
  h_ptDiffNew_5pt10->Draw("sames");
  
  TLegend * leg11_5pt10 =new TLegend(0.75,0.75,0.88,0.88);    
  leg11_5pt10->AddEntry(h_ptDiffOld_5pt10,leg);
  leg11_5pt10->AddEntry(h_ptDiffNew_5pt10,leg2);
  leg11_5pt10->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiff_NNv6VsNNv8_5pt10.pdf");

  h_InvPtDiffOld_5pt10->Draw("");
  h_InvPtDiffNew_5pt10->Draw("sames");
 
  TLegend * leg22_5pt10 =new TLegend(0.75,0.75,0.88,0.88);    
  leg22_5pt10->AddEntry(h_InvPtDiffOld_5pt10,leg);
  leg22_5pt10->AddEntry(h_InvPtDiffNew_5pt10,leg2);
  leg22_5pt10->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiff_NNv6VsNNv8_5pt10.pdf");

  h_InvPtDiffRelOld_5pt10->Draw("");
  h_InvPtDiffRelNew_5pt10->Draw("sames");
 
  TLegend * leg222_5pt10 =new TLegend(0.75,0.75,0.88,0.88);    
  leg222_5pt10->AddEntry(h_InvPtDiffRelOld_5pt10,leg);
  leg222_5pt10->AddEntry(h_InvPtDiffRelNew_5pt10,leg2);
  leg222_5pt10->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiffRel_NNv6VsNNv8_5pt10.pdf");

  h_ptDiffRelOld_5pt10->Draw("");
  h_ptDiffRelNew_5pt10->Draw("sames");
  
  TLegend * leg111_5pt10 =new TLegend(0.75,0.75,0.88,0.88);    
  leg111_5pt10->AddEntry(h_ptDiffRelOld_5pt10,leg);
  leg111_5pt10->AddEntry(h_ptDiffRelNew_5pt10,leg2);
  leg111_5pt10->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiffRel_NNv6VsNNv8_5pt10.pdf");

  h_dxyDiffOld_5pt10->Draw("");
  h_dxyDiffNew_5pt10->Draw("sames");
 
  TLegend * leg33_5pt10 =new TLegend(0.75,0.75,0.88,0.88);    
  leg33_5pt10->AddEntry(h_dxyDiffOld_5pt10,leg);
  leg33_5pt10->AddEntry(h_dxyDiffNew_5pt10,leg2);
  leg33_5pt10->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiff_NNv6VsNNv8_5pt10.pdf");

  h_dxyDiffRelOld_5pt10->Draw("");
  h_dxyDiffRelNew_5pt10->Draw("sames");
 
  TLegend * leg333_5pt10 =new TLegend(0.75,0.75,0.88,0.88);    
  leg333_5pt10->AddEntry(h_dxyDiffRelOld_5pt10,leg);
  leg333_5pt10->AddEntry(h_dxyDiffRelNew_5pt10,leg2);
  leg333_5pt10->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiffRel_NNv6VsNNv8_5pt10.pdf");

  // 10 < pt < 20
  h_ptDiffOld_10pt20->Draw("");
  h_ptDiffNew_10pt20->Draw("sames");
  
  TLegend * leg11_10pt20 =new TLegend(0.75,0.75,0.88,0.88);    
  leg11_10pt20->AddEntry(h_ptDiffOld_10pt20,leg);
  leg11_10pt20->AddEntry(h_ptDiffNew_10pt20,leg2);
  leg11_10pt20->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiff_NNv6VsNNv8_10pt20.pdf");

  h_InvPtDiffOld_10pt20->Draw("");
  h_InvPtDiffNew_10pt20->Draw("sames");
 
  TLegend * leg22_10pt20 =new TLegend(0.75,0.75,0.88,0.88);    
  leg22_10pt20->AddEntry(h_InvPtDiffOld_10pt20,leg);
  leg22_10pt20->AddEntry(h_InvPtDiffNew_10pt20,leg2);
  leg22_10pt20->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiff_NNv6VsNNv8_10pt20.pdf");

  h_InvPtDiffRelOld_10pt20->Draw("");
  h_InvPtDiffRelNew_10pt20->Draw("sames");
 
  TLegend * leg222_10pt20 =new TLegend(0.75,0.75,0.88,0.88);    
  leg222_10pt20->AddEntry(h_InvPtDiffRelOld_10pt20,leg);
  leg222_10pt20->AddEntry(h_InvPtDiffRelNew_10pt20,leg2);
  leg222_10pt20->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiffRel_NNv6VsNNv8_10pt20.pdf");

  h_ptDiffRelOld_10pt20->Draw("");
  h_ptDiffRelNew_10pt20->Draw("sames");
  
  TLegend * leg111_10pt20 =new TLegend(0.75,0.75,0.88,0.88);    
  leg111_10pt20->AddEntry(h_ptDiffRelOld_10pt20,leg);
  leg111_10pt20->AddEntry(h_ptDiffRelNew_10pt20,leg2);
  leg111_10pt20->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiffRel_NNv6VsNNv8_10pt20.pdf");

  h_dxyDiffOld_10pt20->Draw("");
  h_dxyDiffNew_10pt20->Draw("sames");
 
  TLegend * leg33_10pt20 =new TLegend(0.75,0.75,0.88,0.88);    
  leg33_10pt20->AddEntry(h_dxyDiffOld_10pt20,leg);
  leg33_10pt20->AddEntry(h_dxyDiffNew_10pt20,leg2);
  leg33_10pt20->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiff_NNv6VsNNv8_10pt20.pdf");

  h_dxyDiffRelOld_10pt20->Draw("");
  h_dxyDiffRelNew_10pt20->Draw("sames");
 
  TLegend * leg333_10pt20 =new TLegend(0.75,0.75,0.88,0.88);    
  leg333_10pt20->AddEntry(h_dxyDiffRelOld_10pt20,leg);
  leg333_10pt20->AddEntry(h_dxyDiffRelNew_10pt20,leg2);
  leg333_10pt20->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiffRel_NNv6VsNNv8_10pt20.pdf");

  // 20 < pt
  h_ptDiffOld_20pt->Draw("");
  h_ptDiffNew_20pt->Draw("sames");
  
  TLegend * leg11_20pt =new TLegend(0.75,0.75,0.88,0.88);    
  leg11_20pt->AddEntry(h_ptDiffOld_20pt,leg);
  leg11_20pt->AddEntry(h_ptDiffNew_20pt,leg2);
  leg11_20pt->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiff_NNv6VsNNv8_20pt.pdf");

  h_InvPtDiffOld_20pt->Draw("");
  h_InvPtDiffNew_20pt->Draw("sames");
 
  TLegend * leg22_20pt =new TLegend(0.75,0.75,0.88,0.88);    
  leg22_20pt->AddEntry(h_InvPtDiffOld_20pt,leg);
  leg22_20pt->AddEntry(h_InvPtDiffNew_20pt,leg2);
  leg22_20pt->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiff_NNv6VsNNv8_20pt.pdf");

  h_InvPtDiffRelOld_20pt->Draw("");
  h_InvPtDiffRelNew_20pt->Draw("sames");
 
  TLegend * leg222_20pt =new TLegend(0.75,0.75,0.88,0.88);    
  leg222_20pt->AddEntry(h_InvPtDiffRelOld_20pt,leg);
  leg222_20pt->AddEntry(h_InvPtDiffRelNew_20pt,leg2);
  leg222_20pt->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiffRel_NNv6VsNNv8_20pt.pdf");

  h_ptDiffRelOld_20pt->Draw("");
  h_ptDiffRelNew_20pt->Draw("sames");
  
  TLegend * leg111_20pt =new TLegend(0.75,0.75,0.88,0.88);    
  leg111_20pt->AddEntry(h_ptDiffRelOld_20pt,leg);
  leg111_20pt->AddEntry(h_ptDiffRelNew_20pt,leg2);
  leg111_20pt->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiffRel_NNv6VsNNv8_20pt.pdf");

  h_dxyDiffOld_20pt->Draw("");
  h_dxyDiffNew_20pt->Draw("sames");
 
  TLegend * leg33_20pt =new TLegend(0.75,0.75,0.88,0.88);    
  leg33_20pt->AddEntry(h_dxyDiffOld_20pt,leg);
  leg33_20pt->AddEntry(h_dxyDiffNew_20pt,leg2);
  leg33_20pt->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiff_NNv6VsNNv8_20pt.pdf");

  h_dxyDiffRelOld_20pt->Draw("");
  h_dxyDiffRelNew_20pt->Draw("sames");
 
  TLegend * leg333_20pt =new TLegend(0.75,0.75,0.88,0.88);    
  leg333_20pt->AddEntry(h_dxyDiffRelOld_20pt,leg);
  leg333_20pt->AddEntry(h_dxyDiffRelNew_20pt,leg2);
  leg333_20pt->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiffRel_NNv6VsNNv8_20pt.pdf");

  // d0 < 25
  h_ptDiffOld_d025->Draw("");
  h_ptDiffNew_d025->Draw("sames");
  
  TLegend * leg11_d025 =new TLegend(0.75,0.75,0.88,0.88);    
  leg11_d025->AddEntry(h_ptDiffOld_d025,leg);
  leg11_d025->AddEntry(h_ptDiffNew_d025,leg2);
  leg11_d025->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiff_NNv6VsNNv8_d025.pdf");

  h_InvPtDiffOld_d025->Draw("");
  h_InvPtDiffNew_d025->Draw("sames");
 
  TLegend * leg22_d025 =new TLegend(0.75,0.75,0.88,0.88);    
  leg22_d025->AddEntry(h_InvPtDiffOld_d025,leg);
  leg22_d025->AddEntry(h_InvPtDiffNew_d025,leg2);
  leg22_d025->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiff_NNv6VsNNv8_d025.pdf");

  h_InvPtDiffRelOld_d025->Draw("");
  h_InvPtDiffRelNew_d025->Draw("sames");
 
  TLegend * leg222_d025 =new TLegend(0.75,0.75,0.88,0.88);    
  leg222_d025->AddEntry(h_InvPtDiffRelOld_d025,leg);
  leg222_d025->AddEntry(h_InvPtDiffRelNew_d025,leg2);
  leg222_d025->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiffRel_NNv6VsNNv8_d025.pdf");

  h_ptDiffRelOld_d025->Draw("");
  h_ptDiffRelNew_d025->Draw("sames");
  
  TLegend * leg111_d025 =new TLegend(0.75,0.75,0.88,0.88);    
  leg111_d025->AddEntry(h_ptDiffRelOld_d025,leg);
  leg111_d025->AddEntry(h_ptDiffRelNew_d025,leg2);
  leg111_d025->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiffRel_NNv6VsNNv8_d025.pdf");

  h_dxyDiffOld_d025->Draw("");
  h_dxyDiffNew_d025->Draw("sames");
 
  TLegend * leg33_d025 =new TLegend(0.75,0.75,0.88,0.88);    
  leg33_d025->AddEntry(h_dxyDiffOld_d025,leg);
  leg33_d025->AddEntry(h_dxyDiffNew_d025,leg2);
  leg33_d025->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiff_NNv6VsNNv8_d025.pdf");

  h_dxyDiffRelOld_d025->Draw("");
  h_dxyDiffRelNew_d025->Draw("sames");
 
  TLegend * leg333_d025 =new TLegend(0.75,0.75,0.88,0.88);    
  leg333_d025->AddEntry(h_dxyDiffRelOld_d025,leg);
  leg333_d025->AddEntry(h_dxyDiffRelNew_d025,leg2);
  leg333_d025->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiffRel_NNv6VsNNv8_d025.pdf");

  // 25 < d0 < 50
  h_ptDiffOld_25d050->Draw("");
  h_ptDiffNew_25d050->Draw("sames");
  
  TLegend * leg11_25d050 =new TLegend(0.75,0.75,0.88,0.88);    
  leg11_25d050->AddEntry(h_ptDiffOld_25d050,leg);
  leg11_25d050->AddEntry(h_ptDiffNew_25d050,leg2);
  leg11_25d050->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiff_NNv6VsNNv8_25d050.pdf");

  h_InvPtDiffOld_25d050->Draw("");
  h_InvPtDiffNew_25d050->Draw("sames");
 
  TLegend * leg22_25d050 =new TLegend(0.75,0.75,0.88,0.88);    
  leg22_25d050->AddEntry(h_InvPtDiffOld_25d050,leg);
  leg22_25d050->AddEntry(h_InvPtDiffNew_25d050,leg2);
  leg22_25d050->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiff_NNv6VsNNv8_25d050.pdf");

  h_InvPtDiffRelOld_25d050->Draw("");
  h_InvPtDiffRelNew_25d050->Draw("sames");
 
  TLegend * leg222_25d050 =new TLegend(0.75,0.75,0.88,0.88);    
  leg222_25d050->AddEntry(h_InvPtDiffRelOld_25d050,leg);
  leg222_25d050->AddEntry(h_InvPtDiffRelNew_25d050,leg2);
  leg222_25d050->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiffRel_NNv6VsNNv8_25d050.pdf");

  h_ptDiffRelOld_25d050->Draw("");
  h_ptDiffRelNew_25d050->Draw("sames");
  
  TLegend * leg111_25d050 =new TLegend(0.75,0.75,0.88,0.88);    
  leg111_25d050->AddEntry(h_ptDiffRelOld_25d050,leg);
  leg111_25d050->AddEntry(h_ptDiffRelNew_25d050,leg2);
  leg111_25d050->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiffRel_NNv6VsNNv8_25d050.pdf");

  h_dxyDiffOld_25d050->Draw("");
  h_dxyDiffNew_25d050->Draw("sames");
 
  TLegend * leg33_25d050 =new TLegend(0.75,0.75,0.88,0.88);    
  leg33_25d050->AddEntry(h_dxyDiffOld_25d050,leg);
  leg33_25d050->AddEntry(h_dxyDiffNew_25d050,leg2);
  leg33_25d050->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiff_NNv6VsNNv8_25d050.pdf");

  h_dxyDiffRelOld_25d050->Draw("");
  h_dxyDiffRelNew_25d050->Draw("sames");
 
  TLegend * leg333_25d050 =new TLegend(0.75,0.75,0.88,0.88);    
  leg333_25d050->AddEntry(h_dxyDiffRelOld_25d050,leg);
  leg333_25d050->AddEntry(h_dxyDiffRelNew_25d050,leg2);
  leg333_25d050->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiffRel_NNv6VsNNv8_25d050.pdf");

  // 50 < d0
  h_ptDiffOld_50d0->Draw("");
  h_ptDiffNew_50d0->Draw("sames");
  
  TLegend * leg11_50d0 =new TLegend(0.75,0.75,0.88,0.88);    
  leg11_50d0->AddEntry(h_ptDiffOld_50d0,leg);
  leg11_50d0->AddEntry(h_ptDiffNew_50d0,leg2);
  leg11_50d0->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiff_NNv6VsNNv8_50d0.pdf");

  h_InvPtDiffOld_50d0->Draw("");
  h_InvPtDiffNew_50d0->Draw("sames");
 
  TLegend * leg22_50d0 =new TLegend(0.75,0.75,0.88,0.88);    
  leg22_50d0->AddEntry(h_InvPtDiffOld_50d0,leg);
  leg22_50d0->AddEntry(h_InvPtDiffNew_50d0,leg2);
  leg22_50d0->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiff_NNv6VsNNv8_50d0.pdf");

  h_InvPtDiffRelOld_50d0->Draw("");
  h_InvPtDiffRelNew_50d0->Draw("sames");
 
  TLegend * leg222_50d0 =new TLegend(0.75,0.75,0.88,0.88);    
  leg222_50d0->AddEntry(h_InvPtDiffRelOld_50d0,leg);
  leg222_50d0->AddEntry(h_InvPtDiffRelNew_50d0,leg2);
  leg222_50d0->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiffRel_NNv6VsNNv8_50d0.pdf");

  h_ptDiffRelOld_50d0->Draw("");
  h_ptDiffRelNew_50d0->Draw("sames");
  
  TLegend * leg111_50d0 =new TLegend(0.75,0.75,0.88,0.88);    
  leg111_50d0->AddEntry(h_ptDiffRelOld_50d0,leg);
  leg111_50d0->AddEntry(h_ptDiffRelNew_50d0,leg2);
  leg111_50d0->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiffRel_NNv6VsNNv8_50d0.pdf");

  h_dxyDiffOld_50d0->Draw("");
  h_dxyDiffNew_50d0->Draw("sames");
 
  TLegend * leg33_50d0 =new TLegend(0.75,0.75,0.88,0.88);    
  leg33_50d0->AddEntry(h_dxyDiffOld_50d0,leg);
  leg33_50d0->AddEntry(h_dxyDiffNew_50d0,leg2);
  leg33_50d0->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiff_NNv6VsNNv8_50d0.pdf");

  h_dxyDiffRelOld_50d0->Draw("");
  h_dxyDiffRelNew_50d0->Draw("sames");
 
  TLegend * leg333_50d0 =new TLegend(0.75,0.75,0.88,0.88);    
  leg333_50d0->AddEntry(h_dxyDiffRelOld_50d0,leg);
  leg333_50d0->AddEntry(h_dxyDiffRelNew_50d0,leg2);
  leg333_50d0->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiffRel_NNv6VsNNv8_50d0.pdf");

  // z0 < 25
  h_ptDiffOld_z025->Draw("");
  h_ptDiffNew_z025->Draw("sames");
  
  TLegend * leg11_z025 =new TLegend(0.75,0.75,0.88,0.88);    
  leg11_z025->AddEntry(h_ptDiffOld_z025,leg);
  leg11_z025->AddEntry(h_ptDiffNew_z025,leg2);
  leg11_z025->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiff_NNv6VsNNv8_z025.pdf");

  h_InvPtDiffOld_z025->Draw("");
  h_InvPtDiffNew_z025->Draw("sames");
 
  TLegend * leg22_z025 =new TLegend(0.75,0.75,0.88,0.88);    
  leg22_z025->AddEntry(h_InvPtDiffOld_z025,leg);
  leg22_z025->AddEntry(h_InvPtDiffNew_z025,leg2);
  leg22_z025->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiff_NNv6VsNNv8_z025.pdf");

  h_InvPtDiffRelOld_z025->Draw("");
  h_InvPtDiffRelNew_z025->Draw("sames");
 
  TLegend * leg222_z025 =new TLegend(0.75,0.75,0.88,0.88);    
  leg222_z025->AddEntry(h_InvPtDiffRelOld_z025,leg);
  leg222_z025->AddEntry(h_InvPtDiffRelNew_z025,leg2);
  leg222_z025->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiffRel_NNv6VsNNv8_z025.pdf");

  h_ptDiffRelOld_z025->Draw("");
  h_ptDiffRelNew_z025->Draw("sames");
  
  TLegend * leg111_z025 =new TLegend(0.75,0.75,0.88,0.88);    
  leg111_z025->AddEntry(h_ptDiffRelOld_z025,leg);
  leg111_z025->AddEntry(h_ptDiffRelNew_z025,leg2);
  leg111_z025->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiffRel_NNv6VsNNv8_z025.pdf");

  h_dxyDiffOld_z025->Draw("");
  h_dxyDiffNew_z025->Draw("sames");
 
  TLegend * leg33_z025 =new TLegend(0.75,0.75,0.88,0.88);    
  leg33_z025->AddEntry(h_dxyDiffOld_z025,leg);
  leg33_z025->AddEntry(h_dxyDiffNew_z025,leg2);
  leg33_z025->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiff_NNv6VsNNv8_z025.pdf");

  h_dxyDiffRelOld_z025->Draw("");
  h_dxyDiffRelNew_z025->Draw("sames");
 
  TLegend * leg333_z025 =new TLegend(0.75,0.75,0.88,0.88);    
  leg333_z025->AddEntry(h_dxyDiffRelOld_z025,leg);
  leg333_z025->AddEntry(h_dxyDiffRelNew_z025,leg2);
  leg333_z025->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiffRel_NNv6VsNNv8_z025.pdf");

  // 25 < z0 < 50
  h_ptDiffOld_25z050->Draw("");
  h_ptDiffNew_25z050->Draw("sames");
  
  TLegend * leg11_25z050 =new TLegend(0.75,0.75,0.88,0.88);    
  leg11_25z050->AddEntry(h_ptDiffOld_25z050,leg);
  leg11_25z050->AddEntry(h_ptDiffNew_25z050,leg2);
  leg11_25z050->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiff_NNv6VsNNv8_25z050.pdf");

  h_InvPtDiffOld_25z050->Draw("");
  h_InvPtDiffNew_25z050->Draw("sames");
 
  TLegend * leg22_25z050 =new TLegend(0.75,0.75,0.88,0.88);    
  leg22_25z050->AddEntry(h_InvPtDiffOld_25z050,leg);
  leg22_25z050->AddEntry(h_InvPtDiffNew_25z050,leg2);
  leg22_25z050->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiff_NNv6VsNNv8_25z050.pdf");

  h_InvPtDiffRelOld_25z050->Draw("");
  h_InvPtDiffRelNew_25z050->Draw("sames");
 
  TLegend * leg222_25z050 =new TLegend(0.75,0.75,0.88,0.88);    
  leg222_25z050->AddEntry(h_InvPtDiffRelOld_25z050,leg);
  leg222_25z050->AddEntry(h_InvPtDiffRelNew_25z050,leg2);
  leg222_25z050->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiffRel_NNv6VsNNv8_25z050.pdf");

  h_ptDiffRelOld_25z050->Draw("");
  h_ptDiffRelNew_25z050->Draw("sames");
  
  TLegend * leg111_25z050 =new TLegend(0.75,0.75,0.88,0.88);    
  leg111_25z050->AddEntry(h_ptDiffRelOld_25z050,leg);
  leg111_25z050->AddEntry(h_ptDiffRelNew_25z050,leg2);
  leg111_25z050->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiffRel_NNv6VsNNv8_25z050.pdf");

  h_dxyDiffOld_25z050->Draw("");
  h_dxyDiffNew_25z050->Draw("sames");
 
  TLegend * leg33_25z050 =new TLegend(0.75,0.75,0.88,0.88);    
  leg33_25z050->AddEntry(h_dxyDiffOld_25z050,leg);
  leg33_25z050->AddEntry(h_dxyDiffNew_25z050,leg2);
  leg33_25z050->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiff_NNv6VsNNv8_25z050.pdf");

  h_dxyDiffRelOld_25z050->Draw("");
  h_dxyDiffRelNew_25z050->Draw("sames");
 
  TLegend * leg333_25z050 =new TLegend(0.75,0.75,0.88,0.88);    
  leg333_25z050->AddEntry(h_dxyDiffRelOld_25z050,leg);
  leg333_25z050->AddEntry(h_dxyDiffRelNew_25z050,leg2);
  leg333_25z050->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiffRel_NNv6VsNNv8_25z050.pdf");

  // 50 < z0
  h_ptDiffOld_50z0->Draw("");
  h_ptDiffNew_50z0->Draw("sames");
  
  TLegend * leg11_50z0 =new TLegend(0.75,0.75,0.88,0.88);    
  leg11_50z0->AddEntry(h_ptDiffOld_50z0,leg);
  leg11_50z0->AddEntry(h_ptDiffNew_50z0,leg2);
  leg11_50z0->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiff_NNv6VsNNv8_50z0.pdf");

  h_InvPtDiffOld_50z0->Draw("");
  h_InvPtDiffNew_50z0->Draw("sames");
 
  TLegend * leg22_50z0 =new TLegend(0.75,0.75,0.88,0.88);    
  leg22_50z0->AddEntry(h_InvPtDiffOld_50z0,leg);
  leg22_50z0->AddEntry(h_InvPtDiffNew_50z0,leg2);
  leg22_50z0->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiff_NNv6VsNNv8_50z0.pdf");

  h_InvPtDiffRelOld_50z0->Draw("");
  h_InvPtDiffRelNew_50z0->Draw("sames");
 
  TLegend * leg222_50z0 =new TLegend(0.75,0.75,0.88,0.88);    
  leg222_50z0->AddEntry(h_InvPtDiffRelOld_50z0,leg);
  leg222_50z0->AddEntry(h_InvPtDiffRelNew_50z0,leg2);
  leg222_50z0->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiffRel_NNv6VsNNv8_50z0.pdf");

  h_ptDiffRelOld_50z0->Draw("");
  h_ptDiffRelNew_50z0->Draw("sames");
  
  TLegend * leg111_50z0 =new TLegend(0.75,0.75,0.88,0.88);    
  leg111_50z0->AddEntry(h_ptDiffRelOld_50z0,leg);
  leg111_50z0->AddEntry(h_ptDiffRelNew_50z0,leg2);
  leg111_50z0->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiffRel_NNv6VsNNv8_50z0.pdf");

  h_dxyDiffOld_50z0->Draw("");
  h_dxyDiffNew_50z0->Draw("sames");
 
  TLegend * leg33_50z0 =new TLegend(0.75,0.75,0.88,0.88);    
  leg33_50z0->AddEntry(h_dxyDiffOld_50z0,leg);
  leg33_50z0->AddEntry(h_dxyDiffNew_50z0,leg2);
  leg33_50z0->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiff_NNv6VsNNv8_50z0.pdf");

  h_dxyDiffRelOld_50z0->Draw("");
  h_dxyDiffRelNew_50z0->Draw("sames");
 
  TLegend * leg333_50z0 =new TLegend(0.75,0.75,0.88,0.88);    
  leg333_50z0->AddEntry(h_dxyDiffRelOld_50z0,leg);
  leg333_50z0->AddEntry(h_dxyDiffRelNew_50z0,leg2);
  leg333_50z0->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiffRel_NNv6VsNNv8_50z0.pdf");

  // 1.2 < eta < 1.6
  h_ptDiffOld_eta1->Draw("");
  h_ptDiffNew_eta1->Draw("sames");
  
  TLegend * leg11_eta1 =new TLegend(0.75,0.75,0.88,0.88);    
  leg11_eta1->AddEntry(h_ptDiffOld_eta1,leg);
  leg11_eta1->AddEntry(h_ptDiffNew_eta1,leg2);
  leg11_eta1->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiff_NNv6VsNNv8_eta1.pdf");

  h_InvPtDiffOld_eta1->Draw("");
  h_InvPtDiffNew_eta1->Draw("sames");
 
  TLegend * leg22_eta1 =new TLegend(0.75,0.75,0.88,0.88);    
  leg22_eta1->AddEntry(h_InvPtDiffOld_eta1,leg);
  leg22_eta1->AddEntry(h_InvPtDiffNew_eta1,leg2);
  leg22_eta1->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiff_NNv6VsNNv8_eta1.pdf");

  h_InvPtDiffRelOld_eta1->Draw("");
  h_InvPtDiffRelNew_eta1->Draw("sames");
 
  TLegend * leg222_eta1 =new TLegend(0.75,0.75,0.88,0.88);    
  leg222_eta1->AddEntry(h_InvPtDiffRelOld_eta1,leg);
  leg222_eta1->AddEntry(h_InvPtDiffRelNew_eta1,leg2);
  leg222_eta1->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiffRel_NNv6VsNNv8_eta1.pdf");

  h_ptDiffRelOld_eta1->Draw("");
  h_ptDiffRelNew_eta1->Draw("sames");
  
  TLegend * leg111_eta1 =new TLegend(0.75,0.75,0.88,0.88);    
  leg111_eta1->AddEntry(h_ptDiffRelOld_eta1,leg);
  leg111_eta1->AddEntry(h_ptDiffRelNew_eta1,leg2);
  leg111_eta1->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiffRel_NNv6VsNNv8_eta1.pdf");

  h_dxyDiffOld_eta1->Draw("");
  h_dxyDiffNew_eta1->Draw("sames");
 
  TLegend * leg33_eta1 =new TLegend(0.75,0.75,0.88,0.88);    
  leg33_eta1->AddEntry(h_dxyDiffOld_eta1,leg);
  leg33_eta1->AddEntry(h_dxyDiffNew_eta1,leg2);
  leg33_eta1->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiff_NNv6VsNNv8_eta1.pdf");

  h_dxyDiffRelOld_eta1->Draw("");
  h_dxyDiffRelNew_eta1->Draw("sames");
 
  TLegend * leg333_eta1 =new TLegend(0.75,0.75,0.88,0.88);    
  leg333_eta1->AddEntry(h_dxyDiffRelOld_eta1,leg);
  leg333_eta1->AddEntry(h_dxyDiffRelNew_eta1,leg2);
  leg333_eta1->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiffRel_NNv6VsNNv8_eta1.pdf");

  // 1.6 < eta < 2.1
  h_ptDiffOld_eta2->Draw("");
  h_ptDiffNew_eta2->Draw("sames");
  
  TLegend * leg11_eta2 =new TLegend(0.75,0.75,0.88,0.88);    
  leg11_eta2->AddEntry(h_ptDiffOld_eta2,leg);
  leg11_eta2->AddEntry(h_ptDiffNew_eta2,leg2);
  leg11_eta2->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiff_NNv6VsNNv8_eta2.pdf");

  h_InvPtDiffOld_eta2->Draw("");
  h_InvPtDiffNew_eta2->Draw("sames");
 
  TLegend * leg22_eta2 =new TLegend(0.75,0.75,0.88,0.88);    
  leg22_eta2->AddEntry(h_InvPtDiffOld_eta2,leg);
  leg22_eta2->AddEntry(h_InvPtDiffNew_eta2,leg2);
  leg22_eta2->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiff_NNv6VsNNv8_eta2.pdf");

  h_InvPtDiffRelOld_eta2->Draw("");
  h_InvPtDiffRelNew_eta2->Draw("sames");
 
  TLegend * leg222_eta2 =new TLegend(0.75,0.75,0.88,0.88);    
  leg222_eta2->AddEntry(h_InvPtDiffRelOld_eta2,leg);
  leg222_eta2->AddEntry(h_InvPtDiffRelNew_eta2,leg2);
  leg222_eta2->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiffRel_NNv6VsNNv8_eta2.pdf");

  h_ptDiffRelOld_eta2->Draw("");
  h_ptDiffRelNew_eta2->Draw("sames");
  
  TLegend * leg111_eta2 =new TLegend(0.75,0.75,0.88,0.88);    
  leg111_eta2->AddEntry(h_ptDiffRelOld_eta2,leg);
  leg111_eta2->AddEntry(h_ptDiffRelNew_eta2,leg2);
  leg111_eta2->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiffRel_NNv6VsNNv8_eta2.pdf");

  h_dxyDiffOld_eta2->Draw("");
  h_dxyDiffNew_eta2->Draw("sames");
 
  TLegend * leg33_eta2 =new TLegend(0.75,0.75,0.88,0.88);    
  leg33_eta2->AddEntry(h_dxyDiffOld_eta2,leg);
  leg33_eta2->AddEntry(h_dxyDiffNew_eta2,leg2);
  leg33_eta2->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiff_NNv6VsNNv8_eta2.pdf");

  h_dxyDiffRelOld_eta2->Draw("");
  h_dxyDiffRelNew_eta2->Draw("sames");
 
  TLegend * leg333_eta2 =new TLegend(0.75,0.75,0.88,0.88);    
  leg333_eta2->AddEntry(h_dxyDiffRelOld_eta2,leg);
  leg333_eta2->AddEntry(h_dxyDiffRelNew_eta2,leg2);
  leg333_eta2->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiffRel_NNv6VsNNv8_eta2.pdf");

  // 2.1 < eta < 2.5
  h_ptDiffOld_eta3->Draw("");
  h_ptDiffNew_eta3->Draw("sames");
  
  TLegend * leg11_eta3 =new TLegend(0.75,0.75,0.88,0.88);    
  leg11_eta3->AddEntry(h_ptDiffOld_eta3,leg);
  leg11_eta3->AddEntry(h_ptDiffNew_eta3,leg2);
  leg11_eta3->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiff_NNv6VsNNv8_eta3.pdf");

  h_InvPtDiffOld_eta3->Draw("");
  h_InvPtDiffNew_eta3->Draw("sames");
 
  TLegend * leg22_eta3 =new TLegend(0.75,0.75,0.88,0.88);    
  leg22_eta3->AddEntry(h_InvPtDiffOld_eta3,leg);
  leg22_eta3->AddEntry(h_InvPtDiffNew_eta3,leg2);
  leg22_eta3->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiff_NNv6VsNNv8_eta3.pdf");

  h_InvPtDiffRelOld_eta3->Draw("");
  h_InvPtDiffRelNew_eta3->Draw("sames");
 
  TLegend * leg222_eta3 =new TLegend(0.75,0.75,0.88,0.88);    
  leg222_eta3->AddEntry(h_InvPtDiffRelOld_eta3,leg);
  leg222_eta3->AddEntry(h_InvPtDiffRelNew_eta3,leg2);
  leg222_eta3->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/invPTDiffRel_NNv6VsNNv8_eta3.pdf");

  h_ptDiffRelOld_eta3->Draw("");
  h_ptDiffRelNew_eta3->Draw("sames");
  
  TLegend * leg111_eta3 =new TLegend(0.75,0.75,0.88,0.88);    
  leg111_eta3->AddEntry(h_ptDiffRelOld_eta3,leg);
  leg111_eta3->AddEntry(h_ptDiffRelNew_eta3,leg2);
  leg111_eta3->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/pTDiffRel_NNv6VsNNv8_eta3.pdf");

  h_dxyDiffOld_eta3->Draw("");
  h_dxyDiffNew_eta3->Draw("sames");
 
  TLegend * leg33_eta3 =new TLegend(0.75,0.75,0.88,0.88);    
  leg33_eta3->AddEntry(h_dxyDiffOld_eta3,leg);
  leg33_eta3->AddEntry(h_dxyDiffNew_eta3,leg2);
  leg33_eta3->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiff_NNv6VsNNv8_eta3.pdf");

  h_dxyDiffRelOld_eta3->Draw("");
  h_dxyDiffRelNew_eta3->Draw("sames");
 
  TLegend * leg333_eta3 =new TLegend(0.75,0.75,0.88,0.88);    
  leg333_eta3->AddEntry(h_dxyDiffRelOld_eta3,leg);
  leg333_eta3->AddEntry(h_dxyDiffRelNew_eta3,leg2);
  leg333_eta3->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiffRel_NNv6VsNNv8_eta3.pdf");

  c1->SetRightMargin(0.14);
  c1->SetLeftMargin(0.12);
  gPad->SetLogz();
  // 2D plots
  h_ptDiffOld_2D_pt->Draw("colz");
  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/ptDiffOld2D_NNv5_log_pt.pdf");

  h_ptDiffOld_2D_dxy->Draw("colz");
  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/ptDiffOld2D_NNv5_log_dxy.pdf");

  h_ptDiffOld_2D_eta->Draw("colz");
  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/ptDiffOld2D_NNv5_log_eta.pdf");

  h_InvPtDiffOld_2D_pt->Draw("colz");
  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/InvPtDiffOld2D_NNv5_log_pt.pdf");

  h_InvPtDiffOld_2D_dxy->Draw("colz");
  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/InvPtDiffOld2D_NNv5_log_dxy.pdf");

  h_InvPtDiffOld_2D_eta->Draw("colz");
  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/InvPtDiffOld2D_NNv5_log_eta.pdf");

  h_dxyDiffOld_2D_pt->Draw("colz");
  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiffOld2D_NNv5_log_pt.pdf");

  h_dxyDiffOld_2D_dxy->Draw("colz");
  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiffOld2D_NNv5_log_dxy.pdf");

  h_dxyDiffOld_2D_eta->Draw("colz");
  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiffOld2D_NNv5_log_eta.pdf");

  h_ptDiffNew_2D_pt->Draw("colz");
  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/ptDiffNew2D_NNv6_log_pt.pdf");

  h_ptDiffNew_2D_dxy->Draw("colz");
  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/ptDiffNew2D_NNv6_log_dxy.pdf");

  h_ptDiffNew_2D_eta->Draw("colz");
  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/ptDiffNew2D_NNv6_log_eta.pdf");

  h_InvPtDiffNew_2D_pt->Draw("colz");
  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/InvPtDiffNew2D_NNv5_log_pt.pdf");

  h_InvPtDiffNew_2D_dxy->Draw("colz");
  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/InvPtDiffNew2D_NNv5_log_dxy.pdf");

  h_InvPtDiffNew_2D_eta->Draw("colz");
  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/InvPtDiffNew2D_NNv5_log_eta.pdf");

  h_dxyDiffNew_2D_pt->Draw("colz");
  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiffNew2D_NNv5_log_pt.pdf");

  h_dxyDiffNew_2D_dxy->Draw("colz");
  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiffNew2D_NNv5_log_dxy.pdf");

  h_dxyDiffNew_2D_eta->Draw("colz");
  c1->SaveAs("./output_files/comparisons/NNv6_NNv8_genCharge/dxyDiffNew2D_NNv5_log_eta.pdf");






  return 0;
 }
