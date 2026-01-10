#ifndef INTERFACE_H
#define INTERFACE_H

#include "gconio.h"
#include <cstring>
#include "fornecedores.h"
#include "gestao_estoque.h"
#include "vendas.h"
#include "clientes.h"

#define COLUNA_MAX 120
#define LINHA_MAX 30
#define BORDA_SUP_ESQUERDA 218
#define BORDA_SUP_DIREITA 191
#define BORDA_INF_ESQUERDA 192
#define BORDA_INF_DIREITA 217
#define LINHA_HORIZONTAL 196
#define LINHA_VERTICAL 179

#define ENTER '\r'
#define BACKSPACE 8
#define ESC 27
#define TECLA_ESCAPE 224
#define SETA_ESQ 75
#define SETA_DIR 77
#define SETA_CIMA 72
#define SETA_BAIXO 80

#define LINHAS_MAXIMAS_LISTA 18

#define DELAY 30
#define REMOVER_CURSOR() printf("\033[?25l")
#define RETORNAR_CURSOR() printf("\033[?25h")

#define FORNECEDORES_CONSULTA 0
#define FORNECEDORES_CADASTRO 1
#define FORNECEDORES_VOLTAR 2

#define MENU_PRINCIPAL_PRODUTO 0
#define MENU_PRINCIPAL_FORNECEDOR 1
#define MENU_PRINCIPAL_CLIENTE 2
#define MENU_PRINCIPAL_VENDA 3
#define MENU_PRINCIPAL_RELATORIO 4
#define MENU_PRINCIPAL_SAIR 5

// Estrutura que representa um elemento gen�rico da Interface de usu�rio

struct ElementoI {
    char *texto;                // Conte�do textual do elemento
    int corFundo;               // Cor do fundo do elemento
    int corLetra;               // Cor do texto do elemento
    int col;                    // Coluna do elemento no terminal
    int lin;                    // Linha do elemento no terminal
};

struct CaixaFlutuante {
    int col;
    int lin;
    int largura;
    int altura;
    int cor;
};

struct CaixaTexto {
    ElementoI nome;
    CaixaFlutuante caixa;
    char *conteudo;
    bool cursorSobre;
};

// Estrutura que representa uma caixa de sele��o
struct CheckBox {
    ElementoI elemento;         // Elemento da interface
    bool marcado;               // Informa��o se a caixa foi selecionada ou n�o
};

// Estrutura que representa um bot�o
struct Botao {
    ElementoI elemento;         // Elemento da interface
    bool cursorSobre;           // Informa��o se o cursor est� sobre o bot�o
};

struct Menu {
    char **texto;                   // Conte�do textual do elemento
    int corFundo;                   // Cor do fundo do elemento
    int corLetra;                   // Cor do texto do elemento
    int col;                        // Coluna do elemento no terminal
    int lin;
};

struct ListaFornecedor {
    int lin;
    int corFundo;
    int corLetra;
    bool cursorSobre;
    Fornecedor *fornecedor;
};

struct ListaProduto {
    int lin;
    int corFundo;
    int corLetra;
    bool cursorSobre;
    Produto *produto;
};

struct ListaVenda {
    int lin;
    int corFundo;
    int corLetra;
    bool cursorSobre;
    ItemVenda *item;
};

struct ListaCliente {
    int lin;
    int corFundo;
    int corLetra;
    bool cursorSobre;
    Cliente *cliente;
};

void limparTela(int c, int l, int h, int w, int cor);

bool tratarErroVenda(int cod) {
    bool prosseguir = false;
    limparTela(4, 28, 1, 112, LIGHTBLUE);
    textbackground(LIGHTGRAY);
    textcolor(RED);
    if (cod == ERRO_NADA_ESCRITO) {
        char *texto = "ERRO: Preencha todas as informacoes";
        gotoxy(11, 28);
        printf(texto);
    } else if (cod == ERRO_PRODUTO_NAO_EXISTE) {
        char *texto = "ERRO: ID invalido";
        gotoxy(11, 28);
        printf(texto);
    } else if (cod == ERRO_QUANTIDADE_INSUFICIENTE) {
        char *texto = "ERRO: Quantidade em estoque insuficiente";
        gotoxy(11, 28);
        printf(texto);
    } else if (cod == ERRO_QTD_INVALIDA) {
        char *texto = "ERRO: Quantidade n�o pode ser zero";
        gotoxy(11, 28);
        printf(texto);
    } else if (cod == PRODUTO_VALIDO) {
        textcolor(GREEN);
        char *texto = "Produto inserido na fila de vendas";
        gotoxy(11, 28);
        printf(texto);
        prosseguir = true;
    }

    return prosseguir;
}

bool tratarErroCad(int cod) {
    bool prosseguir = false;
    const int c = 115;
    gotoxy(64, 26);
    limparTela(4, 26, 1, 112, LIGHTBLUE);
    textbackground(LIGHTGRAY);
    textcolor(RED);
    if (cod == ERRO_NADA_ESCRITO) {
        char *texto = "ERRO: Preencha todas as caixas de texto";
        int tam = strlen(texto);
        gotoxy(c-tam, 26);
        printf(texto);
    } else if (cod == ERRO_CODIGO_EXISTENTE) {
        char *texto = "ERRO: Codigo/ID existente";
        int tam = strlen(texto);
        gotoxy(c-tam, 26);
        printf(texto);
    } else if (cod == ERRO_NUMERO_INVALIDO) {
        char *texto = "ERRO: Insira o numero corretamente";
        int tam = strlen(texto);
        gotoxy(c-tam, 26);
        printf(texto);
    } else if (cod == SEM_ERRO) {
        textcolor(GREEN);
        char *texto = "Fornecedor cadastrado com sucesso!";
        int tam = strlen(texto);
        gotoxy(c-tam, 26);
        printf(texto);
        prosseguir = true;
    }

    return prosseguir;
}

bool tratarErroCadP(int cod) {
    bool prosseguir = false;
    const int c = 115;
    gotoxy(64, 26);
    limparTela(4, 26, 1, 112, LIGHTBLUE);
    textbackground(LIGHTGRAY);
    textcolor(RED);
    if (cod == ERRO_NADA_ESCRITO) {
        char *texto = "ERRO: Preencha todas as caixas de texto";
        int tam = strlen(texto);
        gotoxy(c-tam, 26);
        printf(texto);
    } else if (cod == ERRO_CODIGO_EXISTENTE) {
        char *texto = "ERRO: ID existente";
        int tam = strlen(texto);
        gotoxy(c-tam, 26);
        printf(texto);
    } else if (cod == SEM_ERRO) {
        textcolor(GREEN);
        char *texto = "Produto cadastrado com sucesso!";
        int tam = strlen(texto);
        gotoxy(c-tam, 26);
        printf(texto);
        prosseguir = true;
    }

    return prosseguir;
}

bool tratarErroEdicao(int cod) {
    bool prosseguir = false;
    const int c = 115;
    gotoxy(64, 26);
    limparTela(4, 26, 1, 112, WHITE);
    textbackground(LIGHTGRAY);
    textcolor(RED);
    if (cod == ERRO_NADA_ESCRITO) {
        char *texto = "ERRO: Preencha todas as caixas de texto";
        int tam = strlen(texto);
        gotoxy(c-tam, 26);
        printf(texto);
    } else if (cod == ERRO_NUMERO_INVALIDO) {
        char *texto = "ERRO: Insira o numero corretamente";
        int tam = strlen(texto);
        gotoxy(c-tam, 26);
        printf(texto);
    } else if (cod == SEM_ERRO) {
        textcolor(GREEN);
        char *texto = "Fornecedor editado com sucesso!";
        int tam = strlen(texto);
        gotoxy(c-tam, 26);
        printf(texto);
        prosseguir = true;
    }

    return prosseguir;
}

/*
 * L� um n�mero inteiro utilizando uma cadeira de caracteres. "conteudo" � uma
 * refer�ncia para um ponteiro de caracteres.
 */

namespace intr {
    bool lerDados(CaixaTexto &caixa, char *&conteudo, int n) {
        int tecla = -1;
        int tam = 0;
        bool enter = false;

        RETORNAR_CURSOR();
        if (conteudo == NULL)
            conteudo = new char[n+1];
        else
            tam = strlen(conteudo);
        gotoxy(caixa.caixa.col+1, caixa.caixa.lin);
        textbackground(LIGHTGRAY);
        textcolor(WHITE);
        gotoxy(caixa.caixa.col+1+tam, caixa.caixa.lin);
        while (tecla != ENTER && tecla != ESC) {
            tecla = getch();
            if (tecla == BACKSPACE && tam > 0) {
                tam--;
                conteudo[tam] = '\0';
                gotoxy(caixa.caixa.col+1+tam, caixa.caixa.lin);
                printf(" ");
                gotoxy(caixa.caixa.col+1+tam, caixa.caixa.lin);
            } else if (tecla >= '0' && tecla <= '9' && tam < n) {
                conteudo[tam++] = tecla;
                printf("%c", tecla);
            }
        }
        if (tecla == ENTER)
            enter = true;
        if (tam == 0) {
            delete[] conteudo;
            conteudo = NULL;
        } else
            conteudo[tam] = '\0';
        REMOVER_CURSOR();
        return enter;
    }

