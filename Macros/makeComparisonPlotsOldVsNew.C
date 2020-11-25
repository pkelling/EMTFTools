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


int makeComparisonPlotsOldVsNew(){

  //read data
  TString ntuple_old = "/eos/cms/store/user/eyigitba/emtf/matchedNtuples/matchedNtuple_HTo2LLTo4Mu_combined_cmssw_11_0_2_fwImplementation_NNv4.root";
  TString ntuple_new = "/eos/cms/store/user/eyigitba/emtf/matchedNtuples/matchedNtuple_HTo2LLTo4Mu_combined_cmssw_11_0_2_fwImplementation_NNv5.root";
  TChain * cc_old=new TChain("tree");
  TChain * cc_new=new TChain("tree");
  cc_old->Add(ntuple_old);
  cc_new->Add(ntuple_new);


  TTreeReader ccReaderOld(cc_old);
  TTreeReader ccReaderNew(cc_new);

  TTreeReaderArray<float> l1MuPtDxyOld(ccReaderOld,"l1_ptDxy");

  TTreeReaderArray<float> l1MuDxyNNOld(ccReaderOld,"l1_dxyNN");
  TTreeReaderArray<int> l1MuDxyOld(ccReaderOld,"l1_dxy");



  TTreeReaderArray<float> l1MuPtDxyNew(ccReaderNew,"l1_ptDxy");

  TTreeReaderArray<float> l1MuDxyNNNew(ccReaderNew,"l1_dxyNN");
  TTreeReaderArray<int> l1MuDxyNew(ccReaderNew,"l1_dxy");


  gStyle->SetOptStat(0);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetLegendTextSize(0.03);

  std::cout<<"Running on "<<cc_old->GetEntries()<<" evts "<<std::endl;

  //plot containers
  std::vector<TString> canvasname;
  std::vector<TString> legs;

  // cosmetic options
  std::vector<bool> grid,logY,logX;


  TH1F *h_ptOld = new TH1F("h_ptOld", "", 40, 0, 80);
  TH1F *h_ptNew = new TH1F("h_ptNew", "", 40, 0, 80);

  TH1F *h_InvPtOld = new TH1F("h_InvPtOld", "", 30, 0, 60);
  TH1F *h_InvPtNew = new TH1F("h_InvPtNew", "", 30, 0, 60);


  TH1F *h_dxyOld = new TH1F("h_den_dxyOld", "", 75, -150, 150);
  TH1F *h_dxyNew = new TH1F("h_den_dxyNew", "", 75, -150, 150);

  TH1F *h_IntDxyOld = new TH1F("h_IntDxyOld", "", 4, -0.5, 3.5);
  TH1F *h_IntDxyNew = new TH1F("h_IntDxyNew", "", 4, -0.5, 3.5);

  TH1F *h_ptDiff = new TH1F("h_ptDiff", "", 81, -40.5, 40.5);
  TH1F *h_InvPtDiff = new TH1F("h_InvPtDiff", "", 61, -30.5, 30.5);

  TH1F *h_dxyDiff = new TH1F("h_dxyDiff", "", 61, -30.5, 30.5);
  TH1F *h_IntDxyDiff = new TH1F("h_IntDxyDiff", "", 7, -3.5, 3.5);




  // Single muon efficiencies 

  int eventCount = 0;
  while(ccReaderOld.Next() && ccReaderNew.Next()){
    eventCount++;
    if (eventCount % 10000 == 0) std::cout << eventCount << " events read!" << std::endl;
    for(int i=0; i<l1MuPtDxyOld.GetSize(); i++){


      h_ptOld->Fill(l1MuPtDxyOld[i]);
      h_dxyOld->Fill(l1MuDxyNNOld[i]);

      h_InvPtOld->Fill(100.0/l1MuPtDxyOld[i]);
      h_IntDxyOld->Fill(l1MuDxyOld[i]);

      h_ptNew->Fill(l1MuPtDxyNew[i]);
      h_dxyNew->Fill(l1MuDxyNNNew[i]);

      h_InvPtNew->Fill(100.0/l1MuPtDxyNew[i]);
      h_IntDxyNew->Fill(l1MuDxyNew[i]);

      h_ptDiff->Fill(l1MuPtDxyNew[i] - l1MuPtDxyOld[i]);
      h_InvPtDiff->Fill(100.0/l1MuPtDxyNew[i] - 100.0/l1MuPtDxyOld[i]);

      h_dxyDiff->Fill(l1MuDxyNNNew[i] - l1MuDxyNNOld[i]);
      h_IntDxyDiff->Fill(l1MuDxyNew[i] - l1MuDxyOld[i]);


    }
  
  }

  // eventCount = 0;
  // while(ccReaderNew.Next()){
  //   eventCount++;
  //   if (eventCount % 10000 == 0) std::cout << eventCount << " events read!" << std::endl;
  //   for(int i=0; i<l1MuPtDxyNew.GetSize(); i++){


  //     h_ptNew->Fill(l1MuPtDxyNew[i]);
  //     h_dxyNew->Fill(l1MuDxyNNNew[i]);

  //     h_InvPtNew->Fill(100.0/l1MuPtDxyNew[i]);
  //     h_IntDxyNew->Fill(l1MuDxyNew[i]);

  //   }
  
  // }




  h_ptOld->GetXaxis()->SetTitle("p_{T} [GeV]");
  h_ptOld->GetYaxis()->SetTitle("# of muons / bin");
  h_ptOld->GetYaxis()->SetRangeUser(0.00001,30000);

  h_dxyOld->GetXaxis()->SetTitle("Dxy [cm]");
  h_dxyOld->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyOld->GetYaxis()->SetRangeUser(0.00001,15000);

  h_InvPtOld->GetXaxis()->SetTitle("100/p_{T} [GeV]^{-1}");
  h_InvPtOld->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtOld->GetYaxis()->SetRangeUser(0.00001,25000);

  h_IntDxyOld->GetXaxis()->SetTitle("Integer Dxy");
  h_IntDxyOld->GetYaxis()->SetTitle("# of muons / bin");
  h_IntDxyOld->GetYaxis()->SetRangeUser(0.00001,100000);

  h_ptDiff->GetXaxis()->SetTitle("p_{T} (NNv5) - p_{T} (NNv4) [GeV]");
  h_ptDiff->GetYaxis()->SetTitle("# of muons / bin");
  h_ptDiff->GetYaxis()->SetRangeUser(0.00001,40000);

  h_InvPtDiff->GetXaxis()->SetTitle("100.0/p_{T} (NNv5) - 100.0/p_{T} (NNv4) [GeV]^{-1}");
  h_InvPtDiff->GetYaxis()->SetTitle("# of muons / bin");
  h_InvPtDiff->GetYaxis()->SetRangeUser(0.00001,30000);

  h_dxyDiff->GetXaxis()->SetTitle("Dxy (NNv5) - Dxy (NNv4) [cm]");
  h_dxyDiff->GetYaxis()->SetTitle("# of muons / bin");
  h_dxyDiff->GetYaxis()->SetRangeUser(0.00001,20000);

  h_IntDxyDiff->GetXaxis()->SetTitle("Int Dxy (NNv5) - Int Dxy (NNv4)");
  h_IntDxyDiff->GetYaxis()->SetTitle("# of muons / bin");
  h_IntDxyDiff->GetYaxis()->SetRangeUser(0.00001,120000);


  h_ptOld->SetLineWidth(3);
  h_ptOld->SetLineColor(1);

  h_InvPtOld->SetLineWidth(3);
  h_InvPtOld->SetLineColor(1);

  h_dxyOld->SetLineWidth(3);
  h_dxyOld->SetLineColor(1);

  h_IntDxyOld->SetLineWidth(3);
  h_IntDxyOld->SetLineColor(1);

  h_ptNew->SetLineWidth(3);
  h_ptNew->SetLineColor(2);

  h_InvPtNew->SetLineWidth(3);
  h_InvPtNew->SetLineColor(2);

  h_dxyNew->SetLineWidth(3);
  h_dxyNew->SetLineColor(2);

  h_IntDxyNew->SetLineWidth(3);
  h_IntDxyNew->SetLineColor(2);

  h_ptDiff->SetLineWidth(3);
  h_ptDiff->SetLineColor(1);

  h_InvPtDiff->SetLineWidth(3);
  h_InvPtDiff->SetLineColor(1);

  h_dxyDiff->SetLineWidth(3);
  h_dxyDiff->SetLineColor(1);

  h_IntDxyDiff->SetLineWidth(3);
  h_IntDxyDiff->SetLineColor(1);

 
  TString leg = "NNv4";
  TString leg2 = "NNv5";

  //create canvas and save histos
  TCanvas * c1=new TCanvas("c1","c1",1200,1200);

  TPad* pad = new TPad("","",0.1,0.1,1,1);  

  pad->SetLeftMargin(0.2);
  
  h_ptOld->Draw("");
  h_ptNew->Draw("sames");
  
  TLegend * leg11 =new TLegend(0.75,0.75,0.88,0.88);    
  leg11->AddEntry(h_ptOld,leg);
  leg11->AddEntry(h_ptNew,leg2);
  leg11->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv4_NNv5/pT_NNv4VsNNv5.pdf");

  h_InvPtOld->Draw("");
  h_InvPtNew->Draw("sames");
 
  TLegend * leg22 =new TLegend(0.75,0.75,0.88,0.88);    
  leg22->AddEntry(h_InvPtOld,leg);
  leg22->AddEntry(h_InvPtNew,leg2);
  leg22->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv4_NNv5/invPT_NNv4VsNNv5.pdf");

  h_dxyOld->Draw("");
  h_dxyNew->Draw("sames");
 
  TLegend * leg33 =new TLegend(0.75,0.75,0.88,0.88);    
  leg33->AddEntry(h_dxyOld,leg);
  leg33->AddEntry(h_dxyNew,leg2);
  leg33->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv4_NNv5/dxy_NNv4VsNNv5.pdf");

  h_IntDxyOld->Draw("");
  h_IntDxyNew->Draw("sames");
 
  TLegend * leg44 =new TLegend(0.75,0.75,0.88,0.88);    
  leg44->AddEntry(h_IntDxyOld,leg);
  leg44->AddEntry(h_IntDxyNew,leg2);
  leg44->Draw("sames");

  c1->SaveAs("./output_files/comparisons/NNv4_NNv5/intDxy_NNv4VsNNv5.pdf");

  h_ptDiff->Draw("");
  c1->SaveAs("./output_files/comparisons/NNv4_NNv5/dPT_NNv4VsNNv5.pdf");

  h_InvPtDiff->Draw("");
  c1->SaveAs("./output_files/comparisons/NNv4_NNv5/dInvPT_NNv4VsNNv5.pdf");

  h_dxyDiff->Draw("");
  c1->SaveAs("./output_files/comparisons/NNv4_NNv5/dDxy_NNv4VsNNv5.pdf");

  h_IntDxyDiff->Draw("");
  c1->SaveAs("./output_files/comparisons/NNv4_NNv5/dIntDxy_NNv4VsNNv5.pdf");




  return 0;
 }
