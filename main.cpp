#include <iostream>
#include <locale.h>
#include <cstring>
#include "interface.h"
#include "fornecedores.h"
#include "clientes.h"
#include "vendas.h"
#include "gestao_estoque.h"
#include "relatorio_vendas.h"
#include "relatorio_estoque.h"

using namespace std;

void telaInicial();

int main() {
    REMOVER_CURSOR();

    telaInicial();

    return 0;
}

int telaEditarExcluirVoltar (){
    int opcao = 0;
    bool enter;
    Botao opcoes[3];

    inicializarTelaEEV(opcoes);
    limparTela(30, 10, 5, 46, LIGHTBLUE);
    desenharBotao(opcoes, 3);
    while (true) {
        enter = false;
        logicaTelaFCP(&opcao, &enter);
        atualizarEstados(opcoes, opcao, 3);
        if (enter) {
            return opcao;
        }
        desenharBotao(opcoes, 3);
    }
    limparMemoria(opcoes, 3);
}

bool telaIdCliente(TabelaCliente &clientes, Venda &venda) {
    int val;
    bool enter;
    CaixaTexto caixas[1];

    inicializarTelaIdCliente(caixas);
    limparTela(49, 12, 4, 22, LIGHTBLUE);
    desenharCaixasTexto(caixas, 1);
    while (true) {
        enter = intr::lerDados(caixas[0], caixas[0].conteudo, 9);
        if (enter == false) return false;
        if (caixas[0].conteudo == NULL) break;
        val = atoi(caixas[0].conteudo);
        if (val > 0 && clientes.dados != NULL && encontrarIdCliente(clientes, val)) {
            venda.codigoCliente = val;
            break;
        } else {
            textbackground(LIGHTGRAY);
            textcolor(RED);
            gotoxy(55, 28);
            printf("ERRO: Insira um ID valido ou finalize com ID vazio");
        }
    }
    limparMemoria(caixas, 1);
    return true;
}

int telaRemocaoitem(ListaVenda *lista, Venda &venda, int &linhas) {
    int opcao, desliza, total = venda.qtdLista;
    int indice = -1;
    bool enter, esc;

    if (linhas <= 0) return indice;
    lista[0].cursorSobre = true;
    desenharLista(lista, linhas);
    opcao = 0;
    desliza = 0;
    while (true) {
        enter = false;
        esc = false;

        logicaListaItens(&opcao, &desliza, linhas, total, &enter, &esc);
        if (desliza >=0)
            deslizarLista(lista, venda, desliza, linhas);
        atualizarEstados(lista, opcao, linhas);
        if (enter) {
            indice = opcao + desliza;
            break;
        }
        if (esc || linhas <= 0)
            break;
        desenharLista(lista, linhas);
    }
    lista[opcao].cursorSobre = false;
    return indice;
}

void telaVendas() {
    int opcao, linhas = 0;;
    bool enter;
    CaixaTexto caixas[2];
    Botao botoes[4];
    TabelaProduto tabela;
    TabelaCliente tabelaC;
    Venda venda;
    ListaVenda lista[18];

    inicializarVenda(venda);
    carregarClientes(tabelaC);
    inicializarTelaVenda(caixas, botoes, lista);
    carregarProdutos(tabela);
    textbackground(LIGHTBLUE);
    clrscr();
    desenharBorda();
    // Tabela
    gotoxy(11, 8);
    textbackground(LIGHTGRAY);
    textcolor(DARKGRAY);
    printf("ID        Produto                           Preco (Unit)        Quantidade        Subtotal    ");
    limparTela(11, 9, 18, 94, WHITE);
    gotoxy(87, 27);
    textbackground(DARKGRAY);
    textcolor(LIGHTGRAY);
    printf("Total             ");

    if (carregarListaVenda(venda)) {
        linhas = venda.qtdLista;
        if (linhas > 18) linhas = 18;
        atualizarListaVenda(lista, venda, linhas);
        desenharLista(lista, linhas);
    }
    desenharBotao(botoes, 4);
    desenharCaixasTexto(caixas, 2);
    desenharPrecoTotal(venda.totalVenda);

    opcao = 0;
    // Lógica do cliente aqui não concluída
    venda.codigoCliente = 0;
    while (true) {
        enter = false;

        logicaTelaVenda(&opcao, &enter);
        atualizarEstados(caixas, botoes, opcao, 2, 4);
        if (enter) {
            if (opcao == 0 || opcao == 1) {
                intr::lerDados(caixas[opcao], caixas[opcao].conteudo, 9);
            } else if (opcao == 2) {
                char *dados[2] = {caixas[0].conteudo, caixas[1].conteudo};
                int indice, qtd;
                if (tratarErroVenda(efetuarPesquisa(indice, qtd, dados, tabela))) {
                    adicionarItem(tabela, venda, indice, qtd);
                    removerEstoqueId(tabela, tabela.dados[indice].id, qtd);
                    armazenarListaVenda(venda.itens[venda.qtdLista-1]);
                    linhas++;
                    if (linhas > 18) linhas = 18;
                    else atualizarListaVenda(lista, venda, linhas);
                    limparConteudoCaixa(caixas, 2);
                }
            } else if (opcao == 3) {
                int indice = telaRemocaoitem(lista, venda, linhas);
                if (indice != -1) {
                    efetuarRemocaoVenda(tabela, venda, indice);
                    if (venda.qtdLista < 18) linhas--;
                    if (linhas == 0) excluirListaVenda();
                    atualizarListaVenda(lista, venda, linhas);
                }
                limparTela(11, 9, 18, 94, WHITE);
            } else if (opcao == 4) {
                if (linhas > 0) {
                    if (telaIdCliente(tabelaC, venda)) {
                        salvarVenda(&venda);
                        resetarVenda(venda);
                        limparListaVenda(lista, linhas);
                        excluirListaVenda();
                        carregamentoVendas();
                        limparTela(11, 9, 18, 94, WHITE);
                    } else
                        limparTela(49, 12, 4, 22, WHITE);
                    limparTela(11, 28, 1, 94, LIGHTBLUE);
                }
            } else break;
        }
        desenharBotao(botoes, 4);
        desenharCaixasTexto(caixas, 2);
        desenharLista(lista, linhas);
        desenharPrecoTotal(venda.totalVenda);
    }
    LiberarMemoria(tabela);
    limparMemoria(botoes, 4);
    limparMemoria(caixas, 2);
}

