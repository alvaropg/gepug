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

#include "br-ebook-model.h"

enum
{
	PROP_0,

	PROP_EPUBDOC,
	PROP_CHAPTERS,
	PROP_EPUBFILE,
	PROP_TITLE
};

struct _BrEBookModelPrivate
{
	GFile *epub_file;
	GEPubDoc *epub_doc;
	GList *chapters;
	gchar *title;
};

G_DEFINE_TYPE (BrEBookModel, br_ebook_model, G_TYPE_OBJECT);

static void
br_ebook_model_init (BrEBookModel *object)
{
	object->priv = G_TYPE_INSTANCE_GET_PRIVATE (object, BR_TYPE_EBOOK_MODEL, BrEBookModelPrivate);
}

static void
br_ebook_model_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (br_ebook_model_parent_class)->finalize (object);
}

static void
br_ebook_model_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (BR_IS_EBOOK_MODEL (object));

	switch (prop_id) {
		case PROP_EPUBDOC:
			BR_EBOOK_MODEL (object)->priv->epub_doc = GEPUB_DOC (g_value_get_object (value));
			break;
		case PROP_CHAPTERS:
			BR_EBOOK_MODEL (object)->priv->chapters = (GList *) g_value_get_pointer (value);
			break;
		case PROP_EPUBFILE:
			BR_EBOOK_MODEL (object)->priv->epub_file = g_value_dup_object (value);
			break;
		case PROP_TITLE:
			BR_EBOOK_MODEL (object)->priv->title = g_value_dup_string (value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
br_ebook_model_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (BR_IS_EBOOK_MODEL (object));

	switch (prop_id) {
		case PROP_EPUBDOC:
			g_value_set_object (value, BR_EBOOK_MODEL (object)->priv->epub_doc);
			break;
		case PROP_CHAPTERS:
			g_value_set_pointer (value, BR_EBOOK_MODEL (object)->priv->chapters); 
			break;
		case PROP_EPUBFILE:
			g_value_set_object (value, BR_EBOOK_MODEL (object)->priv->epub_file);
			break;
		case PROP_TITLE:
			g_value_set_string (value, g_strdup (BR_EBOOK_MODEL (object)->priv->title));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
br_ebook_model_class_init (BrEBookModelClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	GObjectClass* parent_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (BrEBookModelPrivate));
	
	object_class->finalize = br_ebook_model_finalize;
	object_class->set_property = br_ebook_model_set_property;
	object_class->get_property = br_ebook_model_get_property;

	g_object_class_install_property (object_class,
	                                 PROP_EPUBDOC,
	                                 g_param_spec_object ("epub-doc",
	                                                      "epub-doc",
	                                                      "Link to the GEPubDoc",
	                                                      GEPUB_TYPE_DOC,
	                                                      G_PARAM_READABLE | G_PARAM_WRITABLE));

	g_object_class_install_property (object_class,
	                                 PROP_CHAPTERS,
	                                 g_param_spec_pointer ("chapters",
	                                                       "chapters",
	                                                       "EPub list of chapters",
	                                                       G_PARAM_READABLE | G_PARAM_WRITABLE));

	g_object_class_install_property (object_class,
	                                 PROP_EPUBFILE,
	                                 g_param_spec_object ("epub-file",
	                                                      "epub-file",
	                                                      "GFile for the EPub",
	                                                      G_TYPE_FILE,
	                                                      G_PARAM_READWRITE));

	g_object_class_install_property (object_class,
	                                 PROP_TITLE,
	                                 g_param_spec_string ("title",
	                                                      "title",
	                                                      "EPub title",
	                                                      NULL,
	                                                      G_PARAM_READABLE | G_PARAM_WRITABLE));
}

BrEBookModel*
br_ebook_model_new_from_file (GFile *file)
{
	BrEBookModel *model;
	
	g_return_val_if_fail (G_IS_FILE (file), NULL);
	
	model = g_object_new (BR_TYPE_EBOOK_MODEL, "epub-file", file, NULL);

	return model;
}

void
br_ebook_model_open (BrEBookModel *model)
{
	gchar *path;
	gchar *title;

	g_return_if_fail (BR_IS_EBOOK_MODEL (model));
	
	if (g_file_query_exists (model->priv->epub_file, NULL)) {
		path = g_file_get_path (model->priv->epub_file);
		model->priv->epub_doc = gepub_doc_new (path);

		title = gepub_doc_get_metadata (model->priv->epub_doc, META_TITLE);
		g_object_set (G_OBJECT (model), "title", title);
	}
}





