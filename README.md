MyAtlasAnalysisProject
===============================================================================

A skeleton package for developing code for analyzing xAOD data from the
ATLAS experiment at CERN.

-   author:  Ryan Reece <ryan.reece@cern.ch>
-   created: Feb 3, 2016

This package helps users quickly get a framework up and running for reading
xAODs, making corrections with ATLAS SUSYTools, and saving some custom subset
of data in a flat (not xAOD) output ntuple for further analysis.

Users should mainly have to implement something like `MyAnalysisAlgorithm`
and its functions, inheritting the basic helpers and overhead from
`MyAnalysisBaseAlgorithm`, which should not require much if any edits.
Please see the comments marked `USER TODO` in `MyAnalysisPackage/MyAnalysisAlgorithm.h`
and `Root/MyAnalysisAlgorithm.cxx`.


Getting started
-------------------------------------------------------------------------------

Setup:

    source setup.sh

Compiling:

    rc clean
    rc find_packages
    rc compile

Running:
    
    mkdir run
    cd run
    MyNtupleMaker Zee DAOD_EXOT10.root


See also
-------------------------------------------------------------------------------

-   <https://twiki.cern.ch/twiki/bin/view/AtlasComputing/SoftwareTutorialxAODAnalysisInROOT>
-   <https://twiki.cern.ch/twiki/bin/view/AtlasProtected/SusyObjectDefintions>
-   <https://twiki.cern.ch/twiki/bin/view/AtlasProtected/SUSYToolsV6>
-   <https://svnweb.cern.ch/trac/atlasoff/browser/PhysicsAnalysis/SUSYPhys/SUSYTools/tags?order=name>


How to use git
-------------------------------------------------------------------------------

Checkout this:

    git clone https://github.com/rreece/MyAtlasAnalysisProject.git

Helper aliases:

To make common git commands more simple, you can define the following aliases.
Open your `~/.bashrc` file and add the following lines:

    ## git helper aliases
    alias git-st="git status"
    alias git-ci="git commit -a -m "
    alias git-push="git push -u origin master"
    alias git-pull="git pull"
    alias git-add="git add"
    alias git-last="git log -1 HEAD"

With these defined, you can check the status by

    git-st

commit (AKA "check-in") changes by

    git-ci "put comment here"

push your committed changes by

    git-push

pull others' changes by

    git-pull


