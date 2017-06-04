/* $Id: rfc.c,v 1.6 2002/05/27 01:24:50 binary Exp $ */

#include "rfc.h"

/* Main */
int
main( int argc, char *argv[] )
{
    PGconn *conn;
    //struct tupla *primeira_tupla, *t;
    char *query_string;
    int i,j;
    
    i=j=0;
    
    conn = con_conecta();

    query_string = ( char * ) getenv( "QUERY_STRING" );
    mostra_busca_form( );
/*
    primeira_tupla = query(conn, "SELECT * FROM rfc LIMIT 10");

    t = primeira_tupla;

    printf("Linha\t|\tColuna\t|\tValor\t\n");
    while(t != NULL)
    {
        j=0;
        while(t->coluna[j] != NULL)
        {
            printf("\n%d\t|\t%d\t|\t%s", i, j, t->coluna[j]);
            j++;
        }
        t = t->proximo;
        i++;
    }

    printf("\n");

    libera_res(primeira_tupla);
*/

    PQfinish(conn);
    return(OK);
}

/* Mostra Form de Busca */
void
mostra_busca_form( void )
{
    char *form;

    form = template_pega_dados( TP_BUSCA_FORM );
    form = str_replace( "%ACTION%", "rfc",  form );
    form = str_replace( "%METODO%", "post", form );

    printf( "%s\n", form );
}

/* Fecha conexao quando em ERRO */
void
con_erro_fecha(PGconn *conn)
{
    PQfinish(conn);
    exit(FALHA);
}

/* Cria conexao */
PGconn *
con_conecta(void)
{
    PGconn *conn;

    conn = PQsetdbLogin(BD_HOST, BD_PORTA, BD_OPC, BD_TTY, BD_NOME, BD_USUARIO, BD_SENHA);

    if(PQstatus(conn) == CONNECTION_BAD)
    {
        fprintf(stderr, "Erro ao tentar conectar em '%s'.\n", BD_NOME);
        fprintf(stderr, "%s", PQerrorMessage(conn));
        con_erro_fecha(conn);
    }

    return conn;
}

struct tupla *
query(PGconn *conn, char *sqlquery)
{
    PGresult *res;
    ExecStatusType status;
    struct tupla *primeira_tupla, *tupla_atual;
    int linhas, colunas;
    int i, j;

    res = PQexec(conn, sqlquery); 
    status = PQresultStatus(res);

    if(status != PGRES_TUPLES_OK && status != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "A query: '%s' falhou.\n", sqlquery);
        fprintf(stderr, "\n%s\n", PQresultErrorMessage(res));
        con_erro_fecha(conn);
    }

    primeira_tupla = tupla_atual = (struct tupla *) malloc(sizeof(struct tupla));

    linhas  = PQntuples(res);
    colunas = PQnfields(res);

    for(i=0; i<linhas; i++)
    {
        for(j=0; j<colunas; j++)
            tupla_atual->coluna[j] = PQgetvalue(res, i, j);

        tupla_atual->proximo = (struct tupla *) malloc(sizeof(struct tupla));
        tupla_atual = tupla_atual->proximo;
    }

    PQclear(res);

    return primeira_tupla;
}

void
libera_res(struct tupla *primeira_tupla)
{
    struct tupla *tupla_atual, *proxima_tupla;

    tupla_atual = primeira_tupla;

    while(tupla_atual != NULL)
    {
        proxima_tupla = tupla_atual->proximo;
        free(tupla_atual);
        tupla_atual = proxima_tupla;   
    }
}


char *
str_replace( char *old, char *new, char *str )
{
    int t, posicao = -1;
    char *p, *p2;

    for( t = 0; str[ t ]; t++ )
    {
        p = &str[t];
        p2 = old;
        while( *p2 && *p2 == *p )
        {
            p++;
            p2++;
        }
        if(!*p2)
        {
            posicao = t;
            break;
        }
    }

    if( posicao > -1 )
    {
        char *before, *after, *buffer;
        int ate = strlen( str ) - ( posicao + strlen( old ) );
        int a = 0;

        before = (char *) malloc( posicao );
        strncpy( before, str, posicao );

        after = (char *) malloc ( ate );

        for( t = posicao + strlen( old ); t <= strlen( str ); t++)
        {
            after[a] = str[t];
            a += 1;
        }

        buffer = ( char * ) malloc( strlen( before ) + strlen( after ) + strlen( new ) );

        snprintf( buffer, strlen( before ) + strlen( new ) + strlen( after ) + 1, "%s%s%s", before, new, after );
        return( buffer );
    }
    return( str );
}

char *
template_pega_dados( char *arquivo )
{
    FILE *t;
    char *buf;
    char *ret;

    buf = ( char * ) malloc( BUF_MAXTAM );
    ret = ( char * ) malloc( BUF_MAXTAM );

    t = fopen( TP_BUSCA_FORM, "r" );

    if( t != NULL && ! ferror( t ) )
    {
        while( ! feof( t ) )
        {
            ( void ) fread( buf, sizeof( char ), BUF_MAXTAM, t );
            ( void ) strlcat( ret, buf, BUF_MAXTAM );
        }
        fclose( t );
    }
    else
    {
        ( void ) fprintf( stderr, "Arquivo '%s' nao foi encontrado ou voce nao tem permissao para acessa-lo\n", arquivo );
    }

    free( buf );

    return ret;
}
