//Alunos :
//Fabrício Fernandes Ziliotti | Matricula: 11711BCC020
//Felipe Augusto Ferreira de Castro | Matricula: 11711BCC033

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 50

class MeuArquivo {
public:

    struct cabecalho { int quantidade; int disponivel; } cabecalho;

    struct registro { char tamanho; char *palavra; } registro;

    // construtor: abre arquivo. Essa aplicacao deveria ler o arquivo se existente ou criar um novo.
    // Entretando recriaremos o arquivo a cada execucao ("w+").
    MeuArquivo() {
        this->cabecalho.quantidade = 0;
        this->cabecalho.disponivel = -1;
        fd = fopen("dados.dat","w+");
    }

    // Destrutor: fecha arquivo
    ~MeuArquivo() {
        fclose(fd);
    }

    void atualizaCabecalho(int novaQuantidade,int novoDisponivel) {
        this->cabecalho.quantidade = novaQuantidade;
        this->cabecalho.disponivel = novoDisponivel;

        rewind(this->fd);


        //Atualizar primeira linha do arquivo para sincronizar o cabecalho inMemory lá
    }

    // Insere uma nova palavra, consulta se há espaco disponível ou se deve inserir no final
    void inserePalavra(char *palavra) {
        this->substituiBarraNporBarraZero(palavra); // funcao auxiliar substitui terminador por \0

        // implementar aqui
        //fwrite(&registro.tamanho,sizeof(char),1,this->fd);
        //fwrite(registro.palavra,registro.tamanho,1,this->fd);

        fwrite(palavra,sizeof(palavra),1,this->fd);
        //fprintf(this->fd, "%d%s\n",strlen(palavra),palavra);
    }

    // Marca registro como removido, atualiza lista de disponíveis, incluindo o cabecalho
    void removePalavra(int offset) {
        // implementar aqui

        char *palavra = new char[50];
        while (!feof(this->fd)) {
            fgets(palavra,50,this->fd);
            printf(palavra);
        }
    }

    // BuscaPalavra: retorno é o offset para o registro
    // Nao deve considerar registro removido

    int buscaPalavra(char *palavra) {
        this->substituiBarraNporBarraZero(palavra); // funcao auxiliar substitui terminador por \0

        char line_buffer[4*BUFFER_SIZE];
        char word_buffer[BUFFER_SIZE];
        char ch;
        int word_count;
        int line_count;
        int equal_flag = 0;
        int offset;

        rewind(this->fd);

        while((ch=fgetc(this->fd))!=EOF){
            // Read Lines
            word_count = 0;
            line_count = 0;
            offset = 1; // >0 se encontrar
            return offset;

        }
        // retornar -1 caso nao encontrar
        return -1;
    }


private:
    // descritor do arquivo é privado, apenas métodos da classe podem acessa-lo
    FILE *fd;

    // funcao auxiliar substitui terminador por \0
    void substituiBarraNporBarraZero(char *str) {
        int tam = strlen(str); for (int i = 0; i < tam; i++) if (str[i] == '\n') str[i] = '\0';
    }
};

int main(int argc, char** argv) {
    // abrindo arquivo dicionario.txt
    FILE *f = fopen("dicionario.txt","rt");

    // se não abriu
    if (f == NULL) {
        printf("Erro ao abrir arquivo.\n\n");
        return 0;
    }

    char *palavra = new char[50];

    // criando arquivo de dados
    MeuArquivo *arquivo = new MeuArquivo();
    arquivo->inserePalavra(palavra);
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
            // Se existir offset, existe a palavra dentro do arquivo
            if (offset == -1) {
                printf("Palavra não existente, tente novamente.\n\n");
            }
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
                printf("Encontrou %s na posição %d\n\n",palavra,offset);
            else
                printf("Não encontrou %s\n\n",palavra);
        }
        if (opcao != '4') opcao = getchar();
    } while (opcao != '4');

    printf("\n\nAte mais!\n\n");

    return (EXIT_SUCCESS);
}
