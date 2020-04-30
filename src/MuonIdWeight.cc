// system include files
// user include files
#include "Analysis/Tools/interface/MuonIdWeight.h"
#include "TFile.h"

//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
MuonIdWeight::MuonIdWeight()
{
}

// Main constructor
MuonIdWeight::MuonIdWeight(const std::string & fname )
{
   TH2D * tmp;
   std::shared_ptr<TFile> f = std::make_shared<TFile>(fname.c_str(),"old");
   tmp = (TH2D*) f->Get("weight_2down");
   if ( tmp )  histos_[-2] = std::make_shared<TH2D>(*tmp);
   
   tmp = (TH2D*) f->Get("weight_1down");
   if ( tmp )  histos_[-1] = std::make_shared<TH2D>(*tmp);
   
   tmp = (TH2D*) f->Get("weight");
   if ( tmp )  histos_[0] = std::make_shared<TH2D>(*tmp);
   
   tmp = (TH2D*) f->Get("weight_1up");
   if ( tmp )  histos_[1] = std::make_shared<TH2D>(*tmp);
   
   tmp = (TH2D*) f->Get("weight_2up");
   if ( tmp )  histos_[2] = std::make_shared<TH2D>(*tmp);

   if ( ! histos_[-2] ) 
      std::cout << "WARNING - MuonIdWeight::MuonIdWeight | Histogram weight_2down not found. Weight = 1" << std::endl;
   if ( ! histos_[-1] ) 
      std::cout << "WARNING - MuonIdWeight::MuonIdWeight | Histogram weight_1down not found. Weight = 1" << std::endl;
   if ( ! histos_[0] ) 
      std::cout << "WARNING - MuonIdWeight::MuonIdWeight | Histogram weight not found. Weight = 1" << std::endl;
   if ( ! histos_[1] ) 
      std::cout << "WARNING - MuonIdWeight::MuonIdWeight | Histogram weight_1up not found. Weight = 1" << std::endl;
   if ( ! histos_[2] ) 
      std::cout << "WARNING - MuonIdWeight::MuonIdWeight | Histogram weight_2up not found. Weight = 1" << std::endl;
}

MuonIdWeight::~MuonIdWeight()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//
// ------------ method called for each event  ------------

// output tree
float MuonIdWeight::weight(const float & pt, const float & eta , const int & var)
{
   float weight = 1.;
   if ( ! histos_[0] ) return weight;
   if ( ! histos_[var] ) return weight;
   int binx = histos_[var] -> GetXaxis()->FindBin(pt);
   int biny = histos_[var] -> GetYaxis()->FindBin(eta);
   weight = histos_[var] -> GetBinContent(binx, biny);
   return weight;
}

