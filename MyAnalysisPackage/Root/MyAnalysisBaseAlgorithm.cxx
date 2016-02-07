// MyAnalysisBaseAlgorithm.cxx
// author: Ryan Reece <ryan.reece@cern.ch>
// created: 2016-02-04
//=============================================================================

// this package
#include "MyAnalysisPackage/MyAnalysisBaseAlgorithm.h"
#include "MyAnalysisPackage/MyAnalysisMacros.h"

// std includes
#include <string>
#include <vector>

// ROOT includes
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TSystem.h"

// ATLAS framework
#include "EventLoop/Job.h"
#include "EventLoop/StatusCode.h"
#include "EventLoop/Worker.h"
#include "EventLoop/OutputStream.h"
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"
#include "xAODEventInfo/EventInfo.h"
#include "SUSYTools/SUSYObjDef_xAOD.h"


// this is needed to distribute the algorithm to the workers
ClassImp(MyAnalysisBaseAlgorithm)


//-----------------------------------------------------------------------------
MyAnalysisBaseAlgorithm :: MyAnalysisBaseAlgorithm ()
{
    // Here you put any code for the base initialization of variables,
    // e.g. initialize all pointers to 0.  Note that you should only put
    // the most basic initialization here, since this method will be
    // called on both the submission and the worker node.  Most of your
    // initialization code will go into histInitialize() and
    // initialize().

    c_debug              = false;
    c_output_stream_name = "output";
    c_output_tree_name   = "mytree";

    n_events_processed  = 0; 
    n_weights_processed = 0.0;
    n_events_accepted   = 0; 
    n_weights_accepted  = 0.0;

    h_cutflow       = 0;

    m_output_tree   = 0;
    m_SUSYTool      = 0;

    m_event         = 0;
    m_store         = 0;
    m_event_info    = 0;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisBaseAlgorithm :: setupJob (EL::Job& job)
{
    const char *FUNC_NAME = "setupJob";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

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

    EL_RETURN_CHECK(FUNC_NAME, xAOD::Init() ); //call before opening first file
    EL::OutputStream stream(c_output_stream_name.c_str());
    job.outputAdd(stream);

    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisBaseAlgorithm :: histInitialize ()
{
    const char *FUNC_NAME = "histInitialize";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

    // Here you do everything that needs to be done at the very
    // beginning on each worker node, e.g. create histograms and output
    // trees.  This method gets called before any input files are
    // connected.

    n_events_processed = 0; 
    n_weights_processed = 0.0;
    n_events_accepted = 0; 
    n_weights_accepted = 0.0;

    // cutflow histogram
    const std::vector<std::string> xlabels = {"all", "GRL", "trigger", "vertex", "quality", "preskim", "skim"};
    const unsigned int ncut = xlabels.size();
    h_cutflow = new TH1D("h_cutflow", "h_cutflow", ncut, 0.5, ncut+0.5);
    for(unsigned int i=1; i<=ncut; ++i) { h_cutflow->GetXaxis()->SetBinLabel(i, xlabels[i-1].c_str()); }
    wk()->addOutput(h_cutflow); // adding histogram to the outputstream

    // Initialize additional output histograms
    CHECK(FUNC_NAME, user_initialize_hists());

    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisBaseAlgorithm :: fileExecute ()
{
    const char *FUNC_NAME = "fileExecute";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

    // Here you do everything that needs to be done exactly once for every
    // single file, e.g. collect a list of all lumi-blocks processed

    // TODO: initialize and bookkeep metadata
    
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisBaseAlgorithm :: changeInput (bool firstFile)
{
    const char *FUNC_NAME = "changeInput";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s(%i) start", FUNC_NAME, firstFile);

    // Here you do everything you need to do when we change input files,
    // e.g. resetting branch addresses on trees.  If you are using
    // D3PDReader or a similar service this method is not needed.
    
    // TODO: bookkeep metadata
    
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisBaseAlgorithm :: initialize ()
{
    const char *FUNC_NAME = "initialize";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

    // Here you do everything that you need to do after the first input
    // file has been connected and before the first event is processed,
    // e.g. create additional histograms based on which variables are
    // available in the input files.  You can also create all of your
    // histograms and trees in here, but be aware that this method
    // doesn't get called if no events are processed.  So any objects
    // you create here won't be available in the output if you have no
    // input events.
    xAOD::TEvent* event = wk()->xaodEvent();

    // Let's check the number of events in our xAOD
    Info(FUNC_NAME, "Number of events in the input = %lli", event->getEntries() ); //print long long int

    // Initialize output trees.
    m_output_tree = new TTree(c_output_tree_name.c_str(), c_output_tree_name.c_str());

    // Declare branches
    CHECK(FUNC_NAME, user_declare_branches(m_output_tree));

    m_output_tree->SetDirectory(wk()->getOutputFile(c_output_stream_name.c_str()));

    // Just being careful to set the initial values the same as they are reset.
    CHECK(FUNC_NAME, clear_cache_variables());
    CHECK(FUNC_NAME, clear_output_variables());

    CHECK(FUNC_NAME, setup_susy_tools());

    CHECK(FUNC_NAME, user_initialize());

    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisBaseAlgorithm :: execute ()
{
    const char *FUNC_NAME = "execute";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

    // Here you do everything that needs to be done on every single
    // events, e.g. read input variables, apply cuts, and fill
    // histograms and trees.  This is where most of your actual analysis
    // code will go.
    
    CHECK(FUNC_NAME, preprocess_event());

    bool passes_skim1 = false;
    CHECK(FUNC_NAME, check_preskim_event(passes_skim1));
    if(passes_skim1)
    {
        // TODO: for-loop over systematics writing separate ntuples.
        CHECK(FUNC_NAME, process_event());

        bool passes_skim2 = false;
        CHECK(FUNC_NAME, check_skim_event(passes_skim2));

        if(passes_skim2)
            CHECK(FUNC_NAME, write_event()); // write
    }

    // clean-up
    CHECK(FUNC_NAME, clear_cache_variables());
    CHECK(FUNC_NAME, clear_output_variables());
    xAOD::TStore* store = wk()->xaodStore();
    store->clear();

    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisBaseAlgorithm :: postExecute ()
{
    const char *FUNC_NAME = "postExecute";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

    // Here you do everything that needs to be done after the main event
    // processing.  This is typically very rare, particularly in user
    // code.  It is mainly used in implementing the NTupleSvc.

    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisBaseAlgorithm :: finalize ()
{
    const char *FUNC_NAME = "finalize";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

    // This method is the mirror image of initialize(), meaning it gets
    // called after the last event has been processed on the worker node
    // and allows you to finish up any objects you created in
    // initialize() before they are written to disk.  This is actually
    // fairly rare, since this happens separately for each worker node.
    // Most of the time you want to do your post-processing on the
    // submission node after all your histogram outputs have been
    // merged.  This is different from histFinalize() in that it only
    // gets called on worker nodes that processed input events.
    
    CHECK(FUNC_NAME, user_finalize());
    SAFE_DELETE(m_SUSYTool);

    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisBaseAlgorithm :: histFinalize ()
{
    const char *FUNC_NAME = "histFinalize";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

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
    
    Info(FUNC_NAME, "MyAnalysis done!");
    Info(FUNC_NAME, "Number of events processed = %lli", n_events_processed);
    Info(FUNC_NAME, "Number of events in output = %lli", n_events_accepted);
    
    const unsigned int ncut = h_cutflow->GetNbinsX();
    for(unsigned int i=1; i<=ncut; ++i)
    {
        const char* label = h_cutflow->GetXaxis()->GetBinLabel(i);
        const double count = h_cutflow->GetBinContent(i);
        const double err = h_cutflow->GetBinError(i);
        Info(FUNC_NAME, "%16s  %16f  %12f", label, count, err);
    }

    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return EL::StatusCode::SUCCESS;
}


//============================================================================
// protected methods
//============================================================================

//-----------------------------------------------------------------------------
bool MyAnalysisBaseAlgorithm :: is_mc()
{
    const char *FUNC_NAME = "is_mc";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

    if(m_event_info != 0)
    {
        // check if the event is Monte Carlo
        if( m_event_info->eventType(xAOD::EventInfo::IS_SIMULATION) )
            return true;
    }
    else
    {
        Error(FUNC_NAME, "m_event_info == 0, returning is_mc()=false");
    }

    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return false;
}


//============================================================================
// private methods
//============================================================================

//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisBaseAlgorithm :: setup_susy_tools()
{
    const char *FUNC_NAME = "setup_susy_tools";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

    Info(FUNC_NAME, "Setting up SUSYTools...");

    m_SUSYTool = new ST::SUSYObjDef_xAOD("SUSYObjDef_xAOD");
    EL_RETURN_CHECK(FUNC_NAME, m_SUSYTool->setProperty( "ConfigFile",  "$ROOTCOREBIN/data/MyAnalysisPackage/SUSYTools_MyAnalysis.conf" ));
    m_SUSYTool->msg().setLevel( MSG::ERROR);  // MSG::ERROR MSG::DEBUG

    bool isAFII=false;
    ST::SettingDataSource data_source = is_mc() ? (isAFII ? ST::AtlfastII : ST::FullSim) : ST::Data;
    EL_RETURN_CHECK(FUNC_NAME, m_SUSYTool->setProperty("DataSource", data_source ));
    EL_RETURN_CHECK(FUNC_NAME, m_SUSYTool->setProperty("DoPhotonOR", true));
    //CHECK(m_SUSYTool->setProperty("METTauTerm", ""));

    std::string common_path = gSystem->ExpandPathName("$ROOTCOREBIN/data/MyAnalysisPackage/");
    std::vector<std::string> lcalc_files;
    lcalc_files.push_back( common_path + "ilumicalc_histograms_HLT_2g50_loose_276262-282421.root" );

    std::vector<std::string> prw_files;
    prw_files.push_back( common_path + "merged_prw.root" );

    EL_RETURN_CHECK(FUNC_NAME, m_SUSYTool->setProperty("PRWDefaultChannel", 407013 ));
    EL_RETURN_CHECK(FUNC_NAME, m_SUSYTool->setProperty("PRWConfigFiles", prw_files ));
    EL_RETURN_CHECK(FUNC_NAME, m_SUSYTool->setProperty("PRWLumiCalcFiles", lcalc_files ));

    if(m_SUSYTool->initialize().isFailure()) {
        Error(FUNC_NAME, "Failed to initialise tools in SUSYToolsInit()..." );
        Error(FUNC_NAME, "Exiting..." );
        return EL::StatusCode::FAILURE;
    } else {
        Info(FUNC_NAME,"SUSYToolsInit with success!!... " );
    }

    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisBaseAlgorithm :: clear_cache_variables()
{
    const char *FUNC_NAME = "clear_cache_variables";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

    // TODO: Add more variables here.
    m_event         = 0;
    m_store         = 0;
    m_event_info    = 0;

    CHECK(FUNC_NAME, user_clear_cache_variables());

    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisBaseAlgorithm :: clear_output_variables()
{
    const char *FUNC_NAME = "clear_output_variables";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

    CHECK(FUNC_NAME, user_clear_output_variables());

    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisBaseAlgorithm :: preprocess_event()
{
    const char *FUNC_NAME = "preprocess_event";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

    n_events_processed++;
    n_weights_processed += 1.0; // TODO
    h_cutflow->Fill(1); // all

    m_event = wk()->xaodEvent();
    m_store = wk()->xaodStore();
    
    // Get EventInfo
    EL_RETURN_CHECK(FUNC_NAME, m_event->retrieve(m_event_info, "EventInfo"));  

    CHECK(FUNC_NAME, user_preprocess_event());

    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisBaseAlgorithm :: check_preskim_event(bool& passes)
{
    const char *FUNC_NAME = "check_preskim_event";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

    // {"all", "GRL", "trigger", "vertex", "quality", "preskim", "skim"};
    passes = false;

    bool passes_grl = is_mc() || check_grl();
    if(passes_grl)
    {
        h_cutflow->Fill(2); // GRL

        bool passes_trigger = check_trigger();
        if(passes_trigger)
        {
            h_cutflow->Fill(3); // trigger

            bool passes_vertex = check_vertex();
            if(passes_vertex)
            {
                h_cutflow->Fill(4); // vertex

                bool passes_quality = check_quality();
                if(passes_quality)
                {
                    h_cutflow->Fill(5); // quality

                    bool passes_preskim = check_preskim();
                    if(passes_preskim)
                    {
                        h_cutflow->Fill(6); // preskim
                        passes = true;
                    }
                }
            }
        }
    }

    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end, passes=%i", FUNC_NAME, passes);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisBaseAlgorithm :: process_event()
{
    const char *FUNC_NAME = "process_event";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

    CHECK(FUNC_NAME, user_process_event());
    
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisBaseAlgorithm :: check_skim_event(bool& passes)
{
    const char *FUNC_NAME = "check_skim_event";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

    passes = check_skim();
    if(passes)
    {
        h_cutflow->Fill(7); // skim
    }

    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end, passes=%i", FUNC_NAME, passes);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
EL::StatusCode MyAnalysisBaseAlgorithm :: write_event()
{
    const char *FUNC_NAME = "write_event";
    if(c_debug) Info(FUNC_NAME, "DEBUG: %s start", FUNC_NAME);

    // Fill the output tree
    m_output_tree->Fill();

    n_events_accepted++;
    n_weights_accepted += 1.0; // TODO

    if(c_debug) Info(FUNC_NAME, "DEBUG: %s end", FUNC_NAME);
    return EL::StatusCode::SUCCESS;
}


//-----------------------------------------------------------------------------
bool MyAnalysisBaseAlgorithm :: check_grl()
{
    return true; // TODO
}


//-----------------------------------------------------------------------------
bool MyAnalysisBaseAlgorithm :: check_trigger()
{
    return true; // TODO
}


//-----------------------------------------------------------------------------
bool MyAnalysisBaseAlgorithm :: check_vertex()
{
    return true; // TODO
}


//-----------------------------------------------------------------------------
bool MyAnalysisBaseAlgorithm :: check_quality()
{
    return true; // TODO
}


//-----------------------------------------------------------------------------
bool MyAnalysisBaseAlgorithm :: check_preskim()
{
    return user_check_preskim();
}


//-----------------------------------------------------------------------------
bool MyAnalysisBaseAlgorithm :: check_skim()
{
    return user_check_skim();
}


