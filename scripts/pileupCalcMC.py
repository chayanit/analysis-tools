#!/usr/bin/env python

# pileupCalcMC.py 

from argparse import ArgumentParser
from importlib import import_module
from ROOT import TH1D
from ROOT import TFile

# parsing arguments
parser = ArgumentParser()
parser.add_argument("--pu"   , dest="pileup"                                    , help="Pileup from mixing module")
parser.add_argument("--n"    , dest="nbins" , type=int, default=100             , help="number of bins")
parser.add_argument("--min"  , dest="min"   , type=int, default=0               , help="Min pileup")
parser.add_argument("--max"  , dest="max"   , type=int, default=100             , help="Max pileup")
parser.add_argument("--out"  , dest="out"   , default="MyMCPileupHistogram.root", help="output")
args = parser.parse_args()
if not args.pileup:
   print "nothing to be done" 
   quit()
   
puconfig = import_module(args.pileup)

bins = puconfig.mix.input.nbPileupEvents.probFunctionVariable
values = puconfig.mix.input.nbPileupEvents.probValue


hPU = TH1D("pileup",  "true pileup", args.nbins, args.min, args.max)

for i in range(1, args.nbins+1):
   try: 
      hPU.SetBinContent(i,values[bins.index(hPU.GetBinLowEdge(i))])
   except ValueError as error:
      hPU.SetBinContent(i,0)
      
fout = TFile(args.out,"recreate")
hPU.Write()
fout.Close()

