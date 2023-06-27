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

//
// class declaration
//
#include "EMTFTools/MuShowerNtuple/plugins/MuShowerNtuple.h"

//
// constructors and destructor
//
MuShowerNtuple::MuShowerNtuple(const edm::ParameterSet& iConfig)
    : CSCShowerDigiInputTag_(iConfig.getParameter<edm::InputTag>("CSCShowerDigiInputTag")),
      EMTFShowerDigiInputTag_(iConfig.getParameter<edm::InputTag>("EMTFShowerDigiInputTag")),
      EMTFRegionalShowerInputTag_(iConfig.getParameter<edm::InputTag>("EMTFRegionalShowerInputTag")),
      GMTRegionalShowerInputTag_(iConfig.getParameter<edm::InputTag>("GMTRegionalShowerInputTag")),
      GMTShowerInputTag_(iConfig.getParameter<edm::InputTag>("GMTShowerInputTag")),
      useCSCShowerDigis_(iConfig.getParameter<bool>("useCSCShowerDigis")),
      useEMTFShowerDigis_(iConfig.getParameter<bool>("useEMTFShowerDigis")),
      useEMTFRegionalShowers_(iConfig.getParameter<bool>("useEMTFRegionalShowers")),
      useGMTRegionalShowers_(iConfig.getParameter<bool>("useGMTRegionalShowers")),
      useGMTShowers_(iConfig.getParameter<bool>("useGMTShowers"))
{
  //now do what ever initialization is needed
  firstEvent_ = true;

  CSCShowerDigiToken_ = consumes<CSCShowerDigiCollection>(CSCShowerDigiInputTag_);
  EMTFShowerDigiToken_ = consumes<CSCShowerDigiCollection>(EMTFShowerDigiInputTag_);
  EMTFRegionalShowerToken_ = consumes<l1t::RegionalMuonShowerBxCollection>(EMTFRegionalShowerInputTag_);
  GMTRegionalShowerToken_ = consumes<l1t::RegionalMuonShowerBxCollection>(GMTRegionalShowerInputTag_);
  GMTShowerToken_ = consumes<l1t::MuonShowerBxCollection>(GMTShowerInputTag_);

  usesResource("TFileService");
}

MuShowerNtuple::~MuShowerNtuple() {
    // Do nothing
}



//
// member functions
//

