#include <cmath>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

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

enum class TokenType {
    Number,
    Plus,
    Minus,
    Multiply,
    Divide,
    Modulo,
    Power,
    LParen,
    RParen,
    Identifier,
    End
};

struct Token {
    TokenType type;
    std::string text;
    double number = 0.0;
};

class Lexer {
public:
    explicit Lexer(std::string input) : input_(std::move(input)) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        while (true) {
            skip_whitespace();
            if (pos_ >= input_.size()) {
                tokens.push_back({TokenType::End, ""});
                break;
            }

            char ch = input_[pos_];
            if (std::isdigit(static_cast<unsigned char>(ch)) || ch == '.') {
                tokens.push_back(read_number());
                continue;
            }

            if (std::isalpha(static_cast<unsigned char>(ch))) {
                tokens.push_back(read_identifier());
                continue;
            }

            switch (ch) {
                case '+':
                    tokens.push_back({TokenType::Plus, "+"});
                    ++pos_;
                    break;
                case '-':
                    tokens.push_back({TokenType::Minus, "-"});
                    ++pos_;
                    break;
                case '*':
                    tokens.push_back({TokenType::Multiply, "*"});
                    ++pos_;
                    break;
                case '/':
                    tokens.push_back({TokenType::Divide, "/"});
                    ++pos_;
                    break;
                case '%':
                    tokens.push_back({TokenType::Modulo, "%"});
                    ++pos_;
                    break;
                case '^':
                    tokens.push_back({TokenType::Power, "^"});
                    ++pos_;
                    break;
                case '(':
                    tokens.push_back({TokenType::LParen, "("});
                    ++pos_;
                    break;
                case ')':
                    tokens.push_back({TokenType::RParen, ")"});
                    ++pos_;
                    break;
                default:
                    tokens.push_back({TokenType::End, ""});
                    errors_.push_back("Nieznany znak: " + std::string(1, ch));
                    ++pos_;
                    break;
            }
        }
        return tokens;
    }

    const std::vector<std::string> &errors() const { return errors_; }

private:
    Token read_number() {
        size_t start = pos_;
        bool dot_seen = false;
        while (pos_ < input_.size()) {
            char ch = input_[pos_];
            if (ch == '.') {
                if (dot_seen) {
                    break;
                }
                dot_seen = true;
            } else if (!std::isdigit(static_cast<unsigned char>(ch))) {
                break;
            }
            ++pos_;
        }
        std::string text = input_.substr(start, pos_ - start);
        double value = 0.0;
        std::istringstream stream(text);
        stream >> value;
        return {TokenType::Number, text, value};
    }

    Token read_identifier() {
        size_t start = pos_;
        while (pos_ < input_.size() &&
               std::isalpha(static_cast<unsigned char>(input_[pos_]))) {
            ++pos_;
        }
        std::string text = input_.substr(start, pos_ - start);
        return {TokenType::Identifier, text};
    }

    void skip_whitespace() {
        while (pos_ < input_.size() &&
               std::isspace(static_cast<unsigned char>(input_[pos_]))) {
            ++pos_;
        }
    }

    std::string input_;
    size_t pos_ = 0;
    std::vector<std::string> errors_;
};

class Parser {
public:
    explicit Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)) {}

    std::optional<double> parse(std::string &error) {
        current_ = 0;
        auto result = parse_expression(error);
        if (!result) {
            return std::nullopt;
        }
        if (!consume(TokenType::End)) {
            error = "Nieoczekiwane znaki na koncu wyrazenia.";
            return std::nullopt;
        }
        return result;
    }

