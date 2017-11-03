// Ha október, akkor "Oktoberfest"! A "Sör-Kocsi Budapest"(SKB) társaság mintegy társrendezvényként, akciós sörtúrákat szervez.
// Egy sörbicikli túrához szervezzük a jelentkezőket! 
// A készítendő program adjon lehetőséget új jelentkező adatainak(név,email,telefonszám, útvonal) rögzítésére, módosítására, majd igény esetén később annak törlésére.
// Három útvonalon mennek a sörtúrák (Parlament, Hősök tere, Vár). Ezen kívül egy jelentkező megmondhatja, hogy hány fő nevében jelentkezik. (Minden hozott vendég 15% kedvezményt kap!) 
// A program tudja még az adatok kezelése mellett a teljes névsort vagy az egyes útvonalakra jelentkezetteket listázni! 
// A jelentkezéskor tárolja automatikusan annak felvételi dátumát (év,hó,nap,óra,perc,másodperc).

#include <time.h>
#include <stdio.h>

typedef int bool;
#define true 1
#define false 0

typedef enum {PARLAMENT = 0, HOSOK_TERE = 1, VAR = 2} Path;

struct Participant {
    char* name;
    char* email;
    char* phone;
    Path path;
};

struct Apply {
    struct Participant participant;
    int participants;
    time_t time;
};

struct ApplyRead {
    struct Participant participant;
    int participants;
    char* time;
};

struct Apply getDataFromConsole() {
    char* name;
    char* email;
    char* phone;
    Path path;
    int participants;
    
    printf("\nAdd meg az adataid:\nNev: ");
    scanf("%s", name);
    printf("\nEmail: ");
    scanf("%s", email);
    printf("\nTelefon: ");
    scanf("%s", phone);
    printf("\nUtvonal (Parlament - 0, Hosok tere - 1, Var - 2): ");
    scanf("%d", path);
    printf("\nResztvevok szama: ");
    scanf("%d", participants);

    struct Participant participant = {name, email, phone, path};

    time_t currentTime = time(0);

    struct Apply result = {participant, participants, currentTime};
    return result;
}

struct ApplyRead getDataFromLine(char* line) {
    char* name;
    char* email;
    char* phone;
    Path path;
    int participants;
    char* currTime;
    
    scanf(line, "%s,%s,%s,%d,%d,%s", name, email, phone, path, participants, currTime);

    struct Participant participant = {name, email, phone, path};

    struct ApplyRead result = {participant, participants, currTime};
    return result;
}

void drawMenu() {
    char menu = 'o';
    while (menu != 'x' || menu != 'X') {
        printf("\n** Menu **\n1 - Uj jelentkezo\n2 - Nevsor\n3 - Utak\nX - Kilepes\nValasztott menupont: ");
        scanf("%c", menu);

    }
}

int main(int argc, char** argv) {
    drawMenu();
    return 0;
}