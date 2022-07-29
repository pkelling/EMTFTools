/* EMTF Rate Analysis plotting script
 *
 */

#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>
#include "TLatex.h"
#include <boost/algorithm/string.hpp>

float DPhi(double phi1,double phi2){
  float temp=phi1-phi2;
  if (temp>3.14) temp=temp-6.28;
  if (temp<-3.14) temp=temp+6.28;
  return temp;
}

TLatex cms_latex(){
  TLatex cms_label;
  cms_label.SetTextSize(0.04);
  cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary Simulation}}");
  return cms_label;
}

TLatex head(){
  TLatex header; 
  header.SetTextSize(0.03);
  // header.DrawLatexNDC(0.63, 0.92, "#sqrt{s} = 13 TeV, Run 3 MC");
  header.DrawLatexNDC(0.63, 0.92, "BDT");
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


int makeRatePlots(){

  //read data

  // Nu Gun
  // TString ntuple = "/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/Neutrino_Pt-2to20_gun/EMTFNtuple_Neutrino_Pt-2to20_gun_cmssw_11_0_2_fwImplementation_NNv4/201118_143900/EMTFNtuple_Neutrino_Pt-2to20_gun_cmssw_11_0_2_fwImplementation_NNv4.root";
  // TString ntuple = "/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/Neutrino_Pt-2to20_gun/EMTFNtuple_Neutrino_Pt-2to20_gun_cmssw_11_0_2_fwImplementation_NNv5/201118_155052/EMTFNtuple_Neutrino_Pt-2to20_gun_cmssw_11_0_2_fwImplementation_NNv5.root";

  // Zero Bias
  // TString ntuple = "/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/ZeroBias/EMTFNtuple_ZeroBias_Run2018D_cmssw_11_0_2_fwImplementation_NNv4/201123_163739/EMTFNtuple_ZeroBias_Run2018D_cmssw_11_0_2_fwImplementation_NNv4.root";
  // TString ntuple = "/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/ZeroBias/EMTFNtuple_ZeroBias_Run2018D_cmssw_11_0_2_fwImplementation_NNv5/201123_162954/EMTFNtuple_ZeroBias_Run2018D_cmssw_11_0_2_fwImplementation_NNv5.root";
  // TString ntuple = "/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/Nu_E10-pythia8-gun/NuGun_11_3_0_pre5_5M/210421_154733/NuGun_11_3_0_pre5_5M.root";
  // TString ntuple = "/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/ZeroBias/EMTFNtuple_Run3_ZB_data_13p6TeV_355872_v1/220727_154129/EMTFNtuple_Run3_ZB_data_13p6TeV_355872_v1.root";
  TString ntuple = "/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/ZeroBias/EMTFNtuple_Run3_ZB_data_oldBDT_13p6TeV_355872_v1/220727_164248/EMTFNtuple_Run3_ZB_data_oldBDT_13p6TeV_355872_v1.root";

  TChain * cc=new TChain("EMTFNtuple/tree");
  cc->Add(ntuple);


  TTreeReader reader(cc);
  // EMU
  TTreeReaderValue<int32_t> emtfTrackSizeEmu(reader,"emtfTrack_size");
  TTreeReaderArray<float  > emtfTrackPtEmu(reader,"emtfTrack_pt");
  TTreeReaderArray<float  > emtfTrackPtDxyEmu(reader,"emtfTrack_pt_dxy");
  TTreeReaderArray<float  > emtfTrackDxyEmu(reader,"emtfTrack_dxy");
  TTreeReaderArray<float  > emtfTrackPhiEmu(reader,"emtfTrack_phi");
  TTreeReaderArray<float  > emtfTrackThetaEmu(reader,"emtfTrack_theta");
  TTreeReaderArray<float  > emtfTrackEtaEmu(reader,"emtfTrack_eta");
  TTreeReaderArray<int    > emtfTrackGMTPhiEmu(reader,"emtfTrack_GMT_phi");
  TTreeReaderArray<int    > emtfTrackGMTEtaEmu(reader,"emtfTrack_GMT_eta");
  TTreeReaderArray<short  > emtfTrackModeEmu(reader,"emtfTrack_mode");
  TTreeReaderArray<short  > emtfTrackEndcapEmu(reader,"emtfTrack_endcap");
  TTreeReaderArray<short  > emtfTrackSectorEmu(reader,"emtfTrack_sector");
  TTreeReaderArray<short  > emtfTrackBXEmu(reader,"emtfTrack_bx");


  // UNP
  TTreeReaderValue<int32_t> emtfTrackSize(reader,"emtfUnpTrack_size");
  TTreeReaderArray<float  > emtfTrackPt(reader,"emtfUnpTrack_pt");
  TTreeReaderArray<float  > emtfTrackPtDxy(reader,"emtfUnpTrack_pt_dxy");
  TTreeReaderArray<float  > emtfTrackDxy(reader,"emtfUnpTrack_dxy");
  TTreeReaderArray<float  > emtfTrackPhi(reader,"emtfUnpTrack_phi");
  TTreeReaderArray<float  > emtfTrackTheta(reader,"emtfUnpTrack_theta");
  TTreeReaderArray<float  > emtfTrackEta(reader,"emtfUnpTrack_eta");
  // TTreeReaderArray<int    > emtfTrackGMTPhi(reader,"emtfUnpTrack_GMT_phi");
  // TTreeReaderArray<int    > emtfTrackGMTEta(reader,"emtfUnpTrack_GMT_eta");
  TTreeReaderArray<short  > emtfTrackMode(reader,"emtfUnpTrack_mode");
  TTreeReaderArray<short  > emtfTrackEndcap(reader,"emtfUnpTrack_endcap");
  TTreeReaderArray<short  > emtfTrackSector(reader,"emtfUnpTrack_sector");
  TTreeReaderArray<short  > emtfTrackBX(reader,"emtfUnpTrack_bx");


  // TTreeReaderValue<int32_t> gmtMuonSize(reader,"gmtMuon_size");
  // TTreeReaderArray<float  > gmtMuonPt(reader,"gmtMuon_pt");
  // TTreeReaderArray<float  > gmtMuonPtDxy(reader,"gmtMuon_pt_dxy");
  // TTreeReaderArray<float  > gmtMuonPtDxyNN(reader,"gmtMuon_pt_dxyNN");
  // TTreeReaderArray<short  > gmtMuonDxy(reader,"gmtMuon_dxy");
  // TTreeReaderArray<float  > gmtMuonDxyNN(reader,"gmtMuon_dxyNN");
  // TTreeReaderArray<float  > gmtMuonPhi(reader,"gmtMuon_phi");
  // TTreeReaderArray<float  > gmtMuonEta(reader,"gmtMuon_eta");
  // TTreeReaderArray<short  > gmtMuonCharge(reader,"gmtMuon_q");
  // TTreeReaderArray<short  > gmtMuonQual(reader,"gmtMuon_qual");

  gStyle->SetOptStat(0);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetLegendTextSize(0.03);
  std::cout<<"Running on "<<cc->GetEntries()<<" evts "<<std::endl;

  //plot containers
  std::vector<TString> canvasname;
  std::vector<std::string> kwds;
  std::vector<TString> legs;
  std::vector<TGraphAsymmErrors*> errors;

  // cosmetic options
  std::vector<bool> grid,logY,logX;



  // initialize cuts

  TH1F *h_pT_noDxy = new TH1F("h_pT_noDxy", "", 300, 0.5, 300.5);
  // TH1F *h_pT_25Dxy = new TH1F("h_pT_25Dxy", "", 300, 0.5, 300.5);
  // TH1F *h_pT_50Dxy = new TH1F("h_pT_50Dxy", "", 300, 0.5, 300.5);
  // TH1F *h_pT_75Dxy = new TH1F("h_pT_75Dxy", "", 300, 0.5, 300.5);

  TH1F *h_pT_noDxyEta1 = new TH1F("h_pT_noDxyEta1", "", 300, 0.5, 300.5);
  // TH1F *h_pT_25DxyEta1 = new TH1F("h_pT_25DxyEta1", "", 300, 0.5, 300.5);
  // TH1F *h_pT_50DxyEta1 = new TH1F("h_pT_50DxyEta1", "", 300, 0.5, 300.5);
  // TH1F *h_pT_75DxyEta1 = new TH1F("h_pT_75DxyEta1", "", 300, 0.5, 300.5);

  TH1F *h_pT_noDxyEta2 = new TH1F("h_pT_noDxyEta2", "", 300, 0.5, 300.5);
  // TH1F *h_pT_25DxyEta2 = new TH1F("h_pT_25DxyEta2", "", 300, 0.5, 300.5);
  // TH1F *h_pT_50DxyEta2 = new TH1F("h_pT_50DxyEta2", "", 300, 0.5, 300.5);
  // TH1F *h_pT_75DxyEta2 = new TH1F("h_pT_75DxyEta2", "", 300, 0.5, 300.5);

  TH1F *h_pT_noDxyEta3 = new TH1F("h_pT_noDxyEta3", "", 300, 0.5, 300.5);
  // TH1F *h_pT_25DxyEta3 = new TH1F("h_pT_25DxyEta3", "", 300, 0.5, 300.5);
  // TH1F *h_pT_50DxyEta3 = new TH1F("h_pT_50DxyEta3", "", 300, 0.5, 300.5);
  // TH1F *h_pT_75DxyEta3 = new TH1F("h_pT_75DxyEta3", "", 300, 0.5, 300.5);

  TH1F *h_pT_noDxyEndCapNeg = new TH1F("h_pT_noDxyEndCapNeg", "", 300, 0.5, 300.5);
  TH1F *h_pT_noDxyEndCapPos = new TH1F("h_pT_noDxyEndCapPos", "", 300, 0.5, 300.5);


  TH1F *h_pT_noDxyEmu = new TH1F("h_pT_noDxyEmu", "", 300, 0.5, 300.5);

  TH1F *h_pT_noDxyEta1Emu = new TH1F("h_pT_noDxyEta1Emu", "", 300, 0.5, 300.5);

  TH1F *h_pT_noDxyEta2Emu = new TH1F("h_pT_noDxyEta2Emu", "", 300, 0.5, 300.5);

  TH1F *h_pT_noDxyEta3Emu = new TH1F("h_pT_noDxyEta3Emu", "", 300, 0.5, 300.5);

  TH1F *h_pT_noDxyEndCapNegEmu = new TH1F("h_pT_noDxyEndCapNegEmu", "", 300, 0.5, 300.5);

  TH1F *h_pT_noDxyEndCapPosEmu = new TH1F("h_pT_noDxyEndCapPosEmu", "", 300, 0.5, 300.5);

  // TH1F *h_pT_noDxyDimu = new TH1F("h_pT_noDxyDimu", "NNv6", 30, 0.5, 30.5);
  // TH1F *h_pT_25DxyDimu = new TH1F("h_pT_25DxyDimu", "", 30, 0.5, 30.5);
  // TH1F *h_pT_50DxyDimu = new TH1F("h_pT_50DxyDimu", "", 30, 0.5, 30.5);
  // TH1F *h_pT_75DxyDimu = new TH1F("h_pT_75DxyDimu", "", 30, 0.5, 30.5);

  // TH1F *h_pT_noDxyDimu10 = new TH1F("h_pT_noDxyDimu10", "NNv6", 30, 0.5, 30.5);
  // TH1F *h_pT_25DxyDimu10 = new TH1F("h_pT_25DxyDimu10", "", 30, 0.5, 30.5);
  // TH1F *h_pT_50DxyDimu10 = new TH1F("h_pT_50DxyDimu10", "", 30, 0.5, 30.5);
  // TH1F *h_pT_75DxyDimu10 = new TH1F("h_pT_75DxyDimu10", "", 30, 0.5, 30.5);

  // TH1F *h_pT_noDxyDimu20 = new TH1F("h_pT_noDxyDimu20", "NNv6", 30, 0.5, 30.5);
  // TH1F *h_pT_25DxyDimu20 = new TH1F("h_pT_25DxyDimu20", "", 30, 0.5, 30.5);
  // TH1F *h_pT_50DxyDimu20 = new TH1F("h_pT_50DxyDimu20", "", 30, 0.5, 30.5);
  // TH1F *h_pT_75DxyDimu20 = new TH1F("h_pT_75DxyDimu20", "", 30, 0.5, 30.5);






  int eventCount = 0;
  int nBunches = 2760; 
  float fLHC = 11.246; // khZ
  float rate;
  // rate = nBunches*fLHC/cc->GetEntries();
  rate = 1;

  bool doSingleMuon = true;
  bool doDimuon     = false;

  if (doSingleMuon){
    // Single muon rates 
    while(reader.Next()){
      eventCount++;
      if (eventCount % 10000 == 0) std::cout << eventCount << " events read!" << std::endl;
      for (int ptThreshold = 0; ptThreshold < 300; ptThreshold++){
        bool passedEvent = false;

        for (int j=0; j<*emtfTrackSize; j++){
          bool passedMuon = false;
          if(emtfTrackBX[j] != 0) continue;
          // float GMTEta = emtfTrackGMTEta[j] * 0.010875;
          // if(abs(GMTEta) < 1.24 or abs(GMTEta) > 2.5) continue;
          if(abs(emtfTrackEta[j]) < 1.24 or abs(emtfTrackEta[j]) > 2.5) continue;
          // std::cout  << "after eta + BX cuts" << std::endl;

          // }
          // if (idx_EMTF < 0) continue;
          if (emtfTrackMode[j] < 11 || emtfTrackMode[j] == 12) continue;


          if (emtfTrackPt[j] > ptThreshold){
            passedMuon = true;
            h_pT_noDxy->Fill(ptThreshold,rate);
            if (emtfTrackEta[j] > 0){
              h_pT_noDxyEndCapPos->Fill(ptThreshold,rate);
            }
            else {
              h_pT_noDxyEndCapNeg->Fill(ptThreshold,rate);
            }
            if (abs(emtfTrackEta[j]) > 2.1){
              h_pT_noDxyEta3->Fill(ptThreshold,rate);
            }
            else if (abs(emtfTrackEta[j]) > 1.6){
              h_pT_noDxyEta2->Fill(ptThreshold,rate);
            }
            else if (abs(emtfTrackEta[j]) > 1.2){
              h_pT_noDxyEta1->Fill(ptThreshold,rate);
            }
          }
          if (passedMuon) {
            passedEvent = true;
            break;
          }
        } // Unpacked loop

        for (int j=0; j<*emtfTrackSizeEmu; j++){
          bool passedMuon = false;
          if(emtfTrackBXEmu[j] != 0) continue;
          // float GMTEta = emtfTrackGMTEta[j] * 0.010875;
          // if(abs(GMTEta) < 1.24 or abs(GMTEta) > 2.5) continue;
          if(abs(emtfTrackEtaEmu[j]) < 1.24 or abs(emtfTrackEtaEmu[j]) > 2.5) continue;
          // std::cout  << "after eta + BX cuts" << std::endl;

          // }
          // if (idx_EMTF < 0) continue;
          if (emtfTrackModeEmu[j] < 11 || emtfTrackModeEmu[j] == 12) continue;


          if (emtfTrackPtEmu[j] > ptThreshold){
            passedMuon = true;
            h_pT_noDxyEmu->Fill(ptThreshold,rate);
            if (emtfTrackEtaEmu[j] > 0){
              h_pT_noDxyEndCapPosEmu->Fill(ptThreshold,rate);
            }
            else {
              h_pT_noDxyEndCapNegEmu->Fill(ptThreshold,rate);
            }
            if (abs(emtfTrackEtaEmu[j]) > 2.1){
              h_pT_noDxyEta3Emu->Fill(ptThreshold,rate);
            }
            else if (abs(emtfTrackEtaEmu[j]) > 1.6){
              h_pT_noDxyEta2Emu->Fill(ptThreshold,rate);
            }
            else if (abs(emtfTrackEtaEmu[j]) > 1.2){
              h_pT_noDxyEta1Emu->Fill(ptThreshold,rate);
            }
          }
          if (passedMuon) {
            passedEvent = true;
            break;
          }
        } // EMU loop
        if (!passedEvent) break;
      } // pTThreshold loop
    } // event loop
  }


  // canvasname.push_back("out_ptUnc_vs_dxy_wME11a"); 
  TString titlePt="L1 pT threshold [GeV]";
  TString titleDxy="Reco Muon D_{0} [cm]";
  TString titleLxy="Reco Muon Lxy [cm]";
  TString titleEta="Reco Muon #eta";
  TString titlePhi="Reco Muon #phi";



  h_pT_noDxy->GetXaxis()->SetTitle(titlePt);
  h_pT_noDxy->GetYaxis()->SetTitle("a.u.");
  // h_pT_noDxy->GetXaxis()->SetRangeUser(0.4,300);
  // h_pT_noDxy->GetYaxis()->SetRangeUser(0.1,10000);
  h_pT_noDxy->GetYaxis()->SetTitleOffset(1.3);
  h_pT_noDxy->GetXaxis()->SetTitleOffset(1.3);

  h_pT_noDxyEta1->GetXaxis()->SetTitle(titlePt);
  h_pT_noDxyEta1->GetYaxis()->SetTitle("a.u.");
  // h_pT_noDxyEta1->GetXaxis()->SetRangeUser(0.4,300);
  // h_pT_noDxyEta1->GetYaxis()->SetRangeUser(0.1,10000);
  h_pT_noDxyEta1->GetYaxis()->SetTitleOffset(1.3);
  h_pT_noDxyEta1->GetXaxis()->SetTitleOffset(1.3);

  h_pT_noDxyEta2->GetXaxis()->SetTitle(titlePt);
  h_pT_noDxyEta2->GetYaxis()->SetTitle("a.u.");
  // h_pT_noDxyEta2->GetXaxis()->SetRangeUser(0.4,300);
  // h_pT_noDxyEta2->GetYaxis()->SetRangeUser(0.1,10000);
  h_pT_noDxyEta2->GetYaxis()->SetTitleOffset(1.3);
  h_pT_noDxyEta2->GetXaxis()->SetTitleOffset(1.3);

  h_pT_noDxyEta3->GetXaxis()->SetTitle(titlePt);
  h_pT_noDxyEta3->GetYaxis()->SetTitle("a.u.");
  // h_pT_noDxyEta3->GetXaxis()->SetRangeUser(0.4,300);
  // h_pT_noDxyEta3->GetYaxis()->SetRangeUser(0.1,10000);
  h_pT_noDxyEta3->GetYaxis()->SetTitleOffset(1.3);
  h_pT_noDxyEta3->GetXaxis()->SetTitleOffset(1.3);

  h_pT_noDxyEndCapNeg->GetXaxis()->SetTitle(titlePt);
  h_pT_noDxyEndCapNeg->GetYaxis()->SetTitle("a.u.");
  // h_pT_noDxyEta3->GetXaxis()->SetRangeUser(0.4,300);
  // h_pT_noDxyEta3->GetYaxis()->SetRangeUser(0.1,10000);
  h_pT_noDxyEndCapNeg->GetYaxis()->SetTitleOffset(1.3);
  h_pT_noDxyEndCapNeg->GetXaxis()->SetTitleOffset(1.3);

  h_pT_noDxyEndCapPos->GetXaxis()->SetTitle(titlePt);
  h_pT_noDxyEndCapPos->GetYaxis()->SetTitle("a.u.");
  // h_pT_noDxyEta3->GetXaxis()->SetRangeUser(0.4,300);
  // h_pT_noDxyEta3->GetYaxis()->SetRangeUser(0.1,10000);
  h_pT_noDxyEndCapPos->GetYaxis()->SetTitleOffset(1.3);
  h_pT_noDxyEndCapPos->GetXaxis()->SetTitleOffset(1.3);


  h_pT_noDxy->SetLineWidth(3);
  h_pT_noDxy->SetLineColor(1);

  h_pT_noDxyEta1->SetLineWidth(3);
  h_pT_noDxyEta1->SetLineColor(1);

  h_pT_noDxyEta2->SetLineWidth(3);
  h_pT_noDxyEta2->SetLineColor(1);

  h_pT_noDxyEta3->SetLineWidth(3);
  h_pT_noDxyEta3->SetLineColor(1);

  h_pT_noDxyEndCapNeg->SetLineWidth(3);
  h_pT_noDxyEndCapNeg->SetLineColor(1);

  h_pT_noDxyEndCapPos->SetLineWidth(3);
  h_pT_noDxyEndCapPos->SetLineColor(1);

  h_pT_noDxyEmu->SetLineWidth(3);
  h_pT_noDxyEmu->SetLineColor(2);

  h_pT_noDxyEta1Emu->SetLineWidth(3);
  h_pT_noDxyEta1Emu->SetLineColor(2);

  h_pT_noDxyEta2Emu->SetLineWidth(3);
  h_pT_noDxyEta2Emu->SetLineColor(2);

  h_pT_noDxyEta3Emu->SetLineWidth(3);
  h_pT_noDxyEta3Emu->SetLineColor(2);

  h_pT_noDxyEndCapNegEmu->SetLineWidth(3);
  h_pT_noDxyEndCapNegEmu->SetLineColor(2);

  h_pT_noDxyEndCapPosEmu->SetLineWidth(3);
  h_pT_noDxyEndCapPosEmu->SetLineColor(2);


 
  TString leg = "Unpacked";
  TString leg2 = "Re-emulated";
  // TString leg3 = "|L1 Dxy| > 50 cm";
  // TString leg4 = "|L1 Dxy| > 75 cm";


  TCanvas * c1=new TCanvas("c1","c1",1200,1200);

  TPad* pad = new TPad("","",0.3,0.1,1,1);  

  pad->SetLeftMargin(0.3);

  gPad->SetLogy(1);
  gPad->SetLogx(1);

  // TLatex header=head();  
  // header.Draw();
  // gStyle->SetErrorY(0)
  if (doSingleMuon){
    h_pT_noDxy->Draw("hist");
    h_pT_noDxyEmu->Draw("hist same");
    
    TLegend * leg11 =new TLegend(0.6,0.75,0.88,0.88);    
    leg11->AddEntry(h_pT_noDxy,leg);
    leg11->AddEntry(h_pT_noDxyEmu,leg2);
    // leg11->AddEntry(h_pT_50Dxy,leg3);
    // leg11->AddEntry(h_pT_75Dxy,leg4);
    leg11->Draw("sames");

    c1->SaveAs("./output_files/comparisons/rates/EphZB0_SingleMu_Unp_oldBDT.pdf");

    h_pT_noDxyEta1->Draw("hist");
    h_pT_noDxyEta1Emu->Draw("hist same");
    // h_pT_50DxyEta1->Draw("hist same");
    // h_pT_75DxyEta1->Draw("hist same");
    
    TLegend * leg22 =new TLegend(0.6,0.75,0.88,0.88);    
    leg22->AddEntry(h_pT_noDxyEta1,leg);
    leg22->AddEntry(h_pT_noDxyEta1Emu,leg2);
    // leg22->AddEntry(h_pT_50DxyEta1,leg3);
    // leg22->AddEntry(h_pT_75DxyEta1,leg4);
    leg22->Draw("sames");

    c1->SaveAs("./output_files/comparisons/rates/EphZB0_SingleMu_Unp_eta1_oldBDT.pdf");

    h_pT_noDxyEta2->Draw("hist");
    h_pT_noDxyEta2Emu->Draw("hist same");
    // h_pT_50DxyEta2->Draw("hist same");
    // h_pT_75DxyEta2->Draw("hist same");
    
    TLegend * leg33 =new TLegend(0.6,0.75,0.88,0.88);    
    leg33->AddEntry(h_pT_noDxyEta2,leg);
    leg33->AddEntry(h_pT_noDxyEta2Emu,leg2);
    // leg33->AddEntry(h_pT_50DxyEta2,leg3);
    // leg33->AddEntry(h_pT_75DxyEta2,leg4);
    leg33->Draw("sames");

    c1->SaveAs("./output_files/comparisons/rates/EphZB0_SingleMu_Unp_eta2_oldBDT.pdf");

    h_pT_noDxyEta3->Draw("hist");
    h_pT_noDxyEta3Emu->Draw("hist same");
    // h_pT_50DxyEta3->Draw("hist same");
    // h_pT_75DxyEta3->Draw("hist same");
    
    TLegend * leg44 =new TLegend(0.6,0.75,0.88,0.88);    
    leg44->AddEntry(h_pT_noDxyEta3,leg);
    leg44->AddEntry(h_pT_noDxyEta3Emu,leg2);
    // leg44->AddEntry(h_pT_50DxyEta3,leg3);
    // leg44->AddEntry(h_pT_75DxyEta3,leg4);
    leg44->Draw("sames");

    c1->SaveAs("./output_files/comparisons/rates/EphZB0_SingleMu_Unp_eta3_oldBDT.pdf");

    h_pT_noDxyEndCapNeg->Draw("hist");
    h_pT_noDxyEndCapNegEmu->Draw("hist same");
    // h_pT_50DxyEta3->Draw("hist same");
    // h_pT_75DxyEta3->Draw("hist same");
    
    TLegend * leg55 =new TLegend(0.6,0.75,0.88,0.88);    
    leg55->AddEntry(h_pT_noDxyEndCapNeg,leg);
    leg55->AddEntry(h_pT_noDxyEndCapNegEmu,leg2);
    // leg44->AddEntry(h_pT_50DxyEta3,leg3);
    // leg44->AddEntry(h_pT_75DxyEta3,leg4);
    leg55->Draw("sames");

    c1->SaveAs("./output_files/comparisons/rates/EphZB0_SingleMu_Unp_endcapNeg_oldBDT.pdf");

    h_pT_noDxyEndCapPos->Draw("hist");
    h_pT_noDxyEndCapPosEmu->Draw("hist same");
    // h_pT_50DxyEta3->Draw("hist same");
    // h_pT_75DxyEta3->Draw("hist same");
    
    TLegend * leg66 =new TLegend(0.6,0.75,0.88,0.88);    
    leg66->AddEntry(h_pT_noDxyEndCapPos,leg);
    leg66->AddEntry(h_pT_noDxyEndCapPosEmu,leg2);
    // leg44->AddEntry(h_pT_50DxyEta3,leg3);
    // leg44->AddEntry(h_pT_75DxyEta3,leg4);
    leg66->Draw("sames");

    c1->SaveAs("./output_files/comparisons/rates/EphZB0_SingleMu_Unp_endcapPos_oldBDT.pdf");
  }



  return 0;
 }
