#include <string>
#include <TROOT.h>

using std::string;

class DPPchannel {//,ch,brd,en,enShort,flag,name) {
public : 
   ULong64_t Timestamp;
   UShort_t  Channel;
   UShort_t  Board;
   UShort_t  Energy;
   UShort_t  EnergyShort;
   string    Name;
   UInt_t    Flags;
   DPPchannel(ULong64_t, UShort_t, UShort_t, 
         	UShort_t, UShort_t, UInt_t);
};

DPPchannel::DPPchannel(ULong64_t ts, UShort_t c, UShort_t b, UShort_t en, UShort_t  ens, UInt_t flgs): Timestamp(ts), Channel(c), Board(b), Energy(en), EnergyShort(ens), Flags(flgs) 
{
   Name = Form("%d_%d",Board,Channel);
}
