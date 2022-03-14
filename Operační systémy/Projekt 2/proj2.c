/***********************************************/
/* 	IOS  projekt 2                  			*/
/*  autor : Kristian Kralovic <xkralo05>		*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include <unistd.h>
#include <sys/wait.h>

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>

#define No_judge_name "/xkralo05.ios.projekt2.nojudge"
#define write_name "/xkralo05.ios.projekt2.writing"
#define vsetci_zapisany_name "/xkralo05.ios.projekt2.vsetci_zapisany"
#define potvrdenie_name "/xkralo05.ios.projekt2.potvrdenie"
#define writing2_name "/xkralo05.ios.projekt2.writing2"


// deklaracia funkcii
void zdielana_pamet_delete();
void semafory_delete();
void print_error();
void argumenty(char *argv[]);
void subor_otvorenie();
void immigrant_generator(int PI, int IG);
void judge_proces();
void immigrant_proces();
int generator_cisel(int IG);
int semafory_init();
int zdielana_pamet();

// globalne premenne 
int PI=0,IG=0,JG=0,IT=0,JT=0;

int imm_vstupilo=0,imm_id=1;
//semafory
sem_t *nojudge =NULL;
sem_t *writing=NULL;
sem_t *vsetci_zapisany=NULL;
sem_t *potvrdenie=NULL;
sem_t *writing2=NULL;
// zdielana pamet

int *p_c_ulohy=NULL, c_ulohy=0;
int *p_vstupili=NULL, vstupili=0;
int *p_zapisali=NULL, zapisali=0;
int *p_vsetci_imm=NULL, vsetci=0;
int *p_vybaveny=NULL, vybaveny=0;
int *p_sudca=NULL, sudca=0;


FILE *subor=NULL;
//----------------------------------------------------------------------------
//funkcia vypise chybovu hlasku + navod na pouzivanie argumentov
void print_arg_error()
{
	fputs("Zle pouzivanie argumentov , ocakavane spustenie ./proj2 PI IG JG IT JT  \n",stderr);
	fputs("PI -> pocet procesov vygenerovanych v kategorii pristahovalcov\n",stderr);
	fputs("PI >= 1 \n",stderr);
	fputs("IG -> maximalna doba v milisekundach , po ktorej je generovany novy proces immigrant\n",stderr);
	fputs("IG >=0 && IG <= 2000\n",stderr);
	fputs("JG -> maximalna doba v milisekundach , po ktorej sudca znova vstupi do budovy\n",stderr);
	fputs("JG >= 0 && JG <= 2000\n",stderr);
	fputs("IT -> maximalna doba v milisekundach , ktora simuluje trvanie vyzdvihnuta certifikatu\n",stderr);
	fputs("IT >= 0 && IT <= 2000\n",stderr);
	fputs("JT -> maximalna doba v milisekundach , ktora simuluje vydavanie rozhodnutia sudcom\n",stderr);
	fputs("JT >=0 && JT <=2000\n",stderr);
	fputs("Vsetky parametry musia byt cele cisla \n",stderr);
	exit(1);
}
//----------------------------------------------------------------------------
void argumenty(char *argv[])
{
   char *badarg = NULL;
	
	// kontrola argumentu c.1 = PI
	if(!isdigit(*argv[1]))
	{
		print_arg_error();
	}
	else
	{
	 	PI=strtol(argv[1], &badarg,10);
	 	if ( (PI<1 ) || (*badarg))
	 	{
	 		print_arg_error();
	 	}
	}
	// kontrola  argumentu c.2 = IG
	if(!isdigit(*argv[2]))
	{
		print_arg_error();
	}
	else
	{
	 	IG=strtol(argv[2], &badarg,10);
	 	if ( (IG<0 ) ||(IG>2000) || (*badarg))
	 	{
	 		print_arg_error();
	 	}
	}
	// kontrola argumentu c.3 = JG
	if(!isdigit(*argv[3]))
	{
		print_arg_error();
	}
	else
	{
 		JG=strtol(argv[3], &badarg,10);
	 	if ( (JG<0 ) ||(JG>2000) || (*badarg))
	 	{
	 		print_arg_error();
	 	}
	}
	// kontrola argumentu c.4 = IT
	if(!isdigit(*argv[4]))
	{
		print_arg_error();
	}
	else
	{
	 	IT=strtol(argv[4], &badarg,10);
	 	if ( (IT<0 ) ||(IT>2000) || (*badarg))
	 	{
	 		print_arg_error();
	 	}
	}
	//kontrola argumentu c.5 = JT
	if(!isdigit(*argv[5]))
	{
		print_arg_error();
	}
	else
	{
	 	JT=strtol(argv[5], &badarg,10);
	 	if ( (JT<0 ) ||(JT>2000) || (*badarg))
	 	{
	 		print_arg_error();
	 	}
	}
}
//----------------------------------------------------------------------------
// funkcia otvori subor proj2.out pre zapis
void subor_otvorenie()
{
	subor=fopen("proj2.out", "w+");
	if (subor==NULL)
	{
		fputs("Chyba : subor 'proj2.out' sa nepodarilo otvorit ",stderr);
		exit(1);
	}
	
}
//----------------------------------------------------------------------------
// funkcia vygeneruje procesy imigrantov
void immigrant_generator(int PI,int IG)
{

	int status=0;

	pid_t immigrant = 0;
	
	for(int j=0;j<PI;j++)		
	{ 	
		pid_t immigrant=fork();
		if(immigrant==0)
		{
			imm_id=j+1;
			immigrant_proces();
			break;
		}
		if(immigrant==-1)
		{
			semafory_delete();
			zdielana_pamet_delete();
			fputs("Chyba : pri vytvarani procesov",stderr);
			exit(0);
		}
	usleep(generator_cisel(IG));	
	}	

while ((immigrant=waitpid(-1,&status,0))!=-1){
}
exit(0);
}
//----------------------------------------------------------------------------
// funkcia procesu judge
void judge_proces()
{
	while(*p_vybaveny<PI)
	{ 
		
	usleep(generator_cisel(JG));
	sem_wait(nojudge); // zablokovanie semaforu nojudge
	sem_wait(writing); // zaciatok zapisu sudca;
	fprintf(subor, "%d\t: JUDGE \t : wants to enter \n",++*p_c_ulohy );
	fprintf(subor, "%d\t: JUDGE \t : enters:\t\t %d\t: %d\t: %d\t \n",++*p_c_ulohy,*p_vstupili,*p_zapisali,*p_vsetci_imm);
	(*p_sudca)++;
	if(*p_vstupili >*p_zapisali)
	{
		fprintf(subor, "%d\t: JUDGE \t : waits for imm:\t %d\t: %d\t: %d\t \n",++*p_c_ulohy,*p_vstupili,*p_zapisali,*p_vsetci_imm);
		sem_post(writing); // posle mutex immigrantom nech sa mozu checknut
		sem_wait(vsetci_zapisany); // caka az sa vsetci zapisu a potom moze ist dalej , ak niesu este vsetci zapisany tak stoji
	}
	fprintf(subor, "%d\t: JUDGE \t : starts confirmation:\t %d\t: %d\t: %d\t \n",++*p_c_ulohy,*p_vstupili,*p_zapisali,*p_vsetci_imm);
	usleep(generator_cisel(JT));
	int help_counter=*p_zapisali;
	for(int q=0;q<help_counter;q++)
	{
		(*p_vybaveny)++;
		(*p_vstupili)--;
		(*p_zapisali)--;	
	}
	fprintf(subor, "%d\t: JUDGE \t : ends confirmation:\t %d\t: %d\t: %d\t \n",++*p_c_ulohy,*p_vstupili,*p_zapisali,*p_vsetci_imm);
	sem_post(potvrdenie);
	usleep(generator_cisel(JT));
	fprintf(subor, "%d\t: JUDGE \t : leaves:\t\t %d\t: %d\t: %d\t \n",++*p_c_ulohy,*p_vstupili,*p_zapisali,*p_vsetci_imm);
	sem_post(writing);// koniec semaforu writing ->testovaci
	sem_post(nojudge); // odvlokovanie semaforu nojudge
	(*p_sudca)--;
	sem_wait(potvrdenie);
   	}
    sem_wait(writing);
    fprintf(subor, "%d\t: JUDGE \t : finishes \n",++*p_c_ulohy );
    sem_post(writing);
    
	exit(0);
}
//----------------------------------------------------------------------------
void immigrant_proces()
{	
	setbuf(subor, NULL);
	sem_wait(writing); //zaciatok semaforu pre zapis starts
	fprintf(subor, "%d\t: IMM %d \t : starts \n",++*p_c_ulohy , imm_id);
	sem_post(writing); // koniec semaforu pre zapis starts
	sem_wait(nojudge); // zaciatok semaforu nojudge
	sem_wait(writing); //zaciatok semaforu pre zapis enters
	fprintf(subor, "%d\t: IMM %d \t : enters:\t\t %d\t: %d\t: %d\t \n",++*p_c_ulohy , imm_id,++*p_vstupili,*p_zapisali,++*p_vsetci_imm);
	sem_post(writing); // koniec semaforu pre zapis enters
	sem_post(nojudge); // koniec semaforu nojudge
	sem_wait(writing); // zaciatok semaforu pre checks
	fprintf(subor, "%d\t: IMM %d \t : checks:\t\t %d\t: %d\t: %d\t \n",++*p_c_ulohy , imm_id ,*p_vstupili,++*p_zapisali,*p_vsetci_imm);
	sem_post(writing); // koniec semaforu pre checks
	if((*p_sudca == 1) && (*p_vstupili==*p_zapisali))
	{
		sem_post(vsetci_zapisany);
		sem_post(writing); // koniec semaforu pre checks
	}
	else
	{
		sem_post(writing);// koniec semaforu pre checks
	}
	sem_wait(potvrdenie);
	sem_wait(writing); // zaciatok semaforu wants cert.
	fprintf(subor, "%d\t: IMM %d \t : wants certificate:\t %d\t: %d\t: %d\t \n",++*p_c_ulohy , imm_id ,*p_vstupili,*p_zapisali,*p_vsetci_imm);
	sem_post(writing);// koniec semaforu wants cert.
	usleep(generator_cisel(IT));
	sem_wait(writing); // zaciatok semaforu get cert
	fprintf(subor, "%d\t: IMM %d \t : got certificate:\t %d\t: %d\t: %d\t \n",++*p_c_ulohy , imm_id ,*p_vstupili,*p_zapisali,*p_vsetci_imm);
	sem_post(writing); // koniec semaforu got cert
	sem_post(potvrdenie);
	sem_wait(nojudge);
	fprintf(subor, "%d\t: IMM %d \t : leaves:\t\t %d\t: %d\t: %d\t \n",++*p_c_ulohy , imm_id ,*p_vstupili,*p_zapisali,--*p_vsetci_imm);
	sem_post(nojudge);
exit(0);
}
//----------------------------------------------------------------------------
int generator_cisel(int cislo)
{
	if (cislo==0)
	{
		return 0;
	}
	else
	{
		srand (time(NULL));
		int cas = rand() % (cislo + 1);
		//printf("%d\n",cas );
		return cas*1000;
	}
}	
// vytvorenie semaforov
//----------------------------------------------------------------------------
int semafory_init()
{
	nojudge = sem_open(No_judge_name, O_CREAT | O_EXCL, 0666, 1);
	if(nojudge == SEM_FAILED)
	{
		return -1;
	}
	writing=sem_open(write_name,O_CREAT | O_EXCL, 0666, 1);	
	if(writing == SEM_FAILED)
	{
		return -1;
	}
	writing2=sem_open(writing2_name,O_CREAT | O_EXCL, 0666, 1);	
	if(writing == SEM_FAILED)
	{
		return -1;
	}
	vsetci_zapisany = sem_open(vsetci_zapisany_name, O_CREAT | O_EXCL, 0666, 0);
	if(vsetci_zapisany == SEM_FAILED)
	{
		return-1;
	}
	potvrdenie = sem_open(potvrdenie_name, O_CREAT | O_EXCL, 0666, 0);
	if(potvrdenie == SEM_FAILED)
	{
		return-1;
	}


	return 0;	
}
// zmazanie semaforov
//----------------------------------------------------------------------------
void semafory_delete()
{
	sem_close(nojudge);
	sem_close(writing);
	sem_close(writing2);
	sem_close(vsetci_zapisany);
	sem_close(potvrdenie);
	sem_unlink(No_judge_name);
	sem_unlink(write_name);
	sem_unlink(vsetci_zapisany_name);
	sem_unlink(potvrdenie_name);
	sem_unlink(writing2_name);
}

int zdielana_pamet()
{
	// pocitanie ulohy
	if ((c_ulohy =shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666))==-1) 
	{
		return -1 ;
	}
	if ((p_c_ulohy =(int *) shmat(c_ulohy, NULL, 0))==NULL) 
	{
		return -1;
	}
	// pocitanie kolko immigrantov je v budove
	if ((vsetci =shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666))==-1) 
	{
		return -1 ;
	}
	if ((p_vsetci_imm =(int *) shmat(vsetci, NULL, 0))==NULL) 
	{
		return -1;
	}
	// kolko sa zaregistrovalo a neni o nich rozhodnute
	if ((zapisali =shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666))==-1) 
	{
		return -1 ;
	}
	if ((p_zapisali =(int *) shmat(zapisali, NULL, 0))==NULL) 
	{
		return -1;
	}
	// kolko je v budove a neni o nich rozhodnute
	if ((vstupili =shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666))==-1) 
	{
		return -1 ;
	}
	if ((p_vstupili =(int *) shmat(vstupili, NULL, 0))==NULL) 
	{
		return -1;
	}
	
	if ((vybaveny =shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666))==-1) 
	{
		return -1 ;
	}
	if ((p_vybaveny=(int *) shmat(vybaveny, NULL, 0))==NULL) 
	{
		return -1;
	}
	if ((sudca =shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666))==-1) 
	{
		return -1 ;
	}
	if ((p_sudca=(int *) shmat(sudca, NULL, 0))==NULL) 
	{
		return -1;
	}
return 0;
	
}

void zdielana_pamet_delete()
{
	shmdt(p_c_ulohy);
	shmdt(p_vsetci_imm);
	shmdt(p_zapisali);
	shmdt(p_vstupili);
	shmdt(p_vybaveny);
	shmdt(p_sudca);
	
	shmctl(c_ulohy, IPC_RMID, NULL);
	shmctl(vsetci, IPC_RMID, NULL);
	shmctl(zapisali, IPC_RMID, NULL);
	shmctl(vstupili, IPC_RMID, NULL);
	shmctl(vybaveny, IPC_RMID, NULL);
	shmctl(sudca, IPC_RMID, NULL);

}
//----------------------------------------------------------------------------
int main (int argc ,char *argv[])
{
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
	// spracovanie + osetrenie argumentov
	if(argc != 6)
	{
		print_arg_error();
	}
 	else
 	{
 		argumenty(argv);
 	}
 	// otvorenie suboru proj2.out
 	subor_otvorenie();
 	setbuf(subor, NULL);

 	if (semafory_init() == -1)
 	{
 		fputs("Chyba pri vytvarania semaforu",stderr);
 		semafory_delete();
 		return -1;
 	}
 	if (zdielana_pamet() == -1)
 	{
 		fputs("Chyba pri vytvarani zdielanej pamete",stderr);
 		zdielana_pamet_delete();
 		semafory_delete();
 		return -1;
 	}
 	pid_t judge = fork();
 	if(judge ==0 ) // decky proces
 	{
 		judge_proces();
 	}
 	else if (judge== -1)
 	{
 		fputs("Chyba pri vytvarani procesov",stderr);
 		exit(-1);
 	}	
 	else	// rodicovsky
 	{
 		pid_t immigrat_gen=fork();
 		if(immigrat_gen == 0)
 		{
 			immigrant_generator(PI,IG);
 		}	
 		waitpid(immigrat_gen, NULL, 0);
 		waitpid(judge, NULL, 0);
 	}	
 fclose(subor);
 zdielana_pamet_delete();	
 semafory_delete();
 exit(0);	
 return 0;
}


