// Amint az SKB társaság(szülő) azt látja, hogy egy sörtúra nyereségesen indítható(van elég jelentkező), úgy elindul a túra!
// Az SKB munkába állít egy Sör-Bike-Drivert (SBD)(gyerek), aki felpattan egy sörbiciklire, majd beáll "tankolni", megfelelő mennyiségű sört vételezni! 
// Amint kész a vételezésre, erről jelzést küld SKB-nek, aki válaszul elküldi az utaslistát az egy főre eső részvételi díjat és a túraútvonalat.
// Az adatok csővezetéken érkeznek. SBD kiírja a képernyőre az adatokat, majd résztvevőnként 5 liter sört vételez, beszedi a jelentkezési díjakat. 
// A vételezett sör mennyiségét és a beszedett díj összegét visszaküldi SKB társaságnak szintén csővezetéken, és ezzel elindul a túra.
// Amint a túra véget ér (kis idő múlva, véletlen szám, sleep,usleep) SBD jelzi SKB társaságnak, hogy a feladatot elvégezte, kész a következő túrára!

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>

typedef int bool;
#define true 1
#define false 0

typedef enum {PARLAMENT = 0, HOSOK_TERE = 1, VAR = 2} Path;

struct Participant {
    char name[32];
    char email[32];
    char phone[32];
    Path path;
};

struct Apply {
    struct Participant participant;
    int participants;
    char currTime[32];
};

struct Apply getDataFromConsole() {
    char name[32];
    char email[32];
    char phone[32];
    char currTime[32];
    Path path;
    int participants;
    
    printf("\nAdd meg az adataid:\nNev: ");
    scanf("%s", name);
    printf("\nEmail: ");
    scanf("%s", email);
    printf("\nTelefon: ");
    scanf("%s", phone);
    printf("\nUtvonal (Parlament - 0, Hosok tere - 1, Var - 2): ");
    scanf("%d", &path);
    printf("\nResztvevok szama: ");
    scanf("%d", &participants);

    struct Participant participant;
    strcpy(participant.name, name);
    strcpy(participant.email, email);
    strcpy(participant.phone, phone);
    participant.path = path;

    time_t rawtime;
    struct tm * timeinfo;
  
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    strcpy(currTime, asctime(timeinfo));
    
    struct Apply result;
    result.participant = participant;
    result.participants = participants;
    strcpy(result.currTime, currTime);
    return result;
}

struct Apply* loadFile(int* cnt) {
    FILE* file;
    char buffer[255];
    int count = 0;
    int i = 0;
    file = fopen("save.txt", "r");

    if(file == NULL) {
        file = fopen("save.txt", "w");
        fprintf(file, "0");
        fclose(file);
        return NULL;
    }

    fscanf(file, "%d\n", &count);
    *cnt = count;

    if(count == 0) {
        fclose(file);
        return NULL;
    }

    struct Apply* result = malloc(count * sizeof(struct Apply));

    while(i < count){
        char name[32];
        char email[32];
        char phone[32];
        Path path;
        int participants;
        char currTime[32];
        
        fscanf(file, "%[^,],%[^,],%[^,],%d,%d,%[^\n]\n", name, email, phone, &path, &participants, currTime);

        struct Participant participant;
        strcpy(participant.name, name);
        strcpy(participant.email, email);
        strcpy(participant.phone, phone);
        participant.path = path;

        struct Apply tmp;
        tmp.participant = participant;
        tmp.participants = participants;
        strcpy(tmp.currTime, currTime);

        result[i] = tmp;
        ++i;
    }

    fclose(file);
    return result;
}

void saveFile(struct Apply* participants, int count) {
    FILE* file = fopen("save.txt", "w");
    fprintf(file, "%d\n", count);

    int i = 0;
    while(i < count) {
        fprintf(file, "%s,%s,%s,%d,%d,%s\n", participants[i].participant.name, participants[i].participant.email, participants[i].participant.phone, participants[i].participant.path, participants[i].participants, participants[i].currTime);
        ++i;
    }

    fclose(file);
}

void handler(int signumber) {
    printf("\nA kocsi es a tarsasag talakozik.");
}

