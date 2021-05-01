/*
 * File:   main.cpp
 */
//Alunos :
//Fabrício Fernandes Ziliotti | Matricula: 11711BCC020
//Felipe Augusto Ferreira de Castro | Matricula: 11711BCC033
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
    typedef struct registro_listaSecundaria{char palavra[100];int quantidade;int lista_offset;struct registro_listaSecundaria* prox;}LS_REGISTRO; //lista_offset é o offset do primero registro da lista invertida,  prox é um ponteiro pra o proximo item da lista de chaves
    FILE* f;
    LS_REGISTRO* primeiro_registro = NULL; // aponta para o a cabeça da lista de chaves secundarias

    // construtor
    listaInvertida(){ f = fopen("listainvertida.dat","wb+"); }
    // destrutor
    ~listaInvertida(){destroi_listasecundaria();fclose(f);}

    // adiciona palavra na estrutura
    void adiciona(char *palavra, int offset){
         if(primeiro_registro == NULL){
                primeiro_registro = novo_LS_REGISTRO();
                strcpy(primeiro_registro->palavra,palavra);
                if(!insere_lista_Invertida(primeiro_registro,offset)){printf("ERRO AO INSERIR NA LISTA!! ENCERRANDO FUNÇAO..."); exit(1);}
                return;
             }

        // vamos iterar na lista secundaria para encontrar o registro correto
        LS_REGISTRO* aux =  primeiro_registro;
        LS_REGISTRO* ant =  NULL;
        int cmp;
        while(1){
            cmp = strcmp(palavra,aux->palavra);
            if(cmp==0){
                //encontro a palavra
                break;
            }
             //checa se é o fim da lista
            if(aux->prox == NULL){
                //fim da lista;
                aux->prox = novo_LS_REGISTRO();
                strcpy(aux->prox->palavra,palavra);
                aux = aux->prox;
                break;
            }

            if(cmp<0){
                // se for menor e ainda nao encontrou significa que a palavra nao esta na lista
                LS_REGISTRO* novo = novo_LS_REGISTRO();
                strcpy(novo->palavra,palavra);
                if(ant!=NULL){
                    ant->prox = novo;
                }else{
                    primeiro_registro = novo;
                }
                novo->prox = aux;
                aux = novo;
                break;
            }
            ant = aux;
            aux = aux->prox;


        }
        // o registro a inserir a palavra sempre sai em aux
        if(!insere_lista_Invertida(aux,offset)){printf("ERRO AO INSERIR NA LISTA!! ENCERRANDO FUNÇAO..."); exit(1);} //insere o novo offset para a palavra
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
    LS_REGISTRO* novo_LS_REGISTRO(){
        LS_REGISTRO* registro;
        registro = (LS_REGISTRO*)malloc(sizeof(LS_REGISTRO));
        registro->lista_offset = -1;
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

        int registro_atual = registro->lista_offset;
        LI_REGISTRO atual;
        LI_REGISTRO ant;
        int ant_offset= -1;
        while(1){
            if(registro_atual == -1){break;} // fim da lista
            fseek(f,registro_atual,SEEK_SET);
            fread(&atual,sizeof(LI_REGISTRO),1,f);
            if(offset<=atual.offset){
                break;
            }
            ant = atual;
            ant_offset = registro_atual; // registro atual agor a é o anterior
            registro_atual = atual.prox; //proximo registro

        }
        r.prox = registro_atual; // o registro a ser inserio aponta para o registro atual na posiçao
        fseek(f,0,SEEK_END); // seek pro final do arquivo
        int r_offset = ftell(f);
        fwrite(&r,sizeof(LI_REGISTRO),1,f);//escreve o registro a ser inserido no fim do arquivo
        if(ant_offset!=-1){
            ant.prox = r_offset;
            fseek(f,ant_offset,SEEK_SET);
            fwrite(&ant,sizeof(LI_REGISTRO),1,f); // escreve o registro anterior se necessário
        }else{
            registro->lista_offset = r_offset;// caso o registro inserio seja o primeiro da lista faz apontar para a cabeça que esta na lista de chave secundaria
        }
        registro->quantidade++;
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
                printf("%d %s\n", offset,palavra); fflush(stdout); // debug :-)
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

