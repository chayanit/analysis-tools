import utils
utils.modules_path()
import ROOT
from ROOT import TH1F, TCanvas, TLorentzVector
from AnalysisData import AnalysisData
from Triggers import TriggerResults
from Jets import Jets

class Analyzer:
    
    def __init__(self,data = None):
        self.__data = data
        self.__histo = {}
        self.__pt = []
        self.__eta = []
        self.__phi = []
        self.__e = []
        self.__pt2 = []
        self.__eta2 = []
        self.__phi2 = []
        self.__e2 = []
        self.__M = []
        
    @property
    def data(self):
        return self.__data
        
    def plot(self,Pname, jamount = None):
            if not jamount:
                jamount = self.__data.nentries
            
            h_pt = {}
            h_eta = {}
            h_phi = {}
            h_e = {}
            
            for j in range(1,5):
                h_pt["jet{}".format(j)] = self.createHistogram("jet{}_pt".format(j),Pname)
                h_eta["jet{}".format(j)] = self.createHistogram("jet{}_eta".format(j),Pname)
                h_phi["jet{}".format(j)] = self.createHistogram("jet{}_phi".format(j),Pname)
                h_e["jet{}".format(j)] = self.createHistogram("jet{}_e".format(j),Pname)

            
            h_dimass = TH1F(Pname+"_dijet_mass", "", 100, 0, 1000)
            for i in range(jamount):
                self.__data.entry(i)
                jets = self.__data.jets.jets
                
                nmax = 4
                if len(jets) < 4:
                    nmax = len(jets)
                for j in range(1,nmax+1):
                    jet = jets[j-1]
                    h_pt["jet{}".format(j)].Fill(jet.pt)
                    h_eta["jet{}".format(j)].Fill(jet.eta)
                    h_phi["jet{}".format(j)].Fill(jet.phi)
                    h_e["jet{}".format(j)].Fill(jet.e)
                    
                particle_a = self.p4(jets[0])
                particle_b = self.p4(jets[1])
            
                self.__M = particle_a + particle_b
                h_dimass.Fill(self.__M.M())
                
            print("before")
            for j in range(1,5):
                self.__histo["Jet{} pt".format(j)] =  h_pt["jet{}".format(j)]
                self.__histo["Jet{} eta".format(j)] =  h_eta["jet{}".format(j)]
                self.__histo["Jet{} phi".format(j)] =  h_phi["jet{}".format(j)]
                self.__histo["Jet{} e".format(j)] =  h_e["jet{}".format(j)]
            print("after")
                      
            self.__histo["Dijet Mass"] = h_dimass
                
            return self.__histo
            
    @property
    def see(self):
        return self.__histo
    
    @property
    def mass(self):
        return print(self.__M.M())
    
    
    def createHistogram(self,hname,pname):
        var = hname.split("_")[1]
        bins = {}
        bins['pt'] = [100,0,1000]
        bins['eta'] = [100,-5,5]
        bins['phi'] = [100,-3.15,3.15]
        bins['e'] = [100,0,1000]
        
        h = TH1F(pname+'_'+hname, "", bins[var][0], bins[var][1],bins[var][2])
        return h
    
    def p4(self,jet):
        p4 = TLorentzVector()
        p4.SetPtEtaPhiE(jet.pt, jet.eta, jet.phi, jet.e)
        
        return p4 
        