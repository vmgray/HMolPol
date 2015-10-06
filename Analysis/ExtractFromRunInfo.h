/********************************************
 * \author <b>Programmer:<\b> Valerie Gray
 * \author <b>Assisted By:<\b>
 *
 * \brief <b>Purpose:</b> This is header file
 *   for the ExtractFromRunInfo function.
 *
 *   it will be used by many Analysis macros
 *   for HMolPol
 *
 * \date <b>Date:</b> 09-28-2015
 * \date <b>Modified:</b>
 *
 * \note <b>Entry Conditions:</b> none
 *
 ********************************************/

/***********************************************************
 Function: ExtractFromRunInfo
 Purpose:
  To extracts the payload from the input string (if it
  matches the match string)

  If the string that has the information (infoString) starts off with
  the the string that includes what we are looking for (matchString) return true

  If the string that has the information (infoString) does not starts off with
  the the string that includes what we are looking for (matchString) return false


 Entry Conditions:
 - TString infoString
 - String matchString
 - TString &toString

 Global:
 none

 Exit Conditions: Bool_t
 Called By:
 Date: 09-28-2015
 Modified:
 Brilliance by: Juan Carlos Cornejo
 *********************************************************/
// Extracts the payload from the input string (if it matches the match string)
Bool_t ExtractFromRunInfo(
                          TString infoString,
                            TString matchString,
                            TString &toString)
{
  //If infoString starts with matchString
  if (infoString.BeginsWith(matchString))
  {
    //make toString the string from character at number matchString.Length()
    //to character at infoString.Length() of infoString
    // - OR -
    //Cut the out the part of infoString that is the same as the matchString
    //then put what is left into toString
    toString = infoString(matchString.Length(), infoString.Length());

    return kTRUE;
  } //infoString doesn't starts with matchString

  return kFALSE;
}
