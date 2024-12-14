#include "ei_application.h"
#include "ei_widgetclass.h"
#include "global_value.h"
#include "ei_types.h"
#include "ei_geometrymanager.h"
#include "ei_placer.h"
#include "ei_event.h"
#include "ei_implementation.h"


/**
 * \brief	Creates an application.
 *		<ul>
 *			<li> initializes the hardware (calls \ref hw_init), </li>
 *			<li> registers all classes of widget and all geometry managers, </li>
 *			<li> creates the root window (either in a system window, or the entire screen), </li>
 *			<li> creates the root widget to access the root window. </li>
 *		</ul>
 *
 * @param	main_window_size	If "fullscreen is false, the size of the root window of the
 *					application.
 *					If "fullscreen" is true, the current monitor resolution is
 *					used as the size of the root window. \ref hw_surface_get_size
 *					can be used with \ref ei_app_root_surface to get the size.
 * @param	fullScreen		If true, the root window is the entire screen. Otherwise, it
 *					is a system window.
 */
ei_widgetclass_t frame_t={"frame",&frame_allocfunc,&frame_releasefunc,&frame_drawfunc,&frame_setdefaultsfunc,&frame_geomnotifyfunc,NULL};
ei_widgetclass_t button_t={"button",&button_allocfunc,&button_releasefunc,&button_drawfunc,&button_setdefaultsfunc,&button_geomnotifyfunc,NULL};
ei_widgetclass_t toplevel_t={"toplevel",&toplevel_allocfunc,&toplevel_releasefunc,&toplevel_drawfunc,&toplevel_setdefaultsfunc,&toplevel_geomnotifyfunc,NULL};

void ei_app_create(ei_size_t main_window, bool fullscreen)
// création de l'application
{
    hw_init();
    // on crée la surface visible et l'offscreen
    surface = hw_create_window(main_window,fullscreen);
    offscreen = hw_surface_create(surface,main_window, hw_surface_has_alpha(surface));
    // on enregistre les classes qu'on a
    ei_widgetclass_register(&frame_t);
    ei_widgetclass_register(&button_t);
    ei_widgetclass_register(&toplevel_t);
    //on crée la root et on la configure
    root = (struct ei_impl_frame_t*)ei_widget_create_prive("frame",NULL,NULL,NULL);
    ei_place((ei_widget_t)root,NULL,NULL,NULL,&main_window.width,&main_window.height,NULL,NULL,NULL,NULL);
    ei_geometrymanager_register(root->widget.geom_params->manager);
    root->widget.requested_size = main_window;
    root->widget.screen_location = (ei_rect_t){{0, 0}, main_window};
    root->widget.content_rect = &(ei_rect_t){{0, 0}, main_window};
}

/**
 * \brief	Releases all the resources of the application, and releases the hardware
 *		(ie. calls \ref hw_quit).
 */
void ei_app_free(void)
// on detruit le widget de la racine, et on free les surfaces, ensuite on appelle hw_quit() pour bien quitter l'application
{
    ei_widget_destroy(&root->widget);
    hw_surface_free(surface);
    hw_surface_free(offscreen);
    hw_quit();
}


/**
 * \brief	Runs the application: enters the main event loop. Exits when
 *		\ref ei_app_quit_request is called.
 */
void ei_app_run() {
    // on initialise en dessinant les widgets du début de l'application
    ei_widget_t curr = ei_app_root_widget();
    ei_rect_t clipper = hw_surface_get_rect(surface);
    curr->wclass->drawfunc(curr, surface, offscreen, &clipper);
    hw_surface_update_rects(surface, &(const ei_linked_rect_t) {{{0,0}, root->widget.screen_location.size}, NULL});
    // boucle de traitement des évènements
    while(continu){
        ei_event_t* event = calloc(1,sizeof(ei_event_t));
        hw_event_wait_next(event);
        struct linked_callback* curr_callback = eventtype_callback[event->type];
        ei_widget_t widget=NULL;
        // on sépare les évènements qui requierent du picking des autres
        if(event->type>6) {
            widget = ei_widget_pick(&event->param.mouse.where);
        }
        bool treated = false;
        // parcours du gestionnaire d'évènements tant que l'évènement n'a pas été traité
        while (curr_callback != NULL && treated == false) {
            if(widget!=NULL) {
                //on regarde si le callback est bindé avec un tag ou pas
                if(curr_callback->widget == widget ) {
                    treated = curr_callback->callback(curr_callback->widget, event,
                                                      (void *) &curr_callback->user_param);
                }
                if(curr_callback->tag != NULL)
                {
                    if(strcmp("all",curr_callback->tag) == 0 || strcmp(widget->wclass->name,curr_callback->tag) == 0)
                    {
                        treated = curr_callback->callback(curr_callback->widget, event,
                                                          (void *) &curr_callback->user_param);
                    }
                }
            }
            else {
                treated = curr_callback->callback(curr_callback->widget, event,
                                                  (void *) &curr_callback->user_param);
            }
            // si l'évènement a été traité, on redessine l'application
            if (treated == true) {
                    ei_widget_t root_widget = ei_app_root_widget();
                    redessine(root_widget);
            }
            curr_callback = curr_callback->next;
        }
        free(event);
    }
}


/**
 * \brief	Adds a rectangle to the list of rectangles that must be updated on screen. The real
 *		update on the screen will be done at the right moment in the main loop.
 *
 * @param	rect		The rectangle to add, expressed in the root window coordinates.
 *				A copy is made, so it is safe to release the rectangle on return.
 */
void ei_app_invalidate_rect(const ei_rect_t* rect);

/**
 * \brief	Tells the application to quit. Is usually called by an event handler (for example
 *		when pressing the "Escape" key).
 */
void ei_app_quit_request(void) {
    continu = false;
}

/**
 * \brief	Returns the "root widget" of the application: a "frame" widget that span the entire
 *		root window.
 *
 * @return 			The root widget.
 */


ei_widget_t ei_app_root_widget(void)
{
    //renvoie le widget de la racine
    return (ei_widget_t)root;
}

/**
 * \brief	Returns the surface of the root window. Can be used to create surfaces with similar
 * 		r, g, b channels.
 *
 * @return 			The surface of the root window.
 */
ei_surface_t ei_app_root_surface(void)
{
    //renvoie la surface
    return surface;
}

struct linked_callback** ei_app_root_callback(void)
{
    // renvoie le gestionnaire d'applications
    return eventtype_callback;
}

ei_geometrymanager_t* ei_app_geom(void)
{
    // renvoie le placer
    return &placer_t;
}

ei_widgetclass_t** ei_app_class(void)
{
    //renvoie la liste chaînée des classes des widgets
    return &linked_class;
}

u_int32_t* ei_app_id(void) {
    //renvoie l'id du picking
    return &pick_id;
}

ei_surface_t ei_app_offscreen(void) {
    //renvoie l'offscreen
    return offscreen;
}


void ei_destroy_callbacks(ei_widget_t widget)
/* fonction appelée lorsqu'on détruit les widgets.
 Enlève les callbacks liés au widget que l'on veut détruire du gestionnaire d'évènement */
{
    for (uint32_t i = 0; i < 12; i++) {
        struct linked_callback* cour = eventtype_callback[i];
        struct linked_callback* prev = NULL;
        while (cour != NULL){
            struct linked_callback* suiv = cour->next;
            if(cour->widget == widget){
                if(prev == NULL){
                    eventtype_callback[i] = suiv;
                }
                else{
                    prev->next = suiv;
                }
                free(cour);
            }
            else{
                prev = cour;
            }
            cour = suiv;
        }
    }
}