// MyAnalysisBaseAlgorithm.h
// author: Ryan Reece <ryan.reece@cern.ch>
// created: 2016-02-04
//=============================================================================

#ifndef MyAnalysisPackage_MyAnalysisBaseAlgorithm_H
#define MyAnalysisPackage_MyAnalysisBaseAlgorithm_H

// std includes
#include <string>

// ROOT includes
class TH1D;
class TTree;

// ATLAS framework
#include "EventLoop/Algorithm.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"
#include "xAODEventInfo/EventInfo.h"
#include "SUSYTools/SUSYObjDef_xAOD.h"


class MyAnalysisBaseAlgorithm : public EL::Algorithm
{
    //-------------------------------------------------------------------------
    public:
    
        // standard constructor
        MyAnalysisBaseAlgorithm ();

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

        // Put your configuration variables here as public variables.
        // That way they can be set directly from CINT and python.
        // float c_cut_value;  (c_*)
        bool        c_debug;
        std::string c_output_stream_name;
        std::string c_output_tree_name;

    //-------------------------------------------------------------------------
    protected:
        // pure virtual methods that must be implemented in derived classes
        virtual EL::StatusCode user_initialize_hists() = 0;
        virtual EL::StatusCode user_declare_branches(TTree* tree) = 0;
        virtual EL::StatusCode user_clear_cache_variables() = 0;
        virtual EL::StatusCode user_clear_output_variables() = 0;
        virtual EL::StatusCode user_initialize() = 0;
        virtual EL::StatusCode user_preprocess_event() = 0;
        virtual EL::StatusCode user_process_event() = 0;
        virtual EL::StatusCode user_finalize() = 0;
        virtual bool user_check_preskim() = 0;
        virtual bool user_check_skim() = 0;

        // protected helper methods (provided to derived classes privately)
        bool is_mc();
        
        // protected handels (m_*)
        xAOD::TEvent*           m_event;        //!
        xAOD::TStore*           m_store;        //!
        const xAOD::EventInfo*  m_event_info;   //! 
        ST::SUSYObjDef_xAOD*    m_SUSYTool;     //!

    //-------------------------------------------------------------------------
    private:
        // main private functions
        EL::StatusCode setup_susy_tools();
        EL::StatusCode clear_cache_variables();
        EL::StatusCode clear_output_variables();
        EL::StatusCode preprocess_event();
        EL::StatusCode check_preskim_event(bool& passes);
        EL::StatusCode process_event();
        EL::StatusCode check_skim_event(bool& passes);
        EL::StatusCode write_event();

        // private helper functions
        bool check_grl();
        bool check_trigger();
        bool check_vertex();
        bool check_quality();
        bool check_preskim();
        bool check_skim();

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
        TH1D*               h_cutflow;          //!

        // other private variables and caches (m_*)
        TTree*              m_output_tree;      //! 
    
        // varialbles for the output ntuple (o_*)

        // this is needed to distribute the algorithm to the workers
        ClassDef(MyAnalysisBaseAlgorithm, 1);
};

#endif

