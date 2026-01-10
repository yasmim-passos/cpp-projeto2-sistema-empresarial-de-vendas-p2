#include "vendas.h"
#include <cstdio>

int obterProximoNumeroVenda() {
    ifstream arquivo("contador_vendas.txt");
    int contador = 1;

    if (arquivo) {
        arquivo >> contador;
        arquivo.close();
    }

    return contador;
}


void salvarVenda(Venda* venda) {
    ofstream arquivoVendas("vendas.txt");

    if (!arquivoVendas) {
        exit(1);
    }

    arquivoVendas << venda->numeroVenda << endl
                  << venda->data << endl
                  << venda->codigoCliente << endl
                  << venda->qtdLista << endl
                  << fixed << setprecision(2) << venda->totalVenda << endl;

    for (size_t i = 0; i < venda->qtdLista; i++) {
        arquivoVendas << venda->itens[i].codigoProduto << endl
                      << venda->itens[i].descricao << endl
                      << fixed << setprecision(2) << venda->itens[i].precoUnitario << endl
                      << venda->itens[i].quantidade << endl
                      << fixed << setprecision(2) << venda->itens[i].subtotal << endl;
    }

    arquivoVendas.close();

    ofstream arquivoEstoque("estoque_venda.txt");

    if (arquivoEstoque) {
        for (size_t i = 0; i < venda->qtdLista; i++) {
            arquivoEstoque << venda->itens[i].codigoProduto << endl
                          << venda->itens[i].quantidade << endl;
        }

        arquivoEstoque.close();
    }
    ofstream saida("contador_vendas.txt");
    saida << venda->numeroVenda + 1;
    saida.close();
}

void obterDataAtual(char* data) {
    time_t now = time(0);
    tm* ltm = localtime(&now);

    sprintf(data, "%02d/%02d/%04d", ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
}

void inicializarVenda(Venda &venda) {
    venda.totalVenda = 0.0;
    venda.numeroVenda = obterProximoNumeroVenda();
    venda.qtdLista = 0;
    venda.itens = NULL;
    obterDataAtual(venda.data);
}

void inserirVenda(Venda &venda, ItemVenda &item) {
    ItemVenda *novaLista = new ItemVenda[venda.qtdLista+1];

    for (int i = 0; i < venda.qtdLista; i++)
        novaLista[i] = venda.itens[i];
    novaLista[venda.qtdLista] = item;
    if (venda.itens != NULL)
        delete[] venda.itens;
    venda.itens = novaLista;
    venda.qtdLista++;
}

void removerVenda(Venda &venda, int indice) {
    ItemVenda *novaLista = new ItemVenda[venda.qtdLista-1];
    int j = 0;

    for (int i = 0; i < venda.qtdLista; i++) {
        if (i != indice)
            novaLista[j++] = venda.itens[i];
    }
    delete[] venda.itens;
    venda.itens = novaLista;
    venda.qtdLista--;
}

void adicionarItem(TabelaProduto &tabela, Venda &venda, int indice, int qtd) {
    ItemVenda item;

    item.codigoProduto = tabela.dados[indice].id;
    item.precoUnitario = tabela.dados[indice].preco;
    item.quantidade = qtd;
    item.subtotal = item.precoUnitario * item.quantidade;
    strcpy(item.descricao, tabela.dados[indice].nome);

    inserirVenda(venda, item);
    venda.totalVenda += item.subtotal;
}

void carregarItens(Venda &venda, ifstream &arquivo) {
    ItemVenda item;

    arquivo.read((char *) &item, sizeof(item));
    if (arquivo.eof())
        return;
    inserirVenda(venda, item);
    venda.totalVenda += item.subtotal;
    carregarItens(venda, arquivo);
}

bool carregarListaVenda(Venda &venda) {
    ifstream arquivoE("venda_estado.dat", ios::binary);

    if (arquivoE.fail())
        return false;
    carregarItens(venda, arquivoE);
    arquivoE.close();

    return true;
}

void armazenarListaVenda(ItemVenda &item) {
    ofstream arquivo("venda_estado.dat", ios::binary | ios::app);
    arquivo.write((const char *) &item, sizeof(item));
    arquivo.close();
}

void sobrescreverListaVenda(Venda &venda) {
    ofstream arquivo("venda_estado.dat", ios::binary);
    ItemVenda itemInvalido;

    arquivo.write((const char *) venda.itens, sizeof(ItemVenda) * venda.qtdLista);

    arquivo.close();
}

void excluirListaVenda() {
    remove("venda_estado.dat");
}

int efetuarPesquisa(int &indice, int &qtd, char **dados, TabelaProduto &tabela) {
    if (dados[0] == NULL || dados[1] == NULL)
        return ERRO_NADA_ESCRITO;
    int id = atoi(dados[0]);
    qtd = atoi(dados[1]);
    if (qtd <= 0)
        return ERRO_QTD_INVALIDA;
    return checarEstoque(id, qtd, indice, tabela);
}

int efetuarRemocaoVenda(TabelaProduto &tabela, Venda &venda, int indice) {
    adicionarEstoqueId(tabela, venda.itens[indice].codigoProduto, venda.itens[indice].quantidade);
    venda.totalVenda -= venda.itens[indice].subtotal;
    removerVenda(venda, indice);
    sobrescreverListaVenda(venda);
}

void resetarVenda(Venda &venda) {
    inicializarVenda(venda);
    delete[] venda.itens;
    venda.itens = NULL;
}
