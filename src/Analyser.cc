// system include files
#include "boost/program_options.hpp"
#include "boost/algorithm/string.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
// 
// user include files
#include "Analysis/Tools/interface/Analyser.h"

//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

Analyser::Analyser()
{
}


Analyser::Analyser(int argc, char * argv[]) : BaseAnalyser(argc,argv),  // not sure the BaseAnalyser should be called here
                                              TriggerAnalyser(argc,argv),
                                              JetAnalyser(argc,argv),
                                              MuonAnalyser(argc,argv)
{
   this -> pileupHistogram();
}

Analyser::~Analyser()
{
}


//
// member functions
//
// ------------ method called for each event  ------------

bool Analyser::event(const int & i)
{
   bool ok = true;
   analysis_->event(i);
   cutflow_ = 0;
   weight_ = 1.;  // reset weight at the beginning of the event analysis
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   this -> generatorWeight();
   
   if ( config_->runmin_ > 0 && analysis_->run() < config_->runmin_ ) return false;
   if ( config_->runmax_ > 0 && analysis_->run() > config_->runmax_ ) return false;

   if (! config_->isMC() ) 
   {
       auto json = basename(config_->json_);
       ++cutflow_;
       if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" )
       {
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Certified data: %s",json.c_str()));
       }
       ok = analysis_->selectJson();
       if ( ! ok ) return false;
       h1_["cutflow"] -> Fill(cutflow_,weight_);
   }
   
   if ( this->genParticlesAnalysis() )
   {
      ++cutflow_;
      if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" )
      {
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Using GenParticles collection: %s",(config_->genParticlesCollection()).c_str()));
      }
      h1_["cutflow"] -> Fill(cutflow_,weight_);
   }
   
   if ( this->genJetsAnalysis() )
   {
      ++cutflow_;
      if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" )
      {
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Using GenJets collection: %s",(config_->genJetsCollection()).c_str()));
      }
      h1_["cutflow"] -> Fill(cutflow_,weight_);
   }

   analysisWithJets();
   analysisWithMuons();
      
    
   return ok;
   
}

void Analyser::generatorWeight()
{
   if ( ! config_->isMC() ) return;
   
   ++cutflow_;
   float weight = analysis_->genWeight();
   if ( config_->fullGenWeight() )
   {
      weight_ *= weight;
   }
   else
   {
      float sign =  (weight > 0) ? 1 : ((weight < 0) ? -1 : 0);
      weight_ *= sign;
   }
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
}

bool Analyser::muonJet(const int & r)
{
   
   if ( ! muonsanalysis_ ) return true;  // will skip this selection

   int j = r-1;
   ++ cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
   {
      h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d: Jet-muon association",r));
   }
   
   auto jet = selectedJets_[j];
   jet -> addMuon(selectedMuons_);
   
   if ( ! jet -> muon() ) return false;
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   return true;
   
}

