
/*
 * Copyright (c) 2006, 2009, Oracle and/or its affiliates. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <sys/types.h>
#include <stdio.h>
#include <sys/mman.h>

#include "gfx_common.h"		/* GFX Common definitions */
#include "graphicstest.h"
#include "libvtsSUNWast.h"	/* Common VTS library definitions */

#include "X11/Xlib.h"
#include "gfx_vts.h"		/* VTS Graphics Test common routines */
#include "ast.h"

void
ASTSetReg(int fd, int offset, int value)
{
	ast_io_reg	io_reg;
	
	io_reg.offset = offset;
	io_reg.value = value;
	ioctl(fd, AST_SET_IO_REG, &io_reg);
}

void
ASTGetReg(int fd, int offset, int *value)
{
	ast_io_reg	io_reg;
	
	io_reg.offset = offset;
	ioctl(fd, AST_GET_IO_REG, &io_reg);
	*value = io_reg.value;
}

void
ASTSetIndexReg(int fd, int offset, int index, unsigned char value)
{
	ast_io_reg	io_reg;

	io_reg.offset = offset;
	io_reg.value = index;
	ioctl(fd, AST_SET_IO_REG, &io_reg);
	io_reg.offset = offset + 1;
	io_reg.value = value;
	ioctl(fd, AST_SET_IO_REG, &io_reg);
}

void
ASTGetIndexReg(int fd, int offset, int index, unsigned char *value)
{
	ast_io_reg	io_reg;

	io_reg.offset = offset;
	io_reg.value = index;
	ioctl(fd, AST_SET_IO_REG, &io_reg);
	io_reg.offset = offset + 1;
	ioctl(fd, AST_GET_IO_REG, &io_reg);
	*value = io_reg.value;
}

void
ASTSetIndexRegMask(int fd, int offset, int index, int and, unsigned char value)
{
	ast_io_reg	io_reg;
	unsigned char	temp;

	io_reg.offset = offset;
	io_reg.value = index;
	ioctl(fd, AST_SET_IO_REG, &io_reg);
	io_reg.offset = offset + 1;
	ioctl(fd, AST_GET_IO_REG, &io_reg);
	temp = (io_reg.value & and) | value;
	io_reg.offset = offset;
	io_reg.value = index;
	ioctl(fd, AST_SET_IO_REG, &io_reg);
	io_reg.offset = offset + 1;
	io_reg.value = temp;
	ioctl(fd, AST_SET_IO_REG, &io_reg);
}

void
ASTGetIndexRegMask(int fd, int offset, int index, int and, unsigned char *value)
{
	ast_io_reg	io_reg;
	unsigned char	temp;

	io_reg.offset = offset;
	io_reg.value = index;
	ioctl(fd, AST_SET_IO_REG, &io_reg);
	io_reg.offset = offset + 1;
	ioctl(fd, AST_GET_IO_REG, &io_reg);
	*value = io_reg.value & and;
}

void
ASTOpenKey(int fd)
{
	ASTSetIndexReg(fd, CRTC_PORT, 0x80, 0xA8);
}

unsigned int 
ASTMMIORead32(unsigned char *addr)
{
	return (*(volatile unsigned int *)(addr));
}

void 
ASTMMIOWrite32(unsigned char *addr, unsigned int data)
{
	*(unsigned int *)(addr) = data;
}


unsigned int 
ASTMMIORead32_8pp(unsigned char *addr)
{
    union
    {
        unsigned int   ul;
        unsigned char  b[4];
    } data;

    unsigned int m;

    data.ul = *((volatile unsigned int *)(addr));

    m = (((unsigned int)data.b[3]) << 24) |
                (((unsigned int)data.b[2]) << 16) |
                (((unsigned int)data.b[1]) << 8) |
                (((unsigned int)data.b[0]));
    return (m);
}

void 
ASTMMIOWrite32_8pp(unsigned char *addr, unsigned int val)
{
    union
    {
        unsigned int   ul;
        unsigned char  b[4];
    } data;

    unsigned int m;

    data.ul = val;
    m = (((unsigned int)data.b[3]) << 24) |
                (((unsigned int)data.b[2]) << 16) |
                (((unsigned int)data.b[1]) << 8) |
                (((unsigned int)data.b[0]));

    *(unsigned int*)(addr) = m;
}

