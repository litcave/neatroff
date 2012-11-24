#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xroff.h"

static void skipline(FILE* filp)
{
	int c;
	do {
		c = getc(filp);
	} while (c != '\n' && c != EOF);
}

struct glyph *font_find(struct font *fn, char *name)
{
	int i;
	for (i = 0; i < fn->n; i++)
		if (!strcmp(name, fn->c[i]))
			return fn->g[i];
	return NULL;
}

struct glyph *font_glyph(struct font *fn, char *id)
{
	int i;
	for (i = 0; i < fn->nglyphs; i++)
		if (!strcmp(fn->glyphs[i].id, id))
			return &fn->glyphs[i];
	return NULL;
}

static struct glyph *font_glyphmk(struct font *fn, char *id)
{
	struct glyph *g = font_glyph(fn, id);
	if (g)
		return g;
	g = &fn->glyphs[fn->nglyphs++];
	strcpy(g->id, id);
	return g;
}

static void font_charset(struct font *fn, FILE *fin)
{
	char tok[LLEN];
	char name[LLEN];
	char id[LLEN];
	struct glyph *glyph = NULL;
	struct glyph *prev = NULL;
	int wid, type;
	while (1) {
		if (fscanf(fin, "%s", name) != 1)
			break;
		fscanf(fin, "%s", tok);
		glyph = prev;
		if (strcmp("\"", tok)) {
			wid = atoi(tok);
			fscanf(fin, "%d %s", &type, id);
			skipline(fin);
			glyph = font_glyphmk(fn, id);
			strcpy(glyph->name, name);
			glyph->wid = wid;
			glyph->type = type;
			glyph->font = fn;
		}
		prev = glyph;
		strcpy(fn->c[fn->n], name);
		fn->g[fn->n] = glyph;
		fn->n++;
	}
}

struct font *font_open(char *path)
{
	struct font *fn = malloc(sizeof(*fn));
	char tok[LLEN];
	FILE *fin;
	fin = fopen(path, "r");
	memset(fn, 0, sizeof(*fn));
	while (fscanf(fin, "%s", tok) == 1) {
		if (tok[0] == '#') {
			skipline(fin);
			continue;
		}
		if (!strcmp("spacewidth", tok)) {
			fscanf(fin, "%d", &fn->spacewid);
			continue;
		}
		if (!strcmp("special", tok)) {
			fn->special = 1;
			continue;
		}
		if (!strcmp("name", tok)) {
			fscanf(fin, "%s", fn->name);
			continue;
		}
		if (!strcmp("fontname", tok)) {
			skipline(fin);
			continue;
		}
		if (!strcmp("named", tok)) {
			skipline(fin);
			continue;
		}
		if (!strcmp("ligatures", tok)) {
			while (fscanf(fin, "%s", tok) == 1)
				if (!strcmp("0", tok))
					break;
			skipline(fin);
			continue;
		}
		if (!strcmp("charset", tok)) {
			font_charset(fn, fin);
			break;
		}
	}
	fclose(fin);
	return fn;
}

void font_close(struct font *fn)
{
	free(fn);
}