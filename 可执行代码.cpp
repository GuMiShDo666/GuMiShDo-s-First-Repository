#include<stdio.h>  
#include<stdlib.h>  
#include<stdbool.h>  
#include<string.h>
#include<ctype.h>

#define INF 51  
  
int User_Number,Order_Number,N,M,Movie_Number,Hall_Number,User_Now,Hall_Now,Time,Manager_Number,Manager_Now,Session_Number,Cinema_Now;
int Movie_N[INF],Hall_N[INF],Seat_Number[INF],Length[INF],Wide[INF],HALL_T[INF][1441];
char HALL[INF][INF];
char Admin_Key[20]="0123121";//设置管理员权限密码
  
struct user{ 
	//Order_num[i]:表示一年之中的第i天用户的订单数
	//Time[i][j]:表示一年之中的第i天,一天之间的第i分钟用户是否空闲
	//Order[i]表示用户的第i个订单的订单号,Tmp表示用户订单的数量 
    int Order[INF],Time[366][1441],Rest,Order_num[366],Movie[INF],Tmp;
    char User_ID[INF],name[INF],gender[10],phone[INF],password[20],email[INF];
}User[INF],New_User;//User[i]表示编号为i的用户 

struct order{  
    int Seat_Number,Order_Time,Seat[4][3],User,n,m;  
    char Hall_name[INF],Belong_Cinema[INF];
    //Seat_Number表示该订单所包含的座位数量,Order_Time表示订单对应的日期
	//Hall_name表示该订单所对应的影厅编号
	//(Seat[i][1],Seat[i][2])表示该订单第i个具体座位
	//User表示该订单对应的用户编号
	//Belong_Cinema表示该订单属于哪一个电影院(涉及管理员操作)
	//n,m表示该订单的电影是指Movie[n][m] 
}Order[INF];//Order[i]表示订单号为i的订单,Order_Number表示订单数量 
  
struct Hall{  
	char Name[INF];
    int Seat[INF][INF],Start_Time,End_Time,movie1,Number,Rest,movie2;
    //Seat表示该影厅的座位表 Start_Time\End_Time表示开始结束的时间(已经转化成一天中的第几分钟)
	//movie1,movie2表示该场次看的是Movie[movie1][movie2]
	//Number表示已经卖出去的票数 Rest表示余票数 
}Hall[INF][INF];//Hall[i][1--Hall_N[i]]为同一个影厅一天之间不同的场次 
  
struct movie{  
    //Hall表示对应的影厅
	//Seat表示座位图
	//Number表示卖出去的票的数量 Rest_Ticket表示余票数
	//Cinema表示该电影对应的电影院编号 
    int Start_Time,End_Time,Hall,Seat[INF][INF],Number,Price,Time,Rest_Ticket,Cinema,n,m;
	char session[50],name[200],type[50],Hall_Name[50],Cinema_Name[50];
	//session表示场次号 name表示电影名字 
}Movie[INF][INF]; //Movie[i][1--Movie_N[i]]为同一部电影一天之间不同的场次 

struct manager{
	char Manager_ID[INF],name[INF],phone[INF],password[20],email[INF],Cinema[INF];
}Manager[INF],New_Manager;

