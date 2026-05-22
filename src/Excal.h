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

#define PROG_LONG           uint64_t    // We handle signed and other word sizes in ex_prog.c
#define PROG_SIGNEDLONG     int64_t     // Needed when we convert Signed to float...

#define MAX_FUNCS           40

#define RPN_LAST_KEY        -1

#define MAX_DIGITS          18

#define X_NEW               1   // Next keypress pushes stack and starts new edit
#define X_EDIT              2   // Value is under edit and will have the _ at the end
#define X_ENTER             3   // Enter just pressed - next keypress replaces X
#define X_NULL              4   // No change to the X value - it will remain one of those above

#define YES_L               1   // Yes, save the Last X value when executing this function
#define NO_L                2   // No, do not save the Last X value when executing this function

#define MAX_IMPORT_CLIPBOARD_SIZE       (32 * 1024)

// Defines for menu item IDs
#define   IDM_OPEN                          101
#define   IDM_SAVE                          102
#define   IDM_EXIT                          103
#define   IDM_COPYXTOCLIPBOARD              104
#define   IDM_COPYALLTOCLIPBOARD            105
#define   IDM_SETTINGS                      106
#define   IDM_ALWAYSONTOP                   108
#define   IDM_EXCALIBURHELP                 109
#define   IDM_ABOUTPROGRAM                  110
#define   IDM_SCIENTIFIC                    111
#define   IDM_SCIENTIFIC2                   112
#define   IDM_FINANCIAL                     113
#define   IDM_CONVERSION                    114
#define   IDM_COMPSCI                       115
#define   IDM_PROGI                         116
#define   IDM_PROGII                        117
#define   IDM_CUSTOM                        118
#define   IDM_DEFINECUSTOM                  119
#define   IDM_STATS                         120
#define   IDM_USAGESTATS                    121
#define   IDM_COPYCLIPBOARDTOX              122
#define   IDM_COPYMACROTOCLIPBOARD          123

#define   IDM_MACRO                         3000
#define   IDM_MACRO_LIST                    3001

typedef void (*func_t)(void);

struct funcStruct
{
    int16_t     controlID;          // This is the Windows Control ID (IDC) for this button. This is not unique as we map many functions to the same button.
    uint16_t    uniqueIndex;        // Unique index for this function across all banks (used for config file and playback recording)
    uint8_t     useFloatsLongs;     // 1 = uses floats, 2 = uses longs, 3 = can use either (for recording whether to record the float or long value)
    uint8_t     allowRecord;        // Whether this function should be allowed to be recorded in macros (PROGRAM steps) or not
    uint8_t     op;                 // shortcut key associated with this function for direct key entry (0 if no direct key entry)
    const char *desc;               // The button text to display
    uint8_t     saveLastX;          // Whether to save the last X value before executing this function (for recording in macros)
    uint8_t     newXedit;           // Whether this function starts a new edit of X (vs just pushing the stack or doing something else that doesn't start a new edit)
    func_t      routine;            // Pointer to the function that implements this function
    const char *keyTitle;           // The mouseover help title for this function (if NULL, no mouseover help will be shown)
    const char *keyHelp;            // The full help string associated with this key (if NULL, no help text is shown)
};

extern struct funcStruct Scientific_funcs[];
extern struct funcStruct Scientific2_funcs[];
extern struct funcStruct Financial_funcs[];
extern struct funcStruct Conversion_funcs[];
extern struct funcStruct CompSci_funcs[];
extern struct funcStruct Program1_funcs[];
extern struct funcStruct Program2_funcs[];
extern struct funcStruct Statistics_funcs[];
extern struct funcStruct Custom_funcs[];
extern struct funcStruct MacroFuncs[];

// For record/playback
#define ALLOWREC    1   // This button can be recorded
#define NORECORD    0   // This is a meta-button that can't be recorded (e.g. REC or RUN)

// Float or Long...
#define USES_F      1   // Uses floating point numbers
#define USES_L      2   // Uses long integer numbers
#define USES_FL     3   // Can use either floats or longs

#define NOTES_SIZE  8196

// ---------------------------
// Defines for the main dialog
// ---------------------------
#define FUNC_BAR_TEXT_SCI_I     "SCIENTIFIC I"
#define FUNC_BAR_TEXT_SCI2      "SCIENTIFIC II"
#define FUNC_BAR_TEXT_BUIS      "FINANCIAL"
#define FUNC_BAR_TEXT_STATS     "STATISTICS"
#define FUNC_BAR_TEXT_CONV      "CONVERSION"
#define FUNC_BAR_TEXT_COMPSCI   "COMP-SCI"
#define FUNC_BAR_TEXT_PROG1     "PROGRAM I"
#define FUNC_BAR_TEXT_PROG2     "PROGRAM II"
#define FUNC_BAR_TEXT_CUSTOM    "CUSTOM BANK"

// We have 3 global timers in use...
#define TIMER_ONE_MINUTE        1
#define TIMER_SLOW              2
#define TIMER_FAST              3

// ----------
// PROTOTYPES
// ----------
#ifdef CPPLUS
extern "C" void   StackPush(double temp);
extern "C" double StackPop(void);
extern "C" void   RPN_error(char *msg);
extern "C" void   RPN_fact(void);
extern "C" double FromRadians(double t);
extern "C" double ToRadians(double t);
#else
extern void   StackPush(double temp);
extern double StackPop(void);
extern void   RPN_error(char *msg);
extern void   RPN_fact(void);
extern double FromRadians(double t);
extern double ToRadians(double t);
#endif

extern uint64_t strtou64(const char *nptr, char **endptr, int base);
extern int64_t  strtoi64(const char *nptr, char **endptr, int base);
extern uint64_t rotl64(uint64_t value, int shift);
extern uint64_t rotr64(uint64_t value, int shift);

extern uint8_t Xedit;           // One of X_NEW, X_EDIT, X_NULL, etc.
extern char Xstr[64];           // Global buffer for X editing
extern double X;                // Main register X
extern double Y;                // Main register Y
extern double Z;                // Main register Z
extern double T;                // Main Register T
extern double A;                // Extended Stack A
extern double B;                // Extended Stack B
extern double C;                // Extended Stack C
extern double D;                // Extended Stack D

extern double LASTX;            // LAST X register
extern double lastFloat;        // Last floating value in X

extern PROG_LONG XL;            // Main X register in Comp-Sci mode
extern PROG_LONG YL;            // Main Y register in Comp-Sci mode
extern PROG_LONG ZL;            // Main Z register in Comp-Sci mode
extern PROG_LONG TL;            // Main T register in Comp-Sci mode
extern PROG_LONG AL;            // Extended A register in Comp-Sci mode
extern PROG_LONG BL;            // Extended B register in Comp-Sci mode
extern PROG_LONG CL;            // Extended C register in Comp-Sci mode
extern PROG_LONG DL;            // Extended D register in Comp-Sci mode
extern PROG_LONG LASTXL;        // Last X register in Comp-Sci mode

extern uint32_t indirectRegister;

extern int32_t main_x;
extern int32_t main_y;

extern double  taxConstant;
extern uint8_t commaMode;
extern uint8_t eexMode;
extern uint8_t numLockMode;
extern uint8_t lastProgMode;

extern HINSTANCE hExcaliburInstance;
extern HWND calcMainWindow;
extern HWND debugTraceWindow;

extern HFONT holdsfont;
extern HFONT hMainFont;
extern HFONT hNumberFont;
extern HFONT hFixedFont;

extern uint8_t progMode;
#define PROG_FLOAT      0       // For normal floating-point handling
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
extern uint8_t binMode;

#define FIN_REG_MAX     9
extern double fin_reg[FIN_REG_MAX];
extern uint8_t payMode;
extern uint8_t dateMode;

// Programming mode defines
extern uint8_t  padZeros;
extern uint32_t wordSize;
extern uint8_t  wordMode;
extern uint64_t wordSizeMask;
extern uint8_t  hexSpacing;

#define COMPSCI_SIGNED      0
#define COMPSCI_UNSIGNED    1

