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

int makeInputComp(){

  TString file1 = "EMTFNtuple_11_0_X.root";
  TString file2 = "EMTFNtuple_11_2_X.root";

  // load trees
  TString tree = "EMTFNtuple/tree";


  TChain * cc1=new TChain(tree);
  TChain * cc2=new TChain(tree);


  cc1->Add(file1);
  cc2->Add(file2);

  std::cout<<"Running on "<<cc1->GetEntries()<<" evts "<<std::endl;

  gStyle->SetOptStat(0);



  TCanvas* canv1 = new TCanvas("c_str", "c_str", 1200, 1200);


  TH1F* h_endcap1 = new TH1F("endcap1", "endcap1", 5, 0, 5);
  TH1F* h_station1 = new TH1F("station1", "station1", 6, 0, 6);
  TH1F* h_ring1 = new TH1F("ring1", "ring1", 6, 0, 6);
  TH1F* h_sector1 = new TH1F("sector1", "sector1", 8, 0, 8);
  TH1F* h_chamber1 = new TH1F("chamber1", "chamber1", 40, 0, 40);
  TH1F* h_cscid1 = new TH1F("cscid1", "cscid1", 11, 0, 11);
  TH1F* h_bx1 = new TH1F("bx1", "bx1", 15, 0, 15);
  TH1F* h_strip1 = new TH1F("strip1", "strip1", 170, -5, 165);
  TH1F* h_wire1 = new TH1F("wire1", "wire1", 130, -5, 125);
  TH1F* h_quality1 = new TH1F("quality1", "quality1", 18, 0, 18);
  TH1F* h_pattern1 = new TH1F("pattern1", "pattern1", 12, 0, 12);
  TH1F* h_bend1 = new TH1F("bend1", "bend1", 4, -1, 3);


  TH1F* h_endcap2 = new TH1F("endcap2", "endcap2", 5, 0, 5);
  TH1F* h_station2 = new TH1F("station2", "station2", 6, 0, 6);
  TH1F* h_ring2 = new TH1F("ring2", "ring2", 6, 0, 6);
  TH1F* h_sector2 = new TH1F("sector2", "sector2", 8, 0, 8);
  TH1F* h_chamber2 = new TH1F("chamber2", "chamber2", 40, 0, 40);
  TH1F* h_cscid2 = new TH1F("cscid2", "cscid2", 11, 0, 11);
  TH1F* h_bx2 = new TH1F("bx2", "bx2", 15, 0, 15);
  TH1F* h_strip2 = new TH1F("strip2", "strip2", 170, -5, 165);
  TH1F* h_wire2 = new TH1F("wire2", "wire2", 130, -5, 125);
  TH1F* h_quality2 = new TH1F("quality2", "quality2", 18, 0, 18);
  TH1F* h_pattern2 = new TH1F("pattern2", "pattern2", 12, 0, 12);
  TH1F* h_bend2 = new TH1F("bend2", "bend2", 4, -1, 3);


  TH1F* h_hendcap1 = new TH1F("hendcap1", "hendcap1", 6, -3, 3);
  TH1F* h_hstation1 = new TH1F("hstation1", "hstation1", 6, 0, 6);
  TH1F* h_hring1 = new TH1F("hring1", "hring1", 6, 0, 6);
  TH1F* h_hsector1 = new TH1F("hsector1", "hsector1", 8, 0, 8);
  TH1F* h_hsubsector1 = new TH1F("hsubsector1", "hsubsector1", 11, -1, 10);
  TH1F* h_hchamber1 = new TH1F("hchamber1", "hchamber1", 40, 0, 40);
  TH1F* h_hcscid1 = new TH1F("hcscid1", "hcscid1", 11, 0, 11);
  TH1F* h_hbx1 = new TH1F("hbx1", "hbx1", 10, -5, 5);
  TH1F* h_htype1 = new TH1F("htype1", "htype1", 5, 0, 5);
  TH1F* h_hneighbor1 = new TH1F("hneighbor1", "hneighbor1", 4, -1, 3);
  TH1F* h_hstrip1 = new TH1F("hstrip1", "hstrip1", 170, -5, 165);
  TH1F* h_hwire1 = new TH1F("hwire1", "hwire1", 130, -5, 125);
  TH1F* h_hroll1 = new TH1F("hroll1", "hroll1", 15, -5, 10);
  TH1F* h_hquality1 = new TH1F("hquality1", "hquality1", 18, 0, 18);
  TH1F* h_hpattern1 = new TH1F("hpattern1", "hpattern1", 15, -1, 14);
  TH1F* h_hbend1 = new TH1F("hbend1", "hbend1", 4, -1, 3);
  TH1F* h_htime1 = new TH1F("htime1", "htime1", 5, -1, 4);
  TH1F* h_hphi1 = new TH1F("hphi1", "hphi1", 500, 0, 5000);
  TH1F* h_htheta1 = new TH1F("htheta1", "htheta1", 130, 0, 130);
  TH1F* h_hsimphi1 = new TH1F("hsimphi1", "hsimphi1", 400, -200, 200);
  TH1F* h_hsimtheta1 = new TH1F("hsimtheta1", "hsimtheta1", 180, 0, 180);
  TH1F* h_hsimr1 = new TH1F("hsimr1", "hsimr1", 700, 0, 700);
  TH1F* h_hsimz1 = new TH1F("hsimz1", "hsimz1", 300, -1500, 1500);

  TH1F* h_hendcap2 = new TH1F("hendcap2", "hendcap2", 6, -3, 3);
  TH1F* h_hstation2 = new TH1F("hstation2", "hstation2", 6, 0, 6);
  TH1F* h_hring2 = new TH1F("hring2", "hring2", 6, 0, 6);
  TH1F* h_hsector2 = new TH1F("hsector2", "hsector2", 8, 0, 8);
  TH1F* h_hsubsector2 = new TH1F("hsubsector2", "hsubsector2", 11, -1, 10);
  TH1F* h_hchamber2 = new TH1F("hchamber2", "hchamber2", 40, 0, 40);
  TH1F* h_hcscid2 = new TH1F("hcscid2", "hcscid2", 11, 0, 11);
  TH1F* h_hbx2 = new TH1F("hbx2", "hbx2", 10, -5, 5);
  TH1F* h_htype2 = new TH1F("htype2", "htype2", 5, 0, 5);
  TH1F* h_hneighbor2 = new TH1F("hneighbor2", "hneighbor2", 4, -1, 3);
  TH1F* h_hstrip2 = new TH1F("hstrip2", "hstrip2", 170, -5, 165);
  TH1F* h_hwire2 = new TH1F("hwire2", "hwire2", 130, -5, 125);
  TH1F* h_hroll2 = new TH1F("hroll2", "hroll2", 15, -5, 10);
  TH1F* h_hquality2 = new TH1F("hquality2", "hquality2", 18, 0, 18);
  TH1F* h_hpattern2 = new TH1F("hpattern2", "hpattern2", 15, -1, 14);
  TH1F* h_hbend2 = new TH1F("hbend2", "hbend2", 4, -1, 3);
  TH1F* h_htime2 = new TH1F("htime2", "htime2", 5, -1, 4);
  TH1F* h_hphi2 = new TH1F("hphi2", "hphi2", 500, 0, 5000);
  TH1F* h_htheta2 = new TH1F("htheta2", "htheta2", 130, 0, 130);
  TH1F* h_hsimphi2 = new TH1F("hsimphi2", "hsimphi2", 400, -200, 200);
  TH1F* h_hsimtheta2 = new TH1F("hsimtheta2", "hsimtheta2", 180, 0, 180);
  TH1F* h_hsimr2 = new TH1F("hsimr2", "hsimr2", 700, 0, 700);
  TH1F* h_hsimz2 = new TH1F("hsimz2", "hsimz2", 300, -1500, 1500);




  cc1->Draw("cscInput_endcap>>endcap1");
  cc1->Draw("cscInput_station>>station1");
  cc1->Draw("cscInput_ring>>ring1");
  cc1->Draw("cscInput_sector>>sector1");
  cc1->Draw("cscInput_chamber>>chamber1");
  cc1->Draw("cscInput_cscid>>cscid1");
  cc1->Draw("cscInput_bx>>bx1");
  cc1->Draw("cscInput_strip>>strip1");
  cc1->Draw("cscInput_wire>>wire1");
  cc1->Draw("cscInput_quality>>quality1");
  cc1->Draw("cscInput_pattern>>pattern1");
  cc1->Draw("cscInput_bend>>bend1");

  cc2->Draw("cscInput_endcap>>endcap2");
  cc2->Draw("cscInput_station>>station2");
  cc2->Draw("cscInput_ring>>ring2");
  cc2->Draw("cscInput_sector>>sector2");
  cc2->Draw("cscInput_chamber>>chamber2");
  cc2->Draw("cscInput_cscid>>cscid2");
  cc2->Draw("cscInput_bx>>bx2");
  cc2->Draw("cscInput_strip>>strip2");
  cc2->Draw("cscInput_wire>>wire2");
  cc2->Draw("cscInput_quality>>quality2");
  cc2->Draw("cscInput_pattern>>pattern2");
  cc2->Draw("cscInput_bend>>bend2");

  h_endcap1->GetXaxis()->SetTitle("endcap");
  h_station1->GetXaxis()->SetTitle("station");
  h_ring1->GetXaxis()->SetTitle("ring");
  h_sector1->GetXaxis()->SetTitle("sector");
  h_chamber1->GetXaxis()->SetTitle("chamber");
  h_cscid1->GetXaxis()->SetTitle("cscid");
  h_bx1->GetXaxis()->SetTitle("bx");
  h_strip1->GetXaxis()->SetTitle("strip");
  h_wire1->GetXaxis()->SetTitle("wire");
  h_quality1->GetXaxis()->SetTitle("quality");
  h_pattern1->GetXaxis()->SetTitle("pattern");
  h_bend1->GetXaxis()->SetTitle("bend");

  h_endcap1->SetLineColor(1);
  h_station1->SetLineColor(1);
  h_ring1->SetLineColor(1);
  h_sector1->SetLineColor(1);
  h_chamber1->SetLineColor(1);
  h_cscid1->SetLineColor(1);
  h_bx1->SetLineColor(1);
  h_strip1->SetLineColor(1);
  h_wire1->SetLineColor(1);
  h_quality1->SetLineColor(1);
  h_pattern1->SetLineColor(1);
  h_bend1->SetLineColor(1);

  h_endcap2->SetLineColor(2);
  h_station2->SetLineColor(2);
  h_ring2->SetLineColor(2);
  h_sector2->SetLineColor(2);
  h_chamber2->SetLineColor(2);
  h_cscid2->SetLineColor(2);
  h_bx2->SetLineColor(2);
  h_strip2->SetLineColor(2);
  h_wire2->SetLineColor(2);
  h_quality2->SetLineColor(2);
  h_pattern2->SetLineColor(2);
  h_bend2->SetLineColor(2);

  gStyle->SetLegendBorderSize(0);
  gStyle->SetLegendTextSize(0.025); 
  // TLegend * leg =new TLegend(0.55,0.75,0.88,0.88);




  h_endcap1->Draw("h");
  h_endcap2->Draw("sames");
  // leg->AddEntry(h_endcap1,"11_0_X");
  // leg->AddEntry(h_endcap2,"11_2_X");
  // leg->Draw("sames");
  canv1->SaveAs("output_files/comparisons/CSC/csc_endcap_11_0_X_vs_11_2_X.pdf");

  h_station1->Draw("h");
  h_station2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/CSC/csc_station_11_0_X_vs_11_2_X.pdf");

  h_ring1->Draw("h");
  h_ring2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/CSC/csc_ring_11_0_X_vs_11_2_X.pdf");

  h_sector1->Draw("h");
  h_sector2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/CSC/csc_sector_11_0_X_vs_11_2_X.pdf");

  h_chamber1->Draw("h");
  h_chamber2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/CSC/csc_chamber_11_0_X_vs_11_2_X.pdf");

  h_cscid1->Draw("h");
  h_cscid2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/CSC/csc_cscid_11_0_X_vs_11_2_X.pdf");

  h_bx1->Draw("h");
  h_bx2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/CSC/csc_bx_11_0_X_vs_11_2_X.pdf");

  h_strip1->Draw("h");
  h_strip2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/CSC/csc_strip_11_0_X_vs_11_2_X.pdf");

  h_wire1->Draw("h");
  h_wire2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/CSC/csc_wire_11_0_X_vs_11_2_X.pdf");

  h_quality1->Draw("h");
  h_quality2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/CSC/csc_quality_11_0_X_vs_11_2_X.pdf");

  h_pattern1->Draw("h");
  h_pattern2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/CSC/csc_pattern_11_0_X_vs_11_2_X.pdf");

  h_bend1->Draw("h");
  h_bend2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/CSC/csc_bend_11_0_X_vs_11_2_X.pdf");


  cc1->Draw("emtfHit_endcap>>hendcap1");
  cc1->Draw("emtfHit_station>>hstation1");
  cc1->Draw("emtfHit_ring>>hring1");
  cc1->Draw("emtfHit_sector>>hsector1");
  cc1->Draw("emtfHit_subsector>>hsubsector1");
  cc1->Draw("emtfHit_chamber>>hchamber1");
  cc1->Draw("emtfHit_cscid>>hcscid1");
  cc1->Draw("emtfHit_bx>>hbx1");
  cc1->Draw("emtfHit_type>>htype1");
  cc1->Draw("emtfHit_neighbor>>hneighbor1");
  cc1->Draw("emtfHit_strip>>hstrip1");
  cc1->Draw("emtfHit_wire>>hwire1");
  cc1->Draw("emtfHit_roll>>hroll1");
  cc1->Draw("emtfHit_quality>>hquality1");
  cc1->Draw("emtfHit_pattern>>hpattern1");
  cc1->Draw("emtfHit_bend>>hbend1");
  cc1->Draw("emtfHit_time>>htime1");
  cc1->Draw("emtfHit_emtf_phi>>hphi1");
  cc1->Draw("emtfHit_emtf_theta>>htheta1");
  cc1->Draw("emtfHit_sim_phi>>hsimphi1");
  cc1->Draw("emtfHit_sim_theta>>hsimtheta1");
  cc1->Draw("emtfHit_sim_r>>hsimr1");
  cc1->Draw("emtfHit_sim_z>>hsimz1");

  cc2->Draw("emtfHit_endcap>>hendcap2");
  cc2->Draw("emtfHit_station>>hstation2");
  cc2->Draw("emtfHit_ring>>hring2");
  cc2->Draw("emtfHit_sector>>hsector2");
  cc2->Draw("emtfHit_subsector>>hsubsector2");
  cc2->Draw("emtfHit_chamber>>hchamber2");
  cc2->Draw("emtfHit_cscid>>hcscid2");
  cc2->Draw("emtfHit_bx>>hbx2");
  cc2->Draw("emtfHit_type>>htype2");
  cc2->Draw("emtfHit_neighbor>>hneighbor2");
  cc2->Draw("emtfHit_strip>>hstrip2");
  cc2->Draw("emtfHit_wire>>hwire2");
  cc2->Draw("emtfHit_roll>>hroll2");
  cc2->Draw("emtfHit_quality>>hquality2");
  cc2->Draw("emtfHit_pattern>>hpattern2");
  cc2->Draw("emtfHit_bend>>hbend2");
  cc2->Draw("emtfHit_time>>htime2");
  cc2->Draw("emtfHit_emtf_phi>>hphi2");
  cc2->Draw("emtfHit_emtf_theta>>htheta2");
  cc2->Draw("emtfHit_sim_phi>>hsimphi2");
  cc2->Draw("emtfHit_sim_theta>>hsimtheta2");
  cc2->Draw("emtfHit_sim_r>>hsimr2");
  cc2->Draw("emtfHit_sim_z>>hsimz2");



  h_hendcap1->GetXaxis()->SetTitle("endcap");
  h_hstation1->GetXaxis()->SetTitle("station");
  h_hring1->GetXaxis()->SetTitle("ring");
  h_hsector1->GetXaxis()->SetTitle("sector");
  h_hsubsector1->GetXaxis()->SetTitle("subsector");
  h_hchamber1->GetXaxis()->SetTitle("chamber");
  h_hcscid1->GetXaxis()->SetTitle("cscid");
  h_hbx1->GetXaxis()->SetTitle("bx");
  h_htype1->GetXaxis()->SetTitle("type");
  h_hneighbor1->GetXaxis()->SetTitle("neighbor");
  h_hstrip1->GetXaxis()->SetTitle("strip");
  h_hwire1->GetXaxis()->SetTitle("wire");
  h_hroll1->GetXaxis()->SetTitle("roll");
  h_hquality1->GetXaxis()->SetTitle("quality");
  h_hpattern1->GetXaxis()->SetTitle("pattern");
  h_hbend1->GetXaxis()->SetTitle("bend");
  h_htime1->GetXaxis()->SetTitle("time");
  h_hphi1->GetXaxis()->SetTitle("phi");
  h_htheta1->GetXaxis()->SetTitle("theta");
  h_hsimphi1->GetXaxis()->SetTitle("simphi");
  h_hsimtheta1->GetXaxis()->SetTitle("simtheta");
  h_hsimr1->GetXaxis()->SetTitle("simr");
  h_hsimz1->GetXaxis()->SetTitle("simz");

  h_hsubsector1->SetMaximum(h_hsubsector2->GetMaximum()*1.2);
  h_htime1->SetMaximum(h_htime2->GetMaximum()*1.2);
  // h_hsubsector1->GetYaxis()->SetMaximum(h_hsubsector1->GetYaxis()->GetMaximum()*1.2);

  h_hendcap1->SetLineColor(1);
  h_hstation1->SetLineColor(1);
  h_hring1->SetLineColor(1);
  h_hsector1->SetLineColor(1);
  h_hsubsector1->SetLineColor(1);
  h_hchamber1->SetLineColor(1);
  h_hcscid1->SetLineColor(1);
  h_hbx1->SetLineColor(1);
  h_htype1->SetLineColor(1);
  h_hneighbor1->SetLineColor(1);
  h_hstrip1->SetLineColor(1);
  h_hwire1->SetLineColor(1);
  h_hroll1->SetLineColor(1);
  h_hquality1->SetLineColor(1);
  h_hpattern1->SetLineColor(1);
  h_hbend1->SetLineColor(1);
  h_htime1->SetLineColor(1);
  h_hphi1->SetLineColor(1);
  h_htheta1->SetLineColor(1);
  h_hsimphi1->SetLineColor(1);
  h_hsimtheta1->SetLineColor(1);
  h_hsimr1->SetLineColor(1);
  h_hsimz1->SetLineColor(1);

  h_hendcap2->SetLineColor(2);
  h_hstation2->SetLineColor(2);
  h_hring2->SetLineColor(2);
  h_hsector2->SetLineColor(2);
  h_hsubsector2->SetLineColor(2);
  h_hchamber2->SetLineColor(2);
  h_hcscid2->SetLineColor(2);
  h_hbx2->SetLineColor(2);
  h_htype2->SetLineColor(2);
  h_hneighbor2->SetLineColor(2);
  h_hstrip2->SetLineColor(2);
  h_hwire2->SetLineColor(2);
  h_hroll2->SetLineColor(2);
  h_hquality2->SetLineColor(2);
  h_hpattern2->SetLineColor(2);
  h_hbend2->SetLineColor(2);
  h_htime2->SetLineColor(2);
  h_hphi2->SetLineColor(2);
  h_htheta2->SetLineColor(2);
  h_hsimphi2->SetLineColor(2);
  h_hsimtheta2->SetLineColor(2);
  h_hsimr2->SetLineColor(2);
  h_hsimz2->SetLineColor(2);


  h_hendcap1->Draw("h");
  h_hendcap2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/hits/hit_endcap_11_0_X_vs_11_2_X.pdf");

  h_hstation1->Draw("h");
  h_hstation2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/hits/hit_station_11_0_X_vs_11_2_X.pdf");

  h_hring1->Draw("h");
  h_hring2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/hits/hit_ring_11_0_X_vs_11_2_X.pdf");

  h_hsector1->Draw("h");
  h_hsector2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/hits/hit_sector_11_0_X_vs_11_2_X.pdf");

  h_hsubsector1->Draw("h");
  h_hsubsector2->Draw("sames");
  TLegend * leg =new TLegend(0.75,0.75,0.88,0.88);
  leg->AddEntry(h_hsubsector1,"11_0_X");
  leg->AddEntry(h_hsubsector2,"11_2_X");
  leg->Draw("sames");
  canv1->SaveAs("output_files/comparisons/hits/hit_subsector_11_0_X_vs_11_2_X.pdf");

  h_hchamber1->Draw("h");
  h_hchamber2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/hits/hit_chamber_11_0_X_vs_11_2_X.pdf");

  h_hcscid1->Draw("h");
  h_hcscid2->Draw("sames");
  TLegend * leg2 =new TLegend(0.75,0.75,0.88,0.88);
  leg2->AddEntry(h_hcscid1,"11_0_X");
  leg2->AddEntry(h_hcscid2,"11_2_X");
  leg2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/hits/hit_cscid_11_0_X_vs_11_2_X.pdf");

  h_hbx1->Draw("h");
  h_hbx2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/hits/hit_bx_11_0_X_vs_11_2_X.pdf");

  h_htype1->Draw("h");
  h_htype2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/hits/hit_type_11_0_X_vs_11_2_X.pdf");

  h_hneighbor1->Draw("h");
  h_hneighbor2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/hits/hit_neighbor_11_0_X_vs_11_2_X.pdf");

  h_hstrip1->Draw("h");
  h_hstrip2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/hits/hit_strip_11_0_X_vs_11_2_X.pdf");

  h_hwire1->Draw("h");
  h_hwire2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/hits/hit_wire_11_0_X_vs_11_2_X.pdf");

  h_hroll1->Draw("h");
  h_hroll2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/hits/hit_roll_11_0_X_vs_11_2_X.pdf");

  h_hquality1->Draw("h");
  h_hquality2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/hits/hit_quality_11_0_X_vs_11_2_X.pdf");

  h_hpattern1->Draw("h");
  h_hpattern2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/hits/hit_pattern_11_0_X_vs_11_2_X.pdf");

  h_hbend1->Draw("h");
  h_hbend2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/hits/hit_bend_11_0_X_vs_11_2_X.pdf");

  h_htime1->Draw("h");
  h_htime2->Draw("sames");
  TLegend * leg3 =new TLegend(0.75,0.75,0.88,0.88);
  leg3->AddEntry(h_htime1,"11_0_X");
  leg3->AddEntry(h_htime2,"11_2_X");
  leg3->Draw("sames");
  canv1->SaveAs("output_files/comparisons/hits/hit_time_11_0_X_vs_11_2_X.pdf");

  h_hphi1->Draw("h");
  h_hphi2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/hits/hit_phi_11_0_X_vs_11_2_X.pdf");

  h_htheta1->Draw("h");
  h_htheta2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/hits/hit_theta_11_0_X_vs_11_2_X.pdf");

  h_hsimphi1->Draw("h");
  h_hsimphi2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/hits/hit_simphi_11_0_X_vs_11_2_X.pdf");

  h_hsimtheta1->Draw("h");
  h_hsimtheta2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/hits/hit_simtheta_11_0_X_vs_11_2_X.pdf");

  h_hsimr1->Draw("h");
  h_hsimr2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/hits/hit_simr_11_0_X_vs_11_2_X.pdf");

  h_hsimz1->Draw("h");
  h_hsimz2->Draw("sames");
  canv1->SaveAs("output_files/comparisons/hits/hit_simz_11_0_X_vs_11_2_X.pdf");



  return 0;
} // end function
