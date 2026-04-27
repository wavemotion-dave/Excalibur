// ========================================================================================
// Excalibur RPN Calculator is Copyright(c) 1994-2026 by Dave Bernazzani(wavemotion-dave)
//
// This is legacy code that was created to run under Visual C++ 4.5 and 5.0 circa 1995 and
// was largely developed with Windows 95 through Windows 98SE(some very early portions
// of code were started during the Windows 3.1 era but were quickly ported for 32-bit).
//
// I don't think there is any proprietary code here... and as such I release all of this
// source code into the wild using the permissive MIT license as follows:
//
// Copyright(c) 1994-2026 Dave Bernazzani
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this 
// software and associated documentation files(the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be included in all copies
// or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE 
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.
// ========================================================================================

typedef __int64             int64_t;
typedef __int32             int32_t;
typedef __int16             int16_t;
typedef __int8              int8_t;

typedef unsigned __int64    uint64_t;
typedef unsigned __int32    uint32_t;
typedef unsigned __int16    uint16_t;
typedef unsigned __int8     uint8_t;

#define IDC_PUSHBUTTON1     105
#define IDC_PUSHBUTTON2     106
#define IDC_PUSHBUTTON3     107
#define IDC_PUSHBUTTON4     108
#define IDC_PUSHBUTTON12    116
#define IDC_PUSHBUTTON28    132
#define IDC_PUSHBUTTON36    140
#define IDC_PUSHBUTTON76    180
#define IDC_CUSTOM_OK       100
#define IDC_CUSTOM_CANCEL   182
#define IDC_PUSHBUTTON75    179
#define IDC_PUSHBUTTON74    178
#define IDC_PUSHBUTTON73    177
#define IDC_PUSHBUTTON72    176
#define IDC_PUSHBUTTON71    175
#define IDC_PUSHBUTTON70    174
#define IDC_PUSHBUTTON69    173
#define IDC_PUSHBUTTON68    172
#define IDC_PUSHBUTTON67    171
#define IDC_PUSHBUTTON66    170
#define IDC_PUSHBUTTON65    169
#define IDC_PUSHBUTTON64    168
#define IDC_PUSHBUTTON63    167
#define IDC_PUSHBUTTON62    166
#define IDC_PUSHBUTTON61    165
#define IDC_PUSHBUTTON60    164
#define IDC_PUSHBUTTON59    163
#define IDC_PUSHBUTTON58    162
#define IDC_PUSHBUTTON57    161
#define IDC_PUSHBUTTON56    160
#define IDC_PUSHBUTTON55    159
#define IDC_PUSHBUTTON54    158
#define IDC_PUSHBUTTON53    157
#define IDC_PUSHBUTTON52    156
#define IDC_PUSHBUTTON51    155
#define IDC_PUSHBUTTON50    154
#define IDC_PUSHBUTTON49    153
#define IDC_PUSHBUTTON48    152
#define IDC_PUSHBUTTON47    151
#define IDC_PUSHBUTTON46    150
#define IDC_PUSHBUTTON45    149
#define IDC_PUSHBUTTON44    148
#define IDC_PUSHBUTTON43    147
#define IDC_PUSHBUTTON42    146
#define IDC_PUSHBUTTON41    145
#define IDC_PUSHBUTTON40    144
#define IDC_PUSHBUTTON39    143
#define IDC_PUSHBUTTON38    142
#define IDC_PUSHBUTTON37    141
#define IDC_PUSHBUTTON35    139
#define IDC_PUSHBUTTON34    138
#define IDC_PUSHBUTTON33    137
#define IDC_PUSHBUTTON32    136
#define IDC_PUSHBUTTON31    135
#define IDC_PUSHBUTTON30    134
#define IDC_PUSHBUTTON29    133
#define IDC_PUSHBUTTON27    131
#define IDC_PUSHBUTTON26    130
#define IDC_PUSHBUTTON25    129
#define IDC_PUSHBUTTON24    128
#define IDC_PUSHBUTTON23    127
#define IDC_PUSHBUTTON22    126
#define IDC_PUSHBUTTON21    125
#define IDC_PUSHBUTTON20    124
#define IDC_PUSHBUTTON19    123
#define IDC_PUSHBUTTON18    122
#define IDC_PUSHBUTTON17    121
#define IDC_PUSHBUTTON16    120
#define IDC_PUSHBUTTON15    119
#define IDC_PUSHBUTTON14    118
#define IDC_PUSHBUTTON13    117
#define IDC_PUSHBUTTON11    115
#define IDC_PUSHBUTTON10    114
#define IDC_PUSHBUTTON9     113
#define IDC_PUSHBUTTON8     112
#define IDC_PUSHBUTTON7     111
#define IDC_PUSHBUTTON6     110
#define IDC_PUSHBUTTON5     109

#define MAX_FUNCS           40           // The 40th one is for help only

#define RPN_LAST_KEY        -1
    
#define MAX_DIGITS          18

#define X_NEW     01            /* Next keypress pushes stack and starts new edit */
#define X_EDIT    02            /* Value is under edit */
#define X_ENTER   03            /* Enter just pressed - next keypress replaces X */
#define X_NULL    04

#define MAX_IMPORT_CLIPBOARD_SIZE (1024 * 10)

