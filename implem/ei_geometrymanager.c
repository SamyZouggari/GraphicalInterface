#include "ei_geometrymanager.h"
#include "ei_application.h"
#include "ei_implementation.h"
#include "structure.h"


/**
 * \brief	An opaque type that represents the geometry management parameters of a widget.
 */
struct ei_impl_geom_param_t;
typedef struct ei_impl_geom_param_t*	ei_geom_param_t;

/**
 * \brief	Returns the size of \ref ei_impl_geom_param_t.
 */
size_t		ei_geom_param_size(){
    // retourne la taille de l'attribut geom_params commun a tous les widgets
    return sizeof(ei_impl_geom_param_t);
};

/**
 * \brief	A name of a geometry manager.
 */
typedef char	ei_geometrymanager_name_t[20];


/**
 * \brief	Must be called by runfuncs (see \ref ei_geometrymanager_runfunc_t).
 *
 * 		Checks if the geometry computation resulted in a change of geometry of
 * 		the widget (size and/or position). If there was a change:
 * 			* schedule a redraw of the screen on the old and new screen location
 * 			* notify the widget that it's geometry has changed
 * 			* recompute the geometry of the children
 *
 * @param	widget		The which geometry computation has been done.
 * @param	computed_screen_location The new screen location computed by the run of the
 * 				geometry manager. May be the same as the screen location of
 * 				the widget before the run.
 */
void	placer_run_finalize_t(ei_widget_t widget, ei_rect_t* new_screen_location) {
    if ((widget->screen_location).top_left.x == new_screen_location->top_left.x &&
        (widget->screen_location).top_left.y == new_screen_location->top_left.y &&
        (widget->screen_location).size.height == new_screen_location->size.height &&
        (widget->screen_location).size.width == new_screen_location->size.width) {
        return;
    } else {
        ei_widget_t siblings = widget->children_head;
        //ei_widget_t parent=widget->parent;
        //parent->wclass->geomnotifyfunc(parent);
        while (siblings != NULL) {
            widget->geom_params->manager->runfunc(siblings);
            siblings = siblings->next_sibling;
        }
    }
}

/**
 * \brief	A function that runs the geometry computation for this widget.
 * 		Must call \ref ei_geometry_run_finalize before returning.
 *
 * @param	widget		The widget instance for which to compute geometry.
 */

