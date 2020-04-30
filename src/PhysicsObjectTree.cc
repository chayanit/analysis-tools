// system include files
#include <iostream>
//
// user include files
#include "Analysis/Tools/interface/PhysicsObjectTree.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
template <class Object>
      PhysicsObjectTree<Object>::PhysicsObjectTree() : PhysicsObjectTreeBase<Object>()     {}
template <class Object>
      PhysicsObjectTree<Object>::PhysicsObjectTree(TChain * tree, const std::string & name) : PhysicsObjectTreeBase<Object>(tree, name) {}
template <class Object>
      PhysicsObjectTree<Object>::~PhysicsObjectTree()                                                   {}

// member function
template <class Object> Collection<Object>  PhysicsObjectTree<Object>::collection()
{
   // DUMMY
   Collection<Object> obj;
   return obj;
}

// Candidate
// Constructors and destructor
PhysicsObjectTree<Candidate>::PhysicsObjectTree() : PhysicsObjectTreeBase<Candidate>()
{
}
PhysicsObjectTree<Candidate>::PhysicsObjectTree(TChain * tree, const std::string & name) : PhysicsObjectTreeBase<Candidate>(tree, name)
{
}
PhysicsObjectTree<Candidate>::~PhysicsObjectTree() {}

// Member functions
Collection<Candidate>  PhysicsObjectTree<Candidate>::collection()
{
   std::vector<Candidate> candidates;
   for ( int i = 0 ; i < n_ ; ++i )
   {
      Candidate cand(pt_[i], eta_[i], phi_[i], e_[i], q_[i]);
      candidates.push_back(cand);
   }
   Collection<Candidate> CandidateCollection(candidates, name_);
   return CandidateCollection;

}


// JETS
// Constructors and destructor
PhysicsObjectTree<Jet>::PhysicsObjectTree(TChain * tree, const std::string & name) : PhysicsObjectTreeBase<Jet>(tree, name)
{
   isSimpleJet_ = false;
   hasPuppiInfo_ = false;
   hasQGLikelihood_ = false;
   hasBRegCorr_ = false;
   hasBRegRes_ = false;
   hasRho_ = false;
   
//   tree_  -> SetBranchAddress( "btag_csvivf", btag_    );
   int algos = 0;
   for ( auto & branch : branches_ )
   {
      std::size_t found = branch.find("btag_");
      if (found!=std::string::npos)
      {
         mbtag_[branch] = btags_[algos];
         ++algos;
         tree_  -> SetBranchAddress( branch.c_str(), mbtag_[branch]);
      }
   }
//    std::vector<std::string>::iterator it;
//    it = std::find(branches_.begin(),branches_.end(),"btag_");
//    if ( it != branches_.end() )
//       std::cout << *it << std::endl;
   
   if ( mbtag_.size() > 0 )
   {
      tree_  -> SetBranchAddress( "flavour"        , flavour_ );
      tree_  -> SetBranchAddress( "hadronFlavour"  , hadrflavour_ );
      tree_  -> SetBranchAddress( "partonFlavour"  , partflavour_ );
      tree_  -> SetBranchAddress( "physicsFlavour" , physflavour_ );
      tree_  -> SetBranchAddress( "id_nHadFrac", nHadFrac_);
      tree_  -> SetBranchAddress( "id_nEmFrac" , nEmFrac_ );
      tree_  -> SetBranchAddress( "id_nMult"   , nMult_   );
      tree_  -> SetBranchAddress( "id_cHadFrac", cHadFrac_);
      tree_  -> SetBranchAddress( "id_cEmFrac" , cEmFrac_ );
      tree_  -> SetBranchAddress( "id_cMult"   , cMult_   );
      tree_  -> SetBranchAddress( "id_muonFrac", muFrac_  );
      tree_  -> SetBranchAddress( "jecUncert"  , jecUnc_);
      tree_  -> SetBranchAddress( "jerSF", jerSF_);
      tree_  -> SetBranchAddress( "jerSFDown", jerSFDown_);
      tree_  -> SetBranchAddress( "jerSFUp", jerSFUp_);
      tree_  -> SetBranchAddress( "jerResolution", jerResolution_);
      std::vector<std::string>::iterator it;
      it = std::find(branches_.begin(),branches_.end(),"qgLikelihood")            ;  if ( it != branches_.end() ) { tree_  -> SetBranchAddress( (*it).c_str() , qgLikelihood_ ); hasQGLikelihood_ = true; }
      it = std::find(branches_.begin(),branches_.end(),"puJetIdFullDiscriminant") ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , puJetIdFullDisc_ );
      it = std::find(branches_.begin(),branches_.end(),"puJetIdFullId")           ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , puJetIdFullId_ );
      it = std::find(branches_.begin(),branches_.end(),"id_puppi")                ;  if ( it != branches_.end() ) { tree_  -> SetBranchAddress( (*it).c_str() , puppi_ )       ; hasPuppiInfo_ = true; }
      it = std::find(branches_.begin(),branches_.end(),"bjetRegCorr")             ;  if ( it != branches_.end() ) { tree_  -> SetBranchAddress( (*it).c_str() , bRegCorr_ )    ; hasBRegCorr_  = true; }
      it = std::find(branches_.begin(),branches_.end(),"bjetRegRes")              ;  if ( it != branches_.end() ) { tree_  -> SetBranchAddress( (*it).c_str() , bRegRes_ )     ; hasBRegRes_   = true; }
      it = std::find(branches_.begin(),branches_.end(),"Rho")                     ;  if ( it != branches_.end() ) { tree_  -> SetBranchAddress( (*it).c_str() , &rho_ )        ; hasRho_       = true; }
   }
   else
   {
      isSimpleJet_ = true;
   }
