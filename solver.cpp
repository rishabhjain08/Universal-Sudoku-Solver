//when user inputs a sudoku get it checked there

#include <stdio.h>
#include <stdlib.h>
#include <time.h>								//output() should be after returning 2 in case of fillunfill
#include <termios.h>
#include <unistd.h>


int xcord=110;

////////////////////////////////////////////////////////////////////////////
															//			  //
int line,x,y,l;												//	 MACROS	  //
															//			  //
															////////////////
#define hline for(line=1;line<=64;line++)\
		printf("%c",95);


#define vline(x,y,l)	for(line=1;line<=l;line++)\
			{\
			gotoxy(x,y+line-1);\
			printf("%c",124);\
			}



#define dashhline 	for(line=1;line<=64;line++)\
			printf("~");

#define dashvline(x,y,l,type)	for(line=1;line<=l;line++)\
				{\
				gotoxy(x,y+line-1);\
				if(type==1)\
				printf("{");\
				else\
				printf("}");\
				}
												////////////////////////////
												//						  //
												//		END OF MACROS	  //
												//						  //
////////////////////////////////////////////////////////////////////////////

struct linearization								//
		{											//	This structure just remembers the ith row and
		int itsi,itsj,linearval;					//	jth column for its linear value
		}linear[82];								//	i.e 81 for i=9 and j=9

struct locate										//
		{											//	This structure is made just
		int x,y;									//	to return two int values
		}location;									//	from a function instead of one


struct cant_fill									//
													//	This structure is initialized to 0 as soon as the
		{											//	program starts and whenever fillunfill() funtion is
		int cant[82];  //this 82 is for 81 boxes	//	called it checks all non zero values filled till now
		}cantfill[10]; //this 10 is for 9 no.		//	and changes array elements to 1 wherever it cannot be filled
													//

char getch(void);	

int check(int,int);									//	to this function we pass i and j and it tells if that number can be filled a that position or not

int linearval(int,int);								//	to this function we pass i and j and it return us its linear value i.e for linearval(9,9) = 81

int output(void);									//	this function just outputs the values filled in the sudoku at the point of time it is called

int input(void);									//	this function is called at the beginning of program to take the values from user into the sudoku

int update(void);									//	this function counts the numbers filled into sudoku till that point of time

int gotoxy(int, int);
													//	this is second of three functions which fill the sudoku.
int fill(void);										//	This function check that if a number cannit be filled at 8 places in a 3x3 box than
													//	it should be filled at 9th place
////////////////////////////////
//							  //
//	MOST IMPORTANT FUNCTION	  //
//							  //
//	WHICH COVERS ALL SUDOKUS  //
//							  //
	int fill_random(void);							//	when this fill() function is unable to fill any values further than this recursive function is called
//						      //
//							  //
//							  //
////////////////////////////////					//	this function fills the smallest possible value in a box and that too int the box with least possibility
													// 	and again calls fill() function
int fillunfill(void);								//	If a condition arises such that no number can be further filled in a box due to :
													//	1. Wrong filling than it changes the assumed value to the next possible value in that box
													//	2. Requirment to assume more value it assumes more and do everything recursively

int simplecheck(void);								//	This function runs through the sudoku till it is unable to fill any value further
													//	This function fills a value in a box if a unique entry is being filled

int rowcolumnfill(int);								//	This function is called by fill() function and is a extension of fill() function
													//	It sees that if a number cannot be filled at 8 places in a row or column than to fill it at the 9th place


int save_filling(int);								//	This function saves the sudoku filled till that point of time before assuming any value
													//	Since later it may prove to be wrong.

													//	Since the number chosen when fill_random is called need not be the right one
int restore_filling(int);							//	and if it comes out to be wrong than since the filling had been continued assuming it correct   													//	the values need to be restored which were saved in save_filling() function

int timenumber=-1;

int morethan1=1;

int fillsimpossible(void);							//	This function returns 1 if such a condition has arrived that anny value now being filled into the
													//	sudoku clashes with the previous one.

