# RPN (Quick-Start Guide)

This section describes how operations and calculations take place in Excalibur. It is intended that after you read this section you will have enough knowledge to work both basic and more advanced equations in Excalibur with a minimum of difficulty. Excalibur does not use more conventional "algebraic" notation like many low-end calculators on the market. Instead it uses the high-powered parenthesis-free RPN (Reverse Polish Notation). This notation was named in honor of a Polish mathematician and logician named Jan Lukasiewicz. Polish notation places the operators in an expression before the numbers. RPN places them after the numbers (which is why they call it Reverse Polish Notation). This notation has been used on some of the worlds best Calculators – most notably those from Hewlett Packard.

RPN is really nothing more than the method of entry of numbers and operators into the calculator. For example, to compute the following "algebraic" expression:

3\*4+5=

You would enter 3 4 \* 5 +

The operands are simply inserted after the numbers are entered. On an RPN calculator, a key typically labeled ENTER is used to separate numbers. This whole method works because of something called the "Memory Stack" which is a place holder for the numeric entries before any operators are used. Even some of the best algebraic calculators break down into the simpler and more consistent RPN mode when it comes to unary operators. Very rarely do you enter SIN(90). More often you enter 90 and hit the SIN key (in this case since you hit SIN after the number you have preformed an RPN calculation) RPN simply takes it a step further by being more consistent and using this entry method for virtually all of the calculators functions. For longer calculations, you will be amazed at how much control you feel when entering the numbers and producing the results. Some of the most difficult chain calculations are daunting with an algebraic calculator but are easy work for an RPN one. Notice that there are no parentheses on an RPN calculator - they are not needed. Precedence is handled by the stack and by how you enter the numbers into the calculator.

RPN follows very specific and simple rules. Even the best algebraic calculators have vague rules for a given key. Take plus (‘+’) for example… sometimes it adds as in 5+3.  Sometimes it subtracts as in 5-3+2 (on entering the first minus, nothing happens, when you hit the plus key, 3 is subtracted from 5 pending the next number entered). Sometimes hitting plus it does nothing as in 5+3\*3 (this addition on calculators with proper precedence will not take place until after the multiplication). With an RPN calculator hitting plus does one thing only – it adds two numbers together... there are no exceptions. What a novel concept!  Let’s look at the memory stack and start to understand why RPN is both efficient and consistent.

**The Memory Stack**

The automatic memory stack is at the heart of all calculations in this (or any) RPN calculator. It provides for an efficient storage of intermediate results during complex equations and allows for easy visualization of where your entered numbers are stored.

The automatic memory stack is shown as follows (class 4-level stack is shown, Excalibur allows for a bigger stack if desired):

T          0          Oldest register<br>
Z          0<br>
Y          0<br>
X          0          Newest or Current register<br>

Values which you are currently entering are placed in the X register. Older values have been pushed up the stack to Y, Z or T.

Here is an example which will fill in the stack with a desired set of numbers:

(1) Press the CLS button to clear out all of stack memory. The stack will look like the table above with zeros for all elements.

(2) Press the following digit keys: 157. The stack display should look as follows:

T          0          Oldest register<br>
Z          0<br>
Y          0<br>
X          157\_     The underscore tells you current edit position<br>

(3) Press the ENTER key. This separates one number on the stack from another number. The old value is pushed up the stack to the Y register and makes room for a new entry on the X register. This is called a “stack lift”. The stack will look as follows:

T          0          Oldest register<br>
Z          0<br>
Y          157       Pushed value<br>
X          157       Last value is copied to the X register<br>

(4) Notice the X register has the same value as the Y register. This is done for convenience since you may want to do something with the value in both registers. However, if you now press any digit key the new number will automatically replace the X register contents.

(5) Press the following digit keys: 90.5 This will enter a new value in the X register. The memory stack looks as follows:

T          0          Oldest register<br>
Z          0<br>
Y          157       Pushed value<br>
X          90.5\_    New value is being entered here<br>

(6) Again, press ENTER to force a stack push so that a new number can be entered.

T          0          Oldest register<br>
Z          157       Pushed value<br>
Y          90.5      Pushed value<br>
X          90.5      Room for new number<br>

(7) Press the following digit keys: 3.14 This will enter a new value in the X register. The memory stack looks as follows:

T          0          Oldest register<br>
Z          157<br>
Y          90.5<br>
X          3.14\_    New value is being entered here<br>

