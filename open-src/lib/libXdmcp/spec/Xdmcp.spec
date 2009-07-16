#
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# ident	"@(#)Xdmcp.spec	1.2	09/07/15 SMI"
#

Function	XdmcpARRAY8Equal
Include		<X11/Xdmcp.h>
Declaration	int XdmcpARRAY8Equal(ARRAY8Ptr array1, ARRAY8Ptr array2)
Version		SUNW_1.1
Arch		all
End

Function	XdmcpAllocARRAY16
Include		<X11/Xdmcp.h>
Declaration	int XdmcpAllocARRAY16 (ARRAY16Ptr array, int length)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpAllocARRAY32
Include		<X11/Xdmcp.h>
Declaration	int XdmcpAllocARRAY32 (ARRAY32Ptr array, int length)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpAllocARRAY8
Include		<X11/Xdmcp.h>
Declaration	int XdmcpAllocARRAY8 (ARRAY8Ptr array, int length)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpAllocARRAYofARRAY8
Include		<X11/Xdmcp.h>
Declaration	int XdmcpAllocARRAYofARRAY8 (ARRAYofARRAY8Ptr array, int length)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpCompareKeys
Include		<X11/Xdmcp.h>
Declaration	int XdmcpCompareKeys (XdmAuthKeyPtr a, XdmAuthKeyPtr b)
Version		SUNW_1.1
Arch		all
End

Function	XdmcpCopyARRAY8
Include		<X11/Xdmcp.h>
Declaration	int XdmcpCopyARRAY8(ARRAY8Ptr src, ARRAY8Ptr dst)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpDecrementKey
Include		<X11/Xdmcp.h>
Declaration	void XdmcpDecrementKey (XdmAuthKeyPtr key)
Version		SUNW_1.1
Arch		all
End

Function	XdmcpDisposeARRAY16
Include		<X11/Xdmcp.h>
Declaration	void XdmcpDisposeARRAY16(ARRAY16Ptr array)
Version		SUNW_1.1
Arch		all
End

Function	XdmcpDisposeARRAY32
Include		<X11/Xdmcp.h>
Declaration	void XdmcpDisposeARRAY32(ARRAY32Ptr array)
Version		SUNW_1.1
Arch		all
End

Function	XdmcpDisposeARRAY8
Include		<X11/Xdmcp.h>
Declaration	void XdmcpDisposeARRAY8(ARRAY8Ptr array)
Version		SUNW_1.1
Arch		all
End

Function	XdmcpDisposeARRAYofARRAY8
Include		<X11/Xdmcp.h>
Declaration	void XdmcpDisposeARRAYofARRAY8(ARRAYofARRAY8Ptr array)
Version		SUNW_1.1
Arch		all
End

Function	XdmcpFill
Include		<X11/Xdmcp.h>
Declaration	int XdmcpFill(int fd, XdmcpBufferPtr buffer, XdmcpNetaddr from, int *fromlen)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpFlush
Include		<X11/Xdmcp.h>
Declaration	int XdmcpFlush(int fd, XdmcpBufferPtr buffer, XdmcpNetaddr to, int tolen)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpGenerateKey
Include		<X11/Xdmcp.h>
Declaration	void XdmcpGenerateKey (XdmAuthKeyPtr key)
Version		SUNW_1.1
Arch		all
End

Function	XdmcpIncrementKey
Include		<X11/Xdmcp.h>
Declaration	void XdmcpIncrementKey (XdmAuthKeyPtr key)
Version		SUNW_1.1
Arch		all
End