struct locate grid(int,int);						//	This function returns the first box of each 3x3 box when it is given any i and j inside it

int filling[10][10]=								//
		{											//
		{0,0,0,0,0,0,0,0,0,0},						//
		{0,0,0,0,0,0,0,0,0,0},						//
		{0,0,0,0,0,0,0,0,0,0},						//
		{0,0,0,0,0,0,0,0,0,0},						//
		{0,0,0,0,0,0,0,0,0,0},						//
		{0,0,0,0,0,0,0,0,0,0},						//
		{0,0,0,0,0,0,0,0,0,0},						//
		{0,0,0,0,0,0,0,0,0,0},						//
		{0,0,0,0,0,0,0,0,0,0},						//
		{0,0,0,0,0,0,0,0,0,0}						//
		};											//
													//


int _check=0;										//	This is the counter which keeps track the number of non-zero numbers in the sudoku

int unable=0;										//	If this variable when remains zero at the end of a loop it => that functions algo can no more fill any value
													//	into the sudoku

int ability_fillunfill=0;							//	If this variable when remains zero at the end of a loop => that functions algo can no more fill any value
													//	into the sudoku


int uni_flag1=0;									//	This variable makes sure that fll() functon calls random_filling() functions only once
													//	because once called filling_random() function will call fill() function itself

int initial(void);									//	This function initializes the reqired elements to their desired values at the beginning of program

											//	This function goes at ith row and jth column of screen

											//	Intakes one character and does not wait for user to enter the press key

int diffs=0,diffm=0,diffh=0;								//	Decides the difficulty level of sudoku

int display=0;

int getitchecked(void);

int initial()

	{

		// another for to convert 2-D array into linear one

		int position=0,i_,j_;


		for(i_=1;i_<=9;i_++)

			{

				for(j_=1;j_<=9;j_++)

					{

						position++;
								linear[position].itsi=i_;
								linear[position].itsj=j_;


						linear[position].linearval=position;

					}

			}

			
	
		for(i_=0;i_<=9;i_++)
		
			{
			
				for(j_=0;j_<=81;j_++)
				
					{
					
						cantfill[i_].cant[j_]=0;
						
					}
					
			}
	
	
	

	
	
	
	
	}
	



int main()

	{
		system("clear");

		initial();

		input();

		if(fill()==2)
		{
		system("clear");
		printf("Sudoku had unique solution\n\n");
		}

	}//main() ends







int check(int i,int j)

	{

		
	
			int number=0,count=0,__count=0,flag=0;
	
		

			for(int _i=grid(i,j).y;_i<grid(i,j).y+3;_i++)

				{
		
					for(int _j=grid(i,j).x;_j<grid(i,j).x+3;_j++)

						{

					

						if(filling[i][j]!=filling[_i][_j])
						count++;
						
						
						number++;


						}//inner for

				}//outer for



			


				if(count==(number-1))

					{
					
					
					int number1=0,count1=0,__count1=0,flag=0;


							for(int _i=1;_i<=9;_i++)

								{

		
									if(filling[i][j]!=filling[_i][j])
									count1=count1+1;
									
									number1++;
	
								}//end of for
	


				

						if(count1==(number1-1))

							{
							
								
							int number2_=0,count2_=0,__count2=0,number2=0,flag=0;

							
								for(int _j=1;_j<=9;_j++)

									{
										if(filling[i][j]!=filling[i][_j])
											count2_=count2_+1;
											
										number2_++;
										


									}//end of for


	
								
										if(count2_==(number2_-1))
								
											{
												
												
												return 1;
											}

										else
								
											{
												
												return 0;
											}






							}

						
						else
						
							{

							
							return 0;
						
							}


					}//end of if

	
				else
	
					{

						
						return 0;
					}


	}//end of check function

	



	
/////////////////////////////////////////////////////////////////////////////////////////////////////



	



