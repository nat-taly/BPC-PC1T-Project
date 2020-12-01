#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define BUFFSIZE 500

// ID RESTAURACE NELZE NIKDY ZMENIT -> jedná se o jedinečný kód, který bude existovat po celou dobu databáze

// ULOŽENÍ DO SOUBORŮ A NAČTENÍ ZE SOUBORŮ

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


// nastavení ukazatelů na 0
struct restaurace* prvni = NULL;
struct komentar* prvnikom = NULL;

// PŘIDÁNÍ RESTAURACE
void add(char* nazev_restaurace, char* adresa_restaurace, int ID_restaurace, struct restaurace** uk_prvni){

    struct restaurace* novaRestaurace; // ukazatel pro nove vkladanou restauraci
    struct restaurace* aktualniRestaurace; // ukazatel na aktualni restauraci

    // alokace místa pro novou restauraci
    novaRestaurace = (struct restaurace*)malloc(sizeof(struct restaurace));

    //naplnění struktury
    strcpy(novaRestaurace->nazev, nazev_restaurace);
    strcpy(novaRestaurace->mesto, adresa_restaurace);
    novaRestaurace->id = ID_restaurace;
    novaRestaurace->dalsi = NULL; //posunutí ukazatele na poslední prvek seznamu

    if (*uk_prvni == NULL){
        *uk_prvni = novaRestaurace;
        return;
    }
    else if (strcmp(novaRestaurace->nazev, (*uk_prvni)->nazev) < 0){
        novaRestaurace->dalsi = *uk_prvni;
        *uk_prvni = novaRestaurace;
        return;
    }
    aktualniRestaurace = *uk_prvni;
    // procházení seznamu (řazení do seznamu podle abecedy)
    while (aktualniRestaurace){
        //pokud jsme naa poslední položce, provede se následující blok
        if (aktualniRestaurace->dalsi == NULL){
            aktualniRestaurace->dalsi = novaRestaurace; // pridavame na konec
            return;
        }
        //porovnávání názvů restaurace
        else if (strcmp(novaRestaurace->nazev, (*uk_prvni)->dalsi->nazev) < 0){
            novaRestaurace->dalsi = aktualniRestaurace->dalsi; //vlozi za aktualni restauraci
            aktualniRestaurace->dalsi = novaRestaurace;
            return;
        }
        aktualniRestaurace = aktualniRestaurace->dalsi; // posun na dalsi restauraci v seznamu
    }
}

// ODSTRANĚNÍ RESTAURACE
void del(int ID_restaurace, struct restaurace** uk_prvni){
    struct restaurace* aktualni_restaurace;

    while (*uk_prvni && (*uk_prvni)->id == ID_restaurace){
        struct restaurace* newPrvni = (*uk_prvni)->dalsi;
        free(*uk_prvni);  // uvolneni restaurace z pameti
        *uk_prvni = newPrvni;
    }

    aktualni_restaurace = *uk_prvni;
    //procházení seznamu a hledání restaurace k mazání
    while (aktualni_restaurace && aktualni_restaurace->dalsi){
        //pokud bude splněna podmínka, odstraní se restaurace
        if (aktualni_restaurace->dalsi->id == ID_restaurace){
            struct restaurace* newDalsi = aktualni_restaurace->dalsi->dalsi;
            free(aktualni_restaurace->dalsi);  // uvolneni restauraec z pameti
            aktualni_restaurace->dalsi = newDalsi;
        }
        aktualni_restaurace = aktualni_restaurace->dalsi; // posun na dalsi auto
    }
}

// ODSTRANĚNÍ KOMENTÁŘŮ
void  delKom(int ID_restaurace, struct komentar** uk_prvnikom){
    struct komentar* aktualni_komentar;

    //stejný postup mazání jako u mazání restaurací
    while (*uk_prvnikom && (*uk_prvnikom)->id_restaurace == ID_restaurace){
        struct komentar* newPrvniKom = (*uk_prvnikom)->dalsikom;
        free(*uk_prvnikom);  // uvolneni restaurace z pameti
        *uk_prvnikom = newPrvniKom;
    }

    aktualni_komentar = *uk_prvnikom;
    while (aktualni_komentar && aktualni_komentar->dalsikom){
        if (aktualni_komentar->dalsikom->id_restaurace == ID_restaurace){
            struct komentar* newDalsiKom = aktualni_komentar->dalsikom->dalsikom;
            free(aktualni_komentar->dalsikom);  // uvolneni komentare z pameti
            aktualni_komentar->dalsikom = newDalsiKom;
        }
        aktualni_komentar = aktualni_komentar->dalsikom; // posun na dalsi auto
    }
}

