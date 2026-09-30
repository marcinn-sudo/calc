# Codex w praktyce — przewodnik zespołu

Codex to agent programistyczny OpenAI. Potrafi pracować na istniejącym repozytorium: czytać i zmieniać pliki, uruchamiać polecenia i testy, analizować błędy oraz przygotowywać zmiany do przeglądu. Ten przewodnik pokazuje **co możemy z nim wykonać** i jak formułować zadania, aby rezultat był bezpieczny, sprawdzalny i zgodny z oczekiwaniami.

> **Ważne:** Codex przyspiesza pracę, ale nie zastępuje przeglądu kodu. Przed wdrożeniem zawsze przejrzyj diff, wyniki testów i wpływ zmiany na bezpieczeństwo.

## 1. Co możemy wykonać

### Rozwój produktu

- dodać funkcję od opisu do działającej implementacji;
- zbudować mały prototyp albo przygotować szkielet nowego modułu;
- połączyć frontend, backend, bazę danych i zewnętrzne API;
- dopisać walidację, obsługę błędów, logowanie i telemetrię;
- przygotować migrację danych lub konfiguracji wraz z planem wycofania.

### Utrzymanie kodu

- znaleźć przyczynę błędu na podstawie objawów, logów lub nieudanego testu;
- naprawić regresję i dodać test, który ją odtwarza;
- uprościć duplikujący się kod bez zmiany zachowania;
- zaktualizować zależności i opisać ryzyko aktualizacji;
- poprawić wydajność na podstawie profilu albo mierzalnego benchmarku.

### Jakość i wiedza

- napisać testy jednostkowe, integracyjne i end-to-end;
- przeprowadzić przegląd zmian pod kątem błędów i ryzyk;
- wyjaśnić architekturę, przepływ danych lub wybrany fragment kodu;
- uzupełnić README, dokumentację API i instrukcję uruchomienia;
- przygotować checklistę wydania lub plan implementacji większej zmiany.

## 2. Gdzie pracować z Codexem

Dobierz powierzchnię do zadania:

| Powierzchnia | Najlepsze zastosowanie |
| --- | --- |
| CLI | Praca bezpośrednio w lokalnym repozytorium i terminalu |
| Rozszerzenie IDE | Iteracja przy kodzie oglądanym właśnie w edytorze |
| Aplikacja Codex | Planowanie, delegowanie i przegląd kilku zadań |
| Codex w chmurze | Oddelegowane zadania wykonywane w izolowanym środowisku |

