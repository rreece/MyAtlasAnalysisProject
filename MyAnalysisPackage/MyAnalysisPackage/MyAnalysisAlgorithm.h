// MyAnalysisAlgorithm.h
// author: Ryan Reece <ryan.reece@cern.ch>
// created: 2016-02-04
//=============================================================================

#ifndef MyAnalysisPackage_MyAnalysisAlgorithm_H
#define MyAnalysisPackage_MyAnalysisAlgorithm_H

// std includes
#include <vector>

// ROOT includes
#include "TString.h"
#include "TTree.h"

// ATLAS framework
#include "EventLoop/Algorithm.h"
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"


class MyAnalysisAlgorithm : public EL::Algorithm
{
    //-------------------------------------------------------------------------
    public:
    
        // standard constructor
        MyAnalysisAlgorithm ();

        // functions inherited from Algorithm
        virtual EL::StatusCode setupJob (EL::Job& job);
        virtual EL::StatusCode fileExecute ();
        virtual EL::StatusCode histInitialize ();
        virtual EL::StatusCode changeInput (bool firstFile);
        virtual EL::StatusCode initialize ();
        virtual EL::StatusCode execute ();
        virtual EL::StatusCode postExecute ();
        virtual EL::StatusCode finalize ();
        virtual EL::StatusCode histFinalize ();

        // put your configuration variables here as public variables.
        // that way they can be set directly from CINT and python.
        // float cutValue;
        bool        c_debug;
        TString     c_output_stream_name;

    //-------------------------------------------------------------------------
    private:

        ///////////////////////////////////////////////////////////////////////
        // variables that don't get filled at submission time should be
        // protected from being sent from the submission node to the worker
        // node (done by the //!)
        //public:
        // Tree *myTree; //!
        // TH1 *myHist; //!
        ///////////////////////////////////////////////////////////////////////
    
        TTree*              m_output_tree;    //! 

        int                 m_ph_n;           //!
        std::vector<float>  m_ph_pt;          //!

        // private functions
        void clear_output_vectors();

        // this is needed to distribute the algorithm to the workers
        ClassDef(MyAnalysisAlgorithm, 1);
};

#endif
