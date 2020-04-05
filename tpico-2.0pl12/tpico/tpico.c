/*
 *  TextPico 2.0
 *
 *  Dirk van Deun, dirk@dinf.vub.ac.be
 */

/*
 *  Compiles with Theo D'Hondt's Pico interpreter sources after some
 *  patches:
 *
 *  - PicoNAT.c: fix blatant bug (grep for "_"):
 *       _STR_TYPE_ xxx = "_";   
 *              segfaults on write, may be stored read-only
 *       unsigned char xxx[2] = "_";
 *              better
 *       
 *  - PicoMAI.h: char -> unsigned char _CHA_TYPE_
 *
 *  - PicoMAI.c: call _scan_init_ with _TRUE_ as second argument in
 *       PICO_LOAD, for better error messages in tpico
 *
 *  Changes to be made to tpico for new interpreter versions:
 *
 *  - select which errors are supposed to be fatal (for error_loop)
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef TIME
#include <sys/time.h>
#include <sys/resource.h>
#endif

#include <readline/readline.h>
#include <readline/history.h>

#include "../Pico/Pico.h"
#include "../Pico/PicoMAI.h"
#include "../PicoShell/PicoStrings.h"
#include "../Pico/PicoCNT.h"
#include "../Pico/PicoMEM.h"

#ifdef SIZE
#define MEMSIZE SIZE*1024*1024
#else
#define MEMSIZE 10*1024*1024
#endif

#define STDINBUFLEN     10240
#define SPRINTFBUFLEN   10240
#define MSGBUFLEN       10240
#define ACCEPTBUFLEN    10240

#define NAMELEN         512

#define ON_OLD_LINE     0
#define ON_NEW_LINE     1

#define CR             '\x0D'
#define LF             '\x0A'

#ifdef ONEARG
static int banner = 0, input = 0, garbage = 0, help = 0, standalone = 1;
#else
static int banner = 1, input = 1, garbage = 0, help = 0, standalone = 0;
#endif

static int chardev = 0, sockdev = 0, history_active = 0;

static char sprintfbuf[SPRINTFBUFLEN], msgbuf[MSGBUFLEN];

/*@ null @*/ static char *globalfilebuf = NULL;

static char globalfilename[NAMELEN], historyfilename[NAMELEN];

static char last_shown=LF;
static int at_prompt;

static _ESC_TYPE_ breaking = _ESCAPE_NO_;

static void show(const char s[], const int on_new_line)
{ 
  if (on_new_line && last_shown != LF)
     printf("\n%s", s);
  else printf("%s", s);
  (void)fflush(stdout); 
  while (*s)
     last_shown = *s++;
}

static void cleanup_exit(int code) 
{
   if (chardev || sockdev)
      show("", ON_NEW_LINE);
#ifndef NOFILES
   if (history_active)
      (void)write_history(historyfilename);
#endif
   exit(code);
}

/*@ -noret @*/

_NIL_TYPE_ _PICO_PRINT_(/*@ unused @*/ const _SES_TYPE_ dummy,
                        const _STR_TYPE_ s)
{
   show(s, ON_OLD_LINE); 
}

_ESC_TYPE_ _PICO_ESCAPE_(/*@ unused @*/ const _SES_TYPE_ s)
{
   return breaking;
}

_ESC_TYPE_ _PICO_DISPLAY_(/*@ unused @*/ const _SES_TYPE_ dummy,
                          const _STR_TYPE_ s)
{
   show(s, ON_OLD_LINE); 
   return breaking;
}

_NIL_TYPE_ _PICO_MARK_(/*@ unused @*/ const _SES_TYPE_ dummy1,
                       const _POS_TYPE_ start,
                       /*@ unused @*/ const _POS_TYPE_ dummy3)
{
   _POS_TYPE_ read = 0;
   int line = 1, col = 1;
   char *bufptr = globalfilebuf;

   if (bufptr != NULL) {
      while (read != start) {
         if (*bufptr == LF) {
            line++;
            col = 1;
         } else col++;
         read++;
         bufptr++;
      }
      sprintf(msgbuf,"%s: line %d, col %d", globalfilename, line, col);
   }
}

_NIL_TYPE_ _PICO_MESSAGE_(/*@ unused @*/ const _SES_TYPE_ dummy,
                          const _STR_TYPE_ s)
{ 
   strcpy(msgbuf, s);     
}

