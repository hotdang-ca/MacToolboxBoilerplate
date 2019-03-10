/**
 * (c) 2019, James Robert Perih
 * Yes, two-thousand-and-nineteen. Not even kidding.
 *
 * This is just a boilerplate Mac Toolbox C Structure. Needed for basically any Mac app starter.
 *
 */

#include <Controls.h>

// Main Methods
void InitToolbox(void);
void InitWindow(void);
void DoHandleEvent(EventRecord*);
void DoRunLoop(void);

// Handler Methods
void HandleKeyDown(EventRecord*);
void HandleMouseDown(EventRecord*);
void HandleUpdate(EventRecord*);
void HandleOSEvent(EventRecord*);

// Globals
Boolean endProgram; // if we can should exit the run loop and end the program

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
    theWindow = GetNewWindow(128, nil, (WindowPtr)-1L);
    SetPort(theWindow);
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
    InitWindow();
    DoRunLoop();
}
