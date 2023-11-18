#include <stdio.h>
#include <sys/stat.h>
#include "../../ADT/wordmachine/wordmachine.h"
#include "../pengguna/pengguna.h"
#include "../inisialisasi/inisialisasi.h"
#include "../profil/profil.h"
#include "../kicau/kicau.h"
#include "../teman/teman.h"
#include "../draf/draf.h"
#include "../balasan/balasan.h"
#include "../permintaan/permintaan.h"

extern boolean isLogin;
extern boolean isClosed;
extern Word currentWord;
extern ListPengguna ListUser;
extern AdjMatrix matrixPertemanan;

void StoreDataPengguna(int n){
    Word nama,password,bio,no,weton,isPub,pic,publik;
    boolean pub;
    Pengguna P;
    MatrixFoto M;
    strToWord("Publik", &publik);
    int i;
    for(i = 0; i < n; i++){
        // nama
        ReadLine(1);
        nama = currentWord;
        // password
        ReadLine(1);
        password = currentWord;
        // bio
        ReadLine(1);
        bio = currentWord;
        // No.HP
        ReadLine(1);
        no = currentWord;
        // Weton
        ReadLine(1);
        weton = currentWord;
        // isPublic
        ReadLine(1);
        isPub = currentWord;
        if(isKataEqual(isPub,publik)){
            pub = true;
        }else{
            pub = false;
        }
        // ProfPic
        ReadLineWithEnter(5);
        pic = currentWord;
        M = ReadFotoFile(pic);
        CreatePenggunaFile(&P,nama,password,bio,no,weton,pub,M);
        insertLast(&ListUser,P);
    }
}

void loadPengguna(char *path){
    int n,NPermintaan;
    Word dataPengguna,pertemanan,permintaan;
    printf("%s\n",path);
    ReadFromFile(path);
    printWord(currentWord);
    printf("\n");
    n = wordToInteger(currentWord);
    StoreDataPengguna(n);
    
    // // Pertemanan
    // ReadLine(n);
    // pertemanan = currentWord;
    // CreateAdjMatrixFile(&matrixPertemanan,pertemanan,n);
    // ReadLine(1);
    // // Permintaan
    // NPermintaan = wordToInteger(currentWord);
    // ReadLine(NPermintaan);
    // permintaan =currentWord;
    // MatrixPermintaanFile(permintaan, &matrixPermintaan, NPermintaan);
    // printList(ListUser);

}
void loadKicauan(char *path){
    // printf("%s",path);
}
void loadBalasan(char *path){
    // printf("%s",path);
}
void loadDraf(char *path){
    // printf("%s",path);
}
void loadUtas(char *path){
    // printf("%s",path);
}


void load(Word dir){
    char *directory = dir.TabWord;
    // char *file = "text.txt";
    char path[256];
    const char *fileNames[] = {"pengguna.config", "kicauan.config", "balasan.config", "draf.config", "utas.config"};
    // printf("%s",path);
    for(int i=0;i<5;++i){
        snprintf(path,sizeof(path), "%s/%s/%s", "data", directory,fileNames[i]);
        switch (i){
            case 0:
                loadPengguna(path);
                break;
            case 1:
                loadKicauan(path);
                break;
            case 2:
                loadBalasan(path);
                break;
            case 3:
                loadDraf(path);
                break;
            case 4:
                loadUtas(path);
                break;
            
            default:
                break;
        }
    }
}

boolean isDirectoryExists(char *path) {
    struct stat fileStat;

    if (stat(path, &fileStat) == 0) {
        if (S_ISDIR(fileStat.st_mode)) {
            return true; // Direktori ada
        }
    }
    
    return false; // Direktori tidak ada
}

void loadfirst(){
    Word directory;
    boolean succes=false;
    while(!succes){
        printf("Silahkan masukan folder konfigurasi untuk dimuat: ");
        ReadWord();
        directory = currentWord;
        if(isDirectoryExists(directory.TabWord)){// udah ada
            load(directory);
            succes = true;
        }else{
            printf("Tidak ada folder yang dimaksud!");
        }
    }
}
void Muat(){
    Word directory;
    printf("Masukkan nama folder yang hendak dimuat.");
    ReadWord();
    directory = currentWord;
    if(isDirectoryExists(directory.TabWord)){// udah ada
        load(directory);
    }else{
        printf("Tidak ada folder yang dimaksud!");
    }
}


// int main(){
//     Word x;
//     CreateWord(&x);
//     ReadWord();
//     x = currentWord;
//     load(x);
//     // printWord(x);
// }


