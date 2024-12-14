#include "ei_placer.h"
#include "ei_implementation.h"
#include "structure.h"

/**
 * \brief	Configures the geometry of a widget using the "placer" geometry manager.
 *
 *		The placer computes a widget's geometry relative to its parent *content_rect*.
 *
 * 		If the widget was already managed by the "placer", then this calls simply updates
 *		the placer parameters: arguments that are not NULL replace previous values.
 *		Arguments that are NULL don't change the current value of the parameter.
 *
 * 		When the arguments are passed as NULL, the placer uses default values (detailed in
 *		the argument descriptions below). If no size is provided (either absolute or
 *		relative), then either the requested size of the widget is used if one was provided,
 *		or the default size is used.
 *
 * @param	widget		The widget to place.
 * @param	anchor		How to anchor the widget to the position defined by the placer
 *				(defaults to ei_anc_northwest).
 * @param	x		The absolute x position of the widget (defaults to 0).
 * @param	y		The absolute y position of the widget (defaults to 0).
 * @param	width		The absolute width for the widget (defaults to the requested width or
 * 				the default width of the widget if rel_width is NULL, or 0 otherwise).
 * @param	height		The absolute height for the widget (defaults to the requested height or
 *				the default height of the widget if rel_height is NULL, or 0 otherwise).
 * @param	rel_x		The relative x position of the widget: 0.0 corresponds to the left
 *				side of the parent, 1.0 to the right side (defaults to 0.0).
 * @param	rel_y		The relative y position of the widget: 0.0 corresponds to the top
 *				side of the parent, 1.0 to the bottom side (defaults to 0.0).
 * @param	rel_width	The relative width of the widget: 0.0 corresponds to a width of 0,
 *				1.0 to the width of the parent (defaults to 0.0).
 * @param	rel_height	The relative height of the widget: 0.0 corresponds to a height of 0,
 *				1.0 to the height of the parent (defaults to 0.0).
 */



void		ei_place	(ei_widget_t		widget,
                         ei_anchor_t*		anchor,
                         int*			x,
                         int*			y,
                         int*			width,
                         int*			height,
                         float*			rel_x,
                         float*			rel_y,
                         float*			rel_width,
                         float*			rel_height) {
    ei_geometrymanager_t *placer = ei_app_geom();
    ei_impl_geom_param_t *geom_params;
    ei_anchor_t *anc;
    int *x_p;
    int *y_p;
    int *width_p;
    int *height_p ;
    float *rx ;
    float *ry ;
    float *rw ;
    float *rh ;

    geom_params=(widget->geom_params==NULL)?calloc(1,sizeof(ei_impl_geom_param_t)) : widget->geom_params;

    geom_params->manager = placer;

    anc = geom_params->anchor==NULL?calloc(1,sizeof(ei_anchor_t)):geom_params->anchor;

    *anc = (anchor != NULL) ? *anchor : (ei_anchor_t) ei_anc_northwest;
    geom_params->anchor = anc;

    x_p=geom_params->x==NULL?calloc(1,sizeof(int)):geom_params->x;
    *x_p=(x!=NULL)? *x : 0;
    geom_params->x = x_p;

    y_p=geom_params->y==NULL?calloc(1,sizeof(int)):geom_params->y;
    *y_p=(y!=NULL)? *y : 0;
    geom_params->y = y_p;

    width_p=geom_params->width==NULL?calloc(1,sizeof(int)):geom_params->width;
    if (width != NULL) {
        *width_p = *width;
    } else {
        if (rel_width == NULL) {
            *width_p = widget->requested_size.width;
        } else {
            *width_p = 0;
        }
    }
    geom_params->width = width_p;

    height_p=geom_params->height==NULL?calloc(1,sizeof(int)):geom_params->height;
    if (height != NULL) {
        *height_p = *height;
    } else {
        if (rel_height == NULL) {
            *height_p = widget->requested_size.height;
        } else {
            *height_p = 0;
        }
    }
    geom_params->height = height_p;

    rx=geom_params->rel_x==NULL?calloc(1,sizeof(float)):geom_params->rel_x;
    *rx=(rel_x!=NULL)? *rel_x : 0;
    geom_params->rel_x = rx;

    ry=geom_params->rel_y==NULL?calloc(1,sizeof(float)):geom_params->rel_y;
    *ry=(rel_y!=NULL)? *rel_y : 0;
    geom_params->rel_y = ry;

    rw=geom_params->rel_width==NULL?calloc(1,sizeof(float)):geom_params->rel_width;
    *rw=(rel_width!=NULL)? *rel_width : 0;
    geom_params->rel_width = rw;

    rh=geom_params->rel_height==NULL?calloc(1,sizeof(float)):geom_params->rel_height;
    *rh=(rel_height!=NULL)? *rel_height : 0;
    geom_params->rel_height = rh;

    widget->geom_params = geom_params;
    widget->geom_params->manager->runfunc(widget);
}