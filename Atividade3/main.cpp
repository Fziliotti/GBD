/*
 * File:   main.cpp
 */
//Alunos :
//Fabrício Fernandes Ziliotti | Matricula: 11711BCC020
//Felipe Augusto Ferreira de Castro | Matricula: 11711BCC033
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <map>
#include <fstream>

using namespace std;

typedef map<string,int>LS_MAP;
typedef struct lista_int{int offset; struct lista_int* prox;}LISTA_INT;
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
    LS_MAP ls_palavras;
    FILE* f;

    // construtor
    listaInvertida(){ f = fopen("listainvertida.dat","wb+"); }
    // destrutor
    ~listaInvertida(){ls_palavras.clear();fclose(f);}

    // adiciona palavra na estrutura
    void adiciona(char *palavra, int offset){
         string p(palavra);
         LS_MAP::iterator it;
         pair<LS_MAP::iterator,bool>prt;
         it = ls_palavras.find(p);
         if(it == ls_palavras.end()){
                // palavra nao existe na lista
                prt = ls_palavras.insert(pair<string,int>(p,-1)); // insere um novo elemento apontando para nada
                if(!insere_lista_Invertida(prt.first,offset)){printf("ERRO AO INSERIR NA LISTA!! ENCERRANDO FUNÇAO..."); exit(1);}
                return;
             }
        // palavra ja existe na lista
        if(!insere_lista_Invertida(it,offset)){printf("ERRO AO INSERIR NA LISTA!! ENCERRANDO FUNÇAO..."); exit(1);} //insere o novo offset para a palavra
    }
    // realiza busca, retornando vetor de offsets que referenciam a palavra
    int* busca(char *palavra, int *quantidade){
        LS_MAP::iterator it;
        string s(palavra);
        it = ls_palavras.find(s);
        if(it == ls_palavras.end()){
            printf("Palavra nao encontrada....");
            *quantidade = 0;
            return NULL;
        }
        LISTA_INT* topo;
        LISTA_INT* li;
        LISTA_INT* aux;
        LI_REGISTRO r;
        int pos = it->second;
        fseek(f,pos,SEEK_SET);
        fread(&r,sizeof(LI_REGISTRO),1,f);
        // substituir pelo resultado da busca na lista invertida
        li = (LISTA_INT*)malloc(sizeof(LISTA_INT));
        li->offset = r.offset;
        li->prox = NULL;
        topo = li;
        *quantidade = 1;
        while(r.prox!=-1){
            fseek(f,r.prox,SEEK_SET);
            fread(&r,sizeof(LI_REGISTRO),1,f);
            aux = (LISTA_INT*)malloc(sizeof(LISTA_INT));
            aux->prox = li;
            aux->offset = r.offset;
            li = aux;
            *quantidade+=1;
        }
        int *offsets = new int[*quantidade];
        for(int i=0;i<(*quantidade);i++){
            offsets[i] = li->offset;
            li = li->prox;
        }
        aux = topo->prox;
        free(topo);
        //desaloca a lista de inteiros
        while(aux != NULL){
            li = aux;
            aux = aux->prox;
            free(li);
        }
        return offsets;
    }
private:

    //insere na lista Invertida
    int insere_lista_Invertida(LS_MAP::iterator it, int offset){
        if(it == ls_palavras.end()){return 0;}
        int novo_offset;

        LI_REGISTRO r;
        r.offset = offset;
        r.prox = it->second; // o novo registro aponta para o topo da lista
        //// insere no final do arquivo
        fseek(f,0,SEEK_END);
        novo_offset = ftell(f); // salva posicao onde novo registro foi escrito
        fwrite(&r,sizeof(LI_REGISTRO),1,f); // escreve novo registro
        (*it).second = novo_offset; // topo da lista agora é o novo registro;
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

