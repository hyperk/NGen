#ifndef _FluxGen_
#define _FluxGen_

#include <string>
#include "TRandom3.h"
#include "EvtRate.h"
#include "EvtVector.h"
#include "FlxTable.h"
#include "DetectorGeom.h"
#include "Track.h"
#include "Writer.h"
#include "TVector3.h"
#include "TimeStruct.h"
/// Base class FluxGen
/** Abstract base class for Flux Generator*/
class FluxGen
{
  //abstract base class for flux generation.  Handles main work in simulation
  // all virtual funtions should return 0 if successful, -1 if unsuccesful
 public:
  /// Constructor
  FluxGen();
  
  virtual ~FluxGen();


  
  void Run();
  int CreateEvtRate(){Time_Struct t(0,0);return CreateEvtRate(t);};
  int CreateEvtRate(Time_Struct t);
  int CreateNextVector(); // return 0 on success, 1 to skip writeout of this event, 2 if no more events left, -1 on error
  int WriteCurrentVector();
  void AddNewWriter(Writer * writer,const char* name);
  void CloseWriters(); //closes files and clear memory of writers
  void SetSeed(unsigned seed);
  void SetDetector(DetectorGeom* t);
  void SetNYears(float y);
  void SetPOT(float pot){POT=pot;};
  void SetFlxTable(FlxTable* tbl);
  void SetvX(TVector3 v){vX->SetXYZ(v.X(),v.Y(),v.Z());};//x-unit vector of final coordinates in generator coords
  void SetvY(TVector3 v){vY->SetXYZ(v.X(),v.Y(),v.Z());};//y-unit vector of final coordinates in generator coords
  void SetvZ(TVector3 v){vZ->SetXYZ(v.X(),v.Y(),v.Z());};//z-unit vector of final coordinates in generator coords
  void SetSpectralIndecies(std::vector<float>);
  void SetRatios(std::vector<float>);
  void SetEMode(int i){EMode=i;};
  void SetDirMode(int i){dirMode=i;};
 
  void SetEThresh(double eth){e_thresh=eth;};
  void SetEMax(double em){e_max=em;};
  void SetStartTime(Time_Struct t){startTime=t;};
  void SetEndTime(Time_Struct t){endTime=t;};
 protected:
  virtual int Setup(){return 0;};//return 0 on success, negative on error
  int OnlyOnceSetup();
  void RunLoop();
  void RotateCoords(); //transforms from coords of flux generator to requested final coords
  int GetRandDirection(NEUTRINO::FLAVOR flav, float e, TVector3 * dir);
  virtual int GetRandDir(NEUTRINO::FLAVOR flav,float e,TVector3 * dir);
  //sets dir as random selected momentum direction according to distribution in tables
  
  virtual bool CheckEHook(float p,TVector3 pos){return true;};
  
  virtual float GetRandE(NEUTRINO::FLAVOR flav);
  //returns random energy in units of GeV
  float GetRandEStepping(NEUTRINO::FLAVOR flav);
  float GetRandEInversion(NEUTRINO::FLAVOR flav);
  double GetCrossSec(NEUTRINO::FLAVOR flav, float E); //considers e_thresh, e_max
  virtual double GetCrossSection(NEUTRINO::FLAVOR flav, float E)=0;
//return cross section in units of 10^-38 cm^2 nucleons^-1, E in units of GeV
  
  virtual int ProcessIncomingTrack() =0; //0 if successful, 1 to skip writeout,-1 on error
  
  void RotatePoint(TVector3 *v);
  float nYears;
  float POT; //protons on target, for beam, units  of 10e20
  TRandom3 * rndm;

  EvtRate * evtRate;
  EvtVector * currentVect;

  FlxTable * flxTbl;

  double e_thresh;
  double e_max;

  bool hasBeenSetup;
  Time_Struct startTime,endTime;
  DetectorGeom* detector;
  TVector3 * vX;
  TVector3 * vY;
  TVector3 * vZ;
  std::vector<double> fluxCSIntE;
  std::vector<double> fluxCSIntMu;
  std::vector<double> fluxCSIntEBar;
  std::vector<double> fluxCSIntMuBar;
  std::vector<double> fluxCSIntTau;
  std::vector<double> fluxCSIntTauBar;
  std::vector<Writer *> Writers;

  int EMode; //0 for Stepping, 1 for Inversion sampling
  int dirMode; //0 calc here base on flux distro, 1 get from call to flux table
  std::vector<float> specinds;
  std::vector<float> ratios;
  //  int count;
  // int count2,count3;
};


#endif
