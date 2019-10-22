#include <math.h>
static int temp = -1;
void progressbar(double currentiter, double fullsize)
{ 
 double seekpercent = ((double)currentiter/(double)fullsize)*100;
if(((int)floor(seekpercent))%10==0 && (int)floor(seekpercent)!= temp)
  {
	cout << "\nProcess " << (int)seekpercent << "% complete..";
	temp = (int)floor(seekpercent);
  }
}
