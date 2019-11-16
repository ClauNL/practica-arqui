#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string.h>
#include <ctype.h>									//Libreria para verificar el tipo de datos, osea los argumentos que mete el usuario (funcion isdigit)
#include <random>									

using namespace std;

const double G = 6.674e-5;
const double intervaloTiempo = 0.1;
const double distanciaMin = 2;
const double width = 200;
const double height = 200;
const double masa = 1000;
const double sdm = 50;



struct asteroide {                      //Clase asteroide (objeto)
    double posX;
    double posY;
    double velozX;
    double velozY;
	double aceleracionX;
	double aceleracionY;
	double fuerzaX;				//Fuerza total sobre el (Todos los objetos)
	double fuerzaY;				
    double masa;


	void actualizarFuerzas(double nuevaFuerzaX, double nuevaFuerzaY) {
		fuerzaX = fuerzaX + nuevaFuerzaX;
		fuerzaY = fuerzaY + nuevaFuerzaY;
	}

	void actualizarPosicion() {
		aceleracionX = fuerzaX / masa;
		aceleracionY = fuerzaY / masa;
		velozX = velozX + aceleracionX * intervaloTiempo;
		velozY = velozY + aceleracionY * intervaloTiempo;
		posX = posX + velozX * intervaloTiempo;
		posY = posY + velozY * intervaloTiempo;
	}

	void comprobacionFuerzas() {
		if(fuerzaX > 100){
			fuerzaX = 100;
		} 
		else if(fuerzaX < -100){
			fuerzaX = -100;
		}

		if(fuerzaY > 100){
			fuerzaY = 100;
		} 
		else if(fuerzaY < -100){
			fuerzaY = -100;
		}
	}

	void resetearFuerzas() {
		fuerzaX = 0;
		fuerzaY = 0;
	}

};

struct planeta {            
    double posX;
    double posY;
    double masa;
};
								 		 
bool parametrosCorrectos(int argc, char *argv[]) {

	if (argc == 5 && (atoi(argv[4]) > 0)) {			//Si se pone un string solamente el atoi sera 0 y no entrara				
		for (int i = 1; i < argc; i++) {	
			for (unsigned int j = 0; j < strlen(argv[i]); j++) {
				if (!isdigit(argv[i][j])) {								//isdigit> Check if character is decimal digit						
					return 0;
			    }
			}
		}
		return 1;
	}

	return 0;
}	

void printError() {
	cerr << "nasteroids-seq: Wrong arguments." << endl;	
	cerr << "Correct use: " << endl;
	cerr << "nasteroids-seq num_asteroides num_iteraciones num_planetas semilla" << endl;	
}	



double calcularDistancia(double x1, double y1, double x2, double y2) {
	double distancia = sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
	return distancia;
}

double calcularPendiente(double x1, double y1, double x2, double y2) {
	double pendiente = (y1 - y2) / (x1 - x2);

	if (pendiente > 1) {
		return 1;
	}
	else if (pendiente < -1) {
		return -1;
	}
	else {
		return pendiente;
	}
}

double calcularFuerzaAtraccion(double masa1, double masa2, double distancia) {
	return (G*masa1*masa2) / pow(distancia, 2);
}	



