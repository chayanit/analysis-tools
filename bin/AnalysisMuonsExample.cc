#include <string>
#include <iostream>
#include <vector>

#include "TFile.h" 
#include "TFileCollection.h"
#include "TChain.h"
#include "TH1.h" 

#include "Analysis/Tools/interface/Analysis.h"

using namespace std;
using namespace analysis;
using namespace analysis::tools;


// =============================================================================================   
int main(int argc, char * argv[])
{
//   bool isMC = false;
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms
   
   // Input files list
   std::string inputList = "rootFileList.txt";
   Analysis analysis(inputList);
   
   // Physics Objects Collections
   analysis.addTree<Muon> ("Muons","MssmHbb/Events/slimmedMuons");

   TFile hout("histograms_muons.root","recreate");
   
   std::map<std::string, TH1F*> h1;
   h1["n"]        = new TH1F("n" , "" , 30, 0, 30);
   h1["pt"]       = new TH1F("pt" , "" , 100, 0, 770);
   h1["eta"]      = new TH1F("eta" , "" , 100, -5.30, 3.30);
   h1["phi"]      = new TH1F("phi" , "" , 100, -3.7, 3.7);
   h1["q"]        = new TH1F("q", "", 4, -2, 2);
   h1["isPF"]       = new TH1F("isPF","",2,0,2);
   h1["isGlobal"]   = new TH1F("isGlobal","",2,0,2);
   h1["isTracker"]  = new TH1F("isTracker","",2,0,2);
   h1["isLoose"]    = new TH1F("isLoose","",2,0,2);
   h1["isMedium"]   = new TH1F("isMedium","",2,0,2);
   
   
   // Analysis of events
   std::cout << "This analysis has " << analysis.size() << " events" << std::endl;
   int nevents =  analysis.size();

//    int nFired = 0;
   for ( int i = 0 ; i < nevents ; ++i )
   {
      if ( i > 0 && i%100000 == 0 ) std::cout << i << " events processed..." << std::endl;
      
      analysis.event(i);
      
      // Muons
      auto muons = analysis.collection<Muon>("Muons");
      int nmuons = 0;
      for ( int m = 0 ; m < muons->size() ; ++m )
      {
         Muon muon = muons->at(m);
         h1["pt"]         -> Fill(muon.pt());
         h1["eta"]        -> Fill(muon.eta());
         h1["phi"]        -> Fill(muon.phi());
         h1["q"]          -> Fill(muon.q());
         h1["isPF"]       -> Fill(muon.isPFMuon()     );
         h1["isGlobal"]   -> Fill(muon.isGlobalMuon() );
         h1["isTracker"]  -> Fill(muon.isTrackerMuon());
         h1["isLoose"]    -> Fill(muon.isLooseMuon()  );
         h1["isMedium"]   -> Fill(muon.isMediumMuon() );
         ++nmuons;
         
      }
      h1["n"] -> Fill(nmuons);
      
   }
   
   for (auto & ih1 : h1)
   {
      ih1.second -> Write();
   }
   
//    
}

