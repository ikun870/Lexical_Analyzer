#include<stdio.h>
#include<ctype.h>

// 词法分析器
 const char str_array[26][10] = {  
    "",  
    "begin",  
    "end"  ,
    "integer",
    "if",
    "then",
    "else",
    "function",
    "read",
    "write",
    "while",
    "10",
    "11",
    "=",
    "<>",
    "<=",
    "<",
    ">=",
    ">",
    "-",
    "*",
    ":=",
    "(",
    ")",
    ";"
};

enum error_type{
    NO_ERROR,
    IllEGAL_CHARACTER,
    IDENTIFIER_TOO_LONG,
    COLON_MISMATCH 
};

void main(){


    FILE *pas = fopen("label.pas", "w"); // 以只读方式打开文件  
    FILE *dyd = fopen("label.dyd", "w"); // 以只写方式打开文件  
    FILE *error = fopen("label.err", "w"); // 以只写方式打开文件  

    if (pas == NULL) {  
        printf("Failed to open file\n");  
        return;
    }
    if (dyd == NULL || error == NULL) {  
        printf("Failed to make a file\n");  
        return;
    }

    enum error_type error_type=NO_ERROR;

    int line=0;
    while(1){
        char ch = fgetc(pas);
        if(ch == EOF){
            fprintf(dyd, "EOF 25");
            break;
        }
        if(ch == ' ' ||ch == '\t'){
            continue;
        }
        if(ch == '\n'){
            line++;
            fprintf(dyd, "EOLN 24\n");
            continue;
        }
        if(isalpha(ch)){
            char str[17];// 16是最大的标识符长度
            int i = 0;
            while(isalpha(ch)){
                if(i==16)
                {
                    error_type = IDENTIFIER_TOO_LONG;
                }
                str[i++] = ch;
                ch = fgetc(pas);
            }
            str[i] = '\0';
            int flag = 0;
            for(int j = 1; j <= 25; j++){
                if(strcmp(str, str_array[j]) == 0){ //在数组中找到了
                    fprintf(dyd, "%d\n", j);
                    flag = 1;
                    break;
                }
            }
            if(flag == 0){
                 fprintf(dyd, "%s 10\n",str);//暂时没有考虑IDENTIFIER_TOO_LONG的情况，也是当中
                 //标识符输出的
            }
            fseek(pas, -1, SEEK_CUR);
            //这里需要回退一格，因为在while循环中，最后一次读取的字符不是字母，比如说这种begin：
        }
        else if(ch >= '0' && ch <= '9'){
            int num = 0;
            while(ch >= '0' && ch <= '9'){
                num = num * 10 + ch - '0';
                ch = fgetc(pas);
            }
            fprintf(dyd, "%d 11\n", num);
            fseek(pas, -1, SEEK_CUR);
        }
        else{
            switch(ch){
                case ':':{
                    char next = fgetc(pas);
                    if(next == '='){
                        fprintf(dyd, ":= 20\n");
                    }
                    else{
                        error_type = COLON_MISMATCH;
                    }
                    break;
                }
                case '<':{
                    char next = fgetc(pas);
                    if(next == '>'){
                        fprintf(dyd, "<> 13\n");
                    }
                    else if(next == '='){
                        fprintf(dyd, "<= 14\n");
                    }
                    else{
                        fprintf(dyd, "< 15\n");
                        fseek(pas, -1, SEEK_CUR);
                    }
                    break;
                }
                case '>':{
                    char next = fgetc(pas);
                    if(next == '='){
                        fprintf(dyd, "16 >=\n");
                    }
                    else{
                        fprintf(dyd, "16 >\n");
                        fseek(pas, -1, SEEK_CUR);
                    }
                    break;
                }
                case '-':{
                    fprintf(dyd, "- 18\n");
                    break;
                }
                case '*':{
                    fprintf(dyd, "* 19\n");
                    break;
                }
                case '(':{
                    fprintf(dyd, "( 21\n");
                    break;
                }                
                case ')':{
                    fprintf(dyd, ") 22\n");
                    break;
                }
                case ';':{
                    fprintf(dyd, "; 23\n");
                    break;
                }
                default:{
                    error_type = IllEGAL_CHARACTER;
                    break;
                }
            }
        }
        //输出错误信息
        if(error_type != NO_ERROR){
            fprintf(error, "Line %d: ", line);
            switch(error_type){
                case IllEGAL_CHARACTER:{
                    fprintf(error, "IllEGAL_CHARACTER\n");
                    break;
                }
                case IDENTIFIER_TOO_LONG:{
                    fprintf(error, "IDENTIFIER_TOO_LONG\n");
                    break;
                }
                case COLON_MISMATCH:{
                    fprintf(error, "COLON_MISMATCH\n");
                    break;
                }
            }
            break;
        }
    }

    fclose(pas); // 关闭文件
    fclose(dyd); // 关闭文件
    fclose(error); // 关闭文件

}