struct Data{
	char session[50];//场次号
	char name[200];//电影名称
	char cinema[200];//电影院
	char hall[200];//影厅
	int Hall_Index;
	char start[50];//开始时间
	char end[50];//结束时间
	int price,length;//价格
	int rest;//剩余票数
	char type[50];//语言类型
	int x,y;
	//余票 待处理
	//优惠活动 待处理
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
				if(!strcmp(movies[i].cinema,"光影天地")) Movie[j][Movie_N[j]].Cinema=1;
				if(!strcmp(movies[i].cinema,"梦幻影城")) Movie[j][Movie_N[j]].Cinema=2;
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
		if(!strcmp(movies[i].cinema,"光影天地")) Movie[Now][1].Cinema=1;
		if(!strcmp(movies[i].cinema,"梦幻影城")) Movie[Now][1].Cinema=2;
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

int isValidID(const char *Id){//判断ID的输入是否合法
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

int isValidPhone(const char *phone){//判断电话输入是否合法
	if (strlen(phone)!=11)return 0;
	for (int i=0;i<11;i++) {
		if (phone[i]<'0'||phone[i]>'9') return 0;
	}
	return 1;
}

int isValidEmail(const char *email){//判断邮箱输入是否合法
	const char *at=strchr(email,'@');
	if (!at||at==email||!strchr(at,'.')) return 0;
	return 1;
}

void Load_User_Data() {//将初始用户数据库的数据下载下来
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

void save_User_Data(){//将数据库的数据更新（向数据库里导入新数据）
	FILE* fp1=fopen("User_Data.txt", "a");
	if(!fp1){
		printf("无法保存数据.\n");
		return;
	}
	fprintf(fp1,"%s %s %s %s %s %s\n",New_User.User_ID,New_User.name,New_User.gender,New_User.phone,New_User.password,New_User.email);
	fclose(fp1);
}

void save_Manager_Data(){
	FILE* fp1=fopen("Manager_Data.txt", "a");
	if(!fp1){
		printf("无法保存数据.\n");
		return;
	}
	fprintf(fp1,"%s %s %s %s %s %s ",New_Manager.Manager_ID,New_Manager.Cinema,New_Manager.name,New_Manager.phone,New_Manager.password,New_Manager.email);
	fclose(fp1);
}

void Invest_Rest(int Now){
	printf("请输入您充值的数(单位:元):\n");
	int zzh=0;
	scanf("%d",&zzh);
	while(zzh<0){
		printf("请输入正确的数\n");
		scanf("%d",&zzh);
	}
	User[Now].Rest+=zzh;
	printf("充值成功!\n");
	printf("正在返回上一步\n"); 
	system("timeout /t 3"); 
}

void Register_User(){//用户注册
	printf("输入用户ID(6-10 字母与数字的组合):\n");
	scanf("%s", New_User.User_ID);
	while(!isValidID(New_User.User_ID)) {
		printf("用户ID不符合要求.\n");
        system("timeout /t 3");     
        printf("请重新输入:\n");
	    scanf("%s", New_User.User_ID);
	}
	printf("输入名字:\n");
	scanf("%s",New_User.name);
	printf("输入性别:\n");
	scanf("%s",New_User.gender);
	while(strcmp(New_User.gender,"男")&&strcmp(New_User.gender,"女")){
		printf("输入不符合规则,请重新输入\n");
		scanf("%s",New_User.gender);
	}
	printf("输入电话号码(11位):\n");
	scanf("%s",New_User.phone);
	while(!isValidPhone(New_User.phone)) {
		printf("电话号码不符合规则\n");
        system("timeout /t 3");     
        printf("请重新输入:\n");
	    scanf("%s",New_User.phone);
	}
	printf("输入密码:\n");
	scanf("%s",New_User.password);
	printf("输入邮箱:\n");
	scanf("%s",New_User.email);
	while(!isValidEmail(New_User.email)) {
		printf("邮箱地址不符合规则\n");
		system("timeout /t 3");     
        printf("请重新输入:\n");
	    scanf("%s",New_User.email);
	}
	User[++User_Number]=New_User;
	printf("用户注册成功\n");
	printf("当前账户余额为零,是否需要进行充值\n");
	int xlm;
	printf("1.跳转充值页面 2.不进行充值\n");
	scanf("%d",&xlm); 
	while(xlm<1||xlm>2){
		printf("输入不符合规定,请重新输入\n");
		scanf("%d",&xlm); 
    }
    if(xlm==1)Invest_Rest(User_Number);
    New_User.Rest=User[User_Number].Rest;
	save_User_Data();
}

int Register_Manager() {//管理员注册
	char Key[20];
    int Judgement=1;
	printf("输入管理员权限密码：\n");
	scanf("%s", Key);
	while(strcmp(Key,Admin_Key)) {
		printf("密码错误,请重新输入,还剩下%d次机会:\n",5-Judgement);
        Judgement++;
        if(Judgement==5){
            printf("机会已用完,将要跳转回上一页面\n");
            system("timeout \t 3");
            return 0;
        }
	    scanf("%s", Key);
	}
	printf("输入管理员ID(6-10位字母与数字的组合): \n");
	scanf("%s", New_Manager.Manager_ID);
	while(!isValidID(New_Manager.Manager_ID)) {
		printf("输入的ID不符合规则\n");
        system("timeout \t 3");
        printf("请重新输入");
        scanf("%s", New_Manager.Manager_ID);
	}
	printf("输入影院名称: ");
	scanf("%s", New_Manager.Cinema);
	printf("输入名字: ");
	scanf("%s", New_Manager.name);
	printf("输入电话号码(11位): ");
	scanf("%s", New_Manager.phone);
	while(!isValidPhone(New_Manager.phone)) {
		printf("输入的电话号码不符合规则\n");
        system("timeout \t 3");
        printf("请重新输入");
        scanf("%s", New_Manager.phone);
	}
	printf("输入密码:\n");
	scanf("%s", New_Manager.password);
	printf("输入邮箱:\n");
	scanf("%s", New_Manager.email);
	while(!isValidEmail(New_Manager.email)) {
		printf("输入的邮箱不符合规则\n");
        system("timeout \t 3");
        printf("请重新输入");
        scanf("%s", New_Manager.email);
	}
	Manager[Manager_Number++]=New_Manager;
	save_Manager_Data();
	printf("管理员注册成功\n");
    return 1;
}
  
int Trans_Month_And_Date(int x, int y){  
    int month[13]={0,31,28,31,30,31,30,31,31,30,31,30,31},Sum = 0;  
    while(--x) Sum+=month[x];  
    return Sum+y;  
} //查看是一年的第多少天
  
int Trans_Month_Back(int Now){
	int month[13]={0,31,28,31,30,31,30,31,31,30,31,30,31},Sum=1;
	while(Now>month[Sum]) Now-=month[Sum],Sum++;
	return Sum;
}//返回现在的月份

int Trans_date_Back(int Now){
	int month[13]={0,31,28,31,30,31,30,31,31,30,31,30,31},Sum=1;
	while(Now>month[Sum]) Now-=month[Sum],Sum++;
	return Now;
}//返回现在的天数

int Judge_User_Time(int user){  
    int start=Movie[N][M].Start_Time,end=Movie[N][M].End_Time;  
    for(int i=start;i<=end;i++)  
        if(User[user].Time[Time][i])  
            return 0;  
    for(int i=start;i<=end;i++)  
        User[user].Time[Time][i]=1;  
    return 1;  
}//判断用户订票时间是否重复  
  
int Judge_Hall_Seat(int x, int y){
    int n=Movie[N][M].n,m=Movie[N][M].m;  
    if (Hall[n][m].Seat[x][y]) return 0;  
    if (!Hall[n][m].Seat[x][y]&&((Hall[n][m].Seat[x][y - 2]&&Hall[n][m].Seat[x][y-2]!=Order_Number)||(Hall[n][m].Seat[x][y + 2]&&Hall[n][m].Seat[x][y+2]!=Order_Number))) return 0;  
    return 1;  
} //判断订票座位是否只相隔一个座位 
  
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

void Print1(int searchMovie,int j){//显示查询结果
	int Start_H,Start_M,End_H,End_M;
	Start_H=Movie[searchMovie][j].Start_Time/60; 
	Start_M=Movie[searchMovie][j].Start_Time%60;
	End_H=Movie[searchMovie][j].End_Time/60;
	End_M=Movie[searchMovie][j].End_Time%60;
	printf("%s,《%s》今日在%s的%s播放,%d:%d开始,%d:%d结束,共计%d分钟,票价%d元,语言类型为%s\n ", 
    Movie[searchMovie][j].session,Movie[searchMovie][j].name,Movie[searchMovie][j].Cinema_Name,Movie[searchMovie][j].Hall_Name,
	Start_H,Start_M,End_H,End_M,Movie[searchMovie][j].End_Time-Movie[searchMovie][j].Start_Time,Movie[searchMovie][j].Price,Movie[searchMovie][j].type);
}

void Search_Movie(){//影片场次查询
	int choice,searchMovie,searchCinema,Choice1,Judge;
	printf("请输入您想要进行的操作:\n"); 
	printf("影片场次查询\n1.根据影片名查询 \n2. 根据影院名查询\n3. 筛选条件进行排序\n4. 热门场次推荐\n5. 退出\n");
	scanf("%d", &choice);
	while(choice<1||choice>5){
		printf("输入不符合规定,请重新输入:\n");
		scanf("%d",&choice);
	}
    if(choice==1){//根据影片名查询
		printf("根据影片名查询:\n");
		for(int i=1;i<=Movie_Number;i++) printf("%d.%s\n",i,Movie[i][1].name);
		printf("选择影片 (1-%d): \n",Movie_Number);
	   	scanf("%d",&searchMovie);
		while(searchMovie<=0||searchMovie>Movie_Number){
			printf("输入不符合规则,请重新输入\n");
			scanf("%d",&searchMovie);
		}
		for(int j=1;j<=Movie_N[searchMovie];j++) Print1(searchMovie,j);
		printf("增加筛选条件\n1.影厅类型\n2.语言类型\n3.退出\n");
		printf("请选择: \n");
		scanf("%d", &Choice1);
		while(Choice1<1||Choice1>3){
			printf("输入不符合规定,请重新输入:\n");
			scanf("%d",&Choice1);
		}
		//进一步筛选
		if(Choice1==1){//筛选影厅
			printf("请问您想选择哪种影厅?:\n");
			printf("1.3D\n2.4D\n3.IMAX\n");
			int Choice2;
			scanf("%d",&Choice2);
			while(Choice2<1||Choice2>3){
				printf("输入不符合规定,请重新输入:\n");
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
				printf("抱歉,没有该类型的场次\n");
				system("timeout /t 3");
				Search_Movie();
			}
			printf("请问您想要看哪一场电影?:\n");
			scanf("%d",&Judge);
			while(Judge<1||Judge>Index){
				printf("输入不符合规则,请重新输入\n");
				scanf("%d",&Judge);
			}
			N=searchMovie;M=AAA[Judge];
			Purchase();
		}
		if(Choice1==2){//筛选语言
            printf("请问您想要看哪种语言的电影?:\n");
			printf("1.中文\n2.英文\n");
			int Choice2;
			scanf("%d",&Choice2);
			while(Choice2<1||Choice2>2){
				printf("输入不符合规定,请重新输入:\n");
				scanf("%d",&Choice2);
			}
			int Index=0,AAA[INF]={0};
			if(Choice2==1){
			    printf("语言类型为中文的电影有：\n");
				for(int j=1;j<=Movie_N[searchMovie];j++)
					if(!strcmp(Movie[searchMovie][j].type,"中文")){
					   	printf("%d. ",++Index);
					    AAA[Index]=j;
					    Print1(searchMovie,j);
				}
			}
			if(Choice2==2){
				printf("语言类型为英文的电影有：\n");
				for(int j=1;j<=Movie_N[searchMovie];j++)
					if(!strcmp(Movie[searchMovie][j].type,"英文")){
					    printf("%d. ",++Index);
					    AAA[Index]=j;
					    Print1(searchMovie,j);
				}
			}
			if(Index==0){
				printf("抱歉,没有该类型的场次\n");
				system("timeout /t 3");
				Search_Movie();
			}
			int Judge;
			printf("请问您想要看哪一场电影?:\n");
			scanf("%d",&Judge);
			while(Judge<1||Judge>Index){
				printf("输入不符合规则,请重新输入\n");
				scanf("%d",&Judge);
			}
			N=searchMovie;M=AAA[Judge];
			Purchase();
		}
		if(Choice1==3) printf("退出中，请稍候\n");
	}
	if(choice==2){//根据影院名查询
		printf("请问您想要去哪一个影院观影?:\n1.光影天地\n2.梦幻影城\n");
		scanf("%d",&searchCinema);
		while(searchCinema<1||searchCinema>2){
				printf("输入不符合规定,请重新输入:\n");
				scanf("%d",&searchCinema);
		}
		int Index=0,AAA[INF],BBB[INF];
		if(searchCinema==1){//光影天地
			printf("今天在光影天地影厅放映的电影有：\n");
			for(int i=1;i<=Movie_Number;i++)
				for(int j=1;j<=Movie_N[i];j++)
					if(Movie[i][j].Cinema==1){
					    printf("%d. ",++Index);
					    AAA[Index]=i;BBB[Index]=j;
					    Print1(i,j);
				}
		}
		if(searchCinema==2){//梦幻影城
			printf("今天在梦幻影厅放映的电影有：\n");
			for(int i=1;i<=Movie_Number;i++)
				for(int j=1;j<=Movie_N[i];j++)
					if(Movie[i][j].Cinema==2){
					    printf("%d. ",++Index);
					    AAA[Index]=i;BBB[Index]=j;
					    Print1(i,j);
				}
		}
		int Judge;
		printf("请问您想要看哪一场电影?:\n");
		scanf("%d",&Judge);
		while(Judge<1||Judge>Index){
			printf("输入不符合规则,请重新输入\n");
			scanf("%d",&Judge);
		}
		N=AAA[Judge];M=BBB[Judge];
		Purchase();
	}
	if(choice==3){//筛选条件进行排序
		printf("筛选条件进行排序\n1.根据放映开始时间排序 \n2. 根据场次票价排序\n3. 根据场次余票数排序\n4. 退出\n");
		printf("请选择: \n");
		scanf("%d", &Choice1);
		while(Choice1<1||Choice1>4){
			printf("输入不符合规定,请重新输入:\n");
			scanf("%d",Choice1);
		}
		if(Choice1==1)//放映时间排序
			for(int i=1;i<=Session_Number;i++)
				for(int j=1;j<=i;j++){
					if(Trans_Time(movies[i].start)<Trans_Time(movies[j].start)){
						kk=movies[i];
						movies[i]=movies[j];
						movies[j]=kk;
					}
				}
		if(Choice1==2)//场次票价排序
			for(int i=1;i<=Session_Number;i++)
				for(int j=1;j<=i;j++){
					if(movies[i].price<movies[j].price){
						kk=movies[i];
						movies[i]=movies[j];
						movies[j]=kk;
					}
				}
		if(Choice1==3)//场次余票数排序 待处理
			for(int i=1;i<=Session_Number;i++)
				for(int j=1;j<=i;j++){
					if(Movie[movies[i].x][movies[i].y].Rest_Ticket>Movie[movies[i].x][movies[i].y].Rest_Ticket){
						kk=movies[i];
						movies[i]=movies[j];
						movies[j]=kk;
					}
				}
		if(Choice1==4){//退出
				printf("退出中，请稍候\n");
				system("timeout /t 3");
				Search_Movie();
				return;
		}
		for(int i=1;i<=Session_Number;i++){
			printf("%d. 电影场次：%s,《%s》今日在%s的%s播放,%s开始,%s结束,票价%d元,语言类型为%s\n ", 
				i,movies[i].session,movies[i].name,movies[i].cinema,movies[i].hall,
				movies[i].start,movies[i].end,movies[i].price,movies[i].type);
		}
		printf("请问您想要看哪一部电影?:(1-%d)\n",Session_Number);
		int Judge;
		scanf("%d",&Judge);
		while(Judge<1||Judge>Session_Number){
			printf("输入不符合规则,请重新输入\n");
			scanf("%d",&Judge); 
		}
		N=movies[Judge].x;M=movies[Judge].y;
		Purchase();
	}
	if(choice==4){//热门场次推荐 
	    printf("1.票价最低\n2.销量最高\n");
	    int Choice3;
	    scanf("%d",&Choice3);
	    while(Choice3<1||Choice3>2){
	    	printf("输入不符合规定,请重新输入:\n");
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
				printf("%d. 电影场次：%s,《%s》今日在%s的%s播放,%s开始,%s结束,票价%d元,语言类型为%s\n ", 
					i,movies[i].session,movies[i].name,movies[i].cinema,movies[i].hall,
					movies[i].start,movies[i].end,movies[i].price,movies[i].type);
			}
			printf("请问您想要看哪一部电影?:(1-%d)\n",Session_Number);
			int Judge;
			scanf("%d",&Judge);
			while(Judge<1||Judge>Session_Number){
				printf("输入不符合规则,请重新输入");
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
				printf("%d. 电影场次：%s,《%s》今日在%s的%s播放,%s开始,%s结束,票价%d元,语言类型为%s 剩余票数%d\n ", 
					i,movies[i].session,movies[i].name,movies[i].cinema,movies[i].hall,
					movies[i].start,movies[i].end,movies[i].price,movies[i].type,Movie[movies[i].x][movies[i].y].Rest_Ticket);
			}
			printf("请问您想要看哪一部电影?:(1-%d)\n",Session_Number);
			int Judge;
			scanf("%d",&Judge);
			while(Judge<1||Judge>Session_Number){
				printf("输入不符合规则,请重新输入");
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

//订单号 对应的用户 座位数量 影厅名 属于的电影院 n m 座位 
void Purchase(){   
    while(1){  
        printf("请输入您购票的时间(XX月XX日):\n");
        int month,date;
        scanf("%d %d",&month,&date);
        if(Movie[N][M].Rest_Ticket<=0){  
            printf("该场次票已售罄,请重新选择\n");  
			Search_Movie();
            continue;  
        }  
        if(User[User_Now].Movie[N]){  
            printf("您已经看过这一部电影,是否需要再次观看?: Y/N\n");  
            char judge=getchar();  
            if (judge=='N') continue;  
        }  
        while(User[User_Now].Rest<Movie[N][M].Price){  
            printf("您的账户余额不足,请充值"); 
			Invest_Rest(User_Now);
        }  
        if (!Judge_User_Time(User_Now)){  
            printf("您该时间段被占用,请重新选择\n");
			Search_Movie();  
        }  
        if(User[User_Now].Order_num[Time]>=5){  
            printf("您当日订单数已满,请重新选择观看日期\n");  
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
        printf("请问您需要几个座位(最多选择三个)?:\n");
		scanf("%d",&Ss);
		while(Ss!=1&&Ss!=2&&Ss!=3){
			printf("最多只能选择三个,请重新输入\n");
			scanf("%d",&Ss);
		}
        for(int nn=1;nn<=Ss;nn++){  
            scanf("%d%d",&x,&y);
            if(x<=0||x>Length[Movie[N][M].Hall]||y<=0||y>Wide[Movie[N][M].Hall]){
            	printf("输入不规范,请重新输入");
				nn--;continue; 
		    }
            if(Movie[N][M].Rest_Ticket<=0){  
                printf("座位已满\n");  
                break;  
            } 
			while(User[User_Now].Rest<Movie[N][M].Price){  
            printf("您的账户余额不足,请充值"); 
			Invest_Rest(User_Now);
            }  
            if(!Judge_Hall_Seat(x, y)){  
                printf("这个位置无法被选择\n");  
                nn--;continue;  
            }  
            Change_User();  
            Create_Order(x,y);  
            Change_Hall(x,y);  
            Order[Order_Number].Order_Time=Trans_Month_And_Date(month,date);
        }
        save_Order_Data();
		printf("预定位置成功,即将进入查询影片页面");
		system("timeout /t 3");
		Search_Movie();  
    }  
}

int Login_User() {
	char userID[INF];
	char password[20];
	printf("输入用户ID:\n");
	scanf("%s",userID);
	printf("输入密码:\n");
	scanf("%s",password);
	for(int i=1;i<=User_Number;i++){
		if(strcmp(User[i].User_ID,userID)==0&&strcmp(User[i].password,password)==0){
			printf("登录成功,欢迎 %s.\n",User[i].name);
            User_Now=i;
			return 1;
		}
	}
	printf("账号或密码错误\n");
    return 0;
}

int Login_Manager() {
	char managerID[INF];
	char password[20];
	printf("输入管理员ID:\n");
	scanf("%s",managerID);
	printf("输入密码:\n");
	scanf("%s",password);
	for(int i=1;i<=Manager_Number;i++){
		if(strcmp(Manager[i].Manager_ID,managerID)==0&&strcmp(Manager[i].password, password)==0){
			printf("登录成功,欢迎 %s.\n",Manager[i].name);
            Manager_Now=i;
			return 1;
		}
	}
	printf("账号或密码错误.\n");
    return 0;
}

void viewHalls(){
	char sss[50],aaa[50];
	int Now;
	printf("请输入想要查看的影厅名字:\n");
	scanf("%s",sss);
	for(int i=1;i<=Hall_Number;i++)
	    if(!strcmp(sss,HALL[i]))
	        Now=i;
	printf("请输入想要查询的时间(XX:XX)\n");
	scanf("%s",aaa);
	int TT=Trans_Time(aaa),Judge=0;
//	printf("CESHI:%d %d\n",TT,Now);
	for(int k=1;k<=Hall_N[Now];k++){ 
	    if(Hall[Now][k].Start_Time<=TT&&TT<=Hall[Now][k].End_Time){
	    	Judge=1;
			for(int i=1;i<=Length[Now];i++){
				for(int j=1;j<=Wide[Now];j++){
					printf("%d ",Hall[Now][k].Seat[i][j]>0?1:0);//打印座位的空闲状态
				}
				printf("\n");
			}
		}
	}
	if(!Judge) printf("未查询到该影厅在此时间放映电影\n");
	system("timeout /t 3");
	Manager_Operation();
}//查看影厅

int Judge_Hall_Time(int HN){
	for(int i=Trans_Time(New_Movie.start)-15;i<=Trans_Time(New_Movie.end)+15;i++)
	    if(HALL_T[HN][i]) return 0;
	for(int i=Trans_Time(New_Movie.start)-15;i<=Trans_Time(New_Movie.end)+15;i++)  
	    HALL_T[HN][i]=1;
	return 1;
}

void addSchedule(){
    printf("输入电影名字:\n");
    scanf("%s",New_Movie.name);
    printf("输入电影场次号:\n");
    scanf("%s",New_Movie.session);
    printf("输入电影播放影厅:\n");
    scanf("%s",New_Movie.hall);
    int HN;
    for(int i=1;i<=Hall_Number;i++)
        if(!strcmp(HALL[i],New_Movie.hall))
            HN=i;
    printf("输入电影开始时间:\n");
    scanf("%s",New_Movie.start);
    printf("输入电影结束时间:\n");
    scanf("%s",New_Movie.end);
    while(!Judge_Hall_Time(HN)){
    	printf("时间与其他场次冲突,请重新输入\n");
    	printf("输入电影开始时间:\n");
    	scanf("%s",New_Movie.start);
    	printf("输入电影结束时间:\n");
    	scanf("%s",New_Movie.end);
    }
    printf("输入电影价格:\n");
    scanf("%d",&New_Movie.price);
    printf("输入电影类型:\n");
    scanf("%s",New_Movie.type);
    New_Movie.length=Trans_Time(New_Movie.end)-Trans_Time(New_Movie.start);
    Session_Number++;
    movies[Session_Number]=New_Movie;
    save_Manager_Data();
    Load_Movie_Data();
    printf("影片添加成功\n");
    system("timeout /t 3");
    Manager_Operation();
}//管理员输入排片信息

void viewSchedules(){
	printf("排片信息:\n");
	for(int i=1;i<=Session_Number;i++) {
		printf("《%s》今日在 %s 的%s播放,%s开始,%s结束,共计%d分钟,票价%d元,语言类型为%s\n",
		movies[i].name,movies[i].cinema,movies[i].hall,
		movies[i].start,movies[i].end,movies[i].length,movies[i].price,movies[i].type);
	}
	system("timeout /t 10");
	Manager_Operation();
}

//管理员个人信息修改
void Rewrite_Manager(){
	FILE* fp1=fopen("Manager_Data.txt","wb");
	if(!fp1) return;
	for(int i=1;i<=2;i++)
	    fprintf(fp1,"%s %s %s %s %s %s ",Manager[i].Manager_ID,Manager[i].Cinema,Manager[i].name,Manager[i].phone,Manager[i].password,Manager[i].email);
	fclose(fp1);
}

void change_Manger_Password(){
	char Judge[INF],Now[INF];
	printf("请输入您原来的密码:\n");
	scanf("%s",Judge);
	while(strcmp(Judge,Manager[Manager_Now].password)){
		printf("密码错误,请重新输入\n");
		scanf("%s",Judge); 
	}
	printf("输入新密码:\n");
	scanf("%s",Now); 
	strcpy(Manager[Manager_Now].password,Now);
	Rewrite_Manager();
	printf("密码修改成功\n");
	system("timeout /t 3"); 
	personalInformation();
}//修改密码函数

void change_Manager_Email(){
	char Now[INF];
	printf("请输入新邮箱\n"); 
	scanf("%s",Now);
	while(!isValidEmail(Now)){
		printf("邮箱无效,请重新输入\n");
		scanf("%s",Now); 
	}
	strcpy(Manager[Manager_Now].email,Now);
	Rewrite_Manager();
	printf("邮箱修改成功\n");
	system("timeout /t 3"); 
	personalInformation(); 
}//修改邮箱函数

void change_Manager_Name(){
	char Now[INF];
	printf("请输入您的名字\n");
	scanf("%s",Now); 
	strcpy(Manager[Manager_Now].name,Now);
	Rewrite_Manager();
	printf("名字修改成功\n");
	system("timeout /t 3"); 
	personalInformation();
}//修改姓名函数

void viewPersonalInfo(){
	printf("个人id:%s",Manager[Manager_Now].Manager_ID);
	printf("密码：%s",Manager[Manager_Now].password);
	printf("所属影院：%s",Manager[Manager_Now].Cinema);
	printf("邮箱地址：%s",Manager[Manager_Now].email);
	printf("姓名：%s",Manager[Manager_Now].name);
}//打印个人信息函数

void personalInformation() {
	struct manager info={"example@example.com","password123","Default Hall","Default Name"};
	int choice;
	printf("\n管理员个人信息修改\n");
	printf("1. 密码\n");
	printf("2. 邮箱\n");
	printf("3. 姓名\n");
	printf("4.查看个人信息\n");
	printf("5. 退出系统\n");
	printf("输入你要进行的操作(1-5): ");
	scanf("%d", &choice);
	while(choice<1||choice>5){
		printf("输入不符合规定,请重新输入:\n");
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
			printf("退出中...\n");
			break;
		default:
			printf("没有该选项 请重新输入\n");
	}
	system("timeout /t 3");
	Manager_Operation();
}//修改个人信息选择（中转函数）

void Search_Order(){
    for(int i=1;i<=Order_Number;i++){
		if(strcmp(Order[i].Belong_Cinema,Manager[Manager_Now].Cinema)) continue;
		int Useri=Order[i].User,n=Order[i].n,m=Order[i].m;
		int month=Trans_Month_Back(Order[i].Order_Time),date=Trans_date_Back(Order[i].Order_Time);
		printf("%s在%d月%d号购买了场次号为%s的座位:\n",User[Useri].name,month,date,Movie[n][m].session);
		for(int j=1;j<=Order[i].Seat_Number;j++)
		    printf("第%d排%d号座  ",Order[i].Seat[j][1],Order[i].Seat[j][2]);
	}//查看用户订单情况
	struct movie moviei[INF];
	int sss=0,ii[INF],jj[INF];
	for(int i=1;i<=Movie_Number;i++)
	    for(int j=1;j<=Movie_N[i];j++)
		    if(Movie[i][j].Hall/4+1==Manager_Now)
			    moviei[++sss]=Movie[i][j],ii[sss]=i,jj[sss]=j;
	printf("\n请问您想进行什么操作:1.按已购票人数排序2.按场次上座率进行排序3.按场次总票价收入\n");
	int choice;
	scanf("%d",&choice);
	while(choice<1||choice>3){
		printf("输入不符合规定,请重新输入\n");
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
				}//已购票人数排序
		for(int i=1;i<=sss;i++)
		    printf("场次号:%s,电影名:%s,影厅名:%s,余票:%d\n",moviei[i].session,moviei[i].name,moviei[i].Hall_Name,Hall[ii[i]][jj[i]].Rest);
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
				}//按场次上座率排序
		for(int i=1;i<=sss;i++)
		    printf("场次号:%s,电影名:%s,影厅名:%s,售出数:%d\n",moviei[i].session,moviei[i].name,moviei[i].Hall_Name,Hall[ii[i]][jj[i]].Number);
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
				}//按票价总收入排序
		for(int i=1;i<=sss;i++)
		    printf("场次号:%s,电影名:%s,影厅名:%s,总票价:%d\n",moviei[i].session,moviei[i].name,moviei[i].Hall_Name,Hall[ii[i]][jj[i]].Number*(Movie[Hall[ii[i]][jj[i]].movie1][Hall[ii[i]][jj[i]].movie2].Price));
	}
	system("timeout /t 3");
	Manager_Operation();
}

void Manager_Operation(){
	int choice;
	printf("电影院管理员操作系统\n");
	printf("1. 查看影厅\n");
	printf("2. 添加排片\n");
	printf("3. 查看排片\n");
	printf("4. 修改个人信息\n");
	printf("5. 订单管理\n");
	printf("6. 出系统\n");
	printf("输入你要进行的操作(1-6): \n");
	scanf("%d", &choice);
	while(choice<1||choice>6){
		printf("输入不符合规则,请重新输入\n");
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
			printf("退出中...\n");
			exit(0);
			break;
		    
	}
}//管理员操作选择（中转函数）

//User个人信息修改

void Rewrite_User(){
	FILE* fp1=fopen("User_Data.txt","wb");
	if(!fp1) return;
	for(int i=1;i<=User_Number;i++)
	    fprintf(fp1,"%s %s %s %s %s %s ",User[i].User_ID,User[i].name,User[i].gender,User[i].phone,User[i].password,User[i].email);
	fclose(fp1);
}

void change_User_Password(){
	char Judge[INF],Now[INF];
	printf("请输入您原来的密码:\n");
	scanf("%s",Judge);
	while(strcmp(Judge,User[User_Now].password)){
		printf("密码错误,请重新输入\n");
		scanf("%s",Judge); 
	}
	printf("输入新密码:\n");
	scanf("%s",Now); 
	strcpy(User[User_Now].password,Now);
	Rewrite_User();
	printf("密码修改成功\n");
	system("timeout /t 3"); 
	Change_User_Information();
}

void change_User_Email(){
	char Now[INF];
	printf("请输入新邮箱\n"); 
	scanf("%s",Now);
	while(!isValidEmail(Now)){
		printf("邮箱无效,请重新输入\n");
		scanf("%s",Now); 
	}
	strcpy(User[User_Now].email,Now);
	Rewrite_User();
	printf("邮箱修改成功\n");
	system("timeout /t 3"); 
	Change_User_Information(); 
}

void change_User_Name(){
	char Now[INF];
	printf("请输入您的名字\n");
	scanf("%s",Now); 
	strcpy(User[User_Now].name,Now);
	Rewrite_User();
	printf("名字修改成功\n");
	system("timeout /t 3"); 
	Change_User_Information();
}

void view_User_PersonalInfo(){
	printf("个人id:%s\n",User[User_Now].User_ID);
	printf("密码：%s\n",User[User_Now].password);
	printf("邮箱地址：%s\n",User[User_Now].email);
	printf("姓名：%s\n",User[User_Now].name);
	system("timeout /t 10");
	Change_User_Information();
}

void Change_User_Information(){
	int choice;
	printf("用户个人信息修改\n");
	printf("1. 密码\n");
	printf("2. 邮箱\n");
	printf("3. 姓名\n");
	printf("4.查看个人信息\n");
	printf("5. 退出系统\n");
	printf("输入你要进行的操作(1-5):\n");
	scanf("%d", &choice);
	while(choice<1||choice>5){
		printf("输入不符合规定,请重新输入:\n");
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
			printf("退出中...\n");
			system("timeout /t 3");
			Choice_Three();
			break;
		default:
			printf("没有该选项 请重新输入\n");
	}
}

void Load_Hall_Data(){
	FILE* fp1=fopen("Hall_Data.txt","r");
	Hall_Number=1;
	while(fscanf(fp1,"%s",HALL[Hall_Number])!=EOF) Hall_Number++;
}

void Search_User_Order(){
	int Choice=0;
	printf("1.根据影片名查询\n2.查看所有订单\n"); 
	scanf("%d",&Choice);
	while(Choice<1||Choice>2){
		printf("输入不符合规定,请重新输入\n");
		scanf("%d",&Choice); 
	}
	if(Choice==1){
		for(int i=1;i<=Movie_Number;i++) printf("%d.%s\n",i,Movie[i][1].name);
		printf("请选择查询的影片(1-%d)\n",Movie_Number);
		int Movie_n;
		scanf("%d",&Movie_n);
		while(Movie_n<1||Movie_n>Movie_Number){
			printf("输入不符合规定,请重新输入\n");
			scanf("%d",&Movie_n); 
	    }
		for(int i=1;i<=Order_Number;i++)
		    if(Order[i].User==User_Now&&Order[i].n==Movie_n){
		    	int n=Order[i].n,m=Order[i].m;
//		    	printf("cnmsbjsjsx: n:%d m:%d\n",n,m);
		    	int hour1=Movie[n][m].Start_Time/60,min1=Movie[n][m].Start_Time%60;
		    	int hour2=Movie[n][m].End_Time/60,min2=Movie[n][m].End_Time%60;
			    printf("订单号:%d 场次号:%s 电影名称:%s 影院:%s 影厅:%s 开始时间:%d:%d 结束时间:%d:%d\n",i,Movie[n][m].session,
			        Movie[n][m].name,Movie[n][m].Cinema_Name,Movie[n][m].Hall_Name,hour1,min1,hour2,min2);
			    printf("座位号:\n");
				for(int j=1;j<=Order[i].Seat_Number;j++) 
				    printf("%d排 %d座\n",Order[i].Seat[j][1],Order[i].Seat[j][2]); 
				}
	}
	if(Choice==2){
		for(int i=1;i<=Order_Number;i++)
		    if(Order[i].User==User_Now){
		    	int n=Order[i].n,m=Order[i].m;
//		    	printf("cnmsbjsjsx: n:%d m:%d\n",n,m);
		    	int hour1=Movie[n][m].Start_Time/60,min1=Movie[n][m].Start_Time%60;
		    	int hour2=Movie[n][m].End_Time/60,min2=Movie[n][m].End_Time%60;
			    printf("订单号:%d 场次号:%s 电影名称:%s 影院:%s 影厅:%s 开始时间:%d:%d 结束时间:%d:%d\n",i,Movie[n][m].session,
			        Movie[n][m].name,Movie[n][m].Cinema_Name,Movie[n][m].Hall_Name,hour1,min1,hour2,min2);
			    printf("座位号:\n");
				for(int j=1;j<=Order[i].Seat_Number;j++) 
				    printf("%d排 %d座\n",Order[i].Seat[j][1],Order[i].Seat[j][2]); 
			}
	}
	printf("即将返回上一页面\n");
	system("timeout /t 10");
	Choice_Three();
}

void Choice_Three(){
	int Judge;
	printf("请选择您想要进行的操作:\n"); 
    printf("1.查询影片\n2.查询订单\n3.修改个人信息\n4.退出\n");
	scanf("%d",&Judge);
	while(Judge<1||Judge>4){
		printf("输入不符合规则,请重新输入\n");
		scanf("%d",&Judge);
	}
	if(Judge==1) Search_Movie();
	if(Judge==2) Search_User_Order();
	if(Judge==3) Change_User_Information();
	if(Judge==4) exit(0); 
}

int main(){
	printf("欢迎进入电影票购买系统\n"); 
	printf("------------------------------------------------------------\n");
	Load_User_Data();
	Load_Manager_Data();
	Load_Hall_Data();
	Load_Movie_Data();
	Load_Order_Data();
//	for(int i=1;i<=Hall_N[1];i++) printf("%d->%d\n",Hall[1][i].Start_Time,Hall[1][i].End_Time);
	while(1){
		printf("请问您需要进行什么操作:\n"); 
		printf("1.用户注册\n2.管理员注册\n3.用户登录\n4.管理员登录\n5.离开\n");
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
			printf("输入不符合规定，请在三秒后重新输入\n");
            system("timeout /t 3");     
            continue;
        }
	}
	return 0;
}

