#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "simpan.h"

// User
extern ListPengguna ListUser;
// Kicauan
extern ListKicauan ListTweet;
// Balasan
extern ListBalasan ListReply;
// Draf
extern StackDraf SDraf;

void Simpan()
/* Melakukan prosedur simpan sesuai dengan spesifikasi */
{
    Word parentPath;
    strToWord("data/", &parentPath);
    printf("Masukkan nama folder penyimpanan: ");
    ReadWord();
    Word relativePath = currentWord;
    Word completePath = concatWord(parentPath, relativePath);
    char *path;
    // printWord(completePath);
    wordToString(completePath, &path);
    struct stat st = {0};
    if (stat(path, &st) == -1)
    {
        printf("Belum terdapat ");
        printWord(relativePath);
        printf(". Akan dilakukan pembuatan ");
        printWord(relativePath);
        printf(" terlebih dahulu.\n");
        if (mkdir(path, 0777) == 0)
        {
            printf("Directory created: %s\n", path);
            printf("Mohon tunggu... \n");
            printf("1...\n");
            printf("2...\n");
            printf("3...\n");
            printWord(relativePath);
            printf(" telah berhasil dibuat\n");
        }
        else
        {
            perror("mkdir");
        }
    }
    SimpanPengguna(completePath);
    simpanKicauan(completePath);
    // simpanBalasan(completePath);
    simpanDraf(completePath);
}

void printWordToFile(Word w, FILE *filename)
/* Menulis w ke dalam file diakhiri dengan \n */
{
    int i;
    for (i = 0; i < w.Length; i++)
    {
        fprintf(filename, "%c", w.TabWord[i]);
    }
    fprintf(filename, "\n");
}

void SimpanPengguna(Word path)
/* Menyimpan data pengguna ke file pengguna.config sesuai dengan spesifikasi*/
{
    int n, i;
    FILE *userconfig;
    char *confPath;
    Word penggunaConfig;
    strToWord("/pengguna.config", &penggunaConfig);
    Word configPath = concatWord(path, penggunaConfig);
    wordToString(configPath, &confPath);
    // printWord(configPath);
    userconfig = fopen(confPath, "w");
    n = length(ListUser);
    fprintf(userconfig, "%d\n", n);
    for (i = 0; i < length(ListUser); i++)
    {
        printWordToFile(Nama(ELMT(ListUser, i)), userconfig);
        printWordToFile(Password(ELMT(ListUser, i)), userconfig);
        printWordToFile(Bio(Profil(ELMT(ListUser, i))), userconfig);
        printWordToFile(NoHP(Profil(ELMT(ListUser, i))), userconfig);
        printWordToFile(Weton(Profil(ELMT(ListUser, i))), userconfig);
        if (isPublic(Profil(ELMT(ListUser, i))))
        {
            fprintf(userconfig, "Publik\n");
        }
        else
        {
            fprintf(userconfig, "Privat\n");
        }
        int a, b;
        for (a = 0; a < 5; a++)
        {
            for (b = 0; b < 5; b++)
            {
                fprintf(userconfig, "%c ", Warna(PIXEL(Profpic(Profil(ELMT(ListUser, i))), a, b)));
                fprintf(userconfig, "%c", Karakter(PIXEL(Profpic(Profil(ELMT(ListUser, i))), a, b)));
                if (b < 4)
                {
                    fprintf(userconfig, " ");
                }
            }
            fprintf(userconfig, "\n");
        }
    }

    // SIMPAN MATRIKS PERTEMANAN DAN PERMINTAAN PERTEMANAN
    fclose(userconfig);
}

