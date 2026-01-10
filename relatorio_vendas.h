#ifndef RELATORIO_VENDAS_H
#define RELATORIO_VENDAS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <locale.h>
#include <iomanip>
#include <ctime>
#include <climits>
#include <map>
#include <cstdio>

#define TAMANHO_NOME 48
#define MAX_ITENS 100

using namespace std;

const char* vendasTotais = "relatorio_vendasTotais.txt";
const char* vendasLista = "relatorio_vendasLista.txt";
const char* intervaloVendas = "relatorio_intervaloDeVendas.txt";

struct InfoVenda {
    int numeroVenda;
    char data[11];
    int codigoCliente;
    int qntId;
    float totalVenda;
};

struct InfoItens {
    int id;
    char nome[TAMANHO_NOME+1];
    float preco;
    int qnt;
    float subtotal;
};

struct DadosRelatorio {
    int quantidadeTotal = 0;
    float receitaTotal = 0;
    string maisVendido = " ";
    int maisUnidades = 0;
    string menosVendido = " ";
    int menosUnidades = INT_MAX;
};

struct produtosListados {
    string data;
    int id_produto;
    string nome_produto;
    float preco_unitario;
    int quantidade;
    float subtotal;
};

produtosListados vendasListadas[MAX_ITENS];
int totalVendas = 0;

void configurarLocalizacao();
void contLinhas(int &linhas);
void lerDadosVendas(int linhas, string vendasCadastradas[]);
void converteDados(string vendasCadastradas[], int linhas, InfoVenda &vendas, InfoItens itens[], int &numItens, DadosRelatorio &dado);
void carregarDadosRelatorio(DadosRelatorio &dado);
void vendasTotaisRel(DadosRelatorio &dado, InfoVenda &vendas, InfoItens itens[], int numItens);
void vendasListaRel(InfoVenda &vendas, InfoItens itens[], int numItens);
bool conversaoData(const string &dataStr, tm &dataTm);
int compararDatas(const tm& data1, const tm& data2);
void lerVendasLista();
void gerarRelatorioIntervalo(const produtosListados vendasListadas[], int totalVendas);
void limpaArquivo(const char* arquivo);
void exibirTotal();
void exibirLista();
void exibirIntervalo();
void menu(char &opcao);
void carregamentoVendas();

void carregamentoVendas() {
    //configurarLocalizacao();
    int linhas = 0;

    DadosRelatorio dado;

    contLinhas(linhas);

    string* vendasCadastradas = new string[linhas]; // Alocação de Memória - array dinamico
    lerDadosVendas(linhas, vendasCadastradas);

    InfoItens itens[MAX_ITENS];
    InfoVenda vendas;
    int numItens = 0;

    carregarDadosRelatorio(dado);
    converteDados(vendasCadastradas, linhas, vendas, itens, numItens, dado);
    vendasTotaisRel(dado, vendas, itens, numItens);
    vendasListaRel(vendas, itens, numItens);

    delete[] vendasCadastradas;
}

void configurarLocalizacao()
{
    setlocale(LC_ALL, "Portuguese");
    locale::global(locale(""));
    cout.imbue(locale());
    cin.imbue(locale());
}


// Contar linhas do arquivo vendas.txt
void contLinhas(int& linhas) {
    ifstream leitura("vendas.txt");
    if(!leitura.is_open()) {
        cerr << "Erro ao tentar abrir o arquivo!" << endl;
        return;
    }
    string s;
    while(getline(leitura,s)) {
        linhas++;
    }
    leitura.close();
}

// Ler dados do arquivo vendas.txt
// e armazenar em um array de strings
void lerDadosVendas(int linhas, string vendasCadastradas[]) {
    ifstream leitura("vendas.txt");
    string linha;
    if(!leitura.is_open()) {
        cerr << "Erro ao tentar abrir o arquivo!" << endl;
        return;
    }

    for(int i = 0; i < linhas; i++) {
        if(getline(leitura, linha)) {
            vendasCadastradas[i] = linha;
        }
    }
    leitura.close();
}

