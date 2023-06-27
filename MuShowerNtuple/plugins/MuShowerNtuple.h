// -*- C++ -*-
//
// Package:    EMTFTools/MuShowerNtuple
// Class:      MuShowerNtuple
//
/**\class MuShowerNtuple MuShowerNtuple.cc EMTFTools/MuShowerNtuple/plugins/MuShowerNtuple.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Efe Yigitbasi
//         Created:  Tue, 20 Jun 2023 11:22:01 GMT
//
//

// system include files
#include <memory>
#include "TTree.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"


// data formats
#include "DataFormats/L1Trigger/interface/MuonShower.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonShower.h"
#include "DataFormats/CSCDigi/interface/CSCShowerDigi.h"
#include "DataFormats/CSCDigi/interface/CSCShowerDigiCollection.h"



//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.

class MuShowerNtuple : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit MuShowerNtuple(const edm::ParameterSet&);
  ~MuShowerNtuple() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;

  void fillTree();
  void makeTree();
  void getHandles(const edm::Event &iEvent, const edm::EventSetup &iSetup);

  template <typename T> edm::Handle<T> make_handle(T &t) {
      return edm::Handle<T>();
  }

  template <typename T> edm::Handle<T> make_handle(T *t) {
      return edm::Handle<T>();
  }


  // ----------member data ---------------------------
  bool firstEvent_;
  TTree *tree;

  // Input tags
  const edm::InputTag CSCShowerDigiInputTag_;
  const edm::InputTag EMTFShowerDigiInputTag_;
  const edm::InputTag EMTFRegionalShowerInputTag_;
  const edm::InputTag GMTRegionalShowerInputTag_;
  const edm::InputTag GMTShowerInputTag_;

  // Enable/Disable showers from different sources
  bool useCSCShowerDigis_;
  bool useEMTFShowerDigis_;
  bool useEMTFRegionalShowers_;
  bool useGMTRegionalShowers_;
  bool useGMTShowers_;

  // ED Tokens
  edm::EDGetTokenT<CSCShowerDigiCollection> CSCShowerDigiToken_;
  edm::EDGetTokenT<CSCShowerDigiCollection> EMTFShowerDigiToken_;
  edm::EDGetTokenT<l1t::RegionalMuonShowerBxCollection> EMTFRegionalShowerToken_;
  edm::EDGetTokenT<l1t::RegionalMuonShowerBxCollection> GMTRegionalShowerToken_;
  edm::EDGetTokenT<l1t::MuonShowerBxCollection> GMTShowerToken_;

  // Shower collections
  const CSCShowerDigiCollection *CSCShowerDigis_;
  const CSCShowerDigiCollection *EMTFShowerDigis_;
  const l1t::RegionalMuonShowerBxCollection *EMTFRegionalShowers_;
  const l1t::RegionalMuonShowerBxCollection *GMTRegionalShowers_;
  const l1t::MuonShowerBxCollection *GMTShowers_;

  // CSCShowerDigi variables from CSC output
  std::unique_ptr<std::vector<int16_t>> CSCShowerDigi_endcap_;
  std::unique_ptr<std::vector<int16_t>> CSCShowerDigi_station_;
  std::unique_ptr<std::vector<int16_t>> CSCShowerDigi_ring_;
  std::unique_ptr<std::vector<int16_t>> CSCShowerDigi_chamber_;
  std::unique_ptr<std::vector<int16_t>> CSCShowerDigi_bx_;
  std::unique_ptr<std::vector<int16_t>> CSCShowerDigi_oneLoose_;
  std::unique_ptr<std::vector<int16_t>> CSCShowerDigi_twoLoose_;
  std::unique_ptr<std::vector<int16_t>> CSCShowerDigi_oneNominal_;
  std::unique_ptr<std::vector<int16_t>> CSCShowerDigi_oneTight_;
  std::unique_ptr<int32_t> CSCShowerDigiSize_;

  // CSCShowerDigi variables from EMTF input
  std::unique_ptr<std::vector<int16_t>> EMTFShowerDigi_endcap_;
  std::unique_ptr<std::vector<int16_t>> EMTFShowerDigi_station_;
  std::unique_ptr<std::vector<int16_t>> EMTFShowerDigi_ring_;
  std::unique_ptr<std::vector<int16_t>> EMTFShowerDigi_chamber_;
  std::unique_ptr<std::vector<int16_t>> EMTFShowerDigi_bx_;
  std::unique_ptr<std::vector<int16_t>> EMTFShowerDigi_oneLoose_;
  std::unique_ptr<std::vector<int16_t>> EMTFShowerDigi_twoLoose_;
  std::unique_ptr<std::vector<int16_t>> EMTFShowerDigi_oneNominal_;
  std::unique_ptr<std::vector<int16_t>> EMTFShowerDigi_oneTight_;
  std::unique_ptr<int32_t> EMTFShowerDigiSize_;

  // RegionalMuonShower variables from EMTF output
  std::unique_ptr<std::vector<int16_t>> EMTFRegionalShower_endcap_;
  std::unique_ptr<std::vector<int16_t>> EMTFRegionalShower_sector_;
  std::unique_ptr<std::vector<int16_t>> EMTFRegionalShower_bx_;
  std::unique_ptr<std::vector<int16_t>> EMTFRegionalShower_oneLooseInTime_;
  std::unique_ptr<std::vector<int16_t>> EMTFRegionalShower_twoLooseInTime_;
  std::unique_ptr<std::vector<int16_t>> EMTFRegionalShower_oneNominalInTime_;
  std::unique_ptr<std::vector<int16_t>> EMTFRegionalShower_oneTightInTime_;
  std::unique_ptr<int32_t> EMTFRegionalShowerSize_;

  // RegionalMuonShower variables from GMT input
  std::unique_ptr<std::vector<int16_t>> GMTRegionalShower_endcap_;
  std::unique_ptr<std::vector<int16_t>> GMTRegionalShower_sector_;
  std::unique_ptr<std::vector<int16_t>> GMTRegionalShower_bx_;
  std::unique_ptr<std::vector<int16_t>> GMTRegionalShower_oneLooseInTime_;
  std::unique_ptr<std::vector<int16_t>> GMTRegionalShower_twoLooseInTime_;
  std::unique_ptr<std::vector<int16_t>> GMTRegionalShower_oneNominalInTime_;
  std::unique_ptr<std::vector<int16_t>> GMTRegionalShower_oneTightInTime_;
  std::unique_ptr<int32_t> GMTRegionalShowerSize_;

  // MuonShower variables from GMT output
  std::unique_ptr<std::vector<int16_t>> GMTShower_bx_;
  std::unique_ptr<std::vector<int16_t>> GMTShower_oneLooseInTime_;
  std::unique_ptr<std::vector<int16_t>> GMTShower_twoLooseDiffSectorsInTime_;
  std::unique_ptr<std::vector<int16_t>> GMTShower_oneNominalInTime_;
  std::unique_ptr<std::vector<int16_t>> GMTShower_oneTightInTime_;




};
