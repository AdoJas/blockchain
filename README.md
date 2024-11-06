# SUPAPRASTINTAS BLOCKCHAIN (CENTRALIZUOTAS)

## UŽDUOTIES TIKSLAS

Šios užduoties tikslas – sukurti supaprastintą blokų grandinę (angl. blockchain), kurios duomenų sluoksnio struktūra pavaizduota
žemiau:

/////Ideti screenshota/////

## KAS BUVO ĮGYVENDINTA

- **`Block` klasė** – klasė, kuri realizuoja bloko struktūrą. Kiekvienas blokas turi savo unikalų `index` (indeksą), `timestamp` (laiko žymą), `merlkRoot` (Merkle šaknį), `previousHash` (prieš tai buvusio bloko hash'ą), `hash` (šio bloko hash'ą) ir `difficulty` (sudetingumą).
``` 
--- Latest Block ---
==================== Block Header ====================
Current Block Hash : 00000e628a25e91e3f2bc897f1b3d4f6711b481b75f7afea2e69ecc57aad301c
Previous Block Hash: 0
Merkle Root Hash   :
Timestamp          : 2024/11/06 16:23:43
Nonce              : 57398
======================================================
Transactions       : 0
```
- **`Blockchain` klasė** – klasė, kuri realizuoja blokų grandinę. Kiekvienas blokas yra pridedamas prie blokų grandinės tik tada, kai yra tinkamai apskaičiuotas `hash` pagal `difficulty`.
```
==================== Block Header ====================
Previous Block Hash: 00000e628a25e91e3f2bc897f1b3d4f6711b481b75f7afea2e69ecc57aad301c
Merkle Root Hash   :
Timestamp          : 2024/11/06 16:29:03
Nonce              : 0
======================================================
Block mined by thread 11 with nonce: 3937078063
Block hash after mining: 000007ba6333b5cef9d323e8ef1a63008757eb1ac26ee2267b772afbb9d210d4
Successfully mined a new block with 10 transactions.
==================== Block Header ====================
Current Block Hash : 000007ba6333b5cef9d323e8ef1a63008757eb1ac26ee2267b772afbb9d210d4
Previous Block Hash: 00000e628a25e91e3f2bc897f1b3d4f6711b481b75f7afea2e69ecc57aad301c
Merkle Root Hash   : b339be446fb76396d61b7ca572659ea5a3df04087b9095ec1d173c5e7fa80834
Timestamp          : 2024/11/06 16:29:03
Nonce              : 3937078063
======================================================
```

- **`MerkleTree` klasė** – klasė, kuri realizuoja binarųjį Merkle medį. Kiekvienas blokas turi savo transakcijų sąrašą, kuris yra naudojamas Merkle medžio sudarymui.
- **`Transaction` klasė** – klasė, kuri realizuoja transakcijos struktūrą. Kiekviena transakcija turi savo unikalų `transactionID` (transakcijos identifikatorius), `senderPublicKey` (siuntėjo viešąjį raktą), `receiverPublicKey` (gavėjo viešąjį raktą), `amount` (sumą) ir `timestamp` (laiko atžymą).
```
================= Generated Transaction =================
Sender:        2c2add273b250d1f6207d6f4405d9a4da90b01eede01c4fcff7e06c0f754f7b9
Receiver:      a231e9aef1534fe3f600304e83d7a445f9227e5881c83c02fe08fe273e946a7d
Amount:        19896 units
=========================================================
```
- **`UTXO` klasė** – klasė, kuri realizuoja nepanaudotų transakcijų išėjimų sąrašą (angl. Unspent Transaction Outputs). Kiekviena transakcija turi savo išėjimų (outputs) sąrašą, kuris yra naudojamas nepanaudotų transakcijų išėjimų sąrašo sudarymui. Turi `utxoID` (Tokeno identifikatorius), `owner` (UTXO tokeno savininko public key) ir `amount` (sumą).
- **`User` klasė** – klasė, kuri realizuoja vartotojo struktūrą. Kiekvienas vartotojas turi savo viešąjį raktą.
```
==================== User Generation ====================

------------------- User #1 -------------------
Public Key:    5432f130f3adf32fb30a6a2efd8a2e69527c130e063c3fe3c871ae2f60a8396a
Initial Balance:712907 units
----------------------------------------------------------

------------------- User #2 -------------------
Public Key:    7a5e91c8ee538eadcf261b4b75a2c678c8e56b72db7be6d62511b3090ec5c2f0
Initial Balance:457101 units
----------------------------------------------------------
==========================================================`
```
```
================= User Details =================
Index         : 0
Public Key    : 5432f130f3adf32fb30a6a2efd8a2e69527c130e063c3fe3c871ae2f60a8396a
Balance       : 712907 units
UTXOs         :
  - Amount: 712907 units, Transaction ID: 07eb3631bb24232ff0f1ce1e7b63ee2bcce0c84a6fd0a4c8067f12355759e987
