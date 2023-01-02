#!/usr/bin/python3
import random

nazwy = ["chleb", "bulka", "maslo", "mleko", "serek_wiejski", "twarog", "parowki", "kielbasa", "pomidory", "ogorek",
         "cebula", "papryka", "kapusta", "salata", "woda", "CocaCola", "maka", "cukier", "sok_pomaranczowy", "banany",
         "jablko", "czekolada", "platki_owsiane", "kefir", "jogurt", "wedlina", "szynka"]

id_lista = []

def Generuj_id():
    id = []
    for i in range(10):
        id.append(random.randint(0,9))
    idstr = ''.join(map(str, id))
    return idstr


def Generuj_produkt():
    nazwa = random.choice(nazwy)
    cena = round(random.uniform(1.1, 10.99), 2)
    liczba_sztuk = random.randint(10, 100)
    id = Generuj_id()
    #to do poprawy, może się zdażyć, że będzie takie samo id 
    if id not in id_lista:
        id_lista.append(id)
        return id+" "+str(nazwa)+" "+str(cena)+" "+str(liczba_sztuk)
    else:
        id = Generuj_id()
        id_lista.append(id)
        return id+" "+str(nazwa)+" "+str(cena)+" "+str(liczba_sztuk)

def Generuj_plik(rozmiar, nazwa_sklepu):
    plik = open(nazwa_sklepu, "w")
    for i in range(rozmiar):
        plik.write(str(i+1) +" "+Generuj_produkt() +"\n")

def Generuj_zakupy(rozmiar, nazwa_zakupow):
    zakupy = []
    for i in range(rozmiar):
        zakupy.append(id_lista[random.randint(0,(len(id_lista)-1))])
    plik = open(nazwa_zakupow, "w")
    for i in range(rozmiar):
        plik.write(str(zakupy[i])+"\n")
    
Generuj_plik(100, "sklep1.txt")
Generuj_zakupy(10, "zakupy1.txt")
id_lista.clear()
Generuj_plik(100, "sklep2.txt")
Generuj_zakupy(10, "zakupy2.txt")