(8) Again, press ENTER to force a stack push so that a new number can be entered.

T          157       Oldest register<br>
Z          90.5<br>
Y          3.14<br>
X          3.14      Room for new number<br>

(9) Press the following digit keys: 50 This will enter a new value in the X register. The memory stack looks as follows:

T          157       Oldest register<br>
Z          90.5<br>
Y          3.14<br>
X          50\_       New value is being entered here<br>

(10) You have now filled the stack with chosen numbers. New numbers can be entered and the stack will continue to behave as you have seen here except that the contents of the T register will be pushed off the stack and are lost. Only the last four values can ever be shown and used.


# Number Entry

Excalibur uses a traditional 4-level stack for mathematical operations. Numbers are entered directly into the X register of the calculator. Using ENTER, older numbers are pushed up the stack to make room for a new number in the X register. To enter a number, simply start pressing numeric keys. When you have finished entering the number, you can press an operator to perform some action (addition, multiplication, etc) or you can press ENTER to push this number up the stack (to the Y register) and start to enter a new number into the X register.  The use of the stack will become more apparent in the RPN Quick Start section (but please read the rest of this section so you are familiar with how to enter numbers into Excalibur)

**Exponents**

To enter a number with an exponent you must first enter the base (mantissa) part of the number. Then press the E/EEX key to insert the "e+" exponent. Then type the exponent. For example, to enter 6.02e+23 you would enter the 6.02 first, then press the EEX key, then enter 23.

**Change Sign**

To change of a number in the X register, simply press the CHS (change sign) key. To change the sign of an exponent you must do it as you enter the number. After you hit the EEX key and before you finish entering the exponent, hit the CHS key to change the sign of the exponent. For example, to enter 1.34e-12 you would first enter in 1.34 then press EEX. Then you could press CHS and 12 to get the 1.34e-12 value to show in the X register.

**Fractions**

Fractions can be entered in two ways. The most common way is to use the Frac key to separate the components of the fraction. For example, to enter 5 and 3/8 you would enter 5, hit the Frac key, then hit 3 and hit the Frac key again and finally enter 8. This will lead to the fractional display of 5~3/8

You can also use the decimal point to enter fractions. To enter the same 5 and 3/8 using only the decimal point, you enter 5 then hit the decimal point to get "5." on the display. Then enter the 3 and press the decimal point again and finally enter the 8. The display will, once again, read 5~3/8.

Once a fraction is entered, pressing ENTER or any numeric operation key will automatically convert the fraction to its decimal equivalent.

Let's see how work with these numbers on the stack.

# Addition, Subtraction, etc.

Basic math operations (Add, Subtract, Multiply and Divide) in an RPN calculator (like Excalibur) follows easy-to-understand rules and is unambiguous. First you need to get the two numbers you want on the stack. See the Basics section if you don't already know how to do this.

Lets try Addition. To add 13.3 and 45.3 we must first get those numbers on the stack.

T          0<br>
Z          0<br>
Y          45.3<br>
X          13.3<br>

We simply hit the + key on the keypad (or click the + key in the Excalibur window). This will add the contents of the X and Y register. The contents of the X and Y register will be popped off the stack (shifting T and Z down) for the addition. The result of the addition will be pushed back on the stack. After the addition operation, the stack will look as follows:

T          0<br>
Z          0<br>
Y          0<br>
X          58.6<br>

To illustrate this point further, if the stack looked as follows:

T          30<br>
Z          20<br>
Y          10<br>
X          5<br>

When addition is performed, the stack will look as follows:

T          30<br>
Z          30<br>
Y          20<br>
X          15<br>

As you can see, the X and Y registers are popped off the stack, the T and Z registers are shifted down (with the T register repeating to fill in the back end) and after the addition, the result of 15 is pushed back on the stack. The pushing and popping to and from the stack is an important thing to remember about RPN calculators. It can let you work out complex equations in little or no time.

Subtraction is exactly the same as addition. X is subtracted from Y and the result pushed back on the stack.

Multiplication will multiply X and Y and push the result on the stack.

Division will divide X into Y and push the result on the stack.

Always remember, it's just like working out the equation on paper. If you wanted to divide 8 by 2 you would write down 8. Under that you would write down 2 and you would divide them. Do the same with any RPN calculator. Enter 8, enter 2 and hit the Divide key. This is the basis for any calculations you will ever work out.