    void lerDados(CaixaTexto &caixa, char *&conteudo, int n, bool data){
        int tecla = -1;
        int tam = 0;
        int digitos = 0;
        RETORNAR_CURSOR();

        if (conteudo == NULL)
            conteudo = new char[n+1];
        else
            tam = strlen(conteudo);

        gotoxy(caixa.caixa.col+1, caixa.caixa.lin);
        textbackground(LIGHTGRAY);
        textcolor(WHITE);
        gotoxy(caixa.caixa.col+1+tam, caixa.caixa.lin);

        if(tam == 1 || tam == 4){
            digitos = 1;
        }

        while (tecla != ENTER) {
            gotoxy(caixa.caixa.col+3, caixa.caixa.lin); printf("/");
            gotoxy(caixa.caixa.col+6, caixa.caixa.lin); printf("/");
            gotoxy(caixa.caixa.col+1+tam, caixa.caixa.lin);
            tecla = getch();
            if (tecla == TECLA_ESCAPE) {
                getch();
                continue;
            }
            if (tecla == BACKSPACE && tam > 0) {
                tam--;
                if (conteudo[tam] == '/') {
                    gotoxy(caixa.caixa.col+1+tam, caixa.caixa.lin);
                    printf(" ");
                    conteudo[tam--] = '\0';
                    digitos = 2;
                }
                conteudo[tam] = '\0';
                gotoxy(caixa.caixa.col+1+tam, caixa.caixa.lin);
                printf(" ");
                gotoxy(caixa.caixa.col+1+tam, caixa.caixa.lin);
                digitos--;
                if (digitos < 0) digitos = 0;
            } else if (tecla >= '0' && tecla <= '9' && tam < n) {
                digitos++;
                conteudo[tam++] = tecla;
                printf("%c", tecla);
                if (digitos == 2 && tam < 6) {
                    conteudo[tam++] = '/';
                    printf("/");
                    digitos = 0;
                }
            }
        }
        if (tam == 0) {
            delete[] conteudo;
            conteudo = NULL;
        } else
            conteudo[tam] = '\0';
        REMOVER_CURSOR();
    }
}

namespace flt {
    void lerDados(CaixaTexto &caixa, char **conteudo, int n) {
    int tecla = -1;
    int tam = 0;
    bool ponto = false;

    RETORNAR_CURSOR();
    if (*conteudo == NULL)
        *conteudo = new char[n+1];
    else
        tam = strlen(*conteudo);
    gotoxy(caixa.caixa.col+1, caixa.caixa.lin);
    textbackground(LIGHTGRAY);
    textcolor(WHITE);
    gotoxy(caixa.caixa.col+1+tam, caixa.caixa.lin);
    while (tecla != ENTER) {
        tecla = getch();
        if (tecla == BACKSPACE && tam > 0) {
            tam--;
            if ((*conteudo)[tam] == '.') {
                ponto = false;
            }
            (*conteudo)[tam] = '\0';
            gotoxy(caixa.caixa.col+1+tam, caixa.caixa.lin);
            printf(" ");
            gotoxy(caixa.caixa.col+1+tam, caixa.caixa.lin);
        } else if (((tecla >= '0' && tecla <= '9') || (!ponto && tecla == '.')) && tam < n) {
            (*conteudo)[tam++] = tecla;
            printf("%c", tecla);
        }
        if (tecla == '.') ponto = true;
    }
    if (tam == 0) {
        delete[] *conteudo;
        *conteudo = NULL;
    } else
        (*conteudo)[tam] = '\0';
    REMOVER_CURSOR();
}
}

namespace strng {
    /*
     * L� uma string numa cadeira de caracteres. "conteudo" � uma refer�ncia para
     * um ponteiro de caracteres.
     */
    void lerDados(CaixaTexto &caixa, char *&conteudo, int n) {
        int tecla = -1;
        int tam = 0;

        RETORNAR_CURSOR();
        if (conteudo == NULL)
            conteudo = new char[n+1];
        else
            tam = strlen(conteudo);
        gotoxy(caixa.caixa.col+1, caixa.caixa.lin);
        textbackground(LIGHTGRAY);
        textcolor(WHITE);
        gotoxy(caixa.caixa.col+1+tam, caixa.caixa.lin);
        while (tecla != ENTER) {
            tecla = getch();
            if (tecla == TECLA_ESCAPE) {
                    getch();
                    continue;
            }
            if (tecla == BACKSPACE && tam > 0) {
                tam--;
                conteudo[tam] = '\0';
                gotoxy(caixa.caixa.col+1+tam, caixa.caixa.lin);
                printf(" ");
                gotoxy(caixa.caixa.col+1+tam, caixa.caixa.lin);
            } else if (tecla != ENTER && tecla != BACKSPACE && tam < n) {
                conteudo[tam++] = tecla;
                printf("%c", tecla);
            }
        }
        if (tam == 0) {
            delete[] conteudo;
            conteudo = NULL;
        } else
            conteudo[tam] = '\0';
        REMOVER_CURSOR();
    }

    void lerDados(CaixaTexto &caixa, char *&conteudo, int n, bool cpf) {
        int tecla = -1;
        int tam = 0;
        int digitos = 0;

        RETORNAR_CURSOR();
        if (conteudo == NULL)
            conteudo = new char[n+1];
        else
            tam = strlen(conteudo);
        gotoxy(caixa.caixa.col+1, caixa.caixa.lin);
        textbackground(LIGHTGRAY);
        textcolor(WHITE);
        gotoxy(caixa.caixa.col+1+tam, caixa.caixa.lin);
        while (tecla != ENTER) {
            tecla = getch();
            if (tecla == TECLA_ESCAPE) {
                    getch();
                    continue;
            }
            if (tecla == BACKSPACE && tam > 0) {
                tam--;
                if (conteudo[tam] == '.' || conteudo[tam] == '-') {
                    gotoxy(caixa.caixa.col+1+tam, caixa.caixa.lin);
                    printf(" ");
                    conteudo[tam--] = '\0';
                    digitos = 3;
                }
                conteudo[tam] = '\0';
                gotoxy(caixa.caixa.col+1+tam, caixa.caixa.lin);
                printf(" ");
                gotoxy(caixa.caixa.col+1+tam, caixa.caixa.lin);
                digitos--;
                if (digitos < 0) digitos = 0;
            } else if (tecla >= '0' && tecla <= '9' && tam < n) {
                digitos++;
                conteudo[tam++] = tecla;
                printf("%c", tecla);
                if (digitos == 3) {
                    char caractere;
                    if (tam == 11) caractere = '-';
                    else caractere = '.';
                    conteudo[tam++] = caractere;
                    printf("%c", caractere);
                    digitos = 0;
                }
            }
        }
        if (tam == 0) {
            delete[] conteudo;
            conteudo = NULL;
        } else
            conteudo[tam] = '\0';
        REMOVER_CURSOR();
    }
}

void logicaTelaFCP(int *opcao, bool *enter) {
    int tecla;

    tecla = getch();
    if (tecla == TECLA_ESCAPE) {
        tecla = getch();
        switch (tecla) {
        case SETA_ESQ:
            (*opcao)--;
            if (*opcao < 0) *opcao = 2;
            break;
        case SETA_DIR:
            (*opcao)++;
            if (*opcao > 2) *opcao = 0;
            break;
        }
    } else {
        switch (tecla) {
        case ENTER:
            *enter = true;
            break;
        }
    }
}

void logicaListaItens(int *opcao, int *desliza, int linhas, int total, bool *enter, bool *esc) {
    int tecla = getch();

    if (tecla == TECLA_ESCAPE) {
        tecla = getch();
        switch (tecla) {
        case SETA_BAIXO:
            (*opcao)++;
            if (*opcao >= linhas) {
                *opcao = linhas - 1;
                if ((*opcao + *desliza + 1) < total)
                    (*desliza)++;
            }
            break;
        case SETA_CIMA:
            (*opcao)--;
            if (*opcao < 0) {
                *opcao = 0;
                (*desliza)--;
                if (*desliza < 0) *desliza = 0;
            }
            break;
        }
    } else {
        switch (tecla) {
        case ENTER:
            *enter = true;
            break;
        case ESC:
            *esc = true;
            break;
        }
    }
}

void logicaTelaVenda(int *opcao, bool *enter) {
    int tecla;

    tecla = getch();
    if (tecla == TECLA_ESCAPE) {
        tecla = getch();
        switch (tecla) {
        case SETA_ESQ:
            (*opcao)--;
            if (*opcao < 0) *opcao = 0;
            break;
        case SETA_DIR:
            (*opcao)++;
            if (*opcao > 5) *opcao = 5;
            break;
        }
    } else {
        switch (tecla) {
        case ENTER:
            *enter = true;
            break;
        }
    }
}

void logicaListaProduto(int *opcao, int *desliza, int linhas, int total, bool *enter, bool *esc) {
    int tecla = getch();

    if (tecla == TECLA_ESCAPE) {
        tecla = getch();
        switch (tecla) {
        case SETA_BAIXO:
            (*opcao)++;
            if (*opcao >= linhas) {
                *opcao = linhas - 1;
                if ((*opcao + *desliza + 1) < total)
                    (*desliza)++;
            }
            break;
        case SETA_CIMA:
            (*opcao)--;
            if (*opcao < 0) {
                *opcao = 0;
                (*desliza)--;
                if (*desliza < 0) *desliza = 0;
            }
            break;
        }
    } else {
        switch (tecla) {
        case ENTER:
            *enter = true;
            break;
        case ESC:
            *esc = true;
            break;
        }
    }
}

