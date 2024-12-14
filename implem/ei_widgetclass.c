#include "ei_widgetclass.h"
#include "ei_callback.h"
#include "ei_widget_configure.h"
#include "structure.h"
#include "ei_draw.h"
#include "ei_draw_widget.h"

/* allocfunc : on alloue de l'espace mémoire pour les widgets. On alloue d'abord de l'espace mémoire correspondant à la
classe correspondante du widget, et on renvoie un pointeur vers l'attribut widget de la classe du widget, qui est le
 premier attribut de la classe */

ei_widget_t  frame_allocfunc (){
    struct ei_impl_frame_t* frame=calloc(1,sizeof(struct ei_impl_frame_t));
    return (ei_widget_t)frame;
}

ei_widget_t button_allocfunc()
{
    struct ei_impl_button_t* button=calloc(1,sizeof(struct ei_impl_button_t));
    return (ei_widget_t)button;
}

ei_widget_t toplevel_allocfunc()
{
    struct ei_impl_toplevel_t* toplevel=calloc(1,sizeof(struct ei_impl_toplevel_t));
    return (ei_widget_t)toplevel;

}

/* releasefunc : on free tous les attributs de la classe, à part le widget*/
void button_releasefunc(ei_widget_t widget)
{
    // on récupère la structure de classe du widget en effectuant un cast
    struct ei_impl_button_t* button=(struct ei_impl_button_t*)widget;
    free((ei_color_t*)button->color);
    free(button->border_width);
    free(button->corner_radius);
    free(button->relief);
    free(button->text);
    free(button->text_font);
    free(button->text_color);
    free(button->text_anchor);
    free(button->img);
    free(button->img_rect);
    free(button->img_anchor);
    free(button->callback);
    free(widget->geom_params);
    free(widget->pick_color);
}

void toplevel_releasefunc(ei_widget_t widget)
{
    // on récupère la structure de classe du widget en effectuant un cast
    struct ei_impl_toplevel_t* toplevel=(struct ei_impl_toplevel_t*)widget;
    free((ei_color_t*)toplevel->color);
    free(toplevel->border_width);
    free(toplevel->title);
    free(toplevel->closable);
    free(toplevel->resizable);
    free(toplevel->min_size);
    widget->geom_params->manager->releasefunc(widget);
    free(widget->geom_params);
    free(widget->pick_color);
}


void frame_releasefunc(ei_widget_t widget)
{
    // on récupère la structure de classe du widget en effectuant un cast
    struct ei_impl_frame_t* frame=(struct ei_impl_frame_t*)widget;
    free((ei_color_t*)frame->color);
    free(frame->border_width);
    free(frame->relief);
    free(frame->text);
    free(frame->text_font);
    free(frame->text_color);
    free(frame->text_anchor);
    free(frame->img);
    free(frame->img_rect);
    free(frame->img_anchor);
    widget->geom_params->manager->releasefunc(widget);
    free(widget->geom_params);
    free(widget->pick_color);
}

/**
 * \brief	A function that draws a widget of a class.
 * 		The function must also draw the children of the widget.
 *
 * @param	widget		A pointer to the widget instance to draw.
 * @param	surface		A locked surface where to draw the widget. The actual location of the widget in the
 *				surface is stored in its "screen_location" field.
 * @param	pick_surface	The picking offscreen.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference frame).
 */

void		ei_impl_widget_draw_children	(ei_widget_t		widget,
                                             ei_surface_t		surface,
                                             ei_surface_t		pick_surface,
                                             ei_rect_t*		clipper)
/*Fonction appelée par les différentes fonction de dessins des widgets pour dessiner ses enfants*/
{
    if(widget==NULL)
    {
        return;
    }
    widget->wclass->drawfunc(widget,surface,pick_surface,clipper);
    ei_widget_t siblings = widget->children_head;
    while(siblings!=NULL)
    {
        ei_impl_widget_draw_children(siblings,surface,pick_surface,clipper);
        siblings = siblings->next_sibling;
    }
}

/*drawfunc : fonctions appelées pour dessiner les widgets*/
void frame_drawfunc(ei_widget_t widget, ei_surface_t surface,
                    ei_surface_t pick_surface, ei_rect_t *clipper)
{
    ei_draw_frame(surface, widget, pick_surface, clipper);
}

void button_drawfunc(ei_widget_t widget, ei_surface_t surface,
                     ei_surface_t pick_surface, ei_rect_t *clipper)

{
    ei_draw_button(surface, widget, pick_surface,clipper);
}

void toplevel_drawfunc(ei_widget_t widget, ei_surface_t surface,
                       ei_surface_t pick_surface, ei_rect_t *clipper)
{

    ei_draw_toplevel(surface, widget, pick_surface, clipper);
}

/*setdefaultsfunc: fonctions appelées pour créer un widget par défaut*/
void button_setdefaultsfunc(ei_widget_t widget)
{
    ei_button_configure(widget,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
}

void toplevel_setdefaultsfunc(ei_widget_t widget)
{
    ei_toplevel_configure(widget,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
}

void frame_setdefaultsfunc(ei_widget_t widget) {
    ei_frame_configure(widget,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
}

void button_geomnotifyfunc(ei_widget_t widget)
{

}

void toplevel_geomnotifyfunc(ei_widget_t widget)
{

}

void frame_geomnotifyfunc(ei_widget_t widget){

}

void redessine(ei_widget_t widget)
{
    ei_surface_t surface = ei_app_root_surface();
    ei_surface_t offscreen = ei_app_offscreen();
    widget->wclass->drawfunc(widget, surface,offscreen, NULL);
    hw_surface_update_rects(surface, &(const ei_linked_rect_t) {{{0,0}, widget->screen_location.size}, NULL});
}

void ei_widgetclass_register(ei_widgetclass_t* widgetclass)
/* appelée pour enregistrer une classe*/
{
    ei_widgetclass_t** current_widget_class = ei_app_class();
    ei_widgetclass_t* curr = *current_widget_class;
    if (*current_widget_class == NULL)
    {
        *current_widget_class = widgetclass;
        return;
    }
    while(curr->next != NULL)
    {
        // on vérifie si la classe n'est pas déjà enregistrée
        if(strcmp(curr->next->name, widgetclass->name) == 0)
        {
            return;
        }
        curr = curr->next;
    }
    if(strcmp(curr->name, widgetclass->name) == 0) {
        return;
    }
    // ajout en queue de la classe qu'on veut enregistrer
    curr->next = widgetclass;
}


ei_widgetclass_t*	ei_widgetclass_from_name	(ei_const_string_t name)
// on parcours la liste chaînée des classes de widgets jusqu'à trouver celle passée en paramètre
{
    ei_widgetclass_t* class = *ei_app_class();
    while(class != NULL)
    {
        if(strcmp(class->name,name) == 0)
        {
            return class;
        }
        class = class->next;
    }
    return NULL;
}
