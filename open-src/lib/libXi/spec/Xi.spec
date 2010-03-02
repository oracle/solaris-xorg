#
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice (including the next
# paragraph) shall be included in all copies or substantial portions of the
# Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#
#

# Used by DevicePresence macro in public <X11/XInput.h> header
Data		_XiGetDevicePresenceNotifyEvent
Version		SUNW_1.2
Arch		all
End

Function	XAllowDeviceEvents
Version		SUNW_1.1
Arch		all
End

Function	XChangeDeviceControl
Version		SUNW_1.1
Arch		all
End

Function	XChangeDeviceDontPropagateList
Version		SUNW_1.1
Arch		all
End

Function	XChangeDeviceKeyMapping
Version		SUNW_1.1
Arch		all
End

Function	XChangeDeviceProperty
Version		SUNW_1.3
Arch		all
End

Function	XChangeFeedbackControl
Version		SUNW_1.1
Arch		all
End

Function	XChangeKeyboardDevice
Version		SUNW_1.1
Arch		all
End

Function	XChangePointerDevice
Version		SUNW_1.1
Arch		all
End

Function	XCloseDevice
Version		SUNW_1.1
Arch		all
End

Function	XDeleteDeviceProperty
Version		SUNW_1.3
Arch		all
End

Function	XDeviceBell
Version		SUNW_1.1
Arch		all
End

Function	XFreeDeviceControl
Version		SUNW_1.1
Arch		all
End

Function	XFreeDeviceList
Version		SUNW_1.1
Arch		all
End

Function	XFreeDeviceMotionEvents
Version		SUNW_1.1
Arch		all
End

Function	XFreeDeviceState
Version		SUNW_1.1
Arch		all
End

Function	XFreeFeedbackList
Version		SUNW_1.1
Arch		all
End

Function	XGetDeviceButtonMapping
Version		SUNW_1.1
Arch		all
End

Function	XGetDeviceControl
Version		SUNW_1.1
Arch		all
End

Function	XGetDeviceDontPropagateList
Version		SUNW_1.1
Arch		all
End

Function	XGetDeviceFocus
Version		SUNW_1.1
Arch		all
End

Function	XGetDeviceKeyMapping
Version		SUNW_1.1
Arch		all
End

Function	XGetDeviceModifierMapping
Version		SUNW_1.1
Arch		all
End

Function	XGetDeviceMotionEvents
Version		SUNW_1.1
Arch		all
End

Function	XGetDeviceProperty
Version		SUNW_1.3
Arch		all
End

Function	XGetExtensionVersion
Version		SUNW_1.1
Arch		all
End

Function	XGetFeedbackControl
Version		SUNW_1.1
Arch		all
End

Function	XGetSelectedExtensionEvents
Version		SUNW_1.1
Arch		all
End

Function	XGrabDevice
Version		SUNW_1.1
Arch		all
End

Function	XGrabDeviceButton
Version		SUNW_1.1
Arch		all
End

Function	XGrabDeviceKey
Version		SUNW_1.1
Arch		all
End

Function	XListDeviceProperties
Version		SUNW_1.3
Arch		all
End

Function	XListInputDevices
Version		SUNW_1.1
Arch		all
End

Function	XOpenDevice
Version		SUNW_1.1
Arch		all
End

Function	XQueryDeviceState
Version		SUNW_1.1
Arch		all
End

Function	XSelectExtensionEvent
Version		SUNW_1.1
Arch		all
End

Function	XSendExtensionEvent
Version		SUNW_1.1
Arch		all
End

Function	XSetDeviceButtonMapping
Version		SUNW_1.1
Arch		all
End

Function	XSetDeviceFocus
Version		SUNW_1.1
Arch		all
End

Function	XSetDeviceMode
Version		SUNW_1.1
Arch		all
End

Function	XSetDeviceModifierMapping
Version		SUNW_1.1
Arch		all
End

Function	XSetDeviceValuators
Version		SUNW_1.1
Arch		all
End

Function	XUngrabDevice
Version		SUNW_1.1
Arch		all
End

Function	XUngrabDeviceButton
Version		SUNW_1.1
Arch		all
End

Function	XUngrabDeviceKey
Version		SUNW_1.1
Arch		all
End

Function	_xibadclass
Version		SUNWprivate
Arch		all
End

Function	_xibaddevice
Version		SUNWprivate
Arch		all
End

Function	_xibadevent
Version		SUNWprivate
Arch		all
End

Function	_xibadmode
Version		SUNWprivate
Arch		all
End

Function	_xidevicebusy
Version		SUNWprivate
Arch		all
End

# Added in libXi 1.3.0 / <X11/extensions/XInput2.h>:

Function	XIQueryPointer
Version		SUNW_1.4
Arch		all
End

Function	XIWarpPointer
Version		SUNW_1.4
Arch		all
End

Function	XIDefineCursor
Version		SUNW_1.4
Arch		all
End

Function	XIUndefineCursor
Version		SUNW_1.4
Arch		all
End

Function	XIChangeHierarchy
Version		SUNW_1.4
Arch		all
End

Function	XISetClientPointer
Version		SUNW_1.4
Arch		all
End

Function	XIGetClientPointer
Version		SUNW_1.4
Arch		all
End

Function	XISelectEvents
Version		SUNW_1.4
Arch		all
End

Function	XIGetSelectedEvents
Version		SUNW_1.4
Arch		all
End

Function	XIQueryVersion
Version		SUNW_1.4
Arch		all
End

Function	XIQueryDevice
Version		SUNW_1.4
Arch		all
End

Function	XISetFocus
Version		SUNW_1.4
Arch		all
End

Function	XIGetFocus
Version		SUNW_1.4
Arch		all
End

Function	XIGrabDevice
Version		SUNW_1.4
Arch		all
End

Function	XIUngrabDevice
Version		SUNW_1.4
Arch		all
End

Function	XIAllowEvents
Version		SUNW_1.4
Arch		all
End

Function	XIGrabButton
Version		SUNW_1.4
Arch		all
End

Function	XIGrabKeycode
Version		SUNW_1.4
Arch		all
End

Function	XIGrabEnter
Version		SUNW_1.4
Arch		all
End

Function	XIGrabFocusIn
Version		SUNW_1.4
Arch		all
End

Function	XIUngrabButton
Version		SUNW_1.4
Arch		all
End

Function	XIUngrabKeycode
Version		SUNW_1.4
Arch		all
End

Function	XIUngrabEnter
Version		SUNW_1.4
Arch		all
End

Function	XIUngrabFocusIn
Version		SUNW_1.4
Arch		all
End

Function	XIListProperties
Version		SUNW_1.4
Arch		all
End

Function	XIChangeProperty
Version		SUNW_1.4
Arch		all
End

Function	XIUngrabFocusIn
Version		SUNW_1.4
Arch		all
End

Function	XIListProperties
Version		SUNW_1.4
Arch		all
End

Function	XIChangeProperty
Version		SUNW_1.4
Arch		all
End

Function	XIDeleteProperty
Version		SUNW_1.4
Arch		all
End

Function	XIGetProperty
Version		SUNW_1.4
Arch		all
End

Function	XIFreeDeviceInfo
Version		SUNW_1.4
Arch		all
End
