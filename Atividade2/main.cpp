//Alunos :
//Fabr�cio Fernandes Ziliotti | Matricula: 11711BCC020
//Felipe Augusto Ferreira de Castro | Matricula: 11711BCC033

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define HEADER_OFFSET 0


class MeuArquivo {
public:
    struct cabecalho { int quantidade;int disponivel; } cabecalho;
    struct registro { int tamanho_bytes; char* palavra; } registro; // guarda o tamamn

    // construtor: abre arquivo. Essa aplicacao deveria ler o arquivo se existente ou criar um novo.
    // Entretando recriaremos o arquivo a cada execucao ("w+"). wb+ para podermos trabalhar melhor com binarios
    MeuArquivo() {
        fd = fopen("dados.dat","wb+");
        this->cabecalho.disponivel = -1; //  inicia o cabe�alho
        this->cabecalho.quantidade = 0; // inicia o cabe�alho
        fwrite(&cabecalho,2*sizeof(int),1,fd);  // escreve o cabe�alho no arquivo
    }

    // Destrutor: fecha arquivo
    ~MeuArquivo() {
        fclose(fd);
    }

    void atualizaCabecalho(int quantidade, int disponivel){
        long  int pos_atual = ftell(fd); //  salva posi��o atual no arquivo;
        fseek(fd,0,SEEK_SET); //  volta o ponteiro para a posi��o do cabe�alho
        this->cabecalho.disponivel = quantidade; //  atualiza o cabe�alho
        this->cabecalho.quantidade = disponivel; // atualiza o cabe�alho
        fwrite(&cabecalho,2*sizeof(int),1,fd);  // escreve o cabe�alho atualizado
        fseek(fd,pos_atual,SEEK_SET); // retorna o ponteiro para a posi��o original

    }

    void atualizaFilaInsercao(int anterior,int prox){
        if(anterior == -1){
            // o anterior � o cabecalho
            atualizaCabecalho(this->cabecalho.quantidade+1,prox);
        }else{

            fseek(fd,anterior+sizeof(int)+1,SEEK_SET); // seta o ponteiro para o inicio do registro disponivel anterior
            fwrite(&prox,sizeof(int),1,fd);
        }
    }

    // Insere uma nova palavra, consulta se h� espaco dispon�vel ou se deve inserir no final
    void inserePalavra(char *palavra){
        this->substituiBarraNporBarraZero(palavra); // funcao auxiliar substitui terminador por \0

        this->registro.palavra = palavra; // atribui o ponteiro da palavra para o ponteiro palavra no registro
        this->registro.tamanho_bytes = strlen(palavra) + 1; // salva o tamanho da palavra em bytes (cada char � 1 byte) + 1 para o terminador de string

            int anterior = -1;
            int atual = this->cabecalho.disponivel;
            int prox = -1;
            int tamanho_espaco = 0;

            while(1){
                if(atual == -1){
                    //insere o registro no fim do arquivo

                    fseek(fd,0,SEEK_END); // move o ponteiro para o fim do arquivo;
                    fwrite(&(this->registro.tamanho_bytes),sizeof(int),1,fd);
                    fwrite(this->registro.palavra,this->registro.tamanho_bytes,1,fd);
                    atualizaCabecalho(this->cabecalho.quantidade+1,atual);
                    break;
                }

                fseek(fd,atual,SEEK_SET); // seta o ponteiro para o atual espa�o disponivel;
                fread(&tamanho_espaco,sizeof(int),1,fd);

                fseek(fd,1,SEEK_CUR); //pula 1 byte referente ao asterisco
                fread(&prox,sizeof(int),1,fd); // le o offset do proximo registro disponivel

                if(tamanho_espaco< this->registro.tamanho_bytes)
                {
                    // registro n�o cabe neste espa�o vazio necessario ir para o proximo
                    anterior = atual;
                    atual = prox;
                }else{
                    // registro pode ser inserido neste espa�o
                     fseek(fd,prox,SEEK_SET); // seta o ponteiro para o inicio do registro disponivel;
                     fwrite(&(this->registro.tamanho_bytes),sizeof(int),1,fd);
                     fwrite(this->registro.palavra,this->registro.tamanho_bytes,1,fd);
                     atualizaFilaInsercao(anterior,prox); //atualiza o registro anterior a apontar para o proximo registro;
                     break;
                }
            }

    }

    // Marca registro como removido, atualiza lista de dispon�veis, incluindo o cabecalho
    void removePalavra(int offset) {
        fseek(fd,offset,SEEK_SET); // seta o ponteiro para o registro
        char terminador = '*';

        fseek(fd,sizeof(int),SEEK_CUR); // seta o ponteiro para depois do tamanho do registro;
        fwrite(&terminador,sizeof(char),1,fd); // marca o registro com o terminador
        fwrite(&(this->cabecalho.disponivel),sizeof(int),1,fd); // aponta para o proximo registro vazio disponivel que o cabe�alho apontava

        atualizaCabecalho(this->cabecalho.quantidade-1,offset); // cabe�alho agora aponta para este registro

    }

    // BuscaPalavra: retorno � o offset para o registro
    // Nao deve considerar registro removido
    int buscaPalavra(char *palavra) {
        this->substituiBarraNporBarraZero(palavra); // funcao auxiliar substitui terminador por \0

        // implementar aqui

        // retornar -1 caso nao encontrar
        return -1;
    }

private:
    // descritor do arquivo � privado, apenas m�todos da classe podem acessa-lo
    FILE *fd;

    // funcao auxiliar substitui terminador por \0
    void substituiBarraNporBarraZero(char *str) {
        int tam = strlen(str); for (int i = 0; i < tam; i++) if (str[i] == '\n') str[i] = '\0';
    }
};

int main(int argc, char** argv) {
    // abrindo arquivo dicionario.txt
    FILE *f = fopen("dicionario.txt","rt");

    // se n�o abriu
    if (f == NULL) {
        printf("Erro ao abrir arquivo.\n\n");
        return 0;
    }

    char *palavra = new char[50];

    // criando arquivo de dados
    MeuArquivo *arquivo = new MeuArquivo();
    while (!feof(f)) {
        fgets(palavra,50,f);
        arquivo->inserePalavra(palavra);
    }

    // fechar arquivo dicionario.txt
    fclose(f);

    printf("Arquivo criado.\n\n");

    char opcao;
    do {
        printf("\n\n1-Insere\n2-Remove\n3-Busca\n4-Sair\nOpcao:");
        opcao = getchar();
        if (opcao == '1') {
            printf("Palavra: ");
            scanf("%s",palavra);
            arquivo->inserePalavra(palavra);
        }
        else if (opcao == '2') {
            printf("Palavra: ");
            scanf("%s",palavra);
            int offset = arquivo->buscaPalavra(palavra);
            if (offset >= 0) {
                arquivo->removePalavra(offset);
                printf("Removido.\n\n");
            }
        }
        else if (opcao == '3') {
            printf("Palavra: ");
            scanf("%s",palavra);
            int offset = arquivo->buscaPalavra(palavra);
            if (offset >= 0)
                printf("Encontrou %s na posi��o %d\n\n",palavra,offset);
            else
                printf("N�o encontrou %s\n\n",palavra);
        }
        if (opcao != '4') opcao = getchar();
    } while (opcao != '4');

    printf("\n\nAte mais!\n\n");

    return (EXIT_SUCCESS);
}
