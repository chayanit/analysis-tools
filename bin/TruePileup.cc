#include "TRandom3.h"
#include "Analysis/Tools/interface/Analyser.h"

// this produces histogram with the true pileup

using namespace std;
using namespace analysis;
using namespace analysis::tools;

int main(int argc, char ** argv)
{
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms
   
   Analyser analyser(argc,argv);
   
//   analyser.pileupHistogram();
   
   TRandom3 * rnd = new TRandom3();
   
   float ps = 1./analyser.config()->prescale();
   
   // events loop
   for ( int i = 0 ; i < analyser.nEvents() ; ++i )
   {
      // avoiding bias for some samples
      auto x = rnd->Rndm();
      if ( x > ps ) continue;
      analyser.event(i);
      analyser.fillPileupHistogram();
      
   }  //end events loop
   
} // end main
      
