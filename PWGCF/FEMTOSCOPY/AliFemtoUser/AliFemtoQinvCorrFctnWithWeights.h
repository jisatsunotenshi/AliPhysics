/***************************************************************************
 *
 * $Id$
 *
 * Author: Mike Lisa, Ohio State, lisa@mps.ohio-state.edu
 ***************************************************************************
 *
 * Description: part of STAR HBT Framework: AliFemtoMaker package
 *   a simple Q-invariant correlation function
 *
 ***************************************************************************
 *
 * $Log$
 * Revision 1.1  2007/05/16 10:22:11  akisiel
 * Making the directory structure of AliFemto flat. All files go into one common directory
 *
 * Revision 1.2  2007/05/03 09:40:42  akisiel
 * Fixing Effective C++ warnings
 *
 * Revision 1.1.1.1  2007/04/25 15:38:41  panos
 * Importing the HBT code dir
 *
 * Revision 1.1.1.1  2007/03/07 10:14:49  mchojnacki
 * First version on CVS
 *
 * Revision 1.3  2000/01/25 17:34:45  laue
 * I. In order to run the stand alone version of the AliFemtoMaker the following
 * changes have been done:
 * a) all ClassDefs and ClassImps have been put into #ifdef __ROOT__ statements
 * b) unnecessary includes of StMaker.h have been removed
 * c) the subdirectory AliFemtoMaker/doc/Make has been created including everything
 * needed for the stand alone version
 *
 * II. To reduce the amount of compiler warning
 * a) some variables have been type casted
 * b) some destructors have been declared as virtual
 *
 * Revision 1.2  1999/07/06 22:33:20  lisa
 * Adjusted all to work in pro and new - dev itself is broken
 *
 * Revision 1.1.1.1  1999/06/29 16:02:57  lisa
 * Installation of AliFemtoMaker
 *
 **************************************************************************/

#ifndef ALIFEMTOQINVCORRFCTNWITHWEIGHTS_H
#define ALIFEMTOQINVCORRFCTNWITHWEIGHTS_H

#include "TH1D.h"
#include "TH2D.h"
#include "TNtuple.h"

#include "AliFemtoCorrFctn.h"

#include "AliAODInputHandler.h"
#include "AliAnalysisManager.h"


class AliFemtoQinvCorrFctnWithWeights : public AliFemtoCorrFctn {
public:
  AliFemtoQinvCorrFctnWithWeights(const char* title, const int& nbins, const float& QinvLo, const float& QinvHi, TH2D *filter1, TH2D *filter2);
  AliFemtoQinvCorrFctnWithWeights(const AliFemtoQinvCorrFctnWithWeights& aCorrFctn);
  virtual ~AliFemtoQinvCorrFctnWithWeights();

  AliFemtoQinvCorrFctnWithWeights& operator=(const AliFemtoQinvCorrFctnWithWeights& aCorrFctn);

  virtual AliFemtoString Report();
  virtual void AddRealPair(AliFemtoPair* aPair);
  virtual void AddMixedPair(AliFemtoPair* aPair);

  virtual void Finish();

  void CalculateDetaDphis(Bool_t, Double_t);
  void CalculatePairKinematics(Bool_t);

  TH1D* Numerator();
  TH1D* Denominator();
  TH1D* Ratio();

  void Write();
  virtual TList* GetOutputList();

  virtual AliFemtoCorrFctn* Clone() const { return new AliFemtoQinvCorrFctnWithWeights(*this); }

private:
  TH1D* fNumerator;          // numerator - real pairs
  TH1D* fDenominator;        // denominator - mixed pairs
  TH1D* fRatio;              // ratio - correlation function
  TH1D* fkTMonitor;          // Monitor the kT of pairs in the function

  TH2D *fYPtWeightsParticle1;
  TH2D *fYPtWeightsParticle2;

  Bool_t fDetaDphiscal;
  Bool_t fPairKinematics;

  Double_t fRaddedps;
  TH2D* fNumDEtaDPhiS;
  TH2D* fDenDEtaDPhiS;

  TNtuple* PairReader; //PairReader for CorrFit

#ifdef __ROOT__
  /// \cond CLASSIMP
  ClassDef(AliFemtoQinvCorrFctnWithWeights, 1);
  /// \endcond
#endif
};

inline  TH1D* AliFemtoQinvCorrFctnWithWeights::Numerator(){return fNumerator;}
inline  TH1D* AliFemtoQinvCorrFctnWithWeights::Denominator(){return fDenominator;}
inline  TH1D* AliFemtoQinvCorrFctnWithWeights::Ratio(){return fRatio;}


#endif
