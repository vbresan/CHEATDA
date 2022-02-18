/* CHEAT DATE (DOS) - zajebi shareware programe koji citaju system date */

#include <stdio.h>				// za FILE
#include <dos.h>				// za struct date
#include <process.h>				// za spawn

#define DEBUG

main()
 { 	 FILE *fp;	 	  // jedan file pointer za sve (samo jedna datoteka istodobno otvorena)
	 char ImeExeca[13];       // naziv zeljene datoteke za pokretanje
	 struct date Lazni,       //
		     Temp;        //


	 // otvori kofiguracijsku datoteku
	 // ako se pojavi greska
	 //	  izadji u DOS sa prigodnom porukom i error codom

	 if((fp=fopen("cheatdat.e","rt"))==NULL)
		{	fprintf(stderr,"\n\rCHEAT DATE Error: Unable to open configuration file for reading!\n\r");
			exit (3);
		}


	 // procitaj lazni datum i ime izvrsne datoteke
	 // ako se pojavi greska
	 // 	zatvori datoteku
	 // 	izadji u DOS sa prigodnom porukom i error codom		

	 if(fscanf(fp,"%i %i %i %s",&Lazni.da_year,&Lazni.da_day,&Lazni.da_mon,ImeExeca)!=4)
		{ fclose(fp);
			fprintf(stderr,"\n\rCHEAT DATE Error: Unable to read formatted data from configuration file!\n\r");
			exit (4);
		}
	 fclose(fp);              // zatvori konfiguracijsku datoteku

#ifdef DEBUG
 printf("Iz cfga: %i %i %i %s\n",Lazni.da_year,Lazni.da_day,Lazni.da_mon,ImeExeca);
#endif
	 

	 // kreiraj (otvori) privremenu datoteku 
	 // ako se pojavi greska
	 //		izadji u DOS sa prigodnom porukom i error codom

	 if((fp=fopen("$$$$temp.$$$","wt"))==NULL)
		{	fprintf(stderr,"\n\rCHEAT DATE Error: Unable to create temporary file!\n\r");
			exit (1);
		}
	 
	 getdate(&Temp);					// procitaj pravi system date
	 
#ifdef DEBUG
 printf("Danas: %i %i %i\n",Temp.da_year,Temp.da_day,Temp.da_mon);
#endif


	 // u privremenu datoteku snimi pravi datum
	 // ako se pojavi greska
	 // 	zatvori datoteku
	 // 	izbrisi datoteku
	 // 	izadji u DOS sa prigodnom porukom i error codom

	 if(fprintf(fp,"%i %i %i",Temp.da_year,Temp.da_day,Temp.da_mon)==EOF)
		{	fclose(fp);
			unlink("$$$$temp.$$$");
			fprintf(stderr,"\n\rCHEAT DATE Error: Unable to send formatted output to temporary file!\n\r");
			exit (2);
		}
	 fclose(fp);              // zatvori privremenu datoteku
	
	 setdate(&Lazni);         // postavi lazni system date


	 // pokreni izvrsnu datoteku
	 // ako se pojavi greska
	 //   izbrisi privremenu datoteku
	 //   izadji u DOS sa prigodnom porukom i error codom

	 if(spawnl(P_WAIT,ImeExeca,NULL)==-1)
		{ unlink("$$$$temp.$$$");
			perror("\n\rCHEAT DATE Error: Unable to execute a file");
			exit (5);
		}

	 getdate(&Temp);          // procitaj system date (lazni)
	 

   // izracunaj razliku promjene datuma

	 Lazni.da_year=Temp.da_year-Lazni.da_year;
	 Lazni.da_day=Temp.da_day-Lazni.da_day;
	 Lazni.da_mon=Temp.da_mon-Lazni.da_mon;
	 
	 
	 // otvori privremenu datoteku
	 // ako se pojavi greska
	 //   izbrisi pricremenu datoteku
	 //   izadji u DOS sa prigodnom porukom i error codom

	 if((fp=fopen("$$$$temp.$$$","rt"))==NULL)
		{ unlink("$$$$temp.$$$");
			fprintf(stderr,"\n\rCHEAT DATE Error: Unable to open temporary file for reading!\n\r");
			exit (6);
		}

	 // procitaj pravi datum iz privremene datoteke
	 // ako se pojavi greska
	 // 	zatvori privremenu datoteku 
	 // 	izbrisi privremenu datoteku
	 // 	izadji u DOS sa prigodnom porukom i error codom

	 if(fscanf(fp,"%i %i %i",&Temp.da_year,&Temp.da_day,&Temp.da_mon)!=3)
		{	fclose(fp);
			unlink("$$$$temp.$$$");
			fprintf(stderr,"\n\rCHEAT DATE Error: Unable to read formatted data from temporary file!\n\r");
			exit (7);
		}
	 fclose(fp);              // zatvori privremenu datoteku
	 unlink("$$$$temp.$$$");  // izbrisi privremenu datoteku

#ifdef DEBUG
 printf("Pravi procitani: %i %i %i\n",Temp.da_year,Temp.da_day,Temp.da_mon);
#endif

	 // dodaj razliku na pravi datum

	 Temp.da_year+=Lazni.da_year;
	 Temp.da_day+=Lazni.da_day;
	 Temp.da_mon+=Lazni.da_mon;

#ifdef DEBUG
 printf("Pravi s razlikom: %i %i %i\n",Temp.da_year,Temp.da_day,Temp.da_mon);
#endif

	 setdate(&Temp);          // postavi pravi datum
	 return (0);			  // vrati DOSu 0
 }