//   std::cout << "oioi" << std::endl;
//   std::vector<std::string>::iterator it;
//   it = std::find(branches_.begin(),branches_.end(),"jecUncert");  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str(), jecUnc_);

}
PhysicsObjectTree<Jet>::~PhysicsObjectTree() {}

// Member functions
Collection<Jet>  PhysicsObjectTree<Jet>::collection()
{
//    std::string treename = (std::string)tree_->GetName();
//    std::size_t foundPuppi = treename.find("Puppi");
//    bool ispuppi = (foundPuppi != std::string::npos);
   
   std::vector<Jet> jets;
   for ( int i = 0 ; i < n_ ; ++i )
   {
      Jet jet(pt_[i], eta_[i], phi_[i], e_[i]);
      jet.btag(btag_[i]);
      for ( auto & b : mbtag_ )
         jet.btag(b.first,b.second[i]);
      jet.flavour(flavour_[i]);
      jet.flavour("Hadron",hadrflavour_[i]);
      jet.flavour("Parton",partflavour_[i]);
      jet.flavour("Physics",physflavour_[i]);
      jet.jecUncert(jecUnc_[i]);
      // for jet ID depends on Puppi!!!
      if ( hasPuppiInfo_ )
      {
         jet.id(nHadFrac_[i],
                nEmFrac_[i] ,
                nMult_[i]   ,
                cHadFrac_[i],
                cEmFrac_[i] ,
                cMult_[i]   ,
                muFrac_[i]  ,
                puppi_[i]   );
      }
      else
      {
         jet.id(nHadFrac_[i],
                nEmFrac_[i] ,
                nMult_[i]   ,
                cHadFrac_[i],
                cEmFrac_[i] ,
                cMult_[i]   ,
                muFrac_[i]  ,
                -1.        );
      }
      jet.jerPtResolution(jerResolution_[i]);
      jet.jerSF(jerSF_[i]);
      jet.jerSFup(jerSFUp_[i]);
      jet.jerSFdown(jerSFDown_[i]);
      if ( hasQGLikelihood_ ) jet.qgLikelihood(qgLikelihood_[i]);
      else                    jet.qgLikelihood(-10.);
      jet.pileupJetIdFullDiscriminant(puJetIdFullDisc_[i]);
      jet.pileupJetIdFullId(puJetIdFullId_[i]);
      if ( hasBRegCorr_ )  jet.bRegCorr(bRegCorr_[i]);
      else                 jet.bRegCorr(1);
      if ( hasBRegRes_  )  jet.bRegRes (bRegRes_[i]);
      else                 jet.bRegRes(1);
      if ( hasRho_ )       jet.rho(rho_);
      else                 jet.rho(-1);
      
      jets.push_back(jet);
   }
   Collection<Jet> jetCollection(jets, name_);
   return jetCollection;

}

