#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>

#define MAX_HEIGHT 5
#define MAX_WIDTH 10
#define INTRO "intro.txt"
#define SCOREBOARD "scoreboard.txt"
#define WINNER_TEAM_1 "winner1.txt"
#define WINNER_TEAM_2 "winner2.txt"
#define EXIT "exit.txt"

typedef enum {
    STATE_INITIAL,
    STATE_PLAYING,
    STATE_TRUCO,
    STATE_TRUCO_ACCEPTED,
    STATE_11,
    STATE_11_ACCEPTED
} State; 

typedef enum {
    NONE,
    TEAM_1,
    TEAM_2,
} Team;

struct Points {
    int team1;
    int team2; 
};

const char *numbers[13][MAX_HEIGHT] = {
    {
        "  __  ",
        " /  \\ ",
        "|    |",
        "|    |",
        " \\__/ "
    },
    {
        "  ,  ",
        " /|  ",
        "  |  ",
        "  |  ",
        "  |  "
    },
    {
        "  __  ",
        " /  ) ",
        "   /  ",
        "  /   ",
        " /___ "
    },
    {
        " ___ ",
        "/   \\",
        "  __/",
        "    \\",
        "\\___/"
    },
    {
        "     ",
        "|  | ",
        "|__|_",
        "   | ",
        "   | "
    },
    {
        " ____",
        "|    ",
        "|___ ",
        "    \\",
        "\\___/"
    },
    {
        "  __ ",
        " /   ",
        "| __ ",
        "|/  \\",
        " \\__/"
    },
    {
        "_____",
        "    /",
        "   / ",
        "  /  ",
        " /   "
    },
    {
        " __ ",
        "/  \\",
        "\\__/",
        "/  \\",
        "\\__/"
    },
    {
        " __ ",
        "/  |",
        "\\_/|",
        "   |",
        "   |"
    },
    {
        "  ,    __  ",
        " /|   /  \\ ",
        "  |  |    |",
        "  |  |    |",
        "  |   \\__/ "
    },
    {
        "  ,    ,  ",
        " /|   /|  ",
        "  |    |  ",
        "  |    |  ",
        "  |    |  "
    },
    {
        "  ,   __ ",
        " /|  /  )",
        "  |    / ",
        "  |   /  ",
        "  |  /___"
    }
};

State currentState;
Team currentTeam;
struct Points points;
int currentTruco = 1;

void printAsciiArt(char filename[]) {
	FILE *file;
	char txt[128];
	int counter = 0;
	
	if ((file = fopen(filename, "r")) == 0) {
        printf("Não foi possível encontrar um dos arquivos do jogo, favor conferir e iniciar novamente.\n");
		exit(0);
	} else {
		while(fgets(txt, sizeof(txt), file) != NULL) {
			counter++;
            printf("%s", txt);
		}
	}
}

void printMenu() {
    switch(currentState) {
        case STATE_INITIAL:
            printAsciiArt(INTRO);
            break;

        case STATE_PLAYING:
            printf("+************************************************************************************************************+\n");
            printf("1 - Equipe 1 pediu truco\n");
            printf("2 - Equipe 2 pediu truco\n");
            printf("3 - Equipe 1 ganhou\n");
            printf("4 - Equipe 2 ganhou\n");
            printf("+************************************************************************************************************+\n");
            break;

        case STATE_TRUCO:
            printf("+************************************************************************************************************+\n");
            if (currentTeam == TEAM_1) printf("A equipe 2 deseja:\n");
            else if (currentTeam == TEAM_2) printf("A equipe 1 deseja:\n");
            printf("1 - Aceitar\n");
            printf("2 - Correr\n");
            if (currentTruco == 1) printf("3 - Pedir %d\n", currentTruco + 5);
            else if (currentTruco != 9) printf("3 - Pedir %d\n", currentTruco + 6);
            printf("+************************************************************************************************************+\n");
            break;

        case STATE_11_ACCEPTED:
        case STATE_TRUCO_ACCEPTED:
            printf("+************************************************************************************************************+\n");
            printf("1 - Equipe 1 ganhou\n");
            printf("2 - Equipe 2 ganhou\n");
            printf("+************************************************************************************************************+\n");
            break;
            
        case STATE_11:
            printf("+************************************************************************************************************+\n");
            printf("Mão de 11 - Caso a equipe escolha jogar, a rodada vale 3 pontos.\n");
            if (points.team1 == 11) printf("A equipe 1 deseja:\n");
            else printf("A equipe 2 deseja:\n");
            printf("1 - Jogar\n");
            printf("2 - Correr\n");
            printf("+************************************************************************************************************+\n");
            break;
        break;
    }
}