// Defines for menu item IDs
#define   IDM_OPEN                            101
#define   IDM_SAVE                            102
#define   IDM_EXIT                            103
#define   IDM_COPYXTOCLIPBOARD                104
#define   IDM_COPYALLTOCLIPBOARD              105
#define   IDM_SETTINGS                        106
#define   IDM_DIRECTORIES                     107
#define   IDM_SCIENTIFIC                      108
#define   IDM_FINANCIAL                       109
#define   IDM_CONVERSION                      110
#define   IDM_COMPSCI                         111
#define   IDM_ALWAYSONTOP                     112
#define   IDM_EXCALIBURHELP                   113
#define   IDM_ABOUTPROGRAM                    114
#define   IDM_GEOMETRY                        115
#define   IDM_PROGII                          116
#define   IDM_COMPLEX                         117
#define   IDM_STATS                           118
#define   IDM_USAGESTATS                      119
#define   IDM_CUSTOM                          120
#define   IDM_DEFINECUSTOM                    121
#define   IDM_COPYCLIPBOARDTOX                122
#define   IDM_PHYSICS                         123
#define   IDM_PROGI                           124

// Function prototypes for dialog and subdialog modules

#define YES_L   ((char) 1)
#define NO_L    ((char) 2)

struct funcStruct
{
    int16_t index;
    int16_t uniqueIndex;
    uint8_t useFloatsLongs;
    uint8_t allowRecord;
    uint8_t op;
    char desc[9];
    uint8_t saveLastX;             /* Do we save LASTX here? */
    uint8_t newXedit;              /* The new Xedit */
    void(*routine) (void);
    int32_t keyTitle;
    int32_t keyHelp;
};

struct keypadStruct
{
    int16_t index;
    int16_t uniqueIndex;
    uint8_t useFloatsLongs;
    uint8_t allowRecord;
    uint8_t op;
    uint8_t saveLastX;             /* Do we save LASTX here? */
    uint8_t newXedit;              /* The new Xedit */
    void(*routine) (void);
    char keyTitle[26];
    char keyHelp[128];
};


// For record/playback
#define ALLOWREC     1
#define NORECORD     0

// Float or Long...
#define USES_F  1   // Uses floating point numbers
#define USES_L  2   // Uses long integer numbers
#define USES_FL 3   // Can use either floats or longs

#define NOTES_SIZE 2048

/* --------------------------- */
/* Defines for the main dialog */
/* --------------------------- */
#define FUNC_BAR_TEXT_SCI_I    "SCIENTIFIC"
#define FUNC_BAR_TEXT_CONV     "CONVERSION"
#define FUNC_BAR_TEXT_BUIS     "FINANCIAL"
#define FUNC_BAR_TEXT_COMPSCI  "COMP SCI"
#define FUNC_BAR_TEXT_GEOM     "GEOMETRY"
#define FUNC_BAR_TEXT_CUSTOM   "CUSTOM BANK"
#define FUNC_BAR_TEXT_PROG2    "PROGRAM II"
#define FUNC_BAR_TEXT_COMPLEX  "COMPLEX/VECT"
#define FUNC_BAR_TEXT_STATS    "STATISTICS"
#define FUNC_BAR_TEXT_PHYSICS  "PHYSICS"
#define FUNC_BAR_TEXT_PROG1    "PROGRAM I"


/* ---------- */
/* PROTOTYPES */
/* ---------- */
#ifdef CPPLUS
extern "C" void StackPush(double temp);
extern "C" double StackPop(void);
extern "C" void RPN_error(char *msg);
extern "C" void RPN_help(void);
extern "C" double FromRadians(double t);
extern "C" double ToRadians(double t);
#else
extern void StackPush(double temp);
extern double StackPop(void);
extern void RPN_error(char *msg);
extern void RPN_help(void);
extern double FromRadians(double t);
extern double ToRadians(double t);
#endif

#define PROG_LONG        uint32_t    /* We handle signed and other word sizes in ex_prog.c */
#define PROG_SIGNEDLONG  int32_t     /* Needed when we convert Signed to float... */

extern struct funcStruct Scientific_funcs[];
extern struct funcStruct Financial_funcs[];
extern struct funcStruct Conversion_funcs[];
extern struct funcStruct CompSci_funcs[];
extern struct funcStruct Geometry_funcs[];
extern struct funcStruct Custom_funcs[];
extern struct funcStruct Program1_funcs[];
extern struct funcStruct Program2_funcs[];
extern struct funcStruct Complex_funcs[];
extern struct funcStruct Statistics_funcs[];
extern struct funcStruct Physics_funcs[];
extern struct funcStruct Complex_funcs[];
extern struct funcStruct MacroFuncs[];

extern uint8_t Xedit;
extern char Xstr[50];
extern double X;                /* Main register X */
extern double Y;                /* Main register Y */
extern double Z;                /* Main register Z */
extern double T;                /* Main Register T */
extern double A;                /* Extended Stack A */
extern double B;                /* Extended Stack B */
extern double C;                /* Extended Stack C */
extern double D;                /* Extended Stack D */

extern double LASTX;            /* LAST X register */

extern PROG_LONG XL;
extern PROG_LONG YL;
extern PROG_LONG ZL;
extern PROG_LONG TL;
extern PROG_LONG AL;
extern PROG_LONG BL;
extern PROG_LONG CL;
extern PROG_LONG DL;
extern PROG_LONG LASTXL;

extern uint32_t indirectRegister;

extern double taxConstant;
extern uint8_t commaMode;
extern uint8_t eexMode;
extern uint8_t numLockMode;

extern HINSTANCE hExcaliburInstance;
extern HWND hWndSuperMain;
extern HWND calcMainWindow;

extern HFONT holdsfont;
extern HFONT hMainFont;
extern HFONT hNumberFont;
extern HFONT hFixedFont;

extern uint8_t progMode;
#define PROG_NORMAL     0       // For normal floating-point handling
#define PROG_DEC        10      // Comp-Sci DEC mode
#define PROG_HEX        16      // Comp-Sci HEX mode
#define PROG_BIN        2       // Comp-Sci BIN mode
#define PROG_OCT        8       // Comp-Sci OCT mode