Some operations require two numbers (such as Addition, Multiplication, Subtraction and Division). For these, you enter the two numbers in the X and Y registers. Some operations are unary and only require a single number (such as Change Sign and Log) and for these only the X value is needed. There are even some built in formulas which require 3 or 4 numbers in which case Z and T come into play.

The RPN calculator makes quick work of long chain calculations. The trick is to remember what's on the stack (Excalibur makes it easy since you can see all four stack locations on screen). The trick with long calculations is to work from the innermost set of parenthesis out and knowing the precedence of operations (multiply and divide before addition and subtraction).

For example, the equation 45.2 + (6 / 2.98) \* 44.33

You would enter 6, enter 2.98 and press the Divide key. Enter 44.33 (which pushes the result of the previous divide up to the Y register) and press the multiply key. Once again, the result is in the X register. Enter 45.2 and press the plus key. The result is the final result of the equation above.

# Programming Basics

Programming is one of the most powerful features of Excalibur. I was worried that some people would be turned off by the use of the word “programming” and considered calling the feature “Macros” instead since the programming of Excalibur can really be thought of as recording a series of keystrokes for automated rapid playback. That is, any task that you can do by hand with Excalibur can be recorded for instant playback. But the programming mode allows much more flexibility – counters and loops and subroutines (none of which are needed to enjoy quite a bit of power with the programming mode – you can stick to just recording keystrokes for rapid playback). In the end I left this as “Programming Mode” but don’t let that deter you from some real under-the-hood power of Excalibur.

Recording programs is very easy. On the primary keypad you will see a button labeled REC and one labeled PLAY. The REC key puts the calculator into program record mode. A "REC" indicator will appear in the status bar near the bottom to indicate this and the stack will change and will show normal X, Y but the Z, T will now show the current program line and function. It is important to note that when you are in Programming REC mode, the keystrokes are being recorded but also the stack is still "live" and you are actually performing the calculations you are entering. I feel this is superior to the more traditional method of only showing the user the key values as they are pressed but not actually taking action on them until the program is run. In this way, you can feel your way though the programming effort and end up with less bugs in your program sequence. Every calculator key pressed (clicked) is stored in sequence until REC is hit again which will turn off the record feature. Pressing PLAY will then cause the key sequence recorded to playback in rapid succession. So, for example, you could enter the following key sequence:

REC (starts recording)<br>
9<br>
\*<br>
REC (ends recording)<br>

which would effectively multiply the X value by 9 every time PLAY is hit. Give it a try… enter a number into the X register and hit PLAY.

Several hundred Excalibur keys can be recorded in this way. Once recorded, the program will stay with the calculator (even between sessions – it’s saved to disk) until a new REC takes place. If you want to save your program more permanently, simply use the "Program Manager" menu selection from the File menu. The program manager allows you to store up to 200 programs – each of which can be up to 300 steps long. That’s a ton of programming power! Let’s look at the various programming techniques in a bit more detail.

**Keystroke Programming**

Keystroke programming is the most simple form of programming and the most common. Most of the users who ever venture into programming will only venture as far as Keystroke programming - and with good cause since it will allow you to do countless useful programs of your own choosing. Keystroke programming does not use branching, loops or subroutines. Only repetitive keystrokes are recorded for playback. Using only this method, user-defined constants, conversions and straight-line programs can be created and used with little effort. Let's take an example of keystroke programming. Suppose you wish to compute the circumference of a circle given the radius in X (I know this already exists as a built in Excalibur function but it makes a simple and nice example). First, start by pressing REC to turn the programming mode on. You will see at the top of the display the program counter set at 000 (start of program). Starting in the Scientific bank, enter in the following program simply by pressing the keys in the order shown:

001 - X Squared<br>
002 - PI<br>
003 - Multiply<br>
004 - \<End Of Program\><br>
<br>
Checksum: 0279 (you can see the program checksum from the File/Program Manager dialog box)<br>

You have now entered the sequence of keystrokes to produce the area of a circle given the radius in X. Now enter a radius into X and press Play - you will see the answer displayed in X. A nearly infinite number of similar programs can be created in this manner. The Program Manager (on the File menu) allows you to see the entire program at a glance along with the checksum. This checksum is useful to determine if you’ve keyed in the program correctly (it is also used when programs are copied/pasted to and from the clipboard – they ensure that your program transferred correctly).

