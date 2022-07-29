#include "include/owper_ncurses.h"

owper_ncurses::owper_ncurses():baseOwperGUI() {
    this->userItems = NULL;
    this->owperMenuWin = NULL;
	this->owperMenu = NULL;
	this->curUserItem = NULL;
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
    const char* title = "Open Windows Password ERadicator";
    mvprintw(1, (COLS - strlen(title)) / 2, "%s", title);

    /* create the menu */
    owperMenu = new_menu((ITEM **)(this->userItems));

    /* Create window for menu */
    this->owperMenuWin = newwin(LINES - VERT_BORDER_SPACE, COLS - HORIZ_BORDER_SPACE, TOP_BORDER, LEFT_BORDER);
    set_menu_win(this->owperMenu, this->owperMenuWin);
    set_menu_sub(this->owperMenu, derwin(this->owperMenuWin, LINES - (VERT_BORDER_SPACE + 2), COLS - (HORIZ_BORDER_SPACE + 2), 1, 1));
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
    // TODO catch the exception that might be thrown here and tell the user what the result of the load was
    HIVE_LOAD_RESULT loadResult = baseOwperGUI::changeHivePath(newPath); // calls loadUsers
    this->loadGUI();
    return loadResult;
}

void owper_ncurses::loadUsers() {
    vector<samUser*> samUserList = this->sam->getUserList();

    this->userItems = (ITEM **)calloc(samUserList.size() + 1, sizeof(ITEM *));
    for(unsigned int i = 0; i < samUserList.size(); i++) {
        userWidget *curUserWidget = new userWidget(samUserList.at(i), i);
        std::cout << (char*)item_name(curUserWidget->getMenuItem()) << std::endl;
        this->userItems[i] = curUserWidget->getMenuItem();

        if(this->deflt == NULL && !(samUserList.at(i)->getMSAccount().empty())) {
            item_opts_off(this->userItems[i], O_SELECTABLE);
        }

        vectUserWidgets.push_back(curUserWidget);
    }

	this->userItems[samUserList.size()] = (ITEM *)NULL;
}

void owper_ncurses::clearUsers(bool isShutdown/*=false*/) {
    if(owperMenu) {
        unpost_menu(owperMenu);
        free_menu(owperMenu);
        baseOwperGUI::clearUsers(isShutdown); // deletes user widgets, which deletes menu items
        owperMenu = NULL;
    } else {
        baseOwperGUI::clearUsers(isShutdown);
    }
}

owper_ncurses::~owper_ncurses() {
    this->clearUsers(true);
    endwin();
    delscreen(this->owperScreen);
    free(this->userItems);
}
