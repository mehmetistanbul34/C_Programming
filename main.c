#include <stdio.h>
#include <stdlib.h>

int enterChoice(void);
void indexleme();

void indexDosyasiOlustur();

void veriDosyasiOlustur();


void ornekDataOlustur();

void kayitEkle();

int indexSil(int ogrNo);

void kayitSil(int line);

void indexEkle(int ogrNo, int line);
char intToChar(int i);

int *getStudentIndexFile(int ogrNo);

struct kayit *getStudentVeriFile(int lineArr[]);

struct kayit updateStudentVeriFile(int lineArr[], int dersKodu, int puan);

void veriDosyasiniGoster();

void indexDosyasiniGoster();

void indexDosyasiniSil();

void veriDosyasiniSil();

void kayitBul(int ogrNo);

int *getStudentIndexWithBinarySearc(int ogrNo);

void kayitGuncelle();

int *getOgrBilgileri();

int binarySearch(int arrOgrNo[], int arrLine[], int ogrNo, int count);


struct kayit {
    int ogrNo;
    int dersKodu;
    double puan;
    char ogrName[100];
    char dersName[100];
};

struct kayit_index {
    int ogrNoAnahtar;
    int line;
};

void test(){
    printf("test fonksiyonu basladı...\n");
    int **myPtr = (int **)malloc(sizeof (int));

    for(int i=0;i<5;i++){
        myPtr[i] = (int*)malloc(5*sizeof (int));
    }

    int boyut = sizeof (myPtr);

    printf("size = %d\n",boyut);

    for(int i=0;i<5;i++){
        for (int j=0;j<5;j++){
            myPtr[i][j] = i*j;
        }
    }

    for(int i=0;i<5;i++){
        for (int j=0;j<5;j++){
            printf("myPtr[%d][%d]=%d ",i,j,myPtr[i][j]);
        }
        printf("\n");
    }

    printf("test fonksiyonu bitti...");
}

void main() {

    int choice;

    indexDosyasiOlustur();
    veriDosyasiOlustur();

    while ((choice = enterChoice()) != 10) {
        switch (choice) {
            case 0:
                indexleme();
                break;
            case 1:
                kayitEkle();
                break;
            case 2:
                int ogrNo;
                printf("ogrNo Girin :  ");
                scanf("%d", &ogrNo);
                kayitBul(ogrNo);
                break;
            case 3:
                int delOgrNo;
                printf("ogrNo Girin : ");
                scanf("%d", &delOgrNo);
                int removeLine = indexSil(delOgrNo);
                kayitSil(removeLine);
                break;
            case 4:
                kayitGuncelle();
                break;
            case 5:
                veriDosyasiniGoster();
                break;
            case 6:
                indexDosyasiniGoster();
                break;
            case 7:
                indexDosyasiniSil();
                veriDosyasiniSil();
                break;
            case 8:
                indexDosyasiOlustur();
                veriDosyasiOlustur();
                break;
            case 9:
                ornekDataOlustur();
                break;
            case 40:
                test();
        }
    }

}

int enterChoice(void) {
    int menuChoice;
    printf("\nSecenekler\n"
           "0 - Indexle Olustur\n"
           "1 - Kayit Ekle\n"
           "2 - Kayit Bul\n"
           "3 - Kayit Sil\n"
           "4 - Kayit Guncelle\n"
           "5 - Veri Dosyasini Goster\n"
           "6 - Index Dosyasini Goster\n"
           "7 - Index Dosyasini Sil\n"
           "8 - Dosyaları Olustur\n"
           "9 - Ornek Veri Olustur Ve indexle\n"
           "10 - Cikis\n ");
    scanf("%d", &menuChoice);
    return menuChoice;
}

