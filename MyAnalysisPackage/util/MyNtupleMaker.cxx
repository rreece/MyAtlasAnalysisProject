// MyNtupleMaker.cxx
// author: Ryan Reece <ryan.reece@cern.ch>
// created: 2016-02-04
//=============================================================================

// std includes
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

// ROOT includes
#include "TFile.h"
#include "TTree.h"

// ATLAS framework
#include "xAODRootAccess/Init.h"
#include "SampleHandler/Sample.h"
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/ScanDir.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
#include "SampleHandler/DiskListLocal.h"
#include <TSystem.h>
#include "TChain.h"

// this package
#include "MyAnalysisPackage/MyAnalysisAlgorithm.h"


//-----------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
    if( argc <= 2 )
    {
        std::cout << "MyNtupleMaker ERROR: Not enough arguments given!!!!\n";
        return 0;
    }

    // sample_name
    std::string sample_name = argv[1];

    // submitDir
    std::string submitDir = "MyNtupleMaker_" + sample_name;

    // Set up the job for xAOD access:
    xAOD::Init().ignore();

    // Construct the samples to run on:
    SH::SampleHandler sh;

    // use SampleHandler to scan all of the subdirectories of a directory for particular MC single file:
    //const char* inputFilePath = gSystem->ExpandPathName("/afs/cern.ch/work/m/mbaugh/public/NP_Photon_Analysis/mc15_13TeV.402429.MadGraphPythia8EvtGen_A14NNPDF23LO_GMSB_SPS8_300_2.merge.DAOD_STDM2.e4378_s2608_r6869_r6282_p2421/");
    //SH::ScanDir().filePattern("DAOD_*.root*").scan(sh, inputFilePath);

    TChain chain("CollectionTree");
    for(int i=2; i<=argc; i++)
    {
        chain.Add(argv[i]);
    }

    SH::Sample* tmp_sample = SH::makeFromTChain(sample_name.c_str(), chain);
    sh.add(tmp_sample);

    // Set the name of the input TTree. It's always "CollectionTree"
    // for xAOD files.
    sh.setMetaString( "nc_tree", "CollectionTree" );

    // Print what we found:
    sh.print();

    // Create an EventLoop job:
    EL::Job job;
    job.sampleHandler( sh );
    job.options()->setDouble (EL::Job::optMaxEvents, -1);

    // Add our analysis to the job:
    MyAnalysisAlgorithm* alg = new MyAnalysisAlgorithm();
    job.algsAdd( alg );

    // Run the job using the local/direct driver:
    EL::DirectDriver driver;
    driver.submit( job, submitDir );

    return 0;
}


