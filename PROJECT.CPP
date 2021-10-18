#include<fstream.h>
#include<conio.h>
#include<string.h>
#include<dos.h>
#include<stdio.h>
#include<process.h>

int A;	//global int var
//prototypes of interface functions
void welcome_screen();
void main_screen();
void user_window();
void admin_window();
void inventory_window();
void sales_window_a();
void sales_window_u();
void redir_mainscr();
int try_again(int);
void exit_screen();


//class definition starts
class product
{
	protected:
	int item_number;
	char item_name[25];
	float item_price;
	float quantity;
	void get_pro()
	{
		cout<<"Enter the Item number:";
		cin>>item_number;
		cout<<"Enter the Item name:";
		gets(item_name);
		cout<<"Enter the item price(in Rs.):";
		cin>>item_price;
		cout<<"Enter the Quantity of the item(in Kg/in count):";
		cin>>quantity;
	}
	void put_pro()
	{
		cout<<"Item Number:"<<item_number;
		cout<<"\tItem name:"<<item_name;
		cout<<"\nItem price:"<<item_price;
		cout<<"\tItem quantity:"<<quantity;
	}
	product()
	{
		item_number=0;
		strcpy(item_name,"default");
		item_price=0.0;
		quantity=0.0;
	}
	public:
	int ret_itemno()
	{	return item_number;	}
	char* ret_itemname()
	{	return item_name;	}
	float ret_qty()
	{	return quantity;	}
	float ret_price()
	{	return item_price;	}
};

class inventory:public product
{
	int manu_date[3],exp_date[3];
	int inv_credibility_check(int n)       //n is an internal flag
//n=1 refers to checking it for creation and 0 refers to checking it for updation
	{
		inventory i;
		if(n==1)
		{
			int found=inv_search(item_number,A,i);
//found=1 denotes that a record is already present with the given item_number
			if(found==1)	return 0;
				found=inv_search(item_name,i);
//found=1 denotes that a record is already present with the given item_name
			if(found==1)	return 0;
		}
		if(quantity<0)	return 0;
		if(item_price<0)	return 0;
/* x and y are used to compare manu_date and exp_date
No.of days are calculated and compared*/
		long int x=no_of_days(manu_date);
		long int y=no_of_days(exp_date);
		if(x>y)	return 0;
//negative conditions eliminated,hence record is credible
		return 1;
	}
	public:
	inventory()
	{
		manu_date[0]=manu_date[1]=exp_date[0]=exp_date[1]=1;
		manu_date[2]=exp_date[2]=2019;
	}
	int get_inv()
	{
		get_pro();
		cout<<"Enter date of manufacturing of the item (dd/mm/yyyy):";
		get_date(manu_date);
		cout<<"Enter date of expiry of the item (dd/mm/yyyy):";
		get_date(exp_date);
		int n=inv_credibility_check(1);//record checked during creation
		return n;
	}
	int upd_inv()
	{
		cout<<"Note:\nOnly item name,price,quantity and dates of manufacturing and expiry can be changed\n";
		cout<<"Enter the Item name:";
		gets(item_name);
		cout<<"Enter the price of the item(in Rs.):";
		cin>>item_price;
		cout<<"Enter the Quantity of the item(in Kg/in count):";
		cin>>quantity;
		cout<<"Enter date of manufacturing of the item (dd/mm/yyyy):";
		get_date(manu_date);
		cout<<"Enter date of expiry of the item (dd/mm/yyyy):";
		get_date(exp_date);
		int n=inv_credibility_check(0);	//record checked during updation
		return n;
	}
	void put_inv()
	{
		put_pro();
		cout<<"\nDate of manufacturing(dd/mm/yyyy):"<<manu_date[0]<<'/'<<manu_date[1]<<'/'<<manu_date[2]<<endl;
		cout<<"Date of expiry(dd/mm/yyyy):"<<exp_date[0]<<'/'<<exp_date[1]<<'/'<<exp_date[2]<<"\n\n";
	}

