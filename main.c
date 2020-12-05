#include "restFunction.h"

// ID RESTAURACE NELZE NIKDY ZMĚNIT -> jedná se o jedinečný kód, který bude existovat po celou dobu databáze

// HLAVNÍ PROGRAM
int main(){
    // Při spuštění dojde k načtení restaurací a komentářů z dokumentů
    readRest();
    readCom();

    int volba;

    // TISK ÚVODNÍHO MENU - JEN PŘI SPUŠTĚNÍ (pak už se neopakuje)
    printf(ANSI_COLOR_GREEN"");
    printf("--------------------------------------------------------------------------------------------");
    printf("\n                               DATABÁZE RESTAURACÍ                                      \n");
    printf("--------------------------------------------------------------------------------------------");
    printf(ANSI_COLOR_RESET"\n");

    while (1){
        // Vykreslení menu, které se objevuje neustále
        printf("\n--------------------------------------------------------------------------------------------\n");
        printf(" 01: Pridat restauraci            02: Smazat restauraci             03: Editovat restauraci \n");
        printf(" 04: Tisk podle abecedy           05: Tisk podle hodnoceni          06: Přidání jídelníčku  \n");
        printf(" 07: Pridat Komentar              08: Zobrazeni Komentaru           09: Hledání komentářů   \n");
        printf(" 10: Hledání podle jidla          11: Hledání podle mista           12: Ukončení databáze   ");
        printf("\n--------------------------------------------------------------------------------------------\n");

        // VOLBA UŽIVATELE
        scanf("%d",&volba);
        while (getchar() != '\n');

        // Provedení funkcí na základě volby uživatele
        switch (volba){
            case 1: restAdd(); break;
            case 2: restDelete(); break;
            case 3: restEdit(); break;
            case 4: sortRestByName(); restShow(); break;
            case 5: sortRestByRating(); restShow(); break;
            case 6: addMenu(); break;
            case 7: addComment(); break;
            case 8: showComment(); break;
            case 9: searchRestByID(); break;
            case 10: searchRestByMenu(); break;
            case 11: searchRestByLocation(); break;
            case 12: writeRest(); writeCom(); exit(0);
        }
    }
}