void
ASTWaitEngIdle(struct ast_info *pAST)
{
	unsigned int ulEngState, ulEngState2;
	unsigned char reg;
	unsigned int ulEngCheckSetting;


	if (pAST->MMIO2D)
	    return;

	ulEngCheckSetting = 0x80000000;

	ASTGetIndexRegMask(pAST->fd, CRTC_PORT, 0xA4, 0x01, &reg);
	if (!reg) {
	    printf("2d disabled\n");
	    return;
	}

	ASTGetIndexRegMask(pAST->fd, CRTC_PORT, 0xA3, 0x0F, &reg);
	if (!reg) {
	    printf("2d not work if in std mode\n");
	    return;
	}

	do
	{
	    ulEngState   = pAST->read32((pAST->CMDQInfo.pjEngStatePort));
	    ulEngState2   = pAST->read32((pAST->CMDQInfo.pjEngStatePort));
	    ulEngState2   = pAST->read32((pAST->CMDQInfo.pjEngStatePort));
	    ulEngState2   = pAST->read32((pAST->CMDQInfo.pjEngStatePort));
	    ulEngState2   = pAST->read32((pAST->CMDQInfo.pjEngStatePort));
	    ulEngState2   = pAST->read32((pAST->CMDQInfo.pjEngStatePort));
	    ulEngState   &= 0xFFFC0000;
	    ulEngState2  &= 0xFFFC0000;
	} while ((ulEngState & ulEngCheckSetting) || (ulEngState != ulEngState2));
}

void
ASTSaveState(struct ast_info *pAST)
{
	pAST->save_dst_base   = pAST->read32(MMIOREG_DST_BASE);
	pAST->save_line_xy    = pAST->read32(MMIOREG_LINE_XY);
	pAST->save_line_err   = pAST->read32(MMIOREG_LINE_Err);
	pAST->save_line_width = pAST->read32(MMIOREG_LINE_WIDTH);
	pAST->save_line_k1    = pAST->read32(MMIOREG_LINE_K1);
	pAST->save_line_k2    = pAST->read32(MMIOREG_LINE_K2);
	pAST->save_mono_pat1  = pAST->read32(MMIOREG_MONO1);
	pAST->save_mono_pat2  = pAST->read32(MMIOREG_MONO2);
}

void
ASTResetState(struct ast_info *pAST)
{
	pAST->write32(MMIOREG_DST_BASE,   pAST->save_dst_base);
	pAST->write32(MMIOREG_LINE_XY,    pAST->save_line_xy);
	pAST->write32(MMIOREG_LINE_Err,   pAST->save_line_err);
	pAST->write32(MMIOREG_LINE_WIDTH, pAST->save_line_width);
	pAST->write32(MMIOREG_LINE_K1,    pAST->save_line_k1);
	pAST->write32(MMIOREG_LINE_K2,    pAST->save_line_k2);
	pAST->write32(MMIOREG_MONO1,      pAST->save_mono_pat1);
	pAST->write32(MMIOREG_MONO2,      pAST->save_mono_pat2);
}

int
ASTInitCMDQ(struct ast_info *pAST)
{
	int availableLen;

	availableLen = pAST->FBMapSize - 
			(pAST->screenWidth * pAST->bytesPerPixel * pAST->screenHeight);

	if (availableLen < 0x100000) {

#if DEBUG
	    printf("Not enough memory to initialize CMDQ, fallback to MMIO\n");
#endif
	    return -1;
	}

	pAST->CMDQInfo.pjCmdQBasePort	= pAST->MMIOvaddr + 0x8044;
	pAST->CMDQInfo.pjWritePort	= pAST->MMIOvaddr + 0x8048;
	pAST->CMDQInfo.pjReadPort	= pAST->MMIOvaddr + 0x804C;
	pAST->CMDQInfo.pjEngStatePort	= pAST->MMIOvaddr + 0x804C;

	pAST->CMDQInfo.ulCMDQSize 	= 0x40000;			/* byte */
	pAST->CMDQInfo.ulCMDQOffset 	= 
			(pAST->screenWidth * pAST->bytesPerPixel * pAST->screenHeight);
	pAST->CMDQInfo.pjCMDQvaddr 	= pAST->FBvaddr + pAST->CMDQInfo.ulCMDQOffset;
	pAST->CMDQInfo.ulCMDQueueLen 	= pAST->CMDQInfo.ulCMDQSize - CMD_QUEUE_GUARD_BAND;
	pAST->CMDQInfo.ulCMDQMask 	= pAST->CMDQInfo.ulCMDQSize - 1;


	pAST->CMDQInfo.ulWritePointer = 0;

	return 0;
}

