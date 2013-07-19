/*
 * HMolPolEventGenericDetectorHit.hh
 *
 *  Created on: Jul 19, 2013
 *      Author: vmgray
 */

#ifndef HMOLPOLEVENTGENERICDETECTORHIT_HH_
#define HMOLPOLEVENTGENERICDETECTORHIT_HH_

// ROOT includes
#include <TObject.h>
#include <TVector3.h>

class HMolPolEventGenericDetectorHit: public TObject {
  public:
    HMolPolEventGenericDetectorHit();
    virtual ~HMolPolEventGenericDetectorHit();

    Int_t fTrackID;     ///< Track ID
    Int_t fDetectorID;  ///< Detector ID

    TVector3 fPosition; ///< Hit position
    TVector3 fMomentum; ///< Hit momentum

};

#endif /* HMOLPOLEVENTGENERICDETECTORHIT_HH_ */