=================================================

================= User Details =================
Index         : 1
Public Key    : 7a5e91c8ee538eadcf261b4b75a2c678c8e56b72db7be6d62511b3090ec5c2f0
Balance       : 457101 units
UTXOs         :
  - Amount: 457101 units, Transaction ID: c4022c3bef9ba5c7af6cd25f3e01e81a4215aa6aea447c67e9a73b87696dac17
=================================================
=========================================================
```
## KAIP VEIKIA PROGRAMA?

1. Paleidus programą, yra sukuriamas Genesis blokas. Randamas tinkamas `hash` pagal `difficulty` ir blokas pridedamas prie blokų grandinės.
2. Generuojame kiek norime vartotojų, jiems priskiriami viešieji raktai, ir atsitiktinis balansas (Vienas utxo tokenas, kuris ir turi tą sumą).
3. Vartotojai atlieka transakcijas vienas tarp kito, transakcija generuojama atsitiktinai, siuntėjas ir gavėjas visada nesutampa, transakcijos vertė visada nuo vieno balanso kiekio iki 125%. Generuojant transakciją. nėra tikrinama, ar funkcija yra validi.
4. Kasame bloką paraleliai, ieškodami tinkamos maišos reikšmės pagal sudėtingumą. Kai randama tinkama maišos reikšmė, blokas pridedamas prie blokų grandinės. Transakcijos tikrinamos, ar jos tinkamos, jei ne, jos į bloką nepridedamos, 

## BLOCKCHAIN NAVIGACIJA

1. Gaima peržiūrėti visą blockchain, visų blokų header informaciją ir transakcijas, kurios yra tame bloke.
2. Gali peržiūrėti visus vartotojus, jų viešus raktus ir balansus.
3. Galima surasti ir peržiūrėti vartotoją pagla indeksą.
4. Galima peržiūrėti visas nepanaudotas transakcijas.
5. Galima peržiūrėti naujausią bloką.
6. Galima peržiūrėti bloką pagal indeksą.

## Blokų kasimas

Galima rinktis, kaip norime kasti bloką. Galima rinktis, ar kasame kandidatus blokus, ar blokus kasame paraleliai.
Paralelus blokų kasimas buvo įgyvendintas su OpenMP biblioteka, kuri leidžia programai naudoti visus turimas CPU branduolius.

Kandidatai blokai kasami sudarant 5 kandidatus blokus, su 100 transakcijų kiekvienam. Blokuose transakcijos gali kartotis. Tada vienas iš blokų atsitiktinai parenkamas, kasamas tol, kol randamas hashas arba baigias laikas arba nonce nėra randamas per tam tikrą bandymų laiką.

## UTXO

UTXO - nepanaudota transakcijos išeitis. UTXO = (Savininko public key, suma) = UTXOID.

Transakcijos generavimo metu, sugeneruota atsitiktinė suma, gaunami visi siuntėjo UTXO, jie sudedami pagal sumą, ir įrašomi į transakciją. Jei šių UTXO suma viršija siunčiamos sumos sumą, gražinamas perteklius kaip dar vienas naujas UTXO siuntėjui.

## HASH FUNKCIJA
Kiekvieno hashavimo metu naudojama hashavimo funkcija, kuri buvo sukurta pirmajame darbe.

https://github.com/AdoJas/blockchainRepo

## GEROSIOS OOP PRAKTIKOS

Naudojama enkapsuliacija, konstruktoriai. Kiekviena klasė turi privačius kintamuosius, kurių pasiekti tiesiogiai negalima, jiems parodyti arba naudoti naudojami "getteriai", kurie gražina norimo kintamojo reikšmę.