bool telaEditar(Produto &f) {
    int opcao;
    bool enter, salvo = false;
    CaixaTexto caixas[4];
    Botao botoes[2];

    inicializarEditarProduto(caixas, botoes, f);
    limparTela(18, 10, 9, 80, LIGHTBLUE);
    desenharBotao(botoes, 2);
    desenharCaixasTexto(caixas, 4);

    opcao = 0;
    while (true) {
        enter = false;

        logicaEditarProduto(&opcao, &enter);
        atualizarEstados(caixas, botoes, opcao, 4, 2);
        if (enter) {
            if (opcao == 0) {
                strng::lerDados(caixas[opcao], caixas[opcao].conteudo, NOME_TAM);
            } else if (opcao == 1) {
                flt::lerDados(caixas[opcao], &caixas[opcao].conteudo, 9);
            } else if (opcao == 2) {
                intr::lerDados(caixas[opcao], caixas[opcao].conteudo, 9);
            } else if (opcao == 3) {
                strng::lerDados(caixas[opcao], caixas[opcao].conteudo, TIPO_TAM);
            } else if (opcao == 4) {
                char *produto[4] = {
                    caixas[0].conteudo, caixas[1].conteudo,
                    caixas[2].conteudo, caixas[3].conteudo
                    };
                if(tratarErroEdicao(verificarEdicaoP(produto))) {
                    strcpy(f.nome, produto[0]);
                    f.preco = atof(produto[1]);
                    f.Quantidade = atoi(produto[2]);
                    strcpy(f.categoria, produto[3]);
                    salvo = true;
                    break;
                }
            } else
                break;
        }
        desenharCaixasTexto(caixas, 4);
        desenharBotao(botoes, 2);
    }

    limparMemoria(caixas, 3);
    limparMemoria(botoes, 2);

    return salvo;
}

void listaProduto(char *filtro) {
    int opcao, desliza, linhas;
    bool enter, esc, pesquisa = false;
    TabelaProduto tabela;
    ListaProduto *lista;

    carregarProdutos(tabela);
    if (filtro != NULL) {
        efetuarPesquisa(tabela, filtro);
        pesquisa = true;
    }
    if (tabela.qtd == 0)
        return;             // Se tabela vazia, não faz sentido prosseguir
    linhas = tabela.qtd;
    inicializarListaProduto(&lista, tabela.dados, linhas);
    desenharLista(lista, linhas);

    opcao = 0;
    desliza = 0;
    while (true) {
        enter = false;
        esc = false;

        logicaListaProduto(&opcao, &desliza, linhas, tabela.qtd, &enter, &esc);
        if (desliza >= 0) {
            deslizarLista(lista, tabela.dados, desliza, linhas);
        }
        atualizarEstados(lista, opcao, linhas);
        if (enter) {
            int escolha = telaEditarExcluirVoltar();
            int indice = opcao + desliza;
            if (escolha == PRODUTO_EDITAR) {
                if (telaEditar(tabela.dados[indice])) {
                    int linhasAntiga = tabela.qtd;
                    if (pesquisa) {
                        TabelaProduto tabelaTmp;
                        carregarProdutos(tabelaTmp);
                        efetuarEdicao(tabelaTmp, tabela.dados[indice], indice);
                        efetuarPesquisa(tabela, filtro);
                        linhas = tabela.qtd;
                        if (linhas < linhasAntiga) opcao--;
                        if (opcao >= 0) {
                            deslizarLista(lista, tabela.dados, desliza, linhas);
                            atualizarEstados(lista, opcao, linhas);
                        }
                        LiberarMemoria(tabelaTmp);
                    } else {
                        efetuarEdicao(tabela);
                    }
                }
            } else if (escolha == Produto_EXCLUIR) {
                if (pesquisa) {
                    TabelaProduto tabelaTmp;
                    carregarProdutos(tabelaTmp);
                    efetuarExclusao(tabelaTmp, tabela.dados[indice].id);
                    corrigirTabelaInterface(tabela, tabela.dados[indice].id);
                    LiberarMemoria(tabelaTmp);
                } else
                    efetuarExclusao(tabela, tabela.dados[indice].id);
                if (tabela.qtd < LINHAS_MAXIMAS_LISTA) {
                    linhas = tabela.qtd;    // Reduz a quantidade de linha
                    opcao--;                // Faz a opção estar sobre o elemento anterior
                    if (opcao < 0) opcao = 0;
                } else if (desliza > 0)
                    desliza--;              // Faz todos os elementos da lista descer uma unidade
                deslizarLista(lista, tabela.dados, desliza, linhas);
                atualizarEstados(lista, opcao, linhas);
            }
            limparTela(3, 9, 18, 114, WHITE);
        }
        if (esc || tabela.qtd <= 0)
            break;
        desenharLista(lista, linhas);
    }

    LiberarMemoria(tabela);
    limparMemoria(lista);
}