// PŘIDÁNÍ RESTAURACE, VSTUP OD UŽIVATELE
void OnAdd(){
    //lokální proměnné pro restauraci
    int in_id;
    char in_nazev[BUFFSIZE];
    char in_adresa[BUFFSIZE];

    printf("\nID restaurace    : ");
    scanf("%d", &in_id);
    while (getchar() != '\n');

    printf("Název restaurace : ");
    fgets(in_nazev, BUFFSIZE, stdin);
    in_nazev[strlen(in_nazev) - 1]=0;

    printf("Město restaurace : ");
    fgets(in_adresa, BUFFSIZE, stdin);
    in_adresa[strlen(in_adresa) - 1]=0;

    // Volaní funkce, která přidá restauraci
    add(in_nazev, in_adresa, in_id, &prvni);
}

// ODSTRANĚNÍ RESTAURACE, VSTUP OD UŽIVATELE
void OnDel(){
    int in_ID_restaurace;

    // ID restaurace, kterou cheme smazat
    printf("\nID restaurace    : ");
    scanf("%d", &in_ID_restaurace);
    while (getchar() != '\n');

    // volání funkcí pro mazání restaurace a komentářů, které pro ni existují
    del(in_ID_restaurace, &prvni);
    delKom(in_ID_restaurace, &prvnikom);
}

// ZOBRAZENÍ RESTAURACÍ
void ShowRest(){
    struct restaurace* aktualni_restaurace = prvni;
    printf("\n");

    //prochází celý seznam a tiskne restaurace
    while (aktualni_restaurace){
        printf(ANSI_COLOR_GREEN"ID%4d :     HODNOCENI: %.2f     NÁZEV: %s     ADRESA: %s     MENU: %s\n"ANSI_COLOR_RESET, aktualni_restaurace->id, aktualni_restaurace->celkovehodnoceni, aktualni_restaurace->nazev, aktualni_restaurace->mesto, aktualni_restaurace->jidlo);
        aktualni_restaurace = aktualni_restaurace->dalsi;
    }
    getchar();
}

// EDITACE RESTAURACE
void EditRest(){
    int in_ID_restaurace;
    struct restaurace* aktualni_restaurace = prvni;

    //ID restaurace, kerou chce uživatel editovat
    printf("\nID restaurace    : ");
    scanf("%d", &in_ID_restaurace);
    while (getchar() != '\n');

    //procházení všech restaurací
    while(aktualni_restaurace){
        if(in_ID_restaurace == aktualni_restaurace->id){
            printf(ANSI_COLOR_GREEN"RESTAURACE NALEZENA:"ANSI_COLOR_RESET" %s\n", aktualni_restaurace->nazev);

            char new_nazev[BUFFSIZE];
            char new_adresa[BUFFSIZE];

            printf("\nNový název restaurace  : ");
            fgets(new_nazev, BUFFSIZE, stdin);
            new_nazev[strlen(new_nazev) - 1]=0;

            printf("Nová adresa restaurace : ");
            fgets(new_adresa, BUFFSIZE, stdin);
            new_adresa[strlen(new_adresa) - 1]=0;

            // editace restaurace
            stpcpy(aktualni_restaurace->nazev,new_nazev);
            stpcpy(aktualni_restaurace->mesto, new_adresa);
        }
        aktualni_restaurace = aktualni_restaurace->dalsi; // posun na dalsi restauraci
    }
}

// ZOBRAZENÍ KOMENTÁŘŮ
void ShowKom(){
    struct komentar* aktualnikomentar = prvnikom;
    printf("\n");

    // procházení celého seznamu komentářů
    while (aktualnikomentar){
        printf(ANSI_COLOR_GREEN"ID%4d :     HODNOCENI: %.2f     %s\n"ANSI_COLOR_RESET, aktualnikomentar->id_restaurace, aktualnikomentar->hodnoceni, aktualnikomentar->komentar);
        aktualnikomentar = aktualnikomentar->dalsikom;
    }
    getchar();
}

