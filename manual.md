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

Now that you know how enter numbers on the stack, it’s time to learn how to perform basic operations on those numbers. See the summary of Numeric Entry to see all the forms of numeric entry then move onto the Addition/Subtraction section of this help file to see how work with these numbers on the stack.
