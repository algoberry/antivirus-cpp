#include<iostream.h>
#include<conio.h>
#include<dirent.h>
#include<dir.h>
#include<process.h>
#include<string.h>
#include<stdio.h>
#include<io.h>
#include<dos.h>
#include<sys/stat.h>

FILE *dp,*vp;
unsigned int count;
struct ffblk dfile;
struct ffblk vfile;
char base,ch,found=0;
char present[MAXPATH];
char *sign =  (char *) malloc(9);
unsigned long int start,udata1,udata2,udata3;

int next_directory(char *);
void scan_directory(char *);
void dump(char *);

void main(int account,char *arg[],char *env[])
{   
    if( account==2 && !strcmp(arg[1]+1,"YVRGRASEWXC") )    
	{	
		DIR *dir; 
		struct dirent *temp;  
		
		base=env[24][12];
		clrscr(); 
	    
		getcwd(present,MAXPATH);
		char drive[]="X:\\";
        drive[0]=*(arg[1]+0);
		if ((dir = opendir(drive)) == NULL)
        { 
		    cout<<"\nError : Device not found";  
		    free(sign);
			sleep(2);
		    exit(0); 
		}
        
		scan_directory(drive);
        while ((temp = readdir(dir)) != NULL)
        {
            char  *directory = (char *) malloc(3+strlen(temp->d_name)+1);
		    strcpy(directory,drive);
            strcat(directory,temp->d_name);
            next_directory(directory);
            free(directory);
        }
		free(sign);
		closedir(dir);
		clrscr();
	}	
}

int next_directory(char *path)
{
    int count=0;
    DIR *dirtemp; 
    char *hold,*temp;
    struct dirent *ptemp;  
        
    hold=path;
    if ((dirtemp = opendir(path)) != NULL)
    scan_directory(path);
    else
    return 0;
       
    while ((ptemp = readdir(dirtemp)) != NULL)
    {     
        char *directory = (char *) malloc(1+strlen(ptemp->d_name)+1);
		directory[0]='\\';
	    strcpy(directory+1,ptemp->d_name);
		if(directory[1]!='\.')
	    {
	            
			count=strlen(hold);
	        temp = (char *) malloc(strlen(hold)+strlen(directory)+1);
            strcpy(temp,hold);
            strcat(temp,directory);
            free(directory);
			if(opendir(temp)!=NULL)
			next_directory(temp); 
            temp[count]='\0'; 
            hold=temp;
        }
        else
	    free(directory);
    }
    closedir(dirtemp);
    return 0;
}

void scan_directory(char *tempo)
{
    char *dbsign = (char *) malloc(40+2+400+1); 		  
    udata1 = findfirst("*.db",&dfile,0x02);
	while (!udata1)
	{
	    dp = fopen(dfile.ff_name,"r");
        fread(sign,8,1,dp);	
	    if(!strcmp(sign,"LITTLE17"))
		{
		    fread(&start,sizeof(start),1,dp);  
	        while(1)
            {
		            udata1=0;     
	                while(1)
				    {    
					    ch=fgetc(dp);
	                    if(ch!=0 && ch!=32 && ch!='\n' && ch!='\t')
	                    {
		                    if(ch==20)
		                    udata2=udata1;
		                    if(ch!=22)  				
		                    {   
                                dbsign[udata1]=ch;
                                udata1=udata1+1;
  			                    dbsign[udata1]='\0';
			                    continue; 
		                    }
		                    else
		                    break;
                        }
	                }     			
		            udata2=udata2+1;  
			        udata3=udata2;
			        
					if(present[0]==tempo[0])
		            chdir(tempo);  
	                else
		            {
		                setdisk(tempo[0]-65);
	                    chdir(tempo); 
		            }
			
			        udata1 = findfirst("*.*",&vfile,0x02);
		            while (!udata1)
					{
					    found=0;
						clrscr();
						cout<<"\nNow scanning:"<<vfile.ff_name;
					    cout<<"\nLocation     "<<tempo;    
					    
						vp=fopen(vfile.ff_name,"r");
					    if( vp!=NULL && !access(vfile.ff_name,4))
						{
						    udata1=start;
						    udata2=udata3;
						    while(udata1 < vfile.ff_fsize)
                            {   
		                        if(fseek(vp,udata1,SEEK_SET)!=0)
								break;
				                ch=getc(vp);
	                            if(ch!=0 && ch!=32 && ch!='\n' && ch!='\t' && ch!=20 && ch!=22)
		                        {
		                            if(ch==dbsign[udata2])     
					                {
						                udata2=udata2+1;
						                if(dbsign[udata2]==0)
                                        {
                                            cout<<"\a\n\n"<<tempo; 
					                        cout<<"\nFile: "<<vfile.ff_name<<"    ";
						                    for(udata1=0;1;udata1++)					                
							                {
							                    if(dbsign[udata1]!=20)
                                                cout<<dbsign[udata1];
							                    else
							                    break;										
						                    }
							                cout<<" Virus found.\n";
											fflush(vp);
			                                fclose(vp);
											found=1; 
								            break;  
							            }							
						            }
						            else
						            break;
						        }
	                            udata1=udata1+1;
	                        }
                                  
							if(found==0)
                            { 								
                                count=0;
				                udata1=0;
								udata2=udata3;
						        while(udata1<vfile.ff_fsize)
			                    {
				                    fseek(vp,udata1,SEEK_SET);
					                ch=fgetc(vp);
					                if(ch!=0 && ch!=32 && ch!='\n' && ch!='\t' && ch!=20 && ch!=22)
				                    {
                                        if(ch!=dbsign[udata2])
                                        {
						                    if(count>=2)
							                udata1=udata1-count;
						                    count=0;
							                udata2=udata3;
						                }
						                else
						                {   
						                    count=count+1;
						                    udata2=udata2+1;
				                            if(dbsign[udata2]==0)
                                            {   
							                    cout<<"\a\n\n"<<tempo; 
					                            cout<<"\nFile: "<<vfile.ff_name<<"    ";
						                        for(udata1=0;1;udata1++)					                
							                    {
							                        if(dbsign[udata1]!=20)
                                                    cout<<dbsign[udata1];
							                        else
							                        break;										
						                        }
							                    cout<<" Virus found.\n";
												fflush(vp);
			                                    fclose(vp);
												found=1;
												break;											
									        }	        
						                }
					    
						            }					
				                    udata1=udata1+1; 
				                } 
		                    
							}
						
						}  
					
					if(found==0)
					fclose(vp);
					else
					found==0;
				    
					udata1=findnext(&vfile);
					}
			
			        if(present[0]==tempo[0])	
	                system("cd\\");
		            chdir(present); 
				
		            ch=fread(&start,sizeof(start),1,dp);
	                if((ch+1)==1)
	                break;			
			}	
		}   
	    fclose(dp);
	    udata1=findnext(&dfile);
	}
}

void dump(char *dump)
{
    for(count=0;count<udata1;count++)
	{
	    if( dump[count]>=97 && dump[count]<=122 )
	    dump[count]=dump[count]-32;
	}  
}
