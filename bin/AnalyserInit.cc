#include "Analysis/Tools/interface/Analyser.h"

// this example has no selection
// for MC it will apply the generator weights

using namespace std;
using namespace analysis;
using namespace analysis::tools;

int main(int argc, char ** argv)
{
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms
   
   Analyser analyser(argc,argv);
   
   // you can always get the ususal analysis class
   auto analysis = analyser.analysis();
   
   for ( int i = 0 ; i < analyser.nEvents() ; ++i )
   {
      if ( ! analyser.event(i)                  )   continue;
      
   }  //end event loop
} // end main
      