// GENPARTICLE
// Constructors and destructor
PhysicsObjectTree<GenParticle>::PhysicsObjectTree(TChain * tree, const std::string & name) : PhysicsObjectTreeBase<GenParticle>(tree, name)
{
   gp_has_indx_ = false;
   
   tree_  -> SetBranchAddress( "pdg"   , pdgid_  );
   tree_  -> SetBranchAddress( "status", status_ );
   tree_  -> SetBranchAddress( "higgs_dau", higgs_dau_ );
   
  std::vector<std::string>::iterator it;
  it = std::find(branches_.begin(),branches_.end(),"index")            ;  if ( it != branches_.end() ) { tree_  -> SetBranchAddress( (*it).c_str() , indx_ ); gp_has_indx_ = true; }
  if ( gp_has_indx_ )
  {
     it = std::find(branches_.begin(),branches_.end(),"mother1")         ;  if ( it != branches_.end() ) { tree_  -> SetBranchAddress( (*it).c_str() , mo1_ ); }
     it = std::find(branches_.begin(),branches_.end(),"mother2")         ;  if ( it != branches_.end() ) { tree_  -> SetBranchAddress( (*it).c_str() , mo2_ ); }
     it = std::find(branches_.begin(),branches_.end(),"daughter1")       ;  if ( it != branches_.end() ) { tree_  -> SetBranchAddress( (*it).c_str() , da1_ ); }
     it = std::find(branches_.begin(),branches_.end(),"daughter2")       ;  if ( it != branches_.end() ) { tree_  -> SetBranchAddress( (*it).c_str() , da2_ ); }
  }
   
}
PhysicsObjectTree<GenParticle>::~PhysicsObjectTree() {}

// Member functions
Collection<GenParticle>  PhysicsObjectTree<GenParticle>::collection()
{
   std::vector<GenParticle> particles;
   for ( int i = 0 ; i < n_ ; ++i )
   {
      GenParticle p(pt_[i], eta_[i], phi_[i], e_[i], q_[i]);
      p.pdgId(pdgid_[i]);
      p.status(status_[i]);
      p.higgsDaughter(higgs_dau_[i]);
      if ( gp_has_indx_ )
      {
         p.index(indx_[i]);
         p.mother(1,mo1_[i]);
         p.mother(2,mo2_[i]);
         p.daughter(1,da1_[i]);
         p.daughter(2,da2_[i]);
      }
      particles.push_back(p);
   }
   Collection<GenParticle> genPartCollection(particles, name_);
   return genPartCollection;

}

// MET
// Constructors and destructor
PhysicsObjectTree<MET>::PhysicsObjectTree(TChain * tree, const std::string & name) : PhysicsObjectTreeBase<MET>(tree, name)
{
   tree_  -> SetBranchAddress( "sigxx" , sigxx_ );
   tree_  -> SetBranchAddress( "sigxy" , sigxy_ );
   tree_  -> SetBranchAddress( "sigyx" , sigyx_ );
   tree_  -> SetBranchAddress( "sigyy" , sigyy_ );
   // Exists in MC; check if available and set
   // Maybe it's better to use analysis::isMC() for this perpouse?
   std::vector<std::string>::iterator it;
   it = std::find(branches_.begin(),branches_.end(),"gen_px");  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str(), gen_px_);
   it = std::find(branches_.begin(),branches_.end(),"gen_py");  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str(), gen_py_);
   it = std::find(branches_.begin(),branches_.end(),"gen_pz");  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str(), gen_pz_);
}
PhysicsObjectTree<MET>::~PhysicsObjectTree() {}

