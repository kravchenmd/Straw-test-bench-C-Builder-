//---------------------------------------------------------------------------

#include <vcl.h>
//#include <comobj.hpp>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <windows.h>
#include <string.h>
#pragma hdrstop

#include <io.h>         //для работы с файлами
#include <fcntl.h>      //для работы с файлами
#include <sys\stat.h>   //для работы с файлами
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VCLTee.TeeLinearGauge"
#pragma link "VCLTee.TeeNumericGauge"
#pragma link "VCLTee.CurvFitt"
#pragma link "VCLTee.TeeSeriesStats"
#pragma link "VCLTee.TeePageNumTool"
#pragma link "VCLTee.TeeTools"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------

//=============================================================================
//..................... объявления глобальных переменных ......................
//=============================================================================

#define BUFSIZE 82     //ёмкость буфера
#define ESC \033
#define LF \012

char bufrd_AER[BUFSIZE], bufwr_AER[BUFSIZE]; //приёмный и передающий буферы
char bufrd_SM[BUFSIZE], bufwr_SM[BUFSIZE];
char buf_tmp[BUFSIZE], buf[BUFSIZE];


char hrs[7], sec[3];
char lat[12], lon[13];
float dx, dy;
unsigned int len = 0;


int dat;
//---------------------------------------------------------------------------

HANDLE COMport_AER;		//дескриптор порта
HANDLE COMport_SM;

//структура OVERLAPPED необходима для асинхронных операций, при этом для операции чтения и записи нужно объявить разные структуры
//эти структуры необходимо объявить глобально, иначе программа не будет работать правильно
OVERLAPPED overlapped_AER;		//будем использовать для операций чтения (см. поток ReadThread)
OVERLAPPED overlappedwr_AER;       	//будем использовать для операций записи (см. поток WriteThread)
OVERLAPPED overlapped_SM;		//будем использовать для операций чтения (см. поток ReadThread)
OVERLAPPED overlappedwr_SM;       	//будем использовать для операций записи (см. поток WriteThread)
OVERLAPPED overlappedspectr;
OVERLAPPED overlappedspectrgo_SM;
OVERLAPPED overlappedspectrgo_AER;

// Указатели для работы с файлами
FILE *data;

//---------------------------------------------------------------------------

bool fl=0;	//флаг, указывающий на успешность операций записи (1 - успешно, 0 - не успешно)
char nd=0;

unsigned int counter_AER = 0;	//счётчик принятых байтов, обнуляется при каждом открытии порта
unsigned int counter_SM = 0;
//=============================================================================
void COMOpen(void);             //открыть порт
void COMClose(void);            //закрыть порт
//.............................. объявления потоков ...........................
//=============================================================================

HANDLE reader_AER;	//дескриптор потока чтения из порта
HANDLE writer_AER;	//дескриптор потока записи в порт
HANDLE reader_SM;	//дескриптор потока чтения из порта
HANDLE writer_SM;	//дескриптор потока записи в порт
HANDLE spectr;

DWORD WINAPI ReadThread_AER(LPVOID);
//DWORD WINAPI WriteThread_AER(LPVOID);
DWORD WINAPI ReadThread_SM(LPVOID);
DWORD WINAPI WriteThread_SM(LPVOID);

DWORD WINAPI SpectrThread(LPVOID);

//=============================================================================
//.............................. реализация потоков AER........................
//=============================================================================

//-----------------------------------------------------------------------------
//............................... поток ReadThread AER.........................
//-----------------------------------------------------------------------------


void ReadPrinting_AER(void); //ф-я записи потока в файл и на форму
void ReadPrinting_SM(void); //ф-я записи потока в файл и на форму


//---------------------------------------------------------------------------

