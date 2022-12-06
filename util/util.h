struct file_contents {
    char** lines;
    int line_count;
};

struct file_contents read_lines(char* filename);