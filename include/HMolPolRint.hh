/********************************************
 * \author <b>Programmer:<\b> Valerie Gray
 * \author <b>Assisted By:<\b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is header file for the Rint class
 *      - The HMolPolRint class implements a ROOT command line interface.
 *      - It inherits from the ROOT interactive C++ interpreter TRint.
 *      All functionality available in ROOT is available in this interface.
 *      In particular, all histograms that are defined in memory or in a
 *      ROOT file are accessible under the path "/root/ROOT memory",
 *      while they are being filled.
 *      - All HMolPol specific class structures are available in an interactive mode.
 *
 *  \class HMolPolRint
 *  \ingroup I have no idea what this is for
 *  \brief A ROOT command line interface
 *
 * \date <b>Date:</b> 07-16-2013
 * \date <b>Modified:</b> 04-20-2015
 *
 * \note <b>Entry Conditions:</b> none
********************************************/

#ifndef HMOLPOLRINT_HH_
#define HMOLPOLRINT_HH_

// ROOT headers
#include <TRint.h>
#include <TThread.h>

// Global pointers recognized by CINT
R__EXTERN class HMolPolRint* gHMolPolRint;

class HMolPolRint : public TRint {

  protected:

    static HMolPolRint* fExists;   ///< Check whether interface already existing

  public:
    /// \brief Constructor
    HMolPolRint (const char* appClassName, int* argc, char** argv,
                void* options = 0, int numOptions = 0, bool noLogo = kFALSE);
    ///< constructor for HMolPolRint

    /// \brief Destructor
    virtual ~HMolPolRint();   ///< destructor for HMolPolRint

}; // class HMolPolRint



#endif /* HMOLPOLRINT_HH_ */