// ------------ method called for each event  ------------
void MuShowerNtuple::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  // get handles
  getHandles(iEvent, iSetup);




  // CSCShowerDigis from CSC output
  for (auto const& element : *CSCShowerDigis_) {
    auto detId = element.first;
    auto cscShower = element.second.first;
    auto cscShowerEnd = element.second.second;

    int endcap = (detId.endcap() == 1 ? 1 : -1);
    int station = detId.station();
    int ring = detId.ring();
    int chamber = detId.chamber();

    for (; cscShower != cscShowerEnd; ++cscShower) {

      CSCShowerDigi_endcap_->push_back(endcap);
      CSCShowerDigi_station_->push_back(station);
      CSCShowerDigi_ring_->push_back(ring);
      CSCShowerDigi_chamber_->push_back(chamber);
      CSCShowerDigi_bx_->push_back(cscShower->getBX());
      CSCShowerDigi_oneLoose_->push_back(cscShower->isLooseInTime());
      // CSCShowerDigi_twoLoose_->push_back(cscShower->isTwoLooseInTime());
      CSCShowerDigi_oneNominal_->push_back(cscShower->isNominalInTime());
      CSCShowerDigi_oneTight_->push_back(cscShower->isTightInTime());

      (*CSCShowerDigiSize_)++;
    }
  }

  // CSCShowerDigis from EMTF input
  for (auto const& element : *EMTFShowerDigis_) {
    auto detId = element.first;
    auto cscShower = element.second.first;
    auto cscShowerEnd = element.second.second;

    int endcap = (detId.endcap() == 1 ? 1 : -1);
    int station = detId.station();
    int ring = detId.ring();
    int chamber = detId.chamber();

    for (; cscShower != cscShowerEnd; ++cscShower) {

      EMTFShowerDigi_endcap_->push_back(endcap);
      EMTFShowerDigi_station_->push_back(station);
      EMTFShowerDigi_ring_->push_back(ring);
      EMTFShowerDigi_chamber_->push_back(chamber);
      EMTFShowerDigi_bx_->push_back(cscShower->getBX());
      EMTFShowerDigi_oneLoose_->push_back(cscShower->isLooseInTime());
      // EMTFShowerDigi_twoLoose_->push_back(cscShower->isTwoLooseInTime());
      EMTFShowerDigi_oneNominal_->push_back(cscShower->isNominalInTime());
      EMTFShowerDigi_oneTight_->push_back(cscShower->isTightInTime());

      (*EMTFShowerDigiSize_)++;
    }
  }

  // EMTFRegionalShowers from EMTF output
  for (int ibx = EMTFRegionalShowers_->getFirstBX(); ibx <= EMTFRegionalShowers_->getLastBX(); ++ibx) {
    for (l1t::RegionalMuonShowerBxCollection::const_iterator it = EMTFRegionalShowers_->begin(ibx); it != EMTFRegionalShowers_->end(ibx); it++) {
      if (it->isValid()) {
        EMTFRegionalShower_endcap_->push_back(it->trackFinderType() == l1t::tftype::emtf_pos ? 1 : -1);
        EMTFRegionalShower_sector_->push_back(it->processor() + 1);
        EMTFRegionalShower_bx_->push_back(ibx);
        EMTFRegionalShower_oneLooseInTime_->push_back(it->isOneLooseInTime());
        EMTFRegionalShower_twoLooseInTime_->push_back(it->isTwoLooseInTime());
        EMTFRegionalShower_oneNominalInTime_->push_back(it->isOneNominalInTime());
        EMTFRegionalShower_oneTightInTime_->push_back(it->isOneTightInTime());

        (*EMTFRegionalShowerSize_)++;
      }
    }
  }

  // GMTRegionalShowers from GMT input
  for (int ibx = GMTRegionalShowers_->getFirstBX(); ibx <= GMTRegionalShowers_->getLastBX(); ++ibx) {
    for (l1t::RegionalMuonShowerBxCollection::const_iterator it = GMTRegionalShowers_->begin(ibx); it != GMTRegionalShowers_->end(ibx); it++) {
      if (it->isValid()) {
        GMTRegionalShower_endcap_->push_back(it->trackFinderType() == l1t::tftype::emtf_pos ? 1 : -1);
        GMTRegionalShower_sector_->push_back(it->processor() + 1);
        GMTRegionalShower_bx_->push_back(ibx);
        GMTRegionalShower_oneLooseInTime_->push_back(it->isOneLooseInTime());
        GMTRegionalShower_twoLooseInTime_->push_back(it->isTwoLooseInTime());
        GMTRegionalShower_oneNominalInTime_->push_back(it->isOneNominalInTime());
        GMTRegionalShower_oneTightInTime_->push_back(it->isOneTightInTime());

        (*GMTRegionalShowerSize_)++;
      }
    }
  }

  // GMTShowers from GMT output
  for (int ibx = GMTShowers_->getFirstBX(); ibx <= GMTShowers_->getLastBX(); ++ibx) {
    for (l1t::MuonShowerBxCollection::const_iterator it = GMTShowers_->begin(ibx); it != GMTShowers_->end(ibx); it++) {
      if (it->isValid()) {
        GMTShower_bx_->push_back(ibx);
        // GMTShower_oneLooseInTime_->push_back(it->isOneLooseInTime());
        GMTShower_twoLooseDiffSectorsInTime_->push_back(it->isTwoLooseDiffSectorsInTime());
        GMTShower_oneNominalInTime_->push_back(it->isOneNominalInTime());
        GMTShower_oneTightInTime_->push_back(it->isOneTightInTime());
      }
    }
  }
}