int linearval(int i,int j)

	{

		int position=0;

			for(int i_=1;i_<=9;i_++)

				{

					for(int j_=1;j_<=9;j_++)


						{
							position++;

								if(i_==i&&j_==j)
									goto _out_;

						}

				}

	_out_:{}

return position;

	}


///////////////////

int output(void)

	{
		gotoxy(1,1);
		
		if(display==0)
		printf("Press I to view the instructions");
		
		
		if(display==1)
		{
		gotoxy(1,1);

		printf("The difficulty level of sudoku was ");
		
		if(diffh==1)
		printf("hard.");
		
		else if(diffm==1)
		printf("medium.");
		
		else if(diffs==1)
		printf("easy.");
		
		gotoxy(1,3);
		
		printf("This is solution number %d",morethan1);
		
		
		
		
		
		}	
		
		gotoxy(xcord-56,2);
	
		printf("  1      2      3      4      5      6      7      8      9");                            

			int counter=3;

				gotoxy(xcord-58,counter);
				hline

					printf("\n");
					counter++;

						printf("\n\n");
						counter=counter+2;


		int i1,j1;

		for(i1=1;i1<=9;i1++)

			{
		
			gotoxy(xcord-62,counter);
			printf("%d   ",i1);
			
				for(j1=1;j1<=9;j1++)

					{
	
						if(filling[i1][j1]!=0)
						printf("   %d   ",filling[i1][j1]);
	
						else
						printf("       ",filling[i1][j1]);
				
					}//end of inner for
	
						printf("\n");counter=counter+1;
		  	
		
		
								if(i1==3||i1==6)
		
									{
										gotoxy(xcord-58,counter);
										dashhline
									}
									
								else
		
									{
										gotoxy(xcord-58,counter);	
										hline
									}	
		
						printf("\n\n\n\n");counter=counter+2;

			}//end of outer for

		int m;

		for(m=0;m<10;m++)
			
			{

				if(m==3)
				dashvline(xcord-58+m*7,4,28,1)

				if(m==6)
				dashvline(xcord-58+m*7,4,28,2)
	
				if(m!=3&&m!=6)
				vline(xcord-58+m*7,4,28)


			}

	return 0;

	}



///////////////////////////

locate grid(int i,int j)

	{

		int x,y;

			///////////////////////////////////////
			if(i<=3&&i>=1)
			y=1;

				else if(i>=4&&i<=6)
				y=4;

					else if(i>=7&&i<=9)
					y=7;

			/////////////////////////////////////////

			////////////////////////////////////////
			if(j<=3&&j>=1)
			x=1;

				else if(j>=4&&j<=6)
				x=4;

					else if(j>=7&&j<=9)
					x=7;
			///////////////////////////////////////


		/////////////////
			location.x=x;
			location.y=y;
		////////////////

	return location;

}


/////////////////

