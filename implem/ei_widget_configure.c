#include "structure.h"
#include "ei_widget_configure.h"
#include <ei_utils.h>
#include "ei_application.h"
#include "ei_callback.h"
#include "ei_types.h"

/**
 * @brief	Configures the attributes of widgets of the class "frame".
 *
 *		Parameters obey the "optional parameters" protocol: if a parameter is "NULL", the state
 *		of the configured frame for that parameter does not change.
 *		When a frame is created, its parameters are set with the default values specified here.
 *
 * @param	widget		The widget to configure.
 * @param	requested_size	The size requested for this widget, including the widget's borders.
 *				The geometry manager may override this size due to other constraints.
 *				Defaults to the "natural size" of the widget, ie. big enough to
 *				display the border and the text or the image. This may be (0, 0)
 *				if the widget has border_width=0, and no text and no image.
 * @param	color		The color of the background of the widget. Defaults to
 *				\ref ei_default_background_color.
 * @param	border_width	The width in pixel of the border decoration of the widget. The final
 *				appearance depends on the "relief" parameter. Defaults to 0.
 * @param	relief		Appearance of the border of the widget. Defaults to
 *				\ref ei_relief_none.
 * @param	text		The text to display in the widget, or NULL. Only one of the
 *				parameter "text" and "img" should be used (i.e. non-NULL). Defaults
 *				to NULL.
 * @param	text_font	The font used to display the text. Defaults to \ref ei_default_font.
 * @param	text_color	The color used to display the text. Defaults to
 *				\ref ei_font_default_color.
 * @param	text_anchor	The anchor of the text, i.e. where it is placed within the widget.
 *				Defines both the anchoring point on the parent and on the widget.
 *				Defaults to \ref ei_anc_center.
 * @param	img		The image to display in the widget, or NULL. Any surface can be
 *				used, but usually a surface returned by \ref hw_image_load. Only one
 *				of the parameter "text" and "img" should be used (i.e. non-NULL).
 				Defaults to NULL.
 * @param	img_rect	If not NULL, the rectangle defines a subpart of "img" to use as the
 *				image displayed in the widget. Defaults to NULL.
 * @param	img_anchor	The anchor of the image, i.e. where it is placed within the widget
 *				when the size of the widget is bigger than the size of the image.
 *				Defaults to \ref ei_anc_center.
 */
