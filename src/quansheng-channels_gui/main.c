#include "common.h"
#include "minmax.h"

#include <libquansheng-channels/chan.h>
#include <libquansheng-channels/csvChirp.h>
#include <libquansheng-channels/csvChirpFromFile.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <iup.h>
#include <string.h>
#include <libquansheng-channels/libquansheng-channels.h>

qdc_Chan chan = {0};
bool isChanEmpty = true;

int exit_cb(Ihandle *self) {
    unused(self);
    return IUP_CLOSE;
}

Ihandle *progressBar;
Ihandle *list;

unsigned long errorIndex = 1;

void printToList(const char *message, const char *icon) {
    IupSetAttributeId(list, "", errorIndex, message);
    IupSetAttributeId(list, "IMAGE", errorIndex, icon);
    IupSetInt(list, "TOPITEM", errorIndex);
    errorIndex++;
}
static inline void printErrorToList(const char *message) {
    printToList(message, "IUP_MessageError");
}
static inline void printWarningToList(const char *message) {
    printToList(message, "IUP_IconMessageWarning");
}
static inline void printInfoToList(const char *message) {
    printToList(message, "IUP_MessageInfo");
}
static inline void printSuccessToList(const char *message) {
    printToList(message, "IUP_ActionOk");
}

void convert(const char *path) {
    unused(list);
    qdc_CsvChirpFile *csvFile;
    char msg[200];

    switch (qdc_csvChirpFromFileNew(path, &csvFile)) {
        case qdc_CsvChirpFromFileNewErr_FAILED_TO_ALLOCATE_MEMORY:
            sprintf(msg, "Error: Failed to allocate memory.");
            printErrorToList(msg);
            return;
        case qdc_CsvChirpFromFileNewErr_FAILED_TO_OPEN_FILE:
            sprintf(msg, "Error: Failed to open the csv file: %s", path);
            printErrorToList(msg);
            return;
        case qdc_CsvChirpFromFileNewErr_EMPTY_FILE:
            sprintf(msg, "Error: The file %s is empty.", path);
            printErrorToList(msg);
            return;
        case qdc_CsvChirpFromFileNewErr_NONE:
            break;
    }

    qdc_CsvChirpFromFileParseNextChannelErr fileErr;
    unsigned long chIndex;
    qdc_Channel ch;
    qdc_CsvChirpParseChannelErr parserErr;

    IupSetAttribute(progressBar, "VISIBLE", "YES");
    unsigned long discardedChannels = 0;
    while ((fileErr = qdc_CsvChirpFromFileParseNextChannel(csvFile, &chIndex, &ch, &parserErr)) == qdc_CsvChirpFromFileParseNextChannelErr_NONE) {
        if (parserErr == qdc_CsvChirpParseChannelErr_NONE) {
            // No errors, great!
        } else if (parserErr & qdc_CsvChirpParseChannelErr_NAME_TOO_LONG_AUTOFIXED) {
            sprintf(msg, "Info: The name of the channel number %lu has been shrinked to %s in order to fit onto radio screen.", chIndex, ch.name);
            printInfoToList(msg);
        } else if (parserErr & qdc_CsvChirpParseChannelErr_FREQUENCY_MUST_BE_DECIMAL) {
            sprintf(msg, "Warning: Frequency of the channel #%lu %s must be decimal. Defaulting to 0 Hz.", chIndex, ch.name);
            printWarningToList(msg);
        } else if (parserErr & qdc_CsvChirpParseChannelErr_OFFSET_MUST_BE_DECIMAL) {
            sprintf(msg, "Warning: Tx offset of the channel #%lu %s must be decimal. Defaulting to 0 Hz.", chIndex, ch.name);
            printWarningToList(msg);
        } else if (parserErr & qdc_CsvChirpParseChannelErr_TONE_MUST_BE_DECIMAL) {
            sprintf(msg, "Warning: Tone field of the channel #%lu %s must be decimal. Defaulting to no tone.", chIndex, ch.name);
            printWarningToList(msg);
        } else if (parserErr & qdc_CsvChirpParseChannelErr_TONE_SQUELCH_MUST_BE_DECIMAL) {
            sprintf(msg, "Warning: Tone squelch field of the channel #%lu %s must be decimal. Defaulting to no tone squelch.", chIndex, ch.name);
            printWarningToList(msg);
        } else if (parserErr & qdc_CsvChirpParseChannelErr_DTCS_MUST_BE_INTEGER) {
            sprintf(msg, "Warning: DTCS field of the channel #%lu %s must be integer. Defaulting to no DCS.", chIndex, ch.name);
            printWarningToList(msg);
        } else if (parserErr & qdc_CsvChirpParseChannelErr_RX_DTCS_MUST_BE_INTEGER) {
            sprintf(msg, "Warning: Rx DTCS field of the channel #%lu %s must be integer. Defaulting to no Rx DCS.", chIndex, ch.name);
            printWarningToList(msg);
        } else if (parserErr & qdc_CsvChirpParseChannelErr_DTCS_POLARITY_FIELD_TOO_LONG) {
            sprintf(msg, "Warning: DTCS polarity field of the channel #%lu %s is too long. Defaulting to normal polarity.", chIndex, ch.name);
            printWarningToList(msg);
        } else if (parserErr & qdc_CsvChirpParseChannelErr_UNSUPPORTED_FREQUENCY_STEP_AUTOFIXED) {
            sprintf(msg, "Warning: Unsupported frequency step used by the channel #%lu %s. Defaulting to 5 kHz.", chIndex, ch.name);
            printWarningToList(msg);
        } else if (parserErr & qdc_CsvChirpParseChannelErr_POWER_MUST_BE_DECIMAL) {
            sprintf(msg, "Warning: Power field of the channel #%lu %s must be decimal. Defaulting to high power.", chIndex, ch.name);
            printWarningToList(msg);
        } else if (parserErr & qdc_CsvChirpParseChannelErr_LOCATION_MUST_BE_INTEGER) {
            sprintf(msg, "Warning: Location field of the channel %s must be an integer. Defaulting to channel number 0 (first channel).", ch.name);
            printWarningToList(msg);
        } else if (parserErr & qdc_CsvChirpParseChannelErr_LOCATION_MUST_BE_POSITIVE) {
            sprintf(msg, "Warning: Location field of the channel %s must be positive. Defaulting to channel number 0 (first channel).", ch.name);
            printWarningToList(msg);
        } else {
            sprintf(msg, "Error: An unknown error ocured while parding the channel #%lu %s. Tryign to porceed to the next channel...", chIndex, ch.name);
            printErrorToList(msg);
        }

        switch (qdc_chanSetChannel(&chan, chIndex, &ch)) {
            case qdc_ChanSetChannelErr_NONE:
                break;
            case qdc_ChanSetChannelErr_MAX_CHANNEL_NUMBER_IS_199:
                sprintf(msg, "Error: Discarding channel #%lu %s because it does not fit into radio memory. Maximum number of channels is 200. Discarding channel.", chIndex, ch.name);
                printErrorToList(msg);
                discardedChannels++;
                break;
            case qdc_ChanSetChannelErr_MAX_RX_FREQUENCY_IS_1300000000:
                sprintf(msg, "Error: Frequency of channel #%lu %s exceeds the limit of 1.3 GHz. Discarding channel.", chIndex, ch.name);
                printErrorToList(msg);
                discardedChannels++;
                break;
            case qdc_ChanSetChannelErr_TX_OFFSET_BIGGER_THAN_RX_FREQUENCY:
                sprintf(msg, "Error: Tx offset of channel #%lu %s is bigger than rx frequency. Discarding channel.", chIndex, ch.name);
                printErrorToList(msg);
                discardedChannels++;
                break;
            case qdc_ChanSetChannelErr_INVALID_CHANNEL_NAME:
                sprintf(msg, "Error: Name of the channel #%lu %s is invalid. Discarding channel.", chIndex, ch.name);
                printErrorToList(msg);
                discardedChannels++;
                break;
            case qdc_ChanSetChannelErr_NON_ASCII_CHANNEL_NAME_AUTOFIXED:
                sprintf(msg, "Warning: Name of the channel #%lu %s contains non-ASCII characters. Discarding invalid characters.", chIndex, ch.name);
                printErrorToList(msg);
                break;
        }

        char progressStr[4];
        sprintf(progressStr, "%lu", chIndex);
        IupSetAttribute(progressBar, "VALUE", progressStr);
    }
    qdc_csvChirpFromFileDestory(csvFile);

    IupSetAttribute(progressBar, "VALUE", "200");
    IupSetAttribute(progressBar, "VISIBLE", "NO");
    sprintf(msg, "Parsing completed with %lu discarded channels.", discardedChannels);
    if (discardedChannels > 0) {
        printWarningToList(msg);
    } else {
        printSuccessToList(msg);
    }
    isChanEmpty = false;

    return;
}

