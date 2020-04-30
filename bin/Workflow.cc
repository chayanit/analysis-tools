#include <string>
#include <iostream>
#include <vector>

#include "TFile.h" 
#include "TH1.h" 


// =============================================================================================   
int main(int argc, char * argv[])
{
   if ( argc <= 1 ) return -1;
   
   const char * rootfile = argv[1];
   
   TFile f(rootfile,"old");
   
   TH1F * h = (TH1F*) f.Get("workflow");
   
   
   printf("+%s+\n", std::string(170,'-').c_str());
   printf("| %-108s |    %10s |   %16s |   %16s |\n",h->GetTitle(),"n events","ratio wrt first","ratio wrt previous");
   printf("+%s+\n", std::string(170,'-').c_str());
   int firstbin = 2;
   for ( int i = 1; i <= h ->GetNbinsX(); ++i )
   {
      std::string label = std::string(h->GetXaxis()->GetBinLabel(i));
      if ( label == "" ) continue;
//      if ( firstbin < 0 ) firstbin = i;
      float n = h->GetBinContent(i);
      float rn1 = h->GetBinContent(i)/h->GetBinContent(firstbin);
      float rni = 0;
      if ( i == 1 )
      {
         printf("| %-108s |    %10.1f |   %16s |  %19s |\n",label.c_str(),n,"-","-");
      }
      else if ( i == 2 )
      {
         printf("| %2d - %-103s |    %10.1f |   %16.6f |  %19s |\n",i-1,label.c_str(),n,rn1,"-");
      }
      else
      {
         rni = h-> GetBinContent(i)/h->GetBinContent(i-1);
         printf("| %2d - %-103s |    %10.1f |   %16.6f |     %16.6f |\n",i-1,label.c_str(),n,rn1,rni);
      }
      
   }
   printf("+%s+\n", std::string(170,'-').c_str());

   return 0;
   
}

