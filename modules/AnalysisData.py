import ROOT
from ROOT import TFile, TTree, TCanvas,TFileCollection,TChain,TH1F
from Jets import Jets
from Triggers import TriggerResults

class AnalysisData:
    # constructor
    def __init__(self, inputlist, eventpath="MssmHbb/Events/"):
        self.__inputlist = inputlist
        self.__eventPath = eventpath
        self.__tree = {}
        self.__fileCollection = TFileCollection("fileCollection","",self.__inputlist)
        self.__fileList = self.__fileCollection.GetList()

        if self.__eventPath[-1] != "/":
           self.__eventPath += "/"

        self.__tree["EventInfo"] = TChain(self.eventPath+"EventInfo")
        self.__tree["EventInfo"].AddFileInfoList(self.fileList)
        
        self.__nentries = self.__tree["EventInfo"].GetEntries()
        
    
    @property
    def tree(self):
        return self.__tree

    @property
    def fileList(self):
        return self.__fileList

    @property
    def eventPath(self):
        return self.__eventPath

    @ property
    def nentries(self):
        return self.__nentries


    def entry(self,i):
        self.__tree["EventInfo"].GetEntry(i)
      
    @property
    def event(self):
        return self.__tree["EventInfo"].event

    @property
    def run(self):
        return self.__tree["EventInfo"].run

    @property
    def lumiSection(self):
        return self.__tree["EventInfo"].lumisection
    
    @property
    def bunchCrossing(self):
        return self.__tree["EventInfo"].bx
    
    @property
    def pileup(self):
        return self.__tree["EventInfo"].nPileup
    
    @property
    def truePileup(self):
        return self.__tree["EventInfo"].nTruePileup
    
    @property
    def generatorWeight(self):
        return self.__tree["EventInfo"].genWeight
    
    @property
    def generatorScale(self):
        return self.__tree["EventInfo"].genScale
    
    def __addTree(self,unique_name,tree_name):
        self.__tree[unique_name] = TChain(self.eventPath+tree_name)
        self.__tree[unique_name].AddFileInfoList(self.fileList)
        self.__tree["EventInfo"].AddFriend(self.__tree[unique_name])

### Trigger Results
    def addTriggerResults(self,tree_name="TriggerResults"):
        self.__addTree("TriggerResults",tree_name)
        return self.__tree["TriggerResults"]
          
    @property
    def triggerResults(self):
        return TriggerResults(self.__tree["TriggerResults"])

### Jets
    def addJets(self,tree_name):
        self.__addTree("Jets",tree_name)
        return self.__tree["Jets"]
   
    @property
    def jets(self):
        return Jets(self.__tree["Jets"])