int open_cb(Ihandle *self) {
    unused(self);
    Ihandle *filedlg = IupFileDlg();
    IupSetAttribute(filedlg, "DIALOGTYPE", "OPEN");
    IupSetAttribute(filedlg, "EXTFILTER", ".csv file|*.csv|All Files|*.*|");

    IupPopup(filedlg, IUP_CENTER, IUP_CENTER);

    if (IupGetInt(filedlg, "STATUS") != -1) {
        char* filename = IupGetAttribute(filedlg, "VALUE");
        convert(filename);
    }

    IupDestroy(filedlg);
    return IUP_DEFAULT;
}

int saveas_cb(Ihandle *self) {
    unused(self);

    if (isChanEmpty) {
        Ihandle* dlg = IupMessageDlg();
        IupSetAttribute(dlg, "TITLE", "Nothing to save");
        IupSetAttribute(dlg, "VALUE", "No channels are loaded. Pleas load a CSV before exporting.");
        IupPopup(dlg, IUP_CURRENT, IUP_CURRENT);
        IupDestroy(dlg);
        return IUP_DEFAULT;
    }

    bool shouldRetry = true;
    while (shouldRetry) {
        shouldRetry = false;
        Ihandle *filedlg = IupFileDlg();
        IupSetAttribute(filedlg, "DIALOGTYPE", "SAVE");
        IupSetAttribute(filedlg, "EXTFILTER", ".chan files|*.chan|All files|*.*|");
        IupSetAttribute(filedlg, "EXTDEFAULT", "chan");

        IupPopup(filedlg, IUP_CENTER, IUP_CENTER);

        if (IupGetInt(filedlg, "STATUS") != -1)
        {
            char* path = IupGetAttribute(filedlg, "VALUE");

            FILE *chanFile = fopen(path, "wb");
            if (chanFile == NULL) {
                Ihandle* dlg = IupMessageDlg();
                IupSetAttribute(dlg, "DIALOGTYPE", "ERROR");
                IupSetAttribute(dlg, "TITLE", "Failed to open file");
                IupSetAttribute(dlg, "BUTTONS", "YESNO");
                IupSetAttribute(dlg, "VALUE", "The path might be invalid or you might not have permissions to write to the destination directory.\n\n Would you like to select a different destination?");
                IupPopup(dlg, IUP_CURRENT, IUP_CURRENT);
                if(IupGetAttribute(dlg, "BUTTONRESPONSE")[0] == '1') {
                    shouldRetry = true;
                }
                IupDestroy(dlg);
            } else {
                fwrite(&chan, sizeof(chan), 1, chanFile);
                fclose(chanFile);
                printSuccessToList("Successfully saved .chan file.");
            }
        }

        IupDestroy(filedlg);
    }

    return IUP_DEFAULT;
}

