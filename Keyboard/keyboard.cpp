#include "Keyboard.h"
#include <iostream>
#include <windows.h>
#include <QDebug>

#define NUMKEYS 104

int i;
int keyStorage[NUMKEYS] = {            //KEYS                                                          ///INDEX
                                       0xA0,0xC0,0x1B, //left shift, tilde, escape                     :: 0,1,2
                                       0x70,0x71,0x72, //F1-3                                          :: 3,4,5
                                       0x73,0x74,0x75, //F4-6                                          :: 6,7,8
                                       0x76,0x77,0x78, //F7-9                                          :: 9,10,11
                                       0x79,0x7A,0x7B, //F10-12                                        :: 12,13,14
                                       0x30,0x31,0x32, //0-2                                           :: 15,16,17
                                       0x33,0x34,0x35, //3-5                                           :: 18,19,20
                                       0x36,0x37,0x38, //6-8                                           :: 21,22,23
                                       0x39,0x41,0x42, //9, A, B                                       :: 24,25,26
                                       0x43,0x44,0x45, //C, D, E                                       :: 27,28,29
                                       0x46,0x47,0x48, //F, G, H                                       :: 30,31,32
                                       0x49,0x4A,0x4B, //I, J, K                                       :: 33,34,35
                                       0x4C,0x4D,0x4E, //L, M, N                                       :: 36,37,38
                                       0x4F,0x50,0x51, //O, P, Q                                       :: 39,40,41
                                       0x52,0x53,0x54, //R, S, T                                       :: 42,43,44
                                       0x55,0x56,0x57, //U, V, W                                       :: 45,46,47
                                       0x58,0x59,0x5A, //X, Y, Z                                       :: 48,49,50
                                       0x08,0x09,0x0D, //backspace, tab, enter                         :: 51,52,53
                                       0x14,0x20,0x5B, //capslock, space, left windows                 :: 54,55,56
                                       0x5C,0x5D,0x90, //right windows, option key, numlock            :: 57,58,59
                                       0x60,0x61,0x62, //numpad 0-2                                    :: 60,61,62
                                       0x63,0x64,0x65, //numpad 3-5                                    :: 63,64,65
                                       0x66,0x67,0x68, //numpad 6-8                                    :: 66,67,68
                                       0x69,0x6A,0x6B, //numpad 9, numpad multiply, numpad add         :: 69,70,71
                                       0x6C,0x6D,0x6E, //numpad enter, numpad subract, numpad decimal  :: 72,73,74
                                       0x6F,0x21,0x22, //numpad divide, page up, page down             :: 75,76,77
                                       0x23,0x24,0x25, //end, home, left arrow                         :: 78,79,80
                                       0x26,0x27,0x28, //up arrow, right arrow, down arrow             :: 81,82,83
                                       0x2C,0x2D,0x2E, //printscreen, insert, delete                   :: 84,85,86
                                       0xA1,0xA2,0xA3, //right shift, left control, right control      :: 87,88,89
                                       0xBA,0xBB,0xBC, //semicolon, plus, comma                        :: 90,91,92
                                       0xBD,0xBE,0xBF, //minus, period, backslash /                    :: 93,94,95
                                       0xDB,0xDC,0xDD, //left bracket, forwardslash \, right bracket   :: 96,97,98
                                       0x91,0xA4,0xA5, //scroll lock, left alt, right alt              :: 99,100,101
                                       0xDE,0x13};     //quotes, pause break                           :: 102,103

Keyboard::Keyboard() {
    i = 0;
}


void Keyboard::getKeys(){
        for(int j = 0; j < NUMKEYS; j++)
        {
            if(GetAsyncKeyState(keyStorage[j]))
            {
                keys[i] = keyStorage[j];
                i++;
            }
        }
        Sleep(100);
}
