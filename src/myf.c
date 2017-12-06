/**
 *     @file   myf.c
 *     @brief  Auxiliary functions that are not callbacks or handlers
 *
 *     Detailed description starts here.
 *
 *     @author  Vitor Santos, vitor@ua.pt
 *
 * =========================================================
 */

#define _MYF_C_

#include "myf.h"

/**
 * @brief  Function that converts and IplImage into a pixbuf for display purposes
 *
 * 	Must perform some color transformation before
 *      // newer versions could use gdk_pixbuf_new_from_bytes()
 *
 * @param  image The IplImage
 * @param  dst_w width of the new pixbuf
 * @param  dst_h height of the new pixbuf
 * @return The GdkPixbuf *
 */
GdkPixbuf *pari_ConvertOpenCv2Gtk(IplImage * image, int dst_w, int dst_h, gboolean stretch)
{
	IplImage *gtkMask=image;
	cvCvtColor(image, gtkMask, CV_BGR2RGB);	// Usually opencv image is BGR, so we need to change it to RGB
	GdkPixbuf *pix, *rpix;
	pix = gdk_pixbuf_new_from_data((guchar *) gtkMask->imageData,
				       GDK_COLORSPACE_RGB,
				       FALSE,
				       gtkMask->depth,
				       gtkMask->width,
				       gtkMask->height,
				       gtkMask->widthStep, 
				       NULL, //callback to free allocated memory: void (*GdkPixbufDestroyNotify) (guchar *pixels, gpointer fn_data);
				       NULL  //argument to previous callback: gpointer fn_data
				       );

#if 1  //to force image to fill up drawing area
	double ocvRatio=(double)gtkMask->width/gtkMask->height;  //image ratio in opencv
	double gtkRatio=(double)dst_w/dst_h;    //image ratio in gtk
	if(! stretch)
	{
		if( gtkRatio <= ocvRatio)
			dst_h = (int)(dst_w/ocvRatio);
		else
			dst_w = (int)(dst_h*ocvRatio);
	}
#endif

        rpix = gdk_pixbuf_scale_simple(pix, dst_w, dst_h, GDK_INTERP_BILINEAR);
	g_object_unref(pix);  //free the intermediate pixbuf ... not sure if it works as expected

	return rpix;
}


/**
 * @brief Copy from Iplimage to pixbuf and paint the widget
 *
 * @param  widgetName String with the widget name in glade
 * @param  img IplImage where to get the image from
 * @return void
 */
void pari_RefreshDrawingArea( char * widgetName, IplImage *img)  //copy from iplimage to pixbuf and paint DA
{
	GtkWidget *da=GTK_WIDGET(gtk_builder_get_object (builderG, widgetName));
	if( ! da ) { printf("failed for %s\n", widgetName); return;}

	GdkPixbuf *pix=pari_ConvertOpenCv2Gtk(img, da->allocation.width, da->allocation.height, stretchOnG );
    	cairo_t   *cr = gdk_cairo_create (gtk_widget_get_window(da));
    	gdk_cairo_set_source_pixbuf(cr, pix, 0, 0);
    	cairo_paint(cr);
    	cairo_fill(cr);
    	cairo_destroy(cr);
	g_object_unref(pix);  //free the pixbuf ... not sure if it works as expected
}

/**
 * @brief  Function to process user's options upon passed image
 *
 * @param  src the src image
 * @param  dst the destination image
 * @return void
 */
void pari_ProcessUserOperations(IplImage *src, IplImage *dst)
{
	/*If nothing is to be done at least the dst is the same as src*/
	cvCopy( src, dst, NULL);
	
	//Now operate all functions accumulated (one after one)
	pari_UserOperation1(dst, dst, builderG, (gpointer)"checkbutton1", NULL);
	pari_UserOperation2(dst, dst, builderG, (gpointer)"checkbutton2", (gpointer)"adjustment1");
	pari_UserOperation3(dst, dst, builderG, (gpointer)"checkbutton3", (gpointer)"adjustment2");
	pari_UserOperation4(dst, dst, builderG, (gpointer)"checkbutton4", NULL);
	pari_UserOperation5(dst, dst, builderG, (gpointer)"checkbutton5", NULL);
	pari_UserOperation6(dst, dst, builderG, (gpointer)"checkbutton6", (gpointer)"adjustment3");
}


/**
 * @brief  Function to perform some specific operation. Convert to gray level.
 *
 * @param  src Source image to process
 * @param  dst Image to put the processed result
 * @param  gb  GtkBuilder pointer for the current GUI.
 * @param  udata1 gpointer for generic data for the operation. Normally the widget name. Return if NULL
 * @param  udata2 gpointer for generic data for the operation. Normally the widget name for auxiliary parameter. Ignored here.
 */
