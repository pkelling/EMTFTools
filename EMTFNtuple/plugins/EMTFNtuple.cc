// -*- C++ -*-
//
// Package:    EMTFTools/EMTFNtuple
// Class:      EMTFNtuple
//
/**\class EMTFNtuple EMTFNtuple.cc EMTFTools/EMTFNtuple/plugins/EMTFNtuple.cc

 Description: Creates flat ntuples to be used for EMTF studies.

*/
//
// Original Author:  Efe Yigitbasi
//         Created:  Tue, 01 Sep 2020 10:52:51 GMT
//
//

#include "EMTFTools/EMTFNtuple/plugins/EMTFNtuple.h"

EMTFNtuple::EMTFNtuple(const edm::ParameterSet& iConfig): 
  CSCInputTag_    (iConfig.getParameter<edm::InputTag>("CSCInputTag")),
  RPCInputTag_    (iConfig.getParameter<edm::InputTag>("RPCInputTag")),
  CPPFInputTag_   (iConfig.getParameter<edm::InputTag>("CPPFInputTag")),
  GEMInputTag_    (iConfig.getParameter<edm::InputTag>("GEMInputTag")),

  IRPCInputTag_   (iConfig.getParameter<edm::InputTag>("IRPCInputTag")),
  ME0InputTag_    (iConfig.getParameter<edm::InputTag>("ME0InputTag")),
  DTInputTag_     (iConfig.getParameter<edm::InputTag>("DTInputTag")),

  EMTFHitTag_     (iConfig.getParameter<edm::InputTag>("EMTFHitTag")),
  EMTFTrackTag_   (iConfig.getParameter<edm::InputTag>("EMTFTrackTag")),
  GMTMuonTag_     (iConfig.getParameter<edm::InputTag>("GMTMuonTag")),
  GENPartTag_     (iConfig.getParameter<edm::InputTag>("GENPartTag")),

  outFileName_    (iConfig.getParameter<std::string>("outFileName")),
  verbose_        (iConfig.getUntrackedParameter<int> ("verbosity")),
  enablePhase2_   (iConfig.getParameter<bool>("enablePhase2")),
  enableCPPF_     (iConfig.getParameter<bool>("enableCPPF"))

{
  usesResource("TFileService");  // shared resources

  firstEvent_ = true;

  CSCInputToken_    = consumes<emtf::CSCTag::digi_collection>     (CSCInputTag_);
  RPCInputToken_    = consumes<emtf::RPCTag::digi_collection>     (RPCInputTag_);
  CPPFInputToken_   = consumes<emtf::CPPFTag::digi_collection>    (CPPFInputTag_);
  GEMInputToken_    = consumes<emtf::GEMTag::digi_collection>     (GEMInputTag_);

  IRPCInputToken_   = consumes<emtf::IRPCTag::digi_collection>    (IRPCInputTag_);
  ME0InputToken_    = consumes<emtf::ME0Tag::digi_collection>     (ME0InputTag_);
  DTInputToken_     = consumes<emtf::DTTag::digi_collection>      (DTInputTag_);
  
  EMTFHitToken_     = consumes<l1t::EMTFHitCollection>            (EMTFHitTag_);
  EMTFTrackToken_   = consumes<l1t::EMTFTrackCollection>          (EMTFTrackTag_);
  GMTMuonToken_     = consumes<l1t::MuonBxCollection>             (GMTMuonTag_);
  GENPartToken_     = consumes<reco::GenParticleCollection>       (GENPartTag_);

}

EMTFNtuple::~EMTFNtuple() {}


