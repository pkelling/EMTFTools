import FWCore.ParameterSet.Config as cms

MuShowerNtuple = cms.EDAnalyzer('MuShowerNtuple',

    CSCShowerDigiInputTag      = cms.InputTag("simCscTriggerPrimitiveDigis"),
    EMTFShowerDigiInputTag     = cms.InputTag('emtfStage2Digis'),
    EMTFRegionalShowerInputTag  = cms.InputTag('emtfStage2Digis'),
    GMTRegionalShowerInputTag  = cms.InputTag('gmtStage2Digis', 'EMTF'),
    GMTShowerInputTag      = cms.InputTag('gmtStage2Digis', 'MuonShower'),

    useCSCShowerDigis           = cms.bool(True),
    useEMTFShowerDigis           = cms.bool(True),
    useEMTFRegionalShowers           = cms.bool(True),
    useGMTRegionalShowers           = cms.bool(True),
    useGMTShowers           = cms.bool(True),


)
