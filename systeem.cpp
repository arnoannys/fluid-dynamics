// ARNO ANNYS
// arno.annys@student.uantwerpen.be
// 28/05/2021


#include "systeem.h"
#include <iostream>
#include <TMatrixD.h>
using namespace std;


//constructor
systeem::systeem(int nx , int ny , double dx , double dy):
    m_nx(control(nx)), m_ny(control(ny)) , m_dx(control(dx)), m_dy(control(dy)){

    u_mask.ResizeTo(ny, nx) ;
    v_mask.ResizeTo(ny, nx) ;
    p_mask.ResizeTo(ny, nx) ;

    value_all_edges(&u_mask,1 , 1 ,1 ,1);  //RVW voor een simpele lege doos
    value_all_edges(&v_mask,1,1,1, 1);
    value_all_edges(&p_mask,7, 2 ,4 , 3);

    m_ext_force_val = 0 ;       //default geen externe kracht
    m_band_val = 0 ;             //default geen band maar ook geen band in maskers

}
//destructor
systeem::~systeem(){

}


void systeem::add_band(double U){
    for( int j = 0 ; j < m_nx ; j++){
        u_mask(0,j) = 2 ;
        p_mask(0,j) = 1 ; }
        m_band_val = U ; //slaag op in member variable
}
void systeem::add_cyclic(){
    for( int i = 1 ; i < m_ny-1 ; i++){
        u_mask(i,0) = 3 ;
        u_mask(i,m_nx-1) = 4;
        v_mask(i,0) = 2 ;
        v_mask(i,m_nx-1) = 3;
        p_mask(i,0) = 5 ;
        p_mask(i,m_nx-1) = 6;

    }
}
void systeem::add_ext_force(double F){
    m_ext_force_val = F ; //voor kracht geen masker veranderen, wordt altijd mee gerekend is gwn default nul
}

void systeem::add_blok(int xsteps , int ysteps ){
    if (xsteps == (m_nx)/2-1 || ysteps == (m_ny)/2-1){ //conventie is langs beide kanten in x het aantal er bij doen
        cerr << "blok te breed of te hoog" ;
        exit(1) ;
    }

    for (int i = 0 ; i < ysteps ; i++){               //eerst punten in het blokje
        for (int j = 0 ; j < xsteps ; j++){
            u_mask(m_ny-1-i,(m_nx + 1)/2 +j-1) = 8 ;
            u_mask(m_ny-1-i,(m_nx + 1)/2 -j-1) = 8 ;
            v_mask(m_ny-1-i,(m_nx + 1)/2 +j-1) = 8 ;
            v_mask(m_ny-1-i,(m_nx + 1)/2 -j-1) = 8 ;
            p_mask(m_ny-1-i,(m_nx + 1)/2 +j-1) = 8 ;
            p_mask(m_ny-1-i,(m_nx + 1)/2 -j-1) = 8 ;

        }
    }

    for (int j = (m_nx + 1)/2 -xsteps -1 ; j<(m_nx + 1)/2 +xsteps ; j++ ){ //horizontaal
        u_mask(m_ny-1-ysteps,j) = 1 ;
        v_mask(m_ny-1-ysteps,j) = 1 ;
        p_mask(m_ny-1-ysteps,j) = 2 ;
     }
    for (int i = 0 ; i < ysteps ; i++ ){  //verticaal
        u_mask(m_nx-1-i, (m_nx)/2 +xsteps) = 1 ;
        u_mask(m_nx-1-i, (m_nx)/2 -xsteps) = 1 ;
        v_mask(m_nx-1-i, (m_nx)/2 +xsteps) = 1 ;
        v_mask(m_nx-1-i, (m_nx)/2 -xsteps) = 1 ;
        p_mask(m_nx-1-i, (m_nx)/2 +xsteps) = 2 ;
        p_mask(m_nx-1-i, (m_nx)/2 -xsteps) = 2 ;
    }
    //verticale p tussenstukken
    for (int i = 1 ; i < ysteps ; i++ ){ //extra aanpassing druk mask
        p_mask(m_nx-1-i, (m_nx)/2 +xsteps) = 4 ;
        p_mask(m_nx-1-i, (m_nx)/2 -xsteps) = 3 ;
    }
}
template <class T> //niet nul controle voor dx nx dy en ny
T systeem::control(T num){
    if (num == 0){
        cout << "invalid zero als input, set to 1" << endl;
        return 1 ;
    }
    else return num ;
}

void systeem::value_all_edges(TMatrixD* mat, int toprow , int bottomrow , int leftcol , int rightcol){ //alle randen een matrix setten
    for( int i = 0 ; i < mat->GetNrows() ; i++){
        (*mat)(i,0) = leftcol ;
        (*mat)(i,mat->GetNcols()-1) = rightcol;
    }
    for( int j = 0 ; j < mat->GetNcols() ; j++){
        (*mat)(0,j) = toprow ;
        (*mat)(mat->GetNrows()-1,j) = bottomrow ;
    }
}

TMatrixD* systeem::get_mask(string s){
    if (s == "u"){
    return &u_mask ;
    }
    else if (s == "v"){
        return &v_mask ;
    }
    else if (s == "p"){
        return &p_mask ;
    }
    else {
       cerr << "invalid matrix requested" ;
       exit(1);
    }
}

int systeem::get_nx(){
    return m_nx ;
}
int systeem::get_ny(){
    return m_ny ;
}
double systeem::get_dx(){
    return m_dx ;
}
double systeem::get_dy(){
    return m_dy ;
}

double systeem::get_band_val(){
    return m_band_val ;
}
double systeem::get_ext_force(){
    return m_ext_force_val ;
}
