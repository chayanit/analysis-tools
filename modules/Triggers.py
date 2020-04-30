class TriggerResults:
    # constructor
    def __init__(self, tree):
        self.__tree = tree    
    
    @property
    def tree(self):
        return self.__tree
    
    def hlt(self,hlt):
        res = False
        try:
            res = bool(int(self.tree.GetLeaf(hlt).GetValue()))
        except ReferenceError:
            print("\n--->> Trigger ",hlt,"does not exist! <<---\n")
            raise SystemExit(-1)
        return res
       
