/********************************************
 * \author <b>Programmer:<\b> Valerie Gray
 * \author <b>Assisted By:<\b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is header file for the HMolPolLinkDef class.
 *      This class is in charge linking to all the files needed to fill
 *      the ROOT tree
 *
 * \date <b>Date:</b> 07-16-2013
 * \date <b>Modified:</b> 04-13-2016
 *
 * \note <b>Entry Conditions:</b> none
 *
********************************************/

#ifndef HMOLPOLLINKDEF_HH_
#define HMOLPOLLINKDEF_HH_

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class HMolPolRunInformation+;

#pragma link C++ class HMolPolEventUnits+;
#pragma link C++ class HMolPolEventPrimary+;
#pragma link C++ class HMolPolEventPrimaryHit+;
#pragma link C++ class std::vector<HMolPolEventPrimaryHit>+;
#pragma link C++ class HMolPolEventGenericDetector+;
#pragma link C++ class HMolPolEventGenericDetectorHit+;
#pragma link C++ class HMolPolEventPrimaryConditions+;

#endif

#endif /* HMOLPOLLINKDEF_HH_ */
