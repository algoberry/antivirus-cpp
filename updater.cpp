#include<iostream.h>
#include<conio.h>
#include<string.h>
#include<process.h>
#include<io.h>
#include<alloc.h>
#include<dir.h>
#include<ctype.h>

struct ffblk dblist,list;
FILE *temp_p,*dp,*vp;
char *sign =  (char *) malloc(9);

char ch;
unsigned int count,first=0,found=0;
unsigned long int start,udata1,udata2,udata3;

void set_virus_name(char *);
void update_signature(char *,char *,int,int);
void set_data(char *,char *,int);

void main()
{
	clrscr();
	char *vfile = (char *) malloc(40);
	char *dfile = (char *) malloc(40);
	cout<<"\nSystem: Enter the infected filename: ";
	cin>>vfile;
	
	if(access(vfile,0)!=0)
	{  
		cout<<"\nError : File not exist"; 
		free(sign);  
		free(vfile); 
		free(dfile); 
		getch(); 
		exit(0); 
	}
	strcpy(sign,"LITTLE17"); 

	udata1 = findfirst("*.db",&dblist,0);
	while (!udata1)
	{   
		first=1;
		temp_p= fopen(dblist.ff_name,"r");  
		fread(sign,8,1,temp_p); 
		if(!strcmp(sign,"LITTLE17"));
		update_signature(vfile,dblist.ff_name,1,0);  
		fclose(temp_p);
		udata1=findnext(&dblist);
	}
			
	if(first==0)
	{
		cout<<"\nSystem: Enter the database name :";
		cin>>dfile;
		udata1=strlen(dfile);
		if(dfile[udata1-3]=='\.'&& (dfile[udata1-2]=='D'||dfile[udata1-2]=='d') &&  (dfile[udata1-1]=='B'||dfile[udata1-1]=='b') )      
		update_signature(vfile,dfile,0,1); 
		else
		{ 
			cout<<"\n\nError : Create .DB extension file type"; 
			free(sign); 
			free(vfile); 
			free(dfile); 
			getch(); 
			exit(0);  
		} 
	}
	else
	{
		if(found==0)  
		{   
			cout<<"\nSystem: Enter the database name :";
			cin>>dfile;
			udata1=strlen(dfile);
			if(dfile[udata1-3]=='\.'&& (dfile[udata1-2]=='D'||dfile[udata1-2]=='d') &&  (dfile[udata1-1]=='B'||dfile[udata1-1]=='b') )
			{
				if(!access(dfile,0))
				{   
					dp = fopen(dfile,"r");  
					fread(sign,8,1,dp); 
					fclose(dp);
					if(!strcmp(sign,"LITTLE17"))
					update_signature(vfile,dfile,1,1); 
					else
					{ 
						cout<<"\n\nError : Database not supported with "<<vfile;  
						free(sign); 
						free(vfile); 
						free(dfile); 
						exit(0);  
					}  
				
				}
				else
				update_signature(vfile,dfile,0,1); 
			}
			else
			{   
				cout<<"\n\nError: Create .DB extension file type"; 
				free(sign);  
				free(vfile); 
				free(dfile); 
				exit(0); 
			}  
		
		}
	} 
	free(sign);
	free(vfile); 
	free(dfile); 
	getch();
}

void update_signature(char *vfile,char *dfile,int check,int add)
{		
	if(check==1)
	{	
		char *temp = (char *) malloc(strlen(dfile)+1);
		strcpy(temp,dfile);
		char *turn = (char *) malloc(40+2+400+1); 	
		
		dp = fopen(dfile,"r");
		fread(sign,8,1,dp);	
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
						turn[udata1]=ch; 
						udata1=udata1+1;
						continue; 
					}
					else
					break;
				}
			}  
			turn[udata1]='\0';
			udata2=udata2+1;
			udata3=udata2;
			
			count=0;
			udata1=0; 
			udata2=udata3;
			vp=fopen(vfile,"r");
			findfirst(vfile,&list,0);
			while(udata1<list.ff_fsize)
			{
				
				fseek(vp,udata1,SEEK_SET);
				ch=fgetc(vp);
				if(ch!=0 && ch!=32 && ch!='\n' && ch!='\t' && ch!= 20 && ch!= 22)
				{
					if(ch!=turn[udata2])
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
						if(turn[udata2]==0)
						{   
							
							cout<<"\n\n\nError : Unsuccessfully ! ";
							cout<<"\n\nError : Virus were Already added on "<<temp;  
							cout<<"  [ ";
							udata3--;
							for(count=0;count<udata3;count++)
							{
								if(turn[count]!=32)
								cout<<turn[count];
								else
								cout<<" ";									
							}
							cout<<" ]";
							found=1; 
							break; 
						}	        
					}
				}					
				fflush(vp);
				udata1=udata1+1;
				
			}
			fclose(vp);
			
			if(found==1)
			break;
			ch=fread(&start,sizeof(start),1,dp);
			if((ch+1)==1)
			break;
		}
		fclose(dp);
		free(temp);
		free(turn);
		
		if(found==0&&add==1)
		{
			set_data(vfile,dfile,check);   
		}					
	}
	else
	{       
		set_data(vfile,dfile,check);    
	}						
}

                            
void set_data(char *vfile,char *dfile,int check)
{
	char *temp = (char *) malloc(40);
	cout<<"\n\nSystem: Set the name of virus :";
	gets(temp);
	cout<<"\nEnter first index :";
	cin>>udata1;
	start=udata1;
	cout<<"\nEnter last  index :";
	cin>>udata2;
	char *vdata = (char *) malloc((udata2-udata1)+2+2+2+1); 
	vdata[0]=20;
	udata3=1;
	
	vp=fopen(vfile,"r");
	while(udata1<=udata2)
	{   
		fseek(vp,udata1,SEEK_SET);
		ch=getc(vp);
		cout<<udata1<<" "<<ch<<"\n";
		if(ch!=0 && ch!=32 && ch!='\n' && ch!='\t' && ch!=20 && ch!=22)
		{
			vdata[udata3]=ch;
			udata3=udata3+1; 	
			vdata[udata3]='\0'; 			
		}
		udata1=udata1+1;
	}
	fclose(vp);
	vdata[udata3+1]='\0';
	vdata[udata3]=22;
	
	dp=fopen(dfile,"a+"); 
	if(check==0)
	fwrite(sign,8,1,dp);
	fwrite(&start,sizeof(start),1,dp);
	fwrite(temp,strlen(temp),1,dp);
	free(temp);
	fwrite(vdata,strlen(vdata),1,dp);
	free(vdata);
	fclose(dp);
	cout<<"\n\nSystem: Successfully Added on "<<dfile;
}
