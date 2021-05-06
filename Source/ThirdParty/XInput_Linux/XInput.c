/*!***************************************************************************************
\file       XInput.c
\author     Tyler Robbins
\date       9/7/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is a minimal re-implementation of the Win32 XInput library using Linux system calls.
\details For information on how "linux/joystick.h" works, please see this
          document: https://www.kernel.org/doc/Documentation/input/joystick-api.txt
*****************************************************************************************/

//========Self Include==================================================================//
#include "xinput.h"
//========1st Party Includes============================================================//
#include <stdio.h> // open, read, write, snprintf
#include <stdlib.h> // malloc
#include <limits.h> // SHRT_MAX
#include <errno.h> // errno

//========3rd Party Includes============================================================//

#include <linux/joystick.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

// TODO: If we ever want to implement the rest of XInput, we can take a look at
//  the information here: https://www.kernel.org/doc/html/v5.1/input/ff.html

// Note: a better way to get the js devices would be to invoke libudev for
//   easy enumeration. It would allow us to work across _any_ distribution
// However, that would take more time than is really worth, so we can just
//   support the most common way of handling it.

//========Types=========================================================================//
//========Defines=======================================================================//

//! Macro showing the base format string for how to find joystick device files
#define LINUX_JS_BASE "/dev/input/js%d"

//! Helper macro which defines what the ID is for no vibration effect
#define VIBRATION_NO_EFFECT -1

//========Forward Deceleration==========================================================//

/////////========================================================================/////////
///////// Namespace                                                              /////////
/////////========================================================================/////////

////////==========================================================================////////
////////  Struct / Class                                                          ////////
////////==========================================================================////////

/*!***************************************************************************************
\brief Wrapper around XINPUT_STATE, with additional information that Windows
       doesn't usually make available to the end user
*****************************************************************************************/
typedef struct _LinuxXInputState {
    XINPUT_STATE win32_state;

    BOOL connected;
    BOOL can_rumble;
    char* dev_path;
    int file_desc;
    int effect_id; // for force-feedback (i.e: vibration)
} LinuxXInputState;

///////============================================================================///////
///////   Public/Protected/Private                                                 ///////
///////============================================================================///////

//////==============================================================================//////
//////    (Non-)Static                                                              //////
//////==============================================================================//////

/////================================================================================/////
/////     Functions / Data                                                           /////
/////================================================================================/////

//! An array of the state of every gamepad
static LinuxXInputState gamepad_states[XUSER_MAX_COUNT];

//! Whether the "XInput" system has been initialized yet
static BOOL has_initialized = FALSE;

//! An array of string paths specifying the device file for every joystick
static char* js_device_strings[XUSER_MAX_COUNT];

/*!***************************************************************************************
\brief Initializes the "XInput" system exactly once. If it has been called
       already, then the function will do nothing and return.
*****************************************************************************************/
static void initializeXInput() {
    if(has_initialized) return;

    for(int i = 0; i < XUSER_MAX_COUNT; ++i) {
        // We don't need to add any additional space for numbers, as %d gives
        //  us space for 2 digits (99 total joysticks), which is way more than
        //  we will actually need.
        js_device_strings[i] = (char*)malloc(sizeof(LINUX_JS_BASE));
        snprintf(js_device_strings[i], sizeof(LINUX_JS_BASE), LINUX_JS_BASE, i);

        // First setup the base stuff that normal Win32 applications would
        //   expect to see.
        gamepad_states[i].win32_state.dwPacketNumber = 0;
        gamepad_states[i].win32_state.Gamepad.wButtons = 0;
        gamepad_states[i].win32_state.Gamepad.bLeftTrigger = 0;
        gamepad_states[i].win32_state.Gamepad.bRightTrigger = 0;
        gamepad_states[i].win32_state.Gamepad.sThumbLX = 0;
        gamepad_states[i].win32_state.Gamepad.sThumbLY = 0;
        gamepad_states[i].win32_state.Gamepad.sThumbRX = 0;
        gamepad_states[i].win32_state.Gamepad.sThumbRY = 0;

        gamepad_states[i].connected = FALSE;
        gamepad_states[i].can_rumble = FALSE;
        gamepad_states[i].dev_path = js_device_strings[i];
        gamepad_states[i].file_desc = -1;
        gamepad_states[i].effect_id = VIBRATION_NO_EFFECT;
    }

    has_initialized = TRUE;
}

