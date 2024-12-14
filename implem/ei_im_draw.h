#include "structure.h"

#ifndef EI_PICT_DRAW_H
#define EI_PICT_DRAW_H

/**
 * \brief	Draws an image within a frame.
 *
 * @param surface The surface where draw.
 * @param	frame		The frame in which the image will be.
 * @param	rectangle	The rectangle defining the frame.
 */

void ei_draw_image_frame (ei_surface_t surface, struct ei_impl_frame_t frame, ei_rect_t rectangle);

/**
 * \brief	Draws an image within a button.
 *
 * @param surface The surface where to draw.
 * @param	button		The button in which the image will be.
 * @param	rectangle	The rectangle defining the button.
 */

void ei_draw_image_button (ei_surface_t surface, struct ei_impl_button_t button, ei_rect_t rectangle);

#endif
