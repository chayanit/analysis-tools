// system include files
#include "boost/program_options.hpp"
#include "boost/algorithm/string.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
// 
// user include files
#include "Analysis/Tools/interface/MuonAnalyser.h"

//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

MuonAnalyser::MuonAnalyser()
{
}


MuonAnalyser::MuonAnalyser(int argc, char * argv[]) : BaseAnalyser(argc,argv)
{
   // Physics objects
   // Muons
   muonsanalysis_  = ( analysis_->addTree<Muon> ("Muons",config_->muonsCollection()) != nullptr  && config_ -> nMuonsMin() > 0 );
   
}

MuonAnalyser::~MuonAnalyser()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//
// ------------ method called for each event  ------------

bool MuonAnalyser::analysisWithMuons()
{
   muons_.clear();
   selectedMuons_.clear();
   onlineMatchedMuons_.clear();
   if ( ! muonsanalysis_ ) return false;
   
   ++cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" )
   {
      h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Using Muon collection: %s",(config_->muonsCollection()).c_str()));
   }
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   
   if ( config_->triggerObjectsL1Muons() != "" )
      analysis_->match<Muon,TriggerObject>("Muons",config_->triggerObjectsL1Muons(),0.3);
   if ( config_->triggerObjectsL3Muons() != "" )
      analysis_->match<Muon,TriggerObject>("Muons",config_->triggerObjectsL3Muons(),0.3);

   auto muons = analysis_->collection<Muon>("Muons");
   for ( int j = 0 ; j < muons->size() ; ++j )  muons_.push_back(std::make_shared<Muon>(muons->at(j)));
   
   selectedMuons_ = muons_;
   
   return true;
}


std::vector< std::shared_ptr<Muon> > MuonAnalyser::muons()
{
   return muons_;
}

std::vector< std::shared_ptr<Muon> > MuonAnalyser::selectedMuons()
{
   return selectedMuons_;
}

std::vector< std::shared_ptr<Muon> > MuonAnalyser::onlineMatchedMuons()
{
   return onlineMatchedMuons_;
}




// FIXME: need to pass label to the histograms
void MuonAnalyser::muonHistograms(const std::string & obj, const int & n)
{
   if ( obj == "muon" )
   {
      for ( int j = 0; j < n; ++j )
      {
         h1_[Form("pt_%s%d"  , obj.c_str(),j+1)]    = std::make_shared<TH1F>(Form("pt_%s%d"  , obj.c_str(),j+1)   , "" ,100 , 0   , 1000  );
         h1_[Form("eta_%s%d" , obj.c_str(),j+1)]    = std::make_shared<TH1F>(Form("eta_%s%d" , obj.c_str(),j+1)   , "" , 60 , -3, 3 );
         h1_[Form("phi_%s%d" , obj.c_str(),j+1)]    = std::make_shared<TH1F>(Form("phi_%s%d" , obj.c_str(),j+1)   , "" , 64 , -3.2, 3.2 );
      }
      
   }
}


bool MuonAnalyser::selectionMuon(const int & r)
{
   bool isgood = true;
   ++cutflow_;
   int m = r-1;
   
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" )
   {
      if ( config_->muonsPtMax().size() > 0 && config_->muonsPtMax()[m] > config_->muonsPtMin()[m] )
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Muon %d: pt > %5.1f GeV and pt < %5.1f GeV and |eta| < %3.1f",r,config_->muonsPtMin()[m], config_->muonsPtMax()[m],config_->muonsEtaMax()[m] ));
      else
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Muon %d: pt > %5.1f GeV and |eta| < %3.1f",r,config_->muonsPtMin()[m], config_->muonsEtaMax()[m] ));
   }
   
   // kinematic selection
   if ( selectedMuons_[m] -> pt() < config_->muonsPtMin()[m]           && !(config_->muonsPtMin()[m] < 0) )   return false;
   if ( fabs(selectedMuons_[m] -> eta()) > config_->muonsEtaMax()[m]   && !(config_->muonsEtaMax()[m] < 0) )  return false;
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   return isgood;
}

bool MuonAnalyser::selectionMuons()
{
   // selectedMuons will be composed of muons with the lowest pt threshold

   if ( ! muonsanalysis_ ) return true;  // will skip this selection
   
      
   bool isgood = true;
   ++cutflow_;
//    
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" )
   {
      if ( config_->muonsPtMax().size() > 0 && config_->muonsPtMax().back() > config_->muonsPtMin().back() )
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Muons selected: pt > %5.1f GeV and pt < %5.1f GeV and |eta| < %3.1f", config_->muonsPtMin().back(), config_->muonsPtMax().back(), config_->muonsEtaMax().back() ));
      else
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Muons selected: pt > %5.1f GeV and |eta| < %3.1f", config_->muonsPtMin().back(), config_->muonsEtaMax().back() ));
   }
   
   // kinematic selection
   auto muon = std::begin(selectedMuons_);
   while ( muon != std::end(selectedMuons_) )
   {
      if ( config_->muonsPtMax().size() > 0 && config_->muonsPtMax().back() > config_->muonsPtMin().back() )
      {
         if ( (*muon)->pt() < config_->muonsPtMin().back() || (*muon)->pt() > config_->muonsPtMax().back() || fabs((*muon)->eta()) > config_->muonsEtaMax().back() )
            muon = selectedMuons_.erase(muon);
         else
            ++muon;
      }
      else
      {
         if ( (*muon)->pt() < config_->muonsPtMin().back() || fabs((*muon)->eta()) > config_->muonsEtaMax().back() )
            muon = selectedMuons_.erase(muon);
         else
            ++muon;
      }
   }
   if ( (int)selectedMuons_.size() < config_->nMuonsMin() ) return false;
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);
//    
   return isgood;
}