int font_cb(Ihandle *self) {
    unused(self);
    Ihandle* fontdlg = IupFontDlg();
    char* font = IupGetAttribute(list, "FONT");
    IupSetStrAttribute(fontdlg, "VALUE", font);
    IupPopup(fontdlg, IUP_CENTER, IUP_CENTER);

    if (IupGetInt(fontdlg, "STATUS") == 1)
    {
        char* font = IupGetAttribute(fontdlg, "VALUE");
        IupSetStrAttribute(list, "FONT", font);
    }

    IupDestroy(fontdlg);
    return IUP_DEFAULT;
}

int about_cb(Ihandle *self) {
    unused(self);
    IupMessage("About", "Quansheng channel tool\nConvert between channel formats for different UV-K5 firmwares.\n\nVersion: 0.0.1\n\nJosip Tišljar Mataušić, 9A3SFZ");
    return IUP_DEFAULT;
}

int btn(Ihandle *self) {
    unused(self);
    open_cb(NULL);
    return IUP_DEFAULT;
}

int dropfiles_cb(Ihandle* ih, char* filename, int x, int y) {
    unused(ih);
    unused(x);
    unused(y);
    const char* ext = strrchr(filename, '.');
    if (!ext || strcasecmp(ext, ".csv") != 0) {
        printErrorToList("Only .csv files are supported.");
        return IUP_DEFAULT;
    }

    convert(filename);
    return IUP_DEFAULT;
}

int list_button_cb(Ihandle *ih, int button, int pressed, int x, int y, char *status) {
    unused(status);
    if (button == IUP_BUTTON3 && pressed) {
        Ihandle* menu = IupGetAttributeHandle(ih, "CONTEXT_MENU");
        if (menu) {
            int abs_x, abs_y;
            IupGetIntInt(ih, "SCREENPOSITION", &abs_x, &abs_y);
            IupPopup(menu, abs_x + x, abs_y + y);
        }
    }
    return IUP_DEFAULT;
}