// PŘIDÁNÍ KOMENTÁŘE
void addKom(int id_restaurace, float hodnoceni, char* komentar, struct komentar** uk_prvnikom){
    struct komentar* novyKomentar;
    struct komentar* aktualniKomentar;

    // alokace místa pro nový komentář
    novyKomentar = (struct komentar*)malloc(sizeof(struct komentar));

    //naplnění struktury
    novyKomentar->id_restaurace = id_restaurace;
    novyKomentar->hodnoceni = hodnoceni;
    strcpy(novyKomentar->komentar, komentar);
    novyKomentar->dalsikom = NULL;

    // pokud je lineární seznam prázdný
    if (*uk_prvnikom == NULL){
        *uk_prvnikom = novyKomentar;
        return;
    }
    else if (novyKomentar->id_restaurace < (*uk_prvnikom)->id_restaurace){
        novyKomentar->dalsikom = *uk_prvnikom;
        *uk_prvnikom = novyKomentar;
        return;
    }

    //procházení seznamu komentářů a řazení podle ID restaurace
    aktualniKomentar = *uk_prvnikom;
    while (aktualniKomentar){
        if (aktualniKomentar->dalsikom == NULL){
            aktualniKomentar->dalsikom = novyKomentar;
            return;
        }
        else if (novyKomentar->id_restaurace < aktualniKomentar->dalsikom->id_restaurace){
            novyKomentar->dalsikom = aktualniKomentar->dalsikom; // vlozime za aktualni komentar
            aktualniKomentar->dalsikom = novyKomentar;
            return;
        }
        aktualniKomentar = aktualniKomentar->dalsikom; // posun na dalsi hudbu
    }
}

// PŘIDÁNÍ KOMENTÁŘE, VSTUP OD UŽIVATELE
void pridatKomentar() {
    int in_ID_restaurace;
    struct restaurace *aktualni_restaurace = prvni;

    // výzva pro zadání ID restaurace, ke které chceme komentář přidat
    printf("\nID restaurace    : ");
    scanf("%d", &in_ID_restaurace);
    while (getchar() != '\n');

    //procházení restaurací
    while (aktualni_restaurace) {
        if (in_ID_restaurace == aktualni_restaurace->id) {
            float in_hodnoceni = -1;
            char in_komentar[BUFFSIZE];

            printf("Hodnocení 1-10   : ");
            scanf("%f", &in_hodnoceni);
            while (getchar() != '\n');

            while(!((in_hodnoceni <= 10) && (in_hodnoceni >=0))){
                printf("Hodnocení 1-10   : ");
                scanf("%f", &in_hodnoceni);
                while (getchar() != '\n');
            }

            printf("Komentář         : ");
            fgets(in_komentar, BUFFSIZE, stdin);
            in_komentar[strlen(in_komentar) - 1]=0;

            //uložení počtu komentářů, součet hodnocení a průměr hodnocení k restauraci
            aktualni_restaurace->pocetkom = aktualni_restaurace->pocetkom + 1;
            aktualni_restaurace->soucethodkom = aktualni_restaurace->soucethodkom + in_hodnoceni;
            aktualni_restaurace->celkovehodnoceni = aktualni_restaurace->soucethodkom / aktualni_restaurace->pocetkom;
            addKom(in_ID_restaurace, in_hodnoceni, in_komentar, &prvnikom);
        }
        aktualni_restaurace = aktualni_restaurace->dalsi; // posun na dalsi restauraci
    }
}

// PŘIDÁNÍ JÍDLA K RESTAURACI
void pridatJidlo(){
    int in_ID_restaurace;
    struct restaurace *aktualni_restaurace = prvni;

    // výzva pro zadání ID restaurace, ke které chceme komentář přidat
    printf("\nID restaurace    : ");
    scanf("%d", &in_ID_restaurace);
    while (getchar() != '\n');

    //procházení restaurací
    while (aktualni_restaurace) {
        if (in_ID_restaurace == aktualni_restaurace->id) {

            char in_jidlo[BUFFSIZE];
            printf("Menu             : ");
            fgets(in_jidlo, BUFFSIZE, stdin);
            in_jidlo[strlen(in_jidlo) - 1]=0;

            //uložení jídla k restauraci
            strcpy(aktualni_restaurace->jidlo,in_jidlo);
        }
        aktualni_restaurace = aktualni_restaurace->dalsi; // posun na dalsi restauraci
    }

}