**Labels, Branching and Conditionals**

Normally the program execution flows from the beginning of the program to the end one step at a time. Labels and branching allows the programmer to skip lines either forwards or backwards. This is done with a label and a corresponding goto. A series of backwards gotos is called a loop (normally terminated using conditions on the Program bank layout). Here is a simple program which demonstrates labels, branching and conditionals. It takes a value in X and if the value is greater than 200 it will divide it by 3 otherwise it will multiply it by 6.

001 - Digit 2<br>
002 - Digit 0<br>
003 - Digit 0<br>
004 - Exchange X and Y<br>
005 - X <= Y?<br>
006 - Goto A<br>
007 - Goto B<br>
008 - Label A<br>
009 - Digit 6<br>
010 - Multiply<br>
011 - Halt Program<br>
012 - Label B<br>
013 - Digit 3<br>
014 - Divide<br>
015 - Halt Program<br>
016 - \<End Of Program\><br>
<br>
Checksum: 0EFA<br>

The conditional you see above (X<=Y?) is typical of many of the programming keys (found on the Program I and Program II banks). This particular command will compare X and Y… if X is less than or equal to Y the next statement is executed (the condition is TRUE). If X is greater than Y the next statement is skipped (condition is FALSE). Therefore, the code will either execute Goto A (if X is less than or equal to Y) or it will execute Goto B (if X is greater than Y).

There are 9 different labels that can be used with Excalibur labeled A-J (letter ‘I’ is notably absent… we use a similar symbol (i) for indirect jumps and so we avoid confusion by not allowing the letter ‘I’ to be used for a label).

**Looping**

Looping is simply a branch to an earlier instruction that is predicated on the outcome of a conditional test. We loop for several reasons - but mainly to count or sum a series of numbers or calculations. Here is a program to compute a simple summation sequence of 0+1+2+3+4...X. If you enter and X of 6 you should get 0+1+2+3+4+5+6 for a total of 21. The program looks as follows:

001 - Digit 0<br>
002 - Store R0<br>
003 - Drop Stack<br>
004 - Label A<br>
005 - Enter<br>
006 - Enter<br>
007 - Recall R0<br>
008 - Plus<br>
009 - Store R0<br>
010 - Drop Stack<br>
011 - Digit 1<br>
012 - Minus<br>
013 - X > 0?<br>
014 - Goto A<br>
015 - Clear Stack<br>
016 - Recall R0<br>
017 - \<End Of Program\><br>
<br>
Checksum: 0A6F<br>

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
<br>
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

One of the most powerful programming techniques is the use of indirect addressing. Indirect addressing allows variable branch labels and register access. To indirectly address one of the 26 storage registers, use the StoXth and RclXth keys.  These use the X register as the index to determine which storage register should be used. For example, entering 1 in X and pressing StoXth will store the current Y value to the first register (the A register).  Using StoXth and RclXth all 26 registers can be addressed with 1=A, 2=B … 26=Z.  StoXth and RclXth both drop the stack so the index value is no longer on the stack at the end of the operation.  For example, if you wanted to store the value 123 to the Z register, key in the following sequence:

123<br>
ENTER<br>
26<br>
StoXth<br>

This will drop the stack and use 26 as the index to the register that should be used to STO the 123 value from the stack. You can use the Program Debugger or the normal STO key on the main keypad to verify the value was saved properly. To save time and keystrokes the first half dozen registers can be stored and recalled with buttons located on Program Bank II.

Perhaps of greater importance to programmers using Excalibur, labels can also be indirectly addressed. Excalibur uses a special Indirect Register called 'i' which is an integer based internal register. To store a number into the indirect register use the "Sto i" key. To recall the indirect register to the stack, use the "Rcl i" key. The number in the Indirect Register represents a program label with 0=label A, 1=label B, 9=label J.  Using the Goto(i) or Gsb(i) keys will jump to the label indicaetd by the value of the Indirect register.

**Easier Looping**

Let’s revisit the algorithm for summing numbers from 1 to X. Here we use the Loop key which automatically decrements the associated register and will branch to the associated label if the counter is above zero.

001 - Store A<br>
002 - Digit 0<br>
003 - Store B<br>
004 - Drop Stack<br>
005 - Label A<br>
006 - Recall A<br>
007 - Store Plus B<br>
008 - Loop A<br>
009 - Clear Stack<br>
010 - Recall B<br>
011 - \<End Of Program\><br>
<br>
Checksum: 07AF<br>

