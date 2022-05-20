/*
 * Ýsim : Hasan Can Özbek
 * No : 02200201007
 */

#include <stdio.h>
#include <string.h>

int currentIndex;

int compare(char c) { //Programdaki ayraç karakterleri bulmaya yarayan fonksiyon
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
        keyword[j] = token[index][j]; //tokendeki istenilen kelimeyi parçala ve döndür.
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
    //Kelimenin istenilen formatta olup olmadýðýna bakan fonksiyon
    int i, control;
    i = 0;
    control = 1;
    while (token[currentIndex][i] != '\0') { //Anlýk üzerinde bulunduðu kelimenin sonuna kadar git
        if (token[currentIndex][i] == keyword[i]) {//harfler birbirine eþit mi?
            i++;
        } else {
            control = 0; //harf bulunamadý kontrol deðiþkenini 0 yap
            i++;
        }
    }
    if (control == 1) { //kelimeler eþleþti ise
        currentIndex++;//bir sonraki kelimeye geç
        return 1;
    } else {
        return 0;
    }
}

int id(int row, int column, char token[row][column]) {
    //deðiþken tanýmlamalarýný kontrol eden fonksiyon
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
            if (control == 0) { // kontrol 0 ise kaynak kod alfabe dýþýndan eleman içeriyor
                return 0;
            }
            i++;
        }
        currentIndex++; //bir sonraki kelimeye geç
        return 1;
    }
}

int operator(int row, int column, char token[row][column]) {
    // Operatörlerin durumunu kontrol eden fonksiyon
    if (keywordControl(row, column, token, "<") == 1 || keywordControl(row, column, token, ">") == 1 ||
        keywordControl(row, column, token, "<=") == 1 || keywordControl(row, column, token, ">=") == 1 ||
        keywordControl(row, column, token, "==") == 1) {
        return 1;
    } else {
        return 0;
    }
}

