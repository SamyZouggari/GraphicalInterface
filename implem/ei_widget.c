#include "ei_widget.h"

#include <ei_application.h>

#include "ei_implementation.h"
#include "structure.h"



struct ei_event_t;

ei_color_t* pick_id_pick_color(uint32_t* pick_id)
{
    /*
     * Prend un pick_id et renvoie sa couleur associe
     */
    ei_color_t* color=calloc(1,sizeof(ei_color_t));
    ei_surface_t offscreen=ei_app_offscreen();
    int ir,ib,ig,ia;
    hw_surface_get_channel_indices(offscreen,&ir,&ig,&ib,&ia);
    uint8_t* id=(uint8_t*)pick_id;
    uint8_t pick_color[4];
    pick_color[ia]=0xff;
    color->alpha=pick_color[ia];
    pick_color[ir]=*id;
    color->red=pick_color[ir];
    id++;
    pick_color[ig]=*id;
    color->green=pick_color[ig];
    id++;
    pick_color[ib]=*id;
    color->blue=pick_color[ib];
    return color;
}

/**
 * @brief	The type of functions that are called just before a widget is being destroyed
 * 		(the "widget" parameter and its fields are still valid).
 * 		Functions of this type are passed as a parameter to \ref ei_widget_create.
 *
 * @param	widget		The widget that is going to be destroyed.
 */
typedef void		(*ei_widget_destructor_t)	(ei_widget_t widget);





/**
 * @brief	A function that is called in response to a user event. For example, the function that
 *		a programmer wants to be called when the user has pressed on a graphical button.
 *
 * @param	widget		The widget for which the event was generated.
 * @param	event		The event containing all its parameters (type, etc.)
 * @param	user_param	The user parameters that was provided by the caller when registering
 *				this callback.
 *
 * @return			A boolean telling if the event was consumed by the callback or not.
 *				If TRUE, the library does not try to call other callbacks for this
 *				event. If FALSE, the library will call the next callback registered
 *				for this event, if any.
 *				Note: The callback may execute many operations and still return
 *				FALSE, or return TRUE without having done anything.
 */
typedef bool		(*ei_callback_t)		(ei_widget_t		widget,
                                             struct ei_event_t*	event,
                                             ei_user_param_t	user_param);



/**
 * @brief	Creates a new instance of a widget of some particular class, as a descendant of
 *		an existing widget.
 *
 *		The widget is not displayed on screen until it is managed by a geometry manager.
 *		When no more needed, the widget must be released by calling \ref ei_widget_destroy.
 *
 * @param	class_name	The name of the class of the widget that is to be created.
 * @param	parent 		A pointer to the parent widget. Can not be NULL.
 * @param	user_data	A pointer provided by the programmer for private use. May be NULL.
 * @param	destructor	A pointer to a function to call before destroying a widget structure. May be NULL.
 *
 * @return			The newly created widget, or NULL if there was an error.
 *
 *
 */

ei_widget_t		ei_widget_create_prive		(ei_const_string_t	class_name,
                                                  ei_widget_t		parent,
                                                  ei_user_param_t	user_data,
                                                  ei_widget_destructor_t destructor)
{
    /*
     * Pour la création d'un widget on cherche la classe de celui-ci. Puis on alloue l'espace nécéssaire pour sa création
     * Après cette allocation mémoire, on lui attribut son parent,user_data, destructor ainsi que son id et sa couleur.
     */
    ei_widgetclass_t* class= ei_widgetclass_from_name(class_name);
    if (class==NULL)
    {
        return NULL;
    }
    ei_widget_t widget=class->allocfunc();
    widget->wclass=class;
    widget->parent=parent;
    widget->user_data=user_data;
    widget->destructor=destructor;
    u_int32_t* pick_id=ei_app_id();
    widget->pick_id=*pick_id;
    ei_color_t* pick_color=pick_id_pick_color(pick_id);
    (*pick_id)++;
    widget->pick_color=pick_color;
    if(parent!=NULL) {
        if (parent->children_head == NULL) {
            parent->children_head = widget;
            parent->children_tail=widget;
            return widget;
        } else {
            parent->children_tail->next_sibling = widget;
            parent->children_tail=widget;
            return widget;
        }
    }
    else
    {
        return widget;
    }
}

