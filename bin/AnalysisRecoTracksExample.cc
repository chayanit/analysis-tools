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
   Analysis analysis(inputList,"RecoNtuple/Events/EventInfo");
   
   // Physics Objects Collections
   analysis.addTree<RecoTrack> ("Tracks","RecoNtuple/Events/ALCARECOTkAlCosmicsCTF0T");
   

   TFile hout("histograms_tracks.root","recreate");
   
   std::map<std::string, TH1F*> h1;
   h1["n"]        = new TH1F("n" , "" , 30, 0, 30);
   h1["pt"]       = new TH1F("pt" , "" , 100, 0, 770);
   h1["eta"]      = new TH1F("eta" , "" , 100, -5.30, 3.30);
   h1["phi"]      = new TH1F("phi" , "" , 100, -3.7, 3.7);
   h1["q"]        = new TH1F("q", "", 4, -2, 2);
   h1["loose_qual"] = new TH1F("loose_qual", "", 2, 0, 2);
   
   // Analysis of events
   std::cout << "This analysis has " << analysis.size() << " events" << std::endl;
   int nevents =  analysis.size();

//    int nFired = 0;
   for ( int i = 0 ; i < nevents ; ++i )
   {
      if ( i > 0 && i%100000 == 0 ) std::cout << i << " events processed..." << std::endl;
      
      analysis.event(i);
      
      // Tracks
      auto tracks = analysis.collection<RecoTrack>("Tracks");
      int ntracks = 0;
      for ( int t = 0 ; t < tracks->size() ; ++t )
      {
         RecoTrack track = tracks->at(t);
         h1["pt"]         -> Fill(track.pt());
         h1["eta"]        -> Fill(track.eta());
         h1["phi"]        -> Fill(track.phi());
         h1["q"]          -> Fill(track.q());
         h1["loose_qual"] -> Fill(track.quality(TrackQuality::loose));
         ++ntracks;
         
      }
      h1["n"] -> Fill(ntracks);
      
   }
   
   for (auto & ih1 : h1)
   {
      ih1.second -> Write();
   }
   
//    
}

