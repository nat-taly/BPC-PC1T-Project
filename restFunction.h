// POUŽITÉ KNIHOVNY
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// Vytváří CLION
#ifndef PROJEKT_30_RESTFUNCTION_H
#define PROJEKT_30_RESTFUNCTION_H
#endif //PROJEKT_30_RESTFUNCTION_H

// DEFINOVÁNÍ BARVY PRO ZVÝRAZNĚNÍ VÝPISU DATABÁZE
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// VELIKOST BUFFERU
#define BUFFSIZE 500

// Struktura restaurace a komentáře
struct restaurace{
    int  id;
    char nazev[BUFFSIZE];
    char mesto[BUFFSIZE];
    float pocetkom;
    float soucethodkom;
    float celkovehodnoceni;
    char jidlo[BUFFSIZE];

    struct restaurace* dalsi;
};

struct komentar{
    int id_restaurace;
    float hodnoceni;
    char komentar[BUFFSIZE];

    struct komentar* dalsikom;
};


// PŘIDÁNÍ RESTAURACE, VSTUP OD UŽIVATELE
void restAdd();

// ODSTRANĚNÍ RESTAURACE, VSTUP OD UŽIVATELE
void restDelete();

// ZOBRAZENÍ RESTAURACÍ
void restShow();

// EDITACE RESTAURACE
void restEdit();

// ZOBRAZENÍ KOMENTÁŘŮ
void showComment();

// PŘIDÁNÍ KOMENTÁŘE, VSTUP OD UŽIVATELE
void addComment();

// PŘIDÁNÍ JÍDLA K RESTAURACI
void addMenu();

// ŘAZENÍ RESTAURACÍ PODLE HODNOCENÍ
void sortRestByRating();

// ŘAZENÍ RESTAURACÍ PODLE ABECEDY
void sortRestByName();

// HLEDÁNÍ RESTAURACE PODLE ID
void searchRestByID();

// HLEDÁNÍ RESTAURACE PODLE JIDLA
void searchRestByMenu();

// HLEDÁNÍ RESTAURACE PODLE MÍSTA
void searchRestByLocation();


// FUNKCE OD MONČI :D
int writeCom();
int writeRest();
int readRest();
int readCom();

