/*
* Instituto Tecnologico de Costa Rica
* Sistemas Operativos
* Semestre I 2018
*
* Autor: Luis Castillo
*
*/
#ifndef PLACE_H
#define PLACE_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include<gtk/gtk.h>


typedef enum {
  HOSPITAL,
  CASA1,
  CASA2
} PLACENAMES;

typedef struct {

  int x;
  int y;

  PLACENAMES name;

} Place;

#endif /* PLACE_H */