ASTEnableCMDQ(struct ast_info *pAST) 
{
	unsigned int ulVMCmdQBasePort = 0;
	
	ASTWaitEngIdle(pAST);

	ulVMCmdQBasePort = pAST->CMDQInfo.ulCMDQOffset >> 3;

	/*
	 * set CMDQ Threshold 
	 */
	ulVMCmdQBasePort |= 0xF0000000;

	pAST->write32((pAST->CMDQInfo.pjCmdQBasePort), ulVMCmdQBasePort);
	pAST->CMDQInfo.ulWritePointer = pAST->read32((pAST->CMDQInfo.pjWritePort));
	pAST->CMDQInfo.ulWritePointer <<= 3;	/* byte offset */
}


ASTEnable2D(struct ast_info *pAST)
{
	unsigned int ulData;

	pAST->write32((pAST->MMIOvaddr + 0xF004), 0x1e6e0000);
	pAST->write32((pAST->MMIOvaddr + 0xF000), 0x1);

	ulData = pAST->read32((pAST->MMIOvaddr + 0x1200c));
	pAST->write32((pAST->MMIOvaddr + 0x1200c), (ulData & 0xFFFFFFFD));

	ASTSetIndexRegMask(pAST->fd, CRTC_PORT, 0xA4, 0xFE, 0x01);
}

unsigned int
ASTGetCMDQLength(struct ast_info *pAST, unsigned int ulWritePointer, unsigned int ulCMDQMask)
{
	unsigned long ulReadPointer, ulReadPointer2;

	do {
	    ulReadPointer  = pAST->read32((pAST->CMDQInfo.pjReadPort));
	    ulReadPointer2 = pAST->read32((pAST->CMDQInfo.pjReadPort));
	    ulReadPointer2 = pAST->read32((pAST->CMDQInfo.pjReadPort));
	    ulReadPointer2 = pAST->read32((pAST->CMDQInfo.pjReadPort));
	    ulReadPointer2 = pAST->read32((pAST->CMDQInfo.pjReadPort));
	    ulReadPointer2 = pAST->read32((pAST->CMDQInfo.pjReadPort));
	    ulReadPointer  &= 0x3FFFF;
	    ulReadPointer2 &= 0x3FFFF;
	} while (ulReadPointer != ulReadPointer2);

	return ((ulReadPointer << 3) - ulWritePointer - CMD_QUEUE_GUARD_BAND) & ulCMDQMask;
}
 
PKT_SC *
ASTRequestCMDQ(struct ast_info *pAST, int ncmd)
{
	unsigned char   *pjBuffer;
	unsigned long   i, ulWritePointer, ulCMDQMask, ulCurCMDQLen, ulContinueCMDQLen;
	unsigned long	ulDataLen;
	
	ulDataLen = ncmd * 2 * 4;
	ulWritePointer = pAST->CMDQInfo.ulWritePointer;
	ulContinueCMDQLen = pAST->CMDQInfo.ulCMDQSize - ulWritePointer;
	ulCMDQMask = pAST->CMDQInfo.ulCMDQMask;

	if (ulContinueCMDQLen >= ulDataLen)
	{
	    /* Get CMDQ Buffer */
	    if (pAST->CMDQInfo.ulCMDQueueLen >= ulDataLen)
	    {
	        ;
	    }
	    else
	    {
	        do
	        {
	            ulCurCMDQLen = ASTGetCMDQLength(pAST, ulWritePointer, ulCMDQMask);
	        } while (ulCurCMDQLen < ulDataLen);

	        pAST->CMDQInfo.ulCMDQueueLen = ulCurCMDQLen;

	    }

	    pjBuffer = pAST->CMDQInfo.pjCMDQvaddr + ulWritePointer;
	    pAST->CMDQInfo.ulCMDQueueLen -= ulDataLen;
	    pAST->CMDQInfo.ulWritePointer = (ulWritePointer + ulDataLen) & ulCMDQMask;
	    return (PKT_SC *)pjBuffer;
	}
	else
	{        /* Fill NULL CMD to the last of the CMDQ */
	    if (pAST->CMDQInfo.ulCMDQueueLen >= ulContinueCMDQLen)
	    {
	        ;
	    }
	    else
	    {
    
	        do
	        {
	            ulCurCMDQLen = ASTGetCMDQLength(pAST, ulWritePointer, ulCMDQMask);
	        } while (ulCurCMDQLen < ulContinueCMDQLen);
	
	        pAST->CMDQInfo.ulCMDQueueLen = ulCurCMDQLen;
	
	    }
	
	    pjBuffer = pAST->CMDQInfo.pjCMDQvaddr + ulWritePointer;
	    for (i = 0; i<ulContinueCMDQLen/8; i++, pjBuffer+=8)
	    {
	        pAST->write32(pjBuffer , (unsigned long) PKT_NULL_CMD);
	        pAST->write32((pjBuffer+4) , 0);
    	
	    }
	    pAST->CMDQInfo.ulCMDQueueLen -= ulContinueCMDQLen;
	    pAST->CMDQInfo.ulWritePointer = ulWritePointer = 0;
	
	    /* Get CMDQ Buffer */
	    if (pAST->CMDQInfo.ulCMDQueueLen >= ulDataLen)
	    {
	        ;
	    }
	    else
	    {
	
	        do
	        {
	            ulCurCMDQLen = ASTGetCMDQLength(pAST, ulWritePointer, ulCMDQMask);
	        } while (ulCurCMDQLen < ulDataLen);
	
	        pAST->CMDQInfo.ulCMDQueueLen = ulCurCMDQLen;
	
	    }

	    pAST->CMDQInfo.ulCMDQueueLen -= ulDataLen;
	    pjBuffer = pAST->CMDQInfo.pjCMDQvaddr + ulWritePointer;
	    pAST->CMDQInfo.ulWritePointer = (ulWritePointer + ulDataLen) & ulCMDQMask;
	    return (PKT_SC *)pjBuffer;
	}
}