//главная функция потока, реализует приём байтов из COM-порта
DWORD WINAPI ReadThread_AER(LPVOID)
{
 COMSTAT comstat;		//структура текущего состояния порта, в данной программе используется для определения количества принятых в порт байтов
 DWORD btr, temp, mask, signal;	//переменная temp используется в качестве заглушки


 overlapped_AER.hEvent = CreateEvent(NULL, true, true, NULL);	//создать сигнальный объект-событие для асинхронных операций
 overlappedspectrgo_AER.hEvent = CreateEvent(NULL, true, true, NULL);
 SetCommMask(COMport_AER, EV_RXCHAR);                   	    //установить маску на срабатывание по событию приёма байта в порт
 while(1)						//пока поток не будет прерван, выполняем цикл
  {
   WaitCommEvent(COMport_AER, &mask, &overlapped_AER);               	//ожидать события приёма байта (это и есть перекрываемая операция)
   signal = WaitForSingleObject(overlapped_AER.hEvent, INFINITE);	//приостановить поток до прихода байта
   if(signal == WAIT_OBJECT_0)				        //если событие прихода байта произошло
	{
	 if(GetOverlappedResult(COMport_AER, &overlapped_AER, &temp, true)) //проверяем, успешно ли завершилась перекрываемая операция WaitCommEvent
	  if((mask & EV_RXCHAR)!=0)					//если произошло именно событие прихода байта
	   {
		ClearCommError(COMport_AER, &temp, &comstat);		//нужно заполнить структуру COMSTAT
		btr = comstat.cbInQue;                          	//и получить из неё количество принятых байтов


		while(btr--){
			ReadFile(COMport_AER, bufrd_AER+counter_AER, 1, &temp, &overlapped_AER);
			if(*(bufrd_AER+counter_AER++) == '\r'){
				ReadPrinting_AER();
				counter_AER = 0;
			}
		}

	   }
	}
	//SuspendThread(reader_AER);
  }
}

//---------------------------------------------------------------------------

//выводим принятые байты на экран и в файл (если включено)
void ReadPrinting_AER()
{
 strcpy(buf, bufrd_AER);

 if(!strncmp(buf+2, "M01",3) || !strncmp(buf, "M02",3)){
	//Form1->Memo1->Lines->Add((char*)bufrd_AER);	 //выводим принятую строку в Memo
   Form1->Memo1->Text = Form1->Memo1->Text + ((char*)bufrd_AER) + '\n';
   ++nd;

   if (fl) {
	   if(!strncmp(buf+2, "M01",3)) sscanf(buf+6, "%f", &dx);
	   else  sscanf(buf+4, "%f", &dy);
	   Form1->Series1->AddXY(len,(dx+dy)/2);
	   if(data!=NULL){
		   //записать в файл данные из приёмного буфера
		   (nd>1) && fprintf(data, "%i\t%6.3f\t%6.3f\n", len, dx, dy);
		   //fprintf(data, "%s", buf+6);
		   //fprintf(data, "\n");
	   }
	   else{
			ShowMessage("Error: file didn't open!");
	   }

	   fl && SetEvent(overlappedspectrgo_AER.hEvent);
   }
   if (nd>1) {
	SuspendThread(reader_AER);
	nd=0;
   }
 }
 else{
		memset(buf, 0, BUFSIZE);
		memset(bufrd_AER, 0, BUFSIZE);
		counter_AER = 0;
		//SuspendThread(reader_AER);
 }

 memset(buf, 0, BUFSIZE);
 memset(bufrd_AER, 0, BUFSIZE);			//очистить буфер (чтобы данные не накладывались друг на друга)


 }

//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//............................... поток WriteThead_AER.........................
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------

//главная функция потока, выполняет передачу байтов из буфера в COM-порт
/*
DWORD WINAPI WriteThread_AER(LPVOID)
{
 DWORD temp, signal;	//temp - переменная-заглушка

 overlappedwr_AER.hEvent = CreateEvent(NULL, true, true, NULL);   	  //создать событие
 while(1)
  {WriteFile(COMport_AER, bufwr_AER, strlen(bufwr_AER), &temp, &overlappedwr_AER);  //записать байты в порт (перекрываемая операция!)
   signal = WaitForSingleObject(overlappedwr_AER.hEvent, INFINITE);	  //приостановить поток, пока не завершится перекрываемая операция WriteFile

   if((signal == WAIT_OBJECT_0) && (GetOverlappedResult(COMport_AER, &overlappedwr_AER, &temp, true)))	//если операция завершилась успешно
     {
      Form1->StatusBar1->Panels->Items[0]->Text  = "Sending is successful";    //вывести сообщение об этом в строке состояния
     }
   else
   {
     //иначе вывести в строке состояния сообщение об ошибке
     Form1->StatusBar1->Panels->Items[0]->Text  = "Error of sending";
     ShowMessage("Error of sending!");
     }

   Form1->StatusBar1->Panels->Items[1]->Text = temp;
   SuspendThread(writer_AER);

  }
}
*/

//---------------------------------------------------------------------------
//=============================================================================
//.............................. реализация потоков SM.........................
//=============================================================================

void ReadPrinting_SM(void); //ф-я записи потока в файл и на форму