// ------------ method called once each job just before starting event loop  ------------
void MuShowerNtuple::beginJob() {

  // initialize tree
  makeTree();

}

// ------------ method called once each job just after ending the event loop  ------------
void MuShowerNtuple::endJob() {
  // please remove this method if not needed
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void MuShowerNtuple::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

}

void MuShowerNtuple::getHandles(const edm::Event &iEvent,
                            const edm::EventSetup &iSetup) {

  // CSCShowerDigis from CSC output
  auto CSCShowerDigis_handle = make_handle(CSCShowerDigis_);

  if (useCSCShowerDigis_) {
      if (!CSCShowerDigiToken_.isUninitialized()) {
          iEvent.getByToken(CSCShowerDigiToken_, CSCShowerDigis_handle);
      }
      if (!CSCShowerDigis_handle.isValid()) {
          if (firstEvent_)
              edm::LogError("NtupleMaker")
                  << "Cannot get the product: " << CSCShowerDigiInputTag_;
          CSCShowerDigis_ = nullptr;
      } else {
          CSCShowerDigis_ = CSCShowerDigis_handle.product();
      }

  } else {
      CSCShowerDigis_ = nullptr;
  }

  // CSCShowerDigis from EMTF input
  auto EMTFShowerDigis_handle = make_handle(EMTFShowerDigis_);

  if(useEMTFShowerDigis_){
    if (!EMTFShowerDigiToken_.isUninitialized()) {
        iEvent.getByToken(EMTFShowerDigiToken_, EMTFShowerDigis_handle);
    }
    if (!EMTFShowerDigis_handle.isValid()) {
        if (firstEvent_)
            edm::LogError("NtupleMaker")
                << "Cannot get the product: " << EMTFShowerDigiInputTag_;
        EMTFShowerDigis_ = nullptr;
    } else {
        EMTFShowerDigis_ = EMTFShowerDigis_handle.product();
    }
  } else {
    EMTFShowerDigis_ = nullptr;
  }

  // EMTFRegionalShowers from EMTF output
  auto EMTFRegionalShower_handle = make_handle(EMTFRegionalShowers_);

  if(useEMTFRegionalShowers_){
    if (!EMTFRegionalShowerToken_.isUninitialized()) {
        iEvent.getByToken(EMTFRegionalShowerToken_, EMTFRegionalShower_handle);
    }
    if (!EMTFRegionalShower_handle.isValid()) {
        if (firstEvent_)
            edm::LogError("NtupleMaker")
                << "Cannot get the product: " << EMTFRegionalShowerInputTag_;
        EMTFRegionalShowers_ = nullptr;
    } else {
        EMTFRegionalShowers_ = EMTFRegionalShower_handle.product();
    }
  } else {
    EMTFRegionalShowers_ = nullptr;
  }

  // GMTRegionalShowers from GMT input
  auto GMTRegionalShower_handle = make_handle(GMTRegionalShowers_);

  if(useGMTRegionalShowers_){
    if (!GMTRegionalShowerToken_.isUninitialized()) {
        iEvent.getByToken(GMTRegionalShowerToken_, GMTRegionalShower_handle);
    }
    if (!GMTRegionalShower_handle.isValid()) {
        if (firstEvent_)
            edm::LogError("NtupleMaker")
                << "Cannot get the product: " << GMTRegionalShowerInputTag_;
        GMTRegionalShowers_ = nullptr;
    } else {
        GMTRegionalShowers_ = GMTRegionalShower_handle.product();
    }
  } else {
    GMTRegionalShowers_ = nullptr;
  }

  // GMTShowers from GMT output
  auto GMTShower_handle = make_handle(GMTShowers_);

  if(useGMTShowers_){
    if (!GMTShowerToken_.isUninitialized()) {
        iEvent.getByToken(GMTShowerToken_, GMTShower_handle);
    }
    if (!GMTShower_handle.isValid()) {
        if (firstEvent_)
            edm::LogError("NtupleMaker")
                << "Cannot get the product: " << GMTShowerInputTag_;
        GMTShowers_ = nullptr;
    } else {
        GMTShowers_ = GMTShower_handle.product();
    }
  } else {
    GMTShowers_ = nullptr;
  }

    // Fill

    fillTree();

}



