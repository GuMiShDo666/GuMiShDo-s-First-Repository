#include<stdio.h>  
#include<stdlib.h>  
#include<stdbool.h>  
#include<string.h>
#include<ctype.h>

#define INF 51  
  
int User_Number,Order_Number,N,M,Movie_Number,Hall_Number,User_Now,Hall_Now,Time,Manager_Number,Manager_Now,Session_Number,Cinema_Now;
int Movie_N[INF],Hall_N[INF],Seat_Number[INF],Length[INF],Wide[INF],HALL_T[INF][1441];
char HALL[INF][INF];
char Admin_Key[20]="0123121";//���ù���ԱȨ������
  
struct user{ 
	//Order_num[i]:��ʾһ��֮�еĵ�i���û��Ķ�����
	//Time[i][j]:��ʾһ��֮�еĵ�i��,һ��֮��ĵ�i�����û��Ƿ����
	//Order[i]��ʾ�û��ĵ�i�������Ķ�����,Tmp��ʾ�û����������� 
    int Order[INF],Time[366][1441],Rest,Order_num[366],Movie[INF],Tmp;
    char User_ID[INF],name[INF],gender[10],phone[INF],password[20],email[INF];
}User[INF],New_User;//User[i]��ʾ���Ϊi���û� 

struct order{  
    int Seat_Number,Order_Time,Seat[4][3],User,n,m;  
    char Hall_name[INF],Belong_Cinema[INF];
    //Seat_Number��ʾ�ö�������������λ����,Order_Time��ʾ������Ӧ������
	//Hall_name��ʾ�ö�������Ӧ��Ӱ�����
	//(Seat[i][1],Seat[i][2])��ʾ�ö�����i��������λ
	//User��ʾ�ö�����Ӧ���û����
	//Belong_Cinema��ʾ�ö���������һ����ӰԺ(�漰����Ա����)
	//n,m��ʾ�ö����ĵ�Ӱ��ָMovie[n][m] 
}Order[INF];//Order[i]��ʾ������Ϊi�Ķ���,Order_Number��ʾ�������� 
  
struct Hall{  
	char Name[INF];
    int Seat[INF][INF],Start_Time,End_Time,movie1,Number,Rest,movie2;
    //Seat��ʾ��Ӱ������λ�� Start_Time\End_Time��ʾ��ʼ������ʱ��(�Ѿ�ת����һ���еĵڼ�����)
	//movie1,movie2��ʾ�ó��ο�����Movie[movie1][movie2]
	//Number��ʾ�Ѿ�����ȥ��Ʊ�� Rest��ʾ��Ʊ�� 
}Hall[INF][INF];//Hall[i][1--Hall_N[i]]Ϊͬһ��Ӱ��һ��֮�䲻ͬ�ĳ��� 
  
struct movie{  
    //Hall��ʾ��Ӧ��Ӱ��
	//Seat��ʾ��λͼ
	//Number��ʾ����ȥ��Ʊ������ Rest_Ticket��ʾ��Ʊ��
	//Cinema��ʾ�õ�Ӱ��Ӧ�ĵ�ӰԺ��� 
    int Start_Time,End_Time,Hall,Seat[INF][INF],Number,Price,Time,Rest_Ticket,Cinema,n,m;
	char session[50],name[200],type[50],Hall_Name[50],Cinema_Name[50];
	//session��ʾ���κ� name��ʾ��Ӱ���� 
}Movie[INF][INF]; //Movie[i][1--Movie_N[i]]Ϊͬһ����Ӱһ��֮�䲻ͬ�ĳ��� 

struct manager{
	char Manager_ID[INF],name[INF],phone[INF],password[20],email[INF],Cinema[INF];
}Manager[INF],New_Manager;

struct Data{
	char session[50];//���κ�
	char name[200];//��Ӱ����
	char cinema[200];//��ӰԺ
	char hall[200];//Ӱ��
	int Hall_Index;
	char start[50];//��ʼʱ��
	char end[50];//����ʱ��
	int price,length;//�۸�
	int rest;//ʣ��Ʊ��
	char type[50];//��������
	int x,y;
	//��Ʊ ������
	//�Żݻ ������
}movies[INF],kk,New_Movie;

int Trans_Time();
int cmp1();
int cmp2();
int cmp3();
int cmp4();
int cmp5();
void Load_Movie_Data();
int isValidID();
int isValidPhone();
int isValidEmail();
void Load_User_Data();
void Load_Manager_Data();
void Load_Order_Data();
void saveData();
void Invest_Charge();
void Register_User();
int Register_Manager();
int Trans_Month_And_Date();
int Trans_Month_Back();
int Trans_date_Back();
int Judge_User_Time();
int Judge_Hall_Seat();
void Change_User();
void Create_Order();
void Change_Hall();
void Search_Movie();
void save_Order_Data();
void Purchase();
int Login_User();
int Login_Manager();
void Print1();
void addHall();
void viewHalls();
void addSchedule();
void viewSchedules();
void changePassword();
void changeEmail();
void changeHall();
void changeName();
void viewPersonalInfo();
void Change_User_Information();
void Search_User_Order();
void Choice_Three();
void Manager_Operation();
void personalInformation();


int Trans_Time(char T[50]){
    int Hour=0,Minute=0,Judge=0;
	for(int i=0;i<strlen(T);i++){
		if(T[i]==':'){
			Judge=1;continue;
		}
		if(!Judge) Hour=Hour*10+(T[i]-'0');
		else Minute=Minute*10+(T[i]-'0');
	}
	return 60*Hour+Minute;
}

