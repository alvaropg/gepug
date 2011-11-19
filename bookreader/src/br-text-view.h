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

#ifndef _BR_TEXT_VIEW_H_
#define _BR_TEXT_VIEW_H_

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define BR_TYPE_TEXT_VIEW             (br_text_view_get_type ())
#define BR_TEXT_VIEW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), BR_TYPE_TEXT_VIEW, BrTextView))
#define BR_TEXT_VIEW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), BR_TYPE_TEXT_VIEW, BrTextViewClass))
#define BR_IS_TEXT_VIEW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BR_TYPE_TEXT_VIEW))
#define BR_IS_TEXT_VIEW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), BR_TYPE_TEXT_VIEW))
#define BR_TEXT_VIEW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), BR_TYPE_TEXT_VIEW, BrTextViewClass))

typedef struct _BrTextViewClass BrTextViewClass;
typedef struct _BrTextView BrTextView;
typedef struct _BrTextViewPrivate BrTextViewPrivate;

struct _BrTextViewClass
{
	GtkDrawingAreaClass parent_class;
};

struct _BrTextView
{
	GtkDrawingArea parent_instance;

	BrTextViewPrivate *priv;
};

GType br_text_view_get_type (void) G_GNUC_CONST;
GtkWidget* br_text_view_new (void) G_GNUC_CONST;
gint br_text_view_set_text (BrTextView *view, gchar *text);

G_END_DECLS

#endif /* _BR_TEXT_VIEW_H_ */
