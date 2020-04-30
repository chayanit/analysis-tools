// system include files
// user include files
#include "Analysis/Tools/interface/PileupWeight.h"
#include "TFile.h"

//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
PileupWeight::PileupWeight()
{
}

// Main constructor
PileupWeight::PileupWeight(const std::string & fname )
{
   TH1D * tmp;
   std::shared_ptr<TFile> f = std::make_shared<TFile>(fname.c_str(),"old");
   tmp = (TH1D*) f->Get("weight_2down");
   if ( tmp )  histos_[-2] = std::make_shared<TH1D>(*tmp);
   
   tmp = (TH1D*) f->Get("weight_1down");
   if ( tmp )  histos_[-1] = std::make_shared<TH1D>(*tmp);
   
   tmp = (TH1D*) f->Get("weight");
   if ( tmp )  histos_[0] = std::make_shared<TH1D>(*tmp);
   
   tmp = (TH1D*) f->Get("weight_1up");
   if ( tmp )  histos_[1] = std::make_shared<TH1D>(*tmp);
   
   tmp = (TH1D*) f->Get("weight_2up");
   if ( tmp )  histos_[2] = std::make_shared<TH1D>(*tmp);

   if ( ! histos_[-2] ) 
      std::cout << "WARNING - PileupWeight::PileupWeight | Histogram weight_2down not found. Weight = 1" << std::endl;
   if ( ! histos_[-1] ) 
      std::cout << "WARNING - PileupWeight::PileupWeight | Histogram weight_1down not found. Weight = 1" << std::endl;
   if ( ! histos_[0] ) 
      std::cout << "WARNING - PileupWeight::PileupWeight | Histogram weight not found. Weight = 1" << std::endl;
   if ( ! histos_[1] ) 
      std::cout << "WARNING - PileupWeight::PileupWeight | Histogram weight_1up not found. Weight = 1" << std::endl;
   if ( ! histos_[2] ) 
      std::cout << "WARNING - PileupWeight::PileupWeight | Histogram weight_2up not found. Weight = 1" << std::endl;
}

PileupWeight::~PileupWeight()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//
// ------------ method called for each event  ------------

// output tree
float PileupWeight::weight(const float & truepu, const int & var)
{
   float weight = 1.;
   if ( ! histos_[0] ) return weight;
   if ( ! histos_[var] ) return weight;
   int bin = histos_[var] -> FindBin(truepu);
   weight = histos_[var] -> GetBinContent(bin);
   return weight;
}

