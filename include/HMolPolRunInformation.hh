/********************************************
 * \author <b>Programmer:<\b> Valerie Gray
 * \author <b>Assisted By:<\b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is header file for the run information
 *      and get it to the ROOT file.
 *
 * \date <b>Date:</b> 07-16-2013
 * \date <b>Modified:</b> 07-06-2013
 *
 * \note <b>Entry Conditions:</b> none
 *
********************************************/

#ifndef HMOLPOLRUNINFORMATION_HH_
#define HMOLPOLRUNINFORMATION_HH_

// root includes
#include <TObject.h>
#include <TString.h>
#include <TList.h>

class HMolPolRunInformation: public TObject {

 public:

  HMolPolRunInformation();   ///< the constructor for HMolPolRunInformation
  virtual ~HMolPolRunInformation();   ///< the destructor for HMolPolRunInformation

  TList Conditions;   ///< the TList of the conditions

  ClassDef(HMolPolRunInformation,1);  ///< the class definition of the
    /// conditions that will be put into the ROOT file
};



#endif /* HMOLPOLRUNINFORMATION_HH_ */
