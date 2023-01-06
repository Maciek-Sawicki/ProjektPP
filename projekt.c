#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define rozmiar_ciagu 60
#define zakupy_rozmiar 10
#define ilosc_sztuk 70
#define max_cena 4


typedef struct Produkt_sklep {
    int lp;
    char identyfikator_produktu[rozmiar_ciagu+1];
    char nazwa[rozmiar_ciagu+1];
    float cena;
    int liczba_sztuk;
    struct Produkt *nast;
} Element_listy;

typedef Element_listy* Produkt;

Produkt Odczyt_z_pliku(const char *nazwa){
    int i;

    FILE* plik = fopen(nazwa, "r");
    if (plik == NULL)
    {
        printf("Blad otwarcia pliku '%s'\n", nazwa);
        return NULL;
    }

    Produkt pierwszy_produkt = NULL, ostatni_produkt;

    int lp1;
    char identyfikator_produktu1[rozmiar_ciagu+1];
    char nazwa1[rozmiar_ciagu+1];
    float cena1;
    int liczba_sztuk1;

    while (fscanf(plik, "%d%s%s%f%d", &lp1, identyfikator_produktu1, nazwa1, &cena1, &liczba_sztuk1) == 5) {
        Produkt nowy = (Produkt) malloc(sizeof(Element_listy));
        nowy->lp = lp1;
        strcpy(nowy->identyfikator_produktu, identyfikator_produktu1);
        strcpy(nowy->nazwa, nazwa1);
        nowy->cena = cena1;
        nowy->liczba_sztuk = liczba_sztuk1;
        nowy->nast =NULL;

        if (pierwszy_produkt == NULL)
        {
            pierwszy_produkt = nowy;
            ostatni_produkt= nowy;
        }
        else
        {
            ostatni_produkt->nast = nowy;
            ostatni_produkt = nowy;
        }
    }
    fclose(plik);

    return pierwszy_produkt;
}



void czytaj_zakupy_z_pliku(const char *nazwa, char zakupy[][rozmiar_ciagu]) {
    int i =0, dlugosc_pliku;
    FILE *plik = fopen(nazwa, "r");

    while (fgets(zakupy[i], rozmiar_ciagu, plik)) {
        zakupy[i][strlen(zakupy[i])-1] = '\0';
        i++;
    }
    fclose(plik);
}

void wypisz_liste(Produkt pierwszy) {
    Produkt pom = pierwszy;
    while (pom!=NULL) {
        printf("%d ", pom->lp);
        printf("%s ", pom->identyfikator_produktu);
        printf("%s ", pom->nazwa);
        printf("%f ", pom->cena);
        printf("%d\n", pom->liczba_sztuk);
        pom = pom->nast;
    }
}

float policz_srednia_cene(Produkt pierwszy) {
    float srednia, suma = 0;
    int licznik = 0;
    Produkt pom = pierwszy;
    while (pom!=NULL) {
        suma += pom->cena;
        licznik++;
        pom = pom->nast;
    }
    srednia = suma/licznik;
    return srednia;
}

Produkt tworz_liste_progi(Produkt pierwszy, int prog_ilosc, float prog_cena) {
    if (pierwszy == NULL)
        return 1;

    Produkt pierwszy_produkt = NULL, ostatni_produkt;
    Produkt pom = pierwszy;

    while (pom!=NULL) {
        if ((pom->liczba_sztuk>prog_ilosc)&&(pom->cena<prog_cena))
        {
            Produkt nowy = (Produkt) malloc(sizeof(Element_listy));
            nowy->lp = pom->lp;
            strcpy(nowy->identyfikator_produktu, pom->identyfikator_produktu);
            strcpy(nowy->nazwa, pom->nazwa);
            nowy->cena = pom->cena;
            nowy->liczba_sztuk = pom->liczba_sztuk;
            nowy->nast =NULL;

            if (pierwszy_produkt == NULL)
            {
                pierwszy_produkt = nowy;
                ostatni_produkt= nowy;
            }
            else
            {
                ostatni_produkt->nast = nowy;
                ostatni_produkt = nowy;
            }
        }
        pom = pom->nast;
    }
    return pierwszy_produkt;
}

float policz_cene_za_zakupy(const char *nazwa, Produkt pierwszy, char zakupy[][rozmiar_ciagu]) {

    float cena_za_zakupy = 0;
    int i = 0;

    Produkt pom = pierwszy;

    while (pom!=NULL) {
        for (i=0;i<zakupy_rozmiar;i++) {
            if (strcmp(zakupy[i], pom->identyfikator_produktu)==0) {
                cena_za_zakupy += pom->cena;
            }
        }
        pom = pom->nast;
    }

    return cena_za_zakupy;
}

int main () {

    char zakupy[zakupy_rozmiar][rozmiar_ciagu];
    Produkt sklep1 = Odczyt_z_pliku("sklep1.txt");
    Produkt sklep2 = Odczyt_z_pliku("sklep2.txt");

    float srednia_sklep_1 = policz_srednia_cene(sklep1);
    float srednia_sklep_2 = policz_srednia_cene(sklep2);

    printf("Srednia w sklepie nr 1 = %.2f zl\n", srednia_sklep_1);
    printf("Srednia w sklepie nr 2 = %.2f zl\n", srednia_sklep_2);
    printf("\n");

    if (srednia_sklep_1<srednia_sklep_2) {
        printf("Srednia cena za produkty jest nizsza w sklepie nr 1\n");
        printf("\n");
        Produkt nowa_lista = tworz_liste_progi(sklep1, ilosc_sztuk, max_cena);
        printf("Lista produktow ktorych cena jest ponizej %d zl i ilosc na stanie jest wieksza od %d sztuk:\n", max_cena, ilosc_sztuk);
        wypisz_liste(nowa_lista);
        printf("\n");
        czytaj_zakupy_z_pliku("zakupy1.txt", zakupy);
        float cena = policz_cene_za_zakupy("zakupy1.txt", sklep1, zakupy);
        printf("Cena za zakupy z listy to %.2f zl", cena);

    }
    else {
        if (srednia_sklep_2<srednia_sklep_1) {
            printf("Srednia cena za produkty jest nizsza w sklepie nr 2\n");
            printf("\n");
            Produkt nowa_lista = tworz_liste_progi(sklep2, ilosc_sztuk, max_cena);
            printf("Lista produktow ktorych cena jest ponizej %d zl i ilosc na stanie jest wieksza od %d sztuk:\n", max_cena, ilosc_sztuk);
            wypisz_liste(nowa_lista);
            printf("\n");
            czytaj_zakupy_z_pliku("zakupy2.txt", zakupy);
            float cena = policz_cene_za_zakupy("zakupy2.txt", sklep2, zakupy);
            printf("Cena za zakupy z listy to %.2f zl", cena);
        }
    }

    return 0;
}