void drawMenu() {
    int count = 0;
    struct Apply* participants = loadFile(&count);
    struct Apply newParticipant;
    int i = 0;
    int path = 0;
    char menu[1];

    int car = 0;
    int path1 = 0;
    int path2 = 0;
    int path3 = 0;
    int price = 10;

    int pipefd[2];
    pid_t pid;
    int sz[3];

    if (pipe(pipefd) == -1) {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }

    signal(SIGUSR1,handler);

    pid = fork();

    if(pid == 0) {
        while(1) {
            printf("\nKocsi parkol.");
            pause();
            printf("\nKocsi elindult.");
            sleep(5);
            printf("\nKocsi tankol.");
            read(pipefd[0],sz,sizeof(sz));
            int c = sz[0];
            int pr = sz[1];
            int beer = c * 5;
            int profit =c * pr;
            printf("\nA kocsiban %d resztvevo van.\nA kocsi sor tartalma %d l.\nBevetel: %d", c, beer, profit);
            close(pipefd[0]);
            sleep(5);
            printf("\nKocsi uton van.");
            write(pipefd[1], &beer, sizeof(beer));
            write(pipefd[1], &profit, sizeof(profit));
            close(pipefd[1]);
            fflush(NULL);
            kill(getppid(), SIGUSR1);
            printf("\nKocsi vegzett.");
        }
    } else {
        do {
            printf("\n** Menu **\n1 - Uj jelentkezo\n2 - Nevsor\n3 - Utak\n4 - Modositas\n5 - Torles\nX - Kilepes\nValasztott menupont: ");
            scanf("%s", menu);
            switch(menu[0]) {
                case '1':
                    newParticipant = getDataFromConsole();
                    participants = realloc(participants, (count + 1) * sizeof(struct Apply));
                    participants[count] = newParticipant;
                    ++count;
                    saveFile(participants, count);
                break;
                case '2':
                    i = 0;
                    while(i < count) {
                        printf("%s\n", participants[i].participant.name);
                        ++i;
                    }
                break;
                case '3':
                    printf("Valassz utat (Parlament - 0, Hosok tere - 1, Var - 2): ");
                    scanf("%d", &path);
                    i = 0;
                    while(i < count) {
                        if(participants[i].participant.path == path) {
                            printf("%s\n", participants[i].participant.name);
                        }
                        ++i;
                    }
                break;
                case '4':
                    printf("Valassz modositani kivant jelentkezot:\n");
                    i = 0;
                    while(i < count) {
                        printf("%d - %s (%s)\n", i, participants[i].participant.name, participants[i].participant.email);
                        ++i;
                    }
                    scanf("%d", &path);
                    participants[path] = getDataFromConsole();
                    saveFile(participants, count);
                break;
                case '5':
                    printf("Valassz torolni kivant jelentkezot:\n");
                    i = 0;
                    while(i < count) {
                        printf("%d - %s (%s)\n", i, participants[i].participant.name, participants[i].participant.email);
                        ++i;
                    }
                    scanf("%d", &path);
                    while(path < count - 1) {
                        participants[path] = participants[path + 1];
                        ++path;
                    }
                    participants = realloc(participants, (count--) * sizeof(struct Apply));
                    --count;
                    saveFile(participants, count);
                break;
                case 'x':
                case 'X':
                break;
                default:
                    printf("\nPlease choose a valid menu\n");
                break;
            }
            if(car == 0) {
                i = 0;
                path1 = 0;
                path2 = 0;
                path3 = 0;
                while(i < count) {
                    switch(participants[i].participant.path) {
                        case 0: path1 += participants[i].participants; break;
                        case 1: path2 += participants[i].participants; break;
                        case 2: path3 += participants[i].participants; break;
                    }
                    ++i;
                }
                if(path1 > 5) {
                    car = 1;
                    path = 0;
                    write(pipefd[1], &path1, sizeof(path1));
                    write(pipefd[1], &price, sizeof(price));
                    write(pipefd[1], &path, sizeof(path));
                    close(pipefd[1]);
                    fflush(NULL);
                    kill(0, SIGUSR1);
                    pause();
                    car = 0;
                } else if(path2 > 5) {
                    car = 1;
                    path = 1;
                    write(pipefd[1], &path2, sizeof(path2));
                    write(pipefd[1], &price, sizeof(price));
                    write(pipefd[1], &path, sizeof(path));
                    close(pipefd[1]);
                    fflush(NULL);
                    kill(0, SIGUSR1);
                    pause();
                    car = 0;
                } else if(path3 > 5) {
                    car = 1;
                    path = 2;
                    write(pipefd[1], &path3, sizeof(path3));
                    write(pipefd[1], &price, sizeof(price));
                    write(pipefd[1], &path, sizeof(path));
                    close(pipefd[1]);
                    fflush(NULL);
                    kill(0, SIGUSR1);
                    pause();
                    car = 0;
                }
            }
        } while(menu[0] != 'X' && menu[0] != 'x');
    }
}

int main(int argc, char** argv) {
    drawMenu();
    return 0;
}