#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // UNIX
#include <unistd.h> // UNIX
#include <regex.h>

#define ANIME_LIST_SIZE 300

typedef struct {
    char name[100];
    char author[100];
    char genre[100];
    int releaseYear;
} Anime;

typedef struct {
    char day[2];
    char month[2];
    char year[4];
} Date;

typedef struct {
    Anime anime;
    int grade;
    char watchDate[12];
    char text[500];
} Review;

Anime animeList[300];

// UNIX
void toLower(char *string) {
    for (int i = 0; string[i] != '\0'; i++) {
        string[i] = tolower(string[i]);
    }
}

/*
void splitDate(char data[10]) {
    char *token;

    strtok();
}
*/

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

int editReviews() {
    FILE *arquivo;
    arquivo = fopen("reviews.txt", "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo. Reinicie o programa e tente novamente, ou cheque se o arquivo 'lista.txt' existe na raiz do projeto.\n");
        exit(1);
    }

    char linha[600];
    char *token;
    int index = 0;

    printf("Suas reviews:\n");

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char animeName[100];
        int grade;
        char watchDate[12];
        char text[500];

        token = strtok(linha, "|");
        strcpy(animeName, token);

        token = strtok(NULL, "|");
        grade = atoi(token);

        token = strtok(NULL, "|");
        strcpy(watchDate, token);

        token = strtok(NULL, "|");
        strcpy(text, token);

        printf("\n\033[92m%s\033[0m - Nota %d\n", animeName, grade);
        printf("\033[92m[");
        for (int i = 0; i < 10; i++) {
            if (i < grade) {
                printf("X");
            } else {
                printf("-");
            }
        }
        printf("]\033[0m\n");
        printf("\033[3m%s\033[0m", text);
        printf("Data que acabou de assistir: %s\n", watchDate);

        index++;
    }

    printf("1 - Voltar ao menu");

    fclose(arquivo);
}

void writeReviewToFile(Review review) {
    FILE *arquivo;

    arquivo = fopen("reviews.txt", "a");
    fprintf(arquivo, "%s|%d|%s|%s\n", review.anime.name, review.grade, review.watchDate, review.text);
    fclose(arquivo);
}

void writeAnimeToWatchlist(Anime anime) {
    FILE *arquivo;

    arquivo = fopen("watchlist.txt", "a");
    fprintf(arquivo, "%s|%s|%s|%d\n", anime.name, anime.author, anime.genre, anime.releaseYear);
    fclose(arquivo);
}

int writeReview(Anime selectedAnime) {
    Review review;

    int grade = -1;
    char watchDate[12];
    char reviewText[500];

    while (grade < 0 || grade > 10) {
        printf("\n");
        printf("Qual nota você daria para este anime, de 0 a 10? ");
        scanf("%d", &grade);

        if (grade >= 0 && grade <= 10) break;

        printf("\033[31mA nota que você escolheu é invalida. Por favor tente novamente.\033[0m\n");
    }


    printf("Digite a data que você terminou de assistir o anime escolhido (FORMATO: dd/mm/aaaa): ");
    getchar();
    fgets(watchDate, sizeof(watchDate), stdin);
    watchDate[strlen(watchDate) - 1] = '\0';

    printf("Faça uma review sobre o anime assistido, ou deixe em branco se preferir (máx. 500 caracteres): ");
    fgets(reviewText, sizeof(reviewText), stdin);
    reviewText[strlen(reviewText) - 1] = '\0';

    review.anime = selectedAnime;
    review.grade = grade;
    strcpy(review.watchDate, watchDate);
    strcpy(review.text, reviewText);

    printf("Fazendo o upload de sua review ao ultra secreto banco de dados...\n");
    writeReviewToFile(review);
    sleep(3);

    printf("Pronto! Sua review de %s foi cadastrada com sucesso!\n", selectedAnime.name);
    printf("Retornando ao menu principal.\n");
    return 0;
}

int searchAnime(char *name) {
    // Buffer de cópia para não modificar a prop do anime na lista original
    char lowercaseAnimeListName[100];
    int foundAnime = 0;

    toLower(name);

    printf("-------------------------------------------\n");

    for (int i = 0; i < ANIME_LIST_SIZE; i++) {
        strcpy(lowercaseAnimeListName, animeList[i].name);
        toLower(lowercaseAnimeListName);

        if (strstr(lowercaseAnimeListName, name) != NULL) {
            printf("\033[97m%d - %s | %s | %s | %d\033[0m\n", i, animeList[i].name, animeList[i].author, animeList[i].genre, animeList[i].releaseYear);
            foundAnime = 1;
        }
    }

    return foundAnime;
}

