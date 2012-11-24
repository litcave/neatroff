#define SC_IN		(dev_res)
#define SC_PT		(SC_IN / 72)

#define FNLEN		32
#define FNGLYPHS	512
#define FNNAME		32
#define LLEN		128
#define LINELEN		1024


/* number registers */
extern int nreg[];
int num_get(int id);
int num_set(int id, int n);
int tr_int(char *s, int orig, int unit);

/* string registers */
void str_set(int id, char *s);
char *str_get(int id);

/* builtin number registers; n_X for .X register */
#define REG(c1, c2)	((c1) * 256 + (c2))
#define n_f		nreg[REG('.', 'f')]
#define n_s		nreg[REG('.', 's')]
#define n_o		nreg[REG('.', 'o')]
#define n_p		nreg[REG('.', 'p')]
#define n_l		nreg[REG('.', 'l')]
#define n_v		nreg[REG('.', 'v')]
#define n_i		nreg[REG('.', 'i')]
#define n_f0		nreg[REG('\0', 'f')]	/* last font */
#define n_s0		nreg[REG('\0', 's')]	/* last size */
#define n_ad		nreg[REG('\0', 'a')]	/* adjustment */

/* device related variables */
extern int dev_res;
extern int dev_uwid;
extern int dev_hor;
extern int dev_ver;

struct glyph {
	char name[FNNAME];	/* name of the glyph */
	char id[FNNAME];	/* device-dependent glyph identifier */
	struct font *font;	/* glyph font */
	int wid;		/* character width */
	int type;		/* character type; ascender/descender */
};

struct font {
	char name[FNLEN];
	struct glyph glyphs[FNGLYPHS];
	int nglyphs;
	int spacewid;
	int special;
	char c[FNGLYPHS][FNNAME];	/* character names in charset */
	struct glyph *g[FNGLYPHS];	/* character glyphs in charset */
	int n;				/* number of characters in charset */
};

/* output device functions */
int dev_open(char *path);
void dev_close(void);
int dev_mnt(int pos, char *id, char *name);
int dev_font(char *id);

/* font-related functions */
struct font *font_open(char *path);
void font_close(struct font *fn);
struct glyph *font_glyph(struct font *fn, char *id);
struct glyph *font_find(struct font *fn, char *name);

/* glyph handling functions */
struct glyph *dev_glyph(char *c, int fn);
struct glyph *dev_glyph_byid(char *id, int fn);
int dev_spacewid(void);

/* different layers of neatroff */
int in_next(void);
int cp_next(void);
int tr_next(void);
void in_push(char *s);
void in_back(int c);
void cp_back(int c);

/* rendering */
void render(void);
void out_put(char *s);

/* troff commands */
void tr_br(int argc, char **args);
void tr_sp(int argc, char **args);
void tr_nr(int argc, char **args);
void tr_ps(int argc, char **args);
void tr_ft(int argc, char **args);
void tr_fp(int argc, char **args);

/* helpers */
void errmsg(char *msg, ...);
int utf8len(int c);
int charwid(int wid, int sz);

/* variable length string buffer */
struct sbuf {
	char *s;
	int sz;
	int n;
};

void sbuf_init(struct sbuf *sbuf);
void sbuf_done(struct sbuf *sbuf);
char *sbuf_buf(struct sbuf *sbuf);
void sbuf_add(struct sbuf *sbuf, int c);
void sbuf_append(struct sbuf *sbuf, char *s);