int copy_selected_cb(Ihandle* ih) {
    Ihandle* list = IupGetAttributeHandle(ih, "TARGET_LIST");
    int sel = IupGetInt(list, "VALUE");  // 1-based index of selected item
    if (sel > 0) {
        const char* text = IupGetAttributeId(list, "", sel);
        Ihandle *clipboard = IupClipboard();
        IupSetAttribute(clipboard, "TEXT", text);
    }
    return IUP_DEFAULT;
}

int copy_all_cb(Ihandle* ih) {
    Ihandle* list = IupGetAttributeHandle(ih, "TARGET_LIST");
    int count = IupGetInt(list, "COUNT");
    if (count == 0)
        return IUP_DEFAULT;

    int totalLen = 0;
    for (int i = 1; i <= count; ++i)
        totalLen += strlen(IupGetAttributeId(list, "", i)) + 1; // Concatenate all items with \n

    char* buffer;
    bool shouldRetry = true;
    while (shouldRetry) {
        shouldRetry = false;
        buffer = malloc(totalLen + 1);
        if (buffer == NULL) {
            Ihandle* dlg = IupMessageDlg();
            IupSetAttribute(dlg, "DIALOGTYPE", "ERROR");
            IupSetAttribute(dlg, "TITLE", "Failed to allocate memory");
            IupSetAttribute(dlg, "BUTTONS", "RETRYCANCEL");
            IupSetAttribute(dlg, "VALUE", "Your computer does not have enough memory for this operation.");
            IupPopup(dlg, IUP_CURRENT, IUP_CURRENT);
            if(IupGetAttribute(dlg, "BUTTONRESPONSE")[0] == '1') {
                shouldRetry = true;
            } else {
                IupDestroy(dlg);
                return IUP_DEFAULT;
            }
            IupDestroy(dlg);
        }
    }

    buffer[0] = '\0';
    for (int i = 1; i <= count; ++i) {
        strcat(buffer, IupGetAttributeId(list, "", i));
        if (i < count)
            strcat(buffer, "\n");
    }

    Ihandle *clipboard = IupClipboard();
    IupSetAttribute(clipboard, "TEXT", buffer);
    free(buffer);

    return IUP_DEFAULT;
}

int clear_all_cb(Ihandle* ih) {
    Ihandle* list = IupGetAttributeHandle(ih, "TARGET_LIST");
    IupSetAttribute(list, "REMOVEITEM", "ALL");
    errorIndex = 1;
    memset(&chan, 0, sizeof(chan));
    isChanEmpty = true;
    printInfoToList("Drag or select a CSV file.");
    return IUP_DEFAULT;
}

