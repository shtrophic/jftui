/* A recursive-descent parser generated by peg 0.1.18 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define YYRULECOUNT 8
#line 1 "src/cmd.leg"

#define YYSTYPE unsigned long
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "shared.h"
#include "menu.h"

////////// STATE MACHINE //////////
typedef enum jf_cmd_parser_state {
    // make sure to start from 0 so memset init works
    JF_CMD_VALIDATE_START = 0,
    JF_CMD_VALIDATE_ATOMS = 1,
    JF_CMD_VALIDATE_FOLDER = 2,
    JF_CMD_VALIDATE_FILTERS = 3,
    JF_CMD_VALIDATE_OK = 4,
    JF_CMD_SPECIAL = 5,
    JF_CMD_MARK_PLAYED = 6,
    JF_CMD_MARK_UNPLAYED = 7,
    JF_CMD_SUCCESS = 8,

    JF_CMD_FAIL_FOLDER = -1,
    JF_CMD_FAIL_SYNTAX = -2,
    JF_CMD_FAIL_DISPATCH = -3,
    JF_CMD_FAIL_SPECIAL = -4
} jf_cmd_parser_state;

#define JF_CMD_IS_FAIL(state)   ((state) < 0)
///////////////////////////////////


////////// YY_CTX //////////
// forward declaration wrt PEG generated code
typedef struct _yycontext yycontext;

#define YY_CTX_LOCAL
#define YY_CTX_MEMBERS          \
    jf_cmd_parser_state state;  \
    char *input;                \
    size_t read_input;
////////////////////////////


////////// INPUT PROCESSING //////////
#define YY_INPUT(ctx, buf, result, max_size)                        \
    {                                                               \
        size_t to_read = 0;                                         \
        while (to_read < max_size) {                                \
            if (ctx->input[ctx->read_input + to_read] == '\0') {    \
                break;                                              \
            }                                                       \
            to_read++;                                              \
        }                                                           \
        memcpy(buf, ctx->input + ctx->read_input, to_read);         \
        ctx->read_input += to_read;                                 \
        result = to_read;                                           \
    }
//////////////////////////////////////


////////// FUNCTION PROTOTYPES //////////
jf_cmd_parser_state yy_cmd_get_parser_state(const yycontext *ctx);

static void yy_cmd_filters_start(yycontext *ctx);
static void yy_cmd_digest_filter(yycontext *ctx, const enum jf_filter filter);
static void yy_cmd_digest(yycontext *ctx, const size_t n);
static void yy_cmd_digest_range(yycontext *ctx, size_t l, size_t r);
static void yy_cmd_finalize(yycontext *ctx, const bool parse_ok);
/////////////////////////////////////////

#ifndef YY_MALLOC
#define YY_MALLOC(C, N)		malloc(N)
#endif
#ifndef YY_REALLOC
#define YY_REALLOC(C, P, N)	realloc(P, N)
#endif
#ifndef YY_FREE
#define YY_FREE(C, P)		free(P)
#endif
#ifndef YY_LOCAL
#define YY_LOCAL(T)	static T
#endif
#ifndef YY_ACTION
#define YY_ACTION(T)	static T
#endif
#ifndef YY_RULE
#define YY_RULE(T)	static T
#endif
#ifndef YY_PARSE
#define YY_PARSE(T)	T
#endif
#ifndef YYPARSE
#define YYPARSE		yyparse
#endif
#ifndef YYPARSEFROM
#define YYPARSEFROM	yyparsefrom
#endif
#ifndef YYRELEASE
#define YYRELEASE	yyrelease
#endif
#ifndef YY_BEGIN
#define YY_BEGIN	( yy->__begin= yy->__pos, 1)
#endif
#ifndef YY_END
#define YY_END		( yy->__end= yy->__pos, 1)
#endif
#ifdef YY_DEBUG
# define yyprintf(args)	fprintf args
#else
# define yyprintf(args)
#endif
#ifndef YYSTYPE
#define YYSTYPE	int
#endif
#ifndef YY_STACK_SIZE
#define YY_STACK_SIZE 128
#endif

#ifndef YY_BUFFER_SIZE
#define YY_BUFFER_SIZE 1024
#endif

#ifndef YY_PART

typedef struct _yycontext yycontext;
typedef void (*yyaction)(yycontext *yy, char *yytext, int yyleng);
typedef struct _yythunk { int begin, end;  yyaction  action;  struct _yythunk *next; } yythunk;

struct _yycontext {
  char     *__buf;
  int       __buflen;
  int       __pos;
  int       __limit;
  char     *__text;
  int       __textlen;
  int       __begin;
  int       __end;
  int       __textmax;
  yythunk  *__thunks;
  int       __thunkslen;
  int       __thunkpos;
  YYSTYPE   __;
  YYSTYPE  *__val;
  YYSTYPE  *__vals;
  int       __valslen;
#ifdef YY_CTX_MEMBERS
  YY_CTX_MEMBERS
#endif
};

#ifdef YY_CTX_LOCAL
#define YY_CTX_PARAM_	yycontext *yyctx,
#define YY_CTX_PARAM	yycontext *yyctx
#define YY_CTX_ARG_	yyctx,
#define YY_CTX_ARG	yyctx
#ifndef YY_INPUT
#define YY_INPUT(yy, buf, result, max_size)		\
  {							\
    int yyc= getchar();					\
    result= (EOF == yyc) ? 0 : (*(buf)= yyc, 1);	\
    yyprintf((stderr, "<%c>", yyc));			\
  }
#endif
#else
#define YY_CTX_PARAM_
#define YY_CTX_PARAM
#define YY_CTX_ARG_
#define YY_CTX_ARG
yycontext _yyctx= { 0, 0 };
yycontext *yyctx= &_yyctx;
#ifndef YY_INPUT
#define YY_INPUT(buf, result, max_size)			\
  {							\
    int yyc= getchar();					\
    result= (EOF == yyc) ? 0 : (*(buf)= yyc, 1);	\
    yyprintf((stderr, "<%c>", yyc));			\
  }
#endif
#endif

YY_LOCAL(int) yyrefill(yycontext *yy)
{
  int yyn;
  while (yy->__buflen - yy->__pos < 512)
    {
      yy->__buflen *= 2;
      yy->__buf= (char *)YY_REALLOC(yy, yy->__buf, yy->__buflen);
    }
#ifdef YY_CTX_LOCAL
  YY_INPUT(yy, (yy->__buf + yy->__pos), yyn, (yy->__buflen - yy->__pos));
#else
  YY_INPUT((yy->__buf + yy->__pos), yyn, (yy->__buflen - yy->__pos));
#endif
  if (!yyn) return 0;
  yy->__limit += yyn;
  return 1;
}

YY_LOCAL(int) yymatchDot(yycontext *yy)
{
  if (yy->__pos >= yy->__limit && !yyrefill(yy)) return 0;
  ++yy->__pos;
  return 1;
}

YY_LOCAL(int) yymatchChar(yycontext *yy, int c)
{
  if (yy->__pos >= yy->__limit && !yyrefill(yy)) return 0;
  if ((unsigned char)yy->__buf[yy->__pos] == c)
    {
      ++yy->__pos;
      yyprintf((stderr, "  ok   yymatchChar(yy, %c) @ %s\n", c, yy->__buf+yy->__pos));
      return 1;
    }
  yyprintf((stderr, "  fail yymatchChar(yy, %c) @ %s\n", c, yy->__buf+yy->__pos));
  return 0;
}

YY_LOCAL(int) yymatchString(yycontext *yy, const char *s)
{
  int yysav= yy->__pos;
  while (*s)
    {
      if (yy->__pos >= yy->__limit && !yyrefill(yy)) return 0;
      if (yy->__buf[yy->__pos] != *s)
        {
          yy->__pos= yysav;
          return 0;
        }
      ++s;
      ++yy->__pos;
    }
  return 1;
}

YY_LOCAL(int) yymatchClass(yycontext *yy, unsigned char *bits)
{
  int c;
  if (yy->__pos >= yy->__limit && !yyrefill(yy)) return 0;
  c= (unsigned char)yy->__buf[yy->__pos];
  if (bits[c >> 3] & (1 << (c & 7)))
    {
      ++yy->__pos;
      yyprintf((stderr, "  ok   yymatchClass @ %s\n", yy->__buf+yy->__pos));
      return 1;
    }
  yyprintf((stderr, "  fail yymatchClass @ %s\n", yy->__buf+yy->__pos));
  return 0;
}

YY_LOCAL(void) yyDo(yycontext *yy, yyaction action, int begin, int end)
{
  while (yy->__thunkpos >= yy->__thunkslen)
    {
      yy->__thunkslen *= 2;
      yy->__thunks= (yythunk *)YY_REALLOC(yy, yy->__thunks, sizeof(yythunk) * yy->__thunkslen);
    }
  yy->__thunks[yy->__thunkpos].begin=  begin;
  yy->__thunks[yy->__thunkpos].end=    end;
  yy->__thunks[yy->__thunkpos].action= action;
  ++yy->__thunkpos;
}

YY_LOCAL(int) yyText(yycontext *yy, int begin, int end)
{
  int yyleng= end - begin;
  if (yyleng <= 0)
    yyleng= 0;
  else
    {
      while (yy->__textlen < (yyleng + 1))
	{
	  yy->__textlen *= 2;
	  yy->__text= (char *)YY_REALLOC(yy, yy->__text, yy->__textlen);
	}
      memcpy(yy->__text, yy->__buf + begin, yyleng);
    }
  yy->__text[yyleng]= '\0';
  return yyleng;
}

YY_LOCAL(void) yyDone(yycontext *yy)
{
  int pos;
  for (pos= 0;  pos < yy->__thunkpos;  ++pos)
    {
      yythunk *thunk= &yy->__thunks[pos];
      int yyleng= thunk->end ? yyText(yy, thunk->begin, thunk->end) : thunk->begin;
      yyprintf((stderr, "DO [%d] %p %s\n", pos, thunk->action, yy->__text));
      thunk->action(yy, yy->__text, yyleng);
    }
  yy->__thunkpos= 0;
}

YY_LOCAL(void) yyCommit(yycontext *yy)
{
  if ((yy->__limit -= yy->__pos))
    {
      memmove(yy->__buf, yy->__buf + yy->__pos, yy->__limit);
    }
  yy->__begin -= yy->__pos;
  yy->__end -= yy->__pos;
  yy->__pos= yy->__thunkpos= 0;
}

YY_LOCAL(int) yyAccept(yycontext *yy, int tp0)
{
  if (tp0)
    {
      fprintf(stderr, "accept denied at %d\n", tp0);
      return 0;
    }
  else
    {
      yyDone(yy);
      yyCommit(yy);
    }
  return 1;
}

YY_LOCAL(void) yyPush(yycontext *yy, char *text, int count)
{
  yy->__val += count;
  while (yy->__valslen <= yy->__val - yy->__vals)
    {
      long offset= yy->__val - yy->__vals;
      yy->__valslen *= 2;
      yy->__vals= (YYSTYPE *)YY_REALLOC(yy, yy->__vals, sizeof(YYSTYPE) * yy->__valslen);
      yy->__val= yy->__vals + offset;
    }
}
YY_LOCAL(void) yyPop(yycontext *yy, char *text, int count)   { yy->__val -= count; }
YY_LOCAL(void) yySet(yycontext *yy, char *text, int count)   { yy->__val[count]= yy->__; }

#endif /* YY_PART */