int main(int argc, char *argv[]) {                         

	if (!parametrosCorrectos(argc, argv)) {					//Si los parametros no son correctos finaliza el programa
		printError();
		return -1;
	}


	//CONVERTIMOS LOS ARGUMENTOS PASADOS AL PROGRAMA A ENTEROS
	int num_asteroides, num_iteraciones, num_planetas, semilla;
	num_asteroides=atoi(argv[1]);
	num_iteraciones=atoi(argv[2]);
	num_planetas=atoi(argv[3]);
	semilla=atoi(argv[4]);

	//ARRAY DE ASTEROIDES Y PLANETAS
	vector<asteroide> vectorAsteroides(num_asteroides);    
	vector<planeta> vectorPlanetas(num_planetas);

	//FICHERO INICIAL CON LOS PARAMETROS INICIALES
	ofstream ficheroParametros;
	ficheroParametros.open("init_conf.txt");

	ficheroParametros <<num_asteroides<<" "<<num_planetas<<" "<<num_iteraciones<<" "<<semilla<<endl;
	                           

	//GENERACION DE PARAMETROS DE SIMULACION
	default_random_engine re {semilla};
	uniform_real_distribution<double> xdist{0.0, nextafter(width, numeric_limits<double>::max())};
	uniform_real_distribution<double> ydist{0.0, nextafter(height, numeric_limits<double>::max())};
	normal_distribution<double> mdist{masa, sdm};

	//ASIGNACION DE PARAMETROS A LOS ASTEROIDES
	for(int i = 0; i < num_asteroides; i++){
		vectorAsteroides[i].posX = xdist(re);
		vectorAsteroides[i].posY = ydist(re);
		vectorAsteroides[i].masa = mdist(re);
		
		ficheroParametros <<fixed <<setprecision(3)<<vectorAsteroides[i].posX<<" "<<fixed <<setprecision(3)<<vectorAsteroides[i].posY<<" "<<fixed <<setprecision(3)<<vectorAsteroides[i].masa<<" "<<endl;
		/*
		cout <<  "Asteroide " << i << endl;
		cout << "        Posicion: " << vectorAsteroides[i].posX << " "<< vectorAsteroides[i].posY << " Masa: " << vectorAsteroides[i].masa << endl;
		*/
	}

	//ASIGNACION DE PARAMETROS A LOS PLANETAS
	for(int i = 0; i < num_planetas; i++){

		if (i % 4 == 0) {
			vectorPlanetas[i].posX = 0;
			vectorPlanetas[i].posY = ydist(re);
		}
		else if (i % 4 == 1) {
			vectorPlanetas[i].posX = xdist(re);
			vectorPlanetas[i].posY = 0;
		}
		else if (i % 4 == 2) {
			vectorPlanetas[i].posX = width;
			vectorPlanetas[i].posY = ydist(re);
		}
		else if (i % 4 == 3) {
			vectorPlanetas[i].posX = xdist(re);
			vectorPlanetas[i].posY = height;
		}

		vectorPlanetas[i].masa = mdist(re) * 10;
		ficheroParametros <<fixed <<setprecision(3)<<vectorPlanetas[i].posX<<" "<<fixed <<setprecision(3)<<vectorPlanetas[i].posY<<" "<<fixed <<setprecision(3)<<vectorPlanetas[i].masa<<" "<<endl;
		/*
		cout << endl;
		cout <<  "Planeta " << i << endl;
		cout << "        Posicion: " << vectorPlanetas[i].posX << " "<< vectorPlanetas[i].posY << " Masa: " << vectorPlanetas[i].masa << endl;
		*/
	}



	ficheroParametros.close();

		

																	
	
	for (int i = 0; i < num_iteraciones; i++) {	

		for (int j = 0; j < num_asteroides; j++) {
			vectorAsteroides[j].resetearFuerzas();									//Reseteamos para olvidar las anteriores y calcular las nuevas
		}
		//CALCULO DE FUERZAS DE ATRACCION 															
		for (int j = 0; j < num_asteroides; j++) {									//Fuerza asterioide con asteroide 

			for (int k = 0; k < num_asteroides; k++) {
				double distancia = calcularDistancia(vectorAsteroides[j].posX, vectorAsteroides[j].posY, vectorAsteroides[k].posX, vectorAsteroides[k].posY);
			
				if (distancia != 0 && distancia > 5) {						//Si no es el mismo asteroide con el mismo
					double pendiente = calcularPendiente(vectorAsteroides[j].posX, vectorAsteroides[j].posY, vectorAsteroides[k].posX, vectorAsteroides[k].posY);
					double angulo = atan(pendiente);
					double fuerzaAtraccion = calcularFuerzaAtraccion(vectorAsteroides[j].masa, vectorAsteroides[k].masa, distancia);
					
					double fuerzaX = fuerzaAtraccion * cos(angulo);
					double fuerzaY = fuerzaAtraccion * sin(angulo);	

					vectorAsteroides[j].comprobacionFuerzas();
					vectorAsteroides[j].actualizarFuerzas(fuerzaX, fuerzaY);
					vectorAsteroides[k].actualizarFuerzas(-fuerzaX, -fuerzaY);
				}
			}
		

			for (int k = 0; k < num_planetas; k++) {								//Fuerza asterioide con planetas
				double distancia = calcularDistancia(vectorAsteroides[j].posX, vectorAsteroides[j].posY, vectorPlanetas[k].posX, vectorPlanetas[k].posY);
			
				if (distancia != 0 && distancia > 5) {						//Si no es el mismo asteroide con el mismo
					double pendiente = calcularPendiente(vectorAsteroides[j].posX, vectorAsteroides[j].posY, vectorPlanetas[k].posX, vectorPlanetas[k].posY);
					double angulo = atan(pendiente);
					double fuerzaAtraccion = calcularFuerzaAtraccion(vectorAsteroides[j].masa, vectorPlanetas[k].masa, distancia);

					vectorAsteroides[j].comprobacionFuerzas();
					vectorAsteroides[j].actualizarFuerzas(fuerzaAtraccion * cos(angulo), fuerzaAtraccion * sin(angulo));
				}
			}


			//PENDIENTE: ESTO HAY QUE PONERLO EN CADA OBJETO
			//vectorAsteroides[j].comprobacionFuerzas();					//Creo que en esta linea esta bien, darle unas vueltillas
		}

		//Calculo aceleracion, velocidad y posicion
		for (int j = 0; j < num_asteroides; j++) {
			vectorAsteroides[j].actualizarPosicion(); 
		}

		//CHOQUES
		for (int j = 0; j < num_asteroides; j++) {
			//Choques de asteroides con los bordes del escenario
			if (vectorAsteroides[j].posX <= 0) {				//Limite izquierdo
				vectorAsteroides[j].posX = 5;
				vectorAsteroides[j].velozX = -vectorAsteroides[j].velozX;
				if (j == 12) {
					cout << "Holaaa1" << endl;
				}
			}
			else if (vectorAsteroides[j].posX >= width) {		//Limite derecho
				vectorAsteroides[j].posX = width - 5;
				vectorAsteroides[j].velozX = -vectorAsteroides[j].velozX;
				if (j == 12) {
					cout << "Holaaa2" << endl;
				}
			}

			if (vectorAsteroides[j].posY <= 0) {				//Limite inferior
				vectorAsteroides[j].posY = 5;
				vectorAsteroides[j].velozY = -vectorAsteroides[j].velozY;	
				if (j == 12) {
					cout << "Holaaa3" << endl;
				}		
			}
			else if (vectorAsteroides[j].posY >= height) {		//Limite superior
				vectorAsteroides[j].posY = height - 5;
				vectorAsteroides[j].velozY = -vectorAsteroides[j].velozY;
				if (j == 12) {
					cout << "Holaaa4" << endl;
				}
			}
			//Choques de asteroides con los asteroides
			for (int k = j+1; k < num_asteroides; k++) {
				double distancia = calcularDistancia(vectorAsteroides[j].posX, vectorAsteroides[j].posY, vectorAsteroides[k].posX, vectorAsteroides[k].posY);

				if (distancia < 5) {					//distancia != 0 && 
					if (j == 12) {
						cout << "Holaaa0" << endl;
					}
					double aux = vectorAsteroides[j].posX;
					vectorAsteroides[j].posX = vectorAsteroides[k].posX;
					vectorAsteroides[k].posX = aux;
					aux = vectorAsteroides[j].posY;
					vectorAsteroides[j].posY = vectorAsteroides[k].posY;
					vectorAsteroides[k].posY = aux;
				}
			}
		}
	}


	//CREAMOS EL FICHERO DE SALIDA Y ESCRIBIMOS LOS DATOS FINALES
	ofstream ficheroDatosFinales;
	ficheroDatosFinales.open("out.txt");
	for (int i = 0; i < num_asteroides; i++) {
		ficheroDatosFinales <<fixed <<setprecision(3)<<vectorAsteroides[i].posX<<" "<<fixed <<setprecision(3)<<vectorAsteroides[i].posY<<" "<<fixed <<setprecision(3)<<vectorAsteroides[i].velozX<<" "<<fixed <<setprecision(3)<<vectorAsteroides[i].velozY<<" "<<fixed <<setprecision(3)<<vectorAsteroides[i].masa<<" "<<endl;
	}

	ficheroDatosFinales.close();

	return 0;
}