#define COMPSCI_NOPADZEROS  0
#define COMPSCI_PADZEROS    1

#define HEX_SPACE_NONE      0
#define HEX_SPACE_2         1
#define HEX_SPACE_4         2

extern uint8_t numberDisplayMode;

extern void SelectNewBank(struct funcStruct *funcs);
extern void ClipboardCopySelection(HWND hWnd, uint8_t copytype);
extern void SetUpFonts(HWND hWnd);
extern void NotImp(void);
extern void FloatsToLongs(void);
extern void LongsToFloats(void);
extern int Init(void);
extern void ShowStatus(void);
extern void UpdateSpareBar(char *msg);
extern void UpdateVersionBar(void);
extern void ShowFunctionBar(char *msg);
extern double MakeAccurate(double val);
extern int selectFuncs(WPARAM key);
extern int processBank(void);
extern int ProcessDirectKeyHit(WPARAM key);
extern int ProcessHelp(WPARAM key);
extern int ProcessKeyHit(WPARAM key);
extern char Radix(int progM);
extern void PutCommas(char *str);
extern void MakeEngineeringFormat(double val, char *Fstr);
extern void FormatNumberForStack(double val, char *Fstr);
extern void ShowStack(void);
extern void StackPush(double temp);
extern double StackPop(void);
extern void PushConstant(double value);
extern void SaveProgramStep(uint16_t uniqueIndex);
extern void ExcaliburHelp(void);
extern void MakeRadixStr(PROG_LONG val, char *tmpL);
extern void StackPushL(PROG_LONG temp);
extern PROG_LONG StackPopL(void);
extern PROG_LONG MakeProgStr(char *str);
extern PROG_LONG maskStackStuff(PROG_LONG lng);
extern void PROG_dec(void);
extern void PROG_hex(void);
extern void PROG_oct(void);
extern void PROG_bin(void);
extern void cust_define(void);
extern void RPN_clearStack(void);
extern void RPN_clearL(void);
extern void RPN_const(void);
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
extern void RPN_ExchangeReg(void);
extern int GetMenuType(struct funcStruct *cFunc);
extern void SetMenuType(int type);
extern void SaveToDisk(void);
extern void ReadFromDisk(void);
extern void GetUserSettings(void);
extern void RPN_SelectSci(void);
extern void RPN_SelectStat(void);
extern void RPN_SelectFin(void);
extern void RPN_SelectConv(void);
extern void RPN_SelectCompSci(void);
extern void RPN_SelectProgII(void);
extern void RPN_SelectCustom(void);
extern void RPN_SelectSci2(void);
extern void RPN_SelectProgI(void);
extern void RPN_Playback(void);
extern void RPN_Record(void);
extern void RPN_dp(void);
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
extern void RPN_clearX(void);
extern void RPN_drop(void);
extern void RPN_larg(void);
extern void RPN_frac(void);
extern void RPN_edit(void);
extern void RPN_Notes(void);
extern void RPN_Copy(void);
extern void RPN_Paste(void);
extern void RPN_inverse(void);
extern void RPN_pow(void);
extern void SCI_sqrt(void);
extern void SCI_ln(void);
extern void SCI_log(void);
extern void SCI_pow(void);

#define CUSTOM_SAVE_SCI       1
#define CUSTOM_SAVE_SCI2      2
#define CUSTOM_SAVE_FIN       3
#define CUSTOM_SAVE_STATS     4
#define CUSTOM_SAVE_CONV      5
#define CUSTOM_SAVE_COMPSCI   6
#define CUSTOM_SAVE_PROG1     7
#define CUSTOM_SAVE_PROG2     8
#define CUSTOM_SAVE_MAC       99        // macros

struct customSaveStruct
{
    int16_t custom_save_idx;        // CUSTOM_SAVE_xxx defines above
    int16_t func_idx;               // 0 - 39 inclusive
};
extern struct customSaveStruct customSave[MAX_FUNCS];

extern uint64_t stackPushes;
extern uint64_t stackPops;

#define INTERNATIONAL     0
#define NONINTERNATIONAL  1

#define M_PI              3.14159265358979
#define CNULL            '\0'

#define REG_STORE         0x01
#define REG_RECALL        0x02
#define REG_EXCHANGE      0x04
#define REG_DP            0x08
#define REG_PLUS          0x10
#define REG_MINUS         0x20
#define REG_MULTIPLY      0x40
#define REG_DIVIDE        0x80
extern uint8_t rpnStoreRecall;

#define MOD_HYPERBOLIC    0x01
#define MOD_INVERSE       0x02
extern uint8_t modifiers;

extern int allowDigitBasedOnMaxStringSize(char *Xstr, char digit);
extern void makeInternational(char *str);
extern PROG_LONG biggestProgVal(void);
extern PROG_LONG smallestProgVal(void);
extern void turnOnNumLock(void);

#define MAX_STACK_STRLEN    29

#define MAX_FUNCTIONS       400     // Total Excalibur function keys across all banks
#define MAX_REC_PLAYBACK    400     // Maximum number of program steps per program
#define MAX_MACROS          40      // Maximum number of total programs (each getting the max steps)
#define MAX_MACRO_FUNC_TEXT 30      // Maximum macro text that can be assigned to any key

struct playbackStruct
{
    uint16_t    uniqueIndex;
    uint8_t     useFloatsLongs;
    uint8_t     allowRecord;
    uint8_t     saveLastX;
    uint8_t     newXedit;
    func_t      routine;
    const char *funcText;
};

extern struct playbackStruct playBackMap[MAX_FUNCTIONS + 1];

extern char macroName[MAX_MACROS][MAX_MACRO_FUNC_TEXT];
extern char macro_short_names[MAX_MACROS][7];

#define COPY_X_TO_CLIPBOARD         0
#define COPY_ALL_TO_CLIPBOARD       1
#define COPY_X_FROM_CLIPBOARD       2
#define COPY_MACRO_TO_CLIPBOARD     3

extern int16_t playBackSave[MAX_MACROS][MAX_REC_PLAYBACK + 1];
extern int16_t playBackIdxSave[MAX_MACROS];
extern int16_t playBack[MAX_REC_PLAYBACK + 1];
extern int16_t playBackIdx;
extern uint8_t progModecarry;
extern uint8_t recModeON;
extern uint16_t lastUniqueIndex;

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

extern uint8_t depreciationType;

#define MAX_STO 100
extern double STO[MAX_STO];

extern uint8_t macroPlayback;
extern int16_t currentMacroPlaybackIdx;

#define MAX_MACRO_STACK 1000
extern short int MacroStack[MAX_MACRO_STACK];
extern short int MacroStackIdx;
extern unsigned int macroFlags;
extern uint8_t traceMacroPlayback;
extern uint16_t traceDelayValueMs;

extern void blinkXDisplay(uint8_t no_peek);
extern void blinkStack(uint8_t no_peek);
extern void endRunningMacro(void);
extern void UpdateDebugRegs(void);
extern void UpdateDebugProgram(int resetProgramList);
extern void RPN_Playback(void);
extern void sleep_and_peek(int timeMs);

