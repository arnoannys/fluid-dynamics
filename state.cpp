// ARNO ANNYS
// arno.annys@student.uantwerpen.be
// 28/05/2021

#include "state.h"
#include <iostream>
#include <TMatrixD.h>
#include <math.h>
using namespace std;

//constructor
state::state(systeem* sys) : m_nx(sys->get_nx()) , m_ny(sys->get_ny()) , m_dx(sys->get_dx()) , m_dy(sys->get_dy())
  , u_mask(*(sys->get_mask("u"))) , v_mask(*(sys->get_mask("v"))), p_mask(*(sys->get_mask("p"))) , m_U(sys->get_band_val()) , m_F(sys->get_ext_force())
   {

    m_u.ResizeTo(m_ny, m_nx) ;
    m_v.ResizeTo(m_ny, m_nx) ;
    m_p.ResizeTo(m_ny,m_nx) ;
    vorticiteit.ResizeTo(m_ny,m_nx) ;

}
//destructor
state::~state(){

}

TMatrixD* state::get_matrix(string s){
    if (s == "u"){
    return &m_u ;
    }
    else if (s == "v"){
        return &m_v ;
    }
    else if (s == "p"){
        return &m_p ;
    }
    else if (s == "vorticiteit"){
        return &vorticiteit ;
    }
    else {
       cerr << "invalid matrix requested" ;
       exit(1);
    }
}


