#include "ei_draw.h"

#include <ei_utils.h>

#include "ei_dessin_relief.h"
#include "ei_draw_widget.h"

#define min(a, b) ((a) <= (b) ? (a) : (b))
#define max(a, b) ((a) >= (b) ? (a) : (b))


void	ei_draw_text		(ei_surface_t		surface,
                             const ei_point_t*	where,
                             ei_const_string_t	text,
                             ei_font_t		font,
                             ei_color_t		color,
                             const ei_rect_t*	clipper){

    hw_surface_lock(surface);
    if(text==NULL){
        hw_surface_unlock(surface);
        return;
    }
    if (font == NULL){
        font = ei_default_font;
    }

    ei_surface_t surface_text = hw_text_create_surface(text, ei_default_font, color);
    ei_rect_t rectangle_text = hw_surface_get_rect(surface_text);

    int height;
    int width;
    hw_text_compute_size(text, font, &width, &height);

    hw_surface_lock(surface_text);

    if (clipper != NULL){

        ei_point_t point = (ei_point_t){max(where->x, clipper->top_left.x), max(where->y, clipper->top_left.y)};
        ei_rect_t rect_dest = {point, (ei_size_t){min(width, clipper->size.width), min(height, clipper->size.width)}};
        rectangle_text.size = rect_dest.size;
        rectangle_text.top_left = (ei_point_t){0, 0};
        ei_copy_surface(surface, &rect_dest, surface_text, &rectangle_text, 1);
    }
    else {
        ei_rect_t rect_dest = {*where, (ei_size_t){width, height}};

        ei_copy_surface(surface, &rect_dest, surface_text, &rectangle_text, 1);

    }
    hw_surface_unlock(surface_text);
    hw_surface_free(surface_text);
    hw_surface_unlock(surface);
}

/**
 * \brief	Fills the surface with the specified color.
 *
 * @param	surface		The surface to be filled. The surface must be *locked* by
 *				\ref hw_surface_lock.
 * @param	color		The color used to fill the surface. If NULL, it means that the
 *				caller want it painted black (opaque).
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle.
 */
void	ei_fill			(ei_surface_t		surface,
                            const ei_color_t*	color,
                            const ei_rect_t*	clipper){
    hw_surface_lock(surface);

    ei_rect_t rectangle_surface = hw_surface_get_rect(surface);
    ei_rect_t rectangle;
    if (clipper != NULL){
        ei_point_t point = (ei_point_t){max(clipper->top_left.x, rectangle_surface.top_left.x), max(clipper->top_left.y, rectangle_surface.top_left.y)};
        ei_size_t taille = (ei_size_t){min(clipper->size.width, rectangle_surface.size.width), min(clipper->size.height, rectangle_surface.size.height)};
        rectangle = (ei_rect_t){point, taille};
    }
    else{
        rectangle = rectangle_surface;
    }

    ei_color_t couleur;
    if (color != NULL){
        couleur = *color;
    }
    else{
        couleur = (ei_color_t){0, 0, 0, 255};
    }

    ei_point_t point_hg = {rectangle.top_left.x, rectangle.top_left.y};
    ei_point_t point_bg = {rectangle.top_left.x, rectangle.top_left.y + rectangle.size.height};
    ei_point_t point_bd = {rectangle.top_left.x + rectangle.size.width, rectangle.top_left.y + rectangle.size.height};
    ei_point_t point_hd = {rectangle.top_left.x + rectangle.size.width, rectangle.top_left.y};
    ei_point_t tableau[4] = {point_hg, point_bg, point_bd, point_hd};

    ei_draw_polygon(surface, tableau, 4, couleur, clipper);

    hw_surface_unlock(surface);
}

/**
 * \brief	Copies pixels from a source surface to a destination surface.
 *		The source and destination areas of the copy (either the entire surfaces, or
 *		subparts) must have the same size before considering clipping.
 *		Both surfaces must be *locked* by \ref hw_surface_lock.
 *
 * @param	destination	The surface on which to copy pixels.
 * @param	dst_rect	If NULL, the entire destination surface is used. If not NULL,
 *				defines the rectangle on the destination surface where to copy
 *				the pixels.
 * @param	source		The surface from which to copy pixels.
 * @param	src_rect	If NULL, the entire source surface is used. If not NULL, defines the
 *				rectangle on the source surface from which to copy the pixels.
 * @param	alpha		If true, the final pixels are a combination of source and
 *				destination pixels weighted by the source alpha channel and
 *				the transparency of the final pixels is set to opaque.
 *				If false, the final pixels are an exact copy of the source pixels,
 				including the alpha channel.
 *
 * @return			Returns 0 on success, 1 on failure (different sizes between
 * 				source and destination).
 */
