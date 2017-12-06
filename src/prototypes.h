/*File generated automatically in diplodoco2 by vitor on Sáb Dez  2 12:26:55 WET 2017*/
#ifdef __cplusplus
extern "C" {
#endif
/* callbacks.c */
void on_button1_clicked(GtkWidget * widget, gpointer user_data);
gboolean on_drawingarea1_expose_event(GtkWidget * widget, GdkEvent * event, gpointer user_data);
void pari_InterceptCTRL_C(int a);
gboolean pari_UpdateImageAreas(gpointer data);
gboolean pari_delete_event(GtkWidget * window, GdkEvent * event, gpointer data);
void pari_destroy(GtkWidget * window, gpointer data);
/* main.c */
int main(int argc, char *argv[]);
/* myf.c */
GdkPixbuf *pari_ConvertOpenCv2Gtk(IplImage * image, int dst_w, int dst_h, gboolean stretch);
void pari_PerformImageAcquisition(CvCapture * capture);
void pari_ProcessUserOperations(IplImage * src, IplImage * dst);
void pari_RefreshDrawingArea(char *widgetName, IplImage * img);
CvCapture *pari_StartImageAcquisition();
void pari_UserOperation1(IplImage * src, IplImage * dst, GtkBuilder * gb, gpointer udata1, gpointer udata2);
void pari_UserOperation2(IplImage * src, IplImage * dst, GtkBuilder * gb, gpointer udata1, gpointer udata2);
void pari_UserOperation3(IplImage * src, IplImage * dst, GtkBuilder * gb, gpointer udata1, gpointer udata2);
void pari_UserOperation4(IplImage * src, IplImage * dst, GtkBuilder * gb, gpointer udata1, gpointer udata2);
void pari_UserOperation5(IplImage * src, IplImage * dst, GtkBuilder * gb, gpointer udata1, gpointer udata2);
void pari_UserOperation6(IplImage * src, IplImage * dst, GtkBuilder * gb, gpointer udata1, gpointer udata2);
#ifdef __cplusplus
}
#endif
