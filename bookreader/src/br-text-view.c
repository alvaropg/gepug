/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * bookreader
 * Copyright (C) Álvaro Peña 2011 <alvaropg@gmail.com>
 * 
 * bookreader is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * bookreader is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>
#include <pango/pango.h>

#include "br-text-view.h"

G_DEFINE_TYPE (BrTextView, br_text_view, GTK_TYPE_DRAWING_AREA);

static void br_text_view_finalize (GObject *object);
static gint br_text_view_draw (GtkWidget *widget, cairo_t *cr);
static void br_text_view_get_size (GtkWidget *widget, GtkOrientation orientation, gint *minimum_size, gint *natural_size);
static void br_text_view_get_preferred_width (GtkWidget *widget, gint *minimum_size, gint *natural_size);
static void br_text_view_get_preferred_height (GtkWidget *widget, gint *minimum_size, gint *natural_size);

struct _BrTextViewPrivate {
	PangoLayout *pango_layout;
	gchar *text;
};

static void
br_text_view_class_init (BrTextViewClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	GtkWidgetClass* widget_class = GTK_WIDGET_CLASS (klass);

	g_type_class_add_private (klass, sizeof (BrTextViewPrivate));
	
	object_class->finalize = br_text_view_finalize;
	widget_class->draw = br_text_view_draw;
	widget_class->get_preferred_width  = br_text_view_get_preferred_width;
	widget_class->get_preferred_height = br_text_view_get_preferred_height;
}

static void
br_text_view_init (BrTextView *self)
{
	self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self, BR_TYPE_TEXT_VIEW, BrTextViewPrivate);

	self->priv->pango_layout = gtk_widget_create_pango_layout (GTK_WIDGET (self), self->priv->text);
	pango_layout_set_width (self->priv->pango_layout, 380 * PANGO_SCALE);
	pango_layout_set_height (self->priv->pango_layout, 390 * PANGO_SCALE);
	pango_layout_set_justify (self->priv->pango_layout, TRUE);
	pango_layout_set_wrap (self->priv->pango_layout, PANGO_WRAP_WORD_CHAR);
}

static void
br_text_view_finalize (GObject *object)
{
	G_OBJECT_CLASS (br_text_view_parent_class)->finalize (object);
}

static gint
br_text_view_draw (GtkWidget *widget,
				   cairo_t   *cr)
{
	BrTextViewPrivate *priv = BR_TEXT_VIEW (widget)->priv;
	GtkStyleContext *context;
	
	context = gtk_widget_get_style_context (widget);
	gtk_render_layout (context, cr,
					   0, 0,
					   priv->pango_layout);

	return FALSE;
}

static void
br_text_view_get_size (GtkWidget *widget, GtkOrientation orientation, gint *minimum_size, gint *natural_size)
{
	BrTextViewPrivate *priv = BR_TEXT_VIEW (widget)->priv;
	PangoRectangle required_rect;
	gint minimum, natural;
	
	pango_layout_get_pixel_extents (priv->pango_layout, NULL, &required_rect);
	
	if (orientation == GTK_ORIENTATION_HORIZONTAL) {
		minimum = required_rect.width;
		natural = minimum;
	} else {
		minimum = required_rect.height;
		natural = minimum;
	}

	*minimum_size = minimum;
	*natural_size = natural;
}

static void
br_text_view_get_preferred_width (GtkWidget *widget, gint *minimum_size, gint *natural_size)
{
	br_text_view_get_size (widget, GTK_ORIENTATION_HORIZONTAL, minimum_size, natural_size);
}

static void 
br_text_view_get_preferred_height (GtkWidget *widget, gint *minimum_size, gint *natural_size)
{
	br_text_view_get_size (widget, GTK_ORIENTATION_VERTICAL, minimum_size, natural_size);
}

GtkWidget*
br_text_view_new (void)
{
	return g_object_new (BR_TYPE_TEXT_VIEW, NULL);
}

gint
br_text_view_set_text (BrTextView *view, gchar *text)
{
	gint num_chars = 0;
	PangoLayoutIter *iter;
	
	view->priv->text = g_strdup (text);

	pango_layout_set_text (view->priv->pango_layout, view->priv->text, -1);
	iter = pango_layout_get_iter (view->priv->pango_layout);
	while (pango_layout_iter_next_char (iter)) {
		num_chars++;
	}
	
	return num_chars;
}
