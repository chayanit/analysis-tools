// system include files
#include <iostream>
//
// user include files
#include "Analysis/Tools/interface/PhysicsObjectTreeBase.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
template <class Object>
PhysicsObjectTreeBase<Object>::PhysicsObjectTreeBase() : TreeBase()
{
}
template <class Object>
PhysicsObjectTreeBase<Object>::PhysicsObjectTreeBase(TChain * tree, const std::string & name) : TreeBase(tree, name)
{
   tree_  -> SetBranchAddress( "n"  , &n_   );
   tree_  -> SetBranchAddress( "pt" ,  pt_  );
   tree_  -> SetBranchAddress( "eta",  eta_ );
   tree_  -> SetBranchAddress( "phi",  phi_ );
//   tree_  -> SetBranchAddress( "e"  ,  e_   );
   std::vector<std::string>::iterator it;
   it = std::find(branches_.begin(),branches_.end(),"e") ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , e_ );
   it = std::find(branches_.begin(),branches_.end(),"q") ;  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , q_ );
   it = std::find(branches_.begin(),branches_.end(),"px");  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , px_  );
   it = std::find(branches_.begin(),branches_.end(),"py");  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , py_  );
   it = std::find(branches_.begin(),branches_.end(),"pz");  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str() , pz_  );

}

template <class Object>
PhysicsObjectTreeBase<Object>::~PhysicsObjectTreeBase()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------

// ===================================
//  Specialized for vertex

PhysicsObjectTreeBase<Vertex>::PhysicsObjectTreeBase() : TreeBase()
{
}

PhysicsObjectTreeBase<Vertex>::PhysicsObjectTreeBase(TChain * tree, const std::string & name) : TreeBase(tree, name)
{
   tree_  -> SetBranchAddress( "n"   , &n_    );
   tree_  -> SetBranchAddress( "x"   ,  x_    );
   tree_  -> SetBranchAddress( "y"   ,  y_    );
   tree_  -> SetBranchAddress( "z"   ,  z_    );
   tree_  -> SetBranchAddress( "xe"  ,  xe_   );
   tree_  -> SetBranchAddress( "ye"  ,  ye_   );
   tree_  -> SetBranchAddress( "ze"  ,  ze_   );
   tree_  -> SetBranchAddress( "fake",  fake_ );
   tree_  -> SetBranchAddress( "chi2",  chi2_ );
   tree_  -> SetBranchAddress( "ndof",  ndof_ );
   tree_  -> SetBranchAddress( "rho" ,  rho_  );
}

PhysicsObjectTreeBase<Vertex>::~PhysicsObjectTreeBase()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


template class PhysicsObjectTreeBase<Candidate>;
template class PhysicsObjectTreeBase<Jet>;
template class PhysicsObjectTreeBase<MET>;
template class PhysicsObjectTreeBase<Muon>;
template class PhysicsObjectTreeBase<Vertex>;
template class PhysicsObjectTreeBase<TriggerObject>;
template class PhysicsObjectTreeBase<GenParticle>;
template class PhysicsObjectTreeBase<GenJet>;
template class PhysicsObjectTreeBase<JetTag>;
template class PhysicsObjectTreeBase<L1TMuon>;
template class PhysicsObjectTreeBase<L1TJet>;
template class PhysicsObjectTreeBase<RecoMuon>;
template class PhysicsObjectTreeBase<RecoTrack>;