void logicaConsultarProduto(int *opcao, bool *enter) {
    int tecla = getch();

    if (tecla == TECLA_ESCAPE) {
        tecla = getch();
        switch (tecla) {
        case SETA_DIR:
            (*opcao)++;
            if (*opcao >= 3) *opcao = 2;
            break;
        case SETA_ESQ:
            (*opcao)--;
            if (*opcao < 0) *opcao = 0;
            break;
        }
    } else {
        switch (tecla) {
        case ENTER:
            *enter = true;
            break;
        }
    }
}

void logicaCadastroProduto(int *opcao, bool *enter) {
    int tecla = getch();

    if (tecla == TECLA_ESCAPE) {
        tecla = getch();
        switch (tecla) {
        case SETA_BAIXO:
            if (*opcao >= 2 && *opcao <= 4) (*opcao)++;
            break;
        case SETA_CIMA:
            if (*opcao >= 3 && *opcao <= 5) (*opcao)--;
            break;
        case SETA_ESQ:
            if (*opcao < 3 || *opcao == 6) (*opcao)--;
            if (*opcao < 0) *opcao = 0;
            break;
        case SETA_DIR:
            if (*opcao < 2 || *opcao == 5) (*opcao)++;
            break;
        }
    } else {
        switch (tecla) {
        case ENTER:
            *enter = true;
            break;
        }
    }
}

void logicaCadastroFornecedor(int *opcao, bool *enter) {
    int tecla = getch();

    if (tecla == TECLA_ESCAPE) {
        tecla = getch();
        switch (tecla) {
        case SETA_BAIXO:
            if (*opcao < 4) (*opcao)++;
            break;
        case SETA_CIMA:
            if (*opcao <= 4) (*opcao)--;
            if (*opcao < 0) *opcao = 0;
            break;
        case SETA_ESQ:
            if (*opcao == 5) (*opcao)--;
            break;
        case SETA_DIR:
            if (*opcao == 4) (*opcao)++;
            break;
        }
    } else {
        switch (tecla) {
        case ENTER:
            *enter = true;
            break;
        }
    }
}

void logicaConsultarFornecedor(int *opcao, bool *enter) {
    int tecla = getch();

    if (tecla == TECLA_ESCAPE) {
        tecla = getch();
        switch (tecla) {
        case SETA_DIR:
            (*opcao)++;
            if (*opcao >= 3) *opcao = 2;
            break;
        case SETA_ESQ:
            (*opcao)--;
            if (*opcao < 0) *opcao = 0;
            break;
        }
    } else {
        switch (tecla) {
        case ENTER:
            *enter = true;
            break;
        }
    }
}

void logicaListaFornecedor(int *opcao, int *desliza, int linhas, int total, bool *enter, bool *esc) {
    int tecla = getch();

    if (tecla == TECLA_ESCAPE) {
        tecla = getch();
        switch (tecla) {
        case SETA_BAIXO:
            (*opcao)++;
            if (*opcao >= linhas) {
                *opcao = linhas - 1;
                if ((*opcao + *desliza + 1) < total)
                    (*desliza)++;
            }
            break;
        case SETA_CIMA:
            (*opcao)--;
            if (*opcao < 0) {
                *opcao = 0;
                (*desliza)--;
                if (*desliza < 0) *desliza = 0;
            }
            break;
        }
    } else {
        switch (tecla) {
        case ENTER:
            *enter = true;
            break;
        case ESC:
            *esc = true;
            break;
        }
    }
}

void logicaConsultarCliente(int *opcao, bool *enter) {
    int tecla = getch();

    if (tecla == TECLA_ESCAPE) {
        tecla = getch();
        switch (tecla) {
        case SETA_DIR:
            (*opcao)++;
            if (*opcao >= 3) *opcao = 2;
            break;
        case SETA_ESQ:
            (*opcao)--;
            if (*opcao < 0) *opcao = 0;
            break;
        }
    } else {
        switch (tecla) {
        case ENTER:
            *enter = true;
            break;
        }
    }
}

void logicaCadastroCliente(int *opcao, bool *enter) {
    int tecla = getch();

    if (tecla == TECLA_ESCAPE) {
        tecla = getch();
        switch (tecla) {
        case SETA_BAIXO:
            if (*opcao < 3) (*opcao)++;
            break;
        case SETA_CIMA:
            if (*opcao <= 3) (*opcao)--;
            if (*opcao < 0) *opcao = 0;
            break;
        case SETA_ESQ:
            if (*opcao == 4) (*opcao)--;
            break;
        case SETA_DIR:
            if (*opcao == 3) (*opcao)++;
            break;
        }
    } else {
        switch (tecla) {
        case ENTER:
            *enter = true;
            break;
        }
    }
}

void logicaEditarProduto(int *opcao, bool *enter) {
    int tecla = getch();

    if (tecla == TECLA_ESCAPE) {
        tecla = getch();
        switch (tecla) {
        case SETA_BAIXO:
            if (*opcao <= 3) (*opcao)++;
            break;
        case SETA_CIMA:
            if (*opcao <= 4) (*opcao)--;
            if (*opcao < 0) *opcao = 0;
            break;
        case SETA_ESQ:
            if (*opcao == 5) (*opcao)--;
            break;
        case SETA_DIR:
            if (*opcao == 4) (*opcao)++;
            break;
        }
    } else {
        switch (tecla) {
        case ENTER:
            *enter = true;
            break;
        }
    }
}

void logicaEditarFornecedor(int *opcao, bool *enter) {
    int tecla = getch();

    if (tecla == TECLA_ESCAPE) {
        tecla = getch();
        switch (tecla) {
        case SETA_BAIXO:
            if (*opcao < 3) (*opcao)++;
            break;
        case SETA_CIMA:
            if (*opcao <= 3) (*opcao)--;
            if (*opcao < 0) *opcao = 0;
            break;
        case SETA_ESQ:
            if (*opcao == 4) (*opcao)--;
            break;
        case SETA_DIR:
            if (*opcao == 3) (*opcao)++;
            break;
        }
    } else {
        switch (tecla) {
        case ENTER:
            *enter = true;
            break;
        }
    }
}

void logicaTelaEditarVoltar(int *opcao, bool *enter) {
    int tecla;

    tecla = getch();
    if (tecla == TECLA_ESCAPE) {
        tecla = getch();
        switch (tecla) {
        case SETA_ESQ:
            (*opcao)--;
            if (*opcao < 0) *opcao = 1;
            break;
        case SETA_DIR:
            (*opcao)++;
            if (*opcao > 1) *opcao = 0;
            break;
        }
    } else {
        switch (tecla) {
        case ENTER:
            *enter = true;
            break;
        }
    }
}

void logicaListaCliente(int *opcao, int *desliza, int linhas, int total, bool *enter, bool *esc) {
    int tecla = getch();

    if (tecla == TECLA_ESCAPE) {
        tecla = getch();
        switch (tecla) {
        case SETA_BAIXO:
            (*opcao)++;
            if (*opcao >= linhas) {  // Verifica se a op��o ultrapassou o n�mero total de clientes
                *opcao = linhas - 1;  // Impede que a op��o ultrapasse o n�mero total de clientes
                if ((*opcao + *desliza + 1) < total)
                    (*desliza)++;  // Desliza para baixo se houver mais clientes
            }
            break;
        case SETA_CIMA:
            (*opcao)--;
            if (*opcao < 0) {
                *opcao = 0;
                (*desliza)--;
                if (*desliza < 0) *desliza = 0;
            }
            break;
        }
    } else {
        switch (tecla) {
        case ENTER:
            *enter = true;
            break;
        case ESC:
            *esc = true;
            break;
        }
    }
}

void logicaTelaInicial(int *opcao, bool *enter) {
    int tecla = getch();

    if (tecla == TECLA_ESCAPE) {
        tecla = getch();
        switch (tecla) {
            case SETA_ESQ:
                (*opcao)--;
                if ((*opcao) < 0) (*opcao) = 5;
                break;
            case SETA_DIR:
                (*opcao)++;
                if ((*opcao) > 5) (*opcao) = 0;
                break;
        }
    } else if (tecla == ENTER) {
        *enter = true;
    }
}

bool logicaListaVendaRegistro(int &desliza, const int qnt){
    int tecla;

    tecla = getch();
    if (tecla == TECLA_ESCAPE) {
        tecla = getch();
        switch (tecla) {
        case SETA_CIMA:
            if (desliza > 0) (desliza--);
                return true;
            break;
        case SETA_BAIXO:
            if (16 + desliza < qnt) (desliza++);
                return true;
            break;
        case SETA_DIR:
            return false;
            break;
        case SETA_ESQ:
            return false;
            break;
        }
    }else
        return false;
}

void logicaTelaEstoqueRelatorio(int *opcao, bool *enter) {
    int tecla = getch();

    if (tecla == TECLA_ESCAPE) {
        tecla = getch();
        switch (tecla) {
        case SETA_BAIXO:
            if (*opcao < 2) (*opcao)++;
            break;
        case SETA_CIMA:
            if (*opcao < 3) (*opcao)--;
            if (*opcao < 0) *opcao = 0;
            break;
        case SETA_ESQ:
            if (*opcao == 3) (*opcao)--;
            break;
        case SETA_DIR:
            if (*opcao == 2) (*opcao)++;
            break;
        }
    } else if (tecla == ENTER)
        *enter = true;
}

