#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define Rtype 0
#define Itype 1
#define Jtype 2
struct instruction {
    int rs;
    int rt;
    int rd;
    int imm;
    char mnemonic[12];
    char hexValueInstruction[9];
    size_t intValueInstruction;
    size_t instructionType;
};
struct registers{
    char name[12];
    long value;
    int usedNumber;
};
void intToHex16(char *lower, int a);
void printInfo(int pc,struct registers *regs[16]);
int main() {
    int pc=0;
    int numberOfInstructions=0;
    int amountOfMemoryUsed=0;
    long memory[100];
    for (int i = 0; i < 100; ++i) {
        memory[i]=0;
    }
    char *instructions[] = {"add", "sub", "slt", "or", "nand", "addi", "slti", "ori", "lui", "lw", "sw", "beq", "jalr","j", "halt"};
    struct instruction *currentInstruction = (struct instruction *) malloc(sizeof(struct instruction));
    struct registers *R0 = (struct registers *) malloc(sizeof(struct registers));strcpy(R0->name,"R0");R0->value=0;R0->usedNumber=0;
    struct registers *R1 = (struct registers *) malloc(sizeof(struct registers));strcpy(R1->name,"R1");R1->value=0;R1->usedNumber=0;
    struct registers *R2 = (struct registers *) malloc(sizeof(struct registers));strcpy(R2->name,"R2");R2->value=0;R2->usedNumber=0;
    struct registers *R3 = (struct registers *) malloc(sizeof(struct registers));strcpy(R3->name,"R3");R3->value=0;R3->usedNumber=0;
    struct registers *R4 = (struct registers *) malloc(sizeof(struct registers));strcpy(R4->name,"R4");R4->value=0;R4->usedNumber=0;
    struct registers *R5 = (struct registers *) malloc(sizeof(struct registers));strcpy(R5->name,"R5");R5->value=0;R5->usedNumber=0;
    struct registers *R6 = (struct registers *) malloc(sizeof(struct registers));strcpy(R6->name,"R6");R6->value=0;R6->usedNumber=0;
    struct registers *R7 = (struct registers *) malloc(sizeof(struct registers));strcpy(R7->name,"R7");R7->value=0;R7->usedNumber=0;
    struct registers *R8 = (struct registers *) malloc(sizeof(struct registers));strcpy(R8->name,"R8");R8->value=0;R8->usedNumber=0;
    struct registers *R9 = (struct registers *) malloc(sizeof(struct registers));strcpy(R9->name,"R9");R9->value=0;R9->usedNumber=0;
    struct registers *R10 = (struct registers *) malloc(sizeof(struct registers));strcpy(R10->name,"R10");R10->value=0;R10->usedNumber=0;
    struct registers *R11 = (struct registers *) malloc(sizeof(struct registers));strcpy(R11->name,"R11");R11->value=0;R11->usedNumber=0;
    struct registers *R12 = (struct registers *) malloc(sizeof(struct registers));strcpy(R12->name,"R12");R12->value=0;R12->usedNumber=0;
    struct registers *R13 = (struct registers *) malloc(sizeof(struct registers));strcpy(R13->name,"R13");R13->value=0;R13->usedNumber=0;
    struct registers *R14 = (struct registers *) malloc(sizeof(struct registers));strcpy(R14->name,"R14");R14->value=0;R14->usedNumber=0;
    struct registers *R15 = (struct registers *) malloc(sizeof(struct registers));strcpy(R15->name,"R15");R15->value=0;R15->usedNumber=0;
    struct registers *regs[16]={R0,R1,R2,R3,R4,R5,R6,R7,R8,R9,R10,R11,R12,R13,R14,R15};
    printf("\033[1;35m");
    printf("(*) ");
    printInfo(pc,regs);
    printf("\n\033[0m");
    FILE *input;
    size_t lineSize=500;
    int count1=0;
    char fileInstructions[500][500];
    input = fopen("tests\\testcase0.mc", "r");
    char *word = malloc(sizeof(char) * lineSize);
    if(input==NULL)
    {
        printf("%s cannot be openned\n");
        exit(1);
    }
    while(fgets(word, lineSize, input) != NULL)
    {
        strcpy(fileInstructions[count1], word );
        count1++;
    }
    free(word);
    bool isHalt=false;
    for (int i = 0; i < count1; ++i) {
        long number=strtol(fileInstructions[i], NULL, 10);
        int opcode=(number&0b00001111000000000000000000000000)>>24;
        if(isHalt){
            memory[i]=number;
            amountOfMemoryUsed++;
        }
        if(opcode==14){//halt
            isHalt=true;
        }
    }
    int i=0;
    while (pc<count1){
        char lower[9];
        long number=strtol(fileInstructions[pc], NULL, 10);
        int opcode=(number&0b00001111000000000000000000000000)>>24;
        currentInstruction->intValueInstruction=number;
        strcpy(currentInstruction->mnemonic,instructions[opcode]);
        intToHex16(lower,number);
        strcpy(currentInstruction->hexValueInstruction,lower);
        if (opcode >= 0 && opcode <= 4){//R type
            currentInstruction->instructionType=Rtype;
            currentInstruction->rs=(number&0b00000000111100000000000000000000)>>20;
            currentInstruction->rt=(number&0b00000000000011110000000000000000)>>16;
            currentInstruction->rd=(number&0b00000000000000001111000000000000)>>12;
            if(opcode==0){
                //add
                regs[currentInstruction->rd]->value=regs[currentInstruction->rs]->value+regs[currentInstruction->rt]->value;
                regs[currentInstruction->rd]->usedNumber=1;
                regs[currentInstruction->rs]->usedNumber=1;
                regs[currentInstruction->rt]->usedNumber=1;
                pc+=1;
            } else if(opcode==1){
                //sub
                regs[currentInstruction->rd]->value=regs[currentInstruction->rs]->value-regs[currentInstruction->rt]->value;
                regs[currentInstruction->rd]->usedNumber=1;
                regs[currentInstruction->rs]->usedNumber=1;
                regs[currentInstruction->rt]->usedNumber=1;
                pc+=1;
            }else if(opcode==2){
                //slt
                if(regs[currentInstruction->rs]->value<regs[currentInstruction->rt]->value)regs[currentInstruction->rd]->value=1;
                else regs[currentInstruction->rd]->value=0;
                regs[currentInstruction->rd]->usedNumber=1;
                regs[currentInstruction->rs]->usedNumber=1;
                regs[currentInstruction->rt]->usedNumber=1;
                pc+=1;
                break;
            }else if(opcode==3){//or
                regs[currentInstruction->rd]->value=regs[currentInstruction->rs]->value|regs[currentInstruction->rt]->value;
                regs[currentInstruction->rd]->usedNumber=1;
                regs[currentInstruction->rs]->usedNumber=1;
                regs[currentInstruction->rt]->usedNumber=1;
                pc+=1;
            }else if(opcode==4){//nand
                int and=regs[currentInstruction->rs]->value&regs[currentInstruction->rt]->value;
                regs[currentInstruction->rd]->value=~and;
                regs[currentInstruction->rd]->usedNumber=1;
                regs[currentInstruction->rs]->usedNumber=1;
                regs[currentInstruction->rt]->usedNumber=1;
                pc+=1;
            }
        }else if (opcode >= 5 && opcode <= 12){//I type
            currentInstruction->instructionType=Itype;
            currentInstruction->rs=(number&0b00000000111100000000000000000000)>>20;
            currentInstruction->rt=(number&0b00000000000011110000000000000000)>>16;
            currentInstruction->imm=(number&0b00000000000000001111111111111111);
            int num = (currentInstruction->imm & 0b00000000000000001000000000000000)>>15;
            int signExtension;
            if(num==1){//sign extension
                signExtension=(currentInstruction->imm | 0b11111111111111110000000000000000);
            }else if(num==0){
                signExtension=currentInstruction->imm;
            }
            if(opcode==5){//addi
                currentInstruction->imm=signExtension;
                regs[currentInstruction->rt]->value=regs[currentInstruction->rs]->value+currentInstruction->imm;
                regs[currentInstruction->rs]->usedNumber=1;
                regs[currentInstruction->rt]->usedNumber=1;
                pc+=1;
            }else if(opcode==6){//slti
                currentInstruction->imm=signExtension;
                if(regs[currentInstruction->rs]->value<currentInstruction->imm)regs[currentInstruction->rt]->value=1;
                else regs[currentInstruction->rt]->value=0;
                regs[currentInstruction->rs]->usedNumber=1;
                regs[currentInstruction->rt]->usedNumber=1;
                pc+=1;
            }else if(opcode==7){//ori
                currentInstruction->imm=currentInstruction->imm & 0b00000000000000001111111111111111;//zero extension
                regs[currentInstruction->rt]->value=regs[currentInstruction->rs]->value | currentInstruction->imm;
                regs[currentInstruction->rs]->usedNumber=1;
                regs[currentInstruction->rt]->usedNumber=1;
                pc+=1;
            }else if(opcode==8){//lui
                regs[currentInstruction->rt]->value=(currentInstruction->imm<<16);
                regs[currentInstruction->rt]->usedNumber=1;
                pc+=1;
            } else if(opcode==9){//lw
                currentInstruction->imm=signExtension;
                regs[currentInstruction->rt]->value=memory[currentInstruction->imm+regs[currentInstruction->rs]->value];
                regs[currentInstruction->rs]->usedNumber=1;
                regs[currentInstruction->rt]->usedNumber=1;
                amountOfMemoryUsed++;
                pc+=1;
            } else if(opcode==10){//sw
                currentInstruction->imm=signExtension;
                memory[currentInstruction->imm+regs[currentInstruction->rs]->value]=regs[currentInstruction->rt]->value;
                regs[currentInstruction->rs]->usedNumber=1;
                regs[currentInstruction->rt]->usedNumber=1;
                amountOfMemoryUsed++;
                pc+=1;
            } else if(opcode==11){//beq
                currentInstruction->imm=signExtension;
                if(regs[currentInstruction->rs]->value==regs[currentInstruction->rt]->value) pc+=currentInstruction->imm+1;
                else pc+=1;
                regs[currentInstruction->rs]->usedNumber=1;
                regs[currentInstruction->rt]->usedNumber=1;
            } else if(opcode==12){//jalr
                regs[currentInstruction->rt]->value=pc+1;
                pc=regs[currentInstruction->rs]->value;
                regs[currentInstruction->rs]->usedNumber=1;
                regs[currentInstruction->rt]->usedNumber=1;
            }
        }else if (opcode == 13 || opcode == 14){//J type
            currentInstruction->instructionType=Jtype;
            currentInstruction->imm=(number&0b00000000000000001111111111111111);//zero extension
            if(opcode==13){//j
                pc=currentInstruction->imm;
            } else if(opcode==14){//halt
                pc+=1;
                break;
            }
        }
        printf("\033[1;36m");
        printf("( %-3d)  ",i);
        printInfo(pc,regs);
        printf("\033[0m");
        i++;
    }
    double percentOfUsedRegisters=0;
    int sumOfUsedRegisters=0;
    for(int k=0;k<16;k++){
        if(regs[k]->usedNumber==1){
            sumOfUsedRegisters++;
        }
    }
    numberOfInstructions=i;
    printf("\033[1;33m");
    percentOfUsedRegisters=((double)sumOfUsedRegisters/16)*100;
    printf("\nPercent Of Used Registers = %f\n",percentOfUsedRegisters);
    printf("Number Of Instructions = %d\n",numberOfInstructions);
    printf("Amount Of Memory Used = %d\n",amountOfMemoryUsed);
    printf("\033[0m");
    return 0;
}
void printInfo(int pc,struct registers *regs[16]){
    printf("PC = %-5d | R0 = %-5d | R1 = %-5d | R2 = %-5d | R3 = %-5d | R4 = %-5d | R5 = %-5d | R6 = %-5d | R7 = %-5d | R8 = %-5d | R9 = %-5d | R10 = %-5d | R11 = %-5d | R12 = %-5d | R13 = %-5d | R14 = %-5d | R15 = %-5d\n"
           ,pc,regs[0]->value,regs[1]->value,regs[2]->value,regs[3]->value,regs[4]->value,regs[5]->value,regs[6]->value,regs[7]->value,regs[8]->value,regs[9]->value,regs[10]->value,regs[11]->value,regs[12]->value,regs[13]->value,regs[14]->value,regs[15]->value);

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