#define SUM_N           0       // Financial Register - N
#define SUM_X           1       // Financial Register - Sum X
#define SUM_Y           2       // Financial Register - Sum Y
#define SUM_XSQ         3       // Financial Register - Sum X Squared
#define SUM_YSQ         4       // Financial Register - Sum Y Squared
#define SUM_XY          5       // Financial Register - Sum XY
#define SUM_MAX         6       // Last Financial Register
extern double SUM[SUM_MAX];

#define MAX_CF          100
extern double cashFlow[MAX_CF];
extern uint8_t CFn;
extern uint8_t hyperbolic;
extern uint8_t binMode;

#define FIN_REG_MAX     9
extern double fin_reg[FIN_REG_MAX];
extern uint8_t payMode;
extern uint8_t dateMode;

/* Programming mode defines */
extern uint8_t padZeros;
extern uint32_t wordSize;
extern uint8_t wordMode;
extern uint32_t wordSizeMask;

#define PROG_UNSIGNED   1
#define PROG_SIGNED     0
#define PROG_PADZEROS   1
#define PROG_NOPADZEROS 0

extern uint8_t numberDisplayMode;

extern int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow);
extern void SelectNewFunc(struct funcStruct *funcs);
extern BOOL FAR PASCAL MainWndProcSuper(HWND hWnd, WORD wMessage, WORD wParam, LONG lParam);
extern int ClipboardCopySelection(HWND hWnd, int copytype);
extern void SetUpFonts(HWND hWnd);
extern BOOL CALLBACK MainWndProc(HWND hWnd, UINT wMessage, WPARAM wParam, LPARAM lParam);
extern void NotImp(void);
extern void FloatsToLongs(void);
extern void LongsToFloats(void);
extern int Init(void);
extern int ShowStatus(void);
extern int UpdateSpareBar(char *msg);
extern int UpdateTimeBar(void);
extern int ShowFunctionBar(char *msg);
extern double MakeAccurate(double val);
extern int selectFuncs(WPARAM key);
extern int processFuncs(void);
extern int ProcessDirectKeyHit(WPARAM key);
extern int ProcessHelp(WPARAM key);
extern int ProcessKeyHit(WPARAM key);
extern char Radix(int progM);
extern void PutCommas(char *str);
extern void MakeEngineeringFormat(double val, char *Fstr);
extern void MakeSciFormat(double val, char *Fstr);
extern void ShowStack(void);
extern void StackPush(double temp);
extern double StackPop(void);
extern void PushConstant(double value);
extern void SaveProgramStep(int uniqueIndex);
extern void ExcaliburHelp(void);

extern void MakeRadixStr(PROG_LONG val, char *tmpL);
extern void StackPushL(PROG_LONG temp);
extern PROG_LONG StackPopL(void);
extern PROG_LONG MakeProgStr(char *str);
extern PROG_LONG maskStackStuff(PROG_LONG lng);

extern void RPN_clear(void);
extern void RPN_clearL(void);
extern void RPN_enter(void);
extern void RPN_dp(void);
extern void RPN_Ex(void);
extern void RPN_digit(WPARAM key);
extern void RPN_mode(void);
extern void RPN_backspace(void);
extern void RPN_plus(void);
extern void RPN_multiply(void);
extern void RPN_divide(void);
extern void RPN_minus(void);
extern void RPN_exchange_x_y(void);
extern void RPN_negate_x(void);
extern void RPN_rotateStackUp(void);
extern void RPN_rotateStackDn(void);
extern void RPN_lastX(void);
extern void RPN_store(void);
extern void RPN_recall(void);
extern int GetMenuType(struct funcStruct *cFunc);
extern void SetMenuType(int type);
extern void SaveToDisk(void);
extern void ReadFromDisk(void);
extern void GetUserSettings(void);
extern void RPN_SelectSci(void);
extern void RPN_SelectStat(void);
extern void RPN_SelectBusi(void);
extern void RPN_SelectConv(void);
extern void RPN_SelectGeom(void);
extern void RPN_SelectProg(void);
extern void RPN_SelectProg2(void);
extern void RPN_SelectCust(void);
extern void RPN_SelectComplex(void);
extern void RPN_SelectPhysics(void);
extern void RPN_SelectMacro(void);
extern void RPN_Playback(void);
extern void RPN_Record(void);
extern void RPN_dp(void);
extern void RPN_clear(void);
extern void RPN_divide(void);
extern void RPN_multiply(void);
extern void RPN_minus(void);
extern void RPN_plus(void);
extern void RPN_enter(void);
extern void RPN_exchange_x_y(void);
extern void RPN_negate_x(void);
extern void RPN_rotateStackUp(void);
extern void RPN_rotateStackDn(void);
extern void RPN_lastX(void);
extern void RPN_mode(void);
extern void RPN_backspace(void);
extern void RPN_store(void);
extern void RPN_recall(void);
extern void RPN_Ex(void);
extern void RPN_Playback(void);
extern void RPN_clearX(void);
extern void RPN_drop(void);
extern void RPN_larg(void);
extern void RPN_frac(void);
extern void RPN_edit(void);
extern void RPN_Notes(void);
extern void RPN_CopyX(void);
extern void RPN_inverse(void);

#define CUSTOM_SAVE_MAC       99        // macros
#define CUSTOM_SAVE_SCI       1
#define CUSTOM_SAVE_BUS       2
#define CUSTOM_SAVE_PRO       3
#define CUSTOM_SAVE_STA       4
#define CUSTOM_SAVE_GEO       5
#define CUSTOM_SAVE_CON       6
#define CUSTOM_SAVE_COM       7
#define CUSTOM_SAVE_PHY       8
#define CUSTOM_SAVE_MACBANK   9
#define CUSTOM_SAVE_MACBANK2  10

