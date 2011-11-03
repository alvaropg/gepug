/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * bookreader.h
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

#ifndef _BOOKREADER_
#define _BOOKREADER_

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define BOOKREADER_TYPE_APPLICATION             (bookreader_get_type ())
#define BOOKREADER_APPLICATION(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), BOOKREADER_TYPE_APPLICATION, Bookreader))
#define BOOKREADER_APPLICATION_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), BOOKREADER_TYPE_APPLICATION, BookreaderClass))
#define BOOKREADER_IS_APPLICATION(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BOOKREADER_TYPE_APPLICATION))
#define BOOKREADER_IS_APPLICATION_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), BOOKREADER_TYPE_APPLICATION))
#define BOOKREADER_APPLICATION_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), BOOKREADER_TYPE_APPLICATION, BookreaderClass))

typedef struct _BookreaderClass BookreaderClass;
typedef struct _Bookreader Bookreader;

struct _BookreaderClass
{
	GtkApplicationClass parent_class;
};

struct _Bookreader
{
	GtkApplication parent_instance;
};

GType bookreader_get_type (void) G_GNUC_CONST;
Bookreader *bookreader_new (void);

/* Callbacks */

G_END_DECLS

#endif /* _APPLICATION_H_ */