void Load_Movie_Data(){
	for(int i=1;i<=6;i++)
	    for(int j=1;j<=1440;j++)
	        HALL_T[i][j]=0;
	Length[1]=10;Wide[1]=14;
	Length[2]=13;Wide[2]=13;
	Length[3]=10;Wide[3]=13;
	Length[4]=8;Wide[4]=16;
	Length[5]=8;Wide[5]=15;
	Length[6]=13;Wide[6]=16;
	int Now=0,i=1;
	FILE* fp1=fopen("Movie_Data.txt","r");
	if(fp1==NULL) return;
	while(fscanf(fp1, "%s %s %s %s %s %s %d %d %s",
		movies[i].session,movies[i].name,movies[i].cinema,movies[i].hall,
		movies[i].start,movies[i].end,&movies[i].length,&movies[i].price,movies[i].type)!=EOF){
	    int Judge=0;
		for(int j=1;j<=Now;j++)
		    if(!strcmp(movies[i].name,Movie[j][1].name)){
				Movie_N[j]++;Judge=1;movies[i].x=j;movies[i].y=Movie_N[j];
				Movie[j][Movie_N[j]].Price=movies[i].price;
				strcpy(Movie[j][Movie_N[j]].session,movies[i].session);
				strcpy(Movie[j][Movie_N[j]].name,movies[i].name);
				strcpy(Movie[j][Movie_N[j]].type,movies[i].type);
				strcpy(Movie[j][Movie_N[j]].Hall_Name,movies[i].hall);
				strcpy(Movie[j][Movie_N[j]].Cinema_Name,movies[i].cinema);
                Movie[j][Movie_N[j]].Start_Time=Trans_Time(movies[i].start);
				Movie[j][Movie_N[j]].End_Time=Trans_Time(movies[i].end);
				for(int k=1;k<=Hall_Number;k++)
				    if(!strcmp(movies[i].hall,HALL[k]))
					    Movie[j][Movie_N[j]].Hall=k;
				if(!strcmp(movies[i].cinema,"��Ӱ���")) Movie[j][Movie_N[j]].Cinema=1;
				if(!strcmp(movies[i].cinema,"�λ�Ӱ��")) Movie[j][Movie_N[j]].Cinema=2;
				movies[i].Hall_Index=Movie[j][Movie_N[j]].Hall;
				int Now_Hall=Movie[j][Movie_N[j]].Hall;
				Hall_N[Now_Hall]++;
				Hall[Now_Hall][Hall_N[Now_Hall]].Start_Time=Movie[j][Movie_N[j]].Start_Time;
				Hall[Now_Hall][Hall_N[Now_Hall]].End_Time=Movie[j][Movie_N[j]].End_Time;
				Hall[Now_Hall][Hall_N[Now_Hall]].movie1=j;
				Hall[Now_Hall][Hall_N[Now_Hall]].movie2=Movie_N[j];
				Movie[j][Movie_N[j]].Rest_Ticket=Length[Movie[j][Movie_N[j]].Hall]*Wide[Movie[j][Movie_N[j]].Hall];
				Hall[Now_Hall][Hall_N[Now_Hall]].Rest=Movie[j][Movie_N[j]].Rest_Ticket;
				Movie[j][Movie_N[j]].n=Now_Hall;Movie[j][Movie_N[j]].m=Hall_N[Now_Hall];
				for(int k=Movie[j][Movie_N[j]].Start_Time-15;k<=Movie[j][Movie_N[j]].End_Time+15;k++) HALL_T[Now_Hall][k]=1;
				i++;
			}
		if(Judge) continue;
		Now++;Movie_N[Now]++;
		movies[i].x=Now;movies[i].y=1;
		Movie[Now][1].Price=movies[i].price;
		strcpy(Movie[Now][1].session,movies[i].session);
		strcpy(Movie[Now][1].name,movies[i].name);
		strcpy(Movie[Now][1].type,movies[i].type);
		strcpy(Movie[Now][1].Hall_Name,movies[i].hall);
		strcpy(Movie[Now][1].Cinema_Name,movies[i].cinema);
        Movie[Now][1].Start_Time=Trans_Time(movies[i].start);
		Movie[Now][1].End_Time=Trans_Time(movies[i].end);
		for(int k=1;k<=Hall_Number;k++)
				    if(!strcmp(movies[i].hall,HALL[k]))
					    Movie[Now][1].Hall=k;
		if(!strcmp(movies[i].cinema,"��Ӱ���")) Movie[Now][1].Cinema=1;
		if(!strcmp(movies[i].cinema,"�λ�Ӱ��")) Movie[Now][1].Cinema=2;
		int Now_Hall=Movie[Now][1].Hall;
		Hall_N[Now_Hall]++;
		Hall[Now_Hall][Hall_N[Now_Hall]].Start_Time=Movie[Now][1].Start_Time;
		Hall[Now_Hall][Hall_N[Now_Hall]].End_Time=Movie[Now][1].End_Time;
		Hall[Now_Hall][Hall_N[Now_Hall]].movie1=Now;
		Hall[Now_Hall][Hall_N[Now_Hall]].movie2=1;
		Movie[Now][1].Rest_Ticket=Length[Movie[Now][1].Hall]*Wide[Movie[Now][1].Hall];
		Hall[Now_Hall][Hall_N[Now_Hall]].Rest=Movie[Now][1].Rest_Ticket;
		Movie[Now][1].n=Now_Hall;Movie[Now][1].m=Hall_N[Now_Hall];
		for(int k=Movie[Now][1].Start_Time-15;k<=Movie[Now][1].End_Time+15;k++) HALL_T[Now_Hall][k]=1;
		i++;
	}
	Movie_Number=Now;Session_Number=i-1;
}

void save_Movie_Data(){
	FILE* fp1=fopen("Movie_Data.txt","wb");
	if(!fp1) return;
	for(int i=1;i<=Session_Number;i++)
	    fprintf(fp1, "%s %s %s %s %s %s %d %d %s",
		movies[i].session,movies[i].name,movies[i].cinema,movies[i].hall,
		movies[i].start,movies[i].end,movies[i].length,movies[i].price,movies[i].type);
	fclose(fp1);
}

int isValidID(const char *Id){//�ж�ID�������Ƿ�Ϸ�
	int Len=strlen(Id);
	if (Len<6||Len>10)return 0;
	int Judge1=0,Judge2=0;
	for (int i=0;i<Len;i++) {
		if(Id[i]>='0'&&Id[i]<='9') Judge1++;
		if(Id[i]>='a'&&Id[i]<='z') Judge2++;
		if(Id[i]>='A'&&Id[i]<='Z') Judge2++;
		if(Judge1+Judge2<i+1) return 0;
	}
	if(!Judge1||!Judge2) return 0;
	return 1;
}

int isValidPhone(const char *phone){//�жϵ绰�����Ƿ�Ϸ�
	if (strlen(phone)!=11)return 0;
	for (int i=0;i<11;i++) {
		if (phone[i]<'0'||phone[i]>'9') return 0;
	}
	return 1;
}

int isValidEmail(const char *email){//�ж����������Ƿ�Ϸ�
	const char *at=strchr(email,'@');
	if (!at||at==email||!strchr(at,'.')) return 0;
	return 1;
}

void Load_User_Data() {//����ʼ�û����ݿ��������������
    int i=1;
	FILE* fp1=fopen("User_Data.txt","r");
	if(fp1==NULL) return;
	while(fscanf(fp1,"%s %s %s %s %s %s",User[i].User_ID,User[i].name,User[i].gender,User[i].phone,User[i].password,User[i].email)==6)
	    i++;
	User_Number=i;
	i=1;
	fclose(fp1);
}

void Load_Manager_Data(){
	int i=1;
	FILE* fp1=fopen("Manager_Data.txt","r");
	if(fp1==NULL) return;
	while(fscanf(fp1,"%s %s %s %s %s %s ",Manager[i].Manager_ID,Manager[i].Cinema,Manager[i].name,Manager[i].phone,Manager[i].password,Manager[i].email)!=EOF)
	    i++;
	Manager_Number=i;
	fclose(fp1);
}

void save_User_Data(){//�����ݿ�����ݸ��£������ݿ��ﵼ�������ݣ�
	FILE* fp1=fopen("User_Data.txt", "a");
	if(!fp1){
		printf("�޷���������.\n");
		return;
	}
	fprintf(fp1,"%s %s %s %s %s %s\n",New_User.User_ID,New_User.name,New_User.gender,New_User.phone,New_User.password,New_User.email);
	fclose(fp1);
}

void save_Manager_Data(){
	FILE* fp1=fopen("Manager_Data.txt", "a");
	if(!fp1){
		printf("�޷���������.\n");
		return;
	}
	fprintf(fp1,"%s %s %s %s %s %s ",New_Manager.Manager_ID,New_Manager.Cinema,New_Manager.name,New_Manager.phone,New_Manager.password,New_Manager.email);
	fclose(fp1);
}

void Invest_Rest(int Now){
	printf("����������ֵ����(��λ:Ԫ):\n");
	int zzh=0;
	scanf("%d",&zzh);
	while(zzh<0){
		printf("��������ȷ����\n");
		scanf("%d",&zzh);
	}
	User[Now].Rest+=zzh;
	printf("��ֵ�ɹ�!\n");
	printf("���ڷ�����һ��\n"); 
	system("timeout /t 3"); 
}