void			ei_frame_configure		(ei_widget_t		widget,
                                           ei_size_t*		requested_size,
                                           const ei_color_t*	color,
                                           int*			border_width,
                                           ei_relief_t*		relief,
                                           ei_string_t*		text,
                                           ei_font_t*		text_font,
                                           ei_color_t*		text_color,
                                           ei_anchor_t*		text_anchor,
                                           ei_surface_t*		img,
                                           ei_rect_ptr_t*		img_rect,
                                           ei_anchor_t*		img_anchor)
{
    struct ei_impl_frame_t* f = (struct ei_impl_frame_t*) widget;
    if (widget == NULL){
        return;
    }
    ei_color_t* c;
    int* bw;
    ei_relief_t* r;
    ei_font_t* tf;
    ei_color_t* tc;
    ei_anchor_t* ta;
    ei_anchor_t* ia;
    ei_rect_t* ir;

    if (requested_size != NULL) {
        f->widget.requested_size = *requested_size;
    }
    else{
        f->widget.requested_size = widget->screen_location.size;
    }
    c = f->color==NULL ? malloc(sizeof(const ei_color_t)) : f->color;
    *c = color!=NULL ? *color : ei_default_background_color;
    f->color = (const ei_color_t*)c;

    bw = f->border_width==NULL ? malloc(sizeof(int)) : f->border_width;
    *bw = border_width!=NULL ? *border_width : 0;
    f->border_width = bw;

    r = f->relief==NULL ? malloc(sizeof(ei_relief_t)) : f->relief;
    *r = relief!=NULL ? *relief : ei_relief_none;
    f->relief = r;


    if(text != NULL) {
        ei_string_t* t = f->text==NULL ? malloc(sizeof (ei_string_t)) : f->text;
        *t = *text;
        f->text = t;
    }

    tf = f->text_font==NULL ? malloc(sizeof(ei_font_t)) : f->text_font;
    *tf = text_font!=NULL ? *text_font : ei_default_font;
    f->text_font = tf;

    tc = f->text_color==NULL ? malloc(sizeof(ei_color_t)) : f->text_color;
    *tc = text_color!=NULL ? *text_color : ei_font_default_color;
    f->text_color = tc;

    ta = f->text_anchor==NULL ? malloc(sizeof(ei_anchor_t)) : f->text_anchor;
    *ta = text_anchor!=NULL ? *text_anchor : ei_anc_center;
    f->text_anchor = ta;

    if(img != NULL) {
        if (f->img != NULL)
        {
            hw_surface_free(f->img);

        }
        else
        {
            f->img = malloc(sizeof(ei_surface_t));
        }
        ei_surface_t surface_copie = hw_surface_create(ei_app_root_surface(), hw_surface_get_size(*img), 1);
        ei_copy_surface(surface_copie, NULL, *img, NULL, 1);
        f->img = surface_copie;

    }
    ir = f->img_rect==NULL ? malloc(sizeof(ei_rect_t)) : f->img_rect;
    if (img_rect != NULL){
            *ir = **img_rect;
    }
    else{
            ir = NULL;
    }
    f->img_rect = ir;

    ia = f->img_anchor==NULL ? malloc(sizeof (ei_anchor_t)) : f->img_anchor;
    *ia = img_anchor!=NULL ? *img_anchor : ei_anc_center;
    f->img_anchor = ia;
}



//static const int	k_default_button_border_width	= 4;	///< The default border width of button widgets.
//static const int	k_default_button_corner_radius	= 10;	///< The default corner radius of button widgets.

//static inline void ei_frame_set_bg_color	(ei_widget_t frame,  ei_color_t bg_color)	{ ei_frame_configure(frame, NULL, &bg_color, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL); }
//static inline void ei_frame_set_text		(ei_widget_t frame,  ei_string_t text)		{ ei_frame_configure(frame, NULL, NULL, NULL, NULL, &text, NULL, NULL, NULL, NULL, NULL, NULL); }
//static inline void ei_frame_set_image		(ei_widget_t frame,  ei_surface_t image)	{ ei_frame_configure(frame, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &image, NULL, NULL); }


/**
 * @brief	Configures the attributes of widgets of the class "button".
 *
 * @param	widget, requested_size, color, border_width, relief,
 *		text, text_font, text_color, text_anchor,
 *		img, img_rect, img_anchor
 *				See the parameter definition of \ref ei_frame_configure. The only
 *				difference is that relief defaults to \ref ei_relief_raised
 *				and border_width defaults to \ref k_default_button_border_width.
 * @param	corner_radius	The radius (in pixels) of the rounded corners of the button.
 *				0 means straight corners. Defaults to \ref k_default_button_corner_radius.
 * @param	callback	The callback function to call when the user clicks on the button.
 *				Defaults to NULL (no callback).
 * @param	user_param	A programmer supplied parameter that will be passed to the callback
 *				when called. Defaults to NULL.
 */

