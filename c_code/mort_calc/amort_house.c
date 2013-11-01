// Set up to compare with an opening balance of £135,000 @ 3.75% with no offset savings and no overpayments.
// This results in a total interest of £68,500 to be paid over 282 months (23 years and 5 months).

#include<stdio.h>
#include<math.h>

#define MAXMONTHS 300 // My mortgage was initially for 300 months (25 years).

int main() {
	int month;
	int years, rems;
	float totalInterestPaid=0.0;
	float interestThisMonth;
	float principleThisMonth;
	float endBalance;

	float balance=140000.00;
	float rate=0.0375;

	// M = P [ i(1 + i)^n ] / [ (1 + i)^n - 1] from http://www.fonerbooks.com/interest.htm
	float intialAmountBorrowed=140000; 	// need this to calculate the monthly payments
	float i=rate/12; 			// interest rate per month
	float n=300;     			// number of monthly payments (25 years)
        float bracketsPowN=(float)pow(1+i,n);
	float monthlyPayment = intialAmountBorrowed * (i*(bracketsPowN)) / (bracketsPowN-1); 

	//float offsetSavings=30000.0;
	//float overpaymentThisMonth=2200.0;
	float offsetSavings=0000.0;
	float overpaymentThisMonth=00.0;

	float ovrpay[MAXMONTHS]={0.0};
	ovrpay[0]=500;
	ovrpay[1]=400;
	ovrpay[2]=600;
	ovrpay[3]=200;
	ovrpay[4]=800;
	ovrpay[5]=900;
	ovrpay[6]=1500;
	ovrpay[7]=400;
	ovrpay[8]=750;
	ovrpay[9]=1000;
	ovrpay[10]=900;
	ovrpay[11]=850;

	balance-=offsetSavings;
	for (month=1; month<=MAXMONTHS; month++){
		interestThisMonth=balance*rate/12;
		totalInterestPaid+=interestThisMonth;
		principleThisMonth=monthlyPayment-interestThisMonth;
		endBalance=balance-principleThisMonth;
		printf("month %3d: bal=%9.2f monthly=%6.2f int=%6.2f prcple=%6.2f ",
			month,balance,monthlyPayment,interestThisMonth,principleThisMonth);
		printf("endBal=%9.2f totInt=%9.2f savings=%9.2f\n",endBalance,totalInterestPaid,offsetSavings);
		if(endBalance < 0) {
			break;
		}
		balance=endBalance;
		// Can toggle between using different overpayments values in the array or just using a constant overpayment value. 
		//overpaymentThisMonth=ovrpay[month-1];  
		balance-=overpaymentThisMonth;
		offsetSavings+=overpaymentThisMonth;
	}
	printf("With no offset savings and no overpayments, mortgage will last 282 months with total interest of 68500.00\n");
	printf("With offset savings and overpayments, mortgage will last %d months with a total interest of %.2f\n",month,totalInterestPaid);
	years=(int)((282-month)/12);
	rems=(282-month)%12;
	printf("Months saved %d (%d years and %d months). Interest saved %.2f\n",282-month,years,rems,((float)68500)-totalInterestPaid);
}
