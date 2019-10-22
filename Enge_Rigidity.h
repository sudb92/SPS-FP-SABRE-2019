double return_rigidity(double q,double B,double Ek,double m0)
{
/*double q = 2; //number of elementary charges carried by the particle
double B = 5.8 //Field in kG
double Ek = 17.613 //KE of the particle in MeV
double m0 = 4.0 //Mass of particle in amu
*/
double rho = (3.3356/q)*(sqrt(2*931.5*m0*Ek +Ek*Ek))/B;  //rho in cm
std::cout << (143.97/q)*(sqrt(m0*Ek))/B;
return rho;
}