// Member function
Collection<MET>  PhysicsObjectTree<MET>::collection()
{
   std::vector<MET> mets;
   for ( int i = 0 ; i < n_ ; ++i )
   {
      MET met(px_[i], py_[i], pz_[i]);
      met.significanceMatrix(sigxx_[i],sigxy_[i],sigyx_[i],sigyy_[i]);
      met.genP(gen_px_[i],gen_py_[i],gen_pz_[i]);
      mets.push_back(met);
   }
   Collection<MET> metCollection(mets, name_);
   return metCollection;

}

// MUON
// Constructors and destructor
PhysicsObjectTree<Muon>::PhysicsObjectTree() : PhysicsObjectTreeBase<Muon>()
{
}
PhysicsObjectTree<Muon>::PhysicsObjectTree(TChain * tree, const std::string & name) : PhysicsObjectTreeBase<Muon>(tree, name)
{
   hasTightMuon_ = false;
   tree_  -> SetBranchAddress ("isPFMuon"      , isPFMuon_     ) ;      
   tree_  -> SetBranchAddress ("isGlobalMuon"  , isGlobalMuon_ ) ;  
   tree_  -> SetBranchAddress ("isTrackerMuon" , isTrackerMuon_) ; 
   tree_  -> SetBranchAddress ("isLooseMuon"   , isLooseMuon_  ) ;   
   tree_  -> SetBranchAddress ("isMediumMuon"  , isMediumMuon_ ) ;  
//   tree_  -> SetBranchAddress ("isTightMuon"   , isTightMuon_ ) ;  

   tree_  -> SetBranchAddress ("validFraction" ,validFraction_ ) ;
   tree_  -> SetBranchAddress ("segmentCompatibility" , segmentCompatibility_) ;
   tree_  -> SetBranchAddress ("trkKink" , trkKink_) ;      
   tree_  -> SetBranchAddress ("chi2LocalPos" ,chi2LocalPos_) ;

   tree_  -> SetBranchAddress ("normChi2" , normChi2_) ;
  
   // to preserve backward compatibility with old ntuples
   std::vector<std::string>::iterator it;
   it = std::find(branches_.begin(),branches_.end(),"isTightMuon") ; if ( it != branches_.end() ) { tree_  -> SetBranchAddress( (*it).c_str() , isTightMuon_ ); hasTightMuon_ = true; }
  
   if ( ! hasTightMuon_ ) std::cout << "*** warning *** PhysicsObjectTree<Muon> constructor: muon id tight is not available in this ntuple production, isIdTight -> false" << std::endl;
  
}
PhysicsObjectTree<Muon>::~PhysicsObjectTree()
{
}
// Member functions
Collection<Muon>  PhysicsObjectTree<Muon>::collection()
{
   std::vector<Muon> muons;
   for ( int i = 0 ; i < n_ ; ++i )
   {
      Muon muon(pt_[i], eta_[i], phi_[i], e_[i], q_[i]);
      muon.isPFMuon(isPFMuon_[i]) ;      
      muon.isGlobalMuon(isGlobalMuon_[i]) ;  
      muon.isTrackerMuon(isTrackerMuon_[i]) ; 
      muon.isLooseMuon(isLooseMuon_[i]) ;   
      muon.isMediumMuon(isMediumMuon_[i]) ;
      if ( hasTightMuon_ ) muon.isTightMuon(isTightMuon_[i]) ;
      else                 muon.isTightMuon(false) ;

      muon.validFraction(validFraction_[i]) ;
      muon.segmentCompatibility(segmentCompatibility_[i]) ;
      muon.trkKink(trkKink_[i]) ;      
      muon.chi2LocalPos(chi2LocalPos_[i]) ; 
      
      muon.normChi2(normChi2_[i]) ;
      muons.push_back(muon);
   }
   Collection<Muon> muonCollection(muons, name_);
   return muonCollection;
}

