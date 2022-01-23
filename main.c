/*Author Qing Li 
 email: qinglimontreal@gmail.com 
The code is done for IGEE418 assignment.Not much debug works are done*/

#include <stdio.h>
#include "image_traitement.h"
//size of BMP
#define X_LEFT 83
#define Y_UP 32
#define X_RIGHT 600
#define Y_DOWN 381
#define X_START 83
#define Y_START 60


#define _CRT_SECURE_NO_WARNINGS 
float*  trace(const BMP* ) ;
float integrate_part(float, float , float* );//area under curve
float integrate_up_part(float, float, float*);//area under curve-lower rectangle
int main(void) {
    int x,y;
	Uchar r,g,b;
	const float *p;
	float coe_x = (float)8760 / (float)(X_RIGHT - X_LEFT); //in hour/pixel
	BMP* pp = BMP_ReadFile("sample.bmp");
	BMP* bmp_nb = image_convertir_nb(pp);    
	BMP_WriteFile(bmp_nb, "nb_cov.bmp");
    p=trace(bmp_nb);
	printf("ya666g%f \n", p[0]);
	printf("ya500g%f \n", p[516]);
	float val= integrate_part(0, 40, p);
	float val_ful = val * coe_x;
	printf("total area under curve=%2.1f  GWH\n", val_ful);
	printf("percent=%f\n", val_ful/(8760*40));
	float val_pa = integrate_up_part(0, 40, p);
	float val_pa_cov = val_pa * coe_x;
	//printf("Over the capacity limit if showdown=%2.1f\n", val_pa_cov);
	//printf("money=%2.4f\n", val_pa_cov/4);
	
	//FILE *file = fopen("res.tsv", "a");
	float offset = (float)(36.561 * 1.1 - 18.55);
	float *pt = (float*)malloc(offset / (0.3) * sizeof(float));
	float total = (float)(3000)*offset / (0.3);//initial value

	int n=0;
	for (int i = 0; i < offset / (0.3); i++) {
		float energy_by_gas;
		energy_by_gas = integrate_up_part(0, 40, p);
		float comsumption = 0.0;
		float w_wind =  i*.3;
		float gas_w = offset- w_wind;
		float sum_instal = (float)(3.0 * i)*1000.0 + 1.5 * gas_w*1000.0;
		//printf("sun_insta==%2.0f\n", sum_instal);
		//printf("instal_depense=%f\n", sum_instal);
		float point_low = 18.55 + w_wind;
		pt[i]= 0.25*energy_by_gas * coe_x + sum_instal;
		//printf("loopcostPPPP===%2.1f\n power of wind%d\n", pt[i], i);
		if (pt[i] < total) {
			total = pt[i];
			n = i;
			//printf("ifff cost===%2.1f\n power of wind%d\n", total, i);
		}
	}
	printf("Final cost=%2.1f million dollars in 10 years\nWind power= %d WATT\n", total, n);
	BMP_Free(bmp_nb);
	BMP_Free(pp);
	//fclose(file);
	free(p);
}

 float*  trace(const BMP* image_o) {
// no break allows on the curve
    int x,y,y_pos;
	UCHAR r,g,b;
	float* curve =(float*)malloc((X_RIGHT-X_LEFT)*sizeof(float)+5);
	float coe_y = 40/(Y_DOWN-Y_UP); // in GW/pixel
    y_pos=Y_START;
	for (x = 0; x < X_RIGHT-X_START; ++x) {
		for (y = -1; y < 3; ++y) {
			BMP_GetPixelRGB(image_o, x+X_START, y+y_pos, &r, &g, &b);
			if ((r + g + b) / 3 <= 10){ y_pos=y+y_pos;} 
		}
		curve[x]=(float)(Y_DOWN-y_pos)* (float)40/ (float)(Y_DOWN-Y_UP);
	}
	return curve;
}

 float integrate_part(float low, float high, float* ptr) {
	 int sum = 0;

	 for (int i = 0; i < X_RIGHT - X_LEFT-1; i++) {
		 if (ptr[i] > low && ptr[i] < high) { sum = (ptr[i] + ptr[i + 1]) / 2 + sum;}
	 }

	 return sum;
 }
 float integrate_up_part(float low, float high, float* ptr) {
	 int sum = 0;
	 int cnt = 0;
	 float v_l = 40;
	 float V_h = 12;
	 for (int ii = 0; ii < X_RIGHT - X_LEFT - 1; ii++) {

		 if (ptr[ii] > low && ptr[ii] < high) {
			 sum = (ptr[ii] + ptr[ii + 1]) / 2 + sum; 
			 if (ptr[ii] < v_l && ptr[ii]>low) { v_l = ptr[ii]; }
			 if (ptr[ii] > V_h) { V_h = ptr[ii]; }
			 cnt++;
		 }
		 
	 }
	 //printf("v_L==%f\n", v_l);
	 return sum-cnt * v_l;
 }