void
ASTUpdateWritePointer(struct ast_info *pAST)
{
	pAST->write32((pAST->CMDQInfo.pjWritePort), (pAST->CMDQInfo.ulWritePointer >> 3));
}


void
ASTSetupCmdArg1(struct ast_info *pAST, PKT_SC *pCMD, unsigned int header, unsigned int arg)
{
	pAST->write32((unsigned char *)&pCMD->header, PKT_SINGLE_CMD_HEADER + header);
	pAST->write32((unsigned char *)pCMD->data, arg);
	return;
}

void
ASTSetupCmdArg2(struct ast_info *pAST, PKT_SC *pCMD, unsigned int header, 
			unsigned int arg1, unsigned int arg2, 
			unsigned int shift1, unsigned int shift2)
{
	unsigned int ul;

	pAST->write32((unsigned char *)&pCMD->header, PKT_SINGLE_CMD_HEADER + header);
	ul = (arg1 << shift1) | (arg2 << shift2);
	pAST->write32((unsigned char *)pCMD->data, ul);
	return;
}


void
ASTSetupMMIOArg1(struct ast_info *pAST, unsigned char *addr, unsigned int arg)
{
	unsigned int argr;

	do {
	    pAST->write32(addr, arg);
	    argr = pAST->read32(addr);
	} while (arg != argr);

	return;
}

void
ASTSetupMMIOArg2(struct ast_info *pAST, unsigned char *addr,
			unsigned int arg1, unsigned int arg2, 
			unsigned int shift1, unsigned int shift2)
{
	unsigned int ul;
	unsigned int ulr;

	ul = (arg1 << shift1) | (arg2 << shift2);

	do {
	    pAST->write32(addr, ul);
	    ulr = pAST->read32(addr);
	} while (ul != ulr);

	return;
}


void *
ASTSetupForMonoPatternFill(struct ast_info *pAST, int patx, int paty, int fg, int bg, int rop)
{
	unsigned int cmdreg;
	unsigned int ul;
	PKT_SC *pCMD;
    	
	cmdreg = CMD_BITBLT | CMD_PAT_MONOMASK;
	switch (pAST->bytesPerPixel) {
	    case 1:
	        cmdreg |= CMD_COLOR_08;
		break;
	    case 2:
	        cmdreg |= CMD_COLOR_16;
		break;
	    case 3:
	    case 4:
	    default:
	        cmdreg |= CMD_COLOR_32;
		break;
	}

	cmdreg |= rop << 8;
	pAST->cmdreg = cmdreg;

	if (!pAST->MMIO2D) {
	    pCMD = ASTRequestCMDQ(pAST, 5);

	    ASTSetupCmdArg2(pAST, pCMD, CMDQREG_DST_PITCH, pAST->screenPitch, MASK_DST_HEIGHT, 16, 0);
	    pCMD++;

	    ASTSetupCmdArg1(pAST, pCMD, CMDQREG_FG, fg);
	    pCMD++;

	    ASTSetupCmdArg1(pAST, pCMD, CMDQREG_BG, bg);
	    pCMD++;

	    ASTSetupCmdArg1(pAST, pCMD, CMDQREG_MONO1, patx);
	    pCMD++;

	    ASTSetupCmdArg1(pAST, pCMD, CMDQREG_MONO2, paty);
	
	} else {
	    ASTSetupMMIOArg2(pAST, MMIOREG_DST_PITCH, pAST->screenPitch, MASK_DST_HEIGHT, 16, 0);
	    ASTSetupMMIOArg1(pAST, MMIOREG_FG, fg);
	    ASTSetupMMIOArg1(pAST, MMIOREG_BG, bg);
	    ASTSetupMMIOArg1(pAST, MMIOREG_MONO1, patx);
	    ASTSetupMMIOArg1(pAST, MMIOREG_MONO2, paty);
	}
}