struct customSaveStruct
{
    int16_t custom_save_idx;        // CUSTOM_SAVE_xxx defines above
    int16_t func_idx;               // 0 - 39 inclusive
};
extern struct customSaveStruct customSave[MAX_FUNCS];

extern uint32_t stackPushes;
extern uint32_t stackPops;

#define INTERNATIONAL     0
#define NONINTERNATIONAL  1

#define M_PI             3.14159265359
#define CNULL           '\0'

extern int allowDigitBasedOnMaxStringSize(char *Xstr, char digit);
extern void makeInternational(char *str);
extern PROG_LONG biggestProgVal(void);
extern PROG_LONG smallestProgVal(void);
extern void turnOnNumLock(void);

#define MAX_FUNCTIONS    650
#define MAX_REC_PLAYBACK 256
#define MAX_MACROS       64
struct playbackStruct
{
    int16_t uniqueIndex;
    uint8_t useFloatsLongs;
    uint8_t allowRecord;
    uint8_t saveLastX;             /* Do we save LASTX here? */
    uint8_t newXedit;              /* The new Xedit */
    void(*routine) (void);
    char funcText[30];
};
extern struct playbackStruct playBackMap[MAX_FUNCTIONS + 1];

extern char macroName[MAX_MACROS][51];
extern char macro_short_names[MAX_MACROS][7];

extern int16_t playBackSave[MAX_MACROS][MAX_REC_PLAYBACK + 1];
extern int16_t playBackIdxSave[MAX_MACROS];
extern int16_t playBack[MAX_REC_PLAYBACK + 1];
extern int16_t playBackIdx;
extern uint8_t progModecarry;
extern uint8_t recModeON;

#define MAX_CONST_BANKS     5
#define MAX_CONSTS          64
struct constTableStruct
{
    uint8_t includeInList;
    char    name[26];
    char    units[11];
    double  value;
};

extern struct constTableStruct constants[MAX_CONST_BANKS][MAX_CONSTS];

#define MAX_CURRENCY_CONV 50
struct CurrencyStruct
{
    char Country[20];
    double conv;
};

extern struct CurrencyStruct CurrencyConv[MAX_CURRENCY_CONV];

extern int32_t currency1index;
extern int32_t currency2index;

extern uint8_t extendedStack;
extern uint8_t popFillZero;
extern int32_t lastChosenConst;
extern int32_t lastConstBank;
extern char constantBankNames[MAX_CONST_BANKS][15];

extern uint8_t finStore;
extern uint8_t finRecall;
extern uint8_t convInverse;
extern uint8_t depreciationType;

#define MAX_STO 26
extern double STO[MAX_STO];
extern char STOlabels[MAX_STO][9];

extern uint8_t macroPlayback;
extern int16_t currentMacroPlaybackIdx;

#define MAX_MACRO_STACK 1000
extern short int MacroStack[MAX_MACRO_STACK];
extern short int MacroStackIdx;
extern unsigned int macroFlags;
extern uint8_t traceMacroPlayback;
extern uint16_t progDelayValue;

extern void blinkXDisplay(void);
extern void endRunningMacro(void);
extern void UpdateDebugRegs(void);
extern void UpdateDebugProgram(int resetProgramList);
extern void RPN_Playback(void);

extern LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
extern LRESULT CALLBACK helpWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
extern int CreateToolTipWindow(HWND hwnd, HINSTANCE hInstance);
extern int CreateDebugWindow(HWND hwnd, HINSTANCE hInstance);
extern void RPN_SingleStep(void);
extern HWND debugTraceWindow;
int PreInit(void);
void RPN_storeReg(int reg);
void RPN_storeAddReg(int reg);
void RPN_storeSubReg(int reg);
void RPN_storeMulReg(int reg);
void RPN_storeDivReg(int reg);
void RPN_recallReg(int reg);

void RPN_storeA(void);
void RPN_storeB(void);
void RPN_storeC(void);
void RPN_storeD(void);
void RPN_storeE(void);
void RPN_storeF(void);
void RPN_storeG(void);
void RPN_storeH(void);
void RPN_storeI(void);
void RPN_storeJ(void);
void RPN_storeK(void);
void RPN_storeL(void);
void RPN_storeM(void);
void RPN_storeN(void);
void RPN_storeO(void);
void RPN_storeP(void);
void RPN_storeQ(void);
void RPN_storeR(void);
void RPN_storeS(void);
void RPN_storeT(void);
void RPN_storeU(void);
void RPN_storeV(void);
void RPN_storeW(void);
void RPN_storeX(void);
void RPN_storeY(void);
void RPN_storeZ(void);

void RPN_storeAddA(void);
void RPN_storeAddB(void);
void RPN_storeAddC(void);
void RPN_storeAddD(void);
void RPN_storeAddE(void);
void RPN_storeAddF(void);
void RPN_storeAddG(void);
void RPN_storeAddH(void);
void RPN_storeAddI(void);
void RPN_storeAddJ(void);
void RPN_storeAddK(void);
void RPN_storeAddL(void);
void RPN_storeAddM(void);
void RPN_storeAddN(void);
void RPN_storeAddO(void);
void RPN_storeAddP(void);
void RPN_storeAddQ(void);
void RPN_storeAddR(void);
void RPN_storeAddS(void);
void RPN_storeAddT(void);
void RPN_storeAddU(void);
void RPN_storeAddV(void);
void RPN_storeAddW(void);
void RPN_storeAddX(void);
void RPN_storeAddY(void);
void RPN_storeAddZ(void);