// Converter os dados lidos do arquivo vendas.txt
// e armazenar em variaveis do tipo InfoVenda e InfoItens
void converteDados(string vendasCadastradas[], int linhas, InfoVenda &vendas, InfoItens itens[], int &numItens,DadosRelatorio &dado) {
    vendas.numeroVenda = stoi(vendasCadastradas[0]);
    strncpy(vendas.data, vendasCadastradas[1].c_str(), 10);
    vendas.data[10] = '\0';
    vendas.codigoCliente = stoi(vendasCadastradas[2]);
    vendas.qntId = stoi(vendasCadastradas[3]);
    vendas.totalVenda = stof(vendasCadastradas[4]);

    int numVendas = (linhas - 5) / 5; // Ignora as primeiras 5 linhas e calcula o numero de vendas restantes
    numItens = 0; // Inicializa o contador de itens

    for (int i = 0; i < numVendas; i++) {

        int baseIndice = 5 + i * 5; // Indice base para cada venda, comecando apos as 5 primeiras linhas

        // Processa os itens da venda atual
        itens[numItens].id = stoi(vendasCadastradas[baseIndice]);
        strncpy(itens[numItens].nome, vendasCadastradas[baseIndice + 1].c_str(), TAMANHO_NOME);
        itens[numItens].nome[TAMANHO_NOME] = '\0'; // Garante que a string esteja terminada corretamente
        itens[numItens].preco = stof(vendasCadastradas[baseIndice + 2]);
        itens[numItens].qnt = stoi(vendasCadastradas[baseIndice + 3]);
        itens[numItens].subtotal = stof(vendasCadastradas[baseIndice + 4]);

        dado.quantidadeTotal+=itens[numItens].qnt;
        dado.receitaTotal+=itens[numItens].subtotal;

        if (itens[numItens].qnt > dado.maisUnidades) {
            dado.maisUnidades = itens[numItens].qnt;
            dado.maisVendido = itens[numItens].nome;
        }
        if (itens[numItens].qnt < dado.menosUnidades) {
            dado.menosUnidades = itens[numItens].qnt;
            dado.menosVendido = itens[numItens].nome;
        }
        numItens++;
    }
}

// Carregar dados do arquivo vendasTotais.txt
// e armazenar em variaveis do tipo DadosRelatorio
void carregarDadosRelatorio(DadosRelatorio &dado) {
    ifstream leitura(vendasTotais);
    if (!leitura.is_open()) {
        // Se o arquivo nao existir, mantem os valores padrao
        return;
    }

    string linha;
    if (getline(leitura, linha)) dado.quantidadeTotal = stoi(linha);
    if (getline(leitura, linha)) dado.receitaTotal = stof(linha);
    if (getline(leitura, linha)) {}

    if (getline(leitura, linha)) dado.maisVendido = linha;
    if (getline(leitura, linha)) dado.maisUnidades = stoi(linha);
    if (getline(leitura, linha)) dado.menosVendido = linha;
    if (getline(leitura, linha)) dado.menosUnidades = stoi(linha);

    leitura.close();
}

// Gerar o relatorio de vendas totais
void vendasTotaisRel(DadosRelatorio &dado, InfoVenda &vendas, InfoItens itens[],int numItens) {
    ofstream saida(vendasTotais, ios::out | ios::trunc);
    if (!saida.is_open()) {
        //cerr << "Erro ao abrir arquivo." << endl;
        return;
    }
    saida << fixed << setprecision(2);
    saida << dado.quantidadeTotal << endl // Quantidade (em unidades) de produtos vendidos
          << dado.receitaTotal << endl // Receita total gerada na venda
          << dado.receitaTotal/dado.quantidadeTotal << endl // Media de renda por unidade vendida
          << dado.maisVendido << endl // Item mais vendido (nome)
          << dado.maisUnidades << endl // Item mais vendido (quantidade)
          << dado.menosVendido << endl // Item menos vendido (nome)
          << dado.menosUnidades << endl; // Item menos vendido (quantidade)
    saida.close();
}

