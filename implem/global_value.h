//
// Created by zivkovil on 03/05/24.
//

#ifndef PROJETC_IG_GLOBAL_VALUE_H
#define PROJETC_IG_GLOBAL_VALUE_H
#include "ei_implementation.h"
#include "ei_widgetclass.h"
#include "structure.h"
#include "ei_geometrymanager.h"

ei_widget_t frame_allocfunc();

void frame_releasefunc(ei_widget_t widget);

void frame_drawfunc(ei_widget_t widget, ei_surface_t surface,
                    ei_surface_t pick_surface, ei_rect_t *clipper);

void frame_setdefaultsfunc(ei_widget_t widget);

void frame_geomnotifyfunc(ei_widget_t widget);

void	placer_runfunc_t	(ei_widget_t widget);

void	placer_releasefunc_t	(ei_widget_t widget);

void	placer_runfunc_t	(ei_widget_t widget);



void	placer_releasefunc_t	(ei_widget_t widget);


ei_widget_t button_allocfunc();

void button_releasefunc(ei_widget_t widget);

void button_drawfunc(ei_widget_t widget, ei_surface_t surface,
                     ei_surface_t pick_surface, ei_rect_t *clipper);

void button_setdefaultsfunc(ei_widget_t widget);

void button_geomnotifyfunc(ei_widget_t widget);

ei_widget_t toplevel_allocfunc();

void toplevel_releasefunc(ei_widget_t widget);

void toplevel_drawfunc(ei_widget_t widget, ei_surface_t surface,
                       ei_surface_t pick_surface, ei_rect_t *clipper);

void toplevel_setdefaultsfunc(ei_widget_t widget);

void toplevel_geomnotifyfunc(ei_widget_t widget);


bool continu=true;

ei_surface_t surface;

ei_surface_t offscreen;

struct linked_callback* eventtype_callback[12]={NULL};

struct ei_widgetclass_t* linked_class= NULL;


ei_geometrymanager_t placer_t={"placer",&placer_runfunc_t,&placer_releasefunc_t,NULL};

ei_geom_param_t param;

struct ei_impl_frame_t* root= {NULL};

uint32_t pick_id=0;


#endif //PROJETC_IG_GLOBAL_VALUE_H
