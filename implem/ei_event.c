#include "ei_types.h"
#include "ei_widget.h"
#include "ei_event.h"
#include "ei_implementation.h"
#include "ei_application.h"
#include "structure.h"

/**
 * @brief	A bitfield indicating which of the modifier keys are currently pressed.
 * 		e.g., if the 4th bit is 1, it means that the "control" key on the left side of the keyboard
 * 		was pressed at the time of the event (\ref ei_mod_ctrl_left is the 4th identifier in the
 * 		enumerate \ref ei_modifier_key_t).
 */
typedef uint32_t			ei_modifier_mask_t;



/**
 * \brief	Binds a callback to an event type and a widget or a tag.
 *
 * @param	eventtype	The type of the event.
 * @param	widget		The callback is only called if the event is related to this widget.
 *				This parameter must be NULL if the "tag" parameter is not NULL.
 * @param	tag		The callback is only called if the event is related to a widget that
 *				has this tag. A tag can be a widget class name, or the tag "all".
 *				This parameter must be NULL is the "widget" parameter is not NULL.
 * @param	callback	The callback (i.e. the function to call).
 * @param	user_param	A user parameter that will be passed to the callback when it is called.
 */


/**
 * \brief	Unbinds a callback from an event type and widget or tag.
 *
 * @param	eventtype, widget, tag, callback, user_param
 *				All parameters must have the same value as when \ref ei_bind was
 *				called to create the binding.
 */

void insere_en_tete(ei_widget_t widget, ei_callback_t callback,ei_eventtype_t eventtype, void* user_param,ei_tag_t tag){
    /*
     * ajoute en tete de la liste chainée de callback le nouveau linked_callback avec ses arguments
     */
    struct linked_callback** eventtype_callback=ei_app_root_callback();
    struct linked_callback* cb=malloc(sizeof(struct linked_callback));
    cb->callback=callback;
    cb->widget=widget;
    cb->user_param=user_param;
    cb->tag=tag;
    cb->next=eventtype_callback[(int)eventtype];
    eventtype_callback[(int)eventtype]=cb;
}


void		ei_bind			(ei_eventtype_t		eventtype,
                                ei_widget_t		widget,
                                ei_tag_t		tag,
                                ei_callback_t		callback,
                                void*			user_param)
{
    /*
     * bind le callback à la liste des callbacks
     */
    if(widget!=NULL)
    {
        insere_en_tete(widget,callback,eventtype,user_param,tag);
        return;
    }
    if(tag!=NULL)
    {
        insere_en_tete(widget,callback,eventtype,user_param,tag);
    }
}

void		ei_unbind		(ei_eventtype_t		eventtype,
                              ei_widget_t		widget,
                              ei_tag_t		tag,
                              ei_callback_t		callback,
                              void*			user_param)
{
    /*
     * retire de la liste des callbacks le callback
     */
    struct linked_callback** eventtype_callback=ei_app_root_callback();
    struct linked_callback* curr=eventtype_callback[eventtype];
    struct linked_callback* origine=&(struct linked_callback){NULL,NULL,NULL,NULL,curr};
    struct linked_callback* prec;
    bool boucle =true;
    while(boucle)
    {
        boucle=false;
        curr=eventtype_callback[eventtype];
        prec=origine;
        while(curr!=NULL)
        {
            if(curr->widget==widget && curr->user_param==user_param && curr->callback==callback)
            {
                prec->next=curr->next;
                free(curr);
                eventtype_callback[eventtype]=origine->next;
                break;
            }
            if(curr->callback==callback && curr->user_param==user_param && strcmp("all",tag)==0)
            {
                prec->next=curr->next;
                boucle=true;
                free(curr);
                eventtype_callback[eventtype]=origine->next;
                break;
            }
            if(curr->callback==callback && curr->user_param==user_param && strcmp(tag,curr->widget->wclass->name)==0)
            {
                prec->next=curr->next;
                boucle=true;
                free(curr);
                eventtype_callback[eventtype]=origine->next;
                break;
            }
            prec=curr;
            curr=curr->next;
        }
    }
}


bool        callback_button    (ei_widget_t        widget,
                                struct ei_event_t*    event,
                                ei_user_param_t    user_param){
    /*
     * callback interne associé au bouton
     */
    ei_surface_t  surface =ei_app_root_surface();
    ei_surface_t offscreen=ei_app_offscreen();
    struct ei_impl_button_t *button = (struct ei_impl_button_t *) widget;
    ei_widget_t parent=widget->parent;
    if (event->param.mouse.button == ei_mouse_button_left ) {
        if (event->type== ei_ev_mouse_buttondown) {
            *button->relief = ei_relief_sunken;
            widget->wclass->drawfunc(widget, surface, offscreen, NULL);
            hw_surface_update_rects(surface,NULL);
            ei_event_t* event_t=calloc(1,sizeof(ei_event_t));
            while(true){
                hw_event_wait_next(event);
                if(event->type== ei_ev_mouse_buttonup ||  ei_widget_pick(&event->param.mouse.where)!=widget ){
                    *button->relief = ei_relief_raised;
                    widget->wclass->drawfunc(widget, surface, offscreen, NULL);
                    hw_surface_update_rects(surface,NULL);
                    if ((*button->callback)(widget, event, button->user_param) == true) {
                        parent->geom_params->manager->runfunc(parent);
                        if(parent->parent==NULL){
                            redessine(parent);
                        }
                        else {
                            redessine(parent->parent);
                        }
                        hw_surface_update_rects(surface,NULL);
                        free(event_t);
                        return true;
                    }
                }

            }
        }
    }
    return false;
}

bool in_rect(ei_widget_t widget,ei_point_t point)
{
    /*
     * regarde si le point est à l'intérieur du widget
     */

    ei_size_t size=widget->screen_location.size;
    ei_point_t topleft=widget->screen_location.top_left;
    if(topleft.x<=point.x && point.x<=topleft.x+size.width && topleft.y<=point.y && point.y <=topleft.y+size.height)
    {
        return true;
    }
    return false;
}
