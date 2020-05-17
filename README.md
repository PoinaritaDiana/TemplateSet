# TemplateSet
 
Set<class T, class F = Comparator<T>>
 Implementarea unei clase template pentru set.  
 Clasa ofera access rapid la elementele pe  care  aceasta  le  memoreza,  fara  a  impune  o  anumita  restritie  pe  ordinea elementelor.
 Clasa Comparator<T> va fi folosita pe post de comparator default pentru a determina daca o valoare exista deja in set.  
 Clasa Set trebuie sa ofere:
 •constructor fara parametrii care initializeaza un set gol;
 •constructor de copiere si operatorul de atribuire;
 •metoda pentru adaugare si stergere element din set;
 •metoda care verifica daca un element se afla in set;
 •metoda care intoarce numarul de elemente din set;
 •supraincarcarea operatorului de afisare;
 •specializarea clasei Comparator pentru int,  care sa considere doi intregi egali daca au aceeasi paritate