	void changeqty(int qty,char opr)//opr is an internal flag
	{
		if(opr=='-')
			quantity-=qty;
		else if(opr=='+')
			quantity+=qty;
	}
	int *ret_manu_date()
	{	return manu_date;	}
	int *ret_exp_date()
	{	return exp_date;	}
	friend int inv_search(int,int&,inventory&);
	friend int inv_search(char*,inventory&);
	friend void get_date(int*);
	friend long int no_of_days(int*);
}I;

class sales:public product
{
	int date[3];
	float total_cost;
	int sales_credibility_check()
	{
		inventory i;
		int found=inv_search(item_number,A,i);
		long int x=no_of_days(i.ret_manu_date());
		long int y=no_of_days(i.ret_exp_date());
		long int z=no_of_days(date);
		if(found==0)	return 0;           //0 denotes item not found
		if(quantity<0)	return 0;
		if(quantity>i.ret_qty())
			return 0;
		if(z<x || z>y)
			return 0;
		return 1; //negative conditions eliminated,so record is credible
	}

	public:
	sales()
	{
		date[0]=date[1]=1;
		date[2]=2019;
		total_cost=0.0;
	}
	int get_sales()
	{
		inventory i;
		int n;
		cout<<"Enter the Item number:";
		cin>>item_number;
		cout<<"Enter the Quantity(in Kg/in count):";
		cin>>quantity;
		cout<<"Enter  the sales date(dd/mm/yyyy):";
		get_date(date);
		n=sales_credibility_check();
		if(n==1)
		{
			inv_search(item_number,A,i);
			item_price=i.ret_price();
			strcpy(item_name,i.ret_itemname());
			total_cost=quantity*item_price;
		}
		return n;
	}
	int upd_sales()
	{
		cout<<"Note:Only quantity and date can be changed\n";
		cout<<"Enter the Quantity of the item(in Kg/in count):";
		cin>>quantity;
		cout<<"Enter  the sales date(dd/mm/yyyy):";
		get_date(date);
		int n=sales_credibility_check();
		if(n==1)
			total_cost=quantity*item_price;
		return n;
	}
	void put_sales()
	{
		put_pro();
		cout<<"\nSales date(dd/mm/yyyy):"<<date[0]<<'/'<<date[1]<<'/'<<date[2];
		cout<<"\t Total cost:"<<total_cost<<endl;
	}

	int *ret_date()
	{	return date;	}

	friend void get_date(int*);
	friend int date_cmp(int*,int*);
	friend long int no_of_days(int*);
	friend int sales_search(int,int,int&,sales&);
	friend int sales_search(int*);
}S;

//In the program,all sales objects are named s while all inventory objects are named i
//-------------------------------------------------------
//definition of friend functions starts
int inv_search(int itemno,int &rc,inventory &i)
{
	fstream file;
	int found=0;
	rc=0;
	file.open("inventory.dat",ios::in|ios::binary);
	while(file.read((char*)&i,sizeof(i)))
	{
		if(i.ret_itemno()==itemno)
		{
			found=1;//record found
			break;
		}
		rc++;

	}
	file.close();
	return found;
}

int inv_search(char* itemname,inventory &i)
{
	fstream file;
	int found=0,rc=0;
	file.open("inventory.dat",ios::in|ios::binary);
	while(file.read((char*)&i,sizeof(i)))
	{	if(strcmpi(itemname,i.ret_itemname())==0)
		{
			found=1;//record found
			break;
		}
		rc++;
	}
	file.close();
	return found;
}
void get_date(int a[])
{
	char slash;
	cin>>a[0]>>slash>>a[1]>>slash>>a[2];
}

int date_cmp(int *date1,int *date2)
{
	int match=0;
	if(*date1==*date2)
		if(*(date1+1)==*(date2+1))
			if(*(date1+2)==*(date2+2))
				match=1;
	return match;
}
long int no_of_days(int date[])
{
     long int d=date[0]+date[1]*30+date[2]*365;
     return d;
}

int sales_search(int itemno,int pos,int &rc,sales &s)
{
	fstream file;
	int found=0;
	file.open("sales.dat",ios::in|ios::binary);
	file.seekp(pos*sizeof(s),ios::beg);
	while(file.read((char*)&s,sizeof(s)))
	{
		if(s.ret_itemno()==itemno)
		{
			found=1;//record found
			break;
		}
		rc++;
	 }
	if(found==1)
	{
		cout<<"searched record contents:\n";
		s.put_sales();
	}
	file.close();
	return found;
}