void	placer_runfunc_t	(ei_widget_t widget) {
    ei_widget_t parent = widget->parent;
    ei_geom_param_t geo_param = widget->geom_params;
    if(geo_param==NULL){
        return;
    }
    ei_anchor_t anchor = *geo_param->anchor;
    ei_rect_t* new_location=widget->content_rect==NULL?malloc(sizeof(ei_rect_t)):widget->content_rect;
    if (parent == NULL) {
        new_location->top_left.x = *geo_param->x;
        new_location->top_left.y = *geo_param->y;
        new_location->size.height = *geo_param->height;
        new_location->size.width = *geo_param->width;
    } else {
        int top_left_x_parent = parent->content_rect->top_left.x;
        int top_left_y_parent = parent->content_rect->top_left.y;
        ei_size_t size_parent = parent->content_rect->size;
        int top_left_x = top_left_x_parent + (*geo_param->x) + ((*geo_param->rel_x) * size_parent.width);
        int top_left_y = top_left_y_parent + (*geo_param->y) + ((*geo_param->rel_y) * size_parent.height);
        new_location->size.height =
                (int) (size_parent.height) * (*geo_param->rel_height) + *widget->geom_params->height;
        new_location->size.width =
                (int) (size_parent.width) * (*geo_param->rel_width) + *widget->geom_params->width;
        switch (anchor) {
            case ei_anc_none:
                break;
            case ei_anc_northwest:
                new_location->top_left.x = top_left_x;
                new_location->top_left.y = top_left_y;
                break;
            case ei_anc_north:
                new_location->top_left.x = top_left_x - (0.5) * new_location->size.width;
                new_location->top_left.y = top_left_y;
                break;
            case ei_anc_northeast:
                new_location->top_left.x = top_left_x - new_location->size.width;
                new_location->top_left.y = top_left_y;
                break;
            case ei_anc_west:
                new_location->top_left.x = top_left_x;
                new_location->top_left.y = top_left_y - (0.5) * new_location->size.height;
                break;
            case ei_anc_center:
                new_location->top_left.x = top_left_x - (0.5) * new_location->size.width;
                new_location->top_left.y = top_left_y - (0.5) * new_location->size.height;
                break;
            case ei_anc_east:
                new_location->top_left.x = top_left_x - new_location->size.width;
                new_location->top_left.y = top_left_y - (0.5) * new_location->size.height;
                break;
            case ei_anc_southwest:
                new_location->top_left.x = top_left_x;
                new_location->top_left.y = top_left_y - new_location->size.height;
                break;
            case ei_anc_south:
                new_location->top_left.x = top_left_x - (0.5) * new_location->size.width;
                new_location->top_left.y = top_left_y - new_location->size.height;
                break;
            case ei_anc_southeast:
                new_location->top_left.x = top_left_x - new_location->size.width;
                new_location->top_left.y = top_left_y - new_location->size.height;
                break;
        }
    }
    placer_run_finalize_t(widget, new_location);
        if (strcmp(widget->wclass->name, "frame") == 0) {

                struct ei_impl_frame_t *frame = widget;
                int border;
                if (frame->border_width != NULL){
                        border = *frame->border_width;
                }
                else{
                        border = 0;
                }
                widget->content_rect = new_location;
                widget->screen_location = *new_location;
                widget->content_rect->top_left.x += border;
                widget->content_rect->top_left.y += border;
                widget->screen_location.size.width += 2 * border;
                widget->screen_location.size.height += 2 * border;
        }
        else if (strcmp(widget->wclass->name, "toplevel") == 0)
        {
                struct ei_impl_toplevel_t* toplevel= (struct ei_impl_toplevel_t*) widget;
                int w;
                int h;
                hw_text_compute_size(toplevel->title,ei_default_font,&w,&h);
                if(new_location->size.width <160){
                        new_location->size.width=160;
                }
                if(new_location->size.height <120) {
                        new_location->size.height = 120;
                }
                widget->content_rect=new_location;
                widget->screen_location=*new_location;
                widget->content_rect->top_left.x+=*toplevel->border_width;
                widget->content_rect->top_left.y+=*toplevel->border_width + h;
                widget->screen_location.size.width+=2*(*toplevel->border_width);
                widget->screen_location.size.height+=h + 2*(*toplevel->border_width);
                placer_run_finalize_t(widget, widget->content_rect);

        }
        else{
                widget->screen_location=*new_location;
                widget->content_rect = new_location;
        }
    }


/**
 * \brief	A function called when a widget cease to be managed by its geometry manager.
 *		Most of the work is done in \ref ei_geometrymanager_unmap. This function hook is
 *		only provided to trigger recomputation when the disappearance of a widget has an
 *		effect on the geometry of other widgets.
 *
 * @param	widget		The widget instance that must be forgotten by the geometry manager.
 */
void	placer_releasefunc_t	(ei_widget_t widget){
    free(widget->geom_params->anchor);
    free(widget->geom_params->height);
    free(widget->geom_params->rel_height);
    free(widget->geom_params->rel_width);
    free(widget->geom_params->rel_x);
    free(widget->geom_params->rel_y);
    free(widget->geom_params->width);
    free(widget->geom_params->x);
    free(widget->geom_params->y);

}

/**
 * \brief	The structure that stores information about a geometry manager.
typedef struct ei_geometrymanager_t {
    ei_geometrymanager_name_t		name;
    ei_geometrymanager_runfunc_t		runfunc;
    ei_geometrymanager_releasefunc_t	releasefunc;
    struct ei_geometrymanager_t*		next;
} ei_geometrymanager_t;*/






