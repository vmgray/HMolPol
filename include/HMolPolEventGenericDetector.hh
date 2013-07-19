/*
 * HMolPolGenericDetectorEvent.hh
 *
 *  Created on: Jul 19, 2013
 *      Author: vmgray
 */

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
    HMolPolEventGenericDetector();
    virtual ~HMolPolEventGenericDetector();

    std::vector<HMolPolEventGenericDetectorHit> fHits;
};

#endif /* HMOLPOLGENERICDETECTOREVENT_HH_ */