bool MuonAnalyser::selectionMuonId()
{
   if ( ! muonsanalysis_ ) return true;  // will skip this selection
   
   ++cutflow_;
   
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
      h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("MuonId: %s",config_->muonsId().c_str()));
   
//   if ( ! muonsanalysis_ ) return false;
   
   auto muon = std::begin(selectedMuons_);
   while ( muon != std::end(selectedMuons_) )
   {
      if ( ! (*muon)->id(config_->muonsId() ) )
         muon = selectedMuons_.erase(muon);
      else
         ++muon;
   }
   if ( selectedMuons_.size() == 0 ) return false;
   
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   return true;
}

bool MuonAnalyser::selectionNMuons()
{
   if ( ! muonsanalysis_ ) return true;  // will skip this selection
   
   
   ++cutflow_;
   
   if  ((int)selectedMuons_.size() < config_->nMuonsMin()) return false;
   
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
      h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("NMuons >= %d",config_->nMuonsMin()));
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   return true;
   
}


bool MuonAnalyser::onlineMuonMatching()
{
   if ( ! muonsanalysis_ ) return true;  // will skip this selection
   
   if ( config_->triggerObjectsL1Muons() == "" && config_->triggerObjectsL3Muons() == ""  ) return true;
   
   ++cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
      h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,"Muons selected: Online muon matching (L1 and L3)");
   
   auto muon = std::begin(selectedMuons_);
   while ( muon != std::end(selectedMuons_) )
   {
      if ( !((*muon)->matched(config_->triggerObjectsL1Muons()) && (*muon)->matched(config_->triggerObjectsL3Muons()) ))
         muon = selectedMuons_.erase(muon);
      else
         ++muon;
   }
   
   if ( (int)selectedMuons_.size() < config_->nMuonsMin() ) return false;
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   return true;
}

bool MuonAnalyser::onlineL1MuonMatching(const int & r)
{
   int j = r-1;
   if ( config_->triggerObjectsL1Muons() == "" ) return true;
   
   ++cutflow_;
   
   if ( r > config_->nMuonsMin() )
   {
      std::cout << "*Warning* MuonAnalyser::onlineL1MuonMatching(): asking for matching of unselected muon. Returning false!" << std::endl;
      return false;  // asking for a match beyond the selection, that's wrong, therefore false
   }
   if ( selectedMuons_.size() == 0 )
   {
      std::cout << "*Warning* MuonAnalyser::onlineL1MuonMatching(): selectedMuons is empty. Returning false!" << std::endl;
      return false;  // asking for a match beyond the selection, that's wrong, therefore false
   }
   
   if ( ! selectedMuons_[j]->matched(config_->triggerObjectsL1Muons()) ) return false;

   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
      h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("L1MuonTriggerMatch_%d",r));
      
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   return true;
}

bool MuonAnalyser::onlineL3MuonMatching(const int & r)
{
   int j = r-1;
   if ( config_->triggerObjectsL3Muons() == "" ) return true;
   
   ++cutflow_;
   
   if ( r > config_->nMuonsMin() )
   {
      std::cout << "*Warning* MuonAnalyser::onlineL3MuonMatching(): asking for matching of unselected muon. Returning false!" << std::endl;
      return false;  // asking for a match beyond the selection, that's wrong, therefore false
   }
   if ( selectedMuons_.size() == 0 )
   {
      std::cout << "*Warning* MuonAnalyser::onlineL3MuonMatching(): selectedMuons is empty. Returning false!" << std::endl;
      return false;  // asking for a match beyond the selection, that's wrong, therefore false
   }
   
   if ( ! selectedMuons_[j]->matched(config_->triggerObjectsL3Muons()) ) return false;

   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
      h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("L3MuonTriggerMatch_%d",r));
      
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   return true;
}


// FIXME: need to pass label to the histograms
void MuonAnalyser::fillMuonHistograms()
{
   int n = config_->nMuonsMin();
   
   for ( int j = 0; j < n; ++j )
   {
      h1_[Form("pt_muon%d",j+1)] -> Fill(selectedMuons_[j]->pt());
      h1_[Form("eta_muon%d",j+1)] -> Fill(selectedMuons_[j]->eta());
      h1_[Form("phi_muon%d",j+1)] -> Fill(selectedMuons_[j]->phi());
   }
   
}