void indexDosyasiOlustur() {
    FILE *indexFPtr = fopen("../text_data.txt", "r+");
    if (indexFPtr == NULL) {
        indexFPtr = fopen("../text_data.txt", "w");
        if (indexFPtr != NULL) {
            printf("text_data.txt Dosya olusturuldu\n");
        } else {
            printf("text_data.txt Dosya olusturulmadı!!!!!\n");
        }
    } else {
        printf("text_data.txt Dosya acildi\n");
    }
    fclose(indexFPtr);
}

void veriDosyasiOlustur() {
    FILE *binaryFPtr = fopen("../binary_data.bin", "r+b");
    if (binaryFPtr == NULL) {
        binaryFPtr = fopen("../binary_data.bin", "wb");
        if (binaryFPtr != NULL) {
            printf("binary_data.bin Dosya olusturuldu\n");
        } else {
            printf("binary_data.bin Dosya olusturulmadı!!!!!\n");
        }
    } else {
        printf("binary_data.bin Dosya acildi\n");
    }
    fclose(binaryFPtr);
}

void kayitEkle() {
    FILE *binaryFPtr = fopen("../binary_data.bin", "r+b");

    struct kayit client = {0, 0, 0.0, "", ""};
    printf("OgrNo Girin : ");
    scanf("%d", &client.ogrNo);
    printf("DersKodu Girin : ");
    scanf("%d", &client.dersKodu);
    printf("Puan Girin : ");
    scanf("%lf", &client.puan);
    printf("OgrName : ");
    scanf("%s", &client.ogrName);
    printf("DersName Girin : ");
    scanf("%s", &client.dersName);
    fseek(binaryFPtr, 0,
          SEEK_END);
    fwrite(&client, sizeof(struct kayit), 1, binaryFPtr);
    int line = ftell(binaryFPtr) / sizeof(struct kayit);
    indexEkle(client.ogrNo, line);
    fclose(binaryFPtr);
}

void ornekDataOlustur() {
    FILE *binaryFPtr = fopen("../binary_data.bin", "r+b");
    for (int i = 1; i < 7; i++) {
        for (int j = 1; j < 8; j++) {
            struct kayit client = {0, 0, 0.0, "", ""};
            client.ogrNo = i;
            client.dersKodu = j;
            client.puan = 5 * i * j;
            client.ogrName[0] = 'o';
            client.ogrName[1] = 'g';
            client.ogrName[2] = 'r';
            client.ogrName[3] = intToChar(i);//atoi(i);
            client.ogrName[4] = intToChar(j);//atoi(j);
            client.dersName[0] = 'd';
            client.dersName[1] = 'e';
            client.dersName[2] = 'r';
            client.dersName[3] = 's';
            client.dersName[4] = intToChar(i);//atoi(i);
            client.dersName[5] = intToChar(j);//atoi(j);
            client.dersName[6] = '0';
            fseek(binaryFPtr, 0,
                  SEEK_END);
            fwrite(&client, sizeof(struct kayit), 1, binaryFPtr);
            //int line = ftell(binaryFPtr) / sizeof(struct kayit);
            //indexEkle(client.ogrNo, line);
        }
    }
    fclose(binaryFPtr);
}

char intToChar(int i){
    switch(i){
        case 0:
            return '0';
        case 1:
            return '1';
        case 2:
            return '2';
        case 3:
            return '3';
        case 4:
            return '4';
        case 5:
            return '5';
        case 6:
            return '6';
        case 7:
            return '7';
        case 8:
            return '8';
        case 9:
            return '9';
    }
}


void indexleme() {
    FILE *binaryFPtr = fopen("../binary_data.bin", "r+b");
    rewind(binaryFPtr);
    int count = 0;

    while (!feof(binaryFPtr)) {
        struct kayit client = {0, 0, 0.0, "", ""};
        fread(&client, sizeof(struct kayit), 1, binaryFPtr);
        count++;
    }
    rewind(binaryFPtr);
    int fileBinaryIndex = count-1;
    count = 0;
    while (!feof(binaryFPtr) && count<fileBinaryIndex) {
        struct kayit client = {0, 0, 0.0, "", ""};
        fread(&client, sizeof(struct kayit), 1, binaryFPtr);
        if(client.ogrNo!=-1) {
            indexEkle(client.ogrNo, count + 1);
            count++;
        }
    }
    fclose(binaryFPtr);
}

