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
    char anime[100];
    int grade;
    char watchDate[12];
    char text[500];
} Review;

Anime animeList[ANIME_LIST_SIZE];
Review reviews[ANIME_LIST_SIZE];
Anime watchlist[ANIME_LIST_SIZE];

int reviewIndex = 0;
int watchlistIndex = 0;

// UNIX
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
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        token = strtok(linha, "|");
        if (token == NULL) continue;
        strcpy(animeList[index].name, token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(animeList[index].author, token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(animeList[index].genre, token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        animeList[index].releaseYear = atoi(token);

        index++;
    }

    printf("\n");

    fclose(arquivo);
}

void populateReviews() {
    FILE *arquivo;
    arquivo = fopen("reviews.txt", "r");

    if (arquivo == NULL) return;

    char linha[600];
    char *token;
    int index = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        token = strtok(linha, "|");
        if (token == NULL) continue;
        strcpy(reviews[index].anime, token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        reviews[index].grade = atoi(token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(reviews[index].watchDate, token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(reviews[index].text, token);

        index++;
        reviewIndex++;
    }

    fclose(arquivo);
}

void populateWatchlist() {
    FILE *arquivo;
    arquivo = fopen("watchlist.txt", "r");

    if (arquivo == NULL) return;

    char linha[300];
    char *token;
    int index = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        token = strtok(linha, "|");
        if (token == NULL) continue;
        strcpy(watchlist[index].name, token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(watchlist[index].author, token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(watchlist[index].genre, token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        watchlist[index].releaseYear = atoi(token);

        index++;
        watchlistIndex++;
    }

    fclose(arquivo);
}

int removeReview(int index) {
    int offsetIndex = 0;

    for (int i = 0; i < reviewIndex; i++) {
        if (i == index) {
            offsetIndex++;
        }

        reviews[i] = reviews[offsetIndex];
        offsetIndex++;
    }

    // Atualiza tamanho do array
    reviewIndex--;
}

int removeAnimeFromWatchlist(int index) {
    int offsetIndex = 0;

    for (int i = 0; i < watchlistIndex; i++) {
        if (i == index) {
            offsetIndex++;
        }

        watchlist[i] = watchlist[offsetIndex];
        offsetIndex++;
    }

    // Atualiza tamanho do array
    watchlistIndex--;
}

int overwriteReviewFile() {
    FILE *arquivo;
    arquivo = fopen("reviews.txt", "w");

    for (int i = 0; i < reviewIndex; i++) {
        fprintf(arquivo, "%s|%d|%s|%s", reviews[i].anime, reviews[i].grade, reviews[i].watchDate, reviews[i].text);
    }

    fclose(arquivo);
}

int overwriteWatchlistFile() {
    FILE *arquivo;
    arquivo = fopen("watchlist.txt", "w");

    for (int i = 0; i < watchlistIndex; i++) {
        fprintf(arquivo, "%s|%s|%s|%d\n", watchlist[i].name, watchlist[i].author, watchlist[i].genre, watchlist[i].releaseYear);
    }

    fclose(arquivo);
}

int editReviews() {
    if (reviewIndex == 0) {
        printf("\033[31mVocê ainda não possui nenhuma review cadastrada.\033[0m\n");
        return 0;
    }

    int opt = -1;

    printf("\033[1mSUAS REVIEWS (%d TOTAL):\033[0m\n", reviewIndex);

    for (int i = 0; i < reviewIndex; i++) {
        printf("\n\033[93m#%d %s\033[0m - Nota %d\n", i, reviews[i].anime, reviews[i].grade);
        printf("\033[93m[");
        for (int j = 0; j < 10; j++) {
            if (j < reviews[i].grade) {
                printf("■");
            } else {
                printf("□");
            }
        }
        printf("]\033[0m\n");
        printf("\033[3m%s\033[0m\n", reviews[i].text);
        printf("Data que acabou de assistir: %s\n", reviews[i].watchDate);
    }

    printf("-------------------------------------------\n");

    printf("\033[3m1 - Voltar ao menu | 2 - Excluir review\033[0m\n");
    scanf("%d", &opt);
    getchar();

    if (opt == 1) {
        system("clear");
        return 0;
    } else if (opt == 2) {
        int excludeIndex = 0;

        printf("Digite o índice do review que deseja excluir: ");
        scanf("%d", &excludeIndex);
        getchar();

        if (excludeIndex < 0 || excludeIndex > reviewIndex) {
            printf("\033[31mÍndice inválido. Por favor, retorne e tente novamente.\033[0m\n");
            return 0;
        }

        printf("Removendo sua review do banco de dados ultra secreto...\n");
        sleep(3);

        removeReview(excludeIndex);
        overwriteReviewFile();

        printf("\033[94mReview removida com sucesso! Retornando ao menu...\033[0m\n");
    } else {
        printf("\033[31mÍndice inválido. Por favor, retorne e tente novamente.\033[0m\n");
    }

    return 0;
}

int editWatchlist() {

    if (watchlistIndex == 0) {
        printf("\033[31mVocê ainda não possui nenhum anime na sua watchlist.\033[0m\n");
        return 0;
    }

    int opt = -1;

    printf("\033[1mSUA WATCHLIST (%d TOTAL ANIMES):\033[0m\n", watchlistIndex);

    for (int i = 0; i < watchlistIndex; i++) {
        printf("\n\033[93m#%d %s (%d)\033[0m\n", i, watchlist[i].name, watchlist[i].releaseYear);

        printf("\033[3m%s / %s\033[0m\n", watchlist[i].author, watchlist[i].genre);
    }

    printf("-------------------------------------------\n");

    printf("\033[3m1 - Voltar ao menu | 2 - Excluir anime da watchlist\033[0m\n");
    scanf("%d", &opt);
    getchar();

    if (opt == 1) {
        system("clear");
        return 0;
    } else if (opt == 2) {
        int excludeIndex = 0;

        printf("Digite o índice do anime que deseja excluir: ");
        scanf("%d", &excludeIndex);
        getchar();

        if (excludeIndex < 0 || excludeIndex > watchlistIndex) {
            printf("\033[31mÍndice inválido. Por favor, retorne e tente novamente.\033[0m\n");
            return 0;
        }

        printf("Removendo seu anime da watchlist do banco de dados ultra secreto...\n");
        sleep(3);

        removeAnimeFromWatchlist(excludeIndex);
        overwriteWatchlistFile();

        printf("\033[94mAnime removido com sucesso! Retornando ao menu...\033[0m\n");
    } else {
        printf("\033[31mÍndice inválido. Por favor, retorne e tente novamente.\033[0m\n");
    }

    return 0;
}

void writeReviewToFile(Review review) {
    FILE *arquivo;

    arquivo = fopen("reviews.txt", "a");
    fprintf(arquivo, "%s|%d|%s|%s\n", review.anime, review.grade, review.watchDate, review.text);
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

    strcpy(review.anime, selectedAnime.name);
    review.grade = grade;
    strcpy(review.watchDate, watchDate);
    strcpy(review.text, reviewText);

    printf("Fazendo o upload de sua review ao ultra secreto banco de dados...\n");
    writeReviewToFile(review);
    reviews[reviewIndex] = review;
    reviewIndex++;
    sleep(3);

    printf("\033[94mPronto! Sua review de %s foi cadastrada com sucesso!\n\033[0m", selectedAnime.name);
    printf("Retornando ao menu principal.\n");
    return 0;
}

int searchAnime(char *name) {
    // Buffer de cópia para não modificar a prop do anime na lista original
    char lowercaseAnimeListName[100];
    int foundAnime = 0;

    printf("-------------------------------------------\n");

    for (int i = 0; i < ANIME_LIST_SIZE; i++) {
        strcpy(lowercaseAnimeListName, animeList[i].name);

        toLower(lowercaseAnimeListName);
        toLower(name);

        if (strncmp(lowercaseAnimeListName, name, strlen(name)) == 0) {
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
    watchlist[watchlistIndex] = anime;
    watchlistIndex++;
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
        printf("Digite o número da opção desejada: ");
        scanf("%d", &opt);
        getchar();

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
            system("clear");
            editWatchlist();
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
    printf("\033[95mBem-vindo ao ANIMELOGGER (Linux V.), seu espaço para catalogar animes!\033[m\n");
    printf("\033[95mRepositório do Github: https://github.com/gustavo-zsilva/animelogger\033[m\n");
    printf("\033[95mFeito com ♥ por Gustavo Z.\033[m\n");
    printf("\n");

    populateAnimeList();
    populateReviews();
    populateWatchlist();
    menu();

    return 0;
}
