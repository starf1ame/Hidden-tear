#include<stdio.h>
#include<windows.h>
#include<tchar.h>
#include<cstdlib>
#include<ctime>
using namespace std;

/*
This is for windows, change background for mac and ubuntu is in encrypt.cpp(by cmd) 
*/
void setbackground() {
	PVOID s = (PVOID)"C:\\Users\\lenovo\\Desktop\\background.png";
	SystemParametersInfo(20, true, s, 1);
	return 0;
}