// ------------ method called for each event  ------------
void EMTFNtuple::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  getHandles(iEvent, iSetup);


  // EMTF Hits
  for (const auto& hit : EMTFHits_) {

    emtfHit_endcap->push_back(hit.Endcap());
  }

  // EMTF Tracks
  for (const auto& trk : EMTFTracks_) {
    emtfTrack_pt->push_back(trk.Pt());
  }


  // GMT Muons
  for (const auto& muon : *GMTMuons_) {  
    gmtMuon_pt->push_back(muon.pt());
  }

  // Gen particles
  int igenPart = 0;
  for (const auto& part : *GENParts_) {
    genPart_pt ->push_back(part.pt());

  }

  // Event Info
  eventInfo_event->push_back(iEvent.id().event());

  // ___________________________________________________________________________
  // Fill
  tree->Fill();
  
  // Clear objects
  // CSC Inputs
  cscInput_endcap         ->clear();
  cscInput_station        ->clear();
  cscInput_ring           ->clear();
  cscInput_sector         ->clear();
  cscInput_subsector      ->clear();
  cscInput_chamber        ->clear();
  cscInput_cscid          ->clear();
  cscInput_bx             ->clear();
  cscInput_strip          ->clear();
  cscInput_wire           ->clear();
  cscInput_quality        ->clear();
  cscInput_pattern        ->clear();
  cscInput_bend           ->clear();

  // RPC Inputs
  rpcInput_region         ->clear();
  rpcInput_station        ->clear();
  rpcInput_ring           ->clear();
  rpcInput_sector         ->clear();
  rpcInput_subsector      ->clear();
  rpcInput_roll           ->clear();
  rpcInput_bx             ->clear();
  rpcInput_strip          ->clear();
  rpcInput_strip_high     ->clear();
  rpcInput_strip_low      ->clear();
  rpcInput_time           ->clear();
  rpcInput_valid          ->clear();

  // GEM Inputs
  gemInput_region         ->clear();
  gemInput_station        ->clear();
  gemInput_ring           ->clear();
  gemInput_sector         ->clear();
  gemInput_chamber        ->clear();
  gemInput_roll           ->clear();
  gemInput_bx             ->clear();
  gemInput_layer          ->clear();
  gemInput_pad            ->clear();
  gemInput_pad_low        ->clear();
  gemInput_pad_high       ->clear();

  // ME0 Inputs
  me0Input_region         ->clear();
  me0Input_station        ->clear();
  me0Input_ring           ->clear();
  me0Input_sector         ->clear();
  me0Input_chamber        ->clear();
  me0Input_roll           ->clear();
  me0Input_bx             ->clear();
  me0Input_layer          ->clear();
  me0Input_phiposition    ->clear();
  me0Input_deltaphi       ->clear();
  me0Input_quality        ->clear();
  me0Input_bend           ->clear();
  me0Input_partition      ->clear();

  // DT Inputs
  dtInput_wheel           ->clear();
  dtInput_station         ->clear();
  dtInput_btigroup        ->clear();
  dtInput_bx              ->clear();
  dtInput_strip           ->clear();
  dtInput_wire            ->clear();
  dtInput_quality         ->clear();
  dtInput_bend            ->clear();



  // EMTF Hits
  emtfHit_endcap          ->clear();
  emtfHit_station         ->clear();
  emtfHit_ring            ->clear();
  emtfHit_sector          ->clear();
  emtfHit_subsector       ->clear();
  emtfHit_chamber         ->clear();
  emtfHit_cscid           ->clear();
  emtfHit_bx              ->clear();
  emtfHit_type            ->clear();
  emtfHit_neighbor        ->clear();
  //
  emtfHit_strip           ->clear();
  emtfHit_wire            ->clear();
  emtfHit_roll            ->clear();
  emtfHit_quality         ->clear();
  emtfHit_pattern         ->clear();
  emtfHit_bend            ->clear();
  emtfHit_time            ->clear();
  emtfHit_fr              ->clear();
  emtfHit_emtf_phi        ->clear();
  emtfHit_emtf_theta      ->clear();
  //
  emtfHit_sim_phi         ->clear();
  emtfHit_sim_theta       ->clear();
  emtfHit_sim_r           ->clear();
  emtfHit_sim_z           ->clear();
  emtfHit_sim_tp1         ->clear();
  emtfHit_sim_tp2         ->clear();
  (*emtfHit_size)         = 0;

  // EMTF Tracks
  emtfTrack_pt            ->clear();
  emtfTrack_xml_pt        ->clear();
  emtfTrack_pt_dxy        ->clear();
  emtfTrack_dxy           ->clear();
  emtfTrack_invpt_prompt  ->clear();
  emtfTrack_invpt_displ   ->clear();
  emtfTrack_phi           ->clear();
  emtfTrack_theta         ->clear();
  emtfTrack_eta           ->clear();
  emtfTrack_q             ->clear();
  //
  emtfTrack_address       ->clear();
  emtfTrack_mode          ->clear();
  emtfTrack_endcap        ->clear();
  emtfTrack_sector        ->clear();
  emtfTrack_bx            ->clear();
  emtfTrack_nhits         ->clear();
  emtfTrack_hitref1       ->clear();
  emtfTrack_hitref2       ->clear();
  emtfTrack_hitref3       ->clear();
  emtfTrack_hitref4       ->clear();
  (*emtfTrack_size)       = 0;

  // GMT Muons
  gmtMuon_pt              ->clear();
  gmtMuon_pt_dxy          ->clear();
  gmtMuon_dxy             ->clear();
  gmtMuon_phi             ->clear();
  gmtMuon_eta             ->clear();
  gmtMuon_q               ->clear();
  gmtMuon_qual            ->clear();
  (*gmtMuon_size)         = 0;

  // GEN particles
  genPart_pt              ->clear();
  genPart_dxy             ->clear();
  genPart_eta             ->clear();
  genPart_phi             ->clear();
  genPart_q               ->clear();
  genPart_ID              ->clear();
  genPart_vx              ->clear();
  genPart_vy              ->clear();
  genPart_vz              ->clear();

  // Event info
  eventInfo_event         ->clear();
  eventInfo_run           ->clear();
  eventInfo_lumi          ->clear();
  eventInfo_npv           ->clear();
  eventInfo_nvtx          ->clear();
  (*eventInfo_size)       = 0;


}