void printExit() {
    system("cls");
    printAsciiArt(EXIT);
}

void printWinner(Team team) {
    system("cls");
    if (team == TEAM_1) printAsciiArt(WINNER_TEAM_1);
    else printAsciiArt(WINNER_TEAM_2);;
    exit(0);
}

void printScoreNumbers(int num1, int num2) {
    char *spacingBetweenNumbers = "                                                    ";
    char *startingSpacing = "                     ";
    if (num1 < 0 || num1 > 12 || num2 < 0 || num2 > 12) {
        return;
    }

    int i;
    for (i = 0; i < MAX_HEIGHT; ++i) {
        printf("%s%s%s%s\n", startingSpacing, numbers[num1][i], spacingBetweenNumbers, numbers[num2][i]);
    }
}

void printScoreboard() {
    printf("\n");
    printAsciiArt(SCOREBOARD);
    printf("\n");
    printScoreNumbers(points.team1, points.team2);
    printf("\n");
}

void resetTrucoState() {
    currentTeam = NONE;
    currentTruco = 1;
    currentState = STATE_PLAYING;
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    char option;

    currentState = STATE_INITIAL;
    currentTeam = NONE;
    points.team1 = 0;
    points.team2 = 0;

    while(1) {
        system("cls");
        if (currentState != STATE_INITIAL) printScoreboard();
        if (points.team1 == points.team2 && (points.team1 == 11 || points.team2 == 11)) currentState = STATE_11_ACCEPTED;
        else if (currentState != STATE_11_ACCEPTED && points.team1 != points.team2 && (points.team1 == 11 || points.team2 == 11) ) { 
            currentState = STATE_11;
            currentTruco = 3;
        }

        if (points.team1 >= 12) printWinner(TEAM_1);
        if (points.team2 >= 12) printWinner(TEAM_2);

        printMenu();

        if (currentState != STATE_INITIAL) option = getch();

        switch(currentState) {
            case STATE_INITIAL:
                getch();
                currentState = STATE_PLAYING;
                break;

            case STATE_PLAYING:
                switch (option) {
                    case '1':
                        currentState = STATE_TRUCO;
                        currentTeam = TEAM_1;
                        break;

                    case '2':
                        currentState = STATE_TRUCO;
                        currentTeam = TEAM_2;
                        break;

                    case '3':
                        points.team1 += 1;
                        break;

                    case '4':
                        points.team2 += 1;
                        break;

                    case ' ':
                        printExit();
                        exit(0);
                    
                    default:
                        printf("Opção inválida.\n");
                        break;
                }
                break;

            case STATE_TRUCO:
                switch (option) {
                    case '1':
                        currentState = STATE_TRUCO_ACCEPTED;
                        if (currentTruco == 1) currentTruco = 3;
                        else currentTruco += 3;
                        break;

                    case '2':
                        if (currentTeam == TEAM_1) points.team1 += currentTruco;
                        else points.team2 += currentTruco;
                        resetTrucoState();
                        break;
                    
                    case '3':
                        if (currentTruco == 1) currentTruco = 3;
                        else if (currentTruco != 9) currentTruco += 3;
                        if (currentTeam == TEAM_1) currentTeam = TEAM_2;
                        else currentTeam = TEAM_1;
                        break;

                    case ' ':
                        printExit();
                        exit(0);
                    
                    default:
                        printf("Opção inválida.\n");
                        break;
                    }
                break;

            case STATE_TRUCO_ACCEPTED:
                switch (option) {
                    case '1':
                        points.team1 += currentTruco;
                        resetTrucoState();
                        break;

                    case '2':
                        points.team2 += currentTruco;
                        resetTrucoState();
                        break;

                    case ' ':
                        printExit();
                        exit(0);

                    default:
                        printf("Opção inválida.\n");
                        break;
                    }
                break;

            case STATE_11:
                switch (option) {
                    case '1':
                        currentState = STATE_11_ACCEPTED;
                        break;
                    
                    case '2':
                        if (points.team1 == 11) points.team2 += currentTruco;
                        else if (points.team2 == 11) points.team1 += currentTruco;
                        break;

                    case ' ':
                        printExit();
                        exit(0);

                    default:
                        printf("Opção inválida.\n");
                        break;
                }
                break;

            case STATE_11_ACCEPTED:
                switch (option) {
                    case '1':
                        points.team1 += currentTruco;
                        currentState = STATE_11;
                        break;

                    case '2':
                        points.team2 += currentTruco;
                        currentState = STATE_11;
                        break;
                    
                    case ' ':
                        printExit();
                        exit(0);

                    default:
                        printf("Opção inválida.\n");
                        break;
                    }
                break;
        }
    }
    return 0;
}
