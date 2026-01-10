#include "gestao_estoque.h"
#include <string>
#include <cstring>
#include <ctime>
#include <fstream>

using namespace std;

void salvarProdutos(const TabelaProduto &tabela) {
    ofstream produtosOut("produtos.dat", ios::binary);

    if (produtosOut.fail()){
        exit(1);
    }

    produtosOut.write((char*)&tabela.qtd,sizeof(int));

    for(int i = 0; i < tabela.qtd; i++)
        produtosOut.write((const char*) &tabela.dados[i], sizeof(tabela.dados[i]));

    produtosOut.close();
}

void carregarProdutos(TabelaProduto &tabela) {
    ifstream produtosIn("produtos.dat", ios::binary);

    if (produtosIn.fail()){
        tabela.qtd = 0;
        tabela.dados = NULL;
        return;
    }

    produtosIn.read((char*)&tabela.qtd,sizeof(int));
    tabela.dados = new Produto[tabela.qtd];

    for(int i = 0; i < tabela.qtd; i++)
        produtosIn.read((char*) &tabela.dados[i], sizeof(tabela.dados[i]));

    produtosIn.close();
}

bool verificarIdExistente(TabelaProduto &tabela, int id)
{
    for(int i = 0; i < tabela.qtd; i++){
        if(tabela.dados[i].id==id)
            return true;
    }
    return false;
}

int checarEstoque(int id, int quantidade, int &indice, TabelaProduto &tabela) {
    int in, retorno = PRODUTO_VALIDO;
    bool achouId = false;

    for (int i = 0; i < tabela.qtd; i++) {
        if (tabela.dados[i].id == id) {
            achouId = true;
            in = i;
            break;
        }
    }
    if (!achouId) retorno = ERRO_PRODUTO_NAO_EXISTE;
    else if (quantidade > tabela.dados[in].Quantidade)
        retorno = ERRO_QUANTIDADE_INSUFICIENTE;
    indice = in;

    return retorno;
}

void removerEstoqueId(TabelaProduto &tabela, int id, int qtd) {
    for (int i = 0; i < tabela.qtd; i++) {
        if (tabela.dados[i].id == id) {
            tabela.dados[i].Quantidade -= qtd;
            salvarProdutos(tabela);
            break;
        }
    }
}

void adicionarEstoqueId(TabelaProduto &tabela, int id, int qtd) {
    for (int i = 0; i < tabela.qtd; i++) {
        if (tabela.dados[i].id == id) {
            tabela.dados[i].Quantidade += qtd;
            salvarProdutos(tabela);
            break;
        }
    }
}

void ArmazenarDados(TabelaProduto &tabela, Produto &f) {
    Produto *novo_dados = new Produto[tabela.qtd+1];

    if(novo_dados == NULL){
        exit(1);
    }

    for (int i = 0; i < tabela.qtd; i++)
        novo_dados[i] = tabela.dados[i];

    novo_dados[tabela.qtd] = f;
    delete[] tabela.dados;
    tabela.dados = novo_dados;
    tabela.qtd++;
}

void efetuarCadastro(char **dados, TabelaProduto &tabela) {
    Produto f;
    f.id = stoi(dados[0]);
    f.Quantidade = atoi(dados[1]);
    f.preco = atof(dados[2]);
    strcpy(f.nome, dados[3]);
    f.precoTotal = f.preco * f.Quantidade;
    strcpy(f.categoria, dados[4]);
    std::time_t agora = std::time(0);
    std::tm *agoraTM = std::localtime(&agora);
    std::sprintf(f.dia, "%02d", agoraTM->tm_mday);
    std::sprintf(f.mes, "%02d", agoraTM->tm_mon + 1);
    std::sprintf(f.ano, "%04d", agoraTM->tm_year + 1900);
    f.disponivel = true;

    ArmazenarDados(tabela, f);
    salvarProdutos(tabela);
}

void efetuarExclusao(TabelaProduto &tabela, int codigo) {
    Produto *novosDados = new Produto[tabela.qtd-1];
    int j = 0;
    for(int i = 0; i < tabela.qtd; i++) {
        if(codigo != tabela.dados[i].id)
            novosDados[j++] = tabela.dados[i];
    }
    delete[] tabela.dados;
    tabela.dados = novosDados;
    tabela.qtd--;
    salvarProdutos(tabela);
}

void efetuarEdicao(TabelaProduto &tabela, Produto &f, int indice) {
    strcpy(tabela.dados[indice].nome, f.nome);
    tabela.dados[indice].preco = f.preco;
    tabela.dados[indice].Quantidade = f.Quantidade;
    strcpy(tabela.dados[indice].categoria, f.categoria);
    salvarProdutos(tabela);
}

void efetuarEdicao(TabelaProduto &tabela) {
    salvarProdutos(tabela);
}

void efetuarPesquisa(TabelaProduto &tabela, char *produtoPesquisa) {
    TabelaProduto novaTabela;
    novaTabela.qtd = 0;
    novaTabela.dados = NULL;
    for(int i = 0; i < tabela.qtd; i++){
        if(strstr(tabela.dados[i].nome, produtoPesquisa) != 0){
            ArmazenarDados(novaTabela, tabela.dados[i]);
        }
    }
    tabela.qtd = novaTabela.qtd;
    delete[] tabela.dados;
    tabela.dados = novaTabela.dados;
}


int verificarCad(char **dados, TabelaProduto &tabela) {
    for (int i = 0; i < 5; i++)
        if (dados[i] == NULL) return ERRO_NADA_ESCRITO;
    int codF = stoi(dados[0]);
    if (verificarIdExistente(tabela, codF))
        return ERRO_CODIGO_EXISTENTE;
    return SEM_ERRO;
}

int verificarEdicaoP(char **p) {
    for (int i = 0; i < 4; i++)
        if (p[i] == NULL) return ERRO_NADA_ESCRITO;
    return SEM_ERRO;
}

void corrigirTabelaInterface(TabelaProduto &tabela, int codigo) {
    Produto *novosDados = new Produto[tabela.qtd-1];
    int j = 0;
    for(int i = 0; i < tabela.qtd; i++) {
        if(codigo != tabela.dados[i].id)
            novosDados[j++] = tabela.dados[i];
    }
    delete[] tabela.dados;
    tabela.dados = novosDados;
    tabela.qtd--;
}

void LiberarMemoria(TabelaProduto &tabela)
{
    delete[] tabela.dados;
    tabela.dados = NULL;
}

