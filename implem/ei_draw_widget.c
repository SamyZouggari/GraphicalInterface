#include "ei_draw_widget.h"
#include "ei_callback.h"
#include <ei_utils.h>
#include "ei_dessin_relief.h"
#include "ei_draw.h"
#include "ei_implementation.h"
#include "structure.h"
#include "ei_anchor.h"
#include "ei_im_draw.h"
#include "ei_placer.h"


#define max(a, b) ((a) >= (b) ? (a) : (b))
#define min(a, b) ((a) <= (b) ? (a) : (b))



void	ei_draw_button  	(ei_surface_t		surface,
                                   ei_widget_t             widget,
                                   ei_surface_t         pick_surface,
                                   const ei_rect_t*	clipper){

        hw_surface_lock(surface);
        ei_rect_t screen_loc = widget->screen_location;
        ei_rect_t rectangle;
        rectangle = screen_loc;

        struct ei_impl_button_t *button = (struct ei_impl_button_t*)widget;
        ei_color_t color = (ei_color_t)*(button->color);

        ei_color_t couleur_fonce = {max(0, (int)(color.red) * 0.66), max(0, (int)(color.green) * 0.66),max(0, (int)(color.blue) * 0.66), color.alpha};
        ei_color_t couleur_clair = {min(255, (int)(color.red) * 1.33), min(255, (int)(color.green) * 1.33),min(255, (int)(color.blue) * 1.33), color.alpha};

        int rayon = (int) *button->corner_radius;
        ei_point_t *tableau_haut = rounded_frame(rectangle, rayon, "haut");
        ei_point_t *tableau_bas = rounded_frame(rectangle, rayon, "bas");
        ei_point_t *tableau_tout = rounded_frame(rectangle, rayon, "tout");

        uint32_t nombre_el_arc = 7;
        uint32_t demi_tableau_nbr_el = 2 * nombre_el_arc + 2;
        uint32_t tableau_nbr_el = (int)(4 * nombre_el_arc);

        int bordure = (int) *button->border_width;

        ei_rect_t rectangle_interieur =(ei_rect_t) {{rectangle.top_left.x + bordure, rectangle.top_left.y + bordure}, {(int)rectangle.size.width - 2 * bordure, (int)rectangle.size.height - 2 * bordure}};
        ei_point_t *tableau_interieur = rounded_frame(rectangle_interieur, rayon, "tout");

        if (*(button->relief) == ei_relief_sunken){
                ei_draw_polygon(surface, tableau_haut, demi_tableau_nbr_el, couleur_fonce, clipper);
                ei_draw_polygon(surface, tableau_bas, demi_tableau_nbr_el, couleur_clair, clipper);
                ei_draw_polygon(surface, tableau_interieur, tableau_nbr_el, color, clipper);
        }
        else if (*(button->relief) == ei_relief_raised){
                ei_draw_polygon(surface, tableau_haut, demi_tableau_nbr_el, couleur_clair, clipper);
                ei_draw_polygon(surface, tableau_bas, demi_tableau_nbr_el, couleur_fonce, clipper);
                ei_draw_polygon(surface, tableau_interieur, tableau_nbr_el, color, clipper);
        }
        else
        {
                ei_draw_polygon(surface, tableau_tout, tableau_nbr_el, color, clipper);
        }
        ei_draw_polygon(pick_surface, tableau_tout, tableau_nbr_el, *widget->pick_color, clipper);

        if (button->text != NULL){
                ei_point_t ou = ei_anchor_text_button(*button, rectangle);
                ei_draw_text(surface, &(ou), *button->text, *button->text_font,*button->text_color, &rectangle_interieur);
                }

        if (button->img != NULL)
        {
                ei_draw_image_button(surface, *button, rectangle);
        }
        hw_surface_unlock(surface);
        free(tableau_haut);
        free(tableau_tout);
        free(tableau_bas);
        free(tableau_interieur);
}

