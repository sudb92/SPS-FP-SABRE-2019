#include <iostream>
#include <fstream>
#include <map>

//enum Side_Si_Detector {FRONT,BACK,UNUSED};
//enum ID_Si_Detector {A,B,C,D,E,UNUSED}
struct sabrechan
{
  std::string detID;
  std::pair<std::string,int> side_pos;
  int Ecutlo;  //To use in order to get rid of pulser peak events when processing
  int Ecuthi;  // -do-
};
std::map<int,sabrechan> map1;

int test_map()
{
  ifstream in;
     //map[0-127] should return the sabrechan instance with all the properties
  in.open("ChannelMapSabre-Edit-SepOct2019.dat");
  if(!in.is_open())
  {
    std::cerr << "Error opening file ";// << in << " ..." << std::endl;
    return -1;
  }

  //ChannelMapSabre has
  // ch#  detID  Side  PosID  Elow  Ehigh
  int ch_1;
  sabrechan channel;  
  while(!in.eof())
  {
    in>>ch_1>>channel.detID>>channel.side_pos.first>>channel.side_pos.second>>channel.Ecutlo>>channel.Ecuthi;
    //std::cout <<ch_1<<"\t"<<channel.detID<<"\t"<<channel.side_pos.first<<"\t"<<channel.side_pos.second<<"\t"<<channel.Ecutlo<<"\t"<<channel.Ecuthi<<std::endl;
    map1[ch_1] = channel;
  }
/*
  for(int i=0; i<128; i++)
  {
    
    if(map1[i].side_pos.first == "FRONT" && map1[i].detID == "A")
		std::cout << i <<" " << map1[i].detID << " " << map1[i].side_pos.first << " " << map1[i].side_pos.second <<std::endl;
  }*/
  in.close();
  return 1;
}