int sales_search(int* date)
{
	fstream file;
	int found=0,rc=0;
	file.open("sales.dat",ios::in|ios::binary);
	while(file.read((char*)&S,sizeof(S)))
	{	if(date_cmp(date,S.ret_date())==1)
		{
			found=1;//record found
			S.put_sales();
			cout<<"\n";
		}
		rc++;
	}
	file.close();
	return found;
}

//----------------------------------------------------------------------
//definition of other (file handling) functions(non-member & non-friendly) start here
void today_sales()
{
	int date[3],found,z;
	cout<<"\n\nEnter today's date(dd/mm/yyyy):";
	get_date(date);
	cout<<"\nToday's sales:\n\n";
	found=sales_search(date);
	if(found==0)
	{
		cout<<"\nNo sales took place today\n";
		z=try_again(0);
		if(z==1)
			today_sales();
	}
}

void current_stock()
{
	fstream file;
	file.open("inventory.dat",ios::in|ios::binary);
	cout<<"\nCurrent stock:\n\n";
	while(file.read((char*)&I,sizeof(I)))
		I.put_inv();
	file.close();
}
void end_of_day_report()
{
	clrscr();
	cout<<"\t\t";
	textcolor(GREEN);
	cputs("END-OF-DAY REPORT");
	cout<<endl;
	textcolor(7);
	today_sales();
	cout<<"\nPress any key to continue";
	getch();
	clrscr();
	current_stock();
	cout<<"\nPress any key to continue";
	getch();delline();
}

void sales_write(sales s,int rc)
{
	fstream file;
	file.open("sales.dat",ios::in|ios::out|ios::binary);
	file.seekg(rc*sizeof(s),ios::beg);
	file.write((char*)&s,sizeof(s));
	file.close();
}

void inv_write(inventory i,int rc)
{
	fstream file;
	file.open("inventory.dat",ios::in|ios::out|ios::binary);
	file.seekg(rc*sizeof(i),ios::beg);
	file.write((char*)&i,sizeof(i));
	file.close();
}

void sales_create()
{
	clrscr();
	cout<<"\t\t";
	textcolor(GREEN);
	cputs("Create sale entry");
	cout<<"\n\n";
	textcolor(7);
	fstream file,file1;
	sales s;
	inventory i;
	int check,rc;
	file.open("sales.dat",ios::app|ios::binary);
	file1.open("inventory.dat",ios::in|ios::out|ios::binary);
	char ch='y';
	while(ch == 'y')
	{
		check=s.get_sales();
		if(check==0)
		{
			if(try_again(1)==1)
				continue;
		       else
				break;
		}
		inv_search(s.ret_itemno(),rc,i);
		i.changeqty(s.ret_qty(),'-');
		file1.seekg(rc*sizeof(i),ios::beg);
		file1.write((char*)&i,sizeof(i));
		file.write((char*)&s,sizeof(s));
		cout<<"Want to enter more?(y/n):";
		cin>>ch;
		cout<<endl;
	}
	clrscr();
	cout<<"sale entries successfully created!";
	file.close();
	file1.close();
}

void inv_create()
{
	clrscr();
	cout<<"\t\t";
	textcolor(GREEN);
	cputs("Create fooditems");
	cout<<"\n\n";
	textcolor(7);
	fstream file;
	inventory i;
	int check;
	file.open("inventory.dat",ios::app|ios::binary);
	char ch='y';
	while(ch == 'y')
	{
			check=i.get_inv();
			if(check==0)
			{
				if(try_again(1)==1)
					continue;
				else
					break;
			}
			file.write((char*)&i,sizeof(i));
			cout<<"Want to enter more?(y/n):";
			cin>>ch;
			cout<<endl;
	}
	clrscr();
	cout<<"Food items successfully created!";
	file.close();
}