// ------------ method called once each job just before starting event loop  ------------
void EMTFNtuple::beginJob() {
  makeTree();
}

// ------------ method called once each job just after ending the event loop  ------------
void EMTFNtuple::endJob() {}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void EMTFNtuple::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

}

void EMTFNtuple::getHandles(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  // EMTF hits and tracks
  auto EMTFHits_handle = make_handle(EMTFHits_);
  auto EMTFTracks_handle = make_handle(EMTFTracks_);

  if (!EMTFHitToken_.isUninitialized()) {
    iEvent.getByToken(EMTFHitToken_, EMTFHits_handle);
  }
  if (!EMTFHits_handle.isValid()) {
    if (firstEvent_)  edm::LogError("NtupleMaker") << "Cannot get the product: " << EMTFHitTag_;
  }
  else{
    EMTFHits_.clear();
    for (const auto& hit : (*EMTFHits_handle)) {
      // if (!(-2 <= hit.BX() && hit.BX() <= 2))  continue;  // only BX=[-2,+2]
      EMTFHits_.push_back(hit);
    }
  }

  if (!EMTFTrackToken_.isUninitialized()) {
    iEvent.getByToken(EMTFTrackToken_, EMTFTracks_handle);
  }
  if (!EMTFTracks_handle.isValid()) {
    if (firstEvent_)  edm::LogError("NtupleMaker") << "Cannot get the product: " << EMTFTrackTag_;
  }
  else{
    EMTFTracks_.clear();
    for (const auto& track : (*EMTFTracks_handle)) {
      // if (trk.BX() != 0)      continue;  // only BX=0
      EMTFTracks_.push_back(track);
    }
  }

  // GMT muons
  auto GMTMuons_handle = make_handle(GMTMuons_);
  auto GENParts_handle = make_handle(GENParts_);

  if (!GMTMuonToken_.isUninitialized()) {
    iEvent.getByToken(GMTMuonToken_, GMTMuons_handle);
  }
  if (!GMTMuons_handle.isValid()) {
    if (firstEvent_)  edm::LogError("NtupleMaker") << "Cannot get the product: " << GMTMuonTag_;
  }
  else{
    GMTMuons_ = GMTMuons_handle.product();
  }

  if (!iEvent.isRealData()) {
    if (!GENPartToken_.isUninitialized()) {
      iEvent.getByToken(GENPartToken_, GENParts_handle);
    }
    if (!GENParts_handle.isValid()) {
      if (firstEvent_)  edm::LogError("NtupleMaker") << "Cannot get the product: " << GENPartTag_;
      GENParts_ = nullptr;
    } else {
      GENParts_ = GENParts_handle.product();
    }
  }


}

