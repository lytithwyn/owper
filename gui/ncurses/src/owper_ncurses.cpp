#include "include/owper_ncurses.h"

owper_ncurses::owper_ncurses():baseOwperGUI() {
    this->userItems = NULL;
    this->owperMenuWin = NULL;
    this->owperMenuSubWin = NULL;
    this->owperMenu = NULL;
    this->owperScreen = NULL;
}

void owper_ncurses::main() {
    int c;
    while((c = wgetch(this->owperMenuWin)) != 'q') {
        switch(c) {
            case KEY_DOWN:
                menu_driver(this->owperMenu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(this->owperMenu, REQ_UP_ITEM);
                break;
            case 10: /* Enter */
                this->displayMessage("Here is a message to test");
                touchwin(this->owperMenuWin);
                wrefresh(this->owperMenuSubWin);
                pos_menu_cursor(this->owperMenu);
                break;
        }
    }
}

void owper_ncurses::loadGUI() {
    int TOP_BORDER = 3;
    int BOTTOM_BORDER = 4;
    int VERT_BORDER_SPACE = TOP_BORDER + BOTTOM_BORDER;
    int LEFT_BORDER = 1;
    int RIGHT_BORDER = 1;
    int HORIZ_BORDER_SPACE = LEFT_BORDER + RIGHT_BORDER;
    /* COLS is defined by ncurses */
    /* LINES is defined by ncurses */

    /* Initialize curses */
    // the registry library pukes stuff onto stdout and stderr
    // so we have to create a new "screen" for ncurses to output on.
    // honestly, I copied and pasted the bit about /dev/tty from linuxquestions so I just hope it's right
    FILE* termFS = fopen("/dev/tty", "r+"); // create a new file stream for ncurses to use for output
    this->owperScreen = newterm(NULL, termFS, termFS); // create a new screen for ncurses to output to base on this file stream
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    /* print title */
    const char* title = baseOwperGUI::WIN_TITLE;
    mvprintw(1, (COLS - strlen(title)) / 2, "%s", title);

    /* create the menu */
    owperMenu = new_menu((ITEM **)(this->userItems));

    /* Create window for menu */
    this->owperMenuWin = newwin(LINES - VERT_BORDER_SPACE, COLS - HORIZ_BORDER_SPACE, TOP_BORDER, LEFT_BORDER);
    this->owperMenuSubWin = derwin(this->owperMenuWin, LINES - (VERT_BORDER_SPACE + 2), COLS - (HORIZ_BORDER_SPACE + 2), 1, 1);
    set_menu_win(this->owperMenu, this->owperMenuWin);
    set_menu_sub(this->owperMenu, this->owperMenuSubWin);
    keypad(this->owperMenuWin, TRUE);
    box(this->owperMenuWin, 0, 0);
    mvwprintw(this->owperMenuWin, 0, 10, "%s", " User Accounts ");

    /* print the commands at the bottom */
    mvprintw(LINES - 4, LEFT_BORDER, "Use the Up and Down arrow keys to navigate (q to Exit)");
    mvprintw(LINES - 3, LEFT_BORDER, "Use SPACE to select or deselect a user");
    mvprintw(LINES - 2, LEFT_BORDER, "Press C to clear password(s)");
    mvprintw(LINES - 1, LEFT_BORDER, "Press E to toggle account(s) enabled/disabled");

    refresh();
    post_menu(owperMenu);
    wrefresh(owperMenuWin);
}

HIVE_LOAD_RESULT owper_ncurses::changeHivePath(string newPath) {
    // TODO tell the user what the result of the load was
    HIVE_LOAD_RESULT loadResult = 0;
    try {
        loadResult = baseOwperGUI::changeHivePath(newPath); // calls loadUsers on success
    } catch(owpException* e) {
        delete e;
        return 0;
    }

    this->loadGUI();
    return loadResult;
}

void owper_ncurses::loadUsers() {
    vector<samUser*> samUserList = this->sam->getUserList();

    this->userItems = (ITEM **)calloc(samUserList.size() + 1, sizeof(ITEM *));
    for(unsigned int i = 0; i < samUserList.size(); i++) {
        userWidget *curUserWidget = new userWidget(samUserList.at(i), i);
        this->userItems[i] = curUserWidget->getMenuItem();

        if(this->deflt == NULL && !(samUserList.at(i)->getMSAccount().empty())) {
            item_opts_off(this->userItems[i], O_SELECTABLE);
        }

        vectUserWidgets.push_back(curUserWidget);
    }

    this->userItems[samUserList.size()] = (ITEM *)NULL;
}

void owper_ncurses::clearUsers(bool isShutdown/*=false*/) {
    baseOwperGUI::clearUsers(isShutdown); // deletes user widgets, which deletes menu items

    if(this->userItems != NULL) {
        free(this->userItems); // clearUsers deletes the menu items, but doesn't free the array itself
    }
}

// bad things will happen if we pass in VERY long messages
// or if our ncurses window is VERY small
// TODO do checks to make sure none of the calculations
// done in this method result in things like negative sizes
void owper_ncurses::displayMessage(char* message) {
    int TOP_BOTTOM_BORDER = 5;
    int SIDE_BORDER = 20;
    int TEXT_PADDING = 1;
    int BOX_WIDTH = COLS - (2*SIDE_BORDER);
    int TEXT_AREA_WIDTH = BOX_WIDTH - (2*TEXT_PADDING);
    int POS_LINE = TOP_BOTTOM_BORDER;
    int POS_COL = SIDE_BORDER;

    int messageLength = static_cast<int>(strlen(message)); // this cast could technically overflow
    div_t divResult = div(messageLength, TEXT_AREA_WIDTH);
    int messageNumLines = divResult.quot + (divResult.rem > 0)?(1):(0);
    int BOX_HEIGHT = messageNumLines;

    WINDOW* messageWindow = newwin(BOX_HEIGHT + (2 * TEXT_PADDING), BOX_WIDTH, POS_LINE, POS_COL);
    keypad(messageWindow, TRUE);
    box(messageWindow, 0, 0);

    int messageCharIndex = 0;
    int lineIndex = 0;
    char lineToWrite[BOX_WIDTH - (2 * TEXT_PADDING) + 1] = {0};
    while(messageCharIndex < messageLength) {
        bzero(lineToWrite, BOX_WIDTH + 1);
        int charsToCopy = TEXT_AREA_WIDTH;
        int remainingChars = messageLength - messageCharIndex;
        // make sure we don't read past the end of the message
        if(remainingChars < TEXT_AREA_WIDTH) {
            charsToCopy = TEXT_AREA_WIDTH;
        }

        // copy the current section of the message
        strncpy(lineToWrite, &(message[messageCharIndex]), charsToCopy);
        messageCharIndex += charsToCopy;

        mvwprintw(messageWindow, TEXT_PADDING, TEXT_PADDING, "%s", lineToWrite);
    }

    wrefresh(messageWindow);
    wgetch(messageWindow); // wait for any keypress
    werase(messageWindow);
    delwin(messageWindow);
}

owper_ncurses::~owper_ncurses() {
    if(this->owperMenu != NULL) {
        unpost_menu(this->owperMenu);
        free_menu(this->owperMenu); // must delete menu BEFORE items
        delwin(this->owperMenuSubWin);
        delwin(this->owperMenuWin);
    }

    this->clearUsers(true);
    endwin();
    delscreen(this->owperScreen);
}