// PROHOZENÍ STRUKTUR RESTAURACE
void swapNodes(struct restaurace *a, struct restaurace *b) {
    struct restaurace tmp = *a;
    a->id = b->id;
    strcpy(a->nazev, b->nazev);
    strcpy(a->mesto, b->mesto);
    a->pocetkom = b->pocetkom;
    a->soucethodkom = b->soucethodkom;
    a->celkovehodnoceni = b->celkovehodnoceni;
    strcpy(a->jidlo, b->jidlo);

    b->id = tmp.id;
    strcpy(b->nazev, tmp.nazev);
    strcpy(b->mesto, tmp.mesto);
    b->pocetkom = tmp.pocetkom;
    b->soucethodkom = tmp.soucethodkom;
    b->celkovehodnoceni = tmp.celkovehodnoceni;
    strcpy(b->jidlo, tmp.jidlo);
}

// ŘAZENÍ RESTAURACÍ PODLE HODNOCENÍ
void sortRestByRating(){
    struct restaurace *aktualni_restaurace = prvni, *uk_prvni = NULL;

    if(prvni == NULL) {
        return;
    }
    else {
        while(aktualni_restaurace != NULL) {
            uk_prvni = aktualni_restaurace->dalsi;
            while(uk_prvni != NULL) {
                if(aktualni_restaurace->celkovehodnoceni < uk_prvni->celkovehodnoceni) {
                    // výmění aktualní restauraci s ukazatelem
                    swapNodes(aktualni_restaurace,uk_prvni);
                }
                uk_prvni = uk_prvni->dalsi;
            }
            aktualni_restaurace = aktualni_restaurace->dalsi;
        }
    }
}

// ŘAZENÍ RESTAURACÍ PODLE ABECEDY
void sortRestByName(){
    struct restaurace *aktualni_restaurace = prvni, *uk_prvni = NULL;
    if(prvni == NULL) {
        return;
    }
    else {
        while(aktualni_restaurace != NULL) {
            uk_prvni = aktualni_restaurace->dalsi;
            while(uk_prvni != NULL) {
                if(strcmp(aktualni_restaurace->nazev, uk_prvni->nazev) > 0){
                    // výmění aktualní restauraci s ukazatelem
                    swapNodes(aktualni_restaurace,uk_prvni);
                }
                uk_prvni = uk_prvni->dalsi;
            }
            aktualni_restaurace = aktualni_restaurace->dalsi;
        }
    }
}

// ZOBRAZENÍ KOMENTÁŘŮ PRO KONKRÉTNÍ RESTAURACI
void showCommentsForRest(int id_restaurace){
    struct komentar* aktualnikomentar = prvnikom;
    printf("\n");

    //procházení všech komentářů a následný tisk
    while (aktualnikomentar){
        if(aktualnikomentar->id_restaurace==id_restaurace){
            printf(ANSI_COLOR_GREEN"HODNOCENI: %.2f     %s\n"ANSI_COLOR_RESET, aktualnikomentar->hodnoceni, aktualnikomentar->komentar);
        }
        aktualnikomentar = aktualnikomentar->dalsikom;
    }
    getchar();
}

// HLEDÁNÍ RESTAURACE PODLE ID
void searchRestByID(){
    int in_ID_restaurace;
    struct restaurace *aktualni_restaurace = prvni;

    //zadání Id restaurace, kterou cheme hledat
    printf("\nID restaurace    : ");
    scanf("%d", &in_ID_restaurace);
    while (getchar() != '\n');

    while (aktualni_restaurace) {
        if (in_ID_restaurace == aktualni_restaurace->id) {
            showCommentsForRest(in_ID_restaurace);
        }
        aktualni_restaurace = aktualni_restaurace->dalsi; // posun na dalsi restauraci
    }
}

