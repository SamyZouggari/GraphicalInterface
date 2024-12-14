#ifndef EI_DESSIN_RELIEF_H
#define EI_DESSIN_RELIEF_H


#include <stdint.h>
#include "ei_types.h"
#include <math.h>

/**
 * \brief	Creats an array of points defining a arc.
 *
 * @param	centre 	The centre of the circle - in which the arc is.
 * @param	rayon 	The size of the radius of the circle.
 * @param	angle_debut The angle (in radians) from where the circular arc starts.
 * @param	angle_fin   The angle (in radians) where the arc stops.
 * @return	An array of points \ref ei_point_t defining the arc.
 */
ei_point_t*     arc     (ei_point_t centre,
                         int rayon,
                         float angle_debut,
                         float angle_fin);

/**
 * \brief	Creats an array of points defining a rounded frame. This array of point can
 *              also create a top or bottom half rounded frame.
 *
 * @param	rectangle   The rectangle defined by a size \ref ei_size_t and a top left point
 *                          \ref ei_point_t defining the pattern of the rounded frame.
 * @param	rayon 	The size of the radius of the rounded angle. The bigger the radius is
 *                      the more round the angles of the frame will be.
 * @param	hautbastout     A string taking three values : "haut", "bas", "tout". Used to
 *                              know if the array returned defines only a top or bottom half rounded
 *                              frame or the whole frame.
 * @return	An array of points \ref ei_point_t defining the frame.
 */
ei_point_t* rounded_frame(ei_rect_t rectangle, int rayon, char* hautbastout);

/**
 * \brief	Creats an array of points defining a frame. This array of point can
 *              also create a top or bottom half frame.
 *
 * @param	rectangle   The rectangle defined by a size \ref ei_size_t and a top left point
 *                          \ref ei_point_t defining the pattern of the rounded frame.
 * @param	hautbastout     A string taking three values : "haut", "bas", "tout". Used to
 *                              know if the array returned defines only a top or bottom half rounded
 *                              frame or the whole frame.
 * @return	An array of points \ref ei_point_t defining the frame.
 */
ei_point_t* hard_relief(ei_rect_t rectangle, char* hautbastout);

#endif