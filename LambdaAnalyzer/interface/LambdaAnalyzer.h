// -*- C++ -*-
//
// Package:    LambdaAnalyzer
// Class:      LambdaAnalyzer
// 
/**\class LambdaAnalyzer LambdaAnalyzer.h Lambda/LambdaAnalyzer/interface/LambdaAnalyzer.h

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Sang Eon Park,,,
//         Created:  Wed Jun  3 23:19:31 CEST 2015
// $Id$
//
//


#ifndef LAMBDA_ANALYZER_H
#define LAMBDA_ANALYZER_H

// system include files
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <math.h>


// user include files        
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidateFwd.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include <TString.h>
#include <TVector3.h>
#include <TMatrixD.h>

#include <Math/Functions.h>
#include <Math/SVector.h>
#include <Math/SMatrix.h>


class LambdaAnalyzer : public edm::EDAnalyzer {
    
public:
    explicit LambdaAnalyzer(const edm::ParameterSet&);
    ~LambdaAnalyzer();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


private:
    virtual void beginJob() ;
    virtual void produce(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    etaMin_trg_ = iConfig.getUntrackedParameter<double>("etaMin_trg", -2.4);
    etaMax_trg_ = iConfig.getUntrackedParameter<double>("etaMax_trg", 2.4);
    etaMin_ass_ = iConfig.getUntrackedParameter<double>("etaMin_ass", -2.4);
    etaMax_ass_ = iConfig.getUntrackedParameter<double>("etaMax_ass", 2.4);
    multMax_ = iConfig.getUntrackedParameter<double>("multMax", 220);
    multMin_ = iConfig.getUntrackedParameter<double>("multMin", 185);
    bkgFactor_ = iConfig.getUntrackedParameter<int>("bkgFactor", 10);
    mis_ks_range_ = iConfig.getUntrackedParameter<double>("mis_ks_range", 0.020);
    mis_la_range_ = iConfig.getUntrackedParameter<double>("mis_la_range", 0.010);
    mis_ph_range_ = iConfig.getUntrackedParameter<double>("mis_ph_range", 0.015);

    

    TNtuple* V0_Lambda;
    TNtuple* V0_Kshort;
    TString v0CandidateCollection;


  
  double etaMin_trg_;
  double etaMax_trg_;
  double etaMin_ass_;
  double etaMax_ass_;
  double multMax_;
  double multMin_;
  int bkgFactor_;
  double mis_ks_range_;
  double mis_la_range_;
  double mis_ph_range_;      
      
      


    // ----------member data ---------------------------
};

#endif
