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



/*
* This enum contains the different types of transport that we are going to use in this project
*/
typedef enum {
	CAR,
	AMBULANCE,
	SHIP,
	CITY
} TRANSTYPES;

//Definition of the transport object
typedef struct {

	int id;
	TRANSTYPES type;
    
    GdkPixbuf *image;
    int width;
    int height;

    int originX;
    int originY;

    int destinationX;
    int destinationy;

} Transport;

//Functions of the transport object
Transport create (int id, TRANSTYPES type, int width, int height, int originX, int originY );
Transport moveX( Transport transport, int speed );
Transport moveY( Transport transport, int speed );
void wait( int time );




#endif /* TRANSPORT_H */