void logicaTelasRegistro(int *opcao, bool *enter){
    int tecla = getch();

    if (tecla == TECLA_ESCAPE) {
        tecla = getch();
        switch (tecla) {
        case SETA_ESQ:
            (*opcao)--;
            if (*opcao < 0) *opcao = 5;
            break;
        case SETA_DIR:
            (*opcao)++;
            if (*opcao > 5) *opcao = 0;
            break;
        }
    } else {
        switch (tecla) {
        case ENTER:
            *enter = true;
            break;
        }
    }
}

void logicaTelaInicialRegistro(int *opcao, bool *enter){
    int tecla;

    tecla = getch();
    if (tecla == TECLA_ESCAPE) {
        tecla = getch();
        switch (tecla) {
        case SETA_ESQ:
            (*opcao)--;
            if (*opcao < 0) *opcao = 2;
            break;
        case SETA_DIR:
            (*opcao)++;
            if (*opcao > 2) *opcao = 0;
            break;

        }
    } else {
        switch (tecla) {
        case ENTER:
            *enter = true;
            break;
        }
    }
}

void logicaTelaApagarRegistro(int *opcao, bool *enter){
    int tecla;

    tecla = getch();
    if (tecla == TECLA_ESCAPE) {
        tecla = getch();
        switch (tecla) {
        case SETA_ESQ:
            (*opcao)--;
            if (*opcao < 0) *opcao = 3;
            break;
        case SETA_DIR:
            (*opcao)++;
            if (*opcao > 3) *opcao = 0;
            break;

        }
    } else {
        switch (tecla) {
        case ENTER:
            *enter = true;
            break;
        }
    }
}

void atualizarListaVenda(ListaVenda *lista, Venda &venda, int linhas) {
    for (int i = 0; i < linhas; i++)
        lista[i].item = &venda.itens[i];
}

void atualizarEstados(Botao *botoes, int opcao, int n) {
    for (int i = 0; i < n; i++)
        botoes[i].cursorSobre = false;
    botoes[opcao].cursorSobre = true;
}

void atualizarEstados(ListaFornecedor *elemento, int opcao, int n) {
    for (int i = 0; i < n; i++)
        elemento[i].cursorSobre = false;
    elemento[opcao].cursorSobre = true;
}

void atualizarEstados(CaixaTexto *caixas, Botao *botoes, int opcao, int nC, int nB) {
    for (int i = 0; i < nC; i++)
        caixas[i].cursorSobre = false;
    for (int i = 0; i < nB; i++)
        botoes[i].cursorSobre = false;
    if (opcao >= nC) {
        botoes[opcao-nC].cursorSobre = true;
    } else {
        caixas[opcao].cursorSobre = true;
    }
}

void deslizarLista(ListaFornecedor *lista, Fornecedor *fornecedores, int desliza, int nL) {
    int fim = nL + desliza;
    int j = 0;
    for (int i = desliza; i < fim; i++) {
        lista[j++].fornecedor = &fornecedores[i];
    }
}

void deslizarLista(ListaProduto *lista, Produto *produtos, int desliza, int nL) {
    int fim = nL + desliza;
    int j = 0;
    for (int i = desliza; i < fim; i++) {
        lista[j++].produto = &produtos[i];
    }
}

void deslizarLista(ListaVenda *lista, Venda &venda, int desliza, int nL) {
    int fim = nL + desliza;
    int j = 0;
    for (int i = desliza; i < fim; i++) {
        lista[j++].item = &venda.itens[i];
    }
}

void deslizarLista(ListaCliente *lista, Cliente *clientes, int desliza, int nL) {
    int fim = desliza + nL;
    int j = 0;
    for (int i = desliza; i < fim; i++) {
        lista[j++].cliente = &clientes[i];
    }
}

void inicializarApagarRelatorio(Botao *botoes){

    botoes[0].elemento.texto = new char[10];
    strcpy(botoes[0].elemento.texto, "< LISTA >");
    botoes[1].elemento.texto = new char[14];
    strcpy(botoes[1].elemento.texto, "< INTERVALO >");
    botoes[2].elemento.texto = new char[11];
    strcpy(botoes[2].elemento.texto, "< RESUMO >");
    botoes[3].elemento.texto = new char[11];
    strcpy(botoes[3].elemento.texto, "< VOLTAR >");

    int x = 10;
    for (int i = 0; i < 4; i++) {
        if(i == 2)
            x+=2;
        botoes[i].elemento.col = 38 + x*i;
        botoes[i].elemento.lin = 14;
        botoes[i].elemento.corFundo = LIGHTGRAY;
        botoes[i].elemento.corLetra = DARKGRAY;
        botoes[i].cursorSobre = false;
    }
    botoes[0].cursorSobre = false;
}

void inicializarTelaRE(CaixaTexto *caixas, Botao *botoes) {
    caixas[0].nome.texto = new char[12];
    strcpy(caixas[0].nome.texto, "Responsavel");
    caixas[1].nome.texto = new char[12];
    strcpy(caixas[1].nome.texto, "Localizacao");

    for (int i = 0; i < 2; i++) {
        caixas[i].nome.col = 38;
        caixas[i].nome.lin = 10 + 4 * i;
        caixas[i].nome.corFundo = LIGHTBLUE;
        caixas[i].nome.corLetra = WHITE;
        caixas[i].caixa.col = caixas[i].nome.col;
        caixas[i].caixa.lin = caixas[i].nome.lin + 2;
        caixas[i].caixa.altura = 1;
        caixas[i].caixa.largura = 44;
        caixas[i].caixa.cor = WHITE;
        caixas[i].cursorSobre = false;
        caixas[i].conteudo = NULL;
    }
    caixas[1].caixa.largura = 26;
    caixas[0].cursorSobre = true;

    botoes[0].elemento.texto = new char[10];
    strcpy(botoes[0].elemento.texto, "< GERAR >");
    botoes[1].elemento.texto = new char[11];
    strcpy(botoes[1].elemento.texto, "< VOLTAR >");

    for (int i = 0; i < 2; i++) {
        botoes[i].elemento.col = caixas[i].caixa.col + 12 * i;
        botoes[i].elemento.lin = caixas[1].caixa.lin + 2;
        botoes[i].elemento.corFundo = LIGHTGRAY;
        botoes[i].elemento.corLetra = DARKGRAY;
        botoes[i].cursorSobre = false;
    }
}

void inicializarTelasRegistro(CaixaTexto *caixas, Botao *botoes){
    caixas[0].nome.texto = new char[12];
    strcpy(caixas[0].nome.texto, "Data Inicio");
    caixas[1].nome.texto = new char[9];
    strcpy(caixas[1].nome.texto, "Data fim");

    for (int i = 0; i < 2; i++) {
        caixas[i].nome.corFundo = LIGHTBLUE;
        caixas[i].nome.corLetra = WHITE;
        caixas[i].nome.col = 6 + 15 * i;
        caixas[i].nome.lin = 4;
        caixas[i].caixa.col = caixas[i].nome.col;
        caixas[i].caixa.lin = caixas[i].nome.lin + 2;
        caixas[i].caixa.largura = 12;
        caixas[i].caixa.altura = 1;
        caixas[i].caixa.cor = WHITE;
        caixas[i].conteudo = NULL;
        caixas[i].cursorSobre = false;
    }
    caixas[0].cursorSobre = true;

    botoes[0].elemento.texto = new char[11];
    strcpy(botoes[0].elemento.texto, "< GERAR >");
    botoes[1].elemento.texto = new char[11];
    strcpy(botoes[1].elemento.texto, "< RESUMO >");
    botoes[2].elemento.texto = new char[11];
    strcpy(botoes[2].elemento.texto, "< APAGAR >");
    botoes[3].elemento.texto = new char[11];
    strcpy(botoes[3].elemento.texto, "< VOLTAR >");

    for (int i = 0; i < 4; i++) {
        botoes[i].elemento.col = 40 + 13*i;
        botoes[i].elemento.lin = caixas[1].caixa.lin;
        botoes[i].elemento.corFundo = LIGHTGRAY;
        botoes[i].elemento.corLetra = DARKGRAY;
        botoes[i].cursorSobre = false;
    }



}

void inicializarTelaInicialRegistro(Botao *opcoes){
    opcoes[0].elemento.texto = new char[11];
    strcpy(opcoes[0].elemento.texto, " Venda ");
    opcoes[1].elemento.texto = new char[11];
    strcpy(opcoes[1].elemento.texto, " Estoque ");
    opcoes[2].elemento.texto = new char[11];
    strcpy(opcoes[2].elemento.texto, "< Voltar >");

    for (int i = 0; i < 2; i++) {
        opcoes[i].elemento.corFundo = LIGHTGRAY;
        opcoes[i].elemento.corLetra = DARKGRAY;
        opcoes[i].elemento.col = 5 + 12 * i;
        opcoes[i].elemento.lin = 2;
    }
    opcoes[0].cursorSobre = true;
    opcoes[1].cursorSobre = false;
    opcoes[2].cursorSobre = false;

    opcoes[2].elemento.corFundo = LIGHTGRAY;
    opcoes[2].elemento.corLetra = DARKGRAY;
    opcoes[2].elemento.col = 105;
    opcoes[2].elemento.lin = 2;
}

