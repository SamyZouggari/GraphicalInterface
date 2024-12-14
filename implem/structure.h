//
// Created by zivkovil on 03/05/24.
//

#ifndef PROJETC_IG_STRUCTURE_H
#define PROJETC_IG_STRUCTURE_H



#include "ei_widget_configure.h"
#include "ei_implementation.h"


struct ei_impl_frame_t {
    ei_impl_widget_t widget;
    const ei_color_t*	color;
    int*			border_width;
    ei_relief_t*		relief;
    ei_string_t*		text;
    ei_font_t*		text_font;
    ei_color_t*		text_color;
    ei_anchor_t*		text_anchor;
    ei_surface_t*		img;
    ei_rect_ptr_t*		img_rect;
    ei_anchor_t*		img_anchor;
};

struct ei_impl_button_t
{
    ei_impl_widget_t widget;
    const ei_color_t*	color;
    int*			border_width;
    int*			corner_radius;
    ei_relief_t*		relief;
    ei_string_t*		text;
    ei_font_t*		text_font;
    ei_color_t*		text_color;
    ei_anchor_t*		text_anchor;
    ei_surface_t*		img;
    ei_rect_t*		img_rect;
    ei_anchor_t*		img_anchor;
    ei_callback_t*		callback;
    ei_user_param_t	user_param;
};


struct ei_impl_toplevel_t
{
    ei_impl_widget_t widget;
    const ei_color_t*	color;
    int*			border_width;
    ei_string_t*		title;
    bool*			closable;
    ei_axis_set_t*		resizable;
    ei_size_ptr_t*		min_size;
};

struct linked_callback {
    ei_callback_t callback;
    ei_widget_t widget;
    void* user_param;
    ei_tag_t		tag;
    struct linked_callback* next;
};

ei_widget_t		ei_widget_create_prive		(ei_const_string_t	class_name,
                                                  ei_widget_t		parent,
                                                  ei_user_param_t	user_data,
                                                  ei_widget_destructor_t destructor);

ei_widgetclass_t** ei_app_class(void);

void ei_destroy_callbacks(ei_widget_t widget);

bool        callback_button    (ei_widget_t        widget,
                                struct ei_event_t*    event,
                                ei_user_param_t    user_param);

u_int32_t* ei_app_id(void);

struct linked_callback** ei_app_root_callback(void);

ei_geometrymanager_t* ei_app_geom(void);

ei_surface_t ei_app_offscreen(void);

u_int32_t* ei_app_color(void);

bool in_rect(ei_widget_t widget,ei_point_t point);

void redessine(ei_widget_t widget);

#endif //PROJETC_IG_STRUCTURE_H