void Register_User(){//�û�ע��
	printf("�����û�ID(6-10 ��ĸ�����ֵ����):\n");
	scanf("%s", New_User.User_ID);
	while(!isValidID(New_User.User_ID)) {
		printf("�û�ID������Ҫ��.\n");
        system("timeout /t 3");     
        printf("����������:\n");
	    scanf("%s", New_User.User_ID);
	}
	printf("��������:\n");
	scanf("%s",New_User.name);
	printf("�����Ա�:\n");
	scanf("%s",New_User.gender);
	while(strcmp(New_User.gender,"��")&&strcmp(New_User.gender,"Ů")){
		printf("���벻���Ϲ���,����������\n");
		scanf("%s",New_User.gender);
	}
	printf("����绰����(11λ):\n");
	scanf("%s",New_User.phone);
	while(!isValidPhone(New_User.phone)) {
		printf("�绰���벻���Ϲ���\n");
        system("timeout /t 3");     
        printf("����������:\n");
	    scanf("%s",New_User.phone);
	}
	printf("��������:\n");
	scanf("%s",New_User.password);
	printf("��������:\n");
	scanf("%s",New_User.email);
	while(!isValidEmail(New_User.email)) {
		printf("�����ַ�����Ϲ���\n");
		system("timeout /t 3");     
        printf("����������:\n");
	    scanf("%s",New_User.email);
	}
	User[++User_Number]=New_User;
	printf("�û�ע��ɹ�\n");
	printf("��ǰ�˻����Ϊ��,�Ƿ���Ҫ���г�ֵ\n");
	int xlm;
	printf("1.��ת��ֵҳ�� 2.�����г�ֵ\n");
	scanf("%d",&xlm); 
	while(xlm<1||xlm>2){
		printf("���벻���Ϲ涨,����������\n");
		scanf("%d",&xlm); 
    }
    if(xlm==1)Invest_Rest(User_Number);
    New_User.Rest=User[User_Number].Rest;
	save_User_Data();
}

int Register_Manager() {//����Աע��
	char Key[20];
    int Judgement=1;
	printf("�������ԱȨ�����룺\n");
	scanf("%s", Key);
	while(strcmp(Key,Admin_Key)) {
		printf("�������,����������,��ʣ��%d�λ���:\n",5-Judgement);
        Judgement++;
        if(Judgement==5){
            printf("����������,��Ҫ��ת����һҳ��\n");
            system("timeout \t 3");
            return 0;
        }
	    scanf("%s", Key);
	}
	printf("�������ԱID(6-10λ��ĸ�����ֵ����): \n");
	scanf("%s", New_Manager.Manager_ID);
	while(!isValidID(New_Manager.Manager_ID)) {
		printf("�����ID�����Ϲ���\n");
        system("timeout \t 3");
        printf("����������");
        scanf("%s", New_Manager.Manager_ID);
	}
	printf("����ӰԺ����: ");
	scanf("%s", New_Manager.Cinema);
	printf("��������: ");
	scanf("%s", New_Manager.name);
	printf("����绰����(11λ): ");
	scanf("%s", New_Manager.phone);
	while(!isValidPhone(New_Manager.phone)) {
		printf("����ĵ绰���벻���Ϲ���\n");
        system("timeout \t 3");
        printf("����������");
        scanf("%s", New_Manager.phone);
	}
	printf("��������:\n");
	scanf("%s", New_Manager.password);
	printf("��������:\n");
	scanf("%s", New_Manager.email);
	while(!isValidEmail(New_Manager.email)) {
		printf("��������䲻���Ϲ���\n");
        system("timeout \t 3");
        printf("����������");
        scanf("%s", New_Manager.email);
	}
	Manager[Manager_Number++]=New_Manager;
	save_Manager_Data();
	printf("����Աע��ɹ�\n");
    return 1;
}
  
int Trans_Month_And_Date(int x, int y){  
    int month[13]={0,31,28,31,30,31,30,31,31,30,31,30,31},Sum = 0;  
    while(--x) Sum+=month[x];  
    return Sum+y;  
} //�鿴��һ��ĵڶ�����
  
int Trans_Month_Back(int Now){
	int month[13]={0,31,28,31,30,31,30,31,31,30,31,30,31},Sum=1;
	while(Now>month[Sum]) Now-=month[Sum],Sum++;
	return Sum;
}//�������ڵ��·�

int Trans_date_Back(int Now){
	int month[13]={0,31,28,31,30,31,30,31,31,30,31,30,31},Sum=1;
	while(Now>month[Sum]) Now-=month[Sum],Sum++;
	return Now;
}//�������ڵ�����

int Judge_User_Time(int user){  
    int start=Movie[N][M].Start_Time,end=Movie[N][M].End_Time;  
    for(int i=start;i<=end;i++)  
        if(User[user].Time[Time][i])  
            return 0;  
    for(int i=start;i<=end;i++)  
        User[user].Time[Time][i]=1;  
    return 1;  
}//�ж��û���Ʊʱ���Ƿ��ظ�  
  
int Judge_Hall_Seat(int x, int y){
    int n=Movie[N][M].n,m=Movie[N][M].m;  
    if (Hall[n][m].Seat[x][y]) return 0;  
    if (!Hall[n][m].Seat[x][y]&&((Hall[n][m].Seat[x][y - 2]&&Hall[n][m].Seat[x][y-2]!=Order_Number)||(Hall[n][m].Seat[x][y + 2]&&Hall[n][m].Seat[x][y+2]!=Order_Number))) return 0;  
    return 1;  
} //�ж϶�Ʊ��λ�Ƿ�ֻ���һ����λ 
  
void Change_User(){  
    User[User_Now].Rest-=Movie[N][M].Price;  
    User[User_Now].Order[++User[User_Now].Tmp]=Order_Number;  
    User[User_Now].Movie[N]=1;
}  
  
void Create_Order(int x, int y) {  
    Order[Order_Number].Seat_Number++;  
    Order[Order_Number].Seat[Order[Order_Number].Seat_Number][1]=x;  
    Order[Order_Number].Seat[Order[Order_Number].Seat_Number][2]=y;  
	strcpy(Order[Order_Number].Belong_Cinema,Movie[N][M].Cinema_Name);
	strcpy(Order[Order_Number].Hall_name,Movie[N][M].Hall_Name);
	Order[Order_Number].n=N;Order[Order_Number].m=M;
	Order[Order_Number].User=User_Now;
}  
  
void Change_Hall(int x, int y){  
    int n=Movie[N][M].n,m=Movie[N][M].m;
    Hall[n][m].Seat[x][y]=Order_Number;  
    Hall[n][m].Number++;
	Hall[n][m].Rest--;  
	Movie[N][M].Rest_Ticket--;
	Movie[N][M].Number++;
}  

void Load_Order_Data(){
	int i=1;
	FILE* fp1=fopen("Order_Data.txt","r");
	if(fp1==NULL) return;
	while(fscanf(fp1,"%d %d %d %s %s %d %d",&i,&Order[i].User,&Order[i].Seat_Number,Order[i].Hall_name,Order[i].Belong_Cinema,&Order[i].n,&Order[i].m)!=EOF){
	    for(int j=1;j<=Order[i].Seat_Number;j++){
	        fscanf(fp1,"%d %d",&Order[i].Seat[j][1],&Order[i].Seat[j][2]);
	        Order_Number=i;N=Order[i].n;M=Order[i].m;
	        Change_Hall(Order[i].Seat[j][1],Order[i].Seat[j][2]);
	    }
	    i++;
	}
	Order_Number=i-1;
	fclose(fp1);
}

