#include "mainwindow.h"
#include "startup_window.h"
#include <QApplication>
#include <windows.h>
#include <QHotkey>
#include <QMutex>
#include <QFile>
#include <QDebug>
#include <QClipboard>
QMutex mutex;
bool short_cut;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(0x82b7b1accf2671b5);
    w.setWindowFlags(Qt::CustomizeWindowHint );
    w.show();
    QHotkey hotkey(QKeySequence("ctrl+x"), true);
    qDebug() << "Is Registered: " << hotkey.isRegistered();
    QObject::connect(&hotkey, &QHotkey::activated, qApp, [&](){
        QString user = w.get_user();
        QByteArray ba = user.toLatin1();
        const char *output = ba.data();
        const size_t len = strlen(output) + 1;
        HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, len);
        memcpy(GlobalLock(hMem), output, len);
        GlobalUnlock(hMem);
        OpenClipboard(0);
        EmptyClipboard();
        SetClipboardData(CF_TEXT, hMem);
        CloseClipboard();
        INPUT ctrlV [4];
        ZeroMemory(ctrlV, sizeof ctrlV);
        ctrlV [0].type = INPUT_KEYBOARD;
        ctrlV [0].ki.wVk = VK_LCONTROL;
        ctrlV [1].type = INPUT_KEYBOARD;
        ctrlV [1].ki.wVk = 0x56; //assuming ASCII
        ctrlV [2].type = INPUT_KEYBOARD;
        ctrlV [2].ki.wVk = 0x56; //assuming ASCII
        ctrlV [2].ki.dwFlags = KEYEVENTF_KEYUP;
        ctrlV [3].type = INPUT_KEYBOARD;
        ctrlV [3].ki.wVk = VK_LCONTROL;
        ctrlV [3].ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput (4, ctrlV, sizeof (INPUT));
        QString pass =  w.get_pass();
        keybd_event( VK_TAB,0,0,0);
        Sleep(300);
        QByteArray ba1 = pass.toLatin1();
        const char *output1 = ba1.data();
        const size_t len1 = strlen(output1) + 1;
        HGLOBAL hMem1 =  GlobalAlloc(GMEM_MOVEABLE, len1);
        memcpy(GlobalLock(hMem1), output1, len1);
        GlobalUnlock(hMem1);
        OpenClipboard(0);
        EmptyClipboard();
        SetClipboardData(CF_TEXT, hMem1);
        CloseClipboard();
        ZeroMemory(ctrlV, sizeof ctrlV);
        ctrlV [0].type = INPUT_KEYBOARD;
        ctrlV [0].ki.wVk = VK_LCONTROL;
        ctrlV [1].type = INPUT_KEYBOARD;
        ctrlV [1].ki.wVk = 0x56; //assuming ASCII
        ctrlV [2].type = INPUT_KEYBOARD;
        ctrlV [2].ki.wVk = 0x56; //assuming ASCII
        ctrlV [2].ki.dwFlags = KEYEVENTF_KEYUP;
        ctrlV [3].type = INPUT_KEYBOARD;
        ctrlV [3].ki.wVk = VK_LCONTROL;
        ctrlV [3].ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput (4, ctrlV, sizeof (INPUT));
        Sleep(500);
        keybd_event( VK_RETURN,0,0,0);
    });


    QHotkey hotkey1(QKeySequence("ctrl+z"), true);
    qDebug() << "Is Registered: " << hotkey1.isRegistered();
    QObject::connect(&hotkey1, &QHotkey::activated, qApp, [&](){
        QString user = w.get_user();
        QByteArray ba = user.toLatin1();
        const char *output = ba.data();
        const size_t len = strlen(output) + 1;
        HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, len);
        memcpy(GlobalLock(hMem), output, len);
        GlobalUnlock(hMem);
        OpenClipboard(0);
        EmptyClipboard();
        SetClipboardData(CF_TEXT, hMem);
        CloseClipboard();
        INPUT ctrlV [4];
        ZeroMemory(ctrlV, sizeof ctrlV);
        ctrlV [0].type = INPUT_KEYBOARD;
        ctrlV [0].ki.wVk = VK_LCONTROL;
        ctrlV [1].type = INPUT_KEYBOARD;
        ctrlV [1].ki.wVk = 0x56; //assuming ASCII
        ctrlV [2].type = INPUT_KEYBOARD;
        ctrlV [2].ki.wVk = 0x56; //assuming ASCII
        ctrlV [2].ki.dwFlags = KEYEVENTF_KEYUP;
        ctrlV [3].type = INPUT_KEYBOARD;
        ctrlV [3].ki.wVk = VK_LCONTROL;
        ctrlV [3].ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput (4, ctrlV, sizeof (INPUT));
        QString pass =  w.get_pass();
        Sleep(1000);
        keybd_event( VK_RETURN,0,0,0);
        Sleep(3000);
        QByteArray ba1 = pass.toLatin1();
        const char *output1 = ba1.data();
        const size_t len1 = strlen(output1) + 1;
        HGLOBAL hMem1 =  GlobalAlloc(GMEM_MOVEABLE, len1);
        memcpy(GlobalLock(hMem1), output1, len1);
        GlobalUnlock(hMem1);
        OpenClipboard(0);
        EmptyClipboard();
        SetClipboardData(CF_TEXT, hMem1);
        CloseClipboard();
        ZeroMemory(ctrlV, sizeof ctrlV);
        ctrlV [0].type = INPUT_KEYBOARD;
        ctrlV [0].ki.wVk = VK_LCONTROL;
        ctrlV [1].type = INPUT_KEYBOARD;
        ctrlV [1].ki.wVk = 0x56; //assuming ASCII
        ctrlV [2].type = INPUT_KEYBOARD;
        ctrlV [2].ki.wVk = 0x56; //assuming ASCII
        ctrlV [2].ki.dwFlags = KEYEVENTF_KEYUP;
        ctrlV [3].type = INPUT_KEYBOARD;
        ctrlV [3].ki.wVk = VK_LCONTROL;
        ctrlV [3].ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput (4, ctrlV, sizeof (INPUT));
         Sleep(500);
        keybd_event( VK_RETURN,0,0,0);
    });










    return a.exec();
}
