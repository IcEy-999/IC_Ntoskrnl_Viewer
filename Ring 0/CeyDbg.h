#pragma once
#include <ntddk.h>


#define my_device_name  L"\\Device\\CEYDBG"//�豸��
#define my_link_name L"\\??\\CEYDBG"//����������

#define GET_BASE CTL_CODE(FILE_DEVICE_UNKNOWN,0x99f,METHOD_NEITHER,FILE_ANY_ACCESS)//
#define GET_VALUE CTL_CODE(FILE_DEVICE_UNKNOWN,0x99e,METHOD_NEITHER,FILE_ANY_ACCESS)//��0x80�ֽ�����
