#include "ei_widgetclass.h"
#include "ei_application.h"
#include "structure.h"
#include "ei_draw_widget.h"


bool callback_toplevel_redimension(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_point_t point_initial = (ei_point_t) {event->param.mouse.where.x, event->param.mouse.where.y};
    struct ei_impl_toplevel_t* toplevel =(struct ei_impl_toplevel_t*) widget->parent;
    ei_widget_t parent = toplevel->widget.parent;
    ei_size_t size=toplevel->widget.content_rect->size;
    if (widget!=ei_widget_pick(&event->param.mouse.where)) {
        return false;
    }
    ei_event_t* event_courant = calloc(1, sizeof(ei_event_t));
    while (event_courant->type != ei_ev_mouse_buttonup) {
        hw_event_wait_next(event_courant);
        ei_point_t point_courant = (ei_point_t) {event_courant->param.mouse.where.x,
                                                 event_courant->param.mouse.where.y};
        ei_point_t deplacement_relatif = (ei_point_t) {point_courant.x - point_initial.x,
                                                       point_courant.y - point_initial.y};
        if(*toplevel->resizable==ei_axis_x)
        {
            *toplevel->widget.geom_params->width=size.width+deplacement_relatif.x;
        }
        if(*toplevel->resizable==ei_axis_y)
        {
            *toplevel->widget.geom_params->height=size.height+deplacement_relatif.y;
        }
        if(*toplevel->resizable==ei_axis_both)
        {
            *toplevel->widget.geom_params->width=size.width+deplacement_relatif.x;
            *toplevel->widget.geom_params->height=size.height+deplacement_relatif.y;
        }
        toplevel->widget.geom_params->manager->runfunc((ei_widget_t)toplevel);
        redessine(parent);
    }
    free(event_courant);
    return true;
}

bool callback_toplevel_deplace(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    ei_widget_t parent=widget->parent;
    ei_point_t point_initial = (ei_point_t) {event->param.mouse.where.x, event->param.mouse.where.y};
    ei_size_t size_widget=widget->screen_location.size;
    widget->screen_location.size.height=size_widget.height - widget->content_rect->size.height;
    ei_point_t top_left=widget->screen_location.top_left;
    ei_point_t point_courant;
    ei_point_t deplacement_relatif;
    if(in_rect(widget,point_initial)==false){
        return false;
    }
    ei_event_t* event_courant = calloc(1, sizeof(ei_event_t));
    while (event_courant->type != ei_ev_mouse_buttonup) {
        hw_event_wait_next(event_courant);
        point_courant = (ei_point_t) {event_courant->param.mouse.where.x, event_courant->param.mouse.where.y};
        deplacement_relatif = (ei_point_t) {point_courant.x - point_initial.x,point_courant.y - point_initial.y};
        *widget->geom_params->x=top_left.x+deplacement_relatif.x;
        *widget->geom_params->y=top_left.y+deplacement_relatif.y;
        widget->geom_params->manager->runfunc(widget);
        redessine(widget->parent);
    }
    free(event_courant);
    return true;
}

bool callback_button_close_frame(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param)
{
    ei_widget_t parent=widget->parent;
    //    parent->wclass->geomnotifyfunc(parent);
    ei_widget_destroy(parent);
    return true;
}