void inicializarConsultaCliente(CaixaTexto *caixa, Botao *botoes) {
    botoes[0].elemento.texto = new char[14];
    strcpy(botoes[0].elemento.texto, "< CONSULTAR >");
    botoes[1].elemento.texto = new char[9];
    strcpy(botoes[1].elemento.texto, "< SAIR >");

    for (int i = 0; i < 2; i++) {
        botoes[i].cursorSobre = false;
        botoes[i].elemento.col = 80 + 26 * i;
        botoes[i].elemento.lin = 5;
        botoes[i].elemento.corFundo = LIGHTGRAY;
        botoes[i].elemento.corLetra = DARKGRAY;
    }
    caixa[0].nome.texto = new char[16];
    strcpy(caixa[0].nome.texto, "Nome do cliente");
    caixa[0].nome.corFundo = LIGHTBLUE;
    caixa[0].nome.corLetra = WHITE;
    caixa[0].nome.col = 5;
    caixa[0].nome.lin = 5;
    caixa[0].cursorSobre = true;
    caixa[0].conteudo = NULL;
    caixa[0].caixa.col = caixa[0].nome.col + 18;
    caixa[0].caixa.lin = caixa[0].nome.lin;
    caixa[0].caixa.cor = WHITE;
    caixa[0].caixa.altura = 1;
    caixa[0].caixa.largura = 52;
}

void inicializarListaClientes(ListaCliente **lista, Cliente *clientes, int &n) {
    if (n > 18) n = 18;

    // Cria uma lista com capacidade de suportar at� 20 linhas
    *lista = new ListaCliente[n];
    for (int i = 0; i < n; i++) {
        (*lista)[i].lin = 9 + i;
        (*lista)[i].cursorSobre = false;
        (*lista)[i].corFundo = WHITE;
        (*lista)[i].corLetra = DARKGRAY;
        (*lista)[i].cliente = &clientes[i];
    }
    (*lista)[0].cursorSobre = true;
}

void inicializarTelaIdCliente(CaixaTexto *caixas) {
    caixas[0].nome.texto = new char[14];
    strcpy(caixas[0].nome.texto, "ID do cliente");
    caixas[0].nome.corFundo = LIGHTBLUE;
    caixas[0].nome.corLetra = WHITE;
    caixas[0].nome.col = 54;
    caixas[0].nome.lin = 12;
    caixas[0].conteudo = NULL;
    caixas[0].cursorSobre = true;
    caixas[0].caixa.altura = 1;
    caixas[0].caixa.largura = 12;
    caixas[0].caixa.cor = WHITE;
    caixas[0].caixa.col = caixas[0].nome.col;
    caixas[0].caixa.lin = caixas[0].nome.lin + 2;

}

void inicializarTelaVenda(CaixaTexto *caixas, Botao *botoes, ListaVenda *lista) {
    botoes[0].elemento.texto = new char[14];
    strcpy(botoes[0].elemento.texto, "< ADICIONAR >");
    botoes[1].elemento.texto = new char[12];
    strcpy(botoes[1].elemento.texto, "< REMOVER >");
    botoes[2].elemento.texto = new char[14];
    strcpy(botoes[2].elemento.texto, "< FINALIZAR >");
    botoes[3].elemento.texto = new char[9];
    strcpy(botoes[3].elemento.texto, "< SAIR >");

    for (int i = 0; i < 4; i++) {
        botoes[i].cursorSobre = false;
        botoes[i].elemento.corFundo = LIGHTGRAY;
        botoes[i].elemento.corLetra = DARKGRAY;
        botoes[i].elemento.col = 49 + 16 * i;
        botoes[i].elemento.lin = 5;
    }
    botoes[0].elemento.col = 48;
    botoes[2].elemento.col = 49 + 16 * 2 - 1;
    caixas[0].nome.texto = new char[11];
    strcpy(caixas[0].nome.texto, "ID Produto");
    caixas[1].nome.texto = new char[11];
    strcpy(caixas[1].nome.texto, "Quantidade");
    caixas[0].nome.col = 11;
    caixas[0].nome.lin = 3;
    caixas[1].nome.col = 29;
    caixas[1].nome.lin = 3;
    for (int i = 0; i < 2; i++) {
        caixas[i].conteudo = NULL;
        caixas[i].cursorSobre = false;
        caixas[i].nome.corFundo = LIGHTBLUE;
        caixas[i].nome.corLetra = WHITE;
        caixas[i].caixa.altura = 1;
        caixas[i].caixa.largura = 12;
        caixas[i].caixa.cor = WHITE;
        caixas[i].caixa.col = caixas[i].nome.col;
        caixas[i].caixa.lin = caixas[i].nome.lin + 2;
    }
    caixas[0].cursorSobre = true;
    for (int i = 0; i < 18; i++) {
        lista[i].cursorSobre = false;
        lista[i].corFundo = WHITE;
        lista[i].corLetra = DARKGRAY;
        lista[i].lin = 9 + i;
    }
}

void inicializarEditarProduto(CaixaTexto *caixas, Botao *botoes, const Produto &f) {
    caixas[0].nome.texto = new char[16];
    strcpy(caixas[0].nome.texto, "Nome do Produto");
    caixas[1].nome.texto = new char[6];
    strcpy(caixas[1].nome.texto, "Preco");
    caixas[2].nome.texto = new char[11];
    strcpy(caixas[2].nome.texto, "Quantidade");
    caixas[3].nome.texto = new char[11];
    strcpy(caixas[3].nome.texto, "Categoria");

    for (int i = 0; i < 4; i++) {
        caixas[i].nome.corFundo = LIGHTBLUE;
        caixas[i].nome.corLetra = WHITE;
        caixas[i].nome.col = 19;
        caixas[i].nome.lin = 11 + i;
        caixas[i].caixa.col = 35;
        caixas[i].caixa.lin = 11 + i;
        caixas[i].caixa.largura = 44;
        caixas[i].caixa.altura = 1;
        caixas[i].caixa.cor = WHITE;
        caixas[i].conteudo = NULL;
        caixas[i].cursorSobre = false;
    }
    caixas[0].cursorSobre = true;
    caixas[1].caixa.largura = 13;
    caixas[2].caixa.largura = 13;
    caixas[3].caixa.largura = 20;

    botoes[0].elemento.texto = new char[11];
    strcpy(botoes[0].elemento.texto, "< SALVAR >");
    botoes[1].elemento.texto = new char[11];
    strcpy(botoes[1].elemento.texto, "< VOLTAR >");

    for (int i = 0; i < 2; i++) {
        botoes[i].elemento.col = caixas[i].caixa.col + 12 * i;
        botoes[i].elemento.lin = 16;
        botoes[i].elemento.corFundo = LIGHTGRAY;
        botoes[i].elemento.corLetra = DARKGRAY;
        botoes[i].cursorSobre = false;
    }
    caixas[0].conteudo = new char[NOME_TAM+1];
    caixas[1].conteudo = new char[9+1];
    caixas[2].conteudo = new char[9+1];
    caixas[3].conteudo = new char[TIPO_TAM+1];
    strcpy(caixas[0].conteudo, f.nome);
    sprintf(caixas[1].conteudo, "%.2f", f.preco);
    sprintf(caixas[2].conteudo, "%d", f.Quantidade);
    strcpy(caixas[3].conteudo, f.categoria);
}

void inicializarListaProduto(ListaProduto **lista, Produto *produtos, int &n) {
    int total;

    if (n > 18) n = 18;
    //else n = n;
    // Cria uma lista com capacidade de suportar 18 linhas
    *lista = new ListaProduto[n];
    for (int i = 0; i < n; i++) {
        (*lista)[i].lin = 9 + i;
        (*lista)[i].cursorSobre = false;
        (*lista)[i].corFundo = WHITE;
        (*lista)[i].corLetra = DARKGRAY;
        //(*lista)[i].fornecedor = NULL;
        (*lista)[i].produto = &produtos[i];
    }
    /*
    for (int i = 0; i < n; i++)
        (*lista)[i].fornecedor = &fornecedores[i];
    */
    (*lista)[0].cursorSobre = true;
}

void inicializarConsultaProduto(CaixaTexto *caixa, Botao *botoes) {
    botoes[0].elemento.texto = new char[14];
    strcpy(botoes[0].elemento.texto, "< CONSULTAR >");
    botoes[1].elemento.texto = new char[9];
    strcpy(botoes[1].elemento.texto, "< SAIR >");

    for (int i = 0; i < 2; i++) {
        botoes[i].cursorSobre = false;
        botoes[i].elemento.col = 80 + 26 * i;
        botoes[i].elemento.lin = 5;
        botoes[i].elemento.corFundo = LIGHTGRAY;
        botoes[i].elemento.corLetra = DARKGRAY;
    }
    caixa[0].nome.texto = new char[18];
    strcpy(caixa[0].nome.texto, "Nome do produto");
    caixa[0].nome.corFundo = LIGHTBLUE;
    caixa[0].nome.corLetra = WHITE;
    caixa[0].nome.col = 5;
    caixa[0].nome.lin = 5;
    caixa[0].cursorSobre = true;
    caixa[0].conteudo = NULL;
    caixa[0].caixa.col = caixa[0].nome.col + 18;
    caixa[0].caixa.lin = caixa[0].nome.lin;
    caixa[0].caixa.cor = WHITE;
    caixa[0].caixa.altura = 1;
    caixa[0].caixa.largura = 52;
}