void indexEkle(int ogrNo, int line) {
    FILE *indexFPtr = fopen("../text_data.txt", "r+");
    printf("function : indexEkle ogrNo : %d   line : %d\n", ogrNo, line);

    struct kayit_index addItemIndex;

    addItemIndex.ogrNoAnahtar = ogrNo;
    addItemIndex.line = line;
    fseek(indexFPtr, 0, SEEK_END);
    fwrite(&addItemIndex, sizeof(struct kayit_index), 1, indexFPtr);

    int count = 0;
    rewind(indexFPtr);
    while (!feof(indexFPtr)) {
        struct kayit_index item = {-1, -1};
        fread(&item, sizeof(struct kayit_index), 1, indexFPtr);
        if (!feof(indexFPtr))
            printf("item.anahtar : %d line : %d\n", item.ogrNoAnahtar, item.line);
        count++;
    }
    printf("count : %d\n", count);

    int fileIndexSize = count;

    int swapTrue = -1;
    count = 0;
    rewind(indexFPtr);
    fileIndexSize--;
    while (!feof(indexFPtr) && count < fileIndexSize) {
        struct kayit_index item, item2;
        fseek(indexFPtr, count * sizeof(struct kayit_index), SEEK_SET);
        fread(&item, sizeof(struct kayit_index), 1, indexFPtr);

        if (item.ogrNoAnahtar > addItemIndex.ogrNoAnahtar) {
            swapTrue = 0;
        }

        if (swapTrue == 0) {
            fseek(indexFPtr, count * sizeof(struct kayit_index), SEEK_SET);
            fwrite(&addItemIndex, sizeof(struct kayit_index), 1, indexFPtr);
            addItemIndex.ogrNoAnahtar = item.ogrNoAnahtar;
            addItemIndex.line = item.line;
        }

        count++;
    }

    fclose(indexFPtr);

}


int indexSil(int ogrNo) {
    FILE *indexFPtr = fopen("../text_data.txt", "r+");
    int removeLine = -1;

    printf("function : indexSil ogrNo : %d\n", ogrNo);

    int count = 0;
    rewind(indexFPtr);
    while (!feof(indexFPtr)) {
        struct kayit_index item = {-1, -1};
        fread(&item, sizeof(struct kayit_index), 1, indexFPtr);
        if (!feof(indexFPtr))
            printf("item.anahtar : %d line : %d\n", item.ogrNoAnahtar, item.line);
        count++;
    }
    printf("count : %d\n", count);

    int fileIndexSize = count;

    int swapTrue = -1;
    count = 0;
    rewind(indexFPtr);
    fileIndexSize--;
    while (!feof(indexFPtr) && count < fileIndexSize) {
        struct kayit_index item, item2;
        fseek(indexFPtr, count * sizeof(struct kayit_index), SEEK_SET);
        fread(&item, sizeof(struct kayit_index), 1, indexFPtr);
        if (item.ogrNoAnahtar > ogrNo && swapTrue == -1) {
            fseek(indexFPtr, (count - 1) * sizeof(struct kayit_index), SEEK_SET);
            fread(&item2, sizeof(struct kayit_index), 1, indexFPtr);
            swapTrue = 0;
            removeLine = item2.line;
        }

        if (swapTrue == 0) {
            fseek(indexFPtr, (count - 1) * sizeof(struct kayit_index), SEEK_SET);
            fwrite(&item, sizeof(struct kayit_index), 1, indexFPtr);
        }
        count++;
    }

    fseek(indexFPtr, (count - 1) * sizeof(struct kayit_index), SEEK_SET);
    struct kayit_index bosKayit = {-1, -1};
    fwrite(&bosKayit, sizeof(struct kayit_index), 1, indexFPtr);

    fclose(indexFPtr);
    return removeLine;

}

