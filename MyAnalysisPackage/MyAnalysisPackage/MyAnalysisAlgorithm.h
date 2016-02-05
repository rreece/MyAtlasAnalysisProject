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
#include "TH1F.h"
#include "TTree.h"

// ATLAS framework
#include "EventLoop/Algorithm.h"

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

        // USER: Put your configuration variables here as public variables.
        // That way they can be set directly from CINT and python.
        // float c_cut_value;
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

        // private counters
        int                 n_events_processed;  //! 
        float               n_weights_processed; //! 

        // histograms
        TH1F*               h_cutflow;          //!
        // USER: Add additional output histograms here.
    
        // varialbles for the output ntuple
        TTree*              m_output_tree;      //! 
        int                 m_ph_n;             //!
        std::vector<float>  m_ph_pt;            //!
        // USER TODO: Add additional output variables here.

        // private functions
        EL::StatusCode clear_output_variables();
        EL::StatusCode preprocess_event();
        EL::StatusCode check_skim_event(bool& passes);
        EL::StatusCode process_event();
        EL::StatusCode check_skim_event_after(bool& passes);
        EL::StatusCode write_event();

        // this is needed to distribute the algorithm to the workers
        ClassDef(MyAnalysisAlgorithm, 1);
};

#endif
