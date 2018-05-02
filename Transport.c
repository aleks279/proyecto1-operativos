/*
* Instituto Tecnologico de Costa Rica
* Sistemas Operativos
* Semestre I 2018
*
* Autor: Luis Castillo
*
*/

#include "Transport.h"
#include <gtk/gtk.h>
#include <time.h>


Transport create ( int id, TRANSTYPES type, int width, int height, int originX, int originY )
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
    case CITY:
      pixbufImage = gdk_pixbuf_new_from_file("city.png", NULL);
      break;
  }

  result.image = pixbufImage;

  result.originX = originX;
  result.originY = originY;

  return result;
}

Transport moveX ( Transport transport, int speed )
{
  Transport newTransport = transport;
  newTransport.originX = transport.originX + speed;
  return newTransport;
}

Transport moveY ( Transport transport, int speed )
{
  Transport newTransport = transport;
  newTransport.originY = transport.originY + speed;
  return newTransport;
}


gboolean drawTransport( GtkWidget *da, cairo_t *cr, gpointer data )
{
  Transport *d = data;
  gint width = d->width;
  gint height = d->height;

  GdkPixbuf *temp=gdk_pixbuf_scale_simple(d->image, width, height, GDK_INTERP_BILINEAR);
  gdk_cairo_set_source_pixbuf(cr, temp, d->originX, d->originY);
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
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 900);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  GtkWidget *drawingArea=gtk_drawing_area_new();
  gtk_widget_set_hexpand(drawingArea, TRUE);
  gtk_widget_set_vexpand(drawingArea, TRUE);

  //Crea la posición city 
  Transport city = create (1, CITY, 600, 800, 0, 0);
  
  //Crea la posición inicial de un carro 
  Transport car = create (1, CAR, 50, 75, 275, 15);

  //Crea la posición inicial de una ambulance 
  Transport ambulance = create (2, AMBULANCE, 50, 75, 115, 125);

  //Crea la posición inicial de un barco 
  Transport ship = create (3, SHIP, 50, 75, 115, 370);

  //Llama a la función que dibuja el objeto
  g_signal_connect(drawingArea, "draw", G_CALLBACK(drawTransport), &city);
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