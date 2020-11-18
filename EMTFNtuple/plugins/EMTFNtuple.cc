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
  CSCInputTag_        (iConfig.getParameter<edm::InputTag>("CSCInputTag")),
  RPCInputTag_        (iConfig.getParameter<edm::InputTag>("RPCInputTag")),
  CPPFInputTag_       (iConfig.getParameter<edm::InputTag>("CPPFInputTag")),
  GEMInputTag_        (iConfig.getParameter<edm::InputTag>("GEMInputTag")),

  IRPCInputTag_       (iConfig.getParameter<edm::InputTag>("IRPCInputTag")),
  ME0InputTag_        (iConfig.getParameter<edm::InputTag>("ME0InputTag")),
  DTInputTag_         (iConfig.getParameter<edm::InputTag>("DTInputTag")),

  EMTFHitTag_         (iConfig.getParameter<edm::InputTag>("EMTFHitTag")),
  EMTFUnpHitTag_      (iConfig.getParameter<edm::InputTag>("EMTFUnpHitTag")),
  
  EMTFTrackTag_       (iConfig.getParameter<edm::InputTag>("EMTFTrackTag")),
  EMTFUnpTrackTag_    (iConfig.getParameter<edm::InputTag>("EMTFUnpTrackTag")),
  
  GMTMuonTag_         (iConfig.getParameter<edm::InputTag>("GMTMuonTag")),
  GMTUnpMuonTag_      (iConfig.getParameter<edm::InputTag>("GMTUnpMuonTag")),

  GENPartTag_         (iConfig.getParameter<edm::InputTag>("GENPartTag")),

  outFileName_        (iConfig.getParameter<std::string>("outFileName")),
  verbose_            (iConfig.getUntrackedParameter<int> ("verbosity")),

  enablePhase2_       (iConfig.getParameter<bool>("enablePhase2")),

  useCSC_             (iConfig.getParameter<bool>("useCSC")),
  useRPC_             (iConfig.getParameter<bool>("useRPC")),
  useCPPF_            (iConfig.getParameter<bool>("useCPPF")),
  useGEM_             (iConfig.getParameter<bool>("useGEM")),

  useIRPC_            (iConfig.getParameter<bool>("useIRPC")),
  useME0_             (iConfig.getParameter<bool>("useME0")),
  useDT_              (iConfig.getParameter<bool>("useDT")),

  useEMTFHits_        (iConfig.getParameter<bool>("useEMTFHits")),
  useEMTFUnpHits_     (iConfig.getParameter<bool>("useEMTFUnpHits")),

  useEMTFTracks_      (iConfig.getParameter<bool>("useEMTFTracks")),
  useEMTFUnpTracks_   (iConfig.getParameter<bool>("useEMTFUnpTracks")),

  useGMTMuons_        (iConfig.getParameter<bool>("useGMTMuons")),
  useGMTUnpMuons_     (iConfig.getParameter<bool>("useGMTUnpMuons")),

  useGENParts_        (iConfig.getParameter<bool>("useGENParts")),
  useEventInfo_       (iConfig.getParameter<bool>("useEventInfo"))

{
  usesResource("TFileService");  // shared resources

  firstEvent_ = true;

  CSCInputToken_       = consumes<emtf::CSCTag::digi_collection>     (CSCInputTag_);
  RPCInputToken_       = consumes<emtf::RPCTag::digi_collection>     (RPCInputTag_);
  CPPFInputToken_      = consumes<emtf::CPPFTag::digi_collection>    (CPPFInputTag_);
  GEMInputToken_       = consumes<emtf::GEMTag::digi_collection>     (GEMInputTag_);

  IRPCInputToken_      = consumes<emtf::IRPCTag::digi_collection>    (IRPCInputTag_);
  ME0InputToken_       = consumes<emtf::ME0Tag::digi_collection>     (ME0InputTag_);
  DTInputToken_        = consumes<emtf::DTTag::digi_collection>      (DTInputTag_);
  
  EMTFHitToken_        = consumes<l1t::EMTFHitCollection>            (EMTFHitTag_);
  EMTFUnpHitToken_     = consumes<l1t::EMTFHitCollection>            (EMTFUnpHitTag_);

  EMTFTrackToken_      = consumes<l1t::EMTFTrackCollection>          (EMTFTrackTag_);
  EMTFUnpTrackToken_   = consumes<l1t::EMTFTrackCollection>          (EMTFUnpTrackTag_);

  GMTMuonToken_        = consumes<l1t::MuonBxCollection>             (GMTMuonTag_);
  GMTUnpMuonToken_     = consumes<l1t::MuonBxCollection>             (GMTUnpMuonTag_);

  GENPartToken_        = consumes<reco::GenParticleCollection>       (GENPartTag_);

}

EMTFNtuple::~EMTFNtuple() {}


