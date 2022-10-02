// ARNO ANNYS
// arno.annys@student.uantwerpen.be
// 28/05/2021

#ifndef SYSTEEM_H
#define SYSTEEM_H
#include <TROOT.h>
#include <TMatrixD.h>



class systeem{

    public:
    //constructor
    systeem(int, int , double , double);
    systeem() : systeem( 41 , 41 , 0.05 , 0.05) {}; // default constructor
    //destructor
    ~systeem();

    template <class T> //template klasse voor controle zowel double als int
    T control(T) ;

    void value_all_edges(TMatrixD*, int, int , int , int) ;
    TMatrixD* get_mask(std::string) ;
    void add_band(double) ;
    void add_cyclic() ;
    void add_blok(int , int) ;
    void add_ext_force(double ) ;
    int get_nx() ;
    int get_ny() ;
    double get_dx() ;
    double get_dy() ;
    double get_band_val() ;
    double get_ext_force() ;


    private:
     int m_nx;
     int m_ny;
     double m_dx ;
     double m_dy ;
     TMatrixD u_mask;
     TMatrixD  v_mask ;
     TMatrixD  p_mask ;

     double m_ext_force_val ;
     double m_band_val ;

};
#endif // SYSTEEM_H