//-----------------------------------------------------------------------------
//............................... поток ReadThead_SM...........................
//-----------------------------------------------------------------------------


//---------------------------------------------------------------------------

//главная функция потока, реализует приём байтов из COM-порта
DWORD WINAPI ReadThread_SM(LPVOID)
{
 COMSTAT comstat;		//структура текущего состояния порта, в данной программе используется для определения количества принятых в порт байтов
 DWORD btr, temp, mask, signal;	//переменная temp используется в качестве заглушки

 //InitializeCriticalSection(&cs_SM);
 overlapped_SM.hEvent = CreateEvent(NULL, true, true, NULL);	//создать сигнальный объект-событие для асинхронных операций
 overlappedspectrgo_SM.hEvent = CreateEvent(NULL, true, true, NULL);
 SetCommMask(COMport_SM, EV_RXCHAR);                   	        //установить маску на срабатывание по событию приёма байта в порт
 while(1)						//пока поток не будет прерван, выполняем цикл
  {
   WaitCommEvent(COMport_SM, &mask, &overlapped_SM);               	//ожидать события приёма байта (это и есть перекрываемая операция)
   signal = WaitForSingleObject(overlapped_SM.hEvent, INFINITE);	//приостановить поток до прихода байта
   if(signal == WAIT_OBJECT_0)				        //если событие прихода байта произошло
    {
     if(GetOverlappedResult(COMport_SM, &overlapped_SM, &temp, true)) //проверяем, успешно ли завершилась перекрываемая операция WaitCommEvent
      if((mask & EV_RXCHAR)!=0)					//если произошло именно событие прихода байта
       {
        ClearCommError(COMport_SM, &temp, &comstat);		//нужно заполнить структуру COMSTAT
        btr = comstat.cbInQue;                          	//и получить из неё количество принятых байтов


		while(btr--){
			ReadFile(COMport_SM, bufrd_SM+counter_SM, 1, &temp, &overlapped_SM);
			if(*(bufrd_SM+counter_SM++) == '\n'){
				ReadPrinting_SM();
				counter_SM = 0;
			}
		}
		/*
		if(btr)                         			//если действительно есть байты для чтения
        {
         EnterCriticalSection(&cs_SM);
         ReadFile(COMport_SM, bufrd_SM, btr, &temp, &overlapped_SM);     //прочитать байты из порта в буфер программы
         counter_SM+=btr;                                          //увеличиваем счётчик байтов
         ReadPrinting_SM();                      		//вызываем функцию для вывода данных на экран и в файл
         fl && SetEvent(overlappedspectrgo_SM.hEvent);
         LeaveCriticalSection(&cs_SM);
		}
		*/
       }
    }
  }
}

//---------------------------------------------------------------------------

//выводим принятые байты на экран и в файл (если включено)
void ReadPrinting_SM()
{
  //Form1->StatusBar1->Panels->Items[2]->Text = "Input: " + IntToStr(counter_SM) + " bytes";	//выводим счётчик в строке состояния
  //fl && SetEvent(overlappedspectrgo_SM.hEvent);

	strcpy(buf, bufrd_SM);

		//Form1->Memo1->Lines->Add((char*)bufrd_SM);	 //выводим принятую строку в Memo
	   Form1->Memo1->Text = Form1->Memo1->Text + ((char*)bufrd_SM);

	 if(fl && (!strncmp(bufrd_SM, "RUAM",4)||!strncmp(bufrd_SM, "GOAM",4))){
		//ShowMessage("!");
		SetEvent(overlappedspectrgo_SM.hEvent);
	 }


	 memset(buf, 0, BUFSIZE);
	 memset(bufrd_SM, 0, BUFSIZE);			//очистить буфер (чтобы данные не накладывались друг на друга)
 }

//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//............................... поток WriteThead_SM..........................
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------

