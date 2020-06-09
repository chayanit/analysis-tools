# Analysis/Tools package

**Core Analysis Framework of the DESY CMS Higgs -> bb group**

See also the code documentation [Doxygen](https://www.desy.de/~walsh/analysis-framework/doxygen/pro) page


:warning: This package is supposed to contain only general codes to be used for analysis.
Codes for specific analysis must be developed in dedicated packages/repositories, e.g.
for the MSSM Hbb analyses one can use for developments the package
[Analysis/MssmHbb](https://github.com/desy-cms/analysis-mssmhbb),
which is currently under construction.


* [Installation](#installation)
* [Calibrations](#calibrations)
* [Ntuples](#ntuples)
* [Example](#example)
* [Detailed description](#detailed-description)



## Installation

The codes as well as the ntuples are independent of CMSSW. However, in order to compile it uses `scram`.
So the latest version in any architecture should be fine.

```bash
cmsrel CMSSW_X_Y_Z
cd CMSSW_X_Y_Z/src
cmsenv
git clone https://github.com/desy-cms/analysis-tools.git Analysis/Tools
scram b -j4 USER_CXXFLAGS="-Wno-misleading-indentation"
```

:zzz: The `USER_CXXFLAGS="-Wno-misleading-indentation"` prevents a large number of warnings
from misleading identation in modules of the boost library. User can also define the
environment variable below in `.bashrc` or every time after the command `cmsenv`
```bash
export USER_CXXFLAGS="-Wno-misleading-indentation"
```

## Calibrations

Scale factors, efficiencies etc can be obtained from the [analysis-calibrations](https://github.com/desy-cms/analysis-calibrations) repository (see also the README.md in each run period directory). It is recommended to install it in the Analysis/Tools/data directory, for the framework loads the calibrations files from that path

```bash
cd $CMSSW_BASE/src/Analysis/Tools/data
git clone https://github.com/desy-cms/analysis-calibrations.git calibrations
```

## Ntuples

The lists of ntuples files can be obtained from the [analysis-ntuples](https://github.com/desy-cms/analysis-ntuples.git) repository (see also the README.md in each run period directory). The repository can be installed in a directory of your convenience, e.g.

```bash
cd $CMSSW_BASE/src/Analysis/Tools/test
git clone https://github.com/desy-cms/analysis-ntuples.git ntuples
```

## Example

A simple example macro can be found in [Analysis/Tools/bin/AnalyserSimpleExample.cc](bin/AnalyserSimpleExample.cc).
The example is a simple offline and online dijet selection (+ muon selection in the semileptonic case)
using signal MC samples and triggers from 2017 and 2018 periods. The macro uses a configuration file as
input. Configuration files are avaialble for both the all-hadronic and semileptonic cases for both 2017
and 2018 periods:
* [analyser_example_allhad_2017.cfg](test/analyser_example_allhad_2017.cfg)
* [analyser_example_allhad_2018.cfg](test/analyser_example_allhad_2018.cfg)
* [analyser_example_semilep_2017.cfg](test/analyser_example_semilep_2017.cfg)
* [analyser_example_semilep_2018.cfg](test/analyser_example_semilep_2018.cfg)

To execute an example
```bash
cd Analysis/Tools/test
AnalyserSimpleExample -c analyser_example_semilep_2018.cfg
```

### Detailed Description

#### Creating a macro

##### Main

In the macro the first thing done is the main `Analyser` instantiation

```cpp
#include "Analysis/Tools/interface/Analyser.h"
using namespace std;
using namespace analysis;
using namespace analysis::tools;

int main(int argc, char ** argv)
{
   TH1::SetDefaultSumw2();
   Analyser analyser(argc,argv);
...   
```

The `[Info]` block in the `.cfg` file
```
[Info]
process = MssmHbb
eventsMax = -1
ntuplesList = rootFileList.txt
isMC = true
output = histograms.root
```
passes general information to initialise the `Analyser` in the macro. The `process` parameter is the name of the parent `TDirectoryFile` in the ntuple. To find it
```bash
root -l ntuple.root
# in the root prompt type
.ls
# it will show
KEY: TDirectoryFile	MssmHbb;1	MssmHbb
```
Hopefully the other names of the config parameters are self-explanatory.

In **data** one must specify the json file with certified lumis, e.g.
```
[Info]
...
isMC = false
json = certified_json.txt
```

##### Event loop

The event loop must start like this
```cpp
for ( int i = 0 ; i < analyser.nEvents() ; ++i )
{
   if ( ! analyser.event(i) )   continue;
   // do actions and selections
}
```
where the `analyser.event(i)` reads the event from the ntuple and performs some actions, such as applying generator event weights in MC and JSON certified lumis selection in data.

##### Selections

The `Analyser` has several predefined selection methods that reads parameters from the configuration file and apply to the event. The selections must be within the event loop.

###### Jets

For example, if the analysis involves jets, one must define which collection to be used, the minimum number of jets, the jet identification, the jet pT etc. In the configuration there is a block for jets with the relevant parameters
```
[Jets]
jets = updatedPatJets
nMin = 2
id = tight
puId = loose
ptMin = 60
ptMin = 50
etaMax = 2.2
etaMax = 2.2
extendedFlavour = true
```
:warning: Parameters such as ptMin or etaMax are vectors, so the order in which they are put in the configuration makes a difference, so the first entry corresponds to the leading object, the second entry to the second leading object and so on.

In the macro, the selections are performed within the event loop calling the methods, which automatically reads the parameters from the configuration, e.g.
```cpp
      // jet identification selection
      if ( ! analyser.selectionJetId()          )   continue;
      if ( ! analyser.selectionJetPileupId()    )   continue;
      if ( ! analyser.selectionNJets()          )   continue;
```
This will prepare a list of jets containing only jets passing the required identification criteria and with a certain number of jets defined in the configuration.

```cpp
      //  1st and 2nd jet kinematic selection, pt and eta
      if ( ! analyser.selectionJet(1)          )   continue;
      if ( ! analyser.selectionJet(2)          )   continue;

```
In the `Analyser::selectionJet` method the argument is the jet rank, i.e., `1` refers to the leading jets, `2` refers to the second leading jet. the method will select the jet according to its pt and eta criteria defined in the configuration.

###### b-tag

For the b-tagging there is a block in the configuration file, where one defines the algorithm, working points etc.

```
[BTag]
nMin  = 2
wp = medium
wp = medium
algorithm = deepflavour
loose  = 0.0494
medium = 0.2770
tight  = 0.7264
```
With this configuration, one intends to select events with at least the two leading jets tagged with medium working point using the deepflavour algorithm. The thresholds of the working points myst be specified.

To perform the selection in the event loop:

```cpp
      if ( ! analyser.selectionBJet(1)         )   continue;
      if ( ! analyser.selectionBJet(2)         )   continue;

```
where the argument of the `Analyser::selectionBJet` is the jet rank as in the jet kinematic selection above.

###### Muons

Muon selection has its own configuration block and the procedure works in a similar way as jets

```
[Muons]
muons = slimmedMuons
nMin = 1
id = tight
ptMin = 13.
etaMax = 2.2
```

The selection code can be for example

```cpp
      // muon identification selection
      if ( ! analyser.selectionMuonId()         )   continue;
      if ( ! analyser.selectionNMuons()         )   continue;
      // muon kinematic selection
      if ( ! analyser.selectionMuons()          )   continue;
```
:warning: The  method `Analyser::selectionMuons` for muon kinematic selection differs from the one used for jet kinematic selection, for it makes a list of muons passing the required kinematic selection and the event is good if there is at list the minimum number of jets required. This method is useful for the muon-jet association in analyses requiring jets containing a muon.

If one wants to select muons depending on their ranking, then one can use, like in the jets case,
```cpp
      // leading muon kinematic selection
      if ( ! analyser.selectionMuon(1)          )   continue;
```


##### Histograms

Histograms can be predefined in the `Analyser`, e.g. for jets one can use the method `jetHistograms`, which receives the arguments `number of jets`, which must be at most the minimum number of jets required, and a name of a directory. One can create as many directories as needed and fill the histograms at different steps of the analysis workflow, e.g. before the event loop:
```cpp
   analyser.jetHistograms(2,"initial");
   analyser.jetHistograms(2,"final");
```
then within the event loop:
```cpp
...
for ( int i = 0 ; i < analyser.nEvents() ; ++i )
{
   if ( ! analyser.event(i) )   continue;
   // do something
   analyser.fillJetHistograms("initial");
   // do something else
   analyser.fillJetHistograms("final");
...
```