extern LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
extern LRESULT CALLBACK tooltipWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
extern int CreateToolTipWindow(HWND hwnd, HINSTANCE hInstance);
extern int CreateDebugWindow(HWND hwnd, HINSTANCE hInstance);
extern void RPN_SingleStep(void);
extern int PreInit(void);
extern void trim(char *str);

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
    UNI_CLRSTK,
    UNI_HELP,
    UNI_PLAY,
    UNI_DROP,
    UNI_LARG,
    UNI_FRAC,
    UNI_EDIT,
    UNI_CONST,
    UNI_NOTES,
    UNI_INVX,
    UNI_REC,
    UNI_EXREG,
    UNI_COPY,
    UNI_PASTE,
    UNI_CPYX,
    UNI_UNUSED,

    UNI_SCI,
    UNI_SCI2,
    UNI_COMPSCI,
    UNI_FIN,
    UNI_CONV,
    UNI_STAT,
    UNI_PROG1,
    UNI_PROG2,
    UNI_CUST,

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
    UNI_POW,
    UNI_EXP,
    UNI_LN,
    UNI_LOG,
    UNI_FACT,
    UNI_POW10,
    UNI_INT,
    UNI_FRACT,
    UNI_RAND,
    UNI_COMB,
    UNI_PERM,
    UNI_PI,
    UNI_LOG2,
    UNI_ROUND,
    UNI_FLOOR,
    UNI_CEIL,
    UNI_RNDXY,
    UNI_GCD,
    UNI_LCM,
    UNI_MINR,
    UNI_MAXR,
    UNI_PRIME,
    UNI_METRIC,
    UNI_ELEMENT,
    UNI_ASTRO,
    UNI_TSTART,
    UNI_TSTOP,
    UNI_TCLR,
    UNI_TPUSH,

    UNI_MOLES,
    UNI_MOLEC,
    UNI_GASVOL,
    UNI_ENERGY,
    UNI_OHME,
    UNI_OHMI,
    UNI_OHMR,
    UNI_GRAV,
    UNI_CENTF,
    UNI_CENTA,
    UNI_ACELDIST,
    UNI_PROJRANGE,
    UNI_PROJHEIGHT,
    UNI_RESIST,
    UNI_QUAD,
    UNI_PYTH,
    UNI_CIRCA,
    UNI_GAMMA,
    UNI_TOPOLAR,
    UNI_FROMPOLAR,
    UNI_CSIN,
    UNI_CCOS,
    UNI_CTAN,
    UNI_CSINH,
    UNI_CCOSH,
    UNI_CTANH,
    UNI_CCHS,
    UNI_CCONJ,
    UNI_CEXP,
    UNI_CLN,
    UNI_CLOG,
    UNI_CPOW,
    UNI_CSQRT,
    UNI_CABS,
    UNI_CNORM,
    UNI_CARG,
    UNI_CPLUS,
    UNI_CMINUS,
    UNI_CDIV,
    UNI_CMUL,

    UNI_CLRF,
    UNI_FINFV,
    UNI_FINN,
    UNI_INTR,
    UNI_FINPV,
    UNI_FINPMT,
    UNI_MUL12,
    UNI_DIV12,
    UNI_MUL100,
    UNI_DIV100,
    UNI_TAX,
    UNI_PERC,
    UNI_PERCC,
    UNI_PERCT,
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
    UNI_STONLB,
    UNI_LTONLB,
    UNI_MTONLB,
    UNI_NEWLBF,
    UNI_HZTOAF,
    UNI_MPAPSI,

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
    UNI_IEEE,

    UNI_LBLA,
    UNI_GOTOA,
    UNI_GSBA,
    UNI_LOOPA,
    UNI_LBLB,
    UNI_GOTOB,
    UNI_GSBB,
    UNI_LOOPB,
    UNI_LBLC,
    UNI_GOTOC,
    UNI_GSBC,
    UNI_LOOPC,
    UNI_LBLD,
    UNI_GOTOD,
    UNI_GSBD,
    UNI_LOOPD,
    UNI_LBLE,
    UNI_GOTOE,
    UNI_GSBE,
    UNI_LOOPE,
    UNI_LBLF,
    UNI_GOTOF,
    UNI_GSBF,
    UNI_LOOPF,
    UNI_LBLG,
    UNI_GOTOG,
    UNI_GSBG,
    UNI_LOOPG,
    UNI_LBLH,
    UNI_GOTOH,
    UNI_GSBH,
    UNI_LOOPH,
    UNI_LBLI,
    UNI_GOTOI,
    UNI_GSBI,
    UNI_LOOPI,
    UNI_RET,
    UNI_HALT,
    UNI_PAUSE,
    UNI_DSZ,
    UNI_LBLJ,
    UNI_GOTOJ,
    UNI_GSBJ,
    UNI_LOOPJ,
    UNI_LBLK,
    UNI_GOTOK,
    UNI_GSBK,
    UNI_LOOPK,
    UNI_INPA,
    UNI_INPB,
    UNI_INPC,
    UNI_INPD,
    UNI_SFX,
    UNI_CFX,
    UNI_TFX,
    UNI_CLRFL,
    UNI_XLEY,
    UNI_XGY,
    UNI_XEY,
    UNI_XNEY,
    UNI_XLEZ,
    UNI_XGZ,
    UNI_XEZ,
    UNI_XNEZ,
    UNI_STOIND,
    UNI_RCLIND,
    UNI_STOXTH,
    UNI_RCLXTH,
    UNI_STO2I,
    UNI_RCL2I,
    UNI_GOTOIND,
    UNI_GOSUBIND,
    UNI_MEDIT,
    UNI_REV,
    UNI_DEL,
    UNI_FWD,
    UNI_TRACE,
    UNI_STEP,
    UNI_BEEP,
    UNI_DEBUG,

    UNI_ENDCONST,
    UNI_STARTCONST,

    UNI_ADD_NEW_HERE,
};

extern struct funcStruct RPNkeys[];

extern uint8_t  userTimer;
extern uint32_t userTicks;

#define ID_CURRENCY_BOX1                101
#define ID_CURRENCY_A_TO_B              102
#define ID_CURRENCY_CANCEL              103
#define ID_CURRENCY_REDEFINE            104
#define ID_CURRENCY_BOX2                105
#define ID_CURRENCY_B_TO_A              106

#define TRACE_PROGRAM                   101
#define TRACE_REGS1                     102
#define TRACE_REGS2                     103

// RPN_DIGIT_0 to RPN_DIGIT_9 must be 101 to 110 for logic to work
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
#define RPN_CLR_STACK                   128
#define RPN_FACT                        129
#define RPN_PLAYBACK                    130
#define RPN_DROP                        131
#define RPN_LARG                        132
#define RPN_FRAC                        133
#define RPN_EDIT                        134
#define RPN_POW                         135
#define RPN_NOTES                       136
#define RPN_INV                         137
#define RPN_REC                         139
#define RPN_EXREG                       140
#define RPN_COPY                        141
#define RPN_PASTE                       142
#define RPN_SQRT                        143
#define RPN_XY                          144
#define RPN_LN                          145
#define RPN_LOG                         146

#define RPN_SCI                         200
#define RPN_SCI2                        201
#define RPN_FIN                         202
#define RPN_STAT                        203
#define RPN_CONV                        204
#define RPN_COMPSCI                     205
#define RPN_PROGI                       206
#define RPN_PROGII                      207
#define RPN_CUST                        208

#define STATUS_BAR                      300
#define FUNC_BAR                        301
#define TIME_BAR                        302
#define REC_BAR                         303
#define SPARE_BAR                       304
#define ANGLE_BAR                       305
#define PROG_BAR                        306
#define RPN_CARRY                       307

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

// These are pushed way up in values as they are special
#define RPN_STACK_T                     2003
#define RPN_STACK_Z                     2002
#define RPN_STACK_Y                     2001
#define RPN_STACK_X                     2000

// For the Custom Button dialog...
#define IDC_CUSTOM_OK                   100
#define IDC_CUSTOM_SCI                  190
#define IDC_CUSTOM_SCI2                 191
#define IDC_CUSTOM_FIN                  192
#define IDC_CUSTOM_STAT                 193
#define IDC_CUSTOM_CONV                 194
#define IDC_CUSTOM_COMPSCI              195
#define IDC_CUSTOM_PROG1                196
#define IDC_CUSTOM_PROG2                197
#define IDC_CUSTOM_FUNCNAME             198

#define IDC_CUSTOM_CANCEL               299