// Gerar o relatorio de vendas listadas
void vendasListaRel(InfoVenda &vendas, InfoItens itens[], int numItens) {
    ofstream saida(vendasLista, ios::out | ios::app);
    if(!saida.is_open()) {
        //cerr << "Erro ao abrir arquivo." << endl;
        return;  // Adicionei return para sair se o arquivo nao abrir
    }
    saida << fixed << setprecision(2);  // Para consistencia com outras saidas
    for (int i = 0; i < numItens; i++) {
        saida << vendas.data << endl
              << itens[i].id << endl
              << itens[i].nome << endl
              << itens[i].preco << endl
              << itens[i].qnt << endl
              << itens[i].subtotal << endl;
    }
    saida.close();
}

// Limpa os dados de um arquivo
//passar como parametro o arquivo a ser limpo
void limpaArquivo(const char* arquivo) {
    ofstream saida;
        saida.open(arquivo, ios::trunc); // Limpa o arquivo informado
        saida.close();
    gotoxy(45, 17); cout<< "Arquivo apagado com sucesso!";
}

// Converte uma string de data no formato "dd/mm/aaaa" para struct tm
// Retorna true se a conversao for bem-sucedida, false caso contrario
bool conversaoData(const string &dataStr, tm &dataTm) {
    istringstream ss(dataStr);
    char delimiter;

    ss >> dataTm.tm_mday >> delimiter;
    if (delimiter != '/') return false;

    ss >> dataTm.tm_mon >> delimiter;
    if (delimiter != '/') return false;

    ss >> dataTm.tm_year;

    // Ajustes para struct tm
    dataTm.tm_mon -= 1;    // Meses sao 0-11
    dataTm.tm_year -= 1900; // Ano e contado a partir de 1900
    if(dataTm.tm_year < 0)
        return false;

    return true;
}

int compararDatas(const tm& data1, const tm& data2) {
    if (data1.tm_year < data2.tm_year) return -1;
    if (data1.tm_year > data2.tm_year) return 1;

    if (data1.tm_mon < data2.tm_mon) return -1;
    if (data1.tm_mon > data2.tm_mon) return 1;

    if (data1.tm_mday < data2.tm_mday) return -1;
    if (data1.tm_mday > data2.tm_mday) return 1;

    return 0;
}

// Ler o arquivo de vendas listadas e armazenar os dados em um array de produtosListados
void lerVendasLista() {
    ifstream arquivo(vendasLista);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo de vendas." << endl;
        return;
    }

    string linha;
    totalVendas = 0;  // Inicializa o contador

    while (getline(arquivo, linha)) {
        // Verifica se ainda há espaço no array
        if (totalVendas >= MAX_ITENS) {
            cerr << "Aviso: Limite máximo de vendas atingido (" << MAX_ITENS << ")" << endl;
            break;
        }

        // Preenche a estrutura da venda atual
        vendasListadas[totalVendas].data = linha;

        if (!getline(arquivo, linha)) break;
        vendasListadas[totalVendas].id_produto = stoi(linha);

        if (!getline(arquivo, linha)) break;
        vendasListadas[totalVendas].nome_produto = linha;

        if (!getline(arquivo, linha)) break;
        vendasListadas[totalVendas].preco_unitario = stof(linha);

        if (!getline(arquivo, linha)) break;
        vendasListadas[totalVendas].quantidade = stoi(linha);

        if (!getline(arquivo, linha)) break;
        vendasListadas[totalVendas].subtotal = stof(linha);

        totalVendas++;  // Incrementa o contador após ler todos os campos
    }

    arquivo.close();
}