void ei_draw_frame(ei_surface_t		surface,
                   ei_widget_t             widget,
                   ei_surface_t         pick_surface,
                   const ei_rect_t*	clipper){

        hw_surface_lock(surface);
        ei_rect_t rectangle = widget->screen_location;
        ei_rect_t rectangle_interieur = *widget->content_rect;
        struct ei_impl_frame_t *frame = (struct ei_impl_frame_t *) widget;
        ei_color_t color = *frame->color;

        ei_color_t couleur_fonce = {max(0, (int)(color.red) * 0.66), max(0, (int)(color.green) * 0.66),max(0, (int)(color.blue) * 0.66), color.alpha};
        ei_color_t couleur_clair = {min(255, (int)(color.red) * 1.33), min(255, (int)(color.green) * 1.33),min(255, (int)(color.blue) * 1.33), color.alpha};

        ei_point_t *tableau_haut = hard_relief(rectangle, "haut");
        ei_point_t *tableau_bas = hard_relief(rectangle, "bas");
        ei_point_t *tableau_tout = hard_relief(rectangle, "tout");
        ei_point_t *tableau_interieur = hard_relief(rectangle_interieur, "tout");

        if (*(frame->relief) == ei_relief_sunken){
                ei_draw_polygon(surface, tableau_haut, 5, couleur_fonce, clipper);
                ei_draw_polygon(surface, tableau_bas, 5, couleur_clair, clipper);
                ei_draw_polygon(surface, tableau_interieur, 4, color, clipper);
        }
        else if (*(frame->relief) == ei_relief_raised){
                ei_draw_polygon(surface, tableau_haut, 5, couleur_clair, clipper);
                ei_draw_polygon(surface, tableau_bas, 5, couleur_fonce, clipper);
                ei_draw_polygon(surface, tableau_interieur, 4, color, clipper);
        }
        else if (*(frame->relief) == ei_relief_none)
        {
                ei_draw_polygon(surface, tableau_tout, 4, color, clipper);
        }
        ei_draw_polygon(pick_surface, tableau_tout, 4, *widget->pick_color, clipper);

        if (frame->text != NULL){
                ei_point_t ou = ei_anchor_text_frame(*frame, rectangle);

                ei_draw_text(surface, &(ou), *frame->text, *frame->text_font,*frame->text_color, &widget->screen_location);
        }

        if (frame->img != NULL)
        {
                ei_draw_image_frame(surface, *frame, rectangle);
        }
        hw_surface_unlock(surface);
        free(tableau_haut);
        free(tableau_tout);
        free(tableau_bas);
        free(tableau_interieur);
        ei_widget_t curr=widget->children_head;
        while(curr!=NULL){
            ei_impl_widget_draw_children(curr,surface,pick_surface,&widget->screen_location);
            curr=curr->next_sibling;
        }

}