Function	XdmcpReadARRAY16
Include		<X11/Xdmcp.h>
Declaration	int XdmcpReadARRAY16(XdmcpBufferPtr buffer, ARRAY16Ptr array)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpReadARRAY32
Include		<X11/Xdmcp.h>
Declaration	int XdmcpReadARRAY32(XdmcpBufferPtr buffer, ARRAY32Ptr array)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpReadARRAY8
Include		<X11/Xdmcp.h>
Declaration	int XdmcpReadARRAY8(XdmcpBufferPtr buffer, ARRAY8Ptr array)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpReadARRAYofARRAY8
Include		<X11/Xdmcp.h>
Declaration	int XdmcpReadARRAYofARRAY8(XdmcpBufferPtr buffer, ARRAYofARRAY8Ptr array)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpReadCARD16
Include		<X11/Xdmcp.h>
Declaration	int XdmcpReadCARD16(XdmcpBufferPtr buffer, CARD16Ptr valuep)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpReadCARD32
Include		<X11/Xdmcp.h>
Declaration	int XdmcpReadCARD32(XdmcpBufferPtr buffer, CARD32Ptr valuep)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpReadCARD8
Include		<X11/Xdmcp.h>
Declaration	int XdmcpReadCARD8(XdmcpBufferPtr buffer, CARD8Ptr valuep)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpReadHeader
Include		<X11/Xdmcp.h>
Declaration	int XdmcpReadHeader(XdmcpBufferPtr buffer, XdmcpHeaderPtr header)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpReadRemaining
Include		<X11/Xdmcp.h>
Declaration	int XdmcpReadRemaining(XdmcpBufferPtr buffer);
Version		SUNW_1.1
Arch		all
End

Function	XdmcpReallocARRAY16
Include		<X11/Xdmcp.h>
Declaration	int XdmcpReallocARRAY16 (ARRAY16Ptr array, int length)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpReallocARRAY32
Include		<X11/Xdmcp.h>
Declaration	int XdmcpReallocARRAY32 (ARRAY32Ptr array, int length)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpReallocARRAY8
Include		<X11/Xdmcp.h>
Declaration	int XdmcpReallocARRAY8 (ARRAY8Ptr array, int length)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpReallocARRAYofARRAY8
Include		<X11/Xdmcp.h>
Declaration	int XdmcpReallocARRAYofARRAY8 (ARRAYofARRAY8Ptr array, int length)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpWriteARRAY16
Include		<X11/Xdmcp.h>
Declaration	int XdmcpWriteARRAY16(XdmcpBufferPtr buffer, ARRAY16Ptr array)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpWriteARRAY32
Include		<X11/Xdmcp.h>
Declaration	int XdmcpWriteARRAY32(XdmcpBufferPtr buffer, ARRAY32Ptr array)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpWriteARRAY8
Include		<X11/Xdmcp.h>
Declaration	int XdmcpWriteARRAY8(XdmcpBufferPtr buffer, ARRAY8Ptr array)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpWriteARRAYofARRAY8
Include		<X11/Xdmcp.h>
Declaration	int XdmcpWriteARRAYofARRAY8(XdmcpBufferPtr buffer, ARRAYofARRAY8Ptr array)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpWriteCARD16
Include		<X11/Xdmcp.h>
Declaration	int XdmcpWriteCARD16(XdmcpBufferPtr buffer, unsigned value)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpWriteCARD32
Include		<X11/Xdmcp.h>
Declaration	int XdmcpWriteCARD32(XdmcpBufferPtr buffer, unsigned value)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpWriteCARD8
Include		<X11/Xdmcp.h>
Declaration	int XdmcpWriteCARD8(XdmcpBufferPtr buffer, unsigned value)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function	XdmcpWriteHeader
Include		<X11/Xdmcp.h>
Declaration	int XdmcpWriteHeader(XdmcpBufferPtr  buffer, XdmcpHeaderPtr  header)
Exception	$return == FALSE
Version		SUNW_1.1
Arch		all
End

Function        XdmcpWrap
Include         <X11/Xdmcp.h>
Declaration     void XdmcpWrap (unsigned char* input, unsigned char* wrapper, unsigned char* output, int bytes)
Exception       $return == FALSE
Version         SUNW_1.2
Arch            all
End

Function        XdmcpUnwrap
Includde        <X11/Xdmcp.h>
Declaration     void XdmcpUnwrap (unsigned char* input, unsigned char* wrapper, unsigned char* output, int bytes)
Exception       $return == FALSE
Version         SUNW_1.2
Arch            all
End


