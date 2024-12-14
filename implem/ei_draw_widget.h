#ifndef EI_DRAW_BUTTON_H
#define EI_DRAW_BUTTON_H

#include <stdint.h>
#include "ei_types.h"
#include "hw_interface.h"

/**
 * \brief	Draws a filled button.
 *
 * @param	surface 	Where to draw the button. The surface must be *locked* by
 *				\ref hw_surface_lock.
 * @param       widget          The widget to draw.
 * @param       pick_surface    The offscreen surface of the button.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle.
 */
void	ei_draw_button  	(ei_surface_t		surface,
                                   ei_widget_t             widget,
                                   ei_surface_t pick_surface,
                                   const ei_rect_t*	clipper);

/**
 * \brief	Draws a filled frame.
 *
 * @param	surface 	Where to draw the button. The surface must be *locked* by
 *				\ref hw_surface_lock.
 * @param       widget          The widget to draw.
 * @param       pick_surface    The offscreen surface of the frame.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle.
 */
void	ei_draw_frame  	(ei_surface_t		surface,
                                   ei_widget_t             widget,
                                   ei_surface_t pick_surface,
                                   const ei_rect_t*	clipper);
/**
 * \brief	Draws a filled toplevel heading.
 *
 * @param	surface 	Where to draw the toplevel heading. The surface must be *locked* by
 *				\ref hw_surface_lock.
 * @param       widget          The widget to draw.
 * @param       pick_surface    The offscreen surface of the toplevel heading.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle.
 */
void ei_draw_en_tete (ei_surface_t surface, ei_widget_t widget, ei_surface_t pick_surface, ei_rect_t* clipper);

/**
 * \brief	Draws a filled toplevel.
 *
 * @param	surface 	Where to draw the toplevel. The surface must be *locked* by
 *				             \ref hw_surface_lock.
 * @param       widget          The widget to draw.
 * @param       pick_surface    The offscreen surface of the toplevel.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle.
 */
void ei_draw_toplevel (ei_surface_t surface, ei_widget_t widget, ei_surface_t pick_surface, ei_rect_t* clipper);

#endif