void telaConsultaProduto() {
    int opcao;
    bool enter;
    Botao botoes[2];
    CaixaTexto caixa[1];

    inicializarConsultaProduto(caixa, botoes);
    desenharBotao(botoes, 2);
    desenharCaixasTexto(caixa, 1);
    gotoxy(3, 8);
    textbackground(LIGHTGRAY);
    textcolor(DARKGRAY);
    printf("CODIGO     Nome                             Quantidade                Preco                 Categoria             ");
    limparTela(3, 9, 18, 114, WHITE);

    opcao = 0;
    while (true) {
        enter = false;

        logicaConsultarProduto(&opcao, &enter);
        atualizarEstados(caixa, botoes, opcao, 1, 2);
        if (enter) {
            if (opcao == 0) {
                strng::lerDados(caixa[opcao], caixa[opcao].conteudo, 50);
            } else if (opcao == 1) {
                listaProduto(caixa[0].conteudo);
                limparTela(3, 9, 18, 114, WHITE);
            } else
                break;
        }
        desenharCaixasTexto(caixa, 1);
        desenharBotao(botoes, 2);
    }
    limparMemoria(botoes, 2);
    limparMemoria(caixa, 1);
}

void telaCadastroProduto() {
    int opcao;
    bool enter;
    TabelaProduto tabela;
    CaixaTexto caixas[5];
    Botao botoes[2];

    inicializarCadastroProduto(caixas, botoes);
    carregarProdutos(tabela);
    desenharBotao(botoes, 2);
    desenharCaixasTexto(caixas, 5);

    opcao = 0;
    while (true) {
        enter = false;

        logicaCadastroProduto(&opcao, &enter);
        atualizarEstados(caixas, botoes, opcao, 5, 2);
        if (enter) {
            if (opcao == 0 || opcao == 1) {
                intr::lerDados(caixas[opcao], caixas[opcao].conteudo, 9);
            } else if (opcao == 2) {
                flt::lerDados(caixas[opcao], &caixas[opcao].conteudo, 9);
            } else if (opcao == 3) {
                strng::lerDados(caixas[opcao], caixas[opcao].conteudo, 32);
            } else if (opcao == 4) {
                strng::lerDados(caixas[opcao], caixas[opcao].conteudo, 16);
            }else if (opcao == 5) {
                char *dados[5] = {
                    caixas[0].conteudo, caixas[1].conteudo,
                    caixas[2].conteudo, caixas[3].conteudo,
                    caixas[4].conteudo
                    };
                if (tratarErroCadP(verificarCad(dados, tabela))) {
                    efetuarCadastro(dados, tabela);
                    limparConteudoCaixa(caixas, 5);
                }
            } else
                break;
        }
        desenharCaixasTexto(caixas, 5);
        desenharBotao(botoes, 2);
    }
    LiberarMemoria(tabela);
    limparMemoria(caixas, 5);
    limparMemoria(botoes, 2);
}

void telaProduto() {
    int opcao;
    bool enter;
    Botao opcoes[3];

    inicializarTelaFCP(opcoes);
    textbackground(LIGHTBLUE);
    clrscr();
    desenharBorda();
    desenharBorda(2, 2, 26, 116, WHITE);
    desenharBotao(opcoes, 3);
    gotoxy(50,1);
    printf(" Area Estoque ");

    opcao = 0;
    while (true) {
        enter = false;
        logicaTelaFCP(&opcao, &enter);
        atualizarEstados(opcoes, opcao, 3);
        if (enter) {
            if (opcao == 0)
                telaConsultaProduto();
            else if (opcao == 1)
                telaCadastroProduto();
            else
                break;
            limparTela(3, 4, 23, 114);
        }
        desenharBotao(opcoes, 3);
    }
    limparMemoria(opcoes, 3);
}

