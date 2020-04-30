#include "Analysis/Tools/interface/TriggerAnalyser.h"
// system include files
#include "boost/program_options.hpp"
#include "boost/algorithm/string.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
// 
// user include files
#include "TString.h" 

//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

TriggerAnalyser::TriggerAnalyser()
{
}


TriggerAnalyser::TriggerAnalyser(int argc, char * argv[]) : BaseAnalyser(argc,argv)
{
   triggeranalysis_ = false;
   if ( config_->triggerResults() != "" )
      triggeranalysis_  = analysis_->triggerResults(config_->triggerResults());
   
   if ( config_->triggerObjectsDir() != "" )
   {
      // online jets
      analysis_->addTree<TriggerObject> (config_->triggerObjectsL1Jets()  ,Form("%s/%s", config_->triggerObjectsDir().c_str(),config_->triggerObjectsL1Jets().c_str()));
      analysis_->addTree<TriggerObject> (config_->triggerObjectsCaloJets(),Form("%s/%s", config_->triggerObjectsDir().c_str(),config_->triggerObjectsCaloJets().c_str()));
      analysis_->addTree<TriggerObject> (config_->triggerObjectsPFJets()  ,Form("%s/%s", config_->triggerObjectsDir().c_str(),config_->triggerObjectsPFJets().c_str()));
      // online b jets
      analysis_->addTree<TriggerObject> (config_->triggerObjectsBJets(),Form("%s/%s", config_->triggerObjectsDir().c_str(),config_->triggerObjectsBJets().c_str()));
      // online muons
      analysis_->addTree<TriggerObject> (config_->triggerObjectsL1Muons(),Form("%s/%s",config_->triggerObjectsDir().c_str(),config_->triggerObjectsL1Muons().c_str()));
      analysis_->addTree<TriggerObject> (config_->triggerObjectsL3Muons(),Form("%s/%s",config_->triggerObjectsDir().c_str(),config_->triggerObjectsL3Muons().c_str()));
   }
   
}

TriggerAnalyser::~TriggerAnalyser()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//
// ------------ method called for each event  ------------


bool TriggerAnalyser::selectionTrigger() // Maybe not use this, use selectionHLT and selectionL1
{
   bool hlt = selectionHLT();
   bool l1  = selectionL1();
   
   return (hlt && l1);
   
}

bool TriggerAnalyser::selectionHLT()
{
   if ( config_->hltPath_ == "" ) return true;
   
   ++cutflow_;
   if ( ! analysis_->triggerResult(config_->hltPath_) ) return false;
   
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
      h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,(config_->hltPath_).c_str());
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);

   return true;
}

bool TriggerAnalyser::selectionL1()
{
   if ( config_->l1Seed_ == "" ) return true;
   
   ++cutflow_;
   if ( ! analysis_->triggerResult(config_->l1Seed_)  ) return false;
   
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
      h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,(config_->l1Seed_).c_str());
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);

   return true;
}


bool TriggerAnalyser::analysisWithTrigger()
{
   return triggeranalysis_;
}


std::vector< std::shared_ptr<TriggerObject> > TriggerAnalyser::triggerObjectsL1Jets()
{
   auto collection = analysis_->collection<TriggerObject>(config_->triggerObjectsL1Jets());
   std::vector< std::shared_ptr<TriggerObject> > objects;
   for ( int j = 0 ; j < collection->size() ; ++j )
      objects.push_back(std::make_shared<TriggerObject>(collection->at(j)));
   return objects;
}

std::vector< std::shared_ptr<TriggerObject> > TriggerAnalyser::triggerObjectsCaloJets()
{
   auto collection = analysis_->collection<TriggerObject>(config_->triggerObjectsCaloJets());
   std::vector< std::shared_ptr<TriggerObject> > objects;
   for ( int j = 0 ; j < collection->size() ; ++j )
      objects.push_back(std::make_shared<TriggerObject>(collection->at(j)));
   return objects;
}

std::vector< std::shared_ptr<TriggerObject> > TriggerAnalyser::triggerObjectsPFJets()
{
   auto collection = analysis_->collection<TriggerObject>(config_->triggerObjectsPFJets());
   std::vector< std::shared_ptr<TriggerObject> > objects;
   for ( int j = 0 ; j < collection->size() ; ++j )
      objects.push_back(std::make_shared<TriggerObject>(collection->at(j)));
   return objects;
}

std::vector< std::shared_ptr<TriggerObject> > TriggerAnalyser::triggerObjectsL1Muons()
{
   auto collection = analysis_->collection<TriggerObject>(config_->triggerObjectsL1Muons());
   std::vector< std::shared_ptr<TriggerObject> > objects;
   for ( int j = 0 ; j < collection->size() ; ++j )
      objects.push_back(std::make_shared<TriggerObject>(collection->at(j)));
   return objects;
}

std::vector< std::shared_ptr<TriggerObject> > TriggerAnalyser::triggerObjectsL3Muons()
{
   auto collection = analysis_->collection<TriggerObject>(config_->triggerObjectsL3Muons());
   std::vector< std::shared_ptr<TriggerObject> > objects;
   for ( int j = 0 ; j < collection->size() ; ++j )
      objects.push_back(std::make_shared<TriggerObject>(collection->at(j)));
   return objects;
}

