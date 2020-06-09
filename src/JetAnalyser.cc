#include "Analysis/Tools/interface/JetAnalyser.h"

// system include files
#include "boost/program_options.hpp"
#include "boost/algorithm/string.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
// 
// user include files
#include "Analysis/Tools/interface/Composite.h"

//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

JetAnalyser::JetAnalyser()
{
}


JetAnalyser::JetAnalyser(int argc, char * argv[]) : BaseAnalyser(argc,argv)
{
   // Physics objects
   // Jets
   jetsanalysis_  = ( analysis_->addTree<Jet> ("Jets",config_->jetsCollection()) != nullptr );
   
   applyjer_ = false;
   
   if ( config_->btagScaleFactors() != "" )
   {
      bsf_reader_["loose"]  = analysis_->btagCalibration(config_->btagAlgorithm(), config_->btagScaleFactors(), "loose"); 
      bsf_reader_["medium"] = analysis_->btagCalibration(config_->btagAlgorithm(), config_->btagScaleFactors(), "medium");
      bsf_reader_["tight"]  = analysis_->btagCalibration(config_->btagAlgorithm(), config_->btagScaleFactors(), "tight");
   }
   
   if ( config_->jerPtRes() != "" && config_->jerSF() != "" && this->genJetsAnalysis() ) // FIXME: check if files exist
   {
      jerinfo_ = analysis_->jetResolutionInfo(config_->jerPtRes(),config_->jerSF());
      applyjer_ = ( jerinfo_ != nullptr && jetsanalysis_ );
   }
   
   if ( config_->isMC() )
   {
      flavours_ = {"udsg","c","b"};
      if ( config_->useJetsExtendedFlavour() )
      {
         flavours_.push_back("cc");
         flavours_.push_back("bb");
      }
   }
//   histograms("jet",config_->nJetsMin());

}

JetAnalyser::~JetAnalyser()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//
// ------------ method called for each event  ------------

bool JetAnalyser::analysisWithJets()
{
   jets_.clear();
   selectedJets_.clear();
   if ( ! jetsanalysis_ ) return false;
   
   ++cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" )
   {
      h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Using Jet collection: %s",(config_->jetsCollection()).c_str()));
   }
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   
   
   analysis_->match<Jet,TriggerObject>("Jets",config_->triggerObjectsL1Jets(),0.3);
   analysis_->match<Jet,TriggerObject>("Jets",config_->triggerObjectsCaloJets(),0.3);
   analysis_->match<Jet,TriggerObject>("Jets",config_->triggerObjectsPFJets(),0.3);
   analysis_->match<Jet,TriggerObject>("Jets",config_->triggerObjectsBJets(),0.3);

   // std::shared_ptr< Collection<Jet> >
   auto jets = analysis_->collection<Jet>("Jets");
   
   if ( this->genParticlesAnalysis() && config_ -> useJetsExtendedFlavour() )
   {
      auto particles = analysis_->collection<GenParticle>("GenParticles");
      jets->associatePartons(particles,0.4,1.,config_->pythia8());
   }
   
   if ( this->genJetsAnalysis() )
   {
      auto genjets = analysis_->collection<GenJet>("GenJets");
      jets->addGenJets(genjets);
   }
   
   for ( int j = 0 ; j < jets->size() ; ++j )  jets_.push_back(std::make_shared<Jet>(jets->at(j)));
   
   selectedJets_ = jets_;
   
   return true;
}


void JetAnalyser::jets(const std::string & col)
{
   analysis_->addTree<Jet> ("Jets",col);
}

