/* A recursive-descent parser generated by peg 0.1.18 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define YYRULECOUNT 8
#line 1 "src/command_grammar.leg"

	#define YYSTYPE unsigned long
	#include <stdlib.h>
	#include <stdbool.h>

	typedef char jf_zu_stack_state;

	static struct jf_zu_stack {
		size_t *stack;
		size_t size;
		size_t used;
		jf_zu_stack_state state;
	} s_zu_stack = {
		.stack = NULL
	};

	#define JF_ZU_STACK_SUCCESS		0
	#define JF_ZU_STACK_CLEAR		1
	#define JF_ZU_STACK_ATOMS		2
	#define JF_ZU_STACK_FOLDER		3
	#define JF_ZU_STACK_RECURSIVE	4
	#define JF_ZU_STACK_FAIL_FOLDER	-1
	#define JF_ZU_STACK_FAIL_MATCH	-2

	#define JF_ZU_STACK_IS_FAIL(state)	((state) < 0)


	jf_zu_stack_state jf_zu_stack_get_state(void);

	static void jf_zu_stack_init(void);
	static size_t jf_zu_stack_pop(void);
	static void jf_zu_stack_clear(void);
	static void jf_zu_stack_push(const size_t n);
	static void jf_zu_stack_push_range(size_t l, const size_t r);
	static void jf_zu_stack_finalize(const bool parse_ok);


	jf_zu_stack_state jf_zu_stack_get_state()
	{
		return s_zu_stack.state;
	}


	static void jf_zu_stack_init()
	{
		printf("DEBUG: init\n");
		if (s_zu_stack.stack == NULL) {
			s_zu_stack.stack = malloc(20 * sizeof(size_t));
			s_zu_stack.size = 20;
			s_zu_stack.used = 0;
		}
		s_zu_stack.state = JF_ZU_STACK_CLEAR;
	}


	static size_t jf_zu_stack_pop()
	{
		return s_zu_stack.used > 0 ? s_zu_stack.stack[--s_zu_stack.used] : 0;
	}


	static void jf_zu_stack_clear()
	{
		while (jf_zu_stack_pop() != 0) ;
	}


	static void jf_zu_stack_push(const size_t n)
	{
		bool is_folder;

		// no-op on fail state
		if (JF_ZU_STACK_IS_FAIL(s_zu_stack.state)) {
			printf("DEBUG: push noop for failstate\n");
			return;
		}

		is_folder = jf_menu_child_is_folder(n);

		// folder change commands can only target one folder and no other items
		// unless recursive
		if (is_folder && ! (s_zu_stack.state == JF_ZU_STACK_CLEAR
							|| s_zu_stack.state == JF_ZU_STACK_RECURSIVE)) {
			jf_zu_stack_clear();
			s_zu_stack.state = JF_ZU_STACK_FAIL_FOLDER;
			printf("DEBUG: set fail_folder\n");
			return;
		}

		// check space
		if (s_zu_stack.size == s_zu_stack.used) {
			size_t *tmp = realloc(s_zu_stack.stack, s_zu_stack.size * 2 * sizeof(size_t));
			if (tmp == NULL) {
				return;
			}
			s_zu_stack.stack = tmp;
		}

		// actual push
		s_zu_stack.stack[s_zu_stack.used++] = n;
		printf("%zu ", n);

		// check state, preserving recursiveness
		if (s_zu_stack.state != JF_ZU_STACK_RECURSIVE) {
			s_zu_stack.state = is_folder ? JF_ZU_STACK_FOLDER : JF_ZU_STACK_ATOMS;
		}
	}


	static void jf_zu_stack_push_range(size_t l, const size_t r)
	{
		int step = l <= r ? 1 : -1;
		jf_zu_stack_push(l);
		while (l != r) {
			l += step;
			jf_zu_stack_push(l);
		}
	}


	static void jf_zu_stack_finalize(const bool parse_ok)
	{
		size_t n;

		if (parse_ok == false) {
			printf("DEBUG: fail_match\n");
			jf_zu_stack_clear();
			s_zu_stack.state = JF_ZU_STACK_FAIL_MATCH;
			return;
		}

		if (s_zu_stack.state == JF_ZU_STACK_FAIL_FOLDER) {
			printf("DEBUG: fail_folder\n");
			jf_zu_stack_clear();
			return;
		}

		printf("DEBUG: match success: ");
		while ((n = jf_zu_stack_pop()) != 0) {
			jf_menu_child_push(n);
			printf("%zu ", n);
		}
		printf(".\n");
		s_zu_stack.state = JF_ZU_STACK_SUCCESS;
	}

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
YY_RULE(int) yy_eol(yycontext *yy); /* 5 */
YY_RULE(int) yy_string(yycontext *yy); /* 4 */
YY_RULE(int) yy_Selector(yycontext *yy); /* 3 */
YY_RULE(int) yy_ws(yycontext *yy); /* 2 */
YY_RULE(int) yy_Start(yycontext *yy); /* 1 */

