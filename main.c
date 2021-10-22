#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int enterChoice(void);

void indexDosyasiOlustur(FILE *fp);

void veriDosyasiOlustur(FILE *fp);


void kayitEkle(FILE *binaryFPtr,FILE *indexFPtr);

int indexSil(FILE *indexFPtr, int ogrNo);

void kayitSil(FILE *binaryFPtr, int line);

void indexEkle(FILE *indexFPtr, int ogrNo, int line);

int* getStudentIndexFile(FILE *indexFPtr, int ogrNo);

struct kayit getStudentVeriFile(FILE *binaryFPtr, int lineArr[]);

void veriDosyasiniGoster();

void indexDosyasiniGoster();

void indexDosyasiniSil(FILE *fPtr);

void veriDosyasiniSil(FILE *fPtr);

void kayitBul(FILE *binaryFPtr, FILE *indexFPtr, int ogrNo);

void kayitGuncelle(FILE *fPtr);


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

void main() {
    FILE *binaryfPtr = fopen("../binary_data.bin", "r+b");
    FILE *indexfPtr = fopen("../text_data.txt", "r+");
    int choice;
    if (binaryfPtr == NULL || indexfPtr == NULL) {
        indexDosyasiOlustur(indexfPtr);
        veriDosyasiOlustur(binaryfPtr);
        binaryfPtr = fopen("../binary_data.bin", "r+b");
        indexfPtr = fopen("../text_data.txt", "r+");
        printf("index ve binary dosyalar olusturuldu\n");
    }
    while ((choice = enterChoice()) != 9) {
        switch (choice) {
            case 0:
                indexDosyasiOlustur(indexfPtr);
                break;
            case 1:
                kayitEkle(binaryfPtr,indexfPtr);
                break;
            case 2:
                int ogrNo;
                printf("Enter ogrNo\n? ");
                scanf("%d", &ogrNo);
                kayitBul(binaryfPtr, indexfPtr, ogrNo);
                break;
            case 3:
                int delOgrNo;
                printf("Enter ogrNo\n? ");
                scanf("%d", &delOgrNo);
                int removeLine = indexSil(indexfPtr, delOgrNo);
                kayitSil(binaryfPtr,removeLine);
                break;
            case 4:
                kayitGuncelle(binaryfPtr);
                break;
            case 5:
                veriDosyasiniGoster();
                break;
            case 6:
                indexDosyasiniGoster();
                break;
            case 7:
                indexDosyasiniSil(indexfPtr);
                veriDosyasiniSil(binaryfPtr);
                break;
        }
    }
    fclose(binaryfPtr);

}

int enterChoice(void) {
    int menuChoice;
    printf("\nSecenekler\n"
           "0 - Index Dosyasi Olustur\n"
           "1 - Kayit Ekle\n"
           "2 - Kayit Bul\n"
           "3 - Kayit Sil\n"
           "4 - Kayit Guncelle\n"
           "5 - Veri Dosyasini Goster\n"
           "6 - Index Dosyasini Goster\n"
           "7 - Index Dosyasini Sil\n"
           "9 - Cikis\n ");
    scanf("%d", &menuChoice);
    return menuChoice;
}

void indexDosyasiOlustur(FILE *fp) {
    if ((fp = fopen("../text_data.txt", "w")) == NULL) {
        printf("text_data.txt Dosya açılamadı");
    } else {
        printf("text_data.txt Dosya acildi");
    }
}

void veriDosyasiOlustur(FILE *fp) {
    if ((fp = fopen("../binary_data.bin", "wb")) == NULL) {
        printf("binary_data.bin Dosya açılamadı");
    } else {
        printf("binary_data.bin Dosya acildi");
    }
}

