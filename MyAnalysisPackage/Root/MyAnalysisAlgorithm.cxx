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
#include "SUSYTools/SUSYObjDef_xAOD.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODEgamma/PhotonAuxContainer.h"


// this is needed to distribute the algorithm to the workers
ClassImp(MyAnalysisAlgorithm)


//-----------------------------------------------------------------------------
MyAnalysisAlgorithm :: MyAnalysisAlgorithm ()
{
    m_photons = 0;
    m_photons_aux = 0;
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
    tree->Branch("ph_eta",     &o_ph_eta);
    tree->Branch("ph_phi",     &o_ph_phi);
    // USER TODO: Add more variables here.

    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisAlgorithm :: user_clear_cache_variables()
{
    const char *FUNC_NAME = "user_clear_cache_variables";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

    SAFE_DELETE(m_photons);
    SAFE_DELETE(m_photons_aux);
    // USER TODO: Add more tools and collections here.

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
    o_ph_eta        .clear();
    o_ph_phi        .clear();
    // USER TODO: Add more variables here.

    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisAlgorithm :: user_initialize()
{
    const char *FUNC_NAME = "user_initialize";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

    // USER TODO: Write.

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
    
    if(c_debug) Info(FUNC_NAME, "Getting photons...");
    EL_RETURN_CHECK(FUNC_NAME, m_SUSYTool->GetPhotons(m_photons, m_photons_aux, false, "Photons"));

    // Filling photon variables
    o_ph_n = 0;
    for(auto photon : *m_photons)
    {
        float abs_etas2 = abs(photon->caloCluster()->etaBE(2));
        if( ((abs_etas2 >= 1.37)&&(abs_etas2 < 1.52)) || (abs_etas2>2.37) ) continue;
        if(photon->pt() < 20000.) continue;
        if(!photon->auxdata<char>("baseline")) continue;

        o_ph_pt     .push_back( photon->pt() );
        o_ph_eta    .push_back( photon->eta() );
        o_ph_phi    .push_back( photon->phi() );
        o_ph_n++;
    }

    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisAlgorithm :: user_finalize()
{
    const char *FUNC_NAME = "user_finalize";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

    // USER TODO: Write.

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
    return o_ph_n >= 2;
}


