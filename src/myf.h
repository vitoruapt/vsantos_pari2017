/**
 *       @file  myf.h
 *      @brief  Header para projeto
 *
 *     @author  Vitor Santos, vitor@ua.pt
 *
 * ===========================================================
 */
#ifndef _MYF_H
#define _MYF_H


#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <gtk/gtk.h>
//#include <gdk-pixbuf/gdk-pixbuf.h>  //apparently not needed


//#include "opencv2/highgui/highgui_c.h"
//#include "opencv2/imgproc/imgproc_c.h"

#include <cv.h>
#if GTKVER < 3
#	include <highgui.h>
#endif

#define MYGLADEFILE "mydr2.glade"

#if defined (_MAIN_C_)
	GtkBuilder *builderG;
	IplImage *dst_imageG , *src_imageG;
	gboolean stretchOnG=FALSE;
#if GTKVER < 3
	CvCapture *captureG;
#endif
#else
	extern GtkBuilder *builderG;
	extern IplImage *dst_imageG , *src_imageG;
	extern gboolean stretchOnG;
#if GTKVER < 3
	extern CvCapture *captureG;
#endif
#endif


/* prototypes */
// Execute the following line (without comments to put list of functions here.
// or include a generated file using that command
// :r !ctags -x -R --c-kinds=f *.c | grep -v main|awk '{for(n=5;n<NF; n++) printf("\%s ", $n); printf("\%s;\n", $NF)}'

#include "prototypes.h"
void pari_UserOperation7(IplImage *src, IplImage *dst, GtkBuilder *gb, gpointer udata1, gpointer udata2);


#endif /* _MYF_H */
