#include "ei_types.h"
#include <math.h>

#define max(a, b) ((a) >= (b) ? (a) : (b))
#define min(a, b) ((a) <= (b) ? (a) : (b))

ei_point_t* arc(ei_point_t centre, int rayon, float angle_debut, float angle_fin) {
        int x1 = centre.x;
        int y1 = centre.y;
        double vrai_nbr_pts = 6;
        double pas = (max(angle_debut, angle_fin) - min(angle_debut, angle_fin)) / vrai_nbr_pts;
        ei_point_t* tableau = (ei_point_t*)malloc((vrai_nbr_pts + 1) * sizeof(ei_point_t));
        for (int i = 0; i < vrai_nbr_pts + 1; i++) {
                int pluscos = (int)(rayon * cos(angle_debut));
                int plussin = (int)(rayon * sin(angle_debut));
                tableau[i].x = x1 + pluscos;
                tableau[i].y = y1 + plussin;
                angle_debut += pas;
        }
        return tableau;
}

ei_point_t* rounded_frame(ei_rect_t rectangle, int rayon, char* hautbastout) {
        uint32_t nombre_el_arc = 7;
        ei_point_t centre_hg = {rectangle.top_left.x + rayon, rectangle.top_left.y + rayon};
        ei_point_t centre_bg = {rectangle.top_left.x + rayon, rectangle.top_left.y + rectangle.size.height - rayon};
        ei_point_t centre_bd = {rectangle.top_left.x + rectangle.size.width - rayon, rectangle.top_left.y + rectangle.size.height - rayon};
        ei_point_t centre_hd = {rectangle.top_left.x + rectangle.size.width - rayon, rectangle.top_left.y + rayon};
        ei_point_t* tableau_tout = (ei_point_t*)malloc((nombre_el_arc * 4) * sizeof(ei_point_t));
        ei_point_t* tableau_hg = arc(centre_hg, rayon, 3.14159265359, 3.14159265359 * 1.5);
        ei_point_t* tableau_bg = arc(centre_bg, rayon, 3.14159265359 * 0.5, 3.14159265359);
        ei_point_t* tableau_bd = arc(centre_bd, rayon, 0, 3.14159265359 * 0.5);
        ei_point_t* tableau_hd = arc(centre_hd, rayon, 3.14159265359 * 1.5, 2 * 3.14159265359);
        memcpy(tableau_tout, tableau_hg, nombre_el_arc * sizeof(ei_point_t));
        memcpy(tableau_tout + nombre_el_arc, tableau_hd, nombre_el_arc * sizeof(ei_point_t));
        memcpy(tableau_tout + 2 * nombre_el_arc, tableau_bd, nombre_el_arc * sizeof(ei_point_t));
        memcpy(tableau_tout + 3 * nombre_el_arc, tableau_bg, nombre_el_arc * sizeof(ei_point_t));
        if (strcmp(hautbastout, "haut") == 0) {
                ei_point_t* tableau_haut = (ei_point_t*)malloc((2 * nombre_el_arc + 2) * sizeof(ei_point_t));
                uint32_t demi_hauteur = (int)(min(rectangle.size.width, rectangle.size.height) * 0.5);
                memcpy(tableau_haut, tableau_hg, nombre_el_arc * sizeof(ei_point_t));
                memcpy(tableau_haut + nombre_el_arc, tableau_hd, (int)((nombre_el_arc - 1) * 0.5 + 1) * sizeof(ei_point_t));
                ei_point_t point_demi_d = {rectangle.top_left.x + rectangle.size.width - demi_hauteur, rectangle.top_left.y + demi_hauteur};
                tableau_haut[nombre_el_arc + (int)((nombre_el_arc - 1) * 0.5 + 1)] = point_demi_d;
                ei_point_t point_demi_g = {rectangle.top_left.x + demi_hauteur, rectangle.top_left.y + rectangle.size.height - demi_hauteur};
                tableau_haut[nombre_el_arc + (int)((nombre_el_arc - 1) * 0.5 + 1) + 1] = point_demi_g;
                memcpy(tableau_haut + (int)(nombre_el_arc + (nombre_el_arc - 1) * 0.5 + 1 + 1 + 1), tableau_bg + (int)((nombre_el_arc - 1) * 0.5), (int)((nombre_el_arc - 1) * 0.5) * sizeof(ei_point_t));
                free(tableau_hd);
                free(tableau_bg);
                free(tableau_hg);
                free(tableau_bd);
                free(tableau_tout);
                return tableau_haut;
        }
        if (strcmp(hautbastout, "bas") == 0) {
                ei_point_t* tableau_bas = (ei_point_t*)malloc((2 * nombre_el_arc + 2) * sizeof(ei_point_t));
                uint32_t demi_hauteur = (uint32_t)(min(rectangle.size.width, rectangle.size.height) * 0.5);
                memcpy(tableau_bas, tableau_hd + (int)((nombre_el_arc - 1) * 0.5), (int)((nombre_el_arc - 1) * 0.5 + 1) * sizeof(ei_point_t));
                memcpy(tableau_bas + (int)((nombre_el_arc - 1) * 0.5), tableau_bd, nombre_el_arc * sizeof(ei_point_t));
                memcpy(tableau_bas + (int)(nombre_el_arc + (nombre_el_arc - 1) * 0.5), tableau_bg + 1, (int)((nombre_el_arc - 1) * 0.5 + 1) * sizeof(ei_point_t));
                ei_point_t point_demi_g = {rectangle.top_left.x + demi_hauteur, rectangle.top_left.y + rectangle.size.height - demi_hauteur};
                tableau_bas[2 * nombre_el_arc] = point_demi_g;
                ei_point_t point_demi_d = {rectangle.top_left.x + rectangle.size.width - demi_hauteur, rectangle.top_left.y + demi_hauteur};
                tableau_bas[2 * nombre_el_arc + 1] = point_demi_d;
                free(tableau_hd);
                free(tableau_bg);
                free(tableau_hg);
                free(tableau_bd);
                free(tableau_tout);
                return tableau_bas;
        }
        if (strcmp(hautbastout, "tout") == 0) {
                free(tableau_hd);
                free(tableau_bg);
                free(tableau_hg);
                free(tableau_bd);
                return tableau_tout;
        } else {
                printf("Mauvais choix, choisir haut, bas ou tout");
                exit(EXIT_FAILURE);
        }
}