#define IDC_CUSTOM_PB1                  101
#define IDC_CUSTOM_PB2                  102
#define IDC_CUSTOM_PB3                  103
#define IDC_CUSTOM_PB4                  104
#define IDC_CUSTOM_PB5                  105
#define IDC_CUSTOM_PB6                  106
#define IDC_CUSTOM_PB7                  107
#define IDC_CUSTOM_PB8                  108
#define IDC_CUSTOM_PB9                  109
#define IDC_CUSTOM_PB10                 110
#define IDC_CUSTOM_PB11                 111
#define IDC_CUSTOM_PB12                 112
#define IDC_CUSTOM_PB13                 113
#define IDC_CUSTOM_PB14                 114
#define IDC_CUSTOM_PB15                 115
#define IDC_CUSTOM_PB16                 116
#define IDC_CUSTOM_PB17                 117
#define IDC_CUSTOM_PB18                 118
#define IDC_CUSTOM_PB19                 119
#define IDC_CUSTOM_PB20                 120
#define IDC_CUSTOM_PB21                 121
#define IDC_CUSTOM_PB22                 122
#define IDC_CUSTOM_PB23                 123
#define IDC_CUSTOM_PB24                 124
#define IDC_CUSTOM_PB25                 125
#define IDC_CUSTOM_PB26                 126
#define IDC_CUSTOM_PB27                 127
#define IDC_CUSTOM_PB28                 128
#define IDC_CUSTOM_PB29                 129
#define IDC_CUSTOM_PB30                 130
#define IDC_CUSTOM_PB31                 131
#define IDC_CUSTOM_PB32                 132
#define IDC_CUSTOM_PB33                 133
#define IDC_CUSTOM_PB34                 134
#define IDC_CUSTOM_PB35                 135
#define IDC_CUSTOM_PB36                 136
#define IDC_CUSTOM_PB37                 137
#define IDC_CUSTOM_PB38                 138
#define IDC_CUSTOM_PB39                 139
#define IDC_CUSTOM_PB40                 140

#define IDC_CUSTOM_PB41                 141
#define IDC_CUSTOM_PB42                 142
#define IDC_CUSTOM_PB43                 143
#define IDC_CUSTOM_PB44                 144
#define IDC_CUSTOM_PB45                 145
#define IDC_CUSTOM_PB46                 146
#define IDC_CUSTOM_PB47                 147
#define IDC_CUSTOM_PB48                 148
#define IDC_CUSTOM_PB49                 149
#define IDC_CUSTOM_PB50                 150
#define IDC_CUSTOM_PB51                 151
#define IDC_CUSTOM_PB52                 152
#define IDC_CUSTOM_PB53                 153
#define IDC_CUSTOM_PB54                 154
#define IDC_CUSTOM_PB55                 155
#define IDC_CUSTOM_PB56                 156
#define IDC_CUSTOM_PB57                 157
#define IDC_CUSTOM_PB58                 158
#define IDC_CUSTOM_PB59                 159
#define IDC_CUSTOM_PB60                 160
#define IDC_CUSTOM_PB61                 161
#define IDC_CUSTOM_PB62                 162
#define IDC_CUSTOM_PB63                 163
#define IDC_CUSTOM_PB64                 164
#define IDC_CUSTOM_PB65                 165
#define IDC_CUSTOM_PB66                 166
#define IDC_CUSTOM_PB67                 167
#define IDC_CUSTOM_PB68                 168
#define IDC_CUSTOM_PB69                 169
#define IDC_CUSTOM_PB70                 170
#define IDC_CUSTOM_PB71                 171
#define IDC_CUSTOM_PB72                 172
#define IDC_CUSTOM_PB73                 173
#define IDC_CUSTOM_PB74                 174
#define IDC_CUSTOM_PB75                 175
#define IDC_CUSTOM_PB76                 176
#define IDC_CUSTOM_PB77                 177
#define IDC_CUSTOM_PB78                 178
#define IDC_CUSTOM_PB79                 179
#define IDC_CUSTOM_PB80                 180

#define IDC_RADIO0                      1000
#define IDC_RADIO1                      1001
#define IDC_RADIO2                      1002
#define IDC_RADIO3                      1003
#define IDC_RADIO4                      1004
#define IDC_RADIO5                      1005
#define IDC_RADIO6                      1006
#define IDC_RADIO7                      1007
#define IDC_RADIO8                      1008
#define IDC_RADIO9                      1009
#define IDC_RADIO10                     1010
#define IDC_RADIO11                     1011
#define IDC_RADIO12                     1012
#define IDC_RADIO13                     1013
#define IDC_DECIMAL_PLACES              1014


