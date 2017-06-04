/* $Id: rfc.h,v 1.2 2002/05/27 01:24:50 binary Exp $ */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "libpq-fe.h"

/* Banco de Dados */
#define	BD_HOST		NULL
#define	BD_PORTA	NULL
#define	BD_OPC		NULL
#define BD_TTY		NULL
#define	BD_NOME		"b1n"
#define	BD_USUARIO	"binary"
#define	BD_SENHA	"cool"

/* Templates */
#define TP_DEFAULT      "default.htm"
#define TP_BUSCA_FORM   "busca_form.htm"
#define TP_BUSCA_RES    "busca_res.htm"

/* Buffer */
#define BUF_MAXTAM      2048

/* Misc */
#define OK          0
#define FALHA       1
#define MAXCOLS     16          /* Qtde maxima de colunas em uma tupla */

/* Structs */
struct tupla
{
    struct tupla *proximo;
    char *coluna[MAXCOLS];
};

/* Prototipos */
char *str_replace( char *, char *, char * );
char *template_pega_dados( char * ); 
PGconn *con_conecta(void);
void con_erro_fecha(PGconn *);
void uso(char *);
void libera_res(struct tupla *);
void mostra_default( void );
void mostra_busca_form( void );
void mostra_busca_res( char *, int );
struct tupla *query(PGconn *, char *);
