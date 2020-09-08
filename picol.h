/* Tcl in ~ 500 lines of code.
 *
 * Copyright (c) 2007-2016, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef PICOL_H_INCLUDED
#define PICOL_H_INCLUDED

enum {PICOL_OK, PICOL_ERR, PICOL_RETURN, PICOL_BREAK, PICOL_CONTINUE};
enum {PT_ESC,PT_STR,PT_CMD,PT_VAR,PT_SEP,PT_EOL,PT_EOF};

struct picolParser {
    char *text;
    char *p; /* current text position */
    int len; /* remaining length */
    char *start; /* token start */
    char *end; /* token end */
    int type; /* token type, PT_... */
    int insidequote; /* True if inside " " */
};

struct picolVar {
    char *name, *val;
    struct picolVar *next;
};

struct picolInterp; /* forward declaration */
typedef int (*picolCmdFunc)(struct picolInterp *i, int argc, char **argv, void *privdata);

struct picolCmd {
    char *name;
    picolCmdFunc func;
    void *privdata;
    struct picolCmd *next;
};

struct picolCallFrame {
    struct picolVar *vars;
    struct picolCallFrame *parent; /* parent is NULL at top level */
};

struct picolInterp {
    int level; /* Level of nesting */
    struct picolCallFrame *callframe;
    struct picolCmd *commands;
    char *result;
};



// Function prototypes
void picolInitParser(struct picolParser *p, char *text);
int picolParseSep(struct picolParser *p);
int picolParseEol(struct picolParser *p);
int picolParseCommand(struct picolParser *p);
int picolParseVar(struct picolParser *p);
int picolParseBrace(struct picolParser *p);
int picolParseString(struct picolParser *p);
int picolParseComment(struct picolParser *p);
int picolGetToken(struct picolParser *p);
void picolInitInterp(struct picolInterp *i);
void picolSetResult(struct picolInterp *i, char *s);
struct picolVar *picolGetVar(struct picolInterp *i, char *name);
int picolSetVar(struct picolInterp *i, char *name, char *val);
struct picolCmd *picolGetCommand(struct picolInterp *i, char *name);
int picolRegisterCommand(struct picolInterp *i, char *name, picolCmdFunc f, void *privdata);
int picolEval(struct picolInterp *i, char *t);
int picolArityErr(struct picolInterp *i, char *name);
int picolCommandMath(struct picolInterp *i, int argc, char **argv, void *pd);
int picolCommandSet(struct picolInterp *i, int argc, char **argv, void *pd);
int picolCommandPuts(struct picolInterp *i, int argc, char **argv, void *pd);
int picolCommandIf(struct picolInterp *i, int argc, char **argv, void *pd);
int picolCommandWhile(struct picolInterp *i, int argc, char **argv, void *pd);
int picolCommandRetCodes(struct picolInterp *i, int argc, char **argv, void *pd);
void picolDropCallFrame(struct picolInterp *i);
int picolCommandCallProc(struct picolInterp *i, int argc, char **argv, void *pd);
int picolCommandProc(struct picolInterp *i, int argc, char **argv, void *pd);
int picolCommandReturn(struct picolInterp *i, int argc, char **argv, void *pd);
void picolRegisterCoreCommands(struct picolInterp *i);

#endif // PICOL_H_INCLUDED
