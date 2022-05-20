/*
 * �sim : Hasan Can �zbek
 * No : 02200201007
 */

#include <stdio.h>
#include <string.h>

int currentIndex;

int compare(char c) { //Programdaki ayra� karakterleri bulmaya yarayan fonksiyon
    char grammers[] = {' ', '\n', '\t', '(', ')', ';', '{', '}', '+', '-', '=', '/', '*', '&', '%', '<', '>', '\"'};
    int i;
    for (i = 0; i < strlen(grammers); i++) {
        if (c == grammers[i]) { //Parametredeki karakter dilin grammerinde var olan bir karakter mi?
            return 1;
        }
    }
    return 0;
}

char *getKeyword(int index, int row, int column, char token[row][column]) {
    int j;
    j = 0;
    static char keyword[64];
    while (token[index][j] != '\0') {
        keyword[j] = token[index][j]; //tokendeki istenilen kelimeyi par�ala ve d�nd�r.
        j++;
    }
    keyword[j] = '\0'; //kelime sonuna gelindi
    return keyword;
}

int number(int row, int column, char token[row][column]) {
    char figures[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    int i, control;
    i = 0;
    while (token[currentIndex][i] != '\0') {
        control = 0;
        int j;
        for (j = 0; j < 10; ++j) {
            if (token[currentIndex][i] == figures[j]) {
                control = 1;
            }
        }
        if (control == 0) {
            return 0;
        }
        i++;
    }
    return 1;
}

int keywordControl(int row, int column, char token[row][column], char keyword[]) {
    //Kelimenin istenilen formatta olup olmad���na bakan fonksiyon
    int i, control;
    i = 0;
    control = 1;
    while (token[currentIndex][i] != '\0') { //Anl�k �zerinde bulundu�u kelimenin sonuna kadar git
        if (token[currentIndex][i] == keyword[i]) {//harfler birbirine e�it mi?
            i++;
        } else {
            control = 0; //harf bulunamad� kontrol de�i�kenini 0 yap
            i++;
        }
    }
    if (control == 1) { //kelimeler e�le�ti ise
        currentIndex++;//bir sonraki kelimeye ge�
        return 1;
    } else {
        return 0;
    }
}

int id(int row, int column, char token[row][column]) {
    //de�i�ken tan�mlamalar�n� kontrol eden fonksiyon
    char alphabet[6] = {'a', 'b', 'c', 'd', 'e', 'f'};
    int control, i;
    control = 0;
    i = 0;
    if (number(row, column, token) == 1) {
        currentIndex++;
        return 1;
    } else {
        while (token[currentIndex][i] != '\0') { //kelimenin sonuna kadar git
            control = 0;
            int j;
            for (j = 0; j < 6; j++) {
                if (token[currentIndex][i] == alphabet[j]) {
                    control = 1; //harf bulundu
                }
            }
            if (control == 0) { // kontrol 0 ise kaynak kod alfabe d���ndan eleman i�eriyor
                return 0;
            }
            i++;
        }
        currentIndex++; //bir sonraki kelimeye ge�
        return 1;
    }
}

int operator(int row, int column, char token[row][column]) {
    // Operat�rlerin durumunu kontrol eden fonksiyon
    if (keywordControl(row, column, token, "<") == 1 || keywordControl(row, column, token, ">") == 1 ||
        keywordControl(row, column, token, "<=") == 1 || keywordControl(row, column, token, ">=") == 1 ||
        keywordControl(row, column, token, "==") == 1) {
        return 1;
    } else {
        return 0;
    }
}

int condition(int row, int column, char token[row][column]) {
    //ko�ul c�mlelerini kontrol eden fonksiyon
    if (id(row, column, token) == 1) {//de�i�ken ile ba�lamal�
        if (operator(row, column, token) == 1) {//ard�ndan bir operat�r almal�
            if (id(row, column, token) == 1) { //de�i�ken ile bitmeli
                return 1;
            } else {
                printf("Sart cumleleri bir degisken ile bitmelidir. Hatanin bulundugu yer : %s\n",
                       getKeyword(currentIndex, row, column, token));
                return 0;
            }
        } else {
            printf("Sart cumleleri bir operator icermelidir. Hatanin bulundugu yer : %s\n",
                   getKeyword(currentIndex, row, column, token));
            return 0;
        }
    } else {
        printf("Sart cumleleri bir degisken ile baslamalidir. Hatanin bulundugu yer : %s\n",
               getKeyword(currentIndex, row, column, token));
        return 0;
    }
}

int statement(int row, int column, char token[row][column]) {
    //Atama c�mlelerini kontrol eden fonksiyon
    if (id(row, column, token) == 1) { //de�i�ken ile ba�lamal�
        if (keywordControl(row, column, token, "=") == 1) {// atama operat�r� kontrol� yap�ld�
            if (id(row, column, token) == 1) { //= i�aretinden sonra bir de�i�ken gelmeli
                //de�i�kenden sonra tekrar bir operat�r gelebilir. Bu durumu kontrol alt�na al
                while (token[currentIndex][0] == '+' || token[currentIndex][0] == '-' ||
                       token[currentIndex][0] == '*' || token[currentIndex][0] == '/') {
                    currentIndex++;
                    if (id(row, column, token) == 1) { //operat�rden sonra mutlaka de�i�ken ile bitmeli
                    } else {
                        printf("Hatali atama cumlesi tanimi. Hatanin bulundugu yer : %s\n",
                               getKeyword(currentIndex, row, column, token));
                        return 0;
                    }
                }
                return 1;
            } else {
                printf("Atama cumleleri bir degisken ile sonlanmalidir. Hatanin bulundugu yer : %s\n",
                       getKeyword(currentIndex, row, column, token));
                return 0;
            }
        } else {
            printf("Atama cumleleri '=' operatoru barindirmalidir. Hatanin bulundugu yer : %s\n",
                   getKeyword(currentIndex, row, column, token));
            return 0;
        }
    } else {
        printf("Atama cumleleri degisken ile baslamalidir. Hatanin bulundugu yer : %s\n",
               getKeyword(currentIndex, row, column, token));
        return 0;
    }
}

void ifStatement(int row, int column, char token[row][column]) {
    //if elseif else yap�lar�n� gramer testine tabi tutan fonksiyon
    currentIndex = 0; //global de�i�keni kaynak kodun ilk indexinden ba�lat
    //IF YAPISI
    if (keywordControl(row, column, token, "if") == 1) { // if ile ba�lama durumu
        if (keywordControl(row, column, token, "(") == 1) { //parantez varl���n� kontrol et
            if (condition(row, column, token) == 1) { //�art c�mlesini test et
                if (keywordControl(row, column, token, ")") == 1) { // parantez ile kapamay� kontrol et
                    if (keywordControl(row, column, token, "{") == 1) { //parantez kontrol�
                        if (statement(row, column, token)) { //atama c�mlesi testi
                            if (keywordControl(row, column, token, "}") == 1) { //parantez kontrol�

                                //ELSE IF YAPISI
                                while (strcmp(getKeyword(currentIndex, row, column, token), "else") == 0 &&
                                       strcmp(getKeyword(currentIndex + 1, row, column, token), "if") == 0) {
                                    //Gelen kelime else ve arkas�ndan if geliyorsa d�ng�ye gir.
                                    currentIndex += 2;//ilk iki kelime d�ng�de �art� sa�lad��� i�in 2 kelime atland�
                                    if (keywordControl(row, column, token, "(") == 1) { //parantez kontrol�
                                        if (condition(row, column, token) == 1) {//�art c�mlesi testi
                                            if (keywordControl(row, column, token, ")") == 1) {//parantez kontrol�
                                                if (keywordControl(row, column, token, "{") == 1) {//parantez kontrol�
                                                    if (statement(row, column, token) == 1) {//atama c�mlesi testi
                                                        if (keywordControl(row, column, token, "}") ==
                                                            1) {//parantez kontrol�
                                                            //Kodda herhangi bir hata gramer hatas� bulunmad�.
                                                        } else {
                                                            printf("else if kosul blogu '}' barindirmiyor. Hatanin bulundugu yer : %s\n",
                                                                   getKeyword(currentIndex, row, column, token));
                                                            return;
                                                        }
                                                    } else {
                                                        return;
                                                    }
                                                } else {
                                                    printf("else if kosul blogu '{' barindirmiyor. Hatanin bulundugu yer : %s\n",
                                                           getKeyword(currentIndex, row, column, token));
                                                    return;
                                                }
                                            } else {
                                                printf("else if kosul blogu ')' barindirmiyor. Hatanin bulundugu yer : %s\n",
                                                       getKeyword(currentIndex, row, column, token));
                                                return;
                                            }
                                        } else {
                                            return;
                                        }
                                    } else {
                                        printf("else if kosul blogu '(' barindirmiyor. Hatanin bulundugu yer : %s\n",
                                               getKeyword(currentIndex, row, column, token));
                                        return;
                                    }
                                }

                                //ELSE YAPISI
                                if (strcmp(getKeyword(currentIndex, row, column, token), "else") == 0) {
                                    currentIndex++;
                                    if (keywordControl(row, column, token, "{") == 1) {
                                        if (statement(row, column, token) == 1) {
                                            if (keywordControl(row, column, token, "}") == 1) {
                                                //Kod ba�ar�yla derlendi.
                                            } else {
                                                printf("else yapisi '}' icermelidir. Hatanin bulundugu yer : %s\n",
                                                       getKeyword(currentIndex - 1, row, column, token));
                                                return;
                                            }
                                        } else {
                                            return;
                                        }
                                    } else {
                                        printf("else yapisi '{' icermelidir. Hatanin bulundugu yer : %s\n",
                                               getKeyword(currentIndex, row, column, token));
                                        return;
                                    }
                                }
                                if (strcmp(getKeyword(currentIndex, row, column, token), "else") == 0) {
                                    printf("Bir if icin birden fazla else yapisi olusturulamaz. Hatanin bulundugu yer : %s\n",
                                           getKeyword(currentIndex, row, column, token));
                                    return;
                                }


                            } else {
                                printf("if kosul blogu '}' barindirmiyor. Hatanin bulundugu yer : %s\n",
                                       getKeyword(currentIndex - 1, row, column, token));
                                return;
                            }
                        } else {
                            return;
                        }
                    } else {
                        printf("if kosul blogu '{' barindirmalidir. Hatanin bulundugu yer : %s\n",
                               getKeyword(currentIndex, row, column, token));
                        return;
                    }
                } else {
                    printf("Kosul cumleleri ')' ile sonlanmalidir. Hatanin bulundugu yer : %s\n",
                           getKeyword(currentIndex, row, column, token));
                    return;
                }
            } else {
                return;
            }
        } else {
            printf("Kosul cumleleri '(' icerisinde olmalidir. Hatanin bulundugu yer : %s\n",
                   getKeyword(currentIndex, row, column, token));
            return;
        }
    } else {
        printf("Program if ile baslamalidir. Hatanin bulundugu yer : %s\n",
               getKeyword(currentIndex, row, column, token));
        return;
    }
    //E�er return deyimlerine yakalanmazsa herhangi bir hata yakalanmad� demektir.
    printf("Kod basariyla gramer testinden gecti.");
}

int main() {
    char array[256];
    char character;
    FILE *file;
    char file_path[256];
    printf("Dosya yolunu giriniz (\" kullanmadan): ");
    scanf("%s", &file_path);

    //fopen fonksiyonu file tipi d�nd�r�r. E�er hata al�rsa null de�er d�ner.
    if ((file = fopen(file_path, "r")) != NULL) {

        //DOSYADAN KAYNAK KODLARI OKUMA VE CHAR D�Z�YE ATAMA ALGOR�TMASI
        int i;
        i = 0;
        character = fgetc(file); //Dosyadan tek bir karakter al�nd�
        array[i] = character;
        i++;
        while (character != EOF) { //feof karakter dizisinin sonuna gelindi�ini g�sterir
            character = fgetc(file);
            array[i] = character;
            i++;
        }
        // DOSYADAK� KODLARIN KEL�MELE�T�R�LMES� ALGOR�TMASI
        int size_of_array = strlen(array);
        char token[size_of_array][64]; //For,printf, while gibi dilin tan�ml� kelimelerini tutabilmesi i�in 2 boyutlu array
        int j = 0;
        int k = 0;

        for (i = 0; i < size_of_array; i++) {
            char c = array[i];
            if (c == '\"') { //String ifade gelirse yap�lacak i�lemler
                while (array[i + 1] != '\"') { //Tekrar " g�rene kadar ilerle gelen karakteri direkt yaz
                    token[j][k] = array[i];
                    i++;
                    k++;
                }
                token[j][k] = array[i]; //D�ng�den ��kt��� i�in son karakter token arrayine eklendi
                token[j][++k] = '\"'; //" i�areti getirilerek string tamamlanm�� oldu
                token[j][++k] = '\0'; //sat�r sonuna gelindi
                i++;
                j++;
                k = 0;
            } else {
                if (compare(c) == 1) { // c karakteri bu dilin tan�ml� bir karakteri mi?
                    if (c != ' ' && c != '\t' && c != '\n') {
                        /**Buradaki ama� kelimenin bitti�ini programa bildirebilmektir.
                            * Her sat�r�n sonunda '\0' i�areti konur, b�ylece \0 i�areti
                            * g�r�ld��� zaman o kelimenin sonuna gelindi�i anla��l�r. A�a��da ,yazd�rma alan�nda, bu
                            * yap�dan yararlan�lacakt�r.**/
                        token[j][k] = c;
                        if (compare(array[i + 1]) == 0) { // Sonraki karakter ayra� de�il ise
                            token[j][k + 1] = '\0';
                            j++;
                        } else {
                            // ++, --, +=, -=, /=, *= ,==, >=, <= operat�rleri de ard���l �ekilde diziye eklendi.
                            if ((array[i] == '+' && array[i + 1] == '+') || (array[i] == '-' && array[i + 1] == '-') ||
                                (array[i] == '+' && array[i + 1] == '=') || (array[i] == '-' && array[i + 1] == '=') ||
                                (array[i] == '*' && array[i + 1] == '=') || (array[i] == '/' && array[i + 1] == '=') ||
                                (array[i] == '=' && array[i + 1] == '=') || (array[i] == '>' && array[i + 1] == '=') ||
                                (array[i] == '<' && array[i + 1] == '=')) {
                                token[j][++k] = array[++i];
                            }
                            token[j][++k] = '\0';
                            j++;
                            k = 0;
                        }
                    }
                } else {
                    token[j][k] = c; //Ayra� karakterlerden biri de�ilse bir ayra� gelene kadar sonuna ekler
                    k++;
                    if (compare(array[i + 1]) == 1) {
                        token[j][k] = '\0';
                        j++;
                        k = 0;
                    }
                }
            }

        }
        printf("\n--------------Kelimelestirme Sonucu Kod--------------\n");
        int l;
        for (l = 0; l < j; l++) {
            int m = 0;
            while (token[l][m] != '\0') { //Se�ilen : karakterini g�rene kadar (kelimenin sonunu g�rene kadar) d�ner
                printf("%c", token[l][m]);
                m++;
            }
            printf(",");

        }

        printf("\n--------------------Parser Islemi--------------------\n");
        ifStatement(size_of_array, 64, token);

    } else {
        printf("Dosya bulunamadi.");
    }

    //Gereksiz yere alan t�ketmemesi i�in i�lem sonunda dosyan�n adresi serbest b�rak�l�r
    fclose(file);
    return 0;
}
