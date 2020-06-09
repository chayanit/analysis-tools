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
#include "Analysis/Tools/interface/Config.h"

//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
Config::Config()
{
}

// Main constructor
Config::Config(int argc, char ** argv) : opt_cmd_("Options"), opt_cfg_("Configuration")
{
   std::string toolspath = Form("%s/src/Analysis/Tools",getenv("CMSSW_BASE"));
   std::string datapath = Form("%s/src/Analysis/Tools/data",getenv("CMSSW_BASE"));
   argc_ = argc;
   argv_ = argv;
   // read configuration options
   try
   {
      namespace po = boost::program_options;
      opt_cmd_.add_options()
         ("help,h","Show help messages")
         ("config,c",po::value<std::string>(&cfg_),"Configuration file name");
      
      // analysis info
      opt_cfg_.add_options()
         ("Info.ntuplesList"             , po::value <std::string>               (&inputlist_)       -> default_value("rootFileList.txt") ,"File with list of ntuples")
         ("Info.process"                 , po::value <std::string>               (&process_)         -> default_value("MssmHbb")          ,"Process of ntuples")
         ("Info.events"                  , po::value <std::string>               (&eventsdir_)       -> default_value("Events")           ,"Name of the events directory")
         ("Info.eventInfo"               , po::value <std::string>               (&eventinfo_)       -> default_value("EventInfo")        ,"EventInfo directory in the tree")
         ("Info.json"                    , po::value <std::string>               (&json_)            -> default_value("no_json.txt")      ,"JSON file for data")
         ("Info.output"                  , po::value <std::string>               (&outputRoot_)      -> default_value("histograms.root")  ,"Output root file")
         ("Info.seedFile"                , po::value <std::string>               (&seedfile_)        -> default_value("no_seed.txt")      ,"File with seed value for random numbers")
         ("Info.blindAnalysis"           , po::value <bool>                      (&blind_)           -> default_value(false)              ,"Flag for blind analysis")
         ("Info.nloMC"                   , po::value <bool>                      (&nlo_)             -> default_value(false)              ,"Flag for NLO MC samples")
         ("Info.isMC"                    , po::value <bool>                      (&isMC_)            -> default_value(true)               ,"Flag for MC dataset")
         ("Info.fullGenWeight"           , po::value <bool>                      (&fullgenweight_)   -> default_value(false)              ,"Flag for full gen weight of MC samples, otherwise only sign")
         ("Info.signalRegion"            , po::value <bool>                      (&signalregion_)    -> default_value(true)               ,"Flag for signal region")
         ("Info.eventsMax"               , po::value <int>                       (&nevtmax_)         -> default_value(-1)                 , "Maximum number of events")
         ("Info.seed"                    , po::value <int>                       (&seed_)            -> default_value(-1)                 , "Seed value for random numbers");
                                                                                                                                          
      // Corrections                                                                                                                      
      opt_cfg_.add_options()                                                                                                              
         ("Corrections.Pileup.reweight"  , po::value <std::string>               (&puweight_)        -> default_value("")                 , "Root file containing pileup weights")
         ("Corrections.Jets.jerPtRes"    , po::value <std::string>               (&jerptres_)        -> default_value("")                 , "JER pT resolution file")
         ("Corrections.Jets.jerSF"       , po::value <std::string>               (&jersf_)           -> default_value("")                 , "JER SF file")
         ("Corrections.BTag.SF"          , po::value <std::string>               (&btagsf_)          -> default_value("")                 , "b-tagging scale factor in CSV format")
         ("Corrections.BTag.Efficiencies", po::value <std::string>               (&btageff_)         -> default_value("")                 , "b-tagging efficiencies in root file")
         ("Corrections.Jets.bRegression" , po::value <bool>                      (&bregression_)     -> default_value(false)              , "Apply b jet energy regression")
         ("Corrections.force"            , po::value <bool>                      (&apply_correct_)   -> default_value(false)              , "Apply corrections internally when above are defined");
                                                                                                     
      // jets                                                                                        
      opt_cfg_.add_options()                                                                         
         ("Jets.ptMin"                   , po::value <std::vector<float> >       (&jetsptmin_)       -> multitoken()                      , "Mimium pt of the jets")
         ("Jets.ptMax"                   , po::value <std::vector<float> >       (&jetsptmax_)       -> multitoken()                      , "Maximum pt of the jets")
         ("Jets.etaMax"                  , po::value <std::vector<float> >       (&jetsetamax_)      -> multitoken()                      , "Maximum |eta| of the jets")
         ("Jets.jets"                    , po::value <std::string>               (&jetsCol_)         -> default_value("")                 , "Name of the jets collection")
         ("Jets.id"                      , po::value <std::string>               (&jetsid_)          -> default_value("tight")            , "Jets id criteria for all jets")
         ("Jets.puId"                    , po::value <std::string>               (&jetspuid_)        -> default_value("loose")            , "Jets pileup id criteria for all jets")
         ("Jets.extendedFlavour"         , po::value <bool>                      (&usejetsextflv_)   -> default_value(false)              , "For splitting results accoding to jet extended flavour")
         ("Jets.n"                       , po::value <int>                       (&njets_)           -> default_value(-1)                 , "Minimum number of jets")
         ("Jets.nMin"                    , po::value <int>                       (&njetsmin_)        -> default_value(0)                  , "Minimum number of jets")
         ("Jets.nMax"                    , po::value <int>                       (&njetsmax_)        -> default_value(-1)                 , "Maximum number of jets")
         ("Jets.dRMin"                   , po::value <float>                     (&jetsdrmin_)       -> default_value(-1.)                , "Minimum delta R between jets")
         ("Jets.dRMax"                   , po::value <float>                     (&jetsdrmax_)       -> default_value(-1.)                , "Maximum delta R between jets")
         ("Jets.dEtaMax"                 , po::value <float>                     (&jetsdetamax_)     -> default_value(-1.)                , "Maximum delta eta between jets")
         ("Jets.dEtaMin"                 , po::value <float>                     (&jetsdetamin_)     -> default_value(-1.)                , "Minimum delta eta between jets")
         ("Jets.dPhiMin"                 , po::value <float>                     (&jetsdphimin_)     -> default_value(-1.)                , "Minimum delta phi between jets")
         ("Jets.dPhiMax"                 , po::value <float>                     (&jetsdphimax_)     -> default_value(-1.)                , "Maximum delta phi between jets");
         
      // histograms                                                                                        
      opt_cfg_.add_options()                                                                         
         ("Histograms.Jets.splitRegions" , po::value <bool>                      (&histjets_rsplit_) -> default_value(false)              , "Split jets histograms into barrel, barrel-endcap overlap, endcap")
         ("Histograms.Jets.flavour"      , po::value <bool>                      (&histjets_flavour_)-> default_value(false)              , "Split jets histograms per flavour");
         
                                                                                                                                          
      // dijets                                                                                                                           
      opt_cfg_.add_options()                                                                                                              
         ("Dijets.dijets"                , po::value <bool>                      (&dodijet_ )        -> default_value(false)              , "Combine all jets in dijet objects")
         ("Dijets.ranks"                 , po::value<std::vector<int> >          (&dijet_ranks_)     -> multitoken()                      , "Ranks of the jets to construct and select the diject");
                                                                                                                                          
      // btagging                                                                                                                         
      opt_cfg_.add_options()                                                                                                              
         ("BTag.wp"                      , po::value <std::vector<std::string> > (&jetsbtagwp_)      -> multitoken()                      ,"Jets btag minimum (with '-' means maximum)")
         ("BTag.algorithm"               , po::value <std::string>               (&btagalgo_)        -> default_value("csvivf")           ,"BTag algorithm")
         ("BTag.loose"                   , po::value <float>                     (&btagwploose_)     -> default_value(-10000)             ,"BTag working point LOOSE")
         ("BTag.medium"                  , po::value <float>                     (&btagwpmedium_)    -> default_value(-10000)             ,"BTag working point MEDIUM")
         ("BTag.tight"                   , po::value <float>                     (&btagwptight_)     -> default_value(-10000)             ,"BTag working point TIGHT")
         ("BTag.user"                    , po::value <float>                     (&btagwpxxx_)       -> default_value(-10000)             ,"BTag working point USER-defined")
         ("BTag.nMin"                    , po::value <int>                       (&nbjetsmin_)       -> default_value(-1)                  ,"Minimum number of btgaged jets")
         ("BTag.revWP"                   , po::value <std::string>               (&revbtagwp_)       -> default_value("")                 ,"non-Btag working point")
         ("BTag.revBJet"                 , po::value <int>                       (&revbtagjet_)      -> default_value(-1)                 ,"non-Btag Jet");
                                                                                                                                          
      // muons                                                                                                                            
      opt_cfg_.add_options()                                                                                                              
         ("Muons.ptMin"                  , po::value<std::vector<float> >        (&muonsptmin_)      -> multitoken()                      , "Mimium pt of the muons")
         ("Muons.ptMax"                  , po::value<std::vector<float> >        (&muonsptmax_)      -> multitoken()                      , "Maximum pt of the muons")
         ("Muons.etaMax"                 , po::value<std::vector<float> >        (&muonsetamax_)     -> multitoken()                      , "Maximum |eta| of the muons")
         ("Muons.muons"                  , po::value <std::string>               (&muonsCol_)        -> default_value("")                 , "Name of the muons collection")      
         ("Muons.id"                     , po::value <std::string>               (&muonsid_)         -> default_value("LOOSE")            , "muons id criteria for all muons")
         ("Muons.nMin"                   , po::value <int>                       (&nmuonsmin_)       -> default_value(0)                  , "Minimum number of muons")
         ("Muons.nMax"                   , po::value <int>                       (&nmuonsmax_)       -> default_value(-1)                 , "Maximum number of muons");
                                                                                                                                          
      // trigger                                                                                                                          
      opt_cfg_.add_options()                                                                                                              
         ("Trigger.hltPath"              , po::value <std::string>               (&hltPath_)         -> default_value("")                 , "HLT path name")
         ("Trigger.l1Seed"               , po::value <std::string>               (&l1Seed_)          -> default_value("")                 , "L1 seed name")
         ("Trigger.results"              , po::value <std::string>               (&triggerCol_)      -> default_value("TriggerResults")   , "Name of the trigger results collection");
                                                                                                                                          
      // trigger objects                                                                                                                  
      opt_cfg_.add_options()                                                                                                              
         ("Trigger.Objects.directory"    , po::value<std::string>                (&triggerObjDir_)   -> default_value("slimmedPatTrigger"), "Name of the trigger objects directory")
         ("Trigger.Objects.BTag.Calo"    , po::value<std::string>                (&trgObjsBJets_)    -> default_value("")                 , "Trigger objects for btag jets")
         ("Trigger.Objects.Jets.L1"      , po::value<std::string>                (&trgObjsL1Jets_)   -> default_value("")                 , "Trigger objects for L1 jets")
         ("Trigger.Objects.Jets.Calo"    , po::value<std::string>                (&trgObjsCaloJets_) -> default_value("")                 , "Trigger objects for Calo jets")
         ("Trigger.Objects.Jets.PF"      , po::value<std::string>                (&trgObjsPFJets_)   -> default_value("")                 , "Trigger objects for PF jets")
         ("Trigger.Objects.Muons.L1"     , po::value<std::string>                (&trgObjsL1Muons_)  -> default_value("")                 , "Trigger objects for L1 muons")
         ("Trigger.Objects.Muons.L3"     , po::value<std::string>                (&trgObjsL3Muons_)  -> default_value("")                 , "Trigger objects for L3 muons");
                                                                                                                                          
      // L1 trigger                                                                                                                       
      opt_cfg_.add_options()                                                                                                              
         ("L1T.jets"                     , po::value <std::string>               (&l1tjetsCol_)      -> default_value("l1tJets")          , "Name of the L1T jets collection")
         ("L1T.muons"                    , po::value <std::string>               (&l1tmuonsCol_)     -> default_value("l1tMuons")         , "Name of the L1T muons collection");
                                                                                                                                          
      // generator level                                                                                                                  
      opt_cfg_.add_options()                                                                                                              
         ("Generator.genParticles"       , po::value <std::string>               (&genpartsCol_)     -> default_value("")                 , "Name of the gen particle collection")
         ("Generator.genJets"            , po::value <std::string>               (&genjetsCol_)      -> default_value("")                 , "Name of the gen jets collection");
                                                                                                                                          
                                                                                                                                          
      // general                                                                                                                          
      opt_cfg_.add_options()                                                                                                              
         ("User.doTree"                  , po::value <bool>                      (&do_tree_)         -> default_value(false)              , "Flag for output")
         ("User.override"                , po::value <bool>                      (&override_)        -> default_value(false)              , "Flag to be used to override procedure, e.g. a selection")      
         ("User.dRMin"                   , po::value <float>                     (&drmin_)           -> default_value(-1.)                , "Minimum delta R between candidates")
         ("User.dRMax"                   , po::value <float>                     (&drmax_)           -> default_value(-1.)                , "Maximum delta R between candidates")
         ("User.dEtaMax"                 , po::value <float>                     (&detamax_)         -> default_value(-1.)                , "Maximum delta eta between candidates")
         ("User.dEtaMin"                 , po::value <float>                     (&detamin_)         -> default_value(-1.)                , "Minimum delta eta between candidates")
         ("User.dPhiMin"                 , po::value <float>                     (&dphimin_)         -> default_value(-1.)                , "Minimum delta phi between candidates")
         ("User.dPhiMax"                 , po::value <float>                     (&dphimax_)         -> default_value(-1.)                , "Maximum delta phi between candidates")
         ("User.massMin"                 , po::value <float>                     (&massmin_)         -> default_value(-1.)                , "Cut on a mass, min value")
         ("User.massMax"                 , po::value <float>                     (&massmax_)         -> default_value(-1.)                , "Cut on a mass, max value")
         ("User.min"                     , po::value <float>                     (&min_)             -> default_value(-1.)                , "some minimum value")  
         ("User.max"                     , po::value <float>                     (&max_)             -> default_value(-1.)                , "some maximum value")
         ("User.scale"                   , po::value <float>                     (&scale_)           -> default_value(-1.)                , "Overall scale for histograms")  
         ("User.workflow"                , po::value <int>                       (&workflow_)        -> default_value(1)                  , "Workflow index defined by user")
         ("User.prescale"                , po::value <int>                       (&prescale_)        -> default_value(1)                  , "Prescale factor")  
         ("User.n"                       , po::value <int>                       (&n_)               -> default_value(-1)                 , "Some integer")
         ("User.index"                   , po::value <int>                       (&index_)           -> default_value(-1)                 , "Some User index for user");
            
      // others
      opt_cfg_.add_options()
         ("nMin",po::value <int> (&nmin_)->default_value(0),"Minimum number objects")
         ("nMax",po::value <int> (&nmax_)->default_value(0),"Maximum number objects")
         ("ptMin", po::value<std::vector<float> >(&ptmin_)->multitoken(),"Mimium pt of an object")
         ("ptMax", po::value<std::vector<float> >(&ptmax_)->multitoken(),"Maximum pt of an object")
         ("etaMax", po::value<std::vector<float> >(&etamax_)->multitoken(),"Maximum |eta| of an object")
         ("ptImbalanceMin",po::value <float> (&jetsptimbalmin_)->default_value(-1),"Minimum relative imbalance between two candidates")
         ("ptImbalanceMax",po::value <float> (&jetsptimbalmax_)->default_value(-1),"Maximum relative imbalance between two candidates");
      
         
      opt_cfg_.add_options()
         ("qgMin", po::value<std::vector<float> >(&qgmin_)->multitoken(),"Minimum value for q-g likelihood")
         ("qgMax", po::value<std::vector<float> >(&qgmax_)->multitoken(),"Maximum value for q-g likelihood")
         ("jetsBtagMin", po::value<std::vector<float> >(&jetsbtagmin_)->multitoken(),"Minimum btag of the jets; if < 0 -> reverse btag")
         ("jetsBtagProbB", po::value<std::vector<float> >(&jetsbtagprobb_)->multitoken(),"Maximum (minimum) btag prob b of the jets if >0 (<0)")
         ("jetsBtagProbBB", po::value<std::vector<float> >(&jetsbtagprobbb_)->multitoken(),"Maximum (minimum) btag prob bb of the jets if >0 (<0)")
         ("jetsBtagProbLepB", po::value<std::vector<float> >(&jetsbtagproblepb_)->multitoken(),"Maximum (minimum) btag prob lepb of the jets if >0 (<0)")
         ("jetsBtagProbC", po::value<std::vector<float> >(&jetsbtagprobc_)->multitoken(),"Maximum (minimum) btag prob c of the jets if >0 (<0)")
         ("jetsBtagProbG", po::value<std::vector<float> >(&jetsbtagprobg_)->multitoken(),"Maximum (minimum) btag prob g of the jets if >0 (<0)")
         ("jetsBtagProbLight", po::value<std::vector<float> >(&jetsbtagproblight_)->multitoken(),"Maximum (minimum) btag prob light of the jets if >0 (<0)");

      opt_cfg_.add_options()
         ("triggerMatchDeltaRMax",po::value <float> (&trgmatchdrmax_)->default_value(0.3),"DeltaR max for matching online-offline");

      // AI
      opt_cfg_.add_options()
         ("variablesFloatAI", po::value<std::vector<std::string> >(&varsf_ai_)->multitoken(),"Float variables names for AI(TMVA)")
         ("variablesIntAI", po::value<std::vector<std::string> >(&varsi_ai_)->multitoken(),"Integer variables names for AI(TMVA)")
         ("directoryAI",po::value <std::string> (&dir_ai_)->default_value(""),"Directory with weights for AI(TMVA)")
         ("methodAI",po::value <std::string> (&method_ai_)->default_value(""),"Method AI(TMVA)")
         ("efficiencyMinAI",po::value <float> (&eff_min_ai_)->default_value(-1.),"Min value for AI Cuts efficiency")
         ("discriminatorMaxAI",po::value <float> (&disc_max_ai_)->default_value(-1001.),"Max value for AI discriminator")
         ("discriminatorMinAI",po::value <float> (&disc_min_ai_)->default_value(-1001.),"Min value for AI discriminator");
      
      opt_cfg_.add_options()
         ("crossSectionTree",po::value <std::string> (&xsectiontree_)->default_value(""),"Tree containing cross sections")
         ("crossSectionType",po::value <std::string> (&xsectiontype_)->default_value("crossSection"),"Type of cross section")
         ("crossSection",po::value <float> (&xsection_)->default_value(-1.), "Cross section")  
         ("luminosity",po::value <float> (&lumi_)->default_value(-1.), "Luminosity in pb-1 to scale histograms")   
         ("nLumiSections",po::value <int> (&nlumis_)->default_value(-1), "Number of lumi sections processed")
         ("runMin",po::value <int> (&runmin_)->default_value(-1), "Minimum run number")
         ("runMax",po::value <int> (&runmax_)->default_value(-1), "Maximum run number")
         ("pythia8",po::value <bool> (&pythia8_)->default_value(true),"Flag for Pythia8 or other recent generators MC")
         ("erasLumi", po::value<std::vector<float> >(&eraslumi_)->multitoken(),"Lumi of an era")
         ("eras", po::value<std::vector<std::string> >(&eras_)->multitoken(),"Era of data taking");
         
      
            
      po::variables_map vm; 
      try
      {
         po::store(po::parse_command_line(argc, argv, opt_cmd_), vm); // can throw
         // --help option
         
         if ( vm.count("help") )
         {
            std::cout << "SimpleBjetsAnalysis macro" << std::endl
                      << opt_cmd_ << std::endl
                      << opt_cfg_ << std::endl;
         }
         po::notify(vm);
         
         std::ifstream cfg_s(cfg_.c_str());
         po::store(po::parse_config_file(cfg_s, opt_cfg_), vm); // can throw
         if ( vm.count("config") )
         {

         }
         po::notify(vm);
         boost::algorithm::to_lower(jetsid_);
         std::transform(btagalgo_.begin(), btagalgo_.end(), btagalgo_.begin(), ::tolower);
         
//         inputlist_ = Form("%s/test/%s", toolspath.c_str(), inputlist_.c_str());
         
         if ( json_     != "no_json.txt" ) json_ = Form("%s/%s", datapath.c_str(), json_.c_str());
         if ( jerptres_ != "" )  jerptres_ = Form("%s/%s", datapath.c_str(), jerptres_.c_str());
         if ( jersf_    != "" )  jersf_    = Form("%s/%s", datapath.c_str(), jersf_.c_str()   );
         if ( btagsf_   != "" )  btagsf_   = Form("%s/%s", datapath.c_str(), btagsf_.c_str()  );
         if ( btageff_  != "" )  btageff_  = Form("%s/%s", datapath.c_str(), btageff_.c_str() );
         if ( puweight_ != "" )  puweight_ = Form("%s/%s", datapath.c_str(), puweight_.c_str());
         eventinfo_     =  Form("%s/%s/%s" , process_.c_str(), eventsdir_.c_str() , eventinfo_.c_str()      );
         triggerCol_    =  Form("%s/%s/%s" , process_.c_str(), eventsdir_.c_str() , triggerCol_.c_str()     );
         triggerObjDir_ =  Form("%s/%s/%s" , process_.c_str(), eventsdir_.c_str() , triggerObjDir_.c_str()  );
         if ( jetsCol_ != "" )
            jetsCol_       =  Form("%s/%s/%s" , process_.c_str(), eventsdir_.c_str() , jetsCol_.c_str()        );
         if ( muonsCol_ != "" )
            muonsCol_      =  Form("%s/%s/%s" , process_.c_str(), eventsdir_.c_str() , muonsCol_.c_str()       );
         if ( genpartsCol_ != "" )
            genpartsCol_   =  Form("%s/%s/%s" , process_.c_str(), eventsdir_.c_str() , genpartsCol_.c_str()    );
         if ( genjetsCol_ != "" )
            genjetsCol_    =  Form("%s/%s/%s" , process_.c_str(), eventsdir_.c_str() , genjetsCol_.c_str()     );
         l1tjetsCol_    =  Form("%s/%s/%s" , process_.c_str(), eventsdir_.c_str() , l1tjetsCol_.c_str()     );
         l1tmuonsCol_   =  Form("%s/%s/%s" , process_.c_str(), eventsdir_.c_str() , l1tmuonsCol_.c_str()    );
         
         if ( njetsmax_ < njetsmin_ ) njetsmax_ = -1;
         if ( njetsmin_ < 0 && njetsmax_ > 0 ) njetsmin_ = 0;
         
         if ( njets_ >= 0 )
         {
            njetsmin_ = njets_;
            njetsmax_ = njets_;
         }
      }
      catch(po::error& e)
      { 
         std::cerr << "ERROR: " << e.what() << std::endl << std::endl; 
         std::cerr << opt_cmd_ << std::endl;
         throw std::exception();
      } 
      
   }
   catch(std::exception& e) 
   { 
      std::cerr << "ERROR: " << e.what() << std::endl << std::endl; 
      throw std::exception();
   } 
   
}

