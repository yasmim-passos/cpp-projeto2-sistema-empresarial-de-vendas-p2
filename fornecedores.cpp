#include "fornecedores.h"
#include <string>
#include <cstring>
#include <fstream>

using namespace std;

void InicializarTabela(TabelaFornecedor &tabela) {
    tabela.qtd = 0;
    tabela.dados = NULL;
}

void carregarFornecedores(TabelaFornecedor &tabela) {
    ifstream fornecedoresIn("fornecedores.dat", ios::binary);

    if (fornecedoresIn.fail()){
        tabela.qtd = 0;
        tabela.dados = NULL;
        return;
    }

    fornecedoresIn.read((char*)&tabela.qtd,sizeof(int));
    tabela.dados = new Fornecedor[tabela.qtd];

    for(int i = 0; i < tabela.qtd; i++)
        fornecedoresIn.read((char*) &tabela.dados[i], sizeof(tabela.dados[i]));

    fornecedoresIn.close();
}

bool verificarIdExistente(TabelaFornecedor &tabela, int codigo)
{
    for(int i = 0; i < tabela.qtd; i++){
        if(tabela.dados[i].codigo==codigo)
            return true;
    }
    return false;
}

void ArmazenarDados(TabelaFornecedor &tabela, Fornecedor &f) {
    Fornecedor *novo_dados = new Fornecedor[tabela.qtd+1];

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

void salvarFornecedores(const TabelaFornecedor &tabela) {
    ofstream fornecedoresOut("fornecedores.dat", ios::binary);

    if (fornecedoresOut.fail()){
        exit(1);
    }

    fornecedoresOut.write((char*)&tabela.qtd,sizeof(int));

    for(int i = 0; i < tabela.qtd; i++)
        fornecedoresOut.write((const char*) &tabela.dados[i], sizeof(tabela.dados[i]));

    fornecedoresOut.close();
}

void formataContato(char contato[])
{
    char temp[15];

    sprintf(temp, "(%c%c) %c%c%c%c%c-%c%c%c%c",
        contato[0], contato[1],
        contato[2], contato[3], contato[4], contato[5], contato[6],
        contato[7], contato[8], contato[9], contato[10]
    );

    strcpy(contato, temp);
}

void efetuarCadastro(char **dados, TabelaFornecedor &tabela) {
    Fornecedor f;

    f.codigo = stoi(dados[0]);
    strcpy(f.empresaNome, dados[1]);
    strcpy(f.responsavel, dados[2]);
    strcpy(f.contato, dados[3]);
    formataContato(f.contato);

    ArmazenarDados(tabela, f);
    salvarFornecedores(tabela);
}

void efetuarExclusao(TabelaFornecedor &tabela, int codigo) {
    Fornecedor *novosDados = new Fornecedor[tabela.qtd - 1];
    int j = 0;
    for(int i = 0; i < tabela.qtd; i++){
        if(codigo != tabela.dados[i].codigo){
            novosDados[j++] = tabela.dados[i];
        }
    }
    delete[] tabela.dados;
    tabela.dados = novosDados;
    tabela.qtd--;
    salvarFornecedores(tabela);
}

void efetuarPesquisa(TabelaFornecedor &tabela, char *empresaPesquisa) {
    TabelaFornecedor novaTabela;
    novaTabela.qtd = 0;
    novaTabela.dados = NULL;
    for(int i = 0; i < tabela.qtd; i++){
        if(strstr(tabela.dados[i].empresaNome, empresaPesquisa) != 0){
            ArmazenarDados(novaTabela, tabela.dados[i]);
        }
    }
    tabela.qtd = novaTabela.qtd;
    delete[] tabela.dados;
    tabela.dados = novaTabela.dados;
}

void efetuarEdicao(TabelaFornecedor &tabela, Fornecedor &f, int indice) {
    strcpy(tabela.dados[indice].empresaNome, f.empresaNome);
    strcpy(tabela.dados[indice].responsavel, f.responsavel);
    strcpy(tabela.dados[indice].contato, f.contato);
    salvarFornecedores(tabela);
}

void efetuarEdicao(TabelaFornecedor &tabela) {
    salvarFornecedores(tabela);
}

int verificarCad(char **dados, TabelaFornecedor &tabela) {
    for (int i = 0; i < 4; i++)
        if (dados[i] == NULL) return ERRO_NADA_ESCRITO;
    int codF = stoi(dados[0]);
    if (verificarIdExistente(tabela, codF))
        return ERRO_CODIGO_EXISTENTE;
    if (strlen(dados[3]) != 11)
        return ERRO_NUMERO_INVALIDO;
    return SEM_ERRO;
}

int verificarEdicao(Fornecedor &f) {
    int lenContato = strlen(f.contato);
    if (strlen(f.empresaNome) == 0 || strlen(f.responsavel) == 0 || lenContato == 0)
        return ERRO_NADA_ESCRITO;
    if (lenContato != 11)
        return ERRO_NUMERO_INVALIDO;
    return SEM_ERRO;
}

int verificarEdicao(char **f) {
    if (f[0] == NULL || f[1] == NULL || f[2] == NULL)
        return ERRO_NADA_ESCRITO;
    if (strlen(f[2]) != 11)
        return ERRO_NUMERO_INVALIDO;
    return SEM_ERRO;
}

void corrigirTabelaInterface(TabelaFornecedor &tabela, int codigo) {
    Fornecedor *novosDados = new Fornecedor[tabela.qtd - 1];
    int j = 0;
    for(int i = 0; i < tabela.qtd; i++){
        if(codigo != tabela.dados[i].codigo){
            novosDados[j++] = tabela.dados[i];
        }
    }
    delete[] tabela.dados;
    tabela.dados = novosDados;
    tabela.qtd--;
}

void LiberarMemoria(TabelaFornecedor &tabela)
{
    delete[] tabela.dados;
    tabela.dados = NULL;
}