void telaCadastroFornecedor() {
    int opcao;
    bool enter;
    TabelaFornecedor tabela;
    CaixaTexto caixas[4];
    Botao botoes[2];

    inicializarCadastroFornecedor(caixas, botoes);
    carregarFornecedores(tabela);
    desenharBotao(botoes, 2);
    desenharCaixasTexto(caixas, 4);

    opcao = 0;
    while (true) {
        enter = false;

        logicaCadastroFornecedor(&opcao, &enter);
        atualizarEstados(caixas, botoes, opcao, 4, 2);
        if (enter) {
            if (opcao == 0) {
                intr::lerDados(caixas[opcao], caixas[opcao].conteudo, 9);
            } else if (opcao == 1 || opcao == 2) {
                strng::lerDados(caixas[opcao], caixas[opcao].conteudo, 42);
            } else if (opcao == 3) {
                intr::lerDados(caixas[opcao], caixas[opcao].conteudo, 11);
            } else if (opcao == 4) {
                char *dados[4] = {
                    caixas[0].conteudo, caixas[1].conteudo,
                    caixas[2].conteudo, caixas[3].conteudo
                    };
                if (tratarErroCad(verificarCad(dados, tabela))) {
                    efetuarCadastro(dados, tabela);
                    limparConteudoCaixa(caixas, 4);
                }
            } else
                break;
        }
        desenharCaixasTexto(caixas, 4);
        desenharBotao(botoes, 2);
    }
    LiberarMemoria(tabela);
    limparMemoria(caixas, 4);
    limparMemoria(botoes, 2);
}

bool telaEditar(Fornecedor &f) {
    int opcao;
    bool enter, salvo = false;
    CaixaTexto caixas[3];
    Botao botoes[2];

    inicializarEditarFornecedor(caixas, botoes, f);
    limparTela(18, 10, 9, 80, LIGHTBLUE);
    desenharBotao(botoes, 2);
    desenharCaixasTexto(caixas, 3);

    opcao = 0;
    while (true) {
        enter = false;

        logicaEditarFornecedor(&opcao, &enter);
        atualizarEstados(caixas, botoes, opcao, 3, 2);
        if (enter) {
            if (opcao == 0 || opcao == 1) {
                strng::lerDados(caixas[opcao], caixas[opcao].conteudo, 42);
            } else if (opcao == 2) {
                intr::lerDados(caixas[opcao], caixas[opcao].conteudo, 11);
            } else if (opcao == 3) {
                char *fornecedor[3] = {NULL, NULL, NULL};
                fornecedor[0] = caixas[0].conteudo;
                fornecedor[1] = caixas[1].conteudo;
                fornecedor[2] = caixas[2].conteudo;
                if(tratarErroEdicao(verificarEdicao(fornecedor))) {
                    strcpy(f.empresaNome, fornecedor[0]);
                    strcpy(f.responsavel, fornecedor[1]);
                    strcpy(f.contato, fornecedor[2]);
                    formataContato(f.contato);
                    salvo = true;
                    break;
                }
            } else
                break;
        }
        desenharCaixasTexto(caixas, 3);
        desenharBotao(botoes, 2);
    }
    limparMemoria(caixas, 3);
    limparMemoria(botoes, 2);

    return salvo;
}

void listaFornecedor(char *filtro) {
    int opcao, desliza, linhas;
    bool enter, esc, pesquisa = false;
    TabelaFornecedor tabela;
    ListaFornecedor *lista;

    carregarFornecedores(tabela);
    if (filtro != NULL) {
        efetuarPesquisa(tabela, filtro);
        pesquisa = true;
    }
    if (tabela.qtd == 0)
        return;             // Se tabela vazia, não faz sentido prosseguir
    linhas = tabela.qtd;
    inicializarListaFornecedor(&lista, tabela.dados, linhas);
    desenharLista(lista, linhas);
    opcao = 0;
    desliza = 0;
    while (true) {
        enter = false;
        esc = false;

        logicaListaFornecedor(&opcao, &desliza, linhas, tabela.qtd, &enter, &esc);
        if (desliza >= 0) {
            deslizarLista(lista, tabela.dados, desliza, linhas);
        }
        atualizarEstados(lista, opcao, linhas);
        if (enter) {
            int escolha = telaEditarExcluirVoltar();
            int indice = opcao + desliza;
            if (escolha == FORNECEDOR_EDITAR) {
                if (telaEditar(tabela.dados[indice])) {
                    int linhasAntiga = tabela.qtd;
                    if (pesquisa) {
                        TabelaFornecedor tabelaTmp;
                        carregarFornecedores(tabelaTmp);
                        efetuarEdicao(tabelaTmp, tabela.dados[indice], indice);
                        efetuarPesquisa(tabela, filtro);
                        linhas = tabela.qtd;
                        if (linhas < linhasAntiga) opcao--;
                        if (opcao >= 0) {
                            deslizarLista(lista, tabela.dados, desliza, linhas);
                            atualizarEstados(lista, opcao, linhas);
                        }
                        LiberarMemoria(tabelaTmp);
                    } else {
                        efetuarEdicao(tabela);
                    }
                }
            } else if (escolha == FORNECEDOR_EXCLUIR) {
                if (pesquisa) {
                    TabelaFornecedor tabelaTmp;
                    carregarFornecedores(tabelaTmp);
                    efetuarExclusao(tabelaTmp, tabela.dados[indice].codigo);
                    corrigirTabelaInterface(tabela, tabela.dados[indice].codigo);
                    LiberarMemoria(tabelaTmp);
                } else
                    efetuarExclusao(tabela, tabela.dados[indice].codigo);
                if (tabela.qtd < LINHAS_MAXIMAS_LISTA) {
                    linhas = tabela.qtd;    // Reduz a quantidade de linha
                    opcao--;                // Faz a opção estar sobre o elemento anterior
                    if (opcao < 0) opcao = 0;
                } else if (desliza > 0)
                    desliza--;              // Faz todos os elementos da lista descer uma unidade
                deslizarLista(lista, tabela.dados, desliza, linhas);
                atualizarEstados(lista, opcao, linhas);
            }
            limparTela(3, 9, 18, 114, WHITE);
        }
        if (esc || tabela.qtd <= 0)
            break;
        desenharLista(lista, linhas);
    }
    LiberarMemoria(tabela);
    limparMemoria(lista);
}

