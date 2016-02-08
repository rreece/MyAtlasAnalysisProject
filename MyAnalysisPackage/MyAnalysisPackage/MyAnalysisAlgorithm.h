// MyAnalysisAlgorithm.h
// author: Ryan Reece <ryan.reece@cern.ch>
// created: 2016-02-04
//=============================================================================

#ifndef MyAnalysisPackage_MyAnalysisAlgorithm_H
#define MyAnalysisPackage_MyAnalysisAlgorithm_H

// std includes
#include <vector>

// ROOT includes

// ATLAS framework
#include "xAODEgamma/PhotonContainer.h"
#include "xAODEgamma/PhotonAuxContainer.h"

// this package
#include "MyAnalysisPackage/MyAnalysisBaseAlgorithm.h"


class MyAnalysisAlgorithm : public MyAnalysisBaseAlgorithm
{
    //-------------------------------------------------------------------------
    public:

        // standard constructor
        MyAnalysisAlgorithm ();

    //-------------------------------------------------------------------------
    private:

        // methods classes deriving from MyAnalysisBaseAlgorithm must implement
        virtual EL::StatusCode user_initialize_hists();
        virtual EL::StatusCode user_declare_branches(TTree* tree);
        virtual EL::StatusCode user_clear_cache_variables();
        virtual EL::StatusCode user_clear_output_variables();
        virtual EL::StatusCode user_initialize();
        virtual EL::StatusCode user_preprocess_event();
        virtual bool user_check_preskim();
        virtual EL::StatusCode user_process_event();
        virtual bool user_check_skim();
        virtual EL::StatusCode user_finalize();

        ///////////////////////////////////////////////////////////////////////
        // variables that don't get filled at submission time should be
        // protected from being sent from the submission node to the worker
        // node (done by the //!)
        //     public:
        //         Tree *myTree;  //!
        //         TH1  *myHist;  //!
        ///////////////////////////////////////////////////////////////////////

        // other private variables and caches (m_*)
        xAOD::PhotonContainer*      m_photons;        //!
        xAOD::ShallowAuxContainer*  m_photons_aux;    //!

        // varialbles for the output ntuple (o_*)
        int                 o_ph_n;             //!
        std::vector<float>  o_ph_pt;            //!
        std::vector<float>  o_ph_eta;           //!
        std::vector<float>  o_ph_phi;           //!
        // USER TODO: Add additional output variables here.

        // this is needed to distribute the algorithm to the workers
        ClassDef(MyAnalysisAlgorithm, 1);
};

#endif