// JETTAG
// Constructors and destructor
PhysicsObjectTree<JetTag>::PhysicsObjectTree() : PhysicsObjectTreeBase<JetTag>()
{
}
PhysicsObjectTree<JetTag>::PhysicsObjectTree(TChain * tree, const std::string & name) : PhysicsObjectTreeBase<JetTag>(tree, name)
{
   tree_  -> SetBranchAddress( "btag", btag_    );
}
PhysicsObjectTree<JetTag>::~PhysicsObjectTree()
{
}
// Member functions
Collection<JetTag>  PhysicsObjectTree<JetTag>::collection()
{
   std::vector<JetTag> jetstags;
   for ( int i = 0 ; i < n_ ; ++i )
   {
      JetTag jettag(pt_[i], eta_[i], phi_[i], e_[i]);
      jettag.btag(btag_[i]);
      jetstags.push_back(jettag);
   }
   Collection<JetTag> jettagCollection(jetstags, name_);
   return jettagCollection;
}


// GenJet
// Constructors and destructor
PhysicsObjectTree<GenJet>::PhysicsObjectTree() : PhysicsObjectTreeBase<GenJet>()
{
}
PhysicsObjectTree<GenJet>::PhysicsObjectTree(TChain * tree, const std::string & name) : PhysicsObjectTreeBase<GenJet>(tree, name)
{
}
PhysicsObjectTree<GenJet>::~PhysicsObjectTree()
{
}
// Member functions
Collection<GenJet>  PhysicsObjectTree<GenJet>::collection()
{
   std::vector<GenJet> genjets;
   for ( int i = 0 ; i < n_ ; ++i )
   {
      GenJet genjet(pt_[i], eta_[i], phi_[i], e_[i], q_[i]);
      genjets.push_back(genjet);
   }
   Collection<GenJet> genjetCollection(genjets, name_);
   return genjetCollection;
}



// VERTEX
// Constructors and destructor
PhysicsObjectTree<Vertex>::PhysicsObjectTree()              : PhysicsObjectTreeBase<Vertex>()     {}
PhysicsObjectTree<Vertex>::PhysicsObjectTree(TChain * tree, const std::string & name) : PhysicsObjectTreeBase<Vertex>(tree, name) {}
PhysicsObjectTree<Vertex>::~PhysicsObjectTree()                                                   {}

// Member functions
Collection<Vertex>  PhysicsObjectTree<Vertex>::collection()
{
   std::vector<Vertex> vertices;
   for ( int i = 0 ; i < n_ ; ++i )
   {
      Vertex vertex(x_[i], y_[i], z_[i]);
      vertex.chi2(chi2_[i]);
      vertex.ndof(ndof_[i]);
      vertex.rho(rho_[i]);
      vertex.fake(fake_[i]);

      vertices.push_back(vertex);
   }
   Collection<Vertex> vertexCollection(vertices, name_ );
   return vertexCollection;
}

// TriggerObjects
// Constructors and destructor
PhysicsObjectTree<TriggerObject>::PhysicsObjectTree() : PhysicsObjectTreeBase<TriggerObject>()
{
}
PhysicsObjectTree<TriggerObject>::PhysicsObjectTree(TChain * tree, const std::string & name) : PhysicsObjectTreeBase<TriggerObject>(tree, name)
{
  tree_  -> SetBranchAddress ("type"      , type_     ) ;
}
PhysicsObjectTree<TriggerObject>::~PhysicsObjectTree() {}