void telaConsultaFornecedor() {
    int opcao;
    bool enter;
    Botao botoes[2];
    CaixaTexto caixa[1];

    inicializarConsultaFornecedor(caixa, botoes);
    desenharBotao(botoes, 2);
    desenharCaixasTexto(caixa, 1);
    gotoxy(3, 8);
    textbackground(LIGHTGRAY);
    textcolor(DARKGRAY);
    printf("CODIGO     Nome da Empresa                             RESPONSAVEL                                 Telefone       ");
    limparTela(3, 9, 18, 114, WHITE);

    opcao = 0;
    while (true) {
        enter = false;

        logicaConsultarFornecedor(&opcao, &enter);
        atualizarEstados(caixa, botoes, opcao, 1, 2);
        if (enter) {
            if (opcao == 0) {
                strng::lerDados(caixa[opcao], caixa[opcao].conteudo, 50);
            } else if (opcao == 1) {
                listaFornecedor(caixa[0].conteudo);
                limparTela(3, 9, 18, 114, WHITE);
            } else
                break;
        }
        desenharCaixasTexto(caixa, 1);
        desenharBotao(botoes, 2);
    }
    limparMemoria(botoes, 2);
    limparMemoria(caixa, 1);
}

void telaFornecedor() {
    int opcao;
    bool enter;
    Botao opcoes[3];

    inicializarTelaFCP(opcoes);
    textbackground(LIGHTBLUE);
    clrscr();
    desenharBorda();
    desenharBorda(2, 2, 26, 116, WHITE);
    desenharBotao(opcoes, 3);
    gotoxy(50,1);
    printf(" Area Fornecedor ");

    opcao = 0;
    while (true) {
        enter = false;
        logicaTelaFCP(&opcao, &enter);
        atualizarEstados(opcoes, opcao, 3);
        if (enter) {
            if (opcao == 0)
                telaConsultaFornecedor();
            else if (opcao == 1)
                telaCadastroFornecedor();
            else
                break;
            limparTela(3, 4, 23, 114);
        }
        desenharBotao(opcoes, 3);
    }
    limparMemoria(opcoes, 3);
}

int telaExcluirVoltar (){
    int opcao = 0;
    bool enter;
    Botao opcoes[2];

    inicializarTelaEV(opcoes);
    limparTela(30, 10, 5, 46, LIGHTBLUE);
    desenharBotao(opcoes, 2);
    while (true) {
        enter = false;
        logicaTelaEditarVoltar(&opcao, &enter);
        atualizarEstados(opcoes, opcao, 2);
        if (enter) {
            return opcao;
        }
        desenharBotao(opcoes, 2);
    }
    limparMemoria(opcoes, 2);
}

void listaCliente(char *filtro) {
    int opcao, desliza, linhas;
    bool enter, esc, pesquisa = false;
    TabelaCliente tabela;
    ListaCliente *lista;

    carregarClientes(tabela);
    if (filtro != NULL) {
        efetuarPesquisa(tabela, filtro);
        pesquisa = true;
    }
    if (tabela.qtd == 0 || tabela.dados == NULL)
        return;
    linhas = tabela.qtd;
    inicializarListaClientes(&lista, tabela.dados, linhas);
    desenharLista(lista, linhas);
    opcao = 0;
    desliza = 0;

    while (true) {
        enter = false;
        esc = false;
        logicaListaCliente(&opcao, &desliza, linhas, tabela.qtd, &enter, &esc);
        if (desliza >= 0) {
            deslizarLista(lista, tabela.dados, desliza, linhas);
        }
        atualizarEstados(lista, opcao, linhas);
        if (enter) {
            int escolha = telaExcluirVoltar();
            int indice = opcao + desliza;
            if (escolha == CLIENTE_EXCLUIR) {
                if (pesquisa) {
                    TabelaCliente tabelaTmp;
                    carregarClientes(tabelaTmp);
                    efetuarExclusao(tabelaTmp, tabela.dados[indice].id);
                    corrigirTabelaInterface(tabela, tabela.dados[indice].id);
                    liberarMemoria(tabelaTmp);
                } else
                    efetuarExclusao(tabela, tabela.dados[indice].id);
                if (tabela.qtd < LINHAS_MAXIMAS_LISTA) {
                    linhas = tabela.qtd;    // Reduz a quantidade de linha
                    opcao--;                // Faz a opção estar sobre o elemento anterior
                    if (opcao < 0) opcao = 0;
                } else if (desliza > 0)
                    desliza--;              // Faz todos os elementos da lista descer uma unidade
                deslizarLista(lista, tabela.dados, desliza, linhas);
                atualizarEstados(lista, opcao, linhas);
            }
            limparTela(3, 9, 18, 114, WHITE);
        }
        if (esc)
            break;
        desenharLista(lista, linhas);
        gotoxy(0, 0);
    }
    liberarMemoria(tabela);
    limparMemoria(lista);
}

