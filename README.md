# Encrypting-and-Template-matching-project
In this project, the main purpose is to encrypt and decrypt a photo and then recognize the handwritten numbers from it.

Romanian:

                Introducere :  Acest proiect abordeaza urmatoarele tematici: criptarea, decriptarea si recunoasterea unor cifre scrise de mana dintr-o imagine in format BMP.

                 Functiile folosite in acest program:
1)	void xorshift32
- aceasta functie primeste un seed care va fi reprezentat de prima valoare din fisierul ce contine cheia secreta. Acesta va fi folosit pentru a genera n numere (n=2**latime**lungime) care vor fi transmise prin intermediul pointerului r inapoi in functia de criptare, acestea fiind esentiale pentru partea de permutare si xorare.
- se ia un indice i=1( deoarece pe pozitia r[0] se afla prima cheie secreta ) care va reprezenta pozitia din r in care punem valoarea obtinuta in urma operatiilor pe biti si incepem sa generam cu ajutorul unei structuri repetitive de tip for n-1 numere ce vor fi puse in pointerul r. Aceste numere sunt generate prin aplicarea anumitelor operatii pe biti (xorarea cu el insusi dupa siftarea la stanga cu 13 biti, xorarea cu el insusi dupa siftarea la dreapta cu 17 biti, xorarea cu el insusi dupa siftarea la stanga cu 5 biti) asupra valorii lui x care este o copie a variabilei seed.
 2) void incarc_imaginea_liniarizata
- urmatoarea functie deschide un fisier binar pentru citire, folosindu-se de nume_fis_sursa in care se afla numele acestuia.
- in prealabil se verifica daca fisierul este valid sau nu, iar apoi se continua cu citirea din header a latimii si inaltimii (de la pozitia 18), urmand mai tarziu sa i se aloce spatiu ((latime)(inaltime)sizeof(unsigned int) ) unui pointer a alocat dinamic de tip unsigned int. In acesta din urma se va retine si prelucra imaginea in conformitate cu cerintele proiectului. De altfel, se va mai concepe un pointer numit v de tip unsigned char in care va fi retinuta partea de header a imaginii, iar acestuia i se va aloca un spatiu de 54sizeof(unsigned char).
- cu ajutorul unui fseek(fin,0,SEEK_SET); se revine la inceputul fisierului binar si astfel se va incepe si citirea partii de header.
- o situatie care poate ridica anumite probleme o reprezinta cea in care imaginea are octeti de completare astfel devenind necesara calcularea padding-ului.
- se va incepe citirea pixelilor din imagine tinand cont de faptul ca aceasta este reprezentata in memorie in mod rasturnat.
- la sfarsit fisierul din care am citit este inchis.
       3) void incarc_o_imagine_in_memorie
- urmatoarea functie deschide un fisier binar pentru scriere, folosindu-se de cale, in care se afla numele acestuia.
- in prealabil se verifica daca fisierul este valid sau nu, iar apoi se continua cu scrierea partii de header in aceasta.
- se va continua cu scrierea valorilor din pointerul a in fisier in conformitate cu felul in care este reprezentata imaginea in memorie, iar apoi cel din urma va fi inchis.
      4) void criptare(
- urmatoarea functie deschide un fisier binar pentru citire, folosindu-se de cod_secret, in care se afla numele acestuia. Din acel fisier se va extrage un seed care va fi folosit pentru partea de permutare si totodata starting value ( sv ), folosit pentru partea de xorare a valorilor din imaginea permutate.
-se va aloca spatiu pentru pointerul r in care vor fi retinute valorile rezultate in urma apelarii functiei xorshift32, iar pe pozitia 0 din r se va afla prima cheie secreta, folosita ca seed.
- se aloca spatiu pentru pointerul p auxiliar care va fi folosit pentru permutare.
- se aloca spatiu pentru pointerul o care va fi folosit in generarea permutarii. Astfel se va retine in o[i] valoarea indicelui i, pentru a putea mai tarziu genera permutarea.
- folosind algoritmul lui Dustenfeld si valorile din pointerul r se va genera o permutare aleatorie retinuta in pointerul o, iar in prealabil va fi folosit pointerul p in care se vor retine conform permutarilor pixelii din a ( p[o[i]]=(*a)[i] )
- pointerul a mai apoi va indica spre valorile din p
-la partea a doua din criptare, primei valori din a i se va aplica operatia de xorare cu cheia secreta, iar apoi cu valoarea din r[(latime)(inaltime)]. Astfel algoritmul va continua pana la ultima valoare din a , xorand fiecare valoare ce reprezinta unul din pixelii imaginii cu un pixel precedent(deja xorat), cu un r[i+(latime)(inaltime)] .
-se inchide la final fisierul din care am citit valorile secrete.
      5) void decriptare
