/*
BuiltEvent.h

Class that encapsulates all possible signals arising from both the focal-plane detector and SABRE array in the spectrograph.
The eventbuilder dispatches channel-level data within each event into elements of objects of this class.

Based on FocalPlane class written in earlier versions of this code.

B.Sudaran, E.Good, K.Macon
Fall2019-Spring2020
sbalak2@lsu.edu
*/


#pragma once
class BuiltEvent {
public :
   double delayTimeFL;
   double delayTimeFR;
   double delayTimeBL;
   double delayTimeBR;
   double anodeTimeMF;
   double anodeTimeBF;
   double anodeTimeMB;
   double scintTimeL;
   double scintTimeR;
   double cathodeTime;

   double delayLongFL;
   double delayLongFR;
   double delayLongBL;
   double delayLongBR;
   double anodeLongMF;
   double anodeLongBF;
   double anodeLongMB;
   double scintLongL;
   double scintLongR;
   double cathodeLong;

   double delayShortFL;
   double delayShortFR;
   double delayShortBL;
   double delayShortBR;
   double anodeShortMF;
   double anodeShortBF;
   double anodeShortMB;
   double scintShortL;
   double scintShortR;
   double cathodeShort;

   double xavg;
   double yavg;
   double theta;
   double phi;

   double plastic_esum;
   double rf_scint_time;

   //Maximum E values recorded in F and B for SABRE
   double MaxSabreFrontLong;
   double MaxSabreBackLong;

   double MaxSabreFrontTime;
   double MaxSabreBackTime;   

   double MaxSabreFrontCh;
   double MaxSabreBackCh;   

   BuiltEvent();
   void Reset();
};

BuiltEvent::BuiltEvent() : delayTimeFL(0), delayTimeFR(0), delayTimeBL(0), delayTimeBR(0), anodeTimeMF(0), anodeTimeMB(0), 
			   delayLongFL(0), delayLongFR(0), delayLongBL(0), delayLongBR(0), anodeLongMF(0), anodeLongMB(0), delayShortFL(0),
                           delayShortFR(0), delayShortBL(0), delayShortBR(0), anodeShortMF(0), anodeShortMB(0) {}

void BuiltEvent::Reset()
{
   delayTimeFL = 0;
   delayTimeFR = 0;
   delayTimeBL = 0;
   delayTimeBR = 0;
   anodeTimeMF = 0;
   anodeTimeBF = 0;
   anodeTimeMB = 0; 
   scintTimeL  = 0;
   scintTimeR  = 0;

   delayLongFL = 0;
   delayLongFR = 0;
   delayLongBL = 0;
   delayLongBR = 0;
   anodeLongMF = 0;
   anodeLongBF = 0;
   anodeLongMB = 0;
   scintLongL = 0;
   scintLongR = 0;

   delayShortFL = 0;
   delayShortFR = 0;
   delayShortBL = 0;
   delayShortBR = 0; 
   anodeShortMF = 0; 
   anodeShortBF = 0;
   anodeShortMB = 0;
   scintShortL = 0;
   scintShortR = 0;

   MaxSabreFrontLong = 0;
   MaxSabreBackLong = 0;

   MaxSabreFrontTime = 0;
   MaxSabreBackTime = 0;

   MaxSabreFrontCh = -1;
   MaxSabreBackCh = -1;   

   xavg=-16387;
   yavg=-16387;
   theta=-16387;
   phi=-16387;

   plastic_esum=-16387;
   rf_scint_time=-16387;
}