// Gerar o relatorio de vendas por intervalo
bool gerarRelatorioIntervalo(string dataInicioStr, string dataFimStr) {

    lerVendasLista();

    tm dataInicio = {0}, dataFim = {0};
    if (!conversaoData(dataInicioStr, dataInicio)) {
        limparTela(3, 9, 18, 114, LIGHTGRAY);
        gotoxy(4, 10);cerr << "Formato de data de inicio invalido." << endl;
        return false;
    }
    if (!conversaoData(dataFimStr, dataFim)) {
        limparTela(3, 9, 18, 114, LIGHTGRAY);
        gotoxy(4, 10);cerr << "Formato de data de fim invalido." << endl;
        return false;
    }

    ofstream saida(intervaloVendas, ios::out | ios::trunc);
    if (!saida.is_open()) {
        gotoxy(4, 10);cerr << "Erro ao criar arquivo de relatorio por intervalo." << endl;
        return false;
    }

    saida << fixed << setprecision(2);
    saida << "\nRelatorio de Vendas - Periodo: " << dataInicioStr << " a " << dataFimStr << endl;
    saida << "------------------------------------------------------" << endl;

    float totalPeriodo = 0.0;
    int totalUnidades= 0;
    bool encontrouVendas = false;

    // Variaveis para armazenar os produtos mais e menos vendidos
    string produtoMaisVendido = "";
    int unidadesMaisVendido = 0;
    string produtoMenosVendido = "";
    int unidadesMenosVendido = INT_MAX;

    // Mapa para armazenar o total de unidades por produto
    map<string, int> unidadesPorProduto;

    for (int i = 0; i < totalVendas; i++) {
        const produtosListados& venda = vendasListadas[i];
        tm dataVenda = {0};

        if (!conversaoData(venda.data, dataVenda)) {
            gotoxy(4, 10);cerr << "Data invalida no arquivo: " << venda.data << endl;
            continue;
        }

        if (compararDatas(dataVenda, dataInicio) >= 0 &&
            compararDatas(dataVenda, dataFim) <= 0) {
            saida << "Data: " << venda.data << endl;
            saida << "- ID: " << venda.id_produto
                 << " | Produto: " << venda.nome_produto
                 << " | Quantidade: " << venda.quantidade
                 << " | Subtotal: R$ " << venda.subtotal << endl;
            totalPeriodo += venda.subtotal;
            totalUnidades += venda.quantidade;
            encontrouVendas = true;

            // Atualiza o contador de unidades por produto
            unidadesPorProduto[venda.nome_produto] += venda.quantidade;
        }
    }

    // Encontra o produto mais e menos vendido
    for (const auto& par : unidadesPorProduto) {
        if (par.second > unidadesMaisVendido) {
            unidadesMaisVendido = par.second;
            produtoMaisVendido = par.first;
        }
        if (par.second < unidadesMenosVendido) {
            unidadesMenosVendido = par.second;
            produtoMenosVendido = par.first;
        }
    }

    if (!encontrouVendas) {
        saida << "Nenhuma venda encontrada no periodo especificado." << endl;
        gotoxy(4,10); cout << "NENHUMA VENDA ENCONTRADA NO PERIODO ESPECIFICADO";
        return false;
    } else {
        saida << "---------------------------------------------------------" << endl;
        saida << "Unidades vendidadas nesse periodo: " << totalUnidades << " unidades" << endl;
        saida << "Total do periodo: R$ " << totalPeriodo << endl;
        saida << "Produto mais vendido: " << produtoMaisVendido
              << " (" << unidadesMaisVendido << " unidades)" << endl;
        saida << "Produto menos vendido: " << produtoMenosVendido
              << " (" << unidadesMenosVendido << " unidades)" << endl;
        saida << "---------------------------------------------------------" << endl;
    }

    saida.close();
    return true;
}

// Exibir o relatorio de vendas totais
void exibirTotal() {

    ifstream leitura(vendasTotais);
    string s;

    gotoxy(4, 9); cout<< "TOTAL";
    gotoxy(18, 9); cout<< "RENDA BRUTA";
    gotoxy(40, 9); cout<< "MEDIA/PRODUTO";
    gotoxy(62, 9); cout<< "MAIS VENDIDO";
    gotoxy(88, 9); cout<< "MENOS VENDIDO";

    if(getline(leitura, s)){
        gotoxy(4,10); cout << s;
    }
    else{
        leitura.close();
        gotoxy(4,10); cout << "Arquivo vazio";
        return;
    }
    if(getline(leitura, s))
        gotoxy(18,10); cout << "R$ " << stod(s);

    if(getline(leitura, s))
        gotoxy(40,10); cout << "R$ " << stod(s);

    if(getline(leitura, s))
        gotoxy(62,10); cout << s;
    if(getline(leitura, s))
       cout << " ("<< s << ")";

    if(getline(leitura, s))
        gotoxy(88,10); cout << s;
    if(getline(leitura, s))
        cout << " (" << s << ")";

    leitura.close();
}

