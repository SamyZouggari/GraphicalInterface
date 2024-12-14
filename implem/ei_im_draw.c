#include <ei_application.h>
#include "ei_draw.h"
#include "hw_interface.h"
#include "structure.h"
#include "ei_utils.h"
#include "ei_anchor.h"

#define max(a, b) ((a) >= (b) ? (a) : (b))
#define min(a, b) ((a) <= (b) ? (a) : (b))

void ei_draw_image_frame (ei_surface_t surface, struct ei_impl_frame_t frame, ei_rect_t rectangle){

        ei_rect_t rectangle_source;
        if (frame.img_rect != NULL)
        {
                rectangle_source = **frame.img_rect;
        }
        else
        {
                rectangle_source = hw_surface_get_rect(frame.img);
        }

        ei_rect_t rect_dest = {(ei_point_t){0,0}, (ei_size_t){min(rectangle.size.width, rectangle_source.size.width),
                                                              min(rectangle.size.height, rectangle_source.size.width)}};

        ei_point_t ou = ei_anchor_img_frame(frame, rect_dest, rectangle);

        rect_dest.top_left = ou;

        ei_copy_surface(surface, &rect_dest, frame.img, &rectangle_source, 1);
}

void ei_draw_image_button (ei_surface_t surface, struct ei_impl_button_t button, ei_rect_t rectangle){

        ei_rect_t rectangle_source;
        if (button.img_rect != NULL)
        {
                rectangle_source = *button.img_rect;
        }
        else
        {
                rectangle_source = hw_surface_get_rect(button.img);
        }

        ei_rect_t rect_dest = {(ei_point_t){0,0}, (ei_size_t){rectangle_source.size.width,
                                                              rectangle_source.size.width}};

        ei_point_t ou = ei_anchor_img_button(button, rect_dest, rectangle);

        rect_dest.top_left = ou;

        ei_copy_surface(surface, &rect_dest, button.img, &rectangle_source, 0);
}