int input(void)
{

system("clear");

	output();
	


int i=1,j=1,tot,remi,remj;
float score=0;
char pos;

	
	
while(1)

	{
	
			gotoxy(xcord-55+7*(j-1),6+(i-1)*3);
	
			pos=getch();
			
				if(pos=='d'||pos=='D')
		
					{
						tot=linearval(i,j);
						tot++;
						
						
						if(tot>81)
						tot=tot%81;
						
						i=linear[tot].itsi;
						j=linear[tot].itsj;
												
						gotoxy(xcord-55+7*(j-1),6+(i-1)*3);
					
						

					}

		
				else if(pos=='a'||pos=='A')
		
					{
						tot=linearval(i,j);
						tot--;
						
						if(tot<1)
						tot=tot+81;
						
						i=linear[tot].itsi;
						j=linear[tot].itsj;
												
						gotoxy(xcord-55+7*(j-1),6+(i-1)*3);
						
					}
					


				else if(pos=='w'||pos=='W')
		
					{
						
						i=i-1;
						
						if(i<1)
						i=i+9;

											
						gotoxy(xcord-55+7*(j-1),6+(i-1)*3);
						
					}
					
					
					
				else if(pos=='S'||pos=='s')
		
					{
						
						i=i+1;
						
						if(i>9)
						i=i-9;
												
						
						gotoxy(xcord-55+7*(j-1),6+(i-1)*3);

					}
					

				else if(pos<=57&&pos>=48)
				
					{
						
						
						
						gotoxy(1,3);
						printf("           ");
						
						if(filling[i][j]==0)
						_check++;
						
						if(pos==48)
						_check--;
						
						remi=i;
						remj=j;
						gotoxy(1,1);
						
						i=remi;
						j=remj;
						gotoxy(xcord-55+7*(j-1),6+(i-1)*3);
						
						filling[i][j]=pos-48;
						
						remi=i;
						remj=j;
						getitchecked();
						i=remi;
						j=remj;
															
						gotoxy(xcord-55+7*(j-1),6+(i-1)*3);
					
						printf(" ");

						gotoxy(xcord-55+7*(j-1),6+(i-1)*3);
					
						if(filling[i][j]!=0)
						{
							printf("%d",filling[i][j]);
							
						}
						
					}
					
					
					
				else if(pos=='I'||pos=='i')
					
					{
					
						system("clear");
							
						printf("\nINSTRUCTIONS");

						printf("\n");
						hline
						printf("\n");	

						printf("\n1. The user should use the following keys to navigate inside the sudoku\n\n\n\n");

						printf("				W	\n");
						printf("\n			A	S	D");

						printf("\n\n\n\n2. Enter 0 to remove the entry");
						
						printf("\n\n3. Press I whilie playing to view the Instructions");

						printf("\n\n4. Press F when filled the sudoku");

						printf("\n\n5. Press E while filling to exit the solver");

						getch();
						
						system("clear");
						
						remi=i;
						remj=j;
						output();
						i=remi;
						j=remj;
						gotoxy(xcord-55+7*(j-1),6+(i-1)*3);
							
					}
	
						
						
					else if(pos=='f'||pos=='F')
					
						{
							
							system("clear");


							printf("Are you sure you have filled the sudoku (y\\n) ??? : ");
							
							char q;
							
							q=getch();
																			
		
								if(q=='y'||q=='Y')
									{
								
										
										
										
										if(getitchecked()==0)
										{
										display=1;
										return 0;
										}
										
										else
											{
												system("clear");
												
												output();
												i=linear[getitchecked()].itsi;
												j=linear[getitchecked()].itsj;
											
											}
								
									}
							
								else
									
									{
										remi=i;
										remj=j;
											system("clear");
											output();
										i=remi;
										j=remj;
							
										gotoxy(xcord-55+7*(j-1),6+(i-1)*3);
									}
							
						
						}
						
						
						
						
						
						
						

					else if(pos=='e'||pos=='E')
					
						{
						
							system("clear");
							
							printf("Are you sure you want to exit (y\\n) ??? : ");
							
							char q;
							
							q=getch();
							
							if(q=='y'||q=='Y')
							{
							system("clear");
					
							exit(3);
							}
							
							else
							
							{
							
								system("clear");
						
								remi=i;
								remj=j;
								output();
								i=remi;
								j=remj;
		
								gotoxy(xcord-55+7*(j-1),6+(i-1)*3);
							
							}
							
						
						}
	

	}//end of while


}






int simplecheck(void)

	{
	
	diffs=1;
	
	int flag=0,_i,_j,i,j,a;

		for(_i=1;_i<=9;_i++)

			{

				for(_j=1;_j<=9;_j++)

					{

						if(filling[_i][_j]==0)

							 {
								 i=_i;
								 j=_j;

							 flag=1;
							 break;
							 }
							 
									
						
					}

			if(flag==1)
			break;

			}
			
			

int filled=0;

	int inc=linearval(i,j),count,ability;


	while(1)

		{

			 count=0;ability=0;flag=0;


					  
					  for(_i=1;_i<=9;_i++)
		
						{
		
							filling[i][j]=_i;

							
							
							 if(check(i,j)==1)
								
								{	
								
									count++;
									
									a=_i;

									if(count==2)
									{flag=1;
									break;
									}
								}
								
								if(flag==1)
								break;
							
							
		
						}


						
						
					if(count==1)
					{

					filling[i][j]=a;
					
					ability=1;

					_check++;
					
					update();
					
					}
					
					
					else
					{
					filling[i][j]=0;
					}
					
				
								
								do
					
								{
									if(_check==81)
									{return 2;}

									

									inc++;

									if(inc>81)
									{


									if(ability==0)
									{
									return 0;
									}

									ability=0;

									inc=inc%81;

									}	


									i=linear[inc].itsi;
									j=linear[inc].itsj;


									if(_check==81)
										{	
											return 2;
										}

									}while(filling[i][j]!=0);

					  

		}//while loop ends


	
	
	
	
	}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			
