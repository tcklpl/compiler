#pragma once

inline bool is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

inline bool is_uppercase_letter(char c) {
    return c >= 'A' && c <= 'Z';
}

inline bool is_lowercase_letter(char c) {
    return c >= 'a' && c <= 'z';
}

inline bool is_number(char c) {
    return c >= '0' && c <= '9';
}

inline bool is_alphanumeric(char c) {
    return is_uppercase_letter(c) || is_lowercase_letter(c) || is_number(c);
}