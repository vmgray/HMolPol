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

    //define a class for ROOT
    ClassDef(HMolPolEventGenericDetector,1)
};

#endif /* HMOLPOLGENERICDETECTOREVENT_HH_ */
