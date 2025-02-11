# AFD & Compilator Regex

Acest repository conține două componente principale:
1. **Un automat finit determinist (AFD)** implementat în C++.
2. **Un compilator pentru expresii regulate** implementat în C#.

## Automat Finit Determinist (AFD)

Automatul finit determinist (AFD) este o implementare în C++ care permite verificarea dacă un șir de caractere este acceptat de un anumit automat determinist.

### Caracteristici:
- Implementat în C++.
- Utilizează structuri de date eficiente pentru reprezentarea stărilor și tranzițiilor.
- Permite citirea și definirea unui automat dintr-un fișier de configurare.
- Suportă validarea unui șir pe baza automatului definit.

## Compilator Regex (C#)

Compilatorul de expresii regulate este o aplicație scrisă în C# care transformă un regex într-un automat finit echivalent.

### Caracteristici:
- Implementat în C# .NET.
- Suportă expresii regulate standard.
- Convertește regex-ul într-un automat finit echivalent.
- Generează cod C++ pentru AFD-ul rezultat.