// Member functions
Collection<TriggerObject>  PhysicsObjectTree<TriggerObject>::collection()
{
   std::vector<TriggerObject> triggers;
   for ( int i = 0 ; i < n_ ; ++i )
   {
      TriggerObject trig(pt_[i], eta_[i], phi_[i], e_[i]);
      trig.type(type_[i]);
      triggers.push_back(trig);
   }
   Collection<TriggerObject> TriggerObjectCollection(triggers, name_);
   return TriggerObjectCollection;

}

// L1TMuon
// Constructors and destructor
PhysicsObjectTree<L1TMuon>::PhysicsObjectTree() : PhysicsObjectTreeBase<L1TMuon>()
{
}
PhysicsObjectTree<L1TMuon>::PhysicsObjectTree(TChain * tree, const std::string & name) : PhysicsObjectTreeBase<L1TMuon>(tree, name)
{
  tree_  -> SetBranchAddress ("hwQual"   , hwQual_   ) ;      
  tree_  -> SetBranchAddress ("etaAtVtx" , etaAtVtx_ ) ;  
  tree_  -> SetBranchAddress ("phiAtVtx" , phiAtVtx_ ) ; 
}
PhysicsObjectTree<L1TMuon>::~PhysicsObjectTree()
{
}
// Member functions
Collection<L1TMuon>  PhysicsObjectTree<L1TMuon>::collection()
{
   std::vector<L1TMuon> muons;
   for ( int i = 0 ; i < n_ ; ++i )
   {
      L1TMuon muon(pt_[i], eta_[i], phi_[i], e_[i], q_[i]);
      muon.hwQual  (hwQual_  [i]) ;      
      muon.etaAtVtx(etaAtVtx_[i]) ;  
      muon.phiAtVtx(phiAtVtx_[i]) ; 
      muons.push_back(muon);
   }
   Collection<L1TMuon> muonCollection(muons, name_);
   return muonCollection;
}

// L1TJet
// Constructors and destructor
PhysicsObjectTree<L1TJet>::PhysicsObjectTree() : PhysicsObjectTreeBase<L1TJet>()
{
}
PhysicsObjectTree<L1TJet>::PhysicsObjectTree(TChain * tree, const std::string & name) : PhysicsObjectTreeBase<L1TJet>(tree, name)
{
}
PhysicsObjectTree<L1TJet>::~PhysicsObjectTree()
{
}
// Member functions
Collection<L1TJet>  PhysicsObjectTree<L1TJet>::collection()
{
   std::vector<L1TJet> jets;
   for ( int i = 0 ; i < n_ ; ++i )
   {
      L1TJet jet(pt_[i], eta_[i], phi_[i], e_[i], 0);
      jets.push_back(jet);
   }
   Collection<L1TJet> jetCollection(jets, name_);
   return jetCollection;
}

// RecoMuon
// Constructors and destructor
PhysicsObjectTree<RecoMuon>::PhysicsObjectTree() : PhysicsObjectTreeBase<RecoMuon>()
{
}
PhysicsObjectTree<RecoMuon>::PhysicsObjectTree(TChain * tree, const std::string & name) : PhysicsObjectTreeBase<RecoMuon>(tree, name)
{
}
PhysicsObjectTree<RecoMuon>::~PhysicsObjectTree() {}

// Member functions
Collection<RecoMuon>  PhysicsObjectTree<RecoMuon>::collection()
{
   std::vector<RecoMuon> candidates;
   for ( int i = 0 ; i < n_ ; ++i )
   {
      RecoMuon cand(pt_[i], eta_[i], phi_[i], e_[i], q_[i]);
      candidates.push_back(cand);
   }
   Collection<RecoMuon> RecoMuonCollection(candidates, name_);
   return RecoMuonCollection;

}