void pari_UserOperation1(IplImage *src, IplImage *dst, GtkBuilder *gb, gpointer udata1, gpointer udata2)
{
	if( ! (gb && udata1) ) return;

	GtkToggleButton *grayl_w = (GtkToggleButton *)GTK_WIDGET(gtk_builder_get_object(gb, "checkbutton1"));
	gboolean graylOn= gtk_toggle_button_get_active(grayl_w);
	if( graylOn )
	{
		IplImage *tmp_gray=cvCreateImage( cvSize(src->width,src->height), IPL_DEPTH_8U, 1);  //aux image
		cvCvtColor(src, tmp_gray, CV_RGB2GRAY);
		cvMerge(tmp_gray, tmp_gray, tmp_gray, NULL, dst);
		cvReleaseImage(&tmp_gray);   //free aux image
	}
}

/**
 * @brief  Function to perform some specific operation. Threshold!
 *
 * @param  src Source image to process
 * @param  dst Image to put the processed result
 * @param  gb  GtkBuilder pointer for the current GUI.
 * @param  udata1 gpointer for generic data for the operation. Normally the widget name. NULL forces no action.
 * @param  udata2 gpointer for generic data for the operation. Normally the widget name for auxiliary parameter. NULL forces no action.
 */
void pari_UserOperation2(IplImage *src, IplImage *dst, GtkBuilder *gb, gpointer udata1, gpointer udata2)
{
	if( ! (gb && udata1 && udata2) ) return;

	GtkToggleButton *binon_w = (GtkToggleButton *)(gtk_builder_get_object(builderG, udata1));
	GtkAdjustment   *bin_adj = GTK_ADJUSTMENT(gtk_builder_get_object(builderG, udata2));
	gboolean binOn  = gtk_toggle_button_get_active(binon_w);
	gdouble  binLim = gtk_adjustment_get_value(bin_adj);
	if(binOn)   cvThreshold(src, dst, binLim*255/100, 255,CV_THRESH_BINARY);
}


/**
 * @brief  Function to perform some specific operation. Erosion!
 *
 * @param  src Source image to process
 * @param  dst Image to put the processed result
 * @param  gb  GtkBuilder pointer for the current GUI.
 * @param  udata1 gpointer for generic data for the operation. Normally the widget name. NULL forces no action.
 * @param  udata2 gpointer for generic data for the operation. Normally the widget name for auxiliary parameter. NULL forces no action.
 */
void pari_UserOperation3(IplImage *src, IplImage *dst, GtkBuilder *gb, gpointer udata1, gpointer udata2)
{
	if( ! (gb && udata1 && udata2) ) return;

	GtkToggleButton *erode_w = (GtkToggleButton *)GTK_WIDGET(gtk_builder_get_object(gb, udata1));
	GtkAdjustment   *ero_adj = GTK_ADJUSTMENT(gtk_builder_get_object(gb, udata2));
	gdouble   eroNum = gtk_adjustment_get_value(ero_adj);
	gboolean erodeOn = gtk_toggle_button_get_active(erode_w);

	if(erodeOn) cvErode(src, dst, NULL, eroNum);
}

/**
 * @brief  Function to perform some specific operation. Image Negation!
 *
 * @param  src Source image to process
 * @param  dst Image to put the processed result
 * @param  gb  GtkBuilder pointer for the current GUI.
 * @param  udata1 gpointer for generic data for the operation. Normally the widget name. NULL forces no action.
 * @param  udata2 gpointer for generic data for the operation. Normally the widget name for auxiliary parameter. Ignored here.
 */
void pari_UserOperation4(IplImage *src, IplImage *dst, GtkBuilder *gb, gpointer udata1, gpointer udata2)
{
	if( ! (gb && udata1) ) return;

	GtkToggleButton *invrt_w = (GtkToggleButton *)GTK_WIDGET(gtk_builder_get_object(gb, udata1));
	gboolean invrtOn         = gtk_toggle_button_get_active(invrt_w);

	if(invrtOn) cvNot(src, dst);
}


/**
 * @brief  Function to perform some specific operation. Canny edges!
 *
 * @param  src Source image to process
 * @param  dst Image to put the processed result
 * @param  gb  GtkBuilder pointer for the current GUI.
 * @param  udata1 gpointer for generic data for the operation. Normally the widget name. NULL forces no action.
 * @param  udata2 gpointer for generic data for the operation. Normally the widget name for auxiliary parameter. But can be other things
 */