void telaConsultaCliente(){
    int opcao;
    bool enter;
    Botao botoes[2];
    CaixaTexto caixa[1];

    inicializarConsultaCliente(caixa, botoes);
    desenharBotao(botoes, 2);
    desenharCaixasTexto(caixa, 1);
    gotoxy(3, 8);
    textbackground(LIGHTGRAY);
    textcolor(DARKGRAY);
    printf("ID         Nome do cliente                                      CPF                                               ");
    limparTela(3, 9, 18, 114, WHITE);

    opcao = 0;
    while (true) {
        enter = false;

        logicaConsultarCliente(&opcao, &enter);
        atualizarEstados(caixa, botoes, opcao, 1, 2);
        if (enter) {
            if (opcao == 0) {
                strng::lerDados(caixa[opcao], caixa[opcao].conteudo, 50);
            } else if (opcao == 1) {
                listaCliente(caixa[0].conteudo);
                limparTela(3, 9, 18, 114, WHITE);
            } else
                break;
        }
        desenharCaixasTexto(caixa, 1);
        desenharBotao(botoes, 2);
    }
    limparMemoria(botoes, 2);
    limparMemoria(caixa, 1);
}

void telaCadastroCliente() {
    int opcao;
    bool enter;
    CaixaTexto caixas[3];
    Botao botoes[2];
    TabelaCliente tabela;

    inicializarCadastroCliente(caixas, botoes);
    carregarClientes(tabela);
    desenharBotao(botoes, 2);
    desenharCaixasTexto(caixas, 3);

    opcao = 0;
    while (true) {
        enter = false;
        logicaCadastroCliente(&opcao, &enter);
        atualizarEstados(caixas, botoes, opcao, 3, 2);
        if (enter) {
            if (opcao == 0) {
                intr::lerDados(caixas[opcao], caixas[opcao].conteudo, 9);
            } else if (opcao == 1) {
                strng::lerDados(caixas[opcao], caixas[opcao].conteudo, 64);
            } else if (opcao == 2) {
                strng::lerDados(caixas[opcao], caixas[opcao].conteudo, 14, true);
            } else if (opcao == 3) {
                char *clientes[3] = {caixas[0].conteudo, caixas[1].conteudo, caixas[2].conteudo};
                if (tratarErroCadC(verificarCadastro(clientes, tabela))) {
                    efetuarCadastro(tabela, clientes);
                    limparConteudoCaixa(caixas, 3);
                }
            } else break;
        }
        desenharCaixasTexto(caixas, 3);
        desenharBotao(botoes, 2);
    }
    liberarMemoria(tabela);
    limparMemoria(botoes, 2);
    limparMemoria(caixas, 3);
}

void telaCliente() {
    int opcao;
    bool enter;
    Botao opcoes[3];

    inicializarTelaFCP(opcoes);
    textbackground(LIGHTBLUE);
    clrscr();
    desenharBorda();
    desenharBorda(2, 2, 26, 116, WHITE);
    desenharBotao(opcoes, 3);
    gotoxy(50,1);
    printf(" Area cliente ");
    // Define qual tela esta durante toda a tela cliente

    opcao = 0;
    while (true) {
        enter = false;
        logicaTelaFCP(&opcao, &enter);
        atualizarEstados(opcoes, opcao, 3);
        if (enter) {
            if (opcao == 0) {
                telaConsultaCliente();
            } else if (opcao == 1) {
                telaCadastroCliente();
            } else
                break;
            limparTela(3, 4, 23, 114);
        }
        desenharBotao(opcoes, 3);
    }
    limparMemoria(opcoes, 3);
}

void telaApagarRegistros(){
    int opcao;
    bool enter;
    Botao botoes[4];

    limparTela(3, 9, 18, 114, LIGHTBLUE);
    limparTela(36, 10, 8, 50, WHITE);
    gotoxy(46, 11); cout<< "Qual relatorio deseja apagar?";

    inicializarApagarRelatorio(botoes);

    opcao = 0;
    botoes[0].cursorSobre = true;
    desenharBotao(botoes, 4);

        while(true){
            enter = false;
            logicaTelaApagarRegistro(&opcao, &enter);
            atualizarEstados(botoes, opcao, 4);
            if(enter){
                if(opcao == 0){
                    limpaArquivo(vendasLista);
                }else if(opcao == 1){
                    limpaArquivo(intervaloVendas);

                }else if(opcao == 2){
                    limpaArquivo(vendasTotais);

                }else{
                    limparTela(36, 10, 8, 50, LIGHTBLUE);
                    break;
                }
                Sleep(1100);
                limparTela(36, 10, 8, 50, LIGHTBLUE);
                break;
            }
            desenharBotao(botoes, 4);
        }
        delay(DELAY);

    limparMemoria(botoes, 4); //lembrar de colocar dentro de uma funcao sozinha

}