// Exibir o relatorio de vendas listadas


void exibirLista(const char* arquivo) {
    int linhas = 0;
    ifstream leitura(arquivo);
    if(!leitura.is_open()) {
        cerr << "Erro ao tentar abrir o arquivo!" << endl;
        return;
    }
    string s;
    while(getline(leitura,s)) {
        linhas++;
    }
    leitura.close();

    int qnt = (linhas)/6;


    string* produto[6];
    string data[qnt+1], ID[qnt+1], nomeProduto[qnt+1], preco[qnt+1], unidades[qnt+1], precoTotal[qnt+1];

    int p = 0, resto = 0;
    leitura.open(arquivo);
    for(int i = 0; i < linhas; i++){
        getline(leitura, s);
        resto = i % 6;
        if(resto == 0){
            data[p] = s;
        }
        else if(resto == 1){
            ID[p] = s;
        }
        else if(resto == 2){
            nomeProduto[p] = s;
        }
        else if(resto == 3){
            preco[p] = s;
        }
        else if(resto == 4){
            unidades[p] = s;
        }
        else if(resto == 5){
            precoTotal[p] = s;
            p++;
        }
    }
    leitura.close();

    produto[0] = data;
    produto[1] = ID;
    produto[2] = nomeProduto;
    produto[3] = preco;
    produto[4] = unidades;
    produto[5] = precoTotal;

    int desliza = 0, cont = 0;
    while(cont < 16){
        limparTela(26, 11, 16, 2, LIGHTGRAY);
        limparTela(34, 11, 16, 20, LIGHTGRAY);
        limparTela(64, 11, 16, 12, LIGHTGRAY);
        limparTela(85, 11, 16, 2, LIGHTGRAY);
        limparTela(104, 11, 16, 12, LIGHTGRAY);
        for(int i = desliza; i < 16 + desliza; i++){
            int mult = 0;
            if (i == qnt)
                    break;
            for(int j = 0; j < 6; j++){
                if(j == 2) (mult = 15*j);
                else (mult = 20*j);
                gotoxy(4 + mult, cont+11);
                cout << produto[j][i];
            }
            cont++;
        }
        if(logicaListaVendaRegistro(desliza, qnt)){
            cont = 0;
        }else if (cont == qnt){
            break;
        } else{
            limparTela(3, 9, 18, 114, WHITE);
            break;
        }
    }
    desliza = 0; cont = 0;
}

// Exibir o relatorio de vendas por intervalo
void exibirListaIntervalo(const char* arquivo) {
    int linhas = 0;
    ifstream leitura(arquivo);
    if(!leitura.is_open()) {
        cerr << "Erro ao tentar abrir o arquivo!" << endl;
        return;
    }
    string s;
    while(getline(leitura,s)) {
        linhas++;
    }
    leitura.close();

    string listaIntervalo[linhas];
    leitura.open(arquivo);
    int o = 0;
    while(getline(leitura,s)) {
        listaIntervalo[o] = s;
        o++;
    }
    leitura.close();

    int desliza = 0, cont = 0;
    while(cont < 16){
        limparTela(3, 9, 18, 114, LIGHTGRAY);
        for(int i = desliza; i < 16+desliza; i++){
            if (i == linhas)
                break;
            gotoxy(4, 9+cont); cout << listaIntervalo[i];
            cont++;
        }
        if(logicaListaVendaRegistro(desliza, linhas)){
            cont = 0;
        }else if (cont == linhas){
            break;
        } else{
            limparTela(3, 9, 18, 114, WHITE);
            break;
        }
    }
    desliza = 0; cont = 0;
}


#endif // RELATORIO_VENDAS_H