void			ei_button_configure		(ei_widget_t		widget,
                                            ei_size_t*		requested_size,
                                            const ei_color_t*	color,
                                            int*			border_width,
                                            int*			corner_radius,
                                            ei_relief_t*		relief,
                                            ei_string_t*		text,
                                            ei_font_t*		text_font,
                                            ei_color_t*		text_color,
                                            ei_anchor_t*		text_anchor,
                                            ei_surface_t*		img,
                                            ei_rect_ptr_t*		img_rect,
                                            ei_anchor_t*		img_anchor,
                                            ei_callback_t*		callback,
                                            ei_user_param_t*	user_param)
{

    struct ei_impl_button_t* b = (struct ei_impl_button_t*) widget;
    if (widget == NULL){
        return;
    }
    ei_color_t* c;
    int* bw;
    ei_relief_t* r;
    ei_font_t* tf;
    ei_color_t* tc;
    ei_anchor_t* ta;
    ei_surface_t* ig;
    ei_rect_t* ir;
    ei_callback_t* cb;
    ei_user_param_t*  up;
    ei_anchor_t* ia;
    int* cr;
    ei_relief_t* rlf;
    ei_string_t* txt;

    c = b->color==NULL ? malloc(sizeof(const ei_color_t)) : b->color;
    *c = color!=NULL ? *color : ei_default_background_color;
    b->color = c;

    bw = b->border_width==NULL ? malloc(sizeof(int)) : b->border_width;
    *bw = border_width!=NULL ? *border_width : k_default_button_border_width;
    b->border_width = bw;

    cr = b->corner_radius==NULL ? malloc(sizeof(int)) : b->corner_radius;
    *cr = corner_radius!=NULL ? *corner_radius : k_default_button_corner_radius;
    b->corner_radius = cr;

    r = b->relief==NULL ? malloc(sizeof(ei_relief_t)) : b->relief;
    *r= relief!=NULL ? *relief : ei_relief_raised;
    b->relief = r;

    txt=b->text==NULL ? malloc(sizeof(ei_string_t)): b->text;
    *txt=text!=NULL ? *text:NULL;
    b->text=txt;

    tf = b->text_font==NULL ? malloc(sizeof(ei_font_t)) : b->text_font;
    *tf = text_font!=NULL ? *text_font : ei_default_font;
    b->text_font = tf;

    tc = b->text_color==NULL ? malloc(sizeof(ei_color_t)) : b->text_color;
    *tc= text_color!=NULL ? *text_color : ei_font_default_color;
    b->text_color = tc;

    ta= b->text_anchor==NULL ? malloc(sizeof(ei_anchor_t)) : b->text_anchor;
    *ta= text_anchor!=NULL ? *text_anchor : ei_anc_center;
    b->text_anchor = ta;

    if(img != NULL) {
        if (b->img != NULL)
        {
            hw_surface_free(b->img);
        }
        else
        {
            b->img = malloc(sizeof(ei_surface_t));
        }
        ei_surface_t surface_copie = hw_surface_create(ei_app_root_surface(), hw_surface_get_size(*img), 1);
        ei_copy_surface(surface_copie, NULL, *img, NULL, 1);
        b->img = surface_copie;
    }

    ir= b->img_rect==NULL ? malloc(sizeof(ei_rect_t)) : b->img_rect;
    if (img_rect != NULL){
            *ir = **img_rect;
    }
    else{
            ir = NULL;
    }
    b->img_rect = ir;

    ia = b->img_anchor==NULL ? malloc(sizeof (ei_anchor_t)) : b->img_anchor;
    *ia = img_anchor!=NULL ? *img_anchor : ei_anc_center;
    b->img_anchor = ia;




    if(user_param==NULL) {
        up=NULL;
    }
    else {
        up=*user_param;
    }
    b->user_param = up;

    if(callback != NULL) {

        ei_callback_t* cb = malloc(sizeof (ei_callback_t));
        *cb = *callback;
        b->callback=cb;

        ei_bind(ei_ev_mouse_buttondown,widget,NULL,callback_button,/*NULL);*/b->user_param);

    }
    ei_size_t* rs = malloc(sizeof(ei_size_t));
    if (requested_size != NULL) {
        b->widget.requested_size = *requested_size;
    }
    else{
        int w = 0;
        int h = 0;
        hw_text_compute_size("text",ei_default_font,&w,&h);
        *rs = (ei_size_t){w,h};
        b->widget.requested_size = *rs;
    }
    free(rs);
}

