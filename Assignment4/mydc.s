### Name: Melese Medhin
### ID: 20210727
### file: mydc.s
### Assignment 4


### --------------------------------------------------------------------
### mydc.s
###
### Desk Calculator (dc)
### --------------------------------------------------------------------

	.equ   ARRAYSIZE, 20
	.equ   EOF, -1
	.equ   TRUE, 1
	.equ   FALSE, 0
	.equ   NULL, 0

	
		.section ".rodata"

scanfFormat:
	.asciz "%s"

outputmessage:
	.asciz "%d\n"

errormessage:
	.asciz "dc: stack empty\n"


### --------------------------------------------------------------------

        .section ".data"

### --------------------------------------------------------------------

        .section ".bss"
buffer:
        .skip  ARRAYSIZE

### --------------------------------------------------------------------

	.section ".text"

	## -------------------------------------------------------------
	## int main(void)
	## Runs desk calculator program.  Returns 0.
	## -------------------------------------------------------------

	.globl  main
	.type   main,@function

main:

	pushl   %ebp
	movl    %esp, %ebp

	## seeds the random number generator
	## used by the function rand
	## call srand at the beginning of the main

	pushl $NULL
	call  time
	addl  $4, %esp
	pushl %eax
	call  srand
	add   $4, %esp


##1 while (1) {
input:

	## dc number stack initialized. %esp = %ebp
	
	## scanf("%s", buffer)
	pushl	$buffer
	pushl	$scanfFormat
	call    scanf
	addl    $8, %esp

	## check if user input EOF
	##2 if (scanf("%s", buffer) == EOF)
	##3 	return 0;
	cmp	$EOF, %eax
	je	quit

	##33 	else { /* the first no. is a digit */
	movsbl  buffer, %ebx
	push    %ebx
	call    isdigit
	add     $4, %esp

	cmp     $TRUE, %eax
	je      isDigit 

	##4 	if (!isdigit(buffer[0])) { 

	cmp  $'_', %ebx
	je   negative
	cmp  $'p', %ebx
	je   peek
	cmp  $'-', %ebx
	je   minus
	cmp  $'^', %ebx
	je   power
	cmp  $'+', %ebx
	je   plus
	cmp  $'*', %ebx
	je   multiply
	cmp  $'/', %ebx
	je   divide
	cmp  $'%', %ebx
	je   modulo
	cmp  $'q', %ebx
	je   quit

	## Part 2: Advanced functions
	cmp  $'f', %ebx
	je   prLifo
	cmp  $'c', %ebx
	je   clear
	cmp  $'d', %ebx
	je   duplicate
	cmp  $'r', %ebx
	je   reverse
	cmp  $'x', %ebx
	je   randGenerate
	cmp  $'y', %ebx
	je   prime




##===============================================

## if the input is an integer character
## it changes the string to int 
isDigit:
	
	##34	int no = atoi(buffer);
	##35	stack.push(no);	/* pushl some register value */
	push $buffer 
	call atoi
	addl $4, %esp
	push %eax
	jmp input

##===============================================
## if the input is a negative integer character 
## it changes the string to int and pushs on
## the stack 

isDigitN:
	
	##34	int no = atoi(buffer);
	##35	stack.push(no);	/* pushl some register value */
	push %esi 
	call atoi
	addl $4, %esp
	imull $-1,%eax
	push %eax
	jmp input


##==============================================

## used to input a negative integer specified 
## by a _ at the beginning
## it first checks if the character followd by
## _ is a digit or not. If it isn't it jumps
## input. Otherwise, it changes it to digit,
## multiplies by -1 and push's in the stack.
negative:

	leal buffer,%esi
	incl %esi
	movb   (%esi),%cl
	movsbl %cl,%ebx
	pushl  %ebx
    call   isdigit
    addl   $4,%esp
    cmpl   $FALSE,%eax
    jne    isDigitN
    jmp    input


##===============================================

## print the value that sits on the top of the stack

peek:

	## if (stack.peek() == NULL) { /* is %esp == %ebp? */
	##     printf("dc: stack empty\n");}
	cmpl %esp, %ebp
	je   errPrint1

	## else {
	##printf("%d\n", (int)stack.top()); 
		##/* value is already pushed in the stack */}
	movl  (%esp), %ebx
	pushl %ebx
	pushl $outputmessage
	call  printf
	addl  $8, %esp
	jmp   input


##===============================================
plus:
	##else if (buffer[0] == '+') {
	##	int a, b;
	##	if (stack.peek() == NULL) {
	##		printf("dc: stack empty\n");
	##		continue;
	##		}
	##	a = (int)stack.pop();}

	cmpl %esp, %ebp
	je    errPrint1
	popl   %ebx

	##if (stack.peek() == NULL) {
	##	printf("dc: stack empty\n");
	##	stack.push(a); /* pushl some register value */
	##	continue;
	##}
	##b = (int)stack.pop(); /* popl to some register */

	cmpl %esp, %ebp
	je    errPrint2
	popl  %eax

	##res = a + b;
	##stack.push(res);

	addl  %ebx, %eax
	pushl %eax
	jmp   input

##===============================================

## the only difference from plus is subl used 
## insteas of addl
minus:

	cmpl %esp, %ebp
	je    errPrint1
	popl  %ebx

	cmpl %esp, %ebp
	je    errPrint2
	popl  %eax

	subl  %ebx, %eax
	pushl %eax
	jmp   input

##===============================================

## the only difference from plus is imull used 
## instead of addl