void MuShowerNtuple::makeTree() {

  // TFileService
  edm::Service<TFileService> fs;
  tree = fs->make<TTree>("tree", "tree");

  // Create pointers
  // CSCShowerDigis from CSC output
  CSCShowerDigi_endcap_ = std::make_unique<std::vector<int16_t>>();
  CSCShowerDigi_station_ = std::make_unique<std::vector<int16_t>>();
  CSCShowerDigi_ring_ = std::make_unique<std::vector<int16_t>>();
  CSCShowerDigi_chamber_ = std::make_unique<std::vector<int16_t>>();
  CSCShowerDigi_bx_ = std::make_unique<std::vector<int16_t>>();
  CSCShowerDigi_oneLoose_ = std::make_unique<std::vector<int16_t>>();
  CSCShowerDigi_twoLoose_ = std::make_unique<std::vector<int16_t>>();
  CSCShowerDigi_oneNominal_ = std::make_unique<std::vector<int16_t>>();
  CSCShowerDigi_oneTight_ = std::make_unique<std::vector<int16_t>>();
  CSCShowerDigiSize_ = std::make_unique<int32_t>(0);

  // CSCShowerDigis from EMTF input
  EMTFShowerDigi_endcap_ = std::make_unique<std::vector<int16_t>>();
  EMTFShowerDigi_station_ = std::make_unique<std::vector<int16_t>>();
  EMTFShowerDigi_ring_ = std::make_unique<std::vector<int16_t>>();
  EMTFShowerDigi_chamber_ = std::make_unique<std::vector<int16_t>>();
  EMTFShowerDigi_bx_ = std::make_unique<std::vector<int16_t>>();
  EMTFShowerDigi_oneLoose_ = std::make_unique<std::vector<int16_t>>();
  EMTFShowerDigi_twoLoose_ = std::make_unique<std::vector<int16_t>>();
  EMTFShowerDigi_oneNominal_ = std::make_unique<std::vector<int16_t>>();
  EMTFShowerDigi_oneTight_ = std::make_unique<std::vector<int16_t>>();
  EMTFShowerDigiSize_ = std::make_unique<int32_t>(0);

  // EMTFRegionalShowers from EMTF output
  EMTFRegionalShower_endcap_ = std::make_unique<std::vector<int16_t>>();
  EMTFRegionalShower_sector_ = std::make_unique<std::vector<int16_t>>();
  EMTFRegionalShower_bx_ = std::make_unique<std::vector<int16_t>>();
  EMTFRegionalShower_oneLooseInTime_ = std::make_unique<std::vector<int16_t>>();
  EMTFRegionalShower_twoLooseInTime_ = std::make_unique<std::vector<int16_t>>();
  EMTFRegionalShower_oneNominalInTime_ = std::make_unique<std::vector<int16_t>>();
  EMTFRegionalShower_oneTightInTime_ = std::make_unique<std::vector<int16_t>>();
  EMTFRegionalShowerSize_ = std::make_unique<int32_t>(0);

  // GMTRegionalShowers from GMT input
  GMTRegionalShower_endcap_ = std::make_unique<std::vector<int16_t>>();
  GMTRegionalShower_sector_ = std::make_unique<std::vector<int16_t>>();
  GMTRegionalShower_bx_ = std::make_unique<std::vector<int16_t>>();
  GMTRegionalShower_oneLooseInTime_ = std::make_unique<std::vector<int16_t>>();
  GMTRegionalShower_twoLooseInTime_ = std::make_unique<std::vector<int16_t>>();
  GMTRegionalShower_oneNominalInTime_ = std::make_unique<std::vector<int16_t>>();
  GMTRegionalShower_oneTightInTime_ = std::make_unique<std::vector<int16_t>>();
  GMTRegionalShowerSize_ = std::make_unique<int32_t>(0);

  // GMTShowers from GMT output
  GMTShower_bx_ = std::make_unique<std::vector<int16_t>>();
  GMTShower_oneLooseInTime_ = std::make_unique<std::vector<int16_t>>();
  GMTShower_twoLooseDiffSectorsInTime_ = std::make_unique<std::vector<int16_t>>();
  GMTShower_oneNominalInTime_ = std::make_unique<std::vector<int16_t>>();
  GMTShower_oneTightInTime_ = std::make_unique<std::vector<int16_t>>();

  // Set branches
  if (useCSCShowerDigis_){
    tree->Branch("CSCShowerDigi_endcap", &(*CSCShowerDigi_endcap_));
    tree->Branch("CSCShowerDigi_station", &(*CSCShowerDigi_station_));
    tree->Branch("CSCShowerDigi_ring", &(*CSCShowerDigi_ring_));
    tree->Branch("CSCShowerDigi_chamber", &(*CSCShowerDigi_chamber_));
    tree->Branch("CSCShowerDigi_bx", &(*CSCShowerDigi_bx_));
    tree->Branch("CSCShowerDigi_oneLoose", &(*CSCShowerDigi_oneLoose_));
    tree->Branch("CSCShowerDigi_twoLoose", &(*CSCShowerDigi_twoLoose_));
    tree->Branch("CSCShowerDigi_oneNominal", &(*CSCShowerDigi_oneNominal_));
    tree->Branch("CSCShowerDigi_oneTight", &(*CSCShowerDigi_oneTight_));
    tree->Branch("CSCShowerDigiSize", &(*CSCShowerDigiSize_));
  }
  if (useEMTFShowerDigis_){
    tree->Branch("EMTFShowerDigi_endcap", &(*EMTFShowerDigi_endcap_));
    tree->Branch("EMTFShowerDigi_station", &(*EMTFShowerDigi_station_));
    tree->Branch("EMTFShowerDigi_ring", &(*EMTFShowerDigi_ring_));
    tree->Branch("EMTFShowerDigi_chamber", &(*EMTFShowerDigi_chamber_));
    tree->Branch("EMTFShowerDigi_bx", &(*EMTFShowerDigi_bx_));
    tree->Branch("EMTFShowerDigi_oneLoose", &(*EMTFShowerDigi_oneLoose_));
    tree->Branch("EMTFShowerDigi_twoLoose", &(*EMTFShowerDigi_twoLoose_));
    tree->Branch("EMTFShowerDigi_oneNominal", &(*EMTFShowerDigi_oneNominal_));
    tree->Branch("EMTFShowerDigi_oneTight", &(*EMTFShowerDigi_oneTight_));
    tree->Branch("EMTFShowerDigiSize", &(*EMTFShowerDigiSize_));
  }
  if(useEMTFRegionalShowers_){
    tree->Branch("EMTFRegionalShower_endcap", &(*EMTFRegionalShower_endcap_));
    tree->Branch("EMTFRegionalShower_sector", &(*EMTFRegionalShower_sector_));
    tree->Branch("EMTFRegionalShower_bx", &(*EMTFRegionalShower_bx_));
    tree->Branch("EMTFRegionalShower_oneLooseInTime", &(*EMTFRegionalShower_oneLooseInTime_));
    tree->Branch("EMTFRegionalShower_twoLooseInTime", &(*EMTFRegionalShower_twoLooseInTime_));
    tree->Branch("EMTFRegionalShower_oneNominalInTime", &(*EMTFRegionalShower_oneNominalInTime_));
    tree->Branch("EMTFRegionalShower_oneTightInTime", &(*EMTFRegionalShower_oneTightInTime_));
    tree->Branch("EMTFRegionalShowerSize", &(*EMTFRegionalShowerSize_));
  }
  if(useGMTRegionalShowers_){
    tree->Branch("GMTRegionalShower_endcap", &(*GMTRegionalShower_endcap_));
    tree->Branch("GMTRegionalShower_sector", &(*GMTRegionalShower_sector_));
    tree->Branch("GMTRegionalShower_bx", &(*GMTRegionalShower_bx_));
    tree->Branch("GMTRegionalShower_oneLooseInTime", &(*GMTRegionalShower_oneLooseInTime_));
    tree->Branch("GMTRegionalShower_twoLooseInTime", &(*GMTRegionalShower_twoLooseInTime_));
    tree->Branch("GMTRegionalShower_oneNominalInTime", &(*GMTRegionalShower_oneNominalInTime_));
    tree->Branch("GMTRegionalShower_oneTightInTime", &(*GMTRegionalShower_oneTightInTime_));
    tree->Branch("GMTRegionalShowerSize", &(*GMTRegionalShowerSize_));
  }
  if(useGMTShowers_){
    tree->Branch("GMTShower_bx", &(*GMTShower_bx_));
    tree->Branch("GMTShower_oneLooseInTime", &(*GMTShower_oneLooseInTime_));
    tree->Branch("GMTShower_twoLooseDiffSectorsInTime", &(*GMTShower_twoLooseDiffSectorsInTime_));
    tree->Branch("GMTShower_oneNominalInTime", &(*GMTShower_oneNominalInTime_));
    tree->Branch("GMTShower_oneTightInTime", &(*GMTShower_oneTightInTime_));
  }
}

