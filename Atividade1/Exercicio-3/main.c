//Alunos : Felipe Augusto Ferreira de Castro | Matricula: 11711BCC033
//Alunos : Fabrício Fernandes Ziliotti | Matricula: 11711BCC020

#include<string.h>
#include<stdlib.h>
#include<stdio.h>

int main(){

    FILE *cepFileToWrite = fopen("cep2.txt", "wb"); //write in binary 
    FILE *cepFileToRead = fopen("cep.txt", "rb"); //read in binary

    char currentLine[500]; // limitSize
    int position, newPosition, currentByteToRead;

    currentByteToRead = ftell(cepFileToRead);

    fwrite(&currentByteToRead, sizeof(int), 1, cepFileToWrite);


    while(fgets(currentLine, sizeof(currentLine), cepFileToRead)){
        if(strcspn(currentLine, "\n") == 1) //(==EOF)
            break;

        currentByteToRead = ftell(cepFileToRead); // to know what byte the cursor is positioned 
        fwrite(&currentByteToRead, sizeof(int), 1, cepFileToWrite);
    }
    fclose(cepFileToWrite);

    cepFileToWrite = fopen("cep2.txt","rb");

    while(1){
        printf("Qual posicao você deseja buscar? (Digite um numero negativo para Sair): ");
        scanf("%d", &position);

        // validation, position 0 is enabled to search
        if(position < 0){
            printf("Números negativos não são permitidos, tente novamente");
            break;
        }

        // Exit Program 
        if(position  == "S"){
            break;
        }

        // "Assim, se eu quiser ler o centesimo registro, basta posicionar-me no índice (fazendo um fseek) com deslocamente (99-1)*4 a partir do início do arquivo, ler um inteiro, e com esse valor, fazer um fseek no arquivo de dados para ler o registro."

        fseek(cepFileToWrite, (position-1)*sizeof(int), SEEK_SET);

        fread(&newPosition, sizeof(int), 1, cepFileToWrite);
        
        fseek(cepFileToRead, newPosition, SEEK_SET);
        
        fgets(currentLine, sizeof(currentLine), cepFileToRead);

        printf("%s",currentLine);
    }

    // close files
    fclose(cepFileToWrite);
    fclose(cepFileToRead);

    return 0;
}
