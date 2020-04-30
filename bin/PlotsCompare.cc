#include <string>
#include <iostream>
#include <vector>

#include "TFile.h" 
#include "TCanvas.h" 
#include "TApplication.h"
#include "TH1.h" 
#include "TRatioPlot.h" 
#include "TLegend.h" 
#include "Analysis/Tools/interface/plotscomp_config.h"
#include "Analysis/Tools/interface/tdrstyle.C"
#include "Analysis/Tools/interface/CMS_lumi.h"

int W;
int H;
// for the margins
float T;
float B; 
float L;
float R;

TCanvas * c1;
TFile * f[10];
TH1F * h[10];
TRatioPlot * rp;
TLegend * legend;
std::string hn[10];
std::string leg[10];
std::string fn[10];
   
int plot_ratio();
int draw_legend();

int main(int argc, char * argv[])
{
   if ( plotscomp_config(argc, argv) != 0 ) return -1;
   
   W = 800;
   H = 600;
   // for the margins
   T = 0.1*H;
   B = 0.48*H; 
   L = 0.12*W;
   R = 0.04*W;
   
   setTDRStyle();
   gStyle->SetOptStat(0);
   TApplication theApp("App", 0, 0);
   
   hn[0] = hist1_;
   hn[1] = hist2_;
   
   fn[0] = file1_;
   fn[1] = file2_;
   
   leg[0] = legend1_;
   leg[1] = legend2_;
   
   int color[10] = {kBlack,kRed};
   
   TString out = TString(outplot_);
   
//   if ( outplot_ == "" ) outplot_ = "comparison_plot.png";
   
   // read 2 histograms from the same file
   if ( file_ != "" )
   {
      f[0] = new TFile(file_.c_str(),"old");
      for ( int i = 0; i < 2 ; ++i )
      {
         if ( hn[i] == "" ) return -1;
         h[i] = (TH1F*) f[0] -> Get(hn[i].c_str());
         h[i] -> SetNameTitle(Form("%s_%d",hn[i].c_str(),i), Form("%s (%s)",leg[i].c_str(),h[i]->GetXaxis()->GetTitle()));
         h[i] -> GetYaxis() -> SetTitle("entries");
      }
   }
   // read same histogram from 2 different files
   else
   {
      if ( hist_ == "" ) return -1;
      for ( int i = 0; i < 2 ; ++i )
      {
         if ( fn[i] == "" ) return -1;
         f[i] = new TFile(fn[i].c_str(),"old");
         h[i] = (TH1F*) f[i] -> Get(hist_.c_str());
         h[i] -> SetNameTitle(Form("%s_%d",hist_.c_str(),i), Form("%s (%s)",title_.c_str(),h[i]->GetXaxis()->GetTitle()));
      }
   }
   if ( outplot_ == "" )
   {
      out = TString(std::string(h[0]->GetName()) + "_versus_" + std::string(h[1]->GetName()) + ".png");
      out.ReplaceAll("/","_");
   }

   for ( int i = 0; i < 2 ; ++i )
   {
      h[i] -> SetMarkerStyle(20);
      h[i] -> SetMarkerSize(1.2);
      h[i] -> SetMarkerColor(color[i]);
      h[i] -> SetLineColor(color[i]);
      h[i] -> SetLineWidth(2);
      h[i] -> GetYaxis() -> SetTitle("entries");
      if ( max_ > min_ )
      {
         h[i] -> GetXaxis()->SetRangeUser(min_,max_);
      }
      h[i] -> SetMinimum(0);
      if ( logy_ ) h[i] -> SetMinimum(0.8);
   }
   h[1] -> Scale(scale_);
   
   // Ratio plot
   plot_ratio();
   draw_legend();
   
//    
   if ( logx_ ) c1 -> SetLogx();   
   if ( logy_ ) c1 -> SetLogy();   
   
   CMS_lumi(rp->GetUpperPad(),"private");
   
   legend -> SetShadowColor(0);
   
//   if ( outplot_ == "" ) outplot_ = "comparison_plot.png";
   c1 -> SaveAs(out);
   
   if ( app_ ) theApp.Run();    // will not return
   
   delete rp;
   delete c1;
   delete f[0];
   delete f[1];
   
   return 0;
}