void simpanKicauan(Word path)
/* Menyimpan data kicauan ke file kicauan.config sesuai dengan spesifikasi*/
{
    Word kicauanConfig, configPath;
    char *fconfPath;
    FILE *fconfKicauan;
    strToWord("/kicauan.config", &kicauanConfig);
    configPath = concatWord(path, kicauanConfig);
    wordToString(configPath, &fconfPath);
    fconfKicauan = fopen(fconfPath, "w");

    int count;
    count = countKicauan(ListTweet);
    fprintf(fconfKicauan, "%d\n", count);

    int i;
    for (i = 0; i < count; i++)
    {
        fprintf(fconfKicauan, "%d\n", idKicau(ELMTKicau(ListTweet, i)));
        printWordToFile(textKicau(ELMTKicau(ListTweet, i)), fconfKicauan);
        fprintf(fconfKicauan, "%d\n", likeKicau(ELMTKicau(ListTweet, i)));
        printWordToFile(authorKicau(ELMTKicau(ListTweet, i)), fconfKicauan);
        int day = Day(datetimeKicau(ELMTKicau(ListTweet, i)));
        int month = Month(datetimeKicau(ELMTKicau(ListTweet, i)));
        int year = Year(datetimeKicau(ELMTKicau(ListTweet, i)));
        int hour = Hour(Time(datetimeKicau(ELMTKicau(ListTweet, i))));
        int minute = Minute(Time(datetimeKicau(ELMTKicau(ListTweet, i))));
        int second = Second(Time(datetimeKicau(ELMTKicau(ListTweet, i))));
        fprintf(fconfKicauan, "%02d/%02d/%d %02d:%02d:%02d\n", day, month, year, hour, minute, second);
    }

    fclose(fconfKicauan);
}

void writeTree(FILE *filename, TreeNode *node, int depth)
/* Fungsi untuk menuliskan pohon dengan banyak cabang secara rekursif */
{
    if (node == NULL)
    {
        return;
    }

    if (depth != 0)
    {
        Balasan reply = ROOT(*node);
        fprintf(filename, "%d %d\n", idParentBalas(reply), idBalas(reply));
        printWordToFile(textBalas(reply), filename);
        printWordToFile(authorBalas(reply), filename);
        int day = Day(datetimeBalas(reply));
        int month = Month(datetimeBalas(reply));
        int year = Year(datetimeBalas(reply));
        int hour = Hour(Time(datetimeBalas(reply)));
        int minute = Minute(Time(datetimeBalas(reply)));
        int second = Second(Time(datetimeBalas(reply)));
        fprintf(filename, "%02d/%02d/%d %02d:%02d:%02d\n", day, month, year, hour, minute, second);
    }

    writeTree(filename, firstChild(node), depth + 1); // print firstchildnya
    writeTree(filename, nextSibling(node), depth);    // print nextnya
}

// void simpanBalasan(Word path)
// /* Menyimpan data balasan ke file balasan.config sesuai dengan spesifikasi*/
// {
//     Word balasanConfig, configPath;
//     char *fconfPath;
//     FILE *fconfbalasan;
//     strToWord("/balasan.config", &balasanConfig);
//     configPath = concatWord(path, balasanConfig);
//     wordToString(configPath, &fconfPath);
//     fconfbalasan = fopen(fconfPath, "w");

//     int count;
//     count = countKicauBalasan(ListReply);
//     fprintf(fconfbalasan, "%d\n", count);

//     int i;
//     for (i = 0; i < NEFFBalas(ListReply); i++)
//     {
//         if (count(ELMT(ListReply, i)) > 0)
//         {
//             // write id kicau
//             fprintf(fconfbalasan, "%d\n", i + 1);
//             // write jumlah balasan
//             fprintf(fconfbalasan, "%d\n", count(ELMT(ListReply, i)));
//             // write balasan in tree
//             writeTree(fconfbalasan, &content(ELMT(ListReply, i)), 0);
//         }
//     }

//     fclose(fconfbalasan);
// }

void simpanDraf(Word path){
    Word drafConfig, configPath;
    char *fconfPath;
    FILE *fconfdraf;
    strToWord("/draf.config", &drafConfig);
    configPath = concatWord(path, drafConfig);
    wordToString(configPath, &fconfPath);
    fconfdraf = fopen(fconfPath, "w");

    int countDraf = 0;
    for(int i=0; i<ListUser.length;++i ){
        printWord(Nama(ELMT(ListUser,i)));
        countDraf += CountDraftUser(SDraf,Nama(ELMT(ListUser,i)));
    }
    printf("%d", countDraf);
}