void Print1(int searchMovie,int j){//��ʾ��ѯ���
	int Start_H,Start_M,End_H,End_M;
	Start_H=Movie[searchMovie][j].Start_Time/60; 
	Start_M=Movie[searchMovie][j].Start_Time%60;
	End_H=Movie[searchMovie][j].End_Time/60;
	End_M=Movie[searchMovie][j].End_Time%60;
	printf("%s,��%s��������%s��%s����,%d:%d��ʼ,%d:%d����,����%d����,Ʊ��%dԪ,��������Ϊ%s\n ", 
    Movie[searchMovie][j].session,Movie[searchMovie][j].name,Movie[searchMovie][j].Cinema_Name,Movie[searchMovie][j].Hall_Name,
	Start_H,Start_M,End_H,End_M,Movie[searchMovie][j].End_Time-Movie[searchMovie][j].Start_Time,Movie[searchMovie][j].Price,Movie[searchMovie][j].type);
}

void Search_Movie(){//ӰƬ���β�ѯ
	int choice,searchMovie,searchCinema,Choice1,Judge;
	printf("����������Ҫ���еĲ���:\n"); 
	printf("ӰƬ���β�ѯ\n1.����ӰƬ����ѯ \n2. ����ӰԺ����ѯ\n3. ɸѡ������������\n4. ���ų����Ƽ�\n5. �˳�\n");
	scanf("%d", &choice);
	while(choice<1||choice>5){
		printf("���벻���Ϲ涨,����������:\n");
		scanf("%d",&choice);
	}
    if(choice==1){//����ӰƬ����ѯ
		printf("����ӰƬ����ѯ:\n");
		for(int i=1;i<=Movie_Number;i++) printf("%d.%s\n",i,Movie[i][1].name);
		printf("ѡ��ӰƬ (1-%d): \n",Movie_Number);
	   	scanf("%d",&searchMovie);
		while(searchMovie<=0||searchMovie>Movie_Number){
			printf("���벻���Ϲ���,����������\n");
			scanf("%d",&searchMovie);
		}
		for(int j=1;j<=Movie_N[searchMovie];j++) Print1(searchMovie,j);
		printf("����ɸѡ����\n1.Ӱ������\n2.��������\n3.�˳�\n");
		printf("��ѡ��: \n");
		scanf("%d", &Choice1);
		while(Choice1<1||Choice1>3){
			printf("���벻���Ϲ涨,����������:\n");
			scanf("%d",&Choice1);
		}
		//��һ��ɸѡ
		if(Choice1==1){//ɸѡӰ��
			printf("��������ѡ������Ӱ��?:\n");
			printf("1.3D\n2.4D\n3.IMAX\n");
			int Choice2;
			scanf("%d",&Choice2);
			while(Choice2<1||Choice2>3){
				printf("���벻���Ϲ涨,����������:\n");
				scanf("%d",&Choice2);
			}	
			int Index=0,AAA[INF]={0};
			for(int j=1;j<=Movie_N[searchMovie];j++){
				if(Movie[searchMovie][j].Hall==Choice2||Movie[searchMovie][j].Hall==Choice2+3){
					printf("%d. ",++Index);
					AAA[Index]=j;
					Print1(searchMovie,j);
				}
			}
			if(Index==0){
				printf("��Ǹ,û�и����͵ĳ���\n");
				system("timeout /t 3");
				Search_Movie();
			}
			printf("��������Ҫ����һ����Ӱ?:\n");
			scanf("%d",&Judge);
			while(Judge<1||Judge>Index){
				printf("���벻���Ϲ���,����������\n");
				scanf("%d",&Judge);
			}
			N=searchMovie;M=AAA[Judge];
			Purchase();
		}
		if(Choice1==2){//ɸѡ����
            printf("��������Ҫ���������Եĵ�Ӱ?:\n");
			printf("1.����\n2.Ӣ��\n");
			int Choice2;
			scanf("%d",&Choice2);
			while(Choice2<1||Choice2>2){
				printf("���벻���Ϲ涨,����������:\n");
				scanf("%d",&Choice2);
			}
			int Index=0,AAA[INF]={0};
			if(Choice2==1){
			    printf("��������Ϊ���ĵĵ�Ӱ�У�\n");
				for(int j=1;j<=Movie_N[searchMovie];j++)
					if(!strcmp(Movie[searchMovie][j].type,"����")){
					   	printf("%d. ",++Index);
					    AAA[Index]=j;
					    Print1(searchMovie,j);
				}
			}
			if(Choice2==2){
				printf("��������ΪӢ�ĵĵ�Ӱ�У�\n");
				for(int j=1;j<=Movie_N[searchMovie];j++)
					if(!strcmp(Movie[searchMovie][j].type,"Ӣ��")){
					    printf("%d. ",++Index);
					    AAA[Index]=j;
					    Print1(searchMovie,j);
				}
			}
			if(Index==0){
				printf("��Ǹ,û�и����͵ĳ���\n");
				system("timeout /t 3");
				Search_Movie();
			}
			int Judge;
			printf("��������Ҫ����һ����Ӱ?:\n");
			scanf("%d",&Judge);
			while(Judge<1||Judge>Index){
				printf("���벻���Ϲ���,����������\n");
				scanf("%d",&Judge);
			}
			N=searchMovie;M=AAA[Judge];
			Purchase();
		}
		if(Choice1==3) printf("�˳��У����Ժ�\n");
	}
	if(choice==2){//����ӰԺ����ѯ
		printf("��������Ҫȥ��һ��ӰԺ��Ӱ?:\n1.��Ӱ���\n2.�λ�Ӱ��\n");
		scanf("%d",&searchCinema);
		while(searchCinema<1||searchCinema>2){
				printf("���벻���Ϲ涨,����������:\n");
				scanf("%d",&searchCinema);
		}
		int Index=0,AAA[INF],BBB[INF];
		if(searchCinema==1){//��Ӱ���
			printf("�����ڹ�Ӱ���Ӱ����ӳ�ĵ�Ӱ�У�\n");
			for(int i=1;i<=Movie_Number;i++)
				for(int j=1;j<=Movie_N[i];j++)
					if(Movie[i][j].Cinema==1){
					    printf("%d. ",++Index);
					    AAA[Index]=i;BBB[Index]=j;
					    Print1(i,j);
				}
		}
		if(searchCinema==2){//�λ�Ӱ��
			printf("�������λ�Ӱ����ӳ�ĵ�Ӱ�У�\n");
			for(int i=1;i<=Movie_Number;i++)
				for(int j=1;j<=Movie_N[i];j++)
					if(Movie[i][j].Cinema==2){
					    printf("%d. ",++Index);
					    AAA[Index]=i;BBB[Index]=j;
					    Print1(i,j);
				}
		}
		int Judge;
		printf("��������Ҫ����һ����Ӱ?:\n");
		scanf("%d",&Judge);
		while(Judge<1||Judge>Index){
			printf("���벻���Ϲ���,����������\n");
			scanf("%d",&Judge);
		}
		N=AAA[Judge];M=BBB[Judge];
		Purchase();
	}
	if(choice==3){//ɸѡ������������
		printf("ɸѡ������������\n1.���ݷ�ӳ��ʼʱ������ \n2. ���ݳ���Ʊ������\n3. ���ݳ�����Ʊ������\n4. �˳�\n");
		printf("��ѡ��: \n");
		scanf("%d", &Choice1);
		while(Choice1<1||Choice1>4){
			printf("���벻���Ϲ涨,����������:\n");
			scanf("%d",Choice1);
		}
		if(Choice1==1)//��ӳʱ������
			for(int i=1;i<=Session_Number;i++)
				for(int j=1;j<=i;j++){
					if(Trans_Time(movies[i].start)<Trans_Time(movies[j].start)){
						kk=movies[i];
						movies[i]=movies[j];
						movies[j]=kk;
					}
				}
		if(Choice1==2)//����Ʊ������
			for(int i=1;i<=Session_Number;i++)
				for(int j=1;j<=i;j++){
					if(movies[i].price<movies[j].price){
						kk=movies[i];
						movies[i]=movies[j];
						movies[j]=kk;
					}
				}
		if(Choice1==3)//������Ʊ������ ������
			for(int i=1;i<=Session_Number;i++)
				for(int j=1;j<=i;j++){
					if(Movie[movies[i].x][movies[i].y].Rest_Ticket>Movie[movies[i].x][movies[i].y].Rest_Ticket){
						kk=movies[i];
						movies[i]=movies[j];
						movies[j]=kk;
					}
				}
		if(Choice1==4){//�˳�
				printf("�˳��У����Ժ�\n");
				system("timeout /t 3");
				Search_Movie();
				return;
		}
		for(int i=1;i<=Session_Number;i++){
			printf("%d. ��Ӱ���Σ�%s,��%s��������%s��%s����,%s��ʼ,%s����,Ʊ��%dԪ,��������Ϊ%s\n ", 
				i,movies[i].session,movies[i].name,movies[i].cinema,movies[i].hall,
				movies[i].start,movies[i].end,movies[i].price,movies[i].type);
		}
		printf("��������Ҫ����һ����Ӱ?:(1-%d)\n",Session_Number);
		int Judge;
		scanf("%d",&Judge);
		while(Judge<1||Judge>Session_Number){
			printf("���벻���Ϲ���,����������\n");
			scanf("%d",&Judge); 
		}
		N=movies[Judge].x;M=movies[Judge].y;
		Purchase();
	}
	if(choice==4){//���ų����Ƽ� 
	    printf("1.Ʊ�����\n2.�������\n");
	    int Choice3;
	    scanf("%d",&Choice3);
	    while(Choice3<1||Choice3>2){
	    	printf("���벻���Ϲ涨,����������:\n");
	    	scanf("%d",&Choice3);
	    }
	    if(Choice3==1){
	    	for(int i=1;i<=Session_Number;i++)
				for(int j=1;j<=i;j++){
					if(movies[i].price<movies[j].price){
						kk=movies[i];
						movies[i]=movies[j];
						movies[j]=kk;
					}
				}
			for(int i=1;i<=Session_Number;i++){
				printf("%d. ��Ӱ���Σ�%s,��%s��������%s��%s����,%s��ʼ,%s����,Ʊ��%dԪ,��������Ϊ%s\n ", 
					i,movies[i].session,movies[i].name,movies[i].cinema,movies[i].hall,
					movies[i].start,movies[i].end,movies[i].price,movies[i].type);
			}
			printf("��������Ҫ����һ����Ӱ?:(1-%d)\n",Session_Number);
			int Judge;
			scanf("%d",&Judge);
			while(Judge<1||Judge>Session_Number){
				printf("���벻���Ϲ���,����������");
				scanf("%d",&Judge); 
			}
			N=movies[Judge].x;M=movies[Judge].y;
			Purchase();
	    }
	    if(Choice3==2){
	    	for(int i=1;i<=Session_Number;i++)
				for(int j=1;j<=i;j++){
					if(Movie[movies[i].x][movies[i].y].Rest_Ticket<Movie[movies[j].x][movies[j].y].Rest_Ticket){
						kk=movies[i];
						movies[i]=movies[j];
						movies[j]=kk;
					}
				}
			for(int i=1;i<=Session_Number;i++){
				printf("%d. ��Ӱ���Σ�%s,��%s��������%s��%s����,%s��ʼ,%s����,Ʊ��%dԪ,��������Ϊ%s ʣ��Ʊ��%d\n ", 
					i,movies[i].session,movies[i].name,movies[i].cinema,movies[i].hall,
					movies[i].start,movies[i].end,movies[i].price,movies[i].type,Movie[movies[i].x][movies[i].y].Rest_Ticket);
			}
			printf("��������Ҫ����һ����Ӱ?:(1-%d)\n",Session_Number);
			int Judge;
			scanf("%d",&Judge);
			while(Judge<1||Judge>Session_Number){
				printf("���벻���Ϲ���,����������");
				scanf("%d",&Judge); 
			}
			N=movies[Judge].x;M=movies[Judge].y;
			Purchase();
		}
	}
	if(choice==5) Choice_Three(); 
}

