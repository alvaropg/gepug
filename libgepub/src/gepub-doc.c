/* gepub-doc.c
 *
 * Copyright (C) 2011 Daniel Garcia <danigm@wadobo.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <config.h>
#include <libxml/tree.h>
#include <string.h>

#include "gepub-utils.h"
#include "gepub-doc.h"
#include "gepub-archive.h"

#define IN 0
#define OUT 1

static void gepub_doc_finalize (GObject *object);
static void gepub_doc_fill_resources (GEPubDoc *doc);
static void gepub_doc_fill_spine (GEPubDoc *doc);
static gboolean equal_strs (gchar *one, gchar *two);

typedef struct _GEPubResource GEPubResource;

struct _GEPubResource {
        gchar *mime;
        gchar *uri;
};

void
gepub_resource_free (GEPubResource *res)
{
        g_free (res->mime);
        g_free (res->uri);
        g_free (res);
}

G_DEFINE_TYPE (GEPubDoc, gepub_doc, G_TYPE_OBJECT)

static void
gepub_doc_class_init (GEPubDocClass *klass)
{
        GObjectClass *object_class = G_OBJECT_CLASS (klass);
        
        object_class->finalize = gepub_doc_finalize;
}

static void
gepub_doc_init (GEPubDoc *doc)
{
}

static void
gepub_doc_finalize (GObject *object)
{
        GEPubDoc *doc = GEPUB_DOC (object);

        if (doc->archive) {
                g_object_unref (doc->archive);
                doc->archive = NULL;
        }
        if (doc->content) {
                g_free (doc->content);
                doc->content = NULL;
        }
        if (doc->content_base)
                g_free (doc->content_base);
        
        if (doc->resources) {
                g_hash_table_destroy (doc->resources);
                doc->resources = NULL;
        }
        
        if (doc->spine) {
                g_list_foreach (doc->spine, (GFunc)g_free, NULL);
                g_list_free (doc->spine);
                doc->spine = NULL;
        }
        
        G_OBJECT_CLASS (gepub_doc_parent_class)->finalize (object);
}

GEPubDoc *
gepub_doc_new (const gchar *path)
{
        GEPubDoc *doc;
        gchar *file;
        gint bufsize;
        gint i = 0, len;

        doc = GEPUB_DOC (g_object_new (GEPUB_TYPE_DOC, NULL));
        doc->archive = gepub_archive_new (path);

        file = gepub_archive_get_root_file (doc->archive);
        if (!file)
                return NULL;
        if (!gepub_archive_read_entry (doc->archive, file, &(doc->content), &bufsize))
                return NULL;

        len = strlen (file);
        while (file[i++] != '/' && i < len);
        // Parece que content base trata de guardar la carpeta
        // donde se encuentra el fichero content.opf o el que
        // venga especificado en el full-path del rootfile de
        // META-INF/container.xml
        if (i == len) {
                doc->content_base = g_strdup ("");
        } else {
                doc->content_base = g_strndup (file, i);
        }

        // doc resources hashtable:
        // id : (mime, path)
        doc->resources = g_hash_table_new_full (g_str_hash,
                                                (GEqualFunc) equal_strs,
                                                (GDestroyNotify) g_free,
                                                (GDestroyNotify) gepub_resource_free);
        gepub_doc_fill_resources (doc);
        doc->spine = NULL;
        gepub_doc_fill_spine (doc);

        if (file)
                g_free (file);
        
        return doc;
}

static gboolean
equal_strs (gchar *one, gchar *two)
{
        if (strcmp (one, two))
                return FALSE;
        
        return TRUE;
}

static void
gepub_doc_fill_resources (GEPubDoc *doc)
{
        xmlDoc *xdoc = NULL;
        xmlNode *root_element = NULL;
        xmlNode *mnode = NULL;
        xmlNode *item = NULL;
        gchar *id, *tmpuri, *uri;
        GEPubResource *res;

        LIBXML_TEST_VERSION

        xdoc = xmlRecoverDoc (doc->content);
        root_element = xmlDocGetRootElement (xdoc);
        mnode = gepub_utils_get_element_by_tag (root_element, "manifest");

        item = mnode->children;
        while (item) {
                if (item->type != XML_ELEMENT_NODE ) {
                        item = item->next;
                        continue;
                }

                id = xmlGetProp (item, "id");
                tmpuri = xmlGetProp (item, "href");
                uri = g_strdup_printf ("%s%s", doc->content_base, tmpuri);
                g_free (tmpuri);
                // TODO: can't use a g_new0 ???
                res = g_malloc (sizeof (GEPubResource));
                res->mime = xmlGetProp (item, "media-type");
                res->uri = uri;
                g_hash_table_insert (doc->resources, id, res);
                item = item->next;
        }
        
        xmlFreeDoc (xdoc);
        xmlCleanupParser ();
}

static void
gepub_doc_fill_spine (GEPubDoc *doc)
{
        xmlDoc *xdoc = NULL;
        xmlNode *root_element = NULL;
        xmlNode *snode = NULL;
        xmlNode *item = NULL;
        gchar *id;

        LIBXML_TEST_VERSION

        xdoc = xmlRecoverDoc (doc->content);
        root_element = xmlDocGetRootElement (xdoc);
        snode = gepub_utils_get_element_by_tag (root_element, "spine");

        item = snode->children;
        while (item) {
                if (item->type != XML_ELEMENT_NODE ) {
                        item = item->next;
                        continue;
                }

                id = xmlGetProp (item, "idref");

                doc->spine = g_list_append (doc->spine, id);
                item = item->next;
        }

        xmlFreeDoc (xdoc);
        xmlCleanupParser ();
}

gchar *
gepub_doc_get_content (GEPubDoc *doc)
{
        return doc->content;
}

gchar *
gepub_doc_get_metadata (GEPubDoc *doc, gchar *mdata)
{
        xmlDoc *xdoc = NULL;
        xmlNode *root_element = NULL;
        xmlNode *mnode = NULL;
        xmlNode *mdata_node = NULL;
        gchar *ret;
        xmlChar *text;

        LIBXML_TEST_VERSION

        xdoc = xmlRecoverDoc (doc->content);
        root_element = xmlDocGetRootElement (xdoc);
        mnode = gepub_utils_get_element_by_tag (root_element, "metadata");
        mdata_node = gepub_utils_get_element_by_tag (mnode, mdata);

        text = xmlNodeGetContent (mdata_node);
        ret = g_strdup (text);
        xmlFree (text);

        xmlFreeDoc (xdoc);
        xmlCleanupParser ();

        return ret;
}

GHashTable *
gepub_doc_get_resources (GEPubDoc *doc)
{
        return doc->resources;
}

guchar *
gepub_doc_get_resource (GEPubDoc *doc, gchar *id)
{
        guchar *res = NULL;
        gint bufsize = 0;
        GEPubResource *gres = g_hash_table_lookup (doc->resources, id);
        if (!gres) {
                // not found
                return NULL;
        }
        if (!gepub_archive_read_entry (doc->archive, gres->uri, &res, &bufsize))
                return NULL;

        return res;
}

guchar *
gepub_doc_get_resource_v (GEPubDoc *doc, gchar *v, gint *bufsize)
{
        guchar *res = NULL;
        gchar *path = NULL;

        path = g_strdup_printf ("%s%s", doc->content_base, v);
        if (!gepub_archive_read_entry (doc->archive, path, &res, bufsize)) {
                g_free (path);
                return NULL;
        }
        g_free (path);

        return res;
}

guchar *
gepub_doc_get_resource_mime (GEPubDoc *doc, gchar *v)
{
        guchar *res = NULL;
        gchar *path = NULL;
        GEPubResource *gres;
        gint bufsize = 0;
        GList *keys = g_hash_table_get_keys (doc->resources);

        path = g_strdup_printf ("%s%s", doc->content_base, v);

        while (keys) {
                gres = ((GEPubResource*)g_hash_table_lookup (doc->resources, keys->data));
                if (!strcmp (gres->uri, path))
                        break;
                keys = keys->next;
        }

        if (keys)
                return gres->mime;
        else
                return NULL;
}

GList *
gepub_doc_get_spine (GEPubDoc *doc)
{
        return doc->spine;
}

guchar *
gepub_doc_get_current (GEPubDoc *doc)
{
        return gepub_doc_get_resource (doc, doc->spine->data);
}

guchar *
gepub_doc_get_current_markup (GEPubDoc *doc)
{
        guchar *content, *body_content;
        xmlDoc *xdoc = NULL;
        xmlNode *body_node = NULL;
        xmlNode *html_node = NULL;

        content = gepub_doc_get_resource (doc, doc->spine->data);

        xdoc = xmlRecoverDoc (content);
        html_node = xmlDocGetRootElement (xdoc);
        body_node = gepub_utils_get_element_by_tag (html_node, "body");
        body_content = xmlNodeGetContent (body_node);

        return body_content;
}

void
gepub_doc_go_next (GEPubDoc *doc)
{
    if (doc->spine->next)
        doc->spine = doc->spine->next;
}

void
gepub_doc_go_prev (GEPubDoc *doc)
{
    if (doc->spine->prev)
        doc->spine = doc->spine->prev;
}