/**
 * @brief	Configures the attributes of widgets of the class "toplevel".
 *
 * @param	widget		The widget to configure.
 * @param	requested_size	The content size requested for this widget, this does not include
 *				the decorations	(border, title bar). The geometry manager may
 *				override this size due to other constraints.
 *				Defaults to (320x240).
 * @param	color		The color of the background of the content of the widget. Defaults
 *				to \ref ei_default_background_color.
 * @param	border_width	The width in pixel of the border of the widget. Defaults to 4.
 * @param	title		The string title displayed in the title bar. Defaults to "Toplevel".
 *				Uses the font \ref ei_default_font.
 * @param	closable	If true, the toplevel is closable by the user, the toplevel must
 *				show a close button in its title bar. Defaults to true.
 * @param	resizable	Defines if the widget can be resized horizontally and/or vertically
 *				by the user. Defaults to \ref ei_axis_both.
 * @param	min_size	For resizable widgets, defines the minimum size of its content.
 *				The default minimum size of a toplevel is (160, 120).
 *				If *min_size is NULL, this requires the min_size to be configured to
 *				the default size.
 */
void			ei_toplevel_configure		(ei_widget_t		widget,
                                              ei_size_t*		requested_size,
                                              const ei_color_t*	color,
                                              int*			border_width,
                                              ei_string_t*		title,
                                              bool*			closable,
                                              ei_axis_set_t*		resizable,
                                              ei_size_ptr_t*		min_size)
{
    struct ei_impl_toplevel_t* t = (struct ei_impl_toplevel_t*) widget;
    ei_color_t* c = malloc(sizeof(const ei_color_t));
    int* bw = malloc(sizeof(int));
    ei_string_t* ttl = malloc(sizeof(ei_string_t));
    bool* cls = malloc(sizeof(bool));
    ei_axis_set_t* rs = malloc(sizeof(ei_axis_set_t*));
    ei_size_t* min_s = malloc(sizeof(ei_size_t));
    ei_size_ptr_t * min_s_ptr = malloc(sizeof(ei_size_ptr_t));
    if (widget == NULL){
        return;
    }
    if (requested_size != NULL) {
        t->widget.requested_size = *requested_size;
    }
    else{

        t->widget.requested_size = widget->screen_location.size;
    }
    if(color != NULL) {
        *c=*color;
    }
    else{
        *c = ei_default_background_color;
    }
    t->color = (const ei_color_t*)c;

    if(border_width != NULL) {
        *bw = *border_width;
    }
    else{
        *bw = 4;
    }
    t->border_width = bw;
    if(title != NULL) {
        *ttl = *title;
    }
    else{
        *ttl = ei_default_font;
    }
    t->title = ttl;

    if(closable != NULL) {
        *cls = *closable;
    }
    else{
        *cls = true;
    }
    t->closable = cls;
    if(*t->closable)
    {
        ei_widget_t button_close_widget = ei_widget_create("button",(ei_widget_t)t,NULL,NULL);
        ei_button_configure(button_close_widget,NULL,&(ei_color_t){0xFF,0x00,0x00,0xFF},&(int){2},NULL,&(ei_relief_t){ei_relief_raised},NULL,NULL,NULL,NULL,NULL,NULL,NULL,&(ei_callback_t){callback_button_close_frame},NULL);

    }
    if(resizable != NULL) {
        *rs = *resizable;
    }
    else{
        *rs = ei_axis_both;
    }
    t->resizable = rs;
    if(*t->resizable!=ei_axis_none)
    {
        ei_widget_t frame = ei_widget_create("frame",(ei_widget_t)t,NULL,NULL);
        ei_bind(ei_ev_mouse_buttondown,frame,NULL,(ei_callback_t){callback_toplevel_redimension},NULL);
        ei_frame_set_bg_color(frame,(ei_color_t){0x60,0x60,0x60,0xFF});
    }

    if(min_size != NULL) {
        *min_s_ptr = *min_size;
    }
    else{
        *min_s = (ei_size_t){160,120};
        *min_s_ptr = min_s;
    }
    t->min_size = min_s_ptr;
}





