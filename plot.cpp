// ARNO ANNYS
// arno.annys@student.uantwerpen.be
// 28/05/2021


#include "plot.h"
#include <TMatrixD.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TColor.h>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;


//constructor
plot::plot(){
   root_im_count = 0 ;
   py_count = 0;
   mesh_count = 0;
}
//destructor
plot::~plot(){

}
void plot::imagesc(TMatrixD* mat , string s){
    string tit = s + ";X ;Y" ;
    char ctit[tit.size() + 1];          //omzetten is c style array om in de title te kunnen
    strcpy(ctit,tit.c_str());
    TH2D *im = new TH2D("im", ctit, mat->GetNcols(), 1, mat->GetNcols(), mat->GetNrows(), 1, mat->GetNrows());


     for(int i = 0 ; i < mat->GetNrows() ; i++){ //vullen histogram om hist plot te doen
         for( int j = 0 ; j < mat->GetNcols() ; j++){
            im->SetBinContent(j+1, mat->GetNrows()-i , (*mat)(i,j)) ;
            // i is rows = verticaal , y is verticaal, matrix eerste index bovenaan, hist eerste indew onderaan daarom
            //moet gecompenseert worden voor dit verschil in conventie
        }
    }
     string strN = to_string(root_im_count) ;
     char cstr[strN.size() + 1];          //omzetten is c style array om in de title te kunnen
     strcpy(cstr,strN.c_str());

     string strN2 = "root-im" + strN + ".png" ;
     char cstr2[strN2.size() + 1];          //omzetten is c style array om in de title te kunnen
     strcpy(cstr2,strN2.c_str());


    TCanvas* canvas = new TCanvas(cstr,"my canvas");
    gStyle->SetPalette(55) ;
    im->SetStats(0) ;
    im->Draw("colz");
    canvas->Print(cstr2, "png");
    root_im_count++ ;
}

void plot::pyplot(TMatrixD* mat){ //simpel wegschrijven naar bestand met namen die gewoon genummerd worden
     ofstream myfile;
     myfile.open("py-data" + to_string(py_count) + ".csv") ;
     for(int i = 0 ; i < mat->GetNrows() ; i++){
         for( int j = 0 ; j < mat->GetNcols() ; j++){
            myfile << (*mat)(i,j) << "," ;
        }
         myfile << endl ;
    }
    myfile.close() ;
    py_count++ ;
}

void plot::meshgrid(TMatrixD* mat){  //wegschrijven van het onderliggend grid (indiches niet ruimtelijke waarden)
    ofstream myfile;
    myfile.open( "x"+ to_string(mesh_count) +  ".csv") ;
    for(int i = 0 ; i < mat->GetNrows() ; i++){
        for( int j = 0 ; j < mat->GetNcols() ; j++){
           myfile << j << "," ;
       }
        myfile << endl ;
   }
    myfile.close() ;

    myfile.open( "y"+ to_string(mesh_count) +  ".csv") ;
    for(int i = 0 ; i < mat->GetNrows() ; i++){
        for( int j = 0 ; j < mat->GetNcols() ; j++){
           myfile << i << "," ;
       }
        myfile << endl ;
   }
    myfile.close() ;
    mesh_count++ ;
}


