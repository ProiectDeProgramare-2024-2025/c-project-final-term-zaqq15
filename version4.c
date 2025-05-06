#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define MAX_WORDS 100
#define MAX_LEN 100
#define MAX_ATTEMPTS 6
#define FILENAME "words.txt"
#define SCORE_FILE "scores.txt"

// === Color Macros ===
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"

// === Structs ===
typedef struct {
    char text[MAX_LEN];
} Word;

typedef struct {
    char name[50];
    int wins;
    int losses;
} Player;

// === Function Prototypes ===
void enableColors();
int ReadWordsFromFile(Word words[]);
void AddWordToFile();
void drawHangman(int wrong);
void playHangman(Word words[], int count, Player *player);
void showInstructions();
int getValidatedChoice(int min, int max);
int loadPlayerStats(Player players[], int *count);
void savePlayerStats(Player players[], int count);
Player* findOrCreatePlayer(Player players[], int *count, const char *name);
void showLeaderboard();
int compareByWins(const void *a, const void *b);

// === Main ===
int main() {
    enableColors();
    int choice;
    Word words[MAX_WORDS];
    Player players[100];
    int playerCount = 0;
    char pause[10];

    char playerName[50];
    printf(BLUE "Enter your name: " RESET);
    fgets(playerName, sizeof(playerName), stdin);
    playerName[strcspn(playerName, "\n")] = 0;

    loadPlayerStats(players, &playerCount);
    Player *currentPlayer = findOrCreatePlayer(players, &playerCount, playerName);

    while (1) {
        system("cls || clear");
        printf(BLUE "\n=== Hangman Game ===\n" RESET);
        printf("Welcome, " GREEN "%s" RESET "! (Wins: %d | Losses: %d)\n", currentPlayer->name, currentPlayer->wins, currentPlayer->losses);
        printf("1. Start Game\n");
        printf("2. Word List\n");
        printf("3. Instructions\n");
        printf("4. Leaderboard\n");
        printf("5. Exit\n");

        choice = getValidatedChoice(1, 5);

        if (choice == 1) {
            int count = ReadWordsFromFile(words);
            playHangman(words, count, currentPlayer);
            savePlayerStats(players, playerCount);
        } else if (choice == 2) {
            while (1) {
                system("cls || clear");
                printf(BLUE "=== Word List Menu ===\n" RESET);
                printf("1. View Words\n");
                printf("2. Add Word\n");
                printf("3. Return to Main Menu\n");
                int subChoice = getValidatedChoice(1, 3);
                if (subChoice == 1) {
                    system("cls || clear");
                    printf(BLUE "=== Words in File ===\n" RESET);
                    int count = ReadWordsFromFile(words);
                    for (int i = 0; i < count; i++) {
                        printf(GREEN "%d. %s\n" RESET, i + 1, words[i].text);
                    }
                    printf("\nPress Enter to return...\n");
                    getchar();
                } else if (subChoice == 2) {
                    system("cls || clear");
                    AddWordToFile();
                    printf("Press Enter to return...\n");
                    getchar();
                } else break;
            }
        } else if (choice == 3) {
            system("cls || clear");
            showInstructions();
            printf("\nPress Enter to return...\n");
            fgets(pause, sizeof(pause), stdin);
        } else if (choice == 4) {
            system("cls || clear");
            showLeaderboard();
        } else {
            printf(GREEN "Goodbye, %s!\n" RESET, currentPlayer->name);
            break;
        }
    }

    return 0;
}

// === Helper Functions ===
void enableColors() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004;
    SetConsoleMode(hOut, dwMode);
}

int getValidatedChoice(int min, int max) {
    int input;
    while (1) {
        printf(YELLOW "Enter choice (%d-%d): " RESET, min, max);
        if (scanf("%d", &input) == 1 && input >= min && input <= max) {
            getchar();
            return input;
        }
        printf(RED "[ERR] Invalid input. Try again.\n" RESET);
        while (getchar() != '\n');
    }
}

int ReadWordsFromFile(Word words[]) {
    FILE *file = fopen(FILENAME, "r");
    int count = 0;
    if (!file) return 0;
    while (fgets(words[count].text, MAX_LEN, file)) {
        words[count].text[strcspn(words[count].text, "\n")] = 0;
        count++;
        if (count >= MAX_WORDS) break;
    }
    fclose(file);
    return count;
}

void AddWordToFile() {
    FILE *file = fopen(FILENAME, "a");
    if (!file) {
        printf(RED "Error opening file.\n" RESET);
        return;
    }
    char word[MAX_LEN];
    printf("Enter a new word: ");
    scanf("%s", word);
    fprintf(file, "%s\n", word);
    fclose(file);
    printf(GREEN "Word added!\n" RESET);
}

