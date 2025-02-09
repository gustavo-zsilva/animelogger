#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char name[100];
    char author[100];
    char genre[100];
    int releaseYear;
} Anime;

Anime animeList[300];

void toLower(char *string) {
    for (int i = 0; string[i] != '\0'; i++) {
        string[i] = tolower(string[i]);
    }
}

void populateAnimeList() {
    FILE *arquivo;
    arquivo = fopen("lista.txt", "r");

    char linha[100];
    char *token;
    int index = 0;

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo. Reinicie o programa e tente novamente, ou cheque se o arquivo 'lista.txt' existe na raiz do projeto.\n");
        exit(1);
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        token = strtok(linha, "|");
        if (token == NULL) {
            printf("Erro: Ignorando linha nome\n");
            continue;
        }
        strcpy(animeList[index].name, token);

        token = strtok(NULL, "|");
        if (token == NULL) {
            printf("Erro: Ignorando linha autor\n");
            continue;
        }
        strcpy(animeList[index].author, token);

        token = strtok(NULL, "|");
        if (token == NULL) {
            printf("Erro: Ignorando linha gênero\n");
            continue;
        }
        strcpy(animeList[index].genre, token);

        token = strtok(NULL, "|");
        if (token == NULL) {
            printf("Erro: Ignorando linha ano\n");
            continue;
        }
        animeList[index].releaseYear = atoi(token);

        index++;
    }

    fclose(arquivo);
}

int writeReview() {
    int grade = -1;



    while (grade < 0 || grade > 10) {
        printf("Qual nota você daria para este anime, de 0 a 10? ");
        scanf("%d", &grade);

        if (grade >= 0 && grade <= 10) break;

        printf("A nota que você escolheu é invalida. Por favor tente novamente.\n");
    }

    printf("");
}

int catalogAnime() {
    Anime watchedAnime;

    char name[100];
    int animeIndexSelection = 1;
    char lowercaseAnimeListName[100];

    printf("Vamos catalogar o anime que voce já assistiu!\n");
    printf("Qual o nome do anime assistido? ");

    // Remove \n do scanf anterior
    getchar();
    fgets(name, sizeof(name), stdin);
    name[strlen(name) - 1] = '\0';

    toLower(name);

    printf("-------------------------------------------\n");
    // Mostra animes com nome relacionado
    for (int i = 1; i <= 300; i++) {
        strcpy(lowercaseAnimeListName, animeList[i].name);
        toLower(lowercaseAnimeListName);

        if (strstr(lowercaseAnimeListName, name) != NULL) {
            printf("%d - %s | %s | %s | %d\n", i, animeList[i].name, animeList[i].author, animeList[i].genre, animeList[i].releaseYear);
        }
    }
    printf("-------------------------------------------\n");
    printf("Digite o numero relacionado ao anime/temporada que você assistiu: (-1 para voltar ao menu): ");
    scanf("%d", &animeIndexSelection);

    if (animeIndexSelection == -1) {
        return 0;
    }

    printf("Você selecionou %s (%d)!\n", animeList[animeIndexSelection].name, animeList[animeIndexSelection].releaseYear);

    writeReview();
}

void menu() {
    int opt = 0;

    printf("⁍ MENU ⁌\n");
    printf("O que deseja fazer?\n");
    printf("-------------------------------------------\n");
    printf("1 - Quero catalogar um anime que já assisti!\n");
    printf("2 - Quero assistir um anime!\n");
    printf("3 - Quero sair do programa!\n");
    printf("-------------------------------------------\n");
    printf(" ");
    printf("Digite a opção desejada: ");
    scanf("%d", &opt);

    if (opt == 1) {
        // Catalogar anime já assistido
        catalogAnime();
    } else if (opt == 2) {
        // Watchlist
    } else if (opt == 3) {
        // Sair do programa
        printf("Saindo do programa. Volte com mais animes para cadastrar!");
        exit(1);
    } else {
        printf("Opção inválida, tente novamente.\n");
        menu();
    }
}

int main()
{
    printf("Bem vindo ao ANIMELOGGER, seu espaço para catalogar animes!\n");
    populateAnimeList();
    menu();

    return 0;
}