void inicializarCadastroProduto(CaixaTexto *caixas, Botao *botoes) {
    caixas[0].nome.texto = new char[3];
    strcpy(caixas[0].nome.texto, "ID");
    caixas[1].nome.texto = new char[11];
    strcpy(caixas[1].nome.texto, "Quantidade");
    caixas[2].nome.texto = new char[6];
    strcpy(caixas[2].nome.texto, "Preco");
    caixas[3].nome.texto = new char[16];
    strcpy(caixas[3].nome.texto, "Nome do Produto");
    caixas[4].nome.texto = new char[10];
    strcpy(caixas[4].nome.texto, "Categoria");

    for (int i = 0; i < 5; i++) {
        caixas[i].nome.corFundo = LIGHTBLUE;
        caixas[i].nome.corLetra = WHITE;
        caixas[i].caixa.largura = 39;
        caixas[i].caixa.altura = 1;
        caixas[i].caixa.cor = WHITE;
        caixas[i].conteudo = NULL;
        caixas[i].cursorSobre = false;
    }
    caixas[0].cursorSobre = true;
    caixas[0].nome.col = 40;
    caixas[0].nome.lin = 5;
    caixas[0].caixa.col = caixas[0].nome.col;
    caixas[0].caixa.lin = caixas[0].nome.lin + 2;
    caixas[0].caixa.largura = 11;
    caixas[1].caixa.largura = 11;
    caixas[2].caixa.largura = 11;
    caixas[1].nome.lin = caixas[0].nome.lin;
    caixas[1].nome.col = caixas[0].nome.col + 14;
    caixas[1].caixa.lin = caixas[0].caixa.lin;
    caixas[1].caixa.col = caixas[0].caixa.col + 14;
    caixas[2].nome.lin = caixas[1].nome.lin;
    caixas[2].nome.col = caixas[1].nome.col + 14;
    caixas[2].caixa.lin = caixas[1].caixa.lin;
    caixas[2].caixa.col = caixas[1].caixa.col + 14;
    caixas[3].nome.lin = caixas[0].caixa.lin + 3;
    caixas[3].nome.col = caixas[0].caixa.col;
    caixas[3].caixa.lin = caixas[3].nome.lin + 2;
    caixas[3].caixa.col = caixas[3].nome.col;
    caixas[4].nome.lin = caixas[3].caixa.lin + 3;
    caixas[4].nome.col = caixas[3].caixa.col;
    caixas[4].caixa.lin = caixas[4].nome.lin + 2;
    caixas[4].caixa.col = caixas[4].nome.col;
    caixas[4].caixa.largura = 20;

    botoes[0].elemento.texto = new char[11];
    strcpy(botoes[0].elemento.texto, "< SALVAR >");
    botoes[1].elemento.texto = new char[11];
    strcpy(botoes[1].elemento.texto, "< VOLTAR >");

    for (int i = 0; i < 2; i++) {
        botoes[i].elemento.col = caixas[i].caixa.col + 12 * i;
        botoes[i].elemento.lin = caixas[4].caixa.lin + 4;
        botoes[i].elemento.corFundo = LIGHTGRAY;
        botoes[i].elemento.corLetra = DARKGRAY;
        botoes[i].cursorSobre = false;
    }
}

void inicializarEditarFornecedor(CaixaTexto *caixas, Botao *botoes, const Fornecedor &f) {
    caixas[0].nome.texto = new char[16];
    strcpy(caixas[0].nome.texto, "Nome da Empresa");
    caixas[1].nome.texto = new char[12];
    strcpy(caixas[1].nome.texto, "Responsavel");
    caixas[2].nome.texto = new char[9];
    strcpy(caixas[2].nome.texto, "Telefone");

    for (int i = 0; i < 3; i++) {
        caixas[i].nome.corFundo = LIGHTBLUE;
        caixas[i].nome.corLetra = WHITE;
        caixas[i].nome.col = 19;
        caixas[i].nome.lin = 12 + i;
        caixas[i].caixa.col = 35;
        caixas[i].caixa.lin = 12 + i;
        caixas[i].caixa.largura = 44;
        caixas[i].caixa.altura = 1;
        caixas[i].caixa.cor = WHITE;
        caixas[i].conteudo = NULL;
        caixas[i].cursorSobre = false;
    }
    caixas[0].cursorSobre = true;
    caixas[2].caixa.largura = 13;

    botoes[0].elemento.texto = new char[11];
    strcpy(botoes[0].elemento.texto, "< SALVAR >");
    botoes[1].elemento.texto = new char[11];
    strcpy(botoes[1].elemento.texto, "< VOLTAR >");

    for (int i = 0; i < 2; i++) {
        botoes[i].elemento.col = caixas[i].caixa.col + 12 * i;
        botoes[i].elemento.lin = 16;
        botoes[i].elemento.corFundo = LIGHTGRAY;
        botoes[i].elemento.corLetra = DARKGRAY;
        botoes[i].cursorSobre = false;
    }
    caixas[0].conteudo = new char[NOME1_TAM+1];
    caixas[1].conteudo = new char[NOME2_TAM+1];
    caixas[2].conteudo = new char[11+1];
    strcpy(caixas[0].conteudo, f.empresaNome);
    strcpy(caixas[1].conteudo, f.responsavel);
    char contato[12];
    int tam = 0;
    for (int i = 0; f.contato[i] != '\0'; i++)
        if (f.contato[i] >= '0' && f.contato[i] <= '9')
            contato[tam++] = f.contato[i];
    contato[11] = '\0';
    strcpy(caixas[2].conteudo, contato);
}

void inicializarCadastroFornecedor(CaixaTexto *caixas, Botao *botoes) {
    caixas[0].nome.texto = new char[7];
    strcpy(caixas[0].nome.texto, "Codigo");
    caixas[1].nome.texto = new char[16];
    strcpy(caixas[1].nome.texto, "Nome da Empresa");
    caixas[2].nome.texto = new char[12];
    strcpy(caixas[2].nome.texto, "Responsavel");
    caixas[3].nome.texto = new char[9];
    strcpy(caixas[3].nome.texto, "Telefone");

    for (int i = 0; i < 4; i++) {
        caixas[i].nome.corFundo = LIGHTBLUE;
        caixas[i].nome.corLetra = WHITE;
        caixas[i].nome.col = 29;
        caixas[i].nome.lin = 5 + 4 * i;
        caixas[i].caixa.col = caixas[i].nome.col;
        caixas[i].caixa.lin = caixas[i].nome.lin + 2;
        caixas[i].caixa.largura = 44;
        caixas[i].caixa.altura = 1;
        caixas[i].caixa.cor = WHITE;
        caixas[i].conteudo = NULL;
        caixas[i].cursorSobre = false;
    }
    caixas[0].cursorSobre = true;
    caixas[0].caixa.largura = 13;
    caixas[3].caixa.largura = 17;

    botoes[0].elemento.texto = new char[11];
    strcpy(botoes[0].elemento.texto, "< SALVAR >");
    botoes[1].elemento.texto = new char[11];
    strcpy(botoes[1].elemento.texto, "< VOLTAR >");

    for (int i = 0; i < 2; i++) {
        botoes[i].elemento.col = caixas[i].caixa.col + 12 * i;
        botoes[i].elemento.lin = caixas[3].caixa.lin + 4;
        botoes[i].elemento.corFundo = LIGHTGRAY;
        botoes[i].elemento.corLetra = DARKGRAY;
        botoes[i].cursorSobre = false;
    }
}

void inicializarCadastroCliente(CaixaTexto *caixas, Botao *botoes) {
    caixas[0].nome.texto = new char[3];
    strcpy(caixas[0].nome.texto, "ID");
    caixas[1].nome.texto = new char[5];
    strcpy(caixas[1].nome.texto, "Nome");
    caixas[2].nome.texto = new char[4];
    strcpy(caixas[2].nome.texto, "CPF");

    for (int i = 0; i < 3; i++) {
        caixas[i].nome.corFundo = LIGHTBLUE;
        caixas[i].nome.corLetra = WHITE;
        caixas[i].nome.col = 29;
        caixas[i].nome.lin = 5 + 4 * i;
        caixas[i].caixa.col = caixas[i].nome.col;
        caixas[i].caixa.lin = caixas[i].nome.lin + 2;
        caixas[i].caixa.largura = 66;
        caixas[i].caixa.altura = 1;
        caixas[i].caixa.cor = WHITE;
        caixas[i].conteudo = NULL;
        caixas[i].cursorSobre = false;
    }
    caixas[0].cursorSobre = true;
    caixas[0].caixa.largura = 13;
    caixas[2].caixa.largura = 16;

    botoes[0].elemento.texto = new char[11];
    strcpy(botoes[0].elemento.texto, "< SALVAR >");
    botoes[1].elemento.texto = new char[11];
    strcpy(botoes[1].elemento.texto, "< VOLTAR >");

    for (int i = 0; i < 2; i++) {
        botoes[i].elemento.col = caixas[i].caixa.col + 12 * i;
        botoes[i].elemento.lin = caixas[2].caixa.lin + 4;
        botoes[i].elemento.corFundo = LIGHTGRAY;
        botoes[i].elemento.corLetra = DARKGRAY;
        botoes[i].cursorSobre = false;
    }
}

void inicializarListaFornecedor(ListaFornecedor **lista, Fornecedor *fornecedores, int &n) {
    int total;

    if (n > 18) n = 18;
    //else n = n;
    // Cria uma lista com capacidade de suportar 18 linhas
    *lista = new ListaFornecedor[n];
    for (int i = 0; i < n; i++) {
        (*lista)[i].lin = 9 + i;
        (*lista)[i].cursorSobre = false;
        (*lista)[i].corFundo = WHITE;
        (*lista)[i].corLetra = DARKGRAY;
        //(*lista)[i].fornecedor = NULL;
        (*lista)[i].fornecedor = &fornecedores[i];
    }
    /*
    for (int i = 0; i < n; i++)
        (*lista)[i].fornecedor = &fornecedores[i];
    */
    (*lista)[0].cursorSobre = true;
}