Config::~Config()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//
// ------------ method called for each event  ------------

po::options_description  & Config::optionsCMD()
{
   return opt_cmd_;
}

po::options_description  & Config::optionsCFG()
{
   return opt_cfg_;
}


void Config::loadOptions()
{
   po::variables_map vm;
   po::store(po::parse_command_line(argc_, argv_, opt_cmd_), vm); 
   po::notify(vm);
   std::ifstream cfg_s(cfg_.c_str());
   po::store(po::parse_config_file(cfg_s, opt_cfg_), vm); // can throw
   po::notify(vm);
   
   
}

//
std::string        Config::configFile()       const { return cfg_; }

// analysis info
std::string        Config::ntuplesList()      const { return inputlist_; }
std::string        Config::eventInfo()        const { return eventinfo_; }
std::string        Config::crossSectionTree() const { return xsectiontree_; }
std::string        Config::crossSectionType() const { return xsectiontype_; }
float              Config::crossSection()     const { return xsection_; }
float              Config::luminosity()       const { return lumi_; }
int                Config::nEventsMax()       const { return nevtmax_; }
bool               Config::isMC()             const { return isMC_; }
bool               Config::signalRegion()     const { return signalregion_; }
bool               Config::blind()            const { return blind_; }
bool               Config::nlo()              const { return nlo_; }
bool               Config::fullGenWeight()    const { return fullgenweight_; }
int                Config::workflow()         const { return workflow_; }
int                Config::index()            const { return index_; }
float              Config::scale()            const { return scale_; }
std::vector<float> Config::erasLumi()         const { return eraslumi_; }
std::vector<std::string> Config::eras()       const { return eras_; }
std::string        Config::pileupWeights()    const { return puweight_; }