void RPN_storeSubA(void);
void RPN_storeSubB(void);
void RPN_storeSubC(void);
void RPN_storeSubD(void);
void RPN_storeSubE(void);
void RPN_storeSubF(void);
void RPN_storeSubG(void);
void RPN_storeSubH(void);
void RPN_storeSubI(void);
void RPN_storeSubJ(void);
void RPN_storeSubK(void);
void RPN_storeSubL(void);
void RPN_storeSubM(void);
void RPN_storeSubN(void);
void RPN_storeSubO(void);
void RPN_storeSubP(void);
void RPN_storeSubQ(void);
void RPN_storeSubR(void);
void RPN_storeSubS(void);
void RPN_storeSubT(void);
void RPN_storeSubU(void);
void RPN_storeSubV(void);
void RPN_storeSubW(void);
void RPN_storeSubX(void);
void RPN_storeSubY(void);
void RPN_storeSubZ(void);

void RPN_storeMulA(void);
void RPN_storeMulB(void);
void RPN_storeMulC(void);
void RPN_storeMulD(void);
void RPN_storeMulE(void);
void RPN_storeMulF(void);
void RPN_storeMulG(void);
void RPN_storeMulH(void);
void RPN_storeMulI(void);
void RPN_storeMulJ(void);
void RPN_storeMulK(void);
void RPN_storeMulL(void);
void RPN_storeMulM(void);
void RPN_storeMulN(void);
void RPN_storeMulO(void);
void RPN_storeMulP(void);
void RPN_storeMulQ(void);
void RPN_storeMulR(void);
void RPN_storeMulS(void);
void RPN_storeMulT(void);
void RPN_storeMulU(void);
void RPN_storeMulV(void);
void RPN_storeMulW(void);
void RPN_storeMulX(void);
void RPN_storeMulY(void);
void RPN_storeMulZ(void);

void RPN_storeDivA(void);
void RPN_storeDivB(void);
void RPN_storeDivC(void);
void RPN_storeDivD(void);
void RPN_storeDivE(void);
void RPN_storeDivF(void);
void RPN_storeDivG(void);
void RPN_storeDivH(void);
void RPN_storeDivI(void);
void RPN_storeDivJ(void);
void RPN_storeDivK(void);
void RPN_storeDivL(void);
void RPN_storeDivM(void);
void RPN_storeDivN(void);
void RPN_storeDivO(void);
void RPN_storeDivP(void);
void RPN_storeDivQ(void);
void RPN_storeDivR(void);
void RPN_storeDivS(void);
void RPN_storeDivT(void);
void RPN_storeDivU(void);
void RPN_storeDivV(void);
void RPN_storeDivW(void);
void RPN_storeDivX(void);
void RPN_storeDivY(void);
void RPN_storeDivZ(void);

void RPN_recallA(void);
void RPN_recallB(void);
void RPN_recallC(void);
void RPN_recallD(void);
void RPN_recallE(void);
void RPN_recallF(void);
void RPN_recallG(void);
void RPN_recallH(void);
void RPN_recallI(void);
void RPN_recallJ(void);
void RPN_recallK(void);
void RPN_recallL(void);
void RPN_recallM(void);
void RPN_recallN(void);
void RPN_recallO(void);
void RPN_recallP(void);
void RPN_recallQ(void);
void RPN_recallR(void);
void RPN_recallS(void);
void RPN_recallT(void);
void RPN_recallU(void);
void RPN_recallV(void);
void RPN_recallW(void);
void RPN_recallX(void);
void RPN_recallY(void);
void RPN_recallZ(void);

void RPN_recallA(void);
void RPN_recallB(void);
void RPN_recallC(void);
void RPN_recallD(void);
void RPN_recallE(void);
void RPN_recallF(void);
void RPN_recallG(void);
void RPN_recallH(void);
void RPN_recallI(void);
void RPN_recallJ(void);
void RPN_recallK(void);
void RPN_recallL(void);
void RPN_recallM(void);
void RPN_recallN(void);
void RPN_recallO(void);
void RPN_recallP(void);
void RPN_recallQ(void);
void RPN_recallR(void);
void RPN_recallS(void);
void RPN_recallT(void);
void RPN_recallU(void);
void RPN_recallV(void);
void RPN_recallW(void);
void RPN_recallX(void);
void RPN_recallY(void);
void RPN_recallZ(void);


// Add to the end of this list but *NEVER* remove entries or else you will need to update excalibur config file...
enum UniqueButtonIndexTag
{
    UNI_DIG0 = 100,
    UNI_DIG1,
    UNI_DIG2,
    UNI_DIG3,
    UNI_DIG4,
    UNI_DIG5,
    UNI_DIG6,
    UNI_DIG7,
    UNI_DIG8,
    UNI_DIG9,
    UNI_DIGDP,
    UNI_CLX,
    UNI_DIV,
    UNI_MUL,
    UNI_SUB,
    UNI_PLUS,
    UNI_ENT,
    UNI_XCH,
    UNI_CHS,
    UNI_E,
    UNI_STO,
    UNI_RCL,
    UNI_RUP,
    UNI_RDN,
    UNI_LSTX,
    UNI_MODE,
    UNI_BKSP,
    UNI_HELP,
    UNI_PLAY,
    UNI_CLRA,
    UNI_EDIT,

    UNI_ADDA,
    UNI_ADDB,
    UNI_ADDC,
    UNI_ADDD,
    UNI_ADDE,
    UNI_ADDF,
    UNI_ADDG,
    UNI_ADDH,
    UNI_ADDI,
    UNI_ADDJ,
    UNI_ADDK,
    UNI_ADDL,
    UNI_ADDM,
    UNI_ADDN,
    UNI_ADDO,
    UNI_ADDP,
    UNI_ADDQ,
    UNI_ADDR,
    UNI_ADDS,
    UNI_ADDT,
    UNI_ADDU,
    UNI_ADDV,
    UNI_ADDW,
    UNI_ADDX,
    UNI_ADDY,
    UNI_ADDZ,