void telaEstoqueRegistro(){
    int opcao;
    bool enter;
    CaixaTexto caixas[2];
    Botao botoes[2];

    inicializarTelaRE(caixas, botoes);
    textbackground(LIGHTBLUE);
    desenharBorda(34, 8, 13, 52, WHITE);
    desenharBotao(botoes, 2);
    desenharCaixasTexto(caixas, 2);

    opcao = 0;
    while (true) {
        enter = false;
        logicaTelaEstoqueRelatorio(&opcao, &enter);
        atualizarEstados(caixas, botoes, opcao, 2, 2);
        if (enter) {
            if (opcao == 0) {
                strng::lerDados(caixas[opcao], caixas[opcao].conteudo, 42);
            } else if (opcao == 1) {
                strng::lerDados(caixas[opcao], caixas[opcao].conteudo, 24);
            } else if (opcao == 2){
                char *dados[2] = {caixas[0].conteudo, caixas[1].conteudo};
                limparTela(30, 6, 1, 50);
                if (dados[0] != NULL && dados[1] != NULL) {
                    if (gerador_relatorio(dados)) {
                        textbackground(LIGHTGRAY);
                        textcolor(GREEN);
                        gotoxy(45, 6); printf("Arquivo de relatorio gerado");
                        limparConteudoCaixa(caixas, 2);
                    } else {
                        textbackground(LIGHTGRAY);
                        textcolor(RED);
                        gotoxy(45, 6); printf("ERRO: nenhum produto encontrado");
                    }
                } else {
                    textbackground(LIGHTGRAY);
                    textcolor(RED);
                    gotoxy(45, 6); printf("ERRO: preencha todos os campos");
                }
            } else
                break;
        }
        desenharCaixasTexto(caixas, 2);
        desenharBotao(botoes, 2);
    }
    limparMemoria(caixas, 2);
    limparMemoria(botoes, 2);
}

void telaVendaRegistro(){
    int opcao;
    bool desenhar, enter;
    char *conteudo[2] = {NULL, NULL};
    CaixaTexto caixas[2];
    Botao botoes[4];


    inicializarTelasRegistro(caixas, botoes);
    desenharBotao(botoes, 4);
    desenharCaixasTexto(caixas, 2);

    limparTela(3, 9, 18, 114, WHITE);

    opcao = 0;
    while (true) {
        enter = false;
        logicaTelasRegistro(&opcao, &enter);
        atualizarEstados(caixas, botoes, opcao, 2, 4);
        if (enter) {
            if (opcao == 0) {
                intr::lerDados(caixas[opcao], conteudo[opcao],10, true);
                caixas[opcao].conteudo = conteudo[opcao];
            } else if (opcao == 1) {
                intr::lerDados(caixas[opcao], conteudo[opcao], 10, true);
                caixas[opcao].conteudo = conteudo[opcao];
            } else if (opcao == 2){
                if (caixas[0].conteudo == NULL && caixas[1].conteudo == NULL){
                    limparTela(3, 9, 18, 114, LIGHTGRAY);
                    gotoxy(4, 9); cout<< "DATA"; gotoxy(24, 9); cout<< "ID";
                    gotoxy(34, 9); cout<< "NOME  PRODUTO"; gotoxy(64, 9); cout<< "PRECO R$";
                    gotoxy(84, 9); cout<< "UNIDADES"; gotoxy(104, 9); cout<< "TOTAL R$";
                    exibirLista("relatorio_vendasLista.txt");

                }else{
                    if(strlen(caixas[0].conteudo) == 10 && strlen(caixas[0].conteudo) == 10){
                        if(gerarRelatorioIntervalo(caixas[0].conteudo, caixas[1].conteudo)){
                        limparTela(3, 9, 18, 114, LIGHTGRAY);
                        exibirListaIntervalo("relatorio_intervaloDeVendas.txt");
                        }
                    }else{
                        limparTela(3, 9, 18, 114, LIGHTGRAY);
                        gotoxy(4,11); cout<< "INSIRA A DATA COMPLETA";
                    }
                }
            } else if (opcao == 3){
                limparTela(3, 9, 18, 114, WHITE);
                limparTela(3, 9, 18, 114, LIGHTGRAY);
                exibirTotal(); //exibe resumo
            } else if (opcao == 4){
                telaApagarRegistros();
            } else
                break;
        }
            desenharCaixasTexto(caixas, 2);
            desenharBotao(botoes, 4);
        }
        delay(DELAY);

    limparMemoria(caixas, 2);
    limparMemoria(botoes, 4);

}

void telaRegistro(){

    int tecla, opcao;
    bool desenhar, enter;
    Botao opcoes[3];

    inicializarTelaInicialRegistro(opcoes);
    textbackground(LIGHTBLUE);
    clrscr();
    desenharBorda();
    desenharBorda(2, 2, 26, 116, WHITE);
    desenharBotao(opcoes, 3);
    gotoxy(50,1);
    printf(" Area Relatorio ");

    opcao = 0;
    while (true) {
        desenhar = false;
        enter = false;
        tecla = -1;

        logicaTelaInicialRegistro(&opcao, &enter);
        atualizarEstados(opcoes, opcao, 3);
        if (enter) {
            if (opcao == 0)
                telaVendaRegistro();
            else if (opcao == 1)
                telaEstoqueRegistro();
            else
                break;
            limparTela(3, 4, 23, 114);
        }
        desenharBotao(opcoes, 3);
        delay(DELAY);
    }
    limparMemoria(opcoes, 3);
}

