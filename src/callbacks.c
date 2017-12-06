/**
 *       @file  callbacks.c
 *      @brief  Callbacks for buttons and widgets and other handlers
 *
 *     Functions that act as response to asynchronous events such as widget callbacks,
 *     signal handlers, timeout handlers
 *
 *     @author  Vitor Santos, vitor@ua.pt
 *
 * =========================================================
 */

#define _CALLBACKS_C_

#include "myf.h"

/**
 * @brief  Callback para responder em caso de exposição da área de desenho
 *
 * @param  widget the causing widget
 * @param  event the associated event 
 * @param  user_data the usual user data
 * @return TRUE to pass events into the hierarchy (if any)
 */
gboolean on_drawingarea1_expose_event(GtkWidget * widget, GdkEvent * event, gpointer user_data)
{
#if GTKVER < 3
	pari_PerformImageAcquisition(captureG);   //acquire new image
#endif
	// Perform here the openCV transformations
	pari_ProcessUserOperations(src_imageG, dst_imageG);

	//update the drawing are displays
	pari_RefreshDrawingArea("drawingarea1", src_imageG);
	pari_RefreshDrawingArea("drawingarea2", dst_imageG);

	return TRUE;
}

/**
 * @brief  Callback to process the quit button
 *
 * @param  widget the associated widget
 * @return void
 */
void on_button1_clicked(GtkWidget * widget, gpointer user_data)
{
	g_print("Sair normalmente\n");
	gtk_main_quit();
}

/**
 * @brief  Callback to process the checkbox7 (special) when toggled
 *
 * Only changes a global variable for a special effect that can be customised
 *
 * @param  widget the associated widget
 * @return void
 */
gboolean on_checkbutton7_toggled(GtkToggleButton * widget, GdkEvent * event, gpointer user_data)
{
	gboolean specialBox = gtk_toggle_button_get_active (widget);
	
	GtkWidget *box = GTK_WIDGET(gtk_builder_get_object(builderG, "vbox1"));
	GtkWidget *butbox = GTK_WIDGET(gtk_builder_get_object(builderG, "hbuttonbox2"));
	if( specialBox )
	{
		gtk_widget_hide(box);
		gtk_widget_hide(butbox);
	}
	else
	{
		gtk_widget_show(box);
		gtk_widget_show(butbox);
	}

	stretchOnG = specialBox;  //change a global variable


	return TRUE;
}

/**
 * @brief  Handler for the timeout
 *
 *   Create a expose event (draw event in GTK3) in a special widget
 *   as consequence of the timeout action and ensure periodic refresh of widget.
 *
 * @param  data just to be a proper handler. Not really used. But could be some handler.
 * @return boolean to re-arm the timeout
 */
gboolean pari_UpdateImageAreas(gpointer data)
{
	GtkWidget *da = GTK_WIDGET(gtk_builder_get_object(builderG, "drawingarea1"));
	gtk_widget_queue_draw(da);
	return TRUE;  //to re-arm the timeout.
}


/**
 * @brief  Function to detect when main window is destroyed. Nothing else can be done!
 * 
 * Stop the GTK+ main loop function when the window is destroyed.
 *
 * @param  Widget and user data
 */
void pari_destroy(GtkWidget * window, gpointer data)
{
	puts("vou sair");
	gtk_main_quit();
}

/**
 * @brief  Function to detect the forced closing of the main window. That can be avoided.
 *
 * @param  Widget and user data
 * @return Return FALSE to destroy the widget. Return TRUE to ignore.
 */
gboolean pari_delete_event(GtkWidget * window, GdkEvent * event, gpointer data)
{
	g_print("You should not use the OS to force leave!\n");
	return TRUE;
}

/**
 * @brief  handler to intersect CTRL-C
 */
void pari_InterceptCTRL_C( int a )
{
	g_print("Leaving with CTRL-C\n");
	gtk_main_quit();
}

