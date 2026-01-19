#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

namespace {
void print_banner() {
    std::cout << "Prosty kalkulator C++\n";
    std::cout << "Dostepne operacje: + - * / ^ sqrt %\n";
    std::cout << "Aby zakonczyc wpisz q jako operacje.\n\n";
}

void clear_input() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void print_graphic_result(const std::string &label, double value) {
    const std::string border(38, '=');
    std::cout << border << "\n";
    std::cout << "=   " << std::left << std::setw(28) << label << "=\n";
    std::cout << "=   Wynik: " << std::left << std::setw(21) << value << "=\n";
    std::cout << border << "\n\n";
}

bool read_number(const std::string &prompt, double &value) {
    std::cout << prompt;
    if (!(std::cin >> value)) {
        return false;
    }
    return true;
}

bool read_integer(const std::string &prompt, long long &value) {
    std::cout << prompt;
    if (!(std::cin >> value)) {
        return false;
    }
    return true;
}

void run_calculator(bool graphic_mode) {
    while (true) {
        char op = '\0';

        std::cout << "Podaj operacje (+, -, *, /, ^, sqrt, %) lub q aby wyjsc: ";
        if (!(std::cin >> op)) {
            std::cerr << "Blad wejscia.\n";
            return;
        }

        if (op == 'q' || op == 'Q') {
            std::cout << "Koniec programu.\n";
            return;
        }

        if (op == 's') {
            std::string rest;
            std::cin >> rest;
            if (rest != "qrt") {
                std::cout << "Nieznana operacja. Sprobuj ponownie.\n\n";
                clear_input();
                continue;
            }
        } else if (op != '+' && op != '-' && op != '*' && op != '/' && op != '^' && op != '%') {
            std::cout << "Nieznana operacja. Sprobuj ponownie.\n\n";
            clear_input();
            continue;
        }

        if (op == 's') {
            double value = 0.0;
            if (!read_number("Podaj liczbe: ", value)) {
                std::cerr << "Blad wejscia.\n";
                return;
            }
            if (value < 0.0) {
                std::cout << "Blad: pierwiastek z liczby ujemnej.\n\n";
                continue;
            }
            double result = std::sqrt(value);
            if (graphic_mode) {
                print_graphic_result("sqrt", result);
            } else {
                std::cout << "Wynik: " << result << "\n\n";
            }
            continue;
        }

        if (op == '%') {
            long long a = 0;
            long long b = 0;
            if (!read_integer("Podaj pierwsza liczbe calkowita: ", a) ||
                !read_integer("Podaj druga liczbe calkowita: ", b)) {
                std::cerr << "Blad wejscia.\n";
                return;
            }
            if (b == 0) {
                std::cout << "Blad: dzielenie przez zero.\n\n";
                continue;
            }
            long long result = a % b;
            if (graphic_mode) {
                print_graphic_result("modulo", static_cast<double>(result));
            } else {
                std::cout << "Wynik: " << result << "\n\n";
            }
            continue;
        }

        double a = 0.0;
        double b = 0.0;

        if (!read_number("Podaj pierwsza liczbe: ", a) ||
            !read_number("Podaj druga liczbe: ", b)) {
            std::cerr << "Blad wejscia.\n";
            return;
        }

        double result = 0.0;
        std::string label;
        bool has_result = true;

        switch (op) {
            case '+':
                result = a + b;
                label = "dodawanie";
                break;
            case '-':
                result = a - b;
                label = "odejmowanie";
                break;
            case '*':
                result = a * b;
                label = "mnozenie";
                break;
            case '/':
                if (b == 0.0) {
                    std::cout << "Blad: dzielenie przez zero.\n\n";
                    has_result = false;
                } else {
                    result = a / b;
                    label = "dzielenie";
                }
                break;
            case '^':
                result = std::pow(a, b);
                label = "potegowanie";
                break;
            default:
                has_result = false;
                break;
        }

        if (!has_result) {
            continue;
        }

        if (graphic_mode) {
            print_graphic_result(label, result);
        } else {
            std::cout << "Wynik: " << result << "\n\n";
        }
    }
}

bool ask_graphic_mode() {
    std::cout << "Wybierz tryb pracy:\n";
    std::cout << "1 - tekstowy\n";
    std::cout << "2 - graficzny (ASCII)\n";
    std::cout << "Twoj wybor: ";

    int choice = 0;
    if (!(std::cin >> choice)) {
        return false;
    }

    return choice == 2;
}
}  // namespace

int main() {
    print_banner();

    bool graphic_mode = false;
    if (!ask_graphic_mode()) {
        std::cerr << "Blad wejscia.\n";
        return 1;
    }

    run_calculator(graphic_mode);

    return 0;
}
