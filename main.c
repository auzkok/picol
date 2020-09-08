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
#include <stdio.h>
#include <stdlib.h>

#include "picol.h"

int main(int argc, char **argv) {
    struct picolInterp interp;
    picolInitInterp(&interp);
    picolRegisterCoreCommands(&interp);
    if (argc == 1) {
        while(1) {
            char clibuf[1024];
            int retcode;
            printf("picol> "); fflush(stdout);
            if (fgets(clibuf,1024,stdin) == NULL) return 0;
            retcode = picolEval(&interp,clibuf);
            if (interp.result[0] != '\0')
                printf("[%d] %s\n", retcode, interp.result);
        }
    } else if (argc == 2) {
        char buf[1024*16];
        FILE *fp = fopen(argv[1],"r");
        if (!fp) {
            perror("open"); exit(1);
        }
        buf[fread(buf,1,1024*16,fp)] = '\0';
        fclose(fp);
        if (picolEval(&interp,buf) != PICOL_OK) printf("%s\n", interp.result);
    }
    return 0;
}
