#include "rpg.h"

int mainLoop(const struct datastruct *Data, struct mapnode *Location)
{   
    printf("%p %p\n%p %p\n", (void *)Location, (void *)&Data->Map[0], (void *)Location->Exit[0], (void *)&Data->Map[1]);
    printDescription(*Location);

    while (handleInput(Data, &Location) == false)
        ;

    return EXIT_SUCCESS;
}

static bool handleInput(const struct datastruct *Data, struct mapnode **Location)
{
    char input = getchar();
    flushchar(input);
    input = tolower(input);
    switch (input) {
    case 'q' :
        return true;
    case 'l' :
        printDescription(**Location);
        break;
    case 'n' :
        movePlayer(Location, EXIT_NORTH);
        break;
    case 's' :
        movePlayer(Location, EXIT_SOUTH);
        break;
    case 'e' :
        movePlayer(Location, EXIT_EAST);
        break;
    case 'w' :
        movePlayer(Location, EXIT_WEST);
        break;
    case 'u' :
        movePlayer(Location, EXIT_UP);
        break;
    case 'd' :
        movePlayer(Location, EXIT_DOWN);
        break;
    }
    return false;
}

static void printDescription(struct mapnode Location)
{
    clearScreen();
    printf("%s\n\n%s\nExits:\n", Location.Name, Location.Desc);
    if (Location.ValidExits & NORTH_VALID)
        puts("North <N> ");
    if (Location.ValidExits & SOUTH_VALID)
        puts("South <S> ");
    if (Location.ValidExits & EAST_VALID)
        puts("East <E> ");
    if (Location.ValidExits & WEST_VALID)
        puts("West <W> ");
    if (Location.ValidExits & UP_VALID)
        puts("Up <U> ");
    if (Location.ValidExits & DOWN_VALID)
        puts("Down <D> ");
    if (Location.ValidExits & EXTRA0_VALID)
        printf("%s <%c> ", Location.ExitName[0], Location.ExitLetter[0]);
    if (Location.ValidExits & EXTRA1_VALID)
        printf("%s <%c> ", Location.ExitName[1], Location.ExitLetter[1]);


}

static void movePlayer(struct mapnode **Location, short direction)
{
    int exitflags = exit2Flag(direction);
    if ((**Location).ValidExits & exitflags) {
        *Location = (*Location)->Exit[direction];
        printDescription(**Location);
    }
    return;
}

