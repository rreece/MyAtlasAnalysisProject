// MyAnalysisMacros.h
// author: Ryan Reece <ryan.reece@cern.ch>
// created: 2016-02-07
//=============================================================================

#ifndef MyAnalysisPackage_MyAnalysisMacros_H
#define MyAnalysisPackage_MyAnalysisMacros_H


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

// Helper macro for checking EL::StatusCodes.
#define CHECK( ARG )                                        \
    do {                                                    \
        if( ARG != EL::StatusCode::SUCCESS ) {              \
            Error( FUNC_NAME, "Failed to execute: \"%s\"",  \
                   #ARG );                                  \
            return EL::StatusCode::FAILURE;                 \
        }                                                   \
    } while( false )


#endif

