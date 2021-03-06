#include "BeamFlux.h"
#include <iostream>
#include <stdlib.h>
#include <boost/algorithm/string/predicate.hpp>
#include <algorithm>

double BeamFlux::GetFlux(float E,NEUTRINO::FLAVOR flav,Time_Struct t)
{
  if(cdf_splines.size()<NEUTRINO::NUMBER)
    {
      std::cout<<"BeamFlux::GetFlux cdf_splines must be loaded first"<<std::endl;
      exit(1);
    }
  double flux=std::max(0.,cdf_splines[flav]->Eval(E));
  return flux;

}

double BeamFlux::GetFlux(float phi,float cosZ,float E,NEUTRINO::FLAVOR flav,Time_Struct t)
{
  std::cout<<"BeamFlux::GetFlux(phi,cosZ,E,flav,t) should never be called"<<std::endl;

  exit(1);


}


int BeamFlux::LoadFluxTable(std::string fileName)
{
  std::ifstream fileStrm;
  fileStrm.open(fileName.c_str());
  char line[300];
  fileStrm.getline(line,300);//first line is title and source
  if(!CheckFileFormat(line))
    {
      std::cout<<"file "<<fileName<<" with first line: '"<<line<<"' not correct format for Beam flux"<<std::endl;
      return -1;
    }
  fileStrm.getline(line,300);//second line is heading
  
  //format is E,nu_e,nu_e_bar,nu_mu,nu_mu_bar,nu_tau,nu_tau_bar
  //E is the high edge of the bin

  std::vector<double> nu_e_vec,nu_e_bar_vec,nu_mu_vec,nu_mu_bar_vec,nu_tau_vec,nu_tau_bar_vec,E_vec;

  
  double nu_e,nu_e_bar,nu_mu,nu_mu_bar,nu_tau,nu_tau_bar,E;
 
  while(!fileStrm.eof())
    {
      if(!fileStrm.good())
	{
	  std::cout<<"Problem with file "<<fileName.c_str()<<" in BeamFlux::LoadFluxFromTxt"<<std::endl;
	  return -1;
	}
      fileStrm>>E>>nu_e>>nu_e_bar>>nu_mu>>nu_mu_bar>>nu_tau>>nu_tau_bar;
      
      if(E_vec.size()>0 and E==E_vec.back()) continue;
      E_vec.push_back(E);
      nu_e_vec.push_back(nu_e);
      nu_e_bar_vec.push_back(nu_e_bar);
      nu_mu_vec.push_back(nu_mu);
      nu_mu_bar_vec.push_back(nu_mu_bar);
      nu_tau_vec.push_back(nu_tau);
      nu_tau_bar_vec.push_back(nu_tau_bar);
    }
  cdf_splines.push_back(new TSpline3("nue",&E_vec[0],&nu_e_vec[0],E_vec.size()));
  cdf_splines.push_back(new TSpline3("nuebar",&E_vec[0],&nu_e_bar_vec[0],E_vec.size()));
  cdf_splines.push_back(new TSpline3("cdf_numu",&E_vec[0],&nu_mu_vec[0],E_vec.size()));
  cdf_splines.push_back(new TSpline3("cdf_numubar",&E_vec[0],&nu_mu_bar_vec[0],E_vec.size()));
  cdf_splines.push_back(new TSpline3("cdf_nutau",&E_vec[0],&nu_tau_vec[0],E_vec.size()));
  cdf_splines.push_back(new TSpline3("cdf_nutaubar",&E_vec[0],&nu_tau_bar_vec[0],E_vec.size()));

  fileStrm.close();
  return 0;

}