// ------------ method called for each event  ------------
void EMTFNtuple::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  // ___________________________________________________________________________
  // Get Handles

  getHandles(iEvent, iSetup);


  if (verbose_ > 0) std::cout << "******* Processing Objects *******" << std::endl;
  
  // ___________________________________________________________________________
  // Process objects

  // Run 3 inputs

  // CSC inputs
  if (useCSC_) {
    for (const auto& tp : CSCInputs_) {
      if (tp.subsystem() == L1TMuon::kCSC) {
        const CSCDetId& tp_detId = tp.detId<CSCDetId>();
        const CSCData& tp_data = tp.getCSCData();

        cscInput_bx                ->push_back(tp.getBX());
        cscInput_endcap            ->push_back(tp_detId.endcap());
        cscInput_station           ->push_back(tp_detId.station());
        cscInput_ring              ->push_back(tp_detId.ring());
        cscInput_sector            ->push_back(tp_detId.triggerSector());
        cscInput_chamber           ->push_back(tp_detId.chamber());
        cscInput_cscid             ->push_back(tp_data.cscID);
        cscInput_strip             ->push_back(tp.getStrip());
        cscInput_wire              ->push_back(tp.getWire());
        cscInput_quality           ->push_back(tp_data.quality);
        cscInput_pattern           ->push_back(tp_data.pattern);
        cscInput_bend              ->push_back(tp_data.bend);
      }
    }  // end for loop
  }  // end if

  if (useRPC_) {
    for (const auto& tp : RPCInputs_) {
      if (tp.subsystem() == L1TMuon::kRPC) {
        const RPCDetId& tp_detId = tp.detId<RPCDetId>();
        const RPCData& tp_data = tp.getRPCData();

        rpcInput_bx                ->push_back(tp.getBX());
        rpcInput_region            ->push_back(tp_detId.region());
        rpcInput_station           ->push_back(tp_detId.station());
        rpcInput_ring              ->push_back(tp_detId.ring());
        rpcInput_sector            ->push_back(tp_detId.sector());
        rpcInput_subsector         ->push_back(tp_detId.subsector());
        rpcInput_roll              ->push_back(tp_detId.roll());
        rpcInput_strip             ->push_back(tp.getStrip());
        rpcInput_strip_high        ->push_back(tp_data.strip_hi);
        rpcInput_strip_low         ->push_back(tp_data.strip_low);
        rpcInput_time              ->push_back(tp_data.time);
        rpcInput_valid             ->push_back(tp_data.valid);
      }
    } // end for loop
  } // end if

  if (useGEM_) {
    for (const auto& tp : GEMInputs_) {
      if (tp.subsystem() == L1TMuon::kGEM) {
        const GEMDetId& tp_detId = tp.detId<GEMDetId>();
        const GEMData& tp_data = tp.getGEMData();
       
        gemInput_bx                ->push_back(tp.getBX());
        gemInput_region            ->push_back(tp_detId.region());
        gemInput_station           ->push_back(tp_detId.station());
        gemInput_ring              ->push_back(tp_detId.ring());
        // gemInput_sector            ->push_back(tp_detId.sector());
        gemInput_chamber           ->push_back(tp_detId.chamber());
        gemInput_roll              ->push_back(tp_detId.roll());
        gemInput_layer             ->push_back(tp_detId.layer());
        gemInput_pad               ->push_back(tp.getStrip());
        gemInput_pad_low           ->push_back(tp_data.pad_low);
        gemInput_pad_high          ->push_back(tp_data.pad_hi);
      }
    }  // end for loop
  }  // end if

  // Phase 2 inputs

  // TO DO
  
  // EMTF Hits
  for (const auto& hit : EMTFHits_) {
    emtfHit_endcap     ->push_back(hit.Endcap());
    emtfHit_station    ->push_back(hit.Station());
    emtfHit_ring       ->push_back(hit.Ring());
    emtfHit_sector     ->push_back(hit.PC_sector());
    emtfHit_subsector  ->push_back(hit.Subsector());
    emtfHit_chamber    ->push_back(hit.Chamber());
    emtfHit_cscid      ->push_back(hit.CSC_ID());
    emtfHit_bx         ->push_back(hit.BX());
    emtfHit_type       ->push_back(hit.Subsystem());
    emtfHit_neighbor   ->push_back(hit.Neighbor());
    //
    emtfHit_strip      ->push_back(hit.Strip());
    emtfHit_wire       ->push_back(hit.Wire());
    emtfHit_roll       ->push_back(hit.Roll());
    emtfHit_quality    ->push_back(hit.Quality());
    emtfHit_pattern    ->push_back(hit.Pattern());  
    emtfHit_bend       ->push_back(hit.Bend());
    emtfHit_time       ->push_back(hit.Time());    
    // emtfHit_fr         ->push_back(isFront(hit));      
    emtfHit_emtf_phi   ->push_back(hit.Phi_fp());
    emtfHit_emtf_theta ->push_back(hit.Theta_fp());
    //
    emtfHit_sim_phi    ->push_back(hit.Phi_sim());
    emtfHit_sim_theta  ->push_back(hit.Theta_sim());
    emtfHit_sim_r      ->push_back(hit.Rho_sim());
    emtfHit_sim_z      ->push_back(hit.Z_sim());
  }
  (*emtfHit_size) = EMTFHits_.size();

  // EMTF Unpacked Hits
  for (const auto& hit : EMTFUnpHits_) {
    emtfUnpHit_endcap     ->push_back(hit.Endcap());
    emtfUnpHit_station    ->push_back(hit.Station());
    emtfUnpHit_ring       ->push_back(hit.Ring());
    emtfUnpHit_sector     ->push_back(hit.PC_sector());
    emtfUnpHit_subsector  ->push_back(hit.Subsector());
    emtfUnpHit_chamber    ->push_back(hit.Chamber());
    emtfUnpHit_cscid      ->push_back(hit.CSC_ID());
    emtfUnpHit_bx         ->push_back(hit.BX());
    emtfUnpHit_type       ->push_back(hit.Subsystem());
    emtfUnpHit_neighbor   ->push_back(hit.Neighbor());
    //
    emtfUnpHit_strip      ->push_back(hit.Strip());
    emtfUnpHit_wire       ->push_back(hit.Wire());
    emtfUnpHit_roll       ->push_back(hit.Roll());
    emtfUnpHit_quality    ->push_back(hit.Quality());
    emtfUnpHit_pattern    ->push_back(hit.Pattern());  
    emtfUnpHit_bend       ->push_back(hit.Bend());
    emtfUnpHit_time       ->push_back(hit.Time());     
    // emtfUnpHit_fr         ->push_back(isFront(hit));      // added
    emtfUnpHit_emtf_phi   ->push_back(hit.Phi_fp());
    emtfUnpHit_emtf_theta ->push_back(hit.Theta_fp());
    //
    emtfUnpHit_sim_phi    ->push_back(hit.Phi_sim());
    emtfUnpHit_sim_theta  ->push_back(hit.Theta_sim());
    emtfUnpHit_sim_r      ->push_back(hit.Rho_sim());
    emtfUnpHit_sim_z      ->push_back(hit.Z_sim());
  }
  (*emtfUnpHit_size) = EMTFUnpHits_.size();


  // EMTF Tracks
  for (const auto& trk : EMTFTracks_) {
    // const std::vector<int32_t>& hit_refs = get_hit_refs(trk, emuHits_);
    // assert(hit_refs.size() == 4);

    const l1t::EMTFPtLUT& ptlut_data = trk.PtLUT();

    emtfTrack_pt           ->push_back(trk.Pt());
    emtfTrack_xml_pt       ->push_back(trk.Pt_XML());
    emtfTrack_pt_dxy       ->push_back(trk.Pt_dxy());
    emtfTrack_dxy          ->push_back(trk.Dxy());
    // emtfTrack_invpt_prompt ->push_back(trk.Invpt_prompt());
    // emtfTrack_invpt_displ  ->push_back(trk.Invpt_displ());
    emtfTrack_phi          ->push_back(trk.Phi_glob());
    emtfTrack_phi_fp       ->push_back(trk.Phi_fp());
    emtfTrack_theta        ->push_back(trk.Theta());
    emtfTrack_theta_fp     ->push_back(trk.Theta_fp());
    emtfTrack_eta          ->push_back(trk.Eta());
    emtfTrack_GMT_phi      ->push_back(trk.GMT_phi());
    emtfTrack_GMT_eta      ->push_back(trk.GMT_eta());
    emtfTrack_q            ->push_back(trk.Charge());
    //
    emtfTrack_address      ->push_back(ptlut_data.address);
    emtfTrack_mode         ->push_back(trk.Mode());
    emtfTrack_endcap       ->push_back(trk.Endcap());
    emtfTrack_sector       ->push_back(trk.Sector());
    emtfTrack_bx           ->push_back(trk.BX());
    emtfTrack_nhits        ->push_back(trk.Hits().size());
    // emtfTrack_hitref1    ->push_back(hit_refs.at(0));
    // emtfTrack_hitref2    ->push_back(hit_refs.at(1));
    // emtfTrack_hitref3    ->push_back(hit_refs.at(2));
    // emtfTrack_hitref4    ->push_back(hit_refs.at(3));  
  }
  (*emtfTrack_size) = EMTFTracks_.size();

  // EMTF Unpacked Tracks
  for (const auto& trk : EMTFUnpTracks_) {
    // const std::vector<int32_t>& hit_refs = get_hit_refs(trk, emuHits_);
    // assert(hit_refs.size() == 4);

    const l1t::EMTFPtLUT& ptlut_data = trk.PtLUT();

    emtfUnpTrack_pt           ->push_back(trk.Pt());
    emtfUnpTrack_xml_pt       ->push_back(trk.Pt_XML());
    // emtfUnpTrack_pt_dxy       ->push_back(trk.Pt_dxy());
    // emtfUnpTrack_dxy          ->push_back(trk.Dxy());
    // emtfUnpTrack_invpt_prompt ->push_back(trk.Invpt_prompt());
    // emtfUnpTrack_invpt_displ  ->push_back(trk.Invpt_displ());
    emtfUnpTrack_phi          ->push_back(trk.Phi_glob());
    emtfUnpTrack_theta        ->push_back(trk.Theta());
    emtfUnpTrack_eta          ->push_back(trk.Eta());
    emtfUnpTrack_q            ->push_back(trk.Charge());
    //
    emtfUnpTrack_address      ->push_back(ptlut_data.address);
    emtfUnpTrack_mode         ->push_back(trk.Mode());
    emtfUnpTrack_endcap       ->push_back(trk.Endcap());
    emtfUnpTrack_sector       ->push_back(trk.Sector());
    emtfUnpTrack_bx           ->push_back(trk.BX());
    emtfUnpTrack_nhits        ->push_back(trk.Hits().size());
    // emtfUnpTrack_hitref1    ->push_back(hit_refs.at(0));
    // emtfUnpTrack_hitref2    ->push_back(hit_refs.at(1));
    // emtfUnpTrack_hitref3    ->push_back(hit_refs.at(2));
    // emtfUnpTrack_hitref4    ->push_back(hit_refs.at(3)); 
  }
  (*emtfUnpTrack_size) = EMTFUnpTracks_.size();

  // GMT Muons
  if(useGMTMuons_){  
    for (const auto& muon : *GMTMuons_) {  
      gmtMuon_pt        ->push_back(muon.pt());
      gmtMuon_pt_dxy    ->push_back(muon.ptUnconstrained());
      gmtMuon_dxy       ->push_back(muon.hwDXY());
      gmtMuon_phi       ->push_back(muon.phi());
      gmtMuon_eta       ->push_back(muon.eta());
      gmtMuon_q         ->push_back(muon.charge());
      gmtMuon_qual      ->push_back(muon.hwQual());
    }
    (*gmtMuon_size) = GMTMuons_->size();
  }

  // GMT Unpacked Muons
  if(useGMTUnpMuons_){
    for (const auto& muon : *GMTUnpMuons_) {  
      gmtUnpMuon_pt        ->push_back(muon.pt());
      // gmtUnpMuon_pt_dxy    ->push_back(muon.ptUnconstrained());
      // gmtUnpMuon_dxy       ->push_back(muon.hwDXY());
      gmtUnpMuon_phi       ->push_back(muon.phi());
      gmtUnpMuon_eta       ->push_back(muon.eta());
      gmtUnpMuon_q         ->push_back(muon.charge());
      gmtUnpMuon_qual      ->push_back(muon.hwQual());
    }
    (*gmtUnpMuon_size) = GMTUnpMuons_->size();
  }
  
  // Gen particles
  if(useGENParts_){
    for (const auto& part : *GENParts_) {
      if (abs(part.pdgId()) != 13) continue;

      int parentID = -9999;
      if (part.numberOfMothers() > 0) parentID = dynamic_cast<const reco::GenParticle*>(part.mother(0))->pdgId();
      if (parentID == part.pdgId()) continue;

      genPart_pt         ->push_back(part.pt());
      // genPart_dxy        ->push_back(part.dxy());
      genPart_eta        ->push_back(part.eta());
      genPart_phi        ->push_back(part.phi());
      genPart_q          ->push_back(part.charge());
      genPart_ID         ->push_back(part.pdgId());
      genPart_parentID   ->push_back(parentID);
      genPart_vx         ->push_back(part.vx());
      genPart_vy         ->push_back(part.vy());
      genPart_vz         ->push_back(part.vz());
    }
  }
  

  // Event Info
  eventInfo_event->push_back(iEvent.id().event());

  // ___________________________________________________________________________
  // Fill

  fillTree();


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

  if (verbose_ > 0) std::cout << "******* Getting Handles *******" << std::endl;


  geometryTranslator_.checkAndUpdateGeometry(iSetup);

  auto tp_geom = &(geometryTranslator_);

  // Run 3 inputs
  if(useCSC_){
     collector_.extractPrimitives(emtf::CSCTag(), tp_geom, iEvent, CSCInputToken_, CSCInputs_);
  }

  if(useRPC_){
     collector_.extractPrimitives(emtf::RPCTag(), tp_geom, iEvent, RPCInputToken_, RPCInputs_);
  }

  if(useGEM_){
     collector_.extractPrimitives(emtf::GEMTag(), tp_geom, iEvent, GEMInputToken_, GEMInputs_);
  }

  // Phase 2 inputs
  if(useIRPC_){
     collector_.extractPrimitives(emtf::IRPCTag(), tp_geom, iEvent, IRPCInputToken_, IRPCInputs_);
  }

  if(useME0_){
     collector_.extractPrimitives(emtf::ME0Tag(), tp_geom, iEvent, ME0InputToken_, ME0Inputs_);
  }

  if(useDT_){
     // collector_.extractPrimitives(emtf::DTTag(), tp_geom, iEvent, DTInputToken_, DTInputs_);
  }

  // EMTF hits and tracks
  auto EMTFHits_handle = make_handle(EMTFHits_);
  auto EMTFUnpHits_handle = make_handle(EMTFUnpHits_);

  auto EMTFTracks_handle = make_handle(EMTFTracks_);
  auto EMTFUnpTracks_handle = make_handle(EMTFUnpTracks_);

  if (useEMTFHits_){
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
  }

  if (useEMTFUnpHits_){
    if (!EMTFUnpHitToken_.isUninitialized()) {
      iEvent.getByToken(EMTFUnpHitToken_, EMTFUnpHits_handle);
    }
    if (!EMTFUnpHits_handle.isValid()) {
      if (firstEvent_)  edm::LogError("NtupleMaker") << "Cannot get the product: " << EMTFUnpHitTag_;
    }
    else{
      EMTFUnpHits_.clear();
      for (const auto& hit : (*EMTFUnpHits_handle)) {
        // if (!(-2 <= hit.BX() && hit.BX() <= 2))  continue;  // only BX=[-2,+2]
        EMTFUnpHits_.push_back(hit);
      }
    }
  }

  if (useEMTFTracks_){
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
  }

  if (useEMTFUnpTracks_){
    if (!EMTFUnpTrackToken_.isUninitialized()) {
      iEvent.getByToken(EMTFUnpTrackToken_, EMTFUnpTracks_handle);
    }
    if (!EMTFUnpTracks_handle.isValid()) {
      if (firstEvent_)  edm::LogError("NtupleMaker") << "Cannot get the product: " << EMTFUnpTrackTag_;
    }
    else{
      EMTFUnpTracks_.clear();
      for (const auto& track : (*EMTFUnpTracks_handle)) {
        // if (trk.BX() != 0)      continue;  // only BX=0
        EMTFUnpTracks_.push_back(track);
      }
    }
  }

  // GMT muons and GEN particles
  auto GMTMuons_handle = make_handle(GMTMuons_);
  auto GMTUnpMuons_handle = make_handle(GMTUnpMuons_);
  
  auto GENParts_handle = make_handle(GENParts_);

  if (useGMTMuons_){
    if (!GMTMuonToken_.isUninitialized()) {
      iEvent.getByToken(GMTMuonToken_, GMTMuons_handle);
    }
    if (!GMTMuons_handle.isValid()) {
      if (firstEvent_)  edm::LogError("NtupleMaker") << "Cannot get the product: " << GMTMuonTag_;
      GMTMuons_ = nullptr;
    }
    else{
      GMTMuons_ = GMTMuons_handle.product();
    }
  }
  else{
    GMTMuons_ = nullptr;
  }

  if (useGMTUnpMuons_){
    if (!GMTUnpMuonToken_.isUninitialized()) {
      iEvent.getByToken(GMTUnpMuonToken_, GMTUnpMuons_handle);
    }
    if (!GMTUnpMuons_handle.isValid()) {
      if (firstEvent_)  edm::LogError("NtupleMaker") << "Cannot get the product: " << GMTUnpMuonTag_;
      GMTUnpMuons_ = nullptr;
    }
    else{
      GMTUnpMuons_ = GMTUnpMuons_handle.product();
    }
  }
  else{
    GMTUnpMuons_ = nullptr;
  }
  if (!iEvent.isRealData() && useGENParts_) {
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
  else {
    GENParts_ = nullptr;
  }



}

