#include "rpg.h"
#include "../common/files.h"

static struct datastruct * init(char*);
static int cleanup(struct datastruct *);
static void clearScreen(void);

int main(int argc, char *argv[])
{
    clearScreen();
    printf("rpgengine3000 version %.1f\nThis product is free software released under the GPL v3\nFor more information see the file COPYING\n\n", PLAYER_VERSION);

    if (argc == 1) {
        printf("Error: insufficent arguments\nUsage: %s gamefile\n\n", argv[0]);
        return EXIT_ARGUMENTS;
    }

    struct datastruct *Data  = init(argv[1]);

    printf("Load succesful!\nGame: %s\nCreator: %s\nVersion: %.1f\nBuilt with rpgcreator3000 version: %.1f\n", "NYI", "NYI", 0.0, Data->BuiltWith);

    return cleanup(Data);    
}

static struct datastruct * init(char* filename)
{
    printf("Loading game file %s...\n", filename);

    FILE *gamefile = fopen(filename, "r");

    if (gamefile == NULL) {
        printf("Game file %s not found.\n", filename);
        exit(EXIT_NOGAMEFILE);
    }

    char magic[5] = "\0\0\0\0\0";
    int ret;
    ret = fread(magic, sizeof(char), 4, gamefile);

    if (ret != 4 || strcmp(magic, MAGIC)) {
        printf("Game file %s invalid or corrupt\n", filename);
        fclose(gamefile);
        return NULL;
    }

    struct datastruct *Data = readfile(gamefile);
    fclose(gamefile);

    if (Data == NULL) {
        exit(EXIT_INVALIDGAMEFILE);
    }


    return Data;
}

static int cleanup(struct datastruct *Data)
{
    free(Data);
    return EXIT_SUCCESS;
}

static void clearScreen(void)
{
#if defined _WIN32

/* Untested! */
#include <windows.h>

void ClearScreen()
  {
  HANDLE                     hStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD                      count;
  DWORD                      cellCount;
  COORD                      homeCoords = { 0, 0 };

  hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
  if (hStdOut == INVALID_HANDLE_VALUE) return;

  /* Get the number of cells in the current buffer */
  if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
  cellCount = csbi.dwSize.X *csbi.dwSize.Y;

  /* Fill the entire buffer with spaces */
  if (!FillConsoleOutputCharacter(
    hStdOut,
    (TCHAR) ' ',
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Fill the entire buffer with the current colors and attributes */
  if (!FillConsoleOutputAttribute(
    hStdOut,
    csbi.wAttributes,
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Move the cursor home */
  SetConsoleCursorPosition( hStdOut, homeCoords );
  }
#elif defined __linux__ || defined TARGET_OS_MAC

    /* Use ANSI terminal escapes to move cursor to top left and wipe the screen,  */
    /* Windows are you even trying? */
    printf("\033[H\033[2J");

#else
    #error "Unknown Platform. Please compile on Linux, MacOS X or Windows."
#endif
    return;
}
