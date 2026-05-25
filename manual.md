# Excalibur (Quick-Start Guide)

At one time, Excalibur had extensive help and an RPN tutorial that was largely never read. To that end, I've decided to pair down the online help to the things that likely are of interest to the user.

* Internally uses IEEE double precision floating point for rough accuracy out to about 15 places. Excalibur will show up to 13 significant digits - reserving the last two for internal rounding.
* Uses a classic HP 4-deep RPN stack (X, Y, Z, T). In settings you can change this to 8-deep (adding A, B, C, D).
* Default for ENTER key is to duplicate the X register into Y like the classic HP calculators. In settings, you can switch to eRPN (Entry RPN) for a more RPL-like entry.
* Excalibur has 3 different layouts. The classic layout by default and in settings you can switch to left-positioned Operators (-+×÷) or to a smaller 4-banger (it's a bit more than that!) layout.
* Setting the display of numbers is done with the 'Disp' button near the middle-top row. This is the equivilent of the FIX, SCI, ENG handling of HP calculators.
* By default, the Change-Sign (CHS) and Exponent (EEX) keys are in the old HP naming style. You can change them to more modern equivilents in settings.
* Programming is non-merged keystroke based much like the earliest HP calculators. You can have up to 40 different programs and each program can have up to 400 steps (total of 16K worth of programming steps).
* By default, the comma is used for the thousands seperator and the decimal point is the radix seperator. You can change this in settings.
* To see a list of common keyboard keys that you can press to activate common functions of the calculator - see  the 'Help' menu.
  
**Number Formats - Real vs Integer**

Excalibur has a dual stack to handle both real (floating point) numbers and integer-based numbers. The integer number system is used only when in Comp-Sci mode - the stack will shift to be right-justified and you will see a 'd' or 'h' or 'b' to indicate Decimal, Hex or Binary values are being shown. The status bar will let you know you are in integer mode and display the current word size (e.g. S32 for Signed 32-bit).

**Exponents**

To enter a number with an exponent you must first enter the base (mantissa) part of the number. Then press the E/EEX key to insert the "e+" exponent. Then type the exponent. For example, to enter 6.02e+23 you would enter the 6.02 first, then press the EEX key, then enter 23.

**Change Sign**

To change of a number in the X register, simply press the CHS (change sign) key. To change the sign of an exponent you must do it as you enter the number. After you hit the EEX key and before you finish entering the exponent, hit the CHS key to change the sign of the exponent. For example, to enter 1.34e-12 you would first enter in 1.34 then press EEX. Then you could press CHS and 12 to get the 1.34e-12 value to show in the X register.

**Fractions**

Fractions can be entered in two ways. The most common way is to use the Frac key to separate the components of the fraction. For example, to enter 5 and 3/8 you would enter 5, hit the Frac key, then hit 3 and hit the Frac key again and finally enter 8. This will lead to the fractional display of 5~3/8

You can also use the decimal point to enter fractions. To enter the same 5 and 3/8 using only the decimal point, you enter 5 then hit the decimal point to get "5." on the display. Then enter the 3 and press the decimal point again and finally enter the 8. The display will, once again, read 5~3/8.

Once a fraction is entered, pressing ENTER or any numeric operation key will automatically convert the fraction to its decimal equivalent (that is: the fractional notation is not carried across future computations).

# Programming Basics

Programming is one of the most powerful features of Excalibur. I was worried that some people would be turned off by the use of the word “programming” and considered calling the feature “Macros” instead since the programming of Excalibur can really be thought of as recording a series of keystrokes for automated rapid playback. That is, any task that you can do by hand with Excalibur can be recorded for instant playback. But the programming mode allows much more flexibility – counters and loops and subroutines (none of which are needed to enjoy quite a bit of power with the programming mode – you can stick to just recording keystrokes for rapid playback). In the end I left this as “Programming Mode” but don’t let that deter you from some real under-the-hood power of Excalibur.

Recording programs is very easy. On the primary keypad you will see a button labeled REC and one labeled PLAY. The REC key puts the calculator into program record mode. A "REC" indicator will appear in the status bar near the bottom to indicate this and the stack will change and will show normal X, Y but the Z, T will now show the current program line and function. It is important to note that when you are in Programming REC mode, the keystrokes are being recorded but also the stack is still "live" and you are actually performing the calculations you are entering. I feel this is superior to the more traditional method of only showing the user the key values as they are pressed but not actually taking action on them until the program is run. In this way, you can feel your way though the programming effort and end up with less bugs in your program sequence. Every calculator key pressed (clicked) is stored in sequence until REC is hit again which will turn off the record feature. Pressing PLAY will then cause the key sequence recorded to playback in rapid succession. So, for example, you could enter the following key sequence:

REC (starts recording)<br>
9<br>
\*<br>
REC (ends recording)<br>

which would effectively multiply the X value by 9 every time PLAY is hit. Give it a try… enter a number into the X register and hit PLAY.

Several hundred Excalibur keys can be recorded in this way. Once recorded, the program will stay with the calculator (even between sessions – it’s saved to disk) until a new REC takes place. If you want to save your program more permanently, simply use the "Program Manager" menu selection from the File menu. The program manager allows you to store up to 40 programs – each of which can be up to 400 steps long (that's roughly 16K worth of programming steps in total).

**Keystroke Programming**

Keystroke programming is the most simple form of programming and the most common. Most of the users who ever venture into programming will only venture as far as Keystroke programming - and with good cause since it will allow you to do countless useful programs of your own choosing. Keystroke programming does not use branching, loops or subroutines. Only repetitive keystrokes are recorded for playback. Using only this method, user-defined constants, conversions and straight-line programs can be created and used with little effort. Let's take an example of keystroke programming. Suppose you wish to compute the circumference of a circle given the radius in X (I know this already exists as a built in Excalibur function but it makes a simple and nice example). First, start by pressing REC to turn the programming mode on. You will see at the top of the display the program counter set at 000 (start of program). Starting in the Scientific bank, enter in the following program simply by pressing the keys in the order shown:

001 - X Squared<br>
002 - PI<br>
003 - Multiply<br>
004 - \<End Of Program\><br>
<br>
Checksum: 00A0 (you can see the program checksum from the File/Program Manager dialog box)<br>

You have now entered the sequence of keystrokes to produce the area of a circle given the radius in X. Now enter a radius into X and press Play - you will see the answer displayed in X. A nearly infinite number of similar programs can be created in this manner. The Program Manager (on the File menu) allows you to see the entire program at a glance along with the checksum. This checksum is useful to determine if you’ve keyed in the program correctly (it is also used when programs are copied/pasted to and from the clipboard – they ensure that your program transferred correctly).

**Labels, Branching and Conditionals**

Normally the program execution flows from the beginning of the program to the end one step at a time. Labels and branching allows the programmer to skip lines either forwards or backwards. This is done with a label and a corresponding goto. A series of backwards gotos is called a loop (normally terminated using conditions on the Program bank layout). Here is a simple program which demonstrates labels, branching and conditionals. It takes a value in X and if the value is greater than 200 it will divide it by 3 otherwise it will multiply it by 6.

001 - Clear Stack<br>
002 - Digit 2<br>
003 - Digit 0<br>
004 - Digit 0<br>
005 - Exchange X and Y<br>
006 - X <= Y?<br>
007 - Goto A<br>
008 - Goto B<br>
009 - Label A<br>
010 - Digit 6<br>
011 - Multiply<br>
012 - Halt Program<br>
013 - Label B<br>
014 - Digit 3<br>
015 - Divide<br>
016 - Halt Program<br>
017 - <End Of Program><br>
Checksum: 0864<br>
<br>

The conditional you see above (X<=Y?) is typical of many of the programming keys (found on the Program I and Program II banks). This particular command will compare X and Y… if X is less than or equal to Y the next statement is executed (the condition is TRUE). If X is greater than Y the next statement is skipped (condition is FALSE). Therefore, the code will either execute Goto A (if X is less than or equal to Y) or it will execute Goto B (if X is greater than Y).

There are 10 different labels that can be used with Excalibur labeled A-J.

**Looping**

Looping is simply a branch to an earlier instruction that is predicated on the outcome of a conditional test. We loop for several reasons - but mainly to count or sum a series of numbers or calculations. Here is a program to compute a simple summation sequence of 0+1+2+3+4...X. If you enter and X of 6 you should get 0+1+2+3+4+5+6 for a total of 21. The program looks as follows:

001 - Digit 0<br>
002 - Store Register<br>
003 - Digit 0<br>
004 - Drop Stack<br>
005 - Label A<br>
006 - Enter<br>
007 - Enter<br>
008 - Recall Register<br>
009 - Digit 0<br>
010 - Plus<br>
011 - Store Register<br>
012 - Digit 0<br>
013 - Drop Stack<br>
014 - Digit 1<br>
015 - Minus<br>
016 - X > 0?<br>
017 - Goto A<br>
018 - Clear Stack<br>
019 - Recall Register<br>
020 - Digit 0<br>
021 - /<End Of Program/><br>
Checksum: 0465<br>
<br>

WARNING - If you ever goto an earlier programming step and do not provide a way to break the looping cycle you will end up with an infinite loop. It is also possible that given the speed at which Excalibur executes instructions (which is many thousands of instructions per second) extremely long loops could take significant time. During this time, the program will show the word "Run..." in the status bar and will be otherwise oblivious to normal keystrokes or mouse clicks. To interrupt an infinite loop you should click the Play key or press the ESC key and the program will return to the non-running state.

**Subroutines**

Subroutines are useful for repetitive calculations. Each subroutine must start with a label and end with a "Return" statement. The label defines the start of the subroutine – the Return statement tells Excalibur to return to the line following the call to the subroutine. All of the tools for subroutines are found on the Program bank of functions.

For example, to compute the area of 2 circles with the first circle's radius in X and the second circle's radius in Y you would key in the following program:

001 - Gosub A<br>
002 - Exchange X and Y<br>
003 - Gosub A<br>
004 - Exchange X and Y<br>
005 - Halt Program<br>
006 - Label A<br>
007 - X Squared<br>
008 - PI<br>
009 - Multiply<br>
010 - Return<br>
011 - \<End Of Program\><br>
Checksum: 0D2E<br>

Now entering a radius in X and a radius in Y and pressing "Play" will yield the area of both circles. The code is a little tricky but not overly complicated. The first line simply tells Excalibur to go to subroutine A (indicated by the Label A instruction). Excalibur will jump to label A and continue executing until a Return statement is reached at which time control is given back to the line after the Gosub A call and execution continues from there. There is no limit to the number of subroutines that can be called, however if a subroutine calls itself (recursively), the number of recursive calls can be nested only 1000 levels deep (Excalibur will give you an error dialog box if you attempt to call nested subroutines deeper than that).

**Binding Programs to Custom Keys**

To assign a program directly to a key, simply record any program you wish. Save it using "Program Manager" from the File menu. You can give it a short "key" name and a longer more descriptive name. Then you can go to the Define Custom Set menu item also in the File menu. There you will see a list of all keys and programs and you can click to assign any key or program to any desired custom key on the custom key bank. When you show the Custom bank you will see the short key name of your program and the tool-tip will be the longer more descriptive name. Pressing that key will load and invoke the program (but will not harm the contents of any current REC/PLAY macro program).

This should allow you to make your own constants, conversions, formulas, etc. with relative ease. As of right now, menu items are not saved as part of the program record process - just buttons. This should pose very little problem since you can always type in the constant value as part of the program or use the StoA/RclA or StoB/RclB keys to get at more saved storage.

**Editing Programs**

We all make mistakes during programming or need to add/remove features to existing programs. Excalibur allows the editing of the current program while recording by use of the REV, DEL, FWD keys. These keys will step around the current program from beginning to end if necessary. Use DEL to remove the current program step (as shown on the stack… the debug window will actually show the next statement to be executed as it always does). Any new buttons pressed are inserted after the currently displayed program step (as shown on the stack). If you are not recording a program and wish to edit the currently loaded program, simply hit EDIT and Excalibur will enter the program record mode but will not erase the current program (as REC would do) and you can use the REV, DEL, FWD keys to edit the program. When done you can press either REC or EDIT to end the program edit.

**Program Manager**

Only one program is loaded into memory at any given time. This is the program that is run when "Play" is hit.  If the "Rec" key is hit, the current program will be overwritten and a new program would be recorded. The Program Manager allows the user to save the current program to one of 200 program slots. The program can be given a short name (shown when the program is assigned to a key on the Custom bank) as well as a longer more descriptive name. Once saved, any program can be recalled back into service. Simply select the program on the left-hand pane and click "Load Program" - this will overwrite the currently loaded program with the selected program.  When you click on a program the program listing (with checksum) is shown so you can verify this is the program you want to load.

**Importing/Exporting Programs**

The currently loaded program can be exported to the clipboard by selecting the "Export Program to Clipboard". The program listing, line numbers and checksum are placed onto the clipboard in standard text format. They can be pasted into another program, email or text editor.  Importing programs is just as easy - simply highlight and copy a program listing (checksum is optional) and copy it to the clipboard. Within the Excalibur Program Manager click the "Import Program from Clipboard" to bring this program into Excalibur (the currently loaded program will be overwritten). If there are any problems with the program import, the user will be shown the offending program line. Once imported, this program can be run as a normal program or saved to one of the 200 program slots.

**Program Debugger**

The program debugger allows the user to see the entire program as it is running along with the contents of the stack, internal registers and other bits of debug information.  The debug window is activated or removed by selecting the "Program" button on Program Bank II. The debug window remains active while Excalibur is still functional so that everything within Excalibur I still up and running while the debugger acts as a "spy" to some key internals of the program as it runs. Using Trace or Step allows the user to work through the program a line at a time - this should make finding problems or errors in the program fairly painless.

**Tracing Programs**

If you want to see the program played out slowly so you can try and debug it, you use the STEP button on the program bank. This button has is used when not in record mode -- it moves forward one program step and executes that step. The step is briefly shown in the T register on the stack (and also in the debug window if opened). This key is used to slowly play back the program so you can see the steps and the stack. To play back the program slowly but automatically, use the Trace button. To control the delay time between steps, use File/Settings->Pause/Trace Delay.

**Indirect Addressing**

One of the most powerful programming techniques is the use of indirect addressing. Indirect addressing allows variable branch labels and register access. To indirectly address one of the 26 storage registers, use the StoXth and RclXth keys.  These use the X register as the index to determine which storage register should be used. For example, entering 1 in X and pressing StoXth will store the current Y value to the first register (the R0 register).  Using StoXth and RclXth all 26 registers can be addressed with 1=A, 2=B … 26=Z.  StoXth and RclXth both drop the stack so the index value is no longer on the stack at the end of the operation.  For example, if you wanted to store the value 123 to the R26 register, key in the following sequence:

123<br>
ENTER<br>
26<br>
StoXth<br>

This will drop the stack and use 26 as the index to the register that should be used to STO the 123 value from the stack. You can use the Program Debugger or the normal STO key on the main keypad to verify the value was saved properly. To save time and keystrokes the first half dozen registers can be stored and recalled with buttons located on Program Bank II.

Perhaps of greater importance to programmers using Excalibur, labels can also be indirectly addressed. Excalibur uses a special Indirect Register called 'i' which is an integer based internal register. To store a number into the indirect register use the "Sto i" key. To recall the indirect register to the stack, use the "Rcl i" key. The number in the Indirect Register represents a program label with 0=label A, 1=label B, 9=label J.  Using the Goto(i) or Gsb(i) keys will jump to the label indicaetd by the value of the Indirect register.

**Excalibur Programming Speed**

Excalibur program steps run based on the speed of your computer. But, in practice it hardly matters. It will execute many thousands of steps per second on even the lowliest computer capable of running a 32-bit Windows operating system.  On my fairly modest PC it performs about 60,000 steps per second. I ran an experiment taking a simple 6 step program that incremented a counter, inverted the value and then took the SIN of the value and then looped back to do it again. I let a variety of calculators run the program for 30 seconds. What I found was:

HP15C ran 24 iterations of the program in 30 seconds.<br>
HP11C ran 27 iterations of the program in 30 seconds.<br>
HP25 ran 32 iterations of the program in 30 seconds.<br>
HP41C ran 51 iterations of the program in 30 seconds.<br>
HP32SII ran 237 iterations of the program in 30 seconds.<br>
Excalibur v3.00 on a modest i5 ran 57,335,221 iterations of the program in 30 seconds.<br>

Based on this simple experiment, I think it’s safe to say that Excalibur runs more than 1 million times faster than the early HP handhelds.

# Excalibur Functions

Excalibur breaks down functionality into 'banks'. Each bank contains up to 40 keys - and each key performs one function.

| Bank | General Functionality |
| :--- | :--- |
| Scientific I | The main bank of scientific functions, trig calculations, constants, etc. |
| Scientific II | The more advanced scientific functions, physics and complex number handling. |
| Financial | Financial data for TVM of Money, Cash Flow Analysis and basics of Real Estate. |
| Statistics | Statistics and numerical analysis computations. |
| Conversion | A number of conversions (to and from) including most of the Metric to Imperial formulas.|
| Geometry | Basics of Geometry, area and volume computations.|
| Comp-Sci | Computer Science data handling and computations in Decimal, Hex, Binary and Octal. |
| Program I | Macro programming - loops, subroutines and sundry logic. |
| Program II | Even more macro programming - loops, subroutines and sundry logic. |
| Custom | A custom-defined bank - you can put any 40 functions on this bank from any of the other banks above.|

# Handheld Calculators, HP and Other Ramblings

Where can you get a handheld RPN calculator? Well, many years ago the best calculators were made by Hewlett Packard (HP) and virtually all their early series handheld calculators were RPN. The calculators were built to last – using double shot injection molded keys (that is, the numbers and legends on keys were molded with white plastic – you could sand a key down and still see the number!). Unfortunately, HP calculators have undergone serious cost reductions. Key labels are now just painted on. Construction is not the same high-quality. And most of the newer HP calculators are no longer RPN (those that offer RPN are switch-hitters offering both RPN and Algebraic… not the most aesthetic mix).

Older HP calculators tend to be a bit expensive but in my experience they are worth it. They have a number of models which are RPN only, some that are Algebraic only and some that switch-hit between the two notations. If you want an RPN calculator, stick with an RPN only. Current models include the HP 12C - a classic financial calculator (but a real workhorse for any calculations) sold in most office supply stores and most bigger electronic chain stores and the 33S which is the Scientific model (Early models of the 33S had known bugs and some problems such as ultra small decimal point… some of this has been fixed in late 2005 so check that your decimal point is plenty readable before buying!). Discontinued in 2000, the HP 32SII is an RPN scientific calculator with tons of features and functions and probably can still be found in used but good condition. The 48G is the big-boy and has graphics screen for serious computational work. Since they have been making handheld RPN calculators for something like 30 years, you should be able to find a second-hand ones… but don’t be shocked if the online prices are really high – these things are coveted! Try looking at yard sales or flea-markets or even the local want ads. Some interesting discontinued models include the 32S which is the predecessor to the 32SII and has nearly the same functionality. The 42S which packs hundreds of functions onto soft-keys and is a more advanced scientific than the 32SII. The 10C basic scientific, 11C advanced scientific, 15C scientific with matrix and complex math, 16C computer scientist - all of which are built like the 12C financial calculator mentioned above (they are part of a group of calculators that make up the 10C series). Even older would be the classic HP25 scientific programmable or the “original” HP35. It should be easy to determine if a calculator is RPN - it should have a prominent ENTER key and no equals key (some of the newer RPN only calculators have an equals key for equation solving, but it's fairly obvious it's not for primary use).

# Precision, Accuracy and Overflow

Currently, Excalibur for Windows uses an 8 byte IEEE floating point representation of all numbers. This means that there are approximately 15 significant digits and Excalibur will show up to 13 of those.

Because of this Excalibur will NOT be as accurate as a hand-held calculator nor some of the more modern decimal-math based online calculators. This is because floating point decimal values using IEEE floating point representation generally do not have an exact binary representation. This is a side effect of how the CPU represents floating point data. For this reason, you may experience some loss of precision, and some floating point operations may produce unexpected results.

In addition, the calculator only does a modest amount of checking for overflow (numbers greater than the maximum or less than the minimum). The 8 byte float limits for Excalibur are shown below:

DBL\_MIN2.2250738585072014e-308<br>
DBL\_MAX1.7976931348623158e+308<br>

As stated in the license disclaimer, this program has no guarantees associated with it. If the precision (or lack thereof) is a problem for you, please discontinue use of Excalibur.<b>

# Donations Welcome (but never required!)

Excalibur has been a labor of love. More than 1000 hours of development have gone into it - including answering many hundreds of emails over the decades. It's free to use and always will be, however if you feel inclined to buy me a virtual coffee for the effort, that would be amazing!

[<img src="https://github.com/user-attachments/assets/ab67686c-2168-46a3-b39f-77b5f5fef5d0">](https://ko-fi.com/wavemotiondave)

<b>
https://ko-fi.com/wavemotiondave
</b>