void sales_modify()
{
	clrscr();
	cout<<"\t\t";
	textcolor(GREEN);
	cputs("Modify a sale entry");
	cout<<"\n\n";
	textcolor(7);
	sales s;
	inventory i;
	int itemno,pos=0,rc=0,rc1=0,found,check,z=1;
	char ans;
	cout<<"Enter the Item number to be searched:";
	cin>>itemno;
	while(1)
	{
		found=sales_search(itemno,pos,rc,s);
		if(found==1)//sales record is found
		{

			cout<<"\nIs this the record to be modified(y/n)?";
			cin>>ans;
			if(ans=='y' || ans=='Y')
				break;
			else
			{
				cout<<"Do you wish to continue search(y/n)?";
				cin>>ans;
			}
			if(ans=='y' || ans=='Y')
			{       rc++;
				pos=rc;
				continue;
			}
//found is reset to 0 to prevent further processing
			else
			{	found=0;break;	}
		}
		else
			break;
	}
	if(found==1)
	{
			found=inv_search(itemno,rc1,i);
			i.changeqty(s.ret_qty(),'+');
			while(1)
			{
				cout<<"Enter New data\n";
				check=s.upd_sales();						//to be changed
				if(check==0)
				{
					z=try_again(1);
					if(z==1)
						continue;
					else
						break;
				}
				break;
			}
			if(z==1)
			{
				i.changeqty(s.ret_qty(),'-');
				sales_write(s,rc);
				inv_write(i,rc1);
				clrscr();
				cout<<"\n updation successful!";
			}
	}
	else
	{
		cout<<"Item not found";
				z=try_again(0);
				if(z==1)
					sales_modify();
	}
}
void inv_modify()
{
	clrscr();
	cout<<"\t\t";
	textcolor(GREEN);
	cputs("Modify a fooditem");
	cout<<"\n\n";
	textcolor(7);
	inventory i;
	int itemno,rc=0,found=0,check,z=1;
	cout<<"Enter the Item number to be searched:";
	cin>>itemno;
	found=inv_search(itemno,rc,i);
	if(found==1)
	{
		cout<<"searched record contents:\n";
		i.put_inv();
		while(1)
		{
			cout<<"Enter New data\n";
			check=i.upd_inv();
			if(check==0)
			{
				z=try_again(1);
				if(z==1)
					continue;
				else
					break;
			}
			break;
		}
		if(z==1)
		{
			inv_write(i,rc);
			clrscr();
			cout<<"\nupdation successful!";
		}
	}
	else
	{
		cout<<"Item not found";
				z=try_again(0);
				if(z==1)
					inv_modify();
	}
}

void sales_delete()
{
	clrscr();
	cout<<"\t\t";
	textcolor(GREEN);
	cputs("Delete a sale entry");
	cout<<"\n\n";
	textcolor(7);
	fstream file,file1;
	file.open("sales.dat",ios::in|ios::binary);
	file1.open("temp.dat",ios::out|ios::binary);
	sales s;
	char ans;
	int itemno,found=0,pos=0,rc=0,z;
	cout<<"Enter the Item number to be searched:";
	cin>>itemno;
	while(1)
	{
		found=sales_search(itemno,pos,rc,s);
		if(found==1)
		{
			cout<<"\nIs this the record to be deleted(y/n)?";
			cin>>ans;
			if(ans=='y' || ans=='Y')
				break;
			else
			{
				cout<<"Do you wish to continue search(y/n)?";
				cin>>ans;
			}
			if(ans=='y' || ans=='Y')
			{
				rc++;
				pos=rc;
				continue;
			}
			else
			{	found=0;break;	}
		}
		else
			break;
	}
	if(found==1)
	{
		cout<<"Are you sure u want to delete this record(y/n)?";
		cin>>ans;
		if(ans=='y' || ans=='Y')
		{

			pos=0;
			while(file.read((char*)&s,sizeof(s)))
			{
				if(pos!=rc)
				{
					file1.write((char*)&s,sizeof(s));
				}
				pos++;
			}
			clrscr();
			cout<<"Record deleted successfully!";
			file.close();
			file1.close();
			remove("sales.dat");
			rename("temp.dat","sales.dat");
		}
	}
	file.close();
	file1.close();
	if(found==0)
	{
		cout<<"Item not found";
				z=try_again(0);
				if(z==1)
					sales_delete();
	}
}
void inv_delete()
{
	clrscr();
	cout<<"\t\t";
	textcolor(GREEN);
	cputs("Delete a fooditem");
	cout<<"\n\n";
	textcolor(7);
	fstream file,file1;
	file.open("inventory.dat",ios::in|ios::binary);
	file1.open("temp.dat",ios::out|ios::binary);
	inventory i;
	char ans;
	int itemno,z,found;
	cout<<"Enter the Item number to be searched:";
	cin>>itemno;
	found=inv_search(itemno,A,i);
	if(found==1)
	{
		cout<<"searched record contents:\n";
		i.put_inv();
		cout<<"Are you sure u want to delete this record(y/n)?";
		cin>>ans;
		if(ans=='y' || ans=='Y')
		{
		while(file.read((char*)&i,sizeof(i)))
		{
			if(i.ret_itemno()!=itemno)
			{
			     file1.write((char*)&i,sizeof(i));
			}
		}
		clrscr();
		cout<<"Record deleted successfully!";
		file.close();
		file1.close();
		remove("inventory.dat");
		rename("temp.dat","inventory.dat");
		}
	}
	file.close();
	file1.close();
	if(found==0)
	{
		cout<<"Item not found";
				z=try_again(0);
				if(z==1)
					inv_delete();
	}
}