multiply:
	
	cmpl %esp, %ebp
	je    errPrint1
	popl  %ebx

	cmpl %esp, %ebp
	je    errPrint2
	popl  %eax

	imull %ebx, %eax
	pushl %eax
	jmp   input

##===============================================

## similar to the above labels except thay
## it uses a idivl arthmetic.'idivl' divides EDX:EAX 
## by its operand and puts the quotient in EAX and 
## the remainder in EDX. "divide:" pushs
## the quotient stored in eax register to the stack

divide:

	cmpl  %esp, %ebp
	je    errPrint1
	popl   %ebx

	cmpl  %esp, %ebp
	je    errPrint2
	popl  %eax

	cltd
	idivl  %ebx
	pushl  %eax
	jmp input


##===============================================
## similar to the above label(divide) except thay
## it pushs the remainder stored in edx register
## to the stack

modulo:

	cmpl %esp, %ebp
	je    errPrint1
	popl  %ebx

	cmpl %esp, %ebp
	je    errPrint2
	popl  %eax

	cltd
	idivl  %ebx
	pushl %edx
	jmp input


##===============================================

##
##
##
##

power:
	
	cmpl  %esp, %ebp
	je    errPrint1
	popl  %ebx

	## if exp < 0
	cmpl $0, %ebx
	jl   pcase1

	cmpl  %esp, %ebp
	je    errPrint2
	popl  %eax

	movl  $1, %esi
	movl  $1, %edi
	jmp  ploop

pcase1:
	
	pushl %ebx
	jmp input

ploop:

	cmpl  %ebx,%esi
	jg    pendloop

	imull %eax,%edi
	incl  %esi
	jmp ploop

pendloop:
	
	pushl %edi
	jmp input


##=============================================== 
## prints in LIFO order (prLifo + loopLifo)
prLifo:
	
	cmpl %esp, %ebp
	je   input

	# saves the top stack
	# address to traverse it
	movl %esp, %ebx
	jmp  loopLifo

##  prints the value at %ebx and
##  updates it to next one until
##  it becomes equal to base pointer
loopLifo:

	pushl (%ebx)
	pushl $outputmessage
	call  printf
	addl  $8, %esp
	addl  $4, %ebx

	cmpl %ebx, %ebp
	je   input

	jmp loopLifo


##===============================================

## checks if top and bottom of stack are
## the same. If yes, that means the stack
## is empty and prompts for input. Otherwise
## updates top of the stack and repeats again.
clear:

	cmpl %esp, %ebp
	je   input

	addl $4, %esp
	jmp clear


##===============================================

## checks if top and bottom of stack are
## the same. If yes, that means the stack
## is empty and prompts for input. Otherwise
## duplicates top of the stack and repeats again.

duplicate:
	

	cmpl %esp, %ebp
	je   errPrint1

	## pushs the top of the stack on itself.
	push (%esp)
	jmp   input
##===============================================

## checks if top and bottom of stack are
## the same. If yes, that means the stack
## is empty and prompts for input. Otherwise
## pops the top two elemens of the stack 
## and and pushs hhem back in reverse order.
reverse:

	cmpl %esp, %ebp
	je    errPrint1
	popl  %ebx

	cmpl %esp, %ebp
	je    errPrint2
	popl  %eax

	pushl  %ebx
	push   %eax

	jmp input


##===============================================
    .equ RAND_MAX,  1024


## Generates a non_negative random integer smaller 
## than RAND_MAX and pushes it into the stack. 
## It does so by using a rand function which generates
## random numeber and doign modulus operator with
## RAND_MAX. rand is seeded with time everytime the main 
## function is called

randGenerate:
	
	call  rand
	movl  $RAND_MAX, %ebx

	cltd
	idivl %ebx
	pushl %edx
	jmp   input


##===============================================

## Finds the biggest prime number lesser or 
## equal to the last stack element
## and pushes it in the stack
## if it exists.
	## prime: + trLoop: + ispLoop: + 
	## anTrLoop: + back: + pp

# "prime:" label sets the last-element
prime:
	
	cmpl %esp, %ebp
	je   errPrint1

	movl (%esp), %ebx
	incl %ebx
	jmp  trLoop

# traverses down from the last element
# to find a prime number
trLoop:
	
	decl %ebx
	jmp  ispLoop


# checks if the numbers are prime
ispLoop:
	
	movl  $2, %edi
	movl  $0, %ecx

	cmpl  %edi, %ebx
	jl    input 
	je    pp   ##for 2

	movl  %ebx, %eax
	cltd
	idivl %edi
	
	cmpl  %ecx, %edx
	je    trLoop

	movl  $3, %esi
	jmp   anTrLoop

## loop to find if the number is prime
anTrLoop:

	movl  %ebx, %eax
	cltd
	idivl %edi
	cmpl  %eax,%esi
	jge   pp

	movl  %ebx, %eax
	cltd
	idivl %esi
	cmpl  %ecx, %edx
	jne   back 

	jmp   trLoop
	
back:

  	incl  %esi
  	incl  %esi
  	jmp anTrLoop

pp:
	pushl %ebx
	jmp   input

##===============================================

## The two labels below print error message if 
## the stacks are emptpy while poping
## In addition "errPrint2:" pushs back the earlier
## pop'es element
errPrint1:
	
	pushl $errormessage
	call  printf
	addl  $4, %esp
	jmp   input

errPrint2:
	
	pushl %ebx
	pushl $errormessage
	call  printf
	addl  $4, %esp
	jmp   input

##===============================================
quit:	
	## return 0
	movl   $0, %eax
	movl   %ebp, %esp
	popl   %ebp
	ret