int ei_copy_surface(ei_surface_t destination,
                    const ei_rect_t* dst_rect,
                    ei_surface_t source,
                    const ei_rect_t* src_rect,
                    bool alpha)
{
    ei_rect_t rectangle_source;
    if (src_rect != NULL){
        rectangle_source = *src_rect;
        int top_left_src_x = rectangle_source.top_left.x;
        int top_right_src_x = rectangle_source.top_left.x + rectangle_source.size.width;
        if (top_left_src_x < 0)
        {
            rectangle_source.top_left.x = 0;
            rectangle_source.size.width += top_left_src_x;
        }
        if ( top_right_src_x > hw_surface_get_size(source).width){
            rectangle_source.size.width = rectangle_source.size.width - (top_right_src_x - hw_surface_get_size(source).width);
        }
        if (top_left_src_x > hw_surface_get_size(source).width){
            rectangle_source.top_left.x = hw_surface_get_size(source).width;
            rectangle_source.size.width = 0;
        }
    }
    else{
        rectangle_source = hw_surface_get_rect(source);
    }

    ei_rect_t rectangle_dest;
    if (dst_rect != NULL){
        rectangle_dest = *dst_rect;
        int top_left_dest_x = rectangle_dest.top_left.x;
        int top_right_dest_x = rectangle_dest.top_left.x + rectangle_dest.size.width;
        if (top_left_dest_x < 0)
        {
            rectangle_dest.top_left.x = 0;
            rectangle_dest.size.width += top_left_dest_x;
            rectangle_source.top_left.x += -top_left_dest_x;
            rectangle_source.size.width += top_left_dest_x;
        }
        if ( top_right_dest_x > hw_surface_get_size(destination).width)
        {
            rectangle_dest.size.width = rectangle_dest.size.width - (top_right_dest_x - hw_surface_get_size(destination).width);
            rectangle_source.size.width = rectangle_source.size.width - (top_right_dest_x - hw_surface_get_size(destination).width);
        }
        if (top_left_dest_x > hw_surface_get_size(destination).width)
        {
            rectangle_dest.top_left.x = hw_surface_get_size(destination).width;
            rectangle_dest.size.width = 0;
        }
    }
    else{
        rectangle_dest = hw_surface_get_rect(destination);
    }

    if (rectangle_source.size.height != rectangle_dest.size.height || rectangle_source.size.width != rectangle_dest.size.width){
        return 1;
    }
    hw_surface_lock(source);
    hw_surface_lock(destination);
    uint32_t* pixel_ptr_source = (uint32_t*)hw_surface_get_buffer(source);
    pixel_ptr_source += hw_surface_get_size(source).width * rectangle_source.top_left.y + rectangle_source.top_left.x;
    uint32_t* pixel_ptr_dest = (uint32_t*)hw_surface_get_buffer(destination);
    pixel_ptr_dest += hw_surface_get_size(destination).width * rectangle_dest.top_left.y + rectangle_dest.top_left.x;

    int sir, sig, sib, sia, dir, dig, dib, dia;
    hw_surface_get_channel_indices(source, &sir, &sig, &sib, &sia);
    hw_surface_get_channel_indices(destination, &dir, &dig, &dib, &dia);

    uint8_t *pixel_source = hw_surface_get_buffer(source);
    pixel_source += 4 * (hw_surface_get_size(source).width * rectangle_source.top_left.y + rectangle_source.top_left.x);
    uint8_t *pixel_dest = hw_surface_get_buffer(destination);
    pixel_dest += 4 * (hw_surface_get_size(destination).width * rectangle_dest.top_left.y + rectangle_dest.top_left.x);
    hw_surface_unlock(source);
    hw_surface_unlock(destination);

    for (int i = 0; i<min(rectangle_source.size.height, rectangle_dest.size.height); i++){
        for (int j = 0; j<min(rectangle_source.size.width, rectangle_dest.size.width); j++){
            if (alpha){
                pixel_dest[dir]=(uint8_t)((pixel_dest[dir] * (255 - pixel_source[sia]) + pixel_source[sir] * pixel_source[sia]) / 255) ;
                pixel_dest[dig]=(uint8_t)((pixel_dest[dig] * (255 - pixel_source[sia]) + pixel_source[sig] * pixel_source[sia]) / 255) ;
                pixel_dest[dib]=(uint8_t)((pixel_dest[dib] * (255 - pixel_source[sia]) + pixel_source[sib] * pixel_source[sia]) / 255) ;
                pixel_source += 4;
                pixel_dest += 4;
            }
            else{
                *pixel_ptr_dest = *pixel_ptr_source;
                pixel_ptr_source++;
                pixel_ptr_dest++;
            }
        }
        if (alpha){
            pixel_source += 4 * (hw_surface_get_size(source).width - rectangle_source.size.width);
            pixel_dest += 4 * (hw_surface_get_size(destination).width - rectangle_dest.size.width);
        }
        else{
            pixel_ptr_dest += hw_surface_get_size(destination).width - rectangle_dest.size.width;
            pixel_ptr_source += hw_surface_get_size(source).width - rectangle_source.size.width;
        }
    }
    return 0;
}