void kayitEkle(FILE *binaryFPtr,FILE *indexFPtr) {

    printf("otomatik data eklemesi basladı... ");
    for(int i=1;i<4;i++){
        for (int j=1;j<4;j++){
            struct kayit client = {0, 0, 0.0, "", ""};
            client.ogrNo = i;
            client.dersKodu = j;
            client.puan = 5*i*j;
            client.ogrName[0]='o';
            client.ogrName[1]='g';
            client.ogrName[2]='r';
            client.ogrName[3]=i;
            client.dersName[0]='d';
            client.dersName[1]='e';
            client.dersName[2]='r';
            client.dersName[3]='s';
            client.dersName[4]=i*j;
            fseek(binaryFPtr, 0,
                  SEEK_END);
            fwrite(&client, sizeof(struct kayit), 1, binaryFPtr);
            int line = ftell(binaryFPtr) / sizeof(struct kayit);
            indexEkle(indexFPtr,client.ogrNo,line);
        }
    }
}

void indexEkle(FILE *indexFPtr, int ogrNo, int line) {

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
    while (!feof(indexFPtr) && count<fileIndexSize) {
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

}



int indexSil(FILE *indexFPtr, int ogrNo) {

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
    while (!feof(indexFPtr) && count<fileIndexSize) {
        struct kayit_index item, item2;
        fseek(indexFPtr, count * sizeof(struct kayit_index), SEEK_SET);
        fread(&item, sizeof(struct kayit_index), 1, indexFPtr);
        if (item.ogrNoAnahtar > ogrNo && swapTrue==-1) {
            fseek(indexFPtr, (count-1) * sizeof(struct kayit_index), SEEK_SET);
            fread(&item2, sizeof(struct kayit_index), 1, indexFPtr);
            swapTrue = 0;
            removeLine = item2.line;
        }

        if (swapTrue == 0) {
            fseek(indexFPtr, (count-1) * sizeof(struct kayit_index), SEEK_SET);
            fwrite(&item, sizeof(struct kayit_index), 1, indexFPtr);
        }
        count++;
    }

    fseek(indexFPtr, (count-1) * sizeof(struct kayit_index), SEEK_SET);
    struct kayit_index bosKayit = {-1,-1};
    fwrite(&bosKayit, sizeof(struct kayit_index), 1, indexFPtr);

    return removeLine;

}

void kayitSil(FILE *binaryFPtr, int line) {
    line--;
    printf("function : kayitsil line : %d\n", line);

    int count = 0;
    rewind(binaryFPtr);
    while (!feof(binaryFPtr)) {
        struct kayit item;
        fread(&item, sizeof(struct kayit), 1, binaryFPtr);
        if (!feof(binaryFPtr))
            printf("item ogrNo: %d dersKodu : %d puan: %d\n", item.ogrNo, item.dersKodu,item.puan);
        count++;
    }
    printf("count : %d\n", count);

    int fileIndexSize = count;

    int swapTrue = -1;
    count = 0;
    rewind(binaryFPtr);
    fileIndexSize--;
    while (!feof(binaryFPtr) && count<fileIndexSize) {
        struct kayit item;
        fseek(binaryFPtr, count * sizeof(struct kayit), SEEK_SET);
        fread(&item, sizeof(struct kayit), 1, binaryFPtr);
        if (count > line && swapTrue==-1 ) {
            swapTrue = 0;
        }

        if (swapTrue == 0) {
            fseek(binaryFPtr, (count-1) * sizeof(struct kayit), SEEK_SET);
            fwrite(&item, sizeof(struct kayit), 1, binaryFPtr);
        }
        count++;
    }

    fseek(binaryFPtr, (count-1) * sizeof(struct kayit), SEEK_SET);
    struct kayit bosKayit = {-1,-1,-1,"",""};
    fwrite(&bosKayit, sizeof(struct kayit), 1, binaryFPtr);

}


int* getStudentIndexFile(FILE *indexFPtr, int ogrNo) {

    struct kayit_index item = {-1,-1};
    struct kayit_index founded = {-1,-1};
    rewind(indexFPtr);
    int count = 0;
    while (!feof(indexFPtr)) {
        fread(&founded, sizeof(struct kayit_index), 1, indexFPtr);
        if (founded.ogrNoAnahtar == ogrNo) {
            count++;
        }
    }
    rewind(indexFPtr);
    int *lineArr = (int*) malloc(count*sizeof (int));
    count = 0;
    while (!feof(indexFPtr)) {
        fread(&founded, sizeof(struct kayit_index), 1, indexFPtr);
        if (founded.ogrNoAnahtar == ogrNo) {
            lineArr[count] = founded.line;
            count++;
        }
    }

    return lineArr;
}



void veriDosyasiniGoster() {
    FILE *readPtr = fopen("../binary_data.bin","r");
    struct kayit client = {0, 0, 0.0, "", ""};
    int lineSize = 0;
    rewind(readPtr);
    printf("%-9s%-10s%-10s%-16s%16s\n",
           "ogrNo", "dersKodu", "Puan", "ogrName", "dersName");
    while (!feof(readPtr)) {
        fread(&client, sizeof(struct kayit), 1, readPtr);
        if (!feof(readPtr) && client.ogrNo!=-1)
            printf("%-9d%-10d%-10.2f%-16s%16s\n",
                   client.ogrNo, client.dersKodu,
                   client.puan, client.ogrName, client.dersName);
        lineSize++;
    }

    printf("okunan satır sayısı : %d", lineSize);
}

void indexDosyasiniGoster() {
    FILE *indexFPtr = fopen("../text_data.txt","r");
    struct kayit_index kayitIndex = {-1, -1};
    int lineSize = 0;
    rewind(indexFPtr);
    printf("%-9s%-20s%\n",
           "ogrNo", "satir");
    while (!feof(indexFPtr)) {
        fread(&kayitIndex, sizeof(struct kayit_index), 1, indexFPtr);
        if (!feof(indexFPtr) && kayitIndex.ogrNoAnahtar!=-1) {
                printf("%-9d%-20d\n", kayitIndex.ogrNoAnahtar, kayitIndex.line);
        }
        lineSize++;
    }

    printf("okunan satır sayısı : %d", lineSize);
}

void indexDosyasiniSil(FILE *fPtr) {
    fclose(fPtr);
    if (remove("../text_data.txt") == 0)
        printf("text_data dosyasi silindi\n");
    else
        perror("Dosya zaten yok");
}

void veriDosyasiniSil(FILE *fPtr) {
    fclose(fPtr);
    if (remove("../binary_data.bin") == 0)
        printf("binary_data dosyasi silindi\n");
    else
        perror("Dosya zaten yok");
}

void kayitBul(FILE *binaryFPtr, FILE *indexFPtr, int ogrNo) {

    int *foundedIndexs = getStudentIndexFile(indexFPtr, ogrNo);


    struct kayit foundedKayit = getStudentVeriFile(binaryFPtr, foundedIndexs);
    printf("Founded ogrNo : %d   dersKodu : %d  puan : %lf   ogrName : %s  dersName : %s\n",
           foundedKayit.ogrNo, foundedKayit.dersKodu, foundedKayit.puan, foundedKayit.ogrName, foundedKayit.dersName);

}

struct kayit getStudentVeriFile(FILE *binaryFPtr, int kayitIndexs[]) {

    int count=0;

    while(kayitIndexs[count]!=NULL)
        count++;

    struct kayit kayit = {-1,-1,-1,"",""};

    int i=0;
    while(i<count) {
        fseek(binaryFPtr, (kayitIndexs[0] - 1) * sizeof(struct kayit),
              SEEK_SET);
        fread(&kayit, sizeof(struct kayit), 1, binaryFPtr);
        printf("Founded ogrNo : %d   dersKodu : %d  puan : %lf   ogrName : %s  dersName : %s\n",
               kayit.ogrNo, kayit.dersKodu, kayit.puan, kayit.ogrName, kayit.dersName);
        break;
    }
    return kayit;
}


void kayitGuncelle(FILE *fPtr) {
    int ogrNo;
    int count = 1;
    struct kayit client = {-1, -1, 0.0, "", ""};
    printf("Öğrenci Numarası Girin : ");
    scanf("%d", &ogrNo);
    rewind(fPtr);
    while (!feof(fPtr)) {

        fread(&client, sizeof(struct kayit), 1, fPtr);

        if (!feof(fPtr) && client.ogrNo == ogrNo) {
            printf("Enter puan:\n? ");
            scanf("%lf", &client.puan);
            fseek(fPtr, (count - 1) * sizeof(struct kayit),
                  SEEK_SET);
            fwrite(&client, sizeof(struct kayit), 1, fPtr);
            break;
        }
        count++;

    }
}