#define	YYACCEPT	yyAccept(yy, yythunkpos0)

YY_RULE(int) yy_num(yycontext *yy); /* 8 */
YY_RULE(int) yy_Atom(yycontext *yy); /* 7 */
YY_RULE(int) yy_Items(yycontext *yy); /* 6 */
YY_RULE(int) yy_Filter(yycontext *yy); /* 5 */
YY_RULE(int) yy_Selector(yycontext *yy); /* 4 */
YY_RULE(int) yy_Filters(yycontext *yy); /* 3 */
YY_RULE(int) yy_ws(yycontext *yy); /* 2 */
YY_RULE(int) yy_Start(yycontext *yy); /* 1 */

YY_ACTION(void) yy_1_num(yycontext *yy, char *yytext, int yyleng)
{
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_1_num\n"));
  {
#line 113
   __ = strtoul(yytext, NULL, 10); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
}
YY_ACTION(void) yy_2_Atom(yycontext *yy, char *yytext, int yyleng)
{
#define n yy->__val[-1]
#define r yy->__val[-2]
#define l yy->__val[-3]
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_2_Atom\n"));
  {
#line 111
   yy_cmd_digest(yy, n); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
#undef n
#undef r
#undef l
}
YY_ACTION(void) yy_1_Atom(yycontext *yy, char *yytext, int yyleng)
{
#define n yy->__val[-1]
#define r yy->__val[-2]
#define l yy->__val[-3]
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_1_Atom\n"));
  {
#line 110
   yy_cmd_digest_range(yy, l, r); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
#undef n
#undef r
#undef l
}
YY_ACTION(void) yy_1_Selector(yycontext *yy, char *yytext, int yyleng)
{
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_1_Selector\n"));
  {
#line 104
   yy_cmd_digest_range(yy, 1, jf_menu_child_count()); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
}
YY_ACTION(void) yy_6_Filter(yycontext *yy, char *yytext, int yyleng)
{
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_6_Filter\n"));
  {
#line 102
   yy_cmd_digest_filter(yy, JF_FILTER_DISLIKES); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
}
YY_ACTION(void) yy_5_Filter(yycontext *yy, char *yytext, int yyleng)
{
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_5_Filter\n"));
  {
#line 101
   yy_cmd_digest_filter(yy, JF_FILTER_LIKES); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
}
YY_ACTION(void) yy_4_Filter(yycontext *yy, char *yytext, int yyleng)
{
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_4_Filter\n"));
  {
#line 100
   yy_cmd_digest_filter(yy, JF_FILTER_FAVORITE); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
}
YY_ACTION(void) yy_3_Filter(yycontext *yy, char *yytext, int yyleng)
{
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_3_Filter\n"));
  {
#line 99
   yy_cmd_digest_filter(yy, JF_FILTER_RESUMABLE); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
}
YY_ACTION(void) yy_2_Filter(yycontext *yy, char *yytext, int yyleng)
{
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_2_Filter\n"));
  {
#line 98
   yy_cmd_digest_filter(yy, JF_FILTER_IS_UNPLAYED); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
}
YY_ACTION(void) yy_1_Filter(yycontext *yy, char *yytext, int yyleng)
{
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_1_Filter\n"));
  {
#line 97
   yy_cmd_digest_filter(yy, JF_FILTER_IS_PLAYED); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
}
YY_ACTION(void) yy_10_Start(yycontext *yy, char *yytext, int yyleng)
{
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_10_Start\n"));
  {
#line 92
   yy_cmd_finalize(yy, true); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
}
YY_ACTION(void) yy_9_Start(yycontext *yy, char *yytext, int yyleng)
{
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_9_Start\n"));
  {
#line 89
   yy->state = JF_CMD_MARK_UNPLAYED; ;
  }
#undef yythunkpos
#undef yypos
#undef yy
}
YY_ACTION(void) yy_8_Start(yycontext *yy, char *yytext, int yyleng)
{
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_8_Start\n"));
  {
#line 87
   yy->state = JF_CMD_MARK_PLAYED; ;
  }
#undef yythunkpos
#undef yypos
#undef yy
}
YY_ACTION(void) yy_7_Start(yycontext *yy, char *yytext, int yyleng)
{
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_7_Start\n"));
  {
#line 85
   yy_cmd_digest_filter(yy, JF_FILTER_NONE); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
}
YY_ACTION(void) yy_6_Start(yycontext *yy, char *yytext, int yyleng)
{
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_6_Start\n"));
  {
#line 84
   yy_cmd_filters_start(yy); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
}
YY_ACTION(void) yy_5_Start(yycontext *yy, char *yytext, int yyleng)
{
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_5_Start\n"));
  {
#line 83
   yy->state = JF_CMD_SPECIAL; jf_menu_quit(); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
}
YY_ACTION(void) yy_4_Start(yycontext *yy, char *yytext, int yyleng)
{
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_4_Start\n"));
  {
#line 82
   yy->state = JF_CMD_SPECIAL; jf_menu_search(yytext); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
}
YY_ACTION(void) yy_3_Start(yycontext *yy, char *yytext, int yyleng)
{
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_3_Start\n"));
  {
#line 79
   yy->state = JF_CMD_SPECIAL; jf_menu_clear(); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
}
YY_ACTION(void) yy_2_Start(yycontext *yy, char *yytext, int yyleng)
{
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_2_Start\n"));
  {
#line 78
   yy->state = JF_CMD_SPECIAL; jf_menu_help(); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
}
YY_ACTION(void) yy_1_Start(yycontext *yy, char *yytext, int yyleng)
{
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_1_Start\n"));
  {
#line 77
   yy->state = JF_CMD_SPECIAL; jf_menu_dotdot(); ;
  }
#undef yythunkpos
#undef yypos
#undef yy
}

YY_RULE(int) yy_num(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;
  yyprintf((stderr, "%s\n", "num"));  yyText(yy, yy->__begin, yy->__end);  {
#define yytext yy->__text
#define yyleng yy->__textlen
if (!(YY_BEGIN)) goto l1;
#undef yytext
#undef yyleng
  }  if (!yymatchClass(yy, (unsigned char *)"\000\000\000\000\000\000\377\003\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000")) goto l1;
  l2:;	
  {  int yypos3= yy->__pos, yythunkpos3= yy->__thunkpos;  if (!yymatchClass(yy, (unsigned char *)"\000\000\000\000\000\000\377\003\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000")) goto l3;  goto l2;
  l3:;	  yy->__pos= yypos3; yy->__thunkpos= yythunkpos3;
  }  yyText(yy, yy->__begin, yy->__end);  {
#define yytext yy->__text
#define yyleng yy->__textlen
if (!(YY_END)) goto l1;
#undef yytext
#undef yyleng
  }  yyDo(yy, yy_1_num, yy->__begin, yy->__end);
  yyprintf((stderr, "  ok   %s @ %s\n", "num", yy->__buf+yy->__pos));
  return 1;
  l1:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "num", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_Atom(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;  yyDo(yy, yyPush, 3, 0);
  yyprintf((stderr, "%s\n", "Atom"));
  {  int yypos5= yy->__pos, yythunkpos5= yy->__thunkpos;  if (!yy_num(yy)) goto l6;  yyDo(yy, yySet, -3, 0);
  l7:;	
  {  int yypos8= yy->__pos, yythunkpos8= yy->__thunkpos;  if (!yy_ws(yy)) goto l8;  goto l7;
  l8:;	  yy->__pos= yypos8; yy->__thunkpos= yythunkpos8;
  }  if (!yymatchChar(yy, '-')) goto l6;
  l9:;	
  {  int yypos10= yy->__pos, yythunkpos10= yy->__thunkpos;  if (!yy_ws(yy)) goto l10;  goto l9;
  l10:;	  yy->__pos= yypos10; yy->__thunkpos= yythunkpos10;
  }  if (!yy_num(yy)) goto l6;  yyDo(yy, yySet, -2, 0);  yyDo(yy, yy_1_Atom, yy->__begin, yy->__end);  goto l5;
  l6:;	  yy->__pos= yypos5; yy->__thunkpos= yythunkpos5;  if (!yy_num(yy)) goto l4;  yyDo(yy, yySet, -1, 0);  yyDo(yy, yy_2_Atom, yy->__begin, yy->__end);
  }
  l5:;	
  yyprintf((stderr, "  ok   %s @ %s\n", "Atom", yy->__buf+yy->__pos));  yyDo(yy, yyPop, 3, 0);
  return 1;
  l4:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "Atom", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_Items(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;
  yyprintf((stderr, "%s\n", "Items"));
  {  int yypos12= yy->__pos, yythunkpos12= yy->__thunkpos;  if (!yy_Atom(yy)) goto l13;
  l14:;	
  {  int yypos15= yy->__pos, yythunkpos15= yy->__thunkpos;  if (!yy_ws(yy)) goto l15;  goto l14;
  l15:;	  yy->__pos= yypos15; yy->__thunkpos= yythunkpos15;
  }  if (!yymatchChar(yy, ',')) goto l13;
  l16:;	
  {  int yypos17= yy->__pos, yythunkpos17= yy->__thunkpos;  if (!yy_ws(yy)) goto l17;  goto l16;
  l17:;	  yy->__pos= yypos17; yy->__thunkpos= yythunkpos17;
  }  if (!yy_Items(yy)) goto l13;  goto l12;
  l13:;	  yy->__pos= yypos12; yy->__thunkpos= yythunkpos12;  if (!yy_Atom(yy)) goto l11;
  }
  l12:;	
  yyprintf((stderr, "  ok   %s @ %s\n", "Items", yy->__buf+yy->__pos));
  return 1;
  l11:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "Items", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_Filter(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;
  yyprintf((stderr, "%s\n", "Filter"));
  {  int yypos19= yy->__pos, yythunkpos19= yy->__thunkpos;  if (!yymatchChar(yy, 'p')) goto l20;  yyDo(yy, yy_1_Filter, yy->__begin, yy->__end);  goto l19;
  l20:;	  yy->__pos= yypos19; yy->__thunkpos= yythunkpos19;  if (!yymatchChar(yy, 'u')) goto l21;  yyDo(yy, yy_2_Filter, yy->__begin, yy->__end);  goto l19;
  l21:;	  yy->__pos= yypos19; yy->__thunkpos= yythunkpos19;  if (!yymatchChar(yy, 'r')) goto l22;  yyDo(yy, yy_3_Filter, yy->__begin, yy->__end);  goto l19;
  l22:;	  yy->__pos= yypos19; yy->__thunkpos= yythunkpos19;  if (!yymatchChar(yy, 'f')) goto l23;  yyDo(yy, yy_4_Filter, yy->__begin, yy->__end);  goto l19;
  l23:;	  yy->__pos= yypos19; yy->__thunkpos= yythunkpos19;  if (!yymatchChar(yy, 'l')) goto l24;  yyDo(yy, yy_5_Filter, yy->__begin, yy->__end);  goto l19;
  l24:;	  yy->__pos= yypos19; yy->__thunkpos= yythunkpos19;  if (!yymatchChar(yy, 'd')) goto l18;  yyDo(yy, yy_6_Filter, yy->__begin, yy->__end);
  }
  l19:;	
  yyprintf((stderr, "  ok   %s @ %s\n", "Filter", yy->__buf+yy->__pos));
  return 1;
  l18:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "Filter", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_Selector(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;
  yyprintf((stderr, "%s\n", "Selector"));
  {  int yypos26= yy->__pos, yythunkpos26= yy->__thunkpos;  if (!yymatchChar(yy, '*')) goto l27;  yyDo(yy, yy_1_Selector, yy->__begin, yy->__end);  goto l26;
  l27:;	  yy->__pos= yypos26; yy->__thunkpos= yythunkpos26;  if (!yy_Items(yy)) goto l25;
  }
  l26:;	
  yyprintf((stderr, "  ok   %s @ %s\n", "Selector", yy->__buf+yy->__pos));
  return 1;
  l25:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "Selector", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_Filters(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;
  yyprintf((stderr, "%s\n", "Filters"));
  {  int yypos29= yy->__pos, yythunkpos29= yy->__thunkpos;  if (!yy_Filter(yy)) goto l30;
  l31:;	
  {  int yypos32= yy->__pos, yythunkpos32= yy->__thunkpos;  if (!yy_ws(yy)) goto l32;  goto l31;
  l32:;	  yy->__pos= yypos32; yy->__thunkpos= yythunkpos32;
  }  if (!yy_Filters(yy)) goto l30;  goto l29;
  l30:;	  yy->__pos= yypos29; yy->__thunkpos= yythunkpos29;  if (!yy_Filter(yy)) goto l28;
  }
  l29:;	
  yyprintf((stderr, "  ok   %s @ %s\n", "Filters", yy->__buf+yy->__pos));
  return 1;
  l28:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "Filters", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_ws(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;
  yyprintf((stderr, "%s\n", "ws"));  if (!yymatchClass(yy, (unsigned char *)"\000\002\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000")) goto l33;
  yyprintf((stderr, "  ok   %s @ %s\n", "ws", yy->__buf+yy->__pos));
  return 1;
  l33:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "ws", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_Start(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;
  yyprintf((stderr, "%s\n", "Start"));
  l37:;	
  {  int yypos38= yy->__pos, yythunkpos38= yy->__thunkpos;  if (!yy_ws(yy)) goto l38;  goto l37;
  l38:;	  yy->__pos= yypos38; yy->__thunkpos= yythunkpos38;
  }
  {  int yypos39= yy->__pos, yythunkpos39= yy->__thunkpos;  if (!yymatchString(yy, "..")) goto l40;  yyDo(yy, yy_1_Start, yy->__begin, yy->__end);  goto l39;
  l40:;	  yy->__pos= yypos39; yy->__thunkpos= yythunkpos39;
  {  int yypos42= yy->__pos, yythunkpos42= yy->__thunkpos;  if (!yymatchString(yy, "help")) goto l43;  goto l42;
  l43:;	  yy->__pos= yypos42; yy->__thunkpos= yythunkpos42;  if (!yymatchChar(yy, '?')) goto l41;
  }
  l42:;	  yyDo(yy, yy_2_Start, yy->__begin, yy->__end);  goto l39;
  l41:;	  yy->__pos= yypos39; yy->__thunkpos= yythunkpos39;  if (!yymatchChar(yy, 'h')) goto l44;  yyDo(yy, yy_3_Start, yy->__begin, yy->__end);  goto l39;
  l44:;	  yy->__pos= yypos39; yy->__thunkpos= yythunkpos39;  if (!yymatchChar(yy, 's')) goto l45;  if (!yy_ws(yy)) goto l45;
  l46:;	
  {  int yypos47= yy->__pos, yythunkpos47= yy->__thunkpos;  if (!yy_ws(yy)) goto l47;  goto l46;
  l47:;	  yy->__pos= yypos47; yy->__thunkpos= yythunkpos47;
  }  yyText(yy, yy->__begin, yy->__end);  {
#define yytext yy->__text
#define yyleng yy->__textlen
if (!(YY_BEGIN)) goto l45;
#undef yytext
#undef yyleng
  }  if (!yymatchDot(yy)) goto l45;
  l48:;	
  {  int yypos49= yy->__pos, yythunkpos49= yy->__thunkpos;  if (!yymatchDot(yy)) goto l49;  goto l48;
  l49:;	  yy->__pos= yypos49; yy->__thunkpos= yythunkpos49;
  }  yyText(yy, yy->__begin, yy->__end);  {
#define yytext yy->__text
#define yyleng yy->__textlen
if (!(YY_END)) goto l45;
#undef yytext
#undef yyleng
  }  yyDo(yy, yy_4_Start, yy->__begin, yy->__end);  goto l39;
  l45:;	  yy->__pos= yypos39; yy->__thunkpos= yythunkpos39;  if (!yymatchChar(yy, 'q')) goto l50;  yyDo(yy, yy_5_Start, yy->__begin, yy->__end);  goto l39;
  l50:;	  yy->__pos= yypos39; yy->__thunkpos= yythunkpos39;  if (!yymatchChar(yy, 'f')) goto l51;  yyDo(yy, yy_6_Start, yy->__begin, yy->__end);  if (!yy_ws(yy)) goto l51;
  l52:;	
  {  int yypos53= yy->__pos, yythunkpos53= yy->__thunkpos;  if (!yy_ws(yy)) goto l53;  goto l52;
  l53:;	  yy->__pos= yypos53; yy->__thunkpos= yythunkpos53;
  }
  {  int yypos54= yy->__pos, yythunkpos54= yy->__thunkpos;  if (!yymatchChar(yy, 'c')) goto l55;  yyDo(yy, yy_7_Start, yy->__begin, yy->__end);  goto l54;
  l55:;	  yy->__pos= yypos54; yy->__thunkpos= yythunkpos54;  if (!yy_Filters(yy)) goto l51;
  }
  l54:;	  goto l39;
  l51:;	  yy->__pos= yypos39; yy->__thunkpos= yythunkpos39;  if (!yymatchChar(yy, 'm')) goto l56;  if (!yy_ws(yy)) goto l56;
  l57:;	
  {  int yypos58= yy->__pos, yythunkpos58= yy->__thunkpos;  if (!yy_ws(yy)) goto l58;  goto l57;
  l58:;	  yy->__pos= yypos58; yy->__thunkpos= yythunkpos58;
  }  if (!yymatchChar(yy, 'p')) goto l56;  yyDo(yy, yy_8_Start, yy->__begin, yy->__end);  if (!yy_ws(yy)) goto l56;
  l59:;	
  {  int yypos60= yy->__pos, yythunkpos60= yy->__thunkpos;  if (!yy_ws(yy)) goto l60;  goto l59;
  l60:;	  yy->__pos= yypos60; yy->__thunkpos= yythunkpos60;
  }  if (!yy_Selector(yy)) goto l56;  goto l39;
  l56:;	  yy->__pos= yypos39; yy->__thunkpos= yythunkpos39;  if (!yymatchChar(yy, 'm')) goto l61;  if (!yy_ws(yy)) goto l61;
  l62:;	
  {  int yypos63= yy->__pos, yythunkpos63= yy->__thunkpos;  if (!yy_ws(yy)) goto l63;  goto l62;
  l63:;	  yy->__pos= yypos63; yy->__thunkpos= yythunkpos63;
  }  if (!yymatchChar(yy, 'u')) goto l61;  yyDo(yy, yy_9_Start, yy->__begin, yy->__end);  if (!yy_ws(yy)) goto l61;
  l64:;	
  {  int yypos65= yy->__pos, yythunkpos65= yy->__thunkpos;  if (!yy_ws(yy)) goto l65;  goto l64;
  l65:;	  yy->__pos= yypos65; yy->__thunkpos= yythunkpos65;
  }  if (!yy_Selector(yy)) goto l61;  goto l39;
  l61:;	  yy->__pos= yypos39; yy->__thunkpos= yythunkpos39;  if (!yy_Selector(yy)) goto l36;
  }
  l39:;	
  l66:;	
  {  int yypos67= yy->__pos, yythunkpos67= yy->__thunkpos;  if (!yy_ws(yy)) goto l67;  goto l66;
  l67:;	  yy->__pos= yypos67; yy->__thunkpos= yythunkpos67;
  }
  {  int yypos68= yy->__pos, yythunkpos68= yy->__thunkpos;  if (!yymatchDot(yy)) goto l68;  goto l36;
  l68:;	  yy->__pos= yypos68; yy->__thunkpos= yythunkpos68;
  }  goto l35;
  l36:;	  yyText(yy, yy->__begin, yy->__end);  {
#define yytext yy->__text
#define yyleng yy->__textlen
   yy_cmd_finalize(yy, false); ;
#undef yytext
#undef yyleng
  }  goto l34;
  l35:;	  yyDo(yy, yy_10_Start, yy->__begin, yy->__end);
  yyprintf((stderr, "  ok   %s @ %s\n", "Start", yy->__buf+yy->__pos));
  return 1;
  l34:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "Start", yy->__buf+yy->__pos));
  return 0;
}

#ifndef YY_PART

typedef int (*yyrule)(yycontext *yy);

YY_PARSE(int) YYPARSEFROM(YY_CTX_PARAM_ yyrule yystart)
{
  int yyok;
  if (!yyctx->__buflen)
    {
      yyctx->__buflen= YY_BUFFER_SIZE;
      yyctx->__buf= (char *)YY_MALLOC(yyctx, yyctx->__buflen);
      yyctx->__textlen= YY_BUFFER_SIZE;
      yyctx->__text= (char *)YY_MALLOC(yyctx, yyctx->__textlen);
      yyctx->__thunkslen= YY_STACK_SIZE;
      yyctx->__thunks= (yythunk *)YY_MALLOC(yyctx, sizeof(yythunk) * yyctx->__thunkslen);
      yyctx->__valslen= YY_STACK_SIZE;
      yyctx->__vals= (YYSTYPE *)YY_MALLOC(yyctx, sizeof(YYSTYPE) * yyctx->__valslen);
      yyctx->__begin= yyctx->__end= yyctx->__pos= yyctx->__limit= yyctx->__thunkpos= 0;
    }
  yyctx->__begin= yyctx->__end= yyctx->__pos;
  yyctx->__thunkpos= 0;
  yyctx->__val= yyctx->__vals;
  yyok= yystart(yyctx);
  if (yyok) yyDone(yyctx);
  yyCommit(yyctx);
  return yyok;
}

YY_PARSE(int) YYPARSE(YY_CTX_PARAM)
{
  return YYPARSEFROM(YY_CTX_ARG_ yy_Start);
}

YY_PARSE(yycontext *) YYRELEASE(yycontext *yyctx)
{
  if (yyctx->__buflen)
    {
      yyctx->__buflen= 0;
      YY_FREE(yyctx, yyctx->__buf);
      YY_FREE(yyctx, yyctx->__text);
      YY_FREE(yyctx, yyctx->__thunks);
      YY_FREE(yyctx, yyctx->__vals);
    }
  return yyctx;
}

#endif
#line 120 "src/cmd.leg"

jf_cmd_parser_state yy_cmd_get_parser_state(const yycontext *ctx)
{
    return ctx->state;
}


static void yy_cmd_filters_start(yycontext *ctx)
{
    switch (ctx->state) {
        case JF_CMD_VALIDATE_START:
            ctx->state = JF_CMD_VALIDATE_FILTERS;
            break;
        case JF_CMD_VALIDATE_OK:
            ctx->state = JF_CMD_SPECIAL;
            jf_menu_filters_clear();
            break;
        default:
            fprintf(stderr, "Error: yy_cmd_filters_start: unexpected state transition. This is a bug.\n"); 
            break;
    }
}


static void yy_cmd_digest_filter(yycontext *ctx, const enum jf_filter filter)
{
    if (ctx->state != JF_CMD_SPECIAL || filter == JF_FILTER_NONE) return;

    if (jf_menu_filters_add(filter) == false) {
        ctx->state = JF_CMD_FAIL_SPECIAL;
    }
}


static void yy_cmd_digest(yycontext *ctx, size_t n)
{
    jf_item_type item_type;

    // no-op on fail state
    if (JF_CMD_IS_FAIL(ctx->state)) {
        return;
    }

    // no-op if item does not exist (out of bounds)
    if ((item_type = jf_menu_child_get_type(n)) == JF_ITEM_TYPE_NONE) {
        return;
    }

    switch (ctx->state) {
        case JF_CMD_VALIDATE_START:
            if (JF_ITEM_TYPE_IS_FOLDER(item_type)) {
                ctx->state = JF_CMD_VALIDATE_FOLDER;
            } else {
                ctx->state = JF_CMD_VALIDATE_ATOMS;
            }
            break;
        case JF_CMD_VALIDATE_ATOMS:
            if (JF_ITEM_TYPE_IS_FOLDER(item_type)) {
                ctx->state = JF_CMD_FAIL_FOLDER;
            }
            break;
        case JF_CMD_VALIDATE_FOLDER:
            ctx->state = JF_CMD_FAIL_FOLDER;
            break;
        case JF_CMD_VALIDATE_OK:
            if (! jf_menu_child_dispatch(n)) {
                ctx->state = JF_CMD_FAIL_DISPATCH;
            }
            break;
        case JF_CMD_MARK_PLAYED:
            jf_menu_child_mark_played(n, JF_PLAYED_STATUS_YES);
            break;
        case JF_CMD_MARK_UNPLAYED:
            jf_menu_child_mark_played(n, JF_PLAYED_STATUS_NO);
            break;
        default:
            fprintf(stderr, "Error: yy_cmd_digest: unexpected state transition. This is a bug.\n"); 
            break;
    }
}


static void yy_cmd_digest_range(yycontext *ctx, size_t l, size_t r)
{
    // and now for our next trick: unsigned arithmetic!
    size_t step = l <= r ? 1 : (size_t)-1;
    l = jf_clamp_zu(l, 0, jf_menu_child_count()+1);
    r = jf_clamp_zu(r, 0, jf_menu_child_count()+1);
    while (true) {
        yy_cmd_digest(ctx, l);
        if (l == r) break;
        l += step;
    }
}


static void yy_cmd_finalize(yycontext *ctx, const bool parse_ok)
{
    if (parse_ok == false) {
        ctx->state = JF_CMD_FAIL_SYNTAX;
    } else {
        switch (ctx->state) {
            case JF_CMD_VALIDATE_ATOMS:
            case JF_CMD_VALIDATE_FOLDER:
            case JF_CMD_VALIDATE_FILTERS:
                ctx->read_input = 0;
                ctx->state = JF_CMD_VALIDATE_OK;
                break;
            case JF_CMD_MARK_PLAYED:
            case JF_CMD_MARK_UNPLAYED:
                jf_menu_item_mark_played_await_all();
                // no break
            case JF_CMD_VALIDATE_OK:
            case JF_CMD_SPECIAL:
            case JF_CMD_VALIDATE_START: // all items out of bounds
                ctx->state = JF_CMD_SUCCESS;
                break;
            case JF_CMD_FAIL_FOLDER:
            case JF_CMD_FAIL_SPECIAL:
                break;
            default:
                fprintf(stderr, "Error: yy_cmd_finalize: unexpected state transition. This is a bug.\n");   
        }
    }
}