//главная функция потока, выполняет передачу байтов из буфера в COM-порт
DWORD WINAPI WriteThread_SM(LPVOID)
{
 DWORD temp, signal;	//temp - переменная-заглушка

 overlappedwr_SM.hEvent = CreateEvent(NULL, true, true, NULL);   	  //создать событие
 while(1)
  {WriteFile(COMport_SM, bufwr_SM, strlen(bufwr_SM), &temp, &overlappedwr_SM);  //записать байты в порт (перекрываемая операция!)
   signal = WaitForSingleObject(overlappedwr_SM.hEvent, INFINITE);	  //приостановить поток, пока не завершится перекрыва�емая операция WriteFile

   if((signal == WAIT_OBJECT_0) && (GetOverlappedResult(COMport_SM, &overlappedwr_SM, &temp, true)))	//если операция завершилась успешно
	 {
	  //Form1->StatusBar1->Panels->Items[0]->Text  = "Sending to MS is successful";    //вывести сообщение об этом в строке состояния
	 }
   else
   {
     //иначе вывести в строке состояния сообщение об ошибке
	 //Form1->StatusBar1->Panels->Items[0]->Text  = "Error of sending";
	 ShowMessage("Error of sending to MS!");
	 }
   SuspendThread(writer_SM);

  }
}

//-----------------------------------------------------------------------------
//............................... поток SpectrThead ...........................
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------

//главная функция потока, выполняет передачу байтов из буфера в COM-порт

	DWORD WINAPI SpectrThread(LPVOID)
	{
	 DWORD temp, signal;	//temp - переменная-заглушка
	 //float str_spectr, fin_spectr, len;
	 String t;
	 char t1[BUFSIZE];
	 int l0, lf, step;

	 fl=1;
	 overlappedspectr.hEvent = CreateEvent(NULL, true, true, NULL);   	  //создать событие



	 wcstombs(t1, Form1->Edit2->Text.c_str(), wcslen(Form1->Edit2->Text.c_str()));
	 step = atoi(t1);
	 wcstombs(t1, Form1->Edit3->Text.c_str(), wcslen(Form1->Edit3->Text.c_str()));
	 len = atoi(t1);
	 wcstombs(t1, Form1->Edit4->Text.c_str(), wcslen(Form1->Edit4->Text.c_str()));
	 lf = atoi(t1);


	 //запись команды
	 ResetEvent(overlappedspectrgo_SM.hEvent);
	 memset(bufwr_SM,0,BUFSIZE);			//очистить программный передающий буфер, чтобы данные не накладывались друг на друга
	 PurgeComm(COMport_SM, PURGE_TXCLEAR);             //очистить передающий буфер порта

	 strcpy(bufwr_SM, "AMGO\r\n");

	 ResumeThread(writer_SM);               //активировать поток записи данных в порт
	 WaitForSingleObject(overlappedspectrgo_SM.hEvent, INFINITE);

	 if(len){
	 ResetEvent(overlappedspectrgo_SM.hEvent);
	 memset(bufwr_SM,0,BUFSIZE);			//очистить программный передающий буфер, чтобы данные не накладывались друг на друга
	 PurgeComm(COMport_SM, PURGE_TXCLEAR);             //очистить передающий буфер порта

	 strcpy(bufwr_SM, "AMRU ");
	 sprintf(bufwr_SM+5, "%i", -step);
	 strcat(bufwr_SM, "\r\n");

	 ResumeThread(writer_SM);               //активировать поток записи данных в порт
	 WaitForSingleObject(overlappedspectrgo_SM.hEvent, INFINITE);
	 }



	 ShowMessage("Press Enter to start measurment");

	 Sleep(150);

	 while(len < (lf+step))
	  {
        ResetEvent(overlappedspectrgo_AER.hEvent);

		ResumeThread(reader_AER);
		WaitForSingleObject(overlappedspectrgo_AER.hEvent, INFINITE);


		ResetEvent(overlappedspectrgo_SM.hEvent);

		memset(bufwr_SM,0,BUFSIZE);			//очистить программный передающий буфер, чтобы данные не накладывались друг на друга
		PurgeComm(COMport_SM, PURGE_TXCLEAR);             //очистить передающий буфер порта

		strcpy(bufwr_SM, "AMRU ");
		sprintf(bufwr_SM+5, "%i", step);
		strcat(bufwr_SM, "\r\n");

		ResumeThread(writer_SM);               //активировать поток записи данных в порт
		WaitForSingleObject(overlappedspectrgo_SM.hEvent, INFINITE);


		Sleep(100);

		len+= step;
	  }


	  fl=0;


	  ShowMessage("Enjoy");

	  Form1->StaticText3->Enabled =  true;
	  Form1->StaticText4->Enabled =  true;
	  Form1->StaticText5->Enabled =  true;
	  Form1->StaticText6->Enabled =  true;
	  Form1->Edit2->Enabled =  true;
	  Form1->Edit3->Enabled =  true;
	  Form1->Edit4->Enabled =  true;
	  Form1->BitBtn3->Enabled =  true;

	  SuspendThread(spectr);

	}