void save_Order_Data(){
	FILE* fp1=fopen("Order_Data.txt","a");
	if(!fp1) return;
	int i=Order_Number;
	fprintf(fp1,"%d %d %d %s %s %d %d ",i,Order[i].User,Order[i].Seat_Number,Order[i].Hall_name,Order[i].Belong_Cinema,Order[i].n,Order[i].m);
	for(int j=1;j<=Order[i].Seat_Number;j++)
	    fprintf(fp1,"%d %d ",Order[i].Seat[j][1],Order[i].Seat[j][2]);
	fclose(fp1);
}

//������ ��Ӧ���û� ��λ���� Ӱ���� ���ڵĵ�ӰԺ n m ��λ 
void Purchase(){   
    while(1){  
        printf("����������Ʊ��ʱ��(XX��XX��):\n");
        int month,date;
        scanf("%d %d",&month,&date);
        if(Movie[N][M].Rest_Ticket<=0){  
            printf("�ó���Ʊ������,������ѡ��\n");  
			Search_Movie();
            continue;  
        }  
        if(User[User_Now].Movie[N]){  
            printf("���Ѿ�������һ����Ӱ,�Ƿ���Ҫ�ٴιۿ�?: Y/N\n");  
            char judge=getchar();  
            if (judge=='N') continue;  
        }  
        while(User[User_Now].Rest<Movie[N][M].Price){  
            printf("�����˻�����,���ֵ"); 
			Invest_Rest(User_Now);
        }  
        if (!Judge_User_Time(User_Now)){  
            printf("����ʱ��α�ռ��,������ѡ��\n");
			Search_Movie();  
        }  
        if(User[User_Now].Order_num[Time]>=5){  
            printf("�����ն���������,������ѡ��ۿ�����\n");  
            continue;
        }  
        ++Order_Number;  
        User[User_Now].Order_num[Time]++;  
        Hall_Now=Movie[N][M].Hall;    
        for(int i=1;i<=Length[Hall_Now];i++) {  
            for(int j=1;j<=Wide[Hall_Now];j++)  
                printf("%d ",(Hall[Movie[N][M].n][Movie[N][M].m].Seat[i][j]>0?1:0));  
            printf("\n");  
        }  
        int x,y,Ss=1;  
        printf("��������Ҫ������λ(���ѡ������)?:\n");
		scanf("%d",&Ss);
		while(Ss!=1&&Ss!=2&&Ss!=3){
			printf("���ֻ��ѡ������,����������\n");
			scanf("%d",&Ss);
		}
        for(int nn=1;nn<=Ss;nn++){  
            scanf("%d%d",&x,&y);
            if(x<=0||x>Length[Movie[N][M].Hall]||y<=0||y>Wide[Movie[N][M].Hall]){
            	printf("���벻�淶,����������");
				nn--;continue; 
		    }
            if(Movie[N][M].Rest_Ticket<=0){  
                printf("��λ����\n");  
                break;  
            } 
			while(User[User_Now].Rest<Movie[N][M].Price){  
            printf("�����˻�����,���ֵ"); 
			Invest_Rest(User_Now);
            }  
            if(!Judge_Hall_Seat(x, y)){  
                printf("���λ���޷���ѡ��\n");  
                nn--;continue;  
            }  
            Change_User();  
            Create_Order(x,y);  
            Change_Hall(x,y);  
            Order[Order_Number].Order_Time=Trans_Month_And_Date(month,date);
        }
        save_Order_Data();
		printf("Ԥ��λ�óɹ�,���������ѯӰƬҳ��");
		system("timeout /t 3");
		Search_Movie();  
    }  
}

