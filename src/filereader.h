#pragma once

#include <iostream>
#include <fstream>
#include <vector>

class FileReader {
    private:
        FILE *file;
        bool is_fopen();

        int buffer_size;
        char *buffer;
        int current_buffer;
        int buffer_pointer;

        std::string lexeme;
        int lexeme_start_ptr;

        void reload_left_buffer();
        void reload_right_buffer();

        char read_char_from_buffer();
    public:
        FileReader(std::string path, int buffer_size);
        ~FileReader();

        void reset_pointer();
        void commit_pointer();

        std::string cur_lexeme();

        char read();
        void unread();
};