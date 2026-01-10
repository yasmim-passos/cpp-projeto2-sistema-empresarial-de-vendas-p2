#include <iostream>
#include <locale.h>
#include <cstring>
#include "clientes.h"

#include <fstream>
#include <cstring>
#include <string>

using namespace std;

bool encontrarIdCliente(TabelaCliente &tabela, int id) {
    for (int i = 0; i < tabela.qtd; i++) {
        if (tabela.dados[i].id == id)
            return true;
    }
    return false;
}

int verificarCadastro(char *conteudo[], TabelaCliente &tabela) {
    for (int i = 0; i < 3; i++)
        if (conteudo[i] == NULL)
            return ERRO_NADA_ESCRITO;
    int id = atoi(conteudo[0]);
    for (int i = 0; i < tabela.qtd; i++) {
        if (tabela.dados[i].id == id)
            return ERRO_CODIGO_EXISTENTE;
    }
    if (strlen(conteudo[2]) != 14)
        return ERRO_NUMERO_INVALIDO;
    return SEM_ERRO;
}

void inserirCliente(TabelaCliente &tabela, const string *linhas) {
    Cliente *novo = new Cliente[tabela.qtd + 1];

    for (int i = 0; i < tabela.qtd; i++)
        novo[i] = tabela.dados[i];
    novo[tabela.qtd].id = atoi(linhas[0].c_str());
    strcpy(novo[tabela.qtd].nome, linhas[1].c_str());
    strcpy(novo[tabela.qtd].cpf, linhas[2].c_str());
    if (tabela.dados != NULL)
        delete[] tabela.dados;
    tabela.dados = novo;
    tabela.qtd++;
}

void inserirCliente(TabelaCliente &tabela, char *linhas[]) {
    Cliente *novo = new Cliente[tabela.qtd + 1];

    for (int i = 0; i < tabela.qtd; i++)
        novo[i] = tabela.dados[i];
    novo[tabela.qtd].id = atoi(linhas[0]);
    strcpy(novo[tabela.qtd].nome, linhas[1]);
    strcpy(novo[tabela.qtd].cpf, linhas[2]);
    if (tabela.dados != NULL)
        delete[] tabela.dados;
    tabela.dados = novo;
    tabela.qtd++;
}

void inserirCliente(TabelaCliente &tabela, const Cliente &cliente) {
    Cliente *novo = new Cliente[tabela.qtd + 1];

    for (int i = 0; i < tabela.qtd; i++)
        novo[i] = tabela.dados[i];

    novo[tabela.qtd] = cliente;
    if (tabela.dados != NULL)
        delete[] tabela.dados;
    tabela.dados = novo;
    tabela.qtd++;
}

void carregarClientes(TabelaCliente &tabela) {
    ifstream arquivo(ARQUIVO_CLIENTES);
    string linhas[3];
    bool vazio = true;

    if (arquivo.fail()) {
        tabela.qtd = 0;
        tabela.dados = NULL;
        return;
    }
    tabela.qtd = 0;
    tabela.dados = NULL;
    while (true) {
        if (!getline(arquivo, linhas[0])) break;
		if (!getline(arquivo, linhas[1])) break;
		if (!getline(arquivo, linhas[2])) break;
        inserirCliente(tabela, linhas);
    }
    arquivo.close();
}

void salvarClientes(TabelaCliente &tabela) {
    ofstream arquivo(ARQUIVO_CLIENTES, ios::trunc);

    for (int i = 0; i < tabela.qtd; i++) {
        arquivo << tabela.dados[i].id << endl
                << tabela.dados[i].nome << endl
                << tabela.dados[i].cpf << endl;
    }

    arquivo.close();
}

void efetuarCadastro(TabelaCliente &tabela, char *conteudo[]) {
    ofstream arquivo(ARQUIVO_CLIENTES, ios::app);

    arquivo << conteudo[0] << endl
            << conteudo[1] << endl
            << conteudo[2] << endl;

    arquivo.close();
    inserirCliente(tabela, conteudo);
}

void efetuarExclusao(TabelaCliente &tabela, int id) {
    Cliente *novosDados = new Cliente[tabela.qtd-1];
    int j = 0;
    for (int i = 0; i < tabela.qtd; i++) {
        if (id != tabela.dados[i].id)
            novosDados[j++] = tabela.dados[i];
    }
    delete[] tabela.dados;
    tabela.dados = novosDados;
    tabela.qtd--;
    salvarClientes(tabela);
}

void efetuarPesquisa(TabelaCliente &tabela, char *nomePesquisa) {
    TabelaCliente novaTabela;
    novaTabela.qtd = 0;
    novaTabela.dados = NULL;
    for (int i = 0; i < tabela.qtd; i++) {
        if (strstr(tabela.dados[i].nome, nomePesquisa) != 0) {
            inserirCliente(novaTabela, tabela.dados[i]);
        }
    }
    tabela.qtd = novaTabela.qtd;
    if (tabela.dados != NULL)
        delete[] tabela.dados;
    tabela.dados = novaTabela.dados;
}

void corrigirTabelaInterface(TabelaCliente &tabela, int id) {
    Cliente *novosDados = new Cliente[tabela.qtd-1];
    int j = 0;
    for (int i = 0; i < tabela.qtd; i++) {
        if (id != tabela.dados[i].id)
            novosDados[j++] = tabela.dados[i];
    }
    delete[] tabela.dados;
    tabela.dados = novosDados;
    tabela.qtd--;
}

void liberarMemoria(TabelaCliente &tabela) {
    delete[] tabela.dados;
    tabela.dados = NULL;
}
