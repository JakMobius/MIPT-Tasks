/*
 * Math number lexeme:
 * $NUMBER := '0x'DIGITS(16)'0o'$DIGITS(8)|'0b'$DIGITS(2)|$DECIMAL
 * $DECIMAL := ($DIGITS)(.($DIGITS))?(['E'|'e']($DIGITS))?
 * $DIGITS($RADIX) := [0-9|a-c]+
 */

#include "bonk_number_lexeme.h"

bool bonk_parse_number_lexeme(s_bonk_lexical_analyzer* analyzer, s_bonk_lexeme* target) {
    int fraction_digits = 0;
    long long integer_result = 0;
    double float_result = 0;
    double fraction_factor = 1;
    double fraction = 0;
    char c = '\0';
    int radix = 10;

    int mantissa_digits = bonk_parser_digits_lexeme(analyzer, 10, &integer_result, &float_result);
    if(mantissa_digits == 0) {
        return false;
    }

    if(mantissa_digits == 1 && integer_result == 0) {
        c = bonk_lexical_analyzer_next_char(analyzer);
        if(c == 'x') radix = 16;
        else if(c == 'o') radix = 8;
        else if(c == 'b') radix = 2;

        if(radix != 10) {
            bonk_lexical_analyzer_eat_char(analyzer);
            mantissa_digits = bonk_parser_digits_lexeme(analyzer, radix, &integer_result, &float_result);
            if(mantissa_digits == 0) {
                bonk_lexical_error(analyzer, "expected number");
                return false;
            }
        }
    }

    c = bonk_lexical_analyzer_next_char(analyzer);
    if(radix == 10 && c == '.') {
        bonk_lexical_analyzer_eat_char(analyzer);
        fraction_digits = bonk_parser_digits_lexeme(analyzer, radix, NULL, &fraction);
        if(fraction_digits == 0) {
            bonk_lexical_error(analyzer, "expected fraction");
            return false;
        }
        for(int i = 0; i < fraction_digits; i++) fraction_factor *= 10;
        float_result += fraction / fraction_factor;
    }

    c = bonk_lexical_analyzer_next_char(analyzer);
    if(radix == 10 && (c == 'e' || c == 'E')) {
        bonk_lexical_analyzer_eat_char(analyzer);
        char c = bonk_lexical_analyzer_next_char(analyzer);
        bool sign = true;
        if(c == '+' || c == '-') {
            sign = c == '+';
            bonk_lexical_analyzer_eat_char(analyzer);
        }
        long long exponent = 0;
        int exponent_digits = bonk_parser_digits_lexeme(analyzer, radix, &exponent, NULL);
        if(exponent_digits == 0) {
            bonk_lexical_error(analyzer, "exponent is empty");
            return false;
        }

        for(int i = 0; i < exponent; i++) {
            if(sign) {
                float_result *= 10;
                integer_result *= 10;

                if(fraction_digits > 0) {
                    fraction_factor /= 10;
                    integer_result += (long long)(fraction / fraction_factor) % 10;
                }
            } else {
                float_result /= 10;
                integer_result /= 10;
            }
        }
    }

    s_tree_node_number* result = tree_create_node_number();
    result->float_value = float_result;
    result->integer_value = integer_result;
    result->tree_node.source_position = bonk_parser_position_clone(target->position);

    target->type = BONK_LEXEME_NUMBER;
    target->number_data.number = result;
    return true;
}

unsigned char char_to_digit_radix(char c, int radix) {
    if(c < '0') return -1;

    if(radix <= 10) {
        if(c > '0' + (radix - 1)) return -1;
    }

    if(c <= '9') return c - '0';

    if(radix <= 16) {
        if(c > 'a' + (radix - 11)) return -1;
        return c - 'a';
    }

    return -1;
}

int bonk_parser_digits_lexeme(s_bonk_lexical_analyzer* analyzer, int radix, long long* integer_value, double* float_value) {
    long long integer_result = 0;
    double float_result = 0;
    int digits = 0;
    unsigned char digit = '\0';

    while(true) {
        digit = char_to_digit_radix(bonk_lexical_analyzer_next_char(analyzer), radix);
        if(digit == (unsigned char)-1) break;

        bonk_lexical_analyzer_eat_char(analyzer);
        integer_result *= radix;
        float_result *= radix;
        integer_result += digit;
        float_result += digit;
        digits++;
    }

    if(integer_value) *integer_value = integer_result;
    if(float_value) *float_value = float_result;
    return digits;
}