private:
    std::optional<double> parse_expression(std::string &error) {
        auto left = parse_term(error);
        if (!left) {
            return std::nullopt;
        }
        while (match(TokenType::Plus) || match(TokenType::Minus)) {
            TokenType op = previous().type;
            auto right = parse_term(error);
            if (!right) {
                return std::nullopt;
            }
            if (op == TokenType::Plus) {
                left = *left + *right;
            } else {
                left = *left - *right;
            }
        }
        return left;
    }

    std::optional<double> parse_term(std::string &error) {
        auto left = parse_power(error);
        if (!left) {
            return std::nullopt;
        }
        while (match(TokenType::Multiply) || match(TokenType::Divide) ||
               match(TokenType::Modulo)) {
            TokenType op = previous().type;
            auto right = parse_power(error);
            if (!right) {
                return std::nullopt;
            }
            if (op == TokenType::Multiply) {
                left = *left * *right;
            } else if (op == TokenType::Divide) {
                if (*right == 0.0) {
                    error = "Blad: dzielenie przez zero.";
                    return std::nullopt;
                }
                left = *left / *right;
            } else {
                if (*right == 0.0) {
                    error = "Blad: dzielenie przez zero.";
                    return std::nullopt;
                }
                left = std::fmod(*left, *right);
            }
        }
        return left;
    }

    std::optional<double> parse_power(std::string &error) {
        auto left = parse_unary(error);
        if (!left) {
            return std::nullopt;
        }
        if (match(TokenType::Power)) {
            auto right = parse_power(error);
            if (!right) {
                return std::nullopt;
            }
            left = std::pow(*left, *right);
        }
        return left;
    }

    std::optional<double> parse_unary(std::string &error) {
        if (match(TokenType::Minus)) {
            auto value = parse_unary(error);
            if (!value) {
                return std::nullopt;
            }
            return -*value;
        }
        if (match(TokenType::Plus)) {
            return parse_unary(error);
        }
        return parse_primary(error);
    }

    std::optional<double> parse_primary(std::string &error) {
        if (match(TokenType::Number)) {
            return previous().number;
        }
        if (match(TokenType::Identifier)) {
            std::string name = previous().text;
            if (name != "sqrt") {
                error = "Nieznana funkcja: " + name;
                return std::nullopt;
            }
            if (!consume(TokenType::LParen)) {
                error = "Oczekiwano '(' po sqrt.";
                return std::nullopt;
            }
            auto value = parse_expression(error);
            if (!value) {
                return std::nullopt;
            }
            if (!consume(TokenType::RParen)) {
                error = "Oczekiwano ')' po argumencie sqrt.";
                return std::nullopt;
            }
            if (*value < 0.0) {
                error = "Blad: pierwiastek z liczby ujemnej.";
                return std::nullopt;
            }
            return std::sqrt(*value);
        }
        if (match(TokenType::LParen)) {
            auto value = parse_expression(error);
            if (!value) {
                return std::nullopt;
            }
            if (!consume(TokenType::RParen)) {
                error = "Oczekiwano ')'.";
                return std::nullopt;
            }
            return value;
        }
        error = "Nieoczekiwany token w wyrazeniu.";
        return std::nullopt;
    }

    bool match(TokenType type) {
        if (check(type)) {
            advance();
            return true;
        }
        return false;
    }

    bool consume(TokenType type) {
        if (check(type)) {
            advance();
            return true;
        }
        return false;
    }

    bool check(TokenType type) const {
        return current_ < tokens_.size() && tokens_[current_].type == type;
    }

    const Token &advance() {
        if (current_ < tokens_.size()) {
            ++current_;
        }
        return tokens_[current_ - 1];
    }

    const Token &previous() const { return tokens_[current_ - 1]; }

    std::vector<Token> tokens_;
    size_t current_ = 0;
};

void run_calculator(bool graphic_mode) {
    std::cout << "Mozesz wpisywac cale wyrazenia, np. 10+5+18*50/3\n";
    std::cout << "Dla sqrt uzyj zapisu sqrt(16).\n\n";

    std::string line;
    while (true) {
        std::cout << "Podaj wyrazenie lub q aby wyjsc: ";
        if (!std::getline(std::cin, line)) {
            std::cerr << "Blad wejscia.\n";
            return;
        }

        if (line == "q" || line == "Q") {
            std::cout << "Koniec programu.\n";
            return;
        }

        if (line.empty()) {
            continue;
        }

        Lexer lexer(line);
        auto tokens = lexer.tokenize();
        if (!lexer.errors().empty()) {
            std::cout << "Blad: " << lexer.errors().front() << "\n\n";
            continue;
        }

        Parser parser(std::move(tokens));
        std::string error;
        auto result = parser.parse(error);
        if (!result) {
            std::cout << error << "\n\n";
            continue;
        }

        if (graphic_mode) {
            print_graphic_result("wynik", *result);
        } else {
            std::cout << "Wynik: " << *result << "\n\n";
        }
    }
}

bool ask_graphic_mode(bool &graphic_mode) {
    std::cout << "Wybierz tryb pracy:\n";
    std::cout << "1 - tekstowy\n";
    std::cout << "2 - graficzny (ASCII)\n";
    std::cout << "Twoj wybor: ";

    int choice = 0;
    if (!(std::cin >> choice)) {
        return false;
    }

    graphic_mode = (choice == 2);
    return true;
}
}  // namespace

int main() {
    print_banner();

    bool graphic_mode = false;
    if (!ask_graphic_mode(graphic_mode)) {
        std::cerr << "Blad wejscia.\n";
        return 1;
    }
    clear_input();

    run_calculator(graphic_mode);

    return 0;
}