#define T_NULL          "Unused"
#define H_NULL          "Nothing implemented for this key"
#define T_SIN           "Sine"
#define H_SIN           "Computes the Sine of X. Set angle mode (deg, rad, gra) in File/Settings."
#define T_COS           "Cosine"
#define H_COS           "Computes the Cosine of X. Set angle mode (deg, rad, gra) in File/Settings."
#define T_TAN           "Tangent"
#define H_TAN           "Computes the Tangent of X. Set angle mode (deg, rad, gra) in File/Settings."
#define T_HYP           "Hyperbolic"
#define H_HYP           "Used in conjunction with SIN, COS and TAN, produces the Hyperbolic functions."
#define T_ASIN          "ArcSine"
#define H_ASIN          "Computes the arc sine of X. Set angle mode (deg, rad, gra) in File/Settings."
#define T_ACOS          "ArcCosine"
#define H_ACOS          "Computes the arc cosine of X. Set angle mode (deg, rad, gra) in File/Settings."
#define T_ATAN          "ArcTangent"
#define H_ATAN          "Computes the arc tangent of X. Set angle mode (deg, rad, gra) in File/Settings."
#define T_ABS           "Absolute Value"
#define H_ABS           "Computes the absolute value of X"
#define T_XX            "X Squared"
#define H_XX            "Multiples X by X and returns the value to X."
#define T_SQRT          "Square Root"
#define H_SQRT          "Computes the square root of X"
#define T_INV           "Inverse"
#define H_INV           "Computes the inverse of X"
#define T_POW10         "10 ^ X"
#define H_POW10         "Raises 10 to the X power"
#define T_EXP           "Natural Exponential"
#define H_EXP           "Returns e raised to the X power."
#define T_LN            "Natural Log"
#define H_LN            "Computes the natural log of X"
#define T_LOG           "Log Base 10"
#define H_LOG           "Computes the log base 10 of X"
#define T_FACT          "Factorial"
#define H_FACT          "Computes the factorial of X. Beware of overflow on large numbers."
#define T_POW           "Power"
#define H_POW           "Raise Y to the X power"
#define T_INT           "Integer"
#define H_INT           "Computes the integer portion of X./nFor example: INT(23.45) is 23."
#define T_FRAC          "Fraction"
#define H_FRAC          "Computes the fractional portion of X./nFor example: FRAC(23.45) is 0.45."
#define T_RAND          "Random Number"
#define H_RAND          "Produces a random number between 0.0 and 1.0 to 7 decimal places."
#define T_COMB          "Combinations"
#define H_COMB          "Y Combinations grouped X at a time."
#define T_PERM          "Permutations"
#define H_PERM          "Y Permutations grouped X at a time."
#define T_PI            "PI"
#define H_PI            "Physical constant 3.1415926535898"
#define T_SUMP          "Summation +"
#define H_SUMP          "Adds the X and Y values to the statistics and summation registers."
#define T_SUMM          "Summation -"
#define H_SUMM          "Subtracts the X and Y values from statistics registers."
#define T_SUMC          "Summation Clear"
#define H_SUMC          "Clears all statistic registers to zero values."
#define T_SUMN          "Number of Elements"
#define H_SUMN          "Returns the number of X and Y elements in the statistics registers."
#define T_SUMSDX        "Sample Standard Deviation"
#define H_SUMSDX        "Calculates the Sample Standard Deviation of X."
#define T_SUMPDX        "Pop Standard Deviation"
#define H_SUMPDX        "Calculates the Population Standard Deviation of X."
#define H_SUMSDY        "Calculates the Sample Standard Deviation of Y."
#define H_SUMPDY        "Calculates the Population Standard Deviation of Y."
#define T_MEANX         "Mean X"
#define H_MEANX         "Calculates the arithmetic average of X values."
#define T_MEANY         "Mean Y"
#define H_MEANY         "Calculates the arithmetic average of Y values."
#define T_SUMX          "Sum X"
#define H_SUMX          "Calculates the arithmetic sum of X values."
#define T_SUMY          "Sum Y"
#define H_SUMY          "Calculates the arithmetic sum of Y values."
#define T_SUMXX         "Sum X Squared"
#define H_SUMXX         "Calculates the arithmetic sum of X*X values."
#define T_SUMYY         "Sum Y Squared"
#define H_SUMYY         "Calculates the arithmetic sum of Y*Y values."
#define T_SUMXY         "Sum XY"
#define H_SUMXY         "Calculates the arithmetic sum of X*Y values."
#define T_CORR          "Correlation Coefficient"
#define H_CORR          "Calculates the Correlation Coefficient r."
#define T_YINT          "Y Intercept"
#define H_YINT          "Calculates the y-intercept based on statistical entries."
#define T_SLOPE         "Slope"
#define H_SLOPE         "Calculates the slope of a line based on statistical entries."
#define T_ESTX          "Estimate X"
#define H_ESTX          "Estimates the X value based on the displayed value and statistical data."
#define T_ESTY          "Estimate Y"
#define H_ESTY          "Estimates the Y value based on the displayed value and statistical data."
#define T_TAX           "Tax Constant"
#define H_TAX           "Multiplies the number in X by the tax constant (default 1.05)."
#define T_PERC          "Percent"
#define H_PERC          "Calculates X percent of Y."
#define T_PERCC         "Percent Change"
#define H_PERCC         "Calculates percent change from Y to X."
#define T_CLRF          "Clear Financial"
#define H_CLRF          "Clears all of the financial registers. This includes {n} {i} {PV} {PMT} {FV} {COST} {PRICE} {MARKUP%C} {COST%P} {CF0} {CFj}"
#define T_STOFN         "Store Financial"
#define H_STOFN         "Stores the current X number to one of the financial registers. After pressing this key, select the financial register to store the value (for example {n} or {PV}). You should see the display blink to let you know the value was stored."
#define T_RCLFN         "Recall Financial"
#define H_RCLFN         "Recalls a previously stored financial number to the X register. After pressing this key, select the financial register to retrieve the value from (for example {n} or {PV})."
#define T_FINN          "Number of Payments"
#define H_FINN          "Calculates the number of payments required based on entries in the financial registers."
#define T_INTR          "Interest %"
#define H_INTR          "Calculates the APR interest based on entries in the financial registers."
#define T_FINPV         "Present Value"
#define H_FINPV         "Computes the present value of money based on entries in the financial registers."
#define T_FINPMT        "Periodic Payment"
#define H_FINPMT        "Computes the periodic payment of a loan based on entries in the financial registers."
#define T_FINFV         "Future Value"
#define H_FINFV         "Computes the future value of money based on entries in the financial registers."
#define T_AMORT         "Loan Amortization"
#define H_AMORT         "Computes the loan amortization based on entries in the financial registers"
#define T_DIV12         "Divide X by 12"
#define H_DIV12         "Used to convert annual interest to monthly."
#define T_MUL12         "Multiply X by 12"
#define H_MUL12         "Used to convert number of payments from yearly to monthly."
#define T_DIV100        "Make Percentage"
#define H_DIV100        "Used to convert X to a true percentage by dividing by 100."
#define T_MUL100        "Multiply X by 100"
#define H_MUL100        "Used to convert X from a decimal percentage percentage by multiplying by 100."
#define H_LB_KG         "Pounds to Kilograms"
#define T_LB_KG         "Pounds to Kilograms"
#define T_ELEMENT       "Periodic Table"
#define H_ELEMENT       "Periodic table of the elements with atomic numbers and weights."
#define T_CPLXADD       "Complex Addition"
#define H_CPLXADD       "Adds complex pair X,Y with Z,T"
#define T_CPLXSUB       "Complex Subtraction"
#define H_CPLXSUB       "Subtracts complex pair X,Y from Z,T"
#define T_CPLXDIV       "Complex Division"
#define H_CPLXDIV       "Divides complex pair X,Y into Z,T"
#define T_CPLXMUL       "Complex Multiplication"
#define H_CPLXMUL       "Multiplies complex pair X,Y with Z,T"
#define T_PERCT         "Percent Total"
#define H_PERCT         "Computes the % portion that X is of Y"
#define T_DATE          "Date From Days"
#define H_DATE          "Computes the date the number of days in X from a given date in Y.  The result date is stored in X and the day of the week is stored in Y (1=MON..7=SUN)."
#define T_DAYS          "Days Between Dates"
#define H_DAYS          "Computes the number of days between dates from Y to X. Dates are given by the current date format."
#define T_FMODE         "Financial Settings"
#define H_FMODE         "Sets compounding at the beginning of period or end of period and sets date mode."
#define T_BOND          "Bond Valuation (30/360)"
#define H_BOND          "Calculates Bond price based on Annual Coupon rate in {PMT} and YTM in {i}. Uses a 30/360 day basis and semi-annual coupons only."
#define T_YTM           "Yield-To-Maturity (30/360)"
#define H_YTM           "Calculates Yield-To-Maturity based on an Annual Coupon rate in {PMT} and bond price quote in {PV}. Uses a 30/360 day basis and semi-annual coupons only."
#define T_TODAY         "Today's Date"
#define H_TODAY         "Today's date is inserted in currently selected date format. To select date format, use the fModes key."
#define T_DEPR          "Depreciation"
#define H_DEPR          "Depreciation. Original cost in {PV}, Salvage value in {FV}, life expectancy in {n} and declining balance factor in {i} if required. Use fModes to set depreciation type."
#define T_CF0           "Cash Flow 0"
#define H_CF0           "Used to enter the initial cash flow value."
#define T_CFJ           "Cash Flow j"
#define H_CFJ           "Used to enter subsequent cash flow values. Use CF0 to enter the initial cash flow value."
#define T_NPV           "Net Present Value"
#define H_NPV           "Computes the Net Present Value of a series of cash flows."
#define T_IRR           "Internal Rate of Return"
#define H_IRR           "Computes the Internal Rate of Return of a series of cash flows."
#define T_MUC           "Markup as a % Cost"
#define H_MUC           "Calculates markup as a percentage of cost. Must store {COST} and {PRICE} first."
#define T_MUP           "Markup as a % Price"
#define H_MUP           "Calculates markup a percentage of price. Must store {COST} and {PRICE} first."
#define T_COST          "Cost"
#define H_COST          "Calculates Cost based on {PRICE} and {MARKUP%C}. If {MARKUP%C} is 0.0 then it is calculated using {MARKUP%P} instead."
#define T_PRICE         "Price"
#define H_PRICE         "Calculates Price  based on {COST} and {MARKUP%C}. If {MARKUP%C} is 0.0 then it is calculated using {MARKUP%P} instead."
#define H_FA_CE         "Degrees Fahrenheit to Celsius"
#define T_FA_CE         "Degrees Fahrenheit to Celsius"
#define H_IN_CM         "Inches to Centimeters"
#define T_IN_CM         "Inches to Centimeters"
#define H_OZ_GR         "Ounces to Grams"
#define T_OZ_GR         "Ounces to Grams"
#define H_GA_LI         "US Gallons to Liters"
#define T_GA_LI         "US Gallons to Liters"
#define H_FT_MI         "Feet to Miles"
#define T_FT_MI         "Feet to Miles"
#define H_MI_KM         "Miles to Kilometers"
#define T_MI_KM         "Miles to Kilometers"
#define H_CA_JO         "Calories to Joules"
#define T_CA_JO         "Calories to Joules"
#define H_HMS           "Hours to Hours.Min/Sec"
#define T_HMS           "Hours to Hours.Min/Sec"
#define H_DE_RA         "Degrees to Radians"
#define T_DE_RA         "Degrees to Radians"
#define H_RE_PO         "Rectangular to Polar"
#define T_RE_PO         "Rectangular to Polar"
#define H_FT_FA         "Feet to Fathoms"
#define T_FT_FA         "Feet to Fathoms"
#define H_FT_ME         "Feet to Meters"
#define T_FT_ME         "Feet to Meters"
#define H_SI_SC         "Square Inches to Square Centimeters"
#define T_SI_SC         "Square In to Square Cm"
#define H_SF_SM         "Square Feet to Square Meters"
#define T_SF_SM         "Sq Ft to Sq Meters"
#define H_CI_CC         "Cubic Inches to Cubic Centimeters"
#define T_CI_CC         "Cubic In to Cubic cm"
#define H_CF_CM         "Cubic Feet to Cubic Meters"
#define T_CF_CM         "Cubic Feet to Cubic Meters"
#define H_FO_ML         "Fluid Ounces to milliliters"
#define T_FO_ML         "Fluid Ounces to milliliters"
#define H_AT_PSI        "Atmospheres to Pounds per Square Inch"
#define T_AT_PSI        "Atmospheres to PSI"
#define H_ACR_FT2       "Acres to Square Feet"
#define T_ACR_FT2       "Acres to Square Feet"
#define T_DEC           "Decimal Mode"
#define H_DEC           "Places the calculator in Decimal Mode. Use WSIZE to set word-size and other format options."
#define T_HEX           "Hexadecimal Mode"
#define H_HEX           "Places the calculator in Hexadecimal Mode. Use WSIZE to set word-size and other format options."
#define T_BIN           "Binary Mode"
#define H_BIN           "Places the calculator in Binary Mode. Use WSIZE to set word-size and other format options."
#define T_OCT           "Octal Mode"
#define H_OCT           "Places the calculator in Octal Mode. Use WSIZE to set word-size and other format options."
#define T_HEXA          "HEX A"
#define T_HEXB          "HEX B"
#define T_HEXC          "HEX C"
#define T_HEXD          "HEX D"
#define T_HEXE          "HEX E"
#define T_HEXF          "HEX F"
#define H_HEXDIG        "Used to enter the Hexadecimal digits"
#define T_AND           "AND"
#define H_AND           "Computes the logical AND of X and Y."
#define T_OR            "OR"
#define H_OR            "Computes the logical OR of X and Y."
#define T_NOT           "NOT (1's Complement)"
#define H_NOT           "Computes the logical complement of the number in X."
#define T_NAND          "NAND"
#define H_NAND          "Computes the logical NAND of the number in X."
#define T_NOR           "NOR"
#define H_NOR           "Computes the logical NOR of the number in X."
#define T_XOR           "Exclusive OR (XOR)"
#define H_XOR           "Computes the logical XOR of Y and X."
#define T_SL            "Shift Left"
#define H_SL            "Shift the number in X left 1 bit."
#define T_SR            "Shift Right"
#define H_SR            "Shift the number in X left 1 bit."
#define T_RL            "Rotate Left"
#define H_RL            "Rotate the number in X left 1 bit."
#define T_RR            "Rotate Right"
#define H_RR            "Rotate the number in X right 1 bit."
#define T_ASCII         "Ascii Chart"
#define H_ASCII         "Displays the system OEM ASCII chart in all 4 supported bases."
#define T_MOD           "Modulo"
#define H_MOD           "Computes Y mod X."
#define T_BINH          "Binary HI"
#define H_BINH          "Shows the upper 16 bits in the binary display."
#define T_BINL          "Binary LO"
#define H_BINL          "Shows the lower 16 bits in the binary display (default)."
#define T_SLM           "Shift Left Multiple"
#define H_SLM           "Shifts the number in Y left X places."
#define T_SRM           "Shift Right Multiple"
#define H_SRM           "Shifts the number in Y right X places."
#define T_CIRCA         "Circle Area"
#define H_CIRCA         "Computes the area of a circle with radius X."
#define T_QUAD          "Quadratic Formula"
#define H_QUAD          "Computes the two roots of the equation for X,Y and Z - stores the results in X,Y."
#define T_PYTHAG        "Pythagorean Theorem"
#define H_PYTHAG        "Computes the hypotenuse in a right triangle w/ lesser sides X and Y."
#define T_PRIME         "Prime Numbers"
#define H_PRIME         "Prime number generator for primes up to 100,000"
#define T_RESIST        "Resistor Table"
#define H_RESIST        "Resistor color bands to X value"
#define T_CPLXSIN       "Complex Sine"
#define H_CPLXSIN       "Computes the Sine of the complex pair X,Y"
#define T_CPLXCOS       "Complex Cosine"
#define H_CPLXCOS       "Computes the Cosine of the complex pair X,Y"
#define T_CPLXTAN       "Complex Tangent"
#define H_CPLXTAN       "Computes the Tangent of the complex pair X,Y"
#define T_CONJ          "Complex Conjugate"
#define H_CONJ          "Computes the Complex Conjugate of the X,Y pair"
#define T_CPLXEXP       "Complex Natural Exponential"
#define H_CPLXEXP       "Computes the Complex Natural Exponential of the X,Y pair"
#define T_CPLXLN        "Complex Natural Logarithm"
#define H_CPLXLN        "Complex Natural Logarithm"
#define T_CPLXLOG       "Complex Base-10 Logarithm"
#define H_CPLXLOG       "Complex Base-10 Logarithm"
#define T_CPLXPOW       "Complex Power"
#define H_CPLXPOW       "Computes the Complex Power of the X,Y pair"
#define T_CPLXSQRT      "Complex Square root"
#define H_CPLXSQRT      "Computes the Complex Square root of the X,Y pair"
#define T_CPLXABS       "Complex Absolute Value"
#define H_CPLXABS       "Computes the Absolute Value of the X,Y pair"
#define T_RLX           "Rotate Left Multiple"
#define H_RLX           "Rotates the contents of Y left by X bits."
#define T_RRX           "Rotate Right Multiple"
#define H_RRX           "Rotates the contents of Y right by X bits."
#define T_ASR           "Arithmetic Shift Right"
#define H_ASR           "Shifts the contents of the X register right 1 bit sign extended."
#define T_NUMBITS       "Number of Bits"
#define H_NUMBITS       "Counts the number of 1 bits set in X."
#define T_SETBIT        "Set Bit"
#define H_SETBIT        "Sets X bit in Y (zero based)."
#define T_CLRBIT        "Clear Bit"
#define H_CLRBIT        "Clears the X bit in Y (zero based)."
#define T_MASKL         "Mask Left"
#define H_MASKL         "Create a mask of X 1's Left Justified."
#define T_MASKR         "Mask Right"
#define H_MASKR         "Create a mask of X 1's Right Justified."
#define T_WORDSIZE      "Word Size"
#define H_WORDSIZE      "Set Word Size and Signed/Unsigned mode."
#define T_CPLXCHS       "Complex Change Sign"
#define H_CPLXCHS       "Changes the sign of the complex X,Y pair."
#define T_ROUND         "Round X"
#define H_ROUND         "Rounds the value in the X register to the closest integer."
#define T_FLOOR         "Floor X"
#define H_FLOOR         "Returns the next integer lower than X (if X does not already correspond to an integer value)."
#define T_CEIL          "Ceiling X"
#define H_CEIL          "Returns the next integer higher than X (if X does not already correspond to an integer value)."
#define T_GCD           "Greatest Common Divisor"
#define H_GCD           "Returns the Greatest Common Divisor of X, Y. Values should be positive whole numbers."
#define T_LCM           "Lowest Common Multiple"
#define H_LCM           "Returns the Least Common Multiple of X, Y. Values should be positive whole numbers."
#define T_MINR          "Minimum Real"
#define H_MINR          "Returns the Minimum Real supported by this program."
#define T_MAXR          "Maximum Real"
#define H_MAXR          "Returns the Maximum Real supported by this program."
#define T_ROUNDYX       "Round Y at X"
#define H_ROUNDYX       "Rounds Y at the X place (must be positive)."
#define T_2COMP         "2's Complement"
#define H_2COMP         "Takes the 2's complement (same as CHS) of the X value. Use NOT for 1's complement."
#define T_MINW          "Minimum Word Value"
#define H_MINW          "Returns the minimum value represented by the current word size and mode."
#define T_MAXW          "Maximum Word Value"
#define H_MAXW          "Returns the maximum value represented by the current word size and mode."
#define T_CURRENCY      "Currency Conversion"
#define H_CURRENCY      "Brings up a table that can do basic Country to Country currency conversion."
#define T_LOG2          "Log Base 2"
#define H_LOG2          "Computes the Log Base 2 of X."
#define T_MIRROR        "Bitwise Mirror Image"
#define H_MIRROR        "Takes the bit pattern and makes a mirror image."
#define T_TIMERSTART    "Timer Start"
#define H_TIMERSTART    "Starts the stopwatch timer."
#define T_TIMERSTOP     "Stop Timer"
#define H_TIMERSTOP     "Stops the stopwatch timer."
#define T_TIMERCLEAR    "Clear Timer"
#define H_TIMERCLEAR    "Clears the stopwatch timer."
#define T_ASTRO         "Astronomy Tables"
#define H_ASTRO         "Display Astronomy Tables"
#define T_IEEE          "IEEE Floating Point Representation"
#define H_IEEE          "Show IEEE floating point of stack variables in Hex notation"
#define T_MOLES         "Gas - # Moles"
#define H_MOLES         "Gas - Computes # Moles with X=Atomic Weight and Y=Mass of Gas"
#define T_MOLECULES     "Gas - # Molecules"
#define H_MOLECULES     "Gas - Computes # Molecules from # moles in X"
#define T_GASVOL        "Gas Volume"
#define H_GASVOL        "Computes Gas Volume (in liters) with # moles in X"
#define T_OHMSE         "Ohm's Law - Voltage"
#define H_OHMSE         "Computes Voltage (in volts) from current in Y and resistance in X"
#define T_OHMSI         "Ohm's Law - Current"
#define H_OHMSI         "Computes Current (in Amps) from voltage in Y and resistance in X"
#define T_OHMSR         "Ohm's Law - Resistance"
#define H_OHMSR         "Computes Resistance (in Ohms) from voltage in Y and current in X"
#define T_GRAVITATION   "Gravitational Force"
#define H_GRAVITATION   "Computes Gravitational Force (in newtons) given  X=distance between objects. Y=mass of object 1 (in kg) and Z = mass of object 2 (in kg)."
#define T_ENERGY        "Energy (E=mc^2)"
#define H_ENERGY        "Computes Energy (in Joules) of an object with mass X (in kg)."
#define T_CENTFORCE     "Centripetal Force"
#define H_CENTFORCE     "Computes Centripetal Force (in Newtons) given mass in X (kg), linear speed in Y (m/s) and radius of circle in Z (meters)."
#define T_CENTACCEL     "Centripetal Acceleration"
#define H_CENTACCEL     "Computes Centripetal Acceleration (m/s^2) with linear speed in X (m/s), radius in Y (m)"
#define T_ACCELDIST     "Acceleration Distance"
#define H_ACCELDIST     "Computes the falling distance (in meters) of an object at X seconds with Y acceleration (m/s^2)"
#define T_PROJRANGE     "Projectile Range"
#define H_PROJRANGE     "Computes the range of a projectile (in meters) given firing angle X and initial velocity Y (in m/s)."
#define T_PROJHEIGHT    "Projectile Height"
#define H_PROJHEIGHT    "Computes the height of a projectile (in meters) given firing angle X and initial velocity Y (in m/s)."
#define T_RECALLN       "Recall Xth Register"
#define H_RECALLN       "Recall's the Xth register (R0-R99). Convenient for macro record use."
#define T_STOREN        "Store Xth Register"
#define H_STOREN        "Stores the Xth Register (R0-R99). Convenient for macro record use."
#define T_GA_CM3        "Gallons to Cubic Centimeters"
#define H_GA_CM3        "Gallons to Cubic Centimeters"
#define T_PINT_LI       "Pints to Liters"
#define H_PINT_LI       "Pints to Liters"
#define T_IGA_LI        "Imperial Gallons to Liters"
#define H_IGA_LI        "Imperial Gallons to Liters"
#define T_FA_KE         "Fahrenheit to Kelvin"
#define H_FA_KE         "Fahrenheit to Kelvin"
#define T_IN_MM         "Inches to Millimeters"
#define H_IN_MM         "Inches to Millimeters"
#define T_FT_KM         "Feet to Kilometers"
#define H_FT_KM         "Feet to Kilometers"
#define T_KMH_MPS       "Km per Hr to Meters per sec"
#define H_KMH_MPS       "Kilometers per hour to Meters per second"
#define T_KNOT_MPH      "Knots to Miles per Hour"
#define H_KNOT_MPH      "Knots to Miles per Hour"
#define T_AH_COL        "Amp-Hours to Coulombs"
#define H_AH_COL        "Amp-Hours to Coulombs"
#define T_AH_FAR        "Amp-Hours to Faraday"
#define H_AH_FAR        "Amp-Hours to Faraday"
#define T_HP_KW         "Horsepower to Kilowatts"
#define H_HP_KW         "Horsepower to Kilowatts"
#define T_CONV_INV      "Conversion Inverse"
#define H_CONV_INV      "After pressing this key, press a conversion key to preform the inverse conversion."
#define T_MIL_MM        "Mils to mm"
#define H_MIL_MM        "Converts from mils to mm."
#define T_CFNJ          "Repeat Cash Flow Entry"
#define H_CFNJ          "Repeat last Cash Flow Entry X times."
#define T_SPPV          "Single Payment Present Value"
#define H_SPPV          "Computes Single Payment Present Value based on {i} interest and {n} periods."
#define T_SPFV          "Single Payment Future Value"
#define H_SPFV          "Computes Single Payment Future Value based on {i} interest and {n} periods."
#define T_USPV          "Uniform Series Present Value"
#define H_USPV          "Computes Uniform Series Present Value based on {i} interest and {n} periods."
#define T_USFV          "Uniform Series Future Value"
#define H_USFV          "Computes Uniform Series Future Value based on {i} interest and {n} periods."
#define T_INFL          "Inflation"
#define H_INFL          "Computes the effect of {n} periods of {i} inflation on the dollar value in X."
#define T_EFF           "Effective Interest Rate"
#define H_EFF           "Returns Effective Interest Rate of {i} compounded {n} periods."
#define H_LABELS        "Sets program label. After setting a label, you can use goto or gosub to branch to that label."
#define T_MACXLY        "X <= Y?"
#define H_MACXLY        "Compares X against Y. If X is less than or equal to Y execute the next program step, else skip the next program step."
#define T_MACXGY        "X > Y?"
#define H_MACXGY        "Compares X against Y. If X is greater than Y execute the next program step, else skip the next program step."
#define T_XLEZERO       "X <= 0?"
#define H_XLEZERO       "Compares X against zero. If X is less than or equal to zero execute the next program step, else skip the next program step."
#define T_XGTZERO       "X > 0?"
#define H_XGTZERO       "Compares X against zero. If X is greater than zero execute the next program step, else skip the next program step."
#define T_XEY           "X=Y?"
#define H_XEY           "Compares X against Y. If X is equal to Y, execute the next program step, else skip the next program step."
#define T_XNEY          "X!=Y?"
#define H_XNEY          "Compares X against Y. If X is not equal to Y, execute the next program step, else skip the next program step."
#define T_XEZERO        "X=0?"
#define H_XEZERO        "Compares X against zero. If X is equal zero, execute the next program step, else skip the next program step."
#define T_XNEZERO       "X!=0?"
#define H_XNEZERO       "Compares X against zero. If X is not equal zero, execute the next program step, else skip the next program step."
#define T_LBLA          "Label A"
#define T_LBLB          "Label B"
#define T_LBLC          "Label C"
#define T_LBLD          "Label D"
#define T_LBLE          "Label E"
#define T_LBLF          "Label F"
#define T_LBLG          "Label G"
#define T_LBLH          "Label H"
#define T_GOTOA         "Goto A"
#define T_GOTOB         "Goto B"
#define T_GOTOC         "Goto C"
#define T_GOTOD         "Goto D"
#define T_GOTOE         "Goto E"
#define T_GOTOF         "Goto F"
#define T_GOTOG         "Goto G"
#define T_GOTOH         "Goto H"
#define H_GOTOS         "Goto Label. Program will branch to the specified label (if it exists)."
#define T_GSBA          "Gosub A"
#define T_GSBB          "Gosub B"
#define T_GSBC          "Gosub C"
#define T_GSBD          "Gosub D"
#define T_GSBE          "Gosub E"
#define T_GSBF          "Gosub F"
#define T_GSBG          "Gosub G"
#define H_GOSUBS        "Gosub to Label - Execute program until a Return is hit."
#define T_RETURN        "Return"
#define H_RETURN        "Return from current Gosub function."
#define T_SFX           "Set Flag X"
#define H_SFX           "Set Flag X (32 user flags - X must be whole number)."
#define T_CFX           "Clear Flag X"
#define H_CFX           "Clear Flag X (32 user flags - X must be whole number)."
#define T_TFX           "Test Flag X"
#define H_TFX           "Test Flag X (32 user flags - X must be whole number). If TRUE, execute next program line, else skip next line."
#define T_PAUSE         "Pause Program"
#define H_PAUSE         "Pause Program 1 second to show stack."
#define T_INPA          "Input R0"
#define H_INPA          "Prompt user for R0 register value."
#define T_INPB          "Input R1"
#define H_INPB          "Prompt user for R1 register value."
#define T_TRACE         "Trace Program"
#define H_TRACE         "Auto Trace Program - Trace Delay is configured on File/Settings"
#define T_FWD           "Forward Step"
#define H_FWD           "In record mode, it will step forward one program instruction. In non-record mode this key is the same as PLAY but it will play the program back in slow motion and show the step number on the stack. Can help with debugging."
#define T_REV           "Reverse Step"
#define H_REV           "In record mode, will step back one program instruction. New instructions are inserted after the current program line."
#define H_MEDIT         "Edit program. Same as REC but does not overwrite current program. You can use FWD and REV to step thru the program and new buttons pressed will be inserted after the currently displayed program line."
#define T_DEL           "Delete Step"
#define H_DEL           "In record mode it will Delete currently shown program line. You can use FWD and REV to get around the program while in record mode."
#define T_MEDIT         "Edit program."
#define T_LBLI          "Label I"
#define T_GOTOI         "Goto I"
#define T_GSBI          "Gosub I"
#define T_GSBH          "Gosub H"
#define T_LBLJ          "Label J"
#define T_GOTOJ         "Goto J"
#define T_GSBJ          "Gosub J"
#define T_LBLK          "Label K"
#define T_GOTOK         "Goto K"
#define T_GSBK          "Gosub K"
#define T_INPC          "Input R2"
#define H_INPC          "Prompt user for R2 register value."
#define T_INPD          "Input R3"
#define H_INPD          "Prompt user for R3 register value."
#define T_STOIND        "Store Indirect Register"
#define H_STOIND        "Stores X value to the Indirect Register (i)"
#define T_GOTOIND       "Goto Indirect"
#define H_GOTOIND       "Goto label pointed to by the Indirect (i) register (0=A, 1=B, etc)."
#define T_GSBIND        "Gosub Indirect"
#define H_GSBIND        "Gosub to the label pointed to by the Indirect (i) register (0=A, 1=B, etc)."
#define T_RCLIND        "Recall Indirect Register"
#define H_RCLIND        "Recalls the Indirect (i) register to the X register."
#define T_HALT          "Halt Program"
#define H_HALT          "Halts Program Execution"
#define T_STEP          "Single Step Program"
#define H_STEP          "Single Step Program"
#define T_DEBUG         "Debug Program"
#define H_DEBUG         "Enter Program Debug Mode"
#define T_BEEP          "Beep"
#define H_BEEP          "Emit a single beep tone."
#define T_LOOPA         "Loop A"
#define H_LOOPA         "Decrement R0 register, if above zero branch to A."
#define T_LOOPB         "Loop B"
#define H_LOOPB         "Decrement R1 register, if above zero branch to B."
#define T_LOOPC         "Loop C"
#define H_LOOPC         "Decrement R2 register, if above zero branch to C."
#define T_LOOPD         "Loop D"
#define H_LOOPD         "Decrement R3 register, if above zero branch to D."
#define T_LOOPE         "Loop E"
#define H_LOOPE         "Decrement R4 register, if above zero branch to E."
#define T_LOOPF         "Loop F"
#define H_LOOPF         "Decrement R5 register, if above zero branch to F."
#define T_LOOPG         "Loop G"
#define H_LOOPG         "Decrement R6 register, if above zero branch to G."
#define T_LOOPH         "Loop H"
#define H_LOOPH         "Decrement R7 register, if above zero branch to H."
#define T_LOOPI         "Loop I"
#define H_LOOPI         "Decrement R8 register, if above zero branch to I."
#define T_LOOPJ         "Loop J"
#define H_LOOPJ         "Decrement R9 register, if above zero branch to J."
#define T_LOOPK         "Loop K"
#define H_LOOPK         "Decrement R10 register, if above zero branch to K."
#define T_DSZ           "Decrement Skip if Zero"
#define H_DSZ           "Decrement the Indirect (i) Register and skip next instruction if zero."
#define T_TIMERPUSH     "Timer Push"
#define H_TIMERPUSH     "Pushes current timer value onto the stack."
#define T_STON_LB       "Short Tons to Lbs"
#define H_STON_LB       "Short Tons to Lbs"
#define T_LTON_LB       "Long Tons to Lbs"
#define H_LTON_LB       "Long Tons to Lbs"
#define T_MTON_LB       "Metric Tons to Lbs"
#define H_MTON_LB       "Metric Tons to Lbs"
#define T_CLEARF        "Clear All 32 Flags"
#define H_CLEARF        "Clear programming flags register to 0x00000000"
#define T_STO2I         "Store Indirectly"
#define H_STO2I         "Stores X to the register (R0-R99) pointed to by the Indirect (i) register"
#define T_RCL2I         "Recall Indirectly"
#define H_RCL2I         "Recalls value from the register (R0-R99) pointed to by the Indirect (i) register"
#define T_CSINH         "Complex sinh"
#define H_CSINH         "Complex sinh (X=Real, Y=Imaginary)"
#define T_CCOSH         "Complex cosh"
#define H_CCOSH         "Complex cosh (X=Real, Y=Imaginary)"
#define T_CTANH         "Complex tanh"
#define H_CTANH         "Complex tanh (X=Real, Y=Imaginary)"
#define H_TOPOLAR       "Convert to Polar Coordinates (r,0) from Rectangular Coordinates (X,Y)"
#define T_TOPOLAR       "Convert to Polar Coordinates"
#define H_FROMPOLAR     "Convert from Rectangular (X,Y) Coordinates to Polar Coordinates (r,0)"
#define T_FROMPOLAR     "Convert to Rectangular Coordinates"
#define T_CNORM         "Complex Norm"
#define H_CNORM         "Complex Norm"
#define T_CARG          "Complex Arg"
#define H_CARG          "Complex Arg"
#define T_N_LBF         "Newtons to Pounds Force"
#define H_N_LBF         "Convert from Newtons to Pounds Force"
#define T_HZ_AF         "Hertz to Angular Frequency"
#define H_HZ_AF         "Convert from Hertz to Angular Frequency (rad/sec)"
#define T_METRIC        "Metric Table"
#define H_METRIC        "Metric Prefixes and their magnitudes"
#define T_GAMMA         "Gamma Function"
#define H_GAMMA         "Calculate the gamma of a Complex number X (or real number if imaginary Y is zero)"
#define T_MPA_PSI       "Megapascals to PSI"
#define H_MPA_PSI       "Convert from Megapascals to PSI"
#define T_CONST         "Constants"
#define H_CONST         "Select from a number of scientific universal constants"

#include "resource.h"