/*!***************************************************************************************
\brief Adds the given device_id as a usable gamepad.
\details Will attempt to establish R/W permissions to enable force-feedback. If that fails,
         the gamepad will be opened with only R permissions.

\param device_id The ID of the device to add. Must be less than XUSER_MAX_COUNT.

\return 0 upon success, -1 upon error.
*****************************************************************************************/
static int addDevice(DWORD p_device_id) {
    // bad device id
    if(p_device_id > XUSER_MAX_COUNT) {
        syslog(LOG_WARNING, "Device ID %d is greater than the maximum of %d.", p_device_id, XUSER_MAX_COUNT);
        return -1;
    }

    // Also note, we open the device in non-blocking mode so that if there are
    //  no events for the joystick, we don't end up sitting forever waiting for
    //  one to arrive.

    // Read-write is required for rumbling
    gamepad_states[p_device_id].file_desc = open(gamepad_states[p_device_id].dev_path,
                                                 O_RDWR | O_NONBLOCK);
    if(gamepad_states[p_device_id].file_desc != -1) {
        gamepad_states[p_device_id].connected = TRUE;
        gamepad_states[p_device_id].can_rumble = TRUE;
        return 0;
    } else if(errno == ENOENT) {
        // TODO: Should we write a warning in a debug sense?
        return -1;
    }

    syslog(LOG_WARNING, "Failed to open device %d (%s) for writing! Cannot vibrate. Reason: %s", p_device_id, gamepad_states[p_device_id].dev_path, strerror(errno));

    // Try to open just for reading (no rumbling capabilities)
    if(errno == EACCES) {
        gamepad_states[p_device_id].file_desc = open(gamepad_states[p_device_id].dev_path,
                                                     O_RDONLY | O_NONBLOCK);
        if(gamepad_states[p_device_id].file_desc != -1) {
            gamepad_states[p_device_id].connected = TRUE;
            return 0;
        }
    }

    syslog(LOG_ERR, "Failed to open device %d for either reading or writing! Reason: %s", p_device_id, strerror(errno));

    return -1;
}

/*!***************************************************************************************
\brief Sends data to a connected controller. This function is used to activate
       the vibration function of a controller.
\details https://docs.microsoft.com/en-us/windows/win32/api/xinput/nf-xinput-xinputsetstate

\param dwUserIndex Index of the user's controller. Can be a value from 0 to 3.
\param pVibration Pointer to an XINPUT_VIBRATION structure containing the
                  vibration information to send to the controller.

\return ERROR_SUCCESS upon success, ERROR_DEVICE_NOT_CONNECTED if the device
        could not be opened. "errno" if the device is not opened with R/W perms.
*****************************************************************************************/
DWORD XInputSetState(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration) {
    initializeXInput();

    // If we can't actually rumble, go ahead and just return now, as there isn't
    //   any point in trying to do so otherwise.
    if(!gamepad_states[dwUserIndex].can_rumble) {
        return ERROR_ACCESS_DENIED;
    }

    if(!gamepad_states[dwUserIndex].connected && addDevice(dwUserIndex) < 0) {
        // Don't bother using the windows error codes (I doubt anybody is checking them anyway)
        return ERROR_DEVICE_NOT_CONNECTED;
    }

    // For additional documentation, see:
    // https://github.com/torvalds/linux/blob/master/include/uapi/linux/input.h
    // https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h

    struct input_event l_vibration_event;

    // Make sure that we stop the current vibration effect, if one is currently
    //  ongoing
    if(gamepad_states[dwUserIndex].effect_id != VIBRATION_NO_EFFECT) {
        // Before we free the memory of the effect, stop performing it first.

        l_vibration_event.type = EV_FF; // Linux calls vibration "Force Feedback"
        l_vibration_event.code = gamepad_states[dwUserIndex].effect_id;
        l_vibration_event.value = 0;

        write(gamepad_states[dwUserIndex].file_desc, &l_vibration_event,
              sizeof(struct input_event));

        // Make sure that we request the effect to be removed from memory so
        //  that we have enough memory for additional effects later.
        ioctl(gamepad_states[dwUserIndex].file_desc, EVIOCRMFF,
              gamepad_states[dwUserIndex].effect_id);
    }

    // Use the original motor values, as shorts are more reliable than floats
    //  when comparing to 0
    if(pVibration->wLeftMotorSpeed != 0 || pVibration->wRightMotorSpeed != 0) {
        struct ff_effect l_new_effect;

        // First we need to create and upload the new vibration "effect"

        // TODO: Can we somehow accept different effect types from Windows?
        l_new_effect.type = FF_RUMBLE;
        l_new_effect.id = -1; // -1 tells Linux to allocate a new effect for us
        // TODO: Do we need to set a direction?

        // Run for 5s, do not delay
        l_new_effect.replay.length = 5;
        l_new_effect.replay.delay = 0;

        l_new_effect.u.rumble.strong_magnitude = pVibration->wLeftMotorSpeed;
        l_new_effect.u.rumble.weak_magnitude = pVibration->wRightMotorSpeed;

        // Actually upload the new effect.
        if(ioctl(gamepad_states[dwUserIndex].file_desc, EVIOCSFF, &l_new_effect) != -1) {
            // If we succeeded in uploading the effect, then we can set the
            //  gamepad's state to this effect's ID

            gamepad_states[dwUserIndex].effect_id = l_new_effect.id;
        } else {
            return errno; // Since MS's documentation is extremely unhelpful,
                          //  we'll just be returning standard errno.
        }

        // We can now play the effect now that it has been uploaded.
        l_vibration_event.type = EV_FF;
        l_vibration_event.code = gamepad_states[dwUserIndex].effect_id;
        l_vibration_event.value = 1;

        write(gamepad_states[dwUserIndex].file_desc, &l_vibration_event,
              sizeof(struct input_event));
    }

    return ERROR_SUCCESS;
}

