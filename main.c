#include <stdio.h>
#include <stdlib.h>


int enterChoice(void);

void indexDosyasiOlustur(FILE *fp);

void veriDosyasiOlustur(FILE *fp);

int kayitEkle(FILE *fPtr);

void indexEkle(FILE *indexFPtr, int ogrNo, int line);

int* getStudentIndexFile(FILE *indexFPtr, int ogrNo);

struct kayit getStudentVeriFile(FILE *binaryFPtr, int lineArr[]);

void veriDosyasiniGoster(FILE *readPtr);

void indexDosyasiniGoster(FILE *readPtr);

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
                int tmpOgrNo = kayitEkle(binaryfPtr);
                int line = ftell(binaryfPtr) / sizeof(struct kayit);
                indexEkle(indexfPtr, tmpOgrNo, line);
                break;
            case 2:
                int ogrNo;
                printf("Enter ogrNo\n? ");
                scanf("%d", &ogrNo);
                kayitBul(binaryfPtr, indexfPtr, ogrNo);
                break;
            case 3:
                break;
            case 4:
                kayitGuncelle(binaryfPtr);
                break;
            case 5:
                veriDosyasiniGoster(binaryfPtr);
                break;
            case 6:
                indexDosyasiniGoster(indexfPtr);
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

int kayitEkle(FILE *fPtr) {
    struct kayit client = {0, 0, 0.0, "", ""};
    fseek(fPtr, 0,
          SEEK_END);
    printf("Enter ogrNo, dersKodu, puan, ogrName, dersName\n? ");
    scanf("%d%d%lf%s%s", &client.ogrNo, &client.dersKodu, &client.puan,
          &client.ogrName, &client.dersName);
    fwrite(&client, sizeof(struct kayit), 1, fPtr);
    return client.ogrNo;
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



void veriDosyasiniGoster(FILE *readPtr) {
    struct kayit client = {0, 0, 0.0, "", ""};
    int lineSize = 0;
    rewind(readPtr);
    printf("%-9s%-10s%-10s%-16s%16s\n",
           "ogrNo", "dersKodu", "Puan", "ogrName", "dersName");
    while (!feof(readPtr)) {
        fread(&client, sizeof(struct kayit), 1, readPtr);
        if (!feof(readPtr))
            printf("%-9d%-10d%-10.2f%-16s%16s\n",
                   client.ogrNo, client.dersKodu,
                   client.puan, client.ogrName, client.dersName);
        lineSize++;
    }

    printf("okunan satır sayısı : %d", lineSize);
}

void indexDosyasiniGoster(FILE *indexFPtr) {
    struct kayit_index kayitIndex = {-1, -1};
    int lineSize = 0;
    rewind(indexFPtr);
    printf("%-9s%-20s%\n",
           "ogrNo", "satir");
    while (!feof(indexFPtr)) {
        fread(&kayitIndex, sizeof(struct kayit_index), 1, indexFPtr);
        if (!feof(indexFPtr)) {

            if (kayitIndex.ogrNoAnahtar != -1) {

                printf("%-9d%-20d", kayitIndex.ogrNoAnahtar, kayitIndex.line);

            }
        }
        printf("\n");
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

