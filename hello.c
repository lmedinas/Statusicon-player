/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * main.c
 * Copyright (C) Luis Medinas 2010 <lmedinas@gmail.com>
 * 
 * statusicon-player is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * statusicon-player is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>
#include <gst/gst.h>

static 	GstElement *pipeline;

int 
quit_cb(GtkWidget *widget, gpointer data) 
{
	gtk_main_quit();
	return 0;
}

void
about_cb(GtkWidget *widget, gpointer data) 
{
    	GtkWidget *about;
	const gchar *authors[] = {"Luis Medinas <lmedinas@gmail.com\n", NULL};
    
	about = gtk_about_dialog_new();
	gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(about), "StatusIcon Player");
	gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG(about), authors);
	gtk_about_dialog_set_copyright (GTK_ABOUT_DIALOG(about), "Copyright 2010 (c) Luis Medinas\n");
	gtk_about_dialog_set_license (GTK_ABOUT_DIALOG(about), "GPLv2");
	gtk_about_dialog_set_version (GTK_ABOUT_DIALOG(about), "0.1");
	gtk_about_dialog_set_website (GTK_ABOUT_DIALOG(about), "http://gitorious.org/statusicon-player/statusicon-player");
	gtk_dialog_run(GTK_DIALOG (about));
	gtk_widget_destroy(about);
}
void 
run_gst(gchar *filename) 
{
		
       	gchar *uri;
	
       	uri = g_strdup_printf("file://%s", filename);
	pipeline = gst_element_factory_make ("playbin", "player");
       	g_print(uri);
       	g_object_set (G_OBJECT (pipeline), "uri", uri, NULL);
       	gst_element_set_state (GST_ELEMENT (pipeline), GST_STATE_PLAYING);
}

void
stop_gst() 
{
       	if (pipeline) {
      		gst_element_set_state(pipeline, GST_STATE_NULL);
       		gst_object_unref(GST_OBJECT(pipeline));
       		pipeline = NULL;
	}
}
void 
run_mplayer(gchar *filename) 
{
	gchar *command;

	command = g_strdup_printf("mplayer %s", filename);
	g_spawn_command_line_async(command, NULL);
}

void 
fileselector(GtkWidget *widget, gpointer data) {
    	     GtkWidget *dialog;
	     gchar *filename;

	dialog = gtk_file_chooser_dialog_new("Run file",
	                                     NULL,
	                                     GTK_FILE_CHOOSER_ACTION_OPEN,
	                                     GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	                                     GTK_STOCK_OPEN, GTK_RESPONSE_OK);
	gint result = gtk_dialog_run (GTK_DIALOG(dialog));
	if (result == GTK_RESPONSE_OK) {
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		g_print("O file foi: %s\n", filename);
		run_gst(filename);
	}
	gtk_widget_destroy (dialog);

}

void 
popup (GtkStatusIcon *statusicon,
       guint button,
       guint activate_time,
       gpointer user_data) 
{
     	GtkWidget *menu, *item;

      	menu = gtk_menu_new();
       	item = gtk_image_menu_item_new_from_stock(GTK_STOCK_MEDIA_PLAY, NULL);
       	gtk_menu_append(menu, item);
      	g_signal_connect(G_OBJECT(item), "activate",
      	                 G_CALLBACK(fileselector),
	                 NULL);
      	item = gtk_image_menu_item_new_from_stock(GTK_STOCK_MEDIA_STOP, NULL);
   	gtk_menu_append(menu, item);
	g_signal_connect(G_OBJECT(item), "activate",
	                 G_CALLBACK(stop_gst),
	                 NULL);

    	item = gtk_image_menu_item_new_from_stock(GTK_STOCK_ABOUT, NULL);
	gtk_menu_append(menu, item);
      	g_signal_connect(G_OBJECT(item), "activate",
	                 G_CALLBACK(about_cb),
       	                 NULL);
	
	item = gtk_separator_menu_item_new();
	gtk_menu_append(menu,item);
					
     	item = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);
 	gtk_menu_append(menu, item);
      	g_signal_connect (G_OBJECT(item),
	                  "activate",
	                  G_CALLBACK(quit_cb),
                          NULL);


     	gtk_widget_show_all(menu);

 	gtk_menu_popup (GTK_MENU (menu),
	                NULL,
	                NULL,
                        gtk_status_icon_position_menu,
	                statusicon,
	                button,
	                activate_time);
}

int 
main (int argc, char *argv[]) 
{

	GtkStatusIcon *statusicon;

	gtk_init( &argc, &argv);
	gst_init( &argc, &argv);
		
	statusicon =  gtk_status_icon_new_from_stock (GTK_STOCK_ABOUT);


	gtk_status_icon_set_tooltip(statusicon, "StatusIcon Player");
	gtk_status_icon_set_visible(statusicon, TRUE);

	g_signal_connect (G_OBJECT (statusicon), "popup-menu",
	                  G_CALLBACK (popup), NULL);

	gtk_main();

}
