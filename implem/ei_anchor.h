#include "ei_draw_widget.h"
#include "ei_dessin_relief.h"
#include "structure.h"

#ifndef EI_ANCHOR_H
#define EI_ANCHOR_H

/**
 * \brief	Defines the point where to anchor a text of a button knowing the size of the rectangle in which the text is.
 *
 * @param	button		The button in which we can find the anchor.
 * @param	rectangle	The rectangle in which the text is.
 *
 * @return 			The coordinates of the point \ref ei_point_t corresponding to the anchor of the button.
 */
ei_point_t ei_anchor_text_button (struct ei_impl_button_t button, ei_rect_t rectangle);

/**
 * \brief	Defines the point where to anchor a text of a frame knowing the size of the rectangle in which the text is.
 *
 * @param	frame		The frame in which we can find the anchor.
 * @param	rectangle	The rectangle in which the text is.
 *
 * @return 			The coordinates of the point \ref ei_point_t corresponding to the anchor of the frame.
 */
ei_point_t ei_anchor_text_frame (struct ei_impl_frame_t frame, ei_rect_t rectangle);

/**
 * \brief	Defines the point where to anchor a image of a frame knowing the size of the rectangle in which the image is.
 *
 * @param	frame		The frame in which we can find the anchor.
 * @param rect_dest The rectangle in which the image will be.
 * @param	rectangle	The rectangle in which the image is.
 *
 * @return 			The coordinates of the point \ref ei_point_t corresponding to the anchor of the frame.
 */
ei_point_t ei_anchor_img_frame (struct ei_impl_frame_t frame, ei_rect_t rect_dest, ei_rect_t rectangle);

/**
 * \brief	Defines the point where to anchor a image of a button knowing the size of the rectangle in which the iamge is.
 *
 * @param	button		The button in which we can find the anchor.
 * @param rect_dest The rectangle in which the image will be.
 * @param	rectangle	The rectangle in which the image is.
 *
 * @return 			The coordinates of the point \ref ei_point_t corresponding to the anchor of the button.
 */
ei_point_t ei_anchor_img_button (struct ei_impl_button_t button, ei_rect_t rect_dest, ei_rect_t rectangle);

#endif