// RecoTrack
// Constructors and destructor
PhysicsObjectTree<RecoTrack>::PhysicsObjectTree() : PhysicsObjectTreeBase<RecoTrack>()
{
}
PhysicsObjectTree<RecoTrack>::PhysicsObjectTree(TChain * tree, const std::string & name) : PhysicsObjectTreeBase<RecoTrack>(tree, name)
{
   std::vector<std::string>::iterator it;
   it = std::find(branches_.begin(),branches_.end(),"chi2"                             ) ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , chi2_                    );
   it = std::find(branches_.begin(),branches_.end(),"ndof"                             ) ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , ndof_                    );
   it = std::find(branches_.begin(),branches_.end(),"d0"                               ) ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , d0_                      );
   it = std::find(branches_.begin(),branches_.end(),"dxy"                              ) ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , dxy_                     );
   it = std::find(branches_.begin(),branches_.end(),"numberOfLostMuonHits"             ) ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , nLostMuHits_             );
   it = std::find(branches_.begin(),branches_.end(),"numberOfBadMuonHits"              ) ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , nBadMuHits_              );
   it = std::find(branches_.begin(),branches_.end(),"numberOfValidMuonHits"            ) ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , nValMuHits_              );
   it = std::find(branches_.begin(),branches_.end(),"numberOfValidTrackerHits"         ) ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , nValTrackerHits_         );
   it = std::find(branches_.begin(),branches_.end(),"numberOfValidStripTECHits"        ) ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , nValStripTECHits_        );
   it = std::find(branches_.begin(),branches_.end(),"numberOfValidStripTIBHits"        ) ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , nValStripTIBHits_        );
   it = std::find(branches_.begin(),branches_.end(),"numberOfValidStripTIDHits"        ) ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , nValStripTIDHits_        );
   it = std::find(branches_.begin(),branches_.end(),"numberOfValidStripTOBHits"        ) ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , nValStripTOBHits_        );
   it = std::find(branches_.begin(),branches_.end(),"muonStationsWithValidHits"        ) ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , muStationsWithValHits_   );
   it = std::find(branches_.begin(),branches_.end(),"muonStationsWithBadHits"          ) ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , muStationsWithBadHits_   );
   it = std::find(branches_.begin(),branches_.end(),"innermostMuonStationWithValidHits") ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , inMuStationWithValHits_  );
   it = std::find(branches_.begin(),branches_.end(),"outermostMuonStationWithValidHits") ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , outMuStationWithValHits_ );
   
   mqual_[undefQuality       ]  = qual_[0];
   mqual_[loose              ]  = qual_[1];
   mqual_[tight              ]  = qual_[2];
   mqual_[highPurity         ]  = qual_[3];
   mqual_[confirmed          ]  = qual_[4];
   mqual_[goodIterative      ]  = qual_[5];
   mqual_[looseSetWithPV     ]  = qual_[6];
   mqual_[highPuritySetWithPV]  = qual_[7];
   mqual_[discarded          ]  = qual_[8];
   mqual_[qualitySize        ]  = qual_[9];
   
   it = std::find(branches_.begin(),branches_.end(),"quality_undefQuality"       ) ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , mqual_[TrackQuality::undefQuality       ] );
   it = std::find(branches_.begin(),branches_.end(),"quality_loose"              ) ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , mqual_[TrackQuality::loose              ] );
   it = std::find(branches_.begin(),branches_.end(),"quality_tight"              ) ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , mqual_[TrackQuality::tight              ] );
   it = std::find(branches_.begin(),branches_.end(),"quality_highPurity"         ) ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , mqual_[TrackQuality::highPurity         ] );
   it = std::find(branches_.begin(),branches_.end(),"quality_confirmed"          ) ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , mqual_[TrackQuality::confirmed          ] );
   it = std::find(branches_.begin(),branches_.end(),"quality_goodIterative"      ) ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , mqual_[TrackQuality::goodIterative      ] );
   it = std::find(branches_.begin(),branches_.end(),"quality_looseSetWithPV"     ) ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , mqual_[TrackQuality::looseSetWithPV     ] );
   it = std::find(branches_.begin(),branches_.end(),"quality_highPuritySetWithPV") ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , mqual_[TrackQuality::highPuritySetWithPV] );
   it = std::find(branches_.begin(),branches_.end(),"quality_discarded"          ) ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , mqual_[TrackQuality::discarded          ] );
   it = std::find(branches_.begin(),branches_.end(),"quality_qualitySize"        ) ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , mqual_[TrackQuality::qualitySize        ] );
   


}
PhysicsObjectTree<RecoTrack>::~PhysicsObjectTree() {}

