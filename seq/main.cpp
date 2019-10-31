
#include <iostream>
#include <vector>
#include <string.h>
using namespace std;

//Definir consntantes
/*
constante G

*/

struct asteroide {                      //Clase asteroide (objeto)
    double posX;
    double posY;
    double velozX;
    double velozY;



    /*
    poisicion => Como posx y posY o un vector
    velocidad 
    aceleracion
    */
    double masa;
};

struct planeta {            
    double posX;
    double posY;
    double masa;
};

void printError() {

    cerr << " nasteroids-seq: Wrong arguments." << endl;
    cerr << " Correct use:" << endl;
    cerr << " nasteroids-seq num_asteroides num_iteraciones num_planetas semilla" << endl;

}





int main(int argc, char *argv[]) {                          //argc= numero de argumentos, argv= argumentos metidos en la ejecucion argv[0] -> Nombre programa, los siguientes los que sean



    
    if (argc < 0) {

    
        printError(); 
    }



    double primerParametroDouble = atof(argv[1]); 
    if(primerParametroDouble==0){
        for(unsigned int i=0; i<strlen(argv[1]); i++){
            if(argv[1][i] == '0') {
					//valid_param = 0;
					//Cuando se comprueba el ultimo parametro y despues de verificar que es un numero entero, se almacena cada signo en la variable entera suma (se almacena en codigo ascii)
			}
            else{
                printError();
                return -1;
            }
        }
    }
    /*cout<< "Error"; 
    else{
    cout << primerParametroDouble << endl;  */  
    int primerParametroInt = atoi(argv[1]);
    cout << primerParametroInt << endl;


    double resta = primerParametroDouble - primerParametroInt;    //0,4

    cout << resta << endl;
    //}
/*

    int num_asteroides = atoi(argv[1]);
    int num_iteraciones = atoi(argv[2]);
    int num_planetas = atoi(argv[3]);
    int semilla = atoi(argv[4]);


    */

    

}