void EMTFNtuple::makeTree() {

  if (verbose_ > 0) std::cout << "******* Making Output Tree *******" << std::endl;


  // TFileService
  edm::Service<TFileService> fs;
  tree = fs->make<TTree>("tree", "tree");

  // Create pointers
  // CSC Inputs
  cscInput_endcap            = std::make_unique<std::vector<int16_t> >();
  cscInput_station           = std::make_unique<std::vector<int16_t> >();
  cscInput_ring              = std::make_unique<std::vector<int16_t> >();
  cscInput_sector            = std::make_unique<std::vector<int16_t> >();
  cscInput_chamber           = std::make_unique<std::vector<int16_t> >();
  cscInput_cscid             = std::make_unique<std::vector<int16_t> >();
  cscInput_bx                = std::make_unique<std::vector<int16_t> >();
  cscInput_strip             = std::make_unique<std::vector<int16_t> >();
  cscInput_wire              = std::make_unique<std::vector<int16_t> >();
  cscInput_quality           = std::make_unique<std::vector<int16_t> >();
  cscInput_pattern           = std::make_unique<std::vector<int16_t> >();
  cscInput_bend              = std::make_unique<std::vector<int16_t> >();

  // RPC Inputs
  rpcInput_region            = std::make_unique<std::vector<int16_t> >();
  rpcInput_station           = std::make_unique<std::vector<int16_t> >();
  rpcInput_ring              = std::make_unique<std::vector<int16_t> >();
  rpcInput_sector            = std::make_unique<std::vector<int16_t> >();
  rpcInput_subsector         = std::make_unique<std::vector<int16_t> >();
  rpcInput_roll              = std::make_unique<std::vector<int16_t> >();
  rpcInput_bx                = std::make_unique<std::vector<int16_t> >();
  rpcInput_strip             = std::make_unique<std::vector<int16_t> >();
  rpcInput_strip_high        = std::make_unique<std::vector<int16_t> >();
  rpcInput_strip_low         = std::make_unique<std::vector<int16_t> >();
  rpcInput_time              = std::make_unique<std::vector<int16_t> >();
  rpcInput_valid             = std::make_unique<std::vector<int16_t> >();

  // GEM Inputs
  gemInput_region            = std::make_unique<std::vector<int16_t> >();
  gemInput_station           = std::make_unique<std::vector<int16_t> >();
  gemInput_ring              = std::make_unique<std::vector<int16_t> >();
  gemInput_sector            = std::make_unique<std::vector<int16_t> >();
  gemInput_chamber           = std::make_unique<std::vector<int16_t> >();
  gemInput_roll              = std::make_unique<std::vector<int16_t> >();
  gemInput_bx                = std::make_unique<std::vector<int16_t> >();
  gemInput_layer             = std::make_unique<std::vector<int16_t> >();
  gemInput_pad               = std::make_unique<std::vector<int16_t> >();
  gemInput_pad_low           = std::make_unique<std::vector<int16_t> >();
  gemInput_pad_high          = std::make_unique<std::vector<int16_t> >();

  // ME0 Inputs
  me0Input_region            = std::make_unique<std::vector<int16_t> >();
  me0Input_station           = std::make_unique<std::vector<int16_t> >();
  me0Input_ring              = std::make_unique<std::vector<int16_t> >();
  me0Input_sector            = std::make_unique<std::vector<int16_t> >();
  me0Input_chamber           = std::make_unique<std::vector<int16_t> >();
  me0Input_roll              = std::make_unique<std::vector<int16_t> >();
  me0Input_bx                = std::make_unique<std::vector<int16_t> >();
  me0Input_layer             = std::make_unique<std::vector<int16_t> >();
  me0Input_phiposition       = std::make_unique<std::vector<int16_t> >();
  me0Input_deltaphi          = std::make_unique<std::vector<int16_t> >();
  me0Input_quality           = std::make_unique<std::vector<int16_t> >();
  me0Input_bend              = std::make_unique<std::vector<int16_t> >();
  me0Input_partition         = std::make_unique<std::vector<int16_t> >();

  // DT Inputs
  dtInput_wheel              = std::make_unique<std::vector<int16_t> >();
  dtInput_station            = std::make_unique<std::vector<int16_t> >();
  dtInput_btigroup           = std::make_unique<std::vector<int16_t> >();
  dtInput_bx                 = std::make_unique<std::vector<int16_t> >();
  dtInput_strip              = std::make_unique<std::vector<int16_t> >();
  dtInput_wire               = std::make_unique<std::vector<int16_t> >();
  dtInput_quality            = std::make_unique<std::vector<int16_t> >();
  dtInput_bend               = std::make_unique<std::vector<int16_t> >();



  // EMTF Hits
  emtfHit_endcap             = std::make_unique<std::vector<int16_t> >();
  emtfHit_station            = std::make_unique<std::vector<int16_t> >();
  emtfHit_ring               = std::make_unique<std::vector<int16_t> >();
  emtfHit_sector             = std::make_unique<std::vector<int16_t> >();
  emtfHit_subsector          = std::make_unique<std::vector<int16_t> >();
  emtfHit_chamber            = std::make_unique<std::vector<int16_t> >();
  emtfHit_cscid              = std::make_unique<std::vector<int16_t> >();
  emtfHit_bx                 = std::make_unique<std::vector<int16_t> >();
  emtfHit_type               = std::make_unique<std::vector<int16_t> >();  // subsystem: DT=0,CSC=1,RPC=2,GEM=3,ME0=4
  emtfHit_neighbor           = std::make_unique<std::vector<int16_t> >();
  //
  emtfHit_strip              = std::make_unique<std::vector<int16_t> >();
  emtfHit_wire               = std::make_unique<std::vector<int16_t> >();
  emtfHit_roll               = std::make_unique<std::vector<int16_t> >();
  emtfHit_quality            = std::make_unique<std::vector<int16_t> >();
  emtfHit_pattern            = std::make_unique<std::vector<int16_t> >();
  emtfHit_bend               = std::make_unique<std::vector<int16_t> >();
  emtfHit_time               = std::make_unique<std::vector<int16_t> >();
  emtfHit_fr                 = std::make_unique<std::vector<int16_t> >();
  emtfHit_emtf_phi           = std::make_unique<std::vector<int32_t> >();   // integer unit
  emtfHit_emtf_theta         = std::make_unique<std::vector<int32_t> >(); // integer unit
  //
  emtfHit_sim_phi            = std::make_unique<std::vector<float  > >();
  emtfHit_sim_theta          = std::make_unique<std::vector<float  > >();
  emtfHit_sim_r              = std::make_unique<std::vector<float  > >();
  emtfHit_sim_z              = std::make_unique<std::vector<float  > >();
  // emtfHit_sim_tp1            = std::make_unique<std::vector<int32_t> >();
  // emtfHit_sim_tp2            = std::make_unique<std::vector<int32_t> >();
  emtfHit_size               = std::make_unique<int32_t>(0);

  // EMTF Unpacked Hits
  emtfUnpHit_endcap          = std::make_unique<std::vector<int16_t> >();
  emtfUnpHit_station         = std::make_unique<std::vector<int16_t> >();
  emtfUnpHit_ring            = std::make_unique<std::vector<int16_t> >();
  emtfUnpHit_sector          = std::make_unique<std::vector<int16_t> >();
  emtfUnpHit_subsector       = std::make_unique<std::vector<int16_t> >();
  emtfUnpHit_chamber         = std::make_unique<std::vector<int16_t> >();
  emtfUnpHit_cscid           = std::make_unique<std::vector<int16_t> >();
  emtfUnpHit_bx              = std::make_unique<std::vector<int16_t> >();
  emtfUnpHit_type            = std::make_unique<std::vector<int16_t> >();  // subsystem: DT=0,CSC=1,RPC=2,GEM=3,ME0=4
  emtfUnpHit_neighbor        = std::make_unique<std::vector<int16_t> >();
  //
  emtfUnpHit_strip           = std::make_unique<std::vector<int16_t> >();
  emtfUnpHit_wire            = std::make_unique<std::vector<int16_t> >();
  emtfUnpHit_roll            = std::make_unique<std::vector<int16_t> >();
  emtfUnpHit_quality         = std::make_unique<std::vector<int16_t> >();
  emtfUnpHit_pattern         = std::make_unique<std::vector<int16_t> >();
  emtfUnpHit_bend            = std::make_unique<std::vector<int16_t> >();
  emtfUnpHit_time            = std::make_unique<std::vector<int16_t> >();
  emtfUnpHit_fr              = std::make_unique<std::vector<int16_t> >();
  emtfUnpHit_emtf_phi        = std::make_unique<std::vector<int32_t> >();   // integer unit
  emtfUnpHit_emtf_theta      = std::make_unique<std::vector<int32_t> >(); // integer unit
  //
  emtfUnpHit_sim_phi         = std::make_unique<std::vector<float  > >();
  emtfUnpHit_sim_theta       = std::make_unique<std::vector<float  > >();
  emtfUnpHit_sim_r           = std::make_unique<std::vector<float  > >();
  emtfUnpHit_sim_z           = std::make_unique<std::vector<float  > >();
  // emtfUnpHit_sim_tp1         = std::make_unique<std::vector<int32_t> >();
  // emtfUnpHit_sim_tp2         = std::make_unique<std::vector<int32_t> >();
  emtfUnpHit_size            = std::make_unique<int32_t>(0);

  // EMTF Tracks
  emtfTrack_pt               = std::make_unique<std::vector<float  > >();
  emtfTrack_xml_pt           = std::make_unique<std::vector<float  > >();
  emtfTrack_pt_dxy           = std::make_unique<std::vector<float  > >();
  emtfTrack_dxy              = std::make_unique<std::vector<float  > >();
  emtfTrack_invpt_prompt     = std::make_unique<std::vector<float  > >();
  emtfTrack_invpt_displ      = std::make_unique<std::vector<float  > >();
  emtfTrack_phi              = std::make_unique<std::vector<float  > >();        // in degrees
  emtfTrack_phi_fp           = std::make_unique<std::vector<int32_t> >();        // in degrees
  emtfTrack_theta            = std::make_unique<std::vector<float  > >();      // in degrees
  emtfTrack_theta_fp         = std::make_unique<std::vector<int32_t> >();      // in degrees
  emtfTrack_eta              = std::make_unique<std::vector<float  > >();
  emtfTrack_GMT_phi          = std::make_unique<std::vector<int32_t> >();
  emtfTrack_GMT_eta          = std::make_unique<std::vector<int32_t> >();
  emtfTrack_q                = std::make_unique<std::vector<int16_t> >();          // charge
  //
  emtfTrack_address          = std::make_unique<std::vector<uint64_t> >();
  emtfTrack_mode             = std::make_unique<std::vector<int16_t> >();
  emtfTrack_endcap           = std::make_unique<std::vector<int16_t> >();
  emtfTrack_sector           = std::make_unique<std::vector<int16_t> >();
  emtfTrack_bx               = std::make_unique<std::vector<int16_t> >();
  emtfTrack_nhits            = std::make_unique<std::vector<int16_t> >();
  emtfTrack_hitref1          = std::make_unique<std::vector<int32_t> >();
  emtfTrack_hitref2          = std::make_unique<std::vector<int32_t> >();
  emtfTrack_hitref3          = std::make_unique<std::vector<int32_t> >();
  emtfTrack_hitref4          = std::make_unique<std::vector<int32_t> >();
  emtfTrack_size             = std::make_unique<int32_t>(0);

  // EMTF Unpacked Tracks
  emtfUnpTrack_pt            = std::make_unique<std::vector<float> >();
  emtfUnpTrack_xml_pt        = std::make_unique<std::vector<float> >();
  emtfUnpTrack_pt_dxy        = std::make_unique<std::vector<float> >();
  emtfUnpTrack_dxy           = std::make_unique<std::vector<float> >();
  emtfUnpTrack_invpt_prompt  = std::make_unique<std::vector<float> >();
  emtfUnpTrack_invpt_displ   = std::make_unique<std::vector<float> >();
  emtfUnpTrack_phi           = std::make_unique<std::vector<float> >();        // in degrees
  emtfUnpTrack_theta         = std::make_unique<std::vector<float> >();      // in degrees
  emtfUnpTrack_eta           = std::make_unique<std::vector<float> >();
  emtfUnpTrack_q             = std::make_unique<std::vector<int16_t> >();          // charge
  //
  emtfUnpTrack_address       = std::make_unique<std::vector<uint64_t> >();
  emtfUnpTrack_mode          = std::make_unique<std::vector<int16_t> >();
  emtfUnpTrack_endcap        = std::make_unique<std::vector<int16_t> >();
  emtfUnpTrack_sector        = std::make_unique<std::vector<int16_t> >();
  emtfUnpTrack_bx            = std::make_unique<std::vector<int16_t> >();
  emtfUnpTrack_nhits         = std::make_unique<std::vector<int16_t> >();
  emtfUnpTrack_hitref1       = std::make_unique<std::vector<int32_t> >();
  emtfUnpTrack_hitref2       = std::make_unique<std::vector<int32_t> >();
  emtfUnpTrack_hitref3       = std::make_unique<std::vector<int32_t> >();
  emtfUnpTrack_hitref4       = std::make_unique<std::vector<int32_t> >();
  emtfUnpTrack_size          = std::make_unique<int32_t>(0);

  // GMT Muons
  gmtMuon_pt                 = std::make_unique<std::vector<float> >();
  gmtMuon_pt_dxy             = std::make_unique<std::vector<float> >();
  gmtMuon_dxy                = std::make_unique<std::vector<int16_t> >();
  gmtMuon_phi                = std::make_unique<std::vector<float> >();        // in degrees
  gmtMuon_eta                = std::make_unique<std::vector<float> >();
  gmtMuon_q                  = std::make_unique<std::vector<int16_t> >();          // charge
  gmtMuon_qual               = std::make_unique<std::vector<int16_t> >();      
  gmtMuon_size               = std::make_unique<int32_t>(0); 

  // GMT Unpacked Muons
  gmtUnpMuon_pt              = std::make_unique<std::vector<float> >();
  gmtUnpMuon_pt_dxy          = std::make_unique<std::vector<float> >();
  gmtUnpMuon_dxy             = std::make_unique<std::vector<int16_t> >();
  gmtUnpMuon_phi             = std::make_unique<std::vector<float> >();        // in degrees
  gmtUnpMuon_eta             = std::make_unique<std::vector<float> >();
  gmtUnpMuon_q               = std::make_unique<std::vector<int16_t> >();          // charge
  gmtUnpMuon_qual            = std::make_unique<std::vector<int16_t> >();      
  gmtUnpMuon_size            = std::make_unique<int32_t>(0);      

  // GEN particles
  genPart_pt                 = std::make_unique<std::vector<float> >();
  genPart_dxy                = std::make_unique<std::vector<float> >();
  genPart_eta                = std::make_unique<std::vector<float> >();
  genPart_phi                = std::make_unique<std::vector<float> >();
  genPart_q                  = std::make_unique<std::vector<int16_t> >();          // charge
  genPart_ID                 = std::make_unique<std::vector<int16_t> >();      
  genPart_parentID           = std::make_unique<std::vector<int32_t> >();      
  genPart_vx                 = std::make_unique<std::vector<float> >();      
  genPart_vy                 = std::make_unique<std::vector<float> >();      
  genPart_vz                 = std::make_unique<std::vector<float> >();      

  // Event info
  eventInfo_event            = std::make_unique<std::vector<uint64_t> >();
  eventInfo_run              = std::make_unique<std::vector<uint32_t> >();
  eventInfo_lumi             = std::make_unique<std::vector<uint32_t> >();
  eventInfo_npv              = std::make_unique<std::vector<float> >();  // getTrueNumInteractions()
  eventInfo_nvtx             = std::make_unique<std::vector<int32_t> >(); // getPU_NumInteractions()
  eventInfo_size             = std::make_unique<int32_t>(0);

  // Set branches

  // CSC Inputs
  if (useCSC_){
    tree->Branch("cscInput_endcap"    , &(*cscInput_endcap    ));
    tree->Branch("cscInput_station"   , &(*cscInput_station   ));
    tree->Branch("cscInput_ring"      , &(*cscInput_ring      ));
    tree->Branch("cscInput_sector"    , &(*cscInput_sector    ));
    tree->Branch("cscInput_chamber"   , &(*cscInput_chamber   ));
    tree->Branch("cscInput_cscid"     , &(*cscInput_cscid     ));
    tree->Branch("cscInput_bx"        , &(*cscInput_bx        ));
    tree->Branch("cscInput_strip"     , &(*cscInput_strip     ));
    tree->Branch("cscInput_wire"      , &(*cscInput_wire      ));
    tree->Branch("cscInput_quality"   , &(*cscInput_quality   ));
    tree->Branch("cscInput_pattern"   , &(*cscInput_pattern   ));
    tree->Branch("cscInput_bend"      , &(*cscInput_bend      ));
  }

  // RPC Inputs
  if (useRPC_){
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
  }

  // GEM Inputs
  if (useGEM_){
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
  }


  // Phase 2 inputs
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
  if (useEMTFHits_){
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
    // tree->Branch("emtfHit_sim_tp1"   , &(*emtfHit_sim_tp1   ));
    // tree->Branch("emtfHit_sim_tp2"   , &(*emtfHit_sim_tp2   ));
    tree->Branch("emtfHit_size"      , &(*emtfHit_size      ));
  }

  // EMTF Unpacked Hits
  if (useEMTFUnpHits_){
    tree->Branch("emtfUnpHit_endcap"    , &(*emtfUnpHit_endcap    ));
    tree->Branch("emtfUnpHit_station"   , &(*emtfUnpHit_station   ));
    tree->Branch("emtfUnpHit_ring"      , &(*emtfUnpHit_ring      ));
    tree->Branch("emtfUnpHit_sector"    , &(*emtfUnpHit_sector    ));
    tree->Branch("emtfUnpHit_subsector" , &(*emtfUnpHit_subsector ));
    tree->Branch("emtfUnpHit_chamber"   , &(*emtfUnpHit_chamber   ));
    tree->Branch("emtfUnpHit_cscid"     , &(*emtfUnpHit_cscid     ));
    tree->Branch("emtfUnpHit_bx"        , &(*emtfUnpHit_bx        ));
    tree->Branch("emtfUnpHit_type"      , &(*emtfUnpHit_type      ));
    tree->Branch("emtfUnpHit_neighbor"  , &(*emtfUnpHit_neighbor  ));
    //
    tree->Branch("emtfUnpHit_strip"     , &(*emtfUnpHit_strip     ));
    tree->Branch("emtfUnpHit_wire"      , &(*emtfUnpHit_wire      ));
    tree->Branch("emtfUnpHit_roll"      , &(*emtfUnpHit_roll      ));
    tree->Branch("emtfUnpHit_quality"   , &(*emtfUnpHit_quality   ));
    tree->Branch("emtfUnpHit_pattern"   , &(*emtfUnpHit_pattern   ));
    tree->Branch("emtfUnpHit_bend"      , &(*emtfUnpHit_bend      ));
    tree->Branch("emtfUnpHit_time"      , &(*emtfUnpHit_time      ));
    tree->Branch("emtfUnpHit_fr"        , &(*emtfUnpHit_fr        ));
    tree->Branch("emtfUnpHit_emtf_phi"  , &(*emtfUnpHit_emtf_phi  ));
    tree->Branch("emtfUnpHit_emtf_theta", &(*emtfUnpHit_emtf_theta));
    //
    tree->Branch("emtfUnpHit_sim_phi"   , &(*emtfUnpHit_sim_phi   ));
    tree->Branch("emtfUnpHit_sim_theta" , &(*emtfUnpHit_sim_theta ));
    tree->Branch("emtfUnpHit_sim_r"     , &(*emtfUnpHit_sim_r     ));
    tree->Branch("emtfUnpHit_sim_z"     , &(*emtfUnpHit_sim_z     ));
    // tree->Branch("emtfUnpHit_sim_tp1"   , &(*emtfUnpHit_sim_tp1   ));
    // tree->Branch("emtfUnpHit_sim_tp2"   , &(*emtfUnpHit_sim_tp2   ));
    tree->Branch("emtfUnpHit_size"      , &(*emtfUnpHit_size      ));
  }

  // EMTF Tracks
  if (useEMTFTracks_){
    tree->Branch("emtfTrack_pt"          , &(*emtfTrack_pt          ));
    tree->Branch("emtfTrack_xml_pt"      , &(*emtfTrack_xml_pt      ));
    tree->Branch("emtfTrack_pt_dxy"      , &(*emtfTrack_pt_dxy      ));
    tree->Branch("emtfTrack_dxy"         , &(*emtfTrack_dxy         ));
    tree->Branch("emtfTrack_invpt_prompt", &(*emtfTrack_invpt_prompt));
    tree->Branch("emtfTrack_invpt_displ" , &(*emtfTrack_invpt_displ ));
    tree->Branch("emtfTrack_phi"         , &(*emtfTrack_phi         ));
    tree->Branch("emtfTrack_phi_fp"      , &(*emtfTrack_phi_fp      ));
    tree->Branch("emtfTrack_theta"       , &(*emtfTrack_theta       ));
    tree->Branch("emtfTrack_theta_fp"    , &(*emtfTrack_theta_fp    ));
    tree->Branch("emtfTrack_eta"         , &(*emtfTrack_eta         ));
    tree->Branch("emtfTrack_GMT_phi"     , &(*emtfTrack_GMT_phi     ));
    tree->Branch("emtfTrack_GMT_eta"     , &(*emtfTrack_GMT_eta     ));
    tree->Branch("emtfTrack_q"           , &(*emtfTrack_q           ));
    //
    tree->Branch("emtfTrack_address"     , &(*emtfTrack_address     ));
    tree->Branch("emtfTrack_mode"        , &(*emtfTrack_mode        ));
    tree->Branch("emtfTrack_endcap"      , &(*emtfTrack_endcap      ));
    tree->Branch("emtfTrack_sector"      , &(*emtfTrack_sector      ));
    tree->Branch("emtfTrack_bx"          , &(*emtfTrack_bx          ));
    tree->Branch("emtfTrack_nhits"       , &(*emtfTrack_nhits       ));
    tree->Branch("emtfTrack_hitref1"     , &(*emtfTrack_hitref1     ));
    tree->Branch("emtfTrack_hitref2"     , &(*emtfTrack_hitref2     ));
    tree->Branch("emtfTrack_hitref3"     , &(*emtfTrack_hitref3     ));
    tree->Branch("emtfTrack_hitref4"     , &(*emtfTrack_hitref4     ));
    tree->Branch("emtfTrack_size"        , &(*emtfTrack_size        ));
  }

  // EMTF Unpacked Tracks
  if (useEMTFUnpTracks_){
    tree->Branch("emtfUnpTrack_pt"          , &(*emtfUnpTrack_pt          ));
    tree->Branch("emtfUnpTrack_xml_pt"      , &(*emtfUnpTrack_xml_pt      ));
    tree->Branch("emtfUnpTrack_pt_dxy"      , &(*emtfUnpTrack_pt_dxy      ));
    tree->Branch("emtfUnpTrack_dxy"         , &(*emtfUnpTrack_dxy         ));
    tree->Branch("emtfUnpTrack_invpt_prompt", &(*emtfUnpTrack_invpt_prompt));
    tree->Branch("emtfUnpTrack_invpt_displ" , &(*emtfUnpTrack_invpt_displ ));
    tree->Branch("emtfUnpTrack_phi"         , &(*emtfUnpTrack_phi         ));
    tree->Branch("emtfUnpTrack_theta"       , &(*emtfUnpTrack_theta       ));
    tree->Branch("emtfUnpTrack_eta"         , &(*emtfUnpTrack_eta         ));
    tree->Branch("emtfUnpTrack_q"           , &(*emtfUnpTrack_q           ));
    //
    tree->Branch("emtfUnpTrack_address"     , &(*emtfUnpTrack_address     ));
    tree->Branch("emtfUnpTrack_mode"        , &(*emtfUnpTrack_mode        ));
    tree->Branch("emtfUnpTrack_endcap"      , &(*emtfUnpTrack_endcap      ));
    tree->Branch("emtfUnpTrack_sector"      , &(*emtfUnpTrack_sector      ));
    tree->Branch("emtfUnpTrack_bx"          , &(*emtfUnpTrack_bx          ));
    tree->Branch("emtfUnpTrack_nhits"       , &(*emtfUnpTrack_nhits       ));
    tree->Branch("emtfUnpTrack_hitref1"     , &(*emtfUnpTrack_hitref1     ));
    tree->Branch("emtfUnpTrack_hitref2"     , &(*emtfUnpTrack_hitref2     ));
    tree->Branch("emtfUnpTrack_hitref3"     , &(*emtfUnpTrack_hitref3     ));
    tree->Branch("emtfUnpTrack_hitref4"     , &(*emtfUnpTrack_hitref4     ));
    tree->Branch("emtfUnpTrack_size"        , &(*emtfUnpTrack_size        ));
  }

  // GMT muons
  if (useGMTMuons_){
    tree->Branch("gmtMuon_pt"        , &(*gmtMuon_pt        ));
    tree->Branch("gmtMuon_pt_dxy"    , &(*gmtMuon_pt_dxy    ));
    tree->Branch("gmtMuon_dxy"       , &(*gmtMuon_dxy       ));
    tree->Branch("gmtMuon_phi"       , &(*gmtMuon_phi       ));
    tree->Branch("gmtMuon_eta"       , &(*gmtMuon_eta       ));
    tree->Branch("gmtMuon_q"         , &(*gmtMuon_q         ));
    tree->Branch("gmtMuon_qual"      , &(*gmtMuon_qual      ));
    tree->Branch("gmtMuon_size"      , &(*gmtMuon_size      ));
  }

  // GMT Unpacked muons
  if (useGMTMuons_){
    tree->Branch("gmtUnpMuon_pt"        , &(*gmtUnpMuon_pt        ));
    tree->Branch("gmtUnpMuon_pt_dxy"    , &(*gmtUnpMuon_pt_dxy    ));
    tree->Branch("gmtUnpMuon_dxy"       , &(*gmtUnpMuon_dxy       ));
    tree->Branch("gmtUnpMuon_phi"       , &(*gmtUnpMuon_phi       ));
    tree->Branch("gmtUnpMuon_eta"       , &(*gmtUnpMuon_eta       ));
    tree->Branch("gmtUnpMuon_q"         , &(*gmtUnpMuon_q         ));
    tree->Branch("gmtUnpMuon_qual"      , &(*gmtUnpMuon_qual      ));
    tree->Branch("gmtUnpMuon_size"      , &(*gmtUnpMuon_size      ));
  }

  // GEN particles
  if (useGENParts_){
    tree->Branch("genPart_pt"        , &(*genPart_pt        ));
    tree->Branch("genPart_dxy"       , &(*genPart_dxy       ));
    tree->Branch("genPart_phi"       , &(*genPart_phi       ));
    tree->Branch("genPart_eta"       , &(*genPart_eta       ));
    tree->Branch("genPart_q"         , &(*genPart_q         ));
    tree->Branch("genPart_ID"        , &(*genPart_ID        ));
    tree->Branch("genPart_parentID"  , &(*genPart_parentID  ));
    tree->Branch("genPart_vx"        , &(*genPart_vx        ));
    tree->Branch("genPart_vy"        , &(*genPart_vy        ));
    tree->Branch("genPart_vz"        , &(*genPart_vz        ));
  }

  // Event info
  if (useEventInfo_){
    tree->Branch("eventInfo_event"     , &(*eventInfo_event     ));
    tree->Branch("eventInfo_run"       , &(*eventInfo_run       ));
    tree->Branch("eventInfo_lumi"      , &(*eventInfo_lumi      ));
    tree->Branch("eventInfo_npv"       , &(*eventInfo_npv       ));
    tree->Branch("eventInfo_nvtx"      , &(*eventInfo_nvtx      ));
    tree->Branch("eventInfo_size"      , &(*eventInfo_size      ));
  }

}

