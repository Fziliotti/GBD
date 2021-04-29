/*
 * File:   main.cpp
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>

using namespace std;

// remove pontuacao de uma palavra
void removePontuacao (char *palavra) {
    int length = strlen(palavra);
    if (
        (palavra[length-1] == '.') || (palavra[length-1] == ',') || (palavra[length-1] == ';') ||
        (palavra[length-1] == ':') || (palavra[length-1] == '?') || (palavra[length-1] == '!')
       )
        palavra[length-1] = '\0';
}

// imprime linha do arquivo com base no offset da palavra
void imprimeLinha(int offset,FILE *f) {
    int pos = ftell(f);
    char linha[2048];
    while (pos < offset) {
        fgets(linha,2047,f);
        pos = ftell(f);
    }
    printf("%s",linha);
}

// classe que implementa a lista invertida
class listaInvertida {
public:
    typedef struct registro_listaInvertida{int offset;int prox;}LI_REGISTRO; //offset da palavra, prox é o offset do proximo registro da lista
    typedef struct registro_listaSecundaria{char palavra[100];int quantidade;int lista_offset;LS_REGISTRO* prox;}LS_REGISTRO; //lista_offset é o offset do primero registro da lista invertida,  prox é um ponteiro pra o proximo item da lista de chaves
    FILE* f;
    LS_REGISTRO* primeiro_registro = NULL; // aponta para o a cabeça da lista de chaves secundarias

    // construtor
    listaInvertida(){ f=open("listainvertida.dat","wb+"); }
    // destrutor
    ~listaInvertida(){destroi_listasecundaria();close(f);}

    // adiciona palavra na estrutura
    void adiciona(char *palavra, int offset) {
        if(primeiro_registro == NULL){
            primeiro_registro =  novo_LS_REGISTRO();
            primeiro_registro->palavra =  *palavra;
        }

        // vamos iterar na lista secundaria para encontrar o registro correto
        LS_REGISTRO* aux =  primeiro_registro;
        while(1){
            if(aux->palavra == (*palavra))
            {
                // a palavra ja esta na lista

            }
        }

    }
    // realiza busca, retornando vetor de offsets que referenciam a palavra
    int * busca(char *palavra, int *quantidade) {
        // substituir pelo resultado da busca na lista invertida
        quantidade[0] = 10;
        int *offsets = new int[10];
        int i = 0;
        // exemplo: retornar os primeiros 10 offsets da palavra "terra"
        offsets[i++] = 58;
        offsets[i++] = 69;
        offsets[i++] = 846;
        offsets[i++] = 943;
        offsets[i++] = 1083;
        offsets[i++] = 1109;
        offsets[i++] = 1569;
        offsets[i++] = 1792;
        offsets[i++] = 2041;
        offsets[i++] = 2431;
        return offsets;
    }
private:

     // desaloca  a lista de chaves secundarias
    void destroi_listasecundaria(){
        if(primeiro_registro==NULL)return;

        LS_REGISTRO* aux;
        while(1){
               aux =  primeiro_registro->prox;
               free(primeiro_registro);
               if(aux->prox == NULL){
                break;
               }
               primeiro_registro = aux->prox;
        }
        return;
    }

    //cria um novo Registro da lista secundaria
    LS_REGISTRO novo_LS_REGISTRO(){
        LS_REGISTRO* registro;
        registro = malloc(sizeof(LS_REGISTRO));
        registro->lista_offset = NULL;
        registro->quantidade = 0;
        registro->prox = NULL;
        return registro;
    }

    //insere na lista Invertida
    int insere_lista_Invertida(LS_REGISTRO* registro, int offset){
        if(registro == NULL){return 0;}


        LI_REGISTRO r;
        r.offset = offset;
        r.prox = -1;

        int li_registro_offset = registro->lista_offset;
        LI_REGISTRO aux;
        LI_REGISTRO ant;
        int ant_offset= -1;
        while(1){
            fseek(f,li_registro_offset,SEEK_SET);
            fread(&aux,sizeof(LI_REGISTRO),1,f);
            if(offset<=aux.offset){
                break;
            }
            ant = aux;
            ant_offset = li_registro_offset; // registro anterior
            if(aux.prox == -1){break;}
            li_registro_offset = aux.prox; //proximo registro

        }
        r.prox = li_registro_offset;
        fseek(f,0,SEEK_END);
        if(ant_offset!=-1){
            ant.prox = ftell(f); // pega a posção onde o registro atual vai ser escrito;
        }else{
            registro->lista_offset = ftell(f);
        }
        fwrite(&r,sizeof(LI_REGISTRO),1,f);
        if(ant_offset!=-1){
            fseek(f,ant_offset,SEEK_SET);
            fwrite(&ant,sizeof(LI_REGISTRO),1,f);
        }

        return 1;

    }
};

// programa principal
int main(int argc, char** argv) {
    // abrir arquivo
    ifstream in("biblia.txt");
    if (!in.is_open()){
        printf("\n\n Nao consegui abrir arquivo biblia.txt. Sinto muito.\n\n\n\n");
    }
    else{
        // vamos ler o arquivo e criar a lista invertida com as palavras do arquivo
        char *palavra = new char[100];
        int offset, contadorDePalavras = 0;
        listaInvertida lista;
        // ler palavras
        while (!in.eof()) {
            // ler palavra
            in >> palavra;
            // pegar offset
            offset = in.tellg();
            // remover pontuacao
            removePontuacao(palavra);
            // desconsiderar palavras que sao marcadores do arquivo
            if (!((palavra[0] == '#') || (palavra[0] == '[') || ((palavra[0] >= '0') && (palavra[0] <= '9')))) {
                //printf("%d %s\n", offset,palavra); fflush(stdout); // debug :-)
                lista.adiciona(palavra, offset);
                contadorDePalavras++;
                if (contadorDePalavras % 1000 == 0) { printf(".");  fflush(stdout); }
            }
        }
        in.close();

        // agora que ja construimos o indice, podemos realizar buscas
        do {
            printf("\nDigite a palavra desejada ou \"SAIR\" para sair: ");
            scanf("%s",palavra);
            if (strcmp(palavra,"SAIR") != 0) {
                int quantidade;
                // busca na lista invertida
                int *offsets = lista.busca(palavra,&quantidade);
                // com vetor de offsets, recuperar as linhas que contem a palavra desejada
                if (quantidade > 0) {
                    FILE *f = fopen("biblia.txt","rt");
                    for (int i = 0; i < quantidade; i++)
                        imprimeLinha(offsets[i],f);
                    fclose(f);
                }
                else
                    printf("nao encontrou %s\n",palavra);
            }
        } while (strcmp(palavra,"SAIR") != 0);

        printf("\n\nAte mais!\n\n");
    }

    return (EXIT_SUCCESS);
}