//-----------------------------------------------------
//Interface starts here

void main()
{
	welcome_screen();
}

void welcome_screen()
{
	clrscr();
	cout<<"\t\t\t\t";
	textcolor(LIGHTBLUE);
	cputs("LLOYDS BAKERY");
	cout<<"\n\n\n";
	textcolor(CYAN);
	cputs("We welcome you to one of the finest bakeries in the world!!");
	cout<<"\n\n\n\n";delay(1000);
	cputs("Understated Elegance! ");delay(500);
	cputs("Unquestionable Taste! ");delay(500);
	cputs("Uncompromising Quality! ");delay(500);
	cout<<"\n\n\n\n";
	cputs("If you can dream it,we can bake it!!");
	delay(1000);
	cout<<"\n\n\n\n";
	cputs("There's food for every thought,cos' we give thought to every food!!!");
	textcolor(7);delay(500);
	cout<<"\n\n\n\nPress any key to enter the bakery";
	getch();
	main_screen();
}
void main_screen()
{
	clrscr();
	char username[5],password[7];
	int pass=0,i;
	cout<<"\t\t\t\t";
	textcolor(LIGHTBLUE);
	cputs("LLOYDS BAKERY");
	cout<<"\n\n\t";
	textcolor(YELLOW);
	cputs("MAIN SCREEN");
	textcolor(7);
	cout<<"\n\nLOGIN\n";
	cout<<"\nEnter Username:";
	gets(username);
	cout<<"\nEnter password(max.7 characters):";
	for(i=0;i<7;i++)
	{
		password[i]=getch();
		cout<<'*';
	}
	password[i]='\0';
	delay(500);
	if(strcmp(username,"admin")==0)
	{	if(strcmp(password,"Admin07")==0)
		{	pass=1;
			admin_window();
		}
	}
	else if(strcmp(username,"user")==0)
	{
		if(strcmp(password,"User007")==0)
		{	pass=1;
			user_window();
		}
	}
	if (pass==0)
	{
		cout<<"\n\n";
		textcolor(RED);
		cputs("INCORRECT USERNAME OR PASSWORD");
		textcolor(7);
		i=try_again(0);
		if(i==1)
			main_screen();
		else
			exit_screen();
	}
}

void admin_window()
{
	clrscr();
	int choice;
	textcolor(YELLOW);cout<<"\n\t";
	cputs("Welcome Admin");
	textcolor(7);
	cout<<"\nSelect a place:";
	cout<<"\n1.Inventory\n2.Sales\n3.exit\n";
	cout<<"\nEnter the choice(1/2/3):";
	cin>>choice;
	switch(choice)
	{
		case 1:
			inventory_window();
			break;
		case 2:
			sales_window_a();
			break;
		case 3:
			exit_screen();
			break;
		default:
			textcolor(RED);
			cputs("Invalid choice!");
			textcolor(7);
			delay(3000);
			admin_window();
	}
}