void inicializarConsultaFornecedor(CaixaTexto *caixa, Botao *botoes) {
    botoes[0].elemento.texto = new char[14];
    strcpy(botoes[0].elemento.texto, "< CONSULTAR >");
    botoes[1].elemento.texto = new char[9];
    strcpy(botoes[1].elemento.texto, "< SAIR >");

    for (int i = 0; i < 2; i++) {
        botoes[i].cursorSobre = false;
        botoes[i].elemento.col = 80 + 26 * i;
        botoes[i].elemento.lin = 5;
        botoes[i].elemento.corFundo = LIGHTGRAY;
        botoes[i].elemento.corLetra = DARKGRAY;
    }
    caixa[0].nome.texto = new char[16];
    strcpy(caixa[0].nome.texto, "Nome da empresa");
    caixa[0].nome.corFundo = LIGHTBLUE;
    caixa[0].nome.corLetra = WHITE;
    caixa[0].nome.col = 5;
    caixa[0].nome.lin = 5;
    caixa[0].cursorSobre = true;
    caixa[0].conteudo = NULL;
    caixa[0].caixa.col = caixa[0].nome.col + 18;
    caixa[0].caixa.lin = caixa[0].nome.lin;
    caixa[0].caixa.cor = WHITE;
    caixa[0].caixa.altura = 1;
    caixa[0].caixa.largura = 52;
}

void inicializarTelaFCP(Botao *opcoes) {
    opcoes[0].elemento.texto = new char[11];
    strcpy(opcoes[0].elemento.texto, " Consulta ");
    opcoes[1].elemento.texto = new char[11];
    strcpy(opcoes[1].elemento.texto, " Cadastro ");
    opcoes[2].elemento.texto = new char[11];
    strcpy(opcoes[2].elemento.texto, "< Voltar >");

    for (int i = 0; i < 2; i++) {
        opcoes[i].elemento.corFundo = LIGHTGRAY;
        opcoes[i].elemento.corLetra = DARKGRAY;
        opcoes[i].elemento.col = 5 + 12 * i;
        opcoes[i].elemento.lin = 2;
    }
    opcoes[0].cursorSobre = true;
    opcoes[1].cursorSobre = false;
    opcoes[2].cursorSobre = false;

    opcoes[2].elemento.corFundo = LIGHTGRAY;
    opcoes[2].elemento.corLetra = DARKGRAY;
    opcoes[2].elemento.col = 105;
    opcoes[2].elemento.lin = 2;
}

// Bot�es de Editar, Excluir e Voltar no centro da tela.
void inicializarTelaEEV(Botao *opcoes) {
    opcoes[0].elemento.texto = new char[9];
    strcpy(opcoes[0].elemento.texto, " Editar ");
    opcoes[1].elemento.texto = new char[10];
    strcpy(opcoes[1].elemento.texto, " Excluir ");
    opcoes[2].elemento.texto = new char[11];
    strcpy(opcoes[2].elemento.texto, "< Voltar >");

    for (int i = 0; i < 3; i++) {
        opcoes[i].elemento.corFundo = LIGHTGRAY;
        opcoes[i].elemento.corLetra = DARKGRAY;
        opcoes[i].elemento.col = 36 + 12 * i;
        opcoes[i].elemento.lin = 12;
    }
    opcoes[0].cursorSobre = true;
    opcoes[1].cursorSobre = false;
    opcoes[2].cursorSobre = false;
}

void inicializarTelaEV(Botao *opcoes) {
    opcoes[0].elemento.texto = new char[9];
    strcpy(opcoes[0].elemento.texto, " Excluir ");
    opcoes[1].elemento.texto = new char[10];
    strcpy(opcoes[1].elemento.texto, "< Voltar >");

    for (int i = 0; i < 2; i++) {
        opcoes[i].elemento.corFundo = LIGHTGRAY;
        opcoes[i].elemento.corLetra = DARKGRAY;
        opcoes[i].elemento.col = 42 + 12 * i;
        opcoes[i].elemento.lin = 12;
    }
    opcoes[0].cursorSobre = true;
    opcoes[1].cursorSobre = false;
}

void inicializarMenuPrincipal(Botao *opcoes) {
    opcoes[0].elemento.texto = new char[10];
    strcpy(opcoes[0].elemento.texto, " Estoque ");
    opcoes[1].elemento.texto = new char[13];
    strcpy(opcoes[1].elemento.texto, " Fornecedor ");
    opcoes[2].elemento.texto = new char[10];
    strcpy(opcoes[2].elemento.texto, " Cliente ");
    opcoes[3].elemento.texto = new char[8];
    strcpy(opcoes[3].elemento.texto, " Venda ");
    opcoes[4].elemento.texto = new char[12];
    strcpy(opcoes[4].elemento.texto, " Relatorio ");
    opcoes[5].elemento.texto = new char[7];
    strcpy(opcoes[5].elemento.texto, " Sair ");

    for (int i = 0; i < 6; i++) {
        opcoes[i].elemento.corFundo = LIGHTGRAY;
        opcoes[i].elemento.corLetra = DARKGRAY;
        opcoes[i].elemento.col = 5 + 15 * i;
        opcoes[i].elemento.lin = 2;
        opcoes[i].cursorSobre = false;
    }
    opcoes[2].elemento.col += 3;
    opcoes[3].elemento.col += 3;
    opcoes[4].elemento.col += 1;
    opcoes[5].elemento.col += 3;
    opcoes[0].cursorSobre = true;
}

bool tratarErroCadC(int cod) {
    bool prosseguir = false;
    const int c = 115;
    gotoxy(64, 26);
    limparTela(4, 26, 1, 112, LIGHTBLUE);
    textbackground(LIGHTGRAY);
    textcolor(RED);
    if (cod == ERRO_NADA_ESCRITO) {
        char *texto = "ERRO: Preencha todas as caixas de texto";
        int tam = strlen(texto);
        gotoxy(c-tam, 26);
        printf(texto);
    } else if (cod == ERRO_CODIGO_EXISTENTE) {
        char *texto = "ERRO: ID existente";
        int tam = strlen(texto);
        gotoxy(c-tam, 26);
        printf(texto);
    } else if (cod == ERRO_NUMERO_INVALIDO) {
        char *texto = "ERRO: Insira o CPF corretamente";
        int tam = strlen(texto);
        gotoxy(c-tam, 26);
        printf(texto);
    } else if (cod == SEM_ERRO) {
        textcolor(GREEN);
        char *texto = "Cliente cadastrado com sucesso!";
        int tam = strlen(texto);
        gotoxy(c-tam, 26);
        printf(texto);
        prosseguir = true;
    }

    return prosseguir;
}

void atualizarEstados(ListaProduto *elemento, int opcao, int n) {
    for (int i = 0; i < n; i++)
        elemento[i].cursorSobre = false;
    elemento[opcao].cursorSobre = true;
}

void atualizarEstados(ListaVenda *elemento, int opcao, int n) {
    for (int i = 0; i < n; i++)
        elemento[i].cursorSobre = false;
    elemento[opcao].cursorSobre = true;
}

void atualizarEstados(ListaCliente *elemento, int opcao, int n) {
    for (int i = 0; i < n; i++) {
        elemento[i].cursorSobre = false;
    }
    elemento[opcao].cursorSobre = true;
}

/*
 * Desenha uma caixa de texto: seu r�tulo, sua �rea de escrever e, por fim,
 * o seu conte�do.
 */
void desenharCaixasTexto(const CaixaTexto *caixas, int n) {
    for (int i = 0; i < n; i++) {
        gotoxy(caixas[i].nome.col, caixas[i].nome.lin);
        textbackground(caixas[i].nome.corFundo);
        textcolor(caixas[i].nome.corLetra);
        printf("%s", caixas[i].nome.texto);
        gotoxy(caixas[i].caixa.col, caixas[i].caixa.lin);
        int corFundo = caixas[i].caixa.cor;
        int corLetra = BLACK;
        if (caixas[i].cursorSobre) {
            corFundo = LIGHTGRAY;
            corLetra = WHITE;
        }
        textbackground(corFundo);
        for (int j = 0; j < caixas[i].caixa.largura; j++)
            printf(" ");
        if (caixas[i].conteudo != NULL) {
            gotoxy(caixas[i].caixa.col+1, caixas[i].caixa.lin);
            textcolor(corLetra);
            printf("%s", caixas[i].conteudo);
        }
    }
}

/*
 * Desenha a seguinte borda:
 *  ____________________
 * |
 * |
 * |
 * |
 * |
 * |
 * |
 */
void desenharBordaEsqTopo(int c, int l, int h, int w, int cor) {
    textcolor(cor);
    gotoxy(c, l);
    printf("%c", BORDA_SUP_ESQUERDA);
    for (int i = 1; i < w-1; i++)
        printf("%c", LINHA_HORIZONTAL);
    for (int i = 1; i < h-1; i++) {
        gotoxy(c, l+i);
        printf("%c", LINHA_VERTICAL);
    }
    gotoxy(c, l+h-1);
    printf("%c", BORDA_INF_ESQUERDA);
}

/*
 * Desenha a seguinte borda:
 *                             |
 *                             |
 *                             |
 *                             |
 *                             |
 *                             |
 *         ____________________|
 */
