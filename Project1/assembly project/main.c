
//kasra samadi <993623030>

#include <stdio.h>
#include <mqoai.h>
#include<stdlib.h>
#include<string.h>
#define Rtype 0
#define Itype 1
#define Jtype 2
#define numberRtype 5
#define numberItype 8
#define numberJtype 2
struct SymbolTable {
    char *label;
    int value;
};
struct instruction {
    int rs;
    int rt;
    int rd;
    int imm;
    int PC;
    char mnemonic[12];
    char hexValueInstruction[9];
    size_t intValueInstruction;
    size_t instructionType;
};
int findSymbolTableLength(FILE *file);
int fillSymbolTable(struct SymbolTable *symbolTable, FILE *file);
void intToHex16(char *lower, int a);
int hex16ToInt(char *hex);
int findSymbol(int symbolTabLength, struct SymbolTable *symbolTable, char *token2);
void result(int op, struct instruction *currentInstruction, struct SymbolTable *symbolTable, char hex_table[16],char lower[5], char *token2, int symbolTabLength, int programCounter);
void duplicatedLabel(struct SymbolTable *symbolTable,int symbolTabLength);
int validOpCode(char *token,char *instructions[]);

int main(int argc,char**argv) {
    Boolean invalidOpCode=TRUE;
    FILE *assp, *machp, *fopen();
    struct SymbolTable *symbolTable;
    struct instruction *currentInstruction = (struct instruction *) malloc(sizeof(struct instruction));
    char *instructions[] = {"add", "sub", "slt", "or", "nand", "addi", "slti", "ori", "lui", "lw", "sw", "beq", "jalr","j", "halt"};
    char hex_table[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    char lower[5];
    int symbolTabLength;
    int no_inst;
    int inst_cnt = 0;
    if(argc < 3){
        printf("***** Please run this program as follows:\n");
        printf("***** %s assprog.as machprog.m\n",argv[0]);
        printf("***** where assprog.as is your assembly program\n");
        printf("***** and machprog.m will be your machine code.\n");
        exit(1);
    }
    if((assp=fopen(argv[1],"r")) == NULL){
        printf("%s cannot be openned\n",argv[1]);
        exit(1);
    }
    if((machp=fopen(argv[2],"w+")) == NULL){
        printf("%s cannot be openned\n",argv[2]);
        exit(1);
    }
    symbolTabLength = findSymbolTableLength(assp);
    symbolTable = (struct SymbolTable *) malloc(symbolTabLength * sizeof(struct SymbolTable));
    for (int i = 0; i < symbolTabLength; i++) {
        symbolTable[i].label = (char *) malloc(7);
    }
    no_inst = fillSymbolTable(symbolTable, assp);
    duplicatedLabel(symbolTable,symbolTabLength);
    char *line2 = (char *) malloc(72);
    char *token;
    int programCounter = 0;
    while (fgets(line2, 600, assp) != NULL) {
        programCounter++;
        if (line2[0] == ' ' || line2[0] == '\t') {
            token = strtok(line2, "\t,\n ");
            int op= validOpCode(token, instructions);
            if(op!=-1){
                result(op, currentInstruction, symbolTable, hex_table, lower, token, symbolTabLength, programCounter);
                printf("%d\n",currentInstruction->intValueInstruction);
                fprintf(machp,"%d\n",currentInstruction->intValueInstruction);
            }else {
                puts("ERROR! exit(1) : Invalid Opcode\n");
                exit(1);
            }
        }else{
            Boolean isInst = FALSE;
            token = strtok(line2, "\t ");
            token = strtok(NULL, "\t\n ");
            for (int op = 0; op < 15; op++) {
                if (strcmp(instructions[op], token) == 0) {
                    isInst = TRUE;
                    result(op, currentInstruction, symbolTable, hex_table, lower, token, symbolTabLength, programCounter);
                    printf("%d\n",currentInstruction->intValueInstruction);
                    fprintf(machp,"%d\n",currentInstruction->intValueInstruction);
                }
            }
            if (isInst == FALSE){
                if(strcmp(token,".fill")==0){
                    token = strtok(NULL, "\t\n ");
                    int found2 = 0;
                    int count = 0;
                    for (int i = 0; i < symbolTabLength; i++) {
                        if (strcmp(symbolTable[i].label, token) == 0) {
                            found2 = 1;
                            count = i;
                        }
                    }
                    if (found2) {
                        currentInstruction->imm = symbolTable[count].value;
                    } else if(isalpha(token[0])){
                        puts("ERROR! exit(1) : Undefined Label\n");
                        exit(1);
                    }else{
                        currentInstruction->imm = atoi(token);
                    }
                    currentInstruction->intValueInstruction=currentInstruction->imm;
                    printf("%d\n",currentInstruction->intValueInstruction);
                    fprintf(machp,"%d\n",currentInstruction->intValueInstruction);
                } else{
                    puts("ERROR! exit(1) : Invalid Opcode\n");
                    exit(1);
                }
            }
        }
    }
    return 0;
}
int findSymbolTableLength(FILE *file) {
    int count = 0;
    char *line = (char *) malloc(72);
    while (fgets(line, 600, file) != NULL) {
        if (line[0] != ' ' && line[0] != '\t')
            count++;
    }
    rewind(file);
    free(line);
    return count;
}
int fillSymbolTable(struct SymbolTable *symbolTable, FILE *file) {
    char *line = (char *) malloc(72);
    char *token;
    int i = 0;
    int lineNumber = 0;
    while (fgets(line, 600, file) != NULL){
        if (line[0] != ' ' && line[0] != '\t') {
            token = strtok(line, "\t");
            strcpy(symbolTable[i].label, token);
            symbolTable[i].value = lineNumber;
            i++;
        }
        lineNumber++;
    }
    rewind(file);
    free(line);
    return lineNumber;
}
void intToHex16(char *lower, int a) {
    sprintf(lower, "%X", a);
    if (a < 0x10) {
        lower[4] = '\0';
        lower[3] = lower[0];
        lower[2] = '0';
        lower[1] = '0';
        lower[0] = '0';
    } else if (a < 0x100) {
        lower[4] = '\0';
        lower[3] = lower[1];
        lower[2] = lower[0];
        lower[1] = '0';
        lower[0] = '0';
    } else if (a < 0x1000) {
        lower[4] = '\0';
        lower[3] = lower[2];
        lower[2] = lower[1];
        lower[1] = lower[0];
        lower[0] = '0';
    }
}
int hex16ToInt(char *hex) {
    int result = 0;
    while ((*hex) != '\0') {
        if (('0' <= (*hex)) && ((*hex) <= '9'))
            result = result * 16 + (*hex) - '0';
        else if (('A' <= (*hex)) && ((*hex) <= 'F'))
            result = result * 16 + (*hex) - 'A' + 10;
        else if (('a' <= (*hex)) && ((*hex) <= 'f'))
            result = result * 16 + (*hex) - 'a' + 10;
        hex++;
    }
    return (result);
}
int findSymbol(int symbolTabLength, struct SymbolTable *symbolTable, char *token2) {
    int count = -1;
    for (int i = 0; i < symbolTabLength; i++) {
        if (strcmp(symbolTable[i].label, token2) == 0) {
            count = i;
        }
    }
    return count;
}
void result(int op, struct instruction *currentInstruction, struct SymbolTable *symbolTable, char hex_table[16],char lower[5], char *token2, int symbolTabLength, int programCounter){
    if (op >= 0 && op <= 4) {//R format instruction
        currentInstruction->instructionType = Rtype;
        strcpy(currentInstruction->mnemonic, token2);
        token2 = strtok(NULL, ",\t\n ");
        currentInstruction->rd = atoi(token2);
        token2 = strtok(NULL, ",\t\n ");
        currentInstruction->rs = atoi(token2);
        token2 = strtok(NULL, ",\t\n ");
        currentInstruction->rt = atoi(token2);
        currentInstruction->PC = currentInstruction->PC + 1;
        currentInstruction->hexValueInstruction[0] = '0';
        currentInstruction->hexValueInstruction[1] = hex_table[op];
        currentInstruction->hexValueInstruction[2] = hex_table[currentInstruction->rs];
        currentInstruction->hexValueInstruction[3] = hex_table[currentInstruction->rt];
        currentInstruction->hexValueInstruction[4] = hex_table[currentInstruction->rd];
        currentInstruction->hexValueInstruction[5] = '0';
        currentInstruction->hexValueInstruction[6] = '0';
        currentInstruction->hexValueInstruction[7] = '0';
        currentInstruction->hexValueInstruction[8] = '\0';
        int res = hex16ToInt(currentInstruction->hexValueInstruction);
        currentInstruction->intValueInstruction=res;
    } else if (op >= 5 && op <= 12) {//I format instruction
        currentInstruction->instructionType = Itype;
        strcpy(currentInstruction->mnemonic, token2);
        if (strcmp(currentInstruction->mnemonic, "jalr") == 0) {
            token2 = strtok(NULL, ",\t\n ");
            currentInstruction->rt = atoi(token2);
            token2 = strtok(NULL, ",\t\n ");
            currentInstruction->rs = atoi(token2);
            currentInstruction->imm = 0;
        } else if (strcmp(currentInstruction->mnemonic, "lui") == 0) {
            token2 = strtok(NULL, ",\t\n ");
            currentInstruction->rt = atoi(token2);
            token2 = strtok(NULL, ",\t\n ");
            currentInstruction->imm = atoi(token2);
            currentInstruction->rs = 0;
        } else if (strcmp(currentInstruction->mnemonic, "beq") == 0) {
            token2 = strtok(NULL, ",\t\n ");
            currentInstruction->rs = atoi(token2);
            token2 = strtok(NULL, ",\t\n ");
            currentInstruction->rt = atoi(token2);
            token2 = strtok(NULL, ",\t\n ");
            int count = findSymbol(symbolTabLength, symbolTable, token2);
            if (count != -1) {
                currentInstruction->imm = symbolTable[count].value - (programCounter);
            } else if(isalpha((token2[0]))){
                puts("ERROR! exit(1) : Undefined Label\n");
                exit(1);
            }else{
                long num= atol(token2);
                if(num<65536 && num>-65536){
                   num =num - (programCounter);
                    if(num<0){
                        num+=65536;
                    }
                    currentInstruction->imm =num;
                }else {
                    puts("ERROR! exit(1) : invalid Offset\n");
                    exit(1);//invalid offset
                }
            }
        }else{
            token2 = strtok(NULL, ",\t\n ");
            currentInstruction->rt = atoi(token2);
            token2 = strtok(NULL, ",\t\n ");
            currentInstruction->rs = atoi(token2);
            token2 = strtok(NULL, ",\t\n ");
            int count = findSymbol(symbolTabLength, symbolTable, token2);
            if (count != -1) {
                currentInstruction->imm = symbolTable[count].value;
            } else if(isalpha((token2[0]))){
                puts("ERROR! exit(1) : Undefined Label\n");
                exit(1);
            }else{
                long num= atol(token2);
                if(num<65536 && num>-65536){
                    if(num<0 && op!=7){//op ori is 7
                        num+=65536;
                    }
                    currentInstruction->imm = num;
                }else {
                    puts("ERROR! exit(1) : invalid Offset\n");
                    exit(1);//invalid offset
                }
            }
        }
        currentInstruction->PC = currentInstruction->PC + 1;
        currentInstruction->hexValueInstruction[0] = '0';
        currentInstruction->hexValueInstruction[1] = hex_table[op];
        currentInstruction->hexValueInstruction[2] = hex_table[currentInstruction->rs];
        currentInstruction->hexValueInstruction[3] = hex_table[currentInstruction->rt];
        intToHex16(lower, currentInstruction->imm);
        currentInstruction->hexValueInstruction[4] = lower[0];
        currentInstruction->hexValueInstruction[5] = lower[1];
        currentInstruction->hexValueInstruction[6] = lower[2];
        currentInstruction->hexValueInstruction[7] = lower[3];
        currentInstruction->hexValueInstruction[8] = '\0';
        int res = hex16ToInt(currentInstruction->hexValueInstruction);
        currentInstruction->intValueInstruction=res;
    } else if (op == 13 || op == 14) {//J format
        currentInstruction->instructionType = Jtype;
        strcpy(currentInstruction->mnemonic, token2);
        if (strcmp(currentInstruction->mnemonic, "halt") == 0) {
            currentInstruction->imm = 0;
        } else {//j
            token2 = strtok(NULL, ",\t\n ");
            int count = findSymbol(symbolTabLength, symbolTable, token2);
            if (count != -1) {
                currentInstruction->imm = symbolTable[count].value;
            } else if(isalpha((token2[0]))){
                puts("ERROR! exit(1) : Undefined Label\n");
                exit(1);
            }else {
                long num= atol(token2);
                if(num<65536 && num>-65536){
                    currentInstruction->imm = num;
                }else {
                    puts("ERROR! exit(1) : invalid Offset\n");
                    exit(1);//invalid offset
                }
            }
        }
        currentInstruction->PC = currentInstruction->PC + 1;
        currentInstruction->hexValueInstruction[0] = '0';
        currentInstruction->hexValueInstruction[1] = hex_table[op];
        currentInstruction->hexValueInstruction[2] = '0';
        currentInstruction->hexValueInstruction[3] = '0';
        intToHex16(lower, currentInstruction->imm);
        currentInstruction->hexValueInstruction[4] = lower[0];
        currentInstruction->hexValueInstruction[5] = lower[1];
        currentInstruction->hexValueInstruction[6] = lower[2];
        currentInstruction->hexValueInstruction[7] = lower[3];
        currentInstruction->hexValueInstruction[8] = '\0';
        int res = hex16ToInt(currentInstruction->hexValueInstruction);
        currentInstruction->intValueInstruction=res;
    }

}
void duplicatedLabel(struct SymbolTable *symbolTable,int symbolTabLength){
    Boolean duplicated=FALSE;
    for (int i = 0; i < symbolTabLength; ++i) {
        for (int j = i+1; j < symbolTabLength; ++j) {
            if(strcmp(symbolTable[i].label,symbolTable[j].label)==0){
                duplicated=TRUE;
                break;
            }
        }
    }
    if(duplicated){
        puts("ERROR! exit(1) : Duplicated Label\n");
        exit(1);
    }
}
int validOpCode(char *token2,char *instructions[]){
    int opcode=-1;
    for (int op = 0; op < 15; op++) {
        if (strcmp(instructions[op], token2) == 0){
            opcode=op;
            break;
        }
    }
    return opcode;
}