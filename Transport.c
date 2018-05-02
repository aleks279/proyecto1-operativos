/*
* Instituto Tecnologico de Costa Rica
* Sistemas Operativos
* Semestre I 2018
*
* Autor: Luis Castillo
*
*/

#include "Transport.h"
#include "Place.h"
#include <gtk/gtk.h>


Transport create ( int id, TRANSTYPES type, int width, int height, Place position )
{
  GdkPixbuf *pixbufImage;

  Transport result;
  result.width = width;
  result.height = height;
  result.type = type;

  switch ( type )
  {
    case CAR:
      pixbufImage = gdk_pixbuf_new_from_file("car.png", NULL);
      break;
    case AMBULANCE:
      pixbufImage = gdk_pixbuf_new_from_file("ambulance.png", NULL);
      break;
    case SHIP:
      pixbufImage = gdk_pixbuf_new_from_file("ship.png", NULL);
      break;
  }

  result.image = pixbufImage;

  result.position = position;

  return result;
}

gboolean drawTransport(GtkWidget *da, cairo_t *cr, gpointer data)
{
  Transport *d = data;
  gint width = d->width;
  gint height = d->height;
  Place position = d->position;

  GdkPixbuf *temp=gdk_pixbuf_scale_simple(d->image, width, height, GDK_INTERP_BILINEAR);
  gdk_cairo_set_source_pixbuf(cr, temp, position.x, position.y);
  cairo_paint(cr);

  g_object_unref(temp);
  return FALSE;
}


int main(int argc, char *argv[])
{

  //Inicializa la ventana
  gtk_init(&argc, &argv);
  GtkWidget *window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Resize Picture");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  GtkWidget *drawingArea=gtk_drawing_area_new();
  gtk_widget_set_hexpand(drawingArea, TRUE);
  gtk_widget_set_vexpand(drawingArea, TRUE);
  
  //Crea la posición inicial de un carro 
  Place initialPositionCar;
  initialPositionCar.x = 200;
  initialPositionCar.y = 150; 
  Transport car = create (1, CAR, 50, 75, initialPositionCar);

  //Crea la posición inicial de una ambulance 
  Place initialPositionAmbulance;
  initialPositionAmbulance.x = 0;
  initialPositionAmbulance.y = 0; 
  Transport ambulance = create (2, AMBULANCE, 50, 100, initialPositionAmbulance);

  //Crea la posición inicial de un barco 
  Place initialPositionShip;
  initialPositionShip.x = 525;
  initialPositionShip.y = 0; 
  Transport ship = create (2, SHIP, 50, 75, initialPositionShip);

  //Llama a la función que dibuja el objeto
  g_signal_connect(drawingArea, "draw", G_CALLBACK(drawTransport), &car);
  g_signal_connect(drawingArea, "draw", G_CALLBACK(drawTransport), &ambulance);
  g_signal_connect(drawingArea, "draw", G_CALLBACK(drawTransport), &ship);

  GtkWidget *grid=gtk_grid_new();
  gtk_grid_attach(GTK_GRID(grid), drawingArea, 0, 0, 1, 1);
  gtk_container_add(GTK_CONTAINER(window), grid);
  gtk_widget_show_all(window);

  gtk_main();


  return 0;
}