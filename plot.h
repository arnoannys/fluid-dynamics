// ARNO ANNYS
// arno.annys@student.uantwerpen.be
// 28/05/2021



#ifndef PLOT_H
#define PLOT_H
#include <TROOT.h>
#include <TMatrixD.h>

class plot
{
    public:
     //constructor
     plot();
     //destructor
     ~plot();
     void imagesc(TMatrixD* , std::string ) ;
     void pyplot(TMatrixD* ) ;
     void meshgrid(TMatrixD* ) ;

    private:
     int root_im_count ; //telt aantal plots dat al gemaakt is om naamgeving automatisch te maken
     int py_count;
     int mesh_count ;
};

#endif // PLOT_H
