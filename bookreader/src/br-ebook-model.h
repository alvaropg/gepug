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

#ifndef _BR_EBOOK_MODEL_H_
#define _BR_EBOOK_MODEL_H_

#include <glib-object.h>
#include <gepub.h>

G_BEGIN_DECLS

#define BR_TYPE_EBOOK_MODEL             (br_ebook_model_get_type ())
#define BR_EBOOK_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), BR_TYPE_EBOOK_MODEL, BrEBookModel))
#define BR_EBOOK_MODEL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), BR_TYPE_EBOOK_MODEL, BrEBookModelClass))
#define BR_IS_EBOOK_MODEL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BR_TYPE_EBOOK_MODEL))
#define BR_IS_EBOOK_MODEL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), BR_TYPE_EBOOK_MODEL))
#define BR_EBOOK_MODEL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), BR_TYPE_EBOOK_MODEL, BrEBookModelClass))

typedef struct _BrEBookModelClass BrEBookModelClass;
typedef struct _BrEBookModel BrEBookModel;
typedef struct _BrEBookModelPrivate BrEBookModelPrivate;

struct _BrEBookModelClass
{
	GObjectClass parent_class;
};

struct _BrEBookModel
{
	GObject parent_instance;
	BrEBookModelPrivate *priv;
};

GType br_ebook_model_get_type (void) G_GNUC_CONST;
BrEBookModel* br_ebook_model_new_from_file (GFile *file);
void br_ebook_model_open (BrEBookModel *model);

G_END_DECLS

#endif /* _BR_EBOOK_MODEL_H_ */