int catalogAnime() {

    Anime watchedAnime;
    int foundAnime = 0;
    int animeIndexSelection = 1;

    while (!foundAnime) {
        char name[100];

        printf("Vamos catalogar o anime que voce já assistiu!\n");
        printf("Qual o nome do anime assistido? ");

        // Remove \n do scanf anterior
        getchar();
        fgets(name, sizeof(name), stdin);
        name[strlen(name) - 1] = '\0';


        // Pesquisa o anime
        foundAnime = searchAnime(name);

        if (!foundAnime) {
            printf("Anime não encontrado. Verifique a ortografia e tente novamente.\n");
            printf("-------------------------------------------\n");
        }
    }

    printf("-------------------------------------------\n");
    printf("Digite o numero relacionado ao anime/temporada que você assistiu (-1 para voltar ao menu): ");
    scanf("%d", &animeIndexSelection);

    system("clear");

    if (animeIndexSelection == -1) return 0;

    printf("\033[92mVocê selecionou %s (%d)!\033[0m\n", animeList[animeIndexSelection].name, animeList[animeIndexSelection].releaseYear);

    watchedAnime = animeList[animeIndexSelection];

    writeReview(watchedAnime);
}

int addToWatchlist() {
    int foundAnime = 0;
    int animeIndexSelection = -1;
    Anime anime;

    printf("Vamos adicionar seu anime à Watchlist!\n");


    while (!foundAnime) {
        char animeName[100];

        printf("Digite o nome do anime que deseja assistir: ");

        getchar();
        fgets(animeName, sizeof(animeName), stdin);
        animeName[strlen(animeName) - 1] = '\0';

        foundAnime = searchAnime(animeName);

        if (!foundAnime) {
            printf("Anime não encontrado. Verifique a ortografia e tente novamente.\n");
            printf("-------------------------------------------\n");
        }
    }

    printf("-------------------------------------------\n");
    printf("Digite o numero relacionado ao anime/temporada que você assistiu (-1 para voltar ao menu): ");
    scanf("%d", &animeIndexSelection);

    system("clear");

    if (animeIndexSelection == -1) return 0;

    anime = animeList[animeIndexSelection];

    writeAnimeToWatchlist(anime);
    printf("\033[92mO anime %s (%d) foi adicionado com sucesso à sua watchlist!\033[0m\n", animeList[animeIndexSelection].name, animeList[animeIndexSelection].releaseYear);
}

void menu() {
    // Loop infinito. Sempre que uma função retornar algo, mostra o menu novamente.
    while (1) {
        int opt = 0;

        printf("\033[3m⁍ MENU ⁌\033[0m\n");
        printf("O que deseja fazer?\n");
        printf("-------------------------------------------\n");
        printf("1 - Quero catalogar um anime que já assisti!\n");
        printf("2 - Quero assistir um anime!\n");
        printf("3 - Quero editar minhas reviews!\n");
        printf("4 - Quero visualizar/excluir animes da minha watchlist!\n");
        printf("5 - Quero sair do programa!\n");
        printf("-------------------------------------------\n");
        printf("Digite a opção desejada: ");
        scanf("%d", &opt);

        if (opt == 1) {
            // Catalogar anime já assistido
            system("clear");
            catalogAnime();
        } else if (opt == 2) {
            // Watchlist
            system("clear");
            addToWatchlist();
        } else if (opt == 3) {
            // Edit reviews
            system("clear");
            editReviews();
        } else if (opt == 4) {
            // Manage watchlist
        } else if (opt == 5) {
            // Sair do programa
            printf("Saindo do programa. Volte com mais animes para cadastrar!");
            exit(1);
        } else {
            system("clear");
            printf("\033[31mOpção inválida, tente novamente.\n\033[0m");
        }
    }
}

int main()
{
    printf("\033[95mBem vindo ao ANIMELOGGER, seu espaço para catalogar animes!\033[m\n");
    populateAnimeList();
    menu();

    return 0;
}
