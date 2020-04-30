class Jets:
    # constructor
    def __init__(self, tree):
        self.__tree = tree
        self.__n = tree.n
        self.__jets = []
       
        for j in range(self.n):
            jet = Jet()
            jet.pt = tree.pt[j]
            jet.eta = tree.eta[j]
            jet.phi = tree.phi[j]
            jet.e = tree.e[j]
            jet.btag("deepflavour","light",tree.btag_dflight[j])
            jet.btag("deepflavour","g",tree.btag_dfg[j])
            jet.btag("deepflavour","c",tree.btag_dfc[j])
            jet.btag("deepflavour","b",tree.btag_dfb[j])
            jet.btag("deepflavour","bb",tree.btag_dfbb[j])
            jet.btag("deepflavour","lepb",tree.btag_dflepb[j])
            jet.btag("deepcsv","light",tree.btag_deeplight[j])
            jet.btag("deepcsv","c",tree.btag_deepc[j])
            jet.btag("deepcsv","cc",tree.btag_deepcc[j])
            jet.btag("deepcsv","b",tree.btag_deepb[j])
            jet.btag("deepcsv","bb",tree.btag_deepbb[j])
             
            self.__jets.append(jet)            
    
    
    
    @property
    def tree(self):
        return self.__tree
    
    @property
    def n(self):
        return self.__n
    
    @property
    def jets(self):
        return  self.__jets
    
 
class Jet:
    def __init__(self):
        self.__pt = -1000.
        self.__eta = -1000.
        self.__phi = -1000.
        self.__e = -1000.
        self.__btag = {}
        self.__btag["deepflavour"] = {}
        self.__btag["deepcsv"] = {}
        
#
    @property
    def pt(self):
        return self.__pt
    @pt.setter
    def pt(self,x):
        self.__pt = x
#       
    @property
    def eta(self):
        return self.__eta
    @eta.setter
    def eta(self,x):
        self.__eta = x
#
    @property
    def phi(self):
        return self.__phi
    @phi.setter
    def phi(self,x):
        self.__phi = x
#       
    @property
    def e(self):
        return self.__e
    @e.setter
    def e(self,x):
        self.__e = x
#       
    
    def btag(self,algo,prob=None,x=None):
        if not (prob and x):
            return self.__btag[algo]
        else:
            self.__btag[algo][prob] = x
       
