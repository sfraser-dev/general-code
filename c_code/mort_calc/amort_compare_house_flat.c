// Set up to compare with an opening balance of £135,000 @ 3.75% with no offset savings and no overpayments.
// This results in a total interest of £68,500 to be paid over 282 months (23 years and 5 months).

#include<stdio.h>
#include<math.h>

int main() {
	int month;
	int years, rems, j;
	float interestThisMonth;
	float principleThisMonth;
	float endBalance;
	float totalInterestPaidHouse;
	float totalInterestPaidFlat;
	float intialAmountBorrowed;
       	float iEqn; 
	float bracketsPowN;
	float monthlyPayment;
	float interestSavedFlat;
	float interestSavedHouse;
	float ovrpayHouse[300]={0.0};
	float totalOverpaymentFlat=0.0;

	float initialAmountBorrowedFlat=35000.00;	// need this to calculate the monthly payments
	int nFlat =300;  				// number of monthly payments
	float balanceFlat=10000.00;
	float rateFlat=0.0589;
	float overpaymentThisMonthFlat=1800;

	float initialAmountBorrowedHouse=140000.00; 	// need this to calculate the monthly payments
	int nHouse=300;  				// number of monthly payments
	float balanceHouse=134000.00;
	float rateHouse=0.0375;
	float offsetSavingsHouse=5000;
	float overpaymentThisMonthHouse=1800;
	for (j=0; j<300; j++)
		ovrpayHouse[j]=overpaymentThisMonthHouse+200;
	ovrpayHouse[0]=0;
	ovrpayHouse[1]=0;
	ovrpayHouse[2]=0;
	ovrpayHouse[3]=0;
	ovrpayHouse[4]=0;

	/************************************  FLAT (no offsetting)  ********************************/
	printf("-----------------------------------------------------------------------------------------------\n");
	printf("---------------------------------------- FLAT -------------------------------------------------\n");
	printf("-----------------------------------------------------------------------------------------------\n");
	// M = P [ i(1 + i)^n ] / [ (1 + i)^n - 1] from http://www.fonerbooks.com/interest.htm
	iEqn=rateFlat/12; 				// interest rate per month
        bracketsPowN=(float)pow(1+iEqn,nFlat);
	monthlyPayment = initialAmountBorrowedFlat * (iEqn*(bracketsPowN)) / (bracketsPowN-1); 

	totalInterestPaidFlat=0.0;
	for (month=1; month<=300; month++){
		interestThisMonth=balanceFlat*rateFlat/12;
		totalInterestPaidFlat+=interestThisMonth;
		principleThisMonth=monthlyPayment-interestThisMonth;
		endBalance=balanceFlat-principleThisMonth;
		totalOverpaymentFlat+=overpaymentThisMonthFlat;
		printf("month %3d: bal=%9.2f monthly=%6.2f int=%6.2f prcple=%6.2f ",
			month,balanceFlat,monthlyPayment,interestThisMonth,principleThisMonth);
		printf("endBal=%9.2f totInt=%9.2f totOvrpay=%9.2f\n",endBalance,totalInterestPaidFlat,totalOverpaymentFlat);
		if(endBalance < 0) {
			break;
		}
		balanceFlat=endBalance;
		balanceFlat-=overpaymentThisMonthFlat;
	}
	years=(int)month/12;
	rems=month%12;
	printf("With no overpayments, mortgage will last 300 months (25 years) with total interest of 31947.38\n");
	printf("With overpayments, mortgage will last %d months (%d years %d months) ",month,years,rems);
	printf("with a total interest of %.2f\n",totalInterestPaidFlat);
	years=(int)((300-month)/12);
	rems=(300-month)%12;
	interestSavedFlat=31947.38-totalInterestPaidFlat;	
	printf("Months saved %d (%d years and %d months). Interest saved %.2f\n",300-month,years,rems,interestSavedFlat);
	
	/************************************  HOUSE  ********************************/
	printf("-----------------------------------------------------------------------------------------------\n");
	printf("---------------------------------------- HOUSE ------------------------------------------------\n");
	printf("-----------------------------------------------------------------------------------------------\n");
	// M = P [ i(1 + i)^n ] / [ (1 + i)^n - 1] from http://www.fonerbooks.com/interest.htm
	iEqn=rateHouse/12;				// interest rate per month
        bracketsPowN=(float)pow(1+iEqn,nHouse);
	monthlyPayment = initialAmountBorrowedHouse * (iEqn*(bracketsPowN)) / (bracketsPowN-1); 

	totalInterestPaidHouse=0.0;
	balanceHouse-=offsetSavingsHouse;
	for (month=1; month<=300; month++){
		interestThisMonth=balanceHouse*rateHouse/12;
		totalInterestPaidHouse+=interestThisMonth;
		principleThisMonth=monthlyPayment-interestThisMonth;
		endBalance=balanceHouse-principleThisMonth;
		printf("month %3d: bal=%9.2f monthly=%6.2f int=%6.2f prcple=%6.2f ",
			month,balanceHouse,monthlyPayment,interestThisMonth,principleThisMonth);
		printf("endBal=%9.2f totInt=%9.2f savings=%9.2f\n",endBalance,totalInterestPaidHouse,offsetSavingsHouse);
		if(endBalance < 0) {
			break;
		}
		balanceHouse=endBalance;
		//balanceHouse-=overpaymentThisMonthHouse;
		//offsetSavingsHouse+=overpaymentThisMonthHouse;
		balanceHouse-=ovrpayHouse[month-1];
		offsetSavingsHouse+=ovrpayHouse[month-1];
	}
	years=(int)month/12;
	rems=month%12;
	printf("With no offset savings and no overpayments, mortgage will last 300 months (25 years) with total interest of 75935.10\n");
	printf("With offset savings and overpayments, mortgage will last %d months (%d years %d months) ",month,years,rems);
	printf("with a total interest of %.2f\n",totalInterestPaidHouse);
	years=(int)((300-month)/12);
	rems=(300-month)%12;
	interestSavedHouse=75935.10-totalInterestPaidHouse;
	printf("Months saved %d (%d years and %d months). Interest saved %.2f\n",300-month,years,rems,interestSavedHouse);
	printf("-----------------------------------------------------------------------------------------------\n");
	printf("Total interest paid  = %9.2f\n",totalInterestPaidFlat+totalInterestPaidHouse);
	printf("Total interest saved = %9.2f\n",interestSavedFlat+interestSavedHouse);
}