- urmatoarea functie deschide un fisier binar pentru citire, folosindu-se de cod_secret, in care se afla numele acestuia. Din acel fisier se va extrage un seed care va fi folosit pentru partea de permutare si totodata starting value ( sv ), folosit pentru partea de xorare a valorilor din imaginea permutate.
-se va aloca spatiu pentru pointerul r in care vor fi retinute valorile rezultate in urma apelarii functiei xorshift32, iar pe pozitia 0 din r se va afla prima cheie secreta, folosita ca seed.
- se aloca spatiu pentru pointerul p auxiliar care va fi folosit pentru permutare.
-la prima parte din decriptare, se va lua ultimul pixel , se va xora cu cel precedent si cu valoarea din r corespunzatoare. Acest lucru se va face pana se ajunge la prima valoare care va fi xorata cu ea insasi, starting value si valoarea din r ce ii corespunde.
- se aloca spatiu pentru pointerul o care va fi folosit in generarea permutarii. Astfel se va retine in o[i] valoarea indicelui i, pentru a putea mai tarziu genera permutarea.
- folosind algoritmul lui Dustenfeld si valorile din pointerul r se va genera o permutare aleatorie retinuta in pointerul o, iar in prealabil va fi folosit pointerul p in care se vor retine conform permutarilor pixelii din a ( p[i]=(a)[o[i]])
- pointerul a mai apoi va indica spre valorile din p
-se inchide la final fisierul din care am citit valorile secrete.
           6) void test_chi
- aceasta functie va calcula valorile testului chi patrat pentru fiecare canal de culoare.
- se ia o variabila de tip float f care il va reprezenta pe f cu bara deasupra (latime inaltime/256).
- se porneste cu un for care va merge de 3 ori , calculand chi patrat pentru fiecare culoare in parte (R,G,B).
- se va merge apoi cu un i de la 0 la 255, reprezentand valoarea unei culori de pe un anumit canal, iar apoi se va merge pe fiecare pixel si se va calcula (cu ajutorul operatiilor pe biti) prin intermediul variabilei cont initializata inainte de for-ul precedent cu valoarea 0 frecventa unei culori ce are valoarea i pe un anumit canal. In mod repetitiv , dupa parcurgerea imaginii, o anumita variabila numita chi va face suma dintre (frecventa-frecventa cu bara deasupra) la patrat adunata cu valorile precedente ale acestui calcul. Inainte de finalul primului for x, care a fost initial 255 se va sifta la stanga cu un numar corespunzator de biti astfel incat sa se afle sub urmatorul canal de culoare.
         7) void template_matching
- se parcurge sablonul si se calculeaza s barat care va fi retinut in variabila sm. Astfel in sm se adauga valoarea ultimei culori dintr-un pixel, iar la final dup ace sablonul este parcurs complet sm va fi impartit la cate elemente sunt in acesta, adica latimea_sablonului*inaltimea_sablonului ( w*h)
- se parcurge iar sablonul si de data asta se calculeaza deviatia pentru acesta conform formulei.
- se ia apoi fiecare punct din imagine si se considera pentru acesta cate un chenar cu coordonatele calculate in functie de latimea sablonului impartita la doi si inaltimea sablonului impartita la doi, se calculeaza pentru acesta media valorilor intensitatii grayscale ce va fi retinuta in fim si deviatia standard ce va fi memorata in variabila of. Se parcurge chenarul si se calculeaza corelatia pentru acesta, dar pentru indicii care ies din imaginea mare valoarea ultimei culori din pixel va fi considerate 0 ( negru ). 
- corelatia va fi comparata cu un prag, iar daca aceasta il depaseste se va retine alaturi de coordinate si culoare intr-un pointer alocat dinamic f.
- pentru template_matching s-au folosit si doua functii cu ror in a calcula deviatia standard si media valorilor intensitatilor grayscale
        
    8) void colorez
- aceasta functie deseneaza un chenar in jurul a doua puncte, care are lungimea si latimea sablonului.
       9) qsort
- functia precedenta sorteaza o structura speciala pentru detectii, f , in functie de corelatie descrescator.
       10)  void non_maxime
-functia aceasta primeste ca parmetru elementele unei structure retinute prin intermediul pointerului f si parcurge ca la sortarea clasica sirul cu 2 indici , iar prin intermediul functiei intersectia_dreptunghiurilor verifica suprapunerea. Daca aceasta este mai mare de 0.2, atunci elementele lui f[j](j fiind indicele termenului cu corelatie mai mica) vor lua toate valoarea -1.
- w si h reprezinta latimea, respective inaltimea sablonului.
-functia intersectia_dreptunghiurilor primeste i si j ce reprezinta dreptunghiurile a caror suprapunere trebuie calculata. Deoarece se stie ca ambele dreptunghiuri au aceias arie, nu mai are rost sa o calculam separate, asa ca totul se calculeaza dupa o anumita formula(minimul(coordonata x din stanga pentru i, coordonata x din stanga pentru j)- max(coordonata x din dreapta pentru i, coordonata x din dreapta pentru j))* minimul(coordonata y de sus pentru i, coordonata y de sus pentru j)- max(coordonata y de jos pentru i, coordonata y de jos pentru j))



