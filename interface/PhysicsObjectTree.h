#ifndef Analysis_Tools_PhysicsObjectTree_h
#define Analysis_Tools_PhysicsObjectTree_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      PhysicsObjectTree
//
/**\class xxx PhysicsObjectTree.cc Analysis/Tools/src/PhysicsObjectTree.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Roberval Walsh Bastos Rangel
//         Created:  Mon, 20 Oct 2014 14:24:08 GMT
//
//

// system include files
#include <memory>
#include <vector>
//
// user include files

#include "TTree.h"
#include "TChain.h"
#include "Analysis/Tools/interface/PhysicsObjectTreeBase.h"
#include "Analysis/Tools/interface/Collection.h"

//
// class declaration
//

namespace analysis {
   namespace tools {

      template <class Object>
      class PhysicsObjectTree : public PhysicsObjectTreeBase<Object> {
         public:
            PhysicsObjectTree();
            PhysicsObjectTree(TChain * tree, const std::string & name);
           ~PhysicsObjectTree();

            Collection<Object> collection();

            // ----------member data ---------------------------
         protected:

         private:

      };

      // Specialization for JET
      template <>
      class PhysicsObjectTree<Jet> : public PhysicsObjectTreeBase<Jet> {
         public:
            PhysicsObjectTree();
            PhysicsObjectTree(TChain * tree, const std::string & name);
           ~PhysicsObjectTree();

            Collection<Jet> collection();

            // ----------member data ---------------------------
         protected:
            // PatJets
            float btag_    [max_];
            float btags_[15][max_];
            std::map<std::string, float*> mbtag_;
            int   flavour_ [max_];
            int   hadrflavour_ [max_];
            int   partflavour_ [max_];
            int   physflavour_ [max_];
            float nHadFrac_[max_];
            float nEmFrac_ [max_];
            float nMult_   [max_];
            float cHadFrac_[max_];
            float cEmFrac_ [max_];
            float cMult_   [max_];
            float muFrac_  [max_];
            float puppi_   [max_];
            bool  idLoose_ [max_];
            bool  idTight_ [max_];
            float jecUnc_  [max_];
            float jerSF_   [max_];
            float jerSFUp_ [max_];
            float jerSFDown_ [max_];
            float jerResolution_ [max_];
            float qgLikelihood_ [max_];
            float puJetIdFullDisc_ [max_];
            int   puJetIdFullId_ [max_];
            
            float bRegCorr_ [max_];
            float bRegRes_  [max_];
            
            bool isSimpleJet_;
            bool hasPuppiInfo_;
            bool hasQGLikelihood_;
            bool hasBRegCorr_;
            bool hasBRegRes_;
            
            bool hasRho_;
            double rho_;

         private:

      };
         
      // Specialization for Candidate
      template <>
      class PhysicsObjectTree<Candidate> : public PhysicsObjectTreeBase<Candidate> {
         public:
            PhysicsObjectTree();
            PhysicsObjectTree(TChain * tree, const std::string & name);
           ~PhysicsObjectTree();

            Collection<Candidate> collection();

            // ----------member data ---------------------------
         protected:

         private:

      };  

      // Specialization for GENPARTICLE
      template <>
      class PhysicsObjectTree<GenParticle> : public PhysicsObjectTreeBase<GenParticle> {
         public:
            PhysicsObjectTree();
            PhysicsObjectTree(TChain * tree, const std::string & name);
           ~PhysicsObjectTree();

            Collection<GenParticle> collection();

            // ----------member data ---------------------------
         protected:
            // PatJets
            int   pdgid_    [max_];
            int   status_   [max_];
            bool  higgs_dau_[max_];
            int   indx_     [max_];
            int   mo1_      [max_];
            int   mo2_      [max_];
            int   da1_      [max_];
            int   da2_      [max_];
            
            bool gp_has_indx_;

         private:

      };

      // Specialization for MET
      template <>
      class PhysicsObjectTree<MET> : public PhysicsObjectTreeBase<MET>{
         public:
            PhysicsObjectTree();
            PhysicsObjectTree(TChain * tree, const std::string & name);
           ~PhysicsObjectTree();

            Collection<MET> collection();
            // ----------member data ---------------------------
         protected:
            // METs
            float sigxx_  [max_];
            float sigxy_  [max_];
            float sigyx_  [max_];
            float sigyy_  [max_];

            float gen_px_ [max_];
            float gen_py_ [max_];
            float gen_pz_ [max_];

         private:

      };

      // Specialization for Muon
      template <>
      class PhysicsObjectTree<Muon> : public PhysicsObjectTreeBase<Muon> {
         public:
            PhysicsObjectTree();
            PhysicsObjectTree(TChain * tree, const std::string & name);
           ~PhysicsObjectTree();

            Collection<Muon> collection();

            // ----------member data ---------------------------
         protected:
           bool  isPFMuon_        [max_];
           bool  isGlobalMuon_    [max_]; 
           bool  isTrackerMuon_   [max_]; 
           bool  isLooseMuon_     [max_]; 
           bool  isMediumMuon_    [max_];
           bool  isTightMuon_     [max_];

           float validFraction_          [max_];
           float segmentCompatibility_   [max_]; 
           float trkKink_                [max_]; 
           float chi2LocalPos_           [max_];

           float normChi2_  [max_];
           
           bool hasTightMuon_;
           
         private:

      };

               // Specialization for JetTag
      template <>
      class PhysicsObjectTree<JetTag> : public PhysicsObjectTreeBase<JetTag> {
         public:
            PhysicsObjectTree();
            PhysicsObjectTree(TChain * tree, const std::string & name);
           ~PhysicsObjectTree();

            Collection<JetTag> collection();

            // ----------member data ---------------------------
         protected:
            float btag_[max_] ;
         private:

      };

      // Specialization for GenJet
      template <>
      class PhysicsObjectTree<GenJet> : public PhysicsObjectTreeBase<GenJet> {
         public:
            PhysicsObjectTree();
            PhysicsObjectTree(TChain * tree, const std::string & name);
           ~PhysicsObjectTree();

            Collection<GenJet> collection();

            // ----------member data ---------------------------
         protected:

         private:

      };

      // Specialization for Vertex
      template <>
      class PhysicsObjectTree<Vertex> : public PhysicsObjectTreeBase<Vertex> {
         public:
            PhysicsObjectTree();
            PhysicsObjectTree(TChain * tree, const std::string & name);
           ~PhysicsObjectTree();

            Collection<Vertex> collection();

            // ----------member data ---------------------------
         protected:

         private:

      };

      // Specialization for TriggerObject
      template <>
      class PhysicsObjectTree<TriggerObject> : public PhysicsObjectTreeBase<TriggerObject> {
         public:
            PhysicsObjectTree();
            PhysicsObjectTree(TChain * tree, const std::string & name);
           ~PhysicsObjectTree();

            Collection<TriggerObject> collection();

            // ----------member data ---------------------------
         protected:
            int type_    [max_];

         private:
      };
         
      // Specialization for L1TMuon
      template <>
      class PhysicsObjectTree<L1TMuon> : public PhysicsObjectTreeBase<L1TMuon> {
         public:
            PhysicsObjectTree();
            PhysicsObjectTree(TChain * tree, const std::string & name);
           ~PhysicsObjectTree();

            Collection<L1TMuon> collection();

            // ----------member data ---------------------------
         protected:
           int    hwQual_   [max_];
           float  etaAtVtx_ [max_]; 
           float  phiAtVtx_ [max_]; 
           
         private:

      };

      // Specialization for L1TJet
      template <>
      class PhysicsObjectTree<L1TJet> : public PhysicsObjectTreeBase<L1TJet> {
         public:
            PhysicsObjectTree();
            PhysicsObjectTree(TChain * tree, const std::string & name);
           ~PhysicsObjectTree();

            Collection<L1TJet> collection();

            // ----------member data ---------------------------
         protected:
           
         private:

      };
                  
      // Specialization for RecoMuon
      template <>
      class PhysicsObjectTree<RecoMuon> : public PhysicsObjectTreeBase<RecoMuon> {
         public:
            PhysicsObjectTree();
            PhysicsObjectTree(TChain * tree, const std::string & name);
           ~PhysicsObjectTree();

            Collection<RecoMuon> collection();

            // ----------member data ---------------------------
         protected:

         private:

      };  

      // Specialization for RecoTrack
      template <>
      class PhysicsObjectTree<RecoTrack> : public PhysicsObjectTreeBase<RecoTrack> {
         public:
            PhysicsObjectTree();
            PhysicsObjectTree(TChain * tree, const std::string & name);
           ~PhysicsObjectTree();

            Collection<RecoTrack> collection();

            // ----------member data ---------------------------
         protected:
            float chi2_[max_];
            float ndof_[max_];
            float d0_[max_];
            float dxy_[max_];
            int   nLostMuHits_[max_];
            int   nBadMuHits_[max_];
            int   nValMuHits_[max_];
            int   nValTrackerHits_[max_];
            int   nValStripTECHits_[max_];
            int   nValStripTIBHits_[max_];
            int   nValStripTIDHits_[max_];
            int   nValStripTOBHits_[max_];
            int   muStationsWithValHits_[max_];
            int   muStationsWithBadHits_[max_];
            int   inMuStationWithValHits_[max_];
            int   outMuStationWithValHits_[max_];
            std::map<TrackQuality,bool*> mqual_;
            bool  qual_[10][max_];


         private:

      };  
   }
}

#endif  // Analysis_Tools_PhysicsObjectTree_h
