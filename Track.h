#ifndef TRACK
#define TRACK

#include "TObject.h"
#include "TNamed.h"
#include <iostream>

using namespace std;

class Track : public TNamed {

public:
    Track();
    //Track(Short_t plane, Short_t strip, Float_t timeL, Float_t timeR, Float_t time, Float_t widthL, Float_t widthR, Float_t width, Float_t x, Float_t y, Float_t z, Float_t timeT0, Float_t widthT0);
    virtual ~Track();

    Float_t GetfFitParamMX()     const { return fFitParamMX; }
    Float_t GetfFitParamMY()     const { return fFitParamMY; }
    Float_t GetfFitParamNX()       const { return fFitParamNX; }
    Float_t GetfFitParamNY()     const { return fFitParamNY; }
    Float_t GetfTrackLength()     const { return fTrackLength; }
    Float_t GetfFirstPointX()      const { return fFirstPointX; }
    Float_t GetfFirstPointY()      const { return fFirstPointY; }
    Float_t GetfFirstPointZ()      const { return fFirstPointZ; }
    Float_t GetfStopPointX()      const { return fStopPointX; }
    Float_t GetfStopPointY()      const { return fStopPointY; }
    Float_t GetfStopPointZ()      const { return fStopPointZ; }
    Float_t GetfCharge()      const { return fCharge; }
    Float_t GetfTrackMomentumX()      const { return fTrackMomentumX; }
    Float_t GetfTrackMomentumY()      const { return fTrackMomentumY; }
    Float_t GetfTrackMomentumZ()      const { return fTrackMomentumZ; }
    
    // void SetParameters(Short_t plane, Short_t strip, Float_t timeL, Float_t timeR, Float_t time, Float_t widthL, Float_t widthR, Float_t width, Float_t x, Float_t y, Float_t z, Float_t timeT0, Float_t widthT0);
    // void SetPlane(Short_t v)      { fPlane = v; }
    // void SetStrip(Short_t v)      { fStrip = v; }
    // void SetTimeL(Float_t v)      { fTimeL = v; }
    // void SetTimeR(Float_t v)      { fTimeR = v; }
    // void SetTime(Float_t v)       { fTime = v; }
    // void SetWidthL(Float_t v)     { fWidthL = v; }
    // void SetWidthR(Float_t v)     { fWidthR = v; }
    // void SetWidth(Float_t v)      { fWidth = v; }
    // void SetX(Float_t v)          { fX = v; }
    // void SetY(Float_t v)          { fY = v; }
    // void SetZ(Float_t v)          { fZ = v; }
    // void SetTimeT0(Float_t v)     { fTimeT0 = v; }
    // void SetWidthT0(Float_t v)    { fWidthT0 = v; }
    
    void Clear();
    
private: 

    Float_t fFitParamMX;
    Float_t fFitParamMY;
    Float_t fFitParamNX;
    Float_t fFitParamNY;
    Float_t fTrackLength;
    Float_t fFirstPointX;
    Float_t fFirstPointY;
    Float_t fFirstPointZ;
    Float_t fStopPointX;
    Float_t fStopPointY;
    Float_t fStopPointZ;
    Float_t fCharge;
    Float_t fTrackMomentumX;
    Float_t fTrackMomentumY;
    Float_t fTrackMomentumZ;

    ClassDef(Track, 2);
};


#endif /* Track */