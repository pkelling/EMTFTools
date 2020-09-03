# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: l1NtupleMC -s RAW2DIGI --era=Run2_2016 --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAW --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleEMU --conditions=106X_mcRun2_asymptotic_preVFP_v5 -n 10 --mc --no_exec --no_output --filein=/eos/cms/store/relval/CMSSW_10_6_8/RelValZMM_13UP16/GEN-SIM/106X_mcRun2_asymptotic_v8_UL16hltval_preVFP_v5-v1/10000/1336A7AE-A915-294E-A2C9-75E8AA7AA5DB.root
import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Phase2_cff import Phase2

process = cms.Process('RAW2DIGI',Phase2)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2026D58Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2026D58_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)                                                                     

# Input source
process.source = cms.Source("PoolSource",
   # fileNames = cms.untracked.vstring("/store/mc/Run3Winter20DRPremixMiniAOD/HTo2LongLivedTo4mu_MH-125_MFF-50_CTau-3000mm_TuneCP5_14TeV_pythia8/GEN-SIM-RAW/110X_mcRun3_2021_realistic_v6-v2/50000/E974FDF8-40E1-064C-ADC4-8E1E99769637.root",)
#                                     "/store/mc/Run3Winter20DRPremixMiniAOD/HTo2LongLivedTo4mu_MH-125_MFF-50_CTau-3000mm_TuneCP5_14TeV_pythia8/GEN-SIM-RAW/110X_mcRun3_2021_realistic_v6-v2/50000/9BD931C1-FDD0-E94F-A3B3-25409D9D55B5.root",
#                                     "/store/mc/Run3Winter20DRPremixMiniAOD/HTo2LongLivedTo4mu_MH-125_MFF-50_CTau-3000mm_TuneCP5_14TeV_pythia8/GEN-SIM-RAW/110X_mcRun3_2021_realistic_v6-v2/50000/8441052B-C153-454C-A18D-71602DB5C99A.root",
#                                     "/store/mc/Run3Winter20DRPremixMiniAOD/HTo2LongLivedTo4mu_MH-125_MFF-50_CTau-3000mm_TuneCP5_14TeV_pythia8/GEN-SIM-RAW/110X_mcRun3_2021_realistic_v6-v2/10000/ED7E0E05-55F1-754B-B073-46C95DF34752.root",
#                                     "/store/mc/Run3Winter20DRPremixMiniAOD/HTo2LongLivedTo4mu_MH-125_MFF-50_CTau-3000mm_TuneCP5_14TeV_pythia8/GEN-SIM-RAW/110X_mcRun3_2021_realistic_v6-v2/10000/E0D9A0F7-C6DB-1543-A8A5-6A2B80A80BC3.root",
#                                     "/store/mc/Run3Winter20DRPremixMiniAOD/HTo2LongLivedTo4mu_MH-125_MFF-50_CTau-3000mm_TuneCP5_14TeV_pythia8/GEN-SIM-RAW/110X_mcRun3_2021_realistic_v6-v2/10000/C6EA3BCC-77C9-D740-94FF-6252AC74C2DE.root",
#                                     "/store/mc/Run3Winter20DRPremixMiniAOD/HTo2LongLivedTo4mu_MH-125_MFF-50_CTau-3000mm_TuneCP5_14TeV_pythia8/GEN-SIM-RAW/110X_mcRun3_2021_realistic_v6-v2/10000/BE23F547-43CC-B847-A4BF-6B6F38EEC081.root",
#                                     "/store/mc/Run3Winter20DRPremixMiniAOD/HTo2LongLivedTo4mu_MH-125_MFF-50_CTau-3000mm_TuneCP5_14TeV_pythia8/GEN-SIM-RAW/110X_mcRun3_2021_realistic_v6-v2/10000/B3D0B1FC-A0B7-6E4D-80E2-3C2B38EDDFB2.root",
#                                     "/store/mc/Run3Winter20DRPremixMiniAOD/HTo2LongLivedTo4mu_MH-125_MFF-50_CTau-3000mm_TuneCP5_14TeV_pythia8/GEN-SIM-RAW/110X_mcRun3_2021_realistic_v6-v2/10000/A07FE134-DA90-7045-94CA-8BE8CD489191.root",
#                                     "/store/mc/Run3Winter20DRPremixMiniAOD/HTo2LongLivedTo4mu_MH-125_MFF-50_CTau-3000mm_TuneCP5_14TeV_pythia8/GEN-SIM-RAW/110X_mcRun3_2021_realistic_v6-v2/10000/8708E537-16EF-A447-AD72-7F214E48B8D7.root",
#                                     "/store/mc/Run3Winter20DRPremixMiniAOD/HTo2LongLivedTo4mu_MH-125_MFF-50_CTau-3000mm_TuneCP5_14TeV_pythia8/GEN-SIM-RAW/110X_mcRun3_2021_realistic_v6-v2/10000/5B0F360A-902E-4948-BAC7-0A678C6162F5.root",
#                                     "/store/mc/Run3Winter20DRPremixMiniAOD/HTo2LongLivedTo4mu_MH-125_MFF-50_CTau-3000mm_TuneCP5_14TeV_pythia8/GEN-SIM-RAW/110X_mcRun3_2021_realistic_v6-v2/10000/48A1D0AB-8E1C-5447-8CB0-698862150281.root",
#                                     "/store/mc/Run3Winter20DRPremixMiniAOD/HTo2LongLivedTo4mu_MH-125_MFF-50_CTau-3000mm_TuneCP5_14TeV_pythia8/GEN-SIM-RAW/110X_mcRun3_2021_realistic_v6-v2/10000/370D58DE-C5A2-1145-96A0-4B910A280074.root",
#                                     "/store/mc/Run3Winter20DRPremixMiniAOD/HTo2LongLivedTo4mu_MH-125_MFF-50_CTau-3000mm_TuneCP5_14TeV_pythia8/GEN-SIM-RAW/110X_mcRun3_2021_realistic_v6-v2/10000/13625078-168A-9A4F-BD90-9D105B09C04E.root",)
   # fileNames = cms.untracked.vstring("/store/mc/Phase2HLTTDRSummer20ReRECOMiniAOD/DYJetsToLL_M-10to50_TuneCP5_14TeV-madgraphMLM-pythia8/GEN-SIM-DIGI-RAW-MINIAOD/PU200_111X_mcRun4_realistic_T15_v1-v1/120000/0744744E-BB24-AD49-8283-F7D21F5496F7.root",
   #                                    "/store/mc/Phase2HLTTDRSummer20ReRECOMiniAOD/DYJetsToLL_M-10to50_TuneCP5_14TeV-madgraphMLM-pythia8/GEN-SIM-DIGI-RAW-MINIAOD/PU200_111X_mcRun4_realistic_T15_v1-v1/120000/06B3AA28-EA77-DB4E-9E9C-FADC51AB37D4.root",
   #                                    "/store/mc/Phase2HLTTDRSummer20ReRECOMiniAOD/DYJetsToLL_M-10to50_TuneCP5_14TeV-madgraphMLM-pythia8/GEN-SIM-DIGI-RAW-MINIAOD/PU200_111X_mcRun4_realistic_T15_v1-v1/120000/0608B460-D7D0-AC48-A291-DD86D99A73A2.root")
   # fileNames = cms.untracked.vstring("/store/mc/Phase2HLTTDRSummer20ReRECOMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/FEVT/PU200_pilot_111X_mcRun4_realistic_T15_v1-v1/100000/48AC7E45-88D3-9D4E-82AD-E1A664AC217F.root",
   #                                    "/store/mc/Phase2HLTTDRSummer20ReRECOMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/FEVT/PU200_pilot_111X_mcRun4_realistic_T15_v1-v1/100000/4864A957-CBDC-E440-8F46-7C96A15256C9.root",
   #                                    "/store/mc/Phase2HLTTDRSummer20ReRECOMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/FEVT/PU200_pilot_111X_mcRun4_realistic_T15_v1-v1/100000/485742A7-BC00-A04A-B711-10FFA2737382.root",
   #                                    "/store/mc/Phase2HLTTDRSummer20ReRECOMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/FEVT/PU200_pilot_111X_mcRun4_realistic_T15_v1-v1/100000/4847E649-46A8-D449-89F9-C06A1026DF74.root",
   #                                    "/store/mc/Phase2HLTTDRSummer20ReRECOMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/FEVT/PU200_pilot_111X_mcRun4_realistic_T15_v1-v1/100000/477955C9-4357-C64D-8355-84E43FD92C88.root",
   #                                    "/store/mc/Phase2HLTTDRSummer20ReRECOMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/FEVT/PU200_pilot_111X_mcRun4_realistic_T15_v1-v1/100000/470E0111-1E38-2D4D-BBC0-CC6C6DEE1807.root",
   #                                    "/store/mc/Phase2HLTTDRSummer20ReRECOMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/FEVT/PU200_pilot_111X_mcRun4_realistic_T15_v1-v1/100000/46E125D9-E022-DA47-A4FD-B91754405B6F.root",
   #                                    "/store/mc/Phase2HLTTDRSummer20ReRECOMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/FEVT/PU200_pilot_111X_mcRun4_realistic_T15_v1-v1/100000/460CAB56-4158-9243-879F-B3EA098B4F6B.root",
   #                                    "/store/mc/Phase2HLTTDRSummer20ReRECOMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/FEVT/PU200_pilot_111X_mcRun4_realistic_T15_v1-v1/100000/45C9AD58-EA19-8546-93B2-E803F1A68D6B.root",
   #                                    "/store/mc/Phase2HLTTDRSummer20ReRECOMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/FEVT/PU200_pilot_111X_mcRun4_realistic_T15_v1-v1/100000/458627BC-D94D-594A-A6B7-3253E7471B4D.root",
   #                                    "/store/mc/Phase2HLTTDRSummer20ReRECOMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/FEVT/PU200_pilot_111X_mcRun4_realistic_T15_v1-v1/100000/44FB5C49-2392-724C-A64C-8F9B90156389.root",
   #                                    "/store/mc/Phase2HLTTDRSummer20ReRECOMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/FEVT/PU200_pilot_111X_mcRun4_realistic_T15_v1-v1/100000/446FC09F-AC88-E14E-B110-2C8A7DEAE1A1.root",
   #                                    "/store/mc/Phase2HLTTDRSummer20ReRECOMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/FEVT/PU200_pilot_111X_mcRun4_realistic_T15_v1-v1/100000/43DA9306-CB72-F04D-8F8C-BCDC183A7EA8.root",
   #                                    "/store/mc/Phase2HLTTDRSummer20ReRECOMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/FEVT/PU200_pilot_111X_mcRun4_realistic_T15_v1-v1/100000/43CBD0B7-200B-904C-972F-440BCD84439A.root",
   #                                    "/store/mc/Phase2HLTTDRSummer20ReRECOMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/FEVT/PU200_pilot_111X_mcRun4_realistic_T15_v1-v1/100000/43523CC0-C1F2-834D-819E-2B247A2689D5.root",
   #                                    "/store/mc/Phase2HLTTDRSummer20ReRECOMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/FEVT/PU200_pilot_111X_mcRun4_realistic_T15_v1-v1/100000/42F4FB16-9242-234F-907C-DDA0A0D8BC46.root",)

   fileNames = cms.untracked.vstring("/store/mc/Phase2HLTTDRSummer20ReRECOMiniAOD/DYToLL_M-10To50_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW-MINIAOD/PU140_pilot_111X_mcRun4_realistic_T15_v1-v1/110000/FC6ABE47-E55E-FA48-8669-3E8E16874FD4.root",),


)

process.options = cms.untracked.PSet(

)
readFiles = cms.untracked.vstring();
readFiles2 = cms.untracked.vstring();



# process.source = cms.Source("PoolSource", fileNames = readFiles, secondaryFileNames = readFiles2)

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )


# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('l1NtupleMC nevts:-1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
# process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
process.GlobalTag = GlobalTag(process.GlobalTag, '111X_mcRun4_realistic_T15_v1', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.endjob_step = cms.EndPath(process.endOfProcess)

process.load('EMTFTools.EMTFNtuple.EMTFNtupleMaker_cfi')
process.TFileService = cms.Service('TFileService', fileName = process.EMTFNtuple.outFileName)
process.ntuple_step = cms.Path(process.EMTFNtuple)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.endjob_step, process.ntuple_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)



# End of customisation functions

# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion