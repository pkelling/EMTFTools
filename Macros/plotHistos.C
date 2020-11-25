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

int plotHistos(){

  TString file1 = "output_files/comparisons/truth/NNv4/NNv4TruthCompPlots.root";
  TString file2 = "output_files/comparisons/truth/NNv5/NNv5TruthCompPlots.root";

  TFile* f1 = new TFile(file1);
  TFile* f2 = new TFile(file2);


  TCanvas* canv1 = new TCanvas("c_str", "c_str", 1200, 1200);

  TPad* pad = new TPad("","",0,0,1,1);  

  pad->SetLeftMargin(0.2);
  pad->SetBottomMargin(0.1);
  // pad->SetRightMargin(0);
  // pad->SetLeftMargin(0.2);
  pad->cd();


  Double_t etaArray[8] = {-2.5, -2.1, -1.6, -1.2, 1.2, 1.6, 2.1, 2.5};

  
  TH1F* h1_dPt = (TH1F*)f1->Get("dPt");
  TH1F* h1_dPtEta1 = (TH1F*)f1->Get("dPtEta1");
  TH1F* h1_dPtEta2 = (TH1F*)f1->Get("dPtEta2");
  TH1F* h1_dPtEta3 = (TH1F*)f1->Get("dPtEta3");

  TH1F* h1_dPtOverPt = (TH1F*)f1->Get("dPtOverPt");
  TH1F* h1_dPtOverPtEta1 = (TH1F*)f1->Get("dPtOverPtEta1");
  TH1F* h1_dPtOverPtEta2 = (TH1F*)f1->Get("dPtOverPtEta2");
  TH1F* h1_dPtOverPtEta3 = (TH1F*)f1->Get("dPtOverPtEta3");


  TH1F* h2_dPt = (TH1F*)f2->Get("dPt");
  TH1F* h2_dPtEta1 = (TH1F*)f2->Get("dPtEta1");
  TH1F* h2_dPtEta2 = (TH1F*)f2->Get("dPtEta2");
  TH1F* h2_dPtEta3 = (TH1F*)f2->Get("dPtEta3");

  TH1F* h2_dPtOverPt = (TH1F*)f2->Get("dPtOverPt");
  TH1F* h2_dPtOverPtEta1 = (TH1F*)f2->Get("dPtOverPtEta1");
  TH1F* h2_dPtOverPtEta2 = (TH1F*)f2->Get("dPtOverPtEta2");
  TH1F* h2_dPtOverPtEta3 = (TH1F*)f2->Get("dPtOverPtEta3");

  TH2D* h1_ptVsdPt = (TH2D*)f1->Get("ptVsdPt");
  TH2D* h1_ptVsdPtEta1 = (TH2D*)f1->Get("ptVsdPtEta1");
  TH2D* h1_ptVsdPtEta2 = (TH2D*)f1->Get("ptVsdPtEta2");
  TH2D* h1_ptVsdPtEta3 = (TH2D*)f1->Get("ptVsdPtEta3");

  TH2D* h2_ptVsdPt = (TH2D*)f2->Get("ptVsdPt");
  TH2D* h2_ptVsdPtEta1 = (TH2D*)f2->Get("ptVsdPtEta1");
  TH2D* h2_ptVsdPtEta2 = (TH2D*)f2->Get("ptVsdPtEta2");
  TH2D* h2_ptVsdPtEta3 = (TH2D*)f2->Get("ptVsdPtEta3");

  TH2D* h1_ptVsdPtOverPt = (TH2D*)f1->Get("ptVsdPtOverPt");
  TH2D* h1_ptVsdPtOverPtEta1 = (TH2D*)f1->Get("ptVsdPtOverPtEta1");
  TH2D* h1_ptVsdPtOverPtEta2 = (TH2D*)f1->Get("ptVsdPtOverPtEta2");
  TH2D* h1_ptVsdPtOverPtEta3 = (TH2D*)f1->Get("ptVsdPtOverPtEta3");

  TH2D* h2_ptVsdPtOverPt = (TH2D*)f2->Get("ptVsdPtOverPt");
  TH2D* h2_ptVsdPtOverPtEta1 = (TH2D*)f2->Get("ptVsdPtOverPtEta1");
  TH2D* h2_ptVsdPtOverPtEta2 = (TH2D*)f2->Get("ptVsdPtOverPtEta2");
  TH2D* h2_ptVsdPtOverPtEta3 = (TH2D*)f2->Get("ptVsdPtOverPtEta3");


  TH1D* h1_ptVsdPt_0_10 = h1_ptVsdPt->ProjectionY("h1_ptVsdPt_0_10",0, 2);
  TH1D* h1_ptVsdPt_10_20 = h1_ptVsdPt->ProjectionY("h1_ptVsdPt_10_20",2, 4);
  TH1D* h1_ptVsdPt_20_30 = h1_ptVsdPt->ProjectionY("h1_ptVsdPt_20_30",4, 6);
  TH1D* h1_ptVsdPt_30_60 = h1_ptVsdPt->ProjectionY("h1_ptVsdPt_30_60",6, 12);

  TH1D* h1_ptVsdPtEta1_0_10 = h1_ptVsdPtEta1->ProjectionY("h1_ptVsdPtEta1_0_10",0, 2);
  TH1D* h1_ptVsdPtEta1_10_20 = h1_ptVsdPtEta1->ProjectionY("h1_ptVsdPtEta1_10_20",2, 4);
  TH1D* h1_ptVsdPtEta1_20_30 = h1_ptVsdPtEta1->ProjectionY("h1_ptVsdPtEta1_20_30",4, 6);
  TH1D* h1_ptVsdPtEta1_30_60 = h1_ptVsdPtEta1->ProjectionY("h1_ptVsdPtEta1_30_60",6, 12);

  TH1D* h1_ptVsdPtEta2_0_10 = h1_ptVsdPtEta2->ProjectionY("h1_ptVsdPtEta2_0_10",0, 2);
  TH1D* h1_ptVsdPtEta2_10_20 = h1_ptVsdPtEta2->ProjectionY("h1_ptVsdPtEta2_10_20",2, 4);
  TH1D* h1_ptVsdPtEta2_20_30 = h1_ptVsdPtEta2->ProjectionY("h1_ptVsdPtEta2_20_30",4, 6);
  TH1D* h1_ptVsdPtEta2_30_60 = h1_ptVsdPtEta2->ProjectionY("h1_ptVsdPtEta2_30_60",6, 12);

  TH1D* h1_ptVsdPtEta3_0_10 = h1_ptVsdPtEta3->ProjectionY("h1_ptVsdPtEta3_0_10",0, 2);
  TH1D* h1_ptVsdPtEta3_10_20 = h1_ptVsdPtEta3->ProjectionY("h1_ptVsdPtEta3_10_20",2, 4);
  TH1D* h1_ptVsdPtEta3_20_30 = h1_ptVsdPtEta3->ProjectionY("h1_ptVsdPtEta3_20_30",4, 6);
  TH1D* h1_ptVsdPtEta3_30_60 = h1_ptVsdPtEta3->ProjectionY("h1_ptVsdPtEta3_30_60",6, 12);

  TH1D* h2_ptVsdPt_0_10 = h2_ptVsdPt->ProjectionY("h2_ptVsdPt_0_10",0, 2);
  TH1D* h2_ptVsdPt_10_20 = h2_ptVsdPt->ProjectionY("h2_ptVsdPt_10_20",2, 4);
  TH1D* h2_ptVsdPt_20_30 = h2_ptVsdPt->ProjectionY("h2_ptVsdPt_20_30",4, 6);
  TH1D* h2_ptVsdPt_30_60 = h2_ptVsdPt->ProjectionY("h2_ptVsdPt_30_60",6, 12);

  TH1D* h2_ptVsdPtEta1_0_10 = h2_ptVsdPtEta1->ProjectionY("h2_ptVsdPtEta1_0_10",0, 2);
  TH1D* h2_ptVsdPtEta1_10_20 = h2_ptVsdPtEta1->ProjectionY("h2_ptVsdPtEta1_10_20",2, 4);
  TH1D* h2_ptVsdPtEta1_20_30 = h2_ptVsdPtEta1->ProjectionY("h2_ptVsdPtEta1_20_30",4, 6);
  TH1D* h2_ptVsdPtEta1_30_60 = h2_ptVsdPtEta1->ProjectionY("h2_ptVsdPtEta1_30_60",6, 12);

  TH1D* h2_ptVsdPtEta2_0_10 = h2_ptVsdPtEta2->ProjectionY("h2_ptVsdPtEta2_0_10",0, 2);
  TH1D* h2_ptVsdPtEta2_10_20 = h2_ptVsdPtEta2->ProjectionY("h2_ptVsdPtEta2_10_20",2, 4);
  TH1D* h2_ptVsdPtEta2_20_30 = h2_ptVsdPtEta2->ProjectionY("h2_ptVsdPtEta2_20_30",4, 6);
  TH1D* h2_ptVsdPtEta2_30_60 = h2_ptVsdPtEta2->ProjectionY("h2_ptVsdPtEta2_30_60",6, 12);

  TH1D* h2_ptVsdPtEta3_0_10 = h2_ptVsdPtEta3->ProjectionY("h2_ptVsdPtEta3_0_10",0, 2);
  TH1D* h2_ptVsdPtEta3_10_20 = h2_ptVsdPtEta3->ProjectionY("h2_ptVsdPtEta3_10_20",2, 4);
  TH1D* h2_ptVsdPtEta3_20_30 = h2_ptVsdPtEta3->ProjectionY("h2_ptVsdPtEta3_20_30",4, 6);
  TH1D* h2_ptVsdPtEta3_30_60 = h2_ptVsdPtEta3->ProjectionY("h2_ptVsdPtEta3_30_60",6, 12);


  TH1D* h1_ptVsdPtOverPt_0_10 = h1_ptVsdPtOverPt->ProjectionY("h1_ptVsdPtOverPt_0_10",0, 2);
  TH1D* h1_ptVsdPtOverPt_10_20 = h1_ptVsdPtOverPt->ProjectionY("h1_ptVsdPtOverPt_10_20",2, 4);
  TH1D* h1_ptVsdPtOverPt_20_30 = h1_ptVsdPtOverPt->ProjectionY("h1_ptVsdPtOverPt_20_30",4, 6);
  TH1D* h1_ptVsdPtOverPt_30_60 = h1_ptVsdPtOverPt->ProjectionY("h1_ptVsdPtOverPt_30_60",6, 12);

  TH1D* h1_ptVsdPtOverPtEta1_0_10 = h1_ptVsdPtOverPtEta1->ProjectionY("h1_ptVsdPtOverPtEta1_0_10",0, 2);
  TH1D* h1_ptVsdPtOverPtEta1_10_20 = h1_ptVsdPtOverPtEta1->ProjectionY("h1_ptVsdPtOverPtEta1_10_20",2, 4);
  TH1D* h1_ptVsdPtOverPtEta1_20_30 = h1_ptVsdPtOverPtEta1->ProjectionY("h1_ptVsdPtOverPtEta1_20_30",4, 6);
  TH1D* h1_ptVsdPtOverPtEta1_30_60 = h1_ptVsdPtOverPtEta1->ProjectionY("h1_ptVsdPtOverPtEta1_30_60",6, 12);

  TH1D* h1_ptVsdPtOverPtEta2_0_10 = h1_ptVsdPtOverPtEta2->ProjectionY("h1_ptVsdPtOverPtEta2_0_10",0, 2);
  TH1D* h1_ptVsdPtOverPtEta2_10_20 = h1_ptVsdPtOverPtEta2->ProjectionY("h1_ptVsdPtOverPtEta2_10_20",2, 4);
  TH1D* h1_ptVsdPtOverPtEta2_20_30 = h1_ptVsdPtOverPtEta2->ProjectionY("h1_ptVsdPtOverPtEta2_20_30",4, 6);
  TH1D* h1_ptVsdPtOverPtEta2_30_60 = h1_ptVsdPtOverPtEta2->ProjectionY("h1_ptVsdPtOverPtEta2_30_60",6, 12);

  TH1D* h1_ptVsdPtOverPtEta3_0_10 = h1_ptVsdPtOverPtEta3->ProjectionY("h1_ptVsdPtOverPtEta3_0_10",0, 2);
  TH1D* h1_ptVsdPtOverPtEta3_10_20 = h1_ptVsdPtOverPtEta3->ProjectionY("h1_ptVsdPtOverPtEta3_10_20",2, 4);
  TH1D* h1_ptVsdPtOverPtEta3_20_30 = h1_ptVsdPtOverPtEta3->ProjectionY("h1_ptVsdPtOverPtEta3_20_30",4, 6);
  TH1D* h1_ptVsdPtOverPtEta3_30_60 = h1_ptVsdPtOverPtEta3->ProjectionY("h1_ptVsdPtOverPtEta3_30_60",6, 12);

  TH1D* h2_ptVsdPtOverPt_0_10 = h2_ptVsdPtOverPt->ProjectionY("h2_ptVsdPtOverPt_0_10",0, 2);
  TH1D* h2_ptVsdPtOverPt_10_20 = h2_ptVsdPtOverPt->ProjectionY("h2_ptVsdPtOverPt_10_20",2, 4);
  TH1D* h2_ptVsdPtOverPt_20_30 = h2_ptVsdPtOverPt->ProjectionY("h2_ptVsdPtOverPt_20_30",4, 6);
  TH1D* h2_ptVsdPtOverPt_30_60 = h2_ptVsdPtOverPt->ProjectionY("h2_ptVsdPtOverPt_30_60",6, 12);

  TH1D* h2_ptVsdPtOverPtEta1_0_10 = h2_ptVsdPtOverPtEta1->ProjectionY("h2_ptVsdPtOverPtEta1_0_10",0, 2);
  TH1D* h2_ptVsdPtOverPtEta1_10_20 = h2_ptVsdPtOverPtEta1->ProjectionY("h2_ptVsdPtOverPtEta1_10_20",2, 4);
  TH1D* h2_ptVsdPtOverPtEta1_20_30 = h2_ptVsdPtOverPtEta1->ProjectionY("h2_ptVsdPtOverPtEta1_20_30",4, 6);
  TH1D* h2_ptVsdPtOverPtEta1_30_60 = h2_ptVsdPtOverPtEta1->ProjectionY("h2_ptVsdPtOverPtEta1_30_60",6, 12);

  TH1D* h2_ptVsdPtOverPtEta2_0_10 = h2_ptVsdPtOverPtEta2->ProjectionY("h2_ptVsdPtOverPtEta2_0_10",0, 2);
  TH1D* h2_ptVsdPtOverPtEta2_10_20 = h2_ptVsdPtOverPtEta2->ProjectionY("h2_ptVsdPtOverPtEta2_10_20",2, 4);
  TH1D* h2_ptVsdPtOverPtEta2_20_30 = h2_ptVsdPtOverPtEta2->ProjectionY("h2_ptVsdPtOverPtEta2_20_30",4, 6);
  TH1D* h2_ptVsdPtOverPtEta2_30_60 = h2_ptVsdPtOverPtEta2->ProjectionY("h2_ptVsdPtOverPtEta2_30_60",6, 12);

  TH1D* h2_ptVsdPtOverPtEta3_0_10 = h2_ptVsdPtOverPtEta3->ProjectionY("h2_ptVsdPtOverPtEta3_0_10",0, 2);
  TH1D* h2_ptVsdPtOverPtEta3_10_20 = h2_ptVsdPtOverPtEta3->ProjectionY("h2_ptVsdPtOverPtEta3_10_20",2, 4);
  TH1D* h2_ptVsdPtOverPtEta3_20_30 = h2_ptVsdPtOverPtEta3->ProjectionY("h2_ptVsdPtOverPtEta3_20_30",4, 6);
  TH1D* h2_ptVsdPtOverPtEta3_30_60 = h2_ptVsdPtOverPtEta3->ProjectionY("h2_ptVsdPtOverPtEta3_30_60",6, 12);


  
  TString incEta = "Inclusive #eta";
  TString eta1 = "1.2 < |#eta| < 1.6";
  TString eta2 = "1.6 < |#eta| < 2.1";
  TString eta3 = "2.1 < |#eta| < 2.5";

  TString incPt = "Inclusive p_{T}";
  TString Pt1 = "p_{T} < 10 GeV";
  TString Pt2 = "10 GeV < p_{T} < 20 GeV";
  TString Pt3 = "20 GeV < p_{T} < 30 GeV";
  TString Pt4 = "30 GeV < p_{T} < 60 GeV";


  h1_dPt->SetLineWidth(2);
  h1_dPt->SetLineColor(1);
  h1_dPt->SetTitle("#Delta p_{T}");

  h2_dPt->SetLineWidth(2);
  h2_dPt->SetLineColor(2);

  h1_dPtEta1->SetLineWidth(2);
  h1_dPtEta1->SetLineColor(1);
  h1_dPtEta1->SetTitle("#Delta p_{T}");

  h2_dPtEta1->SetLineWidth(2);
  h2_dPtEta1->SetLineColor(2);

  h1_dPtEta2->SetLineWidth(2);
  h1_dPtEta2->SetLineColor(1);
  h1_dPtEta2->SetTitle("#Delta p_{T}");

  h2_dPtEta2->SetLineWidth(2);
  h2_dPtEta2->SetLineColor(2);

  h1_dPtEta3->SetLineWidth(2);
  h1_dPtEta3->SetLineColor(1);
  h1_dPtEta3->SetTitle("#Delta p_{T}");

  h2_dPtEta3->SetLineWidth(2);
  h2_dPtEta3->SetLineColor(2);

  h1_dPtOverPt->SetLineWidth(2);
  h1_dPtOverPt->SetLineColor(1);
  h1_dPtOverPt->SetMaximum(h2_dPtOverPt->GetMaximum()*1.1);
  h1_dPtOverPt->SetTitle("#Delta p_{T}/p_{T}");

  h2_dPtOverPt->SetLineWidth(2);
  h2_dPtOverPt->SetLineColor(2);

  h1_dPtOverPtEta1->SetLineWidth(2);
  h1_dPtOverPtEta1->SetLineColor(1);
  h1_dPtOverPtEta1->SetMaximum(h2_dPtOverPtEta1->GetMaximum()*1.1);
  h1_dPtOverPtEta1->SetTitle("#Delta p_{T}/p_{T}");

  h2_dPtOverPtEta1->SetLineWidth(2);
  h2_dPtOverPtEta1->SetLineColor(2);

  h1_dPtOverPtEta2->SetLineWidth(2);
  h1_dPtOverPtEta2->SetLineColor(1);
  h1_dPtOverPtEta2->SetMaximum(h2_dPtOverPtEta2->GetMaximum()*1.1);
  h1_dPtOverPtEta2->SetTitle("#Delta p_{T}/p_{T}");

  h2_dPtOverPtEta2->SetLineWidth(2);
  h2_dPtOverPtEta2->SetLineColor(2);

  h1_dPtOverPtEta3->SetLineWidth(2);
  h1_dPtOverPtEta3->SetLineColor(1);
  h1_dPtOverPtEta3->SetMaximum(h2_dPtOverPtEta3->GetMaximum()*1.1);
  h1_dPtOverPtEta3->SetTitle("#Delta p_{T}/p_{T}");

  h2_dPtOverPtEta3->SetLineWidth(2);
  h2_dPtOverPtEta3->SetLineColor(2);

  // 0 - 10
  h1_ptVsdPt_0_10->SetLineWidth(2);
  h1_ptVsdPt_0_10->SetLineColor(1);
  h1_ptVsdPt_0_10->SetMaximum(h2_ptVsdPt_0_10->GetMaximum()*1.1);
  h1_ptVsdPt_0_10->SetTitle("#Delta p_{T}");
  h1_ptVsdPt_0_10->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtEta1_0_10->SetLineWidth(2);
  h1_ptVsdPtEta1_0_10->SetLineColor(1);
  h1_ptVsdPtEta1_0_10->SetMaximum(h2_ptVsdPtEta1_0_10->GetMaximum()*1.1);
  h1_ptVsdPtEta1_0_10->SetTitle("#Delta p_{T}");
  h1_ptVsdPtEta1_0_10->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtEta2_0_10->SetLineWidth(2);
  h1_ptVsdPtEta2_0_10->SetLineColor(1);
  h1_ptVsdPtEta2_0_10->SetMaximum(h2_ptVsdPtEta2_0_10->GetMaximum()*1.1);
  h1_ptVsdPtEta2_0_10->SetTitle("#Delta p_{T}");
  h1_ptVsdPtEta2_0_10->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtEta3_0_10->SetLineWidth(2);
  h1_ptVsdPtEta3_0_10->SetLineColor(1);
  h1_ptVsdPtEta3_0_10->SetMaximum(h2_ptVsdPtEta3_0_10->GetMaximum()*1.1);
  h1_ptVsdPtEta3_0_10->SetTitle("#Delta p_{T}");
  h1_ptVsdPtEta3_0_10->GetYaxis()->SetTitle("# of muons / bin");

  h2_ptVsdPt_0_10->SetLineWidth(2);
  h2_ptVsdPt_0_10->SetLineColor(2);

  h2_ptVsdPtEta1_0_10->SetLineWidth(2);
  h2_ptVsdPtEta1_0_10->SetLineColor(2);

  h2_ptVsdPtEta2_0_10->SetLineWidth(2);
  h2_ptVsdPtEta2_0_10->SetLineColor(2);

  h2_ptVsdPtEta3_0_10->SetLineWidth(2);
  h2_ptVsdPtEta3_0_10->SetLineColor(2);

  
  // 10 - 20
  h1_ptVsdPt_10_20->SetLineWidth(2);
  h1_ptVsdPt_10_20->SetLineColor(1);
  h1_ptVsdPt_10_20->SetMaximum(h2_ptVsdPt_10_20->GetMaximum()*1.1);
  h1_ptVsdPt_10_20->SetTitle("#Delta p_{T}");
  h1_ptVsdPt_10_20->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtEta1_10_20->SetLineWidth(2);
  h1_ptVsdPtEta1_10_20->SetLineColor(1);
  h1_ptVsdPtEta1_10_20->SetMaximum(h2_ptVsdPtEta1_10_20->GetMaximum()*1.1);
  h1_ptVsdPtEta1_10_20->SetTitle("#Delta p_{T}");
  h1_ptVsdPtEta1_10_20->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtEta2_10_20->SetLineWidth(2);
  h1_ptVsdPtEta2_10_20->SetLineColor(1);
  h1_ptVsdPtEta2_10_20->SetMaximum(h2_ptVsdPtEta2_10_20->GetMaximum()*1.1);
  h1_ptVsdPtEta2_10_20->SetTitle("#Delta p_{T}");
  h1_ptVsdPtEta2_10_20->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtEta3_10_20->SetLineWidth(2);
  h1_ptVsdPtEta3_10_20->SetLineColor(1);
  h1_ptVsdPtEta3_10_20->SetMaximum(h2_ptVsdPtEta3_10_20->GetMaximum()*1.1);
  h1_ptVsdPtEta3_10_20->SetTitle("#Delta p_{T}");
  h1_ptVsdPtEta3_10_20->GetYaxis()->SetTitle("# of muons / bin");

  h2_ptVsdPt_10_20->SetLineWidth(2);
  h2_ptVsdPt_10_20->SetLineColor(2);

  h2_ptVsdPtEta1_10_20->SetLineWidth(2);
  h2_ptVsdPtEta1_10_20->SetLineColor(2);

  h2_ptVsdPtEta2_10_20->SetLineWidth(2);
  h2_ptVsdPtEta2_10_20->SetLineColor(2);

  h2_ptVsdPtEta3_10_20->SetLineWidth(2);
  h2_ptVsdPtEta3_10_20->SetLineColor(2);


  // 20 - 30
  h1_ptVsdPt_20_30->SetLineWidth(2);
  h1_ptVsdPt_20_30->SetLineColor(1);
  h1_ptVsdPt_20_30->SetMaximum(h2_ptVsdPt_20_30->GetMaximum()*1.1);
  h1_ptVsdPt_20_30->SetTitle("#Delta p_{T}");
  h1_ptVsdPt_20_30->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtEta1_20_30->SetLineWidth(2);
  h1_ptVsdPtEta1_20_30->SetLineColor(1);
  h1_ptVsdPtEta1_20_30->SetMaximum(h2_ptVsdPtEta1_20_30->GetMaximum()*1.1);
  h1_ptVsdPtEta1_20_30->SetTitle("#Delta p_{T}");
  h1_ptVsdPtEta1_20_30->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtEta2_20_30->SetLineWidth(2);
  h1_ptVsdPtEta2_20_30->SetLineColor(1);
  h1_ptVsdPtEta2_20_30->SetMaximum(h2_ptVsdPtEta2_20_30->GetMaximum()*1.1);
  h1_ptVsdPtEta2_20_30->SetTitle("#Delta p_{T}");
  h1_ptVsdPtEta2_20_30->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtEta3_20_30->SetLineWidth(2);
  h1_ptVsdPtEta3_20_30->SetLineColor(1);
  h1_ptVsdPtEta3_20_30->SetMaximum(h2_ptVsdPtEta3_20_30->GetMaximum()*1.1);
  h1_ptVsdPtEta3_20_30->SetTitle("#Delta p_{T}");
  h1_ptVsdPtEta3_20_30->GetYaxis()->SetTitle("# of muons / bin");

  h2_ptVsdPt_20_30->SetLineWidth(2);
  h2_ptVsdPt_20_30->SetLineColor(2);

  h2_ptVsdPtEta1_20_30->SetLineWidth(2);
  h2_ptVsdPtEta1_20_30->SetLineColor(2);

  h2_ptVsdPtEta2_20_30->SetLineWidth(2);
  h2_ptVsdPtEta2_20_30->SetLineColor(2);

  h2_ptVsdPtEta3_20_30->SetLineWidth(2);
  h2_ptVsdPtEta3_20_30->SetLineColor(2);

  // 30 - 60
  h1_ptVsdPt_30_60->SetLineWidth(2);
  h1_ptVsdPt_30_60->SetLineColor(1);
  h1_ptVsdPt_30_60->SetMaximum(h2_ptVsdPt_30_60->GetMaximum()*1.1);
  h1_ptVsdPt_30_60->SetTitle("#Delta p_{T}");
  h1_ptVsdPt_30_60->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtEta1_30_60->SetLineWidth(2);
  h1_ptVsdPtEta1_30_60->SetLineColor(1);
  h1_ptVsdPtEta1_30_60->SetMaximum(h2_ptVsdPtEta1_30_60->GetMaximum()*1.1);
  h1_ptVsdPtEta1_30_60->SetTitle("#Delta p_{T}");
  h1_ptVsdPtEta1_30_60->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtEta2_30_60->SetLineWidth(2);
  h1_ptVsdPtEta2_30_60->SetLineColor(1);
  h1_ptVsdPtEta2_30_60->SetMaximum(h2_ptVsdPtEta2_30_60->GetMaximum()*1.1);
  h1_ptVsdPtEta2_30_60->SetTitle("#Delta p_{T}");
  h1_ptVsdPtEta2_30_60->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtEta3_30_60->SetLineWidth(2);
  h1_ptVsdPtEta3_30_60->SetLineColor(1);
  h1_ptVsdPtEta3_30_60->SetMaximum(h2_ptVsdPtEta3_30_60->GetMaximum()*1.1);
  h1_ptVsdPtEta3_30_60->SetTitle("#Delta p_{T}");
  h1_ptVsdPtEta3_30_60->GetYaxis()->SetTitle("# of muons / bin");

  h2_ptVsdPt_30_60->SetLineWidth(2);
  h2_ptVsdPt_30_60->SetLineColor(2);

  h2_ptVsdPtEta1_30_60->SetLineWidth(2);
  h2_ptVsdPtEta1_30_60->SetLineColor(2);

  h2_ptVsdPtEta2_30_60->SetLineWidth(2);
  h2_ptVsdPtEta2_30_60->SetLineColor(2);

  h2_ptVsdPtEta3_30_60->SetLineWidth(2);
  h2_ptVsdPtEta3_30_60->SetLineColor(2);


  // 0 - 10
  h1_ptVsdPtOverPt_0_10->SetLineWidth(2);
  h1_ptVsdPtOverPt_0_10->SetLineColor(1);
  h1_ptVsdPtOverPt_0_10->SetMaximum(h2_ptVsdPtOverPt_0_10->GetMaximum()*1.1);
  h1_ptVsdPtOverPt_0_10->SetTitle("#Delta p_{T}/p_{T}");
  h1_ptVsdPtOverPt_0_10->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtOverPtEta1_0_10->SetLineWidth(2);
  h1_ptVsdPtOverPtEta1_0_10->SetLineColor(1);
  h1_ptVsdPtOverPtEta1_0_10->SetMaximum(h2_ptVsdPtOverPtEta1_0_10->GetMaximum()*1.1);
  h1_ptVsdPtOverPtEta1_0_10->SetTitle("#Delta p_{T}/p_{T}");
  h1_ptVsdPtOverPtEta1_0_10->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtOverPtEta2_0_10->SetLineWidth(2);
  h1_ptVsdPtOverPtEta2_0_10->SetLineColor(1);
  h1_ptVsdPtOverPtEta2_0_10->SetMaximum(h2_ptVsdPtOverPtEta2_0_10->GetMaximum()*1.1);
  h1_ptVsdPtOverPtEta2_0_10->SetTitle("#Delta p_{T}/p_{T}");
  h1_ptVsdPtOverPtEta2_0_10->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtOverPtEta3_0_10->SetLineWidth(2);
  h1_ptVsdPtOverPtEta3_0_10->SetLineColor(1);
  h1_ptVsdPtOverPtEta3_0_10->SetMaximum(h2_ptVsdPtOverPtEta3_0_10->GetMaximum()*1.1);
  h1_ptVsdPtOverPtEta3_0_10->SetTitle("#Delta p_{T}/p_{T}");
  h1_ptVsdPtOverPtEta3_0_10->GetYaxis()->SetTitle("# of muons / bin");

  h2_ptVsdPtOverPt_0_10->SetLineWidth(2);
  h2_ptVsdPtOverPt_0_10->SetLineColor(2);

  h2_ptVsdPtOverPtEta1_0_10->SetLineWidth(2);
  h2_ptVsdPtOverPtEta1_0_10->SetLineColor(2);

  h2_ptVsdPtOverPtEta2_0_10->SetLineWidth(2);
  h2_ptVsdPtOverPtEta2_0_10->SetLineColor(2);

  h2_ptVsdPtOverPtEta3_0_10->SetLineWidth(2);
  h2_ptVsdPtOverPtEta3_0_10->SetLineColor(2);

  
  // 10 - 20
  h1_ptVsdPtOverPt_10_20->SetLineWidth(2);
  h1_ptVsdPtOverPt_10_20->SetLineColor(1);
  h1_ptVsdPtOverPt_10_20->SetMaximum(h2_ptVsdPtOverPt_10_20->GetMaximum()*1.1);
  h1_ptVsdPtOverPt_10_20->SetTitle("#Delta p_{T}/p_{T}");
  h1_ptVsdPtOverPt_10_20->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtOverPtEta1_10_20->SetLineWidth(2);
  h1_ptVsdPtOverPtEta1_10_20->SetLineColor(1);
  h1_ptVsdPtOverPtEta1_10_20->SetMaximum(h2_ptVsdPtOverPtEta1_10_20->GetMaximum()*1.1);
  h1_ptVsdPtOverPtEta1_10_20->SetTitle("#Delta p_{T}/p_{T}");
  h1_ptVsdPtOverPtEta1_10_20->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtOverPtEta2_10_20->SetLineWidth(2);
  h1_ptVsdPtOverPtEta2_10_20->SetLineColor(1);
  h1_ptVsdPtOverPtEta2_10_20->SetMaximum(h2_ptVsdPtOverPtEta2_10_20->GetMaximum()*1.1);
  h1_ptVsdPtOverPtEta2_10_20->SetTitle("#Delta p_{T}/p_{T}");
  h1_ptVsdPtOverPtEta2_10_20->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtOverPtEta3_10_20->SetLineWidth(2);
  h1_ptVsdPtOverPtEta3_10_20->SetLineColor(1);
  h1_ptVsdPtOverPtEta3_10_20->SetMaximum(h2_ptVsdPtOverPtEta3_10_20->GetMaximum()*1.1);
  h1_ptVsdPtOverPtEta3_10_20->SetTitle("#Delta p_{T}/p_{T}");
  h1_ptVsdPtOverPtEta3_10_20->GetYaxis()->SetTitle("# of muons / bin");

  h2_ptVsdPtOverPt_10_20->SetLineWidth(2);
  h2_ptVsdPtOverPt_10_20->SetLineColor(2);

  h2_ptVsdPtOverPtEta1_10_20->SetLineWidth(2);
  h2_ptVsdPtOverPtEta1_10_20->SetLineColor(2);

  h2_ptVsdPtOverPtEta2_10_20->SetLineWidth(2);
  h2_ptVsdPtOverPtEta2_10_20->SetLineColor(2);

  h2_ptVsdPtOverPtEta3_10_20->SetLineWidth(2);
  h2_ptVsdPtOverPtEta3_10_20->SetLineColor(2);


  // 20 - 30
  h1_ptVsdPtOverPt_20_30->SetLineWidth(2);
  h1_ptVsdPtOverPt_20_30->SetLineColor(1);
  h1_ptVsdPtOverPt_20_30->SetMaximum(h2_ptVsdPtOverPt_20_30->GetMaximum()*1.1);
  h1_ptVsdPtOverPt_20_30->SetTitle("#Delta p_{T}/p_{T}");
  h1_ptVsdPtOverPt_20_30->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtOverPtEta1_20_30->SetLineWidth(2);
  h1_ptVsdPtOverPtEta1_20_30->SetLineColor(1);
  h1_ptVsdPtOverPtEta1_20_30->SetMaximum(h2_ptVsdPtOverPtEta1_20_30->GetMaximum()*1.1);
  h1_ptVsdPtOverPtEta1_20_30->SetTitle("#Delta p_{T}/p_{T}");
  h1_ptVsdPtOverPtEta1_20_30->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtOverPtEta2_20_30->SetLineWidth(2);
  h1_ptVsdPtOverPtEta2_20_30->SetLineColor(1);
  h1_ptVsdPtOverPtEta2_20_30->SetMaximum(h2_ptVsdPtOverPtEta2_20_30->GetMaximum()*1.1);
  h1_ptVsdPtOverPtEta2_20_30->SetTitle("#Delta p_{T}/p_{T}");
  h1_ptVsdPtOverPtEta2_20_30->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtOverPtEta3_20_30->SetLineWidth(2);
  h1_ptVsdPtOverPtEta3_20_30->SetLineColor(1);
  h1_ptVsdPtOverPtEta3_20_30->SetMaximum(h2_ptVsdPtOverPtEta3_20_30->GetMaximum()*1.1);
  h1_ptVsdPtOverPtEta3_20_30->SetTitle("#Delta p_{T}/p_{T}");
  h1_ptVsdPtOverPtEta3_20_30->GetYaxis()->SetTitle("# of muons / bin");

  h2_ptVsdPtOverPt_20_30->SetLineWidth(2);
  h2_ptVsdPtOverPt_20_30->SetLineColor(2);

  h2_ptVsdPtOverPtEta1_20_30->SetLineWidth(2);
  h2_ptVsdPtOverPtEta1_20_30->SetLineColor(2);

  h2_ptVsdPtOverPtEta2_20_30->SetLineWidth(2);
  h2_ptVsdPtOverPtEta2_20_30->SetLineColor(2);

  h2_ptVsdPtOverPtEta3_20_30->SetLineWidth(2);
  h2_ptVsdPtOverPtEta3_20_30->SetLineColor(2);

  // 30 - 60
  h1_ptVsdPtOverPt_30_60->SetLineWidth(2);
  h1_ptVsdPtOverPt_30_60->SetLineColor(1);
  h1_ptVsdPtOverPt_30_60->SetMaximum(h2_ptVsdPtOverPt_30_60->GetMaximum()*1.1);
  h1_ptVsdPtOverPt_30_60->SetTitle("#Delta p_{T}/p_{T}");
  h1_ptVsdPtOverPt_30_60->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtOverPtEta1_30_60->SetLineWidth(2);
  h1_ptVsdPtOverPtEta1_30_60->SetLineColor(1);
  h1_ptVsdPtOverPtEta1_30_60->SetMaximum(h2_ptVsdPtOverPtEta1_30_60->GetMaximum()*1.1);
  h1_ptVsdPtOverPtEta1_30_60->SetTitle("#Delta p_{T}/p_{T}");
  h1_ptVsdPtOverPtEta1_30_60->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtOverPtEta2_30_60->SetLineWidth(2);
  h1_ptVsdPtOverPtEta2_30_60->SetLineColor(1);
  h1_ptVsdPtOverPtEta2_30_60->SetMaximum(h2_ptVsdPtOverPtEta2_30_60->GetMaximum()*1.1);
  h1_ptVsdPtOverPtEta2_30_60->SetTitle("#Delta p_{T}/p_{T}");
  h1_ptVsdPtOverPtEta2_30_60->GetYaxis()->SetTitle("# of muons / bin");

  h1_ptVsdPtOverPtEta3_30_60->SetLineWidth(2);
  h1_ptVsdPtOverPtEta3_30_60->SetLineColor(1);
  h1_ptVsdPtOverPtEta3_30_60->SetMaximum(h2_ptVsdPtOverPtEta3_30_60->GetMaximum()*1.1);
  h1_ptVsdPtOverPtEta3_30_60->SetTitle("#Delta p_{T}/p_{T}");
  h1_ptVsdPtOverPtEta3_30_60->GetYaxis()->SetTitle("# of muons / bin");

  h2_ptVsdPtOverPt_30_60->SetLineWidth(2);
  h2_ptVsdPtOverPt_30_60->SetLineColor(2);

  h2_ptVsdPtOverPtEta1_30_60->SetLineWidth(2);
  h2_ptVsdPtOverPtEta1_30_60->SetLineColor(2);

  h2_ptVsdPtOverPtEta2_30_60->SetLineWidth(2);
  h2_ptVsdPtOverPtEta2_30_60->SetLineColor(2);

  h2_ptVsdPtOverPtEta3_30_60->SetLineWidth(2);
  h2_ptVsdPtOverPtEta3_30_60->SetLineColor(2);

  

  gStyle->SetOptStat(0);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetLegendTextSize(0.03);

  TLegend * leg11 =new TLegend(0.75,0.75,0.88,0.88);    

  TLatex latex;
  latex.SetTextSize(0.025);
  latex.SetTextAlign(13);  //align at top
  




  h1_dPt->Draw("histo");
  h2_dPt->Draw("histo same");
  leg11->AddEntry(h1_dPt,"NNv4");
  leg11->AddEntry(h2_dPt,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,incEta);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPt_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_dPtEta1->Draw("histo");
  h2_dPtEta1->Draw("histo same");
  leg11->AddEntry(h1_dPtEta1,"NNv4");
  leg11->AddEntry(h2_dPtEta1,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta1);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtEta1_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_dPtEta2->Draw("histo");
  h2_dPtEta2->Draw("histo same");
  leg11->AddEntry(h1_dPtEta2,"NNv4");
  leg11->AddEntry(h2_dPtEta2,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta2);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtEta2_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_dPtEta3->Draw("histo");
  h2_dPtEta3->Draw("histo same");
  leg11->AddEntry(h1_dPtEta3,"NNv4");
  leg11->AddEntry(h2_dPtEta3,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta3);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtEta3_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_dPtOverPt->Draw("histo");
  h2_dPtOverPt->Draw("histo same");
  leg11->AddEntry(h1_dPtOverPt,"NNv4");
  leg11->AddEntry(h2_dPtOverPt,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,incEta);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtOverPt_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_dPtOverPtEta1->Draw("histo");
  h2_dPtOverPtEta1->Draw("histo same");
  leg11->AddEntry(h1_dPtOverPtEta1,"NNv4");
  leg11->AddEntry(h2_dPtOverPtEta1,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta1);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtOverPtEta1_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_dPtOverPtEta2->Draw("histo");
  h2_dPtOverPtEta2->Draw("histo same");
  leg11->AddEntry(h1_dPtOverPtEta2,"NNv4");
  leg11->AddEntry(h2_dPtOverPtEta2,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta2);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtOverPtEta2_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_dPtOverPtEta3->Draw("histo");
  h2_dPtOverPtEta3->Draw("histo same");
  leg11->AddEntry(h1_dPtOverPtEta3,"NNv4");
  leg11->AddEntry(h2_dPtOverPtEta3,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta3);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtOverPtEta3_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  // 0 - 10
  h1_ptVsdPt_0_10->Draw("histo");
  h2_ptVsdPt_0_10->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPt_0_10,"NNv4");
  leg11->AddEntry(h2_ptVsdPt_0_10,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,incEta);
  latex.DrawLatexNDC(.76,.65,Pt1);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPt_0_10_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtEta1_0_10->Draw("histo");
  h2_ptVsdPtEta1_0_10->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtEta1_0_10,"NNv4");
  leg11->AddEntry(h2_ptVsdPtEta1_0_10,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta1);
  latex.DrawLatexNDC(.76,.65,Pt1);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtEta1_0_10_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtEta2_0_10->Draw("histo");
  h2_ptVsdPtEta2_0_10->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtEta2_0_10,"NNv4");
  leg11->AddEntry(h2_ptVsdPtEta2_0_10,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta2);
  latex.DrawLatexNDC(.76,.65,Pt1);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtEta2_0_10_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtEta3_0_10->Draw("histo");
  h2_ptVsdPtEta3_0_10->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtEta3_0_10,"NNv4");
  leg11->AddEntry(h2_ptVsdPtEta3_0_10,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta3);
  latex.DrawLatexNDC(.76,.65,Pt1);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtEta3_0_10_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  // 10 - 20
  h1_ptVsdPt_10_20->Draw("histo");
  h2_ptVsdPt_10_20->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPt_10_20,"NNv4");
  leg11->AddEntry(h2_ptVsdPt_10_20,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,incEta);
  latex.DrawLatexNDC(.66,.65,Pt2);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPt_10_20_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtEta1_10_20->Draw("histo");
  h2_ptVsdPtEta1_10_20->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtEta1_10_20,"NNv4");
  leg11->AddEntry(h2_ptVsdPtEta1_10_20,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta1);
  latex.DrawLatexNDC(.66,.65,Pt2);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtEta1_10_20_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtEta2_10_20->Draw("histo");
  h2_ptVsdPtEta2_10_20->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtEta2_10_20,"NNv4");
  leg11->AddEntry(h2_ptVsdPtEta2_10_20,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta2);
  latex.DrawLatexNDC(.66,.65,Pt2);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtEta2_10_20_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtEta3_10_20->Draw("histo");
  h2_ptVsdPtEta3_10_20->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtEta3_10_20,"NNv4");
  leg11->AddEntry(h2_ptVsdPtEta3_10_20,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta3);
  latex.DrawLatexNDC(.66,.65,Pt2);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtEta3_10_20_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  // 20 - 30
  h1_ptVsdPt_20_30->Draw("histo");
  h2_ptVsdPt_20_30->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPt_20_30,"NNv4");
  leg11->AddEntry(h2_ptVsdPt_20_30,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,incEta);
  latex.DrawLatexNDC(.66,.65,Pt3);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPt_20_30_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtEta1_20_30->Draw("histo");
  h2_ptVsdPtEta1_20_30->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtEta1_20_30,"NNv4");
  leg11->AddEntry(h2_ptVsdPtEta1_20_30,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta1);
  latex.DrawLatexNDC(.66,.65,Pt3);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtEta1_20_30_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtEta2_20_30->Draw("histo");
  h2_ptVsdPtEta2_20_30->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtEta2_20_30,"NNv4");
  leg11->AddEntry(h2_ptVsdPtEta2_20_30,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta2);
  latex.DrawLatexNDC(.66,.65,Pt3);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtEta2_20_30_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtEta3_20_30->Draw("histo");
  h2_ptVsdPtEta3_20_30->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtEta3_20_30,"NNv4");
  leg11->AddEntry(h2_ptVsdPtEta3_20_30,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta3);
  latex.DrawLatexNDC(.66,.65,Pt3);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtEta3_20_30_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  // 30 - 60
  h1_ptVsdPt_30_60->Draw("histo");
  h2_ptVsdPt_30_60->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPt_30_60,"NNv4");
  leg11->AddEntry(h2_ptVsdPt_30_60,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,incEta);
  latex.DrawLatexNDC(.66,.65,Pt4);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPt_30_60_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtEta1_30_60->Draw("histo");
  h2_ptVsdPtEta1_30_60->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtEta1_30_60,"NNv4");
  leg11->AddEntry(h2_ptVsdPtEta1_30_60,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta1);
  latex.DrawLatexNDC(.66,.65,Pt4);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtEta1_30_60_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtEta2_30_60->Draw("histo");
  h2_ptVsdPtEta2_30_60->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtEta2_30_60,"NNv4");
  leg11->AddEntry(h2_ptVsdPtEta2_30_60,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta2);
  latex.DrawLatexNDC(.66,.65,Pt4);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtEta2_30_60_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtEta3_30_60->Draw("histo");
  h2_ptVsdPtEta3_30_60->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtEta3_30_60,"NNv4");
  leg11->AddEntry(h2_ptVsdPtEta3_30_60,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta3);
  latex.DrawLatexNDC(.66,.65,Pt4);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtEta3_30_60_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();


  // 0 - 10
  h1_ptVsdPtOverPt_0_10->Draw("histo");
  h2_ptVsdPtOverPt_0_10->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtOverPt_0_10,"NNv4");
  leg11->AddEntry(h2_ptVsdPtOverPt_0_10,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,incEta);
  latex.DrawLatexNDC(.76,.65,Pt1);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtOverPt_0_10_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtOverPtEta1_0_10->Draw("histo");
  h2_ptVsdPtOverPtEta1_0_10->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtOverPtEta1_0_10,"NNv4");
  leg11->AddEntry(h2_ptVsdPtOverPtEta1_0_10,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta1);
  latex.DrawLatexNDC(.76,.65,Pt1);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtOverPtEta1_0_10_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtOverPtEta2_0_10->Draw("histo");
  h2_ptVsdPtOverPtEta2_0_10->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtOverPtEta2_0_10,"NNv4");
  leg11->AddEntry(h2_ptVsdPtOverPtEta2_0_10,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta2);
  latex.DrawLatexNDC(.76,.65,Pt1);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtOverPtEta2_0_10_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtOverPtEta3_0_10->Draw("histo");
  h2_ptVsdPtOverPtEta3_0_10->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtOverPtEta3_0_10,"NNv4");
  leg11->AddEntry(h2_ptVsdPtOverPtEta3_0_10,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta3);
  latex.DrawLatexNDC(.76,.65,Pt1);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtOverPtEta3_0_10_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  // 10 - 20
  h1_ptVsdPtOverPt_10_20->Draw("histo");
  h2_ptVsdPtOverPt_10_20->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtOverPt_10_20,"NNv4");
  leg11->AddEntry(h2_ptVsdPtOverPt_10_20,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,incEta);
  latex.DrawLatexNDC(.66,.65,Pt2);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtOverPt_10_20_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtOverPtEta1_10_20->Draw("histo");
  h2_ptVsdPtOverPtEta1_10_20->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtOverPtEta1_10_20,"NNv4");
  leg11->AddEntry(h2_ptVsdPtOverPtEta1_10_20,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta1);
  latex.DrawLatexNDC(.66,.65,Pt2);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtOverPtEta1_10_20_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtOverPtEta2_10_20->Draw("histo");
  h2_ptVsdPtOverPtEta2_10_20->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtOverPtEta2_10_20,"NNv4");
  leg11->AddEntry(h2_ptVsdPtOverPtEta2_10_20,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta2);
  latex.DrawLatexNDC(.66,.65,Pt2);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtOverPtEta2_10_20_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtOverPtEta3_10_20->Draw("histo");
  h2_ptVsdPtOverPtEta3_10_20->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtOverPtEta3_10_20,"NNv4");
  leg11->AddEntry(h2_ptVsdPtOverPtEta3_10_20,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta3);
  latex.DrawLatexNDC(.66,.65,Pt2);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtOverPtEta3_10_20_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  // 20 - 30
  h1_ptVsdPtOverPt_20_30->Draw("histo");
  h2_ptVsdPtOverPt_20_30->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtOverPt_20_30,"NNv4");
  leg11->AddEntry(h2_ptVsdPtOverPt_20_30,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,incEta);
  latex.DrawLatexNDC(.66,.65,Pt3);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtOverPt_20_30_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtOverPtEta1_20_30->Draw("histo");
  h2_ptVsdPtOverPtEta1_20_30->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtOverPtEta1_20_30,"NNv4");
  leg11->AddEntry(h2_ptVsdPtOverPtEta1_20_30,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta1);
  latex.DrawLatexNDC(.66,.65,Pt3);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtOverPtEta1_20_30_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtOverPtEta2_20_30->Draw("histo");
  h2_ptVsdPtOverPtEta2_20_30->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtOverPtEta2_20_30,"NNv4");
  leg11->AddEntry(h2_ptVsdPtOverPtEta2_20_30,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta2);
  latex.DrawLatexNDC(.66,.65,Pt3);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtOverPtEta2_20_30_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtOverPtEta3_20_30->Draw("histo");
  h2_ptVsdPtOverPtEta3_20_30->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtOverPtEta3_20_30,"NNv4");
  leg11->AddEntry(h2_ptVsdPtOverPtEta3_20_30,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta3);
  latex.DrawLatexNDC(.66,.65,Pt3);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtOverPtEta3_20_30_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  // 30 - 60
  h1_ptVsdPtOverPt_30_60->Draw("histo");
  h2_ptVsdPtOverPt_30_60->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtOverPt_30_60,"NNv4");
  leg11->AddEntry(h2_ptVsdPtOverPt_30_60,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,incEta);
  latex.DrawLatexNDC(.66,.65,Pt4);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtOverPt_30_60_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtOverPtEta1_30_60->Draw("histo");
  h2_ptVsdPtOverPtEta1_30_60->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtOverPtEta1_30_60,"NNv4");
  leg11->AddEntry(h2_ptVsdPtOverPtEta1_30_60,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta1);
  latex.DrawLatexNDC(.66,.65,Pt4);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtOverPtEta1_30_60_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtOverPtEta2_30_60->Draw("histo");
  h2_ptVsdPtOverPtEta2_30_60->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtOverPtEta2_30_60,"NNv4");
  leg11->AddEntry(h2_ptVsdPtOverPtEta2_30_60,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta2);
  latex.DrawLatexNDC(.66,.65,Pt4);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtOverPtEta2_30_60_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();

  h1_ptVsdPtOverPtEta3_30_60->Draw("histo");
  h2_ptVsdPtOverPtEta3_30_60->Draw("histo same");
  leg11->AddEntry(h1_ptVsdPtOverPtEta3_30_60,"NNv4");
  leg11->AddEntry(h2_ptVsdPtOverPtEta3_30_60,"NNv5");
  leg11->Draw("sames");
  latex.DrawLatexNDC(.75,.7,eta3);
  latex.DrawLatexNDC(.66,.65,Pt4);
  pad->SaveAs("output_files/comparisons/truth/NNv4VsNNv5/dPtOverPtEta3_30_60_NNv4VsNNv5_HTo2LL.pdf");
  leg11->Clear();




  return 0;
} // end function
