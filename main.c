#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 4096

void print_error(const char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s source destination\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *src_path = argv[1];
    const char *dst_path = argv[2];

    // Открытие исходного файла (только для чтения)
    int src_fd = open(src_path, O_RDONLY);
    if (src_fd == -1) {
        print_error("Cannot open source file");
        exit(EXIT_FAILURE);
    }

    // Открытие или создание целевого файла (чтение+запись, создаём если нет, усекаем до 0)
    // Права: rw-r--r-- (0644)
    int dst_fd = open(dst_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst_fd == -1) {
        print_error("Cannot create/open destination file");
        close(src_fd);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    // Копирование
    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(dst_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            print_error("Write error");
            close(src_fd);
            close(dst_fd);
            exit(EXIT_FAILURE);
        }
    }

    if (bytes_read == -1) {
        print_error("Read error");
    }

    // Закрытие файлов
    close(src_fd);
    close(dst_fd);

    // Для демонстрации мониторинга – добавим задержку, чтобы успеть посмотреть /proc
    // В реальной программе её можно убрать или сделать опциональной.
    printf("Copy finished. Press Enter to exit...");
    getchar();

    return 0;
}
