//Alunos : Felipe Augusto Ferreira de Castro | Matricula: 11711BCC033
//Alunos : Fabrício Fernandes Ziliotti | Matricula: 11711BCC020

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//////////////////////////////////////constantes /////////////////////////////////////////
#define ARQ_PATH "cep.txt"
#define OUTPUT "output"
#define BUFFER_SIZE 50
#define CEP_SIZE 8 // SIZE IN BYTES
#define ESTADO_SIZE 2 //SIZE IN BYTES
#define ITEM_SIZE sizeof(ITEM)

typedef struct{
    int cep;
    char uf[3];
    char cidade[39];
    char logradouro[67];
} ITEM; // 4 + 3 + 39 + 67 = 113 bytes

//////////////////////////////////funcoes ///////////////////////////////////////////

int read_item(ITEM* registro,FILE* fp){
int count;
char ch;
char cep[CEP_SIZE+1];
// Read CEP
    if(fread(cep,CEP_SIZE,1,fp)==0){
        return 0;
    }
    cep[8]='\0';
    registro->cep =  atoi(cep); // convert to int and save on the registro
    fgetc(fp);
// Read ESTADO
    fread(registro->uf,ESTADO_SIZE,1,fp);
    registro->uf[2]='\0';
    fgetc(fp); // reading one more byte to take off the \t character
//Read CIDADE
    count = 0;
    while(((ch=fgetc(fp))!='\t') && (ch != '\n')){
        registro->cidade[count++] = ch;
    }
    registro->cidade[count]='\0'; // that's just to mark where the string end inside CIDADE buffer
//Read RUA
    if(ch=='\t'){
        count = 0;
        while((ch=fgetc(fp))!='\n'){
                registro->logradouro[count++] = ch;
        }
        registro->logradouro[count]='\0'; // that's just to mark where the string end inside RUA buffer
    }else{
        registro->logradouro[0]='\0';
    }




return 1;
}


int search(char* key,FILE*fp){

ITEM registro;
int i;
int equal_flag = 0;

i = atoi(key);
rewind(fp);
if(i!=0){
while(fread((&registro),sizeof(ITEM),1,fp)){
     // seeing if its a cep
        if(i==(registro.cep)){
            printf("%d\t%s\t%s\t%s",(&registro)->cep,(&registro)->uf,(&registro)->cidade,(&registro)->logradouro);
            return 1;
        }
}
}else{

while(fread((&registro),sizeof(ITEM),1,fp)){

    equal_flag = 0;
    if(strcmp(((&registro)->uf),key)==0){
            equal_flag = 1;
    }else if(strcmp(((&registro)->cidade),key)==0){
        equal_flag = 1;
    }else if(strcmp(((&registro)->logradouro),key)==0){
        equal_flag = 1;
    }
    if(equal_flag){
        printf("%d\t%s\t%s\t%s\n",(&registro)->cep,(&registro)->uf,(&registro)->cidade,(&registro)->logradouro);
    }
}
}


return 0;
}
////////////////////////////////////////////////////// MAIN//////////////////////////////////////////////
int main(){

FILE* fp;
FILE* output;
ITEM  registro;
fp=fopen(ARQ_PATH,"r");
output=fopen(OUTPUT,"wb");

if(fp==NULL){
    return 0;
}else{
    while(1)
    {
        //Read lines
            if(!read_item((&registro),fp)){
                break; // break the while loop if cant read a new line
            }
        //Write lines
            if(fwrite((&registro),sizeof(ITEM),1,output)==0)printf("ERRO DE GRAVACAO");
    }
}

fclose(fp);
fclose(output);



char input[BUFFER_SIZE];

printf("Digite o que deseja buscar(CIDADE,ESTADO,RUA,CEP): ");
scanf("%[^\n]s", input);
output = fopen(OUTPUT,"rb");
search(input,output);
fclose(output);

int numero;
printf("Digite o numero do registro: ");
scanf("%d",&numero);
output = fopen(OUTPUT,"rb");
fseek(output,numero*sizeof(ITEM),0);
if(fread((&registro),sizeof(ITEM),1,output)){
printf("%d\t%s\t%s\t%s\n",(&registro)->cep,(&registro)->uf,(&registro)->cidade,(&registro)->logradouro);
}else{
printf("registro nao encontrado");
}
fclose(output);

return 0;
}