/**
 * \brief	Registers a geometry manager to the program so that it can be called to manager
 *		widgets. This must be done only once in the application.
 *
 * @param	geometrymanager		The structure describing the geometry manager.
 */


void    ei_geometrymanager_register	(ei_geometrymanager_t* geometrymanager) {
    ei_geometrymanager_t *curr_manager = ei_app_root_widget()->geom_params->manager;
    if (curr_manager == NULL) {
        curr_manager = geometrymanager;
    }
    while (curr_manager->next != NULL) {
        if (strcmp(curr_manager->name, geometrymanager->name)==0) {
            return;
        }
        curr_manager = curr_manager->next;
    };
    if(strcmp(curr_manager->name,geometrymanager->name)==0){
        return;
    }
    curr_manager->next=geometrymanager;
}


/**
 * \brief	Returns a geometry manager structure from its name.
 *
 * @param	name		The name of the geometry manager.
 *
 * @return			The structure describing the geometry manager.
 */
ei_geometrymanager_t*	ei_geometrymanager_from_name	(ei_geometrymanager_name_t name){
    ei_widget_t cell=ei_app_root_widget();
    ei_geometrymanager_t* geo_manager=cell->geom_params->manager;
    while(geo_manager!=NULL){
        if(strcmp(geo_manager->name,name)==0){
            return geo_manager;
        }
        geo_manager=geo_manager->next;
    }
};



/**
 * \brief	Tell the geometry manager in charge of a widget to forget it. This removes the
 *		widget from the screen. If the widget is not currently managed, this function
 *		returns silently.
 *		Side effects:
 *		<ul>
 *			<li> the \ref ei_geometrymanager_releasefunc_t of the geometry manager in
 *				charge of this widget is called, </li>
 *			<li> the geom_param field of the widget is freed, </li>
 *			<li> the current screen_location of the widget is invalidated (which will
 *				trigger a redraw), </li>
 *			<li> the screen_location of the widget is reset to 0. </li>
 *		</ul>
 *
 * @param	widget		The widget to unmap from the screen.
 */
void	ei_geometrymanager_unmap	(ei_widget_t widget){
    ei_geometrymanager_t* manager=ei_widget_get_geom_manager(widget);
    manager->releasefunc(widget);
    free(widget->geom_params);
    (widget->screen_location).top_left.x=0;(widget->screen_location).top_left.y=0;
    (widget->screen_location).size.height=0;(widget->screen_location).size.width=0;
}


/**
 * @brief	Get the geometry manager for this widget.
 *
 * @param	widget		The widget.
 *
 * @return			A pointer to the geometry manager, or NULL if the widget is not currently displayed.
 */
ei_geometrymanager_t*	ei_widget_get_geom_manager	(ei_widget_t widget){
    return widget->geom_params->manager;
};

/**
 * @brief	Sets the geometry manager for this widget.
 *
 * @param	widget		The widget.
 * @param	manager		The geometry manager managing this widget.
 */
void			ei_widget_set_geom_manager	(ei_widget_t widget, ei_geometrymanager_t* manager){
    widget->geom_params->manager=manager;
};

/**
 * @brief	Get the geometry management parameters for this widget.
 *
 * @param	widget		The widget.
 *
 * @return			A pointer to the geometry management parameters, or NULL if the widget is not currently displayed.
 */
ei_geom_param_t		ei_widget_get_geom_params	(ei_widget_t widget){
    return widget->geom_params;
};

/**
 * @brief	Sets the geometry management parameters for this widget.
 *
 * @param	widget		The widget.
 * @param	geom_params	The geometry management parameters.
 */
void			ei_widget_set_geom_params	(ei_widget_t widget, ei_geom_param_t geom_param){
    widget->geom_params=geom_param;
};