    UNI_SUBA,
    UNI_SUBB,
    UNI_SUBC,
    UNI_SUBD,
    UNI_SUBE,
    UNI_SUBF,
    UNI_SUBG,
    UNI_SUBH,
    UNI_SUBI,
    UNI_SUBJ,
    UNI_SUBK,
    UNI_SUBL,
    UNI_SUBM,
    UNI_SUBN,
    UNI_SUBO,
    UNI_SUBP,
    UNI_SUBQ,
    UNI_SUBR,
    UNI_SUBS,
    UNI_SUBT,
    UNI_SUBU,
    UNI_SUBV,
    UNI_SUBW,
    UNI_SUBX,
    UNI_SUBY,
    UNI_SUBZ,

    UNI_MULA,
    UNI_MULB,
    UNI_MULC,
    UNI_MULD,
    UNI_MULE,
    UNI_MULF,
    UNI_MULG,
    UNI_MULH,
    UNI_MULI,
    UNI_MULJ,
    UNI_MULK,
    UNI_MULL,
    UNI_MULM,
    UNI_MULN,
    UNI_MULO,
    UNI_MULP,
    UNI_MULQ,
    UNI_MULR,
    UNI_MULS,
    UNI_MULT,
    UNI_MULU,
    UNI_MULV,
    UNI_MULW,
    UNI_MULX,
    UNI_MULY,
    UNI_MULZ,

    UNI_DIVA,
    UNI_DIVB,
    UNI_DIVC,
    UNI_DIVD,
    UNI_DIVE,
    UNI_DIVF,
    UNI_DIVG,
    UNI_DIVH,
    UNI_DIVI,
    UNI_DIVJ,
    UNI_DIVK,
    UNI_DIVL,
    UNI_DIVM,
    UNI_DIVN,
    UNI_DIVO,
    UNI_DIVP,
    UNI_DIVQ,
    UNI_DIVR,
    UNI_DIVS,
    UNI_DIVT,
    UNI_DIVU,
    UNI_DIVV,
    UNI_DIVW,
    UNI_DIVX,
    UNI_DIVY,
    UNI_DIVZ,

    UNI_STOA,
    UNI_STOB,
    UNI_STOC,
    UNI_STOD,
    UNI_STOE,
    UNI_STOF,
    UNI_STOG,
    UNI_STOH,
    UNI_STOI,
    UNI_STOJ,
    UNI_STOK,
    UNI_STOL,
    UNI_STOM,
    UNI_STON,
    UNI_STOO,
    UNI_STOP,
    UNI_STOQ,
    UNI_STOR,
    UNI_STOS,
    UNI_STOT,
    UNI_STOU,
    UNI_STOV,
    UNI_STOW,
    UNI_STOX,
    UNI_STOY,
    UNI_STOZ,

    UNI_RCLA,
    UNI_RCLB,
    UNI_RCLC,
    UNI_RCLD,
    UNI_RCLE,
    UNI_RCLF,
    UNI_RCLG,
    UNI_RCLH,
    UNI_RCLI,
    UNI_RCLJ,
    UNI_RCLK,
    UNI_RCLL,
    UNI_RCLM,
    UNI_RCLN,
    UNI_RCLO,
    UNI_RCLP,
    UNI_RCLQ,
    UNI_RCLR,
    UNI_RCLS,
    UNI_RCLT,
    UNI_RCLU,
    UNI_RCLV,
    UNI_RCLW,
    UNI_RCLX,
    UNI_RCLY,
    UNI_RCLZ,

