/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * bookreader.c
 * Copyright (C) Álvaro Peña 2011 <alvaropg@gmail.com>
 * 
 * BookReader is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BookReader is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "bookreader.h"
#include "br-text-view.h"
#include "br-ebook-model.h"

#include <glib/gi18n.h>
#include <gtk/gtk.h>

/* For testing propose use the local (not installed) ui file */
/* #define UI_FILE PACKAGE_DATA_DIR"/bookreader/ui/bookreader.ui" */
#define UI_FILE "src/bookreader.ui"

G_DEFINE_TYPE (Bookreader, bookreader, GTK_TYPE_APPLICATION);

void on_next_clicked (GtkButton *button, gpointer data);

void
on_next_clicked (GtkButton *button, gpointer data)
{
	
}

/* Create a new window loading a file */
static void
bookreader_new_window (GApplication *app,
					   GFile        *file)
{
	GtkWidget *window, *main_box, *view;
	GtkBuilder *builder;
	GError* error = NULL;

	/* Load UI from file */
	builder = gtk_builder_new ();
	if (!gtk_builder_add_from_file (builder, UI_FILE, &error))
	{
		g_warning ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (builder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (builder, "window"));
	main_box = GTK_WIDGET (gtk_builder_get_object (builder, "main_box"));
	view = GTK_WIDGET (br_text_view_new ());
	gtk_box_pack_start (GTK_BOX (main_box), view, TRUE, TRUE, 0);
	g_object_unref (builder);
	
	gtk_window_set_application (GTK_WINDOW (window), GTK_APPLICATION (app));
	if (file != NULL) {
		BrEBookModel *model;
		gchar *title;
		
		model = br_ebook_model_new_from_file (file);
		br_ebook_model_open (model);
		g_object_get (G_OBJECT (model),	"title", &title, NULL);
		gtk_window_set_title (GTK_WINDOW (window), title);
		/*
		const gchar *file_path;
		GEPubDoc *epub;
		gchar *content;
		gchar *pag2;
		gint character;

		file_path = g_file_get_path (file);
		epub = gepub_doc_new (file_path);
		gepub_doc_go_next (epub);
		gepub_doc_go_next (epub);
		gepub_doc_go_next (epub);
		gepub_doc_go_next (epub);
		gepub_doc_go_next (epub);
		gepub_doc_go_next (epub);
		gepub_doc_go_next (epub);
		gepub_doc_go_next (epub);
		gepub_doc_go_next (epub);
		gepub_doc_go_next (epub);
		gepub_doc_go_next (epub);
		gepub_doc_go_next (epub);
		content = gepub_doc_get_current_markup (epub);
		character = br_text_view_set_text (BR_TEXT_VIEW (view), content);
		*/
		//g_print ("num: %d de %d\n", character, g_utf8_strlen (content, -1));
		//pag2 = g_utf8_substring (content, character, g_utf8_strlen (content, -1));
		//g_print ("Pag2: %s\n", pag2);
	}
	gtk_widget_show_all (GTK_WIDGET (window));
}


/* GApplication implementation */
static void
bookreader_activate (GApplication *application)
{
  bookreader_new_window (application, NULL);
}

static void
bookreader_open (GApplication  *application,
				 GFile        **files,
				 gint           n_files,
				 const gchar   *hint)
{
  gint i;

  for (i = 0; i < n_files; i++)
    bookreader_new_window (application, files[i]);
}

static void
bookreader_init (Bookreader *object)
{

}

static void
bookreader_finalize (GObject *object)
{

	G_OBJECT_CLASS (bookreader_parent_class)->finalize (object);
}

static void
bookreader_class_init (BookreaderClass *klass)
{
	G_APPLICATION_CLASS (klass)->activate = bookreader_activate;
	G_APPLICATION_CLASS (klass)->open = bookreader_open;

	G_OBJECT_CLASS (klass)->finalize = bookreader_finalize;
}

Bookreader *
bookreader_new (void)
{
	g_type_init ();

	return g_object_new (bookreader_get_type (),
	                     "application-id", "org.gnome.bookreader",
	                     "flags", G_APPLICATION_HANDLES_OPEN,
	                     NULL);
}
