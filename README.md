# blockchain projekto dokumentacija


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

## 1. Kaip naudotis
1. Atsisiųskite programos versiją iš Releases.
2. Paleiskite programą.
3. Sekite programos nurodymus.
4. Norėdami išbandyti programą, galite naudoti pavyzdinį kodą iš main.cpp failo.
5. 1 funkcija generuoja pseudo-vertotojus pagal numatytą protokolą.
6. 2 parodo specifinį vartotoją, jo name, public key ir balance.
arba tą pačią informaciją pavaizduoja visų vartotojų, priklausomai nuo programos vartotojo pasirinkimo.
7. 3 funkcija generuoja blokus iš sugeneruotų transakcijų. Pasirinkus šią opciją, blokai generuojami tol, kol yra laisvu transakcijų transakcijų poole.
8. 4 funkcija išveda parodo laisvas transakcijas, kurios dar nėra įtrauktos į jokį bloką.
9. 5 funkcija kasa blokus, jei yra laisvu transakcijų, kurios nėra įtrauktos į jokį bloką.
10. 6 funkcija išveda specifinį bloką.
11. 7 funkcija parodo specifinę transakciją iš specifinio bloko.
12. 8 funkcija išveda naujausio bloko informaciją.
13. 9 funkcija išveda visų blokų informaciją (visą blockchain).

## 2. Komponentai

### 2.1 Vartotojas (User)

#### Tikslas
Aprašo vieną vartotoją blokų grandinėje. Kiekvienas vartotojas turi unikalų `publicKey` (viešąjį raktą), o jo balansas netiesiogiai parodomas ir modifikuojamas per UTXO poolo tokenus.

#### Svarbios Funkcijos
- **Konstruktorius**: Generuoja unikalų `publicKey` kiekvienam vartotojui.
- **`display(const UTXOPool& utxoPool) const`**: Rodo vartotojo informaciją ir apskaičiuoja balansą naudodamas UTXO.
- **`generateRandomUser()`**: Sugeneruoja vartotoją su unikaliu viešuoju raktu.

### 2.2 Transakcija ir UTXO

#### Tikslas
Apibrėžia transakcijos struktūrą ir valdo UTXO poola, kuriame yra saugomi visi utxo tokenai.

#### Svarbios Funkcijos
- **Transakcijos Konstruktorius**: Inicializuoja transakciją su unikaliu `txID`, siuntėju, gavėju ir suma.
- **`generateRandomTransaction()`**: Sukuria atsitiktinę transakciją, parenkant utxo tokenus iš galimų UTXO.
- **`UTXOPool::validateTransaction(const Transaction&)`**: Patvirtina, ar transakcija yra galima pagal galimus UTXO.
- **`UTXOPool::applyTransaction(const Transaction&)`**: Atnaujina UTXO poola po transakcijos patvirtinimo ir įvykdinimo.

### 2.3 Blokas (Block)

#### Tikslas
Vienas blokas blokų grandinėje, kuris turi transakcijų sąrašą ir metaduomenis, tokius kaip ankstesnio bloko hash ir Merkle tree reiksme.

#### Svarbios Funkcijos
- **Konstruktorius**: Inicializuoja bloko duomenis.
- **`calculateHash()`**: Apskaičiuoja bloko hash, remdamasis turiniu ir nonce.
- **`displayBlockTransactions()`**: Parodo visas transakcijas bloke.

### 2.4 Blokų Grandinė (Blockchain)

#### Tikslas
Pagrindinis viso šito darbo elementas - blokų grandinė, kuri valdo visus blokus ir transakcijas.
#### Svarbios Funkcijos
- **`Blockchain::addBlock()`**: Prideda iškastą bloką prie grandinės.
- **`Blockchain::isEmpty() const`**: Patikrina, ar blokų grandinė turi blokų.
- **`Blockchain::processTransactions()`**: Validuoja ir apdoroja transakcijas prieš jas kasimo metu įdedant į bloką.
- **`Blockchain::displayBlockchain()`**: Parodo visus blokus ir transakcijas.

### 2.5 Maišos funkcija (Hash)

#### Tikslas
Maišos funkcija iš pirmojo darbo.

#### Svarbios Funkcijos
- **`generateCustomHash(const std::string&)`**: Generuoja pasirinktą hashą iš perduodamo stirgo (pvz., transakcijos duomenų arba UTXO ID).
- **`generateRandomHash()`**: visas veikimo principas patobulintas taip, kad būtų galima generuoti hash'us iš bet kokio string'o.

### 3. Gerosios OOP praktikos
- **Enkapsuliavimas**: Kiekvienas komponentas yra atskirai aprašytas ir atskirtas nuo kitų.
- **Konstruktorius**: Kiekvienas komponentas turi savo konstruktorių, kuris inicializuoja objektą.
- **RAII idiom**: Kiekvienas komponentas yra atsakingas už savo resursų valdymą.