void ei_draw_en_tete (ei_surface_t surface, ei_widget_t widget, ei_surface_t pick_surface, ei_rect_t* clipper)
{
        hw_surface_lock(surface);
        struct ei_impl_toplevel_t* toplevel=(struct ei_impl_toplevel_t*)widget;
        uint32_t rayon=10;
        uint32_t nombre_el_arc = 7;
        ei_rect_t rectangle = widget->screen_location;
        ei_rect_t rect_int = *widget->content_rect;
        int border = *toplevel->border_width;

        ei_point_t* tableau_haut = (ei_point_t*)malloc(((nombre_el_arc * 2) + 2) * sizeof(ei_point_t));
        ei_point_t* tableau_tout = rounded_frame(rectangle , rayon,"tout");

        ei_point_t first_pt = rect_int.top_left;
        ei_point_t sec_pt = {rect_int.top_left.x, rect_int.top_left.y + rect_int.size.height};
        ei_point_t thi_pt = {rect_int.top_left.x + rect_int.size.width, rect_int.top_left.y + rect_int.size.height};
        ei_point_t for_pt = {rect_int.top_left.x + rect_int.size.width, rect_int.top_left.y};
        ei_point_t fif_pt = {rect_int.top_left.x + rect_int.size.width + border, rect_int.top_left.y};
        ei_point_t six_pt = {rect_int.top_left.x + rect_int.size.width + border, rect_int.top_left.y + rect_int.size.height + border};
        ei_point_t sev_pt = {rect_int.top_left.x - border, rect_int.top_left.y + rect_int.size.height + border};
        ei_point_t eig_pt = {rect_int.top_left.x - border, rect_int.top_left.y};

        ei_point_t tableau_bord[8];
        tableau_bord[0] = first_pt;
        tableau_bord[1] = sec_pt;
        tableau_bord[2] = thi_pt;
        tableau_bord[3] = for_pt;
        tableau_bord[4] = fif_pt;
        tableau_bord[5] = six_pt;
        tableau_bord[6] = sev_pt;
        tableau_bord[7] = eig_pt;

        memcpy(tableau_haut,tableau_tout,nombre_el_arc * 2 * sizeof(ei_point_t));
        tableau_haut[(int)((nombre_el_arc * 2))] = (ei_point_t){fif_pt.x, fif_pt.y};
        tableau_haut[(int)(((nombre_el_arc * 2)) + 1)] = eig_pt;

        ei_point_t point_1 = rectangle.top_left;

        ei_draw_polygon(surface,tableau_haut,((nombre_el_arc * 2) + 2),(ei_color_t){0x60,0x60,0x60,0xFF},clipper);
        ei_draw_polygon(surface, tableau_bord, 8, (ei_color_t){0x60,0x60,0x60,0xFF}, clipper);

        tableau_haut[(int)((nombre_el_arc * 2))] = six_pt;
        tableau_haut[(int)(((nombre_el_arc * 2)) + 1)] = sev_pt;
        ei_draw_polygon(pick_surface,tableau_tout,((nombre_el_arc * 2) + 8),*widget->pick_color,clipper);

        ei_widget_t curr=widget->children_head;

        if(curr!=NULL)
        {
            if(*toplevel->closable)
            {
                ei_widget_t button_close_widget = curr;
                button_close_widget->screen_location.top_left.x=widget->screen_location.top_left.x +10;
                button_close_widget->screen_location.top_left.y=widget->screen_location.top_left.y+5;
                button_close_widget->screen_location.size.width=15;
                button_close_widget->screen_location.size.height=15;
                curr=curr->next_sibling;
            }
            if(curr!=NULL){
                if(*toplevel->resizable!=ei_axis_none)
                {
                    ei_widget_t frame = curr;
                    ei_place(frame,NULL,&(int){border - 10},&(int){border - 10},&(int){10},&(int){10},&(float){1},&(float){1},NULL,NULL);
                }
            }
        }



        int width;
        int height;
        hw_text_compute_size(*toplevel->title, ei_default_font, &width, &height);
        ei_point_t point_text_clip = {rectangle.top_left.x + 20, rectangle.top_left.y};
        ei_rect_t rect_clipper = {point_text_clip,
                                (ei_size_t){rectangle.size.width - 3 * rayon, height}};
        ei_draw_text(surface, &(ei_point_t){point_1.x + 3 * rayon, point_1.y}, *toplevel->title, ei_default_font, (ei_color_t){255,255,255,255}, &rect_clipper);

        free(tableau_haut);
        free(tableau_tout);
        hw_surface_unlock(surface);

}

void ei_draw_toplevel(ei_surface_t surface,
                        ei_widget_t widget,
                        ei_surface_t pick_surface,
                        ei_rect_t* clipper)
{
        struct ei_impl_toplevel_t* toplevel=(struct ei_impl_toplevel_t*)widget;
        ei_draw_en_tete(surface, widget, pick_surface, clipper);

        ei_rect_t rect = *widget->content_rect;
        int top_left_x = rect.top_left.x;
        int top_left_y = rect.top_left.y;
        ei_size_t taille = rect.size;

        int border = *toplevel->border_width;

        ei_point_t point = {top_left_x, top_left_y};
        ei_point_t tab[4] = {{point.x,              point.y},
                               {point.x + taille.width, point.y},
                               {point.x + taille.width, point.y + taille.height},
                               {point.x,              point.y + taille.height}};

        ei_draw_polygon(surface, tab, (size_t) 4, *toplevel->color, &rect);
        ei_bind(ei_ev_mouse_buttondown,widget,NULL,(ei_callback_t){callback_toplevel_deplace},NULL);

        ei_impl_widget_draw_children(widget->children_head, surface, pick_surface, &rect);

}