void drawHangman(int wrong) {
    const char *stages[] = {
        "  +---+\n      |\n      |\n      |\n     ===",
        "  +---+\n  O   |\n      |\n      |\n     ===",
        "  +---+\n  O   |\n  |   |\n      |\n     ===",
        "  +---+\n  O   |\n /|   |\n      |\n     ===",
        "  +---+\n  O   |\n /|\\  |\n      |\n     ===",
        "  +---+\n  O   |\n /|\\  |\n /    |\n     ===",
        "  +---+\n  O   |\n /|\\  |\n / \\  |\n     ==="
    };
    printf("%s\n", stages[wrong]);
}

void playHangman(Word words[], int count, Player *player) {
    if (count == 0) {
        printf(RED "No words to play. Please add some.\n" RESET);
        return;
    }

    srand(time(NULL));
    char word[MAX_LEN];
    strcpy(word, words[rand() % count].text);
    int len = strlen(word);
    char guessed[26] = "";
    char display[MAX_LEN];
    int guessed_flags[256] = {0};
    int correct = 0, wrong = 0;

    for (int i = 0; i < len; i++) display[i] = '_';
    display[len] = '\0';

    while (wrong < MAX_ATTEMPTS && correct < len) {
        system("cls || clear");
        drawHangman(wrong);
        printf("\nWord: ");
        for (int i = 0; i < len; i++) {
            printf(YELLOW "%c " RESET, display[i]);
        }

        printf("\nGuessed: ");
        for (int i = 0; guessed[i]; i++) printf("%c ", guessed[i]);

        char guess;
        printf("\n\nGuess a letter: ");
        scanf(" %c", &guess);
        getchar();

        if (guessed_flags[(unsigned char)guess]) {
            printf(RED "Already guessed '%c'. Try another.\n" RESET, guess);
            Sleep(1000);
            continue;
        }

        guessed_flags[(unsigned char)guess] = 1;
        strncat(guessed, &guess, 1);

        int found = 0;
        for (int i = 0; i < len; i++) {
            if (word[i] == guess && display[i] == '_') {
                display[i] = guess;
                correct++;
                found = 1;
            }
        }

        if (!found) wrong++;
    }

    system("cls || clear");
    drawHangman(wrong);
    if (correct == len) {
        printf(GREEN "\nYou won! The word was: %s\n" RESET, word);
        player->wins++;
    } else {
        printf(RED "\nGame over. The word was: %s\n" RESET, word);
        player->losses++;
    }

    printf("\nPress Enter to return...\n");
    getchar();
}

void showInstructions() {
    printf(BLUE "=== Instructions ===\n" RESET);
    printf("1. Guess the hidden word by entering one letter at a time.\n");
    printf("2. You have %d incorrect attempts.\n", MAX_ATTEMPTS);
    printf("3. If you complete the word before max attempts, you win!\n");
}

int loadPlayerStats(Player players[], int *count) {
    FILE *file = fopen(SCORE_FILE, "r");
    if (!file) return 0;

    *count = 0;
    while (fscanf(file, "%[^,],%d,%d\n", players[*count].name, &players[*count].wins, &players[*count].losses) == 3) {
        (*count)++;
    }
    fclose(file);
    return 1;
}

void savePlayerStats(Player players[], int count) {
    FILE *file = fopen(SCORE_FILE, "w");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%d,%d\n", players[i].name, players[i].wins, players[i].losses);
    }
    fclose(file);
}

Player* findOrCreatePlayer(Player players[], int *count, const char *name) {
    for (int i = 0; i < *count; i++) {
        if (strcmp(players[i].name, name) == 0) {
            return &players[i];
        }
    }
    strcpy(players[*count].name, name);
    players[*count].wins = 0;
    players[*count].losses = 0;
    (*count)++;
    return &players[*count - 1];
}

int compareByWins(const void *a, const void *b) {
    Player *p1 = (Player *)a;
    Player *p2 = (Player *)b;
    return p2->wins - p1->wins;
}

void showLeaderboard() {
    Player players[100];
    int count = 0;
    if (!loadPlayerStats(players, &count)) {
        printf(RED "No leaderboard data.\n" RESET);
        return;
    }

    qsort(players, count, sizeof(Player), compareByWins);

    printf(BLUE "=== Leaderboard ===\n" RESET);
    for (int i = 0; i < count; i++) {
        printf("%d. " GREEN "%s" RESET " - Wins: %d | Losses: %d\n", i + 1, players[i].name, players[i].wins, players[i].losses);
    }

    printf("\nPress Enter to return...\n");
    getchar();
}