void imprimirTituloAscii() {
    textcolor(WHITE);
    textbackground(LIGHTBLUE);

    gotoxy(10, 7);
    printf("   _____ _     _                         ______                                          _       _  ");
    gotoxy(10, 8);
    printf("  / ____(_)   | |                       |  ____|                                        (_)     | | ");
    gotoxy(10, 9);
    printf(" | (___  _ ___| |_ ___ _ __ ___   __ _  | |__   _ __ ___  _ __  _ __ ___  ___  __ _ _ __ _  __ _| | ");
    gotoxy(10, 10);
    printf("  \\___ \\| / __| __/ _ \\ '_ ` _ \\ / _` | |  __| | '_ ` _ \\| '_ \\| '__/ _ \\/ __|/ _` | '__| |/ _` | | ");
    gotoxy(10, 11);
    printf("  ____) | \\__ \\ ||  __/ | | | | | (_| | | |____| | | | | | |_) | | |  __/\\__ \\ (_| | |  | | (_| | | ");
    gotoxy(10, 12);
    printf(" |_____/|_|___/\\__\\___|_| |_| |_|\\__,_| |______|_| |_| |_| .__/|_|  \\___||___/\\__,_|_|  |_|\\__,_|_| ");
    gotoxy(10, 13);
    printf("      _       __      __            _                    | |                                        ");
    gotoxy(10, 14);
    printf("     | |      \\ \\    / /           | |                   |_|                                        ");
    gotoxy(10, 15);
    printf("   __| | ___   \\ \\  / /__ _ __   __| | __ _ ___                                                     ");
    gotoxy(10, 16);
    printf("  / _` |/ _ \\   \\ \\/ / _ \\ '_ \\ / _` |/ _` / __|                                                    ");
    gotoxy(10, 17);
    printf(" | (_| |  __/    \\  /  __/ | | | (_| | (_| \\__ \\                                                    ");
    gotoxy(10, 18);
    printf("  \\__,_|\\___|     \\/ \\___|_| |_|\\__,_|\\__,_|___/                                                    ");
}

//Título menor (para escolher qual usar)
void imprimirTituloAscii2() {
    textcolor(WHITE);
    textbackground(LIGHTBLUE);

    gotoxy(15, 5);
    printf("  ___ _    _                    ___                                 _      _  ");
    gotoxy(15, 6);
    printf(" / __(_)__| |_ ___ _ __  __ _  | __|_ __  _ __ _ _ ___ ___ __ _ _ _(_)__ _| | ");
    gotoxy(15, 7);
    printf(" \\__ \\ (_-<  _/ -_) '  \\/ _` | | _|| '  \\| '_ \\ '_/ -_|_-</ _` | '_| / _` | | ");
    gotoxy(15, 8);
    printf(" |___/_/__/\\__\\___|_|_|_\\__,_|_|___|_|_|_| .__/_| \\___/__/\\__,_|_| |_\\__,_|_| ");
    gotoxy(15, 9);
    printf("  __| |___  \\ \\ / /__ _ _  __| |__ _     |_|                                  ");
    gotoxy(15, 10);
    printf(" / _` / -_)  \\ V / -_) ' \\/ _` / _` |                                         ");
    gotoxy(15, 11);
    printf(" \\__,_\\___|   \\_/\\___|_||_\\__,_\\__,_|                                         ");
}

void telaInicial() {
    int opcao = 0;
    bool enter;
    Botao opcoes[6];

    textbackground(LIGHTBLUE);
    clrscr();
    desenharBorda();
    desenharBorda(2, 2, 26, 116, WHITE);

    imprimirTituloAscii();

    inicializarMenuPrincipal(opcoes);
    desenharBotao(opcoes, 6);

    while (true) {
        enter = false;
        logicaTelaInicial(&opcao, &enter);
        atualizarEstados(opcoes, opcao, 6);
        if (enter) {
            switch (opcao) {
                case MENU_PRINCIPAL_PRODUTO:
                    telaProduto();
                    break;
                case MENU_PRINCIPAL_FORNECEDOR:
                    telaFornecedor();
                    break;
                case MENU_PRINCIPAL_CLIENTE:
                    telaCliente();
                    break;
                case MENU_PRINCIPAL_VENDA:
                    telaVendas();
                    break;
                case MENU_PRINCIPAL_RELATORIO:
                    telaRegistro();
                    break;
                case MENU_PRINCIPAL_SAIR:
                    limparMemoria(opcoes, 6);
                    return;
            }
            // Redesenha a tela após retornar de uma subtela
            textbackground(LIGHTBLUE);
            clrscr();
            desenharBorda();
            desenharBorda(2, 2, 26, 116, WHITE);

            imprimirTituloAscii();

            desenharBotao(opcoes, 6);
        }
        desenharBotao(opcoes, 6);
    }
}
