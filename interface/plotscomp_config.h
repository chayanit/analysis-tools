#ifndef Analysis_Tools_plotscomp_config_h_
#define Analysis_Tools_plotscomp_config_h_

#include "boost/program_options.hpp"
#include "boost/algorithm/string.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

int plotscomp_config(int argc, char * argv[]);

//std::string cfg_;

// if file, give up to 4 histograms
std::vector<std::string> files_;
std::vector<std::string> histos_;
std::vector<std::string> titles_;

std::string file_;
std::string hist1_;
std::string hist2_;

std::string hist_;
std::string file1_;
std::string file2_;

std::string title_;
std::string legend1_;
std::string legend2_;

std::string outplot_;

float min_,max_;

float scale_;

bool logx_;
bool logy_;
bool app_;



int plotscomp_config(int argc, char * argv[])
{
   app_ = true;
   logx_ = false;
   logy_ = false;
   try
   {
      namespace po = boost::program_options;
      po::options_description desc("Options");
      desc.add_options()
         ("help,h"  , "Show help messages")
         ("bkg,b"   , "Run in the background")
         ("logx"    , "x-axis in log scale")
         ("logy"    , "y-axis in log scale")
         ("files"   , po::value<std::vector<std::string> >(&files_)->multitoken(),"list of files")
         ("histos"  , po::value<std::vector<std::string> >(&histos_)->multitoken(),"list of histograms")
         ("titles"  , po::value<std::vector<std::string> >(&titles_)->multitoken(),"titles of histograms")
         ("title,t" , po::value <std::string> (&title_)  -> default_value("")  , "Title of the histogram")
         ("legend1" , po::value <std::string> (&legend1_)-> default_value("histo 1")  , "Legend of the histogram 1")
         ("legend2" , po::value <std::string> (&legend2_)-> default_value("histo 2")  , "Legend of the histogram 2")
         ("scale,s" , po::value <float>       (&scale_)  -> default_value(1)   , "Scale factor for the 2nd histogram")
         ("min"     , po::value <float>       (&min_)    -> default_value(-1)  , "Min value histogram range")
         ("max"     , po::value <float>       (&max_)    -> default_value(-2)  , "Max value histogram range")
         ("file,f"  , po::value <std::string> (&file_)   -> default_value("")  , "File with up to 2 histograms to compare")
         ("hist1"   , po::value <std::string> (&hist1_ ) -> default_value("")  , "1st histogram name in file")
         ("hist2"   , po::value <std::string> (&hist2_ ) -> default_value("")  , "2nd histogram name in file")
         ("hist,h"  , po::value <std::string> (&hist_)   -> default_value("")  , "Histogram name common to the different files")
         ("file1"   , po::value <std::string> (&file1_)  -> default_value("")  , "1st file with 1 histogram to compare")
         ("file2"   , po::value <std::string> (&file2_)  -> default_value("")  , "2nd file with 1 histogram to compare")
         ("output"  , po::value <std::string> (&outplot_)-> default_value("")  , "name of the output plot");
         
//         ("config,c",po::value<std::string>(&cfg_),"Configuration file name");
      
//       po::options_description config("Configuration");
//       config.add_options()
//          ("file",po::value  <std::string> (&file_) ->default_value(""),"File with up to 2 histograms to compare")
//          ("hist",po::value  <std::string> (&hist_) ->default_value(""),"Histogram name common to the different files");
      
      po::variables_map vm; 
      try
      {
         po::store(po::parse_command_line(argc, argv, desc), vm); // can throw
         // --help option
         
         if ( vm.count("help") )
         {
            std::cout << "PlotsCompare macro" << std::endl
                      << desc   << std::endl;
//                      << config << std::endl;
            return 1;
         }
         if ( vm.count("logx") )  logx_ = true;
         if ( vm.count("logy") )  logy_ = true;
         if ( vm.count("bkg") )   app_ = false;
         
         po::notify(vm);
         
//          std::ifstream cfg_s(cfg_.c_str());
//          po::store(po::parse_config_file(cfg_s, config), vm); // can throw
//          if ( vm.count("config") )
//          {
// 
//          }
         po::notify(vm);
      }
      catch(po::error& e)
      { 
         std::cerr << "ERROR: " << e.what() << std::endl << std::endl; 
         std::cerr << desc << std::endl; 
         return -1; 
      } 
      
   }
   catch(std::exception& e) 
   { 
      std::cerr << "ERROR: " << e.what() << std::endl << std::endl; 
      return -1; 
   } 
   
   return 0;
}

   
#endif