void
ASTMonoPatternFill(struct ast_info *pAST, int patx, int paty, int x, int y,
			int width, int height)
{
	unsigned int cmdreg;
	PKT_SC *pCMD;

	cmdreg = pAST->cmdreg;

	if (!pAST->MMIO2D) {

	    pCMD = ASTRequestCMDQ(pAST, 4);
	
	    ASTSetupCmdArg1(pAST, pCMD, CMDQREG_DST_BASE, 0);
	    pCMD++;

	    ASTSetupCmdArg2(pAST, pCMD, CMDQREG_DST_XY, x, y, 16, 0);
	    pCMD++;
	
	    ASTSetupCmdArg2(pAST, pCMD, CMDQREG_RECT_XY, width, height, 16, 0);
	    pCMD++;
	
	    ASTSetupCmdArg1(pAST, pCMD, CMDQREG_CMD, cmdreg);

	    ASTUpdateWritePointer(pAST);

	} else {

	    ASTSetupMMIOArg1(pAST, MMIOREG_DST_BASE, 0);
	    ASTSetupMMIOArg2(pAST, MMIOREG_DST_XY, x, y, 16, 0);
	    ASTSetupMMIOArg2(pAST, MMIOREG_RECT_XY, width, height, 16, 0);
	    ASTSetupMMIOArg1(pAST, MMIOREG_CMD, cmdreg);
	}
}

void
ASTSetupForSolidFill(struct ast_info *pAST, int color, int rop)
{
	unsigned int cmdreg;
	unsigned int ul;
	PKT_SC *pCMD;
    	
	cmdreg = CMD_BITBLT | CMD_PAT_FGCOLOR;
	switch (pAST->bytesPerPixel) {
	    case 1:
	        cmdreg |= CMD_COLOR_08;
		break;
	    case 2:
	        cmdreg |= CMD_COLOR_16;
		break;
	    case 3:
	    case 4:
	    default:
	        cmdreg |= CMD_COLOR_32;
		break;
	}

	cmdreg |= rop << 8;
	pAST->cmdreg = cmdreg;

	if (!pAST->MMIO2D) {

	    pCMD = ASTRequestCMDQ(pAST, 2);

	    ASTSetupCmdArg2(pAST, pCMD, CMDQREG_DST_PITCH, pAST->screenPitch, MASK_DST_HEIGHT, 16, 0);
	    pCMD++;

	    ASTSetupCmdArg1(pAST, pCMD, CMDQREG_FG, color);
	    pCMD++;

	} else {

	    ASTSetupMMIOArg2(pAST, MMIOREG_DST_PITCH, pAST->screenPitch, MASK_DST_HEIGHT, 16, 0);
	    ASTSetupMMIOArg1(pAST, MMIOREG_FG, color);
	}
}

void
ASTSolidFillRect(struct ast_info *pAST, int x, int y, int width, int height)
{
	unsigned int cmdreg;
	unsigned int ul;
	PKT_SC *pCMD;

	cmdreg = pAST->cmdreg;


	if (!pAST->MMIO2D) {

	    pCMD = ASTRequestCMDQ(pAST, 4);

	    ASTSetupCmdArg1(pAST, pCMD, CMDQREG_DST_BASE, 0);
	    pCMD++;

	    ASTSetupCmdArg2(pAST, pCMD, CMDQREG_DST_XY, x, y, 16, 0);
	    pCMD++;

	    ASTSetupCmdArg2(pAST, pCMD, CMDQREG_RECT_XY, width, height, 16, 0);
	    pCMD++;

	    ASTSetupCmdArg1(pAST, pCMD, CMDQREG_CMD, cmdreg);
	    pCMD++;

	    ASTUpdateWritePointer(pAST);
	
	} else {

	    ASTSetupMMIOArg1(pAST, MMIOREG_DST_BASE, 0);
	    ASTSetupMMIOArg2(pAST, MMIOREG_DST_XY, x, y, 16, 0);
	    ASTSetupMMIOArg2(pAST, MMIOREG_RECT_XY, width, height, 16, 0);
	    ASTSetupMMIOArg1(pAST, MMIOREG_CMD, cmdreg);
	}
}