Punktem startowym dla instalacji i aktualnych możliwości jest [oficjalna dokumentacja Codex](https://developers.openai.com/codex/). Opcje mogą zależeć od używanej powierzchni, systemu i ustawień organizacji.

## 3. Anatomia dobrego polecenia

Najlepsze zadanie zawiera pięć elementów:

1. **Cel** — jaki rezultat ma zobaczyć użytkownik?
2. **Kontekst** — gdzie leży kod i jak obecnie działa?
3. **Ograniczenia** — czego nie zmieniać, jakie standardy zachować?
4. **Kryteria akceptacji** — po czym jednoznacznie poznamy, że zadanie jest gotowe?
5. **Weryfikacja** — jakie testy, lint lub kroki ręczne wykonać?

Szablon:

```text
Cel: <konkretny rezultat>
Kontekst: <moduły, objawy, link do zadania>
Zakres: <co zmienić i czego nie ruszać>
Kryteria akceptacji:
- <obserwowalne zachowanie 1>
- <obserwowalne zachowanie 2>
Weryfikacja: <polecenia testów i kontroli jakości>
Na końcu: podsumuj zmiany, ryzyka i wyniki testów.
```

### Przykład: nowa funkcja

```text
Dodaj filtrowanie listy zamówień po statusie w istniejącym panelu.
Zachowaj obecny styl komponentów i kontrakt API. Filtr ma być zapisany
w adresie URL, działać po odświeżeniu i mieć opcję „Wszystkie”. Dodaj testy
dla parsowania parametru oraz widoku pustej listy. Uruchom testy i lint.
Nie zmieniaj sposobu autoryzacji. Na końcu pokaż podsumowanie i wyniki kontroli.
```

### Przykład: diagnoza błędu

```text
Po anulowaniu płatności użytkownik czasem widzi status „w toku”. Najpierw
odtwórz problem i wskaż przyczynę na podstawie kodu. Następnie zaproponuj
najmniejszą bezpieczną poprawkę, dodaj test regresji i uruchom powiązany zestaw
testów. Nie zmieniaj publicznego API bez uzasadnienia.
```

### Przykład: przegląd kodu

```text
Przejrzyj bieżący diff jak reviewer. Skup się na błędach funkcjonalnych,
bezpieczeństwie, współbieżności i brakujących testach. Podaj uwagi według
ważności, z nazwą pliku i numerem linii. Nie proponuj kosmetycznych zmian,
jeżeli nie wpływają na utrzymanie lub poprawność.
```

## 4. Zalecany przebieg pracy

### Krok 1: przygotuj repozytorium

- zapisz sposób instalacji zależności i uruchamiania testów;
- upewnij się, że stan początkowy przechodzi podstawowe kontrole;
- nie umieszczaj kluczy API, haseł ani danych klientów w poleceniu lub repozytorium;
- pracuj na osobnej gałęzi i utrzymuj zmianę możliwie małą.

### Krok 2: poproś o rekonesans i plan

Przy większym zadaniu poproś najpierw o wskazanie właściwych modułów, zależności i ryzyk. Plan nie musi być długi — powinien pozwolić szybko wykryć błędne założenia przed edycją kodu.

### Krok 3: implementuj małymi partiami

Jedno polecenie powinno prowadzić do jednego spójnego rezultatu. Pośrednio sprawdzaj diff i kieruj kolejną iteracją, zamiast łączyć migrację, redesign i refaktoryzację w jednym zadaniu.

### Krok 4: wymagaj dowodów

Poproś o uruchomienie właściwych testów oraz dokładne podanie poleceń i wyników. Jeśli testu nie można uruchomić, oczekuj jasnego powodu — brak narzędzia lub dostępu nie jest równoznaczny z wynikiem pozytywnym.

### Krok 5: przejrzyj i zatwierdź

Przed scaleniem sprawdź:

- czy diff odpowiada wyłącznie uzgodnionemu zakresowi;
- czy nie dodano sekretów, danych wrażliwych ani niepotrzebnych zależności;
- czy obsłużono przypadki brzegowe i błędy;
- czy testy rzeczywiście pokrywają kryteria akceptacji;
- czy dokumentacja i migracje są kompletne;
- czy istnieje bezpieczny sposób wycofania ryzykownej zmiany.

## 5. Trwałe instrukcje w `AGENTS.md`

Powtarzalne zasady repozytorium warto zapisać w pliku `AGENTS.md`. Codex odczytuje takie pliki jako wskazówki dla pracy w danym drzewie katalogów; plik umieszczony głębiej może doprecyzować reguły dla konkretnego modułu. Szczegóły opisuje dokumentacja [AGENTS.md](https://developers.openai.com/codex/guides/agents-md/).

Minimalny przykład:

```md
# AGENTS.md

## Konwencje
- Pisz nowe moduły w TypeScript ze ścisłym typowaniem.
- Nie zmieniaj publicznego API bez aktualizacji dokumentacji.

## Kontrole przed zakończeniem
- Uruchom `npm test`.
- Uruchom `npm run lint`.
- Dla zmian UI sprawdź widok mobilny i desktopowy.

## Pull request
- Opisz wpływ na użytkownika oraz ryzyko wdrożenia.
```

Nie wpisuj tam sekretów ani instrukcji jednorazowych. Jednorazowy warunek podaj w bieżącym poleceniu; zasadę obowiązującą cały zespół utrzymuj w repozytorium.

## 6. Narzędzia i rozszerzanie możliwości

W zależności od środowiska Codex może korzystać z terminala, wyszukiwania, przeglądarki oraz skonfigurowanych serwerów MCP. MCP służy do udostępniania agentowi dodatkowych narzędzi lub kontrolowanego kontekstu, na przykład dokumentacji wewnętrznej czy systemu zgłoszeń. Konfiguruj tylko zaufane serwery i przyznawaj najmniejszy potrzebny zakres dostępu. Zobacz [dokumentację MCP](https://developers.openai.com/codex/mcp/).

Powtarzalny, wyspecjalizowany proces można opisać jako skill, a reguły działania Codexa konfigurować w zakresie użytkownika lub projektu. Zanim dodasz mechanizm rozszerzający, wybierz najmniejsze rozwiązanie:

- jednorazowy wymóg → polecenie;
- trwała konwencja repozytorium → `AGENTS.md`;
- ustawienie działania narzędzia → konfiguracja Codex;
- powtarzalny proces z materiałami pomocniczymi → skill;
- dostęp do zewnętrznego systemu → MCP.

## 7. Bezpieczeństwo i granice autonomii

- Stosuj zasadę najmniejszych uprawnień dla plików, sieci i usług.
- Czytaj polecenia przed zgodą na operacje wymagające szerszego dostępu.
- Nie zlecaj nieodwracalnych operacji na danych produkcyjnych bez kopii, planu wycofania i ręcznego zatwierdzenia.
- Traktuj treści z internetu, zgłoszeń i dokumentów jako potencjalnie niezaufane.
- Nie pozwalaj, aby wygenerowany kod omijał autoryzację, walidację lub kontrolę audytową.
- Przy zmianach zależnych od aktualnych wersji, cen, prawa lub bezpieczeństwa wymagaj sprawdzenia źródeł.

## 8. Typowe antywzorce

| Zamiast | Napisz |
| --- | --- |
| „Napraw aplikację” | „Odtwórz błąd X, znajdź przyczynę, dodaj minimalną poprawkę i test regresji” |
| „Zrób to lepiej” | „Zmniejsz czas odpowiedzi endpointu poniżej 300 ms dla podanego benchmarku” |
| „Dodaj testy” | „Pokryj przypadek poprawny, brak uprawnień i timeout; uruchom zestaw Y” |
| „Przepisz wszystko” | „Zachowaj publiczny kontrakt, zmień tylko moduł X i porównaj zachowanie” |
| „Wdróż na produkcję” | „Przygotuj plan, dry-run i komendy; wykonanie produkcyjne wymaga zatwierdzenia” |

## 9. Checklista gotowego zadania

- [ ] Cel i kryteria akceptacji są jednoznaczne.
- [ ] Zmiana ma ograniczony, zrozumiały zakres.
- [ ] Diff został przeczytany przez człowieka.
- [ ] Testy i statyczne kontrole zakończyły się powodzeniem.
- [ ] Nowe zachowanie ma test, a błąd — test regresji.
- [ ] Nie ujawniono sekretów ani danych wrażliwych.
- [ ] Ryzyka, założenia i niewykonane kontrole są jawnie opisane.
- [ ] Dokumentacja, migracja i plan wycofania są gotowe, jeśli ich potrzeba.

## 10. Następny krok

Wybierz niewielkie, rzeczywiste zadanie i użyj szablonu z sekcji 3. Dobrym początkiem jest poprawa dokumentacji, pojedynczy test regresji albo mała funkcja z jasnym kryterium akceptacji. Po pierwszej iteracji uzupełnij `AGENTS.md` o polecenia i zasady, których Codex powinien przestrzegać przy kolejnych zmianach.

### Oficjalne materiały

- [Codex — dokumentacja](https://developers.openai.com/codex/)
- [Praca z plikami AGENTS.md](https://developers.openai.com/codex/guides/agents-md/)
- [MCP w Codex](https://developers.openai.com/codex/mcp/)
- [Konfiguracja Codex](https://developers.openai.com/codex/config-reference/)