ei_point_t* hard_relief(ei_rect_t rectangle, char* hautbastout){
        ei_point_t* tableau_tout = (ei_point_t*)malloc(4 * sizeof(ei_point_t));
        tableau_tout[0] = (ei_point_t){rectangle.top_left.x, rectangle.top_left.y};
        tableau_tout[1] = (ei_point_t){rectangle.top_left.x + rectangle.size.width, rectangle.top_left.y};
        tableau_tout[2] = (ei_point_t){rectangle.top_left.x + rectangle.size.width, rectangle.top_left.y + rectangle.size.height};
        tableau_tout[3] = (ei_point_t){rectangle.top_left.x, rectangle.top_left.y + rectangle.size.height};
        uint32_t demi_hauteur=0;
        demi_hauteur = (int)(min(rectangle.size.width, rectangle.size.height) * 0.5);
        if (strcmp(hautbastout, "haut") == 0) {
                ei_point_t* tableau_haut = (ei_point_t*)malloc(5 * sizeof(ei_point_t));
                tableau_haut[0] = tableau_tout[0];
                tableau_haut[1] = tableau_tout[1];
                ei_point_t point_demi_d = {rectangle.top_left.x + rectangle.size.width - demi_hauteur, rectangle.top_left.y + demi_hauteur};
                ei_point_t point_demi_g = {rectangle.top_left.x + demi_hauteur, rectangle.top_left.y + rectangle.size.height - demi_hauteur};
                tableau_haut[2] = point_demi_d;
                tableau_haut[3] = point_demi_g;
                tableau_haut[4] = tableau_tout[3];
                free(tableau_tout);
                return tableau_haut;
        }
        if (strcmp(hautbastout, "bas") == 0) {
                ei_point_t* tableau_bas = (ei_point_t*)malloc(5 * sizeof(ei_point_t));
                memcpy(tableau_bas, tableau_tout + 1, 3 * sizeof(ei_point_t));
                ei_point_t point_demi_d = {rectangle.top_left.x + rectangle.size.width - demi_hauteur, rectangle.top_left.y + demi_hauteur};
                ei_point_t point_demi_g = {rectangle.top_left.x + demi_hauteur, rectangle.top_left.y + rectangle.size.height - demi_hauteur};
                tableau_bas[4] = point_demi_d;
                tableau_bas[3] = point_demi_g;
                free(tableau_tout);
                return tableau_bas;
        }
        if (strcmp(hautbastout, "tout") == 0) {
                return tableau_tout;
        }
        else {
                printf("Mauvais choix, choisir haut, bas ou tout");
                exit(EXIT_FAILURE);
        }
}