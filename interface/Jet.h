#ifndef Analysis_Tools_Jet_h
#define Analysis_Tools_Jet_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      Jet
// 
/**\class xxx Jet.cc Analysis/Tools/src/Jet.cc

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
#include <boost/any.hpp>
//

// 
// user include files
#include "Analysis/Tools/interface/Candidate.h"
#include "Analysis/Tools/interface/Muon.h"
#include "Analysis/Tools/interface/GenParticle.h"
#include "Analysis/Tools/interface/GenJet.h"
#include "Analysis/Tools/interface/BTagCalibrationStandalone.h"
#include "Analysis/Tools/interface/Utils.h"


//
// class declaration
//



using namespace JME;

namespace analysis {
   namespace tools {
      
      struct JERCorrections
      {
         float nominal;
         float up;
         float down;
      };
      
      class Jet : public Candidate {
         public:
            /// default constructor
            Jet();
            /// constructor from 4-momentum information
            Jet(const float & pt, const float & eta, const float & phi, const float & e);
            /// constructor from TLorentzVector
            Jet(const TLorentzVector & p4);
            /// destructor
           ~Jet();
           
            // Gets
           
            /// returns if jet is Puppi
            bool  isPuppi()                     const;
            /// returns the btag value of btag_csvivf
//            float btag()                        const;
            /// returns the btag value of algorithm
            float btag(const std::string & )    const;
            /// returns the flavour with the Hadron definition (=0 for data)
            int   flavour()                     const;
            /// returns the flavour given a definition (=0 for data)
            int   flavour(const std::string & ) const;
            /// returns if jet has id loose working point
            bool  idLoose()                     const;
            /// returns if jet has id tight working point
            bool  idTight()                     const;
            /// returns if jet has a given working point
            bool  id(const std::string & wp = "tight" )  const;
            /// returns the jet energy correction uncertainty
            float jecUncert()                   const;
            /// returns the vector containing flavours inside the jet
            std::vector<int> flavours()         const;
            /// returns the extended flavour definition
            std::string extendedFlavour()       const;
            /// returns the vector of pointers to the generated partons
            std::vector< std::shared_ptr<GenParticle> > partons() const;
            /// returns jet pt resolution
            float jerPtResolution() const;
            /// returns jet pt resolution from text file
            float jerPtResolution(const JetResolution &) const;
            /// returns jet energy resolution SF
            float jerSF() const;
            /// returns jet energy resolution SF
            float jerSF(const JetResolutionScaleFactor &) const;
            /// returns jet energy resolution SF Down variation
            float jerSFdown() const;
            /// returns jet energy resolution SF Down variation
            float jerSFdown(const JetResolutionScaleFactor &) const;
            /// returns jet energy resolution SF Up variation
            float jerSFup() const;
            /// returns jet energy resolution SF Up variation
            float jerSFup(const JetResolutionScaleFactor &) const;
            
            /// JER matching
            bool jerMatch(const std::string &);
            bool jerMatch(const float & drmin=0.2);
            bool jerMatch() const;
            
            void jerCorrections();
            float jerCorrection(const std::string & var = "nominal", const float & nsig = 1) const;
            
            float neutralHadronFraction()  const ;
            float neutralEmFraction()      const ;
            float neutralMultiplicity()    const ;
            float chargedHadronFraction()  const ;
            float chargedEmFraction()      const ;
            float chargedMultiplicity()    const ;
            float muonFraction()           const ;
            float constituents()           const ;
            
            /// quark-gluon separation
            float qgLikelihood()  const;
            
            /// pile up jet id
            float pileupJetIdFullDiscriminant() const;
            int pileupJetIdFullId() const;
            bool pileupJetIdFullId(const std::string & wp) const;
            
            /// b-jet regression
            float bRegCorr() const;
            float bRegRes()  const;
            
            /// Rho
            double rho() const;
            
            /// btag SF
            double btagSF    (std::shared_ptr<BTagCalibrationReader> reader, const std::string & flavalgo = "Hadron") const;
            double btagSFup  (std::shared_ptr<BTagCalibrationReader> reader, const float & nsig = 1, const std::string & flavalgo = "Hadron") const;
            double btagSFdown(std::shared_ptr<BTagCalibrationReader> reader, const float & nsig = 1, const std::string & flavalgo = "Hadron") const;
            double btagSFsys (std::shared_ptr<BTagCalibrationReader> reader, const std::string & systype  = "central", const std::string & flavalgo = "Hadron") const;
            
            /// pointer to the FSR jet
            Jet * fsrJet();
            
            /// pointer to the muon
            std::shared_ptr<Muon> muon() const;
            
            /// Pointer to GenJet
            std::shared_ptr<GenJet> generatedJet() const;
//            GenJet * generatedJet(const std::vector<GenJet*> &, const float &);
               
            // Sets
            /// sets the isPuppi value
            void  isPuppi(const bool &);
            /// sets the btag value
            void  btag(const float &);
            /// sets the btag value for difference algorithms
            void  btag(const std::string &, const float &);
            /// sets the default btag algo
            void  btagAlgo(const std::string & );
            /// sets flavour
            void  flavour(const int &);
            /// sets flavour for a given definition
            void  flavour(const std::string & definition, const int & value);
            /// sets if jet id is loose or not
            void  idLoose(const bool &);
            /// sets if jet id is tight or not
            void  idTight(const bool &);
            /// sets the jet energy correction uncertainty
            void  jecUncert(const float &);
            /// sets jet energy resolution
            void jerPtResolution(const float &);
            /// sets jet energy resolution SF
            void jerSF(const float &);
            /// sets jet energy resolution SF Up variation
            void jerSFup(const float &);
            /// sets jet energy resolution SF Down variation
            void jerSFdown(const float &);
            
            void jerInfo(const JetResolutionInfo &, const std::string &);
            void jerInfo(const JetResolutionInfo &, const float & drmin=0.2);
            void applyJER(const JetResolutionInfo &, const float & drmin=0.2);
            
            /// add parton that gave rise to jet
            void addParton(const std::shared_ptr<GenParticle> &);
            /// remove parton from jet parton list
            int removeParton(const int &);
            
            void neutralHadronFraction(const float & nHadFrac);
            void neutralEmFraction(const float & nEmFrac);
            void neutralMultiplicity(const float & nMult);
            void chargedHadronFraction(const float & cHadFrac);
            void chargedEmFraction(const float & cEmFrac);
            void chargedMultiplicity(const float & cMult);
            void muonFraction(const float & muFrac);
            
            /// calculates the jet id
//             void id(const float & nHadFrac,
//                     const float & nEmFrac ,
//                     const float & nMult   ,
//                     const float & cHadFrac,
//                     const float & cEmFrac ,
//                     const float & cMult   ,
//                     const float & muFrac  );
            
            void id(const float & nHadFrac,
                    const float & nEmFrac ,
                    const float & nMult   ,
                    const float & cHadFrac,
                    const float & cEmFrac ,
                    const float & cMult   ,
                    const float & muFrac  ,
                    const float & puppi   );
            
            /// quark-gluon separation
            void qgLikelihood(const float & discr);
            
            /// pile up jet id
            void pileupJetIdFullDiscriminant(const float & discr);
            void pileupJetIdFullId(const int & id);
            
            /// b-jet regression
            void bRegCorr(const float &);
            void bRegRes(const float &);
            
            void applyBjetRegression();
            
            /// Rho
            void rho(const double &);
            
            
            /// associate partons to the jet
            void associatePartons(const std::vector< std::shared_ptr<GenParticle> > &, const float & dRmax = 0.5, const float & ptMin = 1., const bool & pythi8 = true );
//            using Candidate::set; // in case needed to overload the function set
            
            /// gen jets
            void genJets(const std::vector< std::shared_ptr<GenJet> > &);
            
            /// add a final state radiation jet, will modify the 4-momentum
            void addFSR(Jet*);
            /// remove the final state radiation jet, will change back the original 4-momentum
            void rmFSR();
            
            /// associate a muon to the jet
            void addMuon(std::shared_ptr<Muon>);
            /// associate a muon to the jet from a collection of muons 
            void addMuon(std::vector< std::shared_ptr<Muon> > muons, const float & dr = 0.4);
            /// remove muon association to the jet
            void rmMuon();
            
            /// GenJet matching
            void generatedJet(std::shared_ptr<GenJet>);
            
            
         protected:
            // ----------member data ---------------------------
            //
               
            // Jet type
            bool isPuppi_;
            /// btag value 
            float btag_ ;
            /// btag value for each algo
            std::map<std::string, float> btags_ ;
            /// default btag algo
            std::string btagAlgo_;
            /// map of flavour to a given definition
            std::map<std::string, int> flavour_;
            /// flavours inside the jet
            std::vector<int> flavours_;
            /// extended flavour identification for merged jets
            std::string extendedFlavour_;
            /// vector of pointers to Genparticles from merged jets
            std::vector< std::shared_ptr<GenParticle> > partons_;
            /// jet id loose working point
            bool  idloose_;
            /// jet id tight working point
            bool  idtight_;
            /// jet energy correction uncertainty
            float jecUnc_;
            /// jet energy resolution SF
            float jersf_;
            /// jet energy resolution SF Up variation
            float jersfup_;
            /// jet energy resolution SF Down variation
            float jersfdown_;
            /// jet energy resolution
            float jerptres_;
            /// JER matching
            bool jermatch_;
            /// JER correction factor
            JERCorrections jercorr_;
            /// JER info
            JetResolutionInfo jerinfo_;
            
            /// jet id
            float nHadFrac_;
            float nEmFrac_;
            float nMult_;
            float cHadFrac_;
            float cEmFrac_;
            float cMult_;
            float muFrac_;
            float nConst_;
            /// quark-gluon separation
            float qgLikelihood_;
            /// pileup jet id
            float puJetIdFullDisc_;
            int   puJetIdFullId_;
            
            /// b-jet regression
            float bRegCorr_;
            float bRegRes_;
            
            /// Fixedgridrho (for JER)
            double rho_;
            
            /// final state radiation
            Jet * fsr_;
            /// 4-momentum before FSR correction
            TLorentzVector uncorrJetp4_;
            
            /// generated jet
            std::shared_ptr<GenJet> genjet_;
            
            /// collection of GenJets
            std::vector< std::shared_ptr<GenJet> > genjets_;
            
            
            /// muon in jet
            std::shared_ptr<Muon> muon_;
            
            
         private:
            // ----------member data ---------------------------
            
      };
   }
}

#endif  // Analysis_Tools_Jet_h
