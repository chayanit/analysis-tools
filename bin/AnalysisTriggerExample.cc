#include <string>
#include <iostream>
#include <vector>

#include "TFile.h" 
#include "TFileCollection.h"
#include "TChain.h"
#include "TH1.h" 

#include "Analysis/Tools/interface/Analysis.h"
#include "Analysis/Tools/bin/macro_config.h"

using namespace std;
using namespace analysis;
using namespace analysis::tools;


// =============================================================================================   
int main(int argc, char * argv[])
{
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms
   
   if ( macro_config(argc, argv) != 0 ) return -1;
   
   // Input files list
   Analysis analysis(inputlist_);
   
   // Trigger
   analysis.triggerResults(triggerCol_);
   
   // Trigger objects
   for ( auto & obj : triggerObjectsJets_ )
      analysis.addTree<TriggerObject>(obj,triggerObjDir_+"/"+obj);
   
   // Analysis of events
   std::cout << "This analysis has " << analysis.size() << " events" << std::endl;
   int nevts = analysis.size();
   if ( nevts > 0 ) nevts = nevtmax_;
   for ( int i = 0 ; i < nevts ; ++i )
   {
      analysis.event(i);
      
      std::cout << "++++++    ENTRY  " << i << std::endl;
      
      // Trigger results: fired and prescales
      // hltPath 
      int trg_fired = analysis.triggerResult(hltPath_);
      // prescales
      int l1ps  = analysis.triggerPrescale(l1Seed_);
      int hltps = analysis.triggerPrescale(hltPath_);
      std::string s_accept = " fired ";
      
      // printing info
      if ( ! trg_fired )
      {
         std::cout << "The path " << hltPath_ << " did not fire " << std::endl; 
         continue;
      }
      
      std::cout << "The path " << hltPath_ << " fired and has HLT PS = " << hltps << std::endl; 
      std::cout << "and its L1 seed " << l1Seed_ << " has the following prescale: " << l1ps << endl;
      
      // dealing with trigger objects
      // *** WARNING! *** the order you enter the trigger object in the config file matter!
      auto l1jets = analysis.collection<TriggerObject>(triggerObjectsJets_[0]);
      for ( int j = 0 ; j < l1jets->size() ; ++j )
      {
         TriggerObject l1jet = l1jets->at(j);
         std::cout << "L1 Jet: pT = " << l1jet.pt() << ", " << l1jet.eta() << ", " << l1jet.phi() << std::endl;
      }
      
   }
   
   
}