void desenharBordaDirBaixo(int c, int l, int h, int w, int cor) {
    textcolor(cor);
    gotoxy(c+1, l+h-1);
    for (int i = 1; i < w-1; i++)
        printf("%c", LINHA_HORIZONTAL);
    printf("%c", BORDA_INF_DIREITA);
    gotoxy(c+w-1, l);
    printf("%c", BORDA_SUP_DIREITA);
    for (int i = 1; i < h-1; i++) {
        gotoxy(c+w-1, l+i);
        printf("%c", LINHA_VERTICAL);
    }
}

/*
 * Desenha uma caixa de sele��o a partir da defini��o de uma vari�vel do tipo
 * CheckBox.
 */
void desenharCheckBox(const CheckBox *opcoes, int n) {
    for (int i = 0; i < n; i++) {
        gotoxy(opcoes[i].elemento.col, opcoes[i].elemento.lin);
        textbackground(opcoes[i].elemento.corFundo);
        textcolor(opcoes[i].elemento.corLetra);
        printf("%s", opcoes[i].elemento.texto);
        if (opcoes[i].marcado) {
            gotoxy(opcoes[i].elemento.col+1, opcoes[i].elemento.lin);
            textcolor(LIGHTGREEN);
            printf("X");
        }
    }
}

/*
 * Desenha um botao a partir da defini��o de uma vari�vel do tipo Botao.
 */
void desenharBotao(const Botao *botoes, int n) {
    for (int i = 0; i < n; i++) {
        int corFundo = botoes[i].elemento.corFundo;
        int corLetra = botoes[i].elemento.corLetra;
        if (botoes[i].cursorSobre) {
            corFundo = BLUE;
            corLetra = WHITE;
        }
        gotoxy(botoes[i].elemento.col, botoes[i].elemento.lin);
        textbackground(corFundo);
        textcolor(corLetra);
        printf("%s", botoes[i].elemento.texto);
    }
}

void desenharLista(const ListaFornecedor *lista, int n) {
    int bytes;

    for (int i = 0; i < n; i++) {
        int corFundo = lista[i].corFundo;
        int corLetra = lista[i].corLetra;
        if (lista[i].cursorSobre) {
            corFundo = BLUE;
            corLetra = WHITE;
        }
        textbackground(corFundo);
        textcolor(corLetra);
        gotoxy(3, lista[i].lin);
        bytes = printf("%d", lista[i].fornecedor->codigo);
        for (int i = bytes; i < 11; i++)
            printf(" ");
        gotoxy(14, lista[i].lin);
        bytes = printf("%s", lista[i].fornecedor->empresaNome);
        for (int i = bytes; i < 44; i++)
            printf(" ");
        gotoxy(58, lista[i].lin);
        bytes = printf("%s", lista[i].fornecedor->responsavel);
        for (int i = bytes; i < 44; i++)
            printf(" ");
        gotoxy(102, lista[i].lin);
        printf("%s", lista[i].fornecedor->contato);
    }
}

void desenharLista(const ListaProduto *lista, int n) {
    int bytes;

    for (int i = 0; i < n; i++) {
        int corFundo = lista[i].corFundo;
        int corLetra = lista[i].corLetra;
        if (lista[i].cursorSobre) {
            corFundo = BLUE;
            corLetra = WHITE;
        }
        textbackground(corFundo);
        textcolor(corLetra);
        gotoxy(3, lista[i].lin);
        bytes = printf("%d", lista[i].produto->id);
        for (int i = bytes; i < 11; i++)
            printf(" ");
        gotoxy(14, lista[i].lin);
        bytes = printf("%s", lista[i].produto->nome);
        for (int i = bytes; i < 33; i++)
            printf(" ");
        gotoxy(47, lista[i].lin);
        bytes = printf("%d", lista[i].produto->Quantidade);
        for (int i = bytes; i < 26; i++)
            printf(" ");
        gotoxy(73, lista[i].lin);
        bytes = printf("%.2f", lista[i].produto->preco);
        for (int i = bytes; i < 22; i++)
            printf(" ");
        gotoxy(95, lista[i].lin);
        bytes = printf("%s", lista[i].produto->categoria);
        for (int i = bytes; i < 22; i++)
            printf(" ");
    }
}

void desenharLista(const ListaVenda *lista, int n) {
    int bytes;

    for (int i = 0; i < n; i++) {
        int corFundo = lista[i].corFundo;
        int corLetra = lista[i].corLetra;
        if (lista[i].cursorSobre) {
            corFundo = BLUE;
            corLetra = WHITE;
        }
        textbackground(corFundo);
        textcolor(corLetra);
        gotoxy(11, lista[i].lin);
        bytes = printf("%d", lista[i].item->codigoProduto);
        for (; bytes < 10; bytes++)
            printf(" ");
        bytes = printf("%s", lista[i].item->descricao);
        for (; bytes < 34; bytes++)
            printf(" ");
        bytes = printf("%.2f", lista[i].item->precoUnitario);
        for (; bytes < 20; bytes++)
            printf(" ");
        bytes = printf("%d", lista[i].item->quantidade);
        for (; bytes < 18; bytes++)
            printf(" ");
        bytes = printf("%.2f", lista[i].item->subtotal);
        for (; bytes < 12; bytes++)
            printf(" ");
    }
}

void desenharLista(const ListaCliente *lista, int n) {
    int bytes;

    // Limita a itera��o at� o n�mero real de clientes (n)
    for (int i = 0; i < n; i++) {
        int corFundo = lista[i].corFundo;
        int corLetra = lista[i].corLetra;

        // Altera as cores se o cursor estiver sobre o item
        if (lista[i].cursorSobre) {
            corFundo = BLUE;
            corLetra = WHITE;
        }

        // Configura a cor de fundo e de texto
        textbackground(corFundo);
        textcolor(corLetra);

        // Desenha o ID do cliente
        gotoxy(3, lista[i].lin);
        bytes = printf("%d", lista[i].cliente->id);
        for (int i = bytes; i < 11; i++)  // Preenche o espa�o restante at� 11 caracteres
            printf(" ");

        // Desenha o nome do cliente
        gotoxy(14, lista[i].lin);
        bytes = printf("%s", lista[i].cliente->nome);
        for (int i = bytes; i < 53; i++)  // Preenche o espa�o restante at� 53 caracteres
            printf(" ");

        // Desenha o CPF do cliente
        gotoxy(67, lista[i].lin);
        bytes = printf("%s", lista[i].cliente->cpf);
        for (int i = bytes; i < 50; i++)  // Preenche o espa�o restante at� 50 caracteres
            printf(" ");
    }
}

void desenharBorda() {
    desenharBordaEsqTopo(0, 0, LINHA_MAX, COLUNA_MAX, WHITE);
    desenharBordaDirBaixo(0, 0, LINHA_MAX, COLUNA_MAX, WHITE);
}

/*
 * Desenha uma borda a partir da coluna c e linha l do terminal, percorrendo
 * uma altura h a partir de l (l + h) e uma largura w a partir de c (c + w).
 */
void desenharBorda(int c, int l, int h, int w, int cor) {
    desenharBordaEsqTopo(c, l, h, w, cor);
    desenharBordaDirBaixo(c, l, h, w, cor);
}

void desenharPrecoTotal(float preco) {
    int bytes;
    gotoxy(93, 27);
    textbackground(DARKGRAY);
    textcolor(LIGHTGRAY);
    bytes = printf("%.2f", preco);
    for (; bytes < 12; bytes++)
        printf(" ");
}
/*
 * Limpa a tela posicionando o cursor na coluna c e linha l, percorrendo uma
 * altura de h a partir de l (l + h) e uma largura de w a partir de c (c + w).
 */
void limparTela(int c, int l, int h, int w) {
    textbackground(LIGHTBLUE);
    for (int i = l; i < (l + h); i++) {
        gotoxy(c, i);
        for (int j = c; j < (c + w); j++)
            printf(" ");
    }
}

void limparTela(int c, int l, int h, int w, int cor) {
    textbackground(cor);
    for (int i = l; i < (l + h); i++) {
        gotoxy(c, i);
        for (int j = c; j < (c + w); j++)
            printf(" ");
    }
}

void limparMemoria(Botao *elemento, int n) {
    for (int i = 0; i < n; i++)
        delete[] elemento[i].elemento.texto;
}

void limparMemoria(CaixaTexto *elemento, int n) {
    for (int i = 0; i < n; i++) {
        delete[] elemento[i].nome.texto;
        if (elemento[i].conteudo != NULL)
            delete[] elemento[i].conteudo;
    }
}
/*
 * Limpa mem�ria e esvazia o conte�do de uma caixa de texto
 */
void limparConteudoCaixa(CaixaTexto *elemento, int n) {
    for (int i = 0; i < n; i++) {
        delete[] elemento[i].conteudo;
        elemento[i].conteudo = NULL;
    }
}

void limparMemoria(char **elemento, int n) {
    for (int i = 0; i < n; i++) {
        if (elemento[i] != NULL)
            delete[] elemento[i];
    }
}

void limparMemoria(ListaFornecedor *elemento) {
    delete[] elemento;
}

void limparMemoria(ListaProduto *elemento) {
    delete[] elemento;
}

void limparListaVenda(ListaVenda *lista, int &n) {
    for (int i = 0; i < n; i++)
        lista[i].item = NULL;
    n = 0;
}

void limparMemoria(ListaCliente *elemento) {
    delete[] elemento;
}

#endif // INTERFACE_H
