#include "common.h"

///Count of arguments from command line
int Argc;
///Arguments from Command Line
char** Argv;

///Pointer to the selected 3D file
char* THREE_D_FILE=NULL;
///Pointer to the first 2D file
char* TWO_D_FILE_1=NULL;
///Pointer to the second 2D file
char* TWO_D_FILE_2=NULL;
///Pointer to the third 2D file
char* TWO_D_FILE_3=NULL;


void view_object_pressed(GtkWidget *widget,gpointer data);
void view_projections_pressed(GtkWidget *widget,gpointer data);
void quitButtonClicked(GtkWidget *widget,gpointer data);
void view_projections_pressed(GtkWidget *widget,gpointer data);
void construct_wireframe(GtkWidget *widget,gpointer data);
void view_drawing_1(GtkWidget *widget,gpointer data);
void view_drawing_2(GtkWidget *widget,gpointer data);
void view_drawing_3(GtkWidget *widget,gpointer data);

static void construct_3D_object_pressed(GtkWidget *widget,gpointer data)
{
	///Calls function to render 3D drawings
  GtkWidget *select_2D_projections_window,*choose_file_button1,*choose_file_button2,*choose_file_button3,*load_button1,*load_button2,*load_button3,*grid;

  GtkWidget *view_button1,*view_button2,*view_button3,*construct_3D_object,*box,*label;

  select_2D_projections_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  gtk_window_set_title (GTK_WINDOW (select_2D_projections_window), "Input projections of the object");
  gtk_container_set_border_width (GTK_CONTAINER (select_2D_projections_window), 10);
  gtk_window_set_position(GTK_WINDOW(select_2D_projections_window), GTK_WIN_POS_NONE);
  gtk_window_set_default_size(GTK_WINDOW(select_2D_projections_window), 500, 200);

  gchar p[]="Select a Front/Back projection file";
  gchar q[]="Select a Top/Bottom projection file";
  gchar r[]="Select a Left/Right projection file";
  

  box=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);

  gtk_container_add (GTK_CONTAINER (select_2D_projections_window), box);

  grid= gtk_grid_new ();

  label = gtk_label_new("Select the projection files.\nLoad each file.\nProceed as desired.");

  gtk_label_set_justify(GTK_LABEL(label),GTK_JUSTIFY_FILL);

  gtk_box_pack_start(GTK_BOX(box),label,TRUE,0,0);

  choose_file_button1 = gtk_file_chooser_button_new (&p[0],GTK_FILE_CHOOSER_ACTION_OPEN);
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (choose_file_button1),"/home");
  gtk_grid_attach (GTK_GRID (grid), choose_file_button1, 1, 0, 1, 2);

  load_button1 = gtk_button_new_with_label ("Load file");
  g_signal_connect (load_button1, "clicked", G_CALLBACK (quitButtonClicked), NULL);
  gtk_grid_attach (GTK_GRID (grid), load_button1, 2, 0, 1, 2);

  gtk_box_pack_start(GTK_BOX(box),grid,TRUE,0,0);

  gtk_widget_show_all (select_2D_projections_window);

  gtk_main();

  TWO_D_FILE_1=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(choose_file_button1));

  view_button1 = gtk_button_new_with_label ("View file 1");
  g_signal_connect (view_button1, "clicked", G_CALLBACK (view_drawing_1), NULL);
  gtk_grid_attach (GTK_GRID (grid), view_button1, 3, 0, 1, 2);

  choose_file_button2 = gtk_file_chooser_button_new (&q[0],GTK_FILE_CHOOSER_ACTION_OPEN);
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (choose_file_button2),"/home");
  gtk_grid_attach (GTK_GRID (grid), choose_file_button2, 1, 2, 1, 2);

  load_button2 = gtk_button_new_with_label ("Load file");
  g_signal_connect (load_button2, "clicked", G_CALLBACK (quitButtonClicked), NULL);
  gtk_grid_attach (GTK_GRID (grid), load_button2, 2, 2, 1, 2);

  gtk_box_pack_start(GTK_BOX(box),grid,TRUE,0,0);

  gtk_widget_show_all (select_2D_projections_window);

  gtk_main();

  TWO_D_FILE_2=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(choose_file_button2));

  view_button2 = gtk_button_new_with_label ("View file 2");
  g_signal_connect (view_button2, "clicked", G_CALLBACK (view_drawing_2), NULL);
  gtk_grid_attach (GTK_GRID (grid), view_button2, 3, 2, 1, 2);

  choose_file_button3 = gtk_file_chooser_button_new (&r[0],GTK_FILE_CHOOSER_ACTION_OPEN);
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (choose_file_button3),"/home");
  gtk_grid_attach (GTK_GRID (grid), choose_file_button3, 1, 4, 1, 2);

  load_button3 = gtk_button_new_with_label ("Load file");
  g_signal_connect (load_button3,"clicked", G_CALLBACK (quitButtonClicked), NULL);
  gtk_grid_attach (GTK_GRID (grid), load_button3, 2, 4, 1, 2);

  gtk_box_pack_start(GTK_BOX(box),grid,TRUE,0,0);

  gtk_widget_show_all (select_2D_projections_window);

  gtk_main();

  TWO_D_FILE_3=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(choose_file_button3));

  view_button3 = gtk_button_new_with_label ("View file 3");
  g_signal_connect (view_button3, "clicked", G_CALLBACK (view_drawing_3), NULL);
  gtk_grid_attach (GTK_GRID (grid), view_button3, 3, 4, 1, 2);

  construct_3D_object = gtk_button_new_with_label ("construct wireframe");
  g_signal_connect (construct_3D_object, "clicked", G_CALLBACK (construct_wireframe), NULL);
  gtk_grid_attach (GTK_GRID (grid), construct_3D_object, 1, 6, 3, 4);

  gtk_box_pack_start(GTK_BOX(box),grid,TRUE,0,0);

  gtk_widget_show_all (select_2D_projections_window);

  
}
void view_drawing_1(GtkWidget *widget,gpointer data)
{
	///Calls the GL function which renders 2D drawing
   call_2D_view(Argc,&TWO_D_FILE_1);
}

