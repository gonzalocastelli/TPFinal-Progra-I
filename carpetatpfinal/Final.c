
/* 
 * File:   Final.c
 * Author: augustovb
 *
 * Created on June 25, 2014, 12:48 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>              //son todas las librerias que se utilizaran
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <string.h>
#include "Final.h"
/*
 *Main
 */
int main(int argc, char** argv) {
    int estado=0;
    int* pestado=&estado;
    int estadomenu=POS1;
    int* pestadomenu=&estadomenu;
    int estadoconfiguracion=USUARIO;
    int* pestadoconfiguracion=&estadoconfiguracion;
    int condiciones=0;      //por lo que me devuelva la funcion setear comienzo
    int estadomensaje=0;
    int* pestadomensaje=&estadomensaje;
    
    if(inicializar()){
        return -1;
    }
    
    ALLEGRO_DISPLAY* display=NULL;
    ALLEGRO_EVENT_QUEUE* eventos=NULL;      //Son basicamente los punteros a estructuras propias de Allegro para la inicializacion de lo 
    ALLEGRO_TIMER*  timer=NULL;             //basico y necesario para hacer funcionar las funciones de esta libreria
    
    display=getdisplay(PANTALLA_ANCHO,PANTALLA_ALTO);
    eventos=geteventqueue();                              //llamo las funciones que crearan los displays y eventos
    timer=gettimer(1.0/FPS);
    if(display==ERROR||eventos==ERROR||timer==ERROR){
        return -1;                                      //si fallo alguna de las funciones llamadas, retorno del main, mostrando que hubo un error
    }
    
    al_register_event_source(eventos, al_get_display_event_source(display));

    al_register_event_source(eventos, al_get_timer_event_source(timer));     //registro los eventos
 
    al_register_event_source(eventos, al_get_mouse_event_source()); 
    
    al_register_event_source(eventos,al_get_keyboard_event_source());
    
    ALLEGRO_BITMAP* imagencelular=NULL;
    
    imagencelular=al_load_bitmap("foto_celular.png");
    if(imagencelular==NULL){fprintf(stderr,"No se pudo cargar la imagen\n");return -1;}     //chequeo que no haya habido errores
    
    al_draw_bitmap(imagencelular,-40,-20,0);
    al_flip_display();
    
    ALLEGRO_BITMAP* pantalla=NULL;      //a partir de ahora, solo trabajaremos graficamente en la pantalla
    pantalla=al_create_bitmap(220,288); //tiene el tamaño de la pantalla
    
    cambiopantalla(display,pantalla);
    al_clear_to_color(al_map_rgb(255,255,255));             //siempre que quiera modificar al bitmap pantalla para despues "pegarlo"
    cambiopantalla(display,pantalla);
    
    /*
     *  La secuencia que realizamos aqui... al_set_target_bitmap(pantalla);
     *                                      
     *                                      CAMBIOS ASOCIADOS A LA PANTALLA
     * 
     *                                      al_set_target_bitmap(al_get_backbuffer(display));
     *                                      al_draw_bitmap(pantalla,48,118,0);
     *                                      al_flip_display();
     * es la que esta comprimida en estas dos funciones
     * 
     */

    
    
    al_start_timer(timer);   //inicializo el timer
    
    INFOPANTALLA mipantalla=getuserdata();
    INFOPANTALLA* pmipantalla=&mipantalla;
    iniciarpantalla(mipantalla,display,pantalla);
    
    while(estado!=FINALIZAR){
        if(estado==PANTALLAPRINCIPAL){
            mantenerpantalla(mipantalla,display,pantalla);
            analizartecladopantalla(eventos,pestado,display,pantalla);
            
        } 
        if(estado==MENU){
            mantenermenu(pestado,pestadomenu,display,pantalla,mipantalla);
            analizartecladomenu(eventos,pestado,pestadomenu);
        }
        if(estado==MENSAJE){
            escribirmensaje(eventos,display,pantalla,pestado,pestadomensaje,pmipantalla);
        
        }
        if(estado==PERSONALIZAR){
            mantenerconfiguracion(pestadoconfiguracion,display,pantalla,eventos,pestado,pmipantalla);
            analizartecladoconfiguracion(eventos,pestadoconfiguracion,pestado,pmipantalla);        
        }
        
        
        
    }
    
    putuserdata(mipantalla);
    *pestado=PANTALLAPRINCIPAL;
    return (EXIT_SUCCESS);
}

