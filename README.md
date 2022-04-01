# The-Hogsmeade-Train-Station


1. open_train_station

    Verific dacă numărul de platforme există
    Aloc memoria pentru gară, apoi aloc platformele.
    Aloc memoria pentru platforme, apoi pentru fiecare locomotivă, dar ca să știu că ea nu există pun un indiciu - locomotive_power = -1;

2. close_train_station

    Verific dacă există stația, platformele, apoi prin iterație iau fiecare platformă și verific dacă există trenuri (astfel verific și dacă există locomotiva), cu ajutorul recursivității (close_train_car) dezaloc memoria fiecărui vagon (de la sfârșit la început), apoi dezaloc și restul: locomotivele cu platforme și după stația.

3. show_existing_trains

    Verific dacă există ce să pot arăta.
    Apoi afișez (dacă există), platforma, apoi locomotiva, apoi vagoanele.

4. arrive_train

    Verific dacă există stația și dacă este posibil să pun la accea platforma cu ajutorul indiciului (locomotive_power == -1) și modific puterea care o are locomotiva.

5. leave_train

    Verific dacă există stația, platforma și dacă există vreun tren la aceea platformă.
    Apoi cu ajutorul funcției de recursivitate a închiderii vagoanelor(close_train_car), dezaloc toate vagoanele și introduc valorile pentru care îmi dau seama că aceea platformă nu are o locomotivă

6.  add_train_car

    Verific dacă există gara, dacă greutatea este una reală.
    Verific dacă platforma și locomotivă există, deasemenea dacă e un număr posibil.
    Creez nodul vagonului pe care-l introduc, după care verific dacă există vagoane.
    Există două cazuri : 
    a) când nu există vagoane atașate locomotivei
    ...->train_cars == NULL
    unde atașex doar lui train_cars pointerul
    b) când există
    iterez prin toate vagoanele, și adaug la ultimul vagon->next pointerul nodului noului vagon.

7.  remove_train_cars

    Verific dacă există gara, platforma și dacă greutatea este una reală
    Apoi prin iterație caut vagonul cu greutatea specifică unde eliberez vagonul, cu 3 cazuri:
    a) când este la început
    b) când este la capătul vagoanelor
    c) când este în interiorul listei vagoanelor

8.  move_train_cars

    Am creat o structură pentru a salva pointerii vagonului curent(current) și cel predecesor (prev), apoi o funcție care găsește aceste puncte la o anumită poziție.

    Ajungând în funcția move_train_car (mutarea unui singur vagon), obținem punctele de prev și current al fiecărei poziție, apoi verific dacă aceste puncte pot fi valide:
    Dacă vagonul din poziția a are pointerul current == NULL atunci acesta nu există și iese, căci nu am ce să mișc, iar dacă vagonul din poziția b are current NULL, trebuie să verific dacă acesta are prev != NULL, că el poate fi la capătul listei de vagoane, putând să mut secvența din platforma a spre platforma b.

    Apoi la mutare, am niște verificări și posibilități că acesta ar fi la începutul listei, la capăt și între vagoane.

    La funcția principală (move_train_cars), am verificările necesare pentru a vedea dacă pot să mut și dacă pozițiile sunt posibile.

    apoi iau punctele prev și current pentru poziția ultimului vagon din secvența de cars_no, acesta ar fi poziția a + numărul de vagoane din secvență - 1.

    apoi cu ajutorul unui iterator, care ia toată secvența de vagoane și le mută pe fiecare în poziția b + iterator

9. find_express_train

    Am o funcție care returnează puterea locomotivei - greutatea totală a vagoanelor. Cu ajutorul acestei funcții verific fiecare platformă și aflu cea mai mare valoare. 
    variabila platform = -1, e o verificare dacă există o platformă care este de referință + ea fiind cu cea mai mare viteză.
    Dacă platform == -1 atunci înseamnă că nu există o locomotivă rapidă ( nici nu există locomotive ).

10. find_overload_train

    E același principiu ca la funcția find_express_train, doar că caut o valoare mai mică și la sfârșit verific dacă trenul pe care l-am găsit cu cea mai mică valoare e chiar supraîncărcat.

