; assign.j 
.class public assign 
.super java/lang/Object
.field public static ga I
.field public static gr F
.method public static main([Ljava/lang/String;)V
.limit stack 100;
.limit locals 100 
new java/util/Scanner 
dup 
getstatic java/lang/System/in Ljava/io/InputStream; 
invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V 
putstatic hello/_sc Ljava/util/Scanner; 
 ; invoke java.util.Scanner.nextXXX() 
getstatic assign/_sc Ljava/util/Scanner; 
invokevirtual java/util/Scanner/nextInt()I
istore 1
; invoke java.util.Scanner.nextXXX() 
getstatic assign/_sc Ljava/util/Scanner; 
invokevirtual java/util/Scanner/nextInt()I
istore 2
; invoke java.util.Scanner.nextXXX() 
getstatic assign/_sc Ljava/util/Scanner; 
invokevirtual java/util/Scanner/nextInt()I
istore 3
; invoke java.util.Scanner.nextXXX() 
getstatic assign/_sc Ljava/util/Scanner; 
invokevirtual java/util/Scanner/nextFloat()F 
fstore 5
; invoke java.util.Scanner.nextXXX() 
getstatic assign/_sc Ljava/util/Scanner; 
invokevirtual java/util/Scanner/nextFloat()F 
fstore 6
getstatic java/lang/System/out Ljava/io/PrintStream; 
iload 3
invokevirtual java/io/PrintStream/print(I)V 
getstatic java/lang/System/out Ljava/io/PrintStream; 
ldc " "
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V 
getstatic java/lang/System/out Ljava/io/PrintStream; 
iload 1
invokevirtual java/io/PrintStream/print(I)V 
getstatic java/lang/System/out Ljava/io/PrintStream; 
ldc " "
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V 
getstatic java/lang/System/out Ljava/io/PrintStream; 
iload 2
invokevirtual java/io/PrintStream/print(I)V 
getstatic java/lang/System/out Ljava/io/PrintStream; 
ldc " "
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V 
getstatic java/lang/System/out Ljava/io/PrintStream; 
fload 5
invokevirtual java/io/PrintStream/print(F)V 
getstatic java/lang/System/out Ljava/io/PrintStream; 
ldc " "
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V 
getstatic java/lang/System/out Ljava/io/PrintStream; 
fload 6
invokevirtual java/io/PrintStream/print(F)V 
getstatic java/lang/System/out Ljava/io/PrintStream; 
ldc "\n"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V 
iload 2
putstatic assign/ga I
iload 3
istore 2
getstatic assign/ga I
istore 3
getstatic java/lang/System/out Ljava/io/PrintStream; 
iload 2
invokevirtual java/io/PrintStream/print(I)V 
getstatic java/lang/System/out Ljava/io/PrintStream; 
ldc " "
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V 
getstatic java/lang/System/out Ljava/io/PrintStream; 
iload 3
invokevirtual java/io/PrintStream/print(I)V 
getstatic java/lang/System/out Ljava/io/PrintStream; 
ldc "\n"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V 
fload 6
putstatic assign/gr F
fload 5
fstore 6
getstatic assign/gr F
fstore 5
getstatic java/lang/System/out Ljava/io/PrintStream; 
fload 5
invokevirtual java/io/PrintStream/print(F)V 
getstatic java/lang/System/out Ljava/io/PrintStream; 
ldc " "
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V 
getstatic java/lang/System/out Ljava/io/PrintStream; 
fload 6
invokevirtual java/io/PrintStream/print(F)V 
getstatic java/lang/System/out Ljava/io/PrintStream; 
ldc "\n"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V 
iconst_1
istore 4
getstatic java/lang/System/out Ljava/io/PrintStream; 
iload 4
invokevirtual java/io/PrintStream/print(Z)V 
getstatic java/lang/System/out Ljava/io/PrintStream; 
ldc "\n"
invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V 
return 
.end method