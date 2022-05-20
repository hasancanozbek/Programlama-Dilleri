#include <stdio.h>
#include <string.h>
#include <stdbool.h>

char readFile[1024];
int currentIndex;
unsigned short AX = 0, BX = 0, CX = 0, DX = 0;

int isRegister() { //Register turunu tespit eden fonksiyon
    if (readFile[currentIndex] == 'A') {
        return 1;
    } else if (readFile[currentIndex] == 'B') {
        return 2;
    } else if (readFile[currentIndex] == 'C') {
        return 3;
    } else if (readFile[currentIndex] == 'D') {
        return 4;
    }
    return 0;
}

bool isNumber() { //Dosyadaki karakter kumesinin sayi olup olmadigini kontrol eden fonksiyon
    if (readFile[currentIndex] == '0' || readFile[currentIndex] == '1' ||
        readFile[currentIndex] == '2' ||
        readFile[currentIndex] == '3' || readFile[currentIndex] == '4' ||
        readFile[currentIndex] == '5' ||
        readFile[currentIndex] == '6' || readFile[currentIndex] == '7' ||
        readFile[currentIndex] == '8' ||
        readFile[currentIndex] == '9') {
        return true;
    }
    return false;
}

unsigned short covertNumber() { //Dosyadaki karakter kumesi seklinde duran sayilari short tipine ceviren fonksiyon
    unsigned short sum = 0;
    int counter = 0;
    unsigned short arr[8];
    while (readFile[currentIndex + 1] == '0' || readFile[currentIndex + 1] == '1' ||
           readFile[currentIndex + 1] == '2' || readFile[currentIndex + 1] == '3' ||
           readFile[currentIndex + 1] == '4' || readFile[currentIndex + 1] == '5' ||
           readFile[currentIndex + 1] == '6' || readFile[currentIndex + 1] == '7' ||
           readFile[currentIndex + 1] == '8' || readFile[currentIndex + 1] == '9') {
        unsigned short number = readFile[currentIndex] - '0';
        arr[counter] = number;
        counter++;
        currentIndex++;
    }
    arr[counter] = readFile[currentIndex] - '0';
    currentIndex++;
    counter++;
    int digit = 1;
    int i;
    for (i = counter - 1; i >= 0; i--) {
        sum = sum + (arr[i] * digit);
        digit = digit * 10;
    }
    return sum;
}