YY_ACTION(void) yy_1_num(yycontext *yy, char *yytext, int yyleng)
{
#define __ yy->__
#define yypos yy->__pos
#define yythunkpos yy->__thunkpos
  yyprintf((stderr, "do yy_1_num\n"));
  {
#line 162
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
#line 161
   jf_zu_stack_push(n); ;
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
#line 160
   jf_zu_stack_push_range(l, r); ;
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
#line 156
   jf_zu_stack_push_range(1, jf_menu_child_count()); ;
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
#line 155
   jf_zu_stack_finalize(true); ;
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
#line 153
   printf("quit\n"); ;
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
#line 152
   printf("search \"%s\"\n", yytext); ;
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
#line 150
   jf_menu_dotdot(); ;
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
YY_RULE(int) yy_eol(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;
  yyprintf((stderr, "%s\n", "eol"));
  {  int yypos19= yy->__pos, yythunkpos19= yy->__thunkpos;  if (!yymatchChar(yy, '\n')) goto l20;  goto l19;
  l20:;	  yy->__pos= yypos19; yy->__thunkpos= yythunkpos19;  if (!yymatchString(yy, "\r\n")) goto l21;  goto l19;
  l21:;	  yy->__pos= yypos19; yy->__thunkpos= yythunkpos19;  if (!yymatchChar(yy, '\r')) goto l18;
  }
  l19:;	
  yyprintf((stderr, "  ok   %s @ %s\n", "eol", yy->__buf+yy->__pos));
  return 1;
  l18:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "eol", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_string(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;
  yyprintf((stderr, "%s\n", "string"));  if (!yymatchClass(yy, (unsigned char *)"\377\333\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377")) goto l22;
  l23:;	
  {  int yypos24= yy->__pos, yythunkpos24= yy->__thunkpos;  if (!yymatchClass(yy, (unsigned char *)"\377\333\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377")) goto l24;  goto l23;
  l24:;	  yy->__pos= yypos24; yy->__thunkpos= yythunkpos24;
  }
  yyprintf((stderr, "  ok   %s @ %s\n", "string", yy->__buf+yy->__pos));
  return 1;
  l22:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "string", yy->__buf+yy->__pos));
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
YY_RULE(int) yy_ws(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;
  yyprintf((stderr, "%s\n", "ws"));  if (!yymatchClass(yy, (unsigned char *)"\000\002\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000")) goto l28;
  yyprintf((stderr, "  ok   %s @ %s\n", "ws", yy->__buf+yy->__pos));
  return 1;
  l28:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
  yyprintf((stderr, "  fail %s @ %s\n", "ws", yy->__buf+yy->__pos));
  return 0;
}
YY_RULE(int) yy_Start(yycontext *yy)
{  int yypos0= yy->__pos, yythunkpos0= yy->__thunkpos;
  yyprintf((stderr, "%s\n", "Start"));
  l30:;	
  {  int yypos31= yy->__pos, yythunkpos31= yy->__thunkpos;  if (!yy_ws(yy)) goto l31;  goto l30;
  l31:;	  yy->__pos= yypos31; yy->__thunkpos= yythunkpos31;
  }  yyText(yy, yy->__begin, yy->__end);
#define yytext yy->__text
#define yyleng yy->__textlen
 jf_zu_stack_init(); ;
#undef yytext
#undef yyleng

  {  int yypos34= yy->__pos, yythunkpos34= yy->__thunkpos;  if (!yymatchString(yy, "..")) goto l35;
  l36:;	
  {  int yypos37= yy->__pos, yythunkpos37= yy->__thunkpos;  if (!yy_ws(yy)) goto l37;  goto l36;
  l37:;	  yy->__pos= yypos37; yy->__thunkpos= yythunkpos37;
  }  yyDo(yy, yy_1_Start, yy->__begin, yy->__end);  goto l34;
  l35:;	  yy->__pos= yypos34; yy->__thunkpos= yythunkpos34;  if (!yymatchChar(yy, 'r')) goto l38;  if (!yy_ws(yy)) goto l38;
  l39:;	
  {  int yypos40= yy->__pos, yythunkpos40= yy->__thunkpos;  if (!yy_ws(yy)) goto l40;  goto l39;
  l40:;	  yy->__pos= yypos40; yy->__thunkpos= yythunkpos40;
  }  yyText(yy, yy->__begin, yy->__end);
#define yytext yy->__text
#define yyleng yy->__textlen
 s_zu_stack.state = JF_ZU_STACK_RECURSIVE; printf("recursive "); ;
#undef yytext
#undef yyleng
  if (!yy_Selector(yy)) goto l38;
  l41:;	
  {  int yypos42= yy->__pos, yythunkpos42= yy->__thunkpos;  if (!yy_ws(yy)) goto l42;  goto l41;
  l42:;	  yy->__pos= yypos42; yy->__thunkpos= yythunkpos42;
  }  goto l34;
  l38:;	  yy->__pos= yypos34; yy->__thunkpos= yythunkpos34;  if (!yymatchChar(yy, 's')) goto l43;  if (!yy_ws(yy)) goto l43;
  l44:;	
  {  int yypos45= yy->__pos, yythunkpos45= yy->__thunkpos;  if (!yy_ws(yy)) goto l45;  goto l44;
  l45:;	  yy->__pos= yypos45; yy->__thunkpos= yythunkpos45;
  }  yyText(yy, yy->__begin, yy->__end);  {
#define yytext yy->__text
#define yyleng yy->__textlen
if (!(YY_BEGIN)) goto l43;
#undef yytext
#undef yyleng
  }  if (!yy_string(yy)) goto l43;  yyText(yy, yy->__begin, yy->__end);  {
#define yytext yy->__text
#define yyleng yy->__textlen
if (!(YY_END)) goto l43;
#undef yytext
#undef yyleng
  }  yyDo(yy, yy_2_Start, yy->__begin, yy->__end);  goto l34;
  l43:;	  yy->__pos= yypos34; yy->__thunkpos= yythunkpos34;  if (!yymatchChar(yy, 'q')) goto l46;
  l47:;	
  {  int yypos48= yy->__pos, yythunkpos48= yy->__thunkpos;  if (!yy_ws(yy)) goto l48;  goto l47;
  l48:;	  yy->__pos= yypos48; yy->__thunkpos= yythunkpos48;
  }  yyDo(yy, yy_3_Start, yy->__begin, yy->__end);  goto l34;
  l46:;	  yy->__pos= yypos34; yy->__thunkpos= yythunkpos34;  if (!yy_Selector(yy)) goto l33;
  l49:;	
  {  int yypos50= yy->__pos, yythunkpos50= yy->__thunkpos;  if (!yy_ws(yy)) goto l50;  goto l49;
  l50:;	  yy->__pos= yypos50; yy->__thunkpos= yythunkpos50;
  }
  }
  l34:;	  goto l32;
  l33:;	  yyText(yy, yy->__begin, yy->__end);  {
#define yytext yy->__text
#define yyleng yy->__textlen
   jf_zu_stack_finalize(false); ;
#undef yytext
#undef yyleng
  }  goto l29;
  l32:;	  yyDo(yy, yy_4_Start, yy->__begin, yy->__end);  if (!yy_eol(yy)) goto l29;
  yyprintf((stderr, "  ok   %s @ %s\n", "Start", yy->__buf+yy->__pos));
  return 1;
  l29:;	  yy->__pos= yypos0; yy->__thunkpos= yythunkpos0;
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