int condition(int row, int column, char token[row][column]) {
    //koþul cümlelerini kontrol eden fonksiyon
    if (id(row, column, token) == 1) {//deðiþken ile baþlamalý
        if (operator(row, column, token) == 1) {//ardýndan bir operatör almalý
            if (id(row, column, token) == 1) { //deðiþken ile bitmeli
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
    //Atama cümlelerini kontrol eden fonksiyon
    if (id(row, column, token) == 1) { //deðiþken ile baþlamalý
        if (keywordControl(row, column, token, "=") == 1) {// atama operatörü kontrolü yapýldý
            if (id(row, column, token) == 1) { //= iþaretinden sonra bir deðiþken gelmeli
                //deðiþkenden sonra tekrar bir operatör gelebilir. Bu durumu kontrol altýna al
                while (token[currentIndex][0] == '+' || token[currentIndex][0] == '-' ||
                       token[currentIndex][0] == '*' || token[currentIndex][0] == '/') {
                    currentIndex++;
                    if (id(row, column, token) == 1) { //operatörden sonra mutlaka deðiþken ile bitmeli
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
    //if elseif else yapýlarýný gramer testine tabi tutan fonksiyon
    currentIndex = 0; //global deðiþkeni kaynak kodun ilk indexinden baþlat
    //IF YAPISI
    if (keywordControl(row, column, token, "if") == 1) { // if ile baþlama durumu
        if (keywordControl(row, column, token, "(") == 1) { //parantez varlýðýný kontrol et
            if (condition(row, column, token) == 1) { //þart cümlesini test et
                if (keywordControl(row, column, token, ")") == 1) { // parantez ile kapamayý kontrol et
                    if (keywordControl(row, column, token, "{") == 1) { //parantez kontrolü
                        if (statement(row, column, token)) { //atama cümlesi testi
                            if (keywordControl(row, column, token, "}") == 1) { //parantez kontrolü

                                //ELSE IF YAPISI
                                while (strcmp(getKeyword(currentIndex, row, column, token), "else") == 0 &&
                                       strcmp(getKeyword(currentIndex + 1, row, column, token), "if") == 0) {
                                    //Gelen kelime else ve arkasýndan if geliyorsa döngüye gir.
                                    currentIndex += 2;//ilk iki kelime döngüde þartý saðladýðý için 2 kelime atlandý
                                    if (keywordControl(row, column, token, "(") == 1) { //parantez kontrolü
                                        if (condition(row, column, token) == 1) {//þart cümlesi testi
                                            if (keywordControl(row, column, token, ")") == 1) {//parantez kontrolü
                                                if (keywordControl(row, column, token, "{") == 1) {//parantez kontrolü
                                                    if (statement(row, column, token) == 1) {//atama cümlesi testi
                                                        if (keywordControl(row, column, token, "}") ==
                                                            1) {//parantez kontrolü
                                                            //Kodda herhangi bir hata gramer hatasý bulunmadý.
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
                                                //Kod baþarýyla derlendi.
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
    //Eðer return deyimlerine yakalanmazsa herhangi bir hata yakalanmadý demektir.
    printf("Kod basariyla gramer testinden gecti.");
}

int main() {
    char array[256];
    char character;
    FILE *file;
    char file_path[256];
    printf("Dosya yolunu giriniz (\" kullanmadan): ");
    scanf("%s", &file_path);

    //fopen fonksiyonu file tipi döndürür. Eðer hata alýrsa null deðer döner.
    if ((file = fopen(file_path, "r")) != NULL) {

        //DOSYADAN KAYNAK KODLARI OKUMA VE CHAR DÝZÝYE ATAMA ALGORÝTMASI
        int i;
        i = 0;
        character = fgetc(file); //Dosyadan tek bir karakter alýndý
        array[i] = character;
        i++;
        while (character != EOF) { //feof karakter dizisinin sonuna gelindiðini gösterir
            character = fgetc(file);
            array[i] = character;
            i++;
        }
        // DOSYADAKÝ KODLARIN KELÝMELEÞTÝRÝLMESÝ ALGORÝTMASI
        int size_of_array = strlen(array);
        char token[size_of_array][64]; //For,printf, while gibi dilin tanýmlý kelimelerini tutabilmesi için 2 boyutlu array
        int j = 0;
        int k = 0;

        for (i = 0; i < size_of_array; i++) {
            char c = array[i];
            if (c == '\"') { //String ifade gelirse yapýlacak iþlemler
                while (array[i + 1] != '\"') { //Tekrar " görene kadar ilerle gelen karakteri direkt yaz
                    token[j][k] = array[i];
                    i++;
                    k++;
                }
                token[j][k] = array[i]; //Döngüden çýktýðý için son karakter token arrayine eklendi
                token[j][++k] = '\"'; //" iþareti getirilerek string tamamlanmýþ oldu
                token[j][++k] = '\0'; //satýr sonuna gelindi
                i++;
                j++;
                k = 0;
            } else {
                if (compare(c) == 1) { // c karakteri bu dilin tanýmlý bir karakteri mi?
                    if (c != ' ' && c != '\t' && c != '\n') {
                        /**Buradaki amaç kelimenin bittiðini programa bildirebilmektir.
                            * Her satýrýn sonunda '\0' iþareti konur, böylece \0 iþareti
                            * görüldüðü zaman o kelimenin sonuna gelindiði anlaþýlýr. Aþaðýda ,yazdýrma alanýnda, bu
                            * yapýdan yararlanýlacaktýr.**/
                        token[j][k] = c;
                        if (compare(array[i + 1]) == 0) { // Sonraki karakter ayraç deðil ise
                            token[j][k + 1] = '\0';
                            j++;
                        } else {
                            // ++, --, +=, -=, /=, *= ,==, >=, <= operatörleri de ardýþýl þekilde diziye eklendi.
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
                    token[j][k] = c; //Ayraç karakterlerden biri deðilse bir ayraç gelene kadar sonuna ekler
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
            while (token[l][m] != '\0') { //Seçilen : karakterini görene kadar (kelimenin sonunu görene kadar) döner
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

    //Gereksiz yere alan tüketmemesi için iþlem sonunda dosyanýn adresi serbest býrakýlýr
    fclose(file);
    return 0;
}