void JetAnalyser::jetHistograms( const std::string & label )
{
   this -> jetHistograms(config_->nJetsMin(), label);
}
void JetAnalyser::jetHistograms( const int & n, const std::string & label )
{
   this->output()->cd();
   this->output()->mkdir(label.c_str());
   this->output()->cd(label.c_str());
   
   n_hjets_ = n;
   
   h1_[Form("jet_hist_weight_%s",label.c_str())] = std::make_shared<TH1F>(Form("jet_hist_weight_%s",label.c_str()) , Form("jet_hist_weight_%s",label.c_str()) ,1 , 0. , 1. );
   
   // btag variable binning
   float min1 = 0.0;
   float max1 = 0.1;
   float size1 = 0.0001;
   int nbins1 = int((max1-min1)/size1);
   float min2 = 0.1;
   float max2 = 0.9;
   float size2 = 0.001;
   int nbins2 = int((max2-min2)/size2);
   float min3 = 0.9;
   float max3 = 1.0;
   float size3 = 0.0001;
   int nbins3 = int((max3-min3)/size3);
   int nbins_btag = nbins1+nbins2+nbins3;
   
   std::vector<float> bins_btag;
   bins_btag.clear();
   int counter = 0; 
   for ( int i = 0; i<nbins1; ++i)   {  bins_btag.push_back(min1 + size1*i);  ++counter; }
   for ( int i = 0; i<nbins2; ++i)   {  bins_btag.push_back(min2 + size2*i);  ++counter; }
   for ( int i = 0; i<nbins3+1; ++i) {  bins_btag.push_back(min3 + size3*i);  ++counter; }
   
   // uniform binning for btag (comment it out if you want the variable binning above)
   float size = 0.0002;
   nbins_btag = int(1./size);
   bins_btag.clear();
   for ( int i = 0; i<nbins_btag+1; ++i)   {  bins_btag.push_back(size*i);  ++counter; }
   
   
   for ( int j = 0; j < n; ++j ) // loop over jets
   {
      // 1D histograms
      h1_[Form("pt_jet%d_%s"  , j+1,label.c_str())]  = std::make_shared<TH1F>(Form("pt_jet%d"  , j+1) , Form("pt_jet%d_%s"  , j+1,label.c_str()) ,1500 , 0   , 1500  );
      h1_[Form("eta_jet%d_%s" , j+1,label.c_str())]  = std::make_shared<TH1F>(Form("eta_jet%d" , j+1) , Form("eta_jet%d_%s" , j+1,label.c_str()) , 600 , -3, 3 );
      h1_[Form("phi_jet%d_%s" , j+1,label.c_str())]  = std::make_shared<TH1F>(Form("phi_jet%d" , j+1) , Form("phi_jet%d_%s" , j+1,label.c_str()) , 360 , -180, 180 );
      h1_[Form("btag_jet%d_%s", j+1,label.c_str())]  = std::make_shared<TH1F>(Form("btag_jet%d", j+1) , Form("btag_jet%d_%s", j+1,label.c_str()) , nbins_btag, &bins_btag[0] );
      h1_[Form("btaglog_jet%d_%s", j+1,label.c_str())]  = std::make_shared<TH1F>(Form("btaglog_jet%d", j+1) , Form("btaglog_jet%d_%s", j+1,label.c_str()) , 200 , 1.e-6, 10 );
      h1_[Form("qglikelihood_jet%d_%s", j+1,label.c_str())]  = std::make_shared<TH1F>(Form("qglikelihood_jet%d", j+1) , Form("qglikelihood_jet%d_%s", j+1,label.c_str()) , 200 , 0, 1 );
      h1_[Form("nconstituents_jet%d_%s", j+1,label.c_str())]  = std::make_shared<TH1F>(Form("nconstituents_jet%d", j+1) , Form("nconstituents_jet%d_%s", j+1,label.c_str()) , 200 , 0, 200 );
      // pt distributions separate for barrel and endcap, minus and plus sides and overlaps
      if ( config_-> histogramJetsRegionSplit() )
      {
         h1_[Form("pt_jet%d_me_%s"  , j+1,label.c_str())]  = std::make_shared<TH1F>(Form("pt_jet%d_me"  , j+1) , Form("pt_jet%d_me_%s"  , j+1,label.c_str()) ,1500 , 0   , 1500  );
         h1_[Form("pt_jet%d_pe_%s"  , j+1,label.c_str())]  = std::make_shared<TH1F>(Form("pt_jet%d_pe"  , j+1) , Form("pt_jet%d_pe_%s"  , j+1,label.c_str()) ,1500 , 0   , 1500  );
         h1_[Form("pt_jet%d_mb_%s"  , j+1,label.c_str())]  = std::make_shared<TH1F>(Form("pt_jet%d_mb"  , j+1) , Form("pt_jet%d_mb_%s"  , j+1,label.c_str()) ,1500 , 0   , 1500  );
         h1_[Form("pt_jet%d_pb_%s"  , j+1,label.c_str())]  = std::make_shared<TH1F>(Form("pt_jet%d_pb"  , j+1) , Form("pt_jet%d_pb_%s"  , j+1,label.c_str()) ,1500 , 0   , 1500  );
         h1_[Form("pt_jet%d_mbe_%s" , j+1,label.c_str())]  = std::make_shared<TH1F>(Form("pt_jet%d_mbe" , j+1) , Form("pt_jet%d_mbe_%s"  , j+1,label.c_str()) ,1500 , 0   , 1500  );
         h1_[Form("pt_jet%d_pbe_%s" , j+1,label.c_str())]  = std::make_shared<TH1F>(Form("pt_jet%d_pbe" , j+1) , Form("pt_jet%d_pbe_%s"  , j+1,label.c_str()) ,1500 , 0   , 1500  );
      }
      
      h1_[Form("pt_jet%d_%s"  , j+1,label.c_str())] -> GetXaxis() -> SetTitle(Form("Jet %d p_{T} [GeV]",j+1));
      h1_[Form("eta_jet%d_%s" , j+1,label.c_str())] -> GetXaxis() -> SetTitle(Form("Jet %d  #eta",j+1));
      h1_[Form("phi_jet%d_%s" , j+1,label.c_str())] -> GetXaxis() -> SetTitle(Form("Jet %d  #phi",j+1));
      h1_[Form("btag_jet%d_%s", j+1,label.c_str())] -> GetXaxis() -> SetTitle(Form("Jet %d btag discriminator",j+1)); 
      h1_[Form("btaglog_jet%d_%s", j+1,label.c_str())] -> GetXaxis() -> SetTitle(Form("Jet %d -ln(1-btag discriminator)",j+1)); 
      h1_[Form("qglikelihood_jet%d_%s", j+1,label.c_str())] -> GetXaxis() -> SetTitle(Form("Jet %d q-g likelihood",j+1)); 
      h1_[Form("nconstituents_jet%d_%s", j+1,label.c_str())]-> GetXaxis() -> SetTitle(Form("Jet %d n constituents",j+1)); 
      
      if ( config_->btagAlgorithm() == "deepcsv")
      {
         h1_[Form("btag_light_jet%d_%s", j+1,label.c_str())] = std::make_shared<TH1F>(Form("btag_light_jet%d", j+1) , Form("btag_light_jet%d_%s", j+1,label.c_str()) , nbins_btag, &bins_btag[0] );         
         h1_[Form("btag_c_jet%d_%s"    , j+1,label.c_str())] = std::make_shared<TH1F>(Form("btag_c_jet%d"    , j+1) , Form("btag_c_jet%d_%s"    , j+1,label.c_str()) , nbins_btag, &bins_btag[0] );         
         h1_[Form("btag_b_jet%d_%s"    , j+1,label.c_str())] = std::make_shared<TH1F>(Form("btag_b_jet%d"    , j+1) , Form("btag_b_jet%d_%s"    , j+1,label.c_str()) , nbins_btag, &bins_btag[0] );         
         h1_[Form("btag_bb_jet%d_%s"   , j+1,label.c_str())] = std::make_shared<TH1F>(Form("btag_bb_jet%d"   , j+1) , Form("btag_bb_jet%d_%s"   , j+1,label.c_str()) , nbins_btag, &bins_btag[0] );         
         h1_[Form("btag_cc_jet%d_%s"   , j+1,label.c_str())] = std::make_shared<TH1F>(Form("btag_cc_jet%d"   , j+1) , Form("btag_cc_jet%d_%s"   , j+1,label.c_str()) , nbins_btag, &bins_btag[0] );         
      }
      if ( config_->btagAlgorithm() == "deepflavour" || config_->btagAlgorithm() == "deepjet" )
      {
         h1_[Form("btag_light_jet%d_%s", j+1,label.c_str())] = std::make_shared<TH1F>(Form("btag_light_jet%d", j+1) , Form("btag_light_jet%d_%s", j+1,label.c_str()) , nbins_btag, &bins_btag[0] );         
         h1_[Form("btag_g_jet%d_%s"    , j+1,label.c_str())] = std::make_shared<TH1F>(Form("btag_g_jet%d"    , j+1) , Form("btag_g_jet%d_%s"    , j+1,label.c_str()) , nbins_btag, &bins_btag[0] );         
         h1_[Form("btag_c_jet%d_%s"    , j+1,label.c_str())] = std::make_shared<TH1F>(Form("btag_c_jet%d"    , j+1) , Form("btag_c_jet%d_%s"    , j+1,label.c_str()) , nbins_btag, &bins_btag[0] );         
         h1_[Form("btag_b_jet%d_%s"    , j+1,label.c_str())] = std::make_shared<TH1F>(Form("btag_b_jet%d"    , j+1) , Form("btag_b_jet%d_%s"    , j+1,label.c_str()) , nbins_btag, &bins_btag[0] );         
         h1_[Form("btag_bb_jet%d_%s"   , j+1,label.c_str())] = std::make_shared<TH1F>(Form("btag_bb_jet%d"   , j+1) , Form("btag_bb_jet%d_%s"   , j+1,label.c_str()) , nbins_btag, &bins_btag[0] );         
         h1_[Form("btag_lepb_jet%d_%s" , j+1,label.c_str())] = std::make_shared<TH1F>(Form("btag_lepb_jet%d" , j+1) , Form("btag_lepb_jet%d_%s" , j+1,label.c_str()) , nbins_btag, &bins_btag[0] );         
      }
      
      // 2D histograms
      h2_[Form("pt_eta_jet%d_%s"  , j+1,label.c_str())]  = std::make_shared<TH2F>(Form("pt_eta_jet%d"  , j+1) , Form("pt_eta_jet%d_%s"  , j+1,label.c_str()) ,1500 , 0   , 1500, 600, -3, 3  );
      h2_[Form("pt_eta_jet%d_%s"  , j+1,label.c_str())] -> GetXaxis() -> SetTitle(Form("Jet %d p_{T} [GeV]",j+1));
      h2_[Form("pt_eta_jet%d_%s"  , j+1,label.c_str())] -> GetYaxis() -> SetTitle(Form("Jet %d  #eta",j+1));
      
      if ( config_->isMC() && config_->histogramJetsPerFlavour() )
      {
         for ( auto & flv : flavours_ ) // flavour dependent histograms
         {
            // 1D histograms
            h1_[Form("pt_jet%d_%s_%s"  , j+1,label.c_str(),flv.c_str())]  = std::make_shared<TH1F>(Form("pt_jet%d_%s"   , j+1, flv.c_str()) , Form("pt_jet%d_%s_%s"  , j+1,label.c_str(),flv.c_str()) ,1500 , 0   , 1500  );
            h1_[Form("eta_jet%d_%s_%s" , j+1,label.c_str(),flv.c_str())]  = std::make_shared<TH1F>(Form("eta_jet%d_%s"  , j+1, flv.c_str()) , Form("eta_jet%d_%s_%s" , j+1,label.c_str(),flv.c_str()) , 600 , -3, 3 );
            h1_[Form("phi_jet%d_%s_%s" , j+1,label.c_str(),flv.c_str())]  = std::make_shared<TH1F>(Form("phi_jet%d_%s"  , j+1, flv.c_str()) , Form("phi_jet%d_%s_%s" , j+1,label.c_str(),flv.c_str()) , 360 , -180, 180 );
            h1_[Form("btag_jet%d_%s_%s", j+1,label.c_str(),flv.c_str())]  = std::make_shared<TH1F>(Form("btag_jet%d_%s" , j+1, flv.c_str()) , Form("btag_jet%d_%s_%s", j+1,label.c_str(),flv.c_str()) , nbins_btag, &bins_btag[0] );
            h1_[Form("qglikelihood_jet%d_%s_%s", j+1,label.c_str(),flv.c_str())]  = std::make_shared<TH1F>(Form("qglikelihood_jet%d_%s" , j+1, flv.c_str()) , Form("qglikelihood_jet%d_%s_%s", j+1,label.c_str(),flv.c_str()) , nbins_btag, &bins_btag[0] );
            h1_[Form("nconstituents_jet%d_%s_%s", j+1,label.c_str(),flv.c_str())]  = std::make_shared<TH1F>(Form("nconstituents_jet%d_%s" , j+1, flv.c_str()) , Form("nconstituents_jet%d_%s_%s", j+1,label.c_str(),flv.c_str()) , 200 , 0, 200 );
            
            h1_[Form("pt_jet%d_%s_%s"  , j+1,label.c_str(),flv.c_str())] -> GetXaxis() -> SetTitle(Form("Jet %d (%s) p_{T} [GeV]"        , j+1, flv.c_str()));
            h1_[Form("eta_jet%d_%s_%s" , j+1,label.c_str(),flv.c_str())] -> GetXaxis() -> SetTitle(Form("Jet %d (%s)  #eta"              , j+1, flv.c_str()));
            h1_[Form("phi_jet%d_%s_%s" , j+1,label.c_str(),flv.c_str())] -> GetXaxis() -> SetTitle(Form("Jet %d (%s)  #phi"              , j+1, flv.c_str()));
            h1_[Form("btag_jet%d_%s_%s", j+1,label.c_str(),flv.c_str())] -> GetXaxis() -> SetTitle(Form("Jet %d (%s) btag discriminator" , j+1, flv.c_str())); 
            h1_[Form("qglikelihood_jet%d_%s_%s", j+1,label.c_str(),flv.c_str())] -> GetXaxis() -> SetTitle(Form("Jet %d (%s) q-g likelihood" , j+1, flv.c_str())); 
            h1_[Form("nconstituents_jet%d_%s_%s", j+1,label.c_str(),flv.c_str())] -> GetXaxis() -> SetTitle(Form("Jet %d (%s) n constituents" , j+1, flv.c_str())); 
            
            if ( config_->btagAlgorithm() == "deepcsv")
            {
               h1_[Form("btag_light_jet%d_%s_%s", j+1,label.c_str(),flv.c_str())] = std::make_shared<TH1F>(Form("btag_light_jet%d_%s", j+1,flv.c_str()) , Form("btag_light_jet%d_%s_%s", j+1,label.c_str(),flv.c_str()) , nbins_btag, &bins_btag[0] );         
               h1_[Form("btag_c_jet%d_%s_%s"    , j+1,label.c_str(),flv.c_str())] = std::make_shared<TH1F>(Form("btag_c_jet%d_%s"    , j+1,flv.c_str()) , Form("btag_c_jet%d_%s_%s"    , j+1,label.c_str(),flv.c_str()) , nbins_btag, &bins_btag[0] );         
               h1_[Form("btag_b_jet%d_%s_%s"    , j+1,label.c_str(),flv.c_str())] = std::make_shared<TH1F>(Form("btag_b_jet%d_%s"    , j+1,flv.c_str()) , Form("btag_b_jet%d_%s_%s"    , j+1,label.c_str(),flv.c_str()) , nbins_btag, &bins_btag[0] );         
               h1_[Form("btag_bb_jet%d_%s_%s"   , j+1,label.c_str(),flv.c_str())] = std::make_shared<TH1F>(Form("btag_bb_jet%d_%s"   , j+1,flv.c_str()) , Form("btag_bb_jet%d_%s_%s"   , j+1,label.c_str(),flv.c_str()) , nbins_btag, &bins_btag[0] );         
               h1_[Form("btag_cc_jet%d_%s_%s"   , j+1,label.c_str(),flv.c_str())] = std::make_shared<TH1F>(Form("btag_cc_jet%d_%s"   , j+1,flv.c_str()) , Form("btag_cc_jet%d_%s_%s"   , j+1,label.c_str(),flv.c_str()) , nbins_btag, &bins_btag[0] );         
            }
            if ( config_->btagAlgorithm() == "deepflavour" || config_->btagAlgorithm() == "deepjet" )
            {
               h1_[Form("btag_light_jet%d_%s_%s", j+1,label.c_str(),flv.c_str())] = std::make_shared<TH1F>(Form("btag_light_jet%d_%s", j+1,flv.c_str()) , Form("btag_light_jet%d_%s_%s", j+1,label.c_str(),flv.c_str()) , nbins_btag, &bins_btag[0] );         
               h1_[Form("btag_g_jet%d_%s_%s"    , j+1,label.c_str(),flv.c_str())] = std::make_shared<TH1F>(Form("btag_g_jet%d_%s"    , j+1,flv.c_str()) , Form("btag_g_jet%d_%s_%s"    , j+1,label.c_str(),flv.c_str()) , nbins_btag, &bins_btag[0] );         
               h1_[Form("btag_c_jet%d_%s_%s"    , j+1,label.c_str(),flv.c_str())] = std::make_shared<TH1F>(Form("btag_c_jet%d_%s"    , j+1,flv.c_str()) , Form("btag_c_jet%d_%s_%s"    , j+1,label.c_str(),flv.c_str()) , nbins_btag, &bins_btag[0] );         
               h1_[Form("btag_b_jet%d_%s_%s"    , j+1,label.c_str(),flv.c_str())] = std::make_shared<TH1F>(Form("btag_b_jet%d_%s"    , j+1,flv.c_str()) , Form("btag_b_jet%d_%s_%s"    , j+1,label.c_str(),flv.c_str()) , nbins_btag, &bins_btag[0] );         
               h1_[Form("btag_bb_jet%d_%s_%s"   , j+1,label.c_str(),flv.c_str())] = std::make_shared<TH1F>(Form("btag_bb_jet%d_%s"   , j+1,flv.c_str()) , Form("btag_bb_jet%d_%s_%s"   , j+1,label.c_str(),flv.c_str()) , nbins_btag, &bins_btag[0] );         
               h1_[Form("btag_lepb_jet%d_%s_%s" , j+1,label.c_str(),flv.c_str())] = std::make_shared<TH1F>(Form("btag_lepb_jet%d_%s" , j+1,flv.c_str()) , Form("btag_lepb_jet%d_%s_%s" , j+1,label.c_str(),flv.c_str()) , nbins_btag, &bins_btag[0] );         
            }
            // 2D histograms
            h2_[Form("pt_eta_jet%d_%s_%s"  , j+1,label.c_str(),flv.c_str())]  = std::make_shared<TH2F>(Form("pt_eta_jet%d_%s" , j+1,flv.c_str()) , Form("pt_eta_jet%d_%s_%s"  , j+1,label.c_str(),flv.c_str()) ,1500 , 0   , 1500, 600, -3, 3  );
            h2_[Form("pt_eta_jet%d_%s_%s"  , j+1,label.c_str(),flv.c_str())] -> GetXaxis() -> SetTitle(Form("Jet %d (%s) p_{T} [GeV]" , j+1,flv.c_str()));
            h2_[Form("pt_eta_jet%d_%s_%s"  , j+1,label.c_str(),flv.c_str())] -> GetYaxis() -> SetTitle(Form("Jet %d (%s)  #eta" ,j+1,flv.c_str()));

         }
      }
      if ( config_->doDijet()  )  // dijet histograms
      {
         for ( int k = j+1; k < n && j < n; ++k )
         {
            h1_[Form("dptrel_jet%d%d_%s" , j+1,k+1,label.c_str())]  = std::make_shared<TH1F>(Form("dptrel_jet%d%d" , j+1,k+1) , Form("dptrel_jet%d%d_%s" , j+1,k+1,label.c_str()) ,1000 , 0,1 );
            h1_[Form("dpt_jet%d%d_%s"    , j+1,k+1,label.c_str())]  = std::make_shared<TH1F>(Form("dpt_jet%d%d" , j+1,k+1)    , Form("dpt_jet%d%d_%s"    , j+1,k+1,label.c_str()) ,1000 , 0,1000 );
            h1_[Form("dr_jet%d%d_%s"     , j+1,k+1,label.c_str())]  = std::make_shared<TH1F>(Form("dr_jet%d%d"  , j+1,k+1)    , Form("dr_jet%d%d_%s"     , j+1,k+1,label.c_str()) , 100 , 0, 5 );
            h1_[Form("deta_jet%d%d_%s"   , j+1,k+1,label.c_str())]  = std::make_shared<TH1F>(Form("deta_jet%d%d", j+1,k+1)    , Form("deta_jet%d%d_%s"   , j+1,k+1,label.c_str()) , 100 , 0,10 );
            h1_[Form("dphi_jet%d%d_%s"   , j+1,k+1,label.c_str())]  = std::make_shared<TH1F>(Form("dphi_jet%d%d", j+1,k+1)    , Form("dphi_jet%d%d_%s"   , j+1,k+1,label.c_str()) , 315 , 0, 3.15 );
            h1_[Form("pt_jet%d%d_%s"     , j+1,k+1,label.c_str())]  = std::make_shared<TH1F>(Form("pt_jet%d%d"  , j+1,k+1)    , Form("pt_jet%d%d_%s"     , j+1,k+1,label.c_str()) , 300 , 0,3000 );
            h1_[Form("eta_jet%d%d_%s"    , j+1,k+1,label.c_str())]  = std::make_shared<TH1F>(Form("eta_jet%d%d" , j+1,k+1)    , Form("eta_jet%d%d_%s"    , j+1,k+1,label.c_str()) , 200 , -10,10 );
            h1_[Form("phi_jet%d%d_%s"    , j+1,k+1,label.c_str())]  = std::make_shared<TH1F>(Form("phi_jet%d%d" , j+1,k+1)    , Form("phi_jet%d%d_%s"    , j+1,k+1,label.c_str()) , 360 , -180,180 );
            h1_[Form("m_jet%d%d_%s"      , j+1,k+1,label.c_str())]  = std::make_shared<TH1F>(Form("m_jet%d%d"   , j+1,k+1)    , Form("m_jet%d%d_%s"      , j+1,k+1,label.c_str()) ,3000 , 0,3000 );
            
            h1_[Form("dptrel_jet%d%d_%s" , j+1,k+1,label.c_str())] -> GetXaxis() -> SetTitle(Form("#DeltaP_{T}(Jet %d, Jet %d)/Jet %d p_{T}",j+1,k+1,j+1));
            h1_[Form("dpt_jet%d%d_%s"    , j+1,k+1,label.c_str())] -> GetXaxis() -> SetTitle(Form("#Delta p_{T}(Jet %d, Jet %d) [GeV]",j+1,k+1));
            h1_[Form("dr_jet%d%d_%s"     , j+1,k+1,label.c_str())] -> GetXaxis() -> SetTitle(Form("#DeltaR(Jet %d, Jet %d)",j+1,k+1));
            h1_[Form("deta_jet%d%d_%s"   , j+1,k+1,label.c_str())] -> GetXaxis() -> SetTitle(Form("#Delta#eta(Jet %d, Jet %d)",j+1,k+1));
            h1_[Form("dphi_jet%d%d_%s"   , j+1,k+1,label.c_str())] -> GetXaxis() -> SetTitle(Form("#Delta#phi(Jet %d, Jet %d)",j+1,k+1));
            h1_[Form("pt_jet%d%d_%s"     , j+1,k+1,label.c_str())] -> GetXaxis() -> SetTitle(Form("Jet %d + Jet %d p_{T} [GeV]",j+1,k+1));
            h1_[Form("eta_jet%d%d_%s"    , j+1,k+1,label.c_str())] -> GetXaxis() -> SetTitle(Form("Jet %d + Jet %d  #eta",j+1,k+1));
            h1_[Form("phi_jet%d%d_%s"    , j+1,k+1,label.c_str())] -> GetXaxis() -> SetTitle(Form("Jet %d + Jet %d  #phi",j+1,k+1));
            h1_[Form("m_jet%d%d_%s"      , j+1,k+1,label.c_str())] -> GetXaxis() -> SetTitle(Form("M_{%d%d} [GeV]",j+1,k+1));
            
            if ( config_->isMC() && config_->histogramJetsPerFlavour() )
            {
               for ( auto & flv1 : flavours_ )
               {
                  for ( auto & flv2 : flavours_ )
                  {
                     h1_[Form("dptrel_jet%d%d_%s_%s_%s" , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  = std::make_shared<TH1F>(Form("dptrel_jet%d%d_%s_%s" , j+1,k+1,flv1.c_str(),flv2.c_str()) , Form("dptrel_jet%d%d_%s_%s_%s" , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str()) , 100 , 0,1 );
                     h1_[Form("dpt_jet%d%d_%s_%s_%s"    , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  = std::make_shared<TH1F>(Form("dpt_jet%d%d_%s_%s"    , j+1,k+1,flv1.c_str(),flv2.c_str()) , Form("dpt_jet%d%d_%s_%s_%s"    , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str()) , 100 , 0,1000 );
                     h1_[Form("dr_jet%d%d_%s_%s_%s"     , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  = std::make_shared<TH1F>(Form("dr_jet%d%d_%s_%s"     , j+1,k+1,flv1.c_str(),flv2.c_str()) , Form("dr_jet%d%d_%s_%s_%s"     , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str()) , 100 , 0, 5 );
                     h1_[Form("deta_jet%d%d_%s_%s_%s"   , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  = std::make_shared<TH1F>(Form("deta_jet%d%d_%s_%s"   , j+1,k+1,flv1.c_str(),flv2.c_str()) , Form("deta_jet%d%d_%s_%s_%s"   , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str()) , 100 , 0,10 );
                     h1_[Form("dphi_jet%d%d_%s_%s_%s"   , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  = std::make_shared<TH1F>(Form("dphi_jet%d%d_%s_%s"   , j+1,k+1,flv1.c_str(),flv2.c_str()) , Form("dphi_jet%d%d_%s_%s_%s"   , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str()) , 315 , 0, 3.15 );
                     h1_[Form("pt_jet%d%d_%s_%s_%s"     , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  = std::make_shared<TH1F>(Form("pt_jet%d%d_%s_%s"     , j+1,k+1,flv1.c_str(),flv2.c_str()) , Form("pt_jet%d%d_%s_%s_%s"     , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str()) , 300 , 0,300 );
                     h1_[Form("eta_jet%d%d_%s_%s_%s"    , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  = std::make_shared<TH1F>(Form("eta_jet%d%d_%s_%s"    , j+1,k+1,flv1.c_str(),flv2.c_str()) , Form("eta_jet%d%d_%s_%s_%s"    , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str()) , 200 , -10,10 );
                     h1_[Form("phi_jet%d%d_%s_%s_%s"    , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  = std::make_shared<TH1F>(Form("phi_jet%d%d_%s_%s"    , j+1,k+1,flv1.c_str(),flv2.c_str()) , Form("phi_jet%d%d_%s_%s_%s"    , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str()) , 360 , -180,180 );
                     h1_[Form("m_jet%d%d_%s_%s_%s"      , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  = std::make_shared<TH1F>(Form("m_jet%d%d_%s_%s"      , j+1,k+1,flv1.c_str(),flv2.c_str()) , Form("m_jet%d%d_%s_%s_%s"      , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str()) ,3000 , 0,300 );
                     
                     h1_[Form("dptrel_jet%d%d_%s_%s_%s" , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  -> GetXaxis() -> SetTitle(Form("#DeltaP_{T}(Jet %d (%s), Jet %d (%s))/Jet %d p_{T}",j+1,flv1.c_str(),k+1,flv2.c_str(),j+1));
                     h1_[Form("dpt_jet%d%d_%s_%s_%s"    , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  -> GetXaxis() -> SetTitle(Form("#Delta p_{T}(Jet %d (%s), Jet %d (%s)) [GeV]",j+1,flv1.c_str(),k+1,flv2.c_str()));
                     h1_[Form("dr_jet%d%d_%s_%s_%s"     , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  -> GetXaxis() -> SetTitle(Form("#DeltaR(Jet %d (%s), Jet %d (%s))",j+1,flv1.c_str(),k+1,flv2.c_str()));
                     h1_[Form("deta_jet%d%d_%s_%s_%s"   , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  -> GetXaxis() -> SetTitle(Form("#Delta#eta(Jet %d (%s), Jet %d (%s))",j+1,flv1.c_str(),k+1,flv2.c_str()));
                     h1_[Form("dphi_jet%d%d_%s_%s_%s"   , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  -> GetXaxis() -> SetTitle(Form("#Delta#phi(Jet %d (%s), Jet %d (%s))",j+1,flv1.c_str(),k+1,flv2.c_str()));
                     h1_[Form("pt_jet%d%d_%s_%s_%s"     , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  -> GetXaxis() -> SetTitle(Form("Jet %d (%s) + Jet %d (%s) p_{T} [GeV]",j+1,flv1.c_str(),k+1,flv2.c_str()));
                     h1_[Form("eta_jet%d%d_%s_%s_%s"    , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  -> GetXaxis() -> SetTitle(Form("Jet %d (%s) + Jet %d (%s)  #eta",j+1,flv1.c_str(),k+1,flv2.c_str()));
                     h1_[Form("phi_jet%d%d_%s_%s_%s"    , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  -> GetXaxis() -> SetTitle(Form("Jet %d (%s) + Jet %d (%s)  #phi",j+1,flv1.c_str(),k+1,flv2.c_str()));
                     h1_[Form("m_jet%d%d_%s_%s_%s"      , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  -> GetXaxis() -> SetTitle(Form("M_{%d%d} (%s)(%s) [GeV]",j+1,k+1,flv1.c_str(),flv2.c_str()));                                          
                  }
               }
            }
         }
      }
   }
   
   this->output()->cd();
}


float JetAnalyser::btag(const Jet & jet, const std::string & algo)
{
   float btag;
   if ( config_->btagAlgorithm() == "csvivf" || config_->btagAlgorithm() == "csv" )   {      btag = jet.btag("btag_csvivf");   }
   else if ( config_->btagAlgorithm() == "deepcsv" )                            {      btag = jet.btag("btag_deepb") + jet.btag("btag_deepbb");   }
   else if ( config_->btagAlgorithm() == "deepbvsall" )                         {      btag = jet.btag("btag_deepbvsall");   }
   else if ( config_->btagAlgorithm() == "deepflavour" || config_->btagAlgorithm() == "deepjet" ) {      btag = jet.btag("btag_dfb") + jet.btag("btag_dfbb") + jet.btag("btag_dflepb");   }
   else                                                                   {      btag = -9999;   }
   
   return btag;
}

bool JetAnalyser::selectionJet(const int & r)
{
   if ( r > config_->nJetsMin() ) return true;
   int j = r-1;
   
   float pt_min = config_->jetsPtMin()[j];
   float pt_max = -1.;
   float eta_max = config_->jetsEtaMax()[j];
   
   if ( config_->jetsPtMax().size() > 0 && config_->jetsPtMax()[j] > config_->jetsPtMin()[j] ) pt_max = config_->jetsPtMax()[j];
   
   bool isgood = this -> selectionJet(r,pt_min,eta_max,pt_max);
   
   return isgood;
}

bool JetAnalyser::selectionJet(const int & r, const float & pt_min, const float &eta_max, const float &pt_max)
{
   if ( r > config_->nJetsMin() ) return true;
   ++cutflow_;
   bool isgood = true;
   int j = r-1;
   
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" )
   {
      if ( pt_max > pt_min )
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d: pt > %5.1f GeV and pt < %5.1f GeV and |eta| < %3.1f",r,pt_min, pt_max, eta_max ));
      else
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d: pt > %5.1f GeV and |eta| < %3.1f",r,pt_min, eta_max ));
   }
   
   if ( (int)selectedJets_.size() < r ) return false;
   
   // kinematic selection
   if ( selectedJets_[j] -> pt() < pt_min           && !(pt_min < 0) ) return false;
   if ( fabs(selectedJets_[j] -> eta()) > eta_max   && !(eta_max < 0) ) return false;
   if ( config_->jetsPtMax().size() > 0 )
   {
      if ( selectedJets_[j] -> pt() > pt_max && !(pt_max < pt_min ) )  return false;
   }
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   return isgood;
}


bool JetAnalyser::selectionJetDeta(const int & r1, const int & r2, const float & delta)
{
   if ( r1 > config_->nJetsMin() ||  r2 > config_->nJetsMin() ) return true;
   
   ++cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" )
   {
      if ( delta > 0 )
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Deta(jet %d, jet %d) < %4.2f",r1,r2,fabs(delta)));
      else
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Deta(jet %d, jet %d) > %4.2f",r1,r2,fabs(delta)));
   }
   
   
   int j1 = r1-1;
   int j2 = r2-1;
   
   if ( delta > 0 )
   {
      if ( fabs(selectedJets_[j1]->eta() - selectedJets_[j2]->eta()) > fabs(delta) ) return false;
   }
   else
   {
      if ( fabs(selectedJets_[j1]->eta() - selectedJets_[j2]->eta()) < fabs(delta) ) return false;
   }

        
   h1_["cutflow"] -> Fill(cutflow_,weight_);
    
   return true;
   
}
bool JetAnalyser::selectionJetDeta(const int & r1, const int & r2)
{
   bool ok = true;
   if (config_->jetsDetaMax() < 0 )
   {
      ok = ok && true;
   }
   else
   {
      ok = ok && selectionJetDeta(r1,r2,config_->jetsDetaMax());
   }
   
   if (config_->jetsDetaMin() < 0 )
   {
      ok = ok && true;
   }
   else
   {
      ok = ok && selectionJetDeta(r1,r2,-1*config_->jetsDetaMin());
   }
   return ok;
   
}

bool JetAnalyser::selectionJetDphi(const int & r1, const int & r2, const float & delta)
{
   if ( r1 > config_->nJetsMin() ||  r2 > config_->nJetsMin() ) return true;
   
   ++cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" )
   {
      if ( delta > 0 )
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Dphi(jet %d, jet %d) < %4.2f",r1,r2,fabs(delta)));
      else
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Dphi(jet %d, jet %d) > %4.2f",r1,r2,fabs(delta)));
   }
   
   int j1 = r1-1;
   int j2 = r2-1;
   
   if ( delta > 0 )
   {
      if ( fabs(selectedJets_[j1]->deltaPhi(*selectedJets_[j2])) > fabs(delta) ) return false;
   }
   else
   {
      if ( fabs(selectedJets_[j1]->deltaPhi(*selectedJets_[j2])) < fabs(delta) ) return false;
   }
        
   h1_["cutflow"] -> Fill(cutflow_,weight_);
    
   return true;
   
}
bool JetAnalyser::selectionJetDphi(const int & r1, const int & r2)
{
   bool ok = true;
   if (config_->jetsDphiMax() < 0 )
   {
      ok = ok && true;
   }
   else
   {
      ok = ok && selectionJetDphi(r1,r2,config_->jetsDphiMax());
   }
   
   if (config_->jetsDphiMin() < 0 )
   {
      ok = ok && true;
   }
   else
   {
      ok = ok && selectionJetDphi(r1,r2,-1*config_->jetsDphiMin());
   }
   return ok;
   
}


bool JetAnalyser::selectionJetDr(const int & r1, const int & r2, const float & delta)
{
   if ( r1 > config_->nJetsMin() ||  r2 > config_->nJetsMin() ) return true;
   
   ++cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" )
   {
      if ( delta > 0 ) 
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("DR(jet %d, jet %d) < %4.2f",r1,r2,fabs(delta)));
      else
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("DR(jet %d, jet %d) > %4.2f",r1,r2,fabs(delta)));
   }
   
   int j1 = r1-1;
   int j2 = r2-1;
   
   if ( delta > 0 )
   {
      if ( selectedJets_[j1]->deltaR(*selectedJets_[j2]) > fabs(delta) ) return false;
   }
   else
   {
      if ( selectedJets_[j1]->deltaR(*selectedJets_[j2]) < fabs(delta) ) return false;
   }

      
   h1_["cutflow"] -> Fill(cutflow_,weight_);
    
   return true;
   
}

bool JetAnalyser::selectionJetDr(const int & r1, const int & r2)
{
   bool ok = true;
   if (config_->jetsDrMax() < 0 )
   {
      ok = ok && true;
   }
   else
   {
      ok = ok && selectionJetDr(r1,r2,config_->jetsDrMax());
   }
   
   if (config_->jetsDrMin() < 0 )
   {
      ok = ok && true;
   }
   else
   {
      ok = ok && selectionJetDr(r1,r2,-1*config_->jetsDrMin());
   }
   return ok;
}

bool JetAnalyser::selectionJetPtImbalance(const int & r1, const int & r2, const float & delta)
{
   if ( r1 > config_->nJetsMin() ||  r2 > config_->nJetsMin() ) return true;
   
   ++cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" )
   {
      if ( delta > 0 )
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("DpT(jet %d, jet %d)/jet %d pT < %4.2f",r1,r2,r1,fabs(delta)));
      else
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("DpT(jet %d, jet %d)/jet %d pT > %4.2f",r1,r2,r1,fabs(delta)));
   }
   
   int j1 = r1-1;
   int j2 = r2-1;
   
   if ( delta > 0 )
   {
      if ( fabs(selectedJets_[j1]->pt() - selectedJets_[j2]->pt())/selectedJets_[j1]->pt() > fabs(delta) ) return false;
   }
   else
   {
      if ( fabs(selectedJets_[j1]->pt() - selectedJets_[j2]->pt())/selectedJets_[j1]->pt() < fabs(delta) ) return false;
   }

        
   h1_["cutflow"] -> Fill(cutflow_,weight_);
    
   return true;
   
}
bool JetAnalyser::selectionJetPtImbalance(const int & r1, const int & r2)
{
   bool ok = true;
   if (config_->jetsPtImbalanceMax() < 0 )
   {
      ok = ok && true;
   }
   else
   {
      ok = ok && selectionJetPtImbalance(r1,r2,config_->jetsPtImbalanceMax());
   }
   
   if (config_->jetsPtImbalanceMin() < 0 )
   {
      ok = ok && true;
   }
   else
   {
      ok = ok && selectionJetPtImbalance(r1,r2,-1*config_->jetsPtImbalanceMin());
   }
   return ok;
   
}


std::vector< std::shared_ptr<Jet> > JetAnalyser::jets()
{
   return jets_;
}

std::vector< std::shared_ptr<Jet> > JetAnalyser::selectedJets()
{
   return selectedJets_;
}


bool JetAnalyser::selectionJetId()
{
   if ( ! jetsanalysis_ ) return false;
   
   ++cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
      h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("JetId: %s",config_->jetsId().c_str()));
   
   auto jet = std::begin(selectedJets_);
   while ( jet != std::end(selectedJets_) )
   {
      if ( ! (*jet)->id(config_->jetsId() ) )
         jet = selectedJets_.erase(jet);
      else
         ++jet;
   }
   if ( selectedJets_.size() == 0 ) return false;
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   return true;
}

bool JetAnalyser::selectionJetPileupId()
{
   ++cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
      h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("JetPileupId: %s",config_->jetsPuId().c_str()));
   
   
   if ( ! jetsanalysis_ ) return false;
   
   auto jet = std::begin(selectedJets_);
   
   while ( jet != std::end(selectedJets_) )
   {
      if ( ! (*jet)->pileupJetIdFullId(config_->jetsPuId()) )
         jet = selectedJets_.erase(jet);
      else
         ++jet;
   }
   
   if ( selectedJets_.size() == 0 ) return false;
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   return true;

}

bool JetAnalyser::selectionNJets()
{
   if ( config_->nJetsMin() < 0 ) return true;
   
   ++cutflow_;
   
   bool first = ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" );
   
   if ( config_->nJetsMax() <= 0 )
   {
      if ( first )  h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("NJets >= %d",config_->nJetsMin()));
      if ((int)selectedJets_.size() < config_->nJetsMin()) return false;
   }
   else if ( config_->nJets() >= 0 )
   {
      if ( first )  h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("NJets = %d",config_->nJets()));
      if ((int)selectedJets_.size() != config_->nJets()) return false;
   }
   else
   {
      if ( config_->nJetsMin() == 0 )
      {
         if ( first )  h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("NJets <= %d",config_->nJetsMax()));
         if ((int)selectedJets_.size() > config_->nJetsMax()) return false;
      }
      else
      {
         if ( first )  h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("%d <= NJets <= %d",config_->nJetsMin(),config_->nJetsMax()));
         if  ((int)selectedJets_.size() < config_->nJetsMin() || (int)selectedJets_.size() > config_->nJetsMax()) return false;
      }
   }
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   return true;
   
}


bool JetAnalyser::selectionBJet(const int & r )
{
   if ( config_->nJetsMin() < config_->nBJetsMin() || config_->nBJetsMin() < 1 || r > config_->nBJetsMin() ||  (int)(config_->jetsBtagWP()).size() < config_->nBJetsMin() ) return true;
   
   if ( ! config_->signalRegion() && r == config_->revBtagJet() ) return this->selectionNonBJet(r);
      
   int j = r-1;
   
   if ( config_->btagWP(config_->jetsBtagWP()[j]) < 0 ) return true; // there is no selection here, so will not update the cutflow
   
   ++ cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
      h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d: %s btag > %6.4f (%s)",r,config_->btagAlgorithm().c_str(),config_->btagWP(config_->jetsBtagWP()[j]),config_->jetsBtagWP()[j].c_str()));
   
   if ( btag(*selectedJets_[j],config_->btagAlgorithm()) < config_->btagWP(config_->jetsBtagWP()[j]) ) return false;
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   return true;
}


bool JetAnalyser::selectionNonBJet(const int & r )
{
   int j = r-1;

   if ( config_->btagWP(config_->revBtagWP()) < 0 ) return true; // there is no selection here, so will not update the cutflow

   ++ cutflow_;
   
   if ( r > config_->nBJetsMin() ) 
   {
      std::cout << "* warning * -  JetAnalyser::selectionBJet(): given jet rank > nbjetsmin. Returning false! " << std::endl;
      return false;
   }
  
   // jet  non btag
   if ( btag(*selectedJets_[j],config_->btagAlgorithm()) > config_->btagWP(config_->revBtagWP()) ) return false;
   
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
      h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d: %s btag < %6.4f (%s) [reverse btag]",r,config_->btagAlgorithm().c_str(),config_->btagWP(config_->revBtagWP()),config_->revBtagWP().c_str()));
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   return true;
}


bool JetAnalyser::onlineJetMatching(const int & r)
{
   int j = r-1;
   if ( config_->triggerObjectsL1Jets() == "" && config_->triggerObjectsCaloJets() == "" && config_->triggerObjectsPFJets() == "") return true;
   if ( config_->nJetsMin() < 0 ) return true;
   
   ++cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
      h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d: online jet match",r));
      
   
   if ( r > config_->nJetsMin() )
   {
      std::cout << "*Warning* JetAnalyser::onlineJetMatching(): asking for matching of unselected jet. Returning false!" << std::endl;
      return false;  // asking for a match beyond the selection, that's wrong, therefore false
   }
   if ( selectedJets_.size() == 0 )
   {
      std::cout << "*Warning* JetAnalyser::onlineJetMatching(): selectedJets is empty. Returning false!" << std::endl;
      return false;  // asking for a match beyond the selection, that's wrong, therefore false
   }
   
   std::shared_ptr<Jet> jet = selectedJets_[j];
   if ( ! jet->matched(config_->triggerObjectsL1Jets()   ) ) return false;
   if ( ! jet->matched(config_->triggerObjectsCaloJets() ) ) return false;
   if ( ! jet->matched(config_->triggerObjectsPFJets()   ) ) return false;

   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   return true;
}


bool JetAnalyser::onlineBJetMatching(const int & r)
{
   int j = r-1;
   if ( config_->triggerObjectsBJets() == "" ) return true;
   
   ++cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
      h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d: online b jet match",r));
   
   
   if ( r > config_->nJetsMin() )
   {
      std::cout << "*Warning* JetAnalyser::onlineBJetMatching(): asking for matching of unselected jet. Returning false!" << std::endl;
      return false;  // asking for a match beyond the selection, that's wrong, therefore false
   }
   if ( selectedJets_.size() == 0 )
   {
      std::cout << "*Warning* JetAnalyser::onlineBJetMatching(): selectedJets is empty. You must run selectionJetId() before. Returning false!" << std::endl;
      return false;  // asking for a match beyond the selection, that's wrong, therefore false
   }
   
   std::shared_ptr<Jet> jet = selectedJets_[j];
   if ( ! jet->matched(config_->triggerObjectsBJets()) ) return false;
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   return true;
}

void JetAnalyser::fillJetHistograms(const std::string & label)
{
   this->output()->cd();
   ++ cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
      h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("*** Filling jets histograms - %s",label.c_str()));
   
   this->output()->cd(label.c_str());
   
   int n = n_hjets_;
   
   if ( n > config_->nJetsMin() ) n = config_->nJetsMin();
   
   h1_[Form("jet_hist_weight_%s",label.c_str())] -> Fill(0.,weight_);
   
   for ( int j = 0; j < n; ++j )
   {
      // fill each jet with addtional SF to weight = 1
      this->fillJetHistograms(j+1,label,1.);
      
      if ( config_ -> doDijet() )
      {
         for ( int k = j+1; k < n && j < n; ++k )
         {
            Composite<Jet,Jet> c_ij(*(selectedJets_[j]),*(selectedJets_[k]));
            
            h1_[Form("dptrel_jet%d%d_%s" , j+1,k+1,label.c_str())] -> Fill(fabs(selectedJets_[j]->pt()-selectedJets_[k]->pt())/selectedJets_[j]->pt(),weight_);
            h1_[Form("dpt_jet%d%d_%s" , j+1,k+1,label.c_str())]    -> Fill(fabs(selectedJets_[j]->pt()-selectedJets_[k]->pt()),weight_);
            h1_[Form("dr_jet%d%d_%s",j+1,k+1,label.c_str())]       -> Fill(c_ij.deltaR(),weight_);
            h1_[Form("deta_jet%d%d_%s",j+1,k+1,label.c_str())]     -> Fill(c_ij.deltaEta(),weight_);
            h1_[Form("dphi_jet%d%d_%s",j+1,k+1,label.c_str())]     -> Fill(fabs(selectedJets_[j]->deltaPhi(*selectedJets_[k])));
            h1_[Form("pt_jet%d%d_%s",j+1,k+1,label.c_str())]       -> Fill(c_ij.pt(),weight_);
            h1_[Form("eta_jet%d%d_%s",j+1,k+1,label.c_str())]      -> Fill(c_ij.eta(),weight_);
            h1_[Form("phi_jet%d%d_%s",j+1,k+1,label.c_str())]      -> Fill(c_ij.phi()*180./acos(-1.),weight_);
            if ( config_->isMC() || !config_->signalRegion() )
            {
               h1_[Form("m_jet%d%d_%s",j+1,k+1,label.c_str())]  -> Fill(c_ij.m(),weight_);
            }
            else  // blind
            {
               h1_[Form("m_jet%d%d_%s",j+1,k+1,label.c_str())]  -> Fill(0.,weight_);
            }
            if ( config_->isMC() && config_->histogramJetsPerFlavour() )
            {
               std::string flv1 = "udsg";
               std::string flv2 = "udsg";
               if ( ! config_ -> useJetsExtendedFlavour() )
               {
                  if ( abs(selectedJets_[j]->flavour()) == 4 ) flv1 = "c"; 
                  if ( abs(selectedJets_[j]->flavour()) == 5 ) flv1 = "b"; 
                  if ( abs(selectedJets_[k]->flavour()) == 4 ) flv2 = "c"; 
                  if ( abs(selectedJets_[k]->flavour()) == 5 ) flv2 = "b"; 
               }
               else
               {
                  flv1 = selectedJets_[j]->extendedFlavour();
                  flv2 = selectedJets_[k]->extendedFlavour();
               }
               h1_[Form("dptrel_jet%d%d_%s_%s_%s" , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  -> Fill(fabs(selectedJets_[j]->pt()-selectedJets_[k]->pt())/selectedJets_[j]->pt(),weight_);
               h1_[Form("dpt_jet%d%d_%s_%s_%s"    , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  -> Fill(fabs(selectedJets_[j]->pt()-selectedJets_[k]->pt()),weight_);
               h1_[Form("dr_jet%d%d_%s_%s_%s"     , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  -> Fill(c_ij.deltaR(),weight_);
               h1_[Form("deta_jet%d%d_%s_%s_%s"   , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  -> Fill(c_ij.deltaEta(),weight_);
               h1_[Form("dphi_jet%d%d_%s_%s_%s"   , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  -> Fill(fabs(selectedJets_[j]->deltaPhi(*selectedJets_[k])));
               h1_[Form("pt_jet%d%d_%s_%s_%s"     , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  -> Fill(c_ij.pt(),weight_);
               h1_[Form("eta_jet%d%d_%s_%s_%s"    , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  -> Fill(c_ij.eta(),weight_);
               h1_[Form("phi_jet%d%d_%s_%s_%s"    , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  -> Fill(c_ij.phi()*180./acos(-1.),weight_);
               h1_[Form("m_jet%d%d_%s_%s_%s"      , j+1,k+1,label.c_str(),flv1.c_str(),flv2.c_str())]  -> Fill(c_ij.m(),weight_);

            }
         }
      }
   }
   this->output()->cd();
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
}

void JetAnalyser::fillJetHistograms(const int & r, const std::string & label, const float & sf, const bool & workflow)
{
   if ( r < 1 || r > n_hjets_ ) return;
   
   if ( workflow ) // BE CAREFUL with this
   {
      this->output()->cd();
      ++ cutflow_;
      if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("*** Filling jet # %d histograms - %s",r, label.c_str()));
      this->output()->cd(label.c_str());
   }
   
   int j = r-1;
   
   // 1D histograms
   h1_[Form("pt_jet%d_%s",j+1,label.c_str())]   -> Fill(selectedJets_[j]->pt(),weight_*sf);
   // barrel and endcap pt distributions
   float eta = selectedJets_[j]->eta();
   if ( config_-> histogramJetsRegionSplit() )
   {
      if ( eta < -1.4 )               h1_[Form("pt_jet%d_me_%s"  , j+1,label.c_str())] -> Fill(selectedJets_[j]->pt(),weight_*sf);
      if ( eta >  1.4 )               h1_[Form("pt_jet%d_pe_%s"  , j+1,label.c_str())] -> Fill(selectedJets_[j]->pt(),weight_*sf);
      if ( eta <  0.0 && eta > -1.0 ) h1_[Form("pt_jet%d_mb_%s"  , j+1,label.c_str())] -> Fill(selectedJets_[j]->pt(),weight_*sf);
      if ( eta >  0.0 && eta <  1.0 ) h1_[Form("pt_jet%d_pb_%s"  , j+1,label.c_str())] -> Fill(selectedJets_[j]->pt(),weight_*sf);
      if ( eta < -1.0 && eta > -1.4 ) h1_[Form("pt_jet%d_mbe_%s" , j+1,label.c_str())] -> Fill(selectedJets_[j]->pt(),weight_*sf);
      if ( eta >  1.0 && eta <  1.4 ) h1_[Form("pt_jet%d_pbe_%s" , j+1,label.c_str())] -> Fill(selectedJets_[j]->pt(),weight_*sf);
   }
      
   //
   h1_[Form("eta_jet%d_%s",j+1,label.c_str())]  -> Fill(selectedJets_[j]->eta(),weight_*sf);
   h1_[Form("phi_jet%d_%s",j+1,label.c_str())]  -> Fill(selectedJets_[j]->phi()*180./acos(-1.),weight_*sf);
   float mybtag = btag(*selectedJets_[j],config_->btagAlgorithm());
   float mybtaglog = 1.e-7;
   if ( mybtag > 0 ) mybtaglog = -log(1.-mybtag);
   h1_[Form("btag_jet%d_%s",j+1,label.c_str())]    -> Fill(mybtag,weight_*sf);
   h1_[Form("btaglog_jet%d_%s",j+1,label.c_str())] -> Fill(mybtaglog,weight_*sf);
   h1_[Form("qglikelihood_jet%d_%s", j+1,label.c_str())] -> Fill(selectedJets_[j]->qgLikelihood(),weight_*sf);
   h1_[Form("nconstituents_jet%d_%s", j+1,label.c_str())] -> Fill(selectedJets_[j]->constituents(),weight_*sf);
      
   if ( config_->btagAlgorithm() == "deepcsv")
   {
      h1_[Form("btag_light_jet%d_%s", j+1,label.c_str())]  -> Fill(selectedJets_[j]->btag("btag_deeplight"),weight_*sf);
      h1_[Form("btag_c_jet%d_%s"    , j+1,label.c_str())]  -> Fill(selectedJets_[j]->btag("btag_deepc"),weight_*sf); 
      h1_[Form("btag_b_jet%d_%s"    , j+1,label.c_str())]  -> Fill(selectedJets_[j]->btag("btag_deepb"),weight_*sf); 
      h1_[Form("btag_bb_jet%d_%s"   , j+1,label.c_str())]  -> Fill(selectedJets_[j]->btag("btag_deepbb"),weight_*sf); 
      h1_[Form("btag_cc_jet%d_%s"   , j+1,label.c_str())]  -> Fill(selectedJets_[j]->btag("btag_deepcc"),weight_*sf); 
   }
   if ( config_->btagAlgorithm() == "deepflavour" || config_->btagAlgorithm() == "deepjet" )
   {
      h1_[Form("btag_light_jet%d_%s", j+1,label.c_str())]  -> Fill(selectedJets_[j]->btag("btag_dflight"),weight_*sf);
      h1_[Form("btag_g_jet%d_%s"    , j+1,label.c_str())]  -> Fill(selectedJets_[j]->btag("btag_dfg")    ,weight_*sf); 
      h1_[Form("btag_c_jet%d_%s"    , j+1,label.c_str())]  -> Fill(selectedJets_[j]->btag("btag_dfc")    ,weight_*sf); 
      h1_[Form("btag_b_jet%d_%s"    , j+1,label.c_str())]  -> Fill(selectedJets_[j]->btag("btag_dfb")    ,weight_*sf); 
      h1_[Form("btag_bb_jet%d_%s"   , j+1,label.c_str())]  -> Fill(selectedJets_[j]->btag("btag_dfbb")   ,weight_*sf); 
      h1_[Form("btag_lepb_jet%d_%s" , j+1,label.c_str())]  -> Fill(selectedJets_[j]->btag("btag_dflepb") ,weight_*sf); 
   }
   // 2D histograms
   h2_[Form("pt_eta_jet%d_%s"  , j+1,label.c_str())] -> Fill(selectedJets_[j]->pt(), selectedJets_[j]->eta(), weight_*sf);
      
      
   if ( config_->isMC() && config_->histogramJetsPerFlavour() )
   {
      std::string flv = "udsg";
      if ( ! config_ -> useJetsExtendedFlavour() )
      {
         if ( abs(selectedJets_[j]->flavour()) == 4 ) flv = "c"; 
         if ( abs(selectedJets_[j]->flavour()) == 5 ) flv = "b"; 
      }
      else
      {
         flv = selectedJets_[j]->extendedFlavour();
      }
      // 1D histograms
      h1_[Form("pt_jet%d_%s_%s"  , j+1,label.c_str(),flv.c_str())]  -> Fill(selectedJets_[j]->pt(),weight_*sf);
      h1_[Form("eta_jet%d_%s_%s" , j+1,label.c_str(),flv.c_str())]  -> Fill(selectedJets_[j]->eta(),weight_*sf);
      h1_[Form("phi_jet%d_%s_%s" , j+1,label.c_str(),flv.c_str())]  -> Fill(selectedJets_[j]->phi()*180./acos(-1.),weight_*sf);
      h1_[Form("btag_jet%d_%s_%s", j+1,label.c_str(),flv.c_str())]  -> Fill(btag(*selectedJets_[j],config_->btagAlgorithm()),weight_*sf);
      h1_[Form("qglikelihood_jet%d_%s_%s", j+1,label.c_str(),flv.c_str())] -> Fill(selectedJets_[j]->qgLikelihood(),weight_*sf);
      h1_[Form("nconstituents_jet%d_%s_%s", j+1,label.c_str(),flv.c_str())] -> Fill(selectedJets_[j]->constituents(),weight_*sf);
      if ( config_->btagAlgorithm() == "deepcsv")
      {
         h1_[Form("btag_light_jet%d_%s_%s", j+1,label.c_str(),flv.c_str())]  -> Fill(selectedJets_[j]->btag("btag_deeplight"),weight_*sf);
         h1_[Form("btag_c_jet%d_%s_%s"    , j+1,label.c_str(),flv.c_str())]  -> Fill(selectedJets_[j]->btag("btag_deepc"),weight_*sf); 
         h1_[Form("btag_b_jet%d_%s_%s"    , j+1,label.c_str(),flv.c_str())]  -> Fill(selectedJets_[j]->btag("btag_deepb"),weight_*sf); 
         h1_[Form("btag_bb_jet%d_%s_%s"   , j+1,label.c_str(),flv.c_str())]  -> Fill(selectedJets_[j]->btag("btag_deepbb"),weight_*sf); 
         h1_[Form("btag_cc_jet%d_%s_%s"   , j+1,label.c_str(),flv.c_str())]  -> Fill(selectedJets_[j]->btag("btag_deepcc"),weight_*sf); 
      }
      if ( config_->btagAlgorithm() == "deepflavour" || config_->btagAlgorithm() == "deepjet")
      {
         h1_[Form("btag_light_jet%d_%s_%s", j+1,label.c_str(),flv.c_str())]  -> Fill(selectedJets_[j]->btag("btag_dflight"),weight_*sf);
         h1_[Form("btag_g_jet%d_%s_%s"    , j+1,label.c_str(),flv.c_str())]  -> Fill(selectedJets_[j]->btag("btag_dfg")    ,weight_*sf); 
         h1_[Form("btag_c_jet%d_%s_%s"    , j+1,label.c_str(),flv.c_str())]  -> Fill(selectedJets_[j]->btag("btag_dfc")    ,weight_*sf); 
         h1_[Form("btag_b_jet%d_%s_%s"    , j+1,label.c_str(),flv.c_str())]  -> Fill(selectedJets_[j]->btag("btag_dfb")    ,weight_*sf); 
         h1_[Form("btag_bb_jet%d_%s_%s"   , j+1,label.c_str(),flv.c_str())]  -> Fill(selectedJets_[j]->btag("btag_dfbb")   ,weight_*sf); 
         h1_[Form("btag_lepb_jet%d_%s_%s" , j+1,label.c_str(),flv.c_str())]  -> Fill(selectedJets_[j]->btag("btag_dflepb") ,weight_*sf); 
         
         
      }
      // 2D histograms
      h2_[Form("pt_eta_jet%d_%s_%s"  , j+1,label.c_str(),flv.c_str())] -> Fill(selectedJets_[j]->pt(), selectedJets_[j]->eta(), weight_*sf);
   }
      
   this->output()->cd();
   if ( workflow ) h1_["cutflow"] -> Fill(cutflow_,weight_*sf);
   
}



ScaleFactors JetAnalyser::btagSF(const int & r, const std::string & wp)
{
   ScaleFactors sf;
   int j = r-1;
   sf.nominal = selectedJets_[j]->btagSF(bsf_reader_[wp]);
   sf.up      = selectedJets_[j]->btagSFup(bsf_reader_[wp]);
   sf.down    = selectedJets_[j]->btagSFdown(bsf_reader_[wp]);
   
   return sf;
}


void JetAnalyser::actionApplyJER()
{
   if ( ! jetsanalysis_ ||  ! isMC_ ) return;
   ++cutflow_;
   if ( applyjer_ )
   {
      if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
      {
         std::string bnpt = basename(config_->jerPtRes());
         std::string bnsf = basename(config_->jerSF());
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("JER smearing (%s,%s)",bnpt.c_str(),bnsf.c_str()));
      }
      for ( auto & j : selectedJets_ )
      {
         j -> applyJER(*jerinfo_,0.2);
      }
   }
   else
   {
      if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
      {
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,"WARNING: NO JER smearing (*** missing JER Info and/or GenJet collection ***)");
      }
   }
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);
}

float JetAnalyser::actionApplyBtagSF(const int & r, const bool & global_weight)
{
   float sf = 1.;
   if ( ! config_-> isMC() || config_->btagScaleFactors() == "" ) return sf;  // will not apply btag SF
   if ( ! config_->signalRegion() && r == config_->revBtagJet() ) return sf;
   
   int j = r-1;
   ++ cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" )
   {
      if ( config_->jetsBtagWP()[j] == "xxx" )
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d: btag SF = 1 applied (%s %s WP)",r,config_->btagAlgorithm().c_str(),config_->jetsBtagWP()[j].c_str()));
      else
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d: btag SF applied (%s %s WP)",r,config_->btagAlgorithm().c_str(),config_->jetsBtagWP()[j].c_str()));
   }
   
   if ( global_weight || config_->jetsBtagWP()[j] != "xxx" )  sf = this->btagSF(r,config_->jetsBtagWP()[j]).nominal;
   
   weight_ *= sf;
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   return sf;
   
}

float JetAnalyser::getBtagSF(const int & r)
{
   float sf = 1.;
   int j = r-1;
   if ( ! config_-> isMC() || config_->btagScaleFactors() == "" ) return sf;  // will not apply btag SF
   if ( ! config_->signalRegion() && r == config_->revBtagJet() ) return sf;
   
   if ( config_->jetsBtagWP()[j] != "xxx" )  sf = this->btagSF(r,config_->jetsBtagWP()[j]).nominal;
   
   return sf;
}

void JetAnalyser::actionApplyBjetRegression()
{
   if ( ! config_->bRegression() ) return;
   
   ++cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
      h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,"b jet energy regression");
   
   for ( auto & j : selectedJets_ )
   {
      j -> applyBjetRegression();
   }
   h1_["cutflow"] -> Fill(cutflow_,weight_);
}

bool JetAnalyser::selectionDiJetMass(const int & r1, const int & r2)
{
   float min = config_->massMin();
   float max = config_->massMax();
   if ( min < 0. && max < 0. ) return true;
   
   ++cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" )
   {
      std::string label = Form("M(Jet %d + Jet %d)",r1,r2);
      if ( min > 0. )
      {   
         if ( max > 0. && max > min ) label = Form("%5.1f GeV < %s < %5.1f GeV",min,label.c_str(),max);
         else                         label = Form("%s > %5.1f GeV",label.c_str(),min);
      }
      else
      {
         label = Form("%s < %5.1f GeV",label.c_str(),max);
      }
      
      h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,label.c_str());
   }
   
   int j1 = r1-1;
   int j2 = r2-1;
   Composite<Jet,Jet> c_j1j2(*(selectedJets_[j1]),*(selectedJets_[j2]));
   float mass = c_j1j2.m();
   
   if ( min > 0. && mass < min ) return false;
   if ( max > 0. && mass > max ) return false;
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   return true;
}



void JetAnalyser::jetSwap(const int & r1, const int & r2)
{
   if ( r1 == r2 ) return;
   int j1 = r1-1;
   int j2 = r2-1;
//    ++ cutflow_;
//    if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
//       h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d <--> Jet %d: jets ranking was swapped ",r1,r2));
   
   auto jet1 = selectedJets_[j1];
   auto jet2 = selectedJets_[j2];
   
   selectedJets_[j1] = jet2;
   selectedJets_[j2] = jet1;
   
//    h1_["cutflow"] -> Fill(cutflow_,weight_);
   
}



bool JetAnalyser::selectionJetQGlikelihood(const int & r, const float & cut)
{
   int j = r-1;
   
   ++cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" )
   {
      if ( cut > 0 )
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d Q-G likelihood < %4.2f",r,fabs(cut)));
      else
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d Q-G likelihood > %4.2f",r,fabs(cut)));
   }
   
   if ( cut > 0 )
   {
      if ( selectedJets_[j]->qgLikelihood() > fabs(cut) ) return false;
   }
   else
   {
      if ( selectedJets_[j]->qgLikelihood() < fabs(cut) ) return false;
   }
        
   h1_["cutflow"] -> Fill(cutflow_,weight_);
    
   return true;
   
}

bool JetAnalyser::selectionJetQGlikelihood(const int & r)
{
   int j = r-1;
   bool ok = true;
   if ( config_->jetsQGmax().size() == 0 || config_->jetsQGmax()[j] < 0 || (int)config_->jetsQGmax().size() < r )
   {
      ok = ok && true;
   }
   else
   {
      ok = ok && selectionJetQGlikelihood(r,config_->jetsQGmax()[j]);
   }
   
   if (config_->jetsQGmin().size() == 0 || config_->jetsQGmin()[j] < 0 || (int)config_->jetsQGmin().size() < r )
   {
      ok = ok && true;
   }
   else
   {
      ok = ok && selectionJetQGlikelihood(r,-1*config_->jetsQGmin()[j] );
   }
   return ok;
   
}


bool JetAnalyser::selectionBJetProbB(const int & r )
{
   if ( config_->jetsBtagProbB().size() == 0 ) return true;
   int j = r-1;
   float wp = config_->jetsBtagProbB()[j];
   std::string algo = config_->btagAlgorithm();
   if ( fabs(wp) > 1 ) return true; // there is no selection here, so will not update the cutflow
   
   ++ cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
   {
      if ( wp > 0 )
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d: %s btag prob b < %6.4f",r,algo.c_str(),fabs(wp)));
      else
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d: %s btag prob b > %6.4f",r,algo.c_str(),fabs(wp)));
   }
         
   
   if ( r > config_->nBJetsMin() ) 
   {
      std::cout << "* warning * -  JetAnalyser::selectionBJetProbB(): given jet rank > nbjetsmin. Returning false! " << std::endl;
      return false;
   }
   
   // jet  btag
   if ( algo == "deepcsv" )
   {
      if ( wp > 0 && selectedJets_[j]->btag("btag_deepb") > fabs(wp) ) return false;
      if ( wp < 0 && selectedJets_[j]->btag("btag_deepb") < fabs(wp) ) return false;
   }
   if ( algo == "deepflavour" || algo == "deepjet" )
   {
      if ( wp > 0 && selectedJets_[j]->btag("btag_dfb") > fabs(wp) ) return false;
      if ( wp < 0 && selectedJets_[j]->btag("btag_dfb") < fabs(wp) ) return false;
   }
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   return true;
}




bool JetAnalyser::selectionBJetProbBB(const int & r )
{
   if ( config_->jetsBtagProbBB().size() == 0 ) return true;
   int j = r-1;
   float wp = config_->jetsBtagProbBB()[j];
   std::string algo = config_->btagAlgorithm();
   if ( fabs(wp) > 1 ) return true; // there is no selection here, so will not update the cutflow
   
   ++ cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
   {
      if ( wp > 0 )
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d: %s btag prob bb < %6.4f",r,algo.c_str(),fabs(wp)));
      else
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d: %s btag prob bb > %6.4f",r,algo.c_str(),fabs(wp)));
   }
         
   
   if ( r > config_->nBJetsMin() ) 
   {
      std::cout << "* warning * -  JetAnalyser::selectionBJetProbBB(): given jet rank > nbjetsmin. Returning false! " << std::endl;
      return false;
   }
   
   // jet  btag
   if ( algo == "deepcsv" )
   {
      if ( wp > 0 && selectedJets_[j]->btag("btag_deepbb") > fabs(wp) ) return false;
      if ( wp < 0 && selectedJets_[j]->btag("btag_deepbb") < fabs(wp) ) return false;
   }
   if ( algo == "deepflavour" || algo == "deepjet" )
   {
      if ( wp > 0 && selectedJets_[j]->btag("btag_dfbb") > fabs(wp) ) return false;
      if ( wp < 0 && selectedJets_[j]->btag("btag_dfbb") < fabs(wp) ) return false;
   }
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   return true;
}

bool JetAnalyser::selectionBJetProbLepB(const int & r )
{
   if ( config_->jetsBtagProbLepB().size() == 0 ) return true;
   int j = r-1;
   float wp = config_->jetsBtagProbLepB()[j];
   std::string algo = config_->btagAlgorithm();
   if ( fabs(wp) > 1 || algo == "deepcsv" ) return true; // there is no selection here, so will not update the cutflow
   
   ++ cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
   {
      if ( wp > 0 )
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d: %s btag prob lepb < %6.4f",r,algo.c_str(),fabs(wp)));
      else
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d: %s btag prob lepb > %6.4f",r,algo.c_str(),fabs(wp)));
   }
         
   
   if ( r > config_->nBJetsMin() ) 
   {
      std::cout << "* warning * -  JetAnalyser::selectionBJetProbLepB(): given jet rank > nbjetsmin. Returning false! " << std::endl;
      return false;
   }
   
   // jet  btag
   if ( algo == "deepflavour" || algo == "deepjet" )
   {
      if ( wp > 0 && selectedJets_[j]->btag("btag_dflepb") > fabs(wp) ) return false;
      if ( wp < 0 && selectedJets_[j]->btag("btag_dflepb") < fabs(wp) ) return false;
   }
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   return true;
}



bool JetAnalyser::selectionBJetProbC(const int & r )
{
   if ( config_->jetsBtagProbC().size() == 0 ) return true;
   int j = r-1;
   float wp = config_->jetsBtagProbC()[j];
   std::string algo = config_->btagAlgorithm();
   if ( fabs(wp) > 1 ) return true; // there is no selection here, so will not update the cutflow
   
   ++ cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
   {
      if ( wp > 0 )
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d: %s btag prob c < %6.4f",r,algo.c_str(),fabs(wp)));
      else
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d: %s btag prob c > %6.4f",r,algo.c_str(),fabs(wp)));
   }
         
   
   if ( r > config_->nBJetsMin() ) 
   {
      std::cout << "* warning * -  JetAnalyser::selectionBJetProbC(): given jet rank > nbjetsmin. Returning false! " << std::endl;
      return false;
   }
   
   // jet  btag
   if ( algo == "deepcsv" )
   {
      if ( wp > 0 && selectedJets_[j]->btag("btag_deepc") > fabs(wp) ) return false;
      if ( wp < 0 && selectedJets_[j]->btag("btag_deepc") < fabs(wp) ) return false;
   }
   if ( algo == "deepflavour" || algo == "deepjet" )
   {
      if ( wp > 0 && selectedJets_[j]->btag("btag_dfc") > fabs(wp) ) return false;
      if ( wp < 0 && selectedJets_[j]->btag("btag_dfc") < fabs(wp) ) return false;
   }
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   return true;
}

bool JetAnalyser::selectionBJetProbG(const int & r )
{
   if ( config_->jetsBtagProbG().size() == 0 ) return true;
   int j = r-1;
   float wp = config_->jetsBtagProbG()[j];
   std::string algo = config_->btagAlgorithm();
   if ( fabs(wp) > 1 || algo == "deepcsv" ) return true; // there is no selection here, so will not update the cutflow
   
   ++ cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
   {
      if ( wp > 0 )
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d: %s btag prob g < %6.4f",r,algo.c_str(),fabs(wp)));
      else
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d: %s btag prob g > %6.4f",r,algo.c_str(),fabs(wp)));
   }
         
   
   if ( r > config_->nBJetsMin() ) 
   {
      std::cout << "* warning * -  JetAnalyser::selectionBJetProbG(): given jet rank > nbjetsmin. Returning false! " << std::endl;
      return false;
   }
   
   // jet  btag
   if ( algo == "deepflavour" || algo == "deepjet" )
   {
      if ( wp > 0 && selectedJets_[j]->btag("btag_dfg") > fabs(wp) ) return false;
      if ( wp < 0 && selectedJets_[j]->btag("btag_dfg") < fabs(wp) ) return false;
   }
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   return true;
}

bool JetAnalyser::selectionBJetProbLight(const int & r )
{
   if ( config_->jetsBtagProbLight().size() == 0 ) return true;
   int j = r-1;
   float wp = config_->jetsBtagProbLight()[j];
   std::string algo = config_->btagAlgorithm();
   if ( fabs(wp) > 1 ) return true; // there is no selection here, so will not update the cutflow
   
   ++ cutflow_;
   if ( std::string(h1_["cutflow"] -> GetXaxis()-> GetBinLabel(cutflow_+1)) == "" ) 
   {
      if ( wp > 0 )
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d: %s btag prob light < %6.4f",r,algo.c_str(),fabs(wp)));
      else
         h1_["cutflow"] -> GetXaxis()-> SetBinLabel(cutflow_+1,Form("Jet %d: %s btag prob light > %6.4f",r,algo.c_str(),fabs(wp)));
   }
         
   
   if ( r > config_->nBJetsMin() ) 
   {
      std::cout << "* warning * -  JetAnalyser::selectionBJetProbLight(): given jet rank > nbjetsmin. Returning false! " << std::endl;
      return false;
   }
   
   // jet  btag
   if ( algo == "deepcsv" )
   {
      if ( wp > 0 && selectedJets_[j]->btag("btag_deeplight") > fabs(wp) ) return false;
      if ( wp < 0 && selectedJets_[j]->btag("btag_deeplight") < fabs(wp) ) return false;
   }
   if ( algo == "deepflavour" || algo == "deepjet" )
   {
      if ( wp > 0 && selectedJets_[j]->btag("btag_dflight") > fabs(wp) ) return false;
      if ( wp < 0 && selectedJets_[j]->btag("btag_dflight") < fabs(wp) ) return false;
   }
   
   h1_["cutflow"] -> Fill(cutflow_,weight_);
   
   return true;
}

// float JetAnalyser::btagEfficiency(const int & r)
// {
//    float eff = 1;
//    auto btageff = this -> btagEfficiencies();
//    int j = r-1;
//    
//    return eff;
//    
// }

// void JetAnalyser::actionApplyBtagEfficiencyWeight(const int & r)
// {
//    weight_ *= btagEfficiency(r);
//    
// }