**Recursive Subroutines**

Excalibur allows a subroutine to call itself. This can be extremely useful for recursive algorithms. Let's take the example of summing numbers from 1 to X.

001 - Digit 0<br>
002 - Store B<br>
003 - Drop Stack<br>
004 - Gosub A<br>
005 - Recall B<br>
006 - Halt Program<br>
007 - Label A<br>
008 - Store Plus B<br>
009 - Digit 1<br>
010 - Minus<br>
011 - X > 0?<br>
012 - Gosub A<br>
013 - Return<br>
014 - \<End Of Program\><br>
<br>
Checksum: 0ED4<br>

Excalibur allows recursion up to 1000 levels deep. If recursion goes deeper, an error message will be given to the user and the program will terminate. The example above isn’t the shortest or best example of recursion (the looping example that precedes this is shorter and does the same thing), but it does demonstrate the functionality.

**Excalibur Programming Speed**

Excalibur program steps run based on the speed of your computer. But, in practice it hardly matters. It will execute many thousands of steps per second on even the lowliest computer capable of running a 32-bit Windows operating system.  On my fairly modest PC it performs about 60,000 steps per second. I ran an experiment taking a simple 6 step program that incremented a counter, inverted the value and then took the SIN of the value and then looped back to do it again. I let a variety of calculators run the program for 30 seconds. What I found was:

HP15C ran 24 iterations of the program in 30 seconds.<br>
HP11C ran 27 iterations of the program in 30 seconds.<br>
HP25 ran 32 iterations of the program in 30 seconds.<br>
HP41C ran 51 iterations of the program in 30 seconds.<br>
HP32SII ran 237 iterations of the program in 30 seconds.<br>
Excalibur (modest i5) ran 57,335,221 iterations of the program in 30 seconds.<br>

Based on this simple experiment, I think it’s safe to say that Excalibur runs more than 1 million times faster than the early HP handhelds.

# Excalibur Settings

The File/Settings menu item brings up a dialog box to display common Excalibur settings. They are:

**Angle Mode:** Degrees, Radians, Gradients

**Tax Constant:** Constant for calculating Tax (see Business Key Layout)

**Numlock****:** You can have the numlock key active on entry or you can tell Excalibur not to change the numlock state.

**Comma Display:** On or off. Determines if you want commas shown on numbers greater than 1000.

**Key Settings:** For you die-hard HP fans you can have your choice of key labels for exponents and change sign.

**Comma Format:** Allows the user to swap decimal points and commas in the display. This is done for some international users who require the XXX.XXX,yy format instead of the XXX,XXX.yy format.

**Tool Tips:** Configures tool-tips on or off. By default (you can turn it on and off in the Settings dialog box) each function key outside of the typical numbers and ordinary arithmetic operations has a tool tip associated with it. If you rest the mouse cursor over a button, the tool tip will show in a small window below the mouse cursor. The tool tip will tell you a the full name of the key (the key names are often abbreviated to save space). If you want more help on how to use a particular key, you can click the Help button then click any button to get a better description for that key. As a shortcut, you can right-click on any key for additional information (saves having to hit the Help key first).

**Stack Options:**  This allows you to select the classic 4-level stack or the deeper 8-level stack (those stack values are off screen and are internally labeled as A, B, C and D). When the 8-level stack is chosen, some stack operations such as rotate up / rotate down and a few functions such as LCM and GCD still use the visible 4-level stack (which seemed most appropriate since these are operations carried out on the visible stack). All other operations use the 8-level stack, if chosen. In addition, traditionally the T register (in the 4-level stack) or the D register (in the 8-level stack) repeats when the stack is popped. You can override this default behavior by selecting "Zero Fill Pop" which will fill a zero at the back end of the stack on a pop operation. You can select "Show X Min" to show the current X value when the main Excalibur window is minimized to the task bar. You can select an RPL method of entry where the X register is not duplicated. Lastly, you can select “Clear On Exit” to clear the stack when Excalibur closes (normally all stack values are preserved from the last run… registers are always preserved until explicitly cleared).

**Pause/Trace Delay:** This controls the number of milliseconds (1000 milliseconds = 1 second) of delay between program trace steps (using FWD) and also how long the PAUSE command will wait. This is only useful if you are programming Excalibur.
