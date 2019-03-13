/**
 * (c) 2019, James Robert Perih
 * Yes, two-thousand-and-nineteen. Not even kidding.
 *
 * This is just a boilerplate Mac Toolbox C Structure. Needed for basically any Mac app starter.
 *
 */

// Main Methods
void InitToolbox(void);
void InitWindow(void);
void InitMenu(void);
void DoHandleEvent(EventRecord*);
void DoRunLoop(void);

// Handler Methods
void HandleKeyDown(EventRecord*);
void HandleMouseDown(EventRecord*);
void HandleUpdate(EventRecord*);
void HandleOSEvent(EventRecord*);
void HandleMenu(EventRecord*);

// Globals
Boolean endProgram; // if we can should exit the run loop and end the program

// Enums
enum {
    kMainWindowId = 128,

    kAlertId = 128,

    kMenuBarId = 128,

    kMenuAppleId = 129,
    kMenuAppleAboutId = 1,

    kMenuMainId = 128,
    kMenuMainBeepId = 1,
    kMenuMainQuitId = 3
};
/**
 * Handle Window (needs) Update events
 */
void HandleUpdate(EventRecord* theEvent) {
    BeginUpdate((WindowPtr)theEvent->message);
    EndUpdate((WindowPtr)theEvent->message);
}

/**
 * Handle Keydown Events
 */
void HandleKeyDown(EventRecord* theEvent) {
    switch (theEvent->what) {
        default:
            MoveTo(10, 20);
            DrawString("\pKey pressed.");
	}
}

/**
 * Handle menu navigation
 */
void HandleMenu(EventRecord* theEvent) {
    long menuAndItem;
    short theMenu;
    short theMenuItem;

    menuAndItem = MenuSelect(theEvent->where);
    if (menuAndItem > 0) {
        theMenu = HiWord(menuAndItem);
        theMenuItem = LoWord(menuAndItem);

        switch (theMenu) {
            case kMenuAppleId:
                // the apple menu
                switch (theMenuItem) {
                    case kMenuAppleAboutId: // show about
                        NoteAlert(128, NULL);
                        break;
                }
                break;
            case kMenuMainId:
                // the main menu
                switch (theMenuItem) {
                    case kMenuMainBeepId: // beep
                        SysBeep(1);
                        break;
                    case kMenuMainQuitId: // quit
                        endProgram = true;
                        break;
                }
                break;
        }

        HiliteMenu(0);
    }
}
/**
 * Handle Mousedown events
 */
void HandleMouseDown(EventRecord* theEvent) {
    WindowPtr whichWindow;
    WindowPartCode theWindowPart;

    theWindowPart = FindWindow(theEvent->where, &whichWindow);

    switch (theWindowPart) {
        case inDrag:
            DragWindow(whichWindow, theEvent->where, &qd.screenBits.bounds);
            break;

        case inGoAway:
            if (TrackGoAway(whichWindow, theEvent->where)) {
                DisposeWindow(whichWindow);
                endProgram = true;
			}
            break;

        case inSysWindow:
            SystemClick(theEvent, whichWindow);
            break;

        case inMenuBar:
            HandleMenu(theEvent);
            break;
    }
}

void HandleOSEvent(EventRecord* theEvent) {
    WindowRef window;
    SInt16 osMessage;
    Boolean isNowActive;

    osMessage = (theEvent->message & osEvtMessageMask) >> 24;

    switch (osMessage) {
        case suspendResumeMessage:
            HandleUpdate(theEvent);
            MoveTo(10, 60);

            if ((window = FrontWindow()) != NULL) {
                isNowActive = (theEvent->message & resumeFlag) != 0;
                if (isNowActive) {
                    DrawString("\pWelcome back!");
                } else {
                    DrawString("\pWait... where did you go?");
                }
            } else {
                // not our window, but how would this ever happen?!
            }
            break;

        case mouseMovedMessage:
            break;

        default:
            MoveTo(10, 90);
            DrawString("\pI don't know what happened...");
            break;
    }
}

/**
 * Standard Mac Toolbox Init
 */
void InitToolbox(void) {
    InitGraf(&qd.thePort);
    InitFonts();
    InitWindows();
    InitMenus();
    TEInit();
    InitDialogs(0L);
    FlushEvents(everyEvent, 0);
    InitCursor();
}

/**
 * Sets up our main window.
 */
void InitWindow(void) {
    WindowPtr theWindow;

    theWindow = GetNewWindow(kMainWindowId, nil, (WindowPtr)-1L);
    SetWTitle(theWindow, "\pMacBoilerPlate");

    SetPort(theWindow);
}

/**
 * Sets up the Menu resource)
 */
void InitMenu(void) {
    Handle menuBarHandle;
    MenuHandle appleMenuHandle;

    menuBarHandle = GetNewMBar(kMenuBarId);
    SetMenuBar(menuBarHandle);

    appleMenuHandle = GetMenuHandle(kMenuAppleId);
    AppendResMenu(appleMenuHandle, 'DRVR');

    DrawMenuBar();
}

/**
 * Main event handler
 */
void DoHandleEvent(EventRecord* theEvent) {
    switch (theEvent->what) {
        case keyDown:
            HandleKeyDown(theEvent); // TODO: probably follow with the keycode...
            break;

        case mouseDown:
            HandleMouseDown(theEvent);
            break;

        case osEvt:
            HandleOSEvent(theEvent);
            break;

        case updateEvt:
            HandleUpdate(theEvent);
            break;
    }
}

/**
 * Main Runloop
 */
void DoRunLoop(void) {
    EventRecord theEvent;

    endProgram = false;
    while (endProgram != true) {
        if (WaitNextEvent(everyEvent, &theEvent, 30L, nil)) {
            DoHandleEvent(&theEvent);
        } else {
            if (FrontWindow()) {
                // IdleControls(FrontWindow());
            }
        }
    }
}

/**
 * MAIN
 */
void main(void)
{
    InitToolbox();
    InitMenu();
    InitWindow();
    DoRunLoop();
}