11. find_optimal_train

    Același principiu ca la funcția find_overload_train, doar că acum verific cea mai mică valoare pozitivă a cât trage locomotiva.

12. find_heaviest_sequence_train

    Verific dacă există stația, platformele.
    Iterez prin fiecare platformă, verific dacă acesta există și dacă există vagoane la el.
    Apoi iau un current1 (acesta fiind poziția de la care încep iterarea prin vagoane), iar cel care iterează prin vagoane este current2 (pozitia care ia valorile greutății vagonului), el iterează un număr de cars_no ori, adică numărul de vagoane din secvență. Verifică dacă poziția e potrivită și să nu fie NULL. Dacă toate condițiile sunt îndeplinite weight ia valoarea maximă, platforma ia valoarea iteratorului pentru platforme (i), iar start_car (deferențiat) ia pointerul current1 (poziția primului vagon din secvență).
    
13. order_train

    Verific dacă există stația, platforma și dacă există vagoane.
    Voi face cu sortarea Selection sort.
    Voi lua un prev1 și current1, acestea fiind pentru vagonul de referință și cu care voi schimba vagonul găsit cu valoarea maximă de current2, unde pentru current2 tot am un prev2.
    
    Deci, dacă avem current1, care va fi referința și care va compara cu greutatea vagonului de la current2, și acesta va fi de unde current2 își va reseta punctul pentru a căuta din nou maximul din listă.
    Dacă greutatea vagonului current2 este mai mare ca greutatea vagonului de la current1, voi face swap-ul, unde vor fi 2 cazuri cu câte alte 2 cazuri:
    Primul caz: Când vagoanele pentru swap sunt una lângă alta
    a) Acesta e la început de listă prev1 == NULL
    Nu e foarte diferită față de următorul caz în schimbarea pozițiilor, doar fiind diferit că trebuie să modofic ->train_cars, întrucât prev1 îi NULL și nu există

    b) Acesta nu e la început de listă prev1 != NULL
    Aici modific cum trebuie pointerii, iar după există o problemă:
    current1 va fi pe poziția current2 și current2 pe poziția lui current1, astfel trebuie să le modific pozițiile, current1 să fie pe poziția care o avut-o, iar current2 să fie pe poziția pe care o avea (nu pointer, ci poziția), doar trebuie să modific pointerii unul cu altul.

    Al doilea caz: Când vagoanele pentru swap nu sunt unul lângă altul
    a) Acesta e la început de listă prev1 == NULL
    la fel ca la primul caz pct a)

    b) Acesta nu e la început de listă prev1 != NULL
    Modific cum trebuie pointerii și schimb pozițiile cu ajutorul auxiliarilor pe care le am, apoi iar fac aceeași schimbare cu pointerii de la current1 la current2, având aceeași problemă ca la primul caz.

14. fix_overload_train

    Verific dacă există o platformă supraîncărcată.

    Iau valoarea weight = puterei locomotivei - greutatea încărcăturii.
    cu această valoare verific adunând greutatea vagonului cu această greutate (cumva verific dacă ușurez această greutate valoarea va fi pozitivă sau egală cu zero).

    Am un indiciu, optimal_weight == -1, acesta ar fi indiciul dacă nu s-a găsit greutatea care se poate scoate.

    Dacă este egală cu 0 atunci va fi ieși din while, întrucât 0 e cea mai mică valoare, iar dacă nu găsim o valoare 0, căutăm cea mai mică valoare apropiată de 0, pozitivă sau negativă, astfel cu ajutorul modulului (funcția abs(ceva)), pot să verific dacă greutatea pe care am găsit-o anterior e diferită de valoarea prezentă. Modulul mă ajută să verific dacă acesta e egal apropiată de 0, chiar dacă e negativ sau pozitiv.
    Și dacă găsesc o valoare mai potriviă, optimal_weight ia valoarea găsită

    Apoi cu funcția remove_train_cars șterg vagonul cu greutatea găsită. Știind că această valoare la vagoane este unică în listă, pot să folosesc funcția fără să mi se șteargă alte vagoane cu aceeași greutate.

# Mulțumesc pentru atenție