void kayitSil(int line) {
    FILE *binaryFPtr = fopen("../binary_data.bin", "r+b");
    line--;

    int count = 0;
    rewind(binaryFPtr);
    while (!feof(binaryFPtr)) {
        struct kayit item;
        fread(&item, sizeof(struct kayit), 1, binaryFPtr);
        if (!feof(binaryFPtr))
        count++;
    }

    int fileIndexSize = count;

    int swapTrue = -1;
    count = 0;
    rewind(binaryFPtr);
    fileIndexSize--;
    while (!feof(binaryFPtr) && count < fileIndexSize) {
        struct kayit item;
        fseek(binaryFPtr, count * sizeof(struct kayit), SEEK_SET);
        fread(&item, sizeof(struct kayit), 1, binaryFPtr);
        if (count > line && swapTrue == -1) {
            swapTrue = 0;
        }

        if (swapTrue == 0) {
            fseek(binaryFPtr, (count - 1) * sizeof(struct kayit), SEEK_SET);
            fwrite(&item, sizeof(struct kayit), 1, binaryFPtr);
        }
        count++;
    }

    fseek(binaryFPtr, (count - 1) * sizeof(struct kayit), SEEK_SET);
    struct kayit bosKayit = {-1, -1, -1, "", ""};
    fwrite(&bosKayit, sizeof(struct kayit), 1, binaryFPtr);

    fclose(binaryFPtr);

}


int *getStudentIndexFile(int ogrNo) {
    FILE *indexFPtr = fopen("../text_data.txt", "r+");
    struct kayit_index item = {-1, -1};
    struct kayit_index founded = {-1, -1};
    rewind(indexFPtr);
    int count = 0;
    while (!feof(indexFPtr)) {
        fread(&founded, sizeof(struct kayit_index), 1, indexFPtr);
        if (founded.ogrNoAnahtar == ogrNo) {
            count++;
        }
    }
    rewind(indexFPtr);
    int *lineArr = (int *) malloc(count * sizeof(int));
    count = 0;
    while (!feof(indexFPtr)) {
        fread(&founded, sizeof(struct kayit_index), 1, indexFPtr);
        if (founded.ogrNoAnahtar == ogrNo) {
            lineArr[count] = founded.line;
            count++;
        }
    }
    lineArr[count] = NULL;

    fclose(indexFPtr);

    return lineArr;
}

int *getStudentIndexWithBinarySearc(int ogrNo) {
    FILE *indexFPtr = fopen("../text_data.txt", "r+");
    struct kayit_index item = {-1, -1};
    struct kayit_index founded = {-1, -1};

    rewind(indexFPtr);
    int count = 0;
    printf("index dosyasida ogrenci numarasi %d olan orencinin linelari: ",ogrNo);
    while (!feof(indexFPtr)) {
        fread(&founded, sizeof(struct kayit_index), 1, indexFPtr);
        if (founded.ogrNoAnahtar == ogrNo) {
            printf("%d,", founded.line);
            count++;
        }
    }
    printf("\n");

    rewind(indexFPtr);
    int *ogrLineArr = (int*)malloc((count-1)*sizeof (int));
    int *ogrArr = (int*)malloc((count-1)*sizeof (int));
    count = 0;
    while (!feof(indexFPtr)) {
        fread(&founded, sizeof(struct kayit_index), 1, indexFPtr);
        if (founded.ogrNoAnahtar == ogrNo) {
            ogrLineArr[count] = founded.line;
            ogrArr[count] = founded.ogrNoAnahtar;
            count++;
        }
    }

    int ss = 0;
    printf("ogrLineArr: ");
    while (ss<count) {
        printf("%d,",ogrLineArr[ss]);
        ss++;
    }
    printf("\n");

    int *lineArr = (int *) malloc(count * sizeof(int));
    lineArr[0] = binarySearch(ogrArr,ogrLineArr,ogrNo,count);

    lineArr[1] = NULL;

    fclose(indexFPtr);

    return lineArr;
}


