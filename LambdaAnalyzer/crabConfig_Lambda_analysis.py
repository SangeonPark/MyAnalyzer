from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'JUN2015_Lambda_analysis'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'lambdaanalyzer_cfg.py'

config.Data.inputDataset = '/PAMinBiasUPC/davidlw-PA2013_FlowCorr_PromptReco_MB_Gplus_ReTracking_v18-25c9a89be536a41c8ccb3c75e9fd9358/USER'
config.Data.inputDBS = 'phys03'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 20
config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication = True
config.Data.publishDataName = 'CRAB3_tutorial_JUN2015_Lambda_analysis'

config.Site.storageSite = 'T2_US_MIT'
