#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "DbgHelp.lib")
#pragma comment(lib, "Urlmon.lib")


extern int Update_Flag;
extern int Answer_Num;
extern int Root;

struct GUID_StreamData
{
	int ver;
	int date;
	int age;
	GUID guid;
};

struct PdbInfo
{
	DWORD	Signature;
	GUID	Guid;
	DWORD	Age;
	char	PdbFileName[1];
};

typedef struct _Pdb_Task
{
	char szDllDir[MAX_PATH];
	char szDllBaseName[MAX_PATH];
	char szSymbolServerUrl[1024];//Pdb ����·��

	char szPdbPath[MAX_PATH];//Pdb����·��
	char szDllFullPath[MAX_PATH];//
	HANDLE hProcess; //��ǰ���̾��
	DWORD64 kenhandle;//���ž��
	HANDLE My_Drive;//�������
	DWORD64 Ntoskrnl_Base;//nt�ں˻�ַ


}Pdb_Task, * PPdb_Task;

typedef struct _Command {
	int Type;//��������д֮�����������

	DWORD64 Address;//������ַ
	int width;//�ֽ� 1 ���� 2 ��˫�� 4 ������ 8
	UCHAR Parameter[10][50];//�ṩ10��50���ȵĿռ�� ����Ͳ�������һ�������������ǲ���
	int Parameter_Num;//ʵ�ò������������Ƶ�һ������
	DWORD64 OutPut_Len;//�������
}Command, * PCommand;

extern Pdb_Task Pdbfile; //pdb_information

extern DWORD  error;//error_code

extern LPCSTR Ntoskrnl_Path;
extern CHAR Current_Path[MAX_PATH];

extern DWORD64 Ntoskrnl_Base_bf;//�ں˻�ַ����Ҫͨ��KdDebuggerEnabled��ַ���
extern PBYTE Ntoskrnl_Image;//read Ntoskrnl.exe add

extern UCHAR Cin_Command[2048];//�����������

#define DEVICE_NAME L"\\\\.\\CEYDBG" //����������
#define GET_BASE CTL_CODE(FILE_DEVICE_UNKNOWN,0x99f,METHOD_NEITHER,FILE_ANY_ACCESS)//��ʼ����ͨ��ƫ�Ƽ����ں˻�ַ
#define GET_VALUE CTL_CODE(FILE_DEVICE_UNKNOWN,0x99e,METHOD_NEITHER,FILE_ANY_ACCESS)//��0x80�ֽ�����





void Error_Out(int E_C);
int Read_Memory(PPdb_Task Pdbfile, PCommand Task,int width);
BOOL CALLBACK EnumSymProc(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext);
int CheckPdb_X(PCSTR Pdbname);

void Load_Drive();
BOOL Install_Drive();
BOOL Start_Drive();
BOOL Stop_Drive();
BOOL Unload_Drive();

void Read_Ntoskrnl_Image();
DWORD Get_Ntoskrnl_Pdb();
void Load_Pdb();

void Ascii_To_Unicode(IN char* a, IN OUT WCHAR* u);