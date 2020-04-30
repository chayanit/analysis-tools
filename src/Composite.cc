// system include files
//
// user include files
#include "Analysis/Tools/interface/Composite.h"
#include "Analysis/Tools/interface/Jet.h"
#include "Analysis/Tools/interface/Muon.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
template <class A, class B>
Composite<A,B>::Composite() : Candidate()
{
}
template <class A, class B>
Composite<A,B>::Composite(const A & a, const B & b)
{
   a_  = &a;
   b_  = &b;
   p4_ = a_->p4()+ b_->p4();
   q_  = a_->q() + b_->q();
   
}
template <class A, class B>
Composite<A,B>::~Composite()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// Gets 
template <class A, class B>
float Composite<A,B>::deltaR() const
{
   return a_->deltaR(*b_);
}
      
template <class A, class B>
float Composite<A,B>::deltaEta() const
{
   return fabs(a_->eta()-b_->eta());
}


// ------------ methods  ------------


template class Composite<Candidate,Candidate>;
template class Composite<Jet,Jet>;
template class Composite<Jet,Candidate>;
template class Composite<Muon,Muon>;