void EMTFNtuple::fillTree() {

  if (verbose_ > 0) std::cout << "******* Filling Output Tree and Clearing Objects *******" << std::endl;
  

  tree->Fill();
  
  // Clear objects
  // CSC Inputs
  cscInput_endcap            ->clear();
  cscInput_station           ->clear();
  cscInput_ring              ->clear();
  cscInput_sector            ->clear();
  cscInput_chamber           ->clear();
  cscInput_cscid             ->clear();
  cscInput_bx                ->clear();
  cscInput_strip             ->clear();
  cscInput_wire              ->clear();
  cscInput_quality           ->clear();
  cscInput_pattern           ->clear();
  cscInput_bend              ->clear();

  // RPC Inputs
  rpcInput_region            ->clear();
  rpcInput_station           ->clear();
  rpcInput_ring              ->clear();
  rpcInput_sector            ->clear();
  rpcInput_subsector         ->clear();
  rpcInput_roll              ->clear();
  rpcInput_bx                ->clear();
  rpcInput_strip             ->clear();
  rpcInput_strip_high        ->clear();
  rpcInput_strip_low         ->clear();
  rpcInput_time              ->clear();
  rpcInput_valid             ->clear();

  // GEM Inputs
  gemInput_region            ->clear();
  gemInput_station           ->clear();
  gemInput_ring              ->clear();
  gemInput_sector            ->clear();
  gemInput_chamber           ->clear();
  gemInput_roll              ->clear();
  gemInput_bx                ->clear();
  gemInput_layer             ->clear();
  gemInput_pad               ->clear();
  gemInput_pad_low           ->clear();
  gemInput_pad_high          ->clear();

  // ME0 Inputs
  me0Input_region            ->clear();
  me0Input_station           ->clear();
  me0Input_ring              ->clear();
  me0Input_sector            ->clear();
  me0Input_chamber           ->clear();
  me0Input_roll              ->clear();
  me0Input_bx                ->clear();
  me0Input_layer             ->clear();
  me0Input_phiposition       ->clear();
  me0Input_deltaphi          ->clear();
  me0Input_quality           ->clear();
  me0Input_bend              ->clear();
  me0Input_partition         ->clear();

  // DT Inputs
  dtInput_wheel              ->clear();
  dtInput_station            ->clear();
  dtInput_btigroup           ->clear();
  dtInput_bx                 ->clear();
  dtInput_strip              ->clear();
  dtInput_wire               ->clear();
  dtInput_quality            ->clear();
  dtInput_bend               ->clear();



  // EMTF Hits
  emtfHit_endcap             ->clear();
  emtfHit_station            ->clear();
  emtfHit_ring               ->clear();
  emtfHit_sector             ->clear();
  emtfHit_subsector          ->clear();
  emtfHit_chamber            ->clear();
  emtfHit_cscid              ->clear();
  emtfHit_bx                 ->clear();
  emtfHit_type               ->clear();
  emtfHit_neighbor           ->clear();
  //
  emtfHit_strip              ->clear();
  emtfHit_wire               ->clear();
  emtfHit_roll               ->clear();
  emtfHit_quality            ->clear();
  emtfHit_pattern            ->clear();
  emtfHit_bend               ->clear();
  emtfHit_time               ->clear();
  emtfHit_fr                 ->clear();
  emtfHit_emtf_phi           ->clear();
  emtfHit_emtf_theta         ->clear();
  //
  emtfHit_sim_phi            ->clear();
  emtfHit_sim_theta          ->clear();
  emtfHit_sim_r              ->clear();
  emtfHit_sim_z              ->clear();
  // emtfHit_sim_tp1            ->clear();
  // emtfHit_sim_tp2            ->clear();
  (*emtfHit_size)            = 0;

  // EMTF Unpacked Hits
  emtfUnpHit_endcap          ->clear();
  emtfUnpHit_station         ->clear();
  emtfUnpHit_ring            ->clear();
  emtfUnpHit_sector          ->clear();
  emtfUnpHit_subsector       ->clear();
  emtfUnpHit_chamber         ->clear();
  emtfUnpHit_cscid           ->clear();
  emtfUnpHit_bx              ->clear();
  emtfUnpHit_type            ->clear();
  emtfUnpHit_neighbor        ->clear();
  //
  emtfUnpHit_strip           ->clear();
  emtfUnpHit_wire            ->clear();
  emtfUnpHit_roll            ->clear();
  emtfUnpHit_quality         ->clear();
  emtfUnpHit_pattern         ->clear();
  emtfUnpHit_bend            ->clear();
  emtfUnpHit_time            ->clear();
  emtfUnpHit_fr              ->clear();
  emtfUnpHit_emtf_phi        ->clear();
  emtfUnpHit_emtf_theta      ->clear();
  //
  emtfUnpHit_sim_phi         ->clear();
  emtfUnpHit_sim_theta       ->clear();
  emtfUnpHit_sim_r           ->clear();
  emtfUnpHit_sim_z           ->clear();
  // emtfUnpHit_sim_tp1         ->clear();
  // emtfUnpHit_sim_tp2         ->clear();
  (*emtfUnpHit_size)         = 0;

  // EMTF Tracks
  emtfTrack_pt               ->clear();
  emtfTrack_xml_pt           ->clear();
  emtfTrack_pt_dxy           ->clear();
  emtfTrack_dxy              ->clear();
  emtfTrack_invpt_prompt     ->clear();
  emtfTrack_invpt_displ      ->clear();
  emtfTrack_phi              ->clear();
  emtfTrack_phi_fp           ->clear();
  emtfTrack_theta            ->clear();
  emtfTrack_theta_fp         ->clear();
  emtfTrack_eta              ->clear();
  emtfTrack_GMT_phi          ->clear();
  emtfTrack_GMT_eta          ->clear();
  emtfTrack_q                ->clear();
  //
  emtfTrack_address          ->clear();
  emtfTrack_mode             ->clear();
  emtfTrack_endcap           ->clear();
  emtfTrack_sector           ->clear();
  emtfTrack_bx               ->clear();
  emtfTrack_nhits            ->clear();
  emtfTrack_hitref1          ->clear();
  emtfTrack_hitref2          ->clear();
  emtfTrack_hitref3          ->clear();
  emtfTrack_hitref4          ->clear();
  (*emtfTrack_size)          = 0;

  // EMTF Unpacked Tracks
  emtfUnpTrack_pt            ->clear();
  emtfUnpTrack_xml_pt        ->clear();
  emtfUnpTrack_pt_dxy        ->clear();
  emtfUnpTrack_dxy           ->clear();
  emtfUnpTrack_invpt_prompt  ->clear();
  emtfUnpTrack_invpt_displ   ->clear();
  emtfUnpTrack_phi           ->clear();
  emtfUnpTrack_theta         ->clear();
  emtfUnpTrack_eta           ->clear();
  emtfUnpTrack_q             ->clear();
  //
  emtfUnpTrack_address       ->clear();
  emtfUnpTrack_mode          ->clear();
  emtfUnpTrack_endcap        ->clear();
  emtfUnpTrack_sector        ->clear();
  emtfUnpTrack_bx            ->clear();
  emtfUnpTrack_nhits         ->clear();
  emtfUnpTrack_hitref1       ->clear();
  emtfUnpTrack_hitref2       ->clear();
  emtfUnpTrack_hitref3       ->clear();
  emtfUnpTrack_hitref4       ->clear();
  (*emtfUnpTrack_size)       = 0;

  // GMT Muons
  gmtMuon_pt                 ->clear();
  gmtMuon_pt_dxy             ->clear();
  gmtMuon_dxy                ->clear();
  gmtMuon_phi                ->clear();
  gmtMuon_eta                ->clear();
  gmtMuon_q                  ->clear();
  gmtMuon_qual               ->clear();
  (*gmtMuon_size)            = 0;

  // GMT Unpacked Muons
  gmtUnpMuon_pt              ->clear();
  gmtUnpMuon_pt_dxy          ->clear();
  gmtUnpMuon_dxy             ->clear();
  gmtUnpMuon_phi             ->clear();
  gmtUnpMuon_eta             ->clear();
  gmtUnpMuon_q               ->clear();
  gmtUnpMuon_qual            ->clear();
  (*gmtUnpMuon_size)         = 0;

  // GEN particles
  genPart_pt                 ->clear();
  genPart_dxy                ->clear();
  genPart_eta                ->clear();
  genPart_phi                ->clear();
  genPart_q                  ->clear();
  genPart_ID                 ->clear();
  genPart_parentID           ->clear();
  genPart_vx                 ->clear();
  genPart_vy                 ->clear();
  genPart_vz                 ->clear();

  // Event info
  eventInfo_event            ->clear();
  eventInfo_run              ->clear();
  eventInfo_lumi             ->clear();
  eventInfo_npv              ->clear();
  eventInfo_nvtx             ->clear();
  (*eventInfo_size)          = 0;

}

// -------------------------------------------------------
// define this as a plug-in
DEFINE_FWK_MODULE(EMTFNtuple);
