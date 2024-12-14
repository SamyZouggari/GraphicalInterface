#include "ei_draw_widget.h"
#include "ei_dessin_relief.h"
#include "structure.h"

ei_point_t ei_anchor_text_button (struct ei_impl_button_t button, ei_rect_t rectangle){
        int height;
        int width;
        hw_text_compute_size(*button.text, *(button.text_font), &width, &height);

        ei_point_t ou;
        if (button.text_anchor == NULL || *button.text_anchor == ei_anc_center || *button.text_anchor == ei_anc_none){
                ou = (ei_point_t){rectangle.top_left.x + (rectangle.size.width - width) / 2,
                                  rectangle.top_left.y + (rectangle.size.height - height) / 2};
        }
        else if (*button.text_anchor == ei_anc_north){
                ou = (ei_point_t){rectangle.top_left.x + (rectangle.size.width - width) / 2,
                                  rectangle.top_left.y};
        }
        else if (*button.text_anchor == ei_anc_northwest){
                ou = (ei_point_t){rectangle.top_left.x, rectangle.top_left.y};
        }
        else if (*button.text_anchor == ei_anc_northeast){
                ou = (ei_point_t){rectangle.top_left.x + rectangle.size.width - width, rectangle.top_left.y};
        }
        else if (*button.text_anchor == ei_anc_south){
                ou = (ei_point_t){rectangle.top_left.x + (rectangle.size.width - width) / 2, rectangle.top_left.y + rectangle.size.height - height};
        }
        else if (*button.text_anchor == ei_anc_southwest){
                ou = (ei_point_t){rectangle.top_left.x, rectangle.top_left.y + rectangle.size.height - height};
        }
        else if (*button.text_anchor == ei_anc_southeast){
                ou = (ei_point_t){rectangle.top_left.x + rectangle.size.width - width, rectangle.top_left.y + rectangle.size.height - height};
        }
        else if (*button.text_anchor == ei_anc_west){
                ou = (ei_point_t){rectangle.top_left.x, rectangle.top_left.y + (rectangle.size.height - height) / 2};
        }
        else if (*button.text_anchor == ei_anc_east){
                ou = (ei_point_t){rectangle.top_left.x + rectangle.size.width - width, rectangle.top_left.y + (rectangle.size.height - height) / 2};
        }
        return ou;
}

ei_point_t ei_anchor_text_frame (struct ei_impl_frame_t frame, ei_rect_t rectangle){
        int height;
        int width;
        hw_text_compute_size(*frame.text, *(frame.text_font), &width, &height);

        ei_point_t ou;
        if (frame.text_anchor == NULL || *frame.text_anchor == ei_anc_center || *frame.text_anchor == ei_anc_none){
                ou = (ei_point_t){rectangle.top_left.x + (rectangle.size.width - width) / 2,
                                  rectangle.top_left.y + (rectangle.size.height - height) / 2};
        }
        else if (*frame.text_anchor == ei_anc_north){
                ou = (ei_point_t){rectangle.top_left.x + (rectangle.size.width - width) / 2,
                                  rectangle.top_left.y};
        }
        else if (*frame.text_anchor == ei_anc_northwest){
                ou = (ei_point_t){rectangle.top_left.x, rectangle.top_left.y};
        }
        else if (*frame.text_anchor == ei_anc_northeast){
                ou = (ei_point_t){rectangle.top_left.x + rectangle.size.width - width, rectangle.top_left.y};
        }
        else if (*frame.text_anchor == ei_anc_south){
                ou = (ei_point_t){rectangle.top_left.x + (rectangle.size.width - width) / 2, rectangle.top_left.y + rectangle.size.height - height};
        }
        else if (*frame.text_anchor == ei_anc_southwest){
                ou = (ei_point_t){rectangle.top_left.x, rectangle.top_left.y + rectangle.size.height - height};
        }
        else if (*frame.text_anchor == ei_anc_southeast){
                ou = (ei_point_t){rectangle.top_left.x + rectangle.size.width - width, rectangle.top_left.y + rectangle.size.height - height};
        }
        else if (*frame.text_anchor == ei_anc_west){
                ou = (ei_point_t){rectangle.top_left.x, rectangle.top_left.y + (rectangle.size.height - height) / 2};
        }
        else if (*frame.text_anchor == ei_anc_east){
                ou = (ei_point_t){rectangle.top_left.x + rectangle.size.width - width, rectangle.top_left.y + (rectangle.size.height - height) / 2};
        }
        return ou;
}

