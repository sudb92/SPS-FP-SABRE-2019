#include <iostream>
#include <TGraph.h>
#include <vector>
#include "Enge_Rigidity.h"
#include <TCanvas.h>
#include <TText.h>

void specplot()
{
double states[7] = {
9.585,
9.844,
10.356,
11.096,
11.261,
11.520,
11.6};
std::vector<double> x;
std::vector<double> y;

for(auto& state: states)
{
std::cout << return_rigidity(2,5.8,state,4.0) << std::endl;
x.push_back(2000.0-return_rigidity(2,5.8,state,4.0));
y.push_back(1);
}
TCanvas* ca = new TCanvas();

ca->cd();
TGraph* gr = new TGraph(x.size(),x.data(),y.data());
gr->Draw("AC*");
ca->Modified();
ca->Update();

for(int i=0; i< x.size() ; i++)
{
auto t = new TText(x.data()[i],y.data()[i]+0.05*i,Form("%g", states[i]));
//printf("Peakpos:%g ",xp[i]); 
//t->SetTextSize();
t->Draw();
}

ca->Update();


}
