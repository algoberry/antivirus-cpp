#include<iostream.h>
#include<conio.h>
#include<process.h>
#include<dir.h>
#include<dirent.h>
#include<alloc.h>
#include<io.h>

FILE *dp;
unsigned char ch;
struct ffblk dfile;
int i,j=0,k=0,total,mode=0;
char *sign = (char *) malloc(9);
char *drive = (char *) malloc(27);
char *drivefound= (char *) malloc(27);

void scan();
void info();
void dump(char *);

void main(int a,char *arg[],char *env[])
{    
	if(access("cscan.exe",0)!=0)
	{  
	    free(drive); 
	    free(drivefound);  
		exit(0);
	}
	
	if(access("hscan.exe",0)!=0)
	{  
	    free(drive); 
	    free(drivefound);  
		exit(0);
	}
	char *dirlist = (char *) malloc(53);
	
    i = findfirst("*.db",&dfile,0);                  
    while (!i)                                        
    { 
		dp = fopen(dfile.ff_name,"rb");  
        fread(sign,8,1,dp); 
        if(!strcmp(sign,"LITTLE17"))
	    {  
			j=1;
			fclose(dp);
		    break;
		}
		else
	    fclose(dp);
	    i=findnext(&dfile);
	}
	
	if(j!=1)
	{ 
		cout<<"\nError : Supporting database not found"; 
	    free(drive); free(drivefound);
		getch();
        exit(0);  			
	}
		
	scan();
	while(1)
    {	
	    while(1)
		{	        
				cout<<"\nSystem: Drive Found\n\n\n";
                for(i=0;i<total;i++)
                cout<<drivefound[i]<<":   ";
				
				while(1)
                {
                    cout<<"\n\nPress  1-Cool Mode &  2-Hot Mode";
		            cout<<"\n\nEnter your mode:";
                    ch=getche(); 		
                    if(ch==49)
                    { mode=49; break; }
                    else if(ch==50)
                    { mode=50; break; }
                    else
                    {
                        clrscr();
			            cout<<"\nWarning: Sorry your mode is invalid";
                        cout<<"\nWarning: Please select right mode\n";
                        cout<<"\nSystem: Drive Found\n\n\n";
                        for(i=0;i<total;i++)
                        cout<<drivefound[i]<<":   ";						
                    }  		
                }    	
				
                cout<<"\n\nEnter drive to scan:";
                cin>>dirlist;
                j=strlen(dirlist);
				dump(dirlist);
            
			    if(j==5 && !strcmp(dirlist,"RESET"))
		        scan();
		        
				else if(j==3 && !strcmp(dirlist,"ALL"))
			    {
			        char *passdata = (char *) malloc(13); 
				    strcpy(passdata,"XYVRGRASEWXC");
				    for(j=0;j<total;j++)
			        {
					    *(passdata+0)=drivefound[j];
				        arg[1]=passdata;
				        if(mode==49)
					    spawnve(P_WAIT,"CSCAN.EXE",arg,env);
					    if(mode==50)
					    spawnve(P_WAIT,"HSCAN.EXE",arg,env);
				    }       
			        free(passdata); 
			    }
			    
				else if(j==4 && !strcmp(dirlist,"EXIT"))
                { 
				    free(drive); 
					free(drivefound);  
					free(dirlist);  
					exit(0); 
				} 	
		        else
		        break;
		}
        
		if(j%2!=0)
        {
			for(i=1;i<j;i=i+2)
            {
		        if (dirlist[i]!='\,') 
		        break;
            }

		    if(i>=j)
            {
                k=strlen(dirlist);
                char *passdata = (char *) malloc(13); 
				strcpy(passdata,"XYVRGRASEWXC");
				for(j=0;j<k;j=j+2)
				{
					*(passdata+0)=dirlist[j];
				    arg[1]=passdata;
				    if(mode==49)
					spawnve(P_WAIT,"CSCAN.EXE",arg,env);
					if(mode==50)
					spawnve(P_WAIT,"HSCAN.EXE",arg,env);
				}
				free(passdata);
		    }
            else
			{   clrscr();
			    cout<<"\nWarning: Comma missing between drives"; 
			    cout<<"\nEg: C,F,G - Valid"; 
				cout<<"\nEg: CFG  - Invalid\n";
				info();
			}
		} 
        else
        { 
		    clrscr();
			cout<<"\nWarning: Command should be odd size"; 
			cout<<"\nEg: C,F,G - Valid"; 
			cout<<"\nEg: C,FG  - Invalid\n";
			info();		
		}
        clrscr();
	}    
}

void scan()
{ 
    j=0;
	char dir[]="VOL X:";
	clrscr();
	cout<<"\nSystem: Welcome to Little17 Antivirus";
    for(i=0;i<=25;i++)
    {
        strcpy(drive,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		dir[4]=drive[i];
        ch=system(dir);
        clrscr();
        if(ch==0)
        { 
		    drivefound[j]=drive[i];  
		    j++; 
		}      
        drivefound[j]='\0';
	}
    total=j;
}

void info()
{
cout<<"\n\nInformation:\n";
cout<<"\n1.Hello user i give information to use this software";
cout<<"\n2.Suppose you want to scan particular drive"; 
cout<<"\n  Eg: type  \" C,D,W,I \" ";
cout<<"\n3.If you scan all drive type \" ALL \" ";
cout<<"\n4.Want to close type \" EXIT \" ";
getch();
}

void dump(char *dump)
{
    for(i=0;i<j;i++)
	{
	    if( dump[i]!='\,' && dump[i]>=97 && dump[i]<=122 )
	    dump[i]=dump[i]-32;
	}  
}