int *getOgrBilgileri() {
    FILE *indexFPtr = fopen("../text_data.txt", "r+");

    int *studentData = (int*)malloc(2*sizeof(int));

    struct kayit_index founded = {-1, -1};
    rewind(indexFPtr);
    int count = 0;
    while (!feof(indexFPtr)) {
        fread(&founded, sizeof(struct kayit_index), 1, indexFPtr);
        count++;
    }


    int *ogrArr = (int*) malloc(count*sizeof(int));
    int *lineArr = (int*) malloc(count*sizeof(int));
    count = 0;
    while (!feof(indexFPtr)) {
        fread(&founded, sizeof(struct kayit_index), 1, indexFPtr);
        ogrArr[count] = founded.ogrNoAnahtar;
        lineArr[count] = founded.line;
        count++;
    }

    studentData[0] = ogrArr;
    studentData[1] = lineArr;

    fclose(indexFPtr);

    return studentData;
}


void veriDosyasiniGoster() {
    FILE *readPtr = fopen("../binary_data.bin", "r");
    struct kayit client = {0, 0, 0.0, "", ""};
    int lineSize = 0;
    rewind(readPtr);
    printf("%-9s%-10s%-10s%-16s%16s\n",
           "ogrNo", "dersKodu", "Puan", "ogrName", "dersName");
    while (!feof(readPtr)) {
        fread(&client, sizeof(struct kayit), 1, readPtr);
        if (!feof(readPtr) && client.ogrNo != -1)
            printf("%-9d%-10d%-10.2f%-16s%16s\n",
                   client.ogrNo, client.dersKodu,
                   client.puan, client.ogrName, client.dersName);
        lineSize++;
    }
    fclose(readPtr);
}

void indexDosyasiniGoster() {
    FILE *indexFPtr = fopen("../text_data.txt", "r");
    struct kayit_index kayitIndex = {-1, -1};
    int lineSize = 0;
    rewind(indexFPtr);
    printf("%-9s%-20s%\n",
           "ogrNo", "satir");
    while (!feof(indexFPtr)) {
        fread(&kayitIndex, sizeof(struct kayit_index), 1, indexFPtr);
        if (!feof(indexFPtr) && kayitIndex.ogrNoAnahtar != -1) {
            printf("%-9d%-20d\n", kayitIndex.ogrNoAnahtar, kayitIndex.line);
        }
        lineSize++;
    }

    fclose(indexFPtr);

}

void indexDosyasiniSil() {
    if (remove("../text_data.txt") == 0)
        printf("text_data dosyasi silindi\n");
    else
        perror("Dosya zaten yok");
}

void veriDosyasiniSil() {
    if (remove("../binary_data.bin") == 0)
        printf("binary_data dosyasi silindi\n");
    else
        perror("Dosya zaten yok");
}

void kayitBul(int ogrNo) {

    int *lines = getStudentIndexWithBinarySearc(ogrNo);
    printf("\nlines[0] = %d\n",lines[0]);

    struct kayit *foundedKayit = getStudentVeriFile(lines);


    printf("\nFounded ogrNo : %d   dersKodu : %d  puan : %lf   ogrName : %s  dersName : %s\n",
           foundedKayit[0].ogrNo, foundedKayit[0].dersKodu, foundedKayit[0].puan, foundedKayit[0].ogrName,
           foundedKayit[0].dersName);

}

void kayitBul2(int ogrNo) {

    int *studentData = getOgrBilgileri();
    int *student = getOgrBilgileri();

    int founded = binarySearch(student,studentData[0],ogrNo,5);
    int *lines = (int*)malloc(sizeof (int));

    struct kayit *foundedKayit = getStudentVeriFile(lines);
    printf("0000 Founded ogrNo : %d   dersKodu : %d  puan : %lf   ogrName : %s  dersName : %s\n",
           foundedKayit[0].ogrNo, foundedKayit[0].dersKodu, foundedKayit[0].puan, foundedKayit[0].ogrName,
           foundedKayit[0].dersName);

}

