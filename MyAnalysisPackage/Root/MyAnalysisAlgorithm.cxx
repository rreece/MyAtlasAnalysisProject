// MyAnalysisAlgorithm.cxx
// author: Ryan Reece <ryan.reece@cern.ch>
// created: 2016-02-04
//=============================================================================

// std includes
#include <vector>

// ROOT includes
#include "TFile.h"
#include "TTree.h"

// ATLAS framework
#include "EventLoop/Job.h"
#include "EventLoop/StatusCode.h"
#include "EventLoop/Worker.h"
#include "EventLoop/OutputStream.h"
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/tools/Message.h"

// this package
#include "MyAnalysisPackage/MyAnalysisAlgorithm.h"

// Helper macro for checking xAOD::TReturnCode return values
#define EL_RETURN_CHECK( CONTEXT, EXP )                 \
    do {                                                \
        if( ! EXP.isSuccess() ){                        \
            Error( CONTEXT,                             \
                XAOD_MESSAGE( "Failed to execute: %s"), \
                #EXP );                                 \
            return EL::StatusCode::FAILURE;             \
        }                                               \
    } while( false ) 

// this is needed to distribute the algorithm to the workers
ClassImp(MyAnalysisAlgorithm)


//-----------------------------------------------------------------------------
MyAnalysisAlgorithm :: MyAnalysisAlgorithm ()
{
    // Here you put any code for the base initialization of variables,
    // e.g. initialize all pointers to 0.  Note that you should only put
    // the most basic initialization here, since this method will be
    // called on both the submission and the worker node.  Most of your
    // initialization code will go into histInitialize() and
    // initialize().

    m_output_stream_name = "output";
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisAlgorithm :: setupJob (EL::Job& job)
{
    // Here you put code that sets up the job on the submission object
    // so that it is ready to work with your algorithm, e.g. you can
    // request the D3PDReader service or add output files.  Any code you
    // put here could instead also go into the submission script.  The
    // sole advantage of putting it here is that it gets automatically
    // activated/deactivated when you add/remove the algorithm from your
    // job, which may or may not be of value to you.

    //Let's initialize the algorithm to use the xAODRootAccess package
    job.useXAOD ();
    //xAOD::Init();  //call before opening first file

    EL_RETURN_CHECK( "setupJob()", xAOD::Init() ); //call before opening first file
    EL::OutputStream stream(m_output_stream_name.Data());
    job.outputAdd(stream);

    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisAlgorithm :: histInitialize ()
{
    // Here you do everything that needs to be done at the very
    // beginning on each worker node, e.g. create histograms and output
    // trees.  This method gets called before any input files are
    // connected.
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisAlgorithm :: fileExecute ()
{
    // Here you do everything that needs to be done exactly once for every
    // single file, e.g. collect a list of all lumi-blocks processed
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisAlgorithm :: changeInput (bool firstFile)
{
    // Here you do everything you need to do when we change input files,
    // e.g. resetting branch addresses on trees.  If you are using
    // D3PDReader or a similar service this method is not needed.
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisAlgorithm :: initialize ()
{
    // Here you do everything that you need to do after the first input
    // file has been connected and before the first event is processed,
    // e.g. create additional histograms based on which variables are
    // available in the input files.  You can also create all of your
    // histograms and trees in here, but be aware that this method
    // doesn't get called if no events are processed.  So any objects
    // you create here won't be available in the output if you have no
    // input events.
    xAOD::TEvent* event = wk()->xaodEvent();

    //as a check, let's see the number of events in our xAOD
    Info("initialize()", "Number of events = %lli", event->getEntries() ); //print long long int

    // >>>>> ADD INITIALIZATION OF OUTPUT TREES AND HISTOGRAMS HERE <<<<

    m_output_tree = new TTree("NPPTree", "NPPTree");

    // declare branches
    m_output_tree->Branch("ph_n", &m_ph_n);
    m_output_tree->Branch("ph_pt", &m_ph_pt);
    // TODO: add more variables

    m_output_tree->SetDirectory(wk()->getOutputFile(m_output_stream_name.Data()));

    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisAlgorithm :: execute ()
{
    // Here you do everything that needs to be done on every single
    // events, e.g. read input variables, apply cuts, and fill
    // histograms and trees.  This is where most of your actual analysis
    // code will go.

    xAOD::TEvent* event = wk()->xaodEvent();

    // >>>>> DO EVENT-BY-EVENT WORK HERE <<<<
    // TODO: do some calculations
    m_ph_n = 1; // TODO
    m_ph_pt.push_back(100.0); // TODO

    // Fill the output tree
    m_output_tree->Fill();

    // clean-up
    clear_output_vectors();
    xAOD::TStore* store = wk()->xaodStore();
    store->clear();

    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisAlgorithm :: postExecute ()
{
    // Here you do everything that needs to be done after the main event
    // processing.  This is typically very rare, particularly in user
    // code.  It is mainly used in implementing the NTupleSvc.
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisAlgorithm :: finalize ()
{
    // This method is the mirror image of initialize(), meaning it gets
    // called after the last event has been processed on the worker node
    // and allows you to finish up any objects you created in
    // initialize() before they are written to disk.  This is actually
    // fairly rare, since this happens separately for each worker node.
    // Most of the time you want to do your post-processing on the
    // submission node after all your histogram outputs have been
    // merged.  This is different from histFinalize() in that it only
    // gets called on worker nodes that processed input events.

    xAOD::TEvent* event =wk()->xaodEvent();

    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisAlgorithm :: histFinalize ()
{
    // This method is the mirror image of histInitialize(), meaning it
    // gets called after the last event has been processed on the worker
    // node and allows you to finish up any objects you created in
    // histInitialize() before they are written to disk.  This is
    // actually fairly rare, since this happens separately for each
    // worker node.  Most of the time you want to do your
    // post-processing on the submission node after all your histogram
    // outputs have been merged.  This is different from finalize() in
    // that it gets called on all worker nodes regardless of whether
    // they processed input events.
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
void MyAnalysisAlgorithm :: clear_output_vectors()
{
    // TODO: add more vectors
    m_ph_pt.clear();
}


