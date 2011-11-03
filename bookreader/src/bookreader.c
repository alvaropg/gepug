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

#include <glib/gi18n.h>
#include <gepub.h>

/* For testing propose use the local (not installed) ui file */
/* #define UI_FILE PACKAGE_DATA_DIR"/bookreader/ui/bookreader.ui" */
#define UI_FILE "src/bookreader.ui"

G_DEFINE_TYPE (Bookreader, bookreader, GTK_TYPE_APPLICATION);

/* Create a new window loading a file */
static void
bookreader_new_window (GApplication *app,
					   GFile        *file)
{
	GtkWidget *window;
	GtkTextBuffer *buffer;
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
	buffer = GTK_TEXT_BUFFER (gtk_builder_get_object (builder, "current_page_buffer"));
	g_object_unref (builder);
	
	gtk_window_set_application (GTK_WINDOW (window), GTK_APPLICATION (app));
	if (file != NULL) {
		const gchar *file_path;
		GEPubDoc *epub;
		gchar *content;

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
		
		gtk_text_buffer_set_text (buffer, content, -1);
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