    UNI_DROP,
    UNI_LARG,
    UNI_FRAC,
    UNI_REC,
    UNI_NOTES,
    UNI_CPYX,
    UNI_INVX,
    UNI_SCI,
    UNI_STAT,
    UNI_BUSI,
    UNI_CONV,
    UNI_GEOM,
    UNI_PROG1,
    UNI_PROG2,
    UNI_CMPLX,
    UNI_CUST,
    UNI_PHY,
    UNI_MAC,
    UNI_UNUSED,
    UNI_SIN,
    UNI_COS,
    UNI_TAN,
    UNI_HYP,
    UNI_ASIN,
    UNI_ACOS,
    UNI_ATAN,
    UNI_ABS,
    UNI_XX,
    UNI_SQRT,
    UNI_INV,
    UNI_POW10,
    UNI_EXP,
    UNI_LN,
    UNI_LOG,
    UNI_FACT,
    UNI_POW,
    UNI_INT,
    UNI_FRACT,
    UNI_RAND,
    UNI_COMB,
    UNI_PERM,
    UNI_PI,
    UNI_CONST,
    UNI_ROUND,
    UNI_FLOOR,
    UNI_CEIL,
    UNI_RNDXY,
    UNI_GCD,
    UNI_LCM,
    UNI_MINR,
    UNI_MAXR,
    UNI_DEC,
    UNI_HEX,
    UNI_BIN,
    UNI_OCT,
    UNI_HEXA,
    UNI_HEXB,
    UNI_HEXC,
    UNI_HEXD,
    UNI_HEXE,
    UNI_HEXF,
    UNI_BINLO,
    UNI_BINHI,
    UNI_AND,
    UNI_OR,
    UNI_NOT,
    UNI_XOR,
    UNI_NAND,
    UNI_NOR,
    UNI_2SCOMP,
    UNI_ASR,
    UNI_SHL,
    UNI_SHR,
    UNI_ROL,
    UNI_ROR,
    UNI_SHLX,
    UNI_SHRX,
    UNI_RLX,
    UNI_RRX,
    UNI_MOD,
    UNI_NUMB,
    UNI_SETB,
    UNI_CLRB,
    UNI_MASKL,
    UNI_MASKR,
    UNI_MINW,
    UNI_MAXW,
    UNI_ASCII,
    UNI_WSIZE,
    UNI_MIRROR,
    UNI_TAX,
    UNI_PERC,
    UNI_PERCC,
    UNI_PERCT,
    UNI_CLRF,
    UNI_STOFN,
    UNI_RCLFN,
    UNI_FINN,
    UNI_INTR,
    UNI_FINPV,
    UNI_FINPMT,
    UNI_FINFV,
    UNI_AMORT,
    UNI_CF0,
    UNI_CFJ,
    UNI_CFNJ,
    UNI_NPV,
    UNI_IIR,
    UNI_BOND,
    UNI_YTM,
    UNI_DATE,
    UNI_DAYS,
    UNI_TODAY,
    UNI_DEPR,
    UNI_DIV12,
    UNI_MUL12,
    UNI_DIV100,
    UNI_MUL100,
    UNI_MUC,
    UNI_MUP,
    UNI_COST,
    UNI_PRICE,
    UNI_SPPV,
    UNI_SPFV,
    UNI_USPV,
    UNI_USFV,
    UNI_FMODE,
    UNI_CURR,
    UNI_EFF,
    UNI_INFL,
    UNI_CINV,
    UNI_LBKG,
    UNI_OZGR,
    UNI_OZML,
    UNI_GALL,
    UNI_GALCM3,
    UNI_PINL,
    UNI_IGALL,
    UNI_FACE,
    UNI_FAKE,
    UNI_INCM,
    UNI_INMM,
    UNI_FTMI,
    UNI_FTKM,
    UNI_FTFA,
    UNI_FTM,
    UNI_MIKM,
    UNI_IN2CM2,
    UNI_FT2M2,
    UNI_IN3CM3,
    UNI_FT3M3,
    UNI_ACRFT2,
    UNI_ATPSI,
    UNI_HHMS,
    UNI_DRAD,
    UNI_RP,
    UNI_KMHMPS,
    UNI_KNOTMPH,
    UNI_AHCOL,
    UNI_AHFAR,
    UNI_HPKW,
    UNI_JCAL,
    UNI_MILMM,
    UNI_CIRCC,
    UNI_CIRCA,
    UNI_ELIPA,
    UNI_TRIA,
    UNI_RECTC,
    UNI_RECTA,
    UNI_BOXA,
    UNI_BOXV,
    UNI_SPHA,
    UNI_SPHV,
    UNI_CYLA,
    UNI_CYLV,
    UNI_CONEA,
    UNI_CONEB,
    UNI_PYRA,
    UNI_PYRV,
    UNI_QUAD,
    UNI_PYTH,
    UNI_HERO,
    UNI_INSCR,
    UNI_PRIME,
    UNI_RESIST,
    UNI_ELEMENT,
    UNI_METRIC,
    UNI_LOG2,
    UNI_TSTART,
    UNI_TSTOP,
    UNI_TCLR,
    UNI_ASTRO,
    UNI_IEEE,
    UNI_STOXTH,
    UNI_RCLXTH,
    UNI_CPLUS,
    UNI_CMINUS,
    UNI_CDIV,
    UNI_CMUL,
    UNI_CSIN,
    UNI_CCOS,
    UNI_CTAN,
    UNI_CCHS,
    UNI_CCONJ,
    UNI_CEXP,
    UNI_CLN,
    UNI_CLOG,
    UNI_CPOW,
    UNI_CSQRT,
    UNI_CABS,
    UNI_VPLUS,
    UNI_VMINUS,
    UNI_VCROSS,
    UNI_VDOT,
    UNI_VANGLE,
    UNI_VCONVR,
    UNI_VCONVP,
    UNI_SUMPLUS,
    UNI_SUMMINUS,
    UNI_SUMCLR,
    UNI_STATN,
    UNI_STATSDX,
    UNI_STATPDX,
    UNI_STATSDY,
    UNI_STATPDY,
    UNI_STATAVEX,
    UNI_STATAVEY,
    UNI_SUMX,
    UNI_SUMY,
    UNI_SUMXX,
    UNI_SUMYY,
    UNI_SUMXY,
    UNI_CORR,
    UNI_YINT,
    UNI_SLOPE,
    UNI_YGUESS,
    UNI_XGUESS,
    UNI_CONST2,
    UNI_MOLES,
    UNI_MOLEC,
    UNI_GASVOL,
    UNI_OHME,
    UNI_OHMI,
    UNI_OHMR,
    UNI_GRAV,
    UNI_ENERGY,
    UNI_CENTF,
    UNI_CENTA,
    UNI_ACELDIST,
    UNI_PROJRANGE,
    UNI_PROJHEIGHT,
    UNI_LBLA,
    UNI_GOTOA,
    UNI_GSBA,
    UNI_XLEY,
    UNI_LBLB,
    UNI_GOTOB,
    UNI_GSBB,
    UNI_XGY,
    UNI_LBLC,
    UNI_GOTOC,
    UNI_GSBC,
    UNI_XEY,
    UNI_LBLD,
    UNI_GOTOD,
    UNI_GSBD,
    UNI_XNEY,
    UNI_LBLE,
    UNI_GOTOE,
    UNI_GSBE,
    UNI_XLEZ,
    UNI_LBLF,
    UNI_GOTOF,
    UNI_GSBF,
    UNI_XGZ,
    UNI_LBLG,
    UNI_GOTOG,
    UNI_GSBG,
    UNI_XEZ,
    UNI_INPA,
    UNI_INPB,
    UNI_RET,
    UNI_XNEZ,
    UNI_SFX,
    UNI_CFX,
    UNI_TFX,
    UNI_PAUSE,
    UNI_REV,
    UNI_DEL,
    UNI_FWD,
    UNI_MEDIT,
    UNI_LBLH,
    UNI_GOTOH,
    UNI_GSBH,
    UNI_HALT,
    UNI_LBLI,
    UNI_GOTOI,
    UNI_GSBI,
    UNI_LBLJ,
    UNI_GOTOJ,
    UNI_GSBJ,
    UNI_LBLK,
    UNI_GOTOK,
    UNI_GSBK,
    UNI_INPC,
    UNI_INPD,
    UNI_STOIND,
    UNI_GOTOIND,
    UNI_GOSUBIND,
    UNI_RCLIND,
    UNI_SLOOP,
    UNI_LOOP,
    UNI_DEBUG,
    UNI_TRACE,
    UNI_STEP,
    UNI_STRACE,
    UNI_LOOPA,
    UNI_LOOPB,
    UNI_LOOPC,
    UNI_LOOPD,
    UNI_LOOPE,
    UNI_LOOPF,
    UNI_LOOPG,
    UNI_LOOPH,
    UNI_LOOPI,
    UNI_LOOPJ,
    UNI_LOOPK,
    UNI_JUMP,
    UNI_DSZ,
    UNI_TPUSH,
    UNI_STONLB,
    UNI_LTONLB,
    UNI_MTONLB,
    UNI_CLRFL,
    UNI_STO2I,
    UNI_RCL2I,
    UNI_ENDCONST,
    UNI_STARTCONST,

