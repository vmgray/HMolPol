/*
 * HMolPolRunInformation.hh
 *
 *  Created on: Jul 16, 2013
 *      Author: vmgray
 */

#ifndef HMOLPOLRUNINFORMATION_HH_
#define HMOLPOLRUNINFORMATION_HH_

// root includes
#include <TObject.h>
#include <TString.h>
#include <TList.h>

class HMolPolRunInformation: public TObject {

 public:

  HMolPolRunInformation();
  virtual ~HMolPolRunInformation();

  TList Conditions;

  ClassDef(HMolPolRunInformation,1);
};



#endif /* HMOLPOLRUNINFORMATION_HH_ */