void
ASTSetupForSolidLine(struct ast_info *pAST, int color, int rop)
{
	unsigned int cmdreg;
	unsigned int ul;
	PKT_SC *pCMD;
    	
	cmdreg = CMD_BITBLT;
	switch (pAST->bytesPerPixel) {
	    case 1:
	        cmdreg |= CMD_COLOR_08;
		break;
	    case 2:
	        cmdreg |= CMD_COLOR_16;
		break;
	    case 3:
	    case 4:
	    default:
	        cmdreg |= CMD_COLOR_32;
		break;
	}

	cmdreg |= rop << 8;
	pAST->cmdreg = cmdreg;

	if (!pAST->MMIO2D) {

	    pCMD = ASTRequestCMDQ(pAST, 3);

	    ASTSetupCmdArg2(pAST, pCMD, CMDQREG_DST_PITCH, pAST->screenPitch, MASK_DST_HEIGHT, 16, 0);
	    pCMD++;

	    ASTSetupCmdArg1(pAST, pCMD, CMDQREG_FG, color);
	    pCMD++;

	    ASTSetupCmdArg1(pAST, pCMD, CMDQREG_BG, 0);
	    pCMD++;

	} else {

	    ASTSetupMMIOArg2(pAST, MMIOREG_DST_PITCH, pAST->screenPitch, MASK_DST_HEIGHT, 16, 0);
	    ASTSetupMMIOArg1(pAST, MMIOREG_FG, color);
	    ASTSetupMMIOArg1(pAST, MMIOREG_BG, 0);
	}
}

void
ASTSolidLine(struct ast_info *pAST, int x1, int y1, int x2, int y2)
{
	unsigned int cmdreg;
	unsigned int ul;
	PKT_SC   *pCMD;
	int	 GAbsX, GAbsY;
	int      MM, mm, err, k1, k2, xm;
	int	 width, height;

	cmdreg = (pAST->cmdreg & (~CMD_MASK)) | CMD_ENABLE_CLIP | CMD_NOT_DRAW_LAST_PIXEL;

	GAbsX = abs(x1 - x2);
	GAbsY = abs(y1 - y2);

	cmdreg |= CMD_LINEDRAW; 

	if (GAbsX >= GAbsY) {
	    MM = GAbsX;
	    mm = GAbsY;
	    xm = 1;
	} else {
	    MM = GAbsY;
	    mm = GAbsX;
	    xm = 0;
	} 

	if (x1 >= x2) {
	    cmdreg |= CMD_X_DEC;
	}

	if (y1 >= y2) {
	    cmdreg |= CMD_Y_DEC;
	}

	err = (signed) (2 * mm - MM);
	k1 = 2 * mm;
	k2 = (signed) 2 * mm - 2 * MM;
	

	if (!pAST->MMIO2D) {

	    pCMD = ASTRequestCMDQ(pAST, 7);

	    ASTSetupCmdArg1(pAST, pCMD, CMDQREG_DST_BASE, 0);
	    pCMD++;

	    ASTSetupCmdArg2(pAST, pCMD, CMDQREG_LINE_XY, x1, y1, 16, 0);
	    pCMD++;

	    ASTSetupCmdArg2(pAST, pCMD, CMDQREG_LINE_Err, xm, (err & MASK_LINE_ERR), 24, 0);
	    pCMD++;

	    ASTSetupCmdArg2(pAST, pCMD, CMDQREG_LINE_WIDTH, (MM & MASK_LINE_WIDTH), 0, 16, 0);
	    pCMD++;

	    ASTSetupCmdArg1(pAST, pCMD, CMDQREG_LINE_K1, (k1 & MASK_LINE_K1));
	    pCMD++;

	    ASTSetupCmdArg1(pAST, pCMD, CMDQREG_LINE_K2, (k2 & MASK_LINE_K2));
	    pCMD++;

	    ASTSetupCmdArg1(pAST, pCMD, CMDQREG_CMD, cmdreg);
	    pCMD++;

	    ASTUpdateWritePointer(pAST);

	    ASTWaitEngIdle(pAST);

	} else {

	    ASTSetupMMIOArg1(pAST, MMIOREG_DST_BASE, 0);
	    ASTSetupMMIOArg2(pAST, MMIOREG_LINE_XY, x1, y1, 16, 0);
	    ASTSetupMMIOArg2(pAST, MMIOREG_LINE_Err, xm, (err & MASK_LINE_ERR), 24, 0);
	    ASTSetupMMIOArg2(pAST, MMIOREG_LINE_WIDTH, (MM & MASK_LINE_WIDTH), 0, 16, 0);
	    ASTSetupMMIOArg1(pAST, MMIOREG_LINE_K1, (k1 & MASK_LINE_K1));
	    ASTSetupMMIOArg1(pAST, MMIOREG_LINE_K2, (k1 & MASK_LINE_K2));
	    ASTSetupMMIOArg1(pAST, MMIOREG_CMD, cmdreg);
	}
}

