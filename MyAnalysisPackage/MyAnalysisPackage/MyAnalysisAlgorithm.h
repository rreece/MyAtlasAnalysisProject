// MyAnalysisAlgorithm.h
// author: Ryan Reece <ryan.reece@cern.ch>
// created: 2016-02-04
//=============================================================================

#ifndef MyAnalysisPackage_MyAnalysisAlgorithm_H
#define MyAnalysisPackage_MyAnalysisAlgorithm_H

// std includes
#include <string>
#include <vector>

// ROOT includes
class TH1F;
class TTree;

// ATLAS framework
#include "EventLoop/Algorithm.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"
#include "xAODEventInfo/EventInfo.h"


class MyAnalysisAlgorithm : public EL::Algorithm
{
    //-------------------------------------------------------------------------
    public:
    
        // standard constructor
        MyAnalysisAlgorithm ();

        // functions inherited from Algorithm
        virtual EL::StatusCode setupJob(EL::Job& job);
        virtual EL::StatusCode fileExecute();
        virtual EL::StatusCode histInitialize();
        virtual EL::StatusCode changeInput(bool firstFile);
        virtual EL::StatusCode initialize();
        virtual EL::StatusCode execute();
        virtual EL::StatusCode postExecute();
        virtual EL::StatusCode finalize();
        virtual EL::StatusCode histFinalize();

        // USER TODO: Put your configuration variables here as public variables.
        // That way they can be set directly from CINT and python.
        // float c_cut_value;  (c_*)
        bool        c_debug;
        std::string c_output_stream_name;
        std::string c_output_tree_name;

    //-------------------------------------------------------------------------
    private:

        ///////////////////////////////////////////////////////////////////////
        // variables that don't get filled at submission time should be
        // protected from being sent from the submission node to the worker
        // node (done by the //!)
        //     public:
        //         Tree *myTree;  //!
        //         TH1  *myHist;  //!
        ///////////////////////////////////////////////////////////////////////

        // private counters (n_*)
        unsigned long long  n_events_processed;  //! 
        double              n_weights_processed; //! 
        unsigned long long  n_events_accepted;   //!
        double              n_weights_accepted;  //!

        // histograms (h_*)
        TH1F*               h_cutflow;          //!
        // USER TODO: Maybe add additional output histograms here.

        // other private variables and caches (m_*)
        TTree*                  m_output_tree;  //! 
        xAOD::TEvent*           m_event;        //!
        xAOD::TStore*           m_store;        //!
        const xAOD::EventInfo*  m_event_info;   //! 
    
        // varialbles for the output ntuple (o_*)
        int                 o_ph_n;             //!
        std::vector<float>  o_ph_pt;            //!
        // USER TODO: Add additional output variables here.

        // main private functions
        EL::StatusCode clear_cache_variables();
        EL::StatusCode clear_output_variables();
        EL::StatusCode preprocess_event();
        EL::StatusCode check_preskim_event(bool& passes);
        EL::StatusCode process_event();
        EL::StatusCode check_skim_event(bool& passes);
        EL::StatusCode write_event();

        // private helper functions
        bool is_mc();
        bool check_grl();
        bool check_trigger();
        bool check_vertex();
        bool check_quality();
        bool check_preskim();
        bool check_skim();

        // this is needed to distribute the algorithm to the workers
        ClassDef(MyAnalysisAlgorithm, 1);
};

#endif