// HLEDÁNÍ RESTAURACE PODLE JIDLA
void searchRestByJidlo(){
    char in_jidlo[BUFFSIZE];

    struct restaurace *aktualni_restaurace = prvni;

    //zadání jídla, podle kterého cheme restauraci hledat
    printf("Menu             : ");
    fgets(in_jidlo, BUFFSIZE, stdin);
    in_jidlo[strlen(in_jidlo) - 1]=0;

    while (aktualni_restaurace) {
        if(strcmp(in_jidlo, aktualni_restaurace->jidlo) == 0){
            sortRestByRating();
            printf(ANSI_COLOR_GREEN"\nID%4d     HODNOCENI: %.2f     RESTAURACE: %s"ANSI_COLOR_RESET,aktualni_restaurace->id, aktualni_restaurace->celkovehodnoceni, aktualni_restaurace->nazev);
        }
        aktualni_restaurace = aktualni_restaurace->dalsi; // posun na dalsi restauraci
    }
}

// HLEDÁNÍ RESTAURACE PODLE MÍSTA
void searchRestByLocation(){
    char in_misto[BUFFSIZE];
    struct restaurace *aktualni_restaurace = prvni;

    //zadání místa, podle kterého cheme restauraci hledat
    printf("Město restaurace : ");
    fgets(in_misto, BUFFSIZE, stdin);
    in_misto[strlen(in_misto) - 1]=0;

    while (aktualni_restaurace) {
        if(strcmp(in_misto, aktualni_restaurace->mesto) == 0){
            sortRestByRating();
            printf(ANSI_COLOR_GREEN"\nID%4d     HODNOCENI: %.2f     RESTAURACE: %s"ANSI_COLOR_RESET,aktualni_restaurace->id, aktualni_restaurace->celkovehodnoceni, aktualni_restaurace->nazev);
        }
        aktualni_restaurace = aktualni_restaurace->dalsi;
    }
}


/*
// ZÁPIS DO SOUBORŮ
int writeIntoFile(){
    struct komentar *aktualni_komentar = prvnikom;
    FILE *vystup;

    fopen("vystupkom.txt","w");
    if(vystup == NULL){
        printf("\nsoubor nelze načíst...");
        return 1;
    }
    while (aktualni_komentar != NULL){
        //tady to nějak vyřešit, protože to nefunguje :/
        fprintf(vystup, "%d %lf %s\n", aktualni_komentar->id_restaurace, aktualni_komentar->hodnoceni, aktualni_komentar->komentar);
        aktualni_komentar = aktualni_komentar->dalsikom;
    }
    fclose(vystup);
}
*/

// HLAVNÍ PROGRAM
int main(){
    int volba;
        printf(ANSI_COLOR_GREEN"");
        printf("--------------------------------------------------------------------------------------------");
        printf("\n                               DATABÁZE RESTAURACÍ                                      \n");
        printf("--------------------------------------------------------------------------------------------");
        printf(ANSI_COLOR_RESET"\n");

    while (1){
        printf("\n--------------------------------------------------------------------------------------------\n");
        printf(" 01: Pridat restauraci            02: Smazat restauraci             03: Editovat restauraci \n");
        printf(" 04: Tisk podle abecedy           05: Tisk podle hodnoceni          06: Přidání jídelníčku  \n");
        printf(" 07: Pridat Komentar              08: Zobrazeni Komentaru           09: Hledání komentářů   \n");
        printf(" 10: Hledání podle jidla          11: Hledání podle mista           12: Ukončení databáze   ");
        printf("\n--------------------------------------------------------------------------------------------\n");


        scanf("%d",&volba);
        while (getchar() != '\n');

        switch (volba){
            case 1: OnAdd(); break;
            case 2: OnDel(); break;
            case 3: EditRest(); break;
            case 4: sortRestByName(); ShowRest(); break;
            case 5: sortRestByRating(); ShowRest(); break;
            case 6: pridatJidlo(); break;
            case 7: pridatKomentar(); break;
            case 8: ShowKom(); break;
            case 9: searchRestByID(); break;
            case 10: searchRestByJidlo(); break;
            case 11: searchRestByLocation(); break;
            case 12: exit(0);
        }
    }
}
