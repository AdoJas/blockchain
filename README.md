# blockchain

## Reikalavimai v0.1 versijai
- [x] Realizuokite supaprastintą "centralizuotą" blokų grandinę. Maišos funkcijai naudokite savo sukurtą funkciją iš pirmosios
užduoties.
- [x] Transakcijų ir blokų kūrimo procesas turi būti matomas (išvedamas į konsolę). Sukurkite funkcijas, kurios leidžia atspausdinti bet
kurią transakciją ir bloką. Išvedimo detalumas ir vizualumas turės įtakos balui. Pavyzdys: https://www.blockchain.com/btc/block/1
- [x] Vietoj dvejetainio Merkle medžio, galite naudoti visų bloko transakcijų ID maišos reikšmę.
- [x] Sukurkite šios versijos releas'ą ir README faile aprašykite, kaip ja naudotis. Šios versijos atskirai atsiskaityti nereikės, bet bus
vertinama, ar ji buvo sukurta laiku.
- [x] Saugumas yra labai svarbus blokų grandinėms, todėl naudokite gerąsias OOP praktikas (pvz., enkapsuliavimą,
konstruktorius, RAII idiomą). Pristatymo metu akcentuokite šiuos aspektus.

## Kaip naudotis
1. Atsisiųskite programos versiją iš Releases.
2. Paleiskite programą.
3. Sekite programos nurodymus.
4. Norėdami išbandyti programą, galite naudoti pavyzdinį kodą iš main.cpp failo.
5. 1 funkcija generuoja pseudo-vertotojus pagal numatytą protokolą.
6. 2 parodo specifinį vartotoją, jo name, public key ir balance.
```cpp
    User user = User("Vardenis Pavardenis", "public_key", 100);
    user.print();
```
arba tą pačią informaciją pavaizduoja visų vartotojų, priklausomai nuo programos vartotojo pasirinkimo.
7. 3 funkcija generuoja blokus iš sugeneruotų transakcijų. Pasirinkus šią opciją, blokai generuojami tol, kol yra laisvu transakcijų transakcijų poole.
8. 4 funkcija išveda parodo laisvas transakcijas, kurios dar nėra įtrauktos į jokį bloką.
9. 5 funkcija kasa blokus, jei yra laisvu transakcijų, kurios nėra įtrauktos į jokį bloką.
10. 6 funkcija išveda specifinį bloką.
11. 7 funkcija parodo specifinę transakciją iš specifinio bloko.
12. 8 funkcija išveda naujausio bloko informaciją.
13. 9 funkcija išveda visų blokų informaciją (visą blockchain).