int fillunfill(void)

	{
			
	int _i,_j,i=0,j=0,flag=0,inc,a,b;	


			for(_i=1;_i<=9;_i++)

						{
			
							for(_j=1;_j<=9;_j++)
			
								{
			
									if(filling[_i][_j]!=0)
			
										 {
											 i=_i;
											 j=_j;
			
										 flag=1;
										 break;
										 }

												

								}
			
						if(flag==1)
						break;
			
						}			
				
				

		if(i==0&&j==0)
		{
		return 2;
		}

		
		
		inc=linearval(i,j);
	
		int num;
		
	while(1)
	
		{
	
		num=filling[i][j];
		a=i;
		b=j;	
			
		for(_i=1;_i<=9;_i++)

			{
				
				cantfill[num].cant[linearval(_i,b)]=1;
						
			}
			
			
		
		
		
		for(_j=1;_j<=9;_j++)
		
			{
			
				cantfill[num].cant[linearval(a,_j)]=1;
			
			}
			
			
		
		
		
		int startx=grid(a,b).y,starty=grid(a,b).x;

		
		
		
		for(int __i=startx;__i<=startx+2;__i++)
		
			{
			
				for(int __j=starty;__j<=starty+2;__j++)
				
					{
					
					
						cantfill[num].cant[linearval(__i,__j)]=1;
						
					
					}
					
			
			}
			
			
			
			
			
			
		
			

			
			
		if(num!=0)
		{
		for(int number=1;number<=9;number++)
		cantfill[number].cant[linearval(i,j)]=1;
		}
		
		
							do

								{
							
									inc++;
								
									if(inc>81)
									{
									
									return 0;
																	
									}	
									
									if(_check==81)
									{
									return 2;
									}

									
									
									i=linear[inc].itsi;
									j=linear[inc].itsj;

								}while(filling[i][j]==0);









		}//end of while




	}//end of fillunfill function
	
	

