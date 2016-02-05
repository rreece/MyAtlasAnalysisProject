#
# file: setup.sh
# author: Ryan Reece <ryan.reece@cern.ch>
# created: August 17, 2012
#
# Setup script for a release of MyAtlasAnalysisProject
#
###############################################################################


##-----------------------------------------------------------------------------
## pre-setup helpers, don't touch

path_of_this_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
path_above_this_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )/../" && pwd )"
export ANALYSIS_PROJECT=${path_of_this_dir}

add_to_python_path()
{
    export PYTHONPATH=$1:$PYTHONPATH
    echo "  Added $1 to your PYTHONPATH."
}

add_to_path()
{
    export PATH=$1:$PATH
    echo "  Added $1 to your PATH."
}


##-----------------------------------------------------------------------------
## setupATLAS
echo "  Running setupATLAS."
setupATLAS

localSetupGcc gcc484_x86_64_slc6
localSetupPython 2.7.4-x86_64-slc6-gcc48
localSetupROOT 6.02.12-x86_64-slc6-gcc48-opt

if [ -f rcSetup.sh ]; then
    source rcSetup.sh
else
#    rcSetup Base,2.3.34
    rcSetup SUSY,2.3.39
fi 

echo "  done."


##-----------------------------------------------------------------------------
## setup PYTHONPATH

echo "  Setting up your PYTHONPATH."
add_to_python_path ${ANALYSIS_PROJECT}
#add_to_python_path ${ANALYSIS_PROJECT}/pyutils
echo "  done."


##-----------------------------------------------------------------------------
## setup PATH

echo "  Setting up your PATH."
#add_to_path ${ANALYSIS_PROJECT}/SUSYDiphoton2015/scripts
#add_to_path ${ANALYSIS_PROJECT}/root2html
#add_to_path ${ANALYSIS_PROJECT}/samples/scripts
echo "  done."


