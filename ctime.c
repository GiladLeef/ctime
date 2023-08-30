#include <stdio.h>
#include <windows.h>

void formatTime(DWORD milliseconds, char *output, size_t size) {
    if (milliseconds < 1000) {
        snprintf(output, size, "%u milliseconds", milliseconds);
    } else if (milliseconds < 60000) {
        snprintf(output, size, "%.2f seconds", milliseconds / 1000.0);
    } else if (milliseconds < 3600000) {
        snprintf(output, size, "%.2f minutes", milliseconds / 60000.0);
    } else {
        snprintf(output, size, "%.2f hours", milliseconds / 3600000.0);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command>\n", argv[0]);
        printf("Welcome to CTime! This program will tell you how long it took to run your command.\n");
        return 1;
    }

    // Join the command arguments into a single string
    char command[1024] = "";
    for (int i = 1; i < argc; i++) {
        strcat(command, argv[i]);
        if (i < argc - 1) {
            strcat(command, " ");
        }
    }

    // Create process
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    GetStartupInfo(&si);

    DWORD startTime = GetTickCount();  // Get starting time

    if (CreateProcess(NULL, command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);  // Wait for process to finish
        DWORD endTime = GetTickCount();  // Get ending time

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        // Calculate execution time
        DWORD executionTime = endTime - startTime;
        
        // Format and display execution time
        char formattedTime[50];
        formatTime(executionTime, formattedTime, sizeof(formattedTime));
        printf("Command executed in %s.\n", formattedTime);
    } else {
        printf("Failed to execute command.\n");
        return 1;
    }

    return 0;
}