    UNI_ADD_NEW_HERE
};

extern struct keypadStruct RPNkeys[];
extern struct keypadStruct STOkeys[];

extern uint8_t  userTimer;
extern uint32_t userTicks;

#define ID_CURRENCY_BOX1                101
#define TRACE_PROGRAM                   101
#define ID_CURRENCY_A_TO_B              102
#define ID_CURRENCY_CANCEL              103
#define ID_CURRENCY_REDEFINE            104
#define TRACE_REGS                      104
#define TRACE_REGS2                     104
#define ID_CURRENCY_BOX2                105
#define TRACE_REGS1                     105
#define ID_CURRENCY_B_TO_A              106

// RPN_DIGIT_0 to RPN_DIGIT_9 must be 101 for logic to work
#define RPN_START_OF_LIST               100
#define RPN_DIGIT_0                     101
#define RPN_DIGIT_1                     102
#define RPN_DIGIT_2                     103
#define RPN_DIGIT_3                     104
#define RPN_DIGIT_4                     105
#define RPN_DIGIT_5                     106
#define RPN_DIGIT_6                     107
#define RPN_DIGIT_7                     108
#define RPN_DIGIT_8                     109
#define RPN_DIGIT_9                     110
#define RPN_DIGIT_DP                    111
#define RPN_CLEAR_X                     112
#define RPN_DIVIDE                      113
#define RPN_MULTIPLY                    114
#define RPN_SUBTRACT                    115
#define RPN_PLUS                        116
#define RPN_ENTER                       117
#define RPN_EXCH_X_Y                    118
#define RPN_NEGATE                      119
#define RPN_E                           120
#define RPN_STO                         121
#define RPN_RCL                         122
#define RPN_R_UP                        123
#define RPN_R_DN                        124
#define RPN_LASTX                       125
#define RPN_MODE                        126
#define RPN_BKSP                        127
#define RPN_CLEAR_ALL                   128
#define RPN_HELP                        129
#define RPN_PLAYBACK                    130                
#define RPN_DROP                        131
#define RPN_LARG                        132
#define RPN_FRAC                        133
#define RPN_EDIT                        134
#define RPN_RCL1                        135
#define RPN_STO0                        136
#define RPN_RCL0                        137
#define RPN_STO1                        138
#define RPN_NOTES                       139
#define RPN_INV                         140
#define RPN_CARRY                       141
#define RPN_REC                         142

#define RPN_SCI                         200
#define RPN_STAT                        201
#define RPN_FIN                         202
#define RPN_CONV                        203
#define RPN_GEOM                        204
#define RPN_COMPSCI                     205
#define RPN_PHY                         206
#define RPN_CMPLX                       207
#define RPN_PROGI                       208
#define RPN_PROGII                      209
#define RPN_CUST                        210

#define STATUS_BAR                      300
#define FUNC_BAR                        301
#define TIME_BAR                        302
#define REC_BAR                         303
#define SPARE_BAR                       304
#define ANGLE_BAR                       305
#define PROG_BAR                        306

#define RPN_STACK                       400
#define RPN_X_LABEL                     401
#define RPN_Y_LABEL                     402
#define RPN_Z_LABEL                     403
#define RPN_T_LABEL                     404

#define RPN_END_OF_LIST                 999

// Offset these function IDs to keep them grouped easily
#define FN1                             1001
#define FN2                             1002
#define FN3                             1003
#define FN4                             1004
                        
#define FN5                             1005
#define FN6                             1006
#define FN7                             1007
#define FN8                             1008
                        
#define FN9                             1009
#define FN10                            1010
#define FN11                            1011
#define FN12                            1012
                        
#define FN13                            1013
#define FN14                            1014
#define FN15                            1015
#define FN16                            1016
                        
#define FN17                            1017
#define FN18                            1018
#define FN19                            1019
#define FN20                            1020
                        
#define FN21                            1021
#define FN22                            1022
#define FN23                            1023
#define FN24                            1024
                        
#define FN25                            1025
#define FN26                            1026
#define FN27                            1027
#define FN28                            1028
                        
#define FN29                            1029
#define FN30                            1030
#define FN31                            1031
#define FN32                            1032
                        
#define FN33                            1033
#define FN34                            1034
#define FN35                            1035
#define FN36                            1036
                        
#define FN37                            1037
#define FN38                            1038
#define FN39                            1039
#define FN40                            1040

// These are pushed way up in index values as they are special
#define RPN_STACK_T                     2003
#define RPN_STACK_Z                     2002
#define RPN_STACK_Y                     2001
#define RPN_STACK_X                     2000


#include "resource.h"