void
ASTSetClippingRectangle(struct ast_info *pAST, int left, int top, int right, int bottom)
{
	PKT_SC *pCMD;
    	
	if (!pAST->MMIO2D) {

	    pCMD = ASTRequestCMDQ(pAST, 2);

	    ASTSetupCmdArg2(pAST, pCMD, CMDQREG_CLIP1, (left & MASK_CLIP), (top & MASK_CLIP), 16, 0);
	    pCMD++;

	    ASTSetupCmdArg2(pAST, pCMD, CMDQREG_CLIP2, (right & MASK_CLIP), (bottom & MASK_CLIP), 16, 0);
	    ASTUpdateWritePointer(pAST);
	
	} else {

	    ASTSetupMMIOArg2(pAST, MMIOREG_CLIP1, (left & MASK_CLIP), (top & MASK_CLIP), 16, 0);
	    ASTSetupMMIOArg2(pAST, MMIOREG_CLIP2, (right & MASK_CLIP), (bottom & MASK_CLIP), 16, 0);
	}
}

int
ast_get_pci_info(int fd, struct pci_info *pciInfo)
{
	struct gfx_pci_cfg pciCfg;
	int i;
	unsigned int bar;

	if (ioctl(fd, GFX_IOCTL_GET_PCI_CONFIG, &pciCfg) == -1) {
		return -1;
	}

	for (i = 0; i < 6; i++) {
	    bar = pciCfg.bar[i];
	    if (bar != 0) {
		if (bar & PCI_MAP_IO) {
		    pciInfo->ioBase[i] = PCIGETIO(bar);
		    pciInfo->type[i] = bar & PCI_MAP_IO_ATTR_MASK;
		} else {
		    pciInfo->type[i] = bar & PCI_MAP_MEMORY_ATTR_MASK;
		    pciInfo->memBase[i] = PCIGETMEMORY(bar);
		    if (PCI_MAP_IS64BITMEM(bar)) {
			if (i == 5) {
			    pciInfo->memBase[i] = 0;
			} else {
			    int bar_hi = pciCfg.bar[i+1];
			    pciInfo->memBase[i] |= (bar_hi << 32);
			    ++i;
			}
		    }
		    pciInfo->size[i] = AST_REG_SIZE_LOG2;
		}
	    }
	}

	return 0;
}

int
ast_get_mem_info(struct pci_info *pci_info, struct ast_info *pAST) 
{
	unsigned char reg;

	pAST->FBPhysAddr = pci_info->memBase[0] & 0xfff00000;
	pAST->FBMapSize = 0;

	pAST->MMIOPhysAddr = pci_info->memBase[1] & 0xffff0000;
	pAST->MMIOMapSize =  AST_MMIO_SIZE;

	pAST->RelocateIO = pci_info->ioBase[2];

	ASTOpenKey(pAST->fd);
	ASTGetIndexRegMask(pAST->fd, CRTC_PORT, 0xAA, 0xFF, &reg);
	switch (reg & 0x03)
	{
	    case 0x00:
		pAST->FBMapSize = AST_VRAM_SIZE_08M;
		break;
	    case 0x01:
		pAST->FBMapSize = AST_VRAM_SIZE_16M;
		break;
	    case 0x02:
		pAST->FBMapSize = AST_VRAM_SIZE_32M;
		break;
	    case 0x03:
		pAST->FBMapSize = AST_VRAM_SIZE_64M;
		break;
	    default:
		pAST->FBMapSize = AST_VRAM_SIZE_08M;
		break;
	}

#if DEBUG
	printf("FBPhysAddr=0x%x FBMapSize=0x%x\n", pAST->FBPhysAddr, pAST->FBMapSize);
	printf("MMIOPhysAddr=0x%x MMIOMapSize=0x%x\n", pAST->MMIOPhysAddr, pAST->MMIOMapSize);
	printf("RelocateIO=0x%x\n", pAST->RelocateIO);
#endif

	return 0;
}


