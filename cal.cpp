#include <iostream>
#include <time.h>
#include <cstdlib>
#include <cmath>

using namespace std;

const char days [7][4] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
//cotterell's code github
int julian_day(int y, int m, int d);
int gregorian_day(int y, int m, int d);
int month_days(int m, int y);
string month_name(int m);
/**
this program will implement the calendar
system call for gregorian and julian calendar
in which the start days different along with the
exception of the september date in 1752
 */
int main(int argc, const char * argv[]) {
  cout.setf(ios::unitbuf);
  int startday=0;
  if (argc ==1){
    cout<< "     "<< "April 2017" <<endl;
    cout<< "Su Mo Tu We Th Fr Sa"  <<endl;
    startday= gregorian_day(2017, 4, 1);
    // for loop for the spaces before date
    for(int i=0; i< startday*3; i++){
      cout<< " ";
    }
    // for loop for the date of the month of april
    for(int i=1; i<= 30; i++){
      printf("%2d " , i);
      startday++;
      if(startday %7 ==0){
	cout<< endl;
      }
    }
    cout<< endl;

  }
  else if(argc ==2){
    int y1 = atoi(argv[1]);
    //if statement for julian
    if(y1<=1752){
      cout<< "           " << y1<< endl;
      for(int i=1; i<=12; i++){
        cout<< "    "<< month_name(i)<<endl;
        cout<< "Su Mo Tu We Th Fr Sa"  <<endl;

	//if statment for case september 1752
	if(y1== 1752 && i==9){
	  for(int l=0; l<5; l++){
	    cout<< " ";
	  }
	  cout << "  1  2 14 15 16\n17 18 19 20 21 22 23\n24 25 26 27 28 29 30\n";
	  //continue;
	}
	//else statement for gregorian
	else{
	  if(y1 ==1752 && i>9){
	    startday = gregorian_day(y1, i ,1);
	  }
	  else{
	  startday = julian_day(y1,i,1);
	  }
	  for(int j=0; j< startday*3; j++){
	    cout<< " ";
	  }
	  //loop for days of specific month
	  for(int k=1; k<= month_days(i, y1) ; k++){
	    printf("%2d " , k);
	    startday++;
	    if(startday %7 ==0){
	      cout<< endl;
	    }
	  }
	}
        cout<< endl;
      }
    }
    // else statement of gregorian 
    else{
      cout<< "           " << y1<< endl;
      for(int i=1; i<=12; i++){
	cout<< "    "<< month_name(i)<<endl;
	cout<< "Su Mo Tu We Th Fr Sa"  <<endl;
	int startday = gregorian_day(y1,i,1);
	for(int j=0; j< startday*3; j++){
	  cout<< " ";
	}
	for(int k=1; k<= month_days(i, y1) ; k++){
	  printf("%2d " , k);
	  startday++;
	  if(startday %7 ==0){
	    cout<< endl;
	  }
	}
	cout<< endl;
      }
    }
  }
  //else staement for 3 parameter input of month and date
  else{
    //    cout << " jk";
  int m = atoi(argv[1]);                // year as C string                                                 
  int y = atoi(argv[2]);                // month as C string                                                
  //int d = atoi(argv[3]);                // day as C string                                                  

  int gday = gregorian_day(y, m, 1);    // num days since SUN (Gregorian)                                   
  int jday = julian_day(y, m, 1);       // num days since SUN (Julian)                      //if statment for julian or gregorian start days                
  if(y <=1752){
    startday = jday;
  }
  else{
    startday = gday;
  }
  cout<< "   "<< month_name(m)<< " " << y<<endl;
  cout<< "Su Mo Tu We Th Fr Sa"  <<endl;
  //statment for septemeber case
  if(m==9 && y==1752){
    for(int i=0; i<5; i++){
      cout<< " ";
    }
    cout << "  1  2 14 15 16\n17 18 19 20 21 22 23\n24 25 26 27 28 29 30\n";
    return EXIT_SUCCESS;
  }
  for(int i=0; i< startday*3; i++){
    cout<< " ";
  }
  for(int i=1; i<= month_days(m, y) ; i++){
    printf("%2d " , i);
    startday++;
    if(startday %7 ==0){
      cout<< endl;
    }
  }
  cout<< endl;

  }  
return EXIT_SUCCESS;
  
} // main

/**
 * Compute the Julian Day.
 * @see https://en.wikipedia.org/wiki/Julian_day#Converting_Julian_or_Gregorian_calendar_date_to_Julian_day_number
 */
int julian_day(int year, int month, int day) {
  int a = floor((14 - month) / 12);
  int y = year + 4800 - a;
  int m = month + 12 * a - 3;
  int j = day
    + floor((153 * m + 2) / 5)
    + (365 * y)
    + floor(y / 4)
    - 32083;
  return (j + 1) % 7;
} // julian_day

/** 
 * Compute the Gregorian day.
 */
int gregorian_day(int y, int m, int d) {
  struct tm tm;                         // tm structure
  char date [11];                       // date string
  sprintf(date, "%d-%d-%d", y, m, d);   // create date string
  strptime(date, "%Y-%m-%d", &tm);      // extract tm structure
  return tm.tm_wday;    
} // gregorian_day


/**
days of month for cases of months that have
special cases including leap years
@param int m for months
@param int y and for years depends on day output
@return returns an int of days

*/
int month_days(int m, int y){
  if(y !=1752){
    if( m== 1 || m==3|| m==5 || m==7 || m==8 || m== 10 || m==12){
      return 31;
    }
    else if(m ==4 || m==6 || m==9 || m==11){
      return 30;
    }
    else{
      if(((y%4 ==0) && (y %100!=0)) || (y%400 ==0))
	return 29;
      else{
	return 28;
      }
    }
  }
  //gregorian calender for days
  else{
    if(m== 1 || m==3||m==5 ||m==7 ||m==8 ||m== 10 || m==12){
      return 31;
    }
    else if(m ==4 || m==6 || m==11){
      return 30;
    }
    else{
      if(((y%4 ==0) && (y %100!=0)) || (y%400 ==0))
	return 29;
      else{
	return 28;
      } 
    }
  }
}

/**
month in string input for calender
taking in an int for m
@param int m takes in a month
@return a string

 */
string month_name(int m){

  switch (m){
  case 1:
    return "January";
  case 2:
    return "February";
  case 3:
    return "March";
  case 4:
    return "April";
  case 5:
    return "May";
  case 6:
    return "June";
  case 7:
    return "July";
  case 8:
    return "August";
  case 9:
    return "September";
  case 10:
    return "October";
  case 11:
    return "November";
  case 12:
    return "December";
  default:
    return NULL;
  }
}