#define ENVIANDOMENSAJE     0
#define ESCRIBIENDOMENSAJE  1




void escribirmensaje(ALLEGRO_EVENT_QUEUE* eventos,ALLEGRO_DISPLAY* display,ALLEGRO_BITMAP* pantalla,int*pestado,int* pestadomensaje,INFOPANTALLA* mipantalla){
    ALLEGRO_FONT* fuente=NULL;
    ALLEGRO_BITMAP* fondo=NULL;
    fondo=al_create_bitmap(250,300); al_set_target_bitmap(fondo); al_clear_to_color(al_map_rgb(255,255,255));
    
    fuente=crearfuente(PEQUENA); if(fuente==NULL){fprintf(stderr,"Fallo al cargar la fuente en escribirmensaje()\n"); exit -1;}
    cambiopantalla(display,pantalla);
    al_draw_bitmap(fondo,0,15,0);
    al_destroy_bitmap(fondo);
    al_draw_text(fuente,al_map_rgb(0,0,0),0,15,ALLEGRO_ALIGN_LEFT,"Escriba su SMS:");
    fuente=crearfuente(MEDIA);
    al_draw_text(fuente,al_map_rgb(0,0,0),0,270,ALLEGRO_ALIGN_LEFT,"Enviar");
    cambiopantalla(display,pantalla);
    
    if(mipantalla->tipoescritura==MODOLIBRE){
            tecladoCLASSIC3(eventos,pantalla,display,mipantalla,pestado);
    
    } else if(mipantalla->tipoescritura==TECLADOANALOGO){
          
        cambiopantalla(display,pantalla);
        al_draw_text(fuente,al_map_rgb(255,0,10),0,100,ALLEGRO_ALIGN_LEFT,"SU MENSAJE ESTA");
        al_draw_text(fuente,al_map_rgb(255,0,10),0,125,ALLEGRO_ALIGN_LEFT,"SIENDO INTRODUCI-");
        al_draw_text(fuente,al_map_rgb(255,0,10),0,150,ALLEGRO_ALIGN_LEFT,"DO MEDIANTE EL TE-");
        al_draw_text(fuente,al_map_rgb(255,0,10),0,175,ALLEGRO_ALIGN_LEFT,"CLADO ANALOGO. (se");
        al_draw_text(fuente,al_map_rgb(255,0,10),0,200,ALLEGRO_ALIGN_LEFT,"actualizara solo)");
        cambiopantalla(display,pantalla);
            
        printf("\nINGRESE SU MENSAJE AQUI Y SERA CARGADO EN EL CELULAR\n");
        printf("\nRECUERDE QUE ESTA INGRESANDO EL MENSAJE PERSONAL\n");
        char personalmessage[140];
        unsigned int i=0;
        while(i<140){
            personalmessage[i]=getchar();
            
            if(personalmessage[i]=='\n'){
                 personalmessage[i]=0;
                break;
             }
            ++i;
            
        }
        strcpy(mipantalla->mensajepersonal,personalmessage);
        cambiopantalla(display,pantalla);
        al_draw_bitmap(fondo,0,15,0);
        cambiopantalla(display,pantalla);
        
            
        } else if(mipantalla->tipoescritura==T9){
            tecladoT9(eventos,pantalla,display);
        }
    *pestado=PANTALLAPRINCIPAL;
    
   
    al_destroy_font(fuente);
    
}