// analysis control
bool               Config::override()          const { return override_; }

// jets
std::string        Config::jetsCollection()     const { return jetsCol_; }
int                Config::nJetsMin()           const { return njetsmin_; }
int                Config::nJetsMax()           const { return njetsmax_; }
int                Config::nJets()              const { return njets_; }
std::vector<float> Config::jetsPtMin()          const { return jetsptmin_; }
std::vector<float> Config::jetsPtMax()          const { return jetsptmax_; }
std::vector<float> Config::jetsEtaMax()         const { return jetsetamax_; }
std::string        Config::jetsId()             const { return jetsid_; }
std::string        Config::jetsPuId()           const { return jetspuid_; }
std::string        Config::jerPtRes()           const { return jerptres_; }
std::string        Config::jerSF()              const { return jersf_; }
std::string        Config::l1tJetsCollection()  const { return l1tjetsCol_; } 
std::string        Config::btagAlgorithm()      const { return btagalgo_; } 
std::string        Config::btagScaleFactors()   const { return btagsf_; } 
std::vector<std::string> Config::jetsBtagWP()   const { return jetsbtagwp_; }
std::vector<float> Config::jetsBtagProbB()      const { return jetsbtagprobb_; }
std::vector<float> Config::jetsBtagProbBB()     const { return jetsbtagprobbb_; }
std::vector<float> Config::jetsBtagProbLepB()   const { return jetsbtagproblepb_; }
std::vector<float> Config::jetsBtagProbC()      const { return jetsbtagprobc_; }
std::vector<float> Config::jetsBtagProbG()      const { return jetsbtagprobg_; }
std::vector<float> Config::jetsBtagProbLight()  const { return jetsbtagproblight_; }
bool               Config::bRegression()        const { return bregression_; }
std::string        Config::revBtagWP()          const { return revbtagwp_; }
int                Config::revBtagJet()         const { return revbtagjet_; }
bool               Config::useJetsExtendedFlavour() const { return usejetsextflv_; }
bool               Config::doDijet()            const { return dodijet_ ; }
int                Config::nBJetsMin()          const { return nbjetsmin_; }