int Login_User() {
	char userID[INF];
	char password[20];
	printf("�����û�ID:\n");
	scanf("%s",userID);
	printf("��������:\n");
	scanf("%s",password);
	for(int i=1;i<=User_Number;i++){
		if(strcmp(User[i].User_ID,userID)==0&&strcmp(User[i].password,password)==0){
			printf("��¼�ɹ�,��ӭ %s.\n",User[i].name);
            User_Now=i;
			return 1;
		}
	}
	printf("�˺Ż��������\n");
    return 0;
}

int Login_Manager() {
	char managerID[INF];
	char password[20];
	printf("�������ԱID:\n");
	scanf("%s",managerID);
	printf("��������:\n");
	scanf("%s",password);
	for(int i=1;i<=Manager_Number;i++){
		if(strcmp(Manager[i].Manager_ID,managerID)==0&&strcmp(Manager[i].password, password)==0){
			printf("��¼�ɹ�,��ӭ %s.\n",Manager[i].name);
            Manager_Now=i;
			return 1;
		}
	}
	printf("�˺Ż��������.\n");
    return 0;
}

void viewHalls(){
	char sss[50],aaa[50];
	int Now;
	printf("��������Ҫ�鿴��Ӱ������:\n");
	scanf("%s",sss);
	for(int i=1;i<=Hall_Number;i++)
	    if(!strcmp(sss,HALL[i]))
	        Now=i;
	printf("��������Ҫ��ѯ��ʱ��(XX:XX)\n");
	scanf("%s",aaa);
	int TT=Trans_Time(aaa),Judge=0;
//	printf("CESHI:%d %d\n",TT,Now);
	for(int k=1;k<=Hall_N[Now];k++){ 
	    if(Hall[Now][k].Start_Time<=TT&&TT<=Hall[Now][k].End_Time){
	    	Judge=1;
			for(int i=1;i<=Length[Now];i++){
				for(int j=1;j<=Wide[Now];j++){
					printf("%d ",Hall[Now][k].Seat[i][j]>0?1:0);//��ӡ��λ�Ŀ���״̬
				}
				printf("\n");
			}
		}
	}
	if(!Judge) printf("δ��ѯ����Ӱ���ڴ�ʱ���ӳ��Ӱ\n");
	system("timeout /t 3");
	Manager_Operation();
}//�鿴Ӱ��

int Judge_Hall_Time(int HN){
	for(int i=Trans_Time(New_Movie.start)-15;i<=Trans_Time(New_Movie.end)+15;i++)
	    if(HALL_T[HN][i]) return 0;
	for(int i=Trans_Time(New_Movie.start)-15;i<=Trans_Time(New_Movie.end)+15;i++)  
	    HALL_T[HN][i]=1;
	return 1;
}

void addSchedule(){
    printf("�����Ӱ����:\n");
    scanf("%s",New_Movie.name);
    printf("�����Ӱ���κ�:\n");
    scanf("%s",New_Movie.session);
    printf("�����Ӱ����Ӱ��:\n");
    scanf("%s",New_Movie.hall);
    int HN;
    for(int i=1;i<=Hall_Number;i++)
        if(!strcmp(HALL[i],New_Movie.hall))
            HN=i;
    printf("�����Ӱ��ʼʱ��:\n");
    scanf("%s",New_Movie.start);
    printf("�����Ӱ����ʱ��:\n");
    scanf("%s",New_Movie.end);
    while(!Judge_Hall_Time(HN)){
    	printf("ʱ�����������γ�ͻ,����������\n");
    	printf("�����Ӱ��ʼʱ��:\n");
    	scanf("%s",New_Movie.start);
    	printf("�����Ӱ����ʱ��:\n");
    	scanf("%s",New_Movie.end);
    }
    printf("�����Ӱ�۸�:\n");
    scanf("%d",&New_Movie.price);
    printf("�����Ӱ����:\n");
    scanf("%s",New_Movie.type);
    New_Movie.length=Trans_Time(New_Movie.end)-Trans_Time(New_Movie.start);
    Session_Number++;
    movies[Session_Number]=New_Movie;
    save_Manager_Data();
    Load_Movie_Data();
    printf("ӰƬ��ӳɹ�\n");
    system("timeout /t 3");
    Manager_Operation();
}//����Ա������Ƭ��Ϣ

void viewSchedules(){
	printf("��Ƭ��Ϣ:\n");
	for(int i=1;i<=Session_Number;i++) {
		printf("��%s�������� %s ��%s����,%s��ʼ,%s����,����%d����,Ʊ��%dԪ,��������Ϊ%s\n",
		movies[i].name,movies[i].cinema,movies[i].hall,
		movies[i].start,movies[i].end,movies[i].length,movies[i].price,movies[i].type);
	}
	system("timeout /t 10");
	Manager_Operation();
}

//����Ա������Ϣ�޸�
void Rewrite_Manager(){
	FILE* fp1=fopen("Manager_Data.txt","wb");
	if(!fp1) return;
	for(int i=1;i<=2;i++)
	    fprintf(fp1,"%s %s %s %s %s %s ",Manager[i].Manager_ID,Manager[i].Cinema,Manager[i].name,Manager[i].phone,Manager[i].password,Manager[i].email);
	fclose(fp1);
}

void change_Manger_Password(){
	char Judge[INF],Now[INF];
	printf("��������ԭ��������:\n");
	scanf("%s",Judge);
	while(strcmp(Judge,Manager[Manager_Now].password)){
		printf("�������,����������\n");
		scanf("%s",Judge); 
	}
	printf("����������:\n");
	scanf("%s",Now); 
	strcpy(Manager[Manager_Now].password,Now);
	Rewrite_Manager();
	printf("�����޸ĳɹ�\n");
	system("timeout /t 3"); 
	personalInformation();
}//�޸����뺯��

void change_Manager_Email(){
	char Now[INF];
	printf("������������\n"); 
	scanf("%s",Now);
	while(!isValidEmail(Now)){
		printf("������Ч,����������\n");
		scanf("%s",Now); 
	}
	strcpy(Manager[Manager_Now].email,Now);
	Rewrite_Manager();
	printf("�����޸ĳɹ�\n");
	system("timeout /t 3"); 
	personalInformation(); 
}//�޸����亯��

void change_Manager_Name(){
	char Now[INF];
	printf("��������������\n");
	scanf("%s",Now); 
	strcpy(Manager[Manager_Now].name,Now);
	Rewrite_Manager();
	printf("�����޸ĳɹ�\n");
	system("timeout /t 3"); 
	personalInformation();
}//�޸���������

void viewPersonalInfo(){
	printf("����id:%s",Manager[Manager_Now].Manager_ID);
	printf("���룺%s",Manager[Manager_Now].password);
	printf("����ӰԺ��%s",Manager[Manager_Now].Cinema);
	printf("�����ַ��%s",Manager[Manager_Now].email);
	printf("������%s",Manager[Manager_Now].name);
}//��ӡ������Ϣ����

void personalInformation() {
	struct manager info={"example@example.com","password123","Default Hall","Default Name"};
	int choice;
	printf("\n����Ա������Ϣ�޸�\n");
	printf("1. ����\n");
	printf("2. ����\n");
	printf("3. ����\n");
	printf("4.�鿴������Ϣ\n");
	printf("5. �˳�ϵͳ\n");
	printf("������Ҫ���еĲ���(1-5): ");
	scanf("%d", &choice);
	while(choice<1||choice>5){
		printf("���벻���Ϲ涨,����������:\n");
		scanf("%d",&choice);
	}
	switch(choice){
        case 1:
			change_Manger_Password();
			break;
		case 2:
			change_Manager_Email();
			break;
		case 3:
			change_Manager_Name();
			break;
		case 4:
			viewPersonalInfo();
			break;
		case 5:
			printf("�˳���...\n");
			break;
		default:
			printf("û�и�ѡ�� ����������\n");
	}
	system("timeout /t 3");
	Manager_Operation();
}//�޸ĸ�����Ϣѡ����ת������

void Search_Order(){
    for(int i=1;i<=Order_Number;i++){
		if(strcmp(Order[i].Belong_Cinema,Manager[Manager_Now].Cinema)) continue;
		int Useri=Order[i].User,n=Order[i].n,m=Order[i].m;
		int month=Trans_Month_Back(Order[i].Order_Time),date=Trans_date_Back(Order[i].Order_Time);
		printf("%s��%d��%d�Ź����˳��κ�Ϊ%s����λ:\n",User[Useri].name,month,date,Movie[n][m].session);
		for(int j=1;j<=Order[i].Seat_Number;j++)
		    printf("��%d��%d����  ",Order[i].Seat[j][1],Order[i].Seat[j][2]);
	}//�鿴�û��������
	struct movie moviei[INF];
	int sss=0,ii[INF],jj[INF];
	for(int i=1;i<=Movie_Number;i++)
	    for(int j=1;j<=Movie_N[i];j++)
		    if(Movie[i][j].Hall/4+1==Manager_Now)
			    moviei[++sss]=Movie[i][j],ii[sss]=i,jj[sss]=j;
	printf("\n�����������ʲô����:1.���ѹ�Ʊ��������2.�����������ʽ�������3.��������Ʊ������\n");
	int choice;
	scanf("%d",&choice);
	while(choice<1||choice>3){
		printf("���벻���Ϲ涨,����������\n");
		scanf("%d",&choice);
	}
	if(choice==1){
		for(int i=1;i<=sss;i++)
		    for(int j=1;j<=i;j++)
			    if(Hall[ii[i]][jj[i]].Rest>Hall[ii[j]][jj[j]].Rest){
					ii[j]^=ii[i]^=ii[j]^=ii[i];
					jj[j]^=jj[i]^=jj[j]^=jj[i];
					struct movie k=moviei[i];
					moviei[i]=moviei[j];
					moviei[j]=k;
				}//�ѹ�Ʊ��������
		for(int i=1;i<=sss;i++)
		    printf("���κ�:%s,��Ӱ��:%s,Ӱ����:%s,��Ʊ:%d\n",moviei[i].session,moviei[i].name,moviei[i].Hall_Name,Hall[ii[i]][jj[i]].Rest);
	}
	if(choice==2){
		for(int i=1;i<=sss;i++)
		    for(int j=1;j<=i;j++)
			    if(Hall[ii[i]][jj[i]].Number>Hall[ii[j]][jj[j]].Number){
					ii[j]^=ii[i]^=ii[j]^=ii[i];
					jj[j]^=jj[i]^=jj[j]^=jj[i];
					struct movie k=moviei[i];
					moviei[i]=moviei[j];
					moviei[j]=k;
				}//����������������
		for(int i=1;i<=sss;i++)
		    printf("���κ�:%s,��Ӱ��:%s,Ӱ����:%s,�۳���:%d\n",moviei[i].session,moviei[i].name,moviei[i].Hall_Name,Hall[ii[i]][jj[i]].Number);
	}
	if(choice==3){
		for(int i=1;i<=sss;i++)
		    for(int j=1;j<=i;j++)
			    if(Hall[ii[i]][jj[i]].Number*(Movie[Hall[ii[i]][jj[i]].movie1][Hall[ii[i]][jj[i]].movie2].Price)>Hall[ii[j]][jj[j]].Number*(Movie[Hall[ii[j]][jj[j]].movie1][Hall[ii[j]][jj[j]].movie2].Price)){
					ii[j]^=ii[i]^=ii[j]^=ii[i];
					jj[j]^=jj[i]^=jj[j]^=jj[i];
					struct movie k=moviei[i];
					moviei[i]=moviei[j];
					moviei[j]=k;
				}//��Ʊ������������
		for(int i=1;i<=sss;i++)
		    printf("���κ�:%s,��Ӱ��:%s,Ӱ����:%s,��Ʊ��:%d\n",moviei[i].session,moviei[i].name,moviei[i].Hall_Name,Hall[ii[i]][jj[i]].Number*(Movie[Hall[ii[i]][jj[i]].movie1][Hall[ii[i]][jj[i]].movie2].Price));
	}
	system("timeout /t 3");
	Manager_Operation();
}

void Manager_Operation(){
	int choice;
	printf("��ӰԺ����Ա����ϵͳ\n");
	printf("1. �鿴Ӱ��\n");
	printf("2. �����Ƭ\n");
	printf("3. �鿴��Ƭ\n");
	printf("4. �޸ĸ�����Ϣ\n");
	printf("5. ��������\n");
	printf("6. ��ϵͳ\n");
	printf("������Ҫ���еĲ���(1-6): \n");
	scanf("%d", &choice);
	while(choice<1||choice>6){
		printf("���벻���Ϲ���,����������\n");
		scanf("%d",&choice);
	}
	switch(choice){
		case 1:
			viewHalls();
			break;
		case 2:
			addSchedule();
			break;
		case 3:
			viewSchedules();
			break;
		case 4:
			personalInformation();
			break;
		case 5:
		    Search_Order();
		case 6:
			printf("�˳���...\n");
			exit(0);
			break;
		    
	}
}//����Ա����ѡ����ת������

//User������Ϣ�޸�

void Rewrite_User(){
	FILE* fp1=fopen("User_Data.txt","wb");
	if(!fp1) return;
	for(int i=1;i<=User_Number;i++)
	    fprintf(fp1,"%s %s %s %s %s %s ",User[i].User_ID,User[i].name,User[i].gender,User[i].phone,User[i].password,User[i].email);
	fclose(fp1);
}

