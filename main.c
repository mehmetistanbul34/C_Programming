#include <stdio.h>

int enterChoice(void);
void indexDosyasiOlustur(FILE *fp);
void veriDosyasiOlustur(FILE *fp);
int kayitEkle(FILE *fPtr);
void indexEkle(FILE *fPtr,int ogrNo, int line);
struct kayit_index getStudentIndexFile(FILE *fPtr,int ogrNo);
struct kayit getStudentVeriFile(FILE *binaryFPtr,int arr[],int arrSize);
void veriDosyasiniGoster(FILE *readPtr);
void indexDosyasiniGoster(FILE *readPtr);
void indexDosyasiniSil(FILE *fPtr);
void veriDosyasiniSil(FILE *fPtr);
void kayitBul(FILE *binaryFPtr,FILE *indexFPtr, int ogrNo);
void kayitGuncelle(FILE *fPtr);
void newRecord(FILE *fPtr, int accountNum, int line);

struct kayit {
    int ogrNo;
    int dersKodu;
    double puan;
    char ogrName[100];
    char dersName[100];
};

struct kayit_index {
    int ogrNoAnahtar;
    int lineArr[20];
};

void main(){
    FILE *binaryfPtr = fopen("../binary_data.bin", "r+b");
    FILE *indexfPtr = fopen("../text_data.txt", "r+");
    int choice;
    if (binaryfPtr == NULL || indexfPtr  == NULL) {
        indexDosyasiOlustur(indexfPtr);
        veriDosyasiOlustur(binaryfPtr);
        binaryfPtr = fopen("../binary_data.bin", "r+b");
        indexfPtr = fopen("../text_data.txt", "r+");
    }
        while ((choice = enterChoice()) != 9) {
            switch (choice) {
                case 0:
                    indexDosyasiOlustur(indexfPtr);

                    break;
                case 1:
                    int tmpOgrNo = kayitEkle(binaryfPtr);
                    int line = ftell(binaryfPtr)/sizeof (struct kayit);
                    indexEkle(indexfPtr, tmpOgrNo,line);
                    //newRecord(indexfPtr, tmpOgrNo, line);
                    break;
                case 2:
                    int ogrNo;
                    printf("Enter ogrNo\n? ");
                    scanf("%d", &ogrNo);
                    kayitBul(binaryfPtr,indexfPtr, ogrNo);
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

void indexDosyasiOlustur(FILE *fp){
    if ( (fp = fopen("../text_data.txt","w"))==NULL) {
        printf("text_data.txt Dosya açılamadı");
    } else{
        printf("text_data.txt Dosya acildi");
    }
}

void veriDosyasiOlustur(FILE *fp){
    if ( (fp = fopen("../binary_data.bin","wb"))==NULL) {
        printf("binary_data.bin Dosya açılamadı");
    } else{
        printf("binary_data.bin Dosya acildi");
    }
}

int kayitEkle(FILE *fPtr) {
    struct kayit client = {0, 0,0.0,"", ""};
    fseek(fPtr, 0,
          SEEK_END);
    printf("Enter ogrNo, dersKodu, puan, ogrName, dersName\n? ");
    scanf("%d%d%lf%s%s", &client.ogrNo, &client.dersKodu,&client.puan,
          &client.ogrName,&client.dersName);
    fwrite(&client, sizeof(struct kayit), 1, fPtr);
    return client.ogrNo;
}

void indexEkle(FILE *fPtr,int ogrNo, int line) {
    struct kayit_index kayitIndex = getStudentIndexFile(fPtr,ogrNo);

    if(kayitIndex.ogrNoAnahtar==-1) {
        kayitIndex.ogrNoAnahtar = ogrNo;
        kayitIndex.lineArr[0] = line;
        fseek(fPtr, (ogrNo - 1) * sizeof(struct kayit_index),SEEK_SET);
        fwrite(&kayitIndex, sizeof(struct kayit_index), 1, fPtr);
    }
    else{
        for (int i=0;i<20;i++){
            if(kayitIndex.lineArr[i]==-1) {
                kayitIndex.lineArr[i] = line;
                break;
            }
        }

        int count=1;
        struct kayit_index tmpKayitIndex;

        tmpKayitIndex.ogrNoAnahtar = -1;
        for (int i=0;i<20;i++){
            tmpKayitIndex.lineArr[i] = -1;
        }
        rewind(fPtr);
        while (!feof(fPtr)) {

            fread(&tmpKayitIndex, sizeof(struct kayit_index), 1, fPtr);

            if (!feof(fPtr) && tmpKayitIndex.ogrNoAnahtar==ogrNo){
                fseek(fPtr, (count - 1) * sizeof(struct kayit_index),
                      SEEK_SET);
                fwrite(&kayitIndex, sizeof(struct kayit_index), 1, fPtr);
                break;
            }
            count++;

        }

    }
}

struct kayit_index getStudentIndexFile(FILE *fPtr,int ogrNo) {

    struct kayit_index kayitIndex;

    kayitIndex.ogrNoAnahtar = -1;
    for (int i=0;i<20;i++){
        kayitIndex.lineArr[i] = -1;
    }

    fseek(fPtr, (ogrNo - 1) * sizeof(struct kayit_index),
          SEEK_SET);
    fread(&kayitIndex, sizeof(struct kayit_index), 1, fPtr);

    if(kayitIndex.ogrNoAnahtar!=ogrNo){
        kayitIndex.ogrNoAnahtar = -1;
        for (int i=0;i<20;i++){
            kayitIndex.lineArr[i] = -1;
        }
    }

    return kayitIndex;
}

struct kayit getStudentVeriFile(FILE *binaryFPtr,int arr[],int arrSize) {

    struct kayit kayit;
    for(int i=0;i<arrSize;i++) {
        fseek(binaryFPtr, (arr[i] - 1) * sizeof(struct kayit),
              SEEK_SET);
        fread(&kayit, sizeof(struct kayit), 1, binaryFPtr);
        break;
    }

    return kayit;
}

void veriDosyasiniGoster(FILE *readPtr) {
    struct kayit client = {0,0,0.0, "", ""};
    rewind(readPtr);
    printf("%-9s%-10s%-10s%-16s%16s\n",
           "ogrNo", "dersKodu", "Puan", "ogrName","dersName");
    while (!feof(readPtr)) {
        fread(&client, sizeof(struct kayit), 1, readPtr);
        if (!feof(readPtr))
            printf("%-9d%-10d%-10.2f%-16s%16s\n",
                   client.ogrNo, client.dersKodu,
                   client.puan, client.ogrName,client.dersName);
    }
}

void indexDosyasiniGoster(FILE *readPtr) {
    struct kayit_index kayitIndex = {-1,-1};
    rewind(readPtr);
    printf("%-9s%-20s%\n",
           "ogrNo", "satir");
    while (!feof(readPtr)) {
        fread(&kayitIndex, sizeof(struct kayit_index), 1, readPtr);
        if (!feof(readPtr)) {

            if(kayitIndex.ogrNoAnahtar!=-1 && kayitIndex.ogrNoAnahtar!=0) {

                printf("%-9d%", kayitIndex.ogrNoAnahtar);

                for (int m = 0; m < 20; m++) {
                    if (kayitIndex.lineArr[m] != -1) {
                        printf("%d-", kayitIndex.lineArr[m]);
                    }
                }
            }
        }
        printf("\n");
    }
}

void indexDosyasiniSil(FILE *fPtr) {
    fclose(fPtr);
    if (remove("../text_data.txt")==0)
        printf("Dosya başarılı bir şekilde silindi!");
    else
        perror("Dosya silme hatası");
}

void veriDosyasiniSil(FILE *fPtr) {
    fclose(fPtr);
    if (remove("../binary_data.bin")==0)
        printf("Dosya başarılı bir şekilde silindi!");
    else
        perror("Dosya silme hatası");
}

void kayitBul(FILE *binaryFPtr,FILE *indexFPtr, int ogrNo) {

    struct  kayit_index kayitIndex = getStudentIndexFile(indexFPtr,ogrNo);

    printf("log test1 %d\n",kayitIndex.ogrNoAnahtar);

    int arrSize = 0;

    while (kayitIndex.lineArr[arrSize]!=-1){
        arrSize++;
    }

    struct kayit foundedKayit = getStudentVeriFile(binaryFPtr,kayitIndex.lineArr,arrSize);
    printf("Founded ogrNo : %d   dersKodu : %d  puan : %lf   ogrName : %s  dersName : %s\n",
           foundedKayit.ogrNo,foundedKayit.dersKodu,foundedKayit.puan,foundedKayit.ogrName,foundedKayit.dersName);

}

void kayitGuncelle(FILE *fPtr) {
    int ogrNo;
    int count=1;
    struct kayit client = {-1,-1,0.0, "", ""};
    printf("Öğrenci Numarası Girin : ");
    scanf("%d", &ogrNo);
    rewind(fPtr);
    while (!feof(fPtr)) {

        fread(&client, sizeof(struct kayit), 1, fPtr);

        if (!feof(fPtr) && client.ogrNo==ogrNo){
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

void newRecord(FILE *fPtr, int accountNum, int line){
    struct kayit_index client = {0, 0};

    fseek(fPtr, (accountNum - 1) * sizeof(struct kayit_index),
          SEEK_SET);
    fread(&client, sizeof(struct kayit_index), 1, fPtr);
    if (client.ogrNoAnahtar != 0)
        printf("Account #%d already contains information.\n",
               client.ogrNoAnahtar);
    else {
        client.ogrNoAnahtar = accountNum;
        client.lineArr[0] = line;
        fseek(fPtr, (client.ogrNoAnahtar - 1) * sizeof(struct kayit_index),
              SEEK_SET);
        fwrite(&client, sizeof(struct kayit_index), 1, fPtr);
    }
}