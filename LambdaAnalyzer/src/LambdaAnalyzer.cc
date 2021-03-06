// -*- C++ -*-
//
// Package:    LambdaAnalyzer
// Class:      LambdaAnalyzer
// 
/**\class LambdaAnalyzer LambdaAnalyzer.cc Lambda/LambdaAnalyzer/src/LambdaAnalyzer.cc

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


// system include files
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <math.h>

#include <TH1D.h>
#include <TH2D.h>
#include <TNtuple.h>
#include <TFile.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TString.h>
#include <TObjString.h>
#include <TCanvas.h>
#include <TVector3.h>
#include <TMatrixD.h>
#include <TRandom.h>
#include <TNtuple.h>


// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"

#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidateFwd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/TrackReco/interface/DeDxData.h"

#include <Math/Functions.h>
#include <Math/SVector.h>
#include <Math/SMatrix.h>
#include <memory>

//
// constants, enums and typedefs
//
const double piMass = 0.13957018;
const double piMassSquared = piMass*piMass;
const double protonMass = 0.93827203;
const double protonMassSquared = protonMass*protonMass;
const double electronMass = 0.000511;
const double electronMassSquared = electronMass*electronMass;
const double lambdaMass = 1.115683;
const double kshortMass = 0.497614;



class LambdaAnalyzer : public edm::EDAnalyzer {
public:
  explicit LambdaAnalyzer(const edm::ParameterSet&);
  ~LambdaAnalyzer();


private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  // ----------member data ---------------------------
    
  TNtuple* V0_Kshort;
  TNtuple* V0_Lambda;
    
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
    
  TString v0CandidateCollection;
};


//
// constructors and destructor
//
LambdaAnalyzer::LambdaAnalyzer(const edm::ParameterSet& iConfig)

{
    using std::string;
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



    v0CandidateCollection = iConfig.getUntrackedParameter<string>("generalV0Candidates");

	

}


LambdaAnalyzer::~LambdaAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// producer method ( method called for each event)

void
LambdaAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using std::vector;
    using namespace edm;
    using namespace std;



    //select on requirement of valid vertex 
    edm::Handle<reco::VertexCollection> vertices;
    iEvent.getByLabel("offlinePrimaryVertices",vertices);
    double bestvz=-999.9, bestvx=-999.9, bestvy=-999.9;
    
    double bestvzError=-999.9, bestvxError=-999.9, bestvyError=-999.9;

    const reco::Vertex & vtx = (*vertices)[0];
    bestvz = vtx.z(); bestvx = vtx.x(); bestvy = vtx.y();
    bestvzError = vtx.zError(); bestvxError = vtx.xError(); bestvyError = vtx.yError();

    if(bestvz < -15.0 || bestvz>15.0) return;
   
    edm::Handle<reco::VertexCompositeCandidateCollection> v0candidates_ks;
    iEvent.getByLabel(v0CandidateCollection.Data(),"Kshort",v0candidates_ks);
    if(!v0candidates_ks.isValid()) return;
    
    edm::Handle<reco::VertexCompositeCandidateCollection> v0candidates_la;
    iEvent.getByLabel(v0CandidateCollection.Data(),"Lambda",v0candidates_la);
    if(!v0candidates_la.isValid()) return;
    
    edm::Handle<edm::ValueMap<reco::DeDxData> > dEdxHandle;
    iEvent.getByLabel("dedxTruncated40", dEdxHandle);

    for(unsigned it=0; it<v0candidates_ks->size(); it++){

	const reco::VertexCompositeCandidate & trk = (*v0candidates_ks)[it];

	double secvz=-999.9, secvx=-999.9, secvy=-999.9;

	//pt and mass

	double eta = trk.eta();
        double pt = trk.pt();
        double px = trk.px();
        double py = trk.py();
        double pz = trk.pz();
        double mass = trk.mass();

	if(eta>etaMax_trg_ || eta<etaMin_trg_) continue;
        
        const reco::Candidate * d1 = trk.daughter(0);
        const reco::Candidate * d2 = trk.daughter(1);
        
        double pxd1 = d1->px();
        double pyd1 = d1->py();
        double pzd1 = d1->pz();
	//   double pd1 = d1->p();
        double pxd2 = d2->px();
        double pyd2 = d2->py();
        double pzd2 = d2->pz();
        //double pd2 = d2->p();
        
        TVector3 dauvec1(pxd1,pyd1,pzd1);
        TVector3 dauvec2(pxd2,pyd2,pzd2);
        
        TVector3 dauvecsum(dauvec1+dauvec2);
	//cout<<"basic ok"<<endl;
        
        auto dau1 = d1->get<reco::TrackRef>();
        auto dau2 = d2->get<reco::TrackRef>();
        
        double dedx1 = -999.9;
        double dedx2 = -999.9;

	//cout<<"TrackRef ok"<<endl;        


//dEdx,daughter charge, momentum, need p vs dEdx 2D

        if(dEdxHandle->size()){
            const edm::ValueMap<reco::DeDxData> dEdxTrack = *dEdxHandle.product();
            dedx1 = dEdxTrack[dau1].dEdx();
            dedx2 = dEdxTrack[dau2].dEdx();
        }
	
	//cout<<"dEdx ok"<<endl;

	
        double p1 = d1->p()*d1->charge();
        double p2 = d2->p()*d2->charge();


	//cout<<"momentum ok"<<endl;

        //vertexCovariance 00-xError 11-y 22-z
        secvz = trk.vz(); secvx = trk.vx(); secvy = trk.vy();

        
        //trkNHits
        double nhit1 = dau1->numberOfValidHits();
        double nhit2 = dau2->numberOfValidHits();
        
	//cout<<"NHits ok"<<endl;

        //DCA
        math::XYZPoint bestvtx(bestvx,bestvy,bestvz);
        
        double dzbest1 = dau1->dz(bestvtx);
        double dxybest1 = dau1->dxy(bestvtx);
        double dzerror1 = sqrt(dau1->dzError()*dau1->dzError()+bestvzError*bestvzError);
        double dxyerror1 = sqrt(dau1->d0Error()*dau1->d0Error()+bestvxError*bestvyError);
    
        double dzos1 = dzbest1/dzerror1;
        double dxyos1 = dxybest1/dxyerror1;
        
        double dzbest2 = dau2->dz(bestvtx);
        double dxybest2 = dau2->dxy(bestvtx);
        double dzerror2 = sqrt(dau2->dzError()*dau2->dzError()+bestvzError*bestvzError);
        double dxyerror2 = sqrt(dau2->d0Error()*dau2->d0Error()+bestvxError*bestvyError);
        
        double dzos2 = dzbest2/dzerror2;
        double dxyos2 = dxybest2/dxyerror2;
	//cout<<"DCA ok"<<endl;        
        //vtxChi2
        double vtxChi2 = trk.vertexChi2();
	//cout<<"vtx Chi2 ok"<<endl;        
        //PAngle
        TVector3 ptosvec(secvx-bestvx,secvy-bestvy,secvz-bestvz);
        TVector3 secvec(px,py,pz);
            
        double agl = cos(secvec.Angle(ptosvec));
        
        //Decay length
        typedef ROOT::Math::SMatrix<double, 3, 3, ROOT::Math::MatRepSym<double, 3> > SMatrixSym3D;
        typedef ROOT::Math::SVector<double, 3> SVector3;
        
        SMatrixSym3D totalCov = vtx.covariance() + trk.vertexCovariance();
        SVector3 distanceVector(secvx-bestvx,secvy-bestvy,secvz-bestvz);
        
        double dl = ROOT::Math::Mag(distanceVector);
        double dlerror = sqrt(ROOT::Math::Similarity(totalCov, distanceVector))/dl;
        
        
        double dlos = dl/dlerror;
        
        //Fill
        V0_Kshort->Fill(pt,mass,dzos1,dzos2,dxyos1,dxyos2,vtxChi2,nhit1,nhit2,dlos,agl,dedx1,dedx2,p1,p2);
	
    }


    for(unsigned it=0;it<v0candidates_la->size(); ++it){

      

	const reco::VertexCompositeCandidate & trk = (*v0candidates_la)[it];

	double secvz=-999.9, secvx=-999.9, secvy=-999.9;

	//pt,mass
        double eta = trk.eta();
        double pt = trk.pt();
        double px = trk.px();
        double py = trk.py();
        double pz = trk.pz();
        double mass = trk.mass();
        
        if(eta>etaMax_trg_ || eta<etaMin_trg_) continue;
        

        const reco::Candidate * d1 = trk.daughter(0);
        const reco::Candidate * d2 = trk.daughter(1);
        
        double pxd1 = d1->px();
        double pyd1 = d1->py();
        double pzd1 = d1->pz();
        double pd1 = d1->p();
        double pxd2 = d2->px();
        double pyd2 = d2->py();
        double pzd2 = d2->pz();
        double pd2 = d2->p();
        
        TVector3 dauvec1(pxd1,pyd1,pzd1);
        TVector3 dauvec2(pxd2,pyd2,pzd2);
        
        TVector3 dauvecsum(dauvec1+dauvec2);

        double v0masspipi = sqrt((sqrt(0.13957*0.13957+pd1*pd1)+sqrt(0.13957*0.13957+pd2*pd2))*(sqrt(0.13957*0.13957+pd1*pd1)+sqrt(0.13957*0.13957+pd2*pd2))-dauvecsum.Mag2());
        double v0massee = sqrt((sqrt(0.000511*0.000511+pd1*pd1)+sqrt(0.000511*0.000511+pd2*pd2))*(sqrt(0.000511*0.000511+pd1*pd1)+sqrt(0.000511*0.000511+pd2*pd2))-dauvecsum.Mag2());
        
        if( (v0masspipi>=(0.497614-mis_ks_range_) && v0masspipi<=(0.497614+mis_ks_range_)) || v0massee <= mis_ph_range_ ) continue;

	
        
      
	auto dau1 = d1->get<reco::TrackRef>();
        auto dau2 = d2->get<reco::TrackRef>();
        
        double dedx1 = -999.9;
        double dedx2 = -999.9;
        
        //dEdx
        if(dEdxHandle->size()){
            const edm::ValueMap<reco::DeDxData> dEdxTrack = *dEdxHandle.product();
            dedx1 = dEdxTrack[dau1].dEdx();
            dedx2 = dEdxTrack[dau2].dEdx();
        }
        
        double p1 = d1->p()*d1->charge();
        double p2 = d2->p()*d2->charge();

        secvz = trk.vz(); secvx = trk.vx(); secvy = trk.vy();
        //secvzError = sqrt(trk.vertexCovariance(2,2)); secvxError = sqrt(trk.vertexCovariance(0,0)); secvyError = sqrt(trk.vertexCovariance(1,1));
        
        //trkNHits
        double nhit1 = dau1->numberOfValidHits();
        double nhit2 = dau2->numberOfValidHits();
        
        //DCA
        math::XYZPoint bestvtx(bestvx,bestvy,bestvz);
        
        double dzbest1 = dau1->dz(bestvtx);
        double dxybest1 = dau1->dxy(bestvtx);
        double dzerror1 = sqrt(dau1->dzError()*dau1->dzError()+bestvzError*bestvzError);
        double dxyerror1 = sqrt(dau1->d0Error()*dau1->d0Error()+bestvxError*bestvyError);
        
        double dzos1 = dzbest1/dzerror1;
        double dxyos1 = dxybest1/dxyerror1;
        
        double dzbest2 = dau2->dz(bestvtx);
        double dxybest2 = dau2->dxy(bestvtx);
        double dzerror2 = sqrt(dau2->dzError()*dau2->dzError()+bestvzError*bestvzError);
        double dxyerror2 = sqrt(dau2->d0Error()*dau2->d0Error()+bestvxError*bestvyError);
        
        double dzos2 = dzbest2/dzerror2;
        double dxyos2 = dxybest2/dxyerror2;
        
        //vtxChi2
        double vtxChi2 = trk.vertexChi2();
        
        //PAngle
        TVector3 ptosvec(secvx-bestvx,secvy-bestvy,secvz-bestvz);
        TVector3 secvec(px,py,pz);
        
        double agl = cos(secvec.Angle(ptosvec));
        
        
        typedef ROOT::Math::SMatrix<double, 3, 3, ROOT::Math::MatRepSym<double, 3> > SMatrixSym3D;
        typedef ROOT::Math::SVector<double, 3> SVector3;
        
        SMatrixSym3D totalCov = vtx.covariance() + trk.vertexCovariance();
        SVector3 distanceVector(secvx-bestvx,secvy-bestvy,secvz-bestvz);
        
        double dl = ROOT::Math::Mag(distanceVector);
        double dlerror = sqrt(ROOT::Math::Similarity(totalCov, distanceVector))/dl;
        
        double dlos = dl/dlerror;
        
        //Fill
	float data[21]={ pt, mass, px, py, pz, pxd1, pyd1, pzd1, dzos1, dzos2, dxyos1, dxyos2, vtxChi2, nhit1, nhit2,
		       dlos, agl, dedx1, dedx2, p1, p2};
	//	v[0]=pt; v[1]=mass; v[2]=px; v[3]=py; v[4]=pz; v[5]=pxd1; v[6]=pyd1; v[7]=pzd1; v[8]=dzos1; v[9]=dzos2; v[10]=dxyos1; v[11]=dxyos2;

	//v[12]=vtxChi2; v[13]=nhit1; v[14]=nhit2; v[15]=dlos; v[16]=agl; v[17]=dedx1; v[18]=dedx2; v[19]=p1; v[20]=p2;

	V0_Lambda->Fill(data);




        
    }


}


// ------------ method called once each job just before starting event loop  ------------
void 
LambdaAnalyzer::beginJob()    
{
    edm::Service<TFileService> fs;

    TH1D::SetDefaultSumw2();
    V0_Kshort = fs->make< TNtuple>("V0_Kshort","V0_Kshort","pt:mass:trkDCAoSigma1z:trkDCAoSigma2z:trkDCAoSigma1xy:trkDCAoSigma2xy:vtxChi2:trkNHits1:trkNHits2:LoSigma:PAngle:dEdx1:dEdx2:mmt1:mmt2");
    V0_Lambda = fs->make< TNtuple>("V0_Lambda","V0_Lambda","pt:mass:px:py:pz:dau1px:dau1py:dau1pz:trkDCAoSigma1z:trkDCAoSigma2z:trkDCAoSigma1xy:trkDCAoSigma2xy:vtxChi2:trkNHits1:trkNHits2:LoSigma:PAngle:dEdx1:dEdx2:mmt1:mmt2");

}

// ------------ method called once each job just after ending the event loop  ------------
void 
LambdaAnalyzer::endJob() 
{
}


//define this as a plug-in
DEFINE_FWK_MODULE(LambdaAnalyzer);
