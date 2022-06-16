// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME Dict_DRS
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "BmnTOF1Conteiner.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_BmnTOF1Conteiner(void *p = 0);
   static void *newArray_BmnTOF1Conteiner(Long_t size, void *p);
   static void delete_BmnTOF1Conteiner(void *p);
   static void deleteArray_BmnTOF1Conteiner(void *p);
   static void destruct_BmnTOF1Conteiner(void *p);
   static void streamer_BmnTOF1Conteiner(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::BmnTOF1Conteiner*)
   {
      ::BmnTOF1Conteiner *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::BmnTOF1Conteiner >(0);
      static ::ROOT::TGenericClassInfo 
         instance("BmnTOF1Conteiner", ::BmnTOF1Conteiner::Class_Version(), "BmnTOF1Conteiner.h", 23,
                  typeid(::BmnTOF1Conteiner), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::BmnTOF1Conteiner::Dictionary, isa_proxy, 16,
                  sizeof(::BmnTOF1Conteiner) );
      instance.SetNew(&new_BmnTOF1Conteiner);
      instance.SetNewArray(&newArray_BmnTOF1Conteiner);
      instance.SetDelete(&delete_BmnTOF1Conteiner);
      instance.SetDeleteArray(&deleteArray_BmnTOF1Conteiner);
      instance.SetDestructor(&destruct_BmnTOF1Conteiner);
      instance.SetStreamerFunc(&streamer_BmnTOF1Conteiner);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::BmnTOF1Conteiner*)
   {
      return GenerateInitInstanceLocal((::BmnTOF1Conteiner*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::BmnTOF1Conteiner*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr BmnTOF1Conteiner::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *BmnTOF1Conteiner::Class_Name()
{
   return "BmnTOF1Conteiner";
}

//______________________________________________________________________________
const char *BmnTOF1Conteiner::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::BmnTOF1Conteiner*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int BmnTOF1Conteiner::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::BmnTOF1Conteiner*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *BmnTOF1Conteiner::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::BmnTOF1Conteiner*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *BmnTOF1Conteiner::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::BmnTOF1Conteiner*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void BmnTOF1Conteiner::Streamer(TBuffer &R__b)
{
   // Stream an object of class BmnTOF1Conteiner.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TNamed::Streamer(R__b);
      R__b >> fPlane;
      R__b >> fStrip;
      R__b >> fTimeL;
      R__b >> fTimeR;
      R__b >> fTime;
      R__b >> fWidthL;
      R__b >> fWidthR;
      R__b >> fWidth;
      R__b >> fX;
      R__b >> fY;
      R__b >> fZ;
      R__b >> fTimeT0;
      R__b >> fWidthT0;
      R__b.CheckByteCount(R__s, R__c, BmnTOF1Conteiner::IsA());
   } else {
      R__c = R__b.WriteVersion(BmnTOF1Conteiner::IsA(), kTRUE);
      TNamed::Streamer(R__b);
      R__b << fPlane;
      R__b << fStrip;
      R__b << fTimeL;
      R__b << fTimeR;
      R__b << fTime;
      R__b << fWidthL;
      R__b << fWidthR;
      R__b << fWidth;
      R__b << fX;
      R__b << fY;
      R__b << fZ;
      R__b << fTimeT0;
      R__b << fWidthT0;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_BmnTOF1Conteiner(void *p) {
      return  p ? new(p) ::BmnTOF1Conteiner : new ::BmnTOF1Conteiner;
   }
   static void *newArray_BmnTOF1Conteiner(Long_t nElements, void *p) {
      return p ? new(p) ::BmnTOF1Conteiner[nElements] : new ::BmnTOF1Conteiner[nElements];
   }
   // Wrapper around operator delete
   static void delete_BmnTOF1Conteiner(void *p) {
      delete ((::BmnTOF1Conteiner*)p);
   }
   static void deleteArray_BmnTOF1Conteiner(void *p) {
      delete [] ((::BmnTOF1Conteiner*)p);
   }
   static void destruct_BmnTOF1Conteiner(void *p) {
      typedef ::BmnTOF1Conteiner current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_BmnTOF1Conteiner(TBuffer &buf, void *obj) {
      ((::BmnTOF1Conteiner*)obj)->::BmnTOF1Conteiner::Streamer(buf);
   }
} // end of namespace ROOT for class ::BmnTOF1Conteiner

namespace {
  void TriggerDictionaryInitialization_Dict_DRS_Impl() {
    static const char* headers[] = {
"BmnTOF1Conteiner.h",
0
    };
    static const char* includePaths[] = {
"/home/facecerep/root/include/",
"/home/facecerep/diplom/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "Dict_DRS dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$BmnTOF1Conteiner.h")))  BmnTOF1Conteiner;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "Dict_DRS dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "BmnTOF1Conteiner.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"BmnTOF1Conteiner", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("Dict_DRS",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_Dict_DRS_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_Dict_DRS_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_Dict_DRS() {
  TriggerDictionaryInitialization_Dict_DRS_Impl();
}
