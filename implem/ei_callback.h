#include "ei_application.h"
#include "structure.h"

#ifndef EI_CALLBACK_H
#define EI_CALLBACK_H

/**
 * \brief	.
 *
 * @param	widget		.
 * @param	event	.
 * @param user_param .
 *
 * @return .
 */
bool callback_toplevel_redimension(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

/**
 * \brief	.
 *
 * @param	widget		.
 * @param	event	.
 * @param user_param .
 *
 * @return .
 */
bool callback_toplevel_deplace(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

/**
 * \brief	.
 *
 * @param	widget		.
 * @param	event	.
 * @param user_param .
 *
 * @return .
 */
bool callback_button_close_frame(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

#endif