int main() {

    unsigned short memory[2048]; //Makinemizin sanal bellek birimi 2048kb olarak alinmistir.

    char character;
    char file_path[256];
    FILE *file;
    printf("Dosya yolunu giriniz (\" kullanmadan): ");
    scanf("%s", &file_path);
    //Dosyadan verileri cekme islemi
    if ((file = fopen(file_path, "r")) != NULL) {
        int k;
        k = 0;
        character = fgetc(file);
        readFile[k] = character;
        k++;
        while (character != EOF) {
            character = fgetc(file);
            readFile[k] = character;
            k++;
        }
        readFile[k] = '\0';

        //Assembler Algoritması
        currentIndex = 0; //isleme dosyanin 0. indexinden baslar
        while (currentIndex < strlen(readFile)) { //index dosyadaki karakter sayisina esit olanan kadar devam eder
            if (readFile[currentIndex] == 'H' && readFile[currentIndex + 1] == 'R' &&
                readFile[currentIndex + 2] == 'K') {
                currentIndex += 4; //Kontrol islemi 3 karakter saydi, bosluk da bir karakter olarak alindi ve 4 index atlandi
                if (isRegister() != 0) { //Gelen kelime bir register mi?
                    int register1 = isRegister(); //Register turunu belirle
                    currentIndex += 3;
                    if (isNumber()) { //Gelen kelime bir sayi mi?
                        if (register1 == 1) {
                            AX = covertNumber();
                            printf("\nAX <-- %hu", AX);
                        }
                        if (register1 == 2) {
                            BX = covertNumber();
                            printf("\nBX <-- %hu", BX);
                        }
                        if (register1 == 3) {
                            CX = covertNumber();
                            printf("\nCX <-- %hu", CX);
                        }
                        if (register1 == 4) {
                            DX = covertNumber();
                            printf("\nDX <-- %hu", DX);
                        }
                    } else if (readFile[currentIndex] == '[') {// [ parantezi gelirse bellekten okuma yap
                        currentIndex++;
                        if (register1 == 1) {
                            AX = memory[covertNumber()];
                            printf("\nAX <-- %hu", AX);
                        }
                        if (register1 == 2) {
                            BX = memory[covertNumber()];
                            printf("\nBX <-- %hu", BX);
                        }
                        if (register1 == 3) {
                            CX = memory[covertNumber()];
                            printf("\nCX <-- %hu", CX);
                        }
                        if (register1 == 4) {
                            DX = memory[covertNumber()];
                            printf("\nDX <-- %hu", DX);
                        }
                        currentIndex++; //En sondaki kapatma parantezini atlamak icin indexi arttir
                    } else if (isRegister() != 0) { //Sonraki kelime bir register mi?
                        int register2 = isRegister();//Registerin turunu bul
                        currentIndex += 2;
                        if (register1 == 1) {
                            if (register2 == 1) {
                                AX = AX;
                            }
                            if (register2 == 2) {
                                AX = BX;
                            }
                            if (register2 == 3) {
                                AX = CX;
                            }
                            if (register2 == 4) {
                                AX = DX;
                            }
                            printf("\nAX <-- %hu", AX);
                        }
                        if (register1 == 2) {
                            if (register2 == 1) {
                                BX = AX;
                            }
                            if (register2 == 2) {
                                BX = BX;
                            }
                            if (register2 == 3) {
                                BX = CX;
                            }
                            if (register2 == 4) {
                                BX = DX;
                            }
                            printf("\nBX <-- %hu", BX);
                        }
                        if (register1 == 3) {
                            if (register2 == 1) {
                                CX = AX;
                            }
                            if (register2 == 2) {
                                CX = BX;
                            }
                            if (register2 == 3) {
                                CX = CX;
                            }
                            if (register2 == 4) {
                                CX = DX;
                            }
                            printf("\nCX <-- %hu", CX);
                        }
                        if (register1 == 4) {
                            if (register2 == 1) {
                                DX = AX;
                            }
                            if (register2 == 2) {
                                DX = BX;
                            }
                            if (register2 == 3) {
                                DX = CX;
                            }
                            if (register2 == 4) {
                                DX = DX;
                            }
                            printf("\nDX <-- %hu", DX);
                        }
                    }
                }
                if (readFile[currentIndex] == '[') {
                    currentIndex++;
                    unsigned short address = covertNumber(); //Parantez icindeki adresi degiskene ata
                    currentIndex += 2;
                    if (isRegister() != 0) { //Sonraki kelime bir register mi?
                        int registerId = isRegister(); //Registerin turunu belirle
                        currentIndex += 2;
                        if (registerId == 1) {
                            memory[address] = AX;
                            printf("\nMemory[%hu] <-- %hu", address, AX);
                        }
                        if (registerId == 2) {
                            memory[address] = BX;
                            printf("\nMemory[%hu] <-- %hu", address, BX);
                        }
                        if (registerId == 3) {
                            memory[address] = CX;
                            printf("\nMemory[%hu] <-- %hu", address, CX);
                        }
                        if (registerId == 4) {
                            memory[address] = DX;
                            printf("\nMemory[%hu] <-- %hu", address, DX);
                        }
                    }
                    if (isNumber()) {//Sonraki kelime bir sayi mi iceriyor?
                        unsigned short data = covertNumber();//Sayiyi bir degiskende sakla
                        memory[address] = data;
                        printf("\nMemory[%hu] <-- %hu", address, data);
                    }
                    if (readFile[currentIndex] == '[') {
                        currentIndex++;
                        unsigned short address2 = covertNumber();
                        memory[address] = memory[address2]; //1. parametredeki bellek hucresine 2. parametreki bellek hucresini yapistir
                        printf("\nMemory[%hu] <-- %hu", address, memory[address2]);
                        currentIndex++; //Sondaki ] parantezini atla
                    }
                }
            } else if (readFile[currentIndex] == 'D' && readFile[currentIndex + 1] == 'E' &&
                       readFile[currentIndex + 2] == 'G') {
                currentIndex += 4;
                if (isRegister() != 0) {
                    int registerId = isRegister();
                    currentIndex += 2;
                    if (registerId == 1) {
                        unsigned short complement = ~AX;//1. tumleyen al
                        unsigned short result = 15 & complement;//Short 16 bit oldugu icin 1. tumleyeni hatali sonuc verecektir.
                        //Bu yuzden en onemli 8 bitini 0 ile maskeliyorum. h'15' = 00001111
                        printf("\n%hu' -->  %hu", AX, result);
                    }
                    if (registerId == 2) {
                        unsigned short complement = ~BX;
                        unsigned short result = 15 & complement;
                        printf("\n%hu' -->  %hu", BX, result);
                    }
                    if (registerId == 3) {
                        unsigned short complement = ~CX;
                        unsigned short result = 15 & complement;
                        printf("\n%hu' -->  %hu", CX, result);
                    }
                    if (registerId == 4) {
                        unsigned short complement = ~DX;
                        unsigned short result = 15 & complement;
                        printf("\n%hu' -->  %hu", DX, result);
                    }
                }
                if (readFile[currentIndex] == '[') {
                    currentIndex++;
                    unsigned short address = covertNumber();
                    unsigned short complement = ~memory[address];
                    unsigned short result = 15 & complement;
                    printf("\n%hu' -->  %hu", memory[address], result);
                    currentIndex++;
                }
            } else if (readFile[currentIndex] == 'V' && readFile[currentIndex + 1] == 'E' &&
                       readFile[currentIndex + 2] != 'Y') { //VEYA komutundaki 'VE' harflerinden dolayi karismamasi icin sonraki harfi de kontrol et.
                currentIndex += 3;
                if(isRegister() != 0){
                    int register1 = isRegister();
                    currentIndex += 3;
                    if(isRegister() != 0){
                        int register2 = isRegister();
                        currentIndex += 2;
                        if (register1 == 1) {
                            if (register2 == 1) {
                                AX = AX;
                                printf("\nAX <-- %hu", AX);
                            }
                            if (register2 == 2) {
                                AX = AX & BX;
                                printf("\nAX <-- %hu", AX);
                            }
                            if (register2 == 3) {
                                AX = AX & CX;
                                printf("\nAX <-- %hu", AX);
                            }
                            if (register2 == 4) {
                                AX = AX & DX;
                                printf("\nAX <-- %hu", AX);
                            }
                        }
                        if (register1 == 2) {
                            if (register2 == 1) {
                                BX = BX & AX;
                                printf("\nBX <-- %hu", BX);
                            }
                            if (register2 == 2) {
                                BX = BX;
                                printf("\nBX <-- %hu", BX);
                            }
                            if (register2 == 3) {
                                BX = BX & CX;
                                printf("\nBX <-- %hu", BX);
                            }
                            if (register2 == 4) {
                                BX = BX & DX;
                                printf("\nBX <-- %hu", BX);
                            }
                        }
                        if (register1 == 3) {
                            if (register2 == 1) {
                                CX = CX & AX;
                                printf("\nCX <-- %hu", CX);
                            }
                            if (register2 == 2) {
                                CX = CX & BX;
                                printf("\nCX <-- %hu", CX);
                            }
                            if (register2 == 3) {
                                CX = CX;
                                printf("\nCX <-- %hu", CX);
                            }
                            if (register2 == 4) {
                                CX = CX & DX;
                                printf("\nCX <-- %hu", CX);
                            }
                        }
                        if (register1 == 4) {
                            if (register2 == 1) {
                                DX = DX & AX;
                                printf("\nDX <-- %hu", DX);
                            }
                            if (register2 == 2) {
                                DX = DX & BX;
                                printf("\nDX <-- %hu", DX);
                            }
                            if (register2 == 3) {
                                DX = DX & CX;
                                printf("\nDX <-- %hu", DX);
                            }
                            if (register2 == 4) {
                                DX = DX;
                                printf("\nDX <-- %hu", DX);
                            }
                        }
                    }
                    if(isNumber()){
                        unsigned short data = covertNumber();
                        if(register1 == 1){
                            AX = AX & data;
                            printf("\nAX <-- %hu",AX);
                        }
                        if(register1 == 2){
                            BX = BX & data;
                            printf("\nBX <-- %hu",BX);
                        }
                        if(register1 == 3){
                            CX = CX & data;
                            printf("\nCX <-- %hu",CX);
                        }
                        if(register1 == 4){
                            DX = DX & data;
                            printf("\nDX <-- %hu",DX);
                        }
                    }
                    if(readFile[currentIndex] == '['){
                        currentIndex++;
                        unsigned short address = covertNumber();
                        if(register1 == 1){
                            AX = AX & memory[address];
                            printf("\nAX <-- %hu",AX);
                        }
                        if(register1 == 2){
                            BX = BX & memory[address];
                            printf("\nBX <-- %hu",BX);
                        }
                        if(register1 == 3){
                            CX = CX & memory[address];
                            printf("\nCX <-- %hu",CX);
                        }
                        if(register1 == 4){
                            DX = DX & memory[address];
                            printf("\nDX <-- %hu",DX);
                        }
                        currentIndex++;
                    }
                }
                if(readFile[currentIndex] == '['){
                    currentIndex++;
                    unsigned short address = covertNumber();
                    currentIndex+=2;
                    if(isRegister() != 0){
                        int registerId = isRegister();
                        currentIndex+=2;
                        if(registerId == 1){
                            memory[address] = memory[address] & AX;
                            printf("\nMemory[%hu] <-- %hu",address,memory[address]);
                        }
                        if(registerId == 2){
                            memory[address] = memory[address] & BX;
                            printf("\nMemory[%hu] <-- %hu",address,memory[address]);
                        }
                        if(registerId == 3){
                            memory[address] = memory[address] & CX;
                            printf("\nMemory[%hu] <-- %hu",address,memory[address]);
                        }
                        if(registerId == 4){
                            memory[address] = memory[address] & DX;
                            printf("\nMemory[%hu] <-- %hu",address,memory[address]);
                        }
                    }
                    if(isNumber()){
                        unsigned short data = covertNumber();
                        memory[address] = memory[address] & data;
                        printf("\nMemory[%hu] <-- %hu",address,memory[address]);
                    }
                    if(readFile[currentIndex] == '['){
                        currentIndex++;
                        unsigned short address2 = covertNumber();
                        memory[address] = memory[address] & memory[address2];
                        printf("\nMemory[%hu] <-- %hu",address,memory[address]);
                        currentIndex++;
                    }
                }
            }
            else if (readFile[currentIndex] == 'V' && readFile[currentIndex + 1] == 'E' &&
                       readFile[currentIndex + 2] == 'Y' && readFile[currentIndex + 3] == 'A') {
                currentIndex += 5;
                if(isRegister() != 0){
                    int register1 = isRegister();
                    currentIndex += 3;
                    if(isRegister() != 0){
                        int register2 = isRegister();
                        currentIndex += 2;
                        if (register1 == 1) {
                            if (register2 == 1) {
                                AX = AX;
                                printf("\nAX <-- %hu", AX);
                            }
                            if (register2 == 2) {
                                AX = AX | BX;
                                printf("\nAX <-- %hu", AX);
                            }
                            if (register2 == 3) {
                                AX = AX | CX;
                                printf("\nAX <-- %hu", AX);
                            }
                            if (register2 == 4) {
                                AX = AX | DX;
                                printf("\nAX <-- %hu", AX);
                            }
                        }
                        if (register1 == 2) {
                            if (register2 == 1) {
                                BX = BX | AX;
                                printf("\nBX <-- %hu", BX);
                            }
                            if (register2 == 2) {
                                BX = BX;
                                printf("\nBX <-- %hu", BX);
                            }
                            if (register2 == 3) {
                                BX = BX | CX;
                                printf("\nBX <-- %hu", BX);
                            }
                            if (register2 == 4) {
                                BX = BX | DX;
                                printf("\nBX <-- %hu", BX);
                            }
                        }
                        if (register1 == 3) {
                            if (register2 == 1) {
                                CX = CX | AX;
                                printf("\nCX <-- %hu", CX);
                            }
                            if (register2 == 2) {
                                CX = CX | BX;
                                printf("\nCX <-- %hu", CX);
                            }
                            if (register2 == 3) {
                                CX = CX;
                                printf("\nCX <-- %hu", CX);
                            }
                            if (register2 == 4) {
                                CX = CX | DX;
                                printf("\nCX <-- %hu", CX);
                            }
                        }
                        if (register1 == 4) {
                            if (register2 == 1) {
                                DX = DX | AX;
                                printf("\nDX <-- %hu", DX);
                            }
                            if (register2 == 2) {
                                DX = DX | BX;
                                printf("\nDX <-- %hu", DX);
                            }
                            if (register2 == 3) {
                                DX = DX | CX;
                                printf("\nDX <-- %hu", DX);
                            }
                            if (register2 == 4) {
                                DX = DX;
                                printf("\nDX <-- %hu", DX);
                            }
                        }
                    }
                    if(isNumber()){
                        unsigned short data = covertNumber();
                        if(register1 == 1){
                            AX = AX | data;
                            printf("\nAX <-- %hu",AX);
                        }
                        if(register1 == 2){
                            BX = BX | data;
                            printf("\nBX <-- %hu",BX);
                        }
                        if(register1 == 3){
                            CX = CX | data;
                            printf("\nCX <-- %hu",CX);
                        }
                        if(register1 == 4){
                            DX = DX | data;
                            printf("\nDX <-- %hu",DX);
                        }
                    }
                    if(readFile[currentIndex] == '['){
                        currentIndex++;
                        unsigned short address = covertNumber();
                        if(register1 == 1){
                            AX = AX | memory[address];
                            printf("\nAX <-- %hu",AX);
                        }
                        if(register1 == 2){
                            BX = BX | memory[address];
                            printf("\nBX <-- %hu",BX);
                        }
                        if(register1 == 3){
                            CX = CX | memory[address];
                            printf("\nCX <-- %hu",CX);
                        }
                        if(register1 == 4){
                            DX = DX | memory[address];
                            printf("\nDX <-- %hu",DX);
                        }
                        currentIndex++;
                    }
                }
                if(readFile[currentIndex] == '['){
                    currentIndex++;
                    unsigned short address = covertNumber();
                    currentIndex+=2;
                    if(isRegister() != 0){
                        int registerId = isRegister();
                        currentIndex+=2;
                        if(registerId == 1){
                            memory[address] = memory[address] | AX;
                            printf("\nMemory[%hu] <-- %hu",address,memory[address]);
                        }
                        if(registerId == 2){
                            memory[address] = memory[address] | BX;
                            printf("\nMemory[%hu] <-- %hu",address,memory[address]);
                        }
                        if(registerId == 3){
                            memory[address] = memory[address] | CX;
                            printf("\nMemory[%hu] <-- %hu",address,memory[address]);
                        }
                        if(registerId == 4){
                            memory[address] = memory[address] | DX;
                            printf("\nMemory[%hu] <-- %hu",address,memory[address]);
                        }
                    }
                    if(isNumber()){
                        unsigned short data = covertNumber();
                        memory[address] = memory[address] | data;
                        printf("\nMemory[%hu] <-- %hu",address,memory[address]);
                    }
                    if(readFile[currentIndex] == '['){
                        currentIndex++;
                        unsigned short address2 = covertNumber();
                        memory[address] = memory[address] | memory[address2];
                        printf("\nMemory[%hu] <-- %hu",address,memory[address]);
                        currentIndex++;
                    }
                }
            }
            else if (readFile[currentIndex] == 'T' && readFile[currentIndex + 1] == 'O' &&
                       readFile[currentIndex + 2] == 'P') {
                currentIndex += 4;
                if (isRegister() != 0) {
                    int register1 = isRegister();
                    currentIndex += 3;
                    if (isRegister() != 0) {
                        int register2 = isRegister();
                        currentIndex += 2;
                        if (register1 == 1) {
                            if (register2 == 1) {
                                AX = AX + AX;
                                printf("\nAX <-- %hu", AX);
                            }
                            if (register2 == 2) {
                                AX = AX + BX;
                                printf("\nAX <-- %hu", AX);
                            }
                            if (register2 == 3) {
                                AX = AX + CX;
                                printf("\nAX <-- %hu", AX);
                            }
                            if (register2 == 4) {
                                AX = AX + DX;
                                printf("\nAX <-- %hu", AX);
                            }
                        }
                        if (register1 == 2) {
                            if (register2 == 1) {
                                BX = BX + AX;
                                printf("\nCX <-- %hu", BX);
                            }
                            if (register2 == 2) {
                                BX = BX + BX;
                                printf("\nBX <-- %hu", BX);
                            }
                            if (register2 == 3) {
                                BX = BX + CX;
                                printf("\nBX <-- %hu", BX);
                            }
                            if (register2 == 4) {
                                BX = BX + DX;
                                printf("\nBX <-- %hu", BX);
                            }
                        }
                        if (register1 == 3) {
                            if (register2 == 1) {
                                CX = CX + AX;
                                printf("\nCX <-- %hu", CX);
                            }
                            if (register2 == 2) {
                                CX = CX + BX;
                                printf("\nCX <-- %hu", CX);
                            }
                            if (register2 == 3) {
                                CX = CX + CX;
                                printf("\nCX <-- %hu", CX);
                            }
                            if (register2 == 4) {
                                CX = CX + DX;
                                printf("\nCX <-- %hu", CX);
                            }
                        }
                        if (register1 == 4) {
                            if (register2 == 1) {
                                DX = DX + AX;
                                printf("\nDX <-- %hu", DX);
                            }
                            if (register2 == 2) {
                                DX = DX + BX;
                                printf("\nDX <-- %hu", DX);
                            }
                            if (register2 == 3) {
                                DX = DX + CX;
                                printf("\nDX <-- %hu", DX);
                            }
                            if (register2 == 4) {
                                DX = DX + DX;
                                printf("\nDX <-- %hu", DX);
                            }
                        }
                    } else if (readFile[currentIndex] == '[') {
                        currentIndex++;
                        if (register1 == 1) {
                            AX = AX + memory[covertNumber()];
                            printf("\nAX <-- %hu", AX);
                        }
                        if (register1 == 2) {
                            BX = BX + memory[covertNumber()];
                            printf("\nBX <-- %hu", BX);
                        }
                        if (register1 == 3) {
                            CX = CX + memory[covertNumber()];
                            printf("\nCX <-- %hu", CX);
                        }
                        if (register1 == 4) {
                            DX = DX + memory[covertNumber()];
                            printf("\nDX <-- %hu", DX);
                        }
                        currentIndex++;
                    }
                    if (isNumber()) {
                        if (register1 == 1) {
                            AX = AX + covertNumber();
                            printf("\nAX <-- %hu", AX);
                        }
                        if (register1 == 2) {
                            BX = BX + covertNumber();
                            printf("\nBX <-- %hu", BX);
                        }
                        if (register1 == 3) {
                            CX = CX + covertNumber();
                            printf("\nCX <-- %hu", CX);
                        }
                        if (register1 == 4) {
                            DX = DX + covertNumber();
                            printf("\nDX <-- %hu", DX);
                        }
                    }
                }
                if (readFile[currentIndex] == '[') {
                    currentIndex++;
                    unsigned short address = covertNumber();
                    currentIndex += 2;
                    if (isRegister() != 0) {
                        int register1 = isRegister();
                        currentIndex += 2;
                        if (register1 == 1) {
                            memory[address] = memory[address] + AX;
                            printf("\nMemory[%hu] <-- %hu", address, memory[address]);
                        }
                        if (register1 == 2) {
                            memory[address] = memory[address] + BX;
                            printf("\nMemory[%hu] <-- %hu", address, memory[address]);
                        }
                        if (register1 == 3) {
                            memory[address] = memory[address] + CX;
                            printf("\nMemory[%hu] <-- %hu", address, memory[address]);
                        }
                        if (register1 == 4) {
                            memory[address] = memory[address] + DX;
                            printf("\nMemory[%hu] <-- %hu", address, memory[address]);
                        }
                    }
                    if (isNumber()) {
                        unsigned short number = covertNumber();
                        memory[address] = number + memory[address];
                        printf("\nMemory[%hu] <-- %hu", address, memory[address]);
                    }
                    if (readFile[currentIndex] == '[') {
                        currentIndex++;
                        unsigned short address2 = covertNumber();
                        memory[address] = memory[address2] + memory[address];
                        printf("\nMemory[%hu] <-- %hu", address, memory[address]);
                        currentIndex++;
                    }
                }
            } else if (readFile[currentIndex] == 'C' && readFile[currentIndex + 1] == 'I' &&
                       readFile[currentIndex + 2] == 'K') {
                currentIndex += 4;
                if (isRegister() != 0) {
                    int register1 = isRegister();
                    currentIndex += 3;
                    if (isRegister() != 0) {
                        int register2 = isRegister();
                        currentIndex += 2;
                        if (register1 == 1) {
                            if (register2 == 1) {
                                AX = AX - AX;
                                printf("\nAX <-- %hu", AX);
                            }
                            if (register2 == 2) {
                                AX = AX - BX;
                                printf("\nAX <-- %hu", AX);
                            }
                            if (register2 == 3) {
                                AX = AX - CX;
                                printf("\nAX <-- %hu", AX);
                            }
                            if (register2 == 4) {
                                AX = AX - DX;
                                printf("\nAX <-- %hu", AX);
                            }
                        }
                        if (register1 == 2) {
                            if (register2 == 1) {
                                BX = BX - AX;
                                printf("\nBX <-- %hu", BX);
                            }
                            if (register2 == 2) {
                                BX = BX - BX;
                                printf("\nBX <-- %hu", BX);
                            }
                            if (register2 == 3) {
                                BX = BX - CX;
                                printf("\nBX <-- %hu", BX);
                            }
                            if (register2 == 4) {
                                BX = BX - DX;
                                printf("\nBX <-- %hu", BX);
                            }
                        }
                        if (register1 == 3) {
                            if (register2 == 1) {
                                CX = CX - AX;
                                printf("\nCX <-- %hu", CX);
                            }
                            if (register2 == 2) {
                                CX = CX - BX;
                                printf("\nCX <-- %hu", CX);
                            }
                            if (register2 == 3) {
                                CX = CX - CX;
                                printf("\nCX <-- %hu", CX);
                            }
                            if (register2 == 4) {
                                CX = CX - DX;
                                printf("\nCX <-- %hu", CX);
                            }
                        }
                        if (register1 == 4) {
                            if (register2 == 1) {
                                DX = DX - AX;
                                printf("\nDX <-- %hu", DX);
                            }
                            if (register2 == 2) {
                                DX = DX - BX;
                                printf("\nDX <-- %hu", DX);
                            }
                            if (register2 == 3) {
                                DX = DX - CX;
                                printf("\nDX <-- %hu", DX);
                            }
                            if (register2 == 4) {
                                DX = DX - DX;
                                printf("\nDX <-- %hu", DX);
                            }
                        }
                    } else if (readFile[currentIndex] == '[') {
                        currentIndex++;
                        if (register1 == 1) {
                            AX = AX - memory[covertNumber()];
                            printf("\nAX <-- %hu", AX);
                        }
                        if (register1 == 2) {
                            BX = BX - memory[covertNumber()];
                            printf("\nBX <-- %hu", BX);
                        }
                        if (register1 == 3) {
                            CX = CX - memory[covertNumber()];
                            printf("\nCX <-- %hu", CX);
                        }
                        if (register1 == 4) {
                            DX = DX - memory[covertNumber()];
                            printf("\nDX <-- %hu", DX);
                        }
                        currentIndex++;
                    }
                    if (isNumber()) {
                        if (register1 == 1) {
                            AX = AX - covertNumber();
                            printf("\nAX <-- %hu", AX);
                        }
                        if (register1 == 2) {
                            BX = BX - covertNumber();
                            printf("\nBX <-- %hu", BX);
                        }
                        if (register1 == 3) {
                            CX = CX - covertNumber();
                            printf("\nCX <-- %hu", CX);
                        }
                        if (register1 == 4) {
                            DX = DX - covertNumber();
                            printf("\nDX <-- %hu", DX);
                        }
                    }
                }
                if (readFile[currentIndex] == '[') {
                    currentIndex++;
                    unsigned short address = covertNumber();
                    currentIndex += 2;
                    if (isRegister() != 0) {
                        int register1 = isRegister();
                        currentIndex += 2;
                        if (register1 == 1) {
                            memory[address] = memory[address] - AX;
                            printf("\nMemory[%hu] <-- %hu", address, memory[address]);
                        }
                        if (register1 == 2) {
                            memory[address] = memory[address] - BX;
                            printf("\nMemory[%hu] <-- %hu", address, memory[address]);
                        }
                        if (register1 == 3) {
                            memory[address] = memory[address] - CX;
                            printf("\nMemory[%hu] <-- %hu", address, memory[address]);
                        }
                        if (register1 == 4) {
                            memory[address] = memory[address] - DX;
                            printf("\nMemory[%hu] <-- %hu", address, memory[address]);
                        }
                    }
                    if (isNumber()) {
                        unsigned short number = covertNumber();
                        memory[address] = memory[address] - number;
                        printf("\nMemory[%hu] <-- %hu", address, memory[address]);
                    }
                    if (readFile[currentIndex] == '[') {
                        currentIndex++;
                        unsigned short address2 = covertNumber();
                        memory[address] = memory[address] - memory[address2];
                        printf("\nMemory[%hu] <-- %hu", address, memory[address]);
                        currentIndex++;
                    }
                }
            } else if (readFile[currentIndex] == 'C' && readFile[currentIndex + 1] == 'R' &&
                       readFile[currentIndex + 2] == 'P') {
                currentIndex += 4;
                if (isRegister() != 0) {
                    int register1 = isRegister();
                    currentIndex += 3;
                    if (isRegister() != 0) {
                        int register2 = isRegister();
                        currentIndex += 2;
                        if (register1 == 1) {
                            if (register2 == 1) {
                                AX = AX * AX;
                                printf("\nAX <-- %hu", AX);
                            }
                            if (register2 == 2) {
                                AX = AX * BX;
                                printf("\nAX <-- %hu", AX);
                            }
                            if (register2 == 3) {
                                AX = AX * CX;
                                printf("\nAX <-- %hu", AX);
                            }
                            if (register2 == 4) {
                                AX = AX * DX;
                                printf("\nAX <-- %hu", AX);
                            }
                        }
                        if (register1 == 2) {
                            if (register2 == 1) {
                                BX = BX * AX;
                                printf("\nBX <-- %hu", BX);
                            }
                            if (register2 == 2) {
                                BX = BX * BX;
                                printf("\nBX <-- %hu", BX);
                            }
                            if (register2 == 3) {
                                BX = BX * CX;
                                printf("\nBX <-- %hu", BX);
                            }
                            if (register2 == 4) {
                                BX = BX * DX;
                                printf("\nBX <-- %hu", BX);
                            }
                        }
                        if (register1 == 3) {
                            if (register2 == 1) {
                                CX = CX * AX;
                                printf("\nCX <-- %hu", CX);
                            }
                            if (register2 == 2) {
                                CX = CX * BX;
                                printf("\nCX <-- %hu", CX);
                            }
                            if (register2 == 3) {
                                CX = CX * CX;
                                printf("\nCX <-- %hu", CX);
                            }
                            if (register2 == 4) {
                                CX = CX * DX;
                                printf("\nCX <-- %hu", CX);
                            }
                        }
                        if (register1 == 4) {
                            if (register2 == 1) {
                                DX = DX * AX;
                                printf("\nDX <-- %hu", DX);
                            }
                            if (register2 == 2) {
                                DX = DX * BX;
                                printf("\nDX <-- %hu", DX);
                            }
                            if (register2 == 3) {
                                DX = DX * CX;
                                printf("\nDX <-- %hu", DX);
                            }
                            if (register2 == 4) {
                                DX = DX * DX;
                                printf("\nDX <-- %hu", DX);
                            }
                        }
                    }
                    if (readFile[currentIndex] == '[') {
                        currentIndex++;
                        if (register1 == 1) {
                            AX = AX * memory[covertNumber()];
                            printf("\nAX <-- %hu", AX);
                        }
                        if (register1 == 2) {
                            BX = BX * memory[covertNumber()];
                            printf("\nBX <-- %hu", BX);
                        }
                        if (register1 == 3) {
                            CX = CX * memory[covertNumber()];
                            printf("\nCX <-- %hu", CX);
                        }
                        if (register1 == 4) {
                            DX = DX * memory[covertNumber()];
                            printf("\nDX <-- %hu", DX);
                        }
                        currentIndex++;
                    }
                    if (isNumber()) {
                        if (register1 == 1) {
                            AX = AX * covertNumber();
                            printf("\nAX <-- %hu", AX);
                        }
                        if (register1 == 2) {
                            BX = BX * covertNumber();
                            printf("\nBX <-- %hu", BX);
                        }
                        if (register1 == 3) {
                            CX = CX * covertNumber();
                            printf("\nCX <-- %hu", CX);
                        }
                        if (register1 == 4) {
                            DX = DX * covertNumber();
                            printf("\nDX <-- %hu", DX);
                        }
                    }
                }
                if (readFile[currentIndex] == '[') {
                    currentIndex++;
                    unsigned short address = covertNumber();
                    currentIndex += 2;
                    if (isRegister() != 0) {
                        int register1 = isRegister();
                        currentIndex += 2;
                        if (register1 == 1) {
                            memory[address] = memory[address] * AX;
                            printf("\nMemory[%hu] <-- %hu", address, memory[address]);
                        }
                        if (register1 == 2) {
                            memory[address] = memory[address] * BX;
                            printf("\nMemory[%hu] <-- %hu", address, memory[address]);
                        }
                        if (register1 == 3) {
                            memory[address] = memory[address] * CX;
                            printf("\nMemory[%hu] <-- %hu", address, memory[address]);
                        }
                        if (register1 == 4) {
                            memory[address] = memory[address] * DX;
                            printf("\nMemory[%hu] <-- %hu", address, memory[address]);
                        }
                    }
                    if (isNumber()) {
                        unsigned short number = covertNumber();
                        memory[address] = memory[address] * number;
                        printf("\nMemory[%hu] <-- %hu", address, memory[address]);
                    }
                    if (readFile[currentIndex] == '[') {
                        currentIndex++;
                        unsigned short address2 = covertNumber();
                        memory[address] = memory[address] * memory[address2];
                        printf("\nMemory[%hu] <-- %hu", address, memory[address]);
                        currentIndex++;
                    }
                }
            } else if (readFile[currentIndex] == 'B' && readFile[currentIndex + 1] == 'O' &&
                       readFile[currentIndex + 2] == 'L') {
                currentIndex += 4;
                if (isRegister() != 0) {
                    int register1 = isRegister();
                    currentIndex += 3;
                    if (isRegister() != 0) {
                        int register2 = isRegister();
                        currentIndex += 2;
                        if (register1 == 1) {
                            if (register2 == 1) {
                                DX = 0; //Kalan sayiyi tutan register
                                AX = AX / AX;
                                printf("\nAX <-- %hu   (DX : %hu)", AX, DX);
                            }
                            if (register2 == 2) {
                                DX = AX % BX;
                                AX = AX / BX;
                                printf("\nAX <-- %hu   (DX : %hu)", AX, DX);
                            }
                            if (register2 == 3) {
                                DX = AX % CX;
                                AX = AX / CX;
                                printf("\nAX <-- %hu   (DX : %hu)", AX, DX);
                            }
                            if (register2 == 4) { //Eger DX registeri direkt olarak kullanilirsa kalani tutma ozelligini iptal et.
                                AX = AX / DX;
                                printf("\nAX <-- %hu   (DX : %hu)", AX, DX);
                            }
                        }
                        if (register1 == 2) {
                            if (register2 == 1) {
                                DX = BX % AX;
                                BX = BX / AX;
                                printf("\nBX <-- %hu   (DX : %hu)", BX, DX);
                            }
                            if (register2 == 2) {
                                DX = 0;
                                BX = BX / BX;
                                printf("\nBX <-- %hu   (DX : %hu)", BX, DX);
                            }
                            if (register2 == 3) {
                                DX = BX % CX;
                                BX = BX / CX;
                                printf("\nBX <-- %hu   (DX : %hu)", BX, DX);
                            }
                            if (register2 == 4) { //Eger DX registeri direkt olarak kullanilirsa kalani tutma ozelligini iptal et.
                                BX = BX / DX;
                                printf("\nBX <-- %hu   (DX : %hu)", BX, DX);
                            }
                        }
                        if (register1 == 3) {
                            if (register2 == 1) {
                                DX = CX % AX;
                                CX = CX / AX;
                                printf("\nCX <-- %hu   (DX : %hu)", CX, DX);
                            }
                            if (register2 == 2) {
                                DX = CX % BX;
                                CX = CX / BX;
                                printf("\nCX <-- %hu", CX);
                                printf("\nDX : %d", DX);
                            }
                            if (register2 == 3) {
                                DX = 0;
                                CX = CX / CX;
                                printf("\nCX <-- %hu   (DX : %hu)", CX, DX);
                            }
                            if (register2 == 4) { //Eger DX registeri direkt olarak kullanilirsa kalani tutma ozelligini iptal et.
                                CX = CX / DX;
                                printf("\nCX <-- %hu   (DX : %hu)", CX, DX);
                            }
                        }
                        if (register1 == 4) { //Eger DX registeri direkt olarak kullanilirsa kalani tutma ozelligini iptal et.
                            if (register2 == 1) {
                                DX = DX / AX;
                                printf("\nDX <-- %hu", DX);
                            }
                            if (register2 == 2) {
                                DX = DX / BX;
                                printf("\nDX <-- %hu", DX);
                            }
                            if (register2 == 3) {
                                DX = DX / CX;
                                printf("\nDX <-- %hu", DX);
                            }
                            if (register2 == 4) {
                                DX = DX / DX;
                                printf("\nDX <-- %hu", DX);
                            }
                        }
                    }
                    if (readFile[currentIndex] == '[') {
                        currentIndex++;
                        unsigned short address = covertNumber();
                        if (register1 == 1) {
                            DX = AX % memory[address];
                            AX = AX / memory[address];
                            printf("\nAX <-- %hu   (DX : %hu)", AX, DX);
                        }
                        if (register1 == 2) {
                            DX = BX % memory[address];
                            BX = BX / memory[covertNumber()];
                            printf("\nBX <-- %hu   (DX : %hu)", BX, DX);
                        }
                        if (register1 == 3) {
                            DX = CX % memory[address];
                            CX = CX / memory[covertNumber()];
                            printf("\nCX <-- %hu   (DX : %hu)", CX, DX);
                        }
                        if (register1 == 4) { //Eger DX registeri direkt olarak kullanilirsa kalani tutma ozelligini iptal et.
                            DX = DX / memory[covertNumber()];
                            printf("\nDX <-- %hu", DX);
                        }
                        currentIndex++;
                    }
                    if (isNumber()) {
                        unsigned short data = covertNumber();
                        if (register1 == 1) {
                            DX = AX % data;
                            AX = AX / data;
                            printf("\nAX <-- %hu   (DX : %hu)", AX, DX);
                        }
                        if (register1 == 2) {
                            DX = BX % data;
                            BX = BX / data;
                            printf("\nBX <-- %hu   (DX : %hu)", BX, DX);
                        }
                        if (register1 == 3) {
                            DX = CX % data;
                            CX = CX / data;
                            printf("\nCX <-- %hu   (DX : %hu)", CX, DX);
                        }
                        if (register1 == 4) {//Eger DX registeri direkt olarak kullanilirsa kalani tutma ozelligini iptal et.
                            DX = DX / data;
                            printf("\nDX <-- %hu", DX);
                        }
                    }
                }
                if (readFile[currentIndex] == '[') {
                    currentIndex++;
                    unsigned short address = covertNumber();
                    currentIndex += 2;
                    if (isRegister() != 0) {
                        int register1 = isRegister();
                        currentIndex += 2;
                        if (register1 == 1) {
                            DX = memory[address] % AX;
                            memory[address] = memory[address] / AX;
                            printf("\nMemory[%hu] <-- %hu   (DX : %hu)", address, memory[address], DX);
                        }
                        if (register1 == 2) {
                            DX = memory[address] % BX;
                            memory[address] = memory[address] / BX;
                            printf("\nMemory[%hu] <-- %hu   (DX : %hu)", address, memory[address], DX);
                        }
                        if (register1 == 3) {
                            DX = memory[address] % CX;
                            memory[address] = memory[address] / CX;
                            printf("\nMemory[%hu] <-- %hu   (DX : %hu)", address, memory[address], DX);
                        }
                        if (register1 == 4) { //Eger DX registeri direkt olarak kullanilirsa kalani tutma ozelligini iptal et.
                            memory[address] = memory[address] / DX;
                            printf("\nMemory[%hu] <-- %hu   (DX : %hu)", address, memory[address], DX);
                        }
                    }
                    if (isNumber()) {
                        unsigned short number = covertNumber();
                        DX = memory[address] % number;
                        memory[address] = memory[address] / number;
                        printf("\nMemory[%hu] <-- %hu   (DX : %hu)", address, memory[address], DX);
                    }
                    if (readFile[currentIndex] == '[') {
                        currentIndex++;
                        unsigned short address2 = covertNumber();
                        DX = memory[address] % memory[address2];
                        memory[address] = memory[address] / memory[address2];
                        printf("\nMemory[%hu] <-- %hu   (DX : %hu)", address, memory[address], DX);
                        currentIndex++;
                    }
                }
            }
            currentIndex++;
        }
    } else {
        printf("Dosya bulunamadi.");
    }
    return 0;
}