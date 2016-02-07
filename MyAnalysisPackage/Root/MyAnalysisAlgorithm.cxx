// MyAnalysisAlgorithm.cxx
// author: Ryan Reece <ryan.reece@cern.ch>
// created: 2016-02-04
//=============================================================================

// this package
#include "MyAnalysisPackage/MyAnalysisAlgorithm.h"
#include "MyAnalysisPackage/MyAnalysisMacros.h"

// std includes
#include <vector>

// ROOT includes
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"

// ATLAS framework


// this is needed to distribute the algorithm to the workers
ClassImp(MyAnalysisAlgorithm)


//-----------------------------------------------------------------------------
MyAnalysisAlgorithm :: MyAnalysisAlgorithm ()
{

}



//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisAlgorithm :: user_initialize_hists()
{
    const char *FUNC_NAME = "user_initialize_hists";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

    // USER TODO: Optionally declare any output histograms you want to make.
    // h_myhist = new TH1F("h_myhist", "h_myhist", 10, 0.0, 100.0);
    // wk()->addOutput(h_myhist); // adding histogram to the outputstream (releasing ownership)

    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisAlgorithm :: user_declare_branches(TTree* tree)
{
    const char *FUNC_NAME = "user_declare_branches";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

    tree->Branch("ph_n",       &o_ph_n);
    tree->Branch("ph_pt",      &o_ph_pt);
    // USER TODO: Add more variables here.

    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisAlgorithm :: user_clear_output_variables()
{
    const char *FUNC_NAME = "user_clear_output_variables";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

    o_ph_n          = 0;
    o_ph_pt         .clear();
    // USER TODO: Add more variables here.

    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisAlgorithm :: user_preprocess_event()
{
    const char *FUNC_NAME = "user_preprocess_event";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

    // USER TODO: Write.

    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisAlgorithm :: user_process_event()
{
    const char *FUNC_NAME = "user_process_event";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

    // USER TODO: Do some calculations event-by-event here.
    o_ph_n = 1; // TODO
    o_ph_pt.push_back(100.0); // TODO

    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
bool MyAnalysisAlgorithm :: user_check_preskim()
{
    // USER TODO: Write a preskim condition.
    return true;
}


//-----------------------------------------------------------------------------
bool MyAnalysisAlgorithm :: user_check_skim()
{
    // USER TODO: Write a skim condition.
    return true;
}


