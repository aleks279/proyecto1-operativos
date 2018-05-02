/*
* Instituto Tecnologico de Costa Rica
* Sistemas Operativos
* Semestre I 2018
*
* Autor: Luis Castillo
*
*/

#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include<gtk/gtk.h>
#include "Place.h"



/*
* This enum contains the different types of transport that we are going to use in this project
*/
typedef enum {
	CAR,
	AMBULANCE,
	SHIP
} TRANSTYPES;

//Definition of the transport object
typedef struct {

	int id;
	TRANSTYPES type;
    
    GdkPixbuf *image;
    int width;
    int height;

    Place position;

} Transport;

//Functions of the transport object
Transport create (int id, TRANSTYPES type, int width, int height, Place position);
Transport turnRight();
Transport turnLeft();
void move();
void wait( int time );




#endif /* TRANSPORT_H */