void view_drawing_2(GtkWidget *widget,gpointer data)
{
	///Calls the GL function which renders 2D drawing
   call_2D_view(Argc,&TWO_D_FILE_2);
}

void view_drawing_3(GtkWidget *widget,gpointer data)
{
	///Calls the GL function which renders 2D drawing
   call_2D_view(Argc,&TWO_D_FILE_3);
}

void construct_wireframe(GtkWidget *widget,gpointer data)
{
	///Calls the GL function which renders the wireframe
  Argv[1]=TWO_D_FILE_1; 
  Argv[2]=TWO_D_FILE_2;
  Argv[3]=TWO_D_FILE_3;
  
  call_2D_t0_3D(Argc, Argv);
}

 void Take_projections_pressed(GtkWidget *widget,gpointer data)
{
	///Calls functions to render 2D drawings
  GtkWidget *file_select_3D_window,*choose_file_button,*view_object_button,*load_button,*view_projection_button,*grid_take_projection_button;
  GtkWidget *box,*label;

  file_select_3D_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  gtk_window_set_title (GTK_WINDOW (file_select_3D_window), "Input 3D object");
  gtk_container_set_border_width (GTK_CONTAINER (file_select_3D_window), 10);
  gtk_window_set_position(GTK_WINDOW(file_select_3D_window), GTK_WIN_POS_NONE);
  gtk_window_set_default_size(GTK_WINDOW(file_select_3D_window), 350, 100);

  gchar p[]="Select a file";

  box=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);

  gtk_container_add (GTK_CONTAINER (file_select_3D_window), box);

  grid_take_projection_button = gtk_grid_new ();

  label = gtk_label_new("Select a 3D file.\nLoad it.\nProceed as desired.");

  gtk_label_set_justify(GTK_LABEL(label),GTK_JUSTIFY_FILL);

  gtk_box_pack_start(GTK_BOX(box),label,TRUE,0,0);

  choose_file_button = gtk_file_chooser_button_new (&p[0],GTK_FILE_CHOOSER_ACTION_OPEN);
  gtk_grid_attach (GTK_GRID (grid_take_projection_button), choose_file_button, 1, 0, 1, 2);
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (choose_file_button),"/home");

  load_button = gtk_button_new_with_label ("Load file");
  g_signal_connect (load_button, "clicked", G_CALLBACK (quitButtonClicked), NULL);
  gtk_grid_attach (GTK_GRID (grid_take_projection_button), load_button, 2, 0, 1, 2); 

  gtk_box_pack_start(GTK_BOX(box),grid_take_projection_button,TRUE,0,0);

  gtk_widget_show_all (file_select_3D_window);

  gtk_main();

  THREE_D_FILE=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(choose_file_button));

  view_object_button = gtk_button_new_with_label ("View 3D object");
  g_signal_connect (view_object_button, "clicked", G_CALLBACK (view_object_pressed), NULL);
  gtk_grid_attach (GTK_GRID (grid_take_projection_button), view_object_button, 1, 2, 2, 2);

  view_projection_button = gtk_button_new_with_label ("View projections of 3D object");
  g_signal_connect (view_projection_button, "clicked", G_CALLBACK (view_projections_pressed), NULL);
  gtk_grid_attach (GTK_GRID (grid_take_projection_button), view_projection_button, 1, 4, 2, 2);

  gtk_box_pack_start(GTK_BOX(box),grid_take_projection_button,TRUE,0,0);
  gtk_widget_show_all (file_select_3D_window);
}

