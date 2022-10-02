// ARNO ANNYS
// arno.annys@student.uantwerpen.be
// 28/05/2021

#include "systeem.h"
#include "plot.h"
#include "state.h"
#include <iostream>
#include <TMatrixD.h>


using namespace std;

//verkort voorbeeld voor maken eerste plots van elk type uit verslag

int main(){
    plot plt ;

///////////////// MAAK VOORBEELDEN MASKERS//////////////////////////////////////////////////////////////////////////
     {
     systeem sys;   //heeft een default kan ook met inputs
     sys.add_band(10.);
     plt.imagesc(sys.get_mask("p"), "druk masker van de gedreven caviteit");
     plt.imagesc(sys.get_mask("u"), "u masker van de gedreven caviteit");
     plt.imagesc(sys.get_mask("v"), "v masker van de gedreven caviteit");
     }
     {
     systeem sys;   //heeft een default kan ook met inputs
     sys.add_cyclic();
     sys.add_ext_force(1.); //deze hoeft niet want doet niets aan mask voor voor de volledigheid
     plt.imagesc(sys.get_mask("p"), "druk masker bij laminaire stroming door buis");
     plt.imagesc(sys.get_mask("u"), "u masker bij laminaire stroming door buis");
     plt.imagesc(sys.get_mask("v"), "v masker bij laminaire stroming door buis");
     }
     {
     systeem sys(51,51, 0.04,0.04);   //heeft een default kan ook met inputs
     sys.add_cyclic();
     sys.add_ext_force(1.); //deze hoeft niet want doet niets aan mask voor voor de volledigheid
     sys.add_blok(5,10); //de conventie zorgt voor verdubbeling horzintaal
     plt.imagesc(sys.get_mask("p"), "druk masker voor stroming door buis met obstakel");
     plt.imagesc(sys.get_mask("u"), "u masker voor stroming door buis buis met obstakel");
     plt.imagesc(sys.get_mask("v"), "v masker voor stroming door buis buis met obstakel");
     }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////// DE GEDREVEN CAVITEIT /////////////////////////////////////////////////////////////////////////////
     {
      systeem sys(41 , 41 ,0.05, 0.05 ); //Re = 1
      sys.add_band(5.);
      state statte( &sys ) ;
      statte.evolve(0.0005 ,25000, 1. , 0.1);
      plt.meshgrid(statte.get_matrix("p"));
      plt.pyplot(statte.get_matrix("p"));
      plt.pyplot(statte.get_matrix("u"));
      plt.pyplot(statte.get_matrix("v"));
      plt.pyplot(statte.get_matrix("vorticiteit"));
     }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////// BUIS /////////////////////////////////////////////////////////////////////////////
    {
      systeem sys(41 , 41 ,0.05, 0.05 );
      sys.add_cyclic();
      sys.add_ext_force(1.);
      state statte( &sys ) ;
      statte.evolve(0.005 ,25000, 0.2 , .1);
      plt.meshgrid(statte.get_matrix("p"));
      plt.pyplot(statte.get_matrix("p"));
      plt.pyplot(statte.get_matrix("u"));
      plt.pyplot(statte.get_matrix("v"));
      plt.pyplot(statte.get_matrix("vorticiteit"));
     }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////// BUIS met obstakel /////////////////////////////////////////////////////////////////////////////
  {
     systeem sys(61 ,61 ,0.033, 0.033 );
     sys.add_cyclic();
     sys.add_ext_force(1.);
     sys.add_blok(5,10);
     state statte( &sys ) ;
     statte.evolve(0.0001 ,25000, 0.1 ,1.);
     plt.meshgrid(statte.get_matrix("p"));
     plt.pyplot(statte.get_matrix("p"));
     plt.pyplot(statte.get_matrix("u"));
     plt.pyplot(statte.get_matrix("v"));
     plt.pyplot(statte.get_matrix("vorticiteit"));
   }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


     return 0;

}