struct kayit *getStudentVeriFile(int lineArr[]) {
    FILE *binaryFPtr = fopen("../binary_data.bin", "r+b");
    int count = 0;

    //count = sizeof(lineArr) / sizeof(int);
    while (lineArr[count] != NULL) {
        count++;
    }
    printf("binary dosyasindan okunan satir: %d\n",count);

    struct kayit *kayit = (struct kayit *) malloc(count * sizeof(struct kayit));

    int i = 0;
    while (i < count) {
        fseek(binaryFPtr, (lineArr[i] - 1) * sizeof(struct kayit),
              SEEK_SET);
        fread(&kayit[i], sizeof(struct kayit), 1, binaryFPtr);
        printf("%d.Ogrenci ogrNo : %d   dersKodu : %d  puan : %lf   ogrName : %s  dersName : %s\n", i,
               kayit[i].ogrNo, kayit[i].dersKodu, kayit[i].puan, kayit[i].ogrName, kayit[i].dersName);
        i++;
    }

    fclose(binaryFPtr);
    return kayit;
}


struct kayit updateStudentVeriFile(int lineArr[], int dersKodu, int puan) {
    FILE *binaryFPtr = fopen("../binary_data.bin", "r+b");

    int count = 0;

    while (lineArr[count] != NULL) {
        count++;
    }

    struct kayit kayit = {-1, -1, -1, "", ""};
    int i = 0;
    rewind(binaryFPtr);
    while (i < count) {
        rewind(binaryFPtr);
        fseek(binaryFPtr, (lineArr[i] - 1) * sizeof(struct kayit),
              SEEK_SET);
        fread(&kayit, sizeof(struct kayit), 1, binaryFPtr);
        if (kayit.dersKodu == dersKodu) {
            kayit.puan = puan;
            rewind(binaryFPtr);
            fseek(binaryFPtr, (lineArr[i] - 1) * sizeof(struct kayit),
                  SEEK_SET);
            fwrite(&kayit, sizeof(struct kayit), 1, binaryFPtr);
        }
        i++;
    }

    kayit.ogrNo = -1;
    kayit.dersKodu = -1;
    kayit.puan = -1;

    for (i = 0; i < 20; i++) {
        kayit.dersName[i] = "";
        kayit.ogrName[i] = "";
    }

    fclose(binaryFPtr);

    return kayit;
}


void kayitGuncelle() {
    int ogrNo;
    int dersKodu;
    int puan;
    printf("Ogrenci Numarasini Girin : ");
    scanf("%d", &ogrNo);
    printf("Ders Numarası Girin : ");
    scanf("%d", &dersKodu);
    printf("Puan  Girin : ");
    scanf("%d", &puan);
    int *lineArr = getStudentIndexFile(ogrNo);
    int count = 0;
    while (lineArr[count] != NULL) {
        count++;
    }

    for (int i = 0; i < count; i++) {
        printf("lineArr[%d] = %d ", i, lineArr[i]);
    }
    printf("\n");

    updateStudentVeriFile(lineArr, dersKodu, puan);

}


int binarySearch(int arrOgrNo[], int arrLine[], int ogrNo, int count){

    printf("\ncount(ogrNoArr): %d\n",count);

    if (count%2==0)
        count--;

    int low = 0;
    int high = count;

    while(low <= high){
        int index = low+(high-low)/2;
        if(arrOgrNo[index] == ogrNo){
            printf("return index: %d\n",arrLine[index]);
            printf("founded ogr: %d\n",arrOgrNo[index]);
            return arrLine[index];
        }
        else if(arrOgrNo[index] < ogrNo){
            low   = index+1;
        }
        else{
            high = index-1;
        }
    }
    printf("yok\n");
    return -1;
};

