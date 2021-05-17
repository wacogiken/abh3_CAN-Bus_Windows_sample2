/*
 ******************************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2021, Waco Giken Co., Ltd.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/o2r other materials provided
 *     with the distribution.
 *   * Neither the name of the Waco Giken nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************

******************************************************************************
@file           test_canABH3.cpp
******************************************************************************
@brief          ABH3�pCAN �T���v���\�t�g(�A�v���P�[�V����)
******************************************************************************
@date           2021.03.03
@author         Y.Ogawa
@note			1st release
@note           This sourcecode is based by "test_canABH3.c"
******************************************************************************
*/

#include "stdafx.h"
#include <conio.h>
#include <locale.h>
#include "CANa31.h"
#include "typedef.h"
#include "CANa31dll.h"


//�ǂݍ���DLL�t�@�C���iCanA31�v���W�F�N�g�̏o�́j
#define DLLFILE		_T("CanA31.dll")

//�ݒ�v�f
#define USEIF		(0)		//0..IxxatV2�f�o�C�X���g�p	1..IxxatSimple�f�o�C�X���g�p
#define IFNUM		(0)		//IxxatV2�̏ꍇ���{�ڂ�(0�J�n)�AIxxatSimple�̏ꍇCOM�|�[�g�ԍ�(1�J�n)
#define TGTID		(1)		//�^�[�Q�b�g(ABH3)���Ŏg�p����ID
#define	HOSTID		(2)		//�z�X�g(PC)���Ŏg�p����ID
#define	OPENMS		(1000)	//������J���ꍇ�̃^�C���A�E�g�l[ms]
#define SENDMS		(1000)	//���M����ꍇ�̃^�C���A�E�g�l[ms]
#define RECVMS		(1000)	//��M����ꍇ�̃^�C���A�E�g�l[ms]
#define	BAUDRATE	(500)	//CAN�ʐM�Ɏg�p����{�[���[�g[kbps]
#define BRDGRP		(5)		//�u���[�h�L���X�g�O���[�v�ԍ�

//DLL�̊֐����ȒP�Ɏg���ׂ̍\����
CA3_FUNCLIST funcList;

//
void broadCastLoop(int ms)
	{
	//���M�f�[�^�Ǝ�M�f�[�^�̊i�[��
	CANABH3_RESULT res;

	int i,j;

	for (i = 0; i < 1; i++)
		{
		for (j = 0; j < 7; j++)
			{
			funcList.abh3_can_reqBRD(BRDGRP * 8 + j,&res);
			switch (j)
				{
				case 0:
					::_tprintf(_T("Error: %08x  Alarm: %08x\n")
						,res.u.BR0.nErrorBit
						,res.u.BR0.nWarnBit
						);
					break;
				case 1:
					::_tprintf(_T("control: %08x  in_out: %08x\n")
						,res.u.BR1.nCtrlBit
						,res.u.BR1.nIOflag
						);
					break;
				case 2:
					::_tprintf(_T("velCmdAY: %10.6f  velCmdBX: %10.6f  velFbkAY: %10.6f  velFbkBX: %10.6f\n")
						,funcList.cnvCAN2Vel(res.u.BR2.nOrderSpeedAY)
						,funcList.cnvCAN2Vel(res.u.BR2.nOrderSpeedBX)
						,funcList.cnvCAN2Vel(res.u.BR2.nBackSpeedAY)
						,funcList.cnvCAN2Vel(res.u.BR2.nBackSpeedBX)
						);
					break;
				case 3:
					::_tprintf(_T("curCmdAY: %10.6f  curCmdBX: %10.6f  loadA: %10.6f  loadB: %10.6f\n")
						,funcList.cnvCAN2Cur(res.u.BR3.nOrderCurrentAY)
						,funcList.cnvCAN2Cur(res.u.BR3.nOrderCurrentBX)
						,funcList.cnvCAN2Load(res.u.BR3.nLoadA)
						,funcList.cnvCAN2Load(res.u.BR3.nLoadB)
						);
					break;
				case 4:
					::_tprintf(_T("pulseA: %d  pulseB: %d\n")
						,res.u.BR4.nInPulseA
						,res.u.BR4.nInPulseB
						);
					break;
				case 5:
					::_tprintf(_T("analog0: %10.6f  analog1: %10.6f  mainVolt: %10.6f  controlVolt: %10.6f\n")
						,funcList.cnvCAN2Analog(res.u.BR5.nAnalog0)
						,funcList.cnvCAN2Analog(res.u.BR5.nAnalog1)
						,funcList.cnvCAN2Volt(res.u.BR5.nPowerMain)
						,funcList.cnvCAN2Volt(res.u.BR5.nPowerCtrl)
						);
					break;
				case 6:
					::_tprintf(_T("monitor0: %10.6f  monitor1: %10.6f\n")
						,res.u.BR6.nMonitor0
						,res.u.BR6.nMonitor1
					);
					break;
				case 7:
					::_tprintf(_T("BRD7: %d %d %d %d %d %d %d %d\n")
						,res.u.raw[0]
						,res.u.raw[1]
						,res.u.raw[2]
						,res.u.raw[3]
						,res.u.raw[4]
						,res.u.raw[5]
						,res.u.raw[6]
						,res.u.raw[7]
						);
					break;
				}

			Sleep(ms);
			}
		::_tprintf(_T("\n"));
		}
	}