void state::evolve( double dt ,   int Nt , double mu , double rho){
    TMatrixD m_p2 = m_p ;  //maak nieuwe matrices waarin aangepaste waardes zitten
    TMatrixD m_u2 = m_u ;
    TMatrixD m_v2 = m_v ;

    for(int count = 0 ; count < Nt ; count++){ //loop tot aan max aantal iteraties, ook controle convergentie verder
    //slaag deze even op, minder rekenen en minder haakjes
    double FT =  (rho*m_dx*m_dx*m_dy*m_dy)/(2.*(m_dx*m_dx + m_dy*m_dy)) ;
    double idt = 1./dt ;

    //extra p iteraties
    for( int c = 0 ; c < 25 ; c++){
       m_p = m_p2 ;
    for (int i = 0 ; i < m_ny ; i++){
        for (int j = 0 ; j < m_nx ; j++){
            if (p_mask(i,j) == 0){
                m_p2(i,j) =  ( (m_p(i,j+1)+m_p(i,j-1))*m_dy*m_dy + (m_p(i+1,j)+m_p(i-1,j))*m_dx*m_dx)/(2.*(m_dx*m_dx + m_dy*m_dy)) -
                   FT*(  idt* ((m_u(i,j+1) - m_u(i,j-1))/(2.*m_dx) + (m_v(i+1,j) - m_v(i-1,j))/(2.*m_dy))
                          - pow((m_u(i,j+1) - m_u(i,j-1))/(2.*m_dx),2) - pow( (m_v(i+1,j) - m_v(i-1,j))/(2.*m_dy) , 2 ) -
                            2.*((m_u(i+1,j) - m_u(i-1,j))*(m_v(i,j+1) - m_v(i,j-1))/(2.*m_dx*2.*m_dy))  )   ;
             } //if
        }// j loop
    } // i loop
    //eerst alle nieuwe rekenen , dan pas rvw aanpassen
    //rekenen gebeurd nog steeds ifv oude waarden, gelijkstellingen al ifv nieuwe
    for (int i = 0 ; i < m_ny ; i++){
        for (int j = 0 ; j < m_nx ; j++){
            if (p_mask(i,j) == 1){
                m_p2(i,j) = 0.;
            }
            else if (p_mask(i,j )== 2){
                m_p2(i,j) = m_p2(i-1 , j) ;
            }
            else if (p_mask(i,j) == 3){
                m_p2(i,j) = m_p2(i,j-1) ;
            }
            else if (p_mask(i,j) == 4){
                 m_p2(i,j) = m_p2(i,j+1) ;
            }
            else if (p_mask(i,j) == 5){
                m_p2(i,j) =  ( (m_p(i,j+1)+m_p(i,m_nx-1))*m_dy*m_dy + (m_p(i+1,j)+m_p(i-1,j))*m_dx*m_dx) /(2.*(m_dx*m_dx + m_dy*m_dy))
                    - FT*( idt*  ((m_u(i,j+1) - m_u(i,m_nx-1))/(2.*m_dx) + (m_v(i+1,j) - m_v(i-1,j))/(2.*m_dy))
                         - pow((m_u(i,j+1) - m_u(i,m_nx-1))/(2.*m_dx),2) - pow( (m_v(i+1,j) - m_v(i-1,j))/(2.*m_dy) , 2 ) -
                            2.*((m_u(i+1,j) - m_u(i-1,j))*(m_v(i,j+1) - m_v(i,m_nx-1))/(2.*m_dx*2.*m_dy))  )   ;
            }
            else if (p_mask(i,j) == 6){
                m_p2(i,j) =  ( (m_p(i,0)+m_p(i,j-1))*m_dy*m_dy + (m_p(i+1,j)+m_p(i-1,j))*m_dx*m_dx) /(2.*(m_dx*m_dx + m_dy*m_dy))
                    - FT*(  idt* ((m_u(i,0) - m_u(i,j-1))/(2.*m_dx) + (m_v(i+1,j) - m_v(i-1,j))/(2.*m_dy))
                         - pow((m_u(i,0) - m_u(i,j-1))/(2.*m_dx),2) - pow( (m_v(i+1,j) - m_v(i-1,j))/(2.*m_dy) , 2 ) -
                            2.*((m_u(i+1,j) - m_u(i-1,j))*(m_v(i,0) - m_v(i,j-1))/(2.*m_dx*2.*m_dy))  )   ;
            }
            else if (p_mask(i,j) == 7){
                m_p2(i,j) = m_p2(i+1,j) ;
            }
            else if (p_mask(i,j) == 8){
                m_p2(i,j) =  0.000042 ; //zie verslag voor redenering hier achter, deze punten doen niet mee aan berekening
            }
            else if (p_mask(i,j) == 0){}
            else{
                cerr << "invalid mask value" ;
                exit(1);
            } //else
        } // j
    }// i
    } // extra p iters


    // u rekenen
    for (int i = 0 ; i < m_ny ; i++){
        for (int j = 0 ; j < m_nx ; j++){
            if (u_mask(i,j) == 0){
                m_u2(i,j) = m_u(i,j)-  m_u(i,j)*dt/m_dx*(m_u(i,j) - m_u(i,j-1)) - dt/m_dy*(m_u(i,j) - m_u(i-1,j))*m_v(i,j)
                            -dt/(2.*rho*m_dx)*(m_p2(i,j+1) - m_p2(i , j-1)) + mu*(   dt/(m_dx*m_dx)*(m_u(i,j+1)-2.*m_u(i,j) + m_u(i,j-1)) +
                                                                                  dt/(m_dy*m_dy)*(m_u(i+1,j) - 2.*m_u(i,j) + m_u(i-1,j))   ) + dt*m_F ;
            }
        }
    }
    //v rekenen
     for (int i = 0 ; i < m_ny ; i++){
         for (int j = 0 ; j < m_nx ; j++){
              if (v_mask(i,j) == 0){
                    m_v2(i,j) = m_v(i,j) - m_v(i,j)*dt/m_dy*(m_v(i,j) - m_v(i,j-1)) - m_u(i,j)*dt/m_dx*(m_v(i,j) - m_v(i-1,j))
                              -dt/(2.*rho*m_dy)*(m_p2(i+1,j) - m_p2(i-1 , j)) + mu*(   dt/(m_dx*m_dx)*(m_v(i,j+1)-2.*m_v(i,j) + m_v(i,j-1)) +
                                                                                      dt/(m_dy*m_dy)*(m_v(i+1,j) - 2.*m_v(i,j) + m_v(i-1,j))   ) ;
                }
            }
        }
    for (int i = 0 ; i < m_ny ; i++){
        for (int j = 0 ; j < m_nx ; j++){
            if (u_mask(i,j) == 1){
                m_u2(i,j) = 0.;
            }
            else if (u_mask(i,j)== 2){
                m_u2(i,j) = m_U ;
            }
            else if (u_mask(i,j) == 3){
                m_u2(i,j) = m_u(i,j)*(1.- dt/m_dx*(m_u(i,j) - m_u(i,m_nx-1))) - dt/m_dy*(m_u2(i,j) - m_u(i-1,j))*m_v(i,j)
                            -dt/(2.*rho*m_dx)*(m_p2(i,j+1) - m_p2(i , m_nx-1)) + mu*(   dt/(m_dx*m_dx)*(m_u(i,j+1)-2.*m_u(i,j) + m_u(i,m_nx-1)) +
                                                                                  dt/(m_dy*m_dy)*(m_u(i+1,j) - 2.*m_u(i,j) + m_u(i-1,j))   ) + dt*m_F ;
            }
            else if (u_mask(i,j) == 4){
                m_u2(i,j) = m_u(i,j)*(1.- dt/m_dx*(m_u(i,j) - m_u(i,j-1))) - dt/m_dy*(m_u(i,j) - m_u(i-1,j))*m_v(i,j)
                            -dt/(2.*rho*m_dx)*(m_p2(i,0) - m_p2(i , j-1)) + mu*(   dt/(m_dx*m_dx)*(m_u(i,0)-2.*m_u(i,j) + m_u(i,j-1)) +
                                                                                  dt/(m_dy*m_dy)*(m_u(i+1,j) - 2.*m_u(i,j) + m_u(i-1,j))   ) + dt*m_F ;
            }
            else if (u_mask(i,j) == 8){
                m_u2(i,j) =  0.000042 ;
            }
            else if (u_mask(i,j) == 0){}
            else {
                cerr << "invalid mask value" ;
                exit(1);
            }
        } // j
    }// i


    //u en v RVW
    for (int i = 0 ; i < m_ny ; i++){
        for (int j = 0 ; j < m_nx ; j++){
            if (v_mask(i,j) == 1){
                m_v2(i,j) = 0.;
            }
            else if (v_mask(i,j )== 2){
                m_v2(i,j) = m_v(i,j)*(1. - dt/m_dy*(m_v(i,j) - m_v(i,m_nx-1))) - m_u(i,j)*dt/m_dx*(m_v(i,j) - m_v(i-1,j))
                            -dt/(2.*rho*m_dy)*(m_p2(i+1,j) - m_p2(i-1 , j)) + mu*(   dt/(m_dx*m_dx)*(m_v(i,j+1)-2.*m_v(i,j) + m_v(i,m_nx-1)) +
                                                                                  dt/(m_dy*m_dy)*(m_v(i+1,j) - 2.*m_v(i,j) + m_v(i-1,j))   ) ;
            }
            else if (v_mask(i,j) == 3){
                m_v2(i,j) = m_v(i,j)*(1. - dt/m_dy*(m_v(i,j) - m_v(i,j-1))) - m_u(i,j)*dt/m_dx*(m_v(i,j) - m_v(i-1,j))
                            -dt/(2.*rho*m_dy)*(m_p2(i+1,j) - m_p2(i-1 , j)) + mu*(   dt/(m_dx*m_dx)*(m_v(i,0)-2.*m_v(i,j) + m_v(i,j-1)) +
                                                                                  dt/(m_dy*m_dy)*(m_v(i+1,j) - 2.*m_v(i,j) + m_v(i-1,j))   ) ;
            }
            else if (v_mask(i,j) == 8){
               m_v2(i,j) = 0.000042 ;
            }
            else if (u_mask(i,j) == 0){}
            else {
                cerr << "invalid mask value" ;
                exit(1);
            }
        } // j
    }// i


    //convergentie controle
    if (abs(m_p.E2Norm() - m_p2.E2Norm()) <= 0.000001*m_p2.E2Norm() &&  abs(m_u.E2Norm() - m_u2.E2Norm()) <= 0.000001*m_u2.E2Norm()  && abs(m_v.E2Norm() - m_v2.E2Norm()) <= 0.000001*m_v2.E2Norm()){
        //cout << count << endl ;
        count = Nt ;
    }

    m_p = m_p2 ; //nieuwe waardes worden oude waardes
    m_u = m_u2 ;
    m_v = m_v2 ;
    } //iterations
    //bereken nu de vorticiteit
    for (int i = 1 ; i < m_ny-1 ; i++){
        for (int j = 1 ; j < m_nx-1 ; j++){
            vorticiteit(i,j) = (m_v(i,j+1)-m_v(i,j-1))/(2.*m_dx) - (m_u(i+1,j) - m_u(i-1,j))/(2.*m_dy) ;
        }
    }
    for (int i = 1 ; i < m_ny-1 ; i++){
        vorticiteit(i,0) = (m_v(i,1)-m_v(i,0))/(m_dx) - (m_u(i+1,0) - m_u(i-1,0))/(2.*m_dy) ;
        vorticiteit(i,m_nx-1) = (m_v(i,m_nx-1)-m_v(i,m_nx-2))/(m_dx) - (m_u(i+1,m_nx-1) - m_u(i-1,m_nx-1))/(2.*m_dy) ;
    }
    for (int j = 1 ; j < m_nx-1 ; j++){
        vorticiteit(0,j) = (m_v(0,j+1)-m_v(0,j-1))/(2.*m_dx) - (m_u(1,j) - m_u(0,j))/(m_dy) ;
        vorticiteit(m_ny-1,j) = (m_v(m_ny-1,j+1)-m_v(m_ny-1,j-1))/(2.*m_dx) - (m_u(m_ny-1,j) - m_u(m_ny-2,j))/(m_dy) ;
    }
    vorticiteit(0,0) = (m_v(0,1)-m_v(0,0))/(m_dx) - (m_u(1,0) - m_u(0,0))/(m_dy) ;
    vorticiteit(0,m_nx-1) = (m_v(0,m_nx-1)-m_v(0,m_nx-2))/(m_dx) - (m_u(1,m_nx-1) - m_u(0,m_nx-1))/(m_dy) ;
    vorticiteit(m_ny-1,0) = (m_v(m_ny-1,1)-m_v(m_ny-1,0))/(m_dx) - (m_u(m_ny-1,0) - m_u(m_ny-2,0))/(m_dy) ;
    vorticiteit(m_ny-1,m_nx-1) = (m_v(m_ny-1,m_nx-1)-m_v(m_ny-1,m_nx-2))/(m_dx) - (m_u(m_ny-1,m_nx-1) - m_u(m_ny-2,m_nx-1))/(m_dy) ;
    cout << "done " << endl ;
}// evolve