int plot_ratio()
{
   // Prepare a Canvas
   c1 = new TCanvas("c1","test", W, H);
   c1 -> SetFillColor(0);
   c1 -> SetBorderMode(0);
   c1 -> SetFrameFillStyle(0);
   c1 -> SetFrameBorderMode(0);
   c1 -> SetTickx(0);
   c1 -> SetTicky(0);
      
   // The ratio plot
   rp = new TRatioPlot(h[0], h[1]);
   
   rp -> SetH1DrawOpt("E");
   rp -> SetH2DrawOpt("E");
   rp -> SetGraphDrawOpt("P");  
   rp -> SetSeparationMargin(0.03); 
   
   rp -> SetLeftMargin( L/W );
   rp -> SetRightMargin( R/W );
   rp -> SetUpTopMargin( T/H );
   rp -> SetLowBottomMargin( B/H );
   
   rp->Draw();
   
   rp -> GetLowYaxis() -> SetNdivisions(503);
   rp -> GetLowerRefGraph() -> GetXaxis() -> SetTitleOffset(1.1);
   rp -> GetLowerRefGraph() -> GetYaxis() -> SetTitle("ratio");
   rp -> GetLowerRefYaxis() -> SetTitleOffset(1.2);
   rp -> GetLowerRefYaxis() -> SetLabelSize(0.05);
   rp -> GetLowerRefYaxis() -> SetTitleSize(0.05);
   rp->GetLowerRefGraph()->SetMarkerStyle(20);
   rp->GetLowerRefGraph()->SetMarkerColor(kGray+2);
   rp->GetLowerRefGraph()->SetMarkerSize(1.2);
   rp->GetLowerRefGraph()->SetLineWidth(2);
   rp->GetLowerRefGraph()->SetLineColor(kGray+2);
   rp->GetLowerRefGraph()->SetMinimum(0.);
   rp->GetLowerRefGraph()->SetMaximum(2.4);
   
   rp -> GetUpperRefXaxis() -> SetLabelSize(0.05);
   rp -> GetUpperRefXaxis() -> SetTitleSize(0.05);
   
   rp -> GetUpperRefYaxis() -> SetLabelSize(0.05);
   rp -> GetUpperRefYaxis() -> SetTitleSize(0.05);
   rp -> GetUpperRefYaxis() -> SetTitleOffset(1.2);
   
   if ( logx_ ) rp -> GetUpperPad() -> SetLogx();
   if ( logy_ ) rp -> GetUpperPad() -> SetLogy();
   
   c1->Update();
   
   return 0;
}

int draw_legend()
{
   std::string var = h[0]->GetName();

   if ( TString(var).Contains("m_jet") || TString(var).Contains("pt_jet") )
      legend = rp -> GetUpperPad() -> BuildLegend(0.65,0.65,0.90,0.85);
   else if ( !TString(var).Contains("deta_jet") && ( TString(var).Contains("eta_jet") || TString(var).Contains("phi_jet") ))
      legend = rp -> GetUpperPad() -> BuildLegend(0.40,0.05,0.65,0.25);
//      legend = rp -> GetUpperPad() -> BuildLegend(0.65,0.65,0.90,0.85);
   else if ( TString(var).Contains("btag_jet") )
   {  
      if ( logy_ )
         legend = rp -> GetUpperPad() -> BuildLegend(0.35,0.05,0.60,0.25);
      else
         legend = rp -> GetUpperPad() -> BuildLegend(0.35,0.65,0.60,0.85);
   }
   else
      legend = rp -> GetUpperPad() -> BuildLegend(0.15,0.65,0.40,0.85);
   legend -> Clear();
   
   legend ->AddEntry(h[0], leg[0].c_str(), "P");
   legend ->AddEntry(h[1], leg[1].c_str(), "P");
   
   c1->Update();
   
   return 0;
}