int fill(void)

	{
			int diffm=1;
			
			int count,rema,remb,num,extrah,extrav,moreh,b,foundi,foundj,morev,more1s;
		
		
			if(simplecheck()==2)
			{system("clear");output();getch();return 2;}
			
			if(fillunfill()==2)
			{system("clear");output();getch();return 2;}
			
			while(_check<81)
			
				{
						
					
		
					if(simplecheck()==2)
					{system("clear");output();getch();return 2;}
			
					if(fillunfill()==2)
					{system("clear");output();getch();return 2;}
			
					ability_fillunfill=0;
					
					for(num=1;num<=9;num++)
					
						{
						
							for(int i=0;i<=2;i++)
							
								{
								
									for(int j=0;j<=2;j++)

										{
										
											count=0;morev=0;moreh=0;
											
											for(int a=i*3+1;a<=i*3+1+2;a++)
						
												{
												
													extrah=0;extrav=0;
							
													for(b=j*3+1;b<=j*3+1+2;b++)
										
														{
												
															
															
													if(cantfill[num].cant[linearval(a,b)]==1)
															{
																																										
															count++;
														
															extrah++;
											
															}
															
													else
													{rema=a;
													remb=b;
													}
															
													if(cantfill[num].cant[linearval(b,a)]==1)
															{
															extrav++;
															}		
													
															
															
															
														}

												if(extrah==3)
													moreh++;
													
												if(extrah!=3)
													foundi=a;
													
												if(extrav==3)
													morev++;
													
												if(extrav!=3)
													foundj=a;
												}	
													
												
													
													if(moreh==2)
													{
													for(more1s=1;more1s<=9;more1s++)
													{

													if(more1s>b-1||more1s<b-3)
													{
													cantfill[num].cant[linearval(foundi,more1s)]=1;
											
													}
													}
													}
													
													if(morev==2)
													{
													
													for(more1s=1;more1s<=9;more1s++)
													{
													if(more1s>b-1||more1s<b-3)
													{cantfill[num].cant[linearval(more1s,foundj)]=1;
										
													}
													}
													}

		
											
											
											if(count==8&&filling[rema][remb]==0&&cantfill[num].cant[linearval(rema,remb)]==0)
												
														{	
													
															_check++;
															
															
															
															filling[rema][remb]=num;
															
															if(check(rema,remb)==0)
															{
															filling[rema][remb]=0;
															_check--;
															}
															
															update();
															
															
															
															if(filling[rema][remb]!=0&&_check<81){
															
															if(simplecheck()==2)
															{system("clear");output();getch();return 2;}
															
															if(fillunfill()==2)
															{system("clear");output();getch();return 2;}
															
															ability_fillunfill=1;
															
															}
															
												
													}
														

										}

								}


								rowcolumnfill(num);


						}


					if(ability_fillunfill==0&&_check<81)
					{

						if(uni_flag1==0)
							{

							    if(fill_random()==0)
								{
									if(morethan1==1)
									{printf("Sudoku is unsolvable\n\n");getch();exit(5);}


									else
									{system("clear");printf("The sudoku had %d solutions\n\n",morethan1-1);getch();exit(8);}
								}
							}

						else
							{

								if(fillsimpossible()==1)
									{
										return -1;
									}

								else
									{
										return 0;
									}

							}

					}


				}





			system("clear");
			
			output();

			getch();
			
			return 2;
			
			
	 		
	 		}





int rowcolumnfill(int num)

	{
		int i,j,rema1,remb1,count;

		for(i=1;i<=9;i++)

			{

				count=0;


				for(j=1;j<=9;j++)

					{

						if(cantfill[num].cant[linearval(i,j)]==1)
						count++;

						else
						{
						rema1=i;
						remb1=j;
						}

					}





				if(count==8&&filling[rema1][remb1]==0&&cantfill[num].cant[linearval(rema1,remb1)]==0)

						{

							_check++;


							filling[rema1][remb1]=num;

							update();

							if(check(rema1,remb1)==0)
							{
							_check--;
							filling[rema1][remb1]=0;
							}

							else if(_check<81)
							{
							ability_fillunfill=1;
							if(simplecheck()==2)
							{system("clear");output();getch();return 2;}
			
							if(fillunfill()==2)
							{system("clear");output();getch();return 2;}
							}


						}



			}




	for(i=1;i<=9;i++)

		{

			count=0;

			for(j=1;j<=9;j++)

				{


						if(cantfill[num].cant[linearval(j,i)]==1)
						count++;

						else
						{
						rema1=i;
						remb1=j;
						}




				}



			if(count==8&&filling[remb1][rema1]==0&&cantfill[num].cant[linearval(remb1,rema1)]==0)

							{

							_check++;

							filling[remb1][rema1]=num;

							update();

							if(check(rema1,remb1)==0)
							{
							_check--;
							filling[rema1][remb1]=0;
							}




							else if(_check<81)
							{
							ability_fillunfill=1;
							if(simplecheck()==2)
							{system("clear");output();getch();return 2;}
						
							if(fillunfill()==2)
							{system("clear");output();getch();return 2;}
				
							}


							}



	}



		if(_check==81)
		{system("clear");
		output();
		getch();
		return 2;
		}
	
		return 0;




	}





