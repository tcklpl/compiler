#include "filereader.h"

FileReader::FileReader(std::string path, int buffer_size) : buffer_size(buffer_size) {
    file = fopen(path.c_str(), "r");
    if (file == nullptr)
        throw "Failed to open specified file, is the path correct?";
    current_buffer = 2;
    buffer = (char *) malloc(2 * buffer_size * sizeof(char));
    buffer_pointer = 0;
    lexeme_start_ptr = 0;
    lexeme = "";
    reload_left_buffer();
}

FileReader::~FileReader() {
    if (is_fopen())
        fclose(file);
    free(buffer);
}

void FileReader::reload_left_buffer() {
    if (current_buffer == 2) {
        current_buffer = 1;
        
        for (int i = 0; i < buffer_size; i++) {
            buffer[i] = getc(file);
            if (buffer[i] == -1)
                break;
        }
    }
}

void FileReader::reload_right_buffer() {
    if (current_buffer == 1) {
        current_buffer = 2;
        
        for (int i = buffer_size; i < buffer_size * 2; i++) {
            buffer[i] = getc(file);
            if (buffer[i] == -1)
                break;
        }
    }
}

bool FileReader::is_fopen() {
    return file != nullptr;
}

char FileReader::read_char_from_buffer() {
    if (!is_fopen())
        throw "File is not open, cannot read next character";
    
    char ret = buffer[buffer_pointer++];

    if (buffer_pointer == buffer_size) {
        reload_right_buffer();
    } else if (buffer_pointer == buffer_size * 2) {
        reload_left_buffer();
        buffer_pointer = 0;
    }

    return ret;
}

void FileReader::reset_pointer() {
    buffer_pointer = lexeme_start_ptr;
    lexeme = std::string();
    newlines_unstashed = 0;
    pos_unstashed = 0;
}

void FileReader::commit_pointer() {
    //std::cout << "Commit: " << lexeme << std::endl;
    lexeme_start_ptr = buffer_pointer;
    lexeme = std::string();
    file_line += newlines_unstashed;
    file_pos += pos_unstashed;
    newlines_unstashed = 0;
    pos_unstashed = 0;
}

std::string FileReader::cur_lexeme() {
    return lexeme;
}

char FileReader::read() {
    if (!is_fopen())
        throw "File is not open, cannot read next character";
    char c = read_char_from_buffer();
    lexeme += c;
    pos_unstashed++;
    if (c == '\n') newlines_unstashed++;
    //std::cout << "leu " << c << " [bp: " << buffer_pointer << ", lp: " << lexeme_start_ptr << "]" << std::endl;
    return c;
}

void FileReader::unread() {
    if (--buffer_pointer < 0) {
        buffer_pointer = buffer_size * 2 - 1;
    }
    if (buffer[buffer_pointer] == '\n')
        newlines_unstashed--;
    if (lexeme.length() > 0)
        lexeme.pop_back();
    pos_unstashed--;
}

size_t FileReader::cur_pos() {
    return file_pos;
}

size_t FileReader::cur_line() {
    return file_line;
}