int main()
	{
	//�߂�l
	int nResult = 0;

	//========================================
	//������ˑ��v�f
	//========================================

	//�R���\�[���ɓ��{��\������ׂɕK�v�Ȑݒ�
	::_tsetlocale(LC_ALL,_T(""));

	//DLL�ǂݍ���
	HMODULE hDLL = ::LoadLibrary(DLLFILE);
	if (hDLL == NULL)
		{
		::_tprintf(_T("�ُ�FDLL���ǂݍ��߂܂���\n"));
		return(1);
		}

	//DLL�̊֐����y�Ɉ����ׁA�p�ӂ��ꂽ�\���̂ɃG���g�����ꊇ�ݒ�
	GetFunctions(hDLL,&funcList);

	//========================================
	//����e�X�g
	//========================================

	//DLL�̎g���n�߂�1�񂾂��Ăяo���֐�
	funcList.InitInstance(USEIF);

	//�C���^�[�t�F�[�X���g�p����{�[���[�g��ݒ�
	funcList.SetBaudrate(BAUDRATE);
	::_tprintf(_T("���F�{�[���[�g��ݒ肵�܂���\n\n"));

	//���M�f�[�^�Ǝ�M�f�[�^�̊i�[��
	CANABH3_RESULT res;

	//�����J�n
	do
		{
		//�ʐM�̃^�C���A�E�g���Ԃ�ݒ�
		funcList.SetOpenTimeout(OPENMS);
		funcList.SetSendTimeout(SENDMS);
		funcList.SetRecvTimeout(RECVMS);
		::_tprintf(_T("���F���e���Ԃ�ݒ肵�܂���\n\n"));

		//�z�X�g(PC)�Ɛ���Ώ�(ABH3)��ID��ݒ�
		funcList.SetHostID(HOSTID);
		funcList.SetTargetID(TGTID);
		::_tprintf(_T("���F�z�X�g�ƃ^�[�Q�b�g��ID��ݒ肵�܂���\n\n"));

		//CAN�C���^�[�t�F�[�X���J��
		int32_t nResult = funcList.OpenInterface(IFNUM);
		if (nResult)
			{
			::_tprintf(_T("�ُ�F�C���^�[�t�F�[�X���J�����Ɉُ�(%d)���������܂���\n\n"),nResult);
			break;
			}
		else
			::_tprintf(_T("���F�C���^�[�t�F�[�X�𐳏�ɊJ���܂���\n\n"));

		//�w�߂̏�����
		nResult = funcList.abh3_can_init();
		if (nResult)
			{
			::_tprintf(_T("�ُ�F�����ݒ�R�}���h(abh3_can_init)�����s(%d)���܂���\n\n"),nResult);
			break;
			}
		else
			::_tprintf(_T("���F�����ݒ�R�}���h(abh3_can_init)���������܂���\n\n"));

		//
		broadCastLoop(10);

		break;
		} while (1);

	//�ʐM��������
	funcList.CloseInterface();

	//�I������
	funcList.ExitInstance();

	//========================================
	//������ˑ��v�f
	//========================================

	//DLL�J��
	::FreeLibrary(hDLL);

	//�I��
	return nResult;
	}

