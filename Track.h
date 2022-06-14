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

    Double_t GetfFitParamMX()     const { return fFitParamMX; }
    Double_t GetfFitParamMY()     const { return fFitParamMY; }
    Double_t GetfFitParamNX()       const { return fFitParamNX; }
    Double_t GetfFitParamNY()     const { return fFitParamNY; }
    Double_t GetfTrackLength()     const { return fTrackLength; }
    Double_t GetfFirstPointX()      const { return fFirstPointX; }
    Double_t GetfFirstPointY()      const { return fFirstPointY; }
    Double_t GetfFirstPointZ()      const { return fFirstPointZ; }
    Double_t GetfStopPointX()      const { return fStopPointX; }
    Double_t GetfStopPointY()      const { return fStopPointY; }
    Double_t GetfStopPointZ()      const { return fStopPointZ; }
    Short_t GetfCharge()      const { return fCharge; }
    Double_t GetfTrackMomentumX()      const { return fTrackMomentumX; }
    Double_t GetfTrackMomentumY()      const { return fTrackMomentumY; }
    Double_t GetfTrackMomentumZ()      const { return fTrackMomentumZ; }
    
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

    Double_t fFitParamMX;
    Double_t fFitParamMY;
    Double_t fFitParamNX;
    Double_t fFitParamNY;
    Double_t fTrackLength;
    Double_t fFirstPointX;
    Double_t fFirstPointY;
    Double_t fFirstPointZ;
    Double_t fStopPointX;
    Double_t fStopPointY;
    Double_t fStopPointZ;
    Short_t fCharge;
    Double_t fTrackMomentumX;
    Double_t fTrackMomentumY;
    Double_t fTrackMomentumZ;

    ClassDef(Track, 2);
};


#endif /* Track */
