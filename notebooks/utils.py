import os
import sys
cwd = os.getcwd()
sys.path.insert(1, cwd+"/../modules")

def modules_path(path=None):
    if not path:
       cwd = os.getcwd()
       sys.path.insert(1, cwd+"/../modules")
    else:
       sys.path.insert(1, path)