int
ast_init_info(struct ast_info *pAST)
{
	unsigned char val;
	unsigned int  status = 0;

	/* 
	 * first check if the hardware is already initialized.
	 * If not, abort
	 */
	ioctl(pAST->fd, AST_GET_STATUS_FLAGS, &status);
	if (!(status & AST_STATUS_HW_INITIALIZED))
		return -1;

	pAST->MMIO2D = 0;

	ASTOpenKey(pAST->fd);
	
	/*
	 * get the VDE
	 */
	ASTGetIndexRegMask(pAST->fd, CRTC_PORT, 0x01, 0xff, &val);
	pAST->screenWidth = ((int)(val + 1)) << 3;

	switch (pAST->screenWidth) {
	case 1920:
	case 1600:
		pAST->screenHeight = 1200;
		break;
	case 1280:
		pAST->screenHeight = 1024;
		break;
	case 1024:
		pAST->screenHeight = 768;
		break;
	case 800:
		pAST->screenHeight = 600;
		break;
	case 640:
	default:
		pAST->screenHeight = 480;
		break;
	}

	/*
	 * get the display depth info 
	 */
	ASTGetIndexRegMask(pAST->fd, CRTC_PORT, 0xA2, 0xff, &val);
	if (val & 0x80) {
		/* 32 bits */
		pAST->bytesPerPixel = 4;
		pAST->read32	    = (PFNRead32)  ASTMMIORead32;
		pAST->write32	    = (PFNWrite32) ASTMMIOWrite32;
	
	} else {
		pAST->bytesPerPixel = 1;
		pAST->read32	    = (PFNRead32)  ASTMMIORead32_8pp;
		pAST->write32	    = (PFNWrite32) ASTMMIOWrite32_8pp;
	}

	pAST->screenPitch = pAST->screenWidth * pAST->bytesPerPixel;

	/*
	 * Enable MMIO
	 */
	ASTSetIndexRegMask(pAST->fd, CRTC_PORT, 0xA1, 0xff, 0x04);

	return 0;
}

int
ast_map_mem(struct ast_info *pAST, return_packet *rp, int test)
{
        struct pci_info  pci_info;
        int              pageSize, size;
	int		 fd = pAST->fd;

        if (ast_get_pci_info(fd, &pci_info) == -1) {
            gfx_vts_set_message(rp, 1, test, "get pci info failed");
            return -1;
        }


        if (ast_get_mem_info(&pci_info, pAST) == -1) {
            gfx_vts_set_message(rp, 1, test, "get mem info failed");
            return -1;
        }

        /*
         * Map framebuffer
         */
        pageSize = getpagesize();
        size = pAST->FBMapSize + (pageSize - 1) & (~(pageSize - 1));

        pAST->FBvaddr = (unsigned char *)mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED,
                                        fd, pAST->FBPhysAddr);

        if (pAST->FBvaddr == MAP_FAILED) {
            gfx_vts_set_message(rp, 1, test, "map framebuffer failed");
            return -1;
        }


        /*
         * Map MMIO
         */
        size = pAST->MMIOMapSize + (pageSize - 1) & (~(pageSize - 1));

        pAST->MMIOvaddr = (unsigned char *)mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED,
                                        fd, pAST->MMIOPhysAddr);

        if (pAST->MMIOvaddr == MAP_FAILED) {
            gfx_vts_set_message(rp, 1, test, "map MMIO failed");
            return -1;
        }

	return 0;
}

int
ast_unmap_mem(struct ast_info *pAST, return_packet *rp, int test)
{
        /*
         * Unmap Frame Buffer
         */

        if (munmap((void *)pAST->FBvaddr, pAST->FBMapSize) == -1) {
            gfx_vts_set_message(rp, 1, test, "unmap framebuffer failed");
            return -1;
        }

        if (munmap((void *)pAST->MMIOvaddr, pAST->MMIOMapSize) == -1) {
            gfx_vts_set_message(rp, 1, test, "unmap MMIO failed");
            return -1;
        }

	return 0;
}