void MuShowerNtuple::fillTree() {


  tree->Fill();

  // Clear objects
  // CSCShowerDigis from CSC output
  CSCShowerDigi_endcap_->clear();
  CSCShowerDigi_station_->clear();
  CSCShowerDigi_ring_->clear();
  CSCShowerDigi_chamber_->clear();
  CSCShowerDigi_bx_->clear();
  CSCShowerDigi_oneLoose_->clear();
  CSCShowerDigi_twoLoose_->clear();
  CSCShowerDigi_oneNominal_->clear();
  CSCShowerDigi_oneTight_->clear();
  (*CSCShowerDigiSize_) = 0;

  // CSCShowerDigis from EMTF input
  EMTFShowerDigi_endcap_->clear();
  EMTFShowerDigi_station_->clear();
  EMTFShowerDigi_ring_->clear();
  EMTFShowerDigi_chamber_->clear();
  EMTFShowerDigi_bx_->clear();
  EMTFShowerDigi_oneLoose_->clear();
  EMTFShowerDigi_twoLoose_->clear();
  EMTFShowerDigi_oneNominal_->clear();
  EMTFShowerDigi_oneTight_->clear();
  (*EMTFShowerDigiSize_) = 0;

  // EMTFRegionalShowers from EMTF output
  EMTFRegionalShower_endcap_->clear();
  EMTFRegionalShower_sector_->clear();
  EMTFRegionalShower_bx_->clear();
  EMTFRegionalShower_oneLooseInTime_->clear();
  EMTFRegionalShower_twoLooseInTime_->clear();
  EMTFRegionalShower_oneNominalInTime_->clear();
  EMTFRegionalShower_oneTightInTime_->clear();
  (*EMTFRegionalShowerSize_) = 0;

  // GMTRegionalShowers from GMT input
  GMTRegionalShower_endcap_->clear();
  GMTRegionalShower_sector_->clear();
  GMTRegionalShower_bx_->clear();
  GMTRegionalShower_oneLooseInTime_->clear();
  GMTRegionalShower_twoLooseInTime_->clear();
  GMTRegionalShower_oneNominalInTime_->clear();
  GMTRegionalShower_oneTightInTime_->clear();
  (*GMTRegionalShowerSize_) = 0;

  // GMTShowers from GMT output
  GMTShower_bx_->clear();
  GMTShower_oneLooseInTime_->clear();
  GMTShower_twoLooseDiffSectorsInTime_->clear();
  GMTShower_oneNominalInTime_->clear();
  GMTShower_oneTightInTime_->clear();


}


//define this as a plug-in
DEFINE_FWK_MODULE(MuShowerNtuple);
