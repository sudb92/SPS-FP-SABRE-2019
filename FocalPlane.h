#include <TROOT.h>
//redoing this class to have ``signal name'' keys in a map would be better possibly. I couldn't get map to play nice with root. I expect this is due to how root was compiled? or what compiler we are forced to use :-)
class FocalPlane {
public :
   double delayTimeFL;
   double delayTimeFR;
   double delayTimeBL;
   double delayTimeBR;
   double anodeTimeMF;
   double anodeTimeMB;

   double delayLongFL;
   double delayLongFR;
   double delayLongBL;
   double delayLongBR;
   double anodeLongMF;
   double anodeLongMB;

   double delayShortFL;
   double delayShortFR;
   double delayShortBL;
   double delayShortBR;
   double anodeShortMF;
   double anodeShortMB;

   int hits; //What about multiple hit patterns??... 
   FocalPlane();
   void Reset();
};

FocalPlane::FocalPlane() : delayTimeFL(0), delayTimeFR(0), delayTimeBL(0), delayTimeBR(0), anodeTimeMF(0), anodeTimeMB(0), 
			   delayLongFL(0), delayLongFR(0), delayLongBL(0), delayLongBR(0), anodeLongMF(0), anodeLongMB(0), delayShortFL(0),
                           delayShortFR(0), delayShortBL(0), delayShortBR(0), anodeShortMF(0), anodeShortMB(0), hits(0x000000) {}

void FocalPlane::Reset()
{
   delayTimeFL = 0;
   delayTimeFR = 0;
   delayTimeBL = 0;
   delayTimeBR = 0;
   anodeTimeMF = 0;
   anodeTimeMB = 0; 
   delayLongFL = 0;
   delayLongFR = 0;
   delayLongBL = 0;
   delayLongBR = 0;
   anodeLongMF = 0;
   anodeLongMB = 0;
   delayShortFL = 0;
   delayShortFR = 0;
   delayShortBL = 0;
   delayShortBR = 0; 
   anodeShortMF = 0; 
   anodeShortMB = 0;
   hits = 0x000000;  //horrible
}


// Channel Map definitions for reference
   // (make more elegant for final version)
//   UShort_t delayFLchan = 8;
//   UShort_t delayFRchan = 9;
//   UShort_t delayBLchan = 10;
//   UShort_t delayBRchan = 11;
//   UShort_t anodeMFchan = 12;
//   UShort_t anodeMBchan = 13;