_NIL_TYPE_ _PICO_RECLAIM_(const _RCL_TYPE_ r)
{
   if (garbage && r) show("Garbage collect...", ON_NEW_LINE); 
   if (garbage && !r) show(" done.\n", ON_OLD_LINE);
}

/*@ +noret @*/

static void read_file(char * name)
{
   FILE *file = fopen(name,"r");
   int ch, j;
   char *bufptr;

   if (file == NULL) {
      sprintf(sprintfbuf, "ERROR: Cannot open file \"%s\": skipping\n", name);
      show(sprintfbuf, ON_NEW_LINE);
      globalfilebuf = NULL;
      return;
   }
   errno = 0;
   (void)fseek(file, 0, SEEK_END);
      /* fseek return value puzzling, using errno */
   if (errno) {
      sprintf(sprintfbuf, "ERROR: Reading file \"%s\": skipping\n", name);
      show(sprintfbuf, ON_NEW_LINE);
      globalfilebuf = NULL;
      return;
   }
   if ((bufptr = globalfilebuf = 
            (char *)malloc((size_t)ftell(file)+1)) == NULL) {
      sprintf(sprintfbuf,
             "FATAL ERROR: Cannot allocate %ld bytes to load program text\n",
              ftell(file)+1);
      show(sprintfbuf, ON_NEW_LINE);
      cleanup_exit(EXIT_FAILURE);
   }
   strncpy(globalfilename, name, NAMELEN);
   globalfilename[NAMELEN-1] = '\0';

   rewind(file);
   while (!feof(file) && ((ch = getc(file)) != EOF))
      *bufptr++ = (char)ch;
   *bufptr = '\0';

   (void)fclose(file);

   if (standalone && globalfilebuf[0]=='#' && globalfilebuf[1]=='!')
      for (j=0; globalfilebuf[j]!=CR && globalfilebuf[j]!=LF; j++)
         globalfilebuf[j] = ' ';
   /* allow #!/usr/local/bin/pico -bis (and don't mess up mark) */
}
 
static void error_loop(int err)
{
   char acceptbuf[ACCEPTBUFLEN];
   char *bufptr;

   while (err < _PICO_DONE_)
      switch (err) {
	 case _DO_ESCAPE_:
	    breaking = _ESCAPE_NO_;
            assert(!_PICO_KILL_(1));
            assert(!_PICO_SESSION_(1));
            show("User Break\n", ON_NEW_LINE);
	    return;
         case _DO_ACCEPT_: 
	    *acceptbuf = '\0'; /* avoid dirty crash when input = ctrl-d */
            bufptr = fgets(acceptbuf, ACCEPTBUFLEN, stdin);
            bufptr = acceptbuf; /* same reason as above */
            while ((*bufptr != LF) && (*bufptr != CR) && *bufptr)
               bufptr++;
            *bufptr = '\0';
            last_shown = LF;
            err = _PICO_ACCEPT_(1, acceptbuf);
            break;
         case _DO_LOAD_:
#ifdef NOFILES
	    show("Load has been disabled in the on-line version.\n",
			    ON_NEW_LINE);
	    msgbuf[0] = '\0';
	    err = _PICO_LOAD_(1, "display(\"\")");
#else
            if (read_file(msgbuf), globalfilebuf != NULL) {
               msgbuf[0] = '\0';
               err = _PICO_LOAD_(1, globalfilebuf);
               free(globalfilebuf);
               globalfilebuf = NULL;
            } else {
               msgbuf[0] = '\0';
               err = _PICO_LOAD_(1, "display(\"\")");
            }
#endif
            break;
      }

   switch (err) {
      case _PICO_DONE_:
         break;
      case _BUF_ERROR_:
      case _CCH_ERROR_:
      case _CTL_ERROR_:
      case _DPS_ERROR_:
      case _MEM_ERROR_:
      case _REE_ERROR_:
      case _SNA_ERROR_:
      case _STK_ERROR_:
      case _TMN_ERROR_:
      case _TMS_ERROR_:
         sprintf(sprintfbuf, "FATAL ERROR: %s\n", PICO_ERROR(err));
         show(sprintfbuf, ON_NEW_LINE);
         cleanup_exit(EXIT_FAILURE);
      default:
         sprintf(sprintfbuf, "ERROR: %s", PICO_ERROR(err));
         show(sprintfbuf, ON_NEW_LINE); 
         if (msgbuf[0]) {
            sprintf(sprintfbuf, " (%s)\n", msgbuf);
            show(sprintfbuf, ON_OLD_LINE); 
         }
         else show("\n", ON_OLD_LINE);
   } 
}

