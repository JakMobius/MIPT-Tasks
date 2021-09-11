/*
 * Math number grammatic:
 * $NUMBER := '0x'DIGITS(16)'0o'$DIGITS(8)|'0b'$DIGITS(2)|$DECIMAL
 * $DECIMAL := ($DIGITS)(.($DIGITS))?(['E'|'e']($DIGITS))?
 * $DIGITS($RADIX) := [0-9|a-c]+
 */

#include "grammatic_number.h"

s_tree_node* parse_grammatic_number(s_parser* parser) {
    int fraction_digits = 0;
    long long integer_result = 0;
    double float_result = 0;
    long long fraction_factor = 1;
    long long fraction = 0;
    char c = '\0';
    int radix = 10;

    int mantissa_digits = parse_grammatic_digits(parser, 10, &integer_result, &float_result);
    if(mantissa_digits == 0) {
        return NULL;
    }

    if(mantissa_digits == 1 && integer_result == 0) {
        c = parser_next_char(parser);
        if(c == 'x') radix = 16;
        else if(c == 'o') radix = 8;
        else if(c == 'b') radix = 2;

        if(radix != 10) {
            parser_eat_char(parser);
            mantissa_digits = parse_grammatic_digits(parser, radix, &integer_result, &float_result);
            if(mantissa_digits == 0) {
                parser_error(parser, "expected number");
                return NULL;
            }
        }
    }

    c = parser_next_char(parser);
    if(radix == 10 && c == '.') {
        parser_eat_char(parser);
        fraction_digits = parse_grammatic_digits(parser, radix, &fraction, NULL);
        if(fraction_digits == 0) {
            parser_error(parser, "expected fraction");
            return NULL;
        }
        for(int i = 0; i < fraction_digits; i++) fraction_factor *= 10;
        float_result += (double)fraction / (double)fraction_factor;
    }

    c = parser_next_char(parser);
    if(radix == 10 && (c == 'e' || c == 'E')) {
        parser_eat_char(parser);
        long long exponent = 0;
        int exponent_digits = parse_grammatic_digits(parser, radix, &exponent, NULL);
        if(exponent_digits == 0) {
            parser_error(parser, "exponent is empty");
            return NULL;
        }

        for(int i = 0; i < exponent; i++) {
            float_result *= 10;
            integer_result *= 10;

            if(fraction_digits > 0) {
                fraction_factor /= 10;
                integer_result += (fraction / fraction_factor) % 10;
            }
        }
    }

    s_tree_node_number* result = tree_create_node_number();
    result->float_value = float_result;
    result->integer_value = integer_result;
    return (s_tree_node*) result;
}

unsigned char char_to_digit_radix(char c, int radix) {
    if(c < '0') return -1;

    if(radix <= 10) {
        if(c > '0' + (radix - 1)) return -1;
    }

    if(c < '9') return c - '0';

    if(radix <= 16) {
        if(c > 'a' + (radix - 11)) return -1;
        return c - 'a';
    }

    return -1;
}

int parse_grammatic_digits(s_parser* parser, int radix, long long* integer_value, double* float_value) {
    long long integer_result = 0;
    double float_result = 0;
    int digits = 0;
    unsigned char digit = '\0';

    while(true) {
        digit = char_to_digit_radix(parser_next_char(parser), radix);
        if(digit == (unsigned char)-1) break;

        parser_eat_char(parser);
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