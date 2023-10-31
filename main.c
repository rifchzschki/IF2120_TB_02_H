#include <stdio.h>
#include "ADT/pengguna/pengguna.h"
#include "ADT/inisialisasi/inisialisasi.h"
#include "ADT/wordmachine/wordmachine.h"
#include "ADT/profil/profil.h"

ListPengguna ListUser;
int main()
{
    CreateList(&ListUser);
    init();
    Word cmd;
    while (!isClosed)
    {
        printf(">> ");
        ReadWord();
        cmd = currentWord;
        prosesCmd(cmd);
    }
}