/*!***************************************************************************************
\brief Retrieves the current state of the specified controller.
\details https://docs.microsoft.com/en-us/windows/win32/api/xinput/nf-xinput-xinputgetstate

\param dwUserIndex Index of the user's controller. Can be a value from 0 to 3.
\param pState Pointer to an XINPUT_STATE structure that receives the current
              state of the controller.

\return ERROR_SUCCESS upon success, ERROR_DEVICE_NOT_CONNECTED if the device
        could not be opened. If some other error has occurred, the value of
        errno is returned.
*****************************************************************************************/
DWORD XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState) {
    // TODO: Check if state has changed since last polled
    //  if so, then increase dwPacketNumber
    // See: https://docs.microsoft.com/en-us/windows/win32/api/xinput/ns-xinput-xinput_state

    initializeXInput();

    // First, is it connected, and if not, can we open it?
    if(!gamepad_states[dwUserIndex].connected && addDevice(dwUserIndex) < 0) {
        return ERROR_DEVICE_NOT_CONNECTED;
    }

    struct js_event event;

    while(read(gamepad_states[dwUserIndex].file_desc, &event,
               sizeof(struct js_event)) > 0)
    {
        XInputGamepadButtons button;

        // Don't switch with the JS_EVENT_INIT bit set
        // This is because if one happens, the type will be:
        //   JS_EVENT_{BUTTON,AXIS} | JS_EVENT_INIT
        switch(event.type & ~JS_EVENT_INIT) {
            case JS_EVENT_BUTTON:
                switch(event.number) {
                    case 0: // A
                        button = XINPUT_GAMEPAD_A;
                        break;
                    case 1: // B
                        button = XINPUT_GAMEPAD_B;
                        break;
                    case 2: // X
                        button = XINPUT_GAMEPAD_X;
                        break;
                    case 3: // Y
                        button = XINPUT_GAMEPAD_Y;
                        break;
                    case 4: // LSHOULDER
                        button = XINPUT_GAMEPAD_LEFT_SHOULDER;
                        break;
                    case 5: // RSHOULDER
                        button = XINPUT_GAMEPAD_RIGHT_SHOULDER;
                        break;
                    case 6: // BACK
                        button = XINPUT_GAMEPAD_BACK;
                        break;
                    case 7: // START
                        button = XINPUT_GAMEPAD_START;
                        break;
                    case 9: // LTHUMB
                        button = XINPUT_GAMEPAD_LEFT_THUMB;
                        break;
                    case 10: // RTHUMB
                        button = XINPUT_GAMEPAD_RIGHT_THUMB;
                        break;
                    default: break; // DO NOTHING
                }
                // value tells us if the button is being depressed or not
                if(event.value)
                    pState->Gamepad.wButtons |= button;
                else
                    // Equivalent to wButtons &= ~button
                    pState->Gamepad.wButtons ^= button;
                break;
            case JS_EVENT_AXIS:
                switch(event.number) {
                    case 0: // Left Stick X
                        pState->Gamepad.sThumbLX = (short)event.value;
                        break;
                    case 1: // Left Stick Y
                        pState->Gamepad.sThumbLY = (short)event.value;
                        break;
                    case 2: // Left Trigger
                        pState->Gamepad.bLeftTrigger = (event.value + SHRT_MAX + 1) >> 8;
                        break;
                    case 3: // Right Stick X
                        pState->Gamepad.sThumbRX = (short)event.value;
                        break;
                    case 4: // Right Stick Y
                        pState->Gamepad.sThumbRY = (short)event.value;
                        break;
                    case 5: // Right Trigger
                        pState->Gamepad.bRightTrigger = (event.value + SHRT_MAX + 1) >> 8;
                        break;
                    case 6: // DPAD Left+Right
                        if(event.value == SHRT_MIN) {
                            pState->Gamepad.wButtons |= XINPUT_GAMEPAD_DPAD_LEFT;
                            pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_RIGHT;
                        } else if(event.value == SHRT_MAX) {
                            pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_LEFT;
                            pState->Gamepad.wButtons |= XINPUT_GAMEPAD_DPAD_RIGHT;
                        } else {
                            pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_LEFT | ~XINPUT_GAMEPAD_DPAD_RIGHT;
                        }
                        break;
                    case 7: // DPAD Up+Down
                        if(event.value == SHRT_MIN) {
                            pState->Gamepad.wButtons |= XINPUT_GAMEPAD_DPAD_UP;
                            pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_DOWN;
                        } else if(event.value == SHRT_MAX) {
                            pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_UP;
                            pState->Gamepad.wButtons |= XINPUT_GAMEPAD_DPAD_DOWN;
                        } else {
                            pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_UP | ~XINPUT_GAMEPAD_DPAD_DOWN;
                        }
                        break;
                    default: break;
                }
                break;
            default:
                break;
        }
    }

    // EAGAIN is just that there are no more events to consume
    // Therefore, if errno is _not_ EAGAIN, then something has really gone
    //   wrong.
    if(errno != EAGAIN) {
        return errno;
    }

    return ERROR_SUCCESS;
}

