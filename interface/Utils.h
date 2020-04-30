#ifndef Analysis_Tools_Utils_h
#define Analysis_Tools_Utils_h 1

#include <boost/algorithm/string/classification.hpp> // Include boost::for is_any_of
#include <boost/algorithm/string/split.hpp> // Include for boost::split
#include "Analysis/Tools/interface/JetResolution.h"

namespace analysis {
   namespace tools {

      struct FilterResults
      {
         int   total;
         int   filtered;
         float efficiency;
      };
      
      struct PDF
      {
         std::pair<int,int> id;
         std::pair<double,double> x;
      };

      template <typename T> 
      struct less_than_pt
      {
          inline bool operator() (const T& gp1, const T& gp2)
          {
              return (gp1.pt() < gp2.pt());
          }
      };
      
      template <typename T> 
      struct greater_than_pt
      {
          inline bool operator() (const T& gp1, const T& gp2)
          {
              return (gp1.pt() > gp2.pt());
          }
      };
      
      
      inline std::vector<std::string> getWords(const std::string & line)
      {
         std::vector<std::string> words;
         boost::split(words, line, boost::is_any_of(" "), boost::token_compress_on);
         return words;
      }
      
      struct jerDefinitions
      {
      };
      
      struct JetResolutionInfo
      {
//         JetResolutionInfo(JME::JetResolution r, JME::JetResolutionScaleFactor s) :
//               resolution(r), scalefactor(s)
//         {}
         JME::JetResolution resolution;
         JME::JetResolutionScaleFactor scalefactor;
      };      

      struct ScaleFactors
      {
         float nominal;
         float up;
         float down;
      };
      
   }
}




#endif  // Analysis_Tools_Utils_h