int main(int argc, char **argv) {
    Ihandle *dlg, *vbox, *hbox;
    Ihandle *file_menu, *item_exit, *item_open, *item_saveas;
    Ihandle *format_menu, *item_font;
    Ihandle *help_menu, *item_about;
    Ihandle *sub_menu_file, *sub_menu_format, *sub_menu_help, *menu;
    Ihandle *loadBtn, *saveBtn;

    IupOpen(&argc, &argv);
    IupImageLibOpen();
    IupSetGlobal("UTF8MODE", "YES");
    IupSetGlobal("UTF8MODE_FILE", "YES");

    item_open = IupItem("Open CSV", NULL);
    item_saveas = IupItem("Save as .chan", NULL);
    item_exit = IupItem("Exit", NULL);
    item_font = IupItem("Font...", NULL);
    item_about = IupItem("About...", NULL);

    IupSetCallback(item_exit, "ACTION", (Icallback)exit_cb);
    IupSetCallback(item_open, "ACTION", (Icallback)open_cb);
    IupSetCallback(item_saveas, "ACTION", (Icallback)saveas_cb);
    IupSetCallback(item_font, "ACTION", (Icallback)font_cb);
    IupSetCallback(item_about, "ACTION", (Icallback)about_cb);

    /*IupSetAttribute(item_exit, "IMAGE", "IUP_ActionCancel");
    IupSetAttribute(item_open, "IMAGE", "IUP_FileOpen");
    IupSetAttribute(item_saveas, "IMAGE", "IUP_FileSave");
    IupSetAttribute(item_font, "IMAGE", "IUP_FileProperties");
    IupSetAttribute(item_about, "IMAGE", "IUP_MessageHelp");*/

    file_menu = IupMenu(item_open, item_saveas, IupSeparator(), item_exit, NULL);
    format_menu = IupMenu(item_font, NULL);
    help_menu = IupMenu(item_about, NULL);

    sub_menu_file = IupSubmenu("File", file_menu);
    sub_menu_format = IupSubmenu("Format", format_menu);
    sub_menu_help = IupSubmenu("Help", help_menu);

    menu = IupMenu(sub_menu_file, sub_menu_format, sub_menu_help, NULL);

    loadBtn = IupButton("Load Chirp CSV", NULL);
    IupSetCallback(loadBtn, "ACTION", (Icallback)btn);
    IupSetAttribute(loadBtn, "EXPAND", "HORIZONTAL");
    IupSetAttribute(loadBtn, "IMAGE", "IUP_FileOpen");

    saveBtn = IupButton("Save as .chan (for Quansheng Dock)", NULL);
    IupSetCallback(saveBtn, "ACTION", (Icallback)saveas_cb);
    IupSetAttribute(saveBtn, "EXPAND", "HORIZONTAL");
    IupSetAttribute(saveBtn, "IMAGE", "IUP_FileSave");

    /*Ihandle *soonBtn = IupButton("Save as .xlsx (for Losehu)", NULL);
    IupSetAttribute(soonBtn, "EXPAND", "HORIZONTAL");
    IupSetAttribute(soonBtn, "ACTIVE", "NO");
    IupSetAttribute(soonBtn, "TIP", "Comming in version: 0.1.0");
    IupSetAttribute(soonBtn, "TIPBALLOON", "YES");
    IupSetAttribute(soonBtn, "TIPBALLOONTITLE", "Not yet implemented");
    IupSetAttribute(soonBtn, "TIPBALLOONTITLEICON", "1");

    vboxSave = IupVbox(saveBtn, soonBtn, NULL);
    IupSetAttribute(vboxSave, "MARGIN", "0x0");*/
    hbox = IupHbox(loadBtn, saveBtn, NULL);
    IupSetAttribute(hbox, "MARGIN", "0x10");
    IupSetAttribute(hbox, "GAP", "10");

    progressBar = IupProgressBar();
    IupSetAttribute(progressBar, "SIZE", "x5");
    IupSetAttribute(progressBar, "EXPAND", "HORIZONTAL");
    IupSetAttribute(progressBar, "DASHED", "YES");
    IupSetAttribute(progressBar, "MAX", "200");
    IupSetAttribute(progressBar, "VISIBLE", "NO");

    Ihandle  *copySelectedMsg = IupItem("Copy selected message", NULL);
    Ihandle *copyAllMsg = IupItem("Copy all messages", NULL);
    Ihandle *contextSeparator = IupSeparator();
    Ihandle *clearAll = IupItem("Clear all", NULL);

    IupSetCallback(copySelectedMsg, "ACTION", (Icallback)copy_selected_cb);
    IupSetCallback(copyAllMsg, "ACTION", (Icallback)copy_all_cb);
    IupSetCallback(clearAll, "ACTION", (Icallback)clear_all_cb);

    IupSetAttribute(copySelectedMsg, "IMAGE", "IUP_EditCopy");
    IupSetAttribute(copyAllMsg, "IMAGE", "IUP_ToolsSortAscend");
    IupSetAttribute(clearAll, "IMAGE", "IUP_EditErase");

    Ihandle *contextMenu = IupMenu(copySelectedMsg, copyAllMsg, contextSeparator, clearAll, NULL);

    list = IupList(NULL);
    IupSetAttribute(list, "EXPAND", "YES");
    IupSetAttribute(list, "SHOWIMAGE", "YES");
    IupSetCallback(list, "BUTTON_CB", (Icallback)(void*)list_button_cb);
    IupSetAttributeHandle(list, "CONTEXT_MENU", contextMenu);
    IupSetAttributeHandle(copySelectedMsg, "TARGET_LIST", list);
    IupSetAttributeHandle(copyAllMsg, "TARGET_LIST", list);
    IupSetAttributeHandle(clearAll, "TARGET_LIST", list);
    printInfoToList("Drag or select a CSV file.");

    Ihandle* scroll_box = IupScrollBox(list);
    vbox = IupVbox(progressBar, scroll_box, hbox, NULL);
    IupSetAttribute(vbox, "MARGIN", "10x0");

    dlg = IupDialog(vbox);
    IupSetAttributeHandle(dlg, "MENU", menu);
    IupSetAttribute(dlg, "TITLE", "Quansheng channel tool");
    IupSetAttribute(dlg, "SIZE", "QUARTERxQUARTER");
    IupSetAttribute(dlg, "ICON", "#1");
    IupSetCallback(dlg, "DROPFILES_CB", (Icallback)(void*)dropfiles_cb);

    IupShowXY(dlg, IUP_CENTER, IUP_CENTER);
    IupSetAttribute(dlg, "USERSIZE", NULL);

    IupMainLoop();

    IupClose();
    return EXIT_SUCCESS;
}
