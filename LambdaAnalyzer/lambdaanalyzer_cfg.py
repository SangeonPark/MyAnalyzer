import FWCore.ParameterSet.Config as cms

#set up a process
process = cms.Process("TEST")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold ='INFO'
process.MessageLogger.categories.append('TEST')
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
        limit = cms.untracked.int32(-1)
        )
               

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(5000)
process.options   = cms.untracked.PSet( wantSummary = 
cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


#this inputs the input files
process.source = cms.Source("PoolSource",fileNames = cms.untracked.vstring())


process.V0 = cms.EDAnalyzer('LambdaAnalyzer', 
multMax = cms.untracked.double(10000),
multMin = cms.untracked.double(1),
generalV0Candidates = cms.untracked.string('generalV0Candidates')
                            )

# talk to the output module

process.TFileService = cms.Service("TFileService",
                                       fileName = 
cms.string('V0LambdaKshort.root')                                   )

process.p = cms.Path(process.V0)
