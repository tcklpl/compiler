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

        size_t file_pos = 0, file_line = 0, newlines_unstashed = 0, pos_unstashed = 0;

        void reload_left_buffer();
        void reload_right_buffer();

        char read_char_from_buffer();
    public:
        FileReader(std::string path, int buffer_size);
        ~FileReader();

        void reset_pointer();
        void commit_pointer();

        std::string cur_lexeme();
        size_t cur_pos();
        size_t cur_line();

        char read();
        void unread();
};