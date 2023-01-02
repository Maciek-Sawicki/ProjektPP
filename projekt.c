#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define rozmiar_ciagu 60


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

int main () {

    Adres sklep1 = Odczyt_z_pliku("sklep1.txt");
    Adres sklep2 = Odczyt_z_pliku("sklep2.txt");

    wypisz_liste(sklep1);
   // wypisz_liste(sklep2);
    float srednia_sklep_1 = policz_srednia_cene(sklep1);
    float srednia_sklep_2 = policz_srednia_cene(sklep2);

    printf("Srednia1 = %f", srednia_sklep_1);
    printf("\n");
    //printf("Srednia2 = %f", srednia_sklep_2);

    //wypisz_liste(sklep1);
// funkcja do oczyszczenia pamieci
//sprawpozdanie markdown stackedit.io
//readme.so

    return 0;
}