ei_point_t ei_anchor_img_frame (struct ei_impl_frame_t frame, ei_rect_t rect_dest, ei_rect_t rectangle){

        int width = rect_dest.size.width;
        int height = rect_dest.size.height;
        ei_point_t ou;
        if (frame.text_anchor == NULL || *frame.text_anchor == ei_anc_center || *frame.text_anchor == ei_anc_none){
                ou = (ei_point_t){rectangle.top_left.x + (rectangle.size.width - width) / 2,
                                  rectangle.top_left.y + (rectangle.size.height - height) / 2};
        }
        else if (*frame.text_anchor == ei_anc_north){
                ou = (ei_point_t){rectangle.top_left.x + (rectangle.size.width - width) / 2,
                                  rectangle.top_left.y};
        }
        else if (*frame.text_anchor == ei_anc_northwest){
                ou = (ei_point_t){rectangle.top_left.x, rectangle.top_left.y};
        }
        else if (*frame.text_anchor == ei_anc_northeast){
                ou = (ei_point_t){rectangle.top_left.x + rectangle.size.width - width, rectangle.top_left.y};
        }
        else if (*frame.text_anchor == ei_anc_south){
                ou = (ei_point_t){rectangle.top_left.x + (rectangle.size.width - width) / 2, rectangle.top_left.y + rectangle.size.height - height};
        }
        else if (*frame.text_anchor == ei_anc_southwest){
                ou = (ei_point_t){rectangle.top_left.x, rectangle.top_left.y + rectangle.size.height - height};
        }
        else if (*frame.text_anchor == ei_anc_southeast){
                ou = (ei_point_t){rectangle.top_left.x + rectangle.size.width - width, rectangle.top_left.y + rectangle.size.height - height};
        }
        else if (*frame.text_anchor == ei_anc_west){
                ou = (ei_point_t){rectangle.top_left.x, rectangle.top_left.y + (rectangle.size.height - height) / 2};
        }
        else if (*frame.text_anchor == ei_anc_east){
                ou = (ei_point_t){rectangle.top_left.x + rectangle.size.width - width, rectangle.top_left.y + (rectangle.size.height - height) / 2};
        }
        return ou;
}

ei_point_t ei_anchor_img_button (struct ei_impl_button_t button, ei_rect_t rect_dest, ei_rect_t rectangle){

        int width = rect_dest.size.width;
        int height = rect_dest.size.height;
        ei_point_t ou;
        if (button.text_anchor == NULL || *button.text_anchor == ei_anc_center || *button.text_anchor == ei_anc_none){
                ou = (ei_point_t){rectangle.top_left.x + (rectangle.size.width - width) / 2,
                                  rectangle.top_left.y + (rectangle.size.height - height) / 2};
        }
        else if (*button.text_anchor == ei_anc_north){
                ou = (ei_point_t){rectangle.top_left.x + (rectangle.size.width - width) / 2,
                                  rectangle.top_left.y};
        }
        else if (*button.text_anchor == ei_anc_northwest){
                ou = (ei_point_t){rectangle.top_left.x, rectangle.top_left.y};
        }
        else if (*button.text_anchor == ei_anc_northeast){
                ou = (ei_point_t){rectangle.top_left.x + rectangle.size.width - width, rectangle.top_left.y};
        }
        else if (*button.text_anchor == ei_anc_south){
                ou = (ei_point_t){rectangle.top_left.x + (rectangle.size.width - width) / 2, rectangle.top_left.y + rectangle.size.height - height};
        }
        else if (*button.text_anchor == ei_anc_southwest){
                ou = (ei_point_t){rectangle.top_left.x, rectangle.top_left.y + rectangle.size.height - height};
        }
        else if (*button.text_anchor == ei_anc_southeast){
                ou = (ei_point_t){rectangle.top_left.x + rectangle.size.width - width, rectangle.top_left.y + rectangle.size.height - height};
        }
        else if (*button.text_anchor == ei_anc_west){
                ou = (ei_point_t){rectangle.top_left.x, rectangle.top_left.y + (rectangle.size.height - height) / 2};
        }
        else if (*button.text_anchor == ei_anc_east){
                ou = (ei_point_t){rectangle.top_left.x + rectangle.size.width - width, rectangle.top_left.y + (rectangle.size.height - height) / 2};
        }
        return ou;
}