std::vector<float>  Config::jetsQGmin() const { return qgmin_; }
std::vector<float>  Config::jetsQGmax() const { return qgmax_; }

// jet-jet
float Config::jetsDetaMax()         const { return jetsdetamax_; }
float Config::jetsDetaMin()         const { return jetsdetamin_; }
float Config::jetsDphiMax()         const { return jetsdphimax_; }
float Config::jetsDphiMin()         const { return jetsdphimin_; }
float Config::jetsDrMax()           const { return jetsdrmax_;   }
float Config::jetsDrMin()           const { return jetsdrmin_;   }
float Config::jetsPtImbalanceMax()  const { return jetsptimbalmax_;  }
float Config::jetsPtImbalanceMin()  const { return jetsptimbalmin_;  }

// muons
std::string        Config::muonsCollection()    const { return muonsCol_; }
int                Config::nMuonsMin()          const { return nmuonsmin_; }
int                Config::nMuonsMax()          const { return nmuonsmax_; }
std::vector<float> Config::muonsPtMin()         const { return muonsptmin_; }
std::vector<float> Config::muonsPtMax()         const { return muonsptmax_; }
std::vector<float> Config::muonsEtaMax()        const { return muonsetamax_; }
std::string        Config::muonsId()            const { return muonsid_; }
std::string        Config::l1tMuonsCollection() const { return l1tmuonsCol_; } 