void view_object_pressed(GtkWidget *widget,gpointer data)
{
	///Calls the GL function which renders the 3D object
    call_3D_view(Argc,&THREE_D_FILE);
}
void view_projections_pressed(GtkWidget *widget,gpointer data)
{
	///Calls the GL function which renders the projections
    call_3D_to_2D(Argc,&THREE_D_FILE);
}

void quitButtonClicked(GtkWidget *widget,gpointer data)
{
	///Exits the application
    gtk_main_quit();
}

gboolean windowDelete(GtkWidget *widget,GdkEvent *event,gpointer data)
{
  return TRUE;
}

int main(int argc,char **argv)
{
	///Main function of the whole programme
  GtkWidget *window,*box,*label;
  GtkWidget *grid,*button;

  Argc=argc;
  Argv=argv;

  gtk_init(&argc,&argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "CAD software");
  gtk_container_set_border_width (GTK_CONTAINER (window), 10);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 450, 100);

  box=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);

  gtk_container_add (GTK_CONTAINER (window), box);

  grid = gtk_grid_new ();

  label = gtk_label_new("Welcome to the TransformED Software.\n\n"\
  						"This software is capable of performing the following set of tasks:\n"\
  						"# View a 3D object\n"\
  						"# View a 2D object\n"\
  						"# Obtain Projections of 3D object\n"\
  						"# Reconstruct 3D wireframe model of an object from its projections.\n\n"
  						"To view an 3D object or to take its projection press the first button\n\n"
  						"To view a 2D drawing or to Reconstruct a 3D wireframe press the second button\n\n"
  						"To quit the Application use the third button\n");

  gtk_label_set_justify(GTK_LABEL(label),GTK_JUSTIFY_FILL);

  gtk_box_pack_start(GTK_BOX(box),label,TRUE,0,0);

  button = gtk_button_new_with_label ("Take Projections of a 3D object");
  g_signal_connect (button, "clicked", G_CALLBACK (Take_projections_pressed), NULL);
  gtk_grid_attach (GTK_GRID (grid), button, 1, 0, 1, 4);

  button = gtk_button_new_with_label ("Construct a 3D object");
  g_signal_connect (button, "clicked", G_CALLBACK (construct_3D_object_pressed), NULL);
  gtk_grid_attach (GTK_GRID (grid), button, 1, 6, 1, 4);

  button = gtk_button_new_with_label ("Quit Application");
  g_signal_connect (button, "clicked", G_CALLBACK (quitButtonClicked), NULL);
  g_signal_connect (window,"delete_event",G_CALLBACK(windowDelete),NULL);
  gtk_grid_attach (GTK_GRID (grid), button, 1, 12,1, 4);

  gtk_box_pack_start(GTK_BOX(box),grid,TRUE,0,0);
  gtk_widget_show_all (window);

  gtk_main();

  return 0;
  
}
