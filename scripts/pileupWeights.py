#!/usr/bin/env python

# pileupCalcMC.py 

from argparse import ArgumentParser
from ROOT import TH1D
from ROOT import TH1F
from ROOT import TFile

# parsing arguments
parser = ArgumentParser()
parser.add_argument("--data"   , dest="data"   , help="file containing data pileup")
parser.add_argument("--mc"     , dest="mc"     , help="file containing mc pileup")
parser.add_argument("--up1"    , dest="up1"    , help="file containing data pileup 1sigma up var")
parser.add_argument("--down1"  , dest="down1"  , help="file containing data pileup 1sigma down var")
parser.add_argument("--up2"    , dest="up2"    , help="file containing data pileup 2sigma up var")
parser.add_argument("--down2"  , dest="down2"  , help="file containing data pileup 2sigma down var")
parser.add_argument("--output" , dest="out"    , help="name ouf output file", default = "PileupWeight.root")

args = parser.parse_args()
if not ( args.data and args.mc ):
   print "nothing to be done" 
   quit()

f_data = TFile(args.data,"old")
h_data = TH1D(f_data.Get("pileup"))
h_data.Scale(1./h_data.Integral())

f_mc   = TFile(args.mc,"old")
try:
   h_mc   = TH1D(f_mc.Get("pileup"))
except TypeError as error:
   h_mc   = TH1F(f_mc.Get("pileup"))

h_mc.Scale(1./h_mc.Integral())

h_weight = TH1D(h_data.Clone("weight"));
h_weight.Divide(h_mc);

if args.up1:
   f_up1 = TFile(args.up1,"old")
   h_up1 = TH1D(f_up1.Get("pileup"))
   h_up1.SetName("weight_1up")
   h_up1.Scale(1./h_up1.Integral())
   h_up1.Divide(h_mc)

if args.down1:
   f_down1 = TFile(args.down1,"old")
   h_down1 = TH1D(f_down1.Get("pileup"))
   h_down1.SetName("weight_1down")
   h_down1.Scale(1./h_down1.Integral())
   h_down1.Divide(h_mc)

if args.up2:
   f_up2 = TFile(args.up2,"old")
   h_up2 = TH1D(f_up2.Get("pileup"))
   h_up2.SetName("weight_2up")
   h_up2.Scale(1./h_up2.Integral())
   h_up2.Divide(h_mc)

if args.down2:
   f_down2 = TFile(args.down2,"old")
   h_down2 = TH1D(f_down2.Get("pileup"))
   h_down2.SetName("weight_2down")
   h_down2.Scale(1./h_down2.Integral())
   h_down2.Divide(h_mc)


fout = TFile(args.out,"recreate")
h_weight.Write()
if args.up1:
   h_up1.Write()
if args.down1:
   h_down1.Write()
if args.up2:
   h_up2.Write()
if args.down2:
   h_down2.Write()
   
   
fout.Close()

