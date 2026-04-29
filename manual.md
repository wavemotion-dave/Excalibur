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