void change_User_Password(){
	char Judge[INF],Now[INF];
	printf("��������ԭ��������:\n");
	scanf("%s",Judge);
	while(strcmp(Judge,User[User_Now].password)){
		printf("�������,����������\n");
		scanf("%s",Judge); 
	}
	printf("����������:\n");
	scanf("%s",Now); 
	strcpy(User[User_Now].password,Now);
	Rewrite_User();
	printf("�����޸ĳɹ�\n");
	system("timeout /t 3"); 
	Change_User_Information();
}

void change_User_Email(){
	char Now[INF];
	printf("������������\n"); 
	scanf("%s",Now);
	while(!isValidEmail(Now)){
		printf("������Ч,����������\n");
		scanf("%s",Now); 
	}
	strcpy(User[User_Now].email,Now);
	Rewrite_User();
	printf("�����޸ĳɹ�\n");
	system("timeout /t 3"); 
	Change_User_Information(); 
}

void change_User_Name(){
	char Now[INF];
	printf("��������������\n");
	scanf("%s",Now); 
	strcpy(User[User_Now].name,Now);
	Rewrite_User();
	printf("�����޸ĳɹ�\n");
	system("timeout /t 3"); 
	Change_User_Information();
}

void view_User_PersonalInfo(){
	printf("����id:%s\n",User[User_Now].User_ID);
	printf("���룺%s\n",User[User_Now].password);
	printf("�����ַ��%s\n",User[User_Now].email);
	printf("������%s\n",User[User_Now].name);
	system("timeout /t 10");
	Change_User_Information();
}

void Change_User_Information(){
	int choice;
	printf("�û�������Ϣ�޸�\n");
	printf("1. ����\n");
	printf("2. ����\n");
	printf("3. ����\n");
	printf("4.�鿴������Ϣ\n");
	printf("5. �˳�ϵͳ\n");
	printf("������Ҫ���еĲ���(1-5):\n");
	scanf("%d", &choice);
	while(choice<1||choice>5){
		printf("���벻���Ϲ涨,����������:\n");
		scanf("%d",&choice);
	}
	switch(choice){
        case 1:
			change_User_Password();
			break;
		case 2:
			change_User_Email();
			break;
		case 3:
			change_User_Name();
			break;
		case 4:
			view_User_PersonalInfo();
			break;
		case 5:
			printf("�˳���...\n");
			system("timeout /t 3");
			Choice_Three();
			break;
		default:
			printf("û�и�ѡ�� ����������\n");
	}
}

void Load_Hall_Data(){
	FILE* fp1=fopen("Hall_Data.txt","r");
	Hall_Number=1;
	while(fscanf(fp1,"%s",HALL[Hall_Number])!=EOF) Hall_Number++;
}

void Search_User_Order(){
	int Choice=0;
	printf("1.����ӰƬ����ѯ\n2.�鿴���ж���\n"); 
	scanf("%d",&Choice);
	while(Choice<1||Choice>2){
		printf("���벻���Ϲ涨,����������\n");
		scanf("%d",&Choice); 
	}
	if(Choice==1){
		for(int i=1;i<=Movie_Number;i++) printf("%d.%s\n",i,Movie[i][1].name);
		printf("��ѡ���ѯ��ӰƬ(1-%d)\n",Movie_Number);
		int Movie_n;
		scanf("%d",&Movie_n);
		while(Movie_n<1||Movie_n>Movie_Number){
			printf("���벻���Ϲ涨,����������\n");
			scanf("%d",&Movie_n); 
	    }
		for(int i=1;i<=Order_Number;i++)
		    if(Order[i].User==User_Now&&Order[i].n==Movie_n){
		    	int n=Order[i].n,m=Order[i].m;
//		    	printf("cnmsbjsjsx: n:%d m:%d\n",n,m);
		    	int hour1=Movie[n][m].Start_Time/60,min1=Movie[n][m].Start_Time%60;
		    	int hour2=Movie[n][m].End_Time/60,min2=Movie[n][m].End_Time%60;
			    printf("������:%d ���κ�:%s ��Ӱ����:%s ӰԺ:%s Ӱ��:%s ��ʼʱ��:%d:%d ����ʱ��:%d:%d\n",i,Movie[n][m].session,
			        Movie[n][m].name,Movie[n][m].Cinema_Name,Movie[n][m].Hall_Name,hour1,min1,hour2,min2);
			    printf("��λ��:\n");
				for(int j=1;j<=Order[i].Seat_Number;j++) 
				    printf("%d�� %d��\n",Order[i].Seat[j][1],Order[i].Seat[j][2]); 
				}
	}
	if(Choice==2){
		for(int i=1;i<=Order_Number;i++)
		    if(Order[i].User==User_Now){
		    	int n=Order[i].n,m=Order[i].m;
//		    	printf("cnmsbjsjsx: n:%d m:%d\n",n,m);
		    	int hour1=Movie[n][m].Start_Time/60,min1=Movie[n][m].Start_Time%60;
		    	int hour2=Movie[n][m].End_Time/60,min2=Movie[n][m].End_Time%60;
			    printf("������:%d ���κ�:%s ��Ӱ����:%s ӰԺ:%s Ӱ��:%s ��ʼʱ��:%d:%d ����ʱ��:%d:%d\n",i,Movie[n][m].session,
			        Movie[n][m].name,Movie[n][m].Cinema_Name,Movie[n][m].Hall_Name,hour1,min1,hour2,min2);
			    printf("��λ��:\n");
				for(int j=1;j<=Order[i].Seat_Number;j++) 
				    printf("%d�� %d��\n",Order[i].Seat[j][1],Order[i].Seat[j][2]); 
			}
	}
	printf("����������һҳ��\n");
	system("timeout /t 10");
	Choice_Three();
}

void Choice_Three(){
	int Judge;
	printf("��ѡ������Ҫ���еĲ���:\n"); 
    printf("1.��ѯӰƬ\n2.��ѯ����\n3.�޸ĸ�����Ϣ\n4.�˳�\n");
	scanf("%d",&Judge);
	while(Judge<1||Judge>4){
		printf("���벻���Ϲ���,����������\n");
		scanf("%d",&Judge);
	}
	if(Judge==1) Search_Movie();
	if(Judge==2) Search_User_Order();
	if(Judge==3) Change_User_Information();
	if(Judge==4) exit(0); 
}

int main(){
	printf("��ӭ�����ӰƱ����ϵͳ\n"); 
	printf("------------------------------------------------------------\n");
	Load_User_Data();
	Load_Manager_Data();
	Load_Hall_Data();
	Load_Movie_Data();
	Load_Order_Data();
//	for(int i=1;i<=Hall_N[1];i++) printf("%d->%d\n",Hall[1][i].Start_Time,Hall[1][i].End_Time);
	while(1){
		printf("��������Ҫ����ʲô����:\n"); 
		printf("1.�û�ע��\n2.����Աע��\n3.�û���¼\n4.����Ա��¼\n5.�뿪\n");
        int Choice;
		scanf("%d", &Choice);
        if(Choice==1){
			Register_User();
			continue;
        }
		if(Choice==2){
			if(!Register_Manager()) continue;
        }
		if(Choice==3){
			if(!Login_User()) continue;
			Choice_Three();
        }
		if(Choice==4){
			if(!Login_Manager()) continue;
			Manager_Operation();
			break;
        }
		if(Choice==5){
			printf("Exiting...\n");
			break;
        }
		if(Choice>5||Choice<1){
			printf("���벻���Ϲ涨�������������������\n");
            system("timeout /t 3");     
            continue;
        }
	}
	return 0;
}