ei_widget_t		ei_widget_create		(ei_const_string_t	class_name,
                                            ei_widget_t		parent,
                                            ei_user_param_t	user_data,
                                            ei_widget_destructor_t destructor)
{
    //verifie que le parent n'est pas NULL et renvoie le resultat de la fonction ei_widget_create_prive
    if(parent!=NULL)
    {
        return ei_widget_create_prive(class_name,parent,user_data,destructor);
    }
    exit(EXIT_FAILURE);
}




/**
 * @brief	Destroys a widget.
 * 		Removes the widget from the screen if it is currently displayed.
 * 		Destroys all its descendants.
 *		Calls its destructor if it was provided.
 * 		Frees the memory used by the widget (e.g. the widget param).
 *
 * @param	widget		The widget that is to be destroyed.
 */
void			ei_widget_destroy		(ei_widget_t		widget)
{
    if(widget==NULL){
        return;
    }
    //lance le destructor du widget si il en a un
    if(widget->destructor!=NULL){
        widget->destructor(widget);
    }
    /*
     * Retire le widget de la liste des enfants du parent puis détruit le widget ainsi que tout les enfants de ce widget
     * Puis pour chaque widget détruit on supprime ses callbacks associés.
     */
    ei_widget_t parent = widget->parent;
    if(parent==NULL)
    {

    }
    else {
        ei_widget_t parent = widget->parent;
        ei_widget_t curr = parent->children_head;
        ei_widget_t suiv = curr->next_sibling;
        if(curr==widget){
            parent->children_head=suiv;
            if(parent->children_tail==widget){
                parent->children_tail=NULL;
            }
        }
        else {
            while (curr->next_sibling != widget) {
                while (suiv != widget) {
                    curr = curr->next_sibling;
                    suiv = suiv->next_sibling;
                }
                curr->next_sibling = suiv->next_sibling;
            }
        }
        redessine(parent);
        ei_destroy_callbacks(widget);
        ei_widget_t cour=widget->children_head;
        ei_widget_t a_detruire;
        while(cour!=NULL){
            ei_widget_destroy(cour);
            a_detruire=cour;
            cour=cour->next_sibling;
            a_detruire->wclass->releasefunc(a_detruire);
            free(a_detruire);
        }

    }
}



/**
 * @brief	Returns if the widget is currently displayed (i.e. managed by a geometry manager).
 *
 * @param	widget		The widget.
 *
 * @return			true if the widget is displayed, false otherwise.
 */
bool	 		ei_widget_is_displayed		(ei_widget_t		widget)
{

}




uint32_t decode_color(uint32_t* localisation)
{
    /*
     * Décode avec la localisation l'id du pixel.
     */
    ei_surface_t offscreen=ei_app_offscreen();
    int ir,ib,ig,ia;
    hw_surface_get_channel_indices(offscreen,&ir,&ig,&ib,&ia);
    uint8_t* id=(uint8_t*)localisation;
    uint32_t pick_id=id[ir]|id[ib]<<8|+id[ig]<<16;
    return pick_id;
}

ei_widget_t recherche_widget(uint32_t pick_id,ei_widget_t widget)
{
    /*
     * Recherche le widget associé au pick_id
     */
    while(widget!=NULL) {
        if (widget->pick_id == pick_id) {
            return widget;
        }
        ei_widget_t res = recherche_widget(pick_id, widget->children_head);
        if (res != NULL) {
            return res;
        }
        widget=widget->next_sibling;
    }
    return NULL;
}

/**
 * @brief	Returns the widget that is at a given location on screen.
 *
 * @param	where		The location on screen, expressed in the root window coordinates.
 *
 * @return			The top-most widget at this location, or NULL if there is no widget
 *				at this location (except for the root widget).
 */

ei_widget_t		ei_widget_pick			(ei_point_t*		where)
{
    /*
     * Renvoie le widget au dessus duquel se situe le point where
     */
    ei_surface_t surface=ei_app_offscreen();
    hw_surface_lock(surface);
    uint32_t* localisation=(uint32_t*)hw_surface_get_buffer(surface);
    hw_surface_unlock(surface);
    ei_size_t size=hw_surface_get_size(surface);
    localisation=localisation+where->x+where->y*size.width;
    uint32_t pick_id=decode_color(localisation);
    ei_widget_t widget=ei_app_root_widget();
    return recherche_widget(pick_id,widget);
}
