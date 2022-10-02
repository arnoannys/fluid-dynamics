// ARNO ANNYS
// arno.annys@student.uantwerpen.be
// 28/05/2021
#ifndef STATE_H
#define STATE_H

#include "systeem.h"
#include <TROOT.h>
#include <TMatrixD.h>

class state{

    public:
    //constructor
    state( systeem* );
    //destructor
    ~state();
    void evolve(double , int , double , double ) ;
    TMatrixD* get_matrix(std::string) ;
    private:
    int m_nx;
    int m_ny;
    double m_dx;
    double m_dy;
    TMatrixD m_u;
    TMatrixD  m_v ;
    TMatrixD  m_p ;
    TMatrixD u_mask;
    TMatrixD  v_mask ;
    TMatrixD  p_mask ;
    TMatrixD vorticiteit ;
    double m_U ;
    double m_F ;

};
#endif // STATE_H