void EMTFNtuple::makeTree() {

  // TFileService
  edm::Service<TFileService> fs;
  tree = fs->make<TTree>("tree", "tree");

  // Create pointers
  // CSC Inputs
  cscInput_endcap         = std::make_unique<std::vector<int16_t> >();
  cscInput_station        = std::make_unique<std::vector<int16_t> >();
  cscInput_ring           = std::make_unique<std::vector<int16_t> >();
  cscInput_sector         = std::make_unique<std::vector<int16_t> >();
  cscInput_subsector      = std::make_unique<std::vector<int16_t> >();
  cscInput_chamber        = std::make_unique<std::vector<int16_t> >();
  cscInput_cscid          = std::make_unique<std::vector<int16_t> >();
  cscInput_bx             = std::make_unique<std::vector<int16_t> >();
  cscInput_strip          = std::make_unique<std::vector<int16_t> >();
  cscInput_wire           = std::make_unique<std::vector<int16_t> >();
  cscInput_quality        = std::make_unique<std::vector<int16_t> >();
  cscInput_pattern        = std::make_unique<std::vector<int16_t> >();
  cscInput_bend           = std::make_unique<std::vector<int16_t> >();

  // RPC Inputs
  rpcInput_region         = std::make_unique<std::vector<int16_t> >();
  rpcInput_station        = std::make_unique<std::vector<int16_t> >();
  rpcInput_ring           = std::make_unique<std::vector<int16_t> >();
  rpcInput_sector         = std::make_unique<std::vector<int16_t> >();
  rpcInput_subsector      = std::make_unique<std::vector<int16_t> >();
  rpcInput_roll           = std::make_unique<std::vector<int16_t> >();
  rpcInput_bx             = std::make_unique<std::vector<int16_t> >();
  rpcInput_strip          = std::make_unique<std::vector<int16_t> >();
  rpcInput_strip_high     = std::make_unique<std::vector<int16_t> >();
  rpcInput_strip_low      = std::make_unique<std::vector<int16_t> >();
  rpcInput_time           = std::make_unique<std::vector<int16_t> >();
  rpcInput_valid          = std::make_unique<std::vector<int16_t> >();

  // GEM Inputs
  gemInput_region         = std::make_unique<std::vector<int16_t> >();
  gemInput_station        = std::make_unique<std::vector<int16_t> >();
  gemInput_ring           = std::make_unique<std::vector<int16_t> >();
  gemInput_sector         = std::make_unique<std::vector<int16_t> >();
  gemInput_chamber        = std::make_unique<std::vector<int16_t> >();
  gemInput_roll           = std::make_unique<std::vector<int16_t> >();
  gemInput_bx             = std::make_unique<std::vector<int16_t> >();
  gemInput_layer          = std::make_unique<std::vector<int16_t> >();
  gemInput_pad            = std::make_unique<std::vector<int16_t> >();
  gemInput_pad_low        = std::make_unique<std::vector<int16_t> >();
  gemInput_pad_high       = std::make_unique<std::vector<int16_t> >();

  // ME0 Inputs
  me0Input_region         = std::make_unique<std::vector<int16_t> >();
  me0Input_station        = std::make_unique<std::vector<int16_t> >();
  me0Input_ring           = std::make_unique<std::vector<int16_t> >();
  me0Input_sector         = std::make_unique<std::vector<int16_t> >();
  me0Input_chamber        = std::make_unique<std::vector<int16_t> >();
  me0Input_roll           = std::make_unique<std::vector<int16_t> >();
  me0Input_bx             = std::make_unique<std::vector<int16_t> >();
  me0Input_layer          = std::make_unique<std::vector<int16_t> >();
  me0Input_phiposition    = std::make_unique<std::vector<int16_t> >();
  me0Input_deltaphi       = std::make_unique<std::vector<int16_t> >();
  me0Input_quality        = std::make_unique<std::vector<int16_t> >();
  me0Input_bend           = std::make_unique<std::vector<int16_t> >();
  me0Input_partition      = std::make_unique<std::vector<int16_t> >();

  // DT Inputs
  dtInput_wheel           = std::make_unique<std::vector<int16_t> >();
  dtInput_station         = std::make_unique<std::vector<int16_t> >();
  dtInput_btigroup        = std::make_unique<std::vector<int16_t> >();
  dtInput_bx              = std::make_unique<std::vector<int16_t> >();
  dtInput_strip           = std::make_unique<std::vector<int16_t> >();
  dtInput_wire            = std::make_unique<std::vector<int16_t> >();
  dtInput_quality         = std::make_unique<std::vector<int16_t> >();
  dtInput_bend            = std::make_unique<std::vector<int16_t> >();



  // EMTF Hits
  emtfHit_endcap          = std::make_unique<std::vector<int16_t> >();
  emtfHit_station         = std::make_unique<std::vector<int16_t> >();
  emtfHit_ring            = std::make_unique<std::vector<int16_t> >();
  emtfHit_sector          = std::make_unique<std::vector<int16_t> >();
  emtfHit_subsector       = std::make_unique<std::vector<int16_t> >();
  emtfHit_chamber         = std::make_unique<std::vector<int16_t> >();
  emtfHit_cscid           = std::make_unique<std::vector<int16_t> >();
  emtfHit_bx              = std::make_unique<std::vector<int16_t> >();
  emtfHit_type            = std::make_unique<std::vector<int16_t> >();  // subsystem: DT=0,CSC=1,RPC=2,GEM=3,ME0=4
  emtfHit_neighbor        = std::make_unique<std::vector<int16_t> >();
  //
  emtfHit_strip           = std::make_unique<std::vector<int16_t> >();
  emtfHit_wire            = std::make_unique<std::vector<int16_t> >();
  emtfHit_roll            = std::make_unique<std::vector<int16_t> >();
  emtfHit_quality         = std::make_unique<std::vector<int16_t> >();
  emtfHit_pattern         = std::make_unique<std::vector<int16_t> >();
  emtfHit_bend            = std::make_unique<std::vector<int16_t> >();
  emtfHit_time            = std::make_unique<std::vector<int16_t> >();
  emtfHit_fr              = std::make_unique<std::vector<int16_t> >();
  emtfHit_emtf_phi        = std::make_unique<std::vector<int32_t> >();   // integer unit
  emtfHit_emtf_theta      = std::make_unique<std::vector<int32_t> >(); // integer unit
  //
  emtfHit_sim_phi         = std::make_unique<std::vector<float  > >();
  emtfHit_sim_theta       = std::make_unique<std::vector<float  > >();
  emtfHit_sim_r           = std::make_unique<std::vector<float  > >();
  emtfHit_sim_z           = std::make_unique<std::vector<float  > >();
  emtfHit_sim_tp1         = std::make_unique<std::vector<int32_t> >();
  emtfHit_sim_tp2         = std::make_unique<std::vector<int32_t> >();
  emtfHit_size            = std::make_unique<int32_t>(0);

  // EMTF Tracks
  emtfTrack_pt            = std::make_unique<std::vector<float> >();
  emtfTrack_xml_pt        = std::make_unique<std::vector<float> >();
  emtfTrack_pt_dxy        = std::make_unique<std::vector<float> >();
  emtfTrack_dxy           = std::make_unique<std::vector<float> >();
  emtfTrack_invpt_prompt  = std::make_unique<std::vector<float> >();
  emtfTrack_invpt_displ   = std::make_unique<std::vector<float> >();
  emtfTrack_phi           = std::make_unique<std::vector<float> >();        // in degrees
  emtfTrack_theta         = std::make_unique<std::vector<float> >();      // in degrees
  emtfTrack_eta           = std::make_unique<std::vector<float> >();
  emtfTrack_q             = std::make_unique<std::vector<int16_t> >();          // charge
  //
  emtfTrack_address       = std::make_unique<std::vector<uint64_t> >();
  emtfTrack_mode          = std::make_unique<std::vector<int16_t> >();
  emtfTrack_endcap        = std::make_unique<std::vector<int16_t> >();
  emtfTrack_sector        = std::make_unique<std::vector<int16_t> >();
  emtfTrack_bx            = std::make_unique<std::vector<int16_t> >();
  emtfTrack_nhits         = std::make_unique<std::vector<int16_t> >();
  emtfTrack_hitref1       = std::make_unique<std::vector<int32_t> >();
  emtfTrack_hitref2       = std::make_unique<std::vector<int32_t> >();
  emtfTrack_hitref3       = std::make_unique<std::vector<int32_t> >();
  emtfTrack_hitref4       = std::make_unique<std::vector<int32_t> >();
  emtfTrack_size          = std::make_unique<int32_t>(0);

  // GMT Muons
  gmtMuon_pt              = std::make_unique<std::vector<float> >();
  gmtMuon_pt_dxy          = std::make_unique<std::vector<float> >();
  gmtMuon_dxy             = std::make_unique<std::vector<int16_t> >();
  gmtMuon_phi             = std::make_unique<std::vector<float> >();        // in degrees
  gmtMuon_eta             = std::make_unique<std::vector<float> >();
  gmtMuon_q               = std::make_unique<std::vector<int16_t> >();          // charge
  gmtMuon_qual            = std::make_unique<std::vector<int16_t> >();      
  gmtMuon_size            = std::make_unique<int32_t>(0);      

  // GEN particles
  genPart_pt              = std::make_unique<std::vector<float> >();
  genPart_dxy             = std::make_unique<std::vector<float> >();
  genPart_eta             = std::make_unique<std::vector<float> >();
  genPart_phi             = std::make_unique<std::vector<float> >();
  genPart_q               = std::make_unique<std::vector<int16_t> >();          // charge
  genPart_ID              = std::make_unique<std::vector<int16_t> >();      
  genPart_vx              = std::make_unique<std::vector<float> >();      
  genPart_vy              = std::make_unique<std::vector<float> >();      
  genPart_vz              = std::make_unique<std::vector<float> >();      

  // Event info
  eventInfo_event         = std::make_unique<std::vector<uint64_t> >();
  eventInfo_run           = std::make_unique<std::vector<uint32_t> >();
  eventInfo_lumi          = std::make_unique<std::vector<uint32_t> >();
  eventInfo_npv           = std::make_unique<std::vector<float> >();  // getTrueNumInteractions()
  eventInfo_nvtx          = std::make_unique<std::vector<int32_t> >(); // getPU_NumInteractions()
  eventInfo_size          = std::make_unique<int32_t>(0);

  // Set branches

  // CSC Inputs
  tree->Branch("cscInput_endcap"    , &(*cscInput_endcap    ));
  tree->Branch("cscInput_station"   , &(*cscInput_station   ));
  tree->Branch("cscInput_ring"      , &(*cscInput_ring      ));
  tree->Branch("cscInput_sector"    , &(*cscInput_sector    ));
  tree->Branch("cscInput_subsector" , &(*cscInput_subsector ));
  tree->Branch("cscInput_chamber"   , &(*cscInput_chamber   ));
  tree->Branch("cscInput_cscid"     , &(*cscInput_cscid     ));
  tree->Branch("cscInput_bx"        , &(*cscInput_bx        ));
  tree->Branch("cscInput_strip"     , &(*cscInput_strip     ));
  tree->Branch("cscInput_wire"      , &(*cscInput_wire      ));
  tree->Branch("cscInput_quality"   , &(*cscInput_quality   ));
  tree->Branch("cscInput_pattern"   , &(*cscInput_pattern   ));
  tree->Branch("cscInput_bend"      , &(*cscInput_bend      ));

  // RPC Inputs
  tree->Branch("rpcInput_region"    , &(*rpcInput_region    ));
  tree->Branch("rpcInput_station"   , &(*rpcInput_station   ));
  tree->Branch("rpcInput_ring"      , &(*rpcInput_ring      ));
  tree->Branch("rpcInput_sector"    , &(*rpcInput_sector    ));
  tree->Branch("rpcInput_subsector" , &(*rpcInput_subsector ));
  tree->Branch("rpcInput_roll"      , &(*rpcInput_roll      ));
  tree->Branch("rpcInput_bx"        , &(*rpcInput_bx        ));
  tree->Branch("rpcInput_strip"     , &(*rpcInput_strip     ));
  tree->Branch("rpcInput_strip_high", &(*rpcInput_strip_high));
  tree->Branch("rpcInput_strip_low" , &(*rpcInput_strip_low ));
  tree->Branch("rpcInput_time"      , &(*rpcInput_time      ));
  tree->Branch("rpcInput_valid"     , &(*rpcInput_valid     ));

  // GEM Inputs
  tree->Branch("gemInput_region"    , &(*gemInput_region    ));
  tree->Branch("gemInput_station"   , &(*gemInput_station   ));
  tree->Branch("gemInput_ring"      , &(*gemInput_ring      ));
  tree->Branch("gemInput_sector"    , &(*gemInput_sector    ));
  tree->Branch("gemInput_chamber"   , &(*gemInput_chamber   ));
  tree->Branch("gemInput_roll"      , &(*gemInput_roll      ));
  tree->Branch("gemInput_bx"        , &(*gemInput_bx        ));
  tree->Branch("gemInput_layer"     , &(*gemInput_layer     ));
  tree->Branch("gemInput_pad"       , &(*gemInput_pad       ));
  tree->Branch("gemInput_pad_high"  , &(*gemInput_pad_high  ));
  tree->Branch("gemInput_pad_low"   , &(*gemInput_pad_low   ));


  if (enablePhase2_){
    // ME0 Inputs
    tree->Branch("me0Input_region"     , &(*me0Input_region     ));
    tree->Branch("me0Input_station"    , &(*me0Input_station    ));
    tree->Branch("me0Input_ring"       , &(*me0Input_ring       ));
    tree->Branch("me0Input_sector"     , &(*me0Input_sector     ));
    tree->Branch("me0Input_chamber"    , &(*me0Input_chamber    ));
    tree->Branch("me0Input_roll"       , &(*me0Input_roll       ));
    tree->Branch("me0Input_bx"         , &(*me0Input_bx         ));
    tree->Branch("me0Input_layer"      , &(*me0Input_layer      ));
    tree->Branch("me0Input_phiposition", &(*me0Input_phiposition));
    tree->Branch("me0Input_deltaphi"   , &(*me0Input_deltaphi   ));
    tree->Branch("me0Input_quality"    , &(*me0Input_quality    ));
    tree->Branch("me0Input_bend"       , &(*me0Input_bend       ));
    tree->Branch("me0Input_partition"  , &(*me0Input_partition  ));

    // DT Inputs
    tree->Branch("dtInput_wheel"     , &(*dtInput_wheel     ));
    tree->Branch("dtInput_station"   , &(*dtInput_station   ));
    tree->Branch("dtInput_btigroup"  , &(*dtInput_btigroup  ));
    tree->Branch("dtInput_bx"        , &(*dtInput_bx        ));
    tree->Branch("dtInput_strip"     , &(*dtInput_strip     ));
    tree->Branch("dtInput_wire"      , &(*dtInput_wire      ));
    tree->Branch("dtInput_quality"   , &(*dtInput_quality   ));
    tree->Branch("dtInput_bend"      , &(*dtInput_bend      ));
  }




  // EMTF Hits
  tree->Branch("emtfHit_endcap"    , &(*emtfHit_endcap    ));
  tree->Branch("emtfHit_station"   , &(*emtfHit_station   ));
  tree->Branch("emtfHit_ring"      , &(*emtfHit_ring      ));
  tree->Branch("emtfHit_sector"    , &(*emtfHit_sector    ));
  tree->Branch("emtfHit_subsector" , &(*emtfHit_subsector ));
  tree->Branch("emtfHit_chamber"   , &(*emtfHit_chamber   ));
  tree->Branch("emtfHit_cscid"     , &(*emtfHit_cscid     ));
  tree->Branch("emtfHit_bx"        , &(*emtfHit_bx        ));
  tree->Branch("emtfHit_type"      , &(*emtfHit_type      ));
  tree->Branch("emtfHit_neighbor"  , &(*emtfHit_neighbor  ));
  //
  tree->Branch("emtfHit_strip"     , &(*emtfHit_strip     ));
  tree->Branch("emtfHit_wire"      , &(*emtfHit_wire      ));
  tree->Branch("emtfHit_roll"      , &(*emtfHit_roll      ));
  tree->Branch("emtfHit_quality"   , &(*emtfHit_quality   ));
  tree->Branch("emtfHit_pattern"   , &(*emtfHit_pattern   ));
  tree->Branch("emtfHit_bend"      , &(*emtfHit_bend      ));
  tree->Branch("emtfHit_time"      , &(*emtfHit_time      ));
  tree->Branch("emtfHit_fr"        , &(*emtfHit_fr        ));
  tree->Branch("emtfHit_emtf_phi"  , &(*emtfHit_emtf_phi  ));
  tree->Branch("emtfHit_emtf_theta", &(*emtfHit_emtf_theta));
  //
  tree->Branch("emtfHit_sim_phi"   , &(*emtfHit_sim_phi   ));
  tree->Branch("emtfHit_sim_theta" , &(*emtfHit_sim_theta ));
  tree->Branch("emtfHit_sim_r"     , &(*emtfHit_sim_r     ));
  tree->Branch("emtfHit_sim_z"     , &(*emtfHit_sim_z     ));
  tree->Branch("emtfHit_sim_tp1"   , &(*emtfHit_sim_tp1   ));
  tree->Branch("emtfHit_sim_tp2"   , &(*emtfHit_sim_tp2   ));
  tree->Branch("emtfHit_size"      , &(*emtfHit_size      ));

  // EMTF Tracks
  tree->Branch("emtfTrack_pt"        , &(*emtfTrack_pt        ));
  tree->Branch("emtfTrack_xml_pt"    , &(*emtfTrack_xml_pt    ));
  tree->Branch("emtfTrack_pt_dxy"    , &(*emtfTrack_pt_dxy    ));
  tree->Branch("emtfTrack_dxy"       , &(*emtfTrack_dxy       ));
  tree->Branch("emtfTrack_invpt_prompt", &(*emtfTrack_invpt_prompt));
  tree->Branch("emtfTrack_invpt_displ" , &(*emtfTrack_invpt_displ ));
  tree->Branch("emtfTrack_phi"       , &(*emtfTrack_phi       ));
  tree->Branch("emtfTrack_theta"     , &(*emtfTrack_theta     ));
  tree->Branch("emtfTrack_eta"       , &(*emtfTrack_eta       ));
  tree->Branch("emtfTrack_q"         , &(*emtfTrack_q         ));
  //
  tree->Branch("emtfTrack_address"   , &(*emtfTrack_address   ));
  tree->Branch("emtfTrack_mode"      , &(*emtfTrack_mode      ));
  tree->Branch("emtfTrack_endcap"    , &(*emtfTrack_endcap    ));
  tree->Branch("emtfTrack_sector"    , &(*emtfTrack_sector    ));
  tree->Branch("emtfTrack_bx"        , &(*emtfTrack_bx        ));
  tree->Branch("emtfTrack_nhits"     , &(*emtfTrack_nhits     ));
  tree->Branch("emtfTrack_hitref1"   , &(*emtfTrack_hitref1   ));
  tree->Branch("emtfTrack_hitref2"   , &(*emtfTrack_hitref2   ));
  tree->Branch("emtfTrack_hitref3"   , &(*emtfTrack_hitref3   ));
  tree->Branch("emtfTrack_hitref4"   , &(*emtfTrack_hitref4   ));
  tree->Branch("emtfTrack_size"      , &(*emtfTrack_size      ));

  // GMT muons
  tree->Branch("gmtMuon_pt"        , &(*gmtMuon_pt        ));
  tree->Branch("gmtMuon_pt_dxy"    , &(*gmtMuon_pt_dxy    ));
  tree->Branch("gmtMuon_dxy"       , &(*gmtMuon_dxy       ));
  tree->Branch("gmtMuon_phi"       , &(*gmtMuon_phi       ));
  tree->Branch("gmtMuon_eta"       , &(*gmtMuon_eta       ));
  tree->Branch("gmtMuon_q"         , &(*gmtMuon_q         ));
  tree->Branch("gmtMuon_qual"      , &(*gmtMuon_qual      ));
  tree->Branch("gmtMuon_size"      , &(*gmtMuon_size      ));

  // GEN particles
  tree->Branch("genPart_pt"        , &(*genPart_pt        ));
  tree->Branch("genPart_dxy"       , &(*genPart_dxy       ));
  tree->Branch("genPart_phi"       , &(*genPart_phi       ));
  tree->Branch("genPart_eta"       , &(*genPart_eta       ));
  tree->Branch("genPart_q"         , &(*genPart_q         ));
  tree->Branch("genPart_ID"        , &(*genPart_ID        ));
  tree->Branch("genPart_vx"        , &(*genPart_vx        ));
  tree->Branch("genPart_vy"        , &(*genPart_vy        ));
  tree->Branch("genPart_vz"        , &(*genPart_vz        ));

  // Event info
  tree->Branch("eventInfo_event"     , &(*eventInfo_event     ));
  tree->Branch("eventInfo_run"       , &(*eventInfo_run       ));
  tree->Branch("eventInfo_lumi"      , &(*eventInfo_lumi      ));
  tree->Branch("eventInfo_npv"       , &(*eventInfo_npv       ));
  tree->Branch("eventInfo_nvtx"      , &(*eventInfo_nvtx      ));
  tree->Branch("eventInfo_size"      , &(*eventInfo_size      ));

}

// -------------------------------------------------------
// define this as a plug-in
DEFINE_FWK_MODULE(EMTFNtuple);