void user_window()
{
	clrscr();
	int choice;
	textcolor(YELLOW);cout<<"\n\t";
	cputs("Welcome User");
	textcolor(7);
	cout<<"\nYou can only access the sales module\n";
	cout<<"\nRedirecting you to the sales window";
	delay(2000);
	sales_window_u();
}
void inventory_window()
{
	clrscr();
	int choice,z;
	textcolor(LIGHTBLUE);cout<<"\n\t";
	cputs("Welcome to Inventory window");
	textcolor(7);
	cout<<"\nSelect an action:";
	cout<<"\n1.Create fooditems";
	cout<<"\n2.Search a fooditem using item number";
	cout<<"\n3.Search a fooditem using food name";
	cout<<"\n4.Modify a fooditem's details";
	cout<<"\n5.Delete a fooditem";
	cout<<"\n6.View END-OF-DAY reports";
	cout<<"\n7.Exit";
	cout<<"\nEnter the choice(1/2/3/4/5/6/7):";
	cin>>choice;
	switch(choice)
	{
		case 1:
			inv_create();
			break;
		case 2:
			clrscr();
			textcolor(GREEN);
			cputs("Search a fooditem using item number");
			textcolor(7);
			int item_no;
			cout<<"\n\n\nEnter Item number to be searched:";
			cin>>item_no;
			z=inv_search(item_no,A,I);
			if(z==1)
			{
				cout<<"searched record contents:\n";
				I.put_inv();
			}
			else
			{
				cout<<"\nItem not found";
				z=try_again(0);
				if(z==1)
					inventory_window();
			}
			break;
		case 3:
			clrscr();
			char item_name[25];
			textcolor(GREEN);
			cputs("Search a fooditem using item name");
			textcolor(7);
			cout<<"\n\n\nEnter the name of the fooditem to be searched:";
			gets(item_name);
			z=inv_search(item_name,I);
			if(z==1)
			{
				cout<<"searched record contents:\n";
				I.put_inv();
			}
			else
			{
				cout<<"\nItem not found";
				z=try_again(0);
				if(z==1)
					inventory_window();
			}
			break;
		case 4:
			inv_modify();
			break;
		case 5:
			inv_delete();
			break;
		case 6:
			end_of_day_report();
			break;
		case 7:
			exit_screen();
			break;
		default:
			textcolor(RED);
			cputs("Invalid choice!");
			textcolor(7);
			delay(3000);
			inventory_window();
	}

	X:
	cout<<"\nDo you wish to:";
	cout<<"\n1.Perform another action";
	cout<<"\n2.Return to admin window";
	cout<<"\n3.Logout";
	cout<<"\n4.Exit";
	cout<<"\nEnter the choice(1/2/3/4):";
	cin>>choice;
	switch(choice)
	{
		case 1:
			inventory_window();
			break;
		case 2:
			admin_window();
			break;
		case 3:
			redir_mainscr();
			break;
		case 4:
			exit_screen();
			break;
		default:
			textcolor(RED);
			cputs("Invalid choice!");
			textcolor(7);
			delay(3000);
			goto X;
	}
}