int least_possible(void)

	{


	int a=10,b;

	for(int i=1;i<=81;i++)

		{
			if(filling[linear[i].itsi][linear[i].itsj]==0)

			{

				int count=0;

				for(int k=1;k<10;k++)

					{

						if(cantfill[k].cant[i]==0)

							{

								count++;

							}

					}


				if(count<a)
				{b=i;
				a=count;
				}

			}


		}




		return b;

	}





int fill_random(void)

	{
		diffh=1;

		uni_flag1=1;
		timenumber++;
		int box=least_possible();
		int n,k=1;


	while(n!=0)

		{

		n=0;

		while(k<10&&n==0)

			{

				if((cantfill[k].cant[box])==0)

					{
						n=k;
					}

				k++;


			}



		save_filling(timenumber);

		filling[linear[box].itsi][linear[box].itsj]=n;
		fillunfill();
		update();


		if(n==0)
		{
		restore_filling(timenumber-1);
		break;
		}

		int a=fill();


		if(a==0)
		fill_random();

		else if(a==-1)
		{
		restore_filling(timenumber);
		}

		else if(a==2)
		{
		restore_filling(timenumber);
		morethan1++;
		continue;
		}

		}

		timenumber--;

		return 0;




	}



int update(void)

	{

		for(int i_=0;i_<=9;i_++)

			{

				for(int j_=0;j_<=81;j_++)

					{

						cantfill[i_].cant[j_]=0;

					}

			}


		_check=0;

		for(int i=1;i<=9;i++)

			{

				for(int j=1;j<=9;j++)

					{

						if(filling[i][j]!=0)
						_check++;

					}

			}


		fillunfill();


	}


int save[81][10][10];

int save_filling(int timenumber)


	{

		for(int i=0;i<=9;i++)
		
			{
			
				for(int j=0;j<=9;j++)
				
					{
					
						save[timenumber][i][j]=filling[i][j];
				
					}
					
			}
			
			
			
			
	}
	
	
int restore_filling(int timenumber)

	{
	
	
		int i,j;
		
		for(i=0;i<=9;i++)
		
			{
			
				for(j=0;j<=9;j++)
				
					{
					
						filling[i][j]=save[timenumber][i][j];
				
					}
					
			}	
			
			
			
		_check=0;
			
		for(i=1;i<=9;i++)
		
			{
			
				for(j=1;j<=9;j++)
				
					{
					
						if(filling[i][j]!=0)
						_check++;
						
					}
					
			}	
			
			
			
			
			
		update();
			
	}
	
				
				
				



int fillsimpossible(void)

	{
		int count,i,_n;
		
		for(i=1;i<=81;i++)
		
			{
				count=0;
				
				for(_n=1;_n<=9;_n++)
				{
				
				if(cantfill[_n].cant[i]==0)
				count++;
				
				}
				
				if(count==0&&_check<81&&filling[linear[i].itsi][linear[i].itsj]==0)
				{
					return 1;
				}
				
					
					
			}
			
		

	return 0;


	}

int getitchecked(void)

	{
	
		for(int i=1;i<=9;i++)
			{
				for(int j=1;j<=9;j++)
					{
						if(check(i,j)==0&&filling[i][j]!=0)
							{
								gotoxy(1,3);
								printf("Wrong Entry");
								int val=linearval(i,j);
								return val;
							}
					}
			}
			
			
			return 0;
	}		
		
			
		
char getch(void)

	{
	
	struct termios oldt,
	newt;

	char ch;
	
		tcgetattr( STDIN_FILENO, &oldt );
		newt = oldt;
		newt.c_lflag &= ~( ICANON | ECHO );
		tcsetattr( STDIN_FILENO, TCSANOW, &newt );
		
		ch = getchar();

		tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
	return ch;
	}		
		
		

#define MAX_SCREEN_AREA 100

int gotoxy(int x, int y)

	{

		char essq[MAX_SCREEN_AREA]={0}; // String variable to hold the escape sequence
		sprintf(essq, "\033[%d;%df", y,x);
		printf("%s", essq);
		return 0;
	} 
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
