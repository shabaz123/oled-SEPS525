all :
	gcc -c BBBiolib.c
	ar -rs libBBBio.a BBBiolib.o
	gcc -c vfont.c -o vfont.o
	gcc -c color.c -o color.o
	gcc -c oled.c -o oled.o
	gcc vfont.o color.o oled.o -L. -lBBBio -lm -o oled
clean :
	rm -rf *.o libBBBio.a oled.o color.o vfont.o oled