void sales_window_a()
{
	clrscr();
	int choice;
	textcolor(LIGHTBLUE);cout<<"\n\t";
	cputs("Welcome to Sales window");
	textcolor(7);
	cout<<"\nSelect an action:";
	cout<<"\n1.Create a sale entry";
	cout<<"\n2.Modify a sale entry";
	cout<<"\n3.Delete a sale entry";
	cout<<"\n4.View END-OF-DAY reports";
	cout<<"\n5.Exit";
	cout<<"\nEnter the choice(1/2/3/4/5):";
	cin>>choice;
	switch(choice)
	{
		case 1:
			sales_create();
			break;
		case 2:
			sales_modify();
			break;
		case 3:
			sales_delete();
			break;
		case 4:
			end_of_day_report();
			break;
		case 5:
			exit_screen();
			break;
		default:
			textcolor(RED);
			cputs("Invalid choice!");
			textcolor(7);
			delay(3000);
			sales_window_a();
	}
	Y:
	cout<<"\nDo you wish to:";
	cout<<"\n1.Perform another action";
	cout<<"\n2.Return to admin window";
	cout<<"\n3.Logout";
	cout<<"\n4.Exit";
	cout<<"\nEnter the choice(1/2/3/4):";
	cin>>choice;
	switch(choice)
	{
		case 1:
			sales_window_a();
			break;
		case 2:
			admin_window();
			break;
		case 3:
			redir_mainscr();
			break;
		case 4:
			exit_screen();
			break;
		default:
			textcolor(RED);
			cputs("Invalid choice!");
			textcolor(7);
			delay(3000);
			goto Y;
	}
}
void sales_window_u()
{
	clrscr();
	int choice;
	textcolor(LIGHTBLUE);cout<<"\n\t";
	cputs("Welcome to Sales window");
	textcolor(7);
	cout<<"\nSelect an action:";
	cout<<"\n1.Create a sale entry";
	cout<<"\n2.Modify a sale entry";
	cout<<"\n3.View END-OF-DAY reports";
	cout<<"\n4.Exit";
	cout<<"\nEnter the choice(1/2/3/4):";
	cin>>choice;
	switch(choice)
	{
		case 1:
			sales_create();
			break;
		case 2:
			sales_modify();
			break;
		case 3:
			end_of_day_report();
			break;
		case 4:
			exit_screen();
			break;
		default:
			textcolor(RED);
			cputs("Invalid choice!");
			textcolor(7);
			delay(3000);
			sales_window_u();
	}
	Z:
	cout<<"\nDo you wish to:";
	cout<<"\n1.Perform another action";
	cout<<"\n2.Logout";
	cout<<"\n3.Exit";
	cout<<"\nEnter the choice(1/2/3):";
	cin>>choice;
	switch(choice)
	{
		case 1:
			sales_window_u();
			break;
		case 2:
			redir_mainscr();
			break;
		case 3:
			exit_screen();
			break;
		default:
			textcolor(RED);
			cputs("Invalid choice!");
			textcolor(7);
			delay(3000);
			goto Z;
	}
}
int try_again(int n)//n is an internal flag used to display the message
{
	char ch;
	textcolor(RED);
	if(n==1)
	{
		cout<<"\n\n";
		cputs("INVALID INPUT!!!");
		cout<<endl;
	}
	textcolor(7);
	cout<<"\nDo you wish to try again?(y/n):";
	cin>>ch;
	if(ch=='y')
	{
		for(int i=3;i>0;i--)
		{
		cout<<i<<".....";
		delay(1000);
		}
		cout<<"\n\n";
		return 1;
	}
	return 0;
}
void redir_mainscr()
{
	cout<<"\nSuccessfully logged out \nRedirecting to main screen\n";
			for(int i=3;i>0;i--)
			{
				cout<<i<<".......";
				delay(1000);
			}
			main_screen();
}
void exit_screen()
{
	clrscr();
	char ch;
	cout<<"\t\t\t\t";
	textcolor(LIGHTBLUE);
	cputs("LLOYDS BAKERY");
	textcolor(7);
	cout<<"\n\n\n";
	cout<<endl;
	cputs("We hope we had met your your expectations and demands successfully");cout<<endl;
	cputs("We deeply regret any inconveniences if any");cout<<endl;
	cputs("We would be delighted to hear your remarks");cout<<endl;
	cputs("or suggestions as to improve our bakery for your pleasant experience");cout<<endl;
	textcolor(7);
	cout<<"\nWould you like to leave a remark(y/n)";
	cin>>ch;
	if(ch=='Y' || ch=='y')
	{
		cout<<"\nEnter your remarks here(enter # to end your remark):\n";
		while(1)
		{
			cin>>ch;
			if(ch=='#')
				break;
		}
	}
	textcolor(GREEN);cout<<"\n";
	cputs("Thank you for visiting our bakery.Hope you visit us again soon!!");
	cout<<"\nPress any key to exit";getch();
	delline();
	textcolor(YELLOW);cout<<endl;
	cputs("GOODBYE");
	delay(2000);
	exit(1);
}
//------------------------------------------------------------------------------------------------
//----------------------------------------------END-OF-PROGRAM------------------------------------
//------------------------------------------------------------------------------------------------

