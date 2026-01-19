#include <iostream>
#include <limits>

int main() {
    std::cout << "Prosty kalkulator C++\n";
    std::cout << "Dostepne operacje: + - * /\n";
    std::cout << "Aby zakonczyc wpisz q jako operacje.\n\n";

    while (true) {
        char op = '\0';
        double a = 0.0;
        double b = 0.0;

        std::cout << "Podaj operacje (+, -, *, /) lub q aby wyjsc: ";
        std::cin >> op;

        if (!std::cin) {
            std::cerr << "Blad wejscia.\n";
            return 1;
        }

        if (op == 'q' || op == 'Q') {
            std::cout << "Koniec programu.\n";
            break;
        }

        if (op != '+' && op != '-' && op != '*' && op != '/') {
            std::cout << "Nieznana operacja. Sprobuj ponownie.\n\n";
            continue;
        }

        std::cout << "Podaj pierwsza liczbe: ";
        std::cin >> a;
        std::cout << "Podaj druga liczbe: ";
        std::cin >> b;

        if (!std::cin) {
            std::cerr << "Blad wejscia.\n";
            return 1;
        }

        switch (op) {
            case '+':
                std::cout << "Wynik: " << (a + b) << "\n\n";
                break;
            case '-':
                std::cout << "Wynik: " << (a - b) << "\n\n";
                break;
            case '*':
                std::cout << "Wynik: " << (a * b) << "\n\n";
                break;
            case '/':
                if (b == 0.0) {
                    std::cout << "Blad: dzielenie przez zero.\n\n";
                } else {
                    std::cout << "Wynik: " << (a / b) << "\n\n";
                }
                break;
            default:
                break;
        }
    }

    return 0;
}
