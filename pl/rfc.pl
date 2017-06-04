#!/usr/bin/perl --

# $Id: rfc.pl,v 1.1.1.1 2002/03/18 14:30:29 binary Exp $

#
# Esse  arquivo eh parte integrante do projeto 'rfc' por BinarySoul
#
#**************************************************************************
#
# Esse script gera um arquivo .sql para criacao da base de dados dos RFCs
#
# Dependencias: index (Indice de RFCs)
#

sub in_bd
{
    $str = $_[0]; 

    if(!$str)
    {
        $str = "NULL";
    }
    else
    {
        $str =~ s/\s+/ /g;
        $str =~ s/\s+$//g;
        $str =~ s/'/\\'/g;
        $str = "'$str'";
    }

    return $str;
}

open(FD, 'index');
@coisas = <FD>;
close(FD);

@meses = ("January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December", "Jan-", "Feb-", "Mar-", "Apr-", "May-", "Jun-", "Jul-", "Aug-", "Sep-", "Oct-", "Nov-", "Dec-");


print "
\/* \$Id\$ */

\/* Apagando coisas *\/
DROP SEQUENCE rfc_rfc_id_seq;
DROP TABLE rfc;

\/* Criando Tabela *\/

CREATE TABLE rfc (
    rfc_id              SERIAL  NOT NULL    PRIMARY KEY,
    rfc_numero          INT     NOT NULL,
    rfc_titulo          TEXT    NOT NULL,
    rfc_autor           TEXT    NOT NULL,
    rfc_data            TEXT    NOT NULL,
    rfc_formato         TEXT    NOT NULL,
    rfc_torna_obsoleto  TEXT    NULL,
    rfc_obsoleto_por    TEXT    NULL,
    rfc_atualiza        TEXT    NULL,
    rfc_atualizado_por  TEXT    NULL,
    rfc_also_fyi        INT     NULL,
    rfc_also_bcp        INT     NULL,
    rfc_also_std        INT     NULL,
    rfc_status          TEXT    NULL
);

/* Dados */
";


# pega dados de cada rfc
foreach $coisa (@coisas)
{
    # numero do rfc
    if( $coisa =~ m/^[0-9]+/ ) 
    {
        $numero = $&;
    }

    # titulo
    if( $coisa =~ m/^[0-9]+ ?([^.]+)/ )
    {
        $titulo = $1;
    }

    # data e autores
    foreach $mes (@meses)
    {
        if( $coisa =~ m/$mes.[0-9-]+/ )
        {
            $data = $&;
            $autores = $`;
            $autores =~ s/[^.]+\. ([a-zA-Z,. ]+).*/\1/;
            last;
        }
    }

    # opcionais
    if( $coisa =~ m/\(Format: *([^)]+)\)/     ) { $formato = "$1";        }
    if( $coisa =~ m/\(Obsletes *([^)]+)\)/    ) { $torna_obsoleto = "$1"  }
    if( $coisa =~ m/\(Obsleted by *([^)]+)\)/ ) { $obsoleto_por = "$1"    }
    if( $coisa =~ m/\(Updates *([^)]+)\)/     ) { $atualiza = "$1"        }
    if( $coisa =~ m/\(Updated by *([^)]+)\)/  ) { $atualizado_por = "$1"  }
    if( $coisa =~ m/\(Also FYI *([^)]+)\)/    ) { $fyi = "$1"             }
    if( $coisa =~ m/\(Also BCP *([^)]+)\)/    ) { $bcp = "$1"             }
    if( $coisa =~ m/\(Also STD *([^)]+)\)/    ) { $std = "$1"             }
    if( $coisa =~ m/\(Status: *([^)]+)\)/     ) { $status = "$1"          }

    print "\nINSERT INTO rfc (rfc_numero, rfc_titulo, rfc_autor, rfc_data, rfc_formato, rfc_torna_obsoleto, rfc_obsoleto_por, rfc_atualiza, rfc_atualizado_por, rfc_also_fyi, rfc_also_bcp, rfc_also_std, rfc_status) VALUES (" . in_bd($numero) . ", " . in_bd($titulo) . ", " . in_bd($autores) . ", " . in_bd($data) . ", " . in_bd($formato) . ", " . in_bd($torna_obsoleto) . ", " . in_bd($obsoleto_por) . ", " . in_bd($atualiza) . ", " . in_bd($atualizado_por) . ", " . in_bd($fyi) . ", " . in_bd($bcp) . ", " . in_bd($std) . ", " . in_bd($status). ");";
}
