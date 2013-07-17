/*
 * HMolPolRint.hh
 *
 *  Created on: Jul 16, 2013
 *      Author: vmgray
 */

#ifndef HMOLPOLRINT_HH_
#define HMOLPOLRINT_HH_

// ROOT headers
#include <TRint.h>
#include <TThread.h>

// Global pointers recognised by CINT
R__EXTERN class HMolPolRint* gHMolPolRint;

/**
 *  \class HMolPolRint
 *  \ingroup QwThreadSafe
 *  \brief A ROOT command line interface
 *
 * The QwRint class implements a ROOT command line interface.  It inherits
 * from the ROOT interactive C++ interpreter TRint.  All functionality available
 * in ROOT is available in this interface.  In particular, all histograms that
 * are defined in memory or in a ROOT file are accessible under the path
 * "/root/ROOT memory", while they are being filled.  All Qweak specific class
 * structures are available in an interactive mode.
 *
 */
class HMolPolRint : public TRint {

  protected:

    static HMolPolRint* fExists;       ///< Check whether interface already existing

  public:
    /// \brief Constructor
    HMolPolRint (const char* appClassName, int* argc, char** argv,
                void* options = 0, int numOptions = 0, bool noLogo = kFALSE);

    /// \brief Destructor
    virtual ~HMolPolRint();

}; // class HMolPolRint



#endif /* HMOLPOLRINT_HH_ */
