#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define rozmiar_ciagu 60
#define zakupy_rozmiar 10

#define ilosc_sztuk 70
#define max_cena 4


typedef struct Produkt {
    int lp;
    char identyfikator_produktu[rozmiar_ciagu+1];
    char nazwa[rozmiar_ciagu+1];
    float cena;
    int liczba_sztuk;
    struct Produkt *nast;
} Element_listy;

typedef Element_listy* Adres;

Adres Odczyt_z_pliku(const char *nazwa){
    int i;

    FILE* plik = fopen(nazwa, "r");
    if (plik == NULL)
    {
        printf("Blad otwarcia pliku '%s'\n", nazwa);
        return NULL;
    }

    Adres pierwszy_produkt = NULL, ostatni_produkt;

    int lp1;
    char identyfikator_produktu1[rozmiar_ciagu+1];
    char nazwa1[rozmiar_ciagu+1];
    float cena1;
    int liczba_sztuk1;

    while (fscanf(plik, "%d%s%s%f%d", &lp1, identyfikator_produktu1, nazwa1, &cena1, &liczba_sztuk1) == 5) {
        Adres nowy = (Adres) malloc(sizeof(Element_listy));
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



int czytaj_zakupy_z_pliku(const char *nazwa, char zakupy[][rozmiar_ciagu]) {
    int i =0, dlugosc_pliku;
    FILE *plik = fopen(nazwa, "r");

    while (fgets(zakupy[i], rozmiar_ciagu, plik)) {
        zakupy[i][strlen(zakupy[i])-1] = '\0';
        i++;
    }
    dlugosc_pliku = i;

    fclose(plik);
    return dlugosc_pliku;
}

void wypisz_liste(Adres pierwszy) {
    Adres pom = pierwszy;
    while (pom!=NULL) {
        printf("%d ", pom->lp);
        printf("%s ", pom->identyfikator_produktu);
        printf("%s ", pom->nazwa);
        printf("%f ", pom->cena);
        printf("%d\n", pom->liczba_sztuk);
        pom = pom->nast;
    }
}

float policz_srednia_cene(Adres pierwszy) {
    float srednia, suma = 0;
    int licznik = 0;
    Adres pom = pierwszy;
    while (pom!=NULL) {
        suma += pom->cena;
        licznik++;
        pom = pom->nast;
    }
    srednia = suma/licznik;
    return srednia;
}

Adres tworz_liste_progi(Adres pierwszy, int prog_ilosc, float prog_cena) {
    if (pierwszy == NULL)
        return 1;

    Adres pierwszy_produkt = NULL, ostatni_produkt;
    Adres pom = pierwszy;

    while (pom!=NULL) {
        if ((pom->liczba_sztuk>prog_ilosc)&&(pom->cena<prog_cena))
        {
            Adres nowy = (Adres) malloc(sizeof(Element_listy));
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

float policz_cene_za_zakupy(const char *nazwa, Adres pierwszy, char zakupy[][rozmiar_ciagu]) {

    float cena_za_zakupy = 0;
    int licznik_zakupy = czytaj_zakupy_z_pliku(nazwa, zakupy);
    int i = 0;

    Adres pom = pierwszy;

    while (pom!=NULL) {
        for (i=0;i<licznik_zakupy;i++) {
            //printf("%s = %s\n", zakupy[i], pom->identyfikator_produktu);
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
    Adres sklep1 = Odczyt_z_pliku("sklep1.txt");
    Adres sklep2 = Odczyt_z_pliku("sklep2.txt");

    float srednia_sklep_1 = policz_srednia_cene(sklep1);
    float srednia_sklep_2 = policz_srednia_cene(sklep2);

    printf("Srednia w sklepie nr 1 = %f\n", srednia_sklep_1);
    printf("Srednia w sklepie nr 2 = %f\n", srednia_sklep_2);

    if (srednia_sklep_1<srednia_sklep_2) {
        printf("Srednia cena za produkty jest nizsza w sklepie nr 1\n");
        Adres nowa_lista = tworz_liste_progi(sklep1, ilosc_sztuk, max_cena);
        printf("Lista produktow ktorych cena jest ponizej %d zl i ilosc na stanie jest wieksza od %d sztuk\n", max_cena, ilosc_sztuk);
        wypisz_liste(nowa_lista);
        int liczba_zakupow = czytaj_zakupy_z_pliku("zakupy1.txt", zakupy);
        float cena = policz_cene_za_zakupy("zakupy1.txt", sklep1, zakupy);
        printf("Cena za zakupy z listy to %.2f", cena);

    }
    else {
        if (srednia_sklep_2<srednia_sklep_1) {
            printf("Srednia cena za produkty jest nizsza w sklepie nr 2\n");
            Adres nowa_lista = tworz_liste_progi(sklep2, ilosc_sztuk, max_cena);
            printf("Lista produktow ktorych cena jest ponizej %d zl i ilosc na stanie jest wieksza od %d sztuk\n", max_cena, ilosc_sztuk);
            wypisz_liste(nowa_lista);
            int liczba_zakupow = czytaj_zakupy_z_pliku("zakupy2.txt", zakupy);
            float cena = policz_cene_za_zakupy("zakupy2.txt", sklep2, zakupy);
            printf("Cena za zakupy z listy to %.2f", cena);

        }
    }


    return 0;
}
