#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>
#include <string.h>
#include <process.h>

// Function prototypes for modular approach
void show_menu();
void demo_open_close();
void demo_read_write();
void demo_creat();
void demo_lseek();
void demo_stat();
void demo_chmod_access();
void demo_process_calls();
void clear_screen();
void pause_program();

int main() {
    int choice;
    
    printf("=== UNIX SYSTEM CALLS INTERACTIVE DEMO ===\n");
    
    while (1) {
        show_menu();
        printf("Enter your choice (1-8): ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); // Clear buffer
            pause_program();
            continue;
        }
        
        clear_screen();
        
        switch (choice) {
            case 1:
                demo_open_close();
                break;
            case 2:
                demo_read_write();
                break;
            case 3:
                demo_creat();
                break;
            case 4:
                demo_lseek();
                break;
            case 5:
                demo_stat();
                break;
            case 6:
                demo_chmod_access();
                break;
            case 7:
                demo_process_calls();
                break;
            case 8:
                printf("Thanks for using the demo! Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice! Please select 1-8.\n");
        }
        
        pause_program();
    }
    
    return 0;
}

void show_menu() {
    printf("\n=== SYSTEM CALLS MENU ===\n");
    printf("1. open() and close() Demo\n");
    printf("2. read() and write() Demo\n");
    printf("3. creat() Demo\n");
    printf("4. lseek() Demo\n");
    printf("5. stat() Demo\n");
    printf("6. chmod() and access() Demo\n");
    printf("7. getpid() Demo\n");
    printf("8. Exit\n");
    printf("========================\n");
}

void demo_open_close() {
    printf("=== OPEN() and CLOSE() System Calls ===\n\n");
    
    char filename[50];
    printf("Enter filename to create/open: ");
    scanf("%s", filename);
    
    // Open file for writing
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd != -1) {
        printf("File '%s' opened successfully! File descriptor: %d\n", filename, fd);
        
        // Close the file
        if (close(fd) == 0) {
            printf("File closed successfully!\n");
        } else {
            printf("Error closing file\n");
        }
    } else {
        printf("Failed to open file '%s'\n", filename);
    }
}

void demo_read_write() {
    printf("=== READ() and WRITE() System Calls ===\n\n");
    
    char filename[50], content[200];
    printf("Enter filename: ");
    scanf("%s", filename);
    
    // Open file for writing
    int fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd == -1) {
        printf("Cannot open file\n");
        return;
    }
    
    printf("Enter text to write: ");
    getchar(); // Clear buffer
    fgets(content, sizeof(content), stdin);
    
    // Write to file
    int bytes_written = write(fd, content, strlen(content));
    printf("%d bytes written to file\n", bytes_written);
    
    // Move to beginning and read
    lseek(fd, 0, SEEK_SET);
    char buffer[200];
    int bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    buffer[bytes_read] = '\0';
    
    printf("%d bytes read from file:\n", bytes_read);
    printf("Content: %s", buffer);
    
    close(fd);
}

void demo_creat() {
    printf("=== CREAT() System Call ===\n\n");
    
    char filename[50];
    printf("Enter filename to create: ");
    scanf("%s", filename);
    
    int fd = creat(filename, 0644);
    if (fd != -1) {
        printf("File '%s' created successfully!\n", filename);
        printf("File descriptor: %d\n", fd);
        printf("Permissions: 0644 (rw-r--r--)\n");
        close(fd);
    } else {
        printf("Failed to create file '%s'\n", filename);
    }
    
    //Note: creat() creates new file or truncates existing one
}

void demo_lseek() {
    printf("=== LSEEK() System Call ===\n\n");
    
    // Create test file with known content
    int fd = open("seek_test.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd == -1) {
        printf("Cannot create test file\n");
        return;
    }
    
    const char* data = "0123456789ABCDEFGHIJ";
    write(fd, data, strlen(data));
    printf("Test file created with content: %s\n\n", data);
    
    int position;
    printf("Enter position to seek to (0-19): ");
    scanf("%d", &position);
    
    // Seek to position
    off_t result = lseek(fd, position, SEEK_SET);
    if (result != -1) {
        printf("Seeked to position: %ld\n", result);
        
        // Read from current position
        char buffer[20];
        int bytes = read(fd, buffer, eof(data) - position);
        buffer[bytes] = '\0';
        printf("Read from position %d: '%s'\n", position, buffer);
    } else {
        printf("Seek failed\n");
    }
    
    close(fd);
    unlink("seek_test.txt");
}

void demo_stat() {
    printf("=== STAT() System Call ===\n\n");
    
    char filename[50];
    printf("Enter filename to get info: ");
    scanf("%s", filename);
    
    struct stat file_info;
    if (stat(filename, &file_info) == 0) {
        printf("File Information:\n");
        printf("Size: %ld bytes\n", file_info.st_size);
        printf("Permissions: %o\n", file_info.st_mode & 0777);
        printf("Last modified: %ld\n", file_info.st_mtime);
        printf("Number of links: %d\n", file_info.st_nlink);
    } else {
        printf("File '%s' not found or cannot access\n", filename);
    }
}

void demo_chmod_access() {
    printf("=== CHMOD() and ACCESS() System Calls ===\n\n");
    
    char filename[50];
    printf("Enter filename: ");
    scanf("%s", filename);
    
    // Check if file exists
    if (access(filename, 0) == 0) {
        printf("File exists\n");
        
        // Check permissions
        if (access(filename, 2) == 0) {
            printf("File is writable\n");
        } else {
            printf("File is not writable\n");
        }
        
        if (access(filename, 4) == 0) {
            printf("File is readable\n");
        } else {
            printf("File is not readable\n");
        }
        
        // Change permissions
        printf("Changing permissions to 755...\n");
        if (chmod(filename, 0755) == 0) {
            printf("Permissions changed successfully\n");
        } else {
            printf("Failed to change permissions\n");
        }
    } else {
        printf("File '%s' does not exist\n", filename);
    }
}

void demo_process_calls() {
    printf("=== PROCESS SYSTEM CALLS ===\n\n");
    
    // getpid() demo
    printf("Current Process ID: %d\n", getpid());
    
    printf("\nNote: fork() is not supported in MinGW/Windows\n");
    printf("fork() creates child processes (Unix/Linux only)\n");
    
    printf("\nexit() would terminate the program\n");
    printf("We'll return to menu instead of calling exit()\n");
}

void clear_screen() {
    system("cls"); // Windows equivalent of clear
}

void pause_program() {
    printf("\nPress Enter to continue...");
    while (getchar() != '\n'); // Clear any remaining input
    getchar(); // Wait for Enter
}