void pari_UserOperation5(IplImage *src, IplImage *dst, GtkBuilder *gb, gpointer udata1, gpointer udata2)
{
	if( ! (gb && udata1) ) return;

	GtkToggleButton *canny_w = (GtkToggleButton *)GTK_WIDGET(gtk_builder_get_object(gb, udata1));
	gboolean cannyOn         = gtk_toggle_button_get_active(canny_w);

	if(cannyOn)  //must have a single layer image for the canny operation
	{
		IplImage *tmp_gray=cvCreateImage( cvSize(dst->width,dst->height), IPL_DEPTH_8U, 1);  //aux image
		cvCvtColor(dst, tmp_gray, CV_RGB2GRAY);
		cvCanny(tmp_gray, tmp_gray,0,30,3);
		cvMerge(tmp_gray, tmp_gray, tmp_gray, NULL, dst);
		cvReleaseImage(&tmp_gray);   //free aux image
		//cvAdd(src, dst, dst,NULL);   //to overlay image with someother image....
	}
}

/**
 * @brief  Function to perform some specific operation. Rotate image
 *
 * @param  src Source image to process
 * @param  dst Image to put the processed result
 * @param  gb  GtkBuilder pointer for the current GUI.
 * @param  udata1 gpointer for generic data for the operation. Normally the widget name. NULL forces no action.
 * @param  udata2 gpointer for generic data for the operation. Normally the widget name for auxiliary parameter. But can be other things
 */
void pari_UserOperation6(IplImage *src, IplImage *dst, GtkBuilder *gb, gpointer udata1, gpointer udata2)
{
	if( ! (gb && udata1 && udata2) ) return;

	GtkToggleButton *rotate_w = (GtkToggleButton *)GTK_WIDGET(gtk_builder_get_object(gb, udata1));
	GtkAdjustment *rotate_adj = GTK_ADJUSTMENT(gtk_builder_get_object(gb, udata2));
	gboolean         rotateon = gtk_toggle_button_get_active(rotate_w);
	gdouble            rotVal = gtk_adjustment_get_value(rotate_adj);

	if(rotateon)
	{
	        double x = src->width / 2, y = src->height / 2;
	        double ang = (rotVal - 50) * 2 * M_PI / 100;  //angle = +/- pi
	        double t[] = { 1, 0, -x,
	                       0, 1, -y,
	                       0, 0, 1
	                    }; //translation
	
	        double r[] = { cos(ang), -sin(ang), x,
	                       sin(ang),  cos(ang), y,
	                       0,                0, 1
	                     };  //rotate
	        CvMat matr = cvMat(3, 3, CV_64FC1, r);  //3x3 matrix of 64FC1 for double and put r there!
	        // No cvReleaseMat is required because memory allocation is done at double a[]
	        //cvTransform(src, dest, &mat, NULL );
	        //cvWarpPerspective(src, dest, &mat, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS, cvScalarAll(0) );
	        CvMat matt = cvMat(3, 3, CV_64FC1, t);  // 3x3 matrix of 64FC1 for double and put t there!
	        CvMat *mat = cvCreateMat(3, 3, CV_64FC1);  //3x3 mat for aux. operations
	        cvMatMul(&matr, &matt, mat);               //concatenate transformations ( r x t )
	        cvWarpPerspective(src, dst, mat, CV_WARP_FILL_OUTLIERS, cvScalarAll(0)); // apply transf to image (at last!)
	        cvReleaseMat(&mat);   // free aux matrix
	}
}


#if GTKVER < 3

/**
 * @brief  The initializations
 *
 * create device for capturing; create space for images
 * Create color images to ease taks. If Gray is resquired
 * then all channels will be made similar
 *
 * @param  void
 * @return the CvCapture device
 */
CvCapture *pari_StartImageAcquisition()
{
        //CvCapture *capture = cvCaptureFromCAM(CV_CAP_ANY);   //Capture using any camera connected to your system
        CvCapture *capture = cvCaptureFromCAM(0);              //Capture using 1st camera: i.e., no. 0 
	if( capture)
	{
	IplImage *frame=cvQueryFrame(capture);                 //Update frame. Pointer does not change.
        src_imageG = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);//create image for color (8 bits, 3 channels)
        dst_imageG = cvClone(src_imageG);                       //create clone of source image
	}
	return capture;
}

/**
 * @brief  Function to acquire one image
 *
 *         Image is stored in dst_imageG
 * @param  arg usual in these functions
 * @return void* (the usual in these functions)
 */
void pari_PerformImageAcquisition(CvCapture *capture)
{
	static IplImage *frame;
	frame=cvQueryFrame(capture);                    //Update frame. Pointer does not change.
	//cvCvtColor(frame, src_imageG, CV_BGR2GRAY);     //Convert frame into gray...
        cvCopy(frame, src_imageG, NULL);              //... or alternatively copy frame as it is (preserve color)
        cvCopy(src_imageG, dst_imageG, NULL);           //copy src into dst and ready to process (admit similar image strucutre)
}

#endif