//---------------------------------------------------------------------------
//=============================================================================
//............................. элементы формы ................................
//=============================================================================

//конструктор формы, обычно в нём выполняется инициализация элементов формы
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
//инициализация элементов формы при запуске программы
}
//---------------------------------------------------------------------------


//=============================================================================
//........................... реализации функций ..............................
//=============================================================================

//функция открытия и инициализации порта
void COMOpen()
{
 String portname_AER, portname_SM;     	 //имя порта (например, "COM1", "COM2" и т.д.)
 DCB dcb_AER, dcb_SM;                //структура для общей инициализации порта DCB
 COMMTIMEOUTS timeouts_AER, timeouts_SM;  //структура для установки таймаутов

 portname_AER = "\\\\.\\" + Form1->ComboBox1->Text;	//получить имя выбранного порта
 portname_SM = "\\\\.\\" + Form1->ComboBox2->Text;	//получить имя выбранного порта

 //открыть порт, для асинхронных операций обязательно нужно указать флаг FILE_FLAG_OVERLAPPED
 COMport_AER = CreateFile(portname_AER.c_str(),GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
 COMport_SM = CreateFile(portname_SM.c_str(),GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
 //COMport = CreateFile(L"\\\\.\\COM10",GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
 //здесь:
 // - portname.c_str() - имя порта в качестве имени файла, c_str() преобразует строку типа String в строку в виде массива типа char, иначе функция не примет
 // - GENERIC_READ | GENERIC_WRITE - доступ к порту на чтение/записть
 // - 0 - порт не может быть общедоступным (shared)
 // - NULL - дескриптор порта не наследуется, используется дескриптор безопасности по умолчанию
 // - OPEN_EXISTING - порт должен открываться как уже существующий файл
 // - FILE_FLAG_OVERLAPPED - этот флаг указывает на использование асинхронных операций
 // - NULL - указатель на файл шаблона не используется при работе с портами

 if(COMport_AER == INVALID_HANDLE_VALUE)            //если ошибка открытия порта
  {
   Form1->SpeedButton1->Down = false;           //отжать кнопку
   ShowMessage("Failed connection to AER!");
   return;
  }

  if(COMport_SM == INVALID_HANDLE_VALUE)            //если ошибка открытия порта
  {
   Form1->SpeedButton1->Down = false;           //отжать кнопку
   ShowMessage("Failed connection to SM!");
   return;
  }

 //инициализация порта AER

 dcb_AER.DCBlength = sizeof(DCB); 	//в первое поле структуры DCB необходимо занести её длину, она будет использоваться функциями настройки порта для контроля корректности структуры

 //считать структуру DCB из порта
 if(!GetCommState(COMport_AER, &dcb_AER))	//если не удалось - закрыть порт и вывести сообщение об ошибке в строке состояния
  {
   COMClose();
   ShowMessage("DCB_AER error!");
   return;
  }

 //инициализация структуры DCB
 dcb_AER.BaudRate = CBR_9600;       						//задаём скорость передачи 115200 бод
 dcb_AER.fBinary = TRUE;                                    //включаем двоичный режим обмена
 dcb_AER.fOutxCtsFlow = FALSE;                              //выключаем режим слежения за сигналом CTS
 dcb_AER.fOutxDsrFlow = FALSE;                              //выключаем режим слежения за сигналом DSR
 dcb_AER.fDtrControl = DTR_CONTROL_DISABLE;                 //отключаем использование линии DTR
 dcb_AER.fDsrSensitivity = FALSE;                           //отключаем восприимчивость драйвера к состоянию линии DSR
 dcb_AER.fNull = FALSE;                                     //разрешить приём нулевых байтов
 dcb_AER.fRtsControl = RTS_CONTROL_DISABLE;                 //отключаем использование линии RTS
 dcb_AER.fAbortOnError = FALSE;                             //отключаем остановку всех операций чтения/записи при ошибке
 dcb_AER.ByteSize = 8;                                      //задаём 8 бит в байте
 dcb_AER.Parity = 0;                                        //отключаем проверку чётности
 dcb_AER.StopBits = 0;                                      //задаём один стоп-бит
 //dcb.EvtChar = 'LF';

 //загрузить структуру DCB в порт
 if(!SetCommState(COMport_AER, &dcb_AER))	//если не удалось - закрыть порт и вывести сообщение об ошибке в строке состояния
  {
   COMClose();
   ShowMessage("Failed to install DCB_AER");
   return;
  }

 GetCommTimeouts( COMport_AER, &timeouts_AER );
 //установить таймауты
 /*
 timeouts.ReadIntervalTimeout = MAXDWORD;	 	//таймаут между двумя символами
 timeouts.ReadTotalTimeoutMultiplier = 10000000000;	//общий таймаут операции чтения
 timeouts.WriteTotalTimeoutMultiplier = 1000000;      //общий таймаут операции записи
 */
 //timeouts.WriteTotalTimeoutConstant = 10;        //константа для общего таймаута операции записи
 //timeouts.ReadTotalTimeoutConstant = 10000;         //константа для общего таймаута операции чтения
  timeouts_AER.ReadIntervalTimeout        = 0;
  timeouts_AER.ReadTotalTimeoutMultiplier    = 0;
  timeouts_AER.ReadTotalTimeoutConstant    = 0;
  timeouts_AER.WriteTotalTimeoutMultiplier    = 0;
  timeouts_AER.WriteTotalTimeoutConstant    = 0;

 //записать структуру таймаутов в порт
 if(!SetCommTimeouts(COMport_AER, &timeouts_AER))	//если не удалось - закрыть порт и вывести сообщение об ошибке в строке состояния
  {
   COMClose();
   ShowMessage("Timeouts_AER error");
   return;
  }

  //инициализация порта SM

 dcb_SM.DCBlength = sizeof(DCB); 	//в первое поле структуры DCB необходимо занести её длину, она будет использоваться функциями настройки порта для контроля корректности структуры

 //считать структуру DCB из порта
 if(!GetCommState(COMport_SM, &dcb_SM))	//если не удалось - закрыть порт и вывести сообщение об ошибке в строке состояния
  {
   COMClose();
   ShowMessage("DCB SM error!");
   return;
  }

 //инициализация структуры DCB
 dcb_SM.BaudRate = CBR_115200;       //задаём скорость передачи 115200 бод
 dcb_SM.fBinary = TRUE;                                    //включаем двоичный режим обмена
 dcb_SM.fOutxCtsFlow = FALSE;                              //выключаем режим слежения за сигналом CTS
 dcb_SM.fOutxDsrFlow = FALSE;                              //выключаем режим слежения за сигналом DSR
 dcb_SM.fDtrControl = DTR_CONTROL_DISABLE;                 //отключаем использование линии DTR
 dcb_SM.fDsrSensitivity = FALSE;                           //отключаем восприимчивость драйвера к состоянию линии DSR
 dcb_SM.fNull = FALSE;                                     //разрешить приём нулевых байтов
 dcb_SM.fRtsControl = RTS_CONTROL_DISABLE;                 //отключаем использование линии RTS
 dcb_SM.fAbortOnError = FALSE;                             //отключаем остановку всех операций чтения/записи при ошибке
 dcb_SM.ByteSize = 8;                                      //задаём 8 бит в байте
 dcb_SM.Parity = 0;                                        //отключаем проверку чётности
 //dcb_SM.EofChar = EOM;
 dcb_SM.StopBits = 2;                                      //задаём один стоп-бит

 //загрузить структуру DCB в порт
 if(!SetCommState(COMport_SM, &dcb_SM))	//если не удалось - закрыть порт и вывести сообщение об ошибке в строке состояния
  {
   COMClose();
   ShowMessage("Failed to install DCB to SM");
   return;
  }

  GetCommTimeouts( COMport_SM, &timeouts_SM );
 //установить таймауты

  timeouts_SM.ReadIntervalTimeout        = MAXDWORD;
  timeouts_SM.ReadTotalTimeoutMultiplier    = 0;
  timeouts_SM.ReadTotalTimeoutConstant    = 0;
  timeouts_SM.WriteTotalTimeoutMultiplier    = 0;
  timeouts_SM.WriteTotalTimeoutConstant    = 0;

 //записать структуру таймаутов в порт
 if(!SetCommTimeouts(COMport_SM, &timeouts_SM))	//если не удалось - закрыть порт и вывести сообщение об ошибке в строке состояния
  {
   COMClose();
   ShowMessage("Timeouts SM error");
   return;
  }

 //установить размеры очередей приёма и передачи
 SetupComm(COMport_AER,4096,4096);
 SetupComm(COMport_SM,4096,4096);

 //создать или открыть существующий файл для записи принимаемых данных
 if(Form1->CheckBox1->Checked)
 {
   //handle = open("test.txt", O_CREAT | O_APPEND | O_BINARY | O_WRONLY, S_IREAD | S_IWRITE);
   data = fopen("Aeroel_Log.txt", "wb+");

   if(data==NULL){		//если произошла ошибка открытия файла
	 ShowMessage("File \"Aeroel_Log.txt\" havn't created!");
   }

 PurgeComm(COMport_AER, PURGE_RXCLEAR);	//очистить принимающий буфер порта
 PurgeComm(COMport_SM, PURGE_RXCLEAR);
 reader_AER = CreateThread(NULL, 0, ReadThread_AER, NULL, CREATE_SUSPENDED, NULL);			//создаём поток чтения, который сразу начнёт выполняться (предпоследний параметр = 0)
 //writer_AER = CreateThread(NULL, 0, WriteThread_AER, NULL, CREATE_SUSPENDED, NULL);	//создаём поток записи в остановленном состоянии (предпоследний параметр = CREATE_SUSPENDED)

 reader_SM = CreateThread(NULL, 0, ReadThread_SM, NULL, 0, NULL);			//создаём поток чтения, который сразу начнёт выполняться (предпоследний параметр = 0)
 writer_SM = CreateThread(NULL, 0, WriteThread_SM, NULL, CREATE_SUSPENDED, NULL);	//создаём поток записи в остановленном состоянии (предпоследний параметр = CREATE_SUSPENDED)

 spectr = CreateThread(NULL, 0, SpectrThread, NULL, CREATE_SUSPENDED, NULL);

 //memset(buf_AER, 0, GENBUFSIZE);
 //memset(buf_SM, 0, GENBUFSIZE);
}
}


//функция закрытия порта
void COMClose()
{
//Примечание: так как при прерывании потоков, созданных с помощью функций WinAPI, функцией TerminateThread
//	      поток может быть прерван жёстко, в любом месте своего выполнения, то освобождать дескриптор
//	      сигнального объекта-события, находящегося в структуре типа OVERLAPPED, связанной с потоком,
//	      следует не внутри кода потока, а отдельно, после вызова функции TerminateThread.
//	      После чего нужно освободить и сам дескриптор потока.

 if(writer_AER)		//если поток записи работает, завершить его; проверка if(writer) обязательна, иначе возникают ошибки
  {TerminateThread(writer_AER,0);
   CloseHandle(overlappedwr_AER.hEvent);	//нужно закрыть объект-событие
   CloseHandle(writer_AER);
  }
 if(reader_AER)		   //если поток чтения работает, завершить его; проверка if(reader) обязательна, иначе возникают ошибки
  {TerminateThread(reader_AER,0);
   CloseHandle(overlapped_AER.hEvent);	//нужно закрыть объект-событие
   CloseHandle(reader_AER);
  }

 CloseHandle(COMport_AER);                  //закрыть порт
 COMport_AER=0;				//обнулить переменную для дескриптора порта

 if(writer_SM)		//если поток записи работает, завершить его; проверка if(writer) обязательна, иначе возникают ошибки
  {TerminateThread(writer_SM,0);
   CloseHandle(overlappedwr_SM.hEvent);	//нужно закрыть объект-событие
   CloseHandle(writer_SM);
  }
 if(reader_SM)		   //если поток чтения работает, завершить его; проверка if(reader) обязательна, иначе возникают ошибки
  {TerminateThread(reader_SM,0);
   CloseHandle(overlapped_SM.hEvent);	//нужно закрыть объект-событие
   CloseHandle(reader_SM);
  }

  CloseHandle(COMport_SM);                  //закрыть порт
  COMport_SM=0;				//обнулить переменную для дескриптора порта

  if(spectr)		   //если поток чтения работает, завершить его; проверка if(reader) обязательна, иначе возникают ошибки
  {TerminateThread(spectr,0);
   CloseHandle(overlappedspectr.hEvent);	//нужно закрыть объект-событие
   CloseHandle(overlappedspectrgo_SM.hEvent);
   CloseHandle(overlappedspectrgo_AER.hEvent);
   CloseHandle(spectr);
  }

  fclose(data);				//закрыть файл, в который велась запись принимаемых данных
  data=NULL;				//обнулить переменную для дескриптора файла

}

//---------------------------------------------------------------------------
//обработчик нажатия на кнопку "Открыть порт"

//---------------------------------------------------------------------------
void __fastcall TForm1::SpeedButton1Click(TObject *Sender)
{
 if(SpeedButton1->Down)
  {
   COMOpen();                   //если кнопка нажата - открыть порт

	 if(COMport_AER != INVALID_HANDLE_VALUE)
   {
    //показать/спрятать элементы на форме
	Form1->GroupBox1->Enabled = true;
	//Form1->GroupBox2->Enabled = false;
	Form1->CheckBox1->Enabled = false;
	Form1->CheckBox2->Enabled = false;

	Form1->Label3->Visible =  true;
	Form1->Edit1->Visible =  true;

	Form1->BitBtn2->Visible =  true;
	Form1->StaticText3->Visible =  true;
	Form1->StaticText4->Visible =  true;
	Form1->StaticText5->Visible =  true;
	Form1->StaticText6->Visible =  true;
	Form1->Edit2->Visible =  true;
	Form1->Edit3->Visible =  true;
	Form1->Edit4->Visible =  true;
	Form1->BitBtn3->Visible =  true;

	Form1->Chart1->Visible =  true;

	if(Form1->CheckBox2->Checked) {
	   Form1->Label6->Visible =  true;
	   Form1->Memo1->Visible = 	 true;
	   Form1->BitBtn1->Visible = true;
	}

	/*
	memset(bufwr_AER,0,BUFSIZE);			//очистить программный передающий буфер, чтобы данные не накладывались друг на друга
	PurgeComm(COMport, PURGE_TXCLEAR);             //очистить передающий буфер порта
	strcpy(bufwr_AER, "$PKON1,0,2,,,0300,A*6B\r\n");
	//strcpy(bufwr, Form1->Edit1->Text.c_str());      //занести в программный передающий буфер строку из Edit1
	//strcpy(bufwr+4, "\r\n");      //занести в программный передающий буфер строку из Edit1
	ResumeThread(writer);               //активировать поток записи данных в порт
	*/

	Form1->SpeedButton1->Caption = "Disconnect";	//сменить надпись на кнопке

	counter_AER = 0;	//сбрасываем счётчик байтов
	counter_SM = 0;	//сбрасываем счётчик байтов
   }
  }

 else
  {
   COMClose();                  //если кнопка отжата - закрыть порт

   Form1->SpeedButton1->Caption = "Connect";	//сменить надпись на кнопке

   //показать/спрятать элементы на форме
	Form1->CheckBox1->Enabled = true;
	Form1->CheckBox2->Enabled = true;
	Form1->Label3->Visible =  false;
	Form1->Edit1->Visible =  false;

	Form1->BitBtn2->Visible =  false;
	Form1->StaticText3->Visible =  false;
	Form1->StaticText4->Visible =  false;
	Form1->StaticText5->Visible =  false;
	Form1->StaticText6->Visible =  false;
	Form1->Edit2->Visible =  false;
	Form1->Edit3->Visible =  false;
	Form1->Edit4->Visible =  false;
	Form1->BitBtn3->Visible =  false;

    Form1->Chart1->Visible =  true;

	Form1->Label6->Visible =  false;
	Form1->Memo1->Visible =   false;
	Form1->BitBtn1->Visible = false;

  }
}


//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn1Click(TObject *Sender)
{
  Form1->Memo1->Clear();	//очистить Memo1
}
//---------------------------------------------------------------------------



void __fastcall TForm1::BitBtn2Click(TObject *Sender)
{

	memset(bufwr_SM,0,BUFSIZE);			//очистить программный передающий буфер, чтобы данные не накладывались друг на друга
	PurgeComm(COMport_SM, PURGE_TXCLEAR);             //очистить передающий буфер порта
	wcstombs(bufwr_SM, Form1->Edit1->Text.c_str(), wcslen(Form1->Edit1->Text.c_str()));
	//wcstombs(bufwr_SM+wcslen(Form1->Edit1->Text.c_str()), "\r\n", 2);
	strcpy(bufwr_SM+wcslen(Form1->Edit1->Text.c_str()), "\r\n");
	ResumeThread(writer_SM);               //активировать поток записи данных в порт
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn3Click(TObject *Sender)
{
	Form1->StaticText3->Enabled =  false;
	Form1->StaticText4->Enabled =  false;
	Form1->StaticText5->Enabled =  false;
	Form1->StaticText6->Enabled =  false;
	Form1->Edit2->Enabled =  false;
	Form1->Edit3->Enabled =  false;
	Form1->Edit4->Enabled =  false;
	Form1->BitBtn3->Enabled =  false;

   ResumeThread(spectr);
}


