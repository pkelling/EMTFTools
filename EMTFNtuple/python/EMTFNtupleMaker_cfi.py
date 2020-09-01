import FWCore.ParameterSet.Config as cms

EMTFNtuple = cms.EDAnalyzer('EMTFNtuple',

    CSCInputTag     = cms.InputTag('simCscTriggerPrimitiveDigis','MPCSORTED'), 
    RPCInputTag     = cms.InputTag('simMuonRPCDigis'), 
    CPPFInputTag    = cms.InputTag('simCPPFDigis'), 
    GEMInputTag     = cms.InputTag('simMuonGEMPadDigiClusters'), 

    IRPCInputTag    = cms.InputTag('simMuonRPCDigis'), 
    ME0InputTag     = cms.InputTag('me0TriggerConvertedPseudoDigis'), 
    DTInputTag      = cms.InputTag('simDtTriggerPrimitiveDigis'), 

    enablePhase2    = cms.bool(False),
    enableCPPF      = cms.bool(False),



    EMTFHitTag      = cms.InputTag('simEmtfDigis'),
    EMTFTrackTag    = cms.InputTag('simEmtfDigis'),
    GMTMuonTag      = cms.InputTag('simGmtStage2Digis'),
    GENPartTag      = cms.InputTag('genParticles'),


    outFileName     = cms.string('ntuple.root'),
    verbosity       = cms.untracked.int32(0),



  )