static void process_file(char* filename)
{
   int j;
   if (read_file(filename), globalfilebuf != NULL) {
      msgbuf[0] = '\0';
      error_loop(_PICO_DO_(1, globalfilebuf, _MARK_YES_));
      free(globalfilebuf);
      globalfilebuf = NULL;
   }
}
 
static void abrt_helper();

static void abrt(/*@ unused @*/ int signal)
{
   if (at_prompt || standalone) 
      cleanup_exit(EXIT_SUCCESS);
   else {
      breaking = _ESCAPE_YES_;
      abrt_helper();
   }
}

static void abrt_helper()
{
   (void)signal(SIGINT,abrt);
}

static void ill(/*@ unused @*/ int signal)
{
   show("FATAL INTERNAL ERROR: Illegal instruction\n", ON_NEW_LINE);
   cleanup_exit(EXIT_FAILURE);
}

static void fpe(/*@ unused @*/ int signal)
{
   show("FATAL INTERNAL ERROR: Floating point exception\n", ON_NEW_LINE);
   cleanup_exit(EXIT_FAILURE);
}

static void segv(/*@ unused @*/ int signal)
{
   show("FATAL INTERNAL ERROR: Segment violation\n", ON_NEW_LINE);
   cleanup_exit(EXIT_FAILURE);
}

static void xcpu(/*@ unused @*/ int signal)
{
   show("Allowed on-line time exceeded.\n", ON_NEW_LINE);
   cleanup_exit(EXIT_FAILURE);
}

static void * xmalloc(size_t size)  /* not available in OS/X readline */
{
   void *tmp = malloc(size);
   if (tmp == NULL) {
      show ("Malloc: out of memory.\n", ON_NEW_LINE);
      cleanup_exit(EXIT_FAILURE);
   }
   else return tmp;
}

/*@ null @*/ static char * myreadline (char * prompt)
{
   if (chardev)
      return readline(prompt);
   else {
      char *line;
      if (sockdev) {
         (void)fputs(prompt, stdout);
         (void)fflush(stdout);
      }
      if (feof(stdin))
         return NULL; 
      line = (char *)xmalloc(STDINBUFLEN);
      return fgets(line, STDINBUFLEN, stdin);
   }
}

static char * identifiergenerator (const char *text, int state)
{
   static int len;
   static _EXP_TYPE_ dct;
   _EXP_TYPE_ d;
   char *name, *fulltext;
   int quoted=0;

   if (!state) {
      len = strlen (text);
      fulltext = rl_line_buffer;
      while (*fulltext)
         if (*fulltext++=='"') quoted = !quoted;
      if (quoted) return NULL;
      else dct = _DCT_;
   }
   while (!_ag_is_VOI_(dct)) {
      name = _ag_get_TXT_(_ag_get_DCT_NAM_(dct));
      dct = _ag_get_DCT_DCT_(dct);
      if (strncmp (name, text, len) == 0) {
	 for (d = _DCT_;
	      _ag_get_DCT_NAM_(d) != _ag_get_DCT_NAM_(dct);
	      d = _ag_get_DCT_DCT_(d));
	 if (d == dct)
	    return strcpy(xmalloc(strlen(name)+1),name);
      }
   }
   return NULL;
}

/*@ -noret @*/

