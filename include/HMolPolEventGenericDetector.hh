/********************************************
 * \author <b>Programmer:<\b> Valerie Gray
 * \author <b>Assisted By:<\b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is header file for the
 *      HMolPolGenericDetectorEvent class.
 *
 * \date <b>Date:</b> 07-19-2013
 * \date <b>Modified:</b> 07-15-2015
 *
 * \note <b>Entry Conditions:</b> none
 *
********************************************/

#ifndef HMOLPOLGENERICDETECTOREVENT_HH_
#define HMOLPOLGENERICDETECTOREVENT_HH_

// System includes
#include <vector>

// ROOT includes
#include <TObject.h>

// HMolPol includes
#include "HMolPolEventGenericDetectorHit.hh"


class HMolPolEventGenericDetector: public TObject {
  public:
    HMolPolEventGenericDetector() { };    ///< the constructor for
      /// HMolPolEventGenericDetector
    virtual ~HMolPolEventGenericDetector() { };  ///< the destrutor for
    /// HMolPolEventGenericDetector

    std::vector<HMolPolEventGenericDetectorHit> fHits; ///< vector of the Hits

    /// Register this class with ROOT. This adds some standard ROOT functions to
    /// the class, and makes sure that on the command line in ROOT it can do
    /// tab-completion. This is associated with the ClassImp() line at the top
    /// of the .cc file.
    ClassDef(HMolPolEventGenericDetector,1)
};

#endif /* HMOLPOLGENERICDETECTOREVENT_HH_ */