// trigger
std::string        Config::triggerResults()         const { return triggerCol_; }
std::string        Config::triggerObjectsDir()      const { return triggerObjDir_; }
std::string        Config::triggerObjectsL1Muons()  const { return trgObjsL1Muons_; }
std::string        Config::triggerObjectsL3Muons()  const { return trgObjsL3Muons_; }
std::string        Config::triggerObjectsBJets()    const { return trgObjsBJets_; }
std::string        Config::triggerObjectsL1Jets()   const { return trgObjsL1Jets_; }
std::string        Config::triggerObjectsCaloJets() const { return trgObjsCaloJets_; }
std::string        Config::triggerObjectsPFJets()   const { return trgObjsPFJets_; }
// generator level
std::string        Config::genJetsCollection()       const { return genjetsCol_; }
std::string        Config::genParticlesCollection()  const { return genpartsCol_; }

// seed 
std::string        Config::seedFile()           const { return seedfile_; }
int                Config::seed()               const { return seed_;     }

bool               Config::pythia8()            const { return pythia8_;  }

// btag
std::string        Config::btagEfficiencies()             const  { return btageff_; }
float              Config::btagWP(const std::string & wp) const
{
   if ( wp == "loose"  ) return btagwploose_ ;
   if ( wp == "medium" ) return btagwpmedium_;
   if ( wp == "tight"  ) return btagwptight_ ;
   if ( wp == "xxx"    ) return btagwpxxx_;
   
   return -100.;
}
 
// User stuff            
float Config::massMin() const { return massmin_; }
float Config::massMax() const { return massmax_; }


// AI
std::vector<std::string> Config::variablesAI(const std::string & t) const
{
   if ( t == "I" ) return varsi_ai_;
   return varsf_ai_;
}
std::string Config::directoryAI()  const { return dir_ai_     ; }
std::string Config::methodAI()     const { return method_ai_  ; }
float Config::discriminatorMaxAI() const { return disc_max_ai_; }
float Config::discriminatorMinAI() const { return disc_min_ai_; }
float Config::efficiencyMinAI()    const { return eff_min_ai_ ; }

// output tree
bool Config::doTree() const { return do_tree_; }

// User options
int   Config::prescale()   const { return prescale_; }
int   Config::n()          const { return n_;   }
float Config::min()        const { return min_; }
float Config::max()        const { return max_; }

// Histograms
bool  Config::histogramJetsRegionSplit() const { return histjets_rsplit_ ; }
bool  Config::histogramJetsPerFlavour()  const { return histjets_flavour_ ; }
