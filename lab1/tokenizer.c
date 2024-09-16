/*
 * tokenizer.c delar upp en text i sina ord och matar ut orden
 * omvandlade till sm? bokst?ver, ett ord per rad f?ljt av ordets
 * teckenposition i texten. Ett ord definieras som en eller flera
 * p? varandra f?ljande bokst?ver i det svenska alfabetet.
 * Copyright Viggo Kann, viggo@nada.kth.se, 1999
 *
 * L?tt omhackad av Jakob Nordstr?m 2003 f?r att klara tecken med accenter
 * (t.ex. ?, ?. ? osv.).
 *
 * Fixad av Felix Almay 2022 f?r att hantera ?-tecken samt f?rb?ttra l?sbarheten.
 */

#include <stdio.h>
#include <stdlib.h>

 /* alfabet i Latin-1-ordning */
#define ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ???"

static unsigned char   u2l[256];
static long            pos = 0L;

static int readChar() {
    pos++;
    return getchar();
}

static void tokenize() {
   unsigned char   buf[1000];
   unsigned char  *s;
   int             ch;
   long            startpos;

   do {
      for (ch = readChar(); ch != EOF && u2l[ch] == 0; ch = readChar()) ;
      if (ch == EOF) break;
      startpos = pos-1;
      s = buf;
      *s++ = u2l[ch];
      for (ch = readChar(); ch != EOF && u2l[ch]; ch = readChar())
         *s++ = u2l[ch];
      *s = '\0';
      printf("%s %ld\n", buf, startpos);
   } while (ch != EOF);
}

static void initialize() {
      unsigned char ch;

      for (unsigned char &i: u2l)
         i = 0;

      for (unsigned char *s = (unsigned char *) ALPHABET; *s; s++) {
         ch = *s + 'a' - 'A';
         u2l[*s] = u2l[ch] = ch;
      }

      // Special handling for accented characters
      ch = 223; // German double-s
      u2l[ch] = 's';

      for (ch = 224; ch <= 227; ++ch) // a with accent (except ? and ?)
         u2l[ch + 'A' - 'a'] = u2l[ch] = 'a';

      ch = 230; // ae to ?
      u2l[ch + 'A' - 'a'] = u2l[ch] = 'e';

      ch = 231; // c with cedilla to c
      u2l[ch + 'A' - 'a'] = u2l[ch] = 'c';

      for (ch = 232; ch <= 235; ++ch) // e with accent (including ?)
         u2l[ch + 'A' - 'a'] = u2l[ch] = 'e';

      for (ch = 236; ch <= 239; ++ch) // i with accent
         u2l[ch + 'A' - 'a'] = u2l[ch] = 'i';

      ch = 240; // eth to d
      u2l[ch + 'A' - 'a'] = u2l[ch] = 'd';

      ch = 241; // n with ~ to n
      u2l[ch + 'A' - 'a'] = u2l[ch] = 'n';

      for (ch = 242; ch <= 245; ++ch) // o with accent (except ?)
         u2l[ch + 'A' - 'a'] = u2l[ch] = 'o';

      ch = 248; // o with stroke to ?
      u2l[ch + 'A' - 'a'] = u2l[ch] = 'ö';

      for (ch = 249; ch <= 252; ++ch) // u with accent
         u2l[ch + 'A' - 'a'] = u2l[ch] = 'u';

      ch = 253; // y with accent
      u2l[ch + 'A' - 'a'] = u2l[ch] = 'y';
      ch = 255;
      u2l[ch] = 'y';
   }


void _main() {
    initialize();
    tokenize();
}
