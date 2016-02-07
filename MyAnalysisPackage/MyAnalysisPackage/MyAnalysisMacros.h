// MyAnalysisMacros.h
// author: Ryan Reece <ryan.reece@cern.ch>
// created: 2016-02-07
//=============================================================================

#ifndef MyAnalysisPackage_MyAnalysisMacros_H
#define MyAnalysisPackage_MyAnalysisMacros_H

#include "xAODRootAccess/tools/Message.h"

// Helper macro for checking xAOD::TReturnCode return values
#define AOD_CHECK( CONTEXT, EXP )                       \
    do {                                                \
        if( ! EXP.isSuccess() ){                        \
            Error( CONTEXT,                             \
                XAOD_MESSAGE( "Failed to execute: %s"), \
                #EXP );                                 \
            return EL::StatusCode::FAILURE;             \
        }                                               \
    } while( false ) 

// Helper macro for checking EL::StatusCodes.
#define EL_CHECK( CONTEXT, ARG )                          \
    do {                                                  \
        if( ARG != EL::StatusCode::SUCCESS ) {            \
            Error( CONTEXT, "Failed to execute: \"%s\"",  \
                   #ARG );                                \
            return EL::StatusCode::FAILURE;               \
        }                                                 \
    } while( false )

// Helper macro for checking for non-zero before delete
#define SAFE_DELETE(x) { if(x) { delete x; x=0; } }

#endif