int main(int argc, char *argv[])
{
   int i, j, doubledash = 0;
   char *mem, *line, *last = NULL;
   struct stat instat;

#ifdef TIME
   struct rlimit mylimits;
   mylimits.rlim_cur = TIME; 
   mylimits.rlim_max = TIME+1;  /* a second more for a clean exit */
   setrlimit(RLIMIT_CPU,&mylimits);
   (void)signal(SIGXCPU, xcpu); 
#endif

   (void)fstat(0,&instat);  /* XXX */
   if (S_ISCHR(instat.st_mode))
      chardev = 1;
   if (S_ISSOCK(instat.st_mode))
      sockdev = 1;

#ifndef NOARGS
#ifndef ONEARG
   for (i=1; i<argc; i++)
      if (argv[i][0] == '-') {
         if (argv[i][1] == '-' && !argv[i][2])
            break;
         for (j=1; argv[i][j]; j++)
            switch (argv[i][j]) {
               case 'b': banner=0; break;
               case 'i': input=0; break;
               case 'c': garbage=1; break;
               case 'h': help=1; break;
               case 's': standalone=1; break;
               default:  sprintf(sprintfbuf,
                               "Unknown command line option: %s\n",
                               argv[i]); 
                         show(sprintfbuf,ON_NEW_LINE);
                         cleanup_exit(EXIT_FAILURE);
            }
      }
#endif
#endif

   if ((mem = (char *)malloc(MEMSIZE)) == NULL) {
      sprintf(sprintfbuf,
              "FATAL ERROR: Cannot allocate %d bytes for the Virtual Machine\n",
              MEMSIZE);
      show(sprintfbuf, ON_NEW_LINE);
      exit(EXIT_FAILURE);
   }

   if (banner)
      show("Welcome to TextPico 2.0\n\n", ON_OLD_LINE);
   if (help) {
      show ("-b: show no opening banner\n"
            "-c: print a message during garbage collection\n"
            "-h: help\n"
            "-i: no interactive session: only process file arguments\n"
            "-s: ignore #!-line to allow stand-alone executables\n",
            ON_OLD_LINE);
      exit(EXIT_SUCCESS);
   }

   (void)_PICO_INIT_(mem, MEMSIZE);
   assert(!_PICO_SESSION_(1));

   (void)signal(SIGINT, abrt);

   (void)signal(SIGSEGV, segv); 
   (void)signal(SIGILL, ill);
   (void)signal(SIGFPE, fpe);

#ifndef NOARGS
#ifndef ONEARG
   for (i=1; i< argc; i++) {
      if (argv[i][0] == '-' && !argv[i][1]) {
         show("ERROR: Tpico does not support \"-\" syntax", ON_NEW_LINE);
         continue;
      }
      if (argv[i][0] == '-' && argv[i][1] == '-' && !argv[i][2]) {
         doubledash = 1;
         continue;
      }
      if (argv[i][0] == '-' && !doubledash)
	 continue;
      process_file(argv[i]);
   }
#endif
#endif
#ifdef ONEARG
   if (argc>1) process_file(argv[1]);
#endif

   if (input) {

      if (chardev) {
         //rl_inhibit_completion = 1;
         rl_readline_name="tpico";
         rl_completion_entry_function=identifiergenerator;
         rl_completion_append_character=0;
         rl_completer_quote_characters="\"";
#ifdef RL_STATE_NONE // only defined by GNU readline
         rl_basic_quote_characters="\"";
         /* ['disp] + <tab> should give ['display], not ['display'] */
#endif
         rl_basic_word_break_characters=
	    " 	!#$%&'()*+,-./:;<=>?@[\\]^`{|}~";
	 /* including quote character causes [""dis] + <tab> -> [""display"] */
#ifdef RL_STATE_NONE // only defined by GNU readline
         _rl_enable_paren_matching(1);
         //rl_set_paren_blink_timeout(250000);
#endif
#ifndef NOFILES
         /* tilde_expand() does not work on stupid OS/X readline */
         strncpy(historyfilename, getenv("HOME"), NAMELEN);
         historyfilename[NAMELEN-1] = '\0';
         if (strlen(historyfilename)+strlen("/.tpico_history") < NAMELEN)
            strcpy(historyfilename+strlen(historyfilename), "/.tpico_history");
         else strcpy(historyfilename, ".tpico_history");
         (void)read_history(historyfilename);
#endif
         stifle_history(500);  /* 500 history entries should be enough... */
         using_history();
         history_active = 1;
      }

      while (!feof(stdin)) {
         at_prompt = 1;
         show("", ON_NEW_LINE);
         last_shown = '\0';
         if ((line = myreadline(">")) == NULL)
            break;
         last_shown = LF;
         if (!line[0] || line[0] == CR || line[0] == LF) {
            free(line);
            continue;
         }
         msgbuf[0] = '\0';
         if ((last == NULL || strcmp(line, last)) && chardev)
            add_history(line);
         at_prompt = 0;
         error_loop(_PICO_DO_(1, line, _MARK_YES_));
         if (last != NULL)
            free(last);
         last = line;
      }
   }

   cleanup_exit(EXIT_SUCCESS);
}