// Member functions
Collection<RecoTrack>  PhysicsObjectTree<RecoTrack>::collection()
{
//   std::cout << " nnnnn = " << n_ << "   " << chi2_ << std::endl;

   std::vector<RecoTrack> trks;
   for ( int i = 0 ; i < n_ ; ++i )
   {
      RecoTrack trk(px_[i], py_[i], px_[i], q_[i]);
      trk.chi2(chi2_ [i]);
      trk.ndof(ndof_ [i]);
      trk.d0  (d0_   [i]);
      trk.dxy (dxy_  [i]);
      trk.numberOfLostMuonHits             (nLostMuHits_            [i]) ;
      trk.numberOfBadMuonHits              (nBadMuHits_             [i]) ;
      trk.numberOfValidMuonHits            (nValMuHits_             [i]) ;
      trk.numberOfValidTrackerHits         (nValTrackerHits_        [i]) ;
      trk.numberOfValidStripTECHits        (nValStripTECHits_       [i]) ;
      trk.numberOfValidStripTIBHits        (nValStripTIBHits_       [i]) ;
      trk.numberOfValidStripTIDHits        (nValStripTIDHits_       [i]) ;
      trk.numberOfValidStripTOBHits        (nValStripTOBHits_       [i]) ;
      trk.muonStationsWithValidHits        (muStationsWithValHits_  [i]) ;
      trk.muonStationsWithBadHits          (muStationsWithBadHits_  [i]) ;
      trk.innermostMuonStationWithValidHits(inMuStationWithValHits_ [i]) ;
      trk.outermostMuonStationWithValidHits(outMuStationWithValHits_[i]) ;
      trk.quality(undefQuality       ,mqual_[TrackQuality::undefQuality       ][i]);
      trk.quality(loose              ,mqual_[TrackQuality::loose              ][i]);
      trk.quality(tight              ,mqual_[TrackQuality::tight              ][i]);
      trk.quality(highPurity         ,mqual_[TrackQuality::highPurity         ][i]);
      trk.quality(confirmed          ,mqual_[TrackQuality::confirmed          ][i]);
      trk.quality(goodIterative      ,mqual_[TrackQuality::goodIterative      ][i]);
      trk.quality(looseSetWithPV     ,mqual_[TrackQuality::looseSetWithPV     ][i]);
      trk.quality(highPuritySetWithPV,mqual_[TrackQuality::highPuritySetWithPV][i]);
      trk.quality(discarded          ,mqual_[TrackQuality::discarded          ][i]);
      trk.quality(qualitySize        ,mqual_[TrackQuality::qualitySize        ][i]);
      
      trks.push_back(trk);
   }
   Collection<RecoTrack> RecoTrackCollection(trks, name_);
   return RecoTrackCollection;

}


// ======================================
// Templates declarations
template class PhysicsObjectTree<Candidate>;
template class PhysicsObjectTree<Jet>;
template class PhysicsObjectTree<MET>;
template class PhysicsObjectTree<Muon>;
template class PhysicsObjectTree<Vertex>;
template class PhysicsObjectTree<TriggerObject>;
template class PhysicsObjectTree<GenParticle>;
template class PhysicsObjectTree<GenJet>;
template class PhysicsObjectTree<L1TMuon>;
template class PhysicsObjectTree<L1TJet>;
template class PhysicsObjectTree<RecoMuon>;
template class PhysicsObjectTree<RecoTrack>;
