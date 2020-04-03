/*
load_channel_map.h

Header file that loads an index file carrying information formatted as

ch#  detID  Side  PosID  Elow  Ehigh

Where 

	 - ch# runs from 0 upwards to 16*N with N= number of digitizer boards in a chain
	 - detID is a string label that identifies the detector (for sabre it could be A-E for instance)
	 - Side is a string label preferably used to denote the position information (Front/Back, Left/Right etc.)
	 - posID is the positional number of the detector segment (0-7 for SABRE backs, 0-15 for SABRE fronts, say)
	 - Elow and Ehigh are additional numbers that can allow to set numbered 'cuts' in data

B.Sudarsan, E.Good, K.Macon
sbalak2@lsu.edu
Fall2019-Spring2020
*/



#pragma once
#include <iostream>
#include <fstream>
#include <map>

struct sabrechan
{
  std::string detID;
  std::pair<std::string,int> side_pos;
  int Ecutlo;  //To use in order to get rid of pulser peak events when processing
  int Ecuthi;  // -do-
};

std::map<int,sabrechan> sps_channelmap;

int load_channel_map()
{
  std::ifstream in;
  in.open("etc/Feb2020_channelMap.txt");
  if(!in.is_open())
  {
    std::cerr << "Error opening channelmap file ";
    return -1;
  }

  //ChannelMapSabre has
  int ch_1;
  sabrechan channel;  
  while(!in.eof())
  {
    in>>ch_1>>channel.detID>>channel.side_pos.first>>channel.side_pos.second>>channel.Ecutlo>>channel.Ecuthi;
    sps_channelmap[ch_1] = channel;
  }

  in.close();
  return 1;
}