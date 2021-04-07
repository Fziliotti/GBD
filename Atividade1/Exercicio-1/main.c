//Alunos : Felipe Augusto Ferreira de Castro | Matricula: 11711BCC033
//Alunos : Fabrício Fernandes Ziliotti | Matricula: 11711BCC020

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//////////////////////////////////////constantes /////////////////////////////////////////
#define ARQ_PATH "cep.txt"
#define OUTPUT "output.txt"
#define CEP_SIZE 8 // SIZE IN BYTES
#define ESTADO_SIZE 2 //SIZE IN BYTES
#define BUFFER_SIZE 50


//////////////////////////////////funcoes ///////////////////////////////////////////

int read_line(char* cep, char* estado,char* cidade,char* rua,FILE* fp){
int count;
char ch;
// Read CEP
    if(fread(cep,CEP_SIZE,1,fp)==0){
        return 0;
    }
    cep[8]='\0';
    fgetc(fp);
// Read ESTADO
    fread(estado,ESTADO_SIZE,1,fp);
    estado[2]='\0';
    fgetc(fp); // reading one more byte to take off the \t character
//Read CIDADE
    count = 0;
    while(((ch=fgetc(fp))!='\t') && (ch != '\n')){
        cidade[count++] = ch;
    }
    cidade[count]='\0'; // that's just to mark where the string end inside CIDADE buffer
//Read RUA
    if(ch=='\t'){
        count = 0;
        while((ch=fgetc(fp))!='\n'){
                         rua[count++] = ch;
        }
        rua[count]='\0'; // that's just to mark where the string end inside RUA buffer
    }else{
        rua[0]='\0';
    }
return 1;
}


int search(char* key,FILE*fp){

char line_buffer[4*BUFFER_SIZE];
char word_buffer[BUFFER_SIZE];
char ch;
int word_count;
int line_count;
int equal_flag = 0;

rewind(fp);
while((ch=fgetc(fp))!=EOF){
    // Read Lines
    word_count = 0;
    line_count = 0;
    do{
        if(ch!='|'){
            word_buffer[word_count++] = ch;
            line_buffer[line_count++] = ch;
        }else{
            word_buffer[word_count]='\0';
            line_buffer[line_count++] ='\t';
            if(strcmp(key,word_buffer)==0){
                equal_flag = 1;
            }
            word_count = 0;
        }
    }while((ch=fgetc(fp))!='\n');
            word_buffer[word_count]='\0';
            if(strcmp(key,word_buffer)==0){
                equal_flag = 1;
            }
    // see if key matches
    if(equal_flag){
        line_buffer[line_count]='\0';
        printf("%s\n",line_buffer);
    }
    equal_flag = 0;
}



return 0;
}

////////////////////////////////////////////////////// MAIN//////////////////////////////////////////////
int main(){

FILE* fp;
FILE* output;
char CEP[CEP_SIZE+1];
char ESTADO[ESTADO_SIZE+1];
char CIDADE[BUFFER_SIZE];
char RUA[BUFFER_SIZE];

fp=fopen(ARQ_PATH,"r");
output=fopen(OUTPUT,"w+");
if(fp==NULL){
    return 0;
}else{
    while(1)
    {
        //Read lines
            if(!read_line(CEP,ESTADO,CIDADE,RUA,fp)){
                break; // break the while loop if cant read a new line
            }
        //Write lines
            if(RUA[0]=='\0'){
                fprintf(output,"%s|%s|%s\n",CIDADE,ESTADO,CEP);
            }else{
                fprintf(output,"%s|%s|%s|%s\n",RUA,CIDADE,ESTADO,CEP);
            }
                //fprintf(output,"%s|%s|%s\n",CIDADE,ESTADO,CEP);
    }
}
fclose(fp);
fclose(output);

char input[BUFFER_SIZE];

printf("Digite o que deseja buscar(CIDADE,ESTADO,RUA,CEP): ");
scanf("%[^\n]s", input);
output = fopen(OUTPUT,